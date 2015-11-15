#!/usr/bin/env python
# import ROOT in batch mode
import sys
import os
import json
from commands import getstatusoutput

oldargv = sys.argv[:]
sys.argv = [ '-b-' ]
from optparse import OptionParser
import ROOT
ROOT.gROOT.SetBatch(True)
sys.argv = oldargv

# load FWLite C++ librarie
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.gSystem.Load("libDataFormatsEcalDetId.so");
ROOT.gSystem.Load("libPhiSymEcalCalibDataFormats.so");
ROOT.AutoLibraryLoader.enable()

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events, Lumis

from FWCore.PythonUtilities.LumiList import LumiList

# Load TFile and TTree
from ROOT import TTree, TFile

# getting arrays
#from array import array
import numpy as n

# open file (you can use 'edmFileUtil -d /store/whatever.root' to get the physical file name)
#lumis = Lumis("file:/tmp/zwimpee/CMSSW_746-weights-74X_dataRun2_Prompt_v0-Run2015B_v3_test.root")

def resetInterval( interval , index ):
    interval["index"] = index
    interval["firstRun"] = 0
    interval["firstLumi"] = 0
    interval["unixTimeStart"] = 0
    interval["lastRun"] = 0
    interval["lastLumi"] = 0
    interval["unixTimeEnd"] = 0
    interval["nHit"] = 0
    interval["nLS"] =0
    interval["unixTimeMean"] = 0
    interval["flag"] = ""

def closeInterval( interval ):
    interval["unixTimeMean"]=interval["unixTimeStart"]+float(interval["unixTimeMean"])/float(interval["nHit"])

def startInterval( interval, run, lumi, start ):
    interval["firstRun"] = run
    interval["firstLumi"] = lumi
    interval["unixTimeStart"] = start

parser = OptionParser()
parser.add_option("", "--debug", dest="debug", action='store_true')
parser.add_option("", "--saveIsolatedIntervals", dest="saveIsolatedIntervals", action='store_true')
parser.add_option("-n", "--maxHit", dest="maxHit", type="int", default=3000000000)
parser.add_option("-f", "--fileList", dest="fileList", type="string", default="fileList.txt")
parser.add_option("-d", "--dataset", dest="dataset", type="string", default="")
parser.add_option("-o", "--output", dest="output", type="string", default="readMap.root")
parser.add_option("-p", "--prefix", dest="prefix", type="string", default="root://xrootd-cms.infn.it/")
parser.add_option("-t","--maxTime", dest="maxTime", type = "int", default=86400)
parser.add_option("-j","--jsonFile", dest="jsonFile", type = "string", default="default.json")
(options, args) = parser.parse_args()

if options.dataset != "":
    print "Getting files from DAS for dataset "+options.dataset
    if getstatusoutput("das_client.py --query='file dataset="+options.dataset+" instance=prod/phys03' --limit 0 | grep '/store/' >> /tmp/${USER}/filelist.dat"):
        options.fileList = "/tmp/"+os.environ['USER']+"/filelist.dat"

with open(options.fileList,'r') as textfile:
    files = [line.strip() for line in textfile]

fullpath_files = []

if options.debug:
    print "Reading files: "

for aline in files:
    fullpath_files.append( options.prefix+aline )
    if options.debug:
        print options.prefix+aline

lumis = Lumis(fullpath_files)

handlePhiSymInfo  = Handle ("std::vector<PhiSymInfo>")
labelPhiSymInfo = ("PhiSymProducer")

timeMap={}

lumiList = LumiList(os.path.expandvars(options.jsonFile))

