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
   std::vector<variablesToControl_ee> controls_ee;
   int kIntervals=intervals->numberOfIntervals();
   controls.reserve(kIntervals);
   controls_ee.reserve(kIntervals);
   unsigned long int nHitsEB[kIntervals];
   unsigned long int nHitsEE[kIntervals];

   cout<<"creating variables"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
     controls[iinterval].reset();
     controls_ee[iinterval].reset();
     nHitsEB[iinterval]=0;
     nHitsEE[iinterval]=0;
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
   unsigned int nEvents;
   float bsPosVar=0,bsWidVar=0;

   TBranch *b_time=bsInfoTree->GetBranch("time_interval");
   TBranch *b_events=bsInfoTree->GetBranch("nEvents");
   TBranch *b_bsPos=bsInfoTree->GetBranch("bsPos");
   TBranch *b_bsWid=bsInfoTree->GetBranch("bsWid");

   bsInfoTree->SetBranchAddress("time_interval", &timeVar, &b_time);
   bsInfoTree->SetBranchAddress("nEvents", &nEvents, &b_events);
   bsInfoTree->SetBranchAddress("bsPos", &bsPosVar, &b_bsPos);
   bsInfoTree->SetBranchAddress("bsWid", &bsWidVar, &b_bsWid);

   int nentries_bs = bsInfoTree->GetEntries();
   for(int jentry=0;jentry<nentries_bs;++jentry){
     bsInfoTree->GetEntry(jentry);
     bsInfos[timeVar-1].bsPos=bsPosVar;
     bsInfos[timeVar-1].bsWid=bsWidVar;
     bsInfos[timeVar-1].nEvents=nEvents;
   }

   std::cout << "Read beamSpot informations for " << nentries_bs << " intervals " << std::endl;

   std::cout << "Opening eeIndicesMap  " << eeIndicesFile << std::endl;
   TFile *fee=TFile::Open(eeIndicesFile);
   if (!fee->IsOpen())
     std::cout << "Cannot open " << eeIndicesFile << std::endl;
   TTree* eeIndicesTree= (TTree*)fee->Get("eeIndices");
   
   int ixVar,iyVar,signVar,iringVar,ixtalVar,iSCVar,iLMVar;
   TBranch *b_ix=eeIndicesTree->GetBranch("ix");
   TBranch *b_iy=eeIndicesTree->GetBranch("iy");
   TBranch *b_sig=eeIndicesTree->GetBranch("sign");
   TBranch *b_iring=eeIndicesTree->GetBranch("iring");
   TBranch *b_ixtal=eeIndicesTree->GetBranch("ixtal");
   TBranch *b_isc=eeIndicesTree->GetBranch("isc");
   TBranch *b_ilm=eeIndicesTree->GetBranch("ilm");
   
   eeIndicesTree->SetBranchAddress("ix", &ixVar, &b_ix);
   eeIndicesTree->SetBranchAddress("iy", &iyVar, &b_iy);
   eeIndicesTree->SetBranchAddress("sign", &signVar, &b_sig);
   eeIndicesTree->SetBranchAddress("iring", &iringVar, &b_iring);
   eeIndicesTree->SetBranchAddress("ixtal", &ixtalVar, &b_ixtal);
   eeIndicesTree->SetBranchAddress("isc", &iSCVar, &b_isc);
   eeIndicesTree->SetBranchAddress("ilm", &iLMVar, &b_ilm);
   
   int nentries_ee = eeIndicesTree->GetEntries();
   int endcapRings[kEndcWedgesX][kEndcWedgesY][kSides];
   int endcapXtals[kEndcWedgesX][kEndcWedgesY][kSides];
   int endcapSCs[kEndcWedgesX][kEndcWedgesY][kSides];
   int endcapLMs[kEndcWedgesX][kEndcWedgesY][kSides];
   
   for (int isign=0;isign<kSides;++isign)
     for (int ix=0;ix<kEndcWedgesX;++ix)
       for (int iy=0;iy<kEndcWedgesX;++iy)
	 {
	   endcapRings[ix][iy][isign]=-1;
	   endcapXtals[ix][iy][isign]=-1;
	   endcapSCs[ix][iy][isign]=-1;
	   endcapLMs[ix][iy][isign]=-1;
	 }
   
   for(int jentry=0;jentry<nentries_ee;++jentry)
     {
       eeIndicesTree->GetEntry(jentry);
       endcapRings[ixVar-1][iyVar-1][signVar]=iringVar;
       endcapXtals[ixVar-1][iyVar-1][signVar]=ixtalVar;
       endcapSCs[ixVar-1][iyVar-1][signVar]=iSCVar;
       endcapLMs[ixVar-1][iyVar-1][signVar]=iLMVar;
     }

  std::cout << "Read eeIndices for " << nentries_ee << " xtals " << std::endl;

   TF1* bsCorrections[85]; 
   TF1* bsCorrections_ee[kEndcRings]; 

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

       for (int iieta=0;iieta<kEndcRings;++iieta)
	 //    for (int iside=0;iside<2;++iside)
	 {
	   
	   TString name="f_bsAsymm_ee_iring_";
	   name+=(iieta+1);
	   // 	name+="_side_";
	   // 	name+=iside;
	   std::cout << "Getting " << name << std::endl;
	   bsCorrections_ee[iieta]=(TF1*)bsWeights->Get(name);
	 }
     }

   TF1* lumiCorrections[85]; 
   TF1* lumiCorrections_ee[kEndcRings]; 


   if (applyLumiCorrection)
     {
       std::cout << "Reading lumi corrections file " << bsCorrectionFile << std::endl;
       TFile *bsWeights = TFile::Open(bsCorrectionFile);

       //Only for EB at the moment
       for (int iieta=0;iieta<85;++iieta)
	 //    for (int iside=0;iside<2;++iside)
	 {
	   
	   TString name="f_lumiAsymm_ieta_";
	   name+=(iieta+1);
	   // 	name+="_side_";
	   // 	name+=iside;
	   std::cout << "Getting " << name << std::endl;
	   lumiCorrections[iieta]=(TF1*)bsWeights->Get(name);
	 }

       for (int iieta=0;iieta<kEndcRings;++iieta)
	 //    for (int iside=0;iside<2;++iside)
	 {
	   
	   TString name="f_lumiAsymm_ee_iring_";
	   name+=(iieta+1);
	   // 	name+="_side_";
	   // 	name+=iside;
	   std::cout << "Getting " << name << std::endl;
	   lumiCorrections_ee[iieta]=(TF1*)bsWeights->Get(name);
	 }
       Long64_t nbytes = 0, nb = 0;

       for (Long64_t jentry=0; jentry<nentries;jentry++) {
	 Long64_t ientry = LoadTree(jentry);
	 if (ientry < 0) break;
	 nb = fChain->GetEntry(jentry);   nbytes += nb;
	 // if (Cut(ientry) < 0) continue;
	 //Only EB for the moment
	 
	 if (time_interval>186 && time_interval<242)
	   continue;

	 if (det==0)
	   {
	     nHitsEB[time_interval-1]+=nHits;
	   }
	 else if (det==1)
	   {
	     nHitsEE[time_interval-1]+=nHits;
	   }
       } 
       for(int iinterval=0;iinterval<kIntervals;iinterval++)
	 {	  
	   bsInfos[iinterval].nHitsMeanEB = double(nHitsEB[iinterval])/double(bsInfos[iinterval].nEvents);
	   bsInfos[iinterval].nHitsMeanEE = double(nHitsEE[iinterval])/double(bsInfos[iinterval].nEvents);
	   std::cout << "Interval " << iinterval << " <nHitsEB>: " << bsInfos[iinterval].nHitsMeanEB <<  " <nHitsEE>: " << bsInfos[iinterval].nHitsMeanEE << std::endl; 
	 }
     }
   

   Long64_t nbytes = 0, nb = 0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     //Only EB for the moment
     
     if (time_interval>186 && time_interval<242)
       continue;

     double bsCorr=1.;
     double lumiCorr=1.;
     if (det==0)
       {
	 
	 if (applyBSCorrection)
	   {
	     if (sign>0)
	       bsCorr=bsCorrections[ieta-1]->Eval(bsInfos[time_interval-1].bsPos);
	     else
	       bsCorr=bsCorrections[ieta-1]->Eval(-bsInfos[time_interval-1].bsPos);
	     // 	  if (TMath::Abs(ieta)==1)
	      // 	      std::cout << "time " << time_interval-1 << " bsPos " << bsInfos[time_interval-1].bsPos << " ieta " << ieta << " sign " << sign << " bsCorr " << bsCorr << std::endl;  
	    }

	 if (applyLumiCorrection)
	   {
	       lumiCorr=lumiCorrections[ieta-1]->Eval(bsInfos[time_interval-1].nHitsMeanEB);
	    }
	  //      std::cout << bsCorr << std::endl;

	  et=et/bsCorr;
	  et=et/lumiCorr;
	  
	  controls[time_interval-1].counter++;
	  controls[time_interval-1].totalHits+=nHits;
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
      else if (det==1)
	{
	  int iring=(endcapRings[ieta-1][iphi-1][sign]);	

	  if (applyBSCorrection)
	    {
	      if (sign>0)
		bsCorr=bsCorrections_ee[iring]->Eval(bsInfos[time_interval-1].bsPos);
	      else
		bsCorr=bsCorrections_ee[iring]->Eval(-bsInfos[time_interval-1].bsPos);
	      // 	  if (TMath::Abs(ieta)==1)
	      // 	      std::cout << "time " << time_interval-1 << " bsPos " << bsInfos[time_interval-1].bsPos << " ieta " << ieta << " sign " << sign << " bsCorr " << bsCorr << std::endl;  
	    }

	 if (applyLumiCorrection)
	   {
	       lumiCorr=lumiCorrections_ee[iring]->Eval(bsInfos[time_interval-1].nHitsMeanEB);
	    }
	  //      std::cout << bsCorr << std::endl;
	  et=et/bsCorr;	  
	  et=et/lumiCorr;	  

	  controls_ee[time_interval-1].counter++;
	  controls_ee[time_interval-1].totalHits+=nHits;
	  //Histories by eta ring
	  controls_ee[time_interval-1].nhitMean[iring][sign]+=nHits;
	  controls_ee[time_interval-1].etSumMean[iring][sign]+=et;
	  controls_ee[time_interval-1].etSumMeanRMS[iring][sign]+=nHits*pow(RMSet,2);
	  controls_ee[time_interval-1].etMean[iring][sign]+=et/(float)nHits;
	  controls_ee[time_interval-1].etMeanRMS[iring][sign]+=pow(RMSet,2)/(float)nHits;
	  controls_ee[time_interval-1].lcMean[iring][sign]+=lc/(float)nHits;
	  controls_ee[time_interval-1].lcMeanRMS[iring][sign]+=pow(RMSlc,2)/(float)nHits;
	  controls_ee[time_interval-1].tlMean[iring][sign]+=TMath::Exp(-1*TMath::Log(lc)/alphaDB(sign,ieta,iphi));
	  controls_ee[time_interval-1].tlMeanRMS[iring][sign]+=pow(RMSlc*alphaDB(sign,ieta,iphi),2)/(float)nHits; //approximation
	  controls_ee[time_interval-1].counterEta[iring][sign]++;
	}
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
       controls[iinterval].nHitsMean=(double)controls[iinterval].totalHits/(double)bsInfos[iinterval].nEvents;
       std::cout << "Interval " << iinterval << " <nHitsEBPerEvent> " << controls[iinterval].nHitsMean << std::endl; 
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
     }
     if(controls_ee[iinterval].counter!=0){
       controls_ee[iinterval].nHitsMean=(double)controls_ee[iinterval].totalHits/(double)bsInfos[iinterval].nEvents;
       std::cout << "Interval " << iinterval << " <nHitsEEPerEvent> " << controls_ee[iinterval].nHitsMean << std::endl; 
       for (int i=0;i<kEndcRings;++i)
	 {
	   for(int j=0;j<kSides;++j){
	     controls_ee[iinterval].etSumMean[i][j]=controls_ee[iinterval].etSumMean[i][j]/(float)controls_ee[iinterval].counterEta[i][j];
	     controls_ee[iinterval].etSumMeanRMS[i][j]=sqrt(controls_ee[iinterval].etSumMeanRMS[i][j])/(float)controls_ee[iinterval].counterEta[i][j];
	     controls_ee[iinterval].etMean[i][j]=controls_ee[iinterval].etMean[i][j]/(float)controls_ee[iinterval].counterEta[i][j];
	     controls_ee[iinterval].etMeanRMS[i][j]=sqrt(controls_ee[iinterval].etMeanRMS[i][j])/((float)controls_ee[iinterval].counterEta[i][j]);// i want eerror on mean

	     controls_ee[iinterval].lcMeanRMS[i][j]= sqrt(controls_ee[iinterval].lcMeanRMS[i][j])/((float)controls_ee[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls_ee[iinterval].lcMean[i][j]=controls_ee[iinterval].lcMean[i][j]/(float)controls_ee[iinterval].counterEta[i][j];
	     controls_ee[iinterval].tlMeanRMS[i][j]= sqrt(controls_ee[iinterval].tlMeanRMS[i][j])/((float)controls_ee[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls_ee[iinterval].tlMean[i][j]=controls_ee[iinterval].tlMean[i][j]/(float)controls_ee[iinterval].counterEta[i][j];
	     controls_ee[iinterval].nhitMean[i][j]=controls_ee[iinterval].nhitMean[i][j]/(float)controls_ee[iinterval].counterEta[i][j];
	   }
	 }
     }  
   }
   
   
   TProfile* bsAsymm[85];
   TProfile* lumiAsymm[85];
   TProfile* bsWidAsymm[85];

   TProfile* bsAsymm_ee[kEndcRings];
   TProfile* lumiAsymm_ee[kEndcRings];
   TProfile* bsWidAsymm_ee[kEndcRings];

   for (int i=0;i<85;++i)
     {
	 TString name="bsAsymm_ieta_";
	 name+=(i+1);
	 bsAsymm[i]=new TProfile(name,name,41,-2.05,2.05);
	 name="lumiAsymm_ieta_";
	 name+=(i+1);
	 lumiAsymm[i]=new TProfile(name,name,50,0.,300.);
	 name="bsWidAsymm_ieta_";
	 name+=(i+1);
	 bsWidAsymm[i]=new TProfile(name,name,60,4.,7.);
     }

   for (int i=0;i<kEndcRings;++i)
     {
	 TString name="bsAsymm_ee_iring_";
	 name+=(i+1);
	 bsAsymm_ee[i]=new TProfile(name,name,41,-2.05,2.05);
	 name="lumiAsymm_ee_iring_";
	 name+=(i+1);
	 lumiAsymm_ee[i]=new TProfile(name,name,50,0.,300.);
	 name="bsWidAsymm_ee_iring_";
	 name+=(i+1);
	 bsWidAsymm_ee[i]=new TProfile(name,name,60,4.,7.);
     }

   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     if(controls[iinterval].counter!=0){
       for (int i=0;i<kBarlRings;++i)
         {
	   //	   std::cout << bsInfos[iinterval].bsPos << "," << (controls[iinterval].etSumMean[i][1]-controls[iinterval].etSumMean[i][0])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.) << std::endl;
	   //	   bsAsymm[i]->Fill(bsInfos[iinterval].bsPos,((controls[iinterval].etSumMean[i][1]-controls[iinterval].etSumMean[i][0])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.))/2.);
	   bsAsymm[i]->Fill(bsInfos[iinterval].bsPos,((controls[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.)));
	   lumiAsymm[i]->Fill(controls[iinterval].nHitsMean,((controls[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[9][1]+controls[iinterval].etSumMean[9][0])/2.)));
	   bsWidAsymm[i]->Fill(bsInfos[iinterval].bsWid,((controls[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[9][1]+controls[iinterval].etSumMean[9][0])/2.)));
	 }
       
       for (int i=0;i<kEndcRings;++i)
         {
	   //	   std::cout << bsInfos[iinterval].bsPos << "," << (controls[iinterval].etSumMean[i][1]-controls[iinterval].etSumMean[i][0])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.) << std::endl;
	   //	   bsAsymm[i]->Fill(bsInfos[iinterval].bsPos,((controls[iinterval].etSumMean[i][1]-controls[iinterval].etSumMean[i][0])/((controls[iinterval].etSumMean[i][1]+controls[iinterval].etSumMean[i][0])/2.))/2.);

	   std::cout << bsInfos[iinterval].bsPos << "," << ((controls_ee[iinterval].etSumMean[i][1])/((controls_ee[iinterval].etSumMean[i][1]+controls_ee[iinterval].etSumMean[i][0])/2.)) << std::endl;
	   bsAsymm_ee[i]->Fill(bsInfos[iinterval].bsPos,((controls_ee[iinterval].etSumMean[i][1])/((controls_ee[iinterval].etSumMean[i][1]+controls_ee[iinterval].etSumMean[i][0])/2.)));
	   lumiAsymm_ee[i]->Fill(controls[iinterval].nHitsMean,((controls_ee[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[9][1]+controls[iinterval].etSumMean[9][0])/2.)));
	   bsWidAsymm_ee[i]->Fill(bsInfos[iinterval].bsWid,((controls_ee[iinterval].etSumMean[i][1])/((controls[iinterval].etSumMean[9][1]+controls[iinterval].etSumMean[9][0])/2.)));
	 }
     }
   }

   TFile *outFile=TFile::Open(outFileName,outFileMode);
   for (int i=0;i<85;++i)
     {
       if (!applyBSCorrection && !applyLumiCorrection)
	 {
	   TString name="f_bsAsymm_ieta_";
	   name+=(i+1);
	   std::cout << name << std::endl;
	   bsAsymm[i]->Fit("pol1");
	   TF1* f=bsAsymm[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,1.);
	   //       f_corr->SetParameter(1,f->GetParameter(1));
	   f_corr->Write(name);
	   bsAsymm[i]->Write();
	 }

       if (applyBSCorrection && applyLumiCorrection)
	 {
	   TString name="f_bsWidAsymm_ieta_";
	   name+=(i+1);
	   std::cout << name << std::endl;
	   bsWidAsymm[i]->Fit("pol1","","",4.5,5.2);
	   TF1* f= (TF1*) bsWidAsymm[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,(f->GetParameter(0))/(f->GetParameter(0)+5*f->GetParameter(1)));
	   f_corr->SetParameter(1,f->GetParameter(1)/(f->GetParameter(0)+5*f->GetParameter(1)));
	   f_corr->Write(name);
	   bsWidAsymm[i]->Write();
	 }

       if (applyBSCorrection && !applyLumiCorrection)
	 {
	   TString name="f_lumiAsymm_ieta_";
	   name+=(i+1);
	   std::cout << name << std::endl;
	   lumiAsymm[i]->Fit("pol1","","",0.,50.);
	   TF1* f= (TF1*) lumiAsymm[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,(f->GetParameter(0))/(f->GetParameter(0)+20*f->GetParameter(1)));
	   f_corr->SetParameter(1,f->GetParameter(1)/(f->GetParameter(0)+20*f->GetParameter(1)));
	   f_corr->Write(name);
	   lumiAsymm[i]->Write();
	 }
     }

   for (int i=0;i<kEndcRings;++i)
     {

       if (!applyBSCorrection && !applyLumiCorrection)
	 {
	   TString name="f_bsAsymm_ee_iring_";
	   name+=(i+1);
	   std::cout << name << std::endl;
	   bsAsymm_ee[i]->Fit("pol1");
	   TF1* f=bsAsymm_ee[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,1.);
	   //       f_corr->SetParameter(1,f->GetParameter(1));
	   f_corr->Write(name);
	   bsAsymm_ee[i]->Write();
	 }

       if (applyBSCorrection && applyLumiCorrection)
	 {
	   TString name="f_bsWidAsymm_ee_iring_";
	   name+=(i+1);
	   std::cout << name << std::endl;
	   bsWidAsymm_ee[i]->Fit("pol1","","",4.5,5.2);
	   TF1* f= (TF1*) bsWidAsymm_ee[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,(f->GetParameter(0))/(f->GetParameter(0)+5*f->GetParameter(1)));
	   f_corr->SetParameter(1,f->GetParameter(1)/(f->GetParameter(0)+5*f->GetParameter(1)));
	   f_corr->Write(name);
	   bsWidAsymm_ee[i]->Write();
	 }

       if (applyBSCorrection && !applyLumiCorrection)
	 {
	   TString name="f_lumiAsymm_ee_iring_";
	   name+=(i+1);
	   std::cout << name << std::endl;
	   lumiAsymm_ee[i]->Fit("pol1","","",0.,50.);
	   TF1* f= (TF1*) lumiAsymm_ee[i]->GetFunction("pol1");
	   TF1* f_corr=(TF1*)f->Clone(name);
	   f_corr->SetParameter(0,(f->GetParameter(0))/(f->GetParameter(0)+20*f->GetParameter(1)));
	   f_corr->SetParameter(1,f->GetParameter(1)/(f->GetParameter(0)+20*f->GetParameter(1)));
	   f_corr->Write(name);
	   lumiAsymm_ee[i]->Write();
	 }
     }
   outFile->Write();
   outFile->Close();

   
}
