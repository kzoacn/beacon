#include <stdint.h> 
#include <list>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <time.h>
#include <iostream>
#include <fstream>

#define randomLength 512
using namespace std;

// callback function for curl
size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	std::string *str = dynamic_cast<std::string *>((std::string *)userdata);
	str->append((char *)ptr, size * nmemb);
	return size * nmemb;
}


bool beaconDataDownload()
{

	CURLcode res;
	std::string response;

	try
	{
		CURL* pCurl = NULL;

		// In windows, this will init the winsock stuff
		curl_global_init(CURL_GLOBAL_ALL);

		// get a curl handle
		pCurl = curl_easy_init();
		
		if (NULL != pCurl)
		{
			// 设置超时时间为1秒
			curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 10);

			// First set the URL that is about to receive our POST. 
			// This URL can just as well be a 
			// https:// URL if that is what should receive the data.
			curl_easy_setopt(pCurl, CURLOPT_URL, "https://randomnessbeacon.com/index/random/find");
			//curl_easy_setopt(pCurl, CURLOPT_URL, "http://sjs.qilushop.cn/index/index/setRandom");

			// 设置http发送的内容类型为JSON
			curl_slist* plist = curl_slist_append(NULL,
				"Content-Type:application/json;charset=UTF-8");
			plist = curl_slist_append(plist,"token:21e1e27c029cd1e0adc2cf5080229c1a070115822310d79ea48b93cbbc613e96");
			
			curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);
			curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeCallback);  // set callback function
			curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &response); // set var to receive return info from callback function

			Json::Value UploadBody;
			UploadBody["add_bj_time"] = Json::Value("");
			UploadBody["pulse_index"] = Json::Value("");
			UploadBody["external_id"] = Json::Value("2");//根据网站api接口的信息填写
			std::string UploadString = UploadBody.toStyledString();
			// 设置要POST的JSON数据
			curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, UploadString.c_str());
			// Perform the request, res will get the return code 
			res = curl_easy_perform(pCurl);
			// Check for errors
			Json::Value obj;
			Json::Reader reader; 
			if (res != CURLE_OK)
			{
				printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));
				obj["curl_error_code"] = res;
				return res;
			}
			if(reader.parse(response, obj)){
				std::cout << obj["data"]["output_value_pqc"].asString()  << endl;
			
			}else{
				puts("false");
			}
			// always cleanup
			curl_easy_cleanup(pCurl);
			
			
		}
		curl_global_cleanup();
	}
	catch (std::exception& ex)
	{
		printf("curl exception %s.\n", ex.what());
	}
	return res;
}


int main()
{
	//随机数文件位置
	//Json::Reader reader;
	beaconDataDownload();
}
