g++ .\prover_commit.cpp -o .\prover_commit -O2
g++ .\prover_open.cpp -o .\prover_open -O2
g++ .\verifier.cpp -o .\verifier -O2


function Test {

    param (
        $n,
        $m,
        $sec
    )

	echo $n $m $sec
	echo commit 
	Measure-Command {.\prover_commit.exe $n $m $sec } | findstr "TotalSeconds"  
	echo open 
	Measure-Command {.\prover_open.exe $n $m $sec } | findstr "TotalSeconds"  
	echo verify 
	Measure-Command {.\verifier.exe $n $m $sec } | findstr "TotalSeconds" 
	dir commit
	dir open
    $filename="$n" + "$m" +  "$sec"
    mkdir $filename 
    Move-Item "commit" "$filename\commit" -Force
    Move-Item "answer" "$filename\answer" -Force
    Move-Item "graph" "$filename\graph" -Force
    Move-Item "open" "$filename\open" -Force
    Move-Item "result" "$filename\result" -Force
}

Test(55,89,64)
Test(49,152,64)
Test(58,333,64)
Test(42,213,64)
Test(51,411,64)
Test(52,514,64)
Test(43,420,64)
Test(54,775,64)
Test(44,570,64)
Test(51,78,128)
Test(48,145,128)
Test(54,285,128)
Test(47,279,128)
Test(52,425,128)
Test(50,481,128)
Test(45,469,128)
Test(49,635,128)
Test(43,543,128)