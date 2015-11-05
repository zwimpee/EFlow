#!/usr/bin/env python
# import ROOT in batch mode
import sys
import os
import json

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

from intervalMap import *

def resetBeamSpotInfo(beamSpotInfo):
    beamSpotInfo['nEvents']=0
    beamSpotInfo['nHitsEB']=0
    beamSpotInfo['nHitsEE']=0
    beamSpotInfo['bsPos']=0
    beamSpotInfo['bsPosWid']=0


def resetXtalSumInfo(xtalSumInfo):
    xtalSumInfo['nHits']=0
    xtalSumInfo['det']=0
    xtalSumInfo['ieta']=0
    xtalSumInfo['iphi']=0
    xtalSumInfo['sign']=0
    xtalSumInfo['energySum']=0
    xtalSumInfo['energySumSquared']=0
    xtalSumInfo['lcSum']=0
    xtalSumInfo['lcSumSquared']=0

# open file (you can use 'edmFileUtil -d /store/whatever.root' to get the physical file name)
#lumis = Lumis("file:/tmp/zwimpee/CMSSW_746-weights-74X_dataRun2_Prompt_v0-Run2015B_v3_test.root")

parser = OptionParser()
parser.add_option("-d", "--debug", dest="debug", action='store_true')
parser.add_option("-f", "--fileList", dest="fileList", type="string", default="fileList.txt")
parser.add_option("-o", "--output", dest="output", type="string", default="readMap.root")
parser.add_option("-p", "--prefix", dest="prefix", type="string", default="file:")
parser.add_option("-j", "--jsonFile", dest="jsonFile", type = "string", default="default.json")
parser.add_option("-m", "--mapFile", dest="mapFile", type = "string", default="defaultMap.root")
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
handlePhiSymRecHitsEB  = Handle ("std::vector<PhiSymRecHit>")
handlePhiSymRecHitsEE  = Handle ("std::vector<PhiSymRecHit>")
labelPhiSymInfo = ("PhiSymProducer")
labelPhiSymRecHitsEB = ("PhiSymProducer","EB")
labelPhiSymRecHitsEE = ("PhiSymProducer","EE")

lumiList = LumiList(os.path.expandvars(options.jsonFile))
intervals = intervalMap(os.path.expandvars(options.mapFile))

beamSpotInfos=[]
ebSums=[]
eeSums=[]

beamSpotInfo={}
resetBeamSpotInfo(beamSpotInfo)
xtalSumInfo={}
resetXtalSumInfo(xtalSumInfo)

for key,value in intervals.intervals.iteritems():
    #initialize beamSpotInfo for interval
    beamSpotInfos.append(dict(beamSpotInfo))
    #initialize EBsum for interval
    ebSums.append(list())
    for ixtal in range(0,ROOT.EBDetId.kSizeForDenseIndexing):
        detId=ROOT.EBDetId.detIdFromDenseIndex(ixtal)
        ebSums[key].append(dict(xtalSumInfo))
        ebSums[key][ixtal]['det'] = 0 #EB => 0 EE=> 1
        ebSums[key][ixtal]['ieta'] = detId.ietaAbs() #ieta = ieta in EB, ix in EE
        ebSums[key][ixtal]['iphi'] = detId.iphi() #iphi = iphi in EB, iy in EE
        ebSums[key][ixtal]['sign'] = detId.zside()>0 #- => 0  + => 1   

    #initialize EESum for interval
    eeSums.append(list())
    for ixtal in range(0,ROOT.EEDetId.kSizeForDenseIndexing):
        detId=ROOT.EEDetId.detIdFromDenseIndex(ixtal)
        eeSums[key].append(dict(xtalSumInfo))
        eeSums[key][ixtal]['det'] = 1
        eeSums[key][ixtal]['ieta'] = detId.ix()
        eeSums[key][ixtal]['iphi'] = detId.iy()
        eeSums[key][ixtal]['sign'] = detId.zside()>0

for i,lumi in enumerate(lumis):
    lumi.getByLabel (labelPhiSymInfo,handlePhiSymInfo)
    lumi.getByLabel (labelPhiSymRecHitsEB,handlePhiSymRecHitsEB)
    lumi.getByLabel (labelPhiSymRecHitsEE,handlePhiSymRecHitsEE)
    phiSymInfo = handlePhiSymInfo.product()
    phiSymRecHitsEB = handlePhiSymRecHitsEB.product()
    phiSymRecHitsEE = handlePhiSymRecHitsEE.product()

    run=phiSymInfo.back().getStartLumi().run()
    lumi=phiSymInfo.back().getStartLumi().luminosityBlock()

    #skipping BAD lumiSections
    if not lumiList.contains(run,lumi):
        continue

    #identify which interval run,lumi belongs
    interval=intervals.intervalIndex(run,lumi)
    if interval<0 and interval>(intervals.size()-1):
        if options.debug:
                    print "RUN "+str(run)+" LUMI "+str(lumi)+" INTERVAL NOT FOUND"
        continue
    
    if options.debug:
        print "RUN "+str(run)+" LUMI "+str(lumi)+" INTERVAL "+str(interval) 

    #Fill Beam spot information tree
    beamSpotInfos[interval]['nEvents'] += phiSymInfo.back().GetNEvents()
    beamSpotInfos[interval]['nHitsEB'] += phiSymInfo.back().GetTotHitsEB()
    beamSpotInfos[interval]['nHitsEE'] += phiSymInfo.back().GetTotHitsEE()
    beamSpotInfos[interval]['bsPos'] += phiSymInfo.back().GetMean('Z')*phiSymInfo.back().GetNEvents()
    beamSpotInfos[interval]['bsPosWid'] += phiSymInfo.back().GetMeanSigma('Z')*phiSymInfo.back().GetNEvents()
    
    #Loop over EB PhySimRecHit    
    for ih,hit in enumerate(phiSymRecHitsEB):
        xtalIndex=ROOT.EBDetId(hit.GetRawId()).hashedIndex()
        ebSums[interval][xtalIndex]['nHits'] += hit.GetNhits()         
        ebSums[interval][xtalIndex]['energySum'] += hit.GetSumEt()
        ebSums[interval][xtalIndex]['energySumSquared'] += hit.GetSumEt2()
        ebSums[interval][xtalIndex]['lcSum'] += hit.GetLCSum()
        ebSums[interval][xtalIndex]['lcSumSquared'] += hit.GetLC2Sum()

    #Loop over EE PhySimRecHit    
    for ih,hit in enumerate(phiSymRecHitsEE):
        xtalIndex=ROOT.EEDetId(hit.GetRawId()).hashedIndex()
        eeSums[interval][xtalIndex]['nHits'] += hit.GetNhits()         
        eeSums[interval][xtalIndex]['energySum'] += hit.GetSumEt()
        eeSums[interval][xtalIndex]['energySumSquared'] += hit.GetSumEt2()
        eeSums[interval][xtalIndex]['lcSum'] += hit.GetLCSum()
        eeSums[interval][xtalIndex]['lcSumSquared'] += hit.GetLC2Sum()
        
