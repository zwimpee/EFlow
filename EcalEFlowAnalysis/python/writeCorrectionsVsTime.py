#! /usr/bin/env python

from optparse import OptionParser, make_option
import sys
import os
import math
import time
import subprocess as sub
import numpy as np
import scipy
import scipy.stats 
from ROOT import *
from ROOT import TFile
from array import array 

#from DataFormats.FWLite import *
#from DataFormats.DetId import *
#from DataFormats.EcalDetId import *

endcapRings={}
endcapXtals={}
endcapSCs={}

EB_IC={}
EE_IC={}

for i in range(1,39*2+1):
    endcapRings[i]=[]
for i in range(1,14648+1):
    endcapXtals[i]=[]
for i in range(1,624+1):
    endcapSCs[i]=[]

channelsInHarness_eb = {}
channelsInHarness_ee = {}
for i in range(1,324+1):
    channelsInHarness_eb[i]=[]
for i in range(1,38+1):
    channelsInHarness_ee[i]=[]

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
    TColor.CreateGradientColorTable(npoints, s, r, g, b, ncontours)
    gStyle.SetNumberContours(ncontours)

def nRegions(regionType):
    if (regionType == "EB_ring"):
        return 170
    elif (regionType == "EB_tt"):
        return 2448
    elif (regionType == "EB_xtal"):
        return 61200
    elif (regionType == "EB_harness"):
        return 324
    elif (regionType == "EE_ring"):
        return 39
    elif (regionType == "EE_sc"):
        return 624
    elif (regionType == "EE_xtal"):
        return 14648
    elif (regionType == "EE_harness"):
        return 38
    else:
        print "Region not supported"    

def getIC(input):
    for line in input.split("\n"):
        if line.startswith("##"):
            continue
        l = [ i for i in line.replace("="," ").split(" ")  if i != "" ]
        if len(l) < 5:
            continue
        try:
            if (int(l[2])==0):
                ieta, iphi, sign, ic, ic_err = int(l[0]), int(l[1]),  int(l[2]), float(l[3]), float(l[4]) 
            else:
                ix, iy, sign, ic, ic_err = int(l[0]), int(l[1]),  int(l[2]), float(l[3]), float(l[4]) 
        except Exception, e:
            print line
            print e
        if (int(l[2])==0):
            EB_IC[ (ieta,iphi) ] = ( ic, ic_err )
        else:
            EE_IC[ (ix,iy,sign) ] = ( ic, ic_err )

def readEEMap(eeMapFile):
    print "Reading eeMapFile from "+ eeMapFile 
    fee=TFile(eeMapFile)
    eeIndicesTree= fee.Get("eeIndices")
    nentries_ee = eeIndicesTree.GetEntries()
    for jentry in xrange(nentries_ee):
        ientry = eeIndicesTree.LoadTree(jentry)
        if ientry < 0:
            break
        nb = eeIndicesTree.GetEntry(jentry)
        if nb<=0:
            continue
        endcapRings[eeIndicesTree.sign*39+eeIndicesTree.iring+1].append((eeIndicesTree.ix,eeIndicesTree.iy,-1+eeIndicesTree.sign*2))
        endcapXtals[eeIndicesTree.ixtal+1].append((eeIndicesTree.ix,eeIndicesTree.iy,-1+eeIndicesTree.sign*2))
        endcapSCs[eeIndicesTree.isc+1].append((eeIndicesTree.ix,eeIndicesTree.iy,-1+eeIndicesTree.sign*2))

def readHarnessMap(harnessMapFile):
    print "Reading harnessMapFile from "+ harnessMapFile 
    fee=TFile(harnessMapFile)
    harnessMapTree=fee.Get("harnessMap")
    nentries = harnessMapTree.GetEntries()
    for jentry in xrange(nentries):
        ientry = harnessMapTree.LoadTree(jentry)
        if ientry < 0:
            break
        nb = harnessMapTree.GetEntry(jentry)
        if nb<=0:
            continue
      
        if (harnessMapTree.det==0):
            channelsInHarness_eb[harnessMapTree.harness_hashed+1].append((harnessMapTree.ieta,harnessMapTree.iphi,0))
        elif (harnessMapTree.det==1):
            channelsInHarness_ee[harnessMapTree.harness_hashed+1].append((harnessMapTree.ieta,harnessMapTree.iphi,harnessMapTree.sign))
        else:
            print "Don't know what to do..." 

