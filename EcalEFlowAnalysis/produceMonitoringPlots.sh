#!/bin/bash

ntupleTag="EcalLaser_20120419"
dataset="AlCaPhiSym_Run2012A-v1_RAW"
eosNtupleLocation="/eos/cms/store/group/alca_ecalcalib/EFlow/"
jsonFile=analyzed_${dataset}.json
intervalHits=1500
intervalMaxStopHours=12
xrootdServer=pccmsrm27
hitsMapLocation=/cms/local/meridian/EFlow/hitsMaps

doFileList=YES
doMaps=YES
doReadMapFile=YES

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
    taskId=findtaskDir
    sleep 120	    
    while [ isTaskDone ${taskId} != "YES" ]; do
	taskStatus=isTaskDone ${taskId}
	echo "[`date`]: task ${taskId} ${taskStatus}"
	sleep 120
    done

    echo "[`date`]: makeMapJobs completed"
fi

if [ "$doReadMapFile" = "YES" ]; then

    cat >> jobs/readMapExecute_${dataset}_${ntupleTag}.C <<EOF
    {
	TChain c("mapTree_barl");
	c.Add("root://${xrootdServer}//${hitsMapLocation}/${dataset}_${ntupleTag}/makeMapOut_${dataset}*root");
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
echo "root -l -b -q jobs/readMapExecute_${dataset}_${ntupleTag}.C > logs/readMapExecute_${dataset}_${ntupleTag}.log"
root -l -b -q jobs/readMapExecute_${dataset}_${ntupleTag}.C > logs/readMapExecute_${dataset}_${ntupleTag}.log

fi
#./launchCreateJobs.sh `pwd`/listRUN2011/filelist1*txt
#
#./relaunchJobs.sh 20120417_173003
#
#root -l -b -q createLastTreeExecute.C
#
#{
#  gROOT->Reset();
#  TChain*  c=new TChain("tree_barl");
#  c->Add("/xrootdfs/cms/local/meridian/EFlow/historyTree_2012/*root");
#  gSystem->Load("lib/libUtils.so");
#  gROOT->ProcessLine(".L createLastTree.C++");
#  createLastTree t(c);
#  t.setLumiIntervals("readMap_Run2012A_v1_190456_191277.root");
#  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2012A_v1_190456_191277.root");
#  t.Loop();
#}
#
#
#{
#  gROOT->Reset();
#  TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2012A_v1_190456_191277.root");
#  TTree* intree= (TTree*)f->Get("finalTree_barl");
#  gSystem->Load("lib/libUtils.so");
#  gROOT->ProcessLine(".L makeControlPlots.C++");
#  gROOT->ProcessLine("makeControlPlots t(intree)");
#  t.setLumiIntervals("readMap_Run2012A_v1_190456_191277_test.root");
#  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2012A_v1_190456_191277.root");  
#  t.kfactorCorr=true;
#  t.kFactorsFile="/afs/cern.ch/work/m/meridian/EFlow/kFactors.root";
#  t.kfactor_alpha=1.;
#  t.kfactorABCorr=false;
#  t.kfactorAB_alpha=1.;
#  t.errEtCorr_factor=1.;
#  t.normalizationType="ring";
#  t.historyNormalizationInterval=37;
#  t.ringRefRegion=9;
#  gROOT->ProcessLine("t.Loop()");
#}
#
#root -l -b -q drawControlPlots.C
