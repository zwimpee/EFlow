#!/bin/bash

#
# merge etsums
#

#ids to merge
tomerge="50 51 52 53 54 55 56 57 58 59"
n=0

touch etsum_barl.dat
touch etsum_endc.dat

for i  in $tomerge  ; do
  file="CMSSW_$i.stdout"
  let "n= $n + `grep processed  $file | awk '{print $3}' `"

  cat  etsum_barl_1_$i\_1_???.dat >>  etsum_barl.dat
  cat  etsum_endc_1_$i\_1_???.dat >>  etsum_endc.dat

done

echo $n
