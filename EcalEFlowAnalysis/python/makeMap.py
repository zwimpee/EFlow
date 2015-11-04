#!/usr/bin/env python
# import ROOT in batch mode
import sys
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

def closeInterval( interval ):
    interval["unixTimeMean"]=interval["unixTimeStart"]+float(interval["unixTimeMean"])/float(interval["nHit"])

def startInterval( interval, run, lumi, start ):
    interval["firstRun"] = run
    interval["firstLumi"] = lumi
    interval["unixTimeStart"] = start

parser = OptionParser()
parser.add_option("-d", "--debug", dest="debug", action='store_true')
parser.add_option("-n", "--maxHit", dest="maxHit", type="int", default=40000000)
parser.add_option("-f", "--fileList", dest="fileList", type="string", default="fileList.txt")
parser.add_option("-o", "--output", dest="output", type="string", default="readMap.root")
parser.add_option("-p", "--prefix", dest="prefix", type="string", default="file:")
parser.add_option("-t","--maxTime", dest="maxTime", type = "int", default=43200)
(options, args) = parser.parse_args()

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
#lumis = Lumis("root://xrootd-cms.infn.it//store/user/spigazzi/AlCaPhiSym/crab_PHISYM-CMSSW_741-weights-GR_P_V56-Run2015B_v1/150714_150558/0000/phisym_weights_1lumis_13.root")

handlePhiSymInfo  = Handle ("std::vector<PhiSymInfo>")
#handlePhiSymRecHitsEB  = Handle ("std::vector<PhiSymRecHit>")
#handlePhiSymRecHitsEE  = Handle ("std::vector<PhiSymRecHit>")
labelPhiSymInfo = ("PhiSymProducer")
#labelPhiSymRecHitsEB = ("PhiSymProducer","EB")
#labelPhiSymRecHitsEE = ("PhiSymProducer","EE")

timeMap={}
for i,lumi in enumerate(lumis):
    lumi.getByLabel (labelPhiSymInfo,handlePhiSymInfo)
    phiSymInfo = handlePhiSymInfo.product()
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
#print timeMap

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
            interval[ currentInterval["unixTimeStart" ] ]=dict(currentInterval)
            full_interval_count+=1

        else:
            #dropping interval
            if options.debug:
                print "Dropping interval"
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
        interval[ currentInterval["unixTimeStart"] ]=dict(currentInterval)
        full_interval_count+=1
        # resetting for next interval
        resetInterval( currentInterval, full_interval_count )

interval_number=n.zeros(1,dtype=int)
hit=n.zeros(1,dtype=int)
nLSBranch=n.zeros(1,dtype=int)
firstRunBranch=n.zeros(1,dtype=int)
lastRunBranch=n.zeros(1,dtype=int)
firstLumiBranch=n.zeros(1,dtype=int)
lastLumiBranch=n.zeros(1,dtype=int)
unixTimeStartBranch=n.zeros(1,dtype=int)
unixTimeEndBranch=n.zeros(1,dtype=int)
unixTimeMeanBranch=n.zeros(1,dtype=int)

outFile = ROOT.TFile(options.output, "RECREATE")
tree = ROOT.TTree('outTree_barl', 'outTree_barl')
tree.Branch('index', interval_number, 'index/I')
tree.Branch('nHit', hit, 'nHit/I')
tree.Branch('nLs', nLSBranch, 'nLS/I')
tree.Branch('firstRun', firstRunBranch, 'firstRun/I')
tree.Branch('lastRun', lastRunBranch, 'lastRun/I')
tree.Branch('firstLumi', firstLumiBranch, 'firstLumi/I')
tree.Branch('lastLumi', lastLumiBranch, 'lastLumi/I')
tree.Branch('unixTimeStart', unixTimeStartBranch, 'unixTimeStart/I')
tree.Branch('unixTimeEnd', unixTimeEndBranch, 'unixTimeEnd/I')
tree.Branch('unixTimeMean', unixTimeMeanBranch, 'unixTimeMean/I')

for key in sorted(interval):
    interval_number[0]=interval[key]["index"]
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

   





