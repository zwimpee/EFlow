#define createLastTree_cxx
#include "createLastTree.h"
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
#include "TTreeIndex.h"
#include "TChainIndex.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>




void createLastTree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L createLastTree.C
//      Root > createLastTree t
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
  if (!intervals)
    return;
  if (fChain == 0) return;
  cout<<"starting"<<endl;
  Long64_t nentries = fChain->GetEntries();
  cout<<"nentries "<<nentries<<endl;
  
  std::vector<histosLastTree> sums;
  int kIntervals=intervals->numberOfIntervals();
  sums.reserve(kIntervals);
  
  cout<<"creating histos"<<endl;
  for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
    sums[iinterval].reset();
  }


   TFile *outFile=TFile::Open(outFileName,"recreate");
   int timeVar=0,hitVar=0,ietaVar=0,iphiVar=0,signVar=0;
   float etVar=0,RMSetVar=0,etNoCorrVar=0,RMSetNoCorrVar=0;
   float etVarA=0,RMSetVarA=0,etNoCorrVarA=0,RMSetNoCorrVarA=0;
   float etVarB=0,RMSetVarB=0,etNoCorrVarB=0,RMSetNoCorrVarB=0;

   float lcVar=0,RMSlcVar=0;
   TTree* newTree=new TTree("finalTree_barl","tree with et and lc");
   newTree->Branch("time_interval",&timeVar,"timeInterval/I");
   newTree->Branch("nHits",&hitVar, "nHits/i");
   newTree->Branch("ieta",&ietaVar,"ieta/I");
   newTree->Branch("iphi",&iphiVar,"iphi/I");
   newTree->Branch("sign",&signVar,"sign/I");
   newTree->Branch("et",&etVar,"et/F");
   newTree->Branch("RMSet",&RMSetVar,"RMSet/F");
   newTree->Branch("etNoCorr",&etNoCorrVar,"etNoCorr/F");
   newTree->Branch("RMSetNoCorr",&RMSetNoCorrVar,"RMSetNoCorr/F");
   newTree->Branch("etA",&etVarA,"etA/F");
   newTree->Branch("RMSetA",&RMSetVarA,"RMSetA/F");
   newTree->Branch("etNoCorrA",&etNoCorrVarA,"etNoCorrA/F");
   newTree->Branch("RMSetNoCorrA",&RMSetNoCorrVarA,"RMSetNoCorrA/F");
   newTree->Branch("etB",&etVarB,"etB/F");
   newTree->Branch("RMSetB",&RMSetVarB,"RMSetB/F");
   newTree->Branch("etNoCorrB",&etNoCorrVarB,"etNoCorrB/F");
   newTree->Branch("RMSetNoCorrB",&RMSetNoCorrVarB,"RMSetNoCorrB/F");
   newTree->Branch("lc",&lcVar,"lc/F");
   newTree->Branch("RMSlc",&RMSlcVar,"RMSlc/F");


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


     int theSign=sign;
     int theEta=ieta;
     int thePhi=iphi;
     int theInterval = time_interval;

     if(theSign < kSides && thePhi <=kBarlWedges && theInterval>=0 && theInterval <=kIntervals && theEta <=kBarlRings ){
       sums[theInterval-1].energySum[theEta-1][thePhi-1][theSign]+=energySum;
       sums[theInterval-1].energySquared[theEta-1][thePhi-1][theSign]+=energySquared;
       sums[theInterval-1].energyNoCorrSum[theEta-1][thePhi-1][theSign]+=energyNoCorrSum;
       sums[theInterval-1].energyNoCorrSquared[theEta-1][thePhi-1][theSign]+=energyNoCorrSquared;

       sums[theInterval-1].energySumA[theEta-1][thePhi-1][theSign]+=energySumA;
       sums[theInterval-1].energySquaredA[theEta-1][thePhi-1][theSign]+=energySquaredA;
       sums[theInterval-1].energyNoCorrSumA[theEta-1][thePhi-1][theSign]+=energyNoCorrSumA;
       sums[theInterval-1].energyNoCorrSquaredA[theEta-1][thePhi-1][theSign]+=energyNoCorrSquaredA;

       sums[theInterval-1].energySumB[theEta-1][thePhi-1][theSign]+=energySumB;
       sums[theInterval-1].energySquaredB[theEta-1][thePhi-1][theSign]+=energySquaredB;
       sums[theInterval-1].energyNoCorrSumB[theEta-1][thePhi-1][theSign]+=energyNoCorrSumB;
       sums[theInterval-1].energyNoCorrSquaredB[theEta-1][thePhi-1][theSign]+=energyNoCorrSquaredB;

       sums[theInterval-1].lasercorrSum[theEta-1][thePhi-1][theSign]+=lcSum;
       sums[theInterval-1].lasercorrSquared[theEta-1][thePhi-1][theSign]+=lcSquared;
       sums[theInterval-1].nhit[theEta-1][thePhi-1][theSign]+=nHits;
     }
   }

   for(int iisign=0;iisign<kSides;++iisign){
     for(int iieta=0;iieta<kBarlRings;++iieta){
      
       for(int iiphi=0;iiphi<kBarlWedges;++iiphi){
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  
	   if(sums[iinterval].nhit[iieta][iiphi][iisign]>0 ){
	     int N=sums[iinterval].nhit[iieta][iiphi][iisign];
	     ietaVar=iieta+1;
	     iphiVar=iiphi+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     etVar=sums[iinterval].energySum[iieta][iiphi][iisign];
	     RMSetVar=sqrt(sums[iinterval].energySquared[iieta][iiphi][iisign]/N-pow(etVar/float(N),2));
	     etNoCorrVar=sums[iinterval].energyNoCorrSum[iieta][iiphi][iisign];
	     RMSetNoCorrVar=sqrt(sums[iinterval].energyNoCorrSquared[iieta][iiphi][iisign]/N-pow(etNoCorrVar/float(N),2));

	     etVarA=sums[iinterval].energySumA[iieta][iiphi][iisign];
	     RMSetVarA=-999.; //To be fixed
	     etNoCorrVarA=sums[iinterval].energyNoCorrSumA[iieta][iiphi][iisign];
	     RMSetNoCorrVarA=-999.;

	     etVarB=sums[iinterval].energySumB[iieta][iiphi][iisign];
	     RMSetVarB=-999.;
	     etNoCorrVarB=sums[iinterval].energyNoCorrSumB[iieta][iiphi][iisign];
	     RMSetNoCorrVarB=-999.;

	     lcVar=sums[iinterval].lasercorrSum[iieta][iiphi][iisign];
	     RMSlcVar=sqrt(sums[iinterval].lasercorrSquared[iieta][iiphi][iisign]/N-pow(lcVar/float(N),2));
	     hitVar=N;
	     newTree->Fill();
	   }
	 }
	 
       }
     }
   }



   newTree->Write();
   outFile->Write();
   outFile->Close();
   
}
