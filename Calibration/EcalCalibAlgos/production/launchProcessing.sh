#!/bin/sh

dataset=/AlCaPhiSym/Run2012A-v1/RAW
friendly_dataset=`echo $dataset | sed s-/-_-g | sed 's/.\(.*\)/\1/'`
era=Collisions12
start=190456
end=999999
GlobalTag=GR_P_V32::All
BeamSpotTag=BeamSpotObjects_PCL_byLumi_v0_prompt
BeamSpotConnect=frontier://PromptProd/CMS_COND_31X_BEAMSPOT
LaserTag=EcalLaserAPDPNRatios_prompt
LaserConnect=frontier://PromptProd/CMS_COND_42X_ECAL_LAS
outputDir=/eos/cms/store/group/alca_ecalcalib/EFlow/

killall -9 runphisymmetry.sh 
killall -9 checkAndRelaunchJobs.sh 

./runphisymmetry.sh caf $dataset $era $start $end $GlobalTag $BeamSpotTag "$BeamSpotConnect" $LaserTag "$LaserConnect" 2>&1 > runphisymmetry.log &
./checkAndRelaunchJobs.sh ${friendly_dataset} ${outputDir} 2>&1 > checkJobs.log &
