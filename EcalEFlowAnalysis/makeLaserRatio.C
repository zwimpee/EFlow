#define makeLaserRatio_cxx
#include "makeLaserRatio.h"
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


void makeLaserRatio::Loop()
{

  bool doRingPlots=false;
  bool doTTPlots=false;
  bool doXtalPlots=true;
  bool quickTest=true;
  bool savePlots=true;
  

  if(doRingPlots){
    firstFileName+="etaRing.root";
    secondFileName+="etaRing.root";
  }
  if(doTTPlots) {
    firstFileName+="itt.root";
    secondFileName+="itt.root";
  }
  if(doXtalPlots) {
    firstFileName+="ixtal.root";
    secondFileName+="ixtal.root";
  }


  TFile *firstFile=TFile::Open(firstFileName);
  TFile *secondFile=TFile::Open(secondFileName);
  TFile *outFile=TFile::Open(outFileName,"recreate");
  cout<<"reading "<<firstFileName<<" "<<secondFileName<<endl;  

  TGraphErrors* lc;
  if(doRingPlots)  lc=(TGraphErrors*)firstFile->Get("lc_ieta_1_side_0");
  if(doTTPlots)  lc=(TGraphErrors*)firstFile->Get("lc_itt_1");
  if(doXtalPlots)  lc=(TGraphErrors*)firstFile->Get("lc_ixtal_1");
  int npoints=lc->GetN();
  double X[npoints],YFirst[npoints],YSecond[npoints],Ratio[npoints];
  cout<<npoints<<endl;
  for (int ii=0;ii<npoints;++ii){
    X[ii]=(*(lc->GetX()+ii));
  }


  if(doRingPlots){
  for (int i=1;i<kBarlRings+1;++i)
    {
      for (int j=0;j<2;j++){
        TString etaLabel="ieta_";
        etaLabel+=i;
        TString sideLabel="_side_";
        sideLabel+=j;
        TString eta="";
	firstFile->cd();    
	TGraph* graph1=(TGraph*)firstFile->Get("lc_"+etaLabel+sideLabel);
	TGraph* graph2=(TGraph*)secondFile->Get("lc_"+etaLabel+sideLabel);      
        for (int ii=0;ii<npoints;++ii)
          {
            YFirst[ii]=(*(graph1->GetY()+ii));
            YSecond[ii]=(*(graph2->GetY()+ii));
	    Ratio[ii]=YFirst[ii]/YSecond[ii];
	  }
	outFile->cd();
	TGraph* ratiograph=new TGraph(npoints,&X[0],&Ratio[0]);

	ratiograph->SetName("ratio_"+etaLabel+sideLabel);
	ratiograph->SetTitle("ratio_"+etaLabel+sideLabel);
	ratiograph->GetYaxis()->SetTitle("ratioLc");
	ratiograph->GetXaxis()->SetTitle("time");
	if(savePlots)	ratiograph->Write();
	delete ratiograph;


	//	cout<<YFirst[0]<<" "<<YSecond[0]<<endl;

      }//side
    }//rings
  }//doringplots

  if(doTTPlots){
    TH2F ttMap("ttMap","ttMap",72,0.5,72.5,35,-17.5,17.5);
    TH2F ttMapRMS("ttMapRMS","ttMapRMS",72,0.5,72.5,35,-17.5,17.5);
    float axisLower=0.93;
    float axisUp=1.05;
    
    TH1F* histoForRMSTT=new TH1F("histoForRMSTT","histoForRMSTT",200,axisLower,axisUp);
    for (int i=1;i<nTowers+1;++i)
      {

	int etaIndex=0;
	int phiIndex=0;
	int smIndex=(i-1)/68+1;
	int ttIndex=(i-1)%68+1;
	if (smIndex>18)
	  {
	    etaIndex=17-((ttIndex-1)/4+1)+1;
	  }
	else
	  {
	    etaIndex=17+((ttIndex-1)/4+1)+1;
	  }
	phiIndex=(((smIndex-1)%18)*4)+((ttIndex-1)%4+1);
	//	std::cout << "i " << " " << smIndex << " " << ttIndex << " " << etaIndex << " " << phiIndex << std::endl;
	TString ittLabel="itt_";
	ittLabel+=i;
	
	firstFile->cd();
	TGraph* graph1=(TGraph*)firstFile->Get("lc_"+ittLabel);
	TGraph* graph2=(TGraph*)secondFile->Get("lc_"+ittLabel);
	

	for (int ii=0;ii<npoints;++ii)
	  {
	    YFirst[ii]=(*(graph1->GetY()+ii));
	    YSecond[ii]=(*(graph2->GetY()+ii));
	    Ratio[ii]=YFirst[ii]/YSecond[ii];
	    histoForRMSTT->Fill(Ratio[ii]);
	  }
	
	ttMap.Fill(phiIndex,etaIndex,histoForRMSTT->GetMean());
	ttMapRMS.Fill(histoForRMSTT->GetRMS());
	histoForRMSTT->Reset();
      
	outFile->cd();
	TGraph* ratiograph=new TGraph(npoints,&X[0],&Ratio[0]);

	ratiograph->SetName("ratio_"+ittLabel);
	ratiograph->SetTitle("ratio_"+ittLabel);
	ratiograph->GetYaxis()->SetTitle("ratioLc");
	ratiograph->GetXaxis()->SetTitle("time");
	if(savePlots)      ratiograph->Write();
	delete ratiograph;



      }//towernumber
    ttMap.Write();
    ttMapRMS.Write();
  }//dottplots

  if(doXtalPlots){
    TH2F xtalMap("xtalMap","xtalMap",360,0.5,360.5,171,-85.5,85.5);
    TH2F xtalMapRMS("xtalMapRMS","xtalMapRMS",360,0.5,360.5,171,-85.5,85.5);
    float axisLowerXtal=0.9;
    float axisUpXtal=1.1;
    TH1F* histoForRMSXTAL=new TH1F("histoForRMSXTAL","histoForRMSXTAL",200,axisLowerXtal,axisUpXtal);

    for (int i=1;i<=nXtals;++i){

      if(i%1000==0)cout<<"ixtal n."<<i<<endl;
      TString ixtalLabel="ixtal_";
      ixtalLabel+=i;
      TString ixtal="";
      ixtal+=i;

      int etaIndex=(i-1)/360+1;
      int phiIndex=(i-1)%360+1;
      if (etaIndex>85)
	etaIndex++;
      else
	etaIndex=86-etaIndex;

      firstFile->cd();
      TGraph* graph1=(TGraph*)firstFile->Get("lc_"+ixtalLabel);
      TGraph* graph2=(TGraph*)secondFile->Get("lc_"+ixtalLabel);

      for (int ii=0;ii<npoints;++ii)
	{
	  YFirst[ii]=(*(graph1->GetY()+ii));
	  YSecond[ii]=(*(graph2->GetY()+ii));
	  Ratio[ii]=YFirst[ii]/YSecond[ii];
	  histoForRMSXTAL->Fill(Ratio[ii]);
	}
	
	xtalMap.Fill(phiIndex,etaIndex,histoForRMSXTAL->GetMean());
	xtalMapRMS.Fill(histoForRMSXTAL->GetRMS());
	histoForRMSXTAL->Reset();
      
	outFile->cd();
	TGraph* ratiograph=new TGraph(npoints,&X[0],&Ratio[0]);
	ratiograph->SetName("ratio_"+ixtalLabel);
	ratiograph->SetTitle("ratio_"+ixtalLabel);
	ratiograph->GetYaxis()->SetTitle("ratioLc");
	ratiograph->GetXaxis()->SetTitle("time");
	if(savePlots)      ratiograph->Write();
	delete ratiograph;

    }//close nXtals
    xtalMap.Write();
    xtalMapRMS.Write();
  }

  outFile->Write();
  outFile->Close();

}
