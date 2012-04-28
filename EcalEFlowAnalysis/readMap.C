#define readMap_cxx
#include "readMap.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <TChain.h>
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TF1.h"
#include "TH1.h"
#include "THStack.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <map>

using namespace std;

void readMap::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L readMap.C
//      Root > readMap t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;

   interval_t currentInterval;
   currentInterval.nHit=0;

   int fullIntervals=0;
   int mergedIntervals=0;
   int shortIntervals=0;
   int isolatedIntervals=0;

   std::map<long int,Long64_t> timeMap; 
   //First loop to fill time ordered map
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      //Checking quality of LS
      if (goodLS && !goodLS->isGoodLS(run,ls))
	continue;
      
      timeMap[unixtime]=jentry;
   }
   std::cout << "timeMap filled" << std::endl;

   for (std::map<long int,Long64_t>::const_iterator it=timeMap.begin(); it!= timeMap.end();++it)
     {
       Long64_t entry=(*it).second;
       std::cout << "Reading entry " << entry << " time " << (*it).first << std::endl;
       Long64_t ientry = LoadTree(entry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(entry);   
       std::cout << "Read" << std::endl;

       if(currentInterval.nHit==0)
	 {

	  //Just define a new interval start
	  currentInterval.unixTimeMean=(unsigned long long)unixtime*(unsigned long long)nhit;
	  currentInterval.unixTimeStart=unixtime;
	  currentInterval.nLS=1;
	  currentInterval.nHit=(unsigned long long)nhit;
	  currentInterval.runStart=run;
	  currentInterval.lsStart=ls;
	  currentInterval.runEnd=run;
	  currentInterval.lsEnd=ls;
	  currentInterval.unixTimeEnd=unixtime;

	}
       else if (currentInterval.nHit+(unsigned long long)nhit>=NTRIGGERMAX)
	{

	  std::cout << "Reached enough statistics" << std::endl;
	  // Enough statistics. Adding as new interval
	  currentInterval.nHit+=(unsigned long long)nhit;
	  currentInterval.nLS++;
	  currentInterval.runEnd=run;
	  currentInterval.lsEnd=ls;
	  currentInterval.unixTimeEnd=unixtime;
	  currentInterval.unixTimeMean+=(unsigned long long)unixtime*(unsigned long long)nhit;
	  currentInterval.unixTimeMean=(long double)(currentInterval.unixTimeMean)/(long double)(currentInterval.nHit);
	  intervals.push_back(currentInterval);
	  currentInterval.nHit=0;
	  fullIntervals++;
	}
      else if ((unixtime-currentInterval.unixTimeStart)>MAXSTOPTIME)
	{
	  std::cout << "Too long interval" << std::endl;
	  // Case when a too long time elapsed between current LS and interval start time
	  if (currentInterval.nHit>=(unsigned long long)NTRIGGERMAX/2)
	    {

	      // Enough statistics. Adding as new interval
	      currentInterval.unixTimeMean=(long double)(currentInterval.unixTimeMean)/(long double)(currentInterval.nHit);
	      intervals.push_back(currentInterval);
	      shortIntervals++;

	    }
	  else
	    {

	      if(fullIntervals != 0){//if first interval is isolated i don't have to do back()
	      interval_t& lastInterval=intervals.back();
	      if ( (currentInterval.unixTimeEnd - lastInterval.unixTimeStart )<MAXSTOPTIME)
		{

		  // Not enough statistics. Still consistent with the last interval, so merge the two intervals
		  currentInterval.unixTimeMean=(long double)(currentInterval.unixTimeMean)/(long double)(currentInterval.nHit);
		  lastInterval.mergeWith(currentInterval);
		  mergedIntervals++;
		}
	      else
		{

		  // Too short and too isolated interval. Adding as new interval, but can be dropped if needed
		  currentInterval.unixTimeMean=(long double)(currentInterval.unixTimeMean)/(long double)(currentInterval.nHit);
		  intervals.push_back(currentInterval);
		  isolatedIntervals++;
		}
	      }else{
                // Too short and too isolated interval. Adding as new interval, but can be dropped if needed                                
                currentInterval.unixTimeMean=(long double)(currentInterval.unixTimeMean)/(long double)(currentInterval.nHit);
                intervals.push_back(currentInterval);
                isolatedIntervals++;
              }


	    }
	  //Now setting this run,ls as a new interval start

	  currentInterval.unixTimeMean=(unsigned long long)unixtime*(unsigned long long)nhit;
	  currentInterval.unixTimeStart=unixtime;
	  currentInterval.nLS=1;
	  currentInterval.nHit=(unsigned long long)nhit;
	  currentInterval.runStart=run;
	  currentInterval.lsStart=ls;
	  currentInterval.runEnd=run;
	  currentInterval.lsEnd=ls;
	  currentInterval.unixTimeEnd=unixtime;
	}
      else
	{

	  //Not reaching an interval definition condition. Adding this LS to the current intervall
	  currentInterval.nHit+=(unsigned long long)nhit;
	  currentInterval.nLS++;
	  currentInterval.runEnd=run;
	  currentInterval.lsEnd=ls;
	  currentInterval.unixTimeEnd=unixtime;
	  currentInterval.unixTimeMean=currentInterval.unixTimeMean+ (unsigned long long)unixtime*(unsigned long long)nhit;
	}

   }

   std::cout << "FOUND " << intervals.size() << " NEW INTERVALS " << std::endl;
   std::cout << "FULL INTERVALS " << fullIntervals << " OF WHICH " << mergedIntervals << " HAVE BEEN EXTENDED" << std::endl;
   std::cout << "SHORTER INTERVALS " << shortIntervals << std::endl;
   std::cout << "ISOLATED INTERVALS " << isolatedIntervals << std::endl;

   TFile* outFile= TFile::Open(outFileName,"recreate");
   outFile->cd();
   TTree* outTree= new TTree("outTree_barl","outTree_barl");

   int indexVar,oldfirstRun,oldlastRun,oldfirstLumi,oldlastLumi,oldunixtimeStart,oldunixtimeEnd,oldunixtimeMean,nLS;
   unsigned long long nHit;


   outTree->Branch("index",&indexVar,"index/I"); 
   outTree->Branch("nHit",&nHit,"nHit/i"); 
   outTree->Branch("nLS",&nLS,"nLS/I"); 
   outTree->Branch("firstRun",&oldfirstRun,"firstRun/I");
   outTree->Branch("lastRun",&oldlastRun,"lastRun/I");
   outTree->Branch("firstLumi",&oldfirstLumi,"firstLumi/I");
   outTree->Branch("lastLumi",&oldlastLumi,"lastLumi/I");
   outTree->Branch("unixtimeStart",&oldunixtimeStart,"unixtimeStart/i");
   outTree->Branch("unixtimeEnd",&oldunixtimeEnd,"unixtimeEnd/i");
   outTree->Branch("unixtimeMean",&oldunixtimeMean,"unixtimeMean/i");

   //Filling the tree
   for (unsigned int i=0;i<intervals.size();++i)
     {
       indexVar=i;
       oldfirstRun=intervals[i].runStart;
       oldfirstLumi=intervals[i].lsStart;
       oldunixtimeStart=intervals[i].unixTimeStart;
       oldlastRun=intervals[i].runEnd;
       oldlastLumi=intervals[i].lsEnd;
       oldunixtimeEnd=intervals[i].unixTimeEnd;
       oldunixtimeMean=intervals[i].unixTimeMean;
       nHit=intervals[i].nHit;
       nLS=intervals[i].nLS;
       outTree->Fill();
     }

   outTree->Write();
   outFile->Write();
   outFile->Close();
																		  
}
