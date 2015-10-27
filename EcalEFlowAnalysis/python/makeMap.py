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


parser = OptionParser()
parser.add_option("-H", "--MaxHit", dest="maxHit", type="int", default=40000000)
parser.add_option("-T","--MaxTime", dest="maxTime", type = "int", default=43200)
(options, args) = parser.parse_args()



with open('/afs/cern.ch/user/z/zwimpee/work/CMSSW7413/src/PhiSym/EcalCalibAlgos/test/fileList.txt','r') as textfile:
    files = [line.strip() for line in textfile]




fullpath_files = []

prefix="file:"

for aline in files:
    fullpath_files.append( prefix+aline )

lumis = Lumis(fullpath_files)
#lumis = Lumis("root://xrootd-cms.infn.it//store/user/spigazzi/AlCaPhiSym/crab_PHISYM-CMSSW_741-weights-GR_P_V56-Run2015B_v1/150714_150558/0000/phisym_weights_1lumis_13.root")

handlePhiSymInfo  = Handle ("std::vector<PhiSymInfo>")
handlePhiSymRecHitsEB  = Handle ("std::vector<PhiSymRecHit>")
handlePhiSymRecHitsEE  = Handle ("std::vector<PhiSymRecHit>")
labelPhiSymInfo = ("PhiSymProducer")
labelPhiSymRecHitsEB = ("PhiSymProducer","EB")
labelPhiSymRecHitsEE = ("PhiSymProducer","EE")

timeMap={}
for i,lumi in enumerate(lumis):
#    print "====>"
    lumi.getByLabel (labelPhiSymInfo,handlePhiSymInfo)
    phiSymInfo = handlePhiSymInfo.product()
    beginTime=lumi.luminosityBlockAuxiliary().beginTime().unixTime()
    timeMap[beginTime]={"run":phiSymInfo.back().getStartLumi().run(),"lumi":phiSymInfo.back().getStartLumi().luminosityBlock(),"totHitsEB":phiSymInfo.back().GetTotHitsEB()}
#    print "Run "+str(phiSymInfo.back().getStartLumi().run())+" Lumi "+str(phiSymInfo.back().getStartLumi().luminosityBlock())+" beginTime "+str(beginTime)
#    print "NEvents in this LS "+str(phiSymInfo.back().GetNEvents())
#    print "TotHits EB "+str(phiSymInfo.back().GetTotHitsEB())+" Avg occ EB "+str(float(phiSymInfo.back().GetTotHitsEB())/phiSymInfo.back().GetNEvents()) 
#    print "TotHits EE "+str(phiSymInfo.back().GetTotHitsEE())+" Avg occ EE "+str(float(phiSymInfo.back().GetTotHitsEE())/phiSymInfo.back().GetNEvents()) 


#nMaxHits=40000000
nMaxHits=options.maxHit
#maxStopTime=3600*12
maxStopTime=options.maxTime

interval={}

interval_count=0
int_hit_count=0
nLS=0
prevnLS=0
firstRun=0
lastRun=0
firstLumi=0
lastLumi=0
int_time_count=0
prevInterTime=0
prevInthits=0
loopStart=0
loopStartTime=0

#print timeMap
for key in sorted(timeMap):
    if loopStart==0:
        loopStartTime=key
        loopStart+=1
    int_hit_count += timeMap[key]["totHitsEB"]
    nLS+=1
    if int_hit_count >= nMaxHits and interval_count == 0:
       # first interval
        firstRun=timeMap[loopStartTime]["run"]
        lastRun=timeMap[key]["run"]
        firstLumi=timeMap[loopStartTime]["lumi"]
        lastLumi=timeMap[key]["lumi"]
        unixTimeMean=((firstLumi+(nLS *11.05))*timeMap[key]["totHitsEB"])/int_hit_count
        interval[key]={"index": interval_count, "nHit": int_hit_count, "nLS": nLS, "firstRun": firstRun, \
                       "lastRun": lastRun, "firstLumi": firstLumi, "lastLumi": lastLumi, \
                       "unixTimeStart": loopStartTime, "unixTimeEnd": key, "unixTimeMean": unixTimeMean }
        # preparing for next interval
        prevInthits=int_hit_count
        prevnLS=nLS
        nLS=0
        int_hit_count = 0
        interval_count+=1
        prevInterTime=key
    if int_hit_count >= nMaxHits:
       # adding as new interval
        firstRun=timeMap[prevInterTime]["run"]
        lastRun=timeMap[key]["run"]
        firstLumi=timeMap[prevInterTime]["lumi"]
        lastLumi=timeMap[key]["lumi"]
        unixTimeMean=((firstLumi+(nLS *11.05))*timeMap[key]["totHitsEB"])/int_hit_count
        interval[key]={"index": interval_count, "nHit": int_hit_count, "nLS": nLS, "firstRun": firstRun, \
                       "lastRun": lastRun, "firstLumi": firstLumi, "lastLumi": lastLumi, \
                       "unixTimeStart": prevInterTime, "unixTimeEnd": key, "unixTimeMean": unixTimeMean } 
        # preparing for next interval
        prevInthits=int_hit_count
        prevnLS=nLS
        nLS = 0
        int_hit_count = 0
        interval_count+=1
        prevInterTime=key
    elif key-prevInterTime>=maxStopTime and prevInterTime != 0:
        if int_hit_count >= nMaxHits/2:
            # enough statistics for new interval
            firstRun=timeMap[prevInterTime]["run"]
            lastRun=timeMap[key]["run"]
            firstLumi=timeMap[prevInterTime]["lumi"]
            lastLumi=timeMap[key]["lumi"]
            unixTimeMean=((firstLumi+(nLS *11.05))*timeMap[key]["totHitsEB"])/int_hit_count
            interval[key]={"index": interval_count, "nHit": int_hit_count, "nLS": nLS, "firstRun": firstRun, \
                       "lastRun": lastRun, "firstLumi": firstLumi, "lastLumi": lastLumi, \
                       "unixTimeStart": prevInterTime, "unixTimeEnd": key, "unixTimeMean": unixTimeMean }
            # preparing for next interval
            prevInthits=int_hit_count
            prevnLS=nLS
            nLS = 0
            int_hit_count = 0
            interval_count+=1
            prevInterTime=key
        elif int_hit_count <= nMaxHits/2 and key-prevInterTime<=maxStopTime/2:
            # adding to previous interval
            firstRun=timeMap[prevInterTime]["run"]
            lastRun=timeMap[key]["run"]
            firstLumi=timeMap[prevInterTime]["lumi"]
            lastLumi=timeMap[key]["lumi"]
            unixTimeMean=((firstLumi+(nLS *11.05))*timeMap[key]["totHitsEB"])/int_hit_count
            thisIntHits=prevInthits+int_hit_count
            interval[prevInterTime]={"index": interval_count, "nHit": thisIntHits , "nLS": nLS, "firstRun": firstRun, \
                       "lastRun": lastRun, "firstLumi": firstLumi, "lastLumi": lastLumi, \
                       "unixTimeStart": prevInterTime, "unixTimeEnd": key, "unixTimeMean": unixTimeMean }
            prevInthits+=int_hit_count
            int_hit_count=0
            prevInterTime=key
        else:
            # discarding this interval and starting new one here
            nLS = 0
            int_hit_count = 0


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

outFile = ROOT.TFile("makeMap.root", "RECREATE")
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
                                  

   





