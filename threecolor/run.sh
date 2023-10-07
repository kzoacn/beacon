g++ prover_commit.cpp -o prover_commit -O2 -mrdseed
g++ prover_open.cpp -o prover_open -O2 -mrdseed
g++ verifier.cpp -o verifier -O2 -mrdseed
#g++ download.cpp -o download -O2 -lcurl -ljsoncpp
#./download > random

function Test() {

	let n=$1
	let m=$2
	let sec=$3
	echo $n $m $sec
	>&2 echo "running"
	>&2 echo "commit"
	{ time ./prover_commit $n $m $sec; }
	>&2 echo "open"
	{ time ./prover_open $n $m $sec; }
	>&2 echo "verify" 
	{ time ./verifier $n $m $sec; }
	>&2  ls -l commit
	>&2 ls -l open

}

Test 55 89  64
Test 49 152 64
Test 58 333 64
Test 42 213 64
Test 51 411 64
Test 52 514 64
Test 43 420 64
Test 54 775 64
Test 44 570 64
Test 51 78 128
Test 48 145 128
Test 54 285 128
Test 47 279 128
Test 52 425 128
Test 50 481 128
Test 45 469 128
Test 49 635 128
Test 43 543 128