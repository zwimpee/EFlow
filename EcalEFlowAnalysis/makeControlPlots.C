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
   int kIntervals=intervals->numberOfIntervals();
   controls.reserve(kIntervals);

   cout<<"creating variables"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
   controls[iinterval].reset();
   }

   TFile *kFactors;
   float  kFactorsEtSum[85];
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
     }

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      controls[time_interval-1].counter++;
      int tt=iTT(ieta,iphi,sign);
      int xtal=iXtal(ieta,iphi,sign);
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
      controls[time_interval-1].tlMean[ieta-1][sign]+=TMath::Exp(-1*TMath::Log(lc)/alphaDB(sign,ieta,iphi));
      controls[time_interval-1].tlMeanRMS[ieta-1][sign]+=pow(RMSlc*alphaDB(sign,ieta,iphi),2)/(float)nHits; //approximation
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
      controls[time_interval-1].tlTowerMean[tt-1]+=TMath::Exp(-1*TMath::Log(lc)/alphaDB(sign,ieta,iphi));
      controls[time_interval-1].tlTowerMeanRMS[tt-1]+=pow(RMSlc*alphaDB(sign,ieta,iphi),2)/(float)nHits;
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
      controls[time_interval-1].tlXtalMean[xtal-1]+=TMath::Exp(-1*TMath::Log(lc)/alphaDB(sign,ieta,iphi));
      controls[time_interval-1].tlXtalMeanRMS[xtal-1]+=pow(RMSlc*alphaDB(sign,ieta,iphi),2)/(float)nHits;
      controls[time_interval-1].counterXtal[xtal-1]++;
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
	     std::cout << "i " << i << " j " << j << " etMean " << controls[iinterval].etMean[i][j] << " etMeanErr " << controls[iinterval].etMeanRMS[i][j] << std::endl;
	     
// 	     controls[iinterval].etMeanNoCorr[i][j]= controls[iinterval].etMeanNoCorr[i][j]/(float)controls[iinterval].counterEta[i][j];
// 	     controls[iinterval].etMeanNoCorrRMS[i][j]= sqrt(controls[iinterval].etMeanNoCorrRMS[i][j])/((float)controls[iinterval].nhitMean[i][j]);// i want eerror on mean
// 	     controls[iinterval].etABRatio[i][j]=controls[iinterval].etABRatio[i][j]/(float)controls[iinterval].counterEta[i][j];
// 	     controls[iinterval].etABRatioRMS[i][j]=sqrt(controls[iinterval].etABRatioRMS[i][j])/sqrt(controls[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls[iinterval].lcMeanRMS[i][j]= sqrt(controls[iinterval].lcMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls[iinterval].lcMean[i][j]=controls[iinterval].lcMean[i][j]/(float)controls[iinterval].counterEta[i][j];
	     controls[iinterval].tlMeanRMS[i][j]= sqrt(controls[iinterval].tlMeanRMS[i][j])/((float)controls[iinterval].counterEta[i][j]);// i want eerror on mean
	     controls[iinterval].tlMean[i][j]=controls[iinterval].tlMean[i][j]/(float)controls[iinterval].counterEta[i][j];
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
	   controls[iinterval].tlTowerMean[i]=controls[iinterval].tlTowerMean[i]/(float)controls[iinterval].counterTower[i];
	   controls[iinterval].tlTowerMeanRMS[i]= sqrt(controls[iinterval].tlTowerMeanRMS[i])/((float)controls[iinterval].counterTower[i]);// i want eerror on mean
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
	   controls[iinterval].tlXtalMean[i]=controls[iinterval].tlXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	   controls[iinterval].tlXtalMeanRMS[i]= sqrt(controls[iinterval].tlXtalMeanRMS[i])/((float)controls[iinterval].counterXtal[i]);// i want eerror on mean
	   controls[iinterval].nhitXtalMean[i]=controls[iinterval].nhitXtalMean[i]/(float)controls[iinterval].counterXtal[i];
	 }
       nInterval[iinterval]=intervals->intervalTime(iinterval);
       nIntervalError[iinterval]=0.;
       //       nIntervalError[iinterval]=0;
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
   float tlMeanArray[kIntervals];
   float tlMeanRMSArray[kIntervals];
   float nhitMeanArray[kIntervals];
   
   float etSumTowerMeanVsRefArray[kIntervals];
   float etSumTowerMeanVsRefRMSArray[kIntervals];
   float etTowerMeanArray[kIntervals];
   float etTowerMeanRMSArray[kIntervals];
