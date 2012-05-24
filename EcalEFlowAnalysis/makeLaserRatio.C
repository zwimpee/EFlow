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
#include "TGraphAsymmErrors.h"
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
  bool savePlots=true;
  
  TString plotFileName=outFileName;

  if(doRingPlots){
    plotFileName+="etaRing";
    firstFileName+="etaRing.root";
    secondFileName+="etaRing.root";
    outFileName+="etaRing.root";
  }
  if(doTTPlots) {
    plotFileName+="itt";
    firstFileName+="itt.root";
    secondFileName+="itt.root";
    outFileName+="itt.root";
  }
  if(doXtalPlots) {
    plotFileName+="ixtal";
    firstFileName+="ixtal.root";
    secondFileName+="ixtal.root";
    outFileName+="ixtal.root";
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

  int X0=X[0];
  int X1=X[npoints-1];


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
	    if(YSecond[ii]>0.1 )Ratio[ii]=YFirst[ii]/YSecond[ii];
	    //	    cout<<Ratio[ii]<<" "<<YFirst[ii]<<" "<<YSecond[ii]<<endl;
	  }
	outFile->cd();
	TGraph* ratiograph=new TGraph(npoints,&X[0],&Ratio[0]);

	ratiograph->SetName("ratio_"+etaLabel+sideLabel);
	ratiograph->SetTitle("ratio_"+etaLabel+sideLabel);
	ratiograph->GetYaxis()->SetTitle("ratioLc");
	ratiograph->GetXaxis()->SetTitle("time");
	if(savePlots)	ratiograph->Write();
	delete ratiograph;




      }//side
    }//rings
  }//doringplots

  if(doTTPlots){
    TH2F ttMap("ttMap","ttMap",72,0.5,72.5,35,-17.5,17.5);
    TH2F ttMapRMS("ttMapRMS","ttMapRMS",72,0.5,72.5,35,-17.5,17.5);
    ttMap.GetXaxis()->SetTitle("tt phi index");
    ttMap.GetYaxis()->SetTitle("tt eta index");
    ttMapRMS.GetXaxis()->SetTitle("tt phi index");
    ttMapRMS.GetYaxis()->SetTitle("tt eta index");

    double errorlX[npoints];
    double errorhX[npoints];

    TH1F* lcDist[npoints];
    double lcBand[npoints][5];

    for (int ii=0;ii<npoints;++ii)
      {
	TString label="interval_";
	label+=ii;
	lcDist[ii]=new TH1F("lcDist_"+label,"lcDist_"+label,200,0.9,1.1);
	errorlX[ii]=4*3600.;
	errorhX[ii]=4*3600.;
      }


    float axisLower=0.93;
    float axisUp=1.05;
    
    TH1F* histoForRMSTT=new TH1F("histoForRMSTT","histoForRMSTT",200,axisLower,axisUp);
    for (int i=1;i<=nTowers;++i)
      {

	TString ittLabel="itt_";
	ittLabel+=i;


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
	
	firstFile->cd();
	TGraph* graph1=(TGraph*)firstFile->Get("lc_"+ittLabel);
	TGraph* graph2=(TGraph*)secondFile->Get("lc_"+ittLabel);
	

	for (int ii=0;ii<npoints;++ii)
	  {
	    YFirst[ii]=(*(graph1->GetY()+ii));
	    YSecond[ii]=(*(graph2->GetY()+ii));
	    Ratio[ii]=YFirst[ii]/YSecond[ii];
	    histoForRMSTT->Fill(Ratio[ii]);
	    lcDist[ii]->Fill(Ratio[ii]);
	  }
	
	ttMap.SetBinContent(phiIndex,etaIndex,histoForRMSTT->GetMean());
	ttMapRMS.SetBinContent(phiIndex,etaIndex,histoForRMSTT->GetRMS());
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

    double quantiles[5]={0.05,0.16,0.5,0.84,0.95};
     double lcBandGraph[5][npoints];

     for (int ii=0;ii<npoints;++ii)
       {
	 lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	 for (int ij=0;ij<5;++ij)
	   {
	     if (ij!=2)
	       {
		 lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
	       }
		 else
		   {
		     lcBandGraph[ij][ii]=lcBand[ii][ij];
		   }

	      }
	  }

    TGraphAsymmErrors * lc68Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors * lc95Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TCanvas *c1 = new TCanvas("c_full_tt","c_full_tt",1000,500);
    c1->cd();

    TH2F c("c","c",10,X0,X1,10,0.98,1.02);
    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d /%m");


    c.Draw();
    lc95Graph->SetFillColor(kYellow);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");
    if (savePlots)  {
      c1->Write();
      c1->SaveAs("c_full_tt.png");
    }
    ttMap.Write();
    ttMapRMS.Write();
  }//dottplots

  if(doXtalPlots){
    TH2F xtalMap("xtalMap","xtalMap",360,0.5,360.5,171,-85.5,85.5);
    TH2F xtalMapRMS("xtalMapRMS","xtalMapRMS",360,0.5,360.5,171,-85.5,85.5);
    xtalMap.GetXaxis()->SetTitle("xtal phi index");
    xtalMap.GetYaxis()->SetTitle("xtal eta index");
    xtalMapRMS.GetXaxis()->SetTitle("xtal phi index");
    xtalMapRMS.GetYaxis()->SetTitle("xtal eta index");

    double errorlX[npoints];
    double errorhX[npoints];

    TH1F* lcDist[npoints];
    double lcBand[npoints][5];

    for (int ii=0;ii<npoints;++ii)
      {
	TString label="interval_";
	label+=ii;
	lcDist[ii]=new TH1F("lcDist_"+label,"lcDist_"+label,200,0.9,1.1);
	errorlX[ii]=4*3600.;
	errorhX[ii]=4*3600.;
      }


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
	  lcDist[ii]->Fill(Ratio[ii]);
	}
	
	xtalMap.SetBinContent(phiIndex,etaIndex,histoForRMSXTAL->GetMean());
	xtalMapRMS.SetBinContent(phiIndex,etaIndex,histoForRMSXTAL->GetRMS());
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

    double quantiles[5]={0.05,0.16,0.5,0.84,0.95};
    double lcBandGraph[5][npoints];

    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
	      }

	      }
	  }

    TGraphAsymmErrors * lc68Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors * lc95Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TCanvas *c1 = new TCanvas("c_full_xtal","c_full_xtal",1000,500);
    c1->cd();
    TH2F c("c","c",10,X0,X1,10,0.96,1.02);
    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d /%m");


    c.Draw();
    lc95Graph->SetFillColor(kYellow);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");
    TString dummyFileName=plotFileName;
    if (savePlots){

      c1->SaveAs(plotFileName+="_history_full_xtal.png");
      c1->Write();
      c1->Close();
    }
    TCanvas *c2 = new TCanvas("map_xtal","map_xtal",1000,500);
    c2->cd();


    xtalMap.SetAxisRange(0.98,1.02,"Z");
    xtalMap.Draw("colz");

    if(savePlots) c2->SaveAs(dummyFileName+="_map_xtal.png");

    xtalMap.Write();
    xtalMapRMS.Write();
  }

  outFile->Write();
  outFile->Close();

}
