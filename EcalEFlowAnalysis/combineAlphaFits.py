#!/usr/bin/env python

from sys import argv
import json 
from pprint import pprint 
from math import fabs,sqrt
from array import array
import operator

import ROOT

def set_palette(name="color", ncontours=999):
    """Set a color palette from a given RGB list
    stops, red, green and blue should all be lists of the same length
    see set_decent_colors for an example"""
    
    if name == "gray" or name == "grayscale":
        stops = [0.00, 0.34, 0.61, 0.84, 1.00]
        red   = [1.00, 0.84, 0.61, 0.34, 0.00]
        green = [1.00, 0.84, 0.61, 0.34, 0.00]
        blue  = [1.00, 0.84, 0.61, 0.34, 0.00]
        # elif name == "whatever":
        # (define more palettes)
    else:
        # default palette, looks cool
        stops = [0.00, 0.34, 0.61, 0.84, 1.00]
        red   = [0.00, 0.00, 0.87, 1.00, 0.51]
        green = [0.00, 0.81, 1.00, 0.20, 0.00]
        blue  = [0.51, 1.00, 0.12, 0.00, 0.00]
        
    s = array('d', stops)
    r = array('d', red)
    g = array('d', green)
    b = array('d', blue)
        
    npoints = len(s)
    ROOT.TColor.CreateGradientColorTable(npoints, s, r, g, b, ncontours)
    ROOT.gStyle.SetNumberContours(ncontours)

def getlist(input):
    lst = {}
    for line in input.split("\n"):
        if line.startswith("##"):
            continue
        l = [ i for i in line.replace("="," ").split("\t")  if i != "" ]
        try:
            if len(l) < 5:
                continue
            ieta, iphi, alpha, alpha_err, comment = int(l[0]), int(l[1]), float(l[2]), float(l[3]), l[4]
        except Exception, e:
            print line
            print e
        lst[ (ieta,iphi) ] = ( alpha, alpha_err, comment )
    return lst

def fillDiffPlots(channels,lista,listb,scatter,diff,diffMap):
    for channel in channels:
        diff.Fill(lista[channel][0]-listb[channel][0])
        scatter.Fill(lista[channel][0],listb[channel][0])
        etaIndex=channel[0]+86
        diffMap.SetBinContent(channel[1],etaIndex,(lista[channel][0]-listb[channel][0])+1.)

def fillAlphaPlots(list,values,err,map):
    for channel in list.keys():
        #        if ( list[channel][3] != "ok" and list[channel][3] != "large_err_fit" ):
        #            continue
        values.Fill(list[channel][0])
        err.Fill(list[channel][1])
        etaIndex=channel[0]+86
        map.SetBinContent(channel[1],etaIndex,list[channel][0])

def printDiffPlots(scatter,diff,diffMap,suffix):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)
    ROOT.gStyle.SetOptFit(11)
    #a= ROOT.TH2F("a","a",10,-1,4,10,-1,4)
    #    a.GetXaxis()->SetTitle(suffix1);
    #    a.GetYaxis()->SetTitle(suffix2);
    c = ROOT.TCanvas ( "plots" )
    scatter.Draw("BOX")
    c.SaveAs("plotsFitDiff/corrAlpha_"+suffix+".png")

    diff.Draw()
    diff.Fit("gaus","","",diff.GetMean()-3*diff.GetRMS(),diff.GetMean()+3*diff.GetRMS());
    c.SaveAs("plotsFitDiff/diffAlpha_"+suffix+".png")

    diffMap.GetZaxis().SetRangeUser(1+diff.GetMean()-3*diff.GetRMS(),1+diff.GetMean()+3*diff.GetRMS());
    diffMap.Draw("COLZ");
    c.SaveAs("plotsFitDiff/diffAlphaMap_"+suffix+".png")

def printAlphaPlots(values,err,map,suffix):
    ROOT.gStyle.SetOptStat(1)
    ROOT.gStyle.SetOptTitle(0)
    ROOT.gStyle.SetOptFit(11)
    #a= ROOT.TH2F("a","a",10,-1,4,10,-1,4)
    #    a.GetXaxis()->SetTitle(suffix1);
    #    a.GetYaxis()->SetTitle(suffix2);
    c = ROOT.TCanvas ( "plots" )

    values.Draw()
    c.SaveAs("plotsCombined/combinedAlpha_"+suffix+".png")

    err.Draw()
    c.SaveAs("plotsCombined/combinedAlphaErr_"+suffix+".png")

    ROOT.gStyle.SetOptStat(0)
    map.GetZaxis().SetRangeUser(0.5,2.3)
    map.Draw("COLZ");
    c.SaveAs("plotsCombined/combinedAlphaMap_"+suffix+".png")

