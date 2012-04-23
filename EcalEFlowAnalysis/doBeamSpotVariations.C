#define doBeamSpotVariations_cxx
#include "doBeamSpotVariations.h"
#include <TH2.h>
#include <TProfile.h>
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


void doBeamSpotVariations::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L doBeamSpotVariations.C
//      Root > doBeamSpotVariations t
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
// METAHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  if (!intervals)
    return;

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();
  
   std::vector<variablesToControl> controls;
   int kIntervals=intervals->numberOfIntervals();
   controls.reserve(kIntervals);

   cout<<"creating variables"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
   controls[iinterval].reset();
   }


   std::vector<bsInfo> bsInfos;
   bsInfos.reserve(kIntervals);
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
     bsInfos[iinterval].reset();
   }
   
   TFile *fbs=TFile::Open(bsInfoFile);
   if (!fbs->IsOpen())
     std::cout << "Cannot open " << bsInfoFile << std::endl;
   TTree* bsInfoTree= (TTree*)fbs->Get("bsFinalTree");

   int timeVar;
   float bsPosVar=0,bsWidVar=0;

   TBranch *b_time=bsInfoTree->GetBranch("time_interval");
   TBranch *b_bsPos=bsInfoTree->GetBranch("bsPos");
   TBranch *b_bsWid=bsInfoTree->GetBranch("bsWid");

   bsInfoTree->SetBranchAddress("time_interval", &timeVar, &b_time);
   bsInfoTree->SetBranchAddress("bsPos", &bsPosVar, &b_bsPos);
   bsInfoTree->SetBranchAddress("bsWid", &bsWidVar, &b_bsWid);

   int nentries_bs = bsInfoTree->GetEntries();
   for(int jentry=0;jentry<nentries_bs;++jentry){
     bsInfoTree->GetEntry(jentry);
     bsInfos[timeVar-1].bsPos=bsPosVar;
     bsInfos[timeVar-1].bsWid=bsWidVar;
   }

   std::cout << "Read beamSpot informations for " << nentries_bs << " intervals " << std::endl;

   TF1* bsCorrections[85]; 

   if (applyBSCorrection)
     {
       std::cout << "Reading beamSpot corrections file " << bsCorrectionFile << std::endl;
       TFile *bsWeights = TFile::Open(bsCorrectionFile);

       //Only for EB at the moment
       for (int iieta=0;iieta<85;++iieta)
	 //    for (int iside=0;iside<2;++iside)
	 {
	   
	   TString name="f_bsAsymm_ieta_";
	   name+=(iieta+1);
	   // 	name+="_side_";
	   // 	name+=iside;
	   std::cout << "Getting " << name << std::endl;
	   bsCorrections[iieta]=(TF1*)bsWeights->Get(name);
	 }
     }

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      //Only EB for the moment
      if (det!=0)
	continue;
      if (time_interval>186 && time_interval<242)
	continue;

      double bsCorr=1.;

      if (applyBSCorrection)
	{
	  if (sign>0)
	    bsCorr=bsCorrections[ieta-1]->Eval(bsInfos[time_interval-1].bsPos);
	  else
	    bsCorr=bsCorrections[ieta-1]->Eval(-bsInfos[time_interval-1].bsPos);
// 	  if (TMath::Abs(ieta)==1)
// 	      std::cout << "time " << time_interval-1 << " bsPos " << bsInfos[time_interval-1].bsPos << " ieta " << ieta << " sign " << sign << " bsCorr " << bsCorr << std::endl;  
	}
      //      std::cout << bsCorr << std::endl;
      et=et/bsCorr;

      controls[time_interval-1].counter++;
      //Histories by eta ring
      controls[time_interval-1].nhitMean[ieta-1][sign]+=nHits;
      controls[time_interval-1].etSumMean[ieta-1][sign]+=et;
      controls[time_interval-1].etSumMeanRMS[ieta-1][sign]+=nHits*pow(RMSet,2);
      controls[time_interval-1].etMean[ieta-1][sign]+=et/(float)nHits;
      controls[time_interval-1].etMeanRMS[ieta-1][sign]+=pow(RMSet,2)/(float)nHits;
      controls[time_interval-1].lcMean[ieta-1][sign]+=lc/(float)nHits;
      controls[time_interval-1].lcMeanRMS[ieta-1][sign]+=pow(RMSlc,2)/(float)nHits;
      controls[time_interval-1].tlMean[ieta-1][sign]+=TMath::Exp(-1*TMath::Log(lc)/alphaDB(sign,ieta,iphi));
      controls[time_interval-1].tlMeanRMS[ieta-1][sign]+=pow(RMSlc*alphaDB(sign,ieta,iphi),2)/(float)nHits; //approximation
      controls[time_interval-1].counterEta[ieta-1][sign]++;
   }

   int counterTotal=0;
   float nInterval[kIntervals];    
   float nIntervalError[kIntervals];    
   //   float nIntervalError[kInterval];
   float etTotal=0;    
   float etTotalNoCorr=0; 

   cout<<"loop ended"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     if(controls[iinterval].counter!=0){
       for (int i=0;i<kBarlRings;++i)
	 {
	   for(int j=0;j<kSides;++j){
	     controls[iinterval].etSumMean[i][j]=controls[iinterval].etSumMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	     controls[iinterval].etSumMeanRMS[i][j]=sqrt(controls[iinterval].etSumMeanRMS[i][j])/(float)controls[iinterval].counterEta[i][j];
	     controls[iinterval].etMean[i][j]=controls[iinterval].etMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	     controls[iinterval].etMeanRMS[i][j]=sqrt(controls[iinterval].etMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean

	     controls[iinterval].lcMeanRMS[i][j]= sqrt(controls[iinterval].lcMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls[iinterval].lcMean[i][j]=controls[iinterval].lcMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	     controls[iinterval].tlMeanRMS[i][j]= sqrt(controls[iinterval].tlMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls[iinterval].tlMean[i][j]=controls[iinterval].tlMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	     controls[iinterval].nhitMean[i][j]=controls[iinterval].nhitMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	   }
	 }
       
       
       nInterval[iinterval]=intervals->intervalTime(iinterval);
       nIntervalError[iinterval]=0.;
       //       nIntervalError[iinterval]=0;
     }
   }


   TProfile* bsAsymm[85];
   TProfile* bsWidAsymm[85];
   for (int i=0;i<85;++i)
     {
	 TString name="bsAsymm_ieta_";
	 name+=(i+1);
	 bsAsymm[i]=new TProfile(name,name,41,-2.05,2.05);
	 name="bsWidAsymm_ieta_";
	 name+=(i+1);
	 bsWidAsymm[i]=new TProfile(name,name,30,4.,7.);
     }

   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     if(controls[iinterval].counter!=0){
       for (int i=0;i<kBarlRings;++i)
         {
	   //	   std::cout << bsInfos[iinterval].bsPos << "," << (controls[iinterval].etSumMean[i][1]-controls[iinterval].etSumMean[i][0])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.) << std::endl;
	   //	   bsAsymm[i]->Fill(bsInfos[iinterval].bsPos,((controls[iinterval].etSumMean[i][1]-controls[iinterval].etSumMean[i][0])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.))/2.);
	   bsAsymm[i]->Fill(bsInfos[iinterval].bsPos,((controls[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.)));
	   bsWidAsymm[i]->Fill(bsInfos[iinterval].bsWid,((controls[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[9][1]+controls[iinterval].etSumMean[9][0])/2.)));
	 }
     }
   }
   
   TFile *outFile=TFile::Open(outFileName,outFileMode);
   for (int i=0;i<85;++i)
     {
       if (!applyBSCorrection)
	 {
	   TString name="f_bsAsymm_ieta_";
	   name+=(i+1);
	   bsAsymm[i]->Fit("pol1");
	   TF1* f=bsAsymm[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,1.);
	   //       f_corr->SetParameter(1,f->GetParameter(1));
	   f_corr->Write(name);
	   bsAsymm[i]->Write();
	 }
       if (applyBSCorrection)
	 {
	   TString name="f_bsWidAsymm_ieta_";
	   name+=(i+1);
	   bsWidAsymm[i]->Fit("pol1");
	   TF1* f= (TF1*) bsWidAsymm[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,1-5*f->GetParameter(1));
	   f_corr->Write(name);
	   bsWidAsymm[i]->Write();
	 }
     }
   outFile->Write();
   outFile->Close();

   
}
