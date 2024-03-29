echo "-----------------------------------------------------------"

exeDir=$1
root=`pwd`

if [ "$2" = "" ]
then
  echo "No directory given, default to current"
  testDir="."
else  
  echo "| Test id : $2"
  if [ -d "$2" ]
  then 
     testDir="$2"
  else
     echo "$2 is not a directory. Exiting."
     exit 2
  fi
fi

cd $testDir

if [ -r "description" ]
then 
  echo "-----------------------------------------------------------"
  echo "Description :"
  fold -w 60 -s description
  echo 
  echo "-----------------------------------------------------------"
fi

nTestCount=0
nSuccesfulTests=0
nStrResult="$2 "

if [ -r "run" ]
then
  sRun=$(eval "cat run")
else
  echo "No run file found. Exiting."
  exit 2
fi

# stdin has been specified
if [ -r "std.in" ]
then 
  sRun="$sRun < std.in"
fi

# stdout has been specified
if [ -r "std.out" ]
then 
  sRun="$sRun > temp.txt"
fi

# stderr has been specified
if [ -r "stderr.out" ]
then 
  sRun="$sRun 2> temperr.txt"
fi


# adds the executable directory to the path
export PATH="$(realpath $exeDir):$PATH";

# execute the command line
echo $sRun
eval $sRun
returnCode=$?
resultGlobal=1

# compare return code if concerned
resultRC=2
if [ -r "returncode" ]
then 
  if [ "$returnCode" = $(eval "cat returncode") ]
  then
    echo "                                       Return Code : PASSED"
    resultRC=1
  else
    echo "                                       Return Code : FAILED"
    resultRC=0
    resultGlobal=0
  fi
fi

# compare stdout if concerned
resultOut=2
if [ -r "std.out" ]
then 
  diff -wB temp.txt std.out > /dev/null
  if [ $? -eq 0 ]
  then
    echo "                                       Stdout      : PASSED"
    resultOut=1
  else
    echo "                                       Stdout      : FAILED"
    resultOut=0
    resultGlobal=0
  fi
  # clean temporary out file
  rm temp.txt
fi

# compare stderr if concerned
resultErr=2
if [ -r "stderr.out" ]
then 
  diff -wB temperr.txt stderr.out >/dev/null
  if [ $? -eq 0 ]
  then
    echo "                                       Stderr      : PASSED"
    resultErr=1
  else
    echo "                                       Stderr      : FAILED"
    resultErr=0
    resultGlobal=0
  fi
  # clean temporary out file
  rm temperr.txt
fi

# compare files created if concerned
resultFiles=2
if ls *.outfile &> /dev/null
then
  number=1
  for i in *.outfile
  do
    fileName=`basename $i .outfile`
    fileName=${fileName%*/}
    if [ -r $fileName ]
    then
      diff -wB $i $fileName
      if [ $? -eq 0 ]
      then
        echo "                                       File #$number     : PASSED"
      else
        echo "                                       File #$number     : FAILED"
        resultFiles=0
        resultGlobal=0
      fi  
      rm $fileName
    else  
      echo "                                       File #$number     : FAILED"
      resultFiles=0
      resultGlobal=0
    fi
    let "number=$number+1"
  done
  if [ $resultFiles -eq 2 ]
  then
    resultFiles=1
  fi
fi

echo "                                       --------------------"
if [ $resultGlobal -eq 0 ]
then
  echo "                                       Global      : FAILED"
else
  echo "                                       Global      : PASSED"
fi
echo "-----------------------------------------------------------"
echo 

cd $root

# log result in $3 if filename provided
if [ "$3" != "" ]
then
  if [ ! -w "$3" ]
  then
    touch $3
  fi
  if [ -w "$3" ]
  then
    echo "$testDir;$resultRC;$resultOut;$resultErr;$resultFiles;$resultGlobal" >>$3
  fi
fi

exit $resultGlobal