def combineChannels(channels,list1,list2,combined,strategy,status):
    for channel in channels:
        if strategy == "simpleCombination":
            alpha1, err1 = list1[channel][0],list1[channel][1]
            alpha2, err2 = list2[channel][0],list2[channel][1]
            e2_1=1/(err1*err1)
            e2_2=1/(err2*err2)
            sume2=e2_1+e2_2
            alpha=alpha1*e2_1/sume2+alpha2*e2_2/sume2
            err=sqrt(1/sume2)
            if alpha>2.1:
                alpha=2.1
            if alpha<0.7:
                alpha=0.7
            combined[channel]=(alpha,err,status,strategy)
        elif strategy == "ttPriorityFile1":
            alpha1, err1 = list1[channel][0],list1[channel][1]
            alpha2, err2 = list2[channel][0],list2[channel][1]
            if (fabs(alpha2-alpha1)>2*0.13 and err2<0.08): #assign a sigma of 0.13 to values per channel and consider only those 2sigma away
                alpha=alpha2
                err=err2
                message="_2sigmaChannel"
            else:
                alpha=alpha1
                err=err1
                message=""
            if alpha>2.1:
                alpha=2.1
            if alpha<0.7:
                alpha=0.7
            combined[channel]=(alpha,err,status,strategy+message)
        elif strategy == "simpleCombinationlargeErr":
            alpha1, err1 = list1[channel][0],list1[channel][1]
            alpha2, err2 = list2[channel][0],list2[channel][1]
            e2_1=1/(err1*err1)
            e2_2=1/(err2*err2)
            sume2=e2_1+e2_2
            alpha=alpha1*e2_1/sume2+alpha2*e2_2/sume2
            if alpha>2.1:
                alpha=2.1
            if alpha<0.7:
                alpha=0.7
            err=sqrt(1/sume2)
            if (err<0.3):
                combined[channel]=(alpha,err,status,strategy)
            else:
                combined[channel]=(1.52,-9.999,"bad_fit","forced_as_bad_fit_in_combination")
        elif strategy == "ttPriorityFile1largeErr":
            alpha, err = list1[channel][0],list1[channel][1] #for large err trust only values from the TT
            if alpha>2.1:
                alpha=2.1
            if alpha<0.7:
                alpha=0.7
            combined[channel]=(alpha,err,status,strategy)
        elif strategy == "badFitSimpleCombination":
            combined[channel]=(1.52,-9.999,"bad_fit",strategy)
                
def addChannels(channels,list,combined,status,message):
    for channel in channels:
        alpha, err = list[channel][0], list[channel][1]
        if alpha>2.1:
            alpha=2.1
        if alpha<0.7:
            alpha=0.7
        if (err<0.3):
            combined[channel]=(alpha,err,status,message)
        else:
            combined[channel]=(1.52,-9.999,"bad_fit","forced_as_bad_fit")

def badRegion2012Bv6():
    badChannels = set([ (int(x),int(y)) for x in range (26,46) for y in range(141,161) ])
    return badChannels

ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetOptStat(111111)

set_palette()

#sample usage file1 file2 tag1 tag2 strategy (simpleCombination, ttPriorityFile1)

fn1 = argv.pop(1)
fn2 = argv.pop(1)
tag1 = argv.pop(1)
tag2 = argv.pop(1)
globalStrategy = argv.pop(1)

globalSuffix = tag1+"_vs_"+tag2

file1 = open(fn1)
file2 = open(fn2)

list1 = getlist( file1.read() )
print "Read "+str(len(list1.keys()))+" values from file1 "+fn1  
list2 = getlist( file2.read() )
print "Read "+str(len(list2.keys()))+" values from file2 "+fn2  

def cmp(x,y):
    if x[0] < y[0] : return True
    elif x[0] == y[0]:
        if x[1] < y[1] : return True
    return False

