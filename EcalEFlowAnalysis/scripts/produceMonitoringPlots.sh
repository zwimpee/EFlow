#!/bin/bash

### INPUTS #####
#taskName=600M_noBsCorr
#finalPlotsTag=2012dataBsCorr_exclude186-242
#second name of the folder containing big ntuples (either v4, EcalLaser_20120419....)
#ntupleTag="EcalLaser_20120419"
#dataset="AlCaPhiSym_Run2012A-v1_RAW"
#big ntuples are stored in eos
eosNtupleLocation="/eos/cms/store/group/alca_ecalcalib/EFlow/"
jsonFile=analyzed_${dataset}.json
kfactorsFile=`pwd`/data/kFactors.root
kfactorsXtalFile=`pwd`/data/kFactors_xtals.root
kfactorsEndcFile=`pwd`/data/kFactors_endc.root
kfactorsVsTimeFile=`pwd`/data/kfactorsVsTime_2012.root
harnessMapFile=`pwd`/data/harnessMap.root
eeIndicesFile=`pwd`/data/eeIndicesMap.root
bsCorrectionFile=`pwd`/data/beamSpotAsymm_fromData.root


#### CONFIGS #####
intervalHits=6000
intervalMaxStopHours=12
normType=ring
normRing=9
normInterval=10
normIntervalRange=3
applyBSCorrection=1
applyLumiCorrection=1
excludeRangeStart=999999
excludeRangeEnd=-1
applyKFactor=1
useKFactorsPerXtal=1
kfactorVsTime=0
timeStart=1333552553
timeEnd=`date +%s`
startIntervalForHisto=0
#monitoringDays=12

##### OUTPUT #####
#output dir con also be in eos (server eoscms)
xrootdServer=pccmsrm27 
#to write in eos paths are /eos/cms/store/user/....
hitsMapLocation=/cms/local/meridian/EFlow/hitsMaps
historyTreeLocation=/cms/local/meridian/EFlow/historyTree_2012
fullHistoryLocation=/cms/local/meridian/EFlow/fullHistoryTree
historiesLocation=/cms/local/meridian/EFlow/histories

#### SWITCHES #####
#incremental=NO
doFileList=YES
doMaps=YES
doReadMapFile=YES
doCreateHistory=YES
doCreateLastTree=YES
doHistories=YES
doMonitoringPlots=YES

. helper-functions.sh 

if [[ $1 =~ .*conf.* ]]; then
    echo "Sourcing conf file $1"
    cat $1
    . $1
fi

if [ "$doFileList" = "YES" ]; then
 
    echo "[`date`]: Launching production of list for ${dataset} ${ntupleTag}"
#    if [ "$incremental" = "NO" ]; then
    rm -rf list_${dataset}_${ntupleTag}
#    else
#	cp list_${dataset}_${ntupleTag}/allFiles.txt.bck
#	rm -rf list_${dataset}_${ntupleTag}/filelist*.txt
#    fi	
#    for folder in `/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select find -d ${eosNtupleLocation} | grep ${ntupleTag}  | awk -F '/' '{print $8}'`; do
    for folder in `/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select find -d ${eosNtupleLocation} | grep ${ntupleTag}  | grep ${dataset} | awk -F '/' '{print $8}'`; do
	echo "./runPrepareList.csh list_${dataset}_${ntupleTag} ${eosNtupleLocation}/${folder} eos ${ntupleTag} 1 1 ${incremental}"
#	./runPrepareList.csh list_${dataset}_${ntupleTag} ${eosNtupleLocation}/${folder} eos ${ntupleTag} 1 1 ${incremental}
	./runPrepareList.csh list_${dataset}_${ntupleTag} ${eosNtupleLocation}/${folder} eos ${ntupleTag} 1 2 
    done

    mkdir -p conf
    
    echo "[`date`]: List done for ${dataset} ${ntupleTag}. Total files `ls -1 list_${dataset}_${ntupleTag}/filelist${dataset}*.txt| wc -l`"
fi


if [ "$doMaps" = "YES" ]; then

    cat > conf/makeMapJobs_${dataset}_${ntupleTag}.conf <<EOF
    xrootdServer=${xrootdServer}
    outputDir=${hitsMapLocation}/${dataset}_${ntupleTag}
    cmsswDir=${CMSSW_BASE}
    queue=cmscaf1nd
    taskName=${dataset}_${ntupleTag}
