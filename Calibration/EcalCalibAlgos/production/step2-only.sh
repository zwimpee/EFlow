#! /bin/bash

# run step 2 only
#
#
# Author: Stefano Argiro'


usage(){
    echo "Usage: $0 dataset firstrun lastrun globaltag"
    exit
}

if [ $# -ne 4 ] 
then
    usage
fi



. phisym-functions.sh

dataset=$1
firstrun=$2
lastrun=$3
globaltag=$4
mode=caf
group=Collisions11

step2out="CalibHistos.root ehistos.root etsumMean_barl.dat etsumMean_endc.dat PhiSymmetryCalibration_miscal_resid.root PhiSymmetryCalibration.root etsummary_barl.dat etsummary_endc.dat" 

if [ ! $CRABDIR ] ; then
   echo "Please set CRAB environment"
   exit
fi


rundir="$dataset"_"$firstrun"_"$lastrun"
rundir=`echo $rundir | sed s-/-_-g | sed 's/.\(.*\)/\1/'`
echo "$0 : Running dir is $rundir"


./makephisym.py --mode=$mode --dataset=$dataset --runrange=$firstrun-$lastrun --globaltag="$globaltag" --step2only --group="$group" 

cd  $rundir

findcrabdir

i="res"

dostep2




