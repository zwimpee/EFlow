#!/usr/bin/env python

import collections
from ROOT import TTree, TFile

class intervalMap:    
    def __init__(self, mapFile):
        self.mapFile=TFile.Open(mapFile,'read')
        intervals={}
        for interval in self.mapFile.outTree_barl:
            intervals[interval.index]={"firstRun":interval.firstRun,"firstLumi":interval.firstLumi,"lastRun":interval.lastRun,"lastLumi":interval.lastLumi}
        self.intervals=collections.OrderedDict(sorted(intervals.items()))
        print "Initialized intervalMap from "+str(mapFile)

    def intervals(self):
        return self.intervals

    def size(self):
        return len(self.intervals)

    def intervalIndex(self, run, lumi):
        for key,value in self.intervals.iteritems():
            if run>=value["firstRun"] and run<=value["lastRun"]:
                if (value["firstRun"] != value["lastRun"]):
                    return key
                elif lumi>=value["firstLumi"] and lumi<=value["lastLumi"]:
                    return key
        return -1
                
