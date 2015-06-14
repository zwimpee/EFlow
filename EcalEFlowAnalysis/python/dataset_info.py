#!/usr/bin/env python
# PLEASE NOTE: to use this example download das_client.py from
# cmsweb.cern.ch/das/cli

# system modules
import os
import sys
import json
from optparse import OptionParser, make_option

from das_cli import get_data

def drop_das_fields(row):
    "Drop DAS specific headers in given row"
    for key in ['das', 'das_id', 'cache_id', 'qhash']:
        if  row.has_key(key):
            del row[key]

def get_info(query):
    "Helper function to get information for given query"
    host    = 'https://cmsweb.cern.ch'
    idx     = 0
    limit   = 0
    debug   = False
    data    = get_data(host, query, idx, limit, debug)
    if  isinstance(data, basestring):
        dasjson = json.loads(data)
    else:
        dasjson = data
    status  = dasjson.get('status')
    if  status == 'ok':
        data = dasjson.get('data')
        return data

def get_datasets(query):
    "Helper function to get list of datasets for given query pattern"
    for row in get_info(query):
        for dataset in row['dataset']:
            yield dataset['name']

def get_summary(query):
    """
    Helper function to get dataset summary information either for a single
    dataset or dataset pattern
    """
    if  query.find('*') == -1:
#        print "\n### query", query
        data = get_info(query)
#        print data
        for row in data:
            drop_das_fields(row)
            for key in row.keys():
                if (key=='file'):
                    files=row[key]
                    for file in files: 
                        print file['name']
    else:
        print "NOT SUPPORTED"
#        for dataset in get_datasets(query):
#            query = "dataset=%s" % dataset
#            data = get_info(query)
#            print "\n### dataset", dataset
##            print data
#            for row in data:
#                drop_das_fields(row)
#                print row.key()

if __name__ == '__main__':
    parser = OptionParser(option_list=[
            make_option("-d", "--dataset",
                        action="store", type="string",
                        default="/AlCaPhiSym/spigazzi-crab_PHISYM_test_production_Run2015A_v1-b3a63a3628794e07f253ccb3bf8017ab/USER",
                        help="Dataset name", metavar=""
                        ),
        ])
    
    (options, args) = parser.parse_args()
    # query dataset pattern
#    query = "dataset=/ZMM*/*/*"
    # query specific dataset in certain DBS instance
#    query = "dataset=/8TeV_T2tt_2j_semilepts_200_75_FSim526_Summer12_minus_v2/alkaloge-MG154_START52_V9_v2/USER instance=cms_dbs_ph_analysis_02"
    query = "file,run,lumi dataset="+options.dataset+" instance=prod/phys03 status=valid"
    get_summary(query)
