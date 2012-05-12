#!/bin/sh

if [ -f logs/$1/$2jobs.runerror ] ; then
echo "Relaunching runerror jobs"
cat logs/$1/$2jobs.runerror
cat logs/$1/$2jobs.runerror | awk '{printf "bsub -q 1nd -o logs/'$1'/$2_%s.log -J %s < jobs/'$1'/$2_%s.sh\n",$7,$7,$7}' | /bin/sh
mv -f logs/$1/$2jobs.runerror  logs/$1/$2jobs.runerror.old
fi

if [ -f logs/$1/$2jobs.runerror ] ; then 
echo "Relaunching copyerror jobs"
cat logs/$1/$2jobs.copyerror
cat logs/$1/$2jobs.copyerror | awk '{printf "bsub -q 1nd -o logs/'$1'/$2_%s.log -J %s < jobs/'$1'/$2_%s.sh\n",$7,$7,$7}' | /bin/sh
mv -f logs/$1/$2jobs.copyerror logs/$1/$2jobs.copyerror.old
fi