def getXtalInRegion(index,regionType):
    xtalList = []
    if (regionType == "EB_ring"):
        ieta=(-1+(index>85)*2)*((index<86)*index+(index>85)*(index-85))
        for iphi in range(1,361):
            xtalList.append((ieta,iphi,0))
    elif (regionType == "EB_xtal"):
        ie=(index-1)/360+1
        ip=(index-1)%360+1;
        if (ie>85):
            ie=ie-85
        else:
            ie=-ie
        xtalList.append((ie,ip,0))
    elif (regionType == "EB_tt"):
        iT=int(index-1)%68
        ie=int(iT)/4
        ip=int(iT)%4
        ism=(int(index-1)/68)%18
        isign=-1+((int(index-1)/68)<18)*2
#        print index,iT,ie,ip,ism,isign
        for iii in range(ie*5,(ie+1)*5):
            for iij in range(ism*20+ip*5,ism*20+(ip+1)*5):
                xtalList.append((isign*(iii+1),iij+1,0))
    elif (regionType == "EB_harness"):
        for channel in channelsInHarness_eb[index]:
            xtalList.append(channel)
    elif (regionType == "EE_ring"):
        for channel in endcapRings[index]:
            xtalList.append(channel)
    elif (regionType == "EE_sc"):
        for channel in endcapSCs[index]:
            xtalList.append(channel)
    elif (regionType == "EE_xtal"):
        for channel in endcapXtals[index]:
            xtalList.append(channel)
    elif (regionType == "EE_harness"):
        for channel in channelsInHarness_ee[index]:
            xtalList.append(channel)
    else:
        print "Region not supported"
                                                                                                    
    sortedXtals=list(xtalList)
    sortedXtals.sort()
    return sortedXtals

def getIntervalsMap(mapFile):

### ******************************************************************************
### *Tree    :outTree_barl: outTree_barl                                           *
### *Entries :      104 : Total =           10253 bytes  File  Size =       5117 *
### *        :          : Tree compression factor =   1.23                       *
### ******************************************************************************
### *Br    0 :index     : index/I                                                *
### *Entries :      104 : Total  Size=        970 bytes  File Size  =        257 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.93     *
### *............................................................................*
### *Br    1 :nHit      : nHit/i                                                 *
### *Entries :      104 : Total  Size=        965 bytes  File Size  =        495 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
### *............................................................................*
### *Br    2 :nLS       : nLS/I                                                  *
### *Entries :      104 : Total  Size=        960 bytes  File Size  =        362 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.36     *
### *............................................................................*
### *Br    3 :firstRun  : firstRun/I                                             *
### *Entries :      104 : Total  Size=        985 bytes  File Size  =        366 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.36     *
### *............................................................................*
### *Br    4 :lastRun   : lastRun/I                                              *
### *Entries :      104 : Total  Size=        980 bytes  File Size  =        366 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.36     *
### *............................................................................*
### *Br    5 :firstLumi : firstLumi/I                                            *
### *Entries :      104 : Total  Size=        990 bytes  File Size  =        353 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.42     *
### *............................................................................*
### *Br    6 :lastLumi  : lastLumi/I                                             *
### *Entries :      104 : Total  Size=        985 bytes  File Size  =        358 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.39     *
### *............................................................................*
### *Br    7 :unixtimeStart : unixtimeStart/i                                    *
### *Entries :      104 : Total  Size=       1010 bytes  File Size  =        504 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
### *............................................................................*
### *Br    8 :unixtimeEnd : unixtimeEnd/i                                        *
### *Entries :      104 : Total  Size=       1000 bytes  File Size  =        502 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
### *............................................................................*
### *Br    9 :unixtimeMean : unixtimeMean/i                                      *
### *Entries :      104 : Total  Size=       1005 bytes  File Size  =        503 *
### *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
### *............................................................................*

    intervalsMap = {}
    print "Reading histories from file "+mapFile
    inputFile = TFile.Open(mapFile)
    maptree = inputFile.Get("outTree_barl")
    nentries=maptree.GetEntries()
    for jentry in xrange(nentries):
        ientry = maptree.LoadTree(jentry)
        if ientry < 0:
            break
        nb = maptree.GetEntry(jentry)
        if nb<=0:
            continue
        intervalsMap[maptree.index]=[(maptree.firstRun,maptree.firstLumi),(maptree.lastRun,maptree.lastLumi)]
    print "Read informations about "+str(len(intervalsMap.keys()))+" intervals"
    return intervalsMap

def enum(*sequential, **named):
    enums = dict(zip(sequential, range(len(sequential))), **named)
    return type('Enum', (), enums)


def main(options,args):
    # if you want to run in batch mode
    ROOT.gROOT.SetBatch()
    ROOT.gROOT.SetStyle("Plain")
    indexType = {}
    indexType["EB_ring"]=0
    indexType["EB_tt"]=1
    indexType["EB_xtal"]=2
    indexType["EE_ring"]=3
    indexType["EE_sc"]=4
    indexType["EE_xtal"]=5
    indexType["EB_harness"]=6
    indexType["EE_harness"]=7

    # Configure needed channels maps
    readEEMap(options.eeMapFile)
    readHarnessMap(options.harnessMapFile)

####   ******************************************************************************
####   *Tree    :historyTree: historyTree                                            *
####   *Entries :  8271120 : Total =       430323457 bytes  File  Size =  204043450 *
####   *        :          : Tree compression factor =   2.11                       *
####   ******************************************************************************
####   *Br    0 :timeInterval : timeInterval/i                                      *
####   *Entries :  8271120 : Total  Size=   33102186 bytes  File Size  =     363416 *
####   *Baskets :      164 : Basket Size=    3200000 bytes  Compression=  91.08     *
####   *............................................................................*
####   *Br    1 :index     : index/i                                                *
####   *Entries :  8271120 : Total  Size=   33101010 bytes  File Size  =     429063 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression=  77.14     *
####   *............................................................................*
####   *Br    2 :indexType : indexType/i                                            *
####   *Entries :  8271120 : Total  Size=   33101682 bytes  File Size  =     179866 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression= 184.02     *
####   *............................................................................*
####   *Br    3 :unixTime  : unixTime/i                                             *
####   *Entries :  8271120 : Total  Size=   33101514 bytes  File Size  =     311077 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression= 106.40     *
####   *............................................................................*
####   *Br    4 :etSumMeanVsRef : etSumMeanVsRef/F                                  *
####   *Entries :  8271120 : Total  Size=   33102522 bytes  File Size  =   21313987 *
####   *Baskets :      164 : Basket Size=    3200000 bytes  Compression=   1.55     *
####   *............................................................................*
####   *Br    5 :etSumMeanVsRefRMS : etSumMeanVsRefRMS/F                            *
####   *Entries :  8271120 : Total  Size=   33103026 bytes  File Size  =   24503351 *
####   *Baskets :      164 : Basket Size=    3200512 bytes  Compression=   1.35     *
####   *............................................................................*
####   *Br    6 :etMean    : etMean/F                                               *
####   *Entries :  8271120 : Total  Size=   33101178 bytes  File Size  =   21973978 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression=   1.51     *
####   *............................................................................*
####   *Br    7 :etMeanRMS : etMeanRMS/F                                            *
####   *Entries :  8271120 : Total  Size=   33101682 bytes  File Size  =   24581972 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression=   1.35     *
####   *............................................................................*
####   *Br    8 :lcMean    : lcMean/F                                               *
####   *Entries :  8271120 : Total  Size=   33101178 bytes  File Size  =   22962834 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression=   1.44     *
####   *............................................................................*
####   *Br    9 :lcMeanRMS : lcMeanRMS/F                                            *
####   *Entries :  8271120 : Total  Size=   33101682 bytes  File Size  =   25791679 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression=   1.28     *
####   *............................................................................*
####   *Br   10 :alphaMean : alphaMean/F                                            *
####   *Entries :  8271120 : Total  Size=   33101682 bytes  File Size  =   13761560 *
####   *Baskets :      164 : Basket Size=    3199488 bytes  Compression=   2.41     *
####   *............................................................................*
####   *Br   11 :alphaMeanRMS : alphaMeanRMS/F                                      *
####   *Entries :  8271120 : Total  Size=   33102186 bytes  File Size  =   23708523 *
####   *Baskets :      164 : Basket Size=    3200000 bytes  Compression=


    # Reading histories per region
    intervalsMap=getIntervalsMap(options.intervalFile)
    if (options.IC!=""):
        print "Reading absolute IC from file "+options.IC
        ICfile = open(options.IC)
        getIC( ICfile.read())
    histories = [ [(0,0) for i in range(len(intervalsMap)) ] for j in range(1,nRegions(options.regionType)+2) ]
    print "Reading histories from file "+options.infile
    input = TFile.Open(options.infile)
    tree = input.Get("historyTree")
    nentries=tree.GetEntries()
    for jentry in xrange(nentries):
        ientry = tree.LoadTree(jentry)
        if ientry < 0:
            break
        nb = tree.GetEntry(jentry)
        if (jentry%50000 == 0):
            print "%3.1f done" % (float(jentry)/float(nentries)*100)
        if nb<=0:
            continue
        if (tree.indexType != indexType[options.regionType] ):
            continue

