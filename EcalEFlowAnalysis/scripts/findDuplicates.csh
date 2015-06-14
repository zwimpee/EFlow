#!/bin/csh
#

if( $#argv < 2 ) then
  echo "usage: ./findDuplicates.csh  <directory containing crab output files> <name of the output with unique files>"
  exit 0
endif

set indir = $1 

set files = ( `/bin/ls -1 $indir` )
#set files = ( `nsls $indir` )

set tmpfile = /tmp/allfiles.crab
set unifile =  /tmp/uniq.files
set dupfile = /tmp/duplicates.files
rm -Rf $tmpfile
touch  $tmpfile
rm -Rf ${unifile} ${unifile}.tmp ${dupfile}
touch  ${unifile}

foreach i ( $files )
  echo $i | awk 'BEGIN{FS="_"}{ file = $0; if($2<10) j="000"$2;else if($2<100) j = "00"$2;else if($2<1000) j = "0"$2;else j=$2;  print j":"file }' >> $tmpfile
end

rm -Rf ${dupfile}
touch  ${dupfile}

sort $tmpfile | uniq -w 4 | awk -F: '{print $2}' > ${unifile}.tmp
sort $tmpfile | uniq -d -w 4 | awk -F: '{print $2}' > ${dupfile}

foreach i ( `cat ${unifile}.tmp`  ) 
  echo $indir/$i >> ${unifile}
end

wc -l  $tmpfile $unifile $dupfile
cp ${unifile} $2