//    float etTowerMeanNoCorrArray[kIntervals];
//    float etTowerMeanNoCorrRMSArray[kIntervals];
   float lcTowerMeanArray[kIntervals];
   float lcTowerMeanRMSArray[kIntervals];
   float tlTowerMeanArray[kIntervals];
   float tlTowerMeanRMSArray[kIntervals];
   float nhitTowerMeanArray[kIntervals];

   float etSumXtalMeanVsRefArray[kIntervals];
   float etSumXtalMeanVsRefRMSArray[kIntervals];
   float etXtalMeanArray[kIntervals];
   float etXtalMeanRMSArray[kIntervals];
//    float etXtalMeanNoCorrArray[kIntervals];
//    float etXtalMeanNoCorrRMSArray[kIntervals];
   float lcXtalMeanArray[kIntervals];
   float lcXtalMeanRMSArray[kIntervals];
   float tlXtalMeanArray[kIntervals];
   float tlXtalMeanRMSArray[kIntervals];
   float nhitXtalMeanArray[kIntervals];


   int historyNormalizationInterval=20;
   //   kIntervals=487;

   TFile *outFile=TFile::Open(outFileName+"_etaRing.root","recreate");

   for (int i=0;i<kBarlRings;++i)
     {
       for(int j=0;j<kSides;++j){
	 std::cout << "Creating history for iring " <<  i+1 << "/85" << " side  "<<j+1<<"/2"<<std::endl;
	 float etref=0;
	 float etSumOverRef=0;
// 	 float etABRatioref=0;
// 	 float etNoCorrref=0;
	 float lcref=0;
	 float tlref=0;
	 float nhitref=0;
	 int nref=0;
	 
	 for (int iref=-5;iref<6;++iref)
	   {
	     nref++;
	     etSumOverRef+=controls[historyNormalizationInterval+iref].etSumMean[i][j]/((controls[historyNormalizationInterval+iref].etSumMean[2][0]+controls[historyNormalizationInterval+iref].etSumMean[2][1])/2.);
	     etref+=controls[historyNormalizationInterval+iref].etMean[i][j];
// 	     etNoCorrref+=controls[historyNormalizationInterval+iref].etMeanNoCorr[i][j];
// 	     etABRatioref+=controls[historyNormalizationInterval+iref].etABRatio[i][j];
	     lcref+=controls[historyNormalizationInterval+iref].lcMean[i][j];
	     tlref+=controls[historyNormalizationInterval+iref].tlMean[i][j];
	     nhitref+=controls[historyNormalizationInterval+iref].nhitMean[i][j];
	   }
	 
	 etSumOverRef=etSumOverRef/nref;
	 etref=etref/nref;
// 	 etABRatioref=etABRatioref/nref;
// 	 etNoCorrref=etNoCorrref/nref;
	 lcref=lcref/nref;
	 tlref=tlref/nref;
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
	   float etSumRef=(controls[iinterval].etSumMean[2][0]+controls[iinterval].etSumMean[2][1])/2.;
	   etSumMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumMean[i][j]/etSumRef)/etSumOverRef)-1.)/kf;
	   //Approximation now just multipliy the errors for 2
 	   etSumMeanVsRefRMSArray[iinterval]=(((controls[iinterval].etSumMeanRMS[i][j]/etSumRef)/etSumOverRef)/kf)*2.;
	   etMeanArray[iinterval]=(controls[iinterval].etMean[i][j]/etref);
	   etMeanRMSArray[iinterval]=(controls[iinterval].etMeanRMS[i][j]/etref);