for i,lumi in enumerate(lumis):
    lumi.getByLabel (labelPhiSymInfo,handlePhiSymInfo)
    phiSymInfo = handlePhiSymInfo.product()
    #skipping BAD lumiSections
    if not lumiList.contains(phiSymInfo.back().getStartLumi().run(),phiSymInfo.back().getStartLumi().luminosityBlock()):
        continue

    beginTime=lumi.luminosityBlockAuxiliary().beginTime().unixTime()
    timeMap[beginTime]={"run":phiSymInfo.back().getStartLumi().run(),"lumi":phiSymInfo.back().getStartLumi().luminosityBlock(),"totHitsEB":phiSymInfo.back().GetTotHitsEB()}

    if options.debug:
        print "====>"
        print "Run "+str(phiSymInfo.back().getStartLumi().run())+" Lumi "+str(phiSymInfo.back().getStartLumi().luminosityBlock())+" beginTime "+str(beginTime)
        print "NEvents in this LS "+str(phiSymInfo.back().GetNEvents())
        print "TotHits EB "+str(phiSymInfo.back().GetTotHitsEB())+" Avg occ EB "+str(float(phiSymInfo.back().GetTotHitsEB())/phiSymInfo.back().GetNEvents()) 
        print "TotHits EE "+str(phiSymInfo.back().GetTotHitsEE())+" Avg occ EE "+str(float(phiSymInfo.back().GetTotHitsEE())/phiSymInfo.back().GetNEvents()) 


nMaxHits=options.maxHit
maxStopTime=options.maxTime

interval={}

full_interval_count=0
isolated_interval_count=0

currentInterval={}
resetInterval( currentInterval , 0 )

# splitting logic
for key in sorted(timeMap):    
    if currentInterval["nLS"]==0 and currentInterval["unixTimeStart"]==0:
       #start a new interval
        startInterval( currentInterval, timeMap[key]["run"], timeMap[key]["lumi"], key)

    if key-currentInterval["unixTimeStart"]>=maxStopTime and currentInterval["unixTimeStart"] != 0:

        if currentInterval["nHit"] >= nMaxHits/2.:
            # Enough statistics. Closing previous interval 
            if options.debug:
                print "Closing interval by time condition"

            closeInterval( currentInterval )
            currentInterval["flag"]="S"
            interval[ currentInterval["unixTimeStart" ] ]=dict(currentInterval)
            full_interval_count+=1

        else:
            lastInterval=-1
            if len(interval.keys())>0:
                lastInterval=sorted(interval.keys())[-1]
            if lastInterval>0:
                if (currentInterval["unixTimeEnd"]-interval[lastInterval]["unixTimeStart"]<=maxStopTime):
                #merging with last interval
                    if options.debug:
                        print "Merging interval"
                    closeInterval( currentInterval )
                    interval[lastInterval]["lastRun"]=currentInterval["lastRun"]
                    interval[lastInterval]["lastLumi"]=currentInterval["lastLumi"]
                    interval[lastInterval]["unixTimeEnd"]=currentInterval["unixTimeEnd"]
                    interval[lastInterval]["unixTimeMean"]=(interval[lastInterval]["unixTimeMean"]*interval[lastInterval]["nHit"]+currentInterval["unixTimeMean"]*currentInterval["nHit"])/(float(interval[lastInterval]["nHit"]+currentInterval["nHit"]))
                    interval[lastInterval]["nHit"]+=currentInterval["nHit"]
                    interval[lastInterval]["nLS"]+=currentInterval["nLS"]
                    interval[lastInterval]["flag"]="M"
                else:
                    if options.saveIsolatedIntervals:
                        if options.debug:
                            print "Save short interval"
                        closeInterval( currentInterval )
                        currentInterval["flag"]="I"
                        interval[ currentInterval["unixTimeStart" ] ]=dict(currentInterval)
                        full_interval_count+=1
                    else:
                        if options.debug:
                            print "Dropping interval"
                        #dropping interval
                        isolated_interval_count+=1
            else:
                if options.debug:
                    print "First interval is a short one!"
                if options.saveIsolatedIntervals:
                    if options.debug:
                        print "Save short interval"
                    closeInterval( currentInterval )
                    currentInterval["flag"]="I"
                    interval[ currentInterval["unixTimeStart" ] ]=dict(currentInterval)
                    full_interval_count+=1
                else:
                    if options.debug:
                        print "Dropping interval"
                    #dropping interval
                    isolated_interval_count+=1

        # Start a new interval
        resetInterval( currentInterval, full_interval_count )
        startInterval( currentInterval, timeMap[key]["run"], timeMap[key]["lumi"], key)

    currentInterval["lastRun"] = timeMap[key]["run"]
    currentInterval["lastLumi"] = timeMap[key]["lumi"]
    currentInterval["unixTimeEnd"] = key+23.1
    currentInterval["nHit"] += timeMap[key]["totHitsEB"]
    currentInterval["nLS"] +=1
    currentInterval["unixTimeMean"] += float((key-currentInterval["unixTimeStart"]+11.55)*timeMap[key]["totHitsEB"])
    
    if currentInterval["nHit"] >= nMaxHits:
        # adding as new interval
        closeInterval( currentInterval )
        currentInterval["flag"]="F"
        interval[ currentInterval["unixTimeStart"] ]=dict(currentInterval)
        full_interval_count+=1
        # resetting for next interval
        resetInterval( currentInterval, full_interval_count )

