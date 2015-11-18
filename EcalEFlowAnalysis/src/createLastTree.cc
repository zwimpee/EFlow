#define createLastTree_cxx
#include "interface/createLastTree.h"
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

using namespace std;


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
  std::vector<histosLastTree_ee> sums_ee;
  int kIntervals=intervals->numberOfIntervals();
  sums.reserve(kIntervals);
  sums_ee.reserve(kIntervals);
  
  cout<<"creating histos"<<endl;
  for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
    sums[iinterval].reset();
    sums_ee[iinterval].reset();
  }

  cout<<"creating outtree"<<endl;
   TFile *outFile=TFile::Open(outFileName,"RECREATE");
   int timeVar=0,hitVar=0,ietaVar=0,iphiVar=0,signVar=0;
   unsigned int detVar=0; //EB=0 ; EE=1 
   float etVar=0,RMSetVar=0;//,etNoCorrVar=0,RMSetNoCorrVar=0;
   // float etVarA=0,RMSetVarA=0,etNoCorrVarA=0,RMSetNoCorrVarA=0;
   // float etVarB=0,RMSetVarB=0,etNoCorrVarB=0,RMSetNoCorrVarB=0;

   float lcVar=0,RMSlcVar=0;
   // float alphaVar=0,RMSalphaVar=0;
   TTree* newTree=new TTree("finalTree_barl","tree with et and lc");
   newTree->Branch("time_interval",&timeVar,"timeInterval/I");
   newTree->Branch("nHits",&hitVar, "nHits/i");
   newTree->Branch("det",&detVar, "det/i");   // EB => 0    EE=> 1
   newTree->Branch("ieta",&ietaVar,"ieta/I"); // ieta = ieta in EB, ix in EE
   newTree->Branch("iphi",&iphiVar,"iphi/I"); // iphi = iphi in EB, iy in EE
   newTree->Branch("sign",&signVar,"sign/I"); // - => 0  + => 1 
   newTree->Branch("et",&etVar,"et/F");
   newTree->Branch("RMSet",&RMSetVar,"RMSet/F");
   newTree->Branch("lc",&lcVar,"lc/F");
   newTree->Branch("RMSlc",&RMSlcVar,"RMSlc/F");

   cout << "starting loop" << endl;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if ((ientry+1)%10000==0)
	cout << "Reading " << ientry-1 << "/" << nentries << endl;

     int theSign=sign;
     int theEta=ieta;
     int thePhi=iphi;
     int theInterval = time_interval;

#ifdef DEBUG
     std::cout << "time_interval " << theInterval
	       << "\tdet " << det
	       << "\tieta " << ieta
	       << "\tiphi " << iphi
	       << "\tsign " << theSign
	       << endl;