// 	   etMeanNoCorrArray[iinterval]=(controls[iinterval].etMeanNoCorr[i][j]/etNoCorrref);
// 	   etMeanNoCorrRMSArray[iinterval]=(controls[iinterval].etMeanNoCorrRMS[i][j]/(etNoCorrref));
// 	   etABRatioArray[iinterval]=(controls[iinterval].etABRatio[i][j]/etABRatioref)AB;
// 	   etABRatioRMSArray[iinterval]=(controls[iinterval].etABRatioRMS[i][j]/etABRatioref)AB;
	   nhitMeanArray[iinterval]=controls[iinterval].nhitMean[i][j]/nhitref;
	   lcMeanArray[iinterval]=1/(controls[iinterval].lcMean[i][j]/lcref);
	   lcMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcMean[i][j],2)*controls[iinterval].lcMeanRMS[i][j];
	   tlMeanArray[iinterval]=controls[iinterval].tlMean[i][j]/tlref;
	   tlMeanRMSArray[iinterval]=controls[iinterval].tlMeanRMS[i][j]/(tlref);
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
   for (int i=0;i<kSM*kTowerPerSM;++i)
     {
       std::cout << "Creating history for itt " <<  i+1 << "/" << kSM*kTowerPerSM << std::endl;
       float etref=0;
       float etSumOverRef=0;
//        float etNoCorrref=0;
       float lcref=0;
       float tlref=0;
       float nhitref=0;
       int nref=0;

       for (int iref=-5;iref<6;++iref)
	 {
	   nref++;
	   etSumOverRef+=controls[historyNormalizationInterval+iref].etSumTowerMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[2][0]+controls[historyNormalizationInterval+iref].etSumMean[2][1])/2.);
	   etref+=controls[historyNormalizationInterval+iref].etTowerMean[i];
// 	   etNoCorrref+=controls[historyNormalizationInterval+iref].etTowerMeanNoCorr[i];
	   lcref+=controls[historyNormalizationInterval+iref].lcTowerMean[i];
	   tlref+=controls[historyNormalizationInterval+iref].tlTowerMean[i];
	   nhitref+=controls[historyNormalizationInterval+iref].nhitTowerMean[i];
	 }

       etSumOverRef=etSumOverRef/nref;
       etref=etref/nref;
