#!/usr/bin/env python
import sys
# import ROOT in batch mode
import ROOT
ROOT.gROOT.SetBatch(True)

from optparse import OptionParser, make_option

# load FWLite C++ libraries
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.AutoLibraryLoader.enable()

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events, Lumis

class makeMap:
    def __init__(self,filelist):
        self.filelist=file(filelist)
        files = []

        for ifile in self.filelist.readlines():
            if len( ifile ) > 2 : 
                s = ifile.rstrip()
                files.append( s )
                print 'Added ' + s

        # Opening the LumiBlocks tree
        self.lumis = Lumis(files)

        self.handlePhiSymInfo  = Handle ("std::vector<PhiSymInfo>")
        self.labelPhiSymInfo = ("PhiSymProducer")
        
    def run(self):
        for i,lumi in enumerate(self.lumis):
            print "====>"
            lumi.getByLabel(self.labelPhiSymInfo,self.handlePhiSymInfo)
            phiSymInfo = self.handlePhiSymInfo.product()
            print "Run "+str(phiSymInfo.back().getStartLumi().run())+" Lumi "+str(phiSymInfo.back().getStartLumi().luminosityBlock())
            print "Begin Time "+str(lumi.aux().beginTime().unixTime())
            print "End Time "+str(lumi.aux().endTime().unixTime())
            print "NEvents in this LS "+str(phiSymInfo.back().GetNEvents())
            print "TotHits EB "+str(phiSymInfo.back().GetTotHitsEB())+" Avg occ EB "+str(float(phiSymInfo.back().GetTotHitsEB())/phiSymInfo.back().GetNEvents()) 
            print "TotHits EE "+str(phiSymInfo.back().GetTotHitsEE())+" Avg occ EE "+str(float(phiSymInfo.back().GetTotHitsEE())/phiSymInfo.back().GetNEvents()) 
            

if __name__ == '__main__':
    parser = OptionParser(option_list=[
            make_option("-i", "--input",
                        action="store", type="string",
                        default="files.txt",
                        help="file listname", metavar=""
                        ),
        ])
    
    (options, args) = parser.parse_args()
    a=makeMap(options.input)
    a.run()
