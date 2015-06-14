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

def getlist(input,det):
    print det
    lst = {}
    for line in input.split("\n"):
        if line.startswith("##"):
            continue
        l = [ i for i in line.replace("="," ").split(" ")  if i != "" ]
        if len(l) < 5:
            continue
        try:
            if (det=="EB"):
                if (len(l)>=5 and int(l[2])==0 ):
                    ieta, iphi, isign, ic, ic_err = int(l[0]), int(l[1]), int(l[2]), float(l[3]), float(l[4])
            elif (det=="EE"):
                if (len(l)>=5 and int(l[2])!=0 ):
                    ix, iy, sign, ic, ic_err = int(l[0]), int(l[1]), int(l[2]), float(l[3]), float(l[4])
        except Exception, e:
            print line
            print e
        if (det=="EB" and len(l)>=5 and  int(l[2])==0):
            lst[ (ieta,iphi) ] = ( ic, ic_err )
        elif (det=="EE" and len(l)>=5 and  int(l[2])!=0 ):
            lst[ (ix,iy,sign) ] = ( ic, ic_err )
    return lst

def fillRatioPlots(channels,lista,listb,scatter,diff,diffVsIC,diffMap,det):
    ic_ratio = {}
    for channel in channels:
        if (lista[channel][0]==-1 or listb[channel][0]==-1):
            continue
        if (det=="EB"):
            ic_ratio[ (channel[0],channel[1],0) ] = (lista[channel][0]/listb[channel][0],lista[channel][0]/listb[channel][0]*sqrt(pow(lista[channel][1]/lista[channel][0],2)+pow(listb[channel][1]/listb[channel][0],2) ) )
        elif (det=="EE"):
            ic_ratio[ (channel[0],channel[1],channel[2]) ] = (lista[channel][0]/listb[channel][0],lista[channel][0]/listb[channel][0]*sqrt(pow(lista[channel][1]/lista[channel][0],2)+pow(listb[channel][1]/listb[channel][0],2) ) )
        diff.Fill(lista[channel][0]/listb[channel][0])
        diffVsIC.Fill(lista[channel][0],lista[channel][0]/listb[channel][0])
        scatter.Fill(lista[channel][0],listb[channel][0])
        if (det=="EB"):
            etaIndex=channel[0]+86
            diffMap.SetBinContent(channel[1],etaIndex,(lista[channel][0]/listb[channel][0]))
        elif (det=="EE"):
            ixIndex=channel[0]+(channel[2]>0)*100
            diffMap.SetBinContent(ixIndex,channel[1],(lista[channel][0]/listb[channel][0]))
    return ic_ratio

def fillAlphaPlots(list,values,err,map,det):
    for channel in list.keys():
        #        if ( list[channel][3] != "ok" and list[channel][3] != "large_err_fit" ):
        #            continue
        if (list[channel][0]<=0.7 or (det=="EB" and list[channel][0]==1.52) or (det=="EE" and list[channel][0]==1.16) or  list[channel][0]>=2.1):
            continue
        values.Fill(list[channel][0])
        err.Fill(list[channel][1])
        if (det=="EB"):
            etaIndex=channel[0]+86
            map.SetBinContent(channel[1],etaIndex,list[channel][0])
            
        elif (det=="EE"):
            ixIndex=channel[0]+channel[2]*100
            map.SetBinContent(ixIndex,channel[1],list[channel][0])

