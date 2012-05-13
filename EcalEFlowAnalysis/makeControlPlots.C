#define makeControlPlots_cxx
#include "makeControlPlots.h"
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


void makeControlPlots::Loop()
{
  //   In a ROOT session, you can do:
  //      Root > .L makeControlPlots.C
  //      Root > makeControlPlots t
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
  std::vector<variablesToControlEndc> controlsEndc;
  int kIntervals=intervals->numberOfIntervals();
  controls.reserve(kIntervals);
  controlsEndc.reserve(kIntervals);

  cout<<"creating variables"<<endl;
  for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
    controls[iinterval].reset();
    controlsEndc[iinterval].reset();
  }

  TFile *kFactors;
  float  kFactorsEtSum[kBarlRings];
  float  kFactorsEtSumXtal[kBarlRings][kBarlWedges][kSides];
  float  kFactorsEndcEtSum[kEndcRings];

  if (kfactorCorr)
    {

      std::cout << "Opening kFactors " << kFactorsFile << std::endl;
      kFactors = TFile::Open(kFactorsFile);
      TTree* kFactorsTree= (TTree*) kFactors->Get("kFactors");
      int ring;
      float kf;
      TBranch *b_ring=kFactorsTree->GetBranch("ring");
      TBranch *b_kf=kFactorsTree->GetBranch("kFactor");
      kFactorsTree->SetBranchAddress("ring", &ring, &b_ring);
      kFactorsTree->SetBranchAddress("kFactor", &kf, &b_kf);
      //   Long64_t nbytes_int = 0, nb_int = 0;
      int nentries_int = kFactorsTree->GetEntries();
      for(int jentry=0;jentry<nentries_int;++jentry){
	kFactorsTree->GetEntry(jentry);
	std::cout << "kFactor for ieta " << ring << " is " << kf << std::endl;
	kFactorsEtSum[ring-1]=kf;
      }
      
      std::cout << "Opening kFactors " << kFactorsXtalFile << std::endl;
      kFactors = TFile::Open(kFactorsXtalFile);
      kFactorsTree= (TTree*) kFactors->Get("kFactors_xtals");
      int eta,phi,signVar;
      double kfd;
      TBranch *b_eta=kFactorsTree->GetBranch("eta");
      TBranch *b_phi=kFactorsTree->GetBranch("phi");
      TBranch *bsign=kFactorsTree->GetBranch("sign");
      b_kf=kFactorsTree->GetBranch("kFactor");

      kFactorsTree->SetBranchAddress("eta", &eta, &b_eta);
      kFactorsTree->SetBranchAddress("phi", &phi, &b_phi);
      kFactorsTree->SetBranchAddress("sign", &signVar, &bsign);
      kFactorsTree->SetBranchAddress("kFactor", &kfd, &b_kf);
      //   Long64_t nbytes_int = 0, nb_int = 0;
      nentries_int = kFactorsTree->GetEntries();
      for(int jentry=0;jentry<nentries_int;++jentry){
	kFactorsTree->GetEntry(jentry);
	std::cout << "kFactor for ieta " << eta << " phi " << phi << " sign " << signVar << " is " << kfd << std::endl;
	kFactorsEtSumXtal[eta-1][phi-1][signVar]=kfd;
      }

      std::cout << "Opening kFactorsEndc " << kFactorsEndcFile << std::endl;
      TFile* kFactorsEndc = TFile::Open(kFactorsEndcFile);
      TTree* kFactorsEndcTree= (TTree*) kFactorsEndc->Get("kFactors_endc");
      TBranch *b_endcring=kFactorsEndcTree->GetBranch("ring");
      TBranch *b_endckf=kFactorsEndcTree->GetBranch("kfactor");
      kFactorsEndcTree->SetBranchAddress("ring", &ring, &b_endcring);
      kFactorsEndcTree->SetBranchAddress("kfactor", &kf, &b_endckf);
      //   Long64_t nbytes_int = 0, nb_int = 0;
      int nentries_endcint = kFactorsEndcTree->GetEntries();
      for(int jentry=0;jentry<nentries_endcint;++jentry){
	kFactorsEndcTree->GetEntry(jentry);
	std::cout << "kFactorEndc for iring " << ring << " is " << kf << std::endl;
	kFactorsEndcEtSum[ring-1]=kf;
      }
    }      
  
  std::vector<bsInfo> bsInfos;
  bsInfos.reserve(kIntervals);
  for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
    bsInfos[iinterval].reset();
  }
   
  std::cout << "Opening bsInfo  " << bsInfoFile << std::endl;
  TFile *fbs=TFile::Open(bsInfoFile);
  if (!fbs->IsOpen())
    std::cout << "Cannot open " << bsInfoFile << std::endl;
  TTree* bsInfoTree= (TTree*)fbs->Get("bsFinalTree");

  int timeVar;
  float bsPosVar=0,bsWidVar=0;
  unsigned int nEventsVar;
  unsigned long int nHitsEBVar;
  unsigned long int nHitsEEVar;

  TBranch *b_time=bsInfoTree->GetBranch("time_interval");
  TBranch *b_bsPos=bsInfoTree->GetBranch("bsPos");
  TBranch *b_bsWid=bsInfoTree->GetBranch("bsWid");
  TBranch *b_nEvents=bsInfoTree->GetBranch("nEvents");
  TBranch *b_nHitsEB=bsInfoTree->GetBranch("nHitsEB");
  TBranch *b_nHitsEE=bsInfoTree->GetBranch("nHitsEE");

  bsInfoTree->SetBranchAddress("time_interval", &timeVar, &b_time);
  bsInfoTree->SetBranchAddress("bsPos", &bsPosVar, &b_bsPos);
  bsInfoTree->SetBranchAddress("bsWid", &bsWidVar, &b_bsWid);
  bsInfoTree->SetBranchAddress("nEvents", &nEventsVar, &b_nEvents);
  bsInfoTree->SetBranchAddress("nHitsEB", &nHitsEBVar, &b_nHitsEB);
  bsInfoTree->SetBranchAddress("nHitsEE", &nHitsEEVar, &b_nHitsEE);

  int nentries_bs = bsInfoTree->GetEntries();
  for(int jentry=0;jentry<nentries_bs;++jentry){
    bsInfoTree->GetEntry(jentry);
    bsInfos[timeVar-1].bsPos=bsPosVar;
    bsInfos[timeVar-1].bsWid=bsWidVar;
    bsInfos[timeVar-1].nEvents=nEventsVar;
    bsInfos[timeVar-1].nHitsEB=nHitsEBVar;
    bsInfos[timeVar-1].nHitsEE=nHitsEEVar;
  }

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
  TF1* bsWidCorrections[85]; 
  TF1* bsWidCorrections_ee[kEndcRings]; 

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
	  name="f_bsWidAsymm_ieta_";
	  name+=(iieta+1);
	  std::cout << "Getting " << name << std::endl;
	  bsWidCorrections[iieta]=(TF1*)bsWeights->Get(name);
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
	   name="f_bsWidAsymm_ee_iring_";
	   name+=(iieta+1);
	   std::cout << "Getting " << name << std::endl;
	   bsWidCorrections_ee[iieta]=(TF1*)bsWeights->Get(name);
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
     }
  std::cout << "Configuration completed" << std::endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    if (jentry%10000==0)
      std::cout << jentry << "/" <<  nentries << std::endl;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    //Only EB for the moment
    if (time_interval>excludedRangeStart && time_interval<excludedRangeEnd)
      continue;

    if (det==0)
      {
	controls[time_interval-1].counter++;
	int tt=iTT(ieta,iphi,sign);
	int xtal=iXtal(ieta,iphi,sign);
	  
	double bsCorr=1.;
	double lumiCorr=1.;
	
	if (applyBSCorrection)
	  {
	    //Start applying correction on beam spot position
	    if (sign>0)
	      bsCorr=bsCorrections[ieta-1]->Eval(bsInfos[time_interval-1].bsPos);
	    else
	      bsCorr=bsCorrections[ieta-1]->Eval(-bsInfos[time_interval-1].bsPos);
	      
	    //Additional correction on beamSpot Width
	    bsCorr=bsCorr*bsWidCorrections[ieta-1]->Eval(bsInfos[time_interval-1].bsWid);
	  }
	
	if (applyLumiCorrection)
	  {
	    lumiCorr=lumiCorrections[ieta-1]->Eval(double(bsInfos[time_interval-1].nHitsEB)/double(bsInfos[time_interval-1].nEvents));
	  }
	
	et=et/bsCorr;
	et=et/lumiCorr;
  
	RMSet=RMSet*errEtCorr_factor;
	RMSetNoCorr=RMSetNoCorr*errEtCorr_factor;
	//Histories by eta ring
	controls[time_interval-1].nhitMean[ieta-1][sign]+=nHits;
	controls[time_interval-1].etSumMean[ieta-1][sign]+=et;
	controls[time_interval-1].etSumMeanRMS[ieta-1][sign]+=nHits*pow(RMSet,2);
	controls[time_interval-1].etMean[ieta-1][sign]+=et/(float)nHits;
	controls[time_interval-1].etMeanRMS[ieta-1][sign]+=pow(RMSet,2)/(float)nHits;
	//       controls[time_interval-1].etABRatio[ieta-1][sign]+=etB/etA;
	//       controls[time_interval-1].etABRatioRMS[ieta-1][sign]+=0.05*0.05*(etB/etA)*(etB/etA); //provisional value
	//       controls[time_interval-1].etMeanNoCorr[ieta-1][sign]+=etNoCorr/(float)nHits;
	//       controls[time_interval-1].etMeanNoCorrRMS[ieta-1][sign]+=nHits*pow(RMSetNoCorr,2);
	controls[time_interval-1].lcMean[ieta-1][sign]+=lc/(float)nHits;
	controls[time_interval-1].lcMeanRMS[ieta-1][sign]+=pow(RMSlc,2)/(float)nHits;
	controls[time_interval-1].alphaMean[ieta-1][sign]+=alpha/(float)nHits;
	controls[time_interval-1].alphaMeanRMS[ieta-1][sign]+=pow(RMSalpha,2)/(float)nHits;
	controls[time_interval-1].counterEta[ieta-1][sign]++;
	  
	//Histories by tt
	controls[time_interval-1].nhitTowerMean[tt-1]+=nHits;
	controls[time_interval-1].etSumTowerMeanVsEtRef[tt-1]+=et;
	controls[time_interval-1].etSumTowerMeanVsEtRefRMS[tt-1]+=nHits*pow(RMSet,2);
	controls[time_interval-1].etTowerMean[tt-1]+=et/(float)nHits;
	controls[time_interval-1].etTowerMeanRMS[tt-1]+=pow(RMSet,2)/(float)nHits;
	//       controls[time_interval-1].etTowerMeanNoCorr[tt-1]+=etNoCorr/float(nHits);
	//       controls[time_interval-1].etTowerMeanNoCorrRMS[tt-1]+=nHits*pow(RMSetNoCorr,2);
	controls[time_interval-1].lcTowerMean[tt-1]+=lc/float(nHits);
	controls[time_interval-1].lcTowerMeanRMS[tt-1]+=pow(RMSlc,2)/(float)nHits;
	controls[time_interval-1].alphaTowerMean[tt-1]+=alpha/float(nHits);
	controls[time_interval-1].alphaTowerMeanRMS[tt-1]+=pow(RMSalpha,2)/(float)nHits;
	controls[time_interval-1].counterTower[tt-1]++;
	  
	//Histories by xtal
	controls[time_interval-1].nhitXtalMean[xtal-1]+=nHits;
	controls[time_interval-1].etSumXtalMeanVsEtRef[xtal-1]+=et;
	controls[time_interval-1].etSumXtalMeanVsEtRefRMS[xtal-1]+=nHits*pow(RMSet,2);
	controls[time_interval-1].etXtalMean[xtal-1]+=et/(float)nHits;
	controls[time_interval-1].etXtalMeanRMS[xtal-1]+=pow(RMSet,2)/(float)nHits;
	//       controls[time_interval-1].etXtalMeanNoCorr[xtal-1]+=etNoCorr/(float)nHits;
	//       controls[time_interval-1].etXtalMeanNoCorrRMS[xtal-1]+=nHits*pow(RMSetNoCorr,2);
	controls[time_interval-1].lcXtalMean[xtal-1]+=lc/(float)nHits;
	controls[time_interval-1].lcXtalMeanRMS[xtal-1]+=pow(RMSlc,2)/(float)nHits;
	controls[time_interval-1].alphaXtalMean[xtal-1]+=alpha/(float)nHits;
	controls[time_interval-1].alphaXtalMeanRMS[xtal-1]+=pow(RMSalpha,2)/(float)nHits;
	controls[time_interval-1].counterXtal[xtal-1]++;
      }
    else if (det==1)
      {
	//	  continue;

	controlsEndc[time_interval-1].counter++;

	int iring=(endcapRings[ieta-1][iphi-1][sign]);	
	int tt=(endcapSCs[ieta-1][iphi-1][sign])+1;	
	int xtal=(endcapXtals[ieta-1][iphi-1][sign])+1;	
	//	  std::cout << ieta << "," << iphi << "," << sign << "," << iring << "," << tt << "," << xtal << std::endl;
 
	double bsCorr=1.;
	double lumiCorr=1.;
	  
	if (applyBSCorrection)
	  {
	    if (sign>0)
	      bsCorr=bsCorrections_ee[iring]->Eval(bsInfos[time_interval-1].bsPos);
	    else
	      bsCorr=bsCorrections_ee[iring]->Eval(-bsInfos[time_interval-1].bsPos);

	    bsCorr=bsCorr*bsWidCorrections_ee[iring]->Eval(bsInfos[time_interval-1].bsWid);

	  }
	
	if (applyLumiCorrection)
	  {
	    //Using EB <nHits> as luminometer
	    lumiCorr=lumiCorrections_ee[iring]->Eval(double(bsInfos[time_interval-1].nHitsEB)/double(bsInfos[time_interval-1].nEvents));
	  }
	
	et=et/bsCorr;
	et=et/lumiCorr;
	  
	RMSet=RMSet*errEtCorr_factor;
	RMSetNoCorr=RMSetNoCorr*errEtCorr_factor;
	//Histories by eta ring
	controlsEndc[time_interval-1].nhitMean[iring][sign]+=nHits;
	controlsEndc[time_interval-1].etSumMean[iring][sign]+=et;
	controlsEndc[time_interval-1].etSumMeanRMS[iring][sign]+=nHits*pow(RMSet,2);
	controlsEndc[time_interval-1].etMean[iring][sign]+=et/(float)nHits;
	controlsEndc[time_interval-1].etMeanRMS[iring][sign]+=pow(RMSet,2)/(float)nHits;
	//       controlsEndc[time_interval-1].etABRatio[iring][sign]+=etB/etA;
	//       controlsEndc[time_interval-1].etABRatioRMS[iring][sign]+=0.05*0.05*(etB/etA)*(etB/etA); //provisional value
	//       controlsEndc[time_interval-1].etMeanNoCorr[iring][sign]+=etNoCorr/(float)nHits;
	//       controlsEndc[time_interval-1].etMeanNoCorrRMS[iring][sign]+=nHits*pow(RMSetNoCorr,2);
	controlsEndc[time_interval-1].lcMean[iring][sign]+=lc/(float)nHits;
	controlsEndc[time_interval-1].lcMeanRMS[iring][sign]+=pow(RMSlc,2)/(float)nHits;
	controlsEndc[time_interval-1].alphaMean[iring][sign]+=alpha/(float)nHits;
	controlsEndc[time_interval-1].alphaMeanRMS[iring][sign]+=pow(RMSalpha,2)/(float)nHits;
	controlsEndc[time_interval-1].counterEta[iring][sign]++;
	  
	//Histories by tt
	controlsEndc[time_interval-1].nhitTowerMean[tt-1]+=nHits;
	controlsEndc[time_interval-1].etSumTowerMeanVsEtRef[tt-1]+=et;
	controlsEndc[time_interval-1].etSumTowerMeanVsEtRefRMS[tt-1]+=nHits*pow(RMSet,2);
	controlsEndc[time_interval-1].etTowerMean[tt-1]+=et/(float)nHits;
	controlsEndc[time_interval-1].etTowerMeanRMS[tt-1]+=pow(RMSet,2)/(float)nHits;
	//       controlsEndc[time_interval-1].etTowerMeanNoCorr[tt-1]+=etNoCorr/float(nHits);
	//       controlsEndc[time_interval-1].etTowerMeanNoCorrRMS[tt-1]+=nHits*pow(RMSetNoCorr,2);
	controlsEndc[time_interval-1].lcTowerMean[tt-1]+=lc/float(nHits);
	controlsEndc[time_interval-1].lcTowerMeanRMS[tt-1]+=pow(RMSlc,2)/(float)nHits;
	controlsEndc[time_interval-1].alphaTowerMean[tt-1]+=alpha/float(nHits);
	controlsEndc[time_interval-1].alphaTowerMeanRMS[tt-1]+=pow(RMSalpha,2)/(float)nHits;
	controlsEndc[time_interval-1].counterTower[tt-1]++;
	  
	//Histories by xtal
	controlsEndc[time_interval-1].nhitXtalMean[xtal-1]+=nHits;
	controlsEndc[time_interval-1].etSumXtalMeanVsEtRef[xtal-1]+=et;
	controlsEndc[time_interval-1].etSumXtalMeanVsEtRefRMS[xtal-1]+=nHits*pow(RMSet,2);
	controlsEndc[time_interval-1].etXtalMean[xtal-1]+=et/(float)nHits;
	controlsEndc[time_interval-1].etXtalMeanRMS[xtal-1]+=pow(RMSet,2)/(float)nHits;
	//       controlsEndc[time_interval-1].etXtalMeanNoCorr[xtal-1]+=etNoCorr/(float)nHits;
	//       controlsEndc[time_interval-1].etXtalMeanNoCorrRMS[xtal-1]+=nHits*pow(RMSetNoCorr,2);
	controlsEndc[time_interval-1].lcXtalMean[xtal-1]+=lc/(float)nHits;
	controlsEndc[time_interval-1].lcXtalMeanRMS[xtal-1]+=pow(RMSlc,2)/(float)nHits;
	controlsEndc[time_interval-1].alphaXtalMean[xtal-1]+=alpha/(float)nHits;
	controlsEndc[time_interval-1].alphaXtalMeanRMS[xtal-1]+=pow(RMSalpha,2)/(float)nHits;
	controlsEndc[time_interval-1].counterXtal[xtal-1]++;
      } 
  }

  //    int counterTotal=0;
  float nInterval[kIntervals];    
  float nIntervalError[kIntervals];    
  //   float nIntervalError[kInterval];
  //    float etTotal=0;    
  //    float etTotalNoCorr=0; 

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
	    //	     std::cout << "i " << i << " j " << j << " etMean " << controls[iinterval].etMean[i][j] << " etMeanErr " << controls[iinterval].etMeanRMS[i][j] << std::endl;
	     
	    // 	     controls[iinterval].etMeanNoCorr[i][j]= controls[iinterval].etMeanNoCorr[i][j]/(float)controls[iinterval].counterEta[i][j];
	    // 	     controls[iinterval].etMeanNoCorrRMS[i][j]= sqrt(controls[iinterval].etMeanNoCorrRMS[i][j])/((float)controls[iinterval].nhitMean[i][j]);// i want eerror on mean
	    // 	     controls[iinterval].etABRatio[i][j]=controls[iinterval].etABRatio[i][j]/(float)controls[iinterval].counterEta[i][j];
	    // 	     controls[iinterval].etABRatioRMS[i][j]=sqrt(controls[iinterval].etABRatioRMS[i][j])/sqrt(controls[iinterval].counterEta[i][j]);// i want eerror on mean
	    controls[iinterval].lcMeanRMS[i][j]= sqrt(controls[iinterval].lcMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean
	    controls[iinterval].lcMean[i][j]=controls[iinterval].lcMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	    controls[iinterval].alphaMeanRMS[i][j]= sqrt(controls[iinterval].alphaMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean
	    controls[iinterval].alphaMean[i][j]=controls[iinterval].alphaMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	    controls[iinterval].nhitMean[i][j]=controls[iinterval].nhitMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	  }
	}
       
      for (int i=0;i<kSM*kTowerPerSM;++i)
	{
	  controls[iinterval].etSumTowerMeanVsEtRef[i]=controls[iinterval].etSumTowerMeanVsEtRef[i]/(float)controls[iinterval].counterTower[i];
	  controls[iinterval].etSumTowerMeanVsEtRefRMS[i]=sqrt(controls[iinterval].etSumTowerMeanVsEtRefRMS[i])/(float)controls[iinterval].counterTower[i];
	  controls[iinterval].etTowerMean[i]=controls[iinterval].etTowerMean[i]/(float)controls[iinterval].counterTower[i];
	  controls[iinterval].etTowerMeanRMS[i]=sqrt(controls[iinterval].etTowerMeanRMS[i])/((float)controls[iinterval].counterTower[i]);// i want eerror on mean
	  // 	   controls[iinterval].etTowerMeanNoCorr[i]= controls[iinterval].etTowerMeanNoCorr[i]/(float)controls[iinterval].counterTower[i];
	  // 	   controls[iinterval].etTowerMeanNoCorrRMS[i]= sqrt(controls[iinterval].etTowerMeanNoCorrRMS[i])/((float)controls[iinterval]counterTower[i]);// i want eerror on mean
	  controls[iinterval].lcTowerMean[i]=controls[iinterval].lcTowerMean[i]/(float)controls[iinterval].counterTower[i];
	  controls[iinterval].lcTowerMeanRMS[i]= sqrt(controls[iinterval].lcTowerMeanRMS[i])/((float)controls[iinterval].counterTower[i]);// i want eerror on mean
	  controls[iinterval].alphaTowerMean[i]=controls[iinterval].alphaTowerMean[i]/(float)controls[iinterval].counterTower[i];
	  controls[iinterval].alphaTowerMeanRMS[i]= sqrt(controls[iinterval].alphaTowerMeanRMS[i])/((float)controls[iinterval].counterTower[i]);// i want eerror on mean
	  controls[iinterval].nhitTowerMean[i]=controls[iinterval].nhitTowerMean[i]/(float)controls[iinterval].counterTower[i];
	}
      for (int i=0;i<kSM*kXtalPerSM;++i)
	{
	  controls[iinterval].etSumXtalMeanVsEtRef[i]=controls[iinterval].etSumXtalMeanVsEtRef[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].etSumXtalMeanVsEtRefRMS[i]=sqrt(controls[iinterval].etSumXtalMeanVsEtRefRMS[i])/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].etXtalMean[i]=controls[iinterval].etXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].etXtalMeanRMS[i]=sqrt(controls[iinterval].etXtalMeanRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	  // 	   controls[iinterval].etXtalMeanNoCorr[i]= controls[iinterval].etXtalMeanNoCorr[i]/(float)controls[iinterval].counterXtal[i];
	  // 	   controls[iinterval].etXtalMeanNoCorrRMS[i]= sqrt(controls[iinterval].etXtalMeanNoCorrRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	  controls[iinterval].lcXtalMean[i]=controls[iinterval].lcXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].lcXtalMeanRMS[i]= sqrt(controls[iinterval].lcXtalMeanRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	  controls[iinterval].alphaXtalMean[i]=controls[iinterval].alphaXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].alphaXtalMeanRMS[i]= sqrt(controls[iinterval].alphaXtalMeanRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	  controls[iinterval].nhitXtalMean[i]=controls[iinterval].nhitXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	}
      nInterval[iinterval]=intervals->intervalTime(iinterval);
      nIntervalError[iinterval]=0.;
      //       nIntervalError[iinterval]=0;
    }
  }


  for(int iinterval=0;iinterval<kIntervals;iinterval++){
    if(controlsEndc[iinterval].counter!=0){
      for (int i=0;i<kEndcRings;++i)
	{
	  for(int j=0;j<kSides;++j){
	    controlsEndc[iinterval].etSumMean[i][j]=controlsEndc[iinterval].etSumMean[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	    controlsEndc[iinterval].etSumMeanRMS[i][j]=sqrt(controlsEndc[iinterval].etSumMeanRMS[i][j])/(float)controlsEndc[iinterval].counterEta[i][j];
	    controlsEndc[iinterval].etMean[i][j]=controlsEndc[iinterval].etMean[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	    controlsEndc[iinterval].etMeanRMS[i][j]=sqrt(controlsEndc[iinterval].etMeanRMS[i][j])/((float)controlsEndc[iinterval].counterEta[i][j]);// i want eerror on mean
	    //	     std::cout << "i " << i << " j " << j << " etMean " << controlsEndc[iinterval].etMean[i][j] << " etMeanErr " << controlsEndc[iinterval].etMeanRMS[i][j] << std::endl;
	     
	    // 	     controlsEndc[iinterval].etMeanNoCorr[i][j]= controlsEndc[iinterval].etMeanNoCorr[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	    // 	     controlsEndc[iinterval].etMeanNoCorrRMS[i][j]= sqrt(controlsEndc[iinterval].etMeanNoCorrRMS[i][j])/((float)controlsEndc[iinterval].nhitMean[i][j]);// i want eerror on mean
	    // 	     controlsEndc[iinterval].etABRatio[i][j]=controlsEndc[iinterval].etABRatio[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	    // 	     controlsEndc[iinterval].etABRatioRMS[i][j]=sqrt(controlsEndc[iinterval].etABRatioRMS[i][j])/sqrt(controlsEndc[iinterval].counterEta[i][j]);// i want eerror on mean
	    controlsEndc[iinterval].lcMeanRMS[i][j]= sqrt(controlsEndc[iinterval].lcMeanRMS[i][j])/((float)controlsEndc[iinterval].counterEta[i][j]);// i want eerror on mean
	    controlsEndc[iinterval].lcMean[i][j]=controlsEndc[iinterval].lcMean[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	    controlsEndc[iinterval].alphaMeanRMS[i][j]= sqrt(controlsEndc[iinterval].alphaMeanRMS[i][j])/((float)controlsEndc[iinterval].counterEta[i][j]);// i want eerror on mean
	    controlsEndc[iinterval].alphaMean[i][j]=controlsEndc[iinterval].alphaMean[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	    controlsEndc[iinterval].nhitMean[i][j]=controlsEndc[iinterval].nhitMean[i][j]/(float)controlsEndc[iinterval].counterEta[i][j];
	  }
	}
     
      for (int i=0;i<kEndcSCs;++i)
	{
	  controlsEndc[iinterval].etSumTowerMeanVsEtRef[i]=controlsEndc[iinterval].etSumTowerMeanVsEtRef[i]/(float)controlsEndc[iinterval].counterTower[i];
	  controlsEndc[iinterval].etSumTowerMeanVsEtRefRMS[i]=sqrt(controlsEndc[iinterval].etSumTowerMeanVsEtRefRMS[i])/(float)controlsEndc[iinterval].counterTower[i];
	  controlsEndc[iinterval].etTowerMean[i]=controlsEndc[iinterval].etTowerMean[i]/(float)controlsEndc[iinterval].counterTower[i];
	  controlsEndc[iinterval].etTowerMeanRMS[i]=sqrt(controlsEndc[iinterval].etTowerMeanRMS[i])/((float)controlsEndc[iinterval].counterTower[i]);// i want eerror on mean
	  // 	   controlsEndc[iinterval].etTowerMeanNoCorr[i]= controlsEndc[iinterval].etTowerMeanNoCorr[i]/(float)controlsEndc[iinterval].counterTower[i];
	  // 	   controlsEndc[iinterval].etTowerMeanNoCorrRMS[i]= sqrt(controlsEndc[iinterval].etTowerMeanNoCorrRMS[i])/((float)controlsEndc[iinterval]counterTower[i]);// i want eerror on mean
	  controlsEndc[iinterval].lcTowerMean[i]=controlsEndc[iinterval].lcTowerMean[i]/(float)controlsEndc[iinterval].counterTower[i];
	  controlsEndc[iinterval].lcTowerMeanRMS[i]= sqrt(controlsEndc[iinterval].lcTowerMeanRMS[i])/((float)controlsEndc[iinterval].counterTower[i]);// i want eerror on mean
	  controlsEndc[iinterval].alphaTowerMean[i]=controlsEndc[iinterval].alphaTowerMean[i]/(float)controlsEndc[iinterval].counterTower[i];
	  controlsEndc[iinterval].alphaTowerMeanRMS[i]= sqrt(controlsEndc[iinterval].alphaTowerMeanRMS[i])/((float)controlsEndc[iinterval].counterTower[i]);// i want eerror on mean
	  controlsEndc[iinterval].nhitTowerMean[i]=controlsEndc[iinterval].nhitTowerMean[i]/(float)controlsEndc[iinterval].counterTower[i];
	}
   
      for (int i=0;i<kEndcXtals;++i)
	{
	  controlsEndc[iinterval].etSumXtalMeanVsEtRef[i]=controlsEndc[iinterval].etSumXtalMeanVsEtRef[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].etSumXtalMeanVsEtRefRMS[i]=sqrt(controlsEndc[iinterval].etSumXtalMeanVsEtRefRMS[i])/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].etXtalMean[i]=controlsEndc[iinterval].etXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].etXtalMeanRMS[i]=sqrt(controlsEndc[iinterval].etXtalMeanRMS[i])/((float)controlsEndc[iinterval].counterXtal[i]);// i want eerror on mean
	  // 	   controlsEndc[iinterval].etXtalMeanNoCorr[i]= controlsEndc[iinterval].etXtalMeanNoCorr[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  // 	   controlsEndc[iinterval].etXtalMeanNoCorrRMS[i]= sqrt(controlsEndc[iinterval].etXtalMeanNoCorrRMS[i])/((float)controlsEndc[iinterval].counterXtal[i]);// i want eerror on mean
	  controlsEndc[iinterval].lcXtalMean[i]=controlsEndc[iinterval].lcXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].lcXtalMeanRMS[i]= sqrt(controlsEndc[iinterval].lcXtalMeanRMS[i])/((float)controlsEndc[iinterval].counterXtal[i]);// i want eerror on mean
	  controlsEndc[iinterval].alphaXtalMean[i]=controlsEndc[iinterval].alphaXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].alphaXtalMeanRMS[i]= sqrt(controlsEndc[iinterval].alphaXtalMeanRMS[i])/((float)controlsEndc[iinterval].counterXtal[i]);// i want eerror on mean
	  controlsEndc[iinterval].nhitXtalMean[i]=controlsEndc[iinterval].nhitXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	}
    }
  }



  //Arrays to be used in the graph
  float etSumMeanVsRefArray[kIntervals];
  float etSumMeanVsRefRMSArray[kIntervals];
  float etMeanArray[kIntervals];
  float etMeanRMSArray[kIntervals];
  //    float etMeanNoCorrArray[kIntervals];
  //    float etMeanNoCorrRMSArray[kIntervals];
  //    float etABRatioArray[kIntervals];
  //    float etABRatioRMSArray[kIntervals];
  float lcMeanArray[kIntervals];
  float lcMeanRMSArray[kIntervals];
  float alphaMeanArray[kIntervals];
  float alphaMeanRMSArray[kIntervals];
  float nhitMeanArray[kIntervals];
   
  float etSumTowerMeanVsRefArray[kIntervals];
  float etSumTowerMeanVsRefRMSArray[kIntervals];
  float etTowerMeanArray[kIntervals];
  float etTowerMeanRMSArray[kIntervals];
  //    float etTowerMeanNoCorrArray[kIntervals];
  //    float etTowerMeanNoCorrRMSArray[kIntervals];
  float lcTowerMeanArray[kIntervals];
  float lcTowerMeanRMSArray[kIntervals];
  float alphaTowerMeanArray[kIntervals];
  float alphaTowerMeanRMSArray[kIntervals];
  float nhitTowerMeanArray[kIntervals];

  float etSumXtalMeanVsRefArray[kIntervals];
  float etSumXtalMeanVsRefRMSArray[kIntervals];
  float etXtalMeanArray[kIntervals];
  float etXtalMeanRMSArray[kIntervals];
  //    float etXtalMeanNoCorrArray[kIntervals];
  //    float etXtalMeanNoCorrRMSArray[kIntervals];
  float lcXtalMeanArray[kIntervals];
  float lcXtalMeanRMSArray[kIntervals];
  float alphaXtalMeanArray[kIntervals];
  float alphaXtalMeanRMSArray[kIntervals];
  float nhitXtalMeanArray[kIntervals];


  TFile *outFileTree=TFile::Open(outFileName+"_fullTree.root","recreate");
  outFileTree->cd();
  unsigned int indexVar,indexTypeVar,timeIntervalVar,unixTimeVar;
  float  etSumMeanVsRefVar,etSumMeanVsRefRMSVar,etMeanVar,etMeanRMSVar,nhitMeanVar,lcMeanVar,lcMeanRMSVar,alphaMeanVar,alphaMeanRMSVar;
   
  TTree* newTree=new TTree("historyTree","historyTree");
  newTree->Branch("timeInterval",&timeIntervalVar,"timeInterval/i");
  newTree->Branch("index",&indexVar,"index/i");
  newTree->Branch("indexType",&indexTypeVar,"indexType/i");
  newTree->Branch("unixTime",&unixTimeVar,"unixTime/i");
  newTree->Branch("etSumMeanVsRef",&etSumMeanVsRefVar,"etSumMeanVsRef/F");
  newTree->Branch("etSumMeanVsRefRMS",&etSumMeanVsRefRMSVar,"etSumMeanVsRefRMS/F");
  newTree->Branch("etMean",&etMeanVar,"etMean/F");
  newTree->Branch("etMeanRMS",&etMeanRMSVar,"etMeanRMS/F");
  newTree->Branch("lcMean",&lcMeanVar,"lcMean/F");
  newTree->Branch("lcMeanRMS",&lcMeanRMSVar,"lcMeanRMS/F");
  newTree->Branch("alphaMean",&alphaMeanVar,"alphaMean/F");
  newTree->Branch("alphaMeanRMS",&alphaMeanRMSVar,"alphaMeanRMS/F");
  newTree->Branch("nhitMean",&nhitMeanVar,"nhitMean/F");

  //   int historyNormalizationInterval=40;

  //   kIntervals=487;

  TFile *outFile=TFile::Open(outFileName+"_etaRing.root","recreate");
  outFile->cd();
  for (int i=0;i<kBarlRings;++i)
    {
      for(int j=0;j<kSides;++j){
	std::cout << "Creating history for iring " <<  i+1 << "/85" << " side  "<<j+1<<"/2"<<std::endl;
	float etref=0;
	float etSumOverRef=0;
	// 	 float etABRatioref=0;
	// 	 float etNoCorrref=0;
	float lcref=0;
	float alpharef=0;
	float nhitref=0;
	int nref=0;
	 
	for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
	  {
	    nref++;
	    if (normalizationType == "ring")
	      etSumOverRef+=controls[historyNormalizationInterval+iref].etSumMean[i][j]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	    else if (normalizationType == "allEB")
	      {
		float etRef=0;
		for (int ii=0;ii<kBarlRings;++ii)
		  etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
		etRef=etRef/kBarlRings;
		etSumOverRef+=controls[historyNormalizationInterval+iref].etSumMean[i][j]/etRef;
	      }
	    etref+=controls[historyNormalizationInterval+iref].etMean[i][j];
	    // 	     etNoCorrref+=controls[historyNormalizationInterval+iref].etMeanNoCorr[i][j];
	    // 	     etABRatioref+=controls[historyNormalizationInterval+iref].etABRatio[i][j];
	    lcref+=controls[historyNormalizationInterval+iref].lcMean[i][j];
	    alpharef+=controls[historyNormalizationInterval+iref].alphaMean[i][j];
	    nhitref+=controls[historyNormalizationInterval+iref].nhitMean[i][j];
	  }
	 
	etSumOverRef=etSumOverRef/nref;
	etref=etref/nref;
	// 	 etABRatioref=etABRatioref/nref;
	// 	 etNoCorrref=etNoCorrref/nref;
	lcref=lcref/nref;
	alpharef=alpharef/nref;
	nhitref=nhitref/nref;


	 
	for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  //	   cout<<controls[iinterval].etMeanNoCorr[i][j]<<" "<<controls[iinterval].etMeanNoCorrRMS[i][j]<<" "<<controls[iinterval].lcMean[i][j]<<" "<<controls[iinterval].lcMeanRMS[i][j]<<endl;
	  //Normalizing to time reference interval
	  float nXtalRing=controls[iinterval].counterEta[i][j];
	  float kf=1.;
	  //	   float kFactorAB=1.;
	  if (kfactorCorr)
	    kf=kFactorsEtSum[i];
	  // 	   if (kfactorABCorr)
	  // 	     kFactorAB=(1+(controls[iinterval].etABRatio[i][j]/etABRatioref-1.)*kfactorAB_alpha)/((float)controls[iinterval].etABRatio[i][j]/etABRatioref);
	  float etSumRef=0.;
	  if (normalizationType == "ring")
	    etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	  else if (normalizationType == "allEB")
	    {
	      for (int ii=0;ii<kBarlRings;++ii)
		etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	      etSumRef=etSumRef/kBarlRings;
	    }
	  etSumMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumMean[i][j]/etSumRef)/etSumOverRef)-1.)/kf;
	  //Approximation now just multipliy the errors for 2
	  etSumMeanVsRefRMSArray[iinterval]= etSumMeanVsRefArray[iinterval] * sqrt(2) * ( controls[iinterval].etSumMeanRMS[i][j] / controls[iinterval].etSumMean[i][j] ) ;
	  etMeanArray[iinterval]=(controls[iinterval].etMean[i][j]/etref);
	  etMeanRMSArray[iinterval]=(controls[iinterval].etMeanRMS[i][j]/etref);
	  // 	   etMeanNoCorrArray[iinterval]=(controls[iinterval].etMeanNoCorr[i][j]/etNoCorrref);
	  // 	   etMeanNoCorrRMSArray[iinterval]=(controls[iinterval].etMeanNoCorrRMS[i][j]/(etNoCorrref));
	  // 	   etABRatioArray[iinterval]=(controls[iinterval].etABRatio[i][j]/etABRatioref)AB;
	  // 	   etABRatioRMSArray[iinterval]=(controls[iinterval].etABRatioRMS[i][j]/etABRatioref)AB;
	  nhitMeanArray[iinterval]=controls[iinterval].nhitMean[i][j]/nhitref;
	  lcMeanArray[iinterval]=1/(controls[iinterval].lcMean[i][j]/lcref);
	  lcMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcMean[i][j],2)*controls[iinterval].lcMeanRMS[i][j];
	  alphaMeanArray[iinterval]=controls[iinterval].alphaMean[i][j];
	  alphaMeanRMSArray[iinterval]=controls[iinterval].alphaMeanRMS[i][j];

	  indexTypeVar=0;
	  indexVar=(kBarlRings)*j+i+1;
	  timeIntervalVar=iinterval;
	  unixTimeVar=nInterval[iinterval];
	  nhitMeanVar=nhitMeanArray[iinterval];
	  etSumMeanVsRefVar=etSumMeanVsRefArray[iinterval];
	  etSumMeanVsRefRMSVar=etSumMeanVsRefRMSArray[iinterval];
	  etMeanVar=etMeanArray[iinterval];
	  etMeanRMSVar=etMeanRMSArray[iinterval];
	  lcMeanVar=lcMeanArray[iinterval];
	  lcMeanRMSVar=lcMeanRMSArray[iinterval];
	  alphaMeanVar=alphaMeanArray[iinterval];
	  alphaMeanRMSVar=alphaMeanRMSArray[iinterval];
	  newTree->Fill();
	  //Now normalizing other regions to reference region
	  // 	   if (i!=0)
	  // 	     {
	  // 	       etMeanArray[iinterval]=etMeanArray[i][j][iinterval]/etMeanArray[0][iinterval];
	  // 	       etMeanNoCorrArray[i][j][iinterval]=etMeanNoCorrArray[i][j][iinterval]/etMeanNoCorrArray[0][iinterval];
	  // 	       nhitMeanArray[i][j][iinterval]=nhitMeanArray[i][j][iinterval]/nhitMeanArray[0][iinterval];
	  // 	       lcMeanArray[i][j][iinterval]=lcMeanArray[i][j][iinterval]/lcMeanArray[0][iinterval];
	  // 	     }
	}
	 
	TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitMeanArray[0]);
	TString etaLabel="ieta_";
	TString sideLabel="_side_";
	etaLabel+=(i+1);
	sideLabel+=j;
	 
	nhitGraph->SetName("nHit_"+etaLabel+sideLabel);
	nhitGraph->SetTitle("nHit_"+etaLabel+sideLabel);
	nhitGraph->GetYaxis()->SetTitle("nhit");
	nhitGraph->GetXaxis()->SetTitle("interval");
	nhitGraph->Write();
	delete nhitGraph;


	TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumMeanVsRefArray[0],nIntervalError,&etSumMeanVsRefRMSArray[0]);
	etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel+sideLabel);
	etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel+sideLabel);
	etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
	etSumOverRefGraph->GetXaxis()->SetTitle("interval");
       
	etSumOverRefGraph->Write();
	delete etSumOverRefGraph;

	TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etMeanArray[0],nIntervalError,&etMeanRMSArray[0]);
	etGraph->SetName("et_"+etaLabel+sideLabel);
	etGraph->SetTitle("et_"+etaLabel+sideLabel);
	etGraph->GetYaxis()->SetTitle("<et>");   
	etGraph->GetXaxis()->SetTitle("interval");
       
	etGraph->Write();
	delete etGraph;
       
	// 	 TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etMeanNoCorrArray[0],nIntervalError,&etMeanNoCorrRMSArray[0]);
	// 	 etNoCorrGraph->SetName("etNoCorr_"+etaLabel+sideLabel);
	// 	 etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel+sideLabel);
	// 	 etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	// 	 etNoCorrGraph->GetXaxis()->SetTitle("interval");
       
	// 	 etNoCorrGraph->Write();
	// 	 delete etNoCorrGraph;
       
	// 	 TGraphErrors* etABRatioGraph=new TGraphErrors(kIntervals,nInterval,&etABRatioArray[0],nIntervalError,&etABRatioRMSArray[0]);
	// 	 etABRatioGraph->SetName("etABRatio_"+etaLabel+sideLabel);
	// 	 etABRatioGraph->SetTitle("etABRatio_"+etaLabel+sideLabel);
	// 	 etABRatioGraph->GetYaxis()->SetTitle("<etABRatio>");   
	// 	 etABRatioGraph->GetXaxis()->SetTitle("interval");
       
	// 	 etABRatioGraph->Write();
	// 	 delete etABRatioGraph;

	TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcMeanArray[0],nIntervalError,&lcMeanRMSArray[0]);
	lcGraph->SetName("lc_"+etaLabel+sideLabel);
	lcGraph->SetTitle("lc_"+etaLabel+sideLabel);
	lcGraph->GetYaxis()->SetTitle("<tl>");   
	lcGraph->GetXaxis()->SetTitle("interval");
       
	lcGraph->Write();
	delete lcGraph;

	TGraphErrors* alphaGraph=new TGraphErrors(kIntervals,nInterval,&alphaMeanArray[0],nIntervalError,&alphaMeanRMSArray[0]);
	alphaGraph->SetName("alpha_"+etaLabel+sideLabel);
	alphaGraph->SetTitle("alpha_"+etaLabel+sideLabel);
	alphaGraph->GetYaxis()->SetTitle("<tl>");   
	alphaGraph->GetXaxis()->SetTitle("interval");
       
	alphaGraph->Write();
	delete alphaGraph;

	// 	 TGraphErrors* EtNoCorrvsTLGraph=new TGraphErrors((kIntervals-200),&tlMeanArray[200],&etMeanNoCorrArray[200],&tlMeanRMSArray[200],&etMeanNoCorrRMSArray[200]);
	// 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel+sideLabel);       
	// 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel+sideLabel);       
	// 	 EtNoCorrvsTLGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	// 	 EtNoCorrvsTLGraph->GetXaxis()->SetTitle("<lc>");

	// 	 EtNoCorrvsTLGraph->Write();
	// 	 delete EtNoCorrvsTLGraph;
      }
    }

  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_endcRing.root","recreate");
  outFile->cd();
  for (int i=0;i<kEndcRings;++i)
    {
      for(int j=0;j<kSides;++j){
	std::cout << "Creating history for endcap iring " <<  i+1 << "/39" << " side  "<<j+1<<"/2"<<std::endl;
	float etref=0;
	float etSumOverRef=0;
	// 	 float etABRatioref=0;
	// 	 float etNoCorrref=0;
	float lcref=0;
	float alpharef=0;
	float nhitref=0;
	int nref=0;
	 
	for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
	  {
	    nref++;
	    if (normalizationType == "ring")
	      etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumMean[i][j]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	    else if (normalizationType == "allEB")
	      {
		float etRef=0;
		for (int ii=0;ii<kBarlRings;++ii)
		  etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
		etRef=etRef/kBarlRings;
		etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumMean[i][j]/etRef;
	      }
	    etref+=controlsEndc[historyNormalizationInterval+iref].etMean[i][j];
	    // 	     etNoCorrref+=controlsEndc[historyNormalizationInterval+iref].etMeanNoCorr[i][j];
	    // 	     etABRatioref+=controlsEndc[historyNormalizationInterval+iref].etABRatio[i][j];
	    lcref+=controlsEndc[historyNormalizationInterval+iref].lcMean[i][j];
	    alpharef+=controlsEndc[historyNormalizationInterval+iref].alphaMean[i][j];
	    nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitMean[i][j];
	  }
	 
	etSumOverRef=etSumOverRef/nref;
	etref=etref/nref;
	// 	 etABRatioref=etABRatioref/nref;
	// 	 etNoCorrref=etNoCorrref/nref;
	lcref=lcref/nref;
	alpharef=alpharef/nref;
	nhitref=nhitref/nref;


	 
	for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  //	   cout<<controlsEndc[iinterval].etMeanNoCorr[i][j]<<" "<<controlsEndc[iinterval].etMeanNoCorrRMS[i][j]<<" "<<controlsEndc[iinterval].lcMean[i][j]<<" "<<controlsEndc[iinterval].lcMeanRMS[i][j]<<endl;
	  //Normalizing to time reference interval
	  float nXtalRing=controlsEndc[iinterval].counterEta[i][j];
	  float kf=1.;
	  //	   float kFactorAB=1.;
	  if (kfactorCorr)
	    kf=kFactorsEndcEtSum[i];
	  // 	   if (kfactorABCorr)
	  // 	     kFactorAB=(1+(controlsEndc[iinterval].etABRatio[i][j]/etABRatioref-1.)*kfactorAB_alpha)/((float)controlsEndc[iinterval].etABRatio[i][j]/etABRatioref);
	  float etSumRef=0.;
	  if (normalizationType == "ring")
	    etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	  else if (normalizationType == "allEB")
	    {
	      for (int ii=0;ii<kBarlRings;++ii)
		etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	      etSumRef=etSumRef/kBarlRings;
	    }
	  etSumMeanVsRefArray[iinterval]=1 + (((controlsEndc[iinterval].etSumMean[i][j]/etSumRef)/etSumOverRef)-1.)/kf;
	  //Approximation now just multipliy the errors for 2
	  etSumMeanVsRefRMSArray[iinterval]= etSumMeanVsRefArray[iinterval] * sqrt(2) * ( controlsEndc[iinterval].etSumMeanRMS[i][j] / controlsEndc[iinterval].etSumMean[i][j] ) ;
	  etMeanArray[iinterval]=(controlsEndc[iinterval].etMean[i][j]/etref);
	  etMeanRMSArray[iinterval]=(controlsEndc[iinterval].etMeanRMS[i][j]/etref);
	  // 	   etMeanNoCorrArray[iinterval]=(controlsEndc[iinterval].etMeanNoCorr[i][j]/etNoCorrref);
	  // 	   etMeanNoCorrRMSArray[iinterval]=(controlsEndc[iinterval].etMeanNoCorrRMS[i][j]/(etNoCorrref));
	  // 	   etABRatioArray[iinterval]=(controlsEndc[iinterval].etABRatio[i][j]/etABRatioref)AB;
	  // 	   etABRatioRMSArray[iinterval]=(controlsEndc[iinterval].etABRatioRMS[i][j]/etABRatioref)AB;
	  nhitMeanArray[iinterval]=controlsEndc[iinterval].nhitMean[i][j]/nhitref;
	  lcMeanArray[iinterval]=1/(controlsEndc[iinterval].lcMean[i][j]/lcref);
	  lcMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcMean[i][j],2)*controlsEndc[iinterval].lcMeanRMS[i][j];
	  alphaMeanArray[iinterval]=controlsEndc[iinterval].alphaMean[i][j];
	  alphaMeanRMSArray[iinterval]=controlsEndc[iinterval].alphaMeanRMS[i][j];
	  //Now normalizing other regions to reference region
	  // 	   if (i!=0)
	  // 	     {
	  // 	       etMeanArray[iinterval]=etMeanArray[i][j][iinterval]/etMeanArray[0][iinterval];
	  // 	       etMeanNoCorrArray[i][j][iinterval]=etMeanNoCorrArray[i][j][iinterval]/etMeanNoCorrArray[0][iinterval];
	  // 	       nhitMeanArray[i][j][iinterval]=nhitMeanArray[i][j][iinterval]/nhitMeanArray[0][iinterval];
	  // 	       lcMeanArray[i][j][iinterval]=lcMeanArray[i][j][iinterval]/lcMeanArray[0][iinterval];
	  // 	     }
	  indexTypeVar=3;
	  indexVar=kEndcRings*j+i+1;
	  timeIntervalVar=iinterval;
	  unixTimeVar=nInterval[iinterval];
	  nhitMeanVar=nhitMeanArray[iinterval];
	  etSumMeanVsRefVar=etSumMeanVsRefArray[iinterval];
	  etSumMeanVsRefRMSVar=etSumMeanVsRefRMSArray[iinterval];
	  etMeanVar=etMeanArray[iinterval];
	  etMeanRMSVar=etMeanRMSArray[iinterval];
	  lcMeanVar=lcMeanArray[iinterval];
	  lcMeanRMSVar=lcMeanRMSArray[iinterval];
	  alphaMeanVar=alphaMeanArray[iinterval];
	  alphaMeanRMSVar=alphaMeanRMSArray[iinterval];
	  newTree->Fill();
	}
	 
	TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitMeanArray[0]);
	TString etaLabel="iring_";
	TString sideLabel="_side_";
	etaLabel+=(i+1);
	sideLabel+=j;
	 
	nhitGraph->SetName("nHit_"+etaLabel+sideLabel);
	nhitGraph->SetTitle("nHit_"+etaLabel+sideLabel);
	nhitGraph->GetYaxis()->SetTitle("nhit");
	nhitGraph->GetXaxis()->SetTitle("interval");
	nhitGraph->Write();
	delete nhitGraph;


	TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumMeanVsRefArray[0],nIntervalError,&etSumMeanVsRefRMSArray[0]);
	etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel+sideLabel);
	etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel+sideLabel);
	etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
	etSumOverRefGraph->GetXaxis()->SetTitle("interval");
     
	etSumOverRefGraph->Write();
	delete etSumOverRefGraph;

	TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etMeanArray[0],nIntervalError,&etMeanRMSArray[0]);
	etGraph->SetName("et_"+etaLabel+sideLabel);
	etGraph->SetTitle("et_"+etaLabel+sideLabel);
	etGraph->GetYaxis()->SetTitle("<et>");   
	etGraph->GetXaxis()->SetTitle("interval");
     
	etGraph->Write();
	delete etGraph;
     
	// 	 TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etMeanNoCorrArray[0],nIntervalError,&etMeanNoCorrRMSArray[0]);
	// 	 etNoCorrGraph->SetName("etNoCorr_"+etaLabel+sideLabel);
	// 	 etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel+sideLabel);
	// 	 etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	// 	 etNoCorrGraph->GetXaxis()->SetTitle("interval");
     
	// 	 etNoCorrGraph->Write();
	// 	 delete etNoCorrGraph;
     
	// 	 TGraphErrors* etABRatioGraph=new TGraphErrors(kIntervals,nInterval,&etABRatioArray[0],nIntervalError,&etABRatioRMSArray[0]);
	// 	 etABRatioGraph->SetName("etABRatio_"+etaLabel+sideLabel);
	// 	 etABRatioGraph->SetTitle("etABRatio_"+etaLabel+sideLabel);
	// 	 etABRatioGraph->GetYaxis()->SetTitle("<etABRatio>");   
	// 	 etABRatioGraph->GetXaxis()->SetTitle("interval");
     
	// 	 etABRatioGraph->Write();
	// 	 delete etABRatioGraph;

	TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcMeanArray[0],nIntervalError,&lcMeanRMSArray[0]);
	lcGraph->SetName("lc_"+etaLabel+sideLabel);
	lcGraph->SetTitle("lc_"+etaLabel+sideLabel);
	lcGraph->GetYaxis()->SetTitle("<tl>");   
	lcGraph->GetXaxis()->SetTitle("interval");
     
	lcGraph->Write();
	delete lcGraph;

	TGraphErrors* alphaGraph=new TGraphErrors(kIntervals,nInterval,&alphaMeanArray[0],nIntervalError,&alphaMeanRMSArray[0]);
	alphaGraph->SetName("alpha_"+etaLabel+sideLabel);
	alphaGraph->SetTitle("alpha_"+etaLabel+sideLabel);
	alphaGraph->GetYaxis()->SetTitle("<tl>");   
	alphaGraph->GetXaxis()->SetTitle("interval");
     
	alphaGraph->Write();
	delete alphaGraph;

	// 	 TGraphErrors* EtNoCorrvsTLGraph=new TGraphErrors((kIntervals-200),&tlMeanArray[200],&etMeanNoCorrArray[200],&tlMeanRMSArray[200],&etMeanNoCorrRMSArray[200]);
	// 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel+sideLabel);       
	// 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel+sideLabel);       
	// 	 EtNoCorrvsTLGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	// 	 EtNoCorrvsTLGraph->GetXaxis()->SetTitle("<lc>");

	// 	 EtNoCorrvsTLGraph->Write();
	// 	 delete EtNoCorrvsTLGraph;
      }
    }

  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_itt.root","recreate");
  outFile->cd();
  for (int i=0;i<kSM*kTowerPerSM;++i)
    {
      std::cout << "Creating history for itt " <<  i+1 << "/" << kSM*kTowerPerSM << std::endl;
      float etref=0;
      float etSumOverRef=0;
      //        float etNoCorrref=0;
      float lcref=0;
      float alpharef=0;
      float nhitref=0;
      int nref=0;

      for (int iref=-5;iref<6;++iref)
	{
	  nref++;
	  if (normalizationType == "ring")
	    etSumOverRef+=controls[historyNormalizationInterval+iref].etSumTowerMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	  else if (normalizationType == "allEB")
	    {
	      float etRef=0;
	      for (int ii=0;ii<kBarlRings;++ii)
		etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
	      etRef=etRef/kBarlRings;
	      etSumOverRef+=controls[historyNormalizationInterval+iref].etSumTowerMeanVsEtRef[i]/etRef;
	    }
	  etref+=controls[historyNormalizationInterval+iref].etTowerMean[i];
	  // 	   etNoCorrref+=controls[historyNormalizationInterval+iref].etTowerMeanNoCorr[i];
	  lcref+=controls[historyNormalizationInterval+iref].lcTowerMean[i];
	  alpharef+=controls[historyNormalizationInterval+iref].alphaTowerMean[i];
	  nhitref+=controls[historyNormalizationInterval+iref].nhitTowerMean[i];
	}

      etSumOverRef=etSumOverRef/nref;
      etref=etref/nref;
      //        etNoCorrref=etNoCorrref/nref;
      lcref=lcref/nref;
      alpharef=alpharef/nref;
      nhitref=nhitref/nref;

      float kf=1.;
      if (kfactorCorr)
	{
	  int iT=((i)%kTowerPerSM);
	  int ie=(int)iT/4;
	  kf=0;
	  for (int iii=ie*5;iii<(ie+1)*5;++iii)
	    kf+=kFactorsEtSum[iii];
	  kf=kf/5;
	  //std::cout << "kFactor for TT " << i << " iT " << iT << " ie " << ie << " is " << kf << std::endl;
	}

      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval

	float etSumRef=0.;
	if (normalizationType == "ring")
	  etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	else if (normalizationType == "allEB")
	  {
	    for (int ii=0;ii<kBarlRings;++ii)
	      etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	    etSumRef=etSumRef/kBarlRings;
	  }
	etSumTowerMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumTowerMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	etSumTowerMeanVsRefRMSArray[iinterval]= etSumTowerMeanVsRefArray[iinterval] * ( controls[iinterval].etSumTowerMeanVsEtRefRMS[i] / controls[iinterval].etSumTowerMeanVsEtRef[i] )  ;
	etTowerMeanArray[iinterval]=(controls[iinterval].etTowerMean[i]/etref);
	etTowerMeanRMSArray[iinterval]=(controls[iinterval].etTowerMeanRMS[i]/etref);
	// 	 etTowerMeanNoCorrArray[iinterval]=(controls[iinterval].etTowerMeanNoCorr[i]/etNoCorrref);
	// 	 etTowerMeanNoCorrRMSArray[iinterval]=(controls[iinterval].etTowerMeanNoCorrRMS[i]/etNoCorrref);
	nhitTowerMeanArray[iinterval]=controls[iinterval].nhitTowerMean[i]/nhitref;
	lcTowerMeanArray[iinterval]=1/(controls[iinterval].lcTowerMean[i]/lcref);
	lcTowerMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcTowerMean[i],2)*controls[iinterval].lcTowerMeanRMS[i];
	alphaTowerMeanArray[iinterval]=controls[iinterval].alphaTowerMean[i];
	alphaTowerMeanRMSArray[iinterval]=controls[iinterval].alphaTowerMeanRMS[i];

	indexTypeVar=1;
	indexVar=i+1;
	timeIntervalVar=iinterval;
	unixTimeVar=nInterval[iinterval];
	nhitMeanVar=nhitTowerMeanArray[iinterval];
	etSumMeanVsRefVar=etSumTowerMeanVsRefArray[iinterval];
	etSumMeanVsRefRMSVar=etSumTowerMeanVsRefRMSArray[iinterval];
	etMeanVar=etTowerMeanArray[iinterval];
	etMeanRMSVar=etTowerMeanRMSArray[iinterval];
	lcMeanVar=lcTowerMeanArray[iinterval];
	lcMeanRMSVar=lcTowerMeanRMSArray[iinterval];
	alphaMeanVar=alphaTowerMeanArray[iinterval];
	alphaMeanRMSVar=alphaTowerMeanRMSArray[iinterval];
	newTree->Fill();
	//Now normalizing other regions to reference region
	// 	   if (i!=0)
	// 	     {
	// 	       etMeanArray[iinterval]=etMeanArray[iinterval]/etMeanArray[0][iinterval];
	// 	       etMeanNoCorrArray[iinterval]=etMeanNoCorrArray[iinterval]/etMeanNoCorrArray[0][iinterval];
	// 	       nhitMeanArray[iinterval]=nhitMeanArray[iinterval]/nhitMeanArray[0][iinterval];
	// 	       lcMeanArray[iinterval]=lcMeanArray[iinterval]/lcMeanArray[0][iinterval];
	// 	     }
      }
      TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitTowerMeanArray[0]);
      TString etaLabel="itt_";
      etaLabel+=(i+1);
       
      nhitGraph->SetName("nHit_"+etaLabel);
      nhitGraph->SetTitle("nHit_"+etaLabel);
      nhitGraph->GetYaxis()->SetTitle("nhit");
      nhitGraph->GetXaxis()->SetTitle("interval");
      nhitGraph->Write();
      delete nhitGraph;

      TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumTowerMeanVsRefArray[0],nIntervalError,&etSumTowerMeanVsRefRMSArray[0]);
      etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
      etSumOverRefGraph->GetXaxis()->SetTitle("interval");
       
      etSumOverRefGraph->Write();
      delete etSumOverRefGraph;

      TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etTowerMeanArray[0],nIntervalError,&etTowerMeanRMSArray[0]);
      etGraph->SetName("et_"+etaLabel);
      etGraph->SetTitle("et_"+etaLabel);
      etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
      etGraph->GetXaxis()->SetTitle("interval");
       
      etGraph->Write();
      delete etGraph;
       
      //        TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etTowerMeanNoCorrArray[0],nIntervalError,&etTowerMeanNoCorrRMSArray[0]);
      //        etNoCorrGraph->SetName("etNoCorr_"+etaLabel);
      // 	 etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel);
      // 	 etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      // 	 etNoCorrGraph->GetXaxis()->SetTitle("interval");
	 
      // 	 etNoCorrGraph->Write();
      // 	 delete etNoCorrGraph;
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcTowerMeanArray[0],nIntervalError,&lcTowerMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

      TGraphErrors* alphaGraph=new TGraphErrors(kIntervals,nInterval,&alphaTowerMeanArray[0],nIntervalError,&alphaTowerMeanRMSArray[0]);
      alphaGraph->SetName("alpha_"+etaLabel);
      alphaGraph->SetTitle("alpha_"+etaLabel);
      alphaGraph->GetYaxis()->SetTitle("<tl>");   
      alphaGraph->GetXaxis()->SetTitle("interval");
	 
      alphaGraph->Write();
      delete alphaGraph;

      // 	 TGraphErrors* EtNoCorrvsTLGraph=new TGraphErrors((kIntervals-200),&tlTowerMeanArray[200],&etTowerMeanNoCorrArray[200],&tlTowerMeanRMSArray[200],&etTowerMeanNoCorrRMSArray[200]);
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      // 	 EtNoCorrvsTLGraph->GetXaxis()->SetTitle("<lc>");

      // 	 EtNoCorrvsTLGraph->Write();       
      // 	 delete EtNoCorrvsTLGraph;
       
    }
   
  outFile->Write();
  outFile->Close();
   
   
  outFile=TFile::Open(outFileName+"_endcSCs.root","recreate");
  outFile->cd();
  for (int i=0;i<kEndcSCs;++i)
    {
      std::cout << "Creating history for iSC " <<  i+1 << "/" << kEndcSCs << std::endl;
      float etref=0;
      float etSumOverRef=0;
      //        float etNoCorrref=0;
      float lcref=0;
      float alpharef=0;
      float nhitref=0;
      int nref=0;

      for (int iref=-5;iref<6;++iref)
	{
	  nref++;
	  if (normalizationType == "ring")
	    etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumTowerMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	  else if (normalizationType == "allEB")
	    {
	      float etRef=0;
	      for (int ii=0;ii<kBarlRings;++ii)
		etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
	      etRef=etRef/kBarlRings;
	      etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumTowerMeanVsEtRef[i]/etRef;
	    }
	  etref+=controlsEndc[historyNormalizationInterval+iref].etTowerMean[i];
	  // 	   etNoCorrref+=controlsEndc[historyNormalizationInterval+iref].etTowerMeanNoCorr[i];
	  lcref+=controlsEndc[historyNormalizationInterval+iref].lcTowerMean[i];
	  alpharef+=controlsEndc[historyNormalizationInterval+iref].alphaTowerMean[i];
	  nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitTowerMean[i];
	}

      etSumOverRef=etSumOverRef/nref;
      etref=etref/nref;
      //        etNoCorrref=etNoCorrref/nref;
      lcref=lcref/nref;
      alpharef=alpharef/nref;
      nhitref=nhitref/nref;

      float kf=1.;
      if (kfactorCorr)
	{
	  kf=0;
	  int nxtalsSC=0;
	  for (int isign=0;isign<kSides;++isign)
	    for (int ix=0;ix<kEndcWedgesX;++ix)
	      for (int iy=0;iy<kEndcWedgesX;++iy)
		{
		  if (endcapSCs[ix][iy][isign]==i)
		    {
		      //		       std::cout << i << "," << ix << "," << iy << "," << sign << "," << endcapRings[ix][iy][isign] << std::endl;
		      kf+=kFactorsEndcEtSum[endcapRings[ix][iy][isign]];
		      nxtalsSC++;
		    }
		}
	  kf=kf/(float)nxtalsSC;
	  //	   std::cout << "Found " << nxtalsSC << "belonging to SC " << i << " kf is " << kf << std::endl;

	}

      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval

	float etSumRef=0.;
	if (normalizationType == "ring")
	  etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	else if (normalizationType == "allEB")
	  {
	    for (int ii=0;ii<kBarlRings;++ii)
	      etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	    etSumRef=etSumRef/kBarlRings;
	  }
	etSumTowerMeanVsRefArray[iinterval]=1 + (((controlsEndc[iinterval].etSumTowerMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	etSumTowerMeanVsRefRMSArray[iinterval]= etSumTowerMeanVsRefArray[iinterval] * ( controlsEndc[iinterval].etSumTowerMeanVsEtRefRMS[i] / controlsEndc[iinterval].etSumTowerMeanVsEtRef[i] )  ;
	etTowerMeanArray[iinterval]=(controlsEndc[iinterval].etTowerMean[i]/etref);
	etTowerMeanRMSArray[iinterval]=(controlsEndc[iinterval].etTowerMeanRMS[i]/etref);
	// 	 etTowerMeanNoCorrArray[iinterval]=(controlsEndc[iinterval].etTowerMeanNoCorr[i]/etNoCorrref);
	// 	 etTowerMeanNoCorrRMSArray[iinterval]=(controlsEndc[iinterval].etTowerMeanNoCorrRMS[i]/etNoCorrref);
	nhitTowerMeanArray[iinterval]=controlsEndc[iinterval].nhitTowerMean[i]/nhitref;
	lcTowerMeanArray[iinterval]=1/(controlsEndc[iinterval].lcTowerMean[i]/lcref);
	lcTowerMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcTowerMean[i],2)*controlsEndc[iinterval].lcTowerMeanRMS[i];
	alphaTowerMeanArray[iinterval]=controlsEndc[iinterval].alphaTowerMean[i];
	alphaTowerMeanRMSArray[iinterval]=controlsEndc[iinterval].alphaTowerMeanRMS[i];

	indexTypeVar=4;
	indexVar=i+1;
	timeIntervalVar=iinterval;
	unixTimeVar=nInterval[iinterval];
	nhitMeanVar=nhitTowerMeanArray[iinterval];
	etSumMeanVsRefVar=etSumTowerMeanVsRefArray[iinterval];
	etSumMeanVsRefRMSVar=etSumTowerMeanVsRefRMSArray[iinterval];
	etMeanVar=etTowerMeanArray[iinterval];
	etMeanRMSVar=etTowerMeanRMSArray[iinterval];
	lcMeanVar=lcTowerMeanArray[iinterval];
	lcMeanRMSVar=lcTowerMeanRMSArray[iinterval];
	alphaMeanVar=alphaTowerMeanArray[iinterval];
	alphaMeanRMSVar=alphaTowerMeanRMSArray[iinterval];
	newTree->Fill();
	//Now normalizing other regions to reference region
	// 	   if (i!=0)
	// 	     {
	// 	       etMeanArray[iinterval]=etMeanArray[iinterval]/etMeanArray[0][iinterval];
	// 	       etMeanNoCorrArray[iinterval]=etMeanNoCorrArray[iinterval]/etMeanNoCorrArray[0][iinterval];
	// 	       nhitMeanArray[iinterval]=nhitMeanArray[iinterval]/nhitMeanArray[0][iinterval];
	// 	       lcMeanArray[iinterval]=lcMeanArray[iinterval]/lcMeanArray[0][iinterval];
	// 	     }
      }
      TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitTowerMeanArray[0]);
      TString etaLabel="iSC_";
      etaLabel+=(i+1);
     
      nhitGraph->SetName("nHit_"+etaLabel);
      nhitGraph->SetTitle("nHit_"+etaLabel);
      nhitGraph->GetYaxis()->SetTitle("nhit");
      nhitGraph->GetXaxis()->SetTitle("interval");
      nhitGraph->Write();
      delete nhitGraph;

      TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumTowerMeanVsRefArray[0],nIntervalError,&etSumTowerMeanVsRefRMSArray[0]);
      etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
      etSumOverRefGraph->GetXaxis()->SetTitle("interval");
     
      etSumOverRefGraph->Write();
      delete etSumOverRefGraph;

      TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etTowerMeanArray[0],nIntervalError,&etTowerMeanRMSArray[0]);
      etGraph->SetName("et_"+etaLabel);
      etGraph->SetTitle("et_"+etaLabel);
      etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
      etGraph->GetXaxis()->SetTitle("interval");
     
      etGraph->Write();
      delete etGraph;
     
      //        TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etTowerMeanNoCorrArray[0],nIntervalError,&etTowerMeanNoCorrRMSArray[0]);
      //        etNoCorrGraph->SetName("etNoCorr_"+etaLabel);
      // 	 etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel);
      // 	 etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      // 	 etNoCorrGraph->GetXaxis()->SetTitle("interval");
	 
      // 	 etNoCorrGraph->Write();
      // 	 delete etNoCorrGraph;
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcTowerMeanArray[0],nIntervalError,&lcTowerMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

      TGraphErrors* alphaGraph=new TGraphErrors(kIntervals,nInterval,&alphaTowerMeanArray[0],nIntervalError,&alphaTowerMeanRMSArray[0]);
      alphaGraph->SetName("alpha_"+etaLabel);
      alphaGraph->SetTitle("alpha_"+etaLabel);
      alphaGraph->GetYaxis()->SetTitle("<tl>");   
      alphaGraph->GetXaxis()->SetTitle("interval");
	 
      alphaGraph->Write();
      delete alphaGraph;

      // 	 TGraphErrors* EtNoCorrvsTLGraph=new TGraphErrors((kIntervals-200),&tlTowerMeanArray[200],&etTowerMeanNoCorrArray[200],&tlTowerMeanRMSArray[200],&etTowerMeanNoCorrRMSArray[200]);
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      // 	 EtNoCorrvsTLGraph->GetXaxis()->SetTitle("<lc>");

      // 	 EtNoCorrvsTLGraph->Write();       
      // 	 delete EtNoCorrvsTLGraph;
     
    }

  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_ixtal.root","recreate");
  outFile->cd();
  for (int i=0;i<kSM*kXtalPerSM;++i)
    {
      std::cout << "Creating history for ixtal " <<  i+1 << "/" << kSM*kXtalPerSM << std::endl;
      float etref=0;
      //        float etNoCorrref=0;
      float lcref=0;
      float alpharef=0;
      float nhitref=0;
      int nref=0;
      float etSumOverRef=0;

      for (int iref=-5;iref<6;++iref)
	{
	  nref++;
	  etref+=controls[historyNormalizationInterval+iref].etXtalMean[i];
	  if (normalizationType == "ring")
	    etSumOverRef+=controls[historyNormalizationInterval+iref].etSumXtalMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	  else if (normalizationType == "allEB")
	    {
	      float etRef=0;
	      for (int ii=0;ii<kBarlRings;++ii)
		etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
	      etRef=etRef/kBarlRings;
	      etSumOverRef+=controls[historyNormalizationInterval+iref].etSumXtalMeanVsEtRef[i]/etRef;
	    }
	  // 	   etNoCorrref+=controls[historyNormalizationInterval+iref].etXtalMeanNoCorr[i];
	  lcref+=controls[historyNormalizationInterval+iref].lcXtalMean[i];
	  alpharef+=controls[historyNormalizationInterval+iref].alphaXtalMean[i];
	  nhitref+=controls[historyNormalizationInterval+iref].nhitXtalMean[i];
	}

      etref=etref/nref;
      etSumOverRef=etSumOverRef/nref;
      //        etNoCorrref=etNoCorrref/nref;
      lcref=lcref/nref;
      alpharef=alpharef/nref;
      nhitref=nhitref/nref;

      float kf=1.;
      if (kfactorCorr)
	{
	  //Probably Buggy
	  // 	  int ix=((i)%kXtalPerSM);
	  // 	  int ie=(int)ix/20;
	  int ie=(i)/360+1;
	  int ip=(i)%360+1;
	  if (ie>85)
	    ie=ie-85;
	  else
	    ie=-ie;
	  int sig=(ie>0) ? 1:0; 
	  ie=TMath::Abs(ie);
	  kf=kFactorsEtSumXtal[ie-1][ip-1][sig];
					   
	  //	   std::cout << "kFactor for xtal " << i << " ix " << ix << " ie " << ie << " is " << kf << std::endl;
	}
      
      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval
	
	float etSumRef=0.;
	if (normalizationType == "ring")
	  etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	else if (normalizationType == "allEB")
	  {
	    for (int ii=0;ii<kBarlRings;++ii)
	      etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	    etSumRef=etSumRef/kBarlRings;
	  }
	etSumXtalMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumXtalMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	etSumXtalMeanVsRefRMSArray[iinterval]= etSumXtalMeanVsRefArray[iinterval] * ( controls[iinterval].etSumXtalMeanVsEtRefRMS[i] / controls[iinterval].etSumXtalMeanVsEtRef[i] );
	etXtalMeanArray[iinterval]=(controls[iinterval].etXtalMean[i]/etref);
	etXtalMeanRMSArray[iinterval]=(controls[iinterval].etXtalMeanRMS[i]/etref);
	// 	 etXtalMeanNoCorrArray[iinterval]=(controls[iinterval].etXtalMeanNoCorr[i]/etNoCorrref);
	// 	 etXtalMeanNoCorrRMSArray[iinterval]=(controls[iinterval].etXtalMeanNoCorrRMS[i]/etNoCorrref);
	nhitXtalMeanArray[iinterval]=controls[iinterval].nhitXtalMean[i]/nhitref;
	lcXtalMeanArray[iinterval]=1/(controls[iinterval].lcXtalMean[i]/lcref);
	lcXtalMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcXtalMean[i],2)*controls[iinterval].lcXtalMeanRMS[i];
	alphaXtalMeanArray[iinterval]=controls[iinterval].alphaXtalMean[i];
	alphaXtalMeanRMSArray[iinterval]=controls[iinterval].alphaXtalMeanRMS[i];

	indexTypeVar=2;
	indexVar=i+1;
	timeIntervalVar=iinterval;
	unixTimeVar=nInterval[iinterval];
	nhitMeanVar=nhitXtalMeanArray[iinterval];
	etSumMeanVsRefVar=etSumXtalMeanVsRefArray[iinterval];
	etSumMeanVsRefRMSVar=etSumXtalMeanVsRefRMSArray[iinterval];
	etMeanVar=etXtalMeanArray[iinterval];
	etMeanRMSVar=etXtalMeanRMSArray[iinterval];
	lcMeanVar=lcXtalMeanArray[iinterval];
	lcMeanRMSVar=lcXtalMeanRMSArray[iinterval];
	alphaMeanVar=alphaXtalMeanArray[iinterval];
	alphaMeanRMSVar=alphaXtalMeanRMSArray[iinterval];
	newTree->Fill();
	//Now normalizing other regions to reference region
	// 	   if (i!=0)
	// 	     {
	// 	       etMeanArray[iinterval]=etMeanArray[iinterval]/etMeanArray[0][iinterval];
	// 	       etMeanNoCorrArray[iinterval]=etMeanNoCorrArray[iinterval]/etMeanNoCorrArray[0][iinterval];
	// 	       nhitMeanArray[iinterval]=nhitMeanArray[iinterval]/nhitMeanArray[0][iinterval];
	// 	       lcMeanArray[iinterval]=lcMeanArray[iinterval]/lcMeanArray[0][iinterval];
	// 	     }
      }
      TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitXtalMeanArray[0]);
      TString etaLabel="ixtal_";
      etaLabel+=(i+1);
       
      nhitGraph->SetName("nHit_"+etaLabel);
      nhitGraph->SetTitle("nHit_"+etaLabel);
      nhitGraph->GetYaxis()->SetTitle("nhit");
      nhitGraph->GetXaxis()->SetTitle("interval");
      nhitGraph->Write();
      delete nhitGraph;

      TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etXtalMeanArray[0],nIntervalError,&etXtalMeanRMSArray[0]);
      etGraph->SetName("et_"+etaLabel);
      etGraph->SetTitle("et_"+etaLabel);
      etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
      etGraph->GetXaxis()->SetTitle("interval");
       
      etGraph->Write();
      delete etGraph;


      TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumXtalMeanVsRefArray[0],nIntervalError,&etSumXtalMeanVsRefRMSArray[0]);
      etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
      etSumOverRefGraph->GetXaxis()->SetTitle("interval");
       
      etSumOverRefGraph->Write();
      delete etSumOverRefGraph;
       
      //        TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etXtalMeanNoCorrArray[0],nIntervalError,&etXtalMeanNoCorrRMSArray[0]);
      //        etNoCorrGraph->SetName("etNoCorr_"+etaLabel);
      //        etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel);
      //        etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      //        etNoCorrGraph->GetXaxis()->SetTitle("interval");
       
      //        etNoCorrGraph->Write();
      //        delete etNoCorrGraph;
	 
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcXtalMeanArray[0],nIntervalError,&lcXtalMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

      TGraphErrors* alphaGraph=new TGraphErrors(kIntervals,nInterval,&alphaXtalMeanArray[0],nIntervalError,&alphaXtalMeanRMSArray[0]);
      alphaGraph->SetName("alpha_"+etaLabel);
      alphaGraph->SetTitle("alpha_"+etaLabel);
      alphaGraph->GetYaxis()->SetTitle("<tl>");   
      alphaGraph->GetXaxis()->SetTitle("interval");
	 
      alphaGraph->Write();
      delete alphaGraph;

      // 	 TGraphErrors* EtNoCorrvsTLGraph=new TGraphErrors((kIntervals-200),&tlXtalMeanArray[200],&etXtalMeanNoCorrArray[200],&tlXtalMeanRMSArray[200],&etXtalMeanNoCorrRMSArray[200]);
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      // 	 EtNoCorrvsTLGraph->GetXaxis()->SetTitle("<lc>");

      // 	 EtNoCorrvsTLGraph->Write();       
      // 	 delete EtNoCorrvsTLGraph;
       
    }
  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_ixtalEndc.root","recreate");
  outFile->cd();
  for (int i=0;i<kEndcXtals;++i)
    {
      std::cout << "Creating history for ixtal " <<  i+1 << "/" << kEndcXtals << std::endl;
      float etref=0;
      //        float etNoCorrref=0;
      float lcref=0;
      float alpharef=0;
      float nhitref=0;
      int nref=0;
      float etSumOverRef=0;

      for (int iref=-5;iref<6;++iref)
	{
	  nref++;
	  etref+=controlsEndc[historyNormalizationInterval+iref].etXtalMean[i];
	  if (normalizationType == "ring")
	    etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumXtalMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	  else if (normalizationType == "allEB")
	    {
	      float etRef=0;
	      for (int ii=0;ii<kBarlRings;++ii)
		etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
	      etRef=etRef/kBarlRings;
	      etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumXtalMeanVsEtRef[i]/etRef;
	    }
	  // 	   etNoCorrref+=controlsEndc[historyNormalizationInterval+iref].etXtalMeanNoCorr[i];
	  lcref+=controlsEndc[historyNormalizationInterval+iref].lcXtalMean[i];
	  alpharef+=controlsEndc[historyNormalizationInterval+iref].alphaXtalMean[i];
	  nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitXtalMean[i];
	}

      etref=etref/nref;
      etSumOverRef=etSumOverRef/nref;
      //        etNoCorrref=etNoCorrref/nref;
      lcref=lcref/nref;
      alpharef=alpharef/nref;
      nhitref=nhitref/nref;

      float kf=1.;
      if (kfactorCorr)
	{
	  int nxtals=0;
	  for (int isign=0;isign<kSides;++isign)
	    for (int ix=0;ix<kEndcWedgesX;++ix)
	      for (int iy=0;iy<kEndcWedgesX;++iy)
		{
		  if (endcapXtals[ix][iy][isign]==i)
		    {
		      kf=kFactorsEndcEtSum[endcapRings[ix][iy][isign]];
		      nxtals++;
		    }
		}
	  //	   std::cout << "Found " << nxtals << "belonging to xtal " << i << " kf is " << kf << std::endl;
	}
     
      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval

	float etSumRef=0.;
	if (normalizationType == "ring")
	  etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	else if (normalizationType == "allEB")
	  {
	    for (int ii=0;ii<kBarlRings;++ii)
	      etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	    etSumRef=etSumRef/kBarlRings;
	  }
	etSumXtalMeanVsRefArray[iinterval]=1 + (((controlsEndc[iinterval].etSumXtalMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	etSumXtalMeanVsRefRMSArray[iinterval]= etSumXtalMeanVsRefArray[iinterval] * ( controlsEndc[iinterval].etSumXtalMeanVsEtRefRMS[i] / controlsEndc[iinterval].etSumXtalMeanVsEtRef[i] );
	etXtalMeanArray[iinterval]=(controlsEndc[iinterval].etXtalMean[i]/etref);
	etXtalMeanRMSArray[iinterval]=(controlsEndc[iinterval].etXtalMeanRMS[i]/etref);
	// 	 etXtalMeanNoCorrArray[iinterval]=(controlsEndc[iinterval].etXtalMeanNoCorr[i]/etNoCorrref);
	// 	 etXtalMeanNoCorrRMSArray[iinterval]=(controlsEndc[iinterval].etXtalMeanNoCorrRMS[i]/etNoCorrref);
	nhitXtalMeanArray[iinterval]=controlsEndc[iinterval].nhitXtalMean[i]/nhitref;
	lcXtalMeanArray[iinterval]=1/(controlsEndc[iinterval].lcXtalMean[i]/lcref);
	lcXtalMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcXtalMean[i],2)*controlsEndc[iinterval].lcXtalMeanRMS[i];
	alphaXtalMeanArray[iinterval]=controlsEndc[iinterval].alphaXtalMean[i];
	alphaXtalMeanRMSArray[iinterval]=controlsEndc[iinterval].alphaXtalMeanRMS[i];

	indexTypeVar=5;
	indexVar=i+1;
	timeIntervalVar=iinterval;
	unixTimeVar=nInterval[iinterval];
	nhitMeanVar=nhitXtalMeanArray[iinterval];
	etSumMeanVsRefVar=etSumXtalMeanVsRefArray[iinterval];
	etSumMeanVsRefRMSVar=etSumXtalMeanVsRefRMSArray[iinterval];
	etMeanVar=etXtalMeanArray[iinterval];
	etMeanRMSVar=etXtalMeanRMSArray[iinterval];
	lcMeanVar=lcXtalMeanArray[iinterval];
	lcMeanRMSVar=lcXtalMeanRMSArray[iinterval];
	alphaMeanVar=alphaXtalMeanArray[iinterval];
	alphaMeanRMSVar=alphaXtalMeanRMSArray[iinterval];
	newTree->Fill();

	//Now normalizing other regions to reference region
	// 	   if (i!=0)
	// 	     {
	// 	       etMeanArray[iinterval]=etMeanArray[iinterval]/etMeanArray[0][iinterval];
	// 	       etMeanNoCorrArray[iinterval]=etMeanNoCorrArray[iinterval]/etMeanNoCorrArray[0][iinterval];
	// 	       nhitMeanArray[iinterval]=nhitMeanArray[iinterval]/nhitMeanArray[0][iinterval];
	// 	       lcMeanArray[iinterval]=lcMeanArray[iinterval]/lcMeanArray[0][iinterval];
	// 	     }
      }
      TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitXtalMeanArray[0]);
      TString etaLabel="iendcxtal_";
      etaLabel+=(i+1);
     
      nhitGraph->SetName("nHit_"+etaLabel);
      nhitGraph->SetTitle("nHit_"+etaLabel);
      nhitGraph->GetYaxis()->SetTitle("nhit");
      nhitGraph->GetXaxis()->SetTitle("interval");
      nhitGraph->Write();
      delete nhitGraph;

      TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etXtalMeanArray[0],nIntervalError,&etXtalMeanRMSArray[0]);
      etGraph->SetName("et_"+etaLabel);
      etGraph->SetTitle("et_"+etaLabel);
      etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
      etGraph->GetXaxis()->SetTitle("interval");
     
      etGraph->Write();
      delete etGraph;


      TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumXtalMeanVsRefArray[0],nIntervalError,&etSumXtalMeanVsRefRMSArray[0]);
      etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
      etSumOverRefGraph->GetXaxis()->SetTitle("interval");
     
      etSumOverRefGraph->Write();
      delete etSumOverRefGraph;
     
      //        TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etXtalMeanNoCorrArray[0],nIntervalError,&etXtalMeanNoCorrRMSArray[0]);
      //        etNoCorrGraph->SetName("etNoCorr_"+etaLabel);
      //        etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel);
      //        etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      //        etNoCorrGraph->GetXaxis()->SetTitle("interval");
     
      //        etNoCorrGraph->Write();
      //        delete etNoCorrGraph;
	 
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcXtalMeanArray[0],nIntervalError,&lcXtalMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

      TGraphErrors* alphaGraph=new TGraphErrors(kIntervals,nInterval,&alphaXtalMeanArray[0],nIntervalError,&alphaXtalMeanRMSArray[0]);
      alphaGraph->SetName("alpha_"+etaLabel);
      alphaGraph->SetTitle("alpha_"+etaLabel);
      alphaGraph->GetYaxis()->SetTitle("<tl>");   
      alphaGraph->GetXaxis()->SetTitle("interval");
	 
      alphaGraph->Write();
      delete alphaGraph;

      // 	 TGraphErrors* EtNoCorrvsTLGraph=new TGraphErrors((kIntervals-200),&tlXtalMeanArray[200],&etXtalMeanNoCorrArray[200],&tlXtalMeanRMSArray[200],&etXtalMeanNoCorrRMSArray[200]);
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->SetName("EtNoCorrvsTL_"+etaLabel);       
      // 	 EtNoCorrvsTLGraph->GetYaxis()->SetTitle("<etNoCorr>");   
      // 	 EtNoCorrvsTLGraph->GetXaxis()->SetTitle("<lc>");

      // 	 EtNoCorrvsTLGraph->Write();       
      // 	 delete EtNoCorrvsTLGraph;
     
    }

  outFile->Write();
  outFile->Close();

  outFileTree->Write();
  outFileTree->Close();

}
