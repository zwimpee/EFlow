#define createHistoryPlots_cxx
#include "createHistoryPlots.h"
#include "TMath.h"
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

void createHistoryPlots::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L createHistoryPlots.C
//      Root > createHistoryPlots t
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
  //######Reading the tree with time intervals###########


  std::vector<histos> sums;
  int kIntervals=intervals->numberOfIntervals();
  sums.reserve(kIntervals);

   cout<<"creating histos"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
     sums[iinterval].reset();
   }
   
   //######### creating the output tree ##############
   TFile *outFile=TFile::Open(outFileName,"RECREATE");
   int timeVar=0,hitVar=0,ietaVar=0,iphiVar=0,signVar=0;
   float energyVar=0,RMSenergyVar=0,lcVar=0,RMSlcVar=0;
   float energyNoCorrVar=0,RMSenergyNoCorrVar=0;
   float energyVarA=0,RMSenergyVarA=0;
   float energyNoCorrVarA=0,RMSenergyNoCorrVarA=0;
   float energyVarB=0,RMSenergyVarB=0;
   float energyNoCorrVarB=0,RMSenergyNoCorrVarB=0;
   outFile->cd();
   TTree* outTree= new TTree("tree_barl","tree_barl");
   outTree->Branch("time_interval",&timeVar,"timeInterval/I");
   outTree->Branch("nHits",&hitVar, "nHits/i");
   outTree->Branch("ieta",&ietaVar,"ieta/I");
   outTree->Branch("iphi",&iphiVar,"iphi/I");
   outTree->Branch("sign",&signVar,"sign/I");
   outTree->Branch("energySum",&energyVar,"energySum/F");
   outTree->Branch("energySquared",&RMSenergyVar,"energySquared/F");
   outTree->Branch("energySumA",&energyVarA,"energySumA/F");
   outTree->Branch("energySquaredA",&RMSenergyVarA,"energySquaredA/F");
   outTree->Branch("energySumB",&energyVarB,"energySumB/F");
   outTree->Branch("energySquaredB",&RMSenergyVarB,"energySquaredB/F");
   outTree->Branch("energyNoCorrSum",&energyNoCorrVar,"energyNoCorrSum/F");
   outTree->Branch("energyNoCorrSquared",&RMSenergyNoCorrVar,"energyNoCorrSquared/F");
   outTree->Branch("energyNoCorrSumA",&energyNoCorrVarA,"energyNoCorrSumA/F");
   outTree->Branch("energyNoCorrSquaredA",&RMSenergyNoCorrVarA,"energyNoCorrSquaredA/F");
   outTree->Branch("energyNoCorrSumB",&energyNoCorrVarB,"energyNoCorrSumB/F");
   outTree->Branch("energyNoCorrSquaredB",&RMSenergyNoCorrVarB,"energyNoCorrSquaredB/F");
   outTree->Branch("lcSum",&lcVar,"lcSum/F");
   outTree->Branch("lcSquared",&RMSlcVar,"lcSquared/F");

   cout<<"start looping over entries"<<endl;


   TFile *bsWeights = TFile::Open("beamSpotInterpolatedCorrections.root");
   TGraph* bsCorrections[85]; 
   
   for (int iieta=0;iieta<85;++iieta)
    //    for (int iside=0;iside<2;++iside)
     {
       
       TString name="bsPosCorrectionInterpolated_ieta_";
       name+=(iieta+1);
       // 	name+="_side_";
       // 	name+=iside;
       bsCorrections[iieta]=(TGraph*)bsWeights->Get(name);
     }

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(jentry%10000==0) std::cout<< "EVENT " << jentry<< " RUN " << run << " LS " << lumi << std::endl; 
     //Checking quality of LS
     if (goodLS && !goodLS->isGoodLS(run,lumi))
       continue;
     int theInterval=intervals->intervalNumber(run, lumi);
     
     for (unsigned int ihit=0;ihit<nhit_barl;++ihit){
       int theSign=ieta[ihit]>0 ? 1:0;
       int theEta=TMath::Abs(ieta[ihit]);
       int thePhi=iphi[ihit];

       int bin=-1;
       double bsCorr=1.;
       //apply an eta based BeamSpot correction
        double max=TMath::MaxElement(bsCorrections[theEta-1]->GetN(),bsCorrections[theEta-1]->GetX());
        double min=TMath::MinElement(bsCorrections[theEta-1]->GetN(),bsCorrections[theEta-1]->GetX());
        double nbins=bsCorrections[theEta-1]->GetN();
        double binSize=(max-min)/(nbins-1);

     
        if (theSign==0)
 	 beamSpotZ0=beamSpotZ0*-1;
        bin = (int)(beamSpotZ0-min-binSize/2)/binSize;
        if (bin<0)
 	 bin=0;
        if (bin>nbins-1)
 	 bin=nbins-1;
	
        bsCorr=*(bsCorrections[theEta-1]->GetY()+bin);
	et_barl[ihit]=et_barl[ihit]/bsCorr;
       //	 cout<<theEta<<" "<<thePhi<<" "<<theInterval<<" "<<theSign<<endl;
       if(theSign < kSides && thePhi <=kBarlWedges && theInterval>=0 && theInterval <kIntervals && theEta <=kBarlRings ){
	 sums[theInterval].energySum[theEta-1][thePhi-1][theSign]+=et_barl[ihit];
	 sums[theInterval].energySquared[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit],2);
	 sums[theInterval].energyNoCorrSum[theEta-1][thePhi-1][theSign]+=et_barl[ihit]/lc_barl[ihit];
	 sums[theInterval].energyNoCorrSquared[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit]/lc_barl[ihit],2);
	 if (et_barl[ihit]<0.34)
	   {
	     sums[theInterval].energySumA[theEta-1][thePhi-1][theSign]+=et_barl[ihit];
	     sums[theInterval].energySquaredA[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit],2);
	     sums[theInterval].energyNoCorrSumA[theEta-1][thePhi-1][theSign]+=et_barl[ihit]/lc_barl[ihit];
	     sums[theInterval].energyNoCorrSquaredA[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit]/lc_barl[ihit],2);
	   }
	 else
	   {
	     sums[theInterval].energySumB[theEta-1][thePhi-1][theSign]+=et_barl[ihit];
	     sums[theInterval].energySquaredB[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit],2);
	     sums[theInterval].energyNoCorrSumB[theEta-1][thePhi-1][theSign]+=et_barl[ihit]/lc_barl[ihit];
	     sums[theInterval].energyNoCorrSquaredB[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit]/lc_barl[ihit],2);
	   }
	 if ((et_barl[ihit]/lc_barl[ihit])<0.34)
	   {
	     sums[theInterval].energyNoCorrSumA[theEta-1][thePhi-1][theSign]+=et_barl[ihit]/lc_barl[ihit];
	     sums[theInterval].energyNoCorrSquaredA[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit]/lc_barl[ihit],2);
	   }
	 else
	   {
	     sums[theInterval].energyNoCorrSumB[theEta-1][thePhi-1][theSign]+=et_barl[ihit]/lc_barl[ihit];
	     sums[theInterval].energyNoCorrSquaredB[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit]/lc_barl[ihit],2);
	   }
	 sums[theInterval].lasercorrSum[theEta-1][thePhi-1][theSign]+=lc_barl[ihit];
	 sums[theInterval].lasercorrSquared[theEta-1][thePhi-1][theSign]+=pow(lc_barl[ihit],2);
	 sums[theInterval].counter[theEta-1][thePhi-1][theSign]++;
       }
     }
   }//close loop over entries

   cout<<"filling out tree"<<endl;

   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     std::cout << "INTERVAL " << iinterval << "/" << kIntervals-1 << std::endl;
     for(int iisign=0;iisign<kSides;iisign++){
       for(int iieta=0;iieta<kBarlRings;iieta++){
	 for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	   if(sums[iinterval].energySum[iieta][iiphi][iisign]>0){
	     hitVar= sums[iinterval].counter[iieta][iiphi][iisign];
	     ietaVar=iieta+1;
	     iphiVar=iiphi+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     energyVar=sums[iinterval].energySum[iieta][iiphi][iisign];
	     RMSenergyVar=sums[iinterval].energySquared[iieta][iiphi][iisign];
	     energyNoCorrVar=sums[iinterval].energyNoCorrSum[iieta][iiphi][iisign];
	     RMSenergyNoCorrVar=sums[iinterval].energyNoCorrSquared[iieta][iiphi][iisign];
	     energyVarA=sums[iinterval].energySumA[iieta][iiphi][iisign];
	     RMSenergyVarA=sums[iinterval].energySquaredA[iieta][iiphi][iisign];
	     energyNoCorrVarA=sums[iinterval].energyNoCorrSumA[iieta][iiphi][iisign];
	     RMSenergyNoCorrVarA=sums[iinterval].energyNoCorrSquaredA[iieta][iiphi][iisign];
	     energyVarB=sums[iinterval].energySumB[iieta][iiphi][iisign];
	     RMSenergyVarB=sums[iinterval].energySquaredB[iieta][iiphi][iisign];
	     energyNoCorrVarB=sums[iinterval].energyNoCorrSumB[iieta][iiphi][iisign];
	     RMSenergyNoCorrVarB=sums[iinterval].energyNoCorrSquaredB[iieta][iiphi][iisign];
	     lcVar=sums[iinterval].lasercorrSum[iieta][iiphi][iisign];
	     RMSlcVar=sums[iinterval].lasercorrSquared[iieta][iiphi][iisign];
	     outTree->Fill();
	   }
	 }
       }
     }
   }
   
   outFile->cd();
   outTree->Write();
   outFile->Write();
   outFile->Close();
}
