#!/bin/sh
#
# Query DBS to count number of events in a dataset 
# in  a given run range
#
# Author : Stefano Argiro


#!/bin/sh
if [ "$1" == "" ]; then
   echo "Usage: $0 <run_min> <run_max> <dataset> "
   exit 1
fi


firstrun=$1
lastrun=$2
dataset=$3


if [ "$3" == "" ]; then
    dataset=/AlCaPhiSymEcal/Run2010A-StreamALCARECOEcalCalPhiSym-v4/ALCARECO
else
    dataset=$3
fi

if [ ! $CMSSW_BASE ] ; then

  echo "Please set CMSSW environment"
  exit
fi 

echo "DBS Query sent, patience.."

tmpfile=`mktemp`

echo "Number of events in given dataset and range: "
dbs search --url="http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet" --query "find run,count(file),sum(file.numevents) where dataset like $dataset and run >= $firstrun  and run <=$lastrun and site=srm-eoscms.cern.ch" >& $tmpfile 

grep -i "error" $tmpfile
if [ $? == "0" ] ; then
    echo "Error from DBS, try a smaller run range"
    exit 1
fi
#cat $tmpfile |  sed '1,4d' |awk 'BEGIN{n=0}{n=n+$3}END{print n}'
cat $tmpfile |  sed '1,4d' | sort -n
rm $tmpfile
