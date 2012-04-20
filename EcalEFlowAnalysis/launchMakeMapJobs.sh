#!/bin/sh

launchDir=`pwd`
xrootdServer=pccmsrm27
outputDir=/cms/local/meridian/EFlow/hitsMaps
cmsswDir=/afs/cern.ch/cms/CAF/CMSPHYS/PHYS_EGAMMA/electrons/meridian/CMSSW427PhySimm/src
queue=1nd
maxJobs=999999


if [[ $1 =~ .*conf.* ]]; then
    echo "Sourcing conf file $1"
    cat $1
    . $1
fi

taskId=`date +%Y%m%d_%H%M%S`

mkdir -p jobs/${taskId}
mkdir -p logs/${taskId}
mkdir -p output/${taskId}
echo "creating directory  ${outputDir} on server ${xrootdServer}"
xrd ${xrootdServer} mkdir ${outputDir}

createJob()
{
    jobName=`basename $1 .txt | sed -e "s%filelist%%g"`
    echo "Launching $jobName"
    cat <<EOF >$launchDir/jobs/${taskId}/makeMapOut_${jobName}.C
{
  gROOT->Reset();
  TChain inputChain_barl("variablesTree");
  
  std::ifstream fileList("$1", ios::in);
  
  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(1);
  }
  
  int sum=0;
  while(!fileList.eof()){
    string nameFile;
    getline(fileList,nameFile);
    inputChain_barl.Add(nameFile.c_str());
    //  cout<<nameFile<<endl;
    sum++;
    cout<<sum<<endl;
  }
  gSystem->Load("${launchDir}/lib/libUtils.so");
//  gROOT->ProcessLine(".L ${launchDir}/makeMap.C+");
  makeMap t(&inputChain_barl);
  t.setOutfile("makeMapOut_${jobName}.root");
  t.Loop();
}
EOF
cat <<EOF2 >${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.sh
cd ${cmsswDir}
eval \`scramv1 runtime -sh\`
if [ -n "\${WORKDIR:-x}" ]; then
    \${WORKDIR}=\${TMPDIR}
fi
cd \${WORKDIR}
cp ${launchDir}/beamSpotInterpolatedCorrections.root ./
echo "root starting at `date`"
root -l -b -q ${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.C 
exit=\$?
echo "root finished at `date` with status \${exit}"
if [ "\$exit"  != "0" ]; then 
    touch ${launchDir}/logs/${taskId}/makeMapjobs.runerror
    echo "\`date\`: ${jobName}" >> ${launchDir}/logs/${taskId}/makeMapjobs.runerror
    exit \$exit
fi    
ls -ltrh
echo "xrdcp makeMapOut_${jobName}.root root://${xrootdServer}///${outputDir}/makeMapOut_${jobName}.root"
xrdcp makeMapOut_${jobName}.root root://${xrootdServer}///${outputDir}/makeMapOut_${jobName}.root
exit=\$?
echo "copy done with exit \${exit}"
if [ "\$exit"  != "0" ]; then 
    touch ${launchDir}/logs/${taskId}/makeMapjobs.copyerror
    echo "\`date\`: ${jobName}" >> ${launchDir}/logs/${taskId}/makeMapjobs.copyerror
    exit \$exit
fi
touch ${launchDir}/logs/${taskId}/makeMapjobs.success
echo "\`date\`: ${jobName}" >> ${launchDir}/logs/${taskId}/makeMapjobs.success
EOF2

chmod a+x ${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.sh
if [ "$queueX" = "localX" ]; then
echo "${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.sh  > ${launchDir}/logs/${taskId}/makeMapOut_${jobName}.log 2>&1"
${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.sh > ${launchDir}/logs/${taskId}/makeMapOut_${jobName}.log 2>&1 &
else
echo "bsub -q ${queue} -J${jobName} -o ${launchDir}/logs/${taskId}/makeMapOut_${jobName}.log < ${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.sh"
bsub -q ${queue} -J${jobName} -o ${launchDir}/logs/${taskId}/makeMapOutjob_${jobName}.log < ${launchDir}/jobs/${taskId}/makeMapOut_${jobName}.sh
fi
}

for var in "$@"; do
  if [[ $var =~ .*conf.* ]]; then
     continue
  fi
#   echo `jobs | wc -l`
  if [ "$queueX" = "localX" ]; then
   while [ `jobs | wc -l` -gt $maxJobs ]; do
#     echo "Sleeping 5"
      sleep 5 
    done 
   fi
  createJob $var
  sleep 1
done