interval_number=n.zeros(1,dtype=int)
hit=n.zeros(1,dtype=long)
flag=bytearray(2)
nLSBranch=n.zeros(1,dtype=int)
firstRunBranch=n.zeros(1,dtype=int)
lastRunBranch=n.zeros(1,dtype=int)
firstLumiBranch=n.zeros(1,dtype=int)
lastLumiBranch=n.zeros(1,dtype=int)
unixTimeStartBranch=n.zeros(1,dtype=float)
unixTimeEndBranch=n.zeros(1,dtype=float)
unixTimeMeanBranch=n.zeros(1,dtype=float)

outFile = ROOT.TFile(options.output, "RECREATE")
if not outFile:
    print "Cannot open outputFile "+options.output

tree = ROOT.TTree('outTree_barl', 'outTree_barl')
tree.Branch('index', interval_number, 'index/I')
tree.Branch('flag', flag, 'flag/C')
tree.Branch('nHit', hit, 'nHit/L')
tree.Branch('nLS', nLSBranch, 'nLS/I')
tree.Branch('firstRun', firstRunBranch, 'firstRun/I')
tree.Branch('lastRun', lastRunBranch, 'lastRun/I')
tree.Branch('firstLumi', firstLumiBranch, 'firstLumi/I')
tree.Branch('lastLumi', lastLumiBranch, 'lastLumi/I')
tree.Branch('unixTimeStart', unixTimeStartBranch, 'unixTimeStart/D')
tree.Branch('unixTimeEnd', unixTimeEndBranch, 'unixTimeEnd/D')
tree.Branch('unixTimeMean', unixTimeMeanBranch, 'unixTimeMean/D')

for key in sorted(interval):
    interval_number[0]=interval[key]["index"]
    flag[0]=interval[key]["flag"][0]
    hit[0]=interval[key]["nHit"]
    nLSBranch[0]=interval[key]["nLS"]
    firstRunBranch[0]=interval[key]["firstRun"]
    lastRunBranch[0]=interval[key]["lastRun"]
    firstLumiBranch[0]=interval[key]["firstLumi"]
    lastLumiBranch[0]=interval[key]["lastLumi"]
    unixTimeStartBranch[0]=interval[key]["unixTimeStart"]
    unixTimeEndBranch[0]=interval[key]["unixTimeEnd"]
    unixTimeMeanBranch[0]=interval[key]["unixTimeMean"]
    tree.Fill()

outFile.Write()
outFile.Close()

if options.debug:
    for key in sorted(interval):
        print "------------------"
        print "Index: " + str(interval[key]["index"])
        print "nHit: " + str(interval[key]["nHit"])
        print "nLS: " + str(interval[key]["nLS"])
        print "First Run: " + str(interval[key]["firstRun"])
        print "Last Run: " + str(interval[key]["lastRun"])
        print "First Lumi: " + str(interval[key]["firstLumi"])
        print "Last Lumi: " + str(interval[key]["lastLumi"])
        print "Unix Time Start: " + str(interval[key]["unixTimeStart"])
        print "Unix Time End: " + str(interval[key]["unixTimeEnd"])
        print "Unix Time Mean: " + str(interval[key]["unixTimeMean"])
        print "------------------"
                                  
print "====> FULL_INTERVALS:"+str(full_interval_count) + " ISOLATED INTERVALS:" + str(isolated_interval_count)

# erase tmp file
getstatusoutput("rm -fv /tmp/${USER}/filelist.dat")