events1 = list1.keys()
events2 = list2.keys()

events1.sort( )
events2.sort( )
#


        
commonScatterPlot = ROOT.TH2F("commonCorrelation","commonCorrelation",3000,0.,3.,3000,0.,3.)
commonDiff = ROOT.TH1F("commonDiff","commonDiff",600,-3.,3.)
commonMap = ROOT.TH2F("commonMap","commonMap",360,0.5,360.5,171,-85.5,85.5)
common = set(events1).intersection(  set(events2) )
fillDiffPlots(common,list1,list2,commonScatterPlot,commonDiff,commonMap)
printDiffPlots(commonScatterPlot,commonDiff,commonMap,"common")
#
only1 = list(set(events1) -  set(events2))
only2 = list(set(events2) -  set(events1))
#
only1.sort(  )
only2.sort(  )

print "Xtals only in file1 " + str(len(only1)) +  "; Xtals only in file2 " + str(len(only2)) 

goodChannels1 = [ x for x in list1.keys() if list1[x][2] == "ok" ]
goodChannels2 = [ x for x in list2.keys() if list2[x][2] == "ok" ]

goodChannels1.sort(  )
goodChannels2.sort(  )

#goodChannels1 = list(set(goodChannels1)-set(badRegion2012Bv6()))

print "goodXtals in file1 " + str(len(goodChannels1))
print "goodXtals in file2 " + str(len(goodChannels2))
commonGood = set(goodChannels1).intersection(  set(goodChannels2) )
print "Common goodChannels " + str(len(commonGood))

onlyGood1 = list(set(goodChannels1)-set(goodChannels2))
onlyGood2 = list(set(goodChannels2)-set(goodChannels1))

onlyGood1.sort(  )
onlyGood2.sort(  )

channelsDone = set(goodChannels1) | set(goodChannels2)
print "Total Good Channels either in File1 or 2 "+str(len(channelsDone)) 
print "goodXtals only in file1 " + str(len(onlyGood1))
print "goodXtals only in file2 " + str(len(onlyGood2))

combinedResult = {}

goodScatterPlot = ROOT.TH2F("goodCorrelation","goodCorrelation",3000,0.,3.,3000,0.,3.)
goodDiff = ROOT.TH1F("goodDiff","goodDiff",600,-3.,3.)
goodMap = ROOT.TH2F("goodMap","goodMap",360,0.5,360.5,171,-85.5,85.5)
fillDiffPlots(commonGood,list1,list2,goodScatterPlot,goodDiff,goodMap)
printDiffPlots(goodScatterPlot,goodDiff,goodMap,"goodCommon_"+globalSuffix)

largeErrChannels1 = [ x for x in list1.keys() if list1[x][2] == "large_err_fit" ]
largeErrChannels2 = [ x for x in list2.keys() if list2[x][2] == "large_err_fit" ]

largeErrChannels1.sort(  )
largeErrChannels2.sort(  )

largeErrChannels1 = list(set(largeErrChannels1)-channelsDone)
largeErrChannels2 = list(set(largeErrChannels2)-channelsDone)

#largeErrChannels1 = list(set(largeErrChannels1)-set(badRegion2012Bv6()))



print "largeErrXtals in file1 " + str(len(largeErrChannels1))
print "largeErrXtals in file2 " + str(len(largeErrChannels2))
commonLargeErr = set(largeErrChannels1).intersection(  set(largeErrChannels2) ) 
print "Common largeErrChannels " + str(len(commonLargeErr))

onlyLargeErr1 = list(set(largeErrChannels1)-set(largeErrChannels2))
onlyLargeErr2 = list(set(largeErrChannels2)-set(largeErrChannels1))

onlyLargeErr1.sort(  )
onlyLargeErr2.sort(  )

channelsDone = channelsDone | set(largeErrChannels1) | set(largeErrChannels2)
print "Total Channels taken from fit either in File1 or 2 "+str(len(channelsDone)) 

print "largeErrXtals only in file1 " + str(len(onlyLargeErr1))
print "largeErrXtals only in file2 " + str(len(onlyLargeErr2))

