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
        ebSums[key][ixtal]['det'] = 0
        ebSums[key][ixtal]['ieta'] = detId.ietaAbs()
        ebSums[key][ixtal]['iphi'] = detId.iphi()
        ebSums[key][ixtal]['sign'] = detId.zside() #could be 0/1

    #initialize EESum for interval
    eeSums.append(list())
    for ixtal in range(0,ROOT.EEDetId.kSizeForDenseIndexing):
        detId=ROOT.EEDetId.detIdFromDenseIndex(ixtal)
        eeSums[key].append(dict(xtalSumInfo))
        eeSums[key][ixtal]['det'] = 1
        eeSums[key][ixtal]['ieta'] = detId.ix()
        eeSums[key][ixtal]['iphi'] = detId.iy()
        eeSums[key][ixtal]['sign'] = detId.zside()

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
        


#Store information in final root tree
