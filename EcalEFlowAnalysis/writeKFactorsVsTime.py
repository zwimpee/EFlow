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

    
    start_run = array('i', [0] )
    end_run = array('i', [0] )
    iring = array('i', [0] )
    det = array('i', [0] )
    kfactor  = array('f', [0.] )

    output = TFile(options.outfile, "recreate")
    tree = TTree("kfactors_vsTime", "kfactors_vsTime")
    tree.Branch('start_run', start_run, 'start_run/I')
    tree.Branch('end_run', end_run, 'end_run/I')
    tree.Branch('ieta', iring, 'ieta/I')
    tree.Branch('det', det, 'det/I')
    tree.Branch('kfactor', kfactor, 'kfactor/F')

    #Start with EB
    det[0]=0
    for i in range(0,84):
        histName="Kfactor_ring_"+str(i)+"_(Barl)"
        h=input.Get(histName)
        iring[0]=i+1
        print iring
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
    det[0]=1
    for i in range(0,38):
        histName="Kfactor_ring_"+str(i+85)+"_(Endc)"
        h=input.Get(histName)
        iring[0]=i+1
        print iring
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
        make_option("-o", "--outfile",
                    action="store", type="string", dest="outfile",
                    default="data/kfactorsVsTime_2012.root",
                    help="", metavar=""
                    )
        ])
    
    (options, args) = parser.parse_args()
    
    main( options, args) 
