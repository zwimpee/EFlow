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

  harnessMap=new HarnessMap(harnessMapFile);

  TFile *kFactors;
  float  kFactorsEtSum[kBarlRings];
  float  kFactorsEtSumXtal[kBarlRings][kBarlWedges][kSides];
  float  kFactorsEndcEtSum[kEndcRings];
  float  kFactorsEndcEtSumXtal[kEndcWedgesX][kEndcWedgesY][kSides];

  if (kfactorCorr)
    {

      if (!kfactorVsTime)
	{
	  std::cout << "Opening kFactors " << kFactorsXtalFile << std::endl;
	  kFactors = TFile::Open(kFactorsXtalFile);
	  TTree* kFactorsTree= (TTree*) kFactors->Get("kFactors_xtals");
	  int eta,phi,signVar;
	  double kfd;
	  TBranch *b_eta=kFactorsTree->GetBranch("ieta");
	  TBranch *b_phi=kFactorsTree->GetBranch("iphi");
	  TBranch *bsign=kFactorsTree->GetBranch("sign");
	  TBranch *b_kf=kFactorsTree->GetBranch("kFactor");
	  
	  kFactorsTree->SetBranchAddress("ieta", &eta, &b_eta);
	  kFactorsTree->SetBranchAddress("iphi", &phi, &b_phi);
	  kFactorsTree->SetBranchAddress("sign", &signVar, &bsign);
	  kFactorsTree->SetBranchAddress("kFactor", &kfd, &b_kf);
	  //   Long64_t nbytes_int = 0, nb_int = 0;
	  int nentries_int = kFactorsTree->GetEntries();
	  int nx[kBarlRings];
	  for (int ii=0;ii<kBarlRings;++ii)
	    {
	      nx[ii]=0;
	      kFactorsEtSum[ii]=0;
	    }
	  

	  for(int jentry=0;jentry<nentries_int;++jentry){
	    kFactorsTree->GetEntry(jentry);
	    std::cout << "kFactor for ieta " << eta << " phi " << phi << " sign " << signVar << " is " << kfd << std::endl;
	    kFactorsEtSumXtal[eta-1][phi-1][signVar]=kfd;
	    if (kfd>0)
	      {
		nx[eta-1]++;
		kFactorsEtSum[eta-1]+=kfd;
	      }
	  }
      
	  for (int ii=0;ii<kBarlRings;++ii)
	    {
	      if (nx[ii]<=0)
		continue;
	      kFactorsEtSum[ii]=(float)kFactorsEtSum[ii]/(float)nx[ii];
	      std::cout << "kFactor for ieta " << ii << " is " << kFactorsEtSum[ii] << std::endl;
	    }
	  
	  std::cout << "Opening kFactorsEndc " << kFactorsEndcFile << std::endl;
	  TFile* kFactorsEndc = TFile::Open(kFactorsEndcFile);
	  kFactorsTree= (TTree*) kFactorsEndc->Get("kFactors_xtals_ee");
	  int ix,iy,ring;
	  TBranch *b_ix=kFactorsTree->GetBranch("ix");
	  TBranch *b_iy=kFactorsTree->GetBranch("iy");
	  TBranch *b_ring=kFactorsTree->GetBranch("ring");
	  bsign=kFactorsTree->GetBranch("sign");
	  b_kf=kFactorsTree->GetBranch("kFactor");
	  
	  kFactorsTree->SetBranchAddress("ix", &ix, &b_ix);
	  kFactorsTree->SetBranchAddress("iy", &iy, &b_iy);
	  kFactorsTree->SetBranchAddress("ring", &ring, &b_ring);
	  kFactorsTree->SetBranchAddress("sign", &signVar, &bsign);
	  kFactorsTree->SetBranchAddress("kFactor", &kfd, &b_kf);
	  //   Long64_t nbytes_int = 0, nb_int = 0;
	  nentries_int = kFactorsTree->GetEntries();
	  int nxc[kEndcRings];
	  for (int ii=0;ii<kEndcRings;++ii)
	    {
	      nxc[ii]=0;
	      kFactorsEndcEtSum[ii]=0;
	    }
	  
	  for(int jentry=0;jentry<nentries_int;++jentry){
	    kFactorsTree->GetEntry(jentry);
	    std::cout << "kFactor for ix " << ix << " iy " << iy << " sign " << signVar << " is " << kfd << std::endl;
	    kFactorsEndcEtSumXtal[ix-1][ix-1][signVar]=kfd;
	    if (kfd>0)
	      {
		kFactorsEndcEtSum[ring-1]+=kfd;
		nxc[ring-1]++;
	      }
	  }
      
	  for (int ii=0;ii<kEndcRings;++ii)
	    {
	      if (nxc[ii]<=0)
		continue;
	      kFactorsEndcEtSum[ii]=(float)kFactorsEndcEtSum[ii]/(float)nxc[ii];
	      std::cout << "kFactor for iring " << ii << " is " << kFactorsEndcEtSum[ii] << std::endl;
	    }

	}
      else
	{
	  std::cout << "Creating KFactorsVsTime from " <<  kFactorsVsTimeFile << std::endl;
	  kfactorsVsTime= new KFactorsVsTime(kFactorsVsTimeFile,useKFactorsPerXtal);
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
  ULong64_t nHitsEBVar;
  ULong64_t nHitsEEVar;

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
      for (int iy=0;iy<kEndcWedgesY;++iy)
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
	int harness=harnessMap->harnessHashed(0,ieta,iphi,sign);

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
	controls[time_interval-1].ringStability[ieta-1][sign].addMeasurement(nHits,et,RMSet,lc,RMSlc);
	  
	//Histories by tt
	controls[time_interval-1].towerStability[tt-1].addMeasurement(nHits,et,RMSet,lc,RMSlc);

	//Histories by harness
	controls[time_interval-1].harnessStability[harness].addMeasurement(nHits,et,RMSet,lc,RMSlc);
	  
	//Histories by xtal
	controls[time_interval-1].nhitXtalMean[xtal-1]+=nHits;
	controls[time_interval-1].etSumXtalMeanVsEtRef[xtal-1]+=et;
	controls[time_interval-1].etSumXtalMeanVsEtRefRMS[xtal-1]+=nHits*pow(RMSet,2);
	controls[time_interval-1].etXtalMean[xtal-1]+=et/(float)nHits;
	controls[time_interval-1].etXtalMeanRMS[xtal-1]+=pow(RMSet,2)/(float)nHits;
	controls[time_interval-1].lcXtalMean[xtal-1]+=lc/(float)nHits;
	controls[time_interval-1].lcXtalMeanRMS[xtal-1]+=pow(RMSlc,2)/(float)nHits;
	controls[time_interval-1].counterXtal[xtal-1]++;
      }
    else if (det==1)
      {
	controlsEndc[time_interval-1].counter++;

	int iring=(endcapRings[ieta-1][iphi-1][sign]);	
	int tt=(endcapSCs[ieta-1][iphi-1][sign])+1;	
	int xtal=(endcapXtals[ieta-1][iphi-1][sign])+1;	
	int harness=harnessMap->harnessHashed(1,ieta,iphi,sign);
 
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
	controlsEndc[time_interval-1].ringStability[iring][sign].addMeasurement(nHits,et,RMSet,lc,RMSlc);
	//Histories by tt
	controlsEndc[time_interval-1].towerStability[tt-1].addMeasurement(nHits,et,RMSet,lc,RMSlc);

	//Histories by harness
	controlsEndc[time_interval-1].harnessStability[harness].addMeasurement(nHits,et,RMSet,lc,RMSlc);
	  
	//Histories by xtal
	controlsEndc[time_interval-1].nhitXtalMean[xtal-1]+=nHits;
	controlsEndc[time_interval-1].etSumXtalMeanVsEtRef[xtal-1]+=et;
	controlsEndc[time_interval-1].etSumXtalMeanVsEtRefRMS[xtal-1]+=nHits*pow(RMSet,2);
	controlsEndc[time_interval-1].etXtalMean[xtal-1]+=et/(float)nHits;
	controlsEndc[time_interval-1].etXtalMeanRMS[xtal-1]+=pow(RMSet,2)/(float)nHits;
	controlsEndc[time_interval-1].lcXtalMean[xtal-1]+=lc/(float)nHits;
	controlsEndc[time_interval-1].lcXtalMeanRMS[xtal-1]+=pow(RMSlc,2)/(float)nHits;
	controlsEndc[time_interval-1].counterXtal[xtal-1]++;
      } 
  }

  float nInterval[kIntervals];    
  float nIntervalError[kIntervals];    

  cout<<"loop ended"<<endl;

  for(int iinterval=0;iinterval<kIntervals;iinterval++){
    if(controls[iinterval].counter!=0){
      for (int i=0;i<kBarlRings;++i)
	{
	  for(int j=0;j<kSides;++j){

 	  RegionStability::measurement aMeasurement=controls[iinterval].ringStability[i][j].getTruncatedMeanMeasurement(3,10);
 	  controls[iinterval].counterEta[i][j]=aMeasurement.nChannels;
 	  controls[iinterval].etSumMean[i][j]=aMeasurement.etSumMean;
 	  controls[iinterval].etSumMeanRMS[i][j]=aMeasurement.etSumMeanErr;
 	  controls[iinterval].etMean[i][j]=aMeasurement.etMeanMean;
 	  controls[iinterval].etMeanRMS[i][j]=aMeasurement.etMeanMeanErr;
 	  controls[iinterval].lcMean[i][j]=aMeasurement.lcMean;
 	  controls[iinterval].lcMeanRMS[i][j]=aMeasurement.lcMeanErr;
 	  controls[iinterval].nhitMean[i][j]=aMeasurement.nHitsMean;
	  }
	}
       
      for (int i=0;i<kSM*kTowerPerSM;++i)
	{
 	  RegionStability::measurement aMeasurement=controls[iinterval].towerStability[i].getTruncatedMeanMeasurement(3,10);
 	  controls[iinterval].counterTower[i]=aMeasurement.nChannels;
 	  controls[iinterval].etSumTowerMeanVsEtRef[i]=aMeasurement.etSumMean;
 	  controls[iinterval].etSumTowerMeanVsEtRefRMS[i]=aMeasurement.etSumMeanErr;
 	  controls[iinterval].etTowerMean[i]=aMeasurement.etMeanMean;
 	  controls[iinterval].etTowerMeanRMS[i]=aMeasurement.etMeanMeanErr;
 	  controls[iinterval].lcTowerMean[i]=aMeasurement.lcMean;
 	  controls[iinterval].lcTowerMeanRMS[i]=aMeasurement.lcMeanErr;
 	  controls[iinterval].nhitTowerMean[i]=aMeasurement.nHitsMean;
	}

      for (int i=0;i<kSM*kHarnessPerSM;++i)
	{
 	  RegionStability::measurement aMeasurement=controls[iinterval].harnessStability[i].getTruncatedMeanMeasurement(3,10);
 	  controls[iinterval].counterHarness[i]=aMeasurement.nChannels;
 	  controls[iinterval].etSumHarnessMeanVsEtRef[i]=aMeasurement.etSumMean;
 	  controls[iinterval].etSumHarnessMeanVsEtRefRMS[i]=aMeasurement.etSumMeanErr;
 	  controls[iinterval].etHarnessMean[i]=aMeasurement.etMeanMean;
 	  controls[iinterval].etHarnessMeanRMS[i]=aMeasurement.etMeanMeanErr;
 	  controls[iinterval].lcHarnessMean[i]=aMeasurement.lcMean;
 	  controls[iinterval].lcHarnessMeanRMS[i]=aMeasurement.lcMeanErr;
 	  controls[iinterval].nhitHarnessMean[i]=aMeasurement.nHitsMean;
	}

      for (int i=0;i<kSM*kXtalPerSM;++i)
	{
	  controls[iinterval].etSumXtalMeanVsEtRef[i]=controls[iinterval].etSumXtalMeanVsEtRef[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].etSumXtalMeanVsEtRefRMS[i]=sqrt(controls[iinterval].etSumXtalMeanVsEtRefRMS[i])/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].etXtalMean[i]=controls[iinterval].etXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].etXtalMeanRMS[i]=sqrt(controls[iinterval].etXtalMeanRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	  controls[iinterval].lcXtalMean[i]=controls[iinterval].lcXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	  controls[iinterval].lcXtalMeanRMS[i]= sqrt(controls[iinterval].lcXtalMeanRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	  controls[iinterval].nhitXtalMean[i]=controls[iinterval].nhitXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	}
      nInterval[iinterval]=intervals->intervalTime(iinterval);
      nIntervalError[iinterval]=0.;
    }
  }


  for(int iinterval=0;iinterval<kIntervals;iinterval++){
    if(controlsEndc[iinterval].counter!=0){
      for (int i=0;i<kEndcRings;++i)
	{
	  for(int j=0;j<kSides;++j){

 	  RegionStability::measurement aMeasurement=controlsEndc[iinterval].ringStability[i][j].getTruncatedMeanMeasurement(3,10);
 	  controlsEndc[iinterval].counterEta[i][j]=aMeasurement.nChannels;
 	  controlsEndc[iinterval].etSumMean[i][j]=aMeasurement.etSumMean;
 	  controlsEndc[iinterval].etSumMeanRMS[i][j]=aMeasurement.etSumMeanErr;
 	  controlsEndc[iinterval].etMean[i][j]=aMeasurement.etMeanMean;
 	  controlsEndc[iinterval].etMeanRMS[i][j]=aMeasurement.etMeanMeanErr;
 	  controlsEndc[iinterval].lcMean[i][j]=aMeasurement.lcMean;
 	  controlsEndc[iinterval].lcMeanRMS[i][j]=aMeasurement.lcMeanErr;
 	  controlsEndc[iinterval].nhitMean[i][j]=aMeasurement.nHitsMean;
	  }
	}
     
      for (int i=0;i<kEndcSCs;++i)
	{
 	  RegionStability::measurement aMeasurement=controlsEndc[iinterval].towerStability[i].getTruncatedMeanMeasurement(3,10);
 	  controlsEndc[iinterval].counterTower[i]=aMeasurement.nChannels;
 	  controlsEndc[iinterval].etSumTowerMeanVsEtRef[i]=aMeasurement.etSumMean;
 	  controlsEndc[iinterval].etSumTowerMeanVsEtRefRMS[i]=aMeasurement.etSumMeanErr;
 	  controlsEndc[iinterval].etTowerMean[i]=aMeasurement.etMeanMean;
 	  controlsEndc[iinterval].etTowerMeanRMS[i]=aMeasurement.etMeanMeanErr;
 	  controlsEndc[iinterval].lcTowerMean[i]=aMeasurement.lcMean;
 	  controlsEndc[iinterval].lcTowerMeanRMS[i]=aMeasurement.lcMeanErr;
 	  controlsEndc[iinterval].nhitTowerMean[i]=aMeasurement.nHitsMean;
	}

      for (int i=0;i<kEndcHarness;++i)
	{
 	  RegionStability::measurement aMeasurement=controlsEndc[iinterval].harnessStability[i].getTruncatedMeanMeasurement(3,10);
 	  controlsEndc[iinterval].counterHarness[i]=aMeasurement.nChannels;
 	  controlsEndc[iinterval].etSumHarnessMeanVsEtRef[i]=aMeasurement.etSumMean;
 	  controlsEndc[iinterval].etSumHarnessMeanVsEtRefRMS[i]=aMeasurement.etSumMeanErr;
 	  controlsEndc[iinterval].etHarnessMean[i]=aMeasurement.etMeanMean;
 	  controlsEndc[iinterval].etHarnessMeanRMS[i]=aMeasurement.etMeanMeanErr;
 	  controlsEndc[iinterval].lcHarnessMean[i]=aMeasurement.lcMean;
 	  controlsEndc[iinterval].lcHarnessMeanRMS[i]=aMeasurement.lcMeanErr;
 	  controlsEndc[iinterval].nhitHarnessMean[i]=aMeasurement.nHitsMean;
	}
   
      for (int i=0;i<kEndcXtals;++i)
	{
	  controlsEndc[iinterval].etSumXtalMeanVsEtRef[i]=controlsEndc[iinterval].etSumXtalMeanVsEtRef[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].etSumXtalMeanVsEtRefRMS[i]=sqrt(controlsEndc[iinterval].etSumXtalMeanVsEtRefRMS[i])/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].etXtalMean[i]=controlsEndc[iinterval].etXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].etXtalMeanRMS[i]=sqrt(controlsEndc[iinterval].etXtalMeanRMS[i])/((float)controlsEndc[iinterval].counterXtal[i]);// i want eerror on mean
	  controlsEndc[iinterval].lcXtalMean[i]=controlsEndc[iinterval].lcXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	  controlsEndc[iinterval].lcXtalMeanRMS[i]= sqrt(controlsEndc[iinterval].lcXtalMeanRMS[i])/((float)controlsEndc[iinterval].counterXtal[i]);// i want eerror on mean
	  controlsEndc[iinterval].nhitXtalMean[i]=controlsEndc[iinterval].nhitXtalMean[i]/(float)controlsEndc[iinterval].counterXtal[i];
	}
    }
  }

  //Arrays to be used in the graph
  float etSumMeanVsRefArray[kIntervals];
  float etSumMeanVsRefRMSArray[kIntervals];
  float etMeanArray[kIntervals];
  float etMeanRMSArray[kIntervals];
  float lcMeanArray[kIntervals];
  float lcMeanRMSArray[kIntervals];
  float nhitMeanArray[kIntervals];
   
  float etSumTowerMeanVsRefArray[kIntervals];
  float etSumTowerMeanVsRefRMSArray[kIntervals];
  float etTowerMeanArray[kIntervals];
  float etTowerMeanRMSArray[kIntervals];
  float lcTowerMeanArray[kIntervals];
  float lcTowerMeanRMSArray[kIntervals];
  float nhitTowerMeanArray[kIntervals];

  float etSumHarnessMeanVsRefArray[kIntervals];
  float etSumHarnessMeanVsRefRMSArray[kIntervals];
  float etHarnessMeanArray[kIntervals];
  float etHarnessMeanRMSArray[kIntervals];
  float lcHarnessMeanArray[kIntervals];
  float lcHarnessMeanRMSArray[kIntervals];
  float nhitHarnessMeanArray[kIntervals];

  float etSumXtalMeanVsRefArray[kIntervals];
  float etSumXtalMeanVsRefRMSArray[kIntervals];
  float etXtalMeanArray[kIntervals];
  float etXtalMeanRMSArray[kIntervals];
  float lcXtalMeanArray[kIntervals];
  float lcXtalMeanRMSArray[kIntervals];
  float nhitXtalMeanArray[kIntervals];


  TFile *outFileTree=TFile::Open(outFileName+"_fullTree.root","recreate");
  outFileTree->cd();
  unsigned int indexVar,indexTypeVar,timeIntervalVar,unixTimeVar;
  float  etSumMeanVsRefVar,etSumMeanVsRefRMSVar,etMeanVar,etMeanRMSVar,nhitMeanVar,lcMeanVar,lcMeanRMSVar;
   
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
  newTree->Branch("nhitMean",&nhitMeanVar,"nhitMean/F");


  TFile *outFile=TFile::Open(outFileName+"_etaRing.root","recreate");
  outFile->cd();
  for (int i=0;i<kBarlRings;++i)
    {
      for(int j=0;j<kSides;++j){
	std::cout << "Creating history for iring " <<  i+1 << "/85" << " side  "<<j+1<<"/2"<<std::endl;
	float etref=0;
	float etSumOverRef=0;
	float lcref=0;
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
	    lcref+=controls[historyNormalizationInterval+iref].lcMean[i][j];
	    nhitref+=controls[historyNormalizationInterval+iref].nhitMean[i][j];
	  }
	 
	etSumOverRef=etSumOverRef/nref;
	etref=etref/nref;
	lcref=lcref/nref;
	nhitref=nhitref/nref;


	 
	for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  //Normalizing to time reference interval
	  float nXtalRing=controls[iinterval].counterEta[i][j];
	  float kf=1.;
	  if (kfactorCorr) 
	    {
	      if(!kfactorVsTime)
		kf=kFactorsEtSum[i];
	      else
		kf=kfactorsVsTime->kFactor(0,i,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
	    }
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
	  nhitMeanArray[iinterval]=controls[iinterval].nhitMean[i][j]/nhitref;
	  lcMeanArray[iinterval]=1/(controls[iinterval].lcMean[i][j]/lcref);
	  lcMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcMean[i][j],2)*controls[iinterval].lcMeanRMS[i][j];

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
	  newTree->Fill();
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
       

	TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcMeanArray[0],nIntervalError,&lcMeanRMSArray[0]);
	lcGraph->SetName("lc_"+etaLabel+sideLabel);
	lcGraph->SetTitle("lc_"+etaLabel+sideLabel);
	lcGraph->GetYaxis()->SetTitle("<tl>");   
	lcGraph->GetXaxis()->SetTitle("interval");
       
	lcGraph->Write();
	delete lcGraph;

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
	float lcref=0;
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
	    lcref+=controlsEndc[historyNormalizationInterval+iref].lcMean[i][j];
	    nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitMean[i][j];
	  }
	 
	etSumOverRef=etSumOverRef/nref;
	etref=etref/nref;
	lcref=lcref/nref;
	nhitref=nhitref/nref;


	 
	for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  float nXtalRing=controlsEndc[iinterval].counterEta[i][j];
	  float kf=1.;
	  if (kfactorCorr)
	    {
	      if (!kfactorVsTime)
		  kf=kFactorsEndcEtSum[i];
	      else
		  kf=kfactorsVsTime->kFactor(1,i,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
	    }
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
	  nhitMeanArray[iinterval]=controlsEndc[iinterval].nhitMean[i][j]/nhitref;
	  lcMeanArray[iinterval]=1/(controlsEndc[iinterval].lcMean[i][j]/lcref);
	  lcMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcMean[i][j],2)*controlsEndc[iinterval].lcMeanRMS[i][j];
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
     

	TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcMeanArray[0],nIntervalError,&lcMeanRMSArray[0]);
	lcGraph->SetName("lc_"+etaLabel+sideLabel);
	lcGraph->SetTitle("lc_"+etaLabel+sideLabel);
	lcGraph->GetYaxis()->SetTitle("<tl>");   
	lcGraph->GetXaxis()->SetTitle("interval");
     
	lcGraph->Write();
	delete lcGraph;

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
      float lcref=0;
      float nhitref=0;
      int nref=0;

      for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
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
	  lcref+=controls[historyNormalizationInterval+iref].lcTowerMean[i];
	  nhitref+=controls[historyNormalizationInterval+iref].nhitTowerMean[i];
	}

      etSumOverRef=etSumOverRef/nref;
      etref=etref/nref;
      lcref=lcref/nref;
      nhitref=nhitref/nref;



      for(int iinterval=0;iinterval<kIntervals;iinterval++){

      float kf=1.;
      if (kfactorCorr)
	{
	  int iT=((i)%kTowerPerSM);
	  int ie=(int)iT/4;
	  int ip=(int)iT%4;
	  int isign=(i+1/kTowerPerSM<=18);
	  kf=0;
	  int nx=0;
	  if (!kfactorVsTime)
	    {
	      if (useKFactorsPerXtal)
		{
		  for (int iii=ie*5;iii<(ie+1)*5;++iii)
		    {
		      for (int iij=((i/kTowerPerSM)%18)*20+ip*5;iij<((i/kTowerPerSM)%18)*20+(ip+1)*5;++iij)
			{
			  if (kFactorsEtSumXtal[iii][iij][isign]>0)
			    {
			      kf+=kFactorsEtSumXtal[iii][iij][isign];
			      nx++;
			    }
		    }
		    }
		}
	      else
		{
		  for (int iii=ie*5;iii<(ie+1)*5;++iii)
		    {
		      if (kFactorsEtSum[iii]>0)
			{
			  kf+=kFactorsEtSum[iii];
			  nx++;
			}
		    }
		}
	      if (nx>0)
		kf=kf/nx;
	      else
		kf=kFactorsEtSum[ie*5];
	    }
	  else
	    {
	      if (useKFactorsPerXtal)
		{
		  for (int iii=ie*5;iii<(ie+1)*5;++iii)
		    {
		      for (int iij=((i/kTowerPerSM)%18)*20+ip*5;iij<((i/kTowerPerSM)%18)*20+(ip+1)*5;++iij)
			{
			  float k=kfactorsVsTime->kFactor(0,isign,iii,iij,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
			  if (k>0)
			    {
			      kf+=k;
			      nx++;
			    }
			}
		    }
		}
	      else
		{
		  for (int iii=ie*5;iii<(ie+1)*5;++iii)
		    {
		      kf+=kfactorsVsTime->kFactor(0,iii,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
		      nx++;
		    }
		}
	      if (nx>0)
		kf=kf/nx;
	      else
		kf=kfactorsVsTime->kFactor(0,ie*5,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
	    }
	}      
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
	nhitTowerMeanArray[iinterval]=controls[iinterval].nhitTowerMean[i]/nhitref;
	lcTowerMeanArray[iinterval]=1/(controls[iinterval].lcTowerMean[i]/lcref);
	lcTowerMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcTowerMean[i],2)*controls[iinterval].lcTowerMeanRMS[i];

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
	newTree->Fill();
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
       
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcTowerMeanArray[0],nIntervalError,&lcTowerMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;
       
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
      float lcref=0;
      float nhitref=0;
      int nref=0;

      for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
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
	  lcref+=controlsEndc[historyNormalizationInterval+iref].lcTowerMean[i];
	  nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitTowerMean[i];
	}

      etSumOverRef=etSumOverRef/nref;
      etref=etref/nref;
      lcref=lcref/nref;
      nhitref=nhitref/nref;



      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval
      float kf=1.;
      if (kfactorCorr)
	{
	  kf=0;
	  int nxtalsSC=0;
	  if (!kfactorVsTime)
	    {
	      for (int isign=0;isign<kSides;++isign)
		for (int ix=0;ix<kEndcWedgesX;++ix)
		  for (int iy=0;iy<kEndcWedgesY;++iy)
		    {
		      if (endcapSCs[ix][iy][isign]==i)
			{
			  kf+=kFactorsEndcEtSum[endcapRings[ix][iy][isign]];
			  nxtalsSC++;
			}
		    }
	      kf=kf/(float)nxtalsSC;
	    }
	  else
	    {
	      for (int isign=0;isign<kSides;++isign)
		for (int ix=0;ix<kEndcWedgesX;++ix)
		  for (int iy=0;iy<kEndcWedgesY;++iy)
		    {
		      if (endcapSCs[ix][iy][isign]==i)
			{
			  kf+=kfactorsVsTime->kFactor(1,endcapRings[ix][iy][isign],(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
			  nxtalsSC++;
			}
		    }
	      kf=kf/(float)nxtalsSC;

	    }
	}

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
	nhitTowerMeanArray[iinterval]=controlsEndc[iinterval].nhitTowerMean[i]/nhitref;
	lcTowerMeanArray[iinterval]=1/(controlsEndc[iinterval].lcTowerMean[i]/lcref);
	lcTowerMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcTowerMean[i],2)*controlsEndc[iinterval].lcTowerMeanRMS[i];

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
	newTree->Fill();
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
     
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcTowerMeanArray[0],nIntervalError,&lcTowerMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

     
    }

  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_harness.root","recreate");
  outFile->cd();
  for (int i=0;i<kSM*kHarnessPerSM;++i)
    {
      std::cout << "Creating history for harness " <<  i+1 << "/" << kSM*kHarnessPerSM << std::endl;
      float etref=0;
      float etSumOverRef=0;
      float lcref=0;
      float nhitref=0;
      int nref=0;

      for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
	{
	  nref++;
	  if (normalizationType == "ring")
	    etSumOverRef+=controls[historyNormalizationInterval+iref].etSumHarnessMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	  else if (normalizationType == "allEB")
	    {
	      float etRef=0;
	      for (int ii=0;ii<kBarlRings;++ii)
		etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
	      etRef=etRef/kBarlRings;
	      etSumOverRef+=controls[historyNormalizationInterval+iref].etSumHarnessMeanVsEtRef[i]/etRef;
	    }
	  etref+=controls[historyNormalizationInterval+iref].etHarnessMean[i];
	  lcref+=controls[historyNormalizationInterval+iref].lcHarnessMean[i];
	  nhitref+=controls[historyNormalizationInterval+iref].nhitHarnessMean[i];
	}

      etSumOverRef=etSumOverRef/nref;
      etref=etref/nref;
      lcref=lcref/nref;
      nhitref=nhitref/nref;

      for(int iinterval=0;iinterval<kIntervals;iinterval++){

      float kf=1.;
      if (kfactorCorr)
	{
	  kf=0;
	  int nx=0;
	  std::vector<HarnessMap::channel> myChannels=harnessMap->channelsInHarness(0,i);
	  for (unsigned int ic=0;ic<myChannels.size();++ic)
	    {
	      HarnessMap::channel channel=myChannels[ic];
	      if (!kfactorVsTime)
		{
		  if (useKFactorsPerXtal)
		    {
		      if (kFactorsEtSumXtal[TMath::Abs(channel.ieta)-1][channel.iphi-1][(channel.sign>0)]>0)
			{
			  kf+=kFactorsEtSumXtal[TMath::Abs(channel.ieta)-1][channel.iphi-1][(channel.sign>0)];
			  nx++;
			}
		    }
		  else
		    {
		      if (kFactorsEtSum[TMath::Abs(channel.ieta)-1]>0)
			{
			  kf+=kFactorsEtSum[TMath::Abs(channel.ieta)-1];
			  nx++;
			}
		    }
		}
	      else
		{
		  if (useKFactorsPerXtal)
		    kf+=kfactorsVsTime->kFactor(0,(channel.sign>0),TMath::Abs(channel.ieta)-1,channel.iphi-1,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
		  else
		    kf+=kfactorsVsTime->kFactor(0,TMath::Abs(channel.ieta)-1,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
		  if (kf>0)
		    nx++;
		}
	    }
	  if (nx>0)
	    kf=kf/nx;
	  else
	    kf=1.;
	}
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
	etSumHarnessMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumHarnessMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	etSumHarnessMeanVsRefRMSArray[iinterval]= etSumHarnessMeanVsRefArray[iinterval] * ( controls[iinterval].etSumHarnessMeanVsEtRefRMS[i] / controls[iinterval].etSumHarnessMeanVsEtRef[i] )  ;
	etHarnessMeanArray[iinterval]=(controls[iinterval].etHarnessMean[i]/etref);
	etHarnessMeanRMSArray[iinterval]=(controls[iinterval].etHarnessMeanRMS[i]/etref);
	nhitHarnessMeanArray[iinterval]=controls[iinterval].nhitHarnessMean[i]/nhitref;
	lcHarnessMeanArray[iinterval]=1/(controls[iinterval].lcHarnessMean[i]/lcref);
	lcHarnessMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcHarnessMean[i],2)*controls[iinterval].lcHarnessMeanRMS[i];

	indexTypeVar=6;
	indexVar=i+1;
	timeIntervalVar=iinterval;
	unixTimeVar=nInterval[iinterval];
	nhitMeanVar=nhitHarnessMeanArray[iinterval];
	etSumMeanVsRefVar=etSumHarnessMeanVsRefArray[iinterval];
	etSumMeanVsRefRMSVar=etSumHarnessMeanVsRefRMSArray[iinterval];
	etMeanVar=etHarnessMeanArray[iinterval];
	etMeanRMSVar=etHarnessMeanRMSArray[iinterval];
	lcMeanVar=lcHarnessMeanArray[iinterval];
	lcMeanRMSVar=lcHarnessMeanRMSArray[iinterval];
	newTree->Fill();
      }
      TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitHarnessMeanArray[0]);
      TString etaLabel="harness_";
      etaLabel+=(i+1);
       
      nhitGraph->SetName("nHit_"+etaLabel);
      nhitGraph->SetTitle("nHit_"+etaLabel);
      nhitGraph->GetYaxis()->SetTitle("nhit");
      nhitGraph->GetXaxis()->SetTitle("interval");
      nhitGraph->Write();
      delete nhitGraph;

      TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumHarnessMeanVsRefArray[0],nIntervalError,&etSumHarnessMeanVsRefRMSArray[0]);
      etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
      etSumOverRefGraph->GetXaxis()->SetTitle("interval");
       
      etSumOverRefGraph->Write();
      delete etSumOverRefGraph;

      TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etHarnessMeanArray[0],nIntervalError,&etHarnessMeanRMSArray[0]);
      etGraph->SetName("et_"+etaLabel);
      etGraph->SetTitle("et_"+etaLabel);
      etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
      etGraph->GetXaxis()->SetTitle("interval");
       
      etGraph->Write();
      delete etGraph;
       
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcHarnessMeanArray[0],nIntervalError,&lcHarnessMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;
       
    }

  outFile->Write();
  outFile->Close();


  outFile=TFile::Open(outFileName+"_endcHarness.root","recreate");
  outFile->cd();
  for (int i=0;i<kEndcHarness;++i)
    {
      std::cout << "Creating history for ee harness " <<  i+1 << "/" << kEndcHarness << std::endl;
      float etref=0;
      float etSumOverRef=0;
      //        float etNoCorrref=0;
      float lcref=0;
      float nhitref=0;
      int nref=0;

      for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
	{
	  nref++;
	  if (normalizationType == "ring")
	    etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumHarnessMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][0]+controls[historyNormalizationInterval+iref].etSumMean[ringRefRegion][1])/2.);
	  else if (normalizationType == "allEB")
	    {
	      float etRef=0;
	      for (int ii=0;ii<kBarlRings;++ii)
		etRef+=((controls[historyNormalizationInterval+iref].etSumMean[ii][0]+controls[historyNormalizationInterval+iref].etSumMean[ii][1])/2.);
	      etRef=etRef/kBarlRings;
	      etSumOverRef+=controlsEndc[historyNormalizationInterval+iref].etSumHarnessMeanVsEtRef[i]/etRef;
	    }
	  etref+=controlsEndc[historyNormalizationInterval+iref].etHarnessMean[i];
	  // 	   etNoCorrref+=controlsEndc[historyNormalizationInterval+iref].etHarnessMeanNoCorr[i];
	  lcref+=controlsEndc[historyNormalizationInterval+iref].lcHarnessMean[i];
	  nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitHarnessMean[i];
	}

      etSumOverRef=etSumOverRef/nref;
      etref=etref/nref;
      lcref=lcref/nref;
      nhitref=nhitref/nref;



      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval
      float kf=1.;
      if (kfactorCorr)
	{
	  kf=0;
	  int nx=0;
	  std::vector<HarnessMap::channel> myChannels=harnessMap->channelsInHarness(1,i);
	  for (unsigned int ic=0;ic<myChannels.size();++ic)
	    {
	      HarnessMap::channel channel=myChannels[ic];
	      if (!kfactorVsTime)
		{
		  kf+=kFactorsEndcEtSum[endcapRings[channel.ieta-1][channel.iphi-1][channel.sign>0]];
		  nx++;
		}
	      else
		{
		  kf+=kfactorsVsTime->kFactor(1,endcapRings[channel.ieta-1][channel.iphi-1][channel.sign>0],(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
		  nx++;
		}
	    }
	  if (nx>0)
	    kf=kf/nx;
	  else
	    kf=1.;
	}

      float etSumRef=0.;
      if (normalizationType == "ring")
	etSumRef=(controls[iinterval].etSumMean[ringRefRegion][0]+controls[iinterval].etSumMean[ringRefRegion][1])/2.;
	else if (normalizationType == "allEB")
	  {
	    for (int ii=0;ii<kBarlRings;++ii)
	      etSumRef+=((controls[iinterval].etSumMean[ii][0]+controls[iinterval].etSumMean[ii][1])/2.);
	    etSumRef=etSumRef/kBarlRings;
	  }
	etSumHarnessMeanVsRefArray[iinterval]=1 + (((controlsEndc[iinterval].etSumHarnessMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	etSumHarnessMeanVsRefRMSArray[iinterval]= etSumHarnessMeanVsRefArray[iinterval] * ( controlsEndc[iinterval].etSumHarnessMeanVsEtRefRMS[i] / controlsEndc[iinterval].etSumHarnessMeanVsEtRef[i] )  ;
	etHarnessMeanArray[iinterval]=(controlsEndc[iinterval].etHarnessMean[i]/etref);
	etHarnessMeanRMSArray[iinterval]=(controlsEndc[iinterval].etHarnessMeanRMS[i]/etref);
	
	nhitHarnessMeanArray[iinterval]=controlsEndc[iinterval].nhitHarnessMean[i]/nhitref;
	lcHarnessMeanArray[iinterval]=1/(controlsEndc[iinterval].lcHarnessMean[i]/lcref);
	lcHarnessMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcHarnessMean[i],2)*controlsEndc[iinterval].lcHarnessMeanRMS[i];
	
	indexTypeVar=7;
	indexVar=i+1;
	timeIntervalVar=iinterval;
	unixTimeVar=nInterval[iinterval];
	nhitMeanVar=nhitHarnessMeanArray[iinterval];
	etSumMeanVsRefVar=etSumHarnessMeanVsRefArray[iinterval];
	etSumMeanVsRefRMSVar=etSumHarnessMeanVsRefRMSArray[iinterval];
	etMeanVar=etHarnessMeanArray[iinterval];
	etMeanRMSVar=etHarnessMeanRMSArray[iinterval];
	lcMeanVar=lcHarnessMeanArray[iinterval];
	lcMeanRMSVar=lcHarnessMeanRMSArray[iinterval];
	
	newTree->Fill();
	
      }
      TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitHarnessMeanArray[0]);
      TString etaLabel="endcHarness_";
      etaLabel+=(i+1);
      
      nhitGraph->SetName("nHit_"+etaLabel);
      nhitGraph->SetTitle("nHit_"+etaLabel);
      nhitGraph->GetYaxis()->SetTitle("nhit");
      nhitGraph->GetXaxis()->SetTitle("interval");
      nhitGraph->Write();
      delete nhitGraph;
      
      TGraphErrors* etSumOverRefGraph=new TGraphErrors(kIntervals,nInterval,&etSumHarnessMeanVsRefArray[0],nIntervalError,&etSumHarnessMeanVsRefRMSArray[0]);
      etSumOverRefGraph->SetName("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->SetTitle("etSumOverRef_"+etaLabel);
      etSumOverRefGraph->GetYaxis()->SetTitle("et/etref");   
      etSumOverRefGraph->GetXaxis()->SetTitle("interval");
      
      etSumOverRefGraph->Write();
      delete etSumOverRefGraph;
      
      TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etHarnessMeanArray[0],nIntervalError,&etHarnessMeanRMSArray[0]);
      etGraph->SetName("et_"+etaLabel);
      etGraph->SetTitle("et_"+etaLabel);
      etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
      etGraph->GetXaxis()->SetTitle("interval");
      
      etGraph->Write();
      delete etGraph;
     
      	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcHarnessMeanArray[0],nIntervalError,&lcHarnessMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

      
    }

  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_ixtal.root","recreate");
  outFile->cd();
  for (int i=0;i<kSM*kXtalPerSM;++i)
    {
      std::cout << "Creating history for ixtal " <<  i+1 << "/" << kSM*kXtalPerSM << std::endl;
      float etref=0;
      float lcref=0;
      float nhitref=0;
      int nref=0;
      float etSumOverRef=0;

      for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
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
	  nhitref+=controls[historyNormalizationInterval+iref].nhitXtalMean[i];
	}

      etref=etref/nref;
      etSumOverRef=etSumOverRef/nref;
      lcref=lcref/nref;
      nhitref=nhitref/nref;


      
      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval
      float kf=1.;
      if (kfactorCorr)
	{
	  int ie=(i)/360+1;
	  int ip=(i)%360+1;
	  if (ie>85)
	    ie=ie-85;
	  else
	    ie=-ie;
	  int sig=(ie>0) ? 1:0; 
	  ie=TMath::Abs(ie);
	  if (!kfactorVsTime)
	    {
	      if (useKFactorsPerXtal)
		kf=kFactorsEtSumXtal[ie-1][ip-1][sig];
	      else
		kf=kFactorsEtSum[ie-1];
	    }
	  else
	    {
	      kf+=kfactorsVsTime->kFactor(0,ie-1,(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
	    }
	}
	
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
	nhitXtalMeanArray[iinterval]=controls[iinterval].nhitXtalMean[i]/nhitref;
	lcXtalMeanArray[iinterval]=1/(controls[iinterval].lcXtalMean[i]/lcref);
	lcXtalMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcXtalMean[i],2)*controls[iinterval].lcXtalMeanRMS[i];

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
	
	newTree->Fill();
	
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
       	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcXtalMeanArray[0],nIntervalError,&lcXtalMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;
       
    }
  outFile->Write();
  outFile->Close();

  outFile=TFile::Open(outFileName+"_ixtalEndc.root","recreate");
  outFile->cd();
  for (int i=0;i<kEndcXtals;++i)
    {
      std::cout << "Creating history for ixtal " <<  i+1 << "/" << kEndcXtals << std::endl;
      float etref=0;
      float lcref=0;
      float nhitref=0;
      int nref=0;
      float etSumOverRef=0;

      for (int iref=-historyNormalizationIntervalRange;iref<historyNormalizationIntervalRange+1;++iref)
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
	  lcref+=controlsEndc[historyNormalizationInterval+iref].lcXtalMean[i];
	  nhitref+=controlsEndc[historyNormalizationInterval+iref].nhitXtalMean[i];
	}

      etref=etref/nref;
      etSumOverRef=etSumOverRef/nref;
      lcref=lcref/nref;
      nhitref=nhitref/nref;


     
      for(int iinterval=0;iinterval<kIntervals;iinterval++){
	//Normalizing to time reference interval
      float kf=1.;
      if (kfactorCorr)
	{
	  int nxtals=0;
	  for (int isign=0;isign<kSides;++isign)
	    for (int ix=0;ix<kEndcWedgesX;++ix)
	      for (int iy=0;iy<kEndcWedgesY;++iy)
		{
		  if (endcapXtals[ix][iy][isign]==i)
		    {
		      if (!kfactorVsTime)
			kf=kFactorsEndcEtSum[endcapRings[ix][iy][isign]];
		      else
			kf+=kfactorsVsTime->kFactor(1,endcapRings[ix][iy][isign],(intervals->lsStart(iinterval).run + intervals->lsEnd(iinterval).run)/2);
		      nxtals++;
		    }
		}
	}

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
	nhitXtalMeanArray[iinterval]=controlsEndc[iinterval].nhitXtalMean[i]/nhitref;
	lcXtalMeanArray[iinterval]=1/(controlsEndc[iinterval].lcXtalMean[i]/lcref);
	lcXtalMeanRMSArray[iinterval]=pow(lcref/controlsEndc[iinterval].lcXtalMean[i],2)*controlsEndc[iinterval].lcXtalMeanRMS[i];

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
	newTree->Fill();

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
     
	 
	 
      TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcXtalMeanArray[0],nIntervalError,&lcXtalMeanRMSArray[0]);
      lcGraph->SetName("lc_"+etaLabel);
      lcGraph->SetTitle("lc_"+etaLabel);
      lcGraph->GetYaxis()->SetTitle("<tl>");   
      lcGraph->GetXaxis()->SetTitle("interval");
	 
      lcGraph->Write();
      delete lcGraph;

    }

  outFile->Write();
  outFile->Close();

  outFileTree->Write();
  outFileTree->Close();

}

#if defined(__ROOTCLING__)
#pragma link C++ class  makeControlPlots;
#endif
