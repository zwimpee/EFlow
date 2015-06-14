#! /usr/bin/env python

from optparse import OptionParser, make_option
import sys
import os
import math
import time
import subprocess as sub
from array import array
from ROOT import *
#print "done."


def main(options,args):
    # if you want to run in batch mode
    ROOT.gROOT.SetBatch()



    input = TFile.Open(options.infile)
    inputIntervals = TFile.Open(options.intervals)

    ROOT.gStyle.SetOptFit(11111)
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)
    
    if os.path.isdir(options.outdir) is False:
        os.mkdir(options.outdir)


    intervalsTree=inputIntervals.Get("outTree_barl")
    


if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--infile",
                    action="store", type="string", dest="infile",
                    default="pi0.root",
                    help="", metavar=""
                    ),
        make_option("-n", "--intervals",
                    action="store", type="string", dest="intervals",
                    default="readMap.root",
                    help="", metavar=""
                    ),
        make_option("-d", "--outdir",
                    action="store", type="string", dest="outdir",
                    default="plots",
                    help="", metavar=""
                    ),
        ])
    
    (options, args) = parser.parse_args()
    
    main( options, args) 
