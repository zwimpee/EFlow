#! /usr/bin/env python

from optparse import OptionParser, make_option
import sys
import os
import math
import time
import subprocess as sub
from array import array
from ROOT import *
from ROOT import TFile
import numpy as n

def main(options,args):
    # if you want to run in batch mode
    ROOT.gROOT.SetBatch()

    input = TFile.Open(options.infile)
    if (options.infilextal != ""):
        inputxtal = TFile.Open(options.infilextal)
    

    if (not options.infilextal != ""):
        start_run = array('i', [0] )
        end_run = array('i', [0] )
        ieta = array('i', [0] )
        det = array('i', [0] )
        kfactor  = array('f', [0.] )
        
        output = TFile(options.outfile, "recreate")
        tree = TTree("kfactors_vsTime", "kfactors_vsTime")
        tree.Branch('start_run', start_run, 'start_run/I')
        tree.Branch('end_run', end_run, 'end_run/I')
        tree.Branch('ieta', ieta, 'ieta/I')
        tree.Branch('det', det, 'det/I')
        tree.Branch('kfactor', kfactor, 'kfactor/F')

    else:
        #read tree of files per xtal
        start_run = array('i', [0] )
        end_run = array('i', [0] )
        ieta = array('i', [0] )
        iphi = array('i', [0] )
        sign = array('i', [0] )
        det = array('i', [0] )
        kfactor  = array('f', [0.] )
        
        output = TFile(options.outfile, "recreate")
        tree = TTree("kfactors_vsTime", "kfactors_vsTime")
        tree.Branch('start_run', start_run, 'start_run/I')
        tree.Branch('end_run', end_run, 'end_run/I')
        tree.Branch('ieta', ieta, 'ieta/I')
        tree.Branch('iphi', iphi, 'iphi/I')
        tree.Branch('sign', sign, 'sign/I')
        tree.Branch('det', det, 'det/I')
        tree.Branch('kfactor', kfactor, 'kfactor/F')


    kfactorsPerXtal = {}
    kfactorsPerRing = {}
    for i in range(0,85):
        kfactorsPerRing[i]=(0,0)
    if (options.infilextal != ""):
        inputxtal = TFile.Open(options.infilextal)
        kFactors_xtals = inputxtal.Get("kFactors_xtals")
        nentries=kFactors_xtals.GetEntries()
        for jentry in xrange(nentries):
            ientry = kFactors_xtals.LoadTree(jentry)
            if ientry < 0:
                break
            nb = kFactors_xtals.GetEntry(jentry)
            if nb<=0:
                continue
            #            print kFactors_xtals.eta,kFactors_xtals.phi,kFactors_xtals.sign,kFactors_xtals.kFactor 
            kfactorsPerXtal[(kFactors_xtals.sign,kFactors_xtals.eta-1,kFactors_xtals.phi-1)]=kFactors_xtals.kFactor
            kfactorsPerRing[kFactors_xtals.eta-1]=(kfactorsPerRing[kFactors_xtals.eta-1][0]+kFactors_xtals.kFactor,(kfactorsPerRing[kFactors_xtals.eta-1][1]+1))

    for i in range(0,85):
        #print kfactorsPerRing[i][0],kfactorsPerRing[i][1]
        kfactorsPerRing[i]=(kfactorsPerRing[i][0]/kfactorsPerRing[i][1])

    for xtal in kfactorsPerXtal.keys():
        kfactorsPerXtal[xtal]=kfactorsPerXtal[xtal]/kfactorsPerRing[xtal[1]]
        #print kfactorsPerXtal[xtal]
            
    #Start with EB
    det[0]=0
    for i in range(0,85):
        histName="Kfactor_ring_"+str(i)+"_(Barl)"
        h=input.Get(histName)
        ieta[0]=i+1
        print ieta
        for ibin in range(1,h.GetNbinsX()):
            binLabel=h.GetXaxis().GetBinLabel(ibin)
            if (len(binLabel)<13):
                continue
            runs=binLabel.split("_")
            print runs
            start_run[0]=int(runs[0])
            end_run[0]=int(runs[1])
            kfactor[0]=h.GetBinContent(ibin)
            print kfactor[0]
            if (options.infilextal != ""):
                for xtal in kfactorsPerXtal.keys():
                    if xtal[1]==i:
                        iphi[0]=xtal[2]+1
                        sign[0]=xtal[0]
                        kfactor[0]=h.GetBinContent(ibin)*kfactorsPerXtal[xtal]
                        print ieta[0],iphi[0],sign[0],kfactor[0]
                        tree.Fill()
            else:
                tree.Fill()
    det[0]=1
    for i in range(0,39):
        histName="Kfactor_ring_"+str(i+85)+"_(Endc)"
        h=input.Get(histName)
        ieta[0]=i+1
        if (options.infilextal != ""):
            iphi[0]=-1
            sign[0]=-1
        print ieta
        for ibin in range(1,h.GetNbinsX()):
            binLabel=h.GetXaxis().GetBinLabel(ibin)
            if (len(binLabel)<13):
                continue
            runs=binLabel.split("_")
            print runs
            start_run[0]=int(runs[0])
            end_run[0]=int(runs[1])
            kfactor[0]=h.GetBinContent(ibin)
            print kfactor
            tree.Fill()

    output.Write()
    output.Close()

if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--infile",
                    action="store", type="string", dest="infile",
                    default="data/kfactors_2012.root",
                    help="", metavar=""
                    ),
        make_option("-j", "--infilextal",
                    action="store", type="string", dest="infilextal",
                    default="",
                    help="", metavar=""
                    ),
        make_option("-o", "--outfile",
                    action="store", type="string", dest="outfile",
                    default="data/kfactorsVsTime_2012.root",
                    help="", metavar=""
                    )
        ])
    
    (options, args) = parser.parse_args()
    
    main( options, args) 
