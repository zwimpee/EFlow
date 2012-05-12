//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb  3 16:06:38 2012 by ROOT version 5.27/06b
// from TTree finalTree_barl/tree with et and lc
// found on file: finalOutput_2011A.root
//////////////////////////////////////////////////////////

#ifndef doBeamSpotVariations_h
#define doBeamSpotVariations_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
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

#include "lumiIntervals.h"

#define MAXHITS 1000
#define kBarlRings 85
#define kEndcRings 39

#define kBarlWedges 360
#define kEndcWedgesX 100
#define kEndcWedgesY 100

#define kSides 2

#define kTowerPerSM 68
#define kXtalPerSM 1700
#define kSM 36

#define kEndcSCs 624
#define kEndcXtals 14648



class doBeamSpotVariations {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

  bool applyBSCorrection;
  bool applyLumiCorrection;
  TString bsCorrectionFile;
  TString eeIndicesFile;

  struct bsInfo{
    float bsPos;
    float bsWid;
    unsigned int nEvents;
    double nHitsMeanEB;
    double nHitsMeanEE;

    void reset()
    {
      bsPos=0.;
      bsWid=0.;
      nEvents=0;
      nHitsMeanEB=0;
      nHitsMeanEE=0;
    }
  };
   

   lumiIntervals* intervals;
   TString bsInfoFile;
   TString outFileName;
   TString outFileMode;

   // Declaration of leaf types
   Int_t           time_interval;
   UInt_t          nHits;
   UInt_t          det;
   Int_t           ieta;
   Int_t           iphi;
   Int_t           sign;
   Float_t         et;
   Float_t         RMSet;
   Float_t         etNoCorr;
   Float_t         RMSetNoCorr;
   Float_t         etA;
   Float_t         RMSetA;
   Float_t         etNoCorrA;
   Float_t         RMSetNoCorrA;
   Float_t         etB;
   Float_t         RMSetB;
   Float_t         etNoCorrB;
   Float_t         RMSetNoCorrB;
   Float_t         lc;
   Float_t         RMSlc;

   // List of branches
   TBranch        *b_timeInterval;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_det;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_et;   //!
   TBranch        *b_RMSet;   //!
   TBranch        *b_etNoCorr;   //!
   TBranch        *b_RMSetNoCorr;   //!
   TBranch        *b_etA;   //!
   TBranch        *b_RMSetA;   //!
   TBranch        *b_etNoCorrA;   //!
   TBranch        *b_RMSetNoCorrA;   //!
   TBranch        *b_etB;   //!
   TBranch        *b_RMSetB;   //!
   TBranch        *b_etNoCorrB;   //!
   TBranch        *b_RMSetNoCorrB;   //!
   TBranch        *b_lc;   //!
   TBranch        *b_RMSlc;   //!

   // is the xtal Chinese?
   bool isChinese(int eta, int phi)
   {
     if( (eta>-76 ) && (eta<76) ) 
       return false;
     
     if(eta>75){
       if(phi>300 && phi<=340) return true;
       if(phi>120 && phi<=140) return true;
       if(phi>231 && phi<=240) return true;
       if(eta>=81){
	 if(phi>220 && phi<=240) return true;
	 if(phi>180 && phi<=200) return true;
       }
     }
     
     if(eta<-76)
       {
	 if(phi>200 && phi<=220) return true;
	 if(phi>240 && phi<=300) return true;
	 if(eta<= -81){
	   if(phi>20 && phi<=40) return true;
	   if(phi>60 && phi<=100) return true;
	 }
       }
     return false;
   }
   
   // return alpha as from the simple russian/chinese DB
   float alphaDB(int iSign, int eta, int phi)
   {
     int xxsign = (iSign == 1) ? 1 : -1;
     if (isChinese(xxsign*eta,phi))
       return 1.0;
     else
       return 1.52;
   }
   
   // return an index between 1 and 36
   int iSM(int eta, int phi, int isign)
   {
     int id = ( phi - 1 ) / 20 + 1;
     if ( isign == 0 ) id += 18;
     return id;
   }

   // return an index between 1 and 68
   int iTower(int eta, int phi, int isign)
   {
     int ttEta=(eta-1)/5+1;
     int ttPhi=((phi-1)%20)/5+1;
     return (ttEta-1)*4+ttPhi;
   }

   // return an index between 1 and 2448
   int iTT(int eta, int phi, int isign)
   {
     return (iSM(eta,phi,isign)-1)*68+iTower(eta,phi,isign);
   }

   // return an index between 1 and 61200
   int iXtal(int eta, int phi, int isign)
   {
     int etaIndex=eta-1;
     if (isign==1)
       etaIndex+=85;
     return (etaIndex*kBarlWedges+phi);
   }


   struct variablesToControl
   {
     float etSumMean[kBarlRings][kSides];
     float etSumMeanRMS[kBarlRings][kSides];
     float etSumMeanVsEtRef[kBarlRings][kSides];
     float etSumMeanVsEtRefRMS[kBarlRings][kSides];
     float etMean[kBarlRings][kSides];
     float etMeanRMS[kBarlRings][kSides];
/*      float etABRatio[kBarlRings][kSides]; */
/*      float etABRatioRMS[kBarlRings][kSides]; */
/*      float etMeanNoCorr[kBarlRings][kSides];      */
/*      float etMeanNoCorrRMS[kBarlRings][kSides];      */
     float lcMean[kBarlRings][kSides];
     float lcMeanRMS[kBarlRings][kSides];
     float tlMean[kBarlRings][kSides];
     float tlMeanRMS[kBarlRings][kSides];
     float nhitMean[kBarlRings][kSides];
     int   counterEta[kBarlRings][kSides];  


