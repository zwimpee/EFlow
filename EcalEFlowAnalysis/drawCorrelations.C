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


void drawCorrelations(
		      TString selection="BTCP",
		      TString fitResultsFile="file1.root",
		      float errAlphaCut=0.12,
		      float ndofCut=0,
		      float chi2Cut=999,
		      TString icRatioFile="calibMap.root",
		      TString constructionDBFile="root://pccmsrm27///cms/local/meridian/EFlow/constructionDB/ana_rad.root",
		      TString icMapFile="EBMap.root"
		      )
{
  TFile *_file0 = TFile::Open(fitResultsFile);
  TFile *_file1 = TFile:: Open(constructionDBFile);
  TFile *_file2 = TFile::Open(icMapFile);
  TFile *_file3 = TFile::Open(icRatioFile);

  int fitStatus[85][360][2];
  int badXtal[85][360][2];
  bool sicXtals[85][360][2];
  double delta_alpha[85][360][2];
  double chi2red[85][360][2];
  double ndof[85][360][2];
  double err_alpha[85][360][2];
  double lto360[85][360][2];
  double lto420[85][360][2];
  double ly[85][360][2];
  double roughness[85][360][2];
  double alphaLab[85][360][2];
  double ic[85][360][2];
  double icErr[85][360][2];
  double icRatio[85][360][2];
  double icRatioErr[85][360][2];


  TTree* fitResults=(TTree*)_file0->Get("fitResults");
  
  int ietaVar,iphiVar,signVar,statusVar,badXtalVar,indexVar;
  double alphaVar,err_alphaVar,chi2,ndof;
  TBranch *b_ieta=fitResults->GetBranch("ieta");
  TBranch *b_badXtal=fitResults->GetBranch("badXtal");
  TBranch *b_iphi=fitResults->GetBranch("iphi");
  TBranch *b_index=fitResults->GetBranch("index");
  TBranch *b_status=fitResults->GetBranch("status");
  TBranch *b_alpha=fitResults->GetBranch("delta_alpha");
  TBranch *b_erralpha=fitResults->GetBranch("err_alpha");
  TBranch *b_chi2=fitResults->GetBranch("chi2Min");
  TBranch *b_ndof=fitResults->GetBranch("ndof");

  
  fitResults->SetBranchAddress("ieta", &ietaVar, &b_ieta);
  fitResults->SetBranchAddress("iphi", &iphiVar, &b_iphi);
  fitResults->SetBranchAddress("badXtal", &badXtalVar, &b_badXtal);
  fitResults->SetBranchAddress("index", &indexVar, &b_index);
  fitResults->SetBranchAddress("status", &statusVar, &b_status);
  fitResults->SetBranchAddress("delta_alpha", &alphaVar, &b_alpha);
  fitResults->SetBranchAddress("err_alpha", &err_alphaVar, &b_erralpha);
  fitResults->SetBranchAddress("chi2Min", &chi2, &b_chi2);
  fitResults->SetBranchAddress("ndof", &ndof, &b_ndof);

  int nentries_ee = fitResults->GetEntries();
  for(int jentry=0;jentry<nentries_ee;++jentry)
    {
      fitResults->GetEntry(jentry);
      if (indexVar>30600)
	signVar=1;
      else
	signVar=0;

      if (badXtalVar==1)
	std::cout << "FOUND A BAD XTAL " <<  indexVar << std::endl;

      badXtalVar[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=badXtalVar;
      //      std::cout << TMath::Abs(ietaVar)-1 << "," << iphiVar-1 << "," << signVar << ": " << alphaVar << std::endl;
      delta_alpha[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=alphaVar;
      err_alpha[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=err_alphaVar;
      fitStatus[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=statusVar;
      chi2red[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=chi2/ndof;
      ndof[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=ndof;
    }


  TTree* icRatioTree=(TTree*)_file3->Get("interCalibRatios");
  
  double icRatioVar,icRatioErrVar;
  TBranch *b_ieta=icRatioTree->GetBranch("ieta");
  TBranch *b_iphi=icRatioTree->GetBranch("iphi");
  TBranch *b_ic=icRatioTree->GetBranch("ic");
  TBranch *b_icErr=icRatioTree->GetBranch("icErr");

  
  icRatioTree->SetBranchAddress("ieta", &ietaVar, &b_ieta);
  icRatioTree->SetBranchAddress("iphi", &iphiVar, &b_iphi);
  icRatioTree->SetBranchAddress("ic", &icRatioVar, &b_ic);
  icRatioTree->SetBranchAddress("icErr", &icRatioErrVar, &b_icErr);

  int nentriesIc = icRatioTree->GetEntries();
  for(int jentry=0;jentry<nentriesIc;++jentry)
    {
      icRatioTree->GetEntry(jentry);
      if (ietaVar>0)
	signVar=1;
      else
	signVar=0;

      icRatio[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=1/icRatioVar;
      icRatioErr[TMath::Abs(ietaVar)-1][iphiVar-1][signVar]=icRatioErrVar;
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
  double lto420Arr[61200];
  double lto420ErrArr[61200];
  double icArr[61200];
  double icErrArr[61200];
  double icRatioArr[61200];
  double icRatioErrArr[61200];
  double roughnessArr[61200];
  double roughnessErrArr[61200];

  int nXtalsTT[2448];
  double alphaTT[2448];
  double icTT[2448];
  double alphaTTErr[2448];
  double icTTErr[2448];

  for (int i=0;i<2448;++i)
    {
      nXtalsTT[i]=0;
      alphaTT[i]=0.;
      icTT[i]=0.;
    }

  int i=0;

  TH2F correlationWithICRatio("correlationWithICRatio","correlationWithICRatio",100,0.96,1.04,100,-1.5,1.5);
  TH2F correlationWithICRatioTT("correlationWithICRatioTT","correlationWithICRatioTT",100,0.975,1.015,100,-1.5,1.5);
  TH2F correlationWithIC("correlationWithIC","correlationWithIC",100,0.7,1.3,100,-1.5,1.5);
  TH2F correlationWithLTO360("correlationWithLTO360","correlationWithLTO360",100,20.,60.,100,-1.5,1.5);
  TH2F correlationWithLTO420("correlationWithLTO420","correlationWithLTO420",100,50.,80.,100,-1.5,1.5);

  for (int ie=0;ie<85;++ie)
    for (int ip=0;ip<360;++ip)  
      for (int is=0;is<2;++is)  
	{
	  if (badXtal[ie][ip][is]==1)
	    continue;

	  if (delta_alpha[ie][ip][is]==0)
	    continue;

	  if (fitStatus[ie][ip][is]!=0)
	    continue;

	  if (ndof[ie][ip][is]<ndofCut)
	    continue;

	  if (chi2red[ie][ip][is]>chi2Cut)
	    continue;

// 	  if (roughness[ie][ip][is]!=0)
// 	    continue;

 	  if (err_alpha[ie][ip][is]>errAlphaCut)
 	    continue;

 	  if (alphaLab[ie][ip][is]!=0)
	      std::cout << ie+1 << "," << ip+1 << "," << is << "," << alphaLab[ie][ip][is] << "," << 1/ic[ie][ip][is] << std::endl;

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

 	  if (delta_alpha[ie][ip][is]<-1.2)
 	    continue;

 	  if (delta_alpha[ie][ip][is]>1.)
 	    continue;

// 	  if (1/ic[ie][ip][is]<1.25)
// 	    continue;

	  alphaArr[i]=delta_alpha[ie][ip][is];
	  alphaLabArr[i]=alphaLab[ie][ip][is];
	  alphaErrArr[i]=err_alpha[ie][ip][is];
	  lto360Arr[i]=lto360[ie][ip][is];
	  lto360ErrArr[i]=1.;
	  lto420Arr[i]=lto420[ie][ip][is];
	  lto420ErrArr[i]=1.;
	  icArr[i]=1/ic[ie][ip][is];
	  icErrArr[i]=icErr[ie][ip][is];
	  icRatioArr[i]=icRatio[ie][ip][is];
	  icRatioErrArr[i]=icRatioErr[ie][ip][is];
	  roughnessArr[i]=roughness[ie][ip][is];
	  roughnessErrArr[i]=0.007;
	  correlationWithIC.Fill(icArr[i],alphaArr[i]);
	  correlationWithLTO360.Fill(lto360Arr[i],alphaArr[i]);
	  correlationWithLTO420.Fill(lto420Arr[i],alphaArr[i]);
	  ++i;

	  
	  if (delta_alpha[ie][ip][is]!=0 && ie<45)
	    {
	      int tt=iTT(ie+1,ip+1,is);
	      nXtalsTT[tt-1]++;
	      alphaTT[tt-1]+=delta_alpha[ie][ip][is];
	      icTT[tt-1]+=icRatio[ie][ip][is];
	      alphaTTErr[tt-1]+=1/(err_alpha[ie][ip][is]*err_alpha[ie][ip][is]);
	      icTTErr[tt-1]+=1/(icRatioErr[ie][ip][is]*icRatioErr[ie][ip][is]);
	      correlationWithICRatio.Fill(icRatio[ie][ip][is],delta_alpha[ie][ip][is]);
	    }
	}


  int nGoodTT=0;
  double alphaTTArr[2448];
  double icTTArr[2448];
  double alphaTTErrArr[2448];
  double icTTErrArr[2448];
  
  for (int it=0;it<2448;++it)
    {
      if (nXtalsTT[it]>0 && alphaTT[it]!=0)  
	{
	  //	  std::cout << nGoodTT << "," << nXtalsTT[it] << std::endl;
	  alphaTTArr[nGoodTT]=alphaTT[it]/nXtalsTT[it];
	  icTTArr[nGoodTT]=icTT[it]/nXtalsTT[it];
	  alphaTTErrArr[nGoodTT]=0.03;
	  icTTErrArr[nGoodTT]=0.003;
	  correlationWithICRatioTT.Fill(icTTArr[nGoodTT],alphaTTArr[nGoodTT]);
	  nGoodTT++;
	}
    }

  std::cout << "Found " << i << " xtals " << std::endl;

  
  TCanvas* c1=new TCanvas("c1","c1",900,600);
  gStyle->SetOptStat(0);
  //  gStyle->SetOptFit(1111111);
  gStyle->SetOptTitle(0);

  gStyle->SetOptStat(1);
  TH1F alphaHist("alpha","alpha",500,-2.,2.);
  for (int ix=0;ix<i;++ix)
    alphaHist.Fill(alphaArr[ix]);
  alphaHist.SetStats(true);
  alphaHist.Draw();
  c1->SaveAs("plotsFit/alphaHist_"+selection+".png");

  gStyle->SetOptStat(0);
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

  gStyle->SetOptStat(0);
  TH2F a1("a1","a1",10,50,80,10,-2.,2.);
  a1.GetXaxis()->SetTitle("lto420");
  a1.GetYaxis()->SetTitle("alpha correction");
  a1.Draw();
  TGraph* alphaLto420Corr=new TGraphErrors(i,lto420Arr,alphaArr);
//   alphaLto360Corr->SetMarkerStyle(20);
//   alphaLto360Corr->SetMarkerColor(kGreen);
//   alphaLto360Corr->SetMarkerSize(0.8);
  alphaLto420Corr->Draw("PSAME");
  c1->SaveAs("plotsFit/alphaLto420Corr_"+selection+".png");


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

  TH2F b2("b2","b2",10,0.95,1.05,10,-2.,2.);
  b2.GetXaxis()->SetTitle("icRatio");
  b2.GetYaxis()->SetTitle("alpha correction");
  b2.Draw();
  TGraph* alphaIcRatioCorr=new TGraphErrors(i,icRatioArr,alphaArr,icRatioErrArr,alphaErrArr);
//   alphaIcCorr->SetMarkerStyle(20);
//   alphaIcCorr->SetMarkerColor(kGreen);
//   alphaIcCorr->SetMarkerSize(0.4);
  alphaIcRatioCorr->Draw("PSAME");
  alphaIcRatioCorr->Fit("pol1","0+","",0.98,1.02);
  TF1* f=(TF1*) alphaIcRatioCorr->GetFunction("pol1");
  f->SetLineColor(2);
  f->SetLineWidth(4);
  f->Draw("SAME");
  c1->SaveAs("plotsFit/alphaIcRatioCorr"+selection+".png");


  b2.Draw();
  TGraphErrors* alphaTTIcRatioCorr=new TGraphErrors(nGoodTT,icTTArr,alphaTTArr,icTTErrArr,alphaTTErrArr);
//   alphaIcCorr->SetMarkerStyle(20);
//   alphaIcCorr->SetMarkerColor(kGreen);
//   alphaIcCorr->SetMarkerSize(0.4);
  alphaTTIcRatioCorr->Draw("PSAME");
  //  alphaTTIcRatioCorr->Fit("pol1","","",0.98,1.02);
  c1->SaveAs("plotsFit/alphaTTIcRatioCorr"+selection+".png");




  gStyle->SetOptFit(1111111);
  correlationWithICRatioTT.Draw("COLZ");
  correlationWithICRatioTT.GetXaxis()->SetTitle("IC_{end 2011}/IC_{begin 2011} (TT average)");
  correlationWithICRatioTT.GetYaxis()->SetTitle("#delta_{alpha}");
  correlationWithICRatioTT.ProfileX();
  correlationWithICRatioTT_pfx->Draw("SAME");
  correlationWithICRatioTT_pfx->Fit("pol1","+","SAME",0.98,1.02);
  c1->SaveAs("plotsFit/correlationWithICRatioTT"+selection+".png");


  correlationWithICRatio.Draw("COLZ");
  correlationWithICRatio.GetXaxis()->SetTitle("IC_{end 2011}/IC_{begin 2011}");
  correlationWithICRatio.GetYaxis()->SetTitle("#delta_{alpha}");
  correlationWithICRatio.ProfileX();
  //  correlationWithICRatio_pfx->Draw("SAME");
  correlationWithICRatioTT_pfx->Draw("SAME");
  //  correlationWithICRatio_pfx->Fit("pol1","","SAME",0.98,1.015);
  c1->SaveAs("plotsFit/correlationWithICRatio"+selection+".png");

  correlationWithIC.Draw("COLZ");
  correlationWithIC.GetXaxis()->SetTitle("1/IC (LY)");
  correlationWithIC.GetYaxis()->SetTitle("#delta_{alpha}");
  correlationWithIC.ProfileX();
  correlationWithIC_pfx->Draw("SAME");
  correlationWithIC_pfx->Fit("pol2","+","SAME");
  c1->SaveAs("plotsFit/correlationWithIC"+selection+".png");

  correlationWithLTO360.Draw("COLZ");
  correlationWithLTO360.GetXaxis()->SetTitle("LTO360");
  correlationWithLTO360.GetYaxis()->SetTitle("#delta_{alpha}");
  correlationWithLTO360.ProfileX();
  correlationWithLTO360_pfx->Draw("SAME");
  correlationWithLTO360_pfx->Fit("pol2","+","SAME");
  c1->SaveAs("plotsFit/correlationWithLTO360"+selection+".png");

  correlationWithLTO420.Draw("COLZ");
  correlationWithLTO420.GetXaxis()->SetTitle("LTO420");
  correlationWithLTO420.GetYaxis()->SetTitle("#delta_{alpha}");
  correlationWithLTO420.ProfileX();
  correlationWithLTO420_pfx->Draw("SAME");
  correlationWithLTO420_pfx->Fit("pol2","+","SAME");
  c1->SaveAs("plotsFit/correlationWithLTO420"+selection+".png");




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