largeErrScatterPlot = ROOT.TH2F("largeErrCorrelation","largeErrCorrelation",3000,0.,3.,3000,0.,3.)
largeErrDiff = ROOT.TH1F("largeErrDiff","largeErrDiff",600,-3.,3.)
largeErrMap = ROOT.TH2F("largeErrMap","largeErrMap",360,0.5,360.5,171,-85.5,85.5)
fillDiffPlots(commonLargeErr,list1,list2,largeErrScatterPlot,largeErrDiff,largeErrMap)
printDiffPlots(largeErrScatterPlot,largeErrDiff,largeErrMap,"largeErrCommon_"+globalSuffix)

badFitChannels1 = [ x for x in list1.keys() if list1[x][2] == "bad_fit" ]
badFitChannels2 = [ x for x in list2.keys() if list2[x][2] == "bad_fit" ]

badFitChannels1.sort(  )
badFitChannels2.sort(  )

badFitChannels1 = list(set(badFitChannels1)-channelsDone)
badFitChannels2 = list(set(badFitChannels2)-channelsDone)

print "badFitXtals in file1 " + str(len(badFitChannels1))
print "badFitXtals in file2 " + str(len(badFitChannels2))
commonBadFit = set(badFitChannels1).intersection(  set(badFitChannels2) )
print "Common badFitChannels " + str(len(commonBadFit))

onlyBadFit1 = list(set(badFitChannels1)-set(badFitChannels2))
onlyBadFit2 = list(set(badFitChannels2)-set(badFitChannels1))

onlyBadFit1.sort(  )
onlyBadFit2.sort(  )

print "badFitXtals only in file1 " + str(len(onlyBadFit1))
print "badFitXtals only in file2 " + str(len(onlyBadFit2))

badFitScatterPlot = ROOT.TH2F("badFitCorrelation","badFitCorrelation",3000,0.,3.,3000,0.,3.)
badFitDiff = ROOT.TH1F("badFitDiff","badFitDiff",600,-3.,3.)
badFitMap = ROOT.TH2F("badFitMap","badFitMap",360,0.5,360.5,171,-85.5,85.5)
fillDiffPlots(commonBadFit,list1,list2,badFitScatterPlot,badFitDiff,badFitMap)
printDiffPlots(badFitScatterPlot,badFitDiff,badFitMap,"badFitCommon_"+globalSuffix)


#DO THE REAL COMBINATION
combineChannels(commonGood,list1,list2,combinedResult,globalStrategy,"ok")
addChannels(onlyGood1,list1,combinedResult,"ok","good_in_file1")
addChannels(onlyGood2,list2,combinedResult,"ok","good_in_file2")
combineChannels(commonLargeErr,list1,list2,combinedResult,globalStrategy+"largeErr","large_err_fit")
addChannels(onlyLargeErr1,list1,combinedResult,"large_err_fit","largeErr_in_file1")
addChannels(onlyLargeErr2,list2,combinedResult,"large_err_fit","largeErr_in_file2")
combineChannels(commonBadFit,list1,list2,combinedResult,"badFitSimpleCombination","bad_fit")
addChannels(onlyBadFit1,list1,combinedResult,"bad_fit","badFit_in_file1")
addChannels(onlyBadFit2,list2,combinedResult,"bad_fit","badFit_in_file2")
print "Final channels in combination:"+str(len(combinedResult.keys()))

combinedValues = ROOT.TH1F("combinedValues","combinedValues",1000,0.,3.)
combinedErr = ROOT.TH1F("combinedErr","combinedErr",1000,0.,0.3)
combinedMap = ROOT.TH2F("combinedMap","combinedMap",360,0.5,360.5,171,-85.5,85.5)
fillAlphaPlots(combinedResult,combinedValues,combinedErr,combinedMap)
printAlphaPlots(combinedValues,combinedErr,combinedMap,globalSuffix)

out=open("fitCombined_"+globalSuffix+".txt","w")
print>>out, "## File generated from combination of "+fn1+" and "+fn2+" using strategy "+globalStrategy
out.write("##\tieta\tiphi\talpha\talpha_err\tstatus\tcomment\n")
channels=combinedResult.keys()
channels.sort()
for key in channels:
    out.write("\t%d\t%d\t%f\t%f\t%s\t%s\n"% (key[0],key[1],combinedResult[key][0],combinedResult[key][1],combinedResult[key][2],combinedResult[key][3]) )
print "Written combination file fitCombined_"+globalSuffix+".txt"