     float   counter;
     unsigned long long int totalHits;
     double nHitsMean;

     void reset(){
       for (int i=0;i<kBarlRings;++i)
	 {
	   for(int j=0;j<kSides;j++){
	     etSumMean[i][j]=0;	
	     etSumMeanRMS[i][j]=0;	
	     etSumMeanVsEtRef[i][j]=0;	
	     etSumMeanVsEtRefRMS[i][j]=0;	
	     etMean[i][j]=0;	
	     etMeanRMS[i][j]=0;	
/* 	     etMeanNoCorr[i][j]=0; */
/* 	     etMeanNoCorrRMS[i][j]=0;      */
/* 	     etABRatio[i][j]=0;	 */
/* 	     etABRatioRMS[i][j]=0;	 */
	     lcMean[i][j]=0;	
	     lcMeanRMS[i][j]=0;	
	     nhitMean[i][j]=0;	
	     counterEta[i][j]=0;    

	   }
	 }

       counter=0;	
       totalHits=0;
       nHitsMean=0;
     }

   };


   struct variablesToControl_ee
   {
     float etSumMean[kEndcRings][kSides];
     float etSumMeanRMS[kEndcRings][kSides];
     float etSumMeanVsEtRef[kEndcRings][kSides];
     float etSumMeanVsEtRefRMS[kEndcRings][kSides];
     float etMean[kEndcRings][kSides];
     float etMeanRMS[kEndcRings][kSides];
/*      float etABRatio[kEndcRings][kSides]; */
/*      float etABRatioRMS[kEndcRings][kSides]; */
/*      float etMeanNoCorr[kEndcRings][kSides];      */
/*      float etMeanNoCorrRMS[kEndcRings][kSides];      */
     float lcMean[kEndcRings][kSides];
     float lcMeanRMS[kEndcRings][kSides];
     float tlMean[kEndcRings][kSides];
     float tlMeanRMS[kEndcRings][kSides];
     float nhitMean[kEndcRings][kSides];
     int   counterEta[kEndcRings][kSides];  


     float   counter;
     unsigned long long int totalHits;
     double nHitsMean;

     void reset(){
       for (int i=0;i<kEndcRings;++i)
	 {
	   for(int j=0;j<kSides;j++){
	     etSumMean[i][j]=0;	
	     etSumMeanRMS[i][j]=0;	
	     etSumMeanVsEtRef[i][j]=0;	
	     etSumMeanVsEtRefRMS[i][j]=0;	
	     etMean[i][j]=0;	
	     etMeanRMS[i][j]=0;	
/* 	     etMeanNoCorr[i][j]=0; */
/* 	     etMeanNoCorrRMS[i][j]=0;      */
/* 	     etABRatio[i][j]=0;	 */
/* 	     etABRatioRMS[i][j]=0;	 */
	     lcMean[i][j]=0;	
	     lcMeanRMS[i][j]=0;	
	     nhitMean[i][j]=0;	
	     counterEta[i][j]=0;     

	   }
	 }

       counter=0;	
       totalHits=0;
       nHitsMean=0;
     }

   };

   doBeamSpotVariations(TTree *tree=0);
   virtual ~doBeamSpotVariations();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setLumiIntervals(const char* file);
   void setOutfile(const char* outFile);

};

#endif

#ifdef doBeamSpotVariations_cxx
doBeamSpotVariations::doBeamSpotVariations(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("finalOutput_2011A.root");
      if (!f) {
         f = new TFile("finalOutput_2011A.root");
      }
      tree = (TTree*)gDirectory->Get("finalTree_barl");

   }
   Init(tree);
}

doBeamSpotVariations::~doBeamSpotVariations()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void doBeamSpotVariations::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void doBeamSpotVariations::setOutfile(const char* file)
{
   outFileName=TString(file);
}


Int_t doBeamSpotVariations::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t doBeamSpotVariations::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void doBeamSpotVariations::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("time_interval", &time_interval, &b_timeInterval);
   fChain->SetBranchAddress("nHits", &nHits, &b_nHits);
   fChain->SetBranchAddress("det", &det, &b_det);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("sign", &sign, &b_sign);
   fChain->SetBranchAddress("et", &et, &b_et);
   fChain->SetBranchAddress("RMSet", &RMSet, &b_RMSet);
   fChain->SetBranchAddress("etNoCorr", &etNoCorr, &b_etNoCorr);
   fChain->SetBranchAddress("RMSetNoCorr", &RMSetNoCorr, &b_RMSetNoCorr);
   fChain->SetBranchAddress("etA", &etA, &b_etA);
   fChain->SetBranchAddress("RMSetA", &RMSetA, &b_RMSetA);
   fChain->SetBranchAddress("etNoCorrA", &etNoCorrA, &b_etNoCorrA);
   fChain->SetBranchAddress("RMSetNoCorrA", &RMSetNoCorrA, &b_RMSetNoCorrA);
   fChain->SetBranchAddress("etB", &etB, &b_etB);
   fChain->SetBranchAddress("RMSetB", &RMSetB, &b_RMSetB);
   fChain->SetBranchAddress("etNoCorrB", &etNoCorrB, &b_etNoCorrB);
   fChain->SetBranchAddress("RMSetNoCorrB", &RMSetNoCorrB, &b_RMSetNoCorrB);
   fChain->SetBranchAddress("lc", &lc, &b_lc);
   fChain->SetBranchAddress("RMSlc", &RMSlc, &b_RMSlc);
   Notify();
}

Bool_t doBeamSpotVariations::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void doBeamSpotVariations::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t doBeamSpotVariations::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef doBeamSpotVariations_cxx
