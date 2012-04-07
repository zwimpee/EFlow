#!/bin/env python
#
# take the output of
# 
#dbs search --url="http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet" --query="find run,file.numevents where dataset=/AlCaPhiSymEcal/Run2010A-StreamALCARECOEcalCalPhiSym-v4/ALCARECO" | sort  | sed '1,2d' | sed '$d' | sed '$d'
# and mark every 50M and 150M 
#

import sys

def main():


    if len(sys.argv) != 2 :
        print "Usage: groupruns.py <file>"
        sys.exit(1)
        
    try: 
        f=open(sys.argv[1])
    except IOError:
        print 'File not found : ',sys.argv[1] 
        
    sum1=0
    sum2=0

    firstnext=1
    lastrun=0 # to check that the split does not happen within a run
    
    for line in f:


        run= int(line.split()[0])
        ev = int(line.split()[1])
        sum1+=ev
        sum2+=ev

        if run == lastrun:
            lastrun=run
            continue

        if sum1==0 or sum2==0 or firstnext:
            print '%6d %10d %12d %12d'% (run, ev, sum1/1e6, sum2/1e6)
            firstnext=0
            
        if sum1> 20e6 :
            print '%6d %10d %12d %12d'% (run, ev, sum1/1e6, sum2/1e6)
            firstnext=1
            sum1=0
            
        if sum2>160e6 :
            print '%6d %10d %12d %12d'% (run, ev, sum1/1e6, sum2/1e6)
            firstnext=1
            sum2=0

        lastrun=run
        
if __name__ == "__main__":
    main()
