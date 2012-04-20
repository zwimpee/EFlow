#!/bin/bash

### INPUTS #####
ntupleTag="EcalLaser_20120419"
dataset="AlCaPhiSym_Run2012A-v1_RAW"
eosNtupleLocation="/eos/cms/store/group/alca_ecalcalib/EFlow/"
jsonFile=analyzed_${dataset}.json

#### CONFIGS #####
intervalHits=1500
intervalMaxStopHours=12
normType=ring
normRing=9
normInterval=37
kfactorsFile=/afs/cern.ch/work/m/meridian/EFlow/kFactors.root
timeStart=1333552553
timeEnd=`date +%s`
#monitoringDays=12

##### OUTPUT #####
xrootdServer=pccmsrm27
hitsMapLocation=/cms/local/meridian/EFlow/hitsMaps
historyTreeLocation=/cms/local/meridian/EFlow/historyTree_2012
fullHistoryLocation=/cms/local/meridian/EFlow/fullHistoryTree
historiesLocation=/cms/local/meridian/EFlow/histories

#### SWITCHES #####
doFileList=NO
doMaps=NO
doReadMapFile=NO
doCreateHistory=NO
doCreateLastTree=NO
doHistories=NO
doMonitoringPlots=YES

. helper-functions.sh 

if [ "$doFileList" = "YES" ]; then
 
    echo "[`date`]: Launching production of list for ${dataset} ${ntupleTag}"
    rm -rf list_${dataset}_${ntupleTag}	
    for folder in `/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select find -d ${eosNtupleLocation} | grep ${ntupleTag} | awk -F '/' '{print $8}'`; do
	echo "./runPrepareList.csh list_${dataset}_${ntupleTag} ${eosNtupleLocation}/${folder} eos 1 1"
	./runPrepareList.csh list_${dataset}_${ntupleTag} ${eosNtupleLocation}/${folder} eos 1 1
    done

    mkdir -p conf
    
    echo "[`date`]: List done for ${dataset} ${ntupleTag}. Total files `ls -1 list_${dataset}_${ntupleTag}/filelist${dataset}*.txt| wc -l`"
fi


if [ "$doMaps" = "YES" ]; then

    cat > conf/makeMapJobs_${dataset}_${ntupleTag}.conf <<EOF
    xrootdServer=${xrootdServer}
    outputDir=${hitsMapLocation}/${dataset}_${ntupleTag}
    cmsswDir=/afs/cern.ch/cms/CAF/CMSPHYS/PHYS_EGAMMA/electrons/meridian/CMSSW427PhySimm/src
    queue=1nd
EOF

    echo "[`date`]: Launching makeMapJobs"
    echo "./launchMakeMapJobs.sh `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt conf/makeMapJobs_${dataset}_${ntupleTag}.conf" 
    ./launchMakeMapJobs.sh conf/makeMapJobs_${dataset}_${ntupleTag}.conf `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt 
    findtaskdir
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
    cat > jobs/readMapExecute_${dataset}_${ntupleTag}.C <<EOF
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
    t.outFileName="readMap_${dataset}_${ntupleTag}.root";
    t.setJSON("${jsonFile}");
    t.setHitsMaxPerXtal(${intervalHits});
    t.setMaxStopHours(${intervalMaxStopHours});
    t.Loop();
    }
EOF

echo "[`date`]: root -l -b -q jobs/readMapExecute_${dataset}_${ntupleTag}.C > logs/readMapExecute_${dataset}_${ntupleTag}.log"
root -l -b -q jobs/readMapExecute_${dataset}_${ntupleTag}.C > logs/readMapExecute_${dataset}_${ntupleTag}.log

fi


if [ "$doCreateHistory" = "YES" ]; then

    cat > conf/createHistory_${dataset}_${ntupleTag}.conf <<EOF
    xrootdServer=${xrootdServer}
    outputDir=${historyTreeLocation}/${dataset}_${ntupleTag}
    intervalFile=readMap_${dataset}_${ntupleTag}.root
    json=${jsonFile}
    launchDir=`pwd`
    cmsswDir=/afs/cern.ch/cms/CAF/CMSPHYS/PHYS_EGAMMA/electrons/meridian/CMSSW427PhySimm/src
    queue=1nd
    maxJobs=999999
EOF

    echo "[`date`]: Launching createHistory"
    echo "./launchCreateJobs.sh conf/createHistory_${dataset}_${ntupleTag}.conf `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt"
    ./launchCreateJobs.sh conf/createHistory_${dataset}_${ntupleTag}.conf `pwd`/list_${dataset}_${ntupleTag}/filelist${dataset}*.txt 
    findtaskdir
    sleep 120	    
    isTaskDone
    while [ "$taskStatus" != "YES" ]; do
	echo "[`date`]: task ${taskId} ${taskStatus}"
        if [ "$taskStatus" = "ERROR" ]; then
            ./relaunchJobs.sh ${taskId}
        fi
	sleep 120
	isTaskDone 
    done

    echo "[`date`]: createHistory completed"
