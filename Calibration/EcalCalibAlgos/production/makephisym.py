#!/usr/bin/env python

#
# Prepare configuration for a phi symmetry calibration
#
# Author: Stefano Argiro
#
#
# Usage:
#
# makephisym.py --mode=[caf|grid] --dataset=[datasetname]
#               --runrange=[first-last] --globaltag=[tag]
#
#
#
# Files: phisym-cfg.py.tmpl phisym-crab-cfg.tmpl 
#

import string, sys, os, getopt, subprocess, time, shutil, random
from datetime import datetime, date, time



def usage():
    print "Usage: makephisym.py --mode=[caf|grid] --dataset=[datasetname]"
    print "       --runrange=[first-last] --globaltag=[tag] [--step2only] [--runstep2] --maxendtime=[%Y-%m-%dT%H:%M:%S]"
    print "       --group=[Collisions10,Collisions11,Collisions12] "
    
try:
     opts, args = getopt.getopt(sys.argv[1:], "m:d:r:g:so:n:e:", ["mode=","dataset=","runrange=","globaltag=","step2only","group=","runstep2","maxendtime="])

except getopt.GetoptError:
     #* print help information and exit:*
     usage()
     sys.exit(2)


mode=''
dataset=''
group=''
firstrun=0
lastrun=0
globaltag=''
step2only=0
runstep2=0
maxtime=''

cmssw_py_template=   'phisym-cfg.py.tmpl'
cmssw_py_step2_template= 'phisym_step2.py.tmpl'
crab_cfg_template=   'phisym-cfg.crab.tmpl'
runreg_cfg_template= 'runreg.cfg.tmpl'


for opt, arg in opts:
    
     if opt in ("-m", "--mode"):
        mode = arg
        if mode != 'caf' and mode != 'glite' :
           print sys.argv[0]+" mode must be caf or glite"
           sys.exit(2)
            
     if opt in ("-d","--dataset"):
      dataset= arg        
 

     if opt in ("-r","--runrange"):
         range=arg.split('-')
         firstrun=int(range[0])
         lastrun =int(range[1])

     if opt in ("-g","--globaltag"):
         globaltag=arg

     if opt in ("-s","--step2only"):
         step2only=1
         
     if opt in ("-n","--runstep2"):
         runstep2=1

     if opt in ("-o","--group"):
         group=arg

     if opt in ("-e","--maxendtime"):
         maxtime=arg

if firstrun==0 or lastrun==0:
    usage()
    print "Run range must be like 144536-145789"
    sys.exit(2)

if maxtime!='':
    endtime=datetime.strptime(maxtime,"%Y-%m-%dT%H:%M:%S")
else:
    endtime=datetime(2099,01,01,00,00,00)
    
if dataset=='':
    usage()
    print "Please specify dataset"
    sys.exit(2)

if mode=='':    
    usage()
    print "Please specify mode caf or glite"
    sys.exit(2)

if group=='':
    usage()
    print "Please specify group, e.g. Collisions11"
    sys.exit(2)
        
print "Querying DBS ..."

if (runstep2):
    query='dbs search --url=\"http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet\" --query=\"find file where dataset = '+dataset+' and run ='+repr(firstrun)+'\"'
    
    queryp=subprocess.Popen(query,stdout=subprocess.PIPE,shell=True)
    queryp.wait()
    res=queryp.stdout
    lines=res.readlines()

    step2file=lines[len(lines)-1]
    step2file=step2file[:len(step2file)-1] # this ugly trick to remove trailing /n
        
        
    if step2file.find(".root") is -1 :
        print "Error, dbs search gave no files"
        sys.exit(1)

friendly_datasetname=dataset.replace('/','_')
random_string = ''.join(random.choice(string.letters) for i in xrange(3))
dirname=friendly_datasetname[1:]+'_'+repr(firstrun)+'_'+repr(lastrun)+'_'+random_string

f = open(crab_cfg_template)
data=f.read()
data=data.replace('MODE',mode)
data=data.replace('DATASET',dataset)
data=data.replace('OUTPUTDIR',dirname)
outfile = open('phisym-cfg.crab.cfg',"w")
outfile.write(data)
outfile.close()

if (runstep2):
    f = open(cmssw_py_step2_template)
    data=f.read()
    data=data.replace('GLOBALTAG',globaltag)
    data=data.replace('STEP2FILES',step2file)
    outfile = open('phisym_step2.py',"w")
    outfile.write(data)
    outfile.close()


f = open(cmssw_py_template)
data=f.read()
data=data.replace('GLOBALTAG',globaltag)
outfile = open('phisym-cfg.py',"w")
outfile.write(data)
outfile.close()

f = open(runreg_cfg_template)
data=f.read()
data=data.replace('FIRSTRUN',repr(firstrun))
data=data.replace('LASTRUN',repr(lastrun))
data=data.replace('LASTENDTIME',endtime.isoformat())
data=data.replace('GROUP',group)
outfile = open('runreg.cfg',"w")
outfile.write(data)
outfile.close()

if not step2only:
   print "Querying run registry ..."
   runreg_stdout=open('runreg.log','w')
   #rregargs=['']
   #ret =subprocess.Popen(rregargs,0,"./runregparse.py",stdout=runreg_stdout)
   rc=subprocess.check_call(['./runregparse.py'],stdout=runreg_stdout)

   #rc=cmsRun.returncode
   if  rc != 0  :
       print "Errors invoking dbs"
       sys.exit(1)

   print "done"



if not step2only:
   try :
      os.mkdir(dirname)
   except Exception:
      print "Error creating work directory" 
      sys.exit(1)

else:
   if not os.path.exists(dirname):
       print "Dir not found: ",dirname
       print "It appears step1 was not run"
       sys.exit(1)
       
if not step2only:   
   shutil.move('jsonls-alcaphisym.txt',dirname)
   shutil.move('phisym-cfg.py',dirname)
   shutil.move('phisym-cfg.crab.cfg',dirname)

if (runstep2):
    os.rename('phisym_step2.py',dirname+'/phisym_step2.py')
