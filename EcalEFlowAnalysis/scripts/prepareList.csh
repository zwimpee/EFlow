#!/bin/csh
# $Id: prepareList.csh,v 1.6 2012/10/25 12:27:16 meridian Exp $

if( $#argv<3  ) then
  echo "usage:  prepareList.csh  <inputfile> <listname> <location>  [run if 1]"
  exit 0
endif

set run = 0
if( $#argv>2 ) then
  set run = $4
endif

set infile = $1

set listname = $2

set location = $3

# num of files per list file
set filexlist  = 50

set prepend=""

if ( $location == "cern" ) then
  set prepend=""
else if ( $location == "xrootd" ) then
  set prepend="root://pccmsrm27.cern.ch:1094/"
else if ( $location == "eos" ) then
  set prepend="root://eoscms//"
else if ( $location == "t2roma" ) then
  set prepend="root://xrootd-cms.infn.it:1194/"
else if ( $location == "eth" ) then
  set prepend="dcap://t3se01.psi.ch/"
endif 

#set prepend="dcap://cmsrm-se01.roma1.infn.it"

 
#set files = ( `/bin/ls -1 $indir` )
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "Working on dataset $listname" 
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

#if ($location != "xrootd" ) then
#    set files = ( `cat $infile` )
#else
#    set files = ( `cat $infile | grep $listname | sed -e "s%/xrootdfs%%g"` )
#endif
#
set dir = `dirname ${infile}`

set tmpfile = /tmp/tmpfilelist.${listname}
rm -Rf $tmpfile
touch $tmpfile

#cat $infile >! ${tmpfile}
cat $infile | awk '\!match($0,/^#/){printf "'${prepend}'%s\n",$0}' >! ${tmpfile}
set allfiles=`wc -l ${tmpfile} | awk '{print $1}'`

set unifile =  /tmp/uniq.${listname}.files
# Removing duplicates and using the last file (according to order in the inputfile) for a given jobId
#awk 'BEGIN{FS="PHISYM_Run2015A_v0_test_"}{ split($2,jobId,"_"); sum[jobId[1]]++;line[jobId[1]]=$0}END{for(i in sum) print line[i];}' ${tmpfile} | sort -n >! ${unifile}.tmp
cat ${tmpfile} | uniq >! ${unifile}.tmp 
 
#rm -Rf  ${unifile}.tmp
#sort $tmpfile | uniq -w 4 | awk -F: '{print $2":"$3}' > ${unifile}.tmp

set suffixlen = 3

split -l $filexlist -d -a $suffixlen  ${unifile}.tmp  ${dir}/${listname}_

#echo split -l $filexlist -d -a $suffixlen  ${unifile}.tmp  ${dir}/${listname}_

set uniqfiles=`wc -l ${unifile}.tmp | awk '{print $1}'`

echo "# of root files in directory: ${allfiles}"
echo "# of uniq files in directory: ${uniqfiles}"
echo "# of files per list: $filexlist"

foreach i ( ${dir}/${listname}_??? )
    set file = `basename $i`
    mv ${dir}/${file} ${dir}/filelist_${file}.txt
  echo "new list:   ${dir}/filelist_${i}.txt"
end

if( $run != 1 ) then
  rm -rf ${listname}_*.list
endif


#rm -Rf $tmpfile