#endif

     if (det==0) //EB
       {
	 if(theSign < kSides && thePhi <=kBarlWedges && theInterval>=0 && theInterval <=kIntervals && theEta <=kBarlRings ){
	   sums[theInterval].energySum[theEta-1][thePhi-1][theSign]+=energySum;
	   sums[theInterval].energySquared[theEta-1][thePhi-1][theSign]+=energySquared;
	   // sums[theInterval].energyNoCorrSum[theEta-1][thePhi-1][theSign]+=energyNoCorrSum;
	   // sums[theInterval].energyNoCorrSquared[theEta-1][thePhi-1][theSign]+=energyNoCorrSquared;
	   
	   // sums[theInterval].energySumA[theEta-1][thePhi-1][theSign]+=energySumA;
	   // sums[theInterval].energySquaredA[theEta-1][thePhi-1][theSign]+=energySquaredA;
	   // sums[theInterval].energyNoCorrSumA[theEta-1][thePhi-1][theSign]+=energyNoCorrSumA;
	   // sums[theInterval].energyNoCorrSquaredA[theEta-1][thePhi-1][theSign]+=energyNoCorrSquaredA;
	   
	   // sums[theInterval].energySumB[theEta-1][thePhi-1][theSign]+=energySumB;
	   // sums[theInterval].energySquaredB[theEta-1][thePhi-1][theSign]+=energySquaredB;
	   // sums[theInterval].energyNoCorrSumB[theEta-1][thePhi-1][theSign]+=energyNoCorrSumB;
	   // sums[theInterval].energyNoCorrSquaredB[theEta-1][thePhi-1][theSign]+=energyNoCorrSquaredB;
	   
	   sums[theInterval].lasercorrSum[theEta-1][thePhi-1][theSign]+=lcSum;
	   sums[theInterval].lasercorrSquared[theEta-1][thePhi-1][theSign]+=lcSquared;

	   // sums[theInterval].alphaSum[theEta-1][thePhi-1][theSign]+=alphaSum;
	   // sums[theInterval].alphaSquared[theEta-1][thePhi-1][theSign]+=alphaSquared;

	   sums[theInterval].nhit[theEta-1][thePhi-1][theSign]+=nHits;
	 }
       }


     if (det==1) //EE
       {
	 if(theSign < kSides && thePhi <=kY && theInterval>=0 && theInterval <=kIntervals && theEta <=kX ){
	   sums_ee[theInterval].energySum[theEta-1][thePhi-1][theSign]+=energySum;
	   sums_ee[theInterval].energySquared[theEta-1][thePhi-1][theSign]+=energySquared;
	   // sums_ee[theInterval].energyNoCorrSum[theEta-1][thePhi-1][theSign]+=energyNoCorrSum;
	   // sums_ee[theInterval].energyNoCorrSquared[theEta-1][thePhi-1][theSign]+=energyNoCorrSquared;
	   
	   // sums_ee[theInterval].energySumA[theEta-1][thePhi-1][theSign]+=energySumA;
	   // sums_ee[theInterval].energySquaredA[theEta-1][thePhi-1][theSign]+=energySquaredA;
	   // sums_ee[theInterval].energyNoCorrSumA[theEta-1][thePhi-1][theSign]+=energyNoCorrSumA;
	   // sums_ee[theInterval].energyNoCorrSquaredA[theEta-1][thePhi-1][theSign]+=energyNoCorrSquaredA;
	   
	   // sums_ee[theInterval].energySumB[theEta-1][thePhi-1][theSign]+=energySumB;
	   // sums_ee[theInterval].energySquaredB[theEta-1][thePhi-1][theSign]+=energySquaredB;
	   // sums_ee[theInterval].energyNoCorrSumB[theEta-1][thePhi-1][theSign]+=energyNoCorrSumB;
	   // sums_ee[theInterval].energyNoCorrSquaredB[theEta-1][thePhi-1][theSign]+=energyNoCorrSquaredB;
	   
	   sums_ee[theInterval].lasercorrSum[theEta-1][thePhi-1][theSign]+=lcSum;
	   sums_ee[theInterval].lasercorrSquared[theEta-1][thePhi-1][theSign]+=lcSquared;

	   // sums_ee[theInterval].alphaSum[theEta-1][thePhi-1][theSign]+=alphaSum;
	   // sums_ee[theInterval].alphaSquared[theEta-1][thePhi-1][theSign]+=alphaSquared;

	   sums_ee[theInterval].nhit[theEta-1][thePhi-1][theSign]+=nHits;
	 }
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
	     detVar=0; //EB
	     etVar=sums[iinterval].energySum[iieta][iiphi][iisign];
	     RMSetVar=sqrt(TMath::Abs(sums[iinterval].energySquared[iieta][iiphi][iisign]/N-pow(etVar/float(N),2)));
	     // etNoCorrVar=sums[iinterval].energyNoCorrSum[iieta][iiphi][iisign];
	     // RMSetNoCorrVar=sqrt(TMath::Abs(sums[iinterval].energyNoCorrSquared[iieta][iiphi][iisign]/N-pow(etNoCorrVar/float(N),2)));

	     // etVarA=sums[iinterval].energySumA[iieta][iiphi][iisign];
	     // RMSetVarA=-999.; //To be fixed
	     // etNoCorrVarA=sums[iinterval].energyNoCorrSumA[iieta][iiphi][iisign];
	     // RMSetNoCorrVarA=-999.;

	     // etVarB=sums[iinterval].energySumB[iieta][iiphi][iisign];
	     // RMSetVarB=-999.;
	     // etNoCorrVarB=sums[iinterval].energyNoCorrSumB[iieta][iiphi][iisign];
	     // RMSetNoCorrVarB=-999.;

	     lcVar=sums[iinterval].lasercorrSum[iieta][iiphi][iisign];
	     RMSlcVar=sqrt(TMath::Abs(sums[iinterval].lasercorrSquared[iieta][iiphi][iisign]/N - pow ( lcVar/float(N) ,2) ));

	     // alphaVar=sums[iinterval].alphaSum[iieta][iiphi][iisign];
	     // RMSalphaVar=sqrt(TMath::Abs(sums[iinterval].alphaSquared[iieta][iiphi][iisign]/N - pow ( alphaVar/float(N) ,2) ));

	     hitVar=N;
	     newTree->Fill();
	   }
	 }
	 
       }
     }
   }


   for(int iisign=0;iisign<kSides;++iisign){
     for(int iieta=0;iieta<kX;++iieta){
      
       for(int iiphi=0;iiphi<kY;++iiphi){
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  
	   if(sums_ee[iinterval].nhit[iieta][iiphi][iisign]>0 ){
	     int N=sums_ee[iinterval].nhit[iieta][iiphi][iisign];
	     ietaVar=iieta+1;
	     iphiVar=iiphi+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     detVar=1; //EE
	     etVar=sums_ee[iinterval].energySum[iieta][iiphi][iisign];
	     RMSetVar=sqrt(TMath::Abs(sums_ee[iinterval].energySquared[iieta][iiphi][iisign]/N-pow(etVar/float(N),2)));
	     // etNoCorrVar=sums_ee[iinterval].energyNoCorrSum[iieta][iiphi][iisign];
	     // RMSetNoCorrVar=sqrt(TMath::Abs(sums_ee[iinterval].energyNoCorrSquared[iieta][iiphi][iisign]/N-pow(etNoCorrVar/float(N),2)));

	     // etVarA=sums_ee[iinterval].energySumA[iieta][iiphi][iisign];
	     // RMSetVarA=-999.; //To be fixed
	     // etNoCorrVarA=sums_ee[iinterval].energyNoCorrSumA[iieta][iiphi][iisign];
	     // RMSetNoCorrVarA=-999.;

	     // etVarB=sums_ee[iinterval].energySumB[iieta][iiphi][iisign];
	     // RMSetVarB=-999.;
	     // etNoCorrVarB=sums_ee[iinterval].energyNoCorrSumB[iieta][iiphi][iisign];
	     // RMSetNoCorrVarB=-999.;

	     lcVar=sums_ee[iinterval].lasercorrSum[iieta][iiphi][iisign];
	     RMSlcVar=sqrt(TMath::Abs(sums_ee[iinterval].lasercorrSquared[iieta][iiphi][iisign]/N - pow ( lcVar/float(N) ,2) ));

	     // alphaVar=sums_ee[iinterval].alphaSum[iieta][iiphi][iisign];
	     // RMSalphaVar=sqrt(TMath::Abs(sums_ee[iinterval].alphaSquared[iieta][iiphi][iisign]/N - pow ( alphaVar/float(N) ,2) ));

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