if options.debug:
    print "====> First interval checks"
    print beamSpotInfos[0]
    print ebSums[0][0]
    print eeSums[0][0]

#Store information in final root tree
intervalNumber=n.zeros(1,dtype=int)
nEvents=n.zeros(1,dtype=int)
nHitsEB=n.zeros(1,dtype=long)
nHitsEE=n.zeros(1,dtype=long)
bsPos=n.zeros(1,dtype=float)
bsWid=n.zeros(1,dtype=float)

print "Opening output file "+options.output
outFile = ROOT.TFile(options.output, "RECREATE")
if not outFile:
    print "Cannot open outputFile "+options.output

bsTree = ROOT.TTree('bsTree', 'bsTree')
bsTree.Branch("timeInterval",intervalNumber,"timeInterval/I");
bsTree.Branch("nEvents",nEvents,"nEvents/I");
bsTree.Branch("nHitsEB",nHitsEB,"nHitsEB/L");
bsTree.Branch("nHitsEE",nHitsEE,"nHitsEE/L");
bsTree.Branch("bsPos",bsPos, "bsPos/D");
bsTree.Branch("bsWid",bsWid, "bsWid/D");

#Beam spot info tree
for key,value in intervals.intervals.iteritems():
    intervalNumber[0]=key
    nEvents[0]=beamSpotInfos[interval]['nEvents']
    nHitsEB[0]=beamSpotInfos[interval]['nHitsEB']
    nHitsEE[0]=beamSpotInfos[interval]['nHitsEE']
    bsPos[0]=beamSpotInfos[interval]['bsPos']
    bsWid[0]=beamSpotInfos[interval]['bsPosWid']
    bsTree.Fill()

nHits=n.zeros(1,dtype=long)
det=n.zeros(1,dtype=int)
ieta=n.zeros(1,dtype=int)
iphi=n.zeros(1,dtype=int)
sign=n.zeros(1,dtype=int)
energySum=n.zeros(1,dtype=float)
energySquared=n.zeros(1,dtype=float)
lcSum=n.zeros(1,dtype=float)
lcSquared=n.zeros(1,dtype=float)

outTree= ROOT.TTree("tree","tree");
outTree.Branch("timeInterval",intervalNumber,"timeInterval/I");
outTree.Branch("nHits",nHits, "nHits/L");
outTree.Branch("det",det, "det/I");
outTree.Branch("ieta",ieta,"ieta/I");
outTree.Branch("iphi",iphi,"iphi/I");
outTree.Branch("sign",sign,"sign/I");
outTree.Branch("energySum",energySum,"energySum/D");
outTree.Branch("energySquared",energySquared,"energySquared/D");
outTree.Branch("lcSum",lcSum,"lcSum/D");
outTree.Branch("lcSquared",lcSquared,"lcSquared/D");

#Partial sums tree
for key,value in intervals.intervals.iteritems():
    for ixtal in range(0,ROOT.EBDetId.kSizeForDenseIndexing):
        intervalNumber[0]=key
        nHits[0]=ebSums[key][ixtal]['nHits']
        det[0]=ebSums[key][ixtal]['det']
        ieta[0]=ebSums[key][ixtal]['ieta']
        iphi[0]=ebSums[key][ixtal]['iphi']
        sign[0]=ebSums[key][ixtal]['sign']
        energySum[0]=ebSums[key][ixtal]['energySum']
        energySquared[0]=ebSums[key][ixtal]['energySumSquared']
        lcSum[0]=ebSums[key][ixtal]['lcSum']
        lcSquared[0]=ebSums[key][ixtal]['lcSumSquared']
        outTree.Fill()
    for ixtal in range(0,ROOT.EEDetId.kSizeForDenseIndexing):
        intervalNumber[0]=key
        nHits[0]=eeSums[key][ixtal]['nHits']
        det[0]=eeSums[key][ixtal]['det']
        ieta[0]=eeSums[key][ixtal]['ieta']
        iphi[0]=eeSums[key][ixtal]['iphi']
        sign[0]=eeSums[key][ixtal]['sign']
        energySum[0]=eeSums[key][ixtal]['energySum']
        energySquared[0]=eeSums[key][ixtal]['energySumSquared']
        lcSum[0]=eeSums[key][ixtal]['lcSum']
        lcSquared[0]=eeSums[key][ixtal]['lcSumSquared']
        outTree.Fill()

outFile.Write()
outFile.Close()
print "Output file "+options.output+" closed"

