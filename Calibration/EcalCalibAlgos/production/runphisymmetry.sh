#! /bin/bash 
#
# yet another glue script to run phisymmetry calibration
# 
# This one will: 
#    1. Call  makephisym to setup the area
#    2. Use crab to run the calibration step 1
#    3. Run step 2
#
# See usage for instructions
#

crabcfg=phisym-cfg.crab.cfg
datadir=$CMSSW_BASE/src/Calibration/EcalCalibAlgos/data
step2out="ca" 

sleeptime=43200

sleepAndRenew(){
    echo "Sleeping $sleeptime and renewing kerberos ticket"
    kinit -R
    klist
    voms-proxy-info
    sleep $sleeptime
}

usage(){
    echo "$0 mode dataset group firstrun lastrun globaltag [bsTag] [bsConnect] [laserTag] [laserConnect]\nArguments in parenthesis can be omitted"
    exit
}

if [ $# -lt 6 ] 
then
    usage
fi

if [ ! $CRABDIR ] ; then
   echo "Please set CRAB environment"
   exit
fi

if [ ! $CRABDIR ] ; then
   echo "Please set CRAB environment"
   exit
fi


STARTDIR=`pwd`
mode=$1
dataset=$2
group=$3
firstrun=$4
lastrun=$5
globaltag=$6

#When tags are present they are used to decide lastRun and maxEndTime
beamSpotTag=''
beamSpotConnect=''
laserTag=''
laserConnect=''

if [ $# -gt 6 ] ; then
    beamSpotTag=$7
fi
if [ $# -gt 7 ] ; then
    beamSpotConnect=$8
fi
if [ $# -gt 8 ] ; then
    laserTag=$9
fi
if [ $# -gt 9 ] ; then
    laserConnect=${10}
fi

. phisym-functions.sh

while [ "X" = "X" ]; do 
    cd ${STARTDIR}
if [ "X$laserTag" != "X" ] ;  then
    if [ "X$laserConnect" = "X" ] ;  then
	echo "$0 : When laserTag is given also laserConnect is needed" 
	usage
	exit 2
    fi
    #Using lastIov-2 to understand last useful time
    maxEndTime=`cmscond_list_iov -c $laserConnect -t $laserTag | tail -n 5 | sed '2,5d' | awk '{printf "%s %s\n",$5,$6}' | sed -e 's%-% %g' | awk '{printf "date -d\"%s %s %s %s\" +%%Y-%%m-%%dT%%H:%%M:%%S\n",$3,$2,$1,$4}' | /bin/sh`  
    echo "Retrieved maxEndTime: $maxEndTime from lastIov of laserTag $laserTag" 
fi

if [ "X$beamSpotTag" != "X" ] ;  then
    if [ "X$beamSpotConnect" = "X" ] ;  then
	echo "$0 : When beamSpotTag is given also beamSpotConnect is needed" 
	usage
	exit 2
    fi
    #Using lastIov-1 to understand last Run
    lastrun=`cmscond_list_iov -c $beamSpotConnect -t $beamSpotTag | tail -n 4 | sed '2,4d' | awk '{print $5}'` 
    echo "Retrieved lastRun: $lastRun from lastIov of beamSpotTag $beamSpotTag" 
fi

# setup job
echo "$0: Setting up job"
touch /tmp/foo

if [ "$laserTagXXX" != "XXX" ] ;  then
    echo "./makephisym.py --mode=$mode --dataset=$dataset --runrange=$firstrun-$lastrun --globaltag=\"$globaltag\" --group=\"$group\" --maxendtime=\"$maxEndTime\""
    ./makephisym.py --mode=$mode --dataset=$dataset --runrange=$firstrun-$lastrun --globaltag="$globaltag" --group="$group" --maxendtime="$maxEndTime"
else
    echo "./makephisym.py --mode=$mode --dataset=$dataset --runrange=$firstrun-$lastrun --globaltag=\"$globaltag\" --group=\"$group\"" 
    ./makephisym.py --mode=$mode --dataset=$dataset --runrange=$firstrun-$lastrun --globaltag="$globaltag" --group="$group" 
fi

if [ $? -eq 1 ] ; then
   echo "$0 : Got an error from makephisym , exiting" 
   exit 1
fi

#cd into last made dir .. ok think of something smarter
dataset_f=`echo $dataset | sed s-/-_-g | sed 's/.\(.*\)/\1/'`
rundir="$dataset"_"$firstrun"_"$lastrun"
rundir=`echo $rundir | sed s-/-_-g | sed 's/.\(.*\)/\1/'`
rundir=`find -type d -newer /tmp/foo -iname "$rundir*" -print` 
rm -rf /tmp/foo

echo "$0 : Running dir is $rundir"

if [ -f analyzed_${dataset_f}.json ]; then
    compareJSON.py --sub $rundir/jsonls-alcaphisym.txt analyzed_${dataset_f}.json > $rundir/toBeAnalyzed.txt
    mv -f $rundir/toBeAnalyzed.txt $rundir/jsonls-alcaphisym.txt
    mergeJSON.py analyzed_${dataset_f}.json $rundir/jsonls-alcaphisym.txt > analyzed_${dataset_f}.json.tmp
    mv -f analyzed_${dataset_f}.json.tmp analyzed_${dataset_f}.json
else
    printJSON.py $rundir/jsonls-alcaphisym.txt > analyzed_${dataset_f}.json
fi

nchar=`wc $rundir/jsonls-alcaphisym.txt | awk '{print $3-3}'`
echo "Size of new json to be analazyed: $nchar"
if [ $nchar = 0 ]; then
    echo "No new lumi was found to be analyzed. Removing folder $rundir"
    rm -rf $rundir
    sleepAndRenew
fi
    
cd  $rundir

# create and submit jobs
echo "$0: Invoking  crab"
crab -debug -cfg $crabcfg -create    
crab -submit

findcrabdir

#keep checking if crab is done
#crabstatus=0  
#while [  $crabstatus -eq 0 ]; do
#   sleep 120
#   crabdone
#   kinit -R
#done

#if [ $njobs -ne $nzero ] ; then
#    echo "ERROR: some jobs failed - "
#    echo "Manually resubmit with crab --forceResubmit"
#    echo "Then run step2 manually with step2-only.sh"
#    exit 1
#fi

#crab -getoutput 
#crab -report

#echo "$0: jobs done,  process step 2"
#crab is done, run step 2

#i="res" #this is what function dostep2 calls the output dir
#dostep2

#echo "$0 Done at `date`. Results in $rundir/$i"
sleepAndRenew
done