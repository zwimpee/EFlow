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

# open file (you can use 'edmFileUtil -d /store/whatever.root' to get the physical file name)
#lumis = Lumis("file:/tmp/zwimpee/CMSSW_746-weights-74X_dataRun2_Prompt_v0-Run2015B_v3_test.root")

parser = OptionParser()
parser.add_option("-d", "--debug", dest="debug", action='store_true')
parser.add_option("-f", "--fileList", dest="fileList", type="string", default="fileList.txt")
parser.add_option("-o", "--output", dest="output", type="string", default="readMap.root")
parser.add_option("-p", "--prefix", dest="prefix", type="string", default="file:")
parser.add_option("-j", "--jsonFile", dest="jsonFile", type = "string", default="default.json")
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

for i,lumi in enumerate(lumis):
    if options.debug:
        print i
 
    lumi.getByLabel (labelPhiSymInfo,handlePhiSymInfo)
    lumi.getByLabel (labelPhiSymRecHitsEB,handlePhiSymRecHitsEB)
    lumi.getByLabel (labelPhiSymRecHitsEE,handlePhiSymRecHitsEE)
    phiSymInfo = handlePhiSymInfo.product()
    phiSymRecHitsEB = handlePhiSymRecHitsEB.product()
    phiSymRecHitsEE = handlePhiSymRecHitsEE.product()

    #skipping BAD lumiSections
    if not lumiList.contains(phiSymInfo.back().getStartLumi().run(),phiSymInfo.back().getStartLumi().luminosityBlock()):
        continue

    for ih,hit in enumerate(phiSymRecHitsEB):
        if options.debug:
            print ih
        
    for ih,hit in enumerate(phiSymRecHitsEE):
        if options.debug:
            print ih
        