fi

if [ "$doCreateLastTree" = "YES" ]; then
    xrd ${xrootdServer} ls ${historyTreeLocation}/${dataset}_${ntupleTag}/ | grep ".root" | awk '{print $NF}' | sort | awk '{printf "root://'${xrootdServer}'//%s\n",$NF}' > list_${dataset}_${ntupleTag}/createHistoryTree_${dataset}.list

    cat > jobs/createLastTree_${dataset}_${ntupleTag}.C <<EOF
{
  gROOT->Reset();
  TChain*  c=new TChain("tree_barl");
  std::ifstream fileList("list_${dataset}_${ntupleTag}/createHistoryTree_${dataset}.list", ios::in);

  if (!fileList.is_open()) {
    cout<< "file not found"<< endl;
    exit(1);
    }

    int sum=0;
    while(!fileList.eof()){
        string nameFile;
        getline(fileList,nameFile);
        c->Add(nameFile.c_str());

    sum++;
    cout<<sum<<endl;
    }

  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L createLastTree.C++");
  createLastTree t(c);
  t.setLumiIntervals("readMap_${dataset}_${ntupleTag}.root");
  t.setOutfile("root://${xrootdServer}//${fullHistoryLocation}/finalTree_${dataset}_${ntupleTag}.root");
  t.Loop();
}
EOF

echo "[`date`]: root -l -b -q jobs/createLastTree_${dataset}_${ntupleTag}.C > logs/createLastTree_${dataset}_${ntupleTag}.log"
root -l -b -q jobs/createLastTree_${dataset}_${ntupleTag}.C > logs/createLastTree_${dataset}_${ntupleTag}.log
fi

if [ "${doHistories}" = "YES" ]; then
    cat > jobs/makeHistories_${dataset}_${ntupleTag}.C <<EOF
{
  gROOT->Reset();
  TFile* f= TFile::Open("root://${xrootdServer}//${fullHistoryLocation}/finalTree_${dataset}_${ntupleTag}.root");
  TTree* intree= (TTree*)f->Get("finalTree_barl");
  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L makeControlPlots.C++");
  gROOT->ProcessLine("makeControlPlots t(intree)");
  t.setLumiIntervals("readMap_${dataset}_${ntupleTag}.root");
  t.setOutfile("root://${xrootdServer}//${historiesLocation}/histories_${dataset}_${ntupleTag}");  
  t.kfactorCorr=true;
  t.kFactorsFile="${kfactorsFile}";
  t.kfactor_alpha=1.;
  t.kfactorABCorr=false;
  t.kfactorAB_alpha=1.;
  t.errEtCorr_factor=1.;
  t.normalizationType="${normType}";
  t.historyNormalizationInterval=${normInterval};
  t.ringRefRegion=${normRing};
  gROOT->ProcessLine("t.Loop()");
}
EOF

echo "[`date`]: root -l -b -q jobs/makeHistories_${dataset}_${ntupleTag}.C > logs/makeHistories_${dataset}_${ntupleTag}.log"
root -l -b -q jobs/makeHistories_${dataset}_${ntupleTag}.C > logs/makeHistories_${dataset}_${ntupleTag}.log
fi

if [ "${doMonitoringPlots}" = "YES" ]; then

    cat > jobs/drawControlPlots_${dataset}_${ntupleTag}.C <<EOF
{
    gROOT->Reset();
    gROOT->ProcessLine(".L drawControlPlots.C+");
    
    TString prefix="root://${xrootdServer}//${historiesLocation}/histories_${dataset}_${ntupleTag}_";

//    int X0=${timeEnd}-86400*${monitoringDays}-86400*2;
    int X0=${timeStart};
    int X1=${timeEnd}-86400*2;

    float axisLower=0.93;
    float axisUp=1.05;
    float axisLowerXtal=0.9;
    float axisUpXtal=1.1;
    
    drawControlPlots(prefix,1,0,0,0,1,0,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
    drawControlPlots(prefix,1,0,0,0,1,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
    drawControlPlots(prefix,0,1,0,1,1,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
    drawControlPlots(prefix,0,0,1,1,1,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
    drawControlPlots(prefix,0,1,0,0,0,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
    drawControlPlots(prefix,0,0,1,0,0,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
}
EOF

echo "[`date`]: root -l -b -q jobs/drawControlPlots_${dataset}_${ntupleTag}.C > logs/drawControlPlots_${dataset}_${ntupleTag}.log"
root -l -b -q jobs/drawControlPlots_${dataset}_${ntupleTag}.C > logs/drawControlPlots_${dataset}_${ntupleTag}.log
fi