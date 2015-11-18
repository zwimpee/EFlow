#!/usr/bin/env python

import collections
from ROOT import TTree, TFile

class runLS:
    def __init__(self,run,lumi):
        self.run=run
        self.lumi=lumi

    def __str__(self):
        print "("+str(self.run)+":"+str(self.lumi)+")"

    def __lt__(self, other):
        return (self.run < other.run or (self.run==other.run and self.lumi < other.lumi) )

    def __gt__(self, other):
        return (self.run > other.run or (self.run==other.run and self.lumi > other.lumi) )

    def __eq__(self, other):
        return (self.run == other.run and self.lumi == other.lumi)

    def __le__(self, other):
        return self<other or self==other

    def __ge__(self, other):
        return self>other or self==other

    def __ne__(self, other):
        return not self==other


class intervalMap:    
    def __init__(self, mapFile):
        self.mapFile=TFile.Open(mapFile,'read')
        intervals={}
        for interval in self.mapFile.outTree_barl:
            intervals[interval.index]={"begin":runLS(interval.firstRun,interval.firstLumi),"end":runLS(interval.lastRun,interval.lastLumi)}
        self.intervals=collections.OrderedDict(sorted(intervals.items()))
        print "Initialized intervalMap from "+str(mapFile)

    def intervals(self):
        return self.intervals

    def size(self):
        return len(self.intervals)

    def intervalIndex(self, run, lumi):
        aRun=runLS(run,lumi)
        for key,value in self.intervals.iteritems():
            if aRun>=value["begin"] and aRun<=value["end"]:
                return key
        return -1
                
