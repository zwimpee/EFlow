#!/bin/sh

if [ -f logs/$1/createHistoryjobs.runerror ] ; then
echo "Relaunching runerror jobs"
cat logs/$1/createHistoryjobs.runerror
cat logs/$1/createHistoryjobs.runerror | awk '{printf "bsub -q 1nd -o logs/'$1'/createHistoryOutjob_%s.log -J %s < jobs/'$1'/createHistoryOut_%s.sh\n",$7,$7,$7}' | /bin/sh
mv -f logs/$1/createHistoryjobs.runerror  logs/$1/createHistoryjobs.runerror.old
fi

if [ -f logs/$1/createHistoryjobs.runerror ] ; then 
echo "Relaunching copyerror jobs"
cat logs/$1/createHistoryjobs.copyerror
cat logs/$1/createHistoryjobs.copyerror | awk '{printf "bsub -q 1nd -o logs/'$1'/createHistoryOutjob_%s.log -J %s < jobs/'$1'/createHistoryOut_%s.sh\n",$7,$7,$7}' | /bin/sh
mv -f logs/$1/createHistoryjobs.copyerror logs/$1/createHistoryjobs.copyerror.old
fi