#        print tree.index,tree.timeInterval
        histories[int(tree.index)][int(tree.timeInterval)]=(float(tree.etSumMeanVsRef),float(tree.etSumMeanVsRefRMS))
        

    # Rescaling the histories to have mean = 1
    #    indices = [ x for (x,y) in histories.keys() ]
    #    allIntervals = [ y for (x,y) in histories.keys() ]
    #    uniqIndices = set(indices)
    uniqIndices = range(1,nRegions(options.regionType)+1)
    #    uniqIntervals = set(allIntervals)
    uniqIntervals = range(len(intervalsMap))
    #    print "Found " + str(len(uniqIndices)) + " regions X "+ str(len(uniqIntervals)) + " intervals to be monitored"
    #
    for region in uniqIndices:
#        intervals = [ y for (x,y) in histories.keys() if x==region ]
#        intervals.sort()
#
        data = np.zeros(len(uniqIntervals))
        err_data = np.zeros(len(uniqIntervals))

#        if (len(intervals) != len(uniqIntervals) ):
#            print "***** Error ****** region "+str(region)+" has a strange number of intervals. Setting all values to 1"
#            data = np.ones(len(uniqIntervals))
#
        for y in uniqIntervals:
            if (histories[region][y][0] != histories[region][y][0]):
                continue
            data[y]=histories[region][y][0]
            err_data[y]=histories[region][y][1]
#            print data[y]
        cleanData=np.trim_zeros(data)
        cleanDataErr=np.trim_zeros(err_data)

        #truncated mean calculation
        mean=cleanData.mean()
        std=cleanData.std()
        mean_err=cleanDataErr.mean()
        if mean != 0 and len(cleanData)>0:
            for iteration in range(5): # iterative truncated mean
                try:
                    newmean=scipy.stats.tmean(cleanData,(mean-3*std,mean+3*std))
                    newstd=scipy.stats.tstd(cleanData,(mean-3*std,mean+3*std))
                #                print abs(newmean-mean),abs(newstd-std)
                    mean=newmean
                    std=newstd
                except:
                    mean=mean
                    std=std
                    break
            region_mean=mean
