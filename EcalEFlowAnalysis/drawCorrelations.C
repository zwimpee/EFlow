#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TString.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TH2F.h"

#include <iostream>


bool isChinese(int ieta, int iphi)
{
  if( (ieta>-76 ) && (ieta<76) )
    return false;

  if(ieta>75){
    if(iphi>300 && iphi<=340) return true;
    if(iphi>120 && iphi<=140) return true;
    if(iphi>231 && iphi<=240) return true;
    if(ieta>=81){
      if(iphi>220 && iphi<=240) return true;
      if(iphi>180 && iphi<=200) return true;
    }
  }

  if(ieta<-76)
    {
      if(iphi>200 && iphi<=220) return true;
      if(iphi>240 && iphi<=300) return true;
      if(ieta<= -81){
	if(iphi>20 && iphi<=40) return true;
	if(iphi>60 && iphi<=100) return true;
      }
    }
  return false;
}

void drawCorrelations(
		      TString selection="BTCP",
		      TString fitResultsFile="file1.root",
		      TString constructionDBFile="root://pccmsrm27///cms/local/meridian/EFlow/constructionDB/ana_rad.root",
		      TString icMapFile="EBMap.root"
		      )
{
  TFile *_file0 = TFile::Open(fitResultsFile);
  TFile *_file1 = TFile:: Open(constructionDBFile);
  TFile *_file2 = TFile::Open(icMapFile);

  int fitStatus[85][360][2];
  bool sicXtals[85][360][2];
  double delta_alpha[85][360][2];
  double err_alpha[85][360][2];
  double lto360[85][360][2];
  double lto420[85][360][2];
  double ly[85][360][2];
  double roughness[85][360][2];
  double alphaLab[85][360][2];
  double ic[85][360][2];
  double icErr[85][360][2];


  TTree* fitResults=(TTree*)_file0->Get("fitResults");
  
  int ietaVar,iphiVar,signVar,statusVar;
  double alphaVar,err_alphaVar;
  TBranch *b_ieta=fitResults->GetBranch("ieta");
  TBranch *b_iphi=fitResults->GetBranch("iphi");
  TBranch *b_sig=fitResults->GetBranch("sign");
  TBranch *b_status=fitResults->GetBranch("status");
  TBranch *b_alpha=fitResults->GetBranch("delta_alpha");
  TBranch *b_erralpha=fitResults->GetBranch("err_alpha");

  
  fitResults->SetBranchAddress("ieta", &ietaVar, &b_ieta);
  fitResults->SetBranchAddress("iphi", &iphiVar, &b_iphi);
  fitResults->SetBranchAddress("sign", &signVar, &b_sig);
  fitResults->SetBranchAddress("status", &statusVar, &b_status);
  fitResults->SetBranchAddress("delta_alpha", &alphaVar, &b_alpha);
  fitResults->SetBranchAddress("err_alpha", &err_alphaVar, &b_erralpha);

  int nentries_ee = fitResults->GetEntries();
  for(int jentry=0;jentry<nentries_ee;++jentry)
    {
      fitResults->GetEntry(jentry);
      delta_alpha[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=alphaVar;
      err_alpha[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=err_alphaVar;
      fitStatus[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=statusVar;
    }

  TTree* constrDB=(TTree*)_file1->Get("T");
  
  float lto360Var,lto420Var,roughnessVar,lyVar,alphaLabVar;

  b_ieta=constrDB->GetBranch("ieta");
  b_iphi=constrDB->GetBranch("iphi");
  TBranch *b_lto360=constrDB->GetBranch("lto_360");
  TBranch *b_lto420=constrDB->GetBranch("lto_420");
  TBranch *b_ly=constrDB->GetBranch("ly");
  TBranch *b_alphaLab=constrDB->GetBranch("alpha");
  TBranch *b_rough=constrDB->GetBranch("roughness");

  
  constrDB->SetBranchAddress("ieta", &ietaVar, &b_ieta);
  constrDB->SetBranchAddress("iphi", &iphiVar, &b_iphi);
  constrDB->SetBranchAddress("lto_360", &lto360Var, &b_lto360);
  constrDB->SetBranchAddress("lto_420", &lto420Var, &b_lto420);
  constrDB->SetBranchAddress("ly", &lyVar, &b_ly);
  constrDB->SetBranchAddress("alpha", &alphaLabVar, &b_alphaLab);
  constrDB->SetBranchAddress("roughness", &roughnessVar, &b_rough);

  int nentries = constrDB->GetEntries();
  for(int jentry=0;jentry<nentries;++jentry)
    {
      constrDB->GetEntry(jentry);
      signVar = (ietaVar>0) ? 1:0;
      bool SICXtal=isChinese(ietaVar,iphiVar);
      lto360[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=lto360Var;
      lto420[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=lto420Var;
      sicXtals[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=SICXtal;
      ly[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=lyVar;
      alphaLab[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=alphaLabVar;
      roughness[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=roughnessVar;
    }
  
  TH2F* icMap=(TH2F*) _file2->Get("EBMapCoeff1");


  for (int phi=1;phi<361;++phi)
    {
      for (int eta=1;eta<172;++eta)
 	{
	  if (eta==86)
	    continue;
	  ietaVar=TMath::Abs(eta-86);
	  if (eta<86)
	      signVar=0;
	  else
	      signVar=1;

 	  ic[ietaVar-1][phi-1][signVar]=icMap->GetBinContent(phi,eta);
	  icErr[ietaVar-1][phi-1][signVar]=0.005;
 	}
     }

  std::cout << "done with inputs" << std::endl;

  double alphaArr[61200];
  double alphaLabArr[61200];
  double alphaErrArr[61200];
  double lto360Arr[61200];
  double lto360ErrArr[61200];
  double icArr[61200];
  double icErrArr[61200];
  double roughnessArr[61200];
  double roughnessErrArr[61200];

  int i=0;
  for (int ie=0;ie<85;++ie)
    for (int ip=0;ip<360;++ip)  
      for (int is=0;is<2;++is)  
	{
	  if (fitStatus[ie][ip][is]!=0)
	    continue;

// 	  if (roughness[ie][ip][is]!=0)
// 	    continue;

// 	  if (err_alpha[ie][ip][is]==0)
// 	    continue;

// 	  if (alphaLab[ie][ip][is]==0)
// 	    continue;

	  if (selection=="BTCP")
	    {
	      if (sicXtals[ie][ip][is])
		continue;
	    }

	  if (selection=="SIC")
	    {
	      if (!sicXtals[ie][ip][is])
		continue;
	    }

	  alphaArr[i]=delta_alpha[ie][ip][is];
	  alphaLabArr[i]=alphaLab[ie][ip][is];
	  alphaErrArr[i]=err_alpha[ie][ip][is];
	  lto360Arr[i]=lto360[ie][ip][is];
	  lto360ErrArr[i]=1.;
	  icArr[i]=1/ic[ie][ip][is];
	  icErrArr[i]=icErr[ie][ip][is];
	  roughnessArr[i]=roughness[ie][ip][is];
	  roughnessErrArr[i]=0.;
	  ++i;
	}

  std::cout << "Found " << i << " xtals " << std::endl;

  TCanvas* c1=new TCanvas("c1","c1",900,600);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptFit(1111111);
  gStyle->SetOptTitle(0);
  TH2F a("a","a",10,10,60,10,-2.,2.);
  a.GetXaxis()->SetTitle("lto360");
  a.GetYaxis()->SetTitle("alpha correction");
  a.Draw();
  TGraph* alphaLto360Corr=new TGraphErrors(i,lto360Arr,alphaArr);
//   alphaLto360Corr->SetMarkerStyle(20);
//   alphaLto360Corr->SetMarkerColor(kGreen);
//   alphaLto360Corr->SetMarkerSize(0.8);
  alphaLto360Corr->Draw("PSAME");
  c1->SaveAs("plotsFit/alphaLto360Corr_"+selection+".png");


  TH2F b("b","b",10,0.5,1.5,10,-2.,2.);
  b.GetXaxis()->SetTitle("1/ic");
  b.GetYaxis()->SetTitle("alpha correction");
  b.Draw();
  TGraph* alphaIcCorr=new TGraphErrors(i,icArr,alphaArr);
//   alphaIcCorr->SetMarkerStyle(20);
//   alphaIcCorr->SetMarkerColor(kGreen);
//   alphaIcCorr->SetMarkerSize(0.4);
   alphaIcCorr->Draw("PSAME");
  c1->SaveAs("plotsFit/alphaIcCorr"+selection+".png");



  TH2F c("c","c",10,1.,2.,10,-2.,2.);
  c.GetXaxis()->SetTitle("alpha TB");
  c.GetYaxis()->SetTitle("alpha correction");
  c.Draw();
  TGraph* alphaVsalphaLab=new TGraphErrors(i,alphaLabArr,alphaArr);
  alphaVsalphaLab->SetMarkerStyle(20);
  alphaVsalphaLab->SetMarkerColor(kBlack);
  alphaVsalphaLab->SetMarkerSize(0.8);
  alphaVsalphaLab->Draw("PSAME");
  c1->SaveAs("plotsFit/alphaVsalphaLab"+selection+".png");

  TH2F d("d","d",10,-0.2,1.2,10,-2.,2.);
  d.GetXaxis()->SetTitle("roughness");
  d.GetYaxis()->SetTitle("alpha correction");
  d.Draw();
  TGraph* alphaRoughCorr=new TGraphErrors(i,roughnessArr,alphaArr);
//   alphaIcCorr->SetMarkerStyle(20);
//   alphaIcCorr->SetMarkerColor(kGreen);
//   alphaIcCorr->SetMarkerSize(0.4);
  alphaRoughCorr->Draw("PSAME");
  c1->SaveAs("plotsFit/alphaRoughCorr"+selection+".png");
}
