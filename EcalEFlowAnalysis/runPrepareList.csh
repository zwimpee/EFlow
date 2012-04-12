#!/bin/csh
# $Id: runPrepareList.csh,v 1.1 2012/04/11 16:10:24 meridian Exp $

if( $#argv<3  ) then
  echo "usage:  runPrepareList.csh  <list dir>  <directory> <location>   [run if 1] [N-1 diretory is dataset]"
  exit 0
endif

set prepareListCommand = prepareList.csh

set run = 0
set nMinusOne = 0
if( $#argv>2 ) then
  set run = $4
endif

if( $#argv>4 ) then
  set nMinusOne = $5
endif

set listdir = $1

set srmdir = "$2"

set location = $3

echo "Configuring list for $location"

if ( "$location" == "cern" ) then
    set lsCommand="rfdir"
else if ( "$location" == "xrootd" ) then
    set lsCommand="find"
else if ( "$location" == "eos" ) then
    set lsCommand="/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select find"
else if ( "$location" == "eth" ) then
    set lsCommand="lcg-ls"
endif 

echo "lsCommand is $lsCommand"

rm -rf ${listdir}/allFiles.txt
mkdir -p ${listdir}
touch ${listdir}/allFiles.txt

if ($location == "xrootd" ) then 
    ${lsCommand} "${srmdir}" -type f >> ${listdir}/allFiles.txt
else if ($location == "eos" ) then 
    ${lsCommand} -f "${srmdir}" >> ${listdir}/allFiles.txt
else if ($location == "cern") then 
    foreach dir (`${lsCommand} "${srmdir}" | awk '{print $9}'`)
	${lsCommand} "${srmdir}/${dir}" | awk '{print $9}' | xargs -I file echo ${srmdir}/${dir}/file >> ${listdir}/allFiles.txt
    end
else 
    ${lsCommand} "${srmdir}" | awk -F '/' '{print $NF}' | xargs -I {} ${lsCommand} "${srmdir}/{}" >> ${listdir}/allFiles.txt
endif

cd ${listdir}/
#if ( $location == "cern" ) then
#    foreach file (`ls *.txt`)
#	echo $file >> allFiles.txt
#	rm -rf $file
#    end
#endif


if ($location == "xrootd") then    
    ${lsCommand} "${srmdir}" -type d | awk -F '/' '{print $NF}' | xargs -I {} ../${prepareListCommand} allFiles.txt {}  ${location} ${run} >! makeLists.log
else if ($location == "eos") then    
    set mnf=`${lsCommand} -d "${srmdir}" | sed 's%/$%%g' | awk 'BEGIN{FS="/"; MAXNF=0} {if (NF>=MAXNF) {  MAXNF=NF} } END{print MAXNF}'`
    if ( $nMinusOne == 0 ) then
	${lsCommand} -d "${srmdir}" | sed 's%/$%%g' | awk -F'/' '{if (NF>=maxn) print $NF}' maxn=${mnf}| xargs -I {} ../${prepareListCommand} allFiles.txt {}  ${location} ${run} >! makeLists.log
    else
	${lsCommand} -d "${srmdir}" | sed 's%/$%%g' | awk -F'/' '{if (NF>=maxn) {i=NF-1; print $i} }' maxn=${mnf}| xargs -I {} ../${prepareListCommand} allFiles.txt {}  ${location} ${run} >! makeLists.log
else if ($location == "cern") then 
    ${lsCommand} "${srmdir}" | awk '{print $9}' | xargs -I {} ../${prepareListCommand} allFiles.txt {}  ${location} ${run} >! makeLists.log
else 
    ${lsCommand} "${srmdir}" | awk -F '/' '{print $NF}' | xargs -I {} ../${prepareListCommand} allFiles.txt {} ${location} ${run} >! makeLists.log
endif

if( $run != 1 ) then
  rm -rf ${listdir}
endif
