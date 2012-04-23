#define createLastTree_bs_cxx
#include "createLastTree_bs.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void createLastTree_bs::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L createLastTree_bs.C
//      Root > createLastTree_bs t
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
  
  std::vector<bsInfo> bsInfos;
  int kIntervals=intervals->numberOfIntervals();
  bsInfos.reserve(kIntervals);
  
  cout<<"creating histos"<<endl;
  for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
    bsInfos[iinterval].reset();
  }
  
  
   //######### creating the output tree ##############
   TFile *outFile=TFile::Open(outFileName,"RECREATE");
   int timeVar=0;
   unsigned int nEventsVar=0;
   float bsPosVar=0,bsWidVar=0;
   
   outFile->cd();
   TTree* bsInfoTree= new TTree("bsFinalTree","bsFinalTree");
   bsInfoTree->Branch("time_interval",&timeVar,"timeInterval/I");
   bsInfoTree->Branch("nEvents",&nEventsVar,"nEvents/i");
   bsInfoTree->Branch("bsPos",&bsPosVar, "bsPosVar/F");
   bsInfoTree->Branch("bsWid",&bsWidVar, "bsWidVar/F");
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     int theInterval = time_interval;
     if (theInterval>=0 && theInterval <=kIntervals)
       {
	 bsInfos[theInterval-1].bsPos+=bsPos;
	 bsInfos[theInterval-1].bsWid+=bsWid;
	 bsInfos[theInterval-1].nEvents+=nEvents;
       } 
     // if (Cut(ientry) < 0) continue;
   }
   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     nEventsVar=bsInfos[iinterval].nEvents;
     timeVar=iinterval+1;
     bsPosVar=bsInfos[iinterval].bsPos/(float)nEventsVar;
     bsWidVar=bsInfos[iinterval].bsWid/(float)nEventsVar;
     bsInfoTree->Fill();
   }

   bsInfoTree->Write();
   outFile->Write();
   outFile->Close();

}
