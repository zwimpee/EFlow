#!/usr/bin/env python

import collections
from ROOT import TTree, TFile

class intervalMap:    
    def __init__(self, mapFile):
        self.mapFile=ROOT.TFile.Open(mapFile,'read')
        intervals={}
        for interval in self.mapFile.outTree_barl:
            intervals[interval.index]={"firstRun":interval.firstRun,"firstLumi":interval.firstLumi,"lastRun":interval.lastRun,"lastLumi":interval.lastLumi}
        self.intervalMap=collections.OrderedDict(sorted(interval.items()))
        print intervalMap

    def intervalIndex(self, run, lumi):
        for key,value in self.intervalMap.iteritems():
            if run>=value["firstRun"] and run<=value["lastRun"]:
                if (value["firstRun"] != value["lastRun"]):
                    return key
                elif lumi>=value["firstLumi"] and lumi<=value["lastLumi"]:
                    return key
        return -1
                