#            print region_mean,std,mean_err
            if (region_mean != 0):
                data *= 1/region_mean #renormalize to the mean value
                print "********** "+str(region)+" mean: "+ "%5.4f" % (region_mean) + " rms: "+ "%6.5f" % (std)   + " stabilitySignificance: " + "%4.2f" % (std/(1.2*mean_err)) 
            else:
                print "********** "+str(region)+" mean is 0! setting all corrections to 1."
                data = np.ones(len(data))
        else:
            print "********** "+str(region)+" mean is 0! setting all corrections to 1."
            data = np.ones(len(data))

        # Correcting strange values (either 0 or outside 5*RMS)
        for y in uniqIntervals:
            if (data[y]==0 or abs(data[y]-region_mean)>5*data.std()): #either nan or crazy values
                data_good=0
                err_data_good=0
                n_data_good=0
                if ((y-1)>-1):
                    if (data[y-1]!=0 and abs(data[y-1]-region_mean)<5*std): #avoid boundaries and crazy values
                        data_good=data[y-1]+data_good
                        err_data_good=err_data[y-1]+err_data_good
                        n_data_good=1+n_data_good
                if ((y+1)<len(uniqIntervals)):
                    if (data[y+1]!=0 and abs(data[y+1]-region_mean)<5*std): #avoid boundaries and crazy values
                        data_good=data[y+1]+data_good
                        err_data_good=err_data[y+1]+err_data_good
                        n_data_good=1+n_data_good
                if (n_data_good == 0):
                    data[y]=1
                    err_data[y]=0
                else:
                    data[y]=data_good/n_data_good
                    err_data[y]=err_data_good/n_data_good #just taking the mean of the errors
                    
            if ((std/(1.2*mean_err))>3): #correct only regions where it makes sense to do it
                data[y]=1/data[y] #want IC correction
                # If still strange values set them to 1.
                if data[y]>1.2:
                    data[y]=1.
                elif data[y]<0.8:
                    data[y]=1.
            else: #no correction for stable regions
                data[y]=1.
                err_data[y]=0.
            #            print "%d %5.4f" % (y,data[y])
            histories[region][y]=(data[y],err_data[y])
                                       #    print histories


    # Writing out all values
    sortedIntervals=list(uniqIntervals)
    sortedIntervals.sort()
    sortedIndices=list(uniqIndices)
    sortedIndices.sort()
    outHistos=TFile("ICcorrections_"+options.globalSuffix+"_"+options.regionType+".root","RECREATE")

    rmsVsInterval=array('d')
    rmsErrVsInterval=array('d')
    meanVsInterval=array('d')
    meanErrVsInterval=array('d')
    intervalsArray=array('d')
    intervalsErrArray=array('d')

    set_palette()
    c1 = TCanvas("c1","c1",1024,768)
    for y in sortedIntervals:
        intervalsArray.append(y)
        intervalsErrArray.append(0)
        runStart= intervalsMap[y][0][0] 
        if (y-1>-1):
            if (intervalsMap[y][0][0]==intervalsMap[y-1][1][0] and intervalsMap[y][0][1]>=500):
                runStart=intervalsMap[y][0][0]+1
        else:
            runStart=1
        runEnd=intervalsMap[y][1][0]        
        if (y+1<len(sortedIntervals)):
            if (intervalsMap[y][1][0] == intervalsMap[y+1][0][0] and intervalsMap[y][1][1]<500):
                runEnd=intervalsMap[y][1][0]-1
        else:
                runEnd=999999
        intervalID=str(runStart)+"_"+str(runEnd)
        fileName="ICcorrections_"+options.globalSuffix+"_"+options.regionType+"_"+intervalID+".txt"
        out=open(fileName,"w")
        histoCorrection=TH1F("histoCorrection_"+options.regionType+"_"+intervalID,"histoCorrection_"+options.regionType+"_"+intervalID,2000,0.9,1.1)
        if (options.regionType.find("EB") != -1):
            histoCorrectionMap=TH2F("histoCorrectionMap_"+options.globalSuffix+"_"+options.regionType+"_"+intervalID,"histoCorrectionMap_"+options.globalSuffix+"_"+options.regionType+"_"+intervalID,360,0.5,360.5,171,-85.5,85.5)
            histoCorrectionMap.GetXaxis().SetTitle("i#phi")
            histoCorrectionMap.GetYaxis().SetTitle("i#eta")
        elif (options.regionType.find("EE") != -1):
            histoCorrectionMap=TH2F("histoCorrectionMap_"+options.globalSuffix+"_"+options.regionType+"_"+intervalID,"histoCorrectionMap_"+options.globalSuffix+"_"+options.regionType+"_"+intervalID,200,0.5,200.5,100,0.5,100.5)
            histoCorrectionMap.GetXaxis().SetTitle("ix")
            histoCorrectionMap.GetYaxis().SetTitle("iy")
        for region in sortedIndices:
            histoCorrection.Fill(histories[region][y][0])
            xtalsInRegion=getXtalInRegion(region,options.regionType)
            for xtal in xtalsInRegion:
                if (options.regionType.find("EB") != -1):
                    i1=xtal[1]
                    i2=xtal[0]
                elif (options.regionType.find("EE") != -1):
                    i1=xtal[0]+(xtal[2]>0)*100
                    i2=xtal[1]
                ic=1
                ic_err=histories[region][y][1]
                if (options.IC!=""):
                    if (options.regionType.find("EB") != -1):
                        ic=EB_IC[(xtal[0],xtal[1])][0]
                        ic_err=EB_IC[(xtal[0],xtal[1])][1]
                    elif (options.regionType.find("EE") != -1):
                        ic=EE_IC[(xtal[0],xtal[1],xtal[2])][0]
                        ic_err=EE_IC[(xtal[0],xtal[1],xtal[2])][1]
                histoCorrectionMap.Fill(i1,i2,histories[region][y][0]*ic)
                out.write("\t%d\t%d\t%d\t%6.5f\t%6.5f\n"% (xtal[0],xtal[1],xtal[2],histories[region][y][0]*ic,ic_err))
        out.close()
        rmsVsInterval.append(histoCorrection.GetRMS())
        rmsErrVsInterval.append(histoCorrection.GetRMSError())
        meanVsInterval.append(histoCorrection.GetMean())
        meanErrVsInterval.append(histoCorrection.GetMeanError())
        histoCorrection.Write()
        if (options.IC!=""):
            histoCorrectionMap.GetZaxis().SetRangeUser(0.5,1.5)
        else:
            histoCorrectionMap.GetZaxis().SetRangeUser(histoCorrection.GetMean()-5*histoCorrection.GetRMS(),histoCorrection.GetMean()+5*histoCorrection.GetRMS())
        histoCorrectionMap.SetStats(False)
        histoCorrectionMap.Draw("COLZ")
        c1.SaveAs("histoCorrectionMap_"+options.globalSuffix+"_"+options.regionType+"_"+intervalID+".png")
        histoCorrectionMap.Write()
        print "Written output file "+fileName
    rmsVsIntervalGraph=TGraphErrors(len(sortedIntervals),intervalsArray,rmsVsInterval,intervalsErrArray,rmsErrVsInterval)
    meanVsIntervalGraph=TGraphErrors(len(sortedIntervals),intervalsArray,meanVsInterval,intervalsErrArray,meanErrVsInterval)
    rmsVsIntervalGraph.Write("rmsVsInterval")
    meanVsIntervalGraph.Write("meanVsInterval")
    outHistos.Close()