//        etNoCorrref=etNoCorrref/nref;
       lcref=lcref/nref;
       tlref=tlref/nref;
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
	   std::cout << "kFactor for TT " << i << " iT " << iT << " ie " << ie << " is " << kf << std::endl;
	 }

       for(int iinterval=0;iinterval<kIntervals;iinterval++){
	 //Normalizing to time reference interval

	 float etSumRef=(controls[iinterval].etSumMean[2][0]+controls[iinterval].etSumMean[2][1])/2.;
	 etSumTowerMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumTowerMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	 etSumTowerMeanVsRefRMSArray[iinterval]=((controls[iinterval].etSumTowerMeanVsEtRefRMS[i]/etSumRef)/etSumOverRef)/kf;
	 etTowerMeanArray[iinterval]=(controls[iinterval].etTowerMean[i]/etref);
	 etTowerMeanRMSArray[iinterval]=(controls[iinterval].etTowerMeanRMS[i]/etref);
// 	 etTowerMeanNoCorrArray[iinterval]=(controls[iinterval].etTowerMeanNoCorr[i]/etNoCorrref);
// 	 etTowerMeanNoCorrRMSArray[iinterval]=(controls[iinterval].etTowerMeanNoCorrRMS[i]/etNoCorrref);
	 nhitTowerMeanArray[iinterval]=controls[iinterval].nhitTowerMean[i]/nhitref;
	 lcTowerMeanArray[iinterval]=1/(controls[iinterval].lcTowerMean[i]/lcref);
	 lcTowerMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcTowerMean[i],2)*controls[iinterval].lcTowerMeanRMS[i];
	 tlTowerMeanArray[iinterval]=controls[iinterval].tlTowerMean[i]/tlref;
	 tlTowerMeanRMSArray[iinterval]=controls[iinterval].tlTowerMeanRMS[i]/(tlref);

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
   for (int i=0;i<kSM*kXtalPerSM;++i)
     {
       std::cout << "Creating history for ixtal " <<  i+1 << "/" << kSM*kXtalPerSM << std::endl;
       float etref=0;
//        float etNoCorrref=0;
       float lcref=0;
       float tlref=0;
       float nhitref=0;
       int nref=0;
       float etSumOverRef=0;

       for (int iref=-5;iref<6;++iref)
	 {
	   nref++;
	   etref+=controls[historyNormalizationInterval+iref].etXtalMean[i];
	   etSumOverRef+=controls[historyNormalizationInterval+iref].etSumXtalMeanVsEtRef[i]/((controls[historyNormalizationInterval+iref].etSumMean[2][0]+controls[historyNormalizationInterval+iref].etSumMean[2][1])/2.);
// 	   etNoCorrref+=controls[historyNormalizationInterval+iref].etXtalMeanNoCorr[i];
	   lcref+=controls[historyNormalizationInterval+iref].lcXtalMean[i];
	   tlref+=controls[historyNormalizationInterval+iref].tlXtalMean[i];
	   nhitref+=controls[historyNormalizationInterval+iref].nhitXtalMean[i];
	 }

       etref=etref/nref;
       etSumOverRef=etSumOverRef/nref;
//        etNoCorrref=etNoCorrref/nref;
       lcref=lcref/nref;
       tlref=tlref/nref;
       nhitref=nhitref/nref;

       float kf=1.;
       if (kfactorCorr)
	 {
	   int ix=((i)%kXtalPerSM);
	   int ie=(int)ix/20;
	   kf=kFactorsEtSum[ie];
	   std::cout << "kFactor for xtal " << i << " ix " << ix << " ie " << ie << " is " << kf << std::endl;
	 }
       
       for(int iinterval=0;iinterval<kIntervals;iinterval++){
	 //Normalizing to time reference interval

	 float etSumRef=(controls[iinterval].etSumMean[2][0]+controls[iinterval].etSumMean[2][1])/2.;
	 etSumXtalMeanVsRefArray[iinterval]=1 + (((controls[iinterval].etSumXtalMeanVsEtRef[i]/etSumRef)/etSumOverRef)-1.)/kf;
	 etSumXtalMeanVsRefRMSArray[iinterval]=((controls[iinterval].etSumXtalMeanVsEtRefRMS[i]/etSumRef)/etSumOverRef)/kf;
	 etXtalMeanArray[iinterval]=(controls[iinterval].etXtalMean[i]/etref);
	 etXtalMeanRMSArray[iinterval]=(controls[iinterval].etXtalMeanRMS[i]/etref);
// 	 etXtalMeanNoCorrArray[iinterval]=(controls[iinterval].etXtalMeanNoCorr[i]/etNoCorrref);
// 	 etXtalMeanNoCorrRMSArray[iinterval]=(controls[iinterval].etXtalMeanNoCorrRMS[i]/etNoCorrref);
	 nhitXtalMeanArray[iinterval]=controls[iinterval].nhitXtalMean[i]/nhitref;
	 lcXtalMeanArray[iinterval]=1/(controls[iinterval].lcXtalMean[i]/lcref);
	 lcXtalMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcXtalMean[i],2)*controls[iinterval].lcXtalMeanRMS[i];
	 tlXtalMeanArray[iinterval]=controls[iinterval].tlXtalMean[i]/tlref;
	 tlXtalMeanRMSArray[iinterval]=controls[iinterval].tlXtalMeanRMS[i]/(tlref);

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


}
