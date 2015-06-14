#!/bin/csh
# $Id: runPrepareList.csh,v 1.4 2012/10/25 12:27:16 meridian Exp $

if( $#argv<4  ) then
  echo "usage:  runPrepareList.csh  <list dir>  <directory> <location> <tag>  [run if 1] [N-1 diretory is dataset]"
  exit 0
endif

set prepareListCommand = scripts/prepareList.csh
set dasCommand = python/dataset_info.py

set run = 0
set nMinusOne = 0
#set incremental = NO 

set listdir = $1
echo "===> Output list will be created in $listdir"
set fullDataset = "$2"
echo "===> Input dataset $fullDataset"
set dataset = $3
echo "===> Dataset short name $dataset"
set tag = $4
echo "===> Ntuple tag $tag"

if( $#argv>4 ) then
  set run = $5
endif

rm -rf ${listdir}/allFiles.txt
mkdir -p ${listdir}

#touch ${listdir}/allFiles.txt
#cd ${listdir}/

${dasCommand} -d ${fullDataset} | sort -n >! ${listdir}/allFiles.txt
touch ${listdir}/makeLists.log
${prepareListCommand} ${listdir}/allFiles.txt ${dataset} t2roma ${run} >> ${listdir}/makeLists.log 

if( $run != 1 ) then
  rm -rf ${listdir}
endif