if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--infile",
                    action="store", type="string", dest="infile",
                    default=" /xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012_v6_12000M_alphaFit_newLaser_2012dataBsCorr_KFactorsVsTime_fullTree.root",
                    help="", metavar=""
                    ),
        make_option("-I", "--IC",
                    action="store", type="string", dest="IC",
                    default="",
                    help="", metavar=""
                    ),
        make_option("-j", "--intervalFile",
                    action="store", type="string", dest="intervalFile",
                    default="readMap_AlCaPhiSym_Run2012_v6_12000M_alphaFit_newLaser.root",
                    help="", metavar=""
                    ),
        make_option("-e", "--eeMapFile",
                    action="store", type="string", dest="eeMapFile",
                    default="data/eeIndicesMap.root",
                    help="", metavar=""
                    ),
        make_option("-m", "--harnessMapFile",
                    action="store", type="string", dest="harnessMapFile",
                    default="data/harnessMap.root",
                    help="", metavar=""
                    ),
        make_option("-r", "--regionType",
                    action="store", type="string", dest="regionType",
                    default="EB_ring",
                    help="", metavar=""
                    ),
        make_option("-g", "--globalSuffix",
                    action="store", type="string", dest="globalSuffix",
                    default="test",
                    help="", metavar=""
                    ),
        ])
    
    (options, args) = parser.parse_args()
    
    main( options, args) 
