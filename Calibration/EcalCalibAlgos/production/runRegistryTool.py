import os,string,sys,commands,time
import xmlrpclib


def getRunList(minRun):
        runlist = []

        #FULLADDRESS="http://pccmsdqm04.cern.ch/runregistry_api/"
        #FULLADDRESS="http://pccmsdqm04.cern.ch/runregistry/xmlrpc"
        FULLADDRESS="http://cms-service-runregistry-api.web.cern.ch/cms-service-runregistry-api/xmlrpc"
        
        print "RunRegistry from: ",FULLADDRESS
        server = xmlrpclib.ServerProxy(FULLADDRESS)
        # you can use this for single run query
        #    sel_runtable="{runNumber} = "+run+" and {datasetName} LIKE '%Express%'"
        #sel_runtable="{groupName} ='Collisions11' and {runNumber} >= " + str(minRun) + " and {datasetName} LIKE '%Express%'"
        sel_runtable="{groupName} ='Collisions12' and {runNumber} >= " + str(minRun) + " and {datasetName} LIKE '%Online%'"
        
        #sel_runtable="{groupName} ='Commissioning11' and {runNumber} >= " + str(minRun)# + " and {datasetName} LIKE '%Express%'"
        
        run_data = server.DataExporter.export('RUN', 'GLOBAL', 'csv_runs', sel_runtable)
        for line in run_data.split("\n"):
            #print line
            run=line.split(',')[0]
            if "RUN_NUMBER" in run or run == "":
                continue
            #print "RUN: " + run
            runlist.append(int(run))
        return runlist
                                                                                                
list=getRunList(190482)
print list
