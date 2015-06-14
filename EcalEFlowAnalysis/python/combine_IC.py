#!/usr/bin/env python

from ROOT import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()
gSystem.Load("libCalibrationTools.so")

all = DSAll()
eb = DSIsBarrel()
eep = DSIsEndcapPlus()
eem = DSIsEndcapMinus()

#rings = DRings()
#rings.setEERings("eerings.dat")
#
#ic1 = IC()
#ic2 = IC()
#ic_scale = IC()
#res = IC()
#res.setRings(rings)
#icdiff = IC()
#
#check = IC()

#dir = "/afs/cern.ch/cms/CAF/CMSALCA/ALCA_ECALCALIB/energy-calibration-repository/phiSymmetry/2012B_NewLaserTag/"
dir = "/afs/cern.ch/cms/CAF/CMSALCA/ALCA_ECALCALIB/energy-calibration-repository/phiSymmetry/2012C_NewLaserTag/"
import glob
files=glob.glob(dir+"EcalIntercalibConstants_NLT_*_NewCodeChecked_Absolute.txt")
ICset= {}
meanIC = IC()
#print files

for file in files:
    print "Reading "+file
    ICset[file]=IC()
    IC.readTextFile(file, ICset[file])

meanIC = ICset[files[0]]
for file in files[1:]:
    IC.add(meanIC, ICset[file], meanIC)

IC.multiply(meanIC,1/float(len(files)),meanIC,all)
#print type(meanIC)
IC.dump(meanIC,"test.txt",all)

#
#p = TProfile("p", "p", 800, -200, 200)
#
#fout = TFile("combo_histos.root", "recreate")
#
#p1 = p.Clone("p1")
#p2 = p.Clone("p2")
#pr = p.Clone("pr")
#ps = p.Clone("ps")
#pc = p.Clone("pc")
#pd = p.Clone("pd")
#
#IC.profileEta(ic1, p1, all)
#IC.profileEta(ic2, p2, all)
#
#IC.scaleEta(ic1, ic1, True)
#IC.scaleEta(ic1, ic_scale)
#
#IC.scaleEta(ic2, ic2, True)
#IC.scaleEta(ic2, ic_scale)
#
#IC.combine(ic1, ic2, res)
#IC.scaleEta(res, res, True)
#IC.scaleEta(res, ic_scale)
#
#print "overall average:", IC.average(res, all)
#print "     EB average:", IC.average(res, eb)
#print "    EE+ average:", IC.average(res, eep)
#print "    EE- average:", IC.average(res, eem)
#
#IC.profileEta(res, pr, all)
#IC.profileEta(ic_scale, ps, all)
#IC.profileEta(check, pc, all)
#
#IC.dump(res, "combined.txt", all)
#
#IC.multiply(res, -1, res)
#IC.add(res, check, icdiff)
#IC.profileEta(icdiff, pd, all)
#h = TH1F("h", "h", 1000, -.1, .1)
#IC.constantDistribution(icdiff, h, all)
#
#fout.Write()
#fout.Close()
