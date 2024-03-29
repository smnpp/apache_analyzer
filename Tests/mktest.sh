root=$(realpath `pwd`)

if [ "$1" = "" ]
then
  echo "No executable directory given, default to parent"
  exeDir=".."
else  
  if [ -d "$1" ]
  then 
     exeDir="$1"
  else
     echo "$1 is not a directory. Exiting."
     exit 2
  fi
fi
exeDir=$(realpath $exeDir)

cd $(dirname $0)

echo "Test ID;Return code validation;Out result;StdErr result;File creation result;Global result" > $root/results.csv
nOk=0
nKo=0
nTotal=0
nMis=0

for i in */
do
  ./test.sh $exeDir ${i%*/} $root/results.csv
  result=$?
  if [ $result -eq 0 ]
  then
    let "nKo=$nKo+1"
  elif [ $result -eq 1 ]
  then
    let "nOk=$nOk+1"
  else
    let "nMis=$nMis+1"
  fi
  let "nTotal=$nTotal+1"
done

echo "Passed tests     : $nOk"
echo "Failed tests     : $nKo"
echo "Misformed tests  : $nMis"
echo "-----------------------"
echo "Total            : $nTotal"
