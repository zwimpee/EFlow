#!/bin/csh
# $Id: runPrepareList.csh,v 1.2 2012/04/12 09:41:41 meridian Exp $

if( $#argv<4  ) then
  echo "usage:  runPrepareList.csh  <list dir>  <directory> <location> <tag>  [run if 1] [N-1 diretory is dataset]"
  exit 0
endif

set prepareListCommand = prepareList.csh

set run = 0
set nMinusOne = 0
#set incremental = NO 

set listdir = $1
echo $listdir
set srmdir = "$2"
echo $srmdir
set location = $3
echo $location
set tag = $4
echo $tag

if( $#argv>4 ) then
  set run = $5
endif

if( $#argv>5 ) then
  set nMinusOne = $6
endif

#if( $#argv>6 ) then
#  set nMinusOne = $7
#endif

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
    ${lsCommand} "${srmdir}" -type f | grep ${tag} >> ${listdir}/allFiles.txt
else if ($location == "eos" ) then 
    ${lsCommand} -f "${srmdir}" | grep ${tag}  >> ${listdir}/allFiles.txt
else if ($location == "cern") then 
    foreach dir (`${lsCommand} "${srmdir}" | awk '{print $9}'`)
	${lsCommand} "${srmdir}/${dir}" | awk '{print $9}' |  grep ${tag} | xargs -I file echo ${srmdir}/${dir}/file >> ${listdir}/allFiles.txt
    end
else 
    ${lsCommand} "${srmdir}" | awk -F '/' '{print $NF}' |  grep ${tag} | xargs -I {} ${lsCommand} "${srmdir}/{}" >> ${listdir}/allFiles.txt
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
	${lsCommand} -d "${srmdir}" | sed 's%/$%%g' | awk -F'/' '{if (NF>=maxn) {i=NF-'${nMinusOne}'; print $i} }' maxn=${mnf}| xargs -I {} ../${prepareListCommand} allFiles.txt {}  ${location} ${run} >! makeLists.log
else if ($location == "cern") then 
    ${lsCommand} "${srmdir}" | awk '{print $9}' | xargs -I {} ../${prepareListCommand} allFiles.txt {}  ${location} ${run} >! makeLists.log
else 
    ${lsCommand} "${srmdir}" | awk -F '/' '{print $NF}' | xargs -I {} ../${prepareListCommand} allFiles.txt {} ${location} ${run} >! makeLists.log
endif

if( $run != 1 ) then
  rm -rf ${listdir}
endif
