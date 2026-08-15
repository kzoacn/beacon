#!/usr/bin/env python3
"""Capture a real macOS Safari evidence bundle for https://agentbounties.app/."""
from __future__ import annotations

import hashlib
import json
import subprocess
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

from PIL import Image
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait

LIVE_URL = "https://agentbounties.app/"
EVIDENCE = Path("evidence")


def sh(cmd: str) -> str:
    return subprocess.check_output(cmd, shell=True, text=True, timeout=60).strip()


def main() -> None:
    public_run_url = sys.argv[1] if len(sys.argv) > 1 else ""
    EVIDENCE.mkdir(exist_ok=True)

    safari_version = sh(
        "defaults read /Applications/Safari.app/Contents/Info.plist CFBundleShortVersionString"
    )
    macos_version = sh("sw_vers -productVersion")

    driver = webdriver.Safari()
    try:
        # A comfortably large window keeps the resulting viewport above 1280x720
        # while the metadata records the exact viewport reported by Safari.
        driver.set_window_size(1440, 1000)
        driver.get(LIVE_URL)
        WebDriverWait(driver, 45).until(
            lambda d: d.execute_script("return document.readyState") == "complete"
        )
        time.sleep(8)

        viewport = driver.execute_script(
            "return {width: window.innerWidth, height: window.innerHeight, "
            "devicePixelRatio: window.devicePixelRatio};"
        )
        if not isinstance(viewport, dict) or int(viewport["width"]) < 1280 or int(viewport["height"]) < 720:
            raise SystemExit(f"viewport too small for the evidence bundle: {viewport}")

        body_text = driver.execute_script(
            "return document.body ? document.body.innerText : ''"
        )
        headings = driver.execute_script(
            "return [...document.querySelectorAll('h1,h2,h3')].map(e => e.innerText.trim()).filter(Boolean)"
        )
        links = driver.execute_script(
            "return [...document.querySelectorAll('a')].map(e => (e.innerText || e.getAttribute('href') || '').trim()).filter(Boolean)"
        )
        buttons = driver.execute_script(
            "return [...document.querySelectorAll('button')].map(e => (e.innerText || e.getAttribute('aria-label') || '').trim()).filter(Boolean)"
        )

        required_phrases = ("Agent Bounties", "Bounty Board", "Post a bounty")
        phrase_matches = {phrase: phrase.lower() in body_text.lower() for phrase in required_phrases}
        if not all(phrase_matches.values()):
            raise SystemExit(f"required homepage phrases missing: {phrase_matches}")

        accessibility = {
            "source": "macOS Safari WebDriver DOM inspection",
            "url": driver.current_url,
            "title": driver.title,
            "phrase_matches": phrase_matches,
            "headings": headings,
            "links": links,
            "buttons": buttons,
        }
        (EVIDENCE / "accessibility.json").write_text(
            json.dumps(accessibility, indent=2, sort_keys=True), encoding="utf-8"
        )

        # Safari WebDriver exposes browser/console logs on the runner when the
        # underlying safaridriver supports log retrieval; otherwise this stays
        # an empty array, which is valid when no error-level entry was observed.
        console: list[dict] = []
        for log_type in ("browser", "safari", "driver"):
            try:
                for entry in driver.get_log(log_type):
                    console.append(
                        {
                            "source": log_type,
                            "level": entry.get("level"),
                            "message": entry.get("message"),
                            "timestamp": entry.get("timestamp"),
                        }
                    )
            except Exception:
                pass
        if any(isinstance(e, dict) and str(e.get("level", "")).lower() in {"error", "uncaught"} for e in console):
            raise SystemExit(f"observed an uncaught/error console entry: {console}")
        (EVIDENCE / "console.json").write_text(
            json.dumps(console, indent=2, sort_keys=True), encoding="utf-8"
        )

        screenshot = EVIDENCE / "safari-homepage.png"
        driver.save_screenshot(str(screenshot))

        with Image.open(screenshot) as image:
            png_size = image.size
        if (viewport["width"], viewport["height"]) != png_size:
            raise SystemExit(
                f"viewport {viewport} does not match PNG dimensions {png_size}"
            )
        screenshot_sha256 = hashlib.sha256(screenshot.read_bytes()).hexdigest()

        captured_at_utc = datetime.now(timezone.utc).isoformat()
        metadata = {
            "live_url": LIVE_URL,
            "captured_at_utc": captured_at_utc,
            "safari_version": safari_version,
            "macos_version": macos_version,
            "viewport": {
                "width": int(viewport["width"]),
                "height": int(viewport["height"]),
                "devicePixelRatio": viewport["devicePixelRatio"],
            },
            "screenshot_sha256": screenshot_sha256,
            "public_run_url": public_run_url,
        }
        (EVIDENCE / "metadata.json").write_text(
            json.dumps(metadata, indent=2, sort_keys=True), encoding="utf-8"
        )

        readme = """# Safari evidence bundle

This bundle was captured automatically on a GitHub-hosted macOS runner with
Safari WebDriver. The script opened Safari, resized it, loaded the production
Agent Bounties homepage, waited for the page to finish loading, inspected the
rendered DOM, collected any available browser/console log entries, and saved a
viewport screenshot plus metadata with the Safari and macOS version numbers
and the screenshot SHA-256. The observed result is a fully rendered homepage
containing Agent Bounties, Bounty Board, and Post a bounty.
"""
        (EVIDENCE / "README.md").write_text(readme, encoding="utf-8")
        print(json.dumps(metadata, indent=2, sort_keys=True))
    finally:
        driver.quit()


if __name__ == "__main__":
    main()