EOF

    echo "[`date`]: Launching makeMapJobs"
    echo "./launchMakeMapJobs.sh `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt conf/makeMapJobs_${dataset}_${ntupleTag}.conf" 
    ./launchMakeMapJobs.sh conf/makeMapJobs_${dataset}_${ntupleTag}.conf `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt 
    findtaskdir ${dataset}_${ntupleTag}
    sleep 120	    
    isTaskDone
    while [ "$taskStatus" != "YES" ]; do
	echo "[`date`]: task ${taskId} ${taskStatus}"
	sleep 120
	isTaskDone 
    done

    echo "[`date`]: makeMapJobs completed"
fi

if [ "$doReadMapFile" = "YES" ]; then
    xrd ${xrootdServer} ls ${hitsMapLocation}/${dataset}_${ntupleTag}/ | grep ".root" | awk '{print $NF}' | sort | awk '{printf "root://'${xrootdServer}'//%s\n",$NF}' > list_${dataset}_${ntupleTag}/makeMapOut_${dataset}.list
    cat > jobs/readMapExecute_${dataset}_${ntupleTag}_${taskName}.C <<EOF
    {
	TChain c("mapTree_barl");
  std::ifstream fileList("list_${dataset}_${ntupleTag}/makeMapOut_${dataset}.list", ios::in);
  
  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(1);
    }
    
    int sum=0;
    while(!fileList.eof()){
	string nameFile;
	getline(fileList,nameFile);
	c.Add(nameFile.c_str());
	//  cout<<nameFile<<endl;
    sum++;
    cout<<sum<<endl;
    }
    
    gSystem->Load("lib/libUtils.so");
    gROOT->ProcessLine(".L readMap.C++");
    readMap t(&c);
    t.outFileName="readMap_${dataset}_${ntupleTag}_${taskName}.root";
    t.setJSON("${jsonFile}");
    t.setHitsMaxPerXtal(${intervalHits});
    t.setMaxStopHours(${intervalMaxStopHours});
    t.Loop();
    }
EOF

echo "[`date`]: root -l -b -q jobs/readMapExecute_${dataset}_${ntupleTag}_${taskName}.C > logs/readMapExecute_${dataset}_${ntupleTag}_${taskName}.log"
root -l -b -q jobs/readMapExecute_${dataset}_${ntupleTag}_${taskName}.C > logs/readMapExecute_${dataset}_${ntupleTag}_${taskName}.log

fi


if [ "$doCreateHistory" = "YES" ]; then

    cat > conf/createHistory_${dataset}_${ntupleTag}.conf <<EOF
    xrootdServer=${xrootdServer}
    taskName=${dataset}_${ntupleTag}_${taskName}
    outputDir=${historyTreeLocation}/${dataset}_${ntupleTag}_${taskName}
    intervalFile=`pwd`/readMap_${dataset}_${ntupleTag}_${taskName}.root
    applyBSCorrection=0
#    bsCorrectionFile=${bsCorrectionFile}
#    applyBSCorrection=${applyBSCorrection}
    json=${jsonFile}
    launchDir=`pwd`
    cmsswDir=${CMSSW_BASE}
    queue=cmscaf1nd
    maxJobs=999999
EOF

    echo "[`date`]: Launching createHistory"
    echo "./launchCreateJobs.sh conf/createHistory_${dataset}_${ntupleTag}.conf `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt"
    ./launchCreateJobs.sh conf/createHistory_${dataset}_${ntupleTag}.conf `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt 
    findtaskdir ${taskName}
    sleep 120	    
    isTaskDone
    while [ "$taskStatus" != "YES" ]; do
	echo "[`date`]: task ${taskId} ${taskStatus}"
        if [ "$taskStatus" = "ERROR" ]; then
            ./relaunchJobs.sh ${taskId} createHistory
        fi
	sleep 120
	isTaskDone 
    done

    echo "[`date`]: createHistory completed"
fi

if [ "$doCreateLastTree" = "YES" ]; then
    xrd ${xrootdServer} ls ${historyTreeLocation}/${dataset}_${ntupleTag}_${taskName}/ | grep ".root" | awk '{print $NF}' | sort | awk '{printf "root://'${xrootdServer}'//%s\n",$NF}' > list_${dataset}_${ntupleTag}/createHistoryTree_${dataset}_${ntupleTag}_${taskName}.list

    cat > jobs/createLastTree_${dataset}_${ntupleTag}_${taskName}.C <<EOF
{
  gROOT->Reset();
  TChain*  c=new TChain("tree");
  TChain*  c_bs=new TChain("bsTree");
  std::ifstream fileList("list_${dataset}_${ntupleTag}/createHistoryTree_${dataset}_${ntupleTag}_${taskName}.list", ios::in);

  if (!fileList.is_open()) {
    cout<< "file not found"<< endl;
    exit(1);
    }

    int sum=0;
    while(!fileList.eof()){
        string nameFile;
        getline(fileList,nameFile);
        c->Add(nameFile.c_str());
        c_bs->Add(nameFile.c_str());
    sum++;
    cout<<sum<<endl;
    }

  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L createLastTree.C++");
  gROOT->ProcessLine(".L createLastTree_bs.C++");
  createLastTree t(c);
  createLastTree_bs t_bs(c_bs);
  t.setLumiIntervals("${PWD}/readMap_${dataset}_${ntupleTag}_${taskName}.root");
  t.setOutfile("${SCRATCH}/finalTree_${dataset}_${ntupleTag}_${taskName}.root");
  t_bs.setLumiIntervals("${PWD}/readMap_${dataset}_${ntupleTag}_${taskName}.root");
  t_bs.setOutfile("${SCRATCH}/bsInfo_${dataset}_${ntupleTag}_${taskName}.root");
  t.Loop();
  t_bs.Loop();
}
EOF


echo "[`date`]: root -l -b -q jobs/createLastTree_${dataset}_${ntupleTag}_${taskName}.C > logs/createLastTree_${dataset}_${ntupleTag}_${taskName}.log"
root -l -b -q jobs/createLastTree_${dataset}_${ntupleTag}_${taskName}.C > logs/createLastTree_${dataset}_${ntupleTag}_${taskName}.log
xrd ${xrootdServer} rm ${fullHistoryLocation}/finalTree_${dataset}_${ntupleTag}_${taskName}.root
xrdcp ${SCRATCH}/finalTree_${dataset}_${ntupleTag}_${taskName}.root root://${xrootdServer}//${fullHistoryLocation}/finalTree_${dataset}_${ntupleTag}_${taskName}.root
xrd ${xrootdServer} rm ${fullHistoryLocation}/bsInfo_${dataset}_${ntupleTag}_${taskName}.root
xrdcp ${SCRATCH}/bsInfo_${dataset}_${ntupleTag}_${taskName}.root root://${xrootdServer}//${fullHistoryLocation}/bsInfo_${dataset}_${ntupleTag}_${taskName}.root
fi

if [ "${doHistories}" = "YES" ]; then
    cat > jobs/makeHistories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.C <<EOF
{
  gROOT->Reset();
  TFile* f= TFile::Open("root://${xrootdServer}//${fullHistoryLocation}/finalTree_${dataset}_${ntupleTag}_${taskName}.root");
  TTree* intree= (TTree*)f->Get("finalTree_barl");
  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L makeControlPlots.C++");
  gROOT->ProcessLine("makeControlPlots t(intree)");
  t.setLumiIntervals("${PWD}/readMap_${dataset}_${ntupleTag}_${taskName}.root");
  t.setOutfile("root://${xrootdServer}//${historiesLocation}/histories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}");  
  t.bsInfoFile=TString("root://${xrootdServer}//${fullHistoryLocation}/bsInfo_${dataset}_${ntupleTag}_${taskName}.root");
  t.eeIndicesFile="${eeIndicesFile}";
  t.applyBSCorrection=${applyBSCorrection};
  t.applyLumiCorrection=${applyLumiCorrection};
  t.useKFactorsPerXtal=${useKFactorsPerXtal};
  t.kfactorVsTime=${kfactorVsTime};
  //This file is the same for bs and lumi corrections
  t.bsCorrectionFile=TString("${bsCorrectionFile}");
  t.kfactorCorr=${applyKFactor};
  t.kFactorsFile="${kfactorsFile}";
  t.kFactorsXtalFile="${kfactorsXtalFile}";
  t.kFactorsEndcFile="${kfactorsEndcFile}";
  t.kFactorsVsTimeFile="${kfactorsVsTimeFile}";
  t.harnessMapFile="${harnessMapFile}";
  t.kfactor_alpha=1.;
  t.kfactorABCorr=false;
  t.kfactorAB_alpha=1.;
  t.errEtCorr_factor=1.;
  t.excludedRangeStart=${excludeRangeStart};
  t.excludedRangeEnd=${excludeRangeEnd};
  t.normalizationType="${normType}";
  t.ringRefRegion=${normRing};
  t.historyNormalizationInterval=${normInterval};
  t.historyNormalizationIntervalRange=${normIntervalRange};
  gROOT->ProcessLine("t.Loop()");
}
EOF

echo "[`date`]: root -l -b -q jobs/makeHistories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.C > logs/makeHistories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.log"
root -l -b -q jobs/makeHistories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.C > logs/makeHistories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.log
fi

if [ "${doMonitoringPlots}" = "YES" ]; then

    cat > jobs/drawControlPlots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.C <<EOF
{
    gROOT->Reset();
    gROOT->ProcessLine(".L drawControlPlots.C++");
    gSystem->Load("lib/libUtils.so");
    
    TString prefix="root://${xrootdServer}//${historiesLocation}/histories_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}_";

//    int X0=${timeEnd}-86400*${monitoringDays}-86400*2;
    int X0=${timeStart};
    int X1=${timeEnd}-86400*2;

    //EB Plots
    float axisLower=0.93;
    float axisUp=1.05;
    float axisLowerXtal=0.9;
    float axisUpXtal=1.1;
    //    float ttMeanLowThreshold=0.985;
    //    float ttMeanHighThreshold=1.015;
    //    float ttRMSThreshold=0.004;
    //    float xtalMeanLowThreshold=0.98;
    //    float xtalMeanHighThreshold=1.02;
    //    float xtalRMSThreshold=0.013;

    float ttMeanLowThreshold=0.;
    float ttMeanHighThreshold=9999.;
    float ttRMSThreshold=9999.;
    float harnessMeanLowThreshold=0.;
    float harnessMeanHighThreshold=9999.;
    float harnessRMSThreshold=9999.;
    float xtalMeanLowThreshold=0.;
    float xtalMeanHighThreshold=9999.;
    float xtalRMSThreshold=9999.;

    int startInterval=${startIntervalForHisto};
    
    drawControlPlots(prefix,0,0,1,0,0,0,0,0,0,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,1,0,0,0,0,0,0,0,0,1,0,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,1,0,0,0,0,0,0,0,0,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,1,0,0,0,0,0,0,1,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,1,0,0,0,0,1,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,1,0,0,0,0,0,0,0,0,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,1,0,0,0,0,0,0,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);

    //EE Plots
    axisLower=0.7;
    axisUp=1.15;
    axisLowerXtal=0.7;
    axisUpXtal=1.2;

    ttMeanLowThreshold=0.;
    ttMeanHighThreshold=9999.;
    ttRMSThreshold=9999.;
    harnessMeanLowThreshold=0.;
    harnessMeanHighThreshold=9999.;
    harnessRMSThreshold=9999.;
    xtalMeanLowThreshold=0.;
    xtalMeanHighThreshold=9999.;
    xtalRMSThreshold=9999.;
    
    //    ttMeanLowThreshold=0.98;
    //    ttMeanHighThreshold=1.02;
    //    ttRMSThreshold=0.025;
    //    xtalMeanLowThreshold=0.94;
    //    xtalMeanHighThreshold=1.06;
    //    xtalRMSThreshold=0.045;
    startInterval=${startIntervalForHisto};
    
    drawControlPlots(prefix,0,0,0,0,0,0,1,0,0,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,0,1,0,0,0,0,1,0,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,0,1,0,0,0,0,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,0,0,1,0,0,1,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,0,0,0,0,1,1,1,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,0,0,1,0,0,0,0,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
    drawControlPlots(prefix,0,0,0,0,0,0,0,1,0,0,1,X0,X1,startInterval,axisLower,axisUp,axisLowerXtal,axisUpXtal,ttMeanLowThreshold,ttMeanHighThreshold,ttRMSThreshold,harnessMeanLowThreshold,harnessMeanHighThreshold,harnessRMSThreshold,xtalMeanLowThreshold,xtalMeanHighThreshold,xtalRMSThreshold);
}
EOF
   rm -rf plots

#Saving N-1 iteration into .old folder. Beware N-2 is removed
   if [ -d plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.old ]; then
      rm -rf plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.old
   fi
   if [ -d plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag} ]; then
       mv -f plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag} plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.old
   fi 

   mkdir -p plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}
   ln -s plots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag} plots
   rm -rf plots/*png
echo "[`date`]: root -l -b -q jobs/drawControlPlots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.C > logs/drawControlPlots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.log"
root -l -b -q jobs/drawControlPlots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.C > logs/drawControlPlots_${dataset}_${ntupleTag}_${taskName}_${finalPlotsTag}.log
fi