#!/bin/sh
rm -rf /tmp/atoy_* 
rm -rf toysKfactor_seed_*

echo "$randstring"
for i in `seq 1 4`; do
    echo $i
    str0="$$"
    POS=2  # Starting from position 2 in the string.
    LEN=8  # Extract eight characters.
    str1=$( echo "$str0$i" | md5sum | md5sum )
    randstring="${str1:$POS:$LEN}"
    cat toyKfactor.C | sed -e "s%TOYNAME%seed_${randstring}%g" > /tmp/atoy_${randstring}.C
    root -l -b -q /tmp/atoy_${randstring}.C > /dev/null &
done
#rm -rf /tmp/atoy*.C