def printRatioPlots(scatter,diff,diffVsIC,diffMap,suffix):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)
    ROOT.gStyle.SetOptFit(11)
    #a= ROOT.TH2F("a","a",10,-1,4,10,-1,4)
    #    a.GetXaxis()->SetTitle(suffix1);
    #    a.GetYaxis()->SetTitle(suffix2);
    c = ROOT.TCanvas ( "plots" )
    scatter.GetXaxis().SetTitle(tag1)
    scatter.GetYaxis().SetTitle(tag2)
    scatter.Draw("BOX")
    scatter.SaveAs("plotsICDiff/corrIC_"+suffix+".root")
    c.SaveAs("plotsICDiff/corrIC_"+suffix+".png")

    diffVsIC.GetXaxis().SetTitle(tag1)
    diffVsIC.GetYaxis().SetTitle(tag1+" / "+tag2)
    diffVsIC.Draw("BOX")
    diffVsIC.SaveAs("plotsICDiff/ratioVsIC_"+suffix+".root")
    c.SaveAs("plotsICDiff/ratioVsIC_"+suffix+".png")

    diff.GetXaxis().SetTitle(tag1+" / "+tag2)
    diff.Draw()
    diff.Fit("gaus","","",diff.GetMean()-3*diff.GetRMS(),diff.GetMean()+3*diff.GetRMS());
    diff.SaveAs("plotsICDiff/ratioIC_"+suffix+".root")
    c.SaveAs("plotsICDiff/ratioIC_"+suffix+".png")

    diffMap.GetZaxis().SetTitle(tag1+" - "+tag2)
    diffMap.GetZaxis().SetRangeUser(diff.GetMean()-3*diff.GetRMS(),diff.GetMean()+3*diff.GetRMS());
    diffMap.Draw("COLZ");
    diffMap.SaveAs("plotsICDiff/ratioICMap_"+suffix+".png")
    c.SaveAs("plotsICDiff/ratioICMap_"+suffix+".png")

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
                if det=="EB":
                    combined[channel]=(1.52,-9.999,"bad_fit","forced_as_bad_fit_in_combination")
                elif det=="EE":
                    combined[channel]=(1.16,-9.999,"bad_fit","forced_as_bad_fit_in_combination")
        elif strategy == "ttPriorityFile1largeErr":
            alpha, err = list1[channel][0],list1[channel][1] #for large err trust only values from the TT
            if alpha>2.1:
                alpha=2.1
            if alpha<0.7:
                alpha=0.7
            combined[channel]=(alpha,err,status,strategy)
        elif strategy == "badFitSimpleCombination":
            if det=="EB":
                combined[channel]=(1.52,-9.999,"bad_fit",strategy)
            elif det=="EE":
                combined[channel]=(1.16,-9.999,"bad_fit",strategy)
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
            if det=="EB":
                combined[channel]=(1.52,-9.999,"bad_fit","forced_as_bad_fit")
            elif det=="EE":
                combined[channel]=(1.16,-9.999,"bad_fit","forced_as_bad_fit")

def badRegionEB2012Bv6():
    badChannels = set([ (int(x),int(y)) for x in range (26,46) for y in range(141,161) ])
    return badChannels

ROOT.gROOT.SetBatch(1)
ROOT.gROOT.SetStyle("Plain")
#ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetOptStat(111111)

set_palette()

#sample usage file1 file2 tag1 tag2 strategy (simpleCombination, ttPriorityFile1)
fn1 = argv.pop(1)
fn2 = argv.pop(1)
tag1 = argv.pop(1)
tag2 = argv.pop(1)
#globalStrategy = argv.pop(1)

det="EB"
if (len(argv)>1):
    det = argv.pop(1)
globalSuffix = tag1+"_vs_"+tag2

file1 = open(fn1)
file2 = open(fn2)

list1 = getlist( file1.read() , det)
print "Read "+str(len(list1.keys()))+" values from file1 "+fn1  
list2 = getlist( file2.read() , det)
print "Read "+str(len(list2.keys()))+" values from file2 "+fn2  

def cmp(x,y):
    if x[0] < y[0] : return True
    elif x[0] == y[0]:
        if x[1] < y[1] : return True
    return False

xtals1 = list1.keys()
xtals2 = list2.keys()

xtals1.sort( )
xtals2.sort( )
#
#

only1 = list(set(xtals1) -  set(xtals2))
only2 = list(set(xtals2) -  set(xtals1))
#
only1.sort(  )
only2.sort(  )

print "Xtals only in file1 " + str(len(only1)) +  "; Xtals only in file2 " + str(len(only2)) 
commonScatterPlot = ROOT.TH2F("commonCorrelation","commonCorrelation",3000,0.,2,3000,0.,2.)
commonRatioVsIC = ROOT.TH2F("commonRatioVsIC","commonRatioVsIC",3000,0.,2,1000,0.8,1.2)
commonDiff = ROOT.TH1F("commonDiff","commonDiff",1000,0.8,1.2)
if det=="EB":
    commonMap = ROOT.TH2F("commonMap","commonMap",360,0.5,360.5,171,-85.5,85.5)
elif det=="EE":
    commonMap = ROOT.TH2F("commonMap","commonMap",200,0.5,200.5,100,0.5,100.5)
    
common = set(xtals1).intersection(  set(xtals2) )
commonRatio=fillRatioPlots(common,list1,list2,commonScatterPlot,commonDiff,commonRatioVsIC,commonMap,det)
printRatioPlots(commonScatterPlot,commonDiff,commonRatioVsIC,commonMap,"common"+globalSuffix)

out=open("icRatio_"+globalSuffix+".txt","w")
print>>out, "## File generated from ratio of "+fn1+" and "+fn2
#out.write("##ieta\tiphi\tisign\tic\tic_err\n")
channels=commonRatio.keys()
channels.sort()
for key in channels:
    out.write("%d %d %d %f %f\n"% (key[0],key[1],key[2],commonRatio[key][0],commonRatio[key][1]) )
print "Written combination file icRatio_"+globalSuffix+".txt"
