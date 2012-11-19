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

from DataFormats.FWLite import *
#from DataFormats.DetId import *
#from DataFormats.EcalDetId import *

def getlist(input):
    lst = {}
    nlines = 0
    for line in input.split("\n"):
        if line.startswith("#"):
            continue
        l = [ i for i in line.replace("="," ").split(" ")  if i != "" ]
        if len(l) < 3:
            continue
        try:
            a=DetId(int(l[0]))
            if (a.det()!=3):
                print "Not an ECAL DetId!!"
                continue
            if (a.subdetId()==1):
                detId=EBDetId(int(l[0]))
                det, ieta, iphi, sign, fedid, harness, harness_hashed = 0, detId.ieta(), detId.iphi(), -1+(detId.ieta()>0)*2, int(l[1]), int(l[2]), (int(l[1]) - 610)*9+(int(l[2])-1)
            if (a.subdetId()==2):
                detId=EEDetId(int(l[0]))
                det, ieta, iphi, sign, fedid, harness, harness_hashed = 1, detId.ix(), detId.iy(), detId.zside(), int(l[1]), int(l[2]), (detId.zside()>0)* 19+(int(l[2])-1)
        except Exception, e:
            print line
            print e
        nlines = nlines + 1 
        lst[ (det,ieta,iphi,sign) ] = ( fedid, harness, harness_hashed )
    print str(nlines)+" processed"
    return lst

def main(options,args):
    # if you want to run in batch mode
    ROOT.gROOT.SetBatch()

    file1 = open(options.infile)
    map = getlist(file1.read())
    
    ieta = array('i', [0] )
    iphi = array('i', [0] )
    sign = array('i', [0] )
    det = array('i', [0] )
    fedid = array('i', [0] )
    harness = array('i', [0] )
    harness_hashed = array('i', [0] )

    output = TFile(options.outfile, "recreate")
    tree = TTree("harnessMap", "harnessMap")
    tree.Branch('ieta', ieta, 'ieta/I')
    tree.Branch('iphi', iphi, 'iphi/I')
    tree.Branch('det', det, 'det/I')
    tree.Branch('sign', sign, 'sign/I')
    tree.Branch('fedid', fedid, 'fedid/I')
    tree.Branch('harness', harness, 'harness/I')
    tree.Branch('harness_hashed', harness_hashed, 'harness_hashed/I')

    channels=map.keys()
    channels.sort( )
    for i in channels:
#        print map[i]
        det[0], ieta[0], iphi[0], sign[0], fedid[0], harness[0], harness_hashed[0] = i[0], i[1], i[2], i[3], map[i][0], map[i][1], map[i][2]
        tree.Fill()
    output.Write()
    output.Close()

if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--infile",
                    action="store", type="string", dest="infile",
                    default="data/detid_fed_harness.dat",
                    help="", metavar=""
                    ),
        make_option("-o", "--outfile",
                    action="store", type="string", dest="outfile",
                    default="data/harnessMap.root",
                    help="", metavar=""
                    )
        ])
    
    (options, args) = parser.parse_args()
    
    main( options, args) 
