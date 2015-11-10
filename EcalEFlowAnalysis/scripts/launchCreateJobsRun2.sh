#!/bin/sh

intervalFile=readMap_Run2012A_v1_190456_191277.root
json=analyzed_AlCaPhiSym_Run2012A-v1_RAW.json
taskName=v1
filePrefix=root://xrootd-cms.infn.it/
launchDir=`pwd`
xrootdServer=pccmsrm27
outputDir=/cms/local/meridian/EFlow/historyTree_2012
cmsswDir=/afs/cern.ch/cms/CAF/CMSPHYS/PHYS_EGAMMA/electrons/meridian/CMSSW427PhySimm/src
queue=1nd
maxJobs=999999

if [[ $1 =~ .*conf.* ]]; then
    echo "Sourcing conf file $1"
    cat $1
    . $1
fi

taskId=${taskName}_`date +%Y%m%d_%H%M%S`

mkdir -p jobs/${taskId}
mkdir -p logs/${taskId}
mkdir -p output/${taskId}
xrd ${xrootdServer} mkdir ${outputDir}

createJob()
{
    jobName=`basename $1 .txt | sed -e "s%filelist%%g"`
    echo "Launching $jobName"
cat <<EOF2 >${launchDir}/jobs/${taskId}/createHistory_${jobName}.sh
cd ${cmsswDir}
export SCRAM_ARCH=${SCRAM_ARCH} 
eval \`scramv1 runtime -sh\`
if [ -n "\${WORKDIR:-x}" ]; then
    \${WORKDIR}=\${TMPDIR}
fi
cd \${WORKDIR}

echo "createHistory starting at \`date\`"
${launchDir}/python/createHistory.py --debug --fileList=$1 --output=createHistoryOut_${jobName}.root --jsonFile=${json} --prefix=${filePrefix} --mapFile=${intervalFile}
exit=\$?
echo "createHistory finished at \`date\` with status \${exit}"
if [ "\$exit"  != "0" ]; then 
    touch ${launchDir}/logs/${taskId}/createHistoryjobs.runerror
    echo "\`date\`: ${jobName}" >> ${launchDir}/logs/${taskId}/createHistoryjobs.runerror
    exit \$exit
fi    
ls -ltrh
echo "xrdcp createHistoryOut_${jobName}.root root://${xrootdServer}///${outputDir}/createHistoryOut_${jobName}.root"
xrd ${xrootdServer} rm ${outputDir}/createHistoryOut_${jobName}.root
xrdcp createHistoryOut_${jobName}.root root://${xrootdServer}///${outputDir}/createHistoryOut_${jobName}.root
exit=\$?
echo "copy done with exit \${exit}"
if [ "\$exit"  != "0" ]; then 
    touch ${launchDir}/logs/${taskId}/createHistoryjobs.copyerror
    echo "\`date\`: ${jobName}" >> ${launchDir}/logs/${taskId}/createHistoryjobs.copyerror
    exit \$exit
fi
touch ${launchDir}/logs/${taskId}/createHistoryjobs.success
echo "\`date\`: ${jobName}" >> ${launchDir}/logs/${taskId}/createHistoryjobs.success
EOF2

chmod a+x ${launchDir}/jobs/${taskId}/createHistory_${jobName}.sh
if [ "$queueX" = "localX" ]; then
echo "${launchDir}/jobs/${taskId}/createHistory_${jobName}.sh  > ${launchDir}/logs/${taskId}/createHistory_${jobName}.log 2>&1"
${launchDir}/jobs/${taskId}/createHistory_${jobName}.sh > ${launchDir}/logs/${taskId}/createHistory_${jobName}.log 2>&1 &
else
echo "bsub -q ${queue} -J${jobName} -o ${launchDir}/logs/${taskId}/createHistory_${jobName}.log < ${launchDir}/jobs/${taskId}/createHistory_${jobName}.sh"
bsub -q ${queue} -J${jobName} -o ${launchDir}/logs/${taskId}/createHistory_${jobName}.log < ${launchDir}/jobs/${taskId}/createHistory_${jobName}.sh
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
