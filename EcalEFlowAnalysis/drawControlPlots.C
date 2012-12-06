#include "TStyle.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TLine.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraphAsymmErrors.h"
#include "constants.h"
#include "HarnessMap.h"
#include "HarnessMap.cc"

using namespace std;

void drawControlPlots (
		       TString prefix="/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_800M__",
		       bool doRingPlots=true,
		       bool doAlsoTTPlots=false,
		       bool doAlsoHarnessPlots=false,
		       bool doAlsoXtalPlots=false,
		       bool doAlsoEndcRingPlots=true,
		       bool doAlsoEndcSCsPlots=false,
		       bool doAlsoEndcHarnessPlots=false,
		       bool doAlsoEndcXtalPlots=false,
		       bool quickTest=false,
		       bool savePlots=true,
		       bool useEtSumOverEtSumRef=true,
		       int X0=1300000000+86400*25,
		       int X1=1320105600+86400*4,
		       int startInterval=0,
		       float axisLower=0.93,
		       float axisUp=1.05,
		       float axisLowerXtal=0.9,
		       float axisUpXtal=1.1,
		       float ttMeanLowThreshold=0.985,
		       float ttMeanHighThreshold=1.015,
		       float ttRMSThreshold=0.004,
		       float harnessMeanLowThreshold=0.985,
		       float harnessMeanHighThreshold=1.015,
		       float harnessRMSThreshold=0.004,
		       float xtalMeanLowThreshold=0.98,
		       float xtalMeanHighThreshold=1.02,
		       float xtalRMSThreshold=0.013,
		       TString eeIndicesFile="data/eeIndicesMap.root",
		       TString harnessMapFile="data/harnessMap.root"
		       )
{

  TString suffix;
  if (useEtSumOverEtSumRef)
    suffix="_EtSumOverEtSumRef.png";
  else
    suffix=".png";
  int nRings=85;
  int nTowers=36*68;
  int nHarness_EB=324;
  int nHarness_EE=38;
  int nXtals=36*1700;

  int nEndcRings=39;  
  int nEndcXtals=14648;
  int nSCs=624;


  //   if (quickTest)
  //     {
  //       nRings=85;
  //       nTowers=68*2;
  //       nXtals=500;

  //       nEndcRings=39;  
  //       nEndcXtals=500;
  //       nSCs=100;
  //     }

  gStyle->SetStatH(0.045);
  gStyle->SetStatFontSize(0.023);
  gStyle->SetStatW(0.65); 
  gStyle->SetStatY( 0.87 ); 
  gStyle->SetStatX( 0.82 ); 

  //  TString prefix="/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2012A_v1_190456_191277.root_";

  TFile *_file0 = TFile::Open(prefix+"etaRing.root");

  TFile * _file1=0;
  if (doAlsoTTPlots)
    {
      _file1 = TFile::Open(prefix+"itt.root");
      if (!_file1->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }



  TFile *_file2=0;
  if (doAlsoXtalPlots)
    {
      _file2 = TFile::Open(prefix+"ixtal.root");
      if (!_file2->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }

  TFile *_file3=0;
  if (doAlsoEndcRingPlots)
    {
      _file3 = TFile::Open(prefix+"endcRing.root");
      if (!_file3->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }

  TFile *_file4=0;
  if (doAlsoEndcSCsPlots)
    {
      _file4 = TFile::Open(prefix+"endcSCs.root");
      if (!_file4->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }

  TFile *_file5=0;
  if (doAlsoEndcXtalPlots)
    {
      _file5 = TFile::Open(prefix+"ixtalEndc.root");
      if (!_file5->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }

  TFile * _file6=0;
  if (doAlsoHarnessPlots)
    {
      _file6 = TFile::Open(prefix+"harness.root");
      if (!_file6->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }

  TFile * _file7=0;
  if (doAlsoEndcHarnessPlots)
    {
      _file7 = TFile::Open(prefix+"endcHarness.root");
      if (!_file7->IsOpen())
	{
	  std::cout << "File not found. exit" << std::endl; 
	  exit(-1);
	}
    }

  TCanvas *c1=new TCanvas("c1","c1",900,600);
  //   TDatime T0(2012,03,01,00,00,00);
  //   int X0 = T0.Convert();
  //   TDatime T1(2012,09,01,00,00,00);
  //   int X1 = T1.Convert();

  //2011A boundaries
  //  int X0=1300000000+86400*15;
  //  int X1=1315200000;

  //2011B boundaries
  //  int X0=1315200000;
  //  int X1=1320000000;


  
  //2012
  //   int X0=1333620000;
  //   int X1=1333620000+86400*11;


       
  TH2F b("b","b",10,X0,X1,10,axisLower,axisUp);
  b.Draw();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  b.GetXaxis()->SetTitle("Time");
  b.GetXaxis()->SetTimeDisplay(1);
  b.GetXaxis()->SetTimeFormat("%d/%m");


  TH2F balpha("balpha","balpha",10,X0,X1,10,0.7,2.0);
  //  balpha.GetXaxis()->SetTitle("Time");
  balpha.SetStats(false);
  balpha.GetXaxis()->SetTimeDisplay(1);
  balpha.GetXaxis()->SetTimeFormat("%d/%m");
  balpha.GetYaxis()->SetLabelSize(0.09);
  balpha.GetXaxis()->SetLabelSize(0.07);


  TH2F bXtal("bXtal","bXtal",10,X0,X1,10,axisLowerXtal,axisUpXtal);
  bXtal.Draw();
  bXtal.GetXaxis()->SetTitle("Time");
  bXtal.GetXaxis()->SetTimeDisplay(1);
  bXtal.GetXaxis()->SetTimeFormat("%d/%m");

  gStyle->SetOptFit(111111);
  TH2F a("a","a",10,0.945,1.015,10,0.945,1.015); //CAMBIARE

  TH2F c("c","c",10,X0,X1,10,axisLower,axisUp);
  //  TH2F a("a","a",10,0.95,1.03,10,0.95,1.03); 

  HarnessMap* harnessMap_=new HarnessMap(harnessMapFile);

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

  TGraphErrors* lct=(TGraphErrors*)_file0->Get("lc_ieta_1_side_0");
  int npoints=lct->GetN();
  double X[npoints];
  double errorlX[npoints];
  double errorhX[npoints];

  double lcBand[npoints][5];
  double etBand[npoints][5];
  //   double etNoCorrBand[npoints][5];

  TH1F* lcDist[npoints];
  TH1F* etDist[npoints];
  //   TH1F* etNoCorrDist[npoints];

  double quantiles[5]={0.05,0.16,0.5,0.84,0.95};

  double lcBandGraph[5][npoints];
  double etBandGraph[5][npoints];
  //   double etNoCorrBandGraph[5][npoints];

  TH1F ringAlpha("ringAlpha","ringAlpha",240,0.8,2.0);
  TH1F ringAlphaVsEta("ringAlphaVsEta","ringAlphaVsEta",171,-85.5,85.5);
  //   TH1F ringEndcAlpha("ringEndsAlpha","ringEndcAlpha",240,0.8,2.0);
  //   TH1F ringEndcAlphaVsEta("ringEndcAlphaVsEta","ringEndcAlphaVsEta",171,-85.5,85.5);

  TH2F ttMap("ttMap","ttMap",72,0.5,72.5,35,-17.5,17.5);
  TH1F ttAlpha("ttAlpha","ttAlpha",240,0.8,2.0);
  TH2F ttAlphaMap("ttAlphaMap","ttAlphaMap",72,0.5,72.5,35,-17.5,17.5);
  TH1F ttMean("ttMean","ttMean",500,0.95,1.05);
  TH1F ttRMS("ttRMS","ttRMS",1000,0.,0.05);
  TH2F ttMeanMap("ttMeanMap","ttMeanMap",72,0.5,72.5,35,-17.5,17.5);
  TH2F ttRMSMap("ttRMSMap","ttRMSMap",72,0.5,72.5,35,-17.5,17.5);

  TH2F harnessMap("harnessMap","harnessMap",360,0.5,360.5,171,-85.5,85.5);
  TH1F harnessAlpha("harnessAlpha","harnessAlpha",480,0.8,2.0);
  TH2F harnessAlphaMap("harnessAlphaMap","harnessAlphaMap",360,0.5,360.5,171,-85.5,85.5);
  TH1F harnessMean("harnessMean","harnessMean",500,0.95,1.05);
  TH1F harnessRMS("harnessRMS","harnessRMS",1000,0.,0.05);
  TH2F harnessMeanMap("harnessMeanMap","harnessMeanMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F harnessRMSMap("harnessRMSMap","harnessRMSMap",360,0.5,360.5,171,-85.5,85.5);

  TH2F xtalMap("xtalMap","xtalMap",360,0.5,360.5,171,-85.5,85.5);
  TH1F xtalAlpha("xtalAlpha","xtalAlpha",480,0.8,2.0);
  TH2F xtalAlphaMap("xtalAlphaMap","xtalAlphaMap",360,0.5,360.5,171,-85.5,85.5);
  TH1F xtalMean("xtalMean","xtalMean",500,0.95,1.05);
  TH1F xtalRMS("xtalRMS","xtalRMS",1000,0.,0.05);
  TH2F xtalMeanMap("xtalMeanMap","xtalMeanMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalRMSMap("xtalRMSMap","xtalRMSMap",360,0.5,360.5,171,-85.5,85.5);

  TH2F scMap("scMap","scMap",200,0.5,200.5,100,0.5,100.5);
  TH1F scAlpha("scAlpha","scAlpha",240,0.8,2.0);
  TH2F scAlphaMap("scAlphaMap","scAlphaMap",200,0.5,200.5,100,0.5,100.5);
  TH1F scMean("scMean","scMean",500,0.95,1.05);
  TH1F scRMS("scRMS","scRMS",1000,0.,0.05);
  TH2F scMeanMap("scMeanMap","scMeanMap",200,0.5,200.5,100,0.5,100.5);
  TH2F scRMSMap("scRMSMap","scRMSMap",200,0.5,200.5,100,0.5,100.5);

  TH2F endcHarnessMap("endcHarnessMap","endcHarnessMap",200,0.5,200.5,100,0.5,100.5);
  TH1F endcHarnessAlpha("endcHarnessAlpha","endcHarnessAlpha",240,0.8,2.0);
  TH2F endcHarnessAlphaMap("endcHarnessAlphaMap","endcHarnessAlphaMap",200,0.5,200.5,100,0.5,100.5);
  TH1F endcHarnessMean("endcHarnessMean","endcHarnessMean",500,0.95,1.05);
  TH1F endcHarnessRMS("endcHarnessRMS","endcHarnessRMS",1000,0.,0.05);
  TH2F endcHarnessMeanMap("endcHarnessMeanMap","endcHarnessMeanMap",200,0.5,200.5,100,0.5,100.5);
  TH2F endcHarnessRMSMap("endcHarnessRMSMap","endcHarnessRMSMap",200,0.5,200.5,100,0.5,100.5);

  TH2F endcXtalMap("endcXtalMap","endcXtalMap",200,0.5,200.5,100,0.5,100.5);
  TH1F endcXtalAlpha("endcXtalAlpha","endcXtalAlpha",480,0.8,2.0);
  TH2F endcXtalAlphaMap("endcXtalAlphaMap","endcXtalAlphaMap",200,0.5,200.5,100,0.5,100.5);
  TH1F endcXtalMean("endcXtalMean","endcXtalMean",500,0.95,1.05);
  TH1F endcXtalRMS("endcXtalRMS","endcXtalRMS",1000,0.,0.05);
  TH2F endcXtalMeanMap("endcXtalMeanMap","endcXtalMeanMap",200,0.5,200.5,100,0.5,100.5);
  TH2F endcXtalRMSMap("endcXtalRMSMap","endcXtalRMSMap",200,0.5,200.5,100,0.5,100.5);

  TLine* line=new TLine(X0,1,X1,1);

  TF1 *fa1 = new TF1("fa1","pow(x,[0])",0,1.2);
  fa1->SetParameter(0, 1.52);

  for (int ii=0;ii<npoints;++ii)
    {
      TString label="interval_";
      label+=ii;
      X[ii]=*(lct->GetX()+ii);
      errorlX[ii]=400.;
      errorhX[ii]=400.;
      lcDist[ii]=new TH1F("lcDist_"+label,"lcDist_"+label,400,0.9,1.1);
      etDist[ii]=new TH1F("etDist_"+label,"etDist_"+label,400,0.9,1.1);
      //       etNoCorrDist[ii]=new TH1F("etNoCorrDist_"+label,"etNoCorrDist_"+label,200,0.9,1.1);
    }

  if (doRingPlots)
    {
      for (int i=1;i<nRings+1;++i)
	{

	  if (quickTest && i%10 != 1)
	    continue;

	  for (int j=0;j<2;j++){

	    gStyle->SetOptStat(0);
	    TString etaLabel="ieta_";
	    etaLabel+=i;
	    TString sideLabel="_side_";       
	    sideLabel+=j;
	    TString eta="";
	    eta+=i;

	    std::cout <<  "monitor_"+etaLabel+sideLabel << std::endl;
	
	    TGraphErrors* lc=(TGraphErrors*)_file0->Get("lc_"+etaLabel+sideLabel);
	    TGraphErrors* alpha=(TGraphErrors*)_file0->Get("alpha_"+etaLabel+sideLabel);
	    TGraphErrors* et=(TGraphErrors*)_file0->Get("et_"+etaLabel+sideLabel);
	    TGraphErrors* etSumOverRef=(TGraphErrors*)_file0->Get("etSumOverRef_"+etaLabel+sideLabel);
	    // 	TGraphErrors* etNoCorr=(TGraphErrors*)_file0->Get("etNoCorr_"+etaLabel+sideLabel);
	    // 	TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file0->Get("EtNoCorrvsTL_"+etaLabel+sideLabel);
	    TH1F* histoForRMS=new TH1F("histoForRMS","histoForRMS",400,axisLower,axisUp);
	    Double_t * yAxis;


	    for (int ii=0;ii<npoints;++ii)
	      {
		lcDist[ii]->Fill(*(lc->GetY()+ii));
		if (!useEtSumOverEtSumRef)
		  etDist[ii]->Fill(*(et->GetY()+ii));
		else
		  etDist[ii]->Fill(*(etSumOverRef->GetY()+ii));
		// 	    etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	      }
	    // 	EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
	    // 	ringAlpha.Fill(fa1->GetParameter(0));
	    // 	int etaIndex=0;
	    // 	if (j==0)
	    // 	  etaIndex=85-i;
	    // 	else
	    // 	  etaIndex=86+i;
	    // 	ringAlphaVsEta.SetBinContent(etaIndex+1,fa1->GetParameter(0));
	    // 	ringAlphaVsEta.SetBinError(etaIndex+1,fa1->GetParError(0));

	    if (!useEtSumOverEtSumRef)
	      yAxis=(Double_t*)et->GetY();
	    else
	      yAxis=(Double_t*)etSumOverRef->GetY();
	
	    for (int ii=startInterval;ii<npoints;++ii){
	      histoForRMS->Fill(yAxis[ii]);
	    }
	    histoForRMS->SetFillColor(kRed);
	    if (savePlots)
	      {

	    
		lc->SetMarkerColor(1);
		lc->SetMarkerStyle(20);
		lc->SetMarkerSize(0.7);
		lc->SetLineWidth(2);

		alpha->SetMarkerColor(kAzure);
		alpha->SetMarkerStyle(20);
		alpha->SetMarkerSize(0.7);
		alpha->SetLineWidth(2);

		et->SetMarkerColor(kRed);
		et->SetMarkerStyle(20);
		et->SetMarkerSize(0.5);
		etSumOverRef->SetMarkerColor(kRed);
		etSumOverRef->SetMarkerStyle(20);
		etSumOverRef->SetMarkerSize(0.5);
		// 	    etNoCorr->SetMarkerColor(kViolet);
		// 	    etNoCorr->SetMarkerStyle(20);
		// 	    etNoCorr->SetMarkerSize(0.5);


		TCanvas *c_mon = new TCanvas("c_mon","c_mon",1000,600);
		c_mon->cd();
		TPad *pad1 = new TPad("pad1", "monitoring",0.,0.,0.75,1.0);
		pad1->SetMargin(0.1,0.01,0.1,0.1);

		pad1->Draw();
		pad1->cd();

		b.Draw();
		b.SetStats(kFALSE);

		if (!useEtSumOverEtSumRef)
		  et->Draw("PSAME");
		else
		  etSumOverRef->Draw("PSAME");
		// 	    etNoCorr->Draw("PSAME");
		lc->Draw("PSAME");
	    
		line->Draw("same");
	    
		TLegend l(0.55,0.75,0.8,0.88);
	    
		l.SetTextSize(0.033);
		l.SetBorderSize(0);
		l.SetFillColor(0);
	    
		l.AddEntry(lc,"1/laser corr i#eta("+eta+")","P");
		if (!useEtSumOverEtSumRef)
		  l.AddEntry(et,"<et corrected> i#eta("+eta+")","P");
		else
		  l.AddEntry(etSumOverRef,"<et corrected / et corrected ref> i#eta("+eta+")","P");
		//	    l.AddEntry(etNoCorr,"<et uncorrected> i#eta("+eta+")","P");

		l.Draw();

 		TPad *pad1_1 = new TPad("pad1_1", "monitoring",0.12,0.11,0.55,0.46);
		pad1_1->SetMargin(0.1,0.01,0.15,0.1);
		pad1_1->Draw();
 		pad1_1->cd();
		TLegend l1(0.55,0.75,0.8,0.88);
		l1.SetTextSize(0.09);
		l1.SetBorderSize(0);
		l1.SetFillColor(0);
	    

  		balpha.Draw("");
		balpha.SetStats(false);
  		alpha->Draw("psame");
		l1.AddEntry(alpha,"#alpha i#eta("+eta+")","P");
		l1.Draw();
		c_mon->cd();

		gStyle->SetOptStat("mr");
		TPad *pad2 = new TPad("pad2", "rms",0.76,0.,1.,1.);
		//            pad2->SetMargin(0.1,0,0.1,0.14);
		pad2->Draw();
		pad2->cd();	    
		histoForRMS->SetStats(kTRUE);
		histoForRMS->Draw("HBAR");

		c_mon->cd();

		c_mon->SaveAs("plots/monitor_"+etaLabel+sideLabel+suffix);
		histoForRMS->Reset();

		//	l.Delete();
	    
		a.Draw();
		a.GetYaxis()->SetTitle("");
		a.GetXaxis()->SetTitle("1/<lc>");
		// 	    EtNoCorrvsTL->SetMarkerColor(kViolet);
		// 	    EtNoCorrvsTL->SetMarkerStyle(20);
		// 	    EtNoCorrvsTL->SetMarkerSize(0.5);
	    
		// 	    EtNoCorrvsTL->Draw("PESAME");

		// 	    c1->SaveAs("plots/EtNoCorrvsTL"+etaLabel+sideLabel+suffix);
		delete c_mon;
		// 	    delete pad1;
		// delete pad2;
	      }
	    delete histoForRMS;

	  }

	}

      gStyle->SetOptStat(1111);
      ringAlpha.GetXaxis()->SetTitle("alpha correction");
      ringAlpha.Draw();
      ringAlpha.SaveAs("plots/ringAlpha.root");
      //   if (savePlots)
      c1->SaveAs("plots/ringAlpha"+suffix);

      gStyle->SetOptStat(0);
      ringAlphaVsEta.GetXaxis()->SetTitle("ring index");
      ringAlphaVsEta.GetYaxis()->SetTitle("alpha correction");
      ringAlphaVsEta.SetMaximum(ringAlphaVsEta.GetBinContent(ringAlphaVsEta.GetMaximumBin())*1.2);
      ringAlphaVsEta.SetMinimum(0.8);
      ringAlphaVsEta.SetMarkerStyle(20);
      ringAlphaVsEta.SetMarkerSize(0.7);
      ringAlphaVsEta.Draw("E");
      ringAlphaVsEta.SaveAs("plots/ringAlphaVsEta.root");
      //   if (savePlots)
      c1->SaveAs("plots/ringAlphaVsEta"+suffix);



      for (int ii=0;ii<npoints;++ii)
	{
	  lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	  etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	  //       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	  for (int ij=0;ij<5;++ij)
	    {
	      if (ij!=2)
		{
		  lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		  etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		  // 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
		}
	      else
		{
		  std::cout << etBand[ii][ij] << std::endl;
		  lcBandGraph[ij][ii]=lcBand[ii][ij];
		  etBandGraph[ij][ii]=etBand[ii][ij];
		  // 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
		}
	  
	    } 
	} 
      
  
      TGraphAsymmErrors * lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
      TGraphAsymmErrors * lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

      TGraphAsymmErrors * et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
      TGraphAsymmErrors * et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

      //   TGraphAsymmErrors * etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
      //   TGraphAsymmErrors * etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);


      c.Draw();
      c.GetXaxis()->SetTitle("Time");
      c.GetXaxis()->SetTimeDisplay(1);
      c.GetXaxis()->SetTimeFormat("%d/%m");


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
      //  if (savePlots)
      c1->SaveAs("plots/lcGraph"+suffix);


      c.Draw();
      et95Graph->SetFillColor(kYellow);
      et95Graph->SetFillStyle(1001);
      et95Graph->Draw("2same");

      et68Graph->SetFillColor(kGreen);
      et68Graph->SetFillStyle(1001);
      et68Graph->SetMarkerColor(kBlack);
      et68Graph->SetMarkerStyle(20);
      et68Graph->SetMarkerSize(0.4);
      et68Graph->Draw("2same");
      et68Graph->Draw("pxsame");

      //  if (savePlots)
      c1->SaveAs("plots/etGraph"+suffix);


      //   c.Draw();
      //   etNoCorr95Graph->SetFillColor(kYellow);
      //   etNoCorr95Graph->SetFillStyle(1001);
      //   etNoCorr95Graph->Draw("2same");

      //   etNoCorr68Graph->SetFillColor(kGreen);
      //   etNoCorr68Graph->SetFillStyle(1001);
      //   etNoCorr68Graph->SetMarkerColor(kBlack);
      //   etNoCorr68Graph->SetMarkerStyle(20);
      //   etNoCorr68Graph->SetMarkerSize(0.4);
      //   etNoCorr68Graph->Draw("2same");
      //   etNoCorr68Graph->Draw("pxsame");

      //   if (savePlots)
      //     c1->SaveAs("plots/etNoCorrGraph"+suffix);


      c.Draw();
      lc95Graph->SetFillColor(kYellow+1);
      lc95Graph->SetFillStyle(1001);
      lc95Graph->Draw("2same");

      lc68Graph->SetFillColor(kGreen+3);
      lc68Graph->SetFillStyle(1001);
      lc68Graph->SetMarkerColor(kBlack);
      lc68Graph->SetMarkerStyle(20);
      lc68Graph->SetMarkerSize(0.4);
      lc68Graph->Draw("2same");
      lc68Graph->Draw("pxsame");

      et95Graph->SetFillColor(kYellow);
      et95Graph->SetFillStyle(1001);
      et95Graph->Draw("2same");

      et68Graph->SetFillColor(kGreen);
      et68Graph->SetFillStyle(1001);
      et68Graph->SetMarkerColor(kRed);
      et68Graph->SetMarkerStyle(20);
      et68Graph->SetMarkerSize(0.4);
      et68Graph->Draw("2same");
      et68Graph->Draw("pxsame");

      //   etNoCorr68Graph->SetMarkerColor(kViolet);
      //   etNoCorr68Graph->SetMarkerStyle(20);
      //   etNoCorr68Graph->SetMarkerSize(0.4);
      //   etNoCorr68Graph->Draw("pxsame");

      line->Draw("same");
      //  if (savePlots)
      c1->SaveAs("plots/fullHistory"+suffix);

    }
     
  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      //       etNoCorrDist[ii]->Reset();
    }
  
  if (doAlsoEndcRingPlots)
    {
      for (int i=1;i<nEndcRings+1;++i)
	{
	  if (quickTest && i%10 != 1)
	    continue;
	  for (int j=0;j<2;j++){
	    gStyle->SetOptStat(0);
	    TString etaLabel="iring_";
	    etaLabel+=i;
	    TString sideLabel="_side_";       
	    sideLabel+=j;
	    TString eta="";
	    eta+=i;
	    
	    std::cout <<  "monitor_"+etaLabel+sideLabel << std::endl;
	    
	    TGraphErrors* lc=(TGraphErrors*)_file3->Get("lc_"+etaLabel+sideLabel);
	    TGraphErrors* alpha=(TGraphErrors*)_file3->Get("alpha_"+etaLabel+sideLabel);
	    TGraphErrors* et=(TGraphErrors*)_file3->Get("et_"+etaLabel+sideLabel);
	    TGraphErrors* etSumOverRef=(TGraphErrors*)_file3->Get("etSumOverRef_"+etaLabel+sideLabel);
	    // 	TGraphErrors* etNoCorr=(TGraphErrors*)_file3->Get("etNoCorr_"+etaLabel+sideLabel);
	    // 	TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file3->Get("EtNoCorrvsTL_"+etaLabel+sideLabel);
	    TH1F* histoForRMS=new TH1F("histoForRMS","histoForRMS",400,axisLower,axisUp);
	    Double_t * yAxis;


	    for (int ii=0;ii<npoints;++ii)
	      {
		lcDist[ii]->Fill(*(lc->GetY()+ii));
		if (!useEtSumOverEtSumRef)
		  etDist[ii]->Fill(*(et->GetY()+ii));
		else
		  etDist[ii]->Fill(*(etSumOverRef->GetY()+ii));
		// 	    etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	      }
	    // 	EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
	    // 	ringAlpha.Fill(fa1->GetParameter(0));
	    // 	int etaIndex=0;
	    // 	if (j==0)
	    // 	  etaIndex=85-i;
	    // 	else
	    // 	  etaIndex=86+i;
	    // 	ringAlphaVsEta.SetBinContent(etaIndex+1,fa1->GetParameter(0));
	    // 	ringAlphaVsEta.SetBinError(etaIndex+1,fa1->GetParError(0));
	    
	    if (!useEtSumOverEtSumRef)
	      yAxis=(Double_t*)et->GetY();
	    else
	      yAxis=(Double_t*)etSumOverRef->GetY();
	    
	    for (int ii=startInterval;ii<npoints;++ii){
	      histoForRMS->Fill(yAxis[ii]);
	    }
	    histoForRMS->SetFillColor(kRed);
	    if (savePlots)
	      {
		
		
		lc->SetMarkerColor(1);
		lc->SetMarkerStyle(20);
		lc->SetMarkerSize(0.7);
		lc->SetLineWidth(2);
		alpha->SetMarkerColor(kAzure);
		alpha->SetMarkerStyle(20);
		alpha->SetMarkerSize(0.7);
		alpha->SetLineWidth(2);
		et->SetMarkerColor(kRed);
		et->SetMarkerStyle(20);
		et->SetMarkerSize(0.5);
		etSumOverRef->SetMarkerColor(kRed);
		etSumOverRef->SetMarkerStyle(20);
		etSumOverRef->SetMarkerSize(0.5);
		// 	    etNoCorr->SetMarkerColor(kViolet);
		// 	    etNoCorr->SetMarkerStyle(20);
		// 	    etNoCorr->SetMarkerSize(0.5);
		
		
		TCanvas *c_mon = new TCanvas("c_mon","c_mon",1000,500);
		c_mon->cd();
		TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
		pad1->SetMargin(0.1,0.01,0.1,0.1);
		pad1->Draw();
		pad1->cd();
		
		b.Draw();
		b.SetStats(kFALSE);
		
		if (!useEtSumOverEtSumRef)
		  et->Draw("PSAME");
		else
		  etSumOverRef->Draw("PSAME");
		// 	    etNoCorr->Draw("PSAME");
		lc->Draw("PSAME");
		
		line->Draw("same");
		
		TLegend l(0.55,0.75,0.8,0.88);
		
		l.SetTextSize(0.033);
		l.SetBorderSize(0);
		l.SetFillColor(0);
		
		l.AddEntry(lc,"1/laser corr iring("+eta+")","P");
		if (!useEtSumOverEtSumRef)
		  l.AddEntry(et,"<et corrected> iring("+eta+")","P");
		else
		  l.AddEntry(etSumOverRef,"<et corrected / et corrected ref> iring("+eta+")","P");
		//	    l.AddEntry(etNoCorr,"<et uncorrected> i#eta("+eta+")","P");
		l.Draw();

 		TPad *pad1_1 = new TPad("pad1_1", "monitoring",0.12,0.11,0.55,0.46);
		pad1_1->SetMargin(0.1,0.01,0.15,0.1);
		pad1_1->Draw();
 		pad1_1->cd();
		TLegend l1(0.55,0.75,0.8,0.88);
		l1.SetTextSize(0.09);
		l1.SetBorderSize(0);
		l1.SetFillColor(0);
	    
  		balpha.Draw("");
		balpha.SetStats(false);
  		alpha->Draw("psame");
		l1.AddEntry(alpha,"#alpha i#eta("+eta+")","P");
		l1.Draw();
		
		c_mon->cd();
		gStyle->SetOptStat("mr");
		TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
		//            pad2->SetMargin(0.1,0,0.1,0.14);
		pad2->Draw();
		pad2->cd();	    
		histoForRMS->SetStats(kTRUE);
		histoForRMS->Draw("HBAR");
		
		c_mon->cd();
		
		c_mon->SaveAs("plots/monitor_"+etaLabel+sideLabel+suffix);
		histoForRMS->Reset();
		
		//	l.Delete();
		
		a.Draw();
		a.GetYaxis()->SetTitle("");
		a.GetXaxis()->SetTitle("1/<lc>");
		// 	    EtNoCorrvsTL->SetMarkerColor(kViolet);
		// 	    EtNoCorrvsTL->SetMarkerStyle(20);
		// 	    EtNoCorrvsTL->SetMarkerSize(0.5);
		
		// 	    EtNoCorrvsTL->Draw("PESAME");
		
		// 	    c1->SaveAs("plots/EtNoCorrvsTL"+etaLabel+sideLabel+suffix);
		delete c_mon;
		// 	    delete pad1;
		// delete pad2;
	      }
	    delete histoForRMS;
	    
	  }

	}

      //       gStyle->SetOptStat(1111);
      //       ringAlpha.GetXaxis()->SetTitle("alpha correction");
      //       ringAlpha.Draw();
      //       ringAlpha.SaveAs("plots/ringEndcAlpha.root");
      //       //   if (savePlots)
      //       c1->SaveAs("plots/ringEndcAlpha"+suffix);

      //   gStyle->SetOptStat(0);
      //   ringAlphaVsEta.GetXaxis()->SetTitle("ring index");
      //   ringAlphaVsEta.GetYaxis()->SetTitle("alpha correction");
      //   ringAlphaVsEta.SetMaximum(ringAlphaVsEta.GetBinContent(ringAlphaVsEta.GetMaximumBin())*1.2);
      //   ringAlphaVsEta.SetMinimum(0.8);
      //   ringAlphaVsEta.SetMarkerStyle(20);
      //   ringAlphaVsEta.SetMarkerSize(0.7);
      //   ringAlphaVsEta.Draw("E");
      //   ringAlphaVsEta.SaveAs("plots/ringAlphaVsEta.root");
      // //   if (savePlots)
      //     c1->SaveAs("plots/ringAlphaVsEta"+suffix);



      for (int ii=0;ii<npoints;++ii)
	{
	  lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	  etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	  //       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	  for (int ij=0;ij<5;++ij)
	    {
	      if (ij!=2)
		{
		  lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		  etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		  // 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
		}
	      else
		{
		  std::cout << etBand[ii][ij] << std::endl;
		  lcBandGraph[ij][ii]=lcBand[ii][ij];
		  etBandGraph[ij][ii]=etBand[ii][ij];
		  // 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
		}
	      
	    } 
	} 
      
      
      TGraphAsymmErrors * lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
      TGraphAsymmErrors * lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);
      
      TGraphAsymmErrors * et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
      TGraphAsymmErrors * et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);
      
      //   TGraphAsymmErrors * etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
      //   TGraphAsymmErrors * etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);
      
      
      c.Draw();
      c.GetXaxis()->SetTitle("Time");
      c.GetXaxis()->SetTimeDisplay(1);
      c.GetXaxis()->SetTimeFormat("%d/%m");
      
      
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
      //  if (savePlots)
      c1->SaveAs("plots/lcEndcGraph"+suffix);


      c.Draw();
      et95Graph->SetFillColor(kYellow);
      et95Graph->SetFillStyle(1001);
      et95Graph->Draw("2same");
      
      et68Graph->SetFillColor(kGreen);
      et68Graph->SetFillStyle(1001);
      et68Graph->SetMarkerColor(kBlack);
      et68Graph->SetMarkerStyle(20);
      et68Graph->SetMarkerSize(0.4);
      et68Graph->Draw("2same");
      et68Graph->Draw("pxsame");

      //  if (savePlots)
      c1->SaveAs("plots/etEndcGraph"+suffix);


      //   c.Draw();
      //   etNoCorr95Graph->SetFillColor(kYellow);
      //   etNoCorr95Graph->SetFillStyle(1001);
      //   etNoCorr95Graph->Draw("2same");
      
      //   etNoCorr68Graph->SetFillColor(kGreen);
      //   etNoCorr68Graph->SetFillStyle(1001);
      //   etNoCorr68Graph->SetMarkerColor(kBlack);
      //   etNoCorr68Graph->SetMarkerStyle(20);
      //   etNoCorr68Graph->SetMarkerSize(0.4);
      //   etNoCorr68Graph->Draw("2same");
      //   etNoCorr68Graph->Draw("pxsame");

      //   if (savePlots)
      //     c1->SaveAs("plots/etNoCorrGraph"+suffix);


      c.Draw();
      lc95Graph->SetFillColor(kYellow+1);
      lc95Graph->SetFillStyle(1001);
      lc95Graph->Draw("2same");

      lc68Graph->SetFillColor(kGreen+3);
      lc68Graph->SetFillStyle(1001);
      lc68Graph->SetMarkerColor(kBlack);
      lc68Graph->SetMarkerStyle(20);
      lc68Graph->SetMarkerSize(0.4);
      lc68Graph->Draw("2same");
      lc68Graph->Draw("pxsame");

      et95Graph->SetFillColor(kYellow);
      et95Graph->SetFillStyle(1001);
      et95Graph->Draw("2same");

      et68Graph->SetFillColor(kGreen);
      et68Graph->SetFillStyle(1001);
      et68Graph->SetMarkerColor(kRed);
      et68Graph->SetMarkerStyle(20);
      et68Graph->SetMarkerSize(0.4);
      et68Graph->Draw("2same");
      et68Graph->Draw("pxsame");

      //   etNoCorr68Graph->SetMarkerColor(kViolet);
      //   etNoCorr68Graph->SetMarkerStyle(20);
      //   etNoCorr68Graph->SetMarkerSize(0.4);
      //   etNoCorr68Graph->Draw("pxsame");

      line->Draw("same");
      //  if (savePlots)
      c1->SaveAs("plots/fullHistoryEndc"+suffix);
    }

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      //       etNoCorrDist[ii]->Reset();
    }

  if(doAlsoTTPlots){
    for (int i=1;i<=nTowers;++i)
      {
	if (quickTest && i%10 != 1)
	  continue;
	TString ittLabel="itt_";
	ittLabel+=i;
	TString itt="";
	itt+=i;

	TGraphErrors* lc=(TGraphErrors*)_file1->Get("lc_"+ittLabel);
	TGraphErrors* et=(TGraphErrors*)_file1->Get("et_"+ittLabel);
	TGraphErrors* etSumOverRef=(TGraphErrors*)_file1->Get("etSumOverRef_"+ittLabel);
	//        TGraphErrors* etNoCorr=(TGraphErrors*)_file1->Get("etNoCorr_"+ittLabel);
	//        TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file1->Get("EtNoCorrvsTL_"+ittLabel);
	TH1F* histoForRMSTT=new TH1F("histoForRMSTT","histoForRMSTT",200,axisLower,axisUp);
	Double_t * yAxisTT;

	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lc->GetY()+ii));
	    if (!useEtSumOverEtSumRef)
	      etDist[ii]->Fill(*(et->GetY()+ii));
	    else
	      etDist[ii]->Fill(*(etSumOverRef->GetY()+ii));
	    // 	   etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	  }

	//        EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
	ttAlpha.Fill(fa1->GetParameter(0));
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
	std::cout <<  "monitor_"+ittLabel << std::endl;
	std::cout << "i " << " " << smIndex << " " << ttIndex << " " << etaIndex << " " << phiIndex << std::endl;
	ttAlphaMap.SetBinContent(phiIndex,etaIndex,fa1->GetParameter(0));
	ttAlphaMap.SetBinError(phiIndex,etaIndex,fa1->GetParError(0));

	if (!useEtSumOverEtSumRef)
	  yAxisTT=(Double_t*)et->GetY();
	else
	  yAxisTT=(Double_t*)etSumOverRef->GetY();
       
	for (int ii=startInterval;ii<npoints;++ii){
	  histoForRMSTT->Fill(yAxisTT[ii]);
	}
       
	histoForRMSTT->SetFillColor(kRed);
       
	ttMean.Fill(histoForRMSTT->GetMean());
	ttRMS.Fill(histoForRMSTT->GetRMS());
	ttMap.SetBinContent(phiIndex,etaIndex,i);
	ttMeanMap.SetBinContent(phiIndex,etaIndex,histoForRMSTT->GetMean());
	ttMeanMap.SetBinError(phiIndex,etaIndex,histoForRMSTT->GetMeanError());
	ttRMSMap.SetBinContent(phiIndex,etaIndex,histoForRMSTT->GetRMS());
	ttRMSMap.SetBinError(phiIndex,etaIndex,histoForRMSTT->GetRMSError());
       
	if ( ( savePlots || histoForRMSTT->GetRMS() > ttRMSThreshold || histoForRMSTT->GetMean() > ttMeanHighThreshold || histoForRMSTT->GetMean() < ttMeanLowThreshold ) && (histoForRMSTT->GetRMS() > 0 ||  histoForRMSTT->GetMean() > 0 ) )
	  {

	    lc->SetMarkerColor(1);
	    lc->SetMarkerStyle(20);
	    lc->SetMarkerSize(0.7);
	    lc->SetLineWidth(2);

	    et->SetMarkerColor(kRed);
	    et->SetMarkerStyle(20);
	    et->SetMarkerSize(0.5);

	    etSumOverRef->SetMarkerColor(kRed);
	    etSumOverRef->SetMarkerStyle(20);
	    etSumOverRef->SetMarkerSize(0.5);
	    // 	   etNoCorr->SetMarkerColor(kViolet);
	    // 	   etNoCorr->SetMarkerStyle(20);
	    // 	   etNoCorr->SetMarkerSize(0.5);


	    TCanvas *c_monTT = new TCanvas("c_monTT","c_monTT",1000,500);
	    c_monTT->cd();
	    cout<<"ok"<<endl;
	    TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
	    pad1->SetMargin(0.1,0.01,0.1,0.1);
	    //	   pad1->SetMargin(0.1,0.01,0.1,0.14);
	    pad1->Draw();
	    pad1->cd();
	    b.Draw();
	    b.SetStats(kFALSE);





	    if (!useEtSumOverEtSumRef)
	      et->Draw("PSAME");
	    else
	      etSumOverRef->Draw("PSAME");
	    // 	   etNoCorr->Draw("PSAME");
	    lc->Draw("PSAME");
	   
	    line->Draw("same");
	    TLegend l(0.45,0.75,0.91,0.88);
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    l.AddEntry(lc,"1/laser corr itt("+itt+")","P");
	    if (!useEtSumOverEtSumRef)
	      l.AddEntry(et,"<et corrected> itt("+itt+")","P");
	    else
	      l.AddEntry(etSumOverRef,"<et corrected/ et ref> itt("+itt+")","P");
	    // 	   l.AddEntry(etNoCorr,"<et uncorrected> itt("+itt+")","P");
	    l.Draw();

	    c_monTT->cd();

	    gStyle->SetOptStat("mr");
	    TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
	    //           pad2->SetMargin(0.1,0,0.1,0.14);
	    pad2->Draw();
	    pad2->cd();
	    histoForRMSTT->SetStats(kTRUE);
	    histoForRMSTT->Draw("HBAR");


	    c_monTT->cd();

	    c_monTT->SaveAs("plots/monitor_"+ittLabel+suffix);
	    histoForRMSTT->Reset();
	    a.Draw();
	    a.GetYaxis()->SetTitle();
	    a.GetXaxis()->SetTitle("1/<lc>");
	    // 	   EtNoCorrvsTL->SetMarkerColor(kViolet);
	    // 	   EtNoCorrvsTL->SetMarkerStyle(20);
	    // 	   EtNoCorrvsTL->SetMarkerSize(0.5);
	   
	    // 	   EtNoCorrvsTL->Draw("PESAME");
	    // 	   c1->SaveAs("plots/EtNoCorrvsTL"+ittLabel+suffix);
	    delete c_monTT;
	    // 	   delete pad1;
	    // 	   delete pad2;
	  }
	delete histoForRMSTT;

      }

    gPad->SetMargin(0.1,0.16,0.12,0.1);
    gStyle->SetOptStat(1111);
    ttAlpha.GetXaxis()->SetTitle("alpha correction");
    ttAlpha.SetMaximum(ttAlpha.GetMaximum()*1.3);
    ttAlpha.Draw();
    ttAlpha.SaveAs("plots/ttAlpha.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttAlpha"+suffix);

    gStyle->SetOptStat(0);
    ttAlphaMap.GetXaxis()->SetTitle("tt phi index");
    ttAlphaMap.GetYaxis()->SetTitle("tt eta index");
    ttAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
    ttAlphaMap.Draw("COLZ");
    ttAlphaMap.SaveAs("plots/ttAlphaMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttAlphaMap"+suffix);

    gStyle->SetOptStat(1111);
    ttMean.GetXaxis()->SetTitle("Mean of normalized tt response");
    ttMean.SetMaximum(ttMean.GetMaximum()*1.3);
    ttMean.Draw();
    ttMean.SaveAs("plots/ttMean.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttMean"+suffix);

    gStyle->SetOptStat(0);
    ttMeanMap.GetXaxis()->SetTitle("tt phi index");
    ttMeanMap.GetYaxis()->SetTitle("tt eta index");
    ttMeanMap.GetZaxis()->SetRangeUser(ttMean.GetMean()-5*ttMean.GetRMS(),ttMean.GetMean()+5*ttMean.GetRMS());
    ttMeanMap.Draw("COLZ");
    ttMeanMap.SaveAs("plots/ttMeanMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttMeanMap"+suffix);

    gStyle->SetOptStat(0);
    ttMap.GetXaxis()->SetTitle("tt phi index");
    ttMap.GetYaxis()->SetTitle("tt eta index");
    //    ttMap.GetZaxis()->SetRangeUser(tt.GetMean()-5*tt.GetRMS(),tt.GetMean()+5*tt.GetRMS());
    ttMap.Draw("COLZ");
    ttMap.SaveAs("plots/ttMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttMap"+suffix);

    gStyle->SetOptStat(1111);
    ttRMS.GetXaxis()->SetTitle("RMS of normalized tt response");
    ttRMS.SetMaximum(ttRMS.GetMaximum()*1.3);
    ttRMS.Draw();
    ttRMS.SaveAs("plots/ttRMS.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttRMS"+suffix);

    gStyle->SetOptStat(0);
    ttRMSMap.GetXaxis()->SetTitle("tt phi index");
    ttRMSMap.GetYaxis()->SetTitle("tt eta index");
    float rms_mean=ttRMS.GetMean(); 
    float rms_rms=ttRMS.GetRMS();
    ttRMSMap.GetZaxis()->SetRangeUser(TMath::Max((float)0.,(float)rms_mean-3*rms_rms), rms_mean+3*rms_rms);
    ttRMSMap.Draw("COLZ");
    ttRMSMap.SaveAs("plots/ttRMSMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/ttRMSMap"+suffix);

    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	//       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		// 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
		etBandGraph[ij][ii]=etBand[ii][ij];
		// 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	      }
	  
	  } 
      } 
      
  
    TGraphAsymmErrors* lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors* lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TGraphAsymmErrors* et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
    TGraphAsymmErrors* et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

    //   etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
    //   etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d/%m");


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

    //   if (savePlots)
    c1->SaveAs("plots/lcGraph_itt"+suffix);


    c.Draw();
    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kBlack);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   if (savePlots)
    c1->SaveAs("plots/etGraph_itt"+suffix);


    c.Draw();
    //   etNoCorr95Graph->SetFillColor(kYellow);
    //   etNoCorr95Graph->SetFillStyle(1001);
    //   etNoCorr95Graph->Draw("2same");

    //   etNoCorr68Graph->SetFillColor(kGreen);
    //   etNoCorr68Graph->SetFillStyle(1001);
    //   etNoCorr68Graph->SetMarkerColor(kBlack);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("2same");
    //   etNoCorr68Graph->Draw("pxsame");

    // //   if (savePlots)
    //     c1->SaveAs("plots/etNoCorrGraph_itt"+suffix);


    c.Draw();
    lc95Graph->SetFillColor(kYellow+1);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen+3);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");

    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kRed);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   etNoCorr68Graph->SetMarkerColor(kViolet);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("pxsame");

    line->Draw("same");

    //   if (savePlots)
    c1->SaveAs("plots/fullHistory_itt"+suffix);
  }


  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      //       etNoCorrDist[ii]->Reset();
    }

  if(doAlsoHarnessPlots){
    for (int i=1;i<=nHarness_EB;++i)
      {
	if (quickTest && i%4 != 1)
	  continue;
	TString harnessFileLabel="harness_eb_";
	harnessFileLabel+=(i<9*18+1)? -((i-1)/9+1) : (i-1)/9-17 ;
	harnessFileLabel+="_";
	harnessFileLabel+=((i-1)%9) + 1;
	TString harnessLabel="harness_";
	harnessLabel+=i;
	TString harness="";
	harness+=i;

	TGraphErrors* lc=(TGraphErrors*)_file6->Get("lc_"+harnessLabel);
	TGraphErrors* et=(TGraphErrors*)_file6->Get("et_"+harnessLabel);
	TGraphErrors* etSumOverRef=(TGraphErrors*)_file6->Get("etSumOverRef_"+harnessLabel);
	//        TGraphErrors* etNoCorr=(TGraphErrors*)_file6->Get("etNoCorr_"+harnessLabel);
	//        TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file6->Get("EtNoCorrvsTL_"+harnessLabel);
	TH1F* histoForRMSHARNESS=new TH1F("histoForRMSHARNESS","histoForRMSHARNESS",200,axisLower,axisUp);
	Double_t * yAxisHARNESS;

	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lc->GetY()+ii));
	    if (!useEtSumOverEtSumRef)
	      etDist[ii]->Fill(*(et->GetY()+ii));
	    else
	      etDist[ii]->Fill(*(etSumOverRef->GetY()+ii));
	    // 	   etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	  }

	//        EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
	// 	harnessAlpha.Fill(fa1->GetParameter(0));
	
	// 	harnessAlphaMap.SetBinContent(phiIndex,etaIndex,fa1->GetParameter(0));
	// 	harnessAlphaMap.SetBinError(phiIndex,etaIndex,fa1->GetParError(0));

	if (!useEtSumOverEtSumRef)
	  yAxisHARNESS=(Double_t*)et->GetY();
	else
	  yAxisHARNESS=(Double_t*)etSumOverRef->GetY();
       
	for (int ii=startInterval;ii<npoints;++ii){
	  histoForRMSHARNESS->Fill(yAxisHARNESS[ii]);
	}
       
	histoForRMSHARNESS->SetFillColor(kRed);
	harnessMean.Fill(histoForRMSHARNESS->GetMean());
	harnessRMS.Fill(histoForRMSHARNESS->GetRMS());

	std::vector<HarnessMap::channel> myChannels=harnessMap_->channelsInHarness(0,i-1);
	  for (unsigned int ic=0;ic<myChannels.size();++ic)
	    {
	      HarnessMap::channel channel=myChannels[ic];
	      int phiIndex=channel.iphi;
	      int etaIndex=channel.ieta+86;
	      harnessMap.SetBinContent(phiIndex,etaIndex,i);
	      harnessMeanMap.SetBinContent(phiIndex,etaIndex,histoForRMSHARNESS->GetMean());
	      harnessMeanMap.SetBinError(phiIndex,etaIndex,histoForRMSHARNESS->GetMeanError());
	      harnessRMSMap.SetBinContent(phiIndex,etaIndex,histoForRMSHARNESS->GetRMS());
	      harnessRMSMap.SetBinError(phiIndex,etaIndex,histoForRMSHARNESS->GetRMSError());
	    }

	if ( ( savePlots || histoForRMSHARNESS->GetRMS() > harnessRMSThreshold || histoForRMSHARNESS->GetMean() > harnessMeanHighThreshold || histoForRMSHARNESS->GetMean() < harnessMeanLowThreshold ) && (histoForRMSHARNESS->GetRMS() > 0 ||  histoForRMSHARNESS->GetMean() > 0 ) )
	  {

	    lc->SetMarkerColor(1);
	    lc->SetMarkerStyle(20);
	    lc->SetMarkerSize(0.7);
	    lc->SetLineWidth(2);

	    et->SetMarkerColor(kRed);
	    et->SetMarkerStyle(20);
	    et->SetMarkerSize(0.5);

	    etSumOverRef->SetMarkerColor(kRed);
	    etSumOverRef->SetMarkerStyle(20);
	    etSumOverRef->SetMarkerSize(0.5);
	    // 	   etNoCorr->SetMarkerColor(kViolet);
	    // 	   etNoCorr->SetMarkerStyle(20);
	    // 	   etNoCorr->SetMarkerSize(0.5);


	    TCanvas *c_monHARNESS = new TCanvas("c_monHARNESS","c_monHARNESS",1000,500);
	    c_monHARNESS->cd();
	    cout<<"ok"<<endl;
	    TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
	    pad1->SetMargin(0.1,0.01,0.1,0.1);
	    //	   pad1->SetMargin(0.1,0.01,0.1,0.14);
	    pad1->Draw();
	    pad1->cd();
	    b.Draw();
	    b.SetStats(kFALSE);


	    if (!useEtSumOverEtSumRef)
	      et->Draw("PSAME");
	    else
	      etSumOverRef->Draw("PSAME");
	    // 	   etNoCorr->Draw("PSAME");
	    lc->Draw("PSAME");
	   
	    line->Draw("same");
	    TLegend l(0.45,0.75,0.91,0.88);
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    l.AddEntry(lc,"1/laser corr harness("+harness+")","P");
	    if (!useEtSumOverEtSumRef)
	      l.AddEntry(et,"<et corrected> harness("+harness+")","P");
	    else
	      l.AddEntry(etSumOverRef,"<et corrected/ et ref> harness("+harness+")","P");
	    // 	   l.AddEntry(etNoCorr,"<et uncorrected> harness("+harness+")","P");
	    l.Draw();

	    c_monHARNESS->cd();

	    gStyle->SetOptStat("mr");
	    TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
	    //           pad2->SetMargin(0.1,0,0.1,0.14);
	    pad2->Draw();
	    pad2->cd();
	    histoForRMSHARNESS->SetStats(kTRUE);
	    histoForRMSHARNESS->Draw("HBAR");


	    c_monHARNESS->cd();

	    c_monHARNESS->SaveAs("plots/monitor_"+harnessFileLabel+suffix);
	    histoForRMSHARNESS->Reset();
	    a.Draw();
	    a.GetYaxis()->SetTitle();
	    a.GetXaxis()->SetTitle("1/<lc>");
	    // 	   EtNoCorrvsTL->SetMarkerColor(kViolet);
	    // 	   EtNoCorrvsTL->SetMarkerStyle(20);
	    // 	   EtNoCorrvsTL->SetMarkerSize(0.5);
	   
	    // 	   EtNoCorrvsTL->Draw("PESAME");
	    // 	   c1->SaveAs("plots/EtNoCorrvsTL"+harnessLabel+suffix);
	    delete c_monHARNESS;
	    // 	   delete pad1;
	    // 	   delete pad2;
	  }
	delete histoForRMSHARNESS;

      }

    gPad->SetMargin(0.1,0.16,0.12,0.1);
    gStyle->SetOptStat(1111);
    harnessAlpha.GetXaxis()->SetTitle("alpha correction");
    harnessAlpha.SetMaximum(harnessAlpha.GetMaximum()*1.3);
    harnessAlpha.Draw();
    harnessAlpha.SaveAs("plots/harnessAlpha.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessAlpha"+suffix);

    gStyle->SetOptStat(0);
    harnessAlphaMap.GetXaxis()->SetTitle("harness phi index");
    harnessAlphaMap.GetYaxis()->SetTitle("harness eta index");
    harnessAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
    harnessAlphaMap.Draw("COLZ");
    harnessAlphaMap.SaveAs("plots/harnessAlphaMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessAlphaMap"+suffix);

    gStyle->SetOptStat(1111);
    harnessMean.GetXaxis()->SetTitle("Mean of normalized harness response");
    harnessMean.SetMaximum(harnessMean.GetMaximum()*1.3);
    harnessMean.Draw();
    harnessMean.SaveAs("plots/harnessMean.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessMean"+suffix);

    gStyle->SetOptStat(0);
    harnessMeanMap.GetXaxis()->SetTitle("harness phi index");
    harnessMeanMap.GetYaxis()->SetTitle("harness eta index");
    harnessMeanMap.GetZaxis()->SetRangeUser(harnessMean.GetMean()-5*harnessMean.GetRMS(),harnessMean.GetMean()+5*harnessMean.GetRMS());
    harnessMeanMap.Draw("COLZ");
    harnessMeanMap.SaveAs("plots/harnessMeanMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessMeanMap"+suffix);

    gStyle->SetOptStat(0);
    harnessMap.GetXaxis()->SetTitle("harness phi index");
    harnessMap.GetYaxis()->SetTitle("harness eta index");
    //    harnessMap.GetZaxis()->SetRangeUser(harness.GetMean()-5*harness.GetRMS(),harness.GetMean()+5*harness.GetRMS());
    harnessMap.Draw("COLZ");
    harnessMap.SaveAs("plots/harnessMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessMap"+suffix);

    gStyle->SetOptStat(1111);
    harnessRMS.GetXaxis()->SetTitle("RMS of normalized harness response");
    harnessRMS.SetMaximum(harnessRMS.GetMaximum()*1.3);
    harnessRMS.Draw();
    harnessRMS.SaveAs("plots/harnessRMS.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessRMS"+suffix);

    gStyle->SetOptStat(0);
    harnessRMSMap.GetXaxis()->SetTitle("harness phi index");
    harnessRMSMap.GetYaxis()->SetTitle("harness eta index");
    float rms_mean=harnessRMS.GetMean(); 
    float rms_rms=harnessRMS.GetRMS();
    harnessRMSMap.GetZaxis()->SetRangeUser(TMath::Max((float)0.,(float)rms_mean-3*rms_rms), rms_mean+3*rms_rms);
    harnessRMSMap.Draw("COLZ");
    harnessRMSMap.SaveAs("plots/harnessRMSMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/harnessRMSMap"+suffix);

    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	//       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		// 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
		etBandGraph[ij][ii]=etBand[ii][ij];
		// 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	      }
	  
	  } 
      } 
      
  
    TGraphAsymmErrors* lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors* lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TGraphAsymmErrors* et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
    TGraphAsymmErrors* et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

    //   etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
    //   etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d/%m");


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

    //   if (savePlots)
    c1->SaveAs("plots/lcGraph_harness"+suffix);


    c.Draw();
    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kBlack);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   if (savePlots)
    c1->SaveAs("plots/etGraph_harness"+suffix);


    c.Draw();
    //   etNoCorr95Graph->SetFillColor(kYellow);
    //   etNoCorr95Graph->SetFillStyle(1001);
    //   etNoCorr95Graph->Draw("2same");

    //   etNoCorr68Graph->SetFillColor(kGreen);
    //   etNoCorr68Graph->SetFillStyle(1001);
    //   etNoCorr68Graph->SetMarkerColor(kBlack);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("2same");
    //   etNoCorr68Graph->Draw("pxsame");

    // //   if (savePlots)
    //     c1->SaveAs("plots/etNoCorrGraph_harness"+suffix);


    c.Draw();
    lc95Graph->SetFillColor(kYellow+1);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen+3);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");

    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kRed);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   etNoCorr68Graph->SetMarkerColor(kViolet);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("pxsame");

    line->Draw("same");

    //   if (savePlots)
    c1->SaveAs("plots/fullHistory_harness"+suffix);
  }


  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      //       etNoCorrDist[ii]->Reset();
    }


  if(doAlsoEndcSCsPlots){
    for (int i=1;i<=nSCs;++i)
      {
	if (quickTest && i%10 != 1)
	  continue;
	TString ittLabel="iSC_";
	ittLabel+=i;
	TString itt="";
	itt+=i;


	TGraphErrors* lc=(TGraphErrors*)_file4->Get("lc_"+ittLabel);
	TGraphErrors* et=(TGraphErrors*)_file4->Get("et_"+ittLabel);
	TGraphErrors* etSumOverRef=(TGraphErrors*)_file4->Get("etSumOverRef_"+ittLabel);

	if (!lc || !et || !etSumOverRef)
	  {
	    std::cout << "TGraphs not found" << std::endl;
	    continue;
	  }

	//        TGraphErrors* etNoCorr=(TGraphErrors*)_file6->Get("etNoCorr_"+ittLabel);
	//        TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file6->Get("EtNoCorrvsTL_"+ittLabel);
	TH1F* histoForRMSTT=new TH1F("histoForRMSTT","histoForRMSTT",200,axisLower,axisUp);
	Double_t * yAxisTT;

	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lc->GetY()+ii));
	    if (!useEtSumOverEtSumRef)
	      etDist[ii]->Fill(*(et->GetY()+ii));
	    else
	      etDist[ii]->Fill(*(etSumOverRef->GetY()+ii));
	    // 	   etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	  }

	//        EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
	ttAlpha.Fill(fa1->GetParameter(0));
	std::cout <<  "monitor_"+ittLabel << std::endl;
       
	if (!useEtSumOverEtSumRef)
	  yAxisTT=(Double_t*)et->GetY();
	else
	  yAxisTT=(Double_t*)etSumOverRef->GetY();
       
	for (int ii=startInterval;ii<npoints;++ii){
	  histoForRMSTT->Fill(yAxisTT[ii]);
	}
       
	histoForRMSTT->SetFillColor(kRed);
	scMean.Fill(histoForRMSTT->GetMean());
	scRMS.Fill(histoForRMSTT->GetRMS());
	for (int isign=0;isign<kSides;++isign)
	  for (int ix=0;ix<kEndcWedgesX;++ix)
	    for (int iy=0;iy<kEndcWedgesX;++iy)
	      {
		if (endcapSCs[ix][iy][isign]==(i-1))
		  {
		    int ix_i=ix;
		    if (isign==1)
		      ix_i+=100;
		    scMap.SetBinContent(ix_i+1,iy+1,i);
		    scMeanMap.SetBinContent(ix_i+1,iy+1,histoForRMSTT->GetMean());
		    scMeanMap.SetBinError(ix_i+1,iy+1,histoForRMSTT->GetMeanError());
		    scRMSMap.SetBinContent(ix_i+1,iy+1,histoForRMSTT->GetRMS());
		    scRMSMap.SetBinError(ix_i+1,iy+1,histoForRMSTT->GetRMSError());
		    scAlphaMap.SetBinContent(ix_i+1,iy+1,fa1->GetParameter(0));
		    scAlphaMap.SetBinError(ix_i+1,iy+1,fa1->GetParError(0));
		  }
	       
	      }
       
	if ( ( savePlots || histoForRMSTT->GetRMS() > ttRMSThreshold || histoForRMSTT->GetMean() > ttMeanHighThreshold || histoForRMSTT->GetMean() < ttMeanLowThreshold ) && (histoForRMSTT->GetRMS() > 0 ||  histoForRMSTT->GetMean() > 0 ) )
	  {

	    lc->SetMarkerColor(1);
	    lc->SetMarkerStyle(20);
	    lc->SetMarkerSize(0.7);
	    lc->SetLineWidth(2);

	    et->SetMarkerColor(kRed);
	    et->SetMarkerStyle(20);
	    et->SetMarkerSize(0.5);

	    etSumOverRef->SetMarkerColor(kRed);
	    etSumOverRef->SetMarkerStyle(20);
	    etSumOverRef->SetMarkerSize(0.5);
	    // 	   etNoCorr->SetMarkerColor(kViolet);
	    // 	   etNoCorr->SetMarkerStyle(20);
	    // 	   etNoCorr->SetMarkerSize(0.5);


	    TCanvas *c_monTT = new TCanvas("c_monTT","c_monTT",1000,500);
	    c_monTT->cd();
	    TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
	    pad1->SetMargin(0.1,0.01,0.1,0.1);
	    //	   pad1->SetMargin(0.1,0.01,0.1,0.14);
	    pad1->Draw();
	    pad1->cd();
	    b.Draw();
	    b.SetStats(kFALSE);





	    if (!useEtSumOverEtSumRef)
	      et->Draw("PSAME");
	    else
	      etSumOverRef->Draw("PSAME");
	    // 	   etNoCorr->Draw("PSAME");
	    lc->Draw("PSAME");
	   
	    line->Draw("same");
	    TLegend l(0.45,0.75,0.91,0.88);
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    l.AddEntry(lc,"1/laser corr isc("+itt+")","P");
	    if (!useEtSumOverEtSumRef)
	      l.AddEntry(et,"<et corrected> isc("+itt+")","P");
	    else
	      l.AddEntry(etSumOverRef,"<et corrected/ et ref> isc("+itt+")","P");
	    // 	   l.AddEntry(etNoCorr,"<et uncorrected> itt("+itt+")","P");
	    l.Draw();

	    c_monTT->cd();

	    gStyle->SetOptStat("mr");
	    TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
	    //           pad2->SetMargin(0.1,0,0.1,0.14);
	    pad2->Draw();
	    pad2->cd();
	    histoForRMSTT->SetStats(kTRUE);
	    histoForRMSTT->Draw("HBAR");


	    c_monTT->cd();

	    c_monTT->SaveAs("plots/monitor_"+ittLabel+suffix);
	    histoForRMSTT->Reset();
	    a.Draw();
	    a.GetYaxis()->SetTitle();
	    a.GetXaxis()->SetTitle("1/<lc>");
	    // 	   EtNoCorrvsTL->SetMarkerColor(kViolet);
	    // 	   EtNoCorrvsTL->SetMarkerStyle(20);
	    // 	   EtNoCorrvsTL->SetMarkerSize(0.5);
	   
	    // 	   EtNoCorrvsTL->Draw("PESAME");
	    // 	   c1->SaveAs("plots/EtNoCorrvsTL"+ittLabel+suffix);
	    delete c_monTT;
	    // 	   delete pad1;
	    // 	   delete pad2;
	  }
	delete histoForRMSTT;

      }

    gPad->SetMargin(0.1,0.16,0.12,0.1);
    gStyle->SetOptStat(1111);
    scAlpha.GetXaxis()->SetTitle("alpha correction");
    scAlpha.SetMaximum(scAlpha.GetMaximum()*1.3);
    scAlpha.Draw();
    scAlpha.SaveAs("plots/scAlpha.root");
    //   if (savePlots)
    c1->SaveAs("plots/scAlpha"+suffix);

    gStyle->SetOptStat(0);
    scAlphaMap.GetXaxis()->SetTitle("ix");
    scAlphaMap.GetYaxis()->SetTitle("iy");
    scAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
    scAlphaMap.Draw("COLZ");
    scAlphaMap.SaveAs("plots/scAlphaMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/scAlphaMap"+suffix);

    gStyle->SetOptStat(1111);
    scMean.GetXaxis()->SetTitle("Mean of normalized sc response");
    scMean.SetMaximum(scMean.GetMaximum()*1.3);
    scMean.Draw();
    scMean.SaveAs("plots/scMean.root");
    //   if (savePlots)
    c1->SaveAs("plots/scMean"+suffix);

    gStyle->SetOptStat(0);
    scMeanMap.GetXaxis()->SetTitle("ix");
    scMeanMap.GetYaxis()->SetTitle("iy");
    scMeanMap.GetZaxis()->SetRangeUser(scMean.GetMean()-5*scMean.GetRMS(),scMean.GetMean()+5*scMean.GetRMS());
    scMeanMap.Draw("COLZ");
    scMeanMap.SaveAs("plots/scMeanMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/scMeanMap"+suffix);

    gStyle->SetOptStat(0);
    scMap.GetXaxis()->SetTitle("ix");
    scMap.GetYaxis()->SetTitle("iy");
    //    scMap.GetZaxis()->SetRangeUser(scMean.GetMean()-5*scMean.GetRMS(),scMean.GetMean()+5*scMean.GetRMS());
    scMap.Draw("COLZ");
    scMap.SaveAs("plots/scMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/scMap"+suffix);

    gStyle->SetOptStat(1111);
    scRMS.GetXaxis()->SetTitle("RMS of normalized sc response");
    scRMS.SetMaximum(scRMS.GetMaximum()*1.3);
    scRMS.Draw();
    scRMS.SaveAs("plots/scRMS.root");
    //   if (savePlots)
    c1->SaveAs("plots/scRMS"+suffix);

    gStyle->SetOptStat(0);
    scRMSMap.GetXaxis()->SetTitle("ix");
    scRMSMap.GetYaxis()->SetTitle("iy");
    float rms_mean=scRMS.GetMean(); 
    float rms_rms=scRMS.GetRMS();
    scRMSMap.GetZaxis()->SetRangeUser(TMath::Max((float)0.,(float)rms_mean-3*rms_rms), rms_mean+3*rms_rms);
    scRMSMap.Draw("COLZ");
    scRMSMap.SaveAs("plots/scRMSMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/scRMSMap"+suffix);

    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	//       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		// 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
		etBandGraph[ij][ii]=etBand[ii][ij];
		// 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	      }
	  
	  } 
      } 
      
  
    TGraphAsymmErrors* lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors* lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TGraphAsymmErrors* et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
    TGraphAsymmErrors* et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

    //   etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
    //   etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d/%m");


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

    //   if (savePlots)
    c1->SaveAs("plots/lcGraph_isc"+suffix);


    c.Draw();
    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kBlack);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   if (savePlots)
    c1->SaveAs("plots/etGraph_isc"+suffix);


    c.Draw();
    //   etNoCorr95Graph->SetFillColor(kYellow);
    //   etNoCorr95Graph->SetFillStyle(1001);
    //   etNoCorr95Graph->Draw("2same");

    //   etNoCorr68Graph->SetFillColor(kGreen);
    //   etNoCorr68Graph->SetFillStyle(1001);
    //   etNoCorr68Graph->SetMarkerColor(kBlack);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("2same");
    //   etNoCorr68Graph->Draw("pxsame");

    // //   if (savePlots)
    //     c1->SaveAs("plots/etNoCorrGraph_isc"+suffix);


    c.Draw();
    lc95Graph->SetFillColor(kYellow+1);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen+3);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");

    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kRed);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   etNoCorr68Graph->SetMarkerColor(kViolet);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("pxsame");

    line->Draw("same");

    //   if (savePlots)
    c1->SaveAs("plots/fullHistory_isc"+suffix);
  }

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      //       etNoCorrDist[ii]->Reset();
    }

  if(doAlsoEndcHarnessPlots){
    for (int i=1;i<=nHarness_EE;++i)
      {
	if (quickTest && i%2 != 1)
	  continue;
	TString harnessLabel="endcHarness_";
	harnessLabel+=i;
	TString harness="";
	harness+=i;


	TGraphErrors* lc=(TGraphErrors*)_file7->Get("lc_"+harnessLabel);
	TGraphErrors* et=(TGraphErrors*)_file7->Get("et_"+harnessLabel);
	TGraphErrors* etSumOverRef=(TGraphErrors*)_file7->Get("etSumOverRef_"+harnessLabel);
	
	if (!lc || !et || !etSumOverRef)
	  {
	    std::cout << "TGraphs not found" << std::endl;
	    continue;
	  }
	
	//        TGraphErrors* etNoCorr=(TGraphErrors*)_file6->Get("etNoCorr_"+harnessLabel);
	//        TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file6->Get("EtNoCorrvsTL_"+harnessLabel);
	TH1F* histoForRMSHARNESS=new TH1F("histoForRMSHARNESS","histoForRMSHARNESS",200,axisLower,axisUp);
	Double_t * yAxisHARNESS;

	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lc->GetY()+ii));
	    if (!useEtSumOverEtSumRef)
	      etDist[ii]->Fill(*(et->GetY()+ii));
	    else
	      etDist[ii]->Fill(*(etSumOverRef->GetY()+ii));
	    // 	   etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	  }
	
	//        EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
	// harnessAlpha.Fill(fa1->GetParameter(0));
	std::cout <<  "monitor_"+harnessLabel << std::endl;
	
	if (!useEtSumOverEtSumRef)
	  yAxisHARNESS=(Double_t*)et->GetY();
	else
	  yAxisHARNESS=(Double_t*)etSumOverRef->GetY();
       
	for (int ii=startInterval;ii<npoints;++ii){
	  histoForRMSHARNESS->Fill(yAxisHARNESS[ii]);
	}
       
	histoForRMSHARNESS->SetFillColor(kRed);
	endcHarnessMean.Fill(histoForRMSHARNESS->GetMean());
	endcHarnessRMS.Fill(histoForRMSHARNESS->GetRMS());
	
	std::vector<HarnessMap::channel> myChannels=harnessMap_->channelsInHarness(1,i-1);
	for (unsigned int ic=0;ic<myChannels.size();++ic)
	  {
	    HarnessMap::channel channel=myChannels[ic];
	    int ix_i=channel.ieta+(channel.sign>0)*100;
	    int iy=channel.iphi;
	    endcHarnessMap.SetBinContent(ix_i,iy,i);
	    endcHarnessMeanMap.SetBinContent(ix_i,iy,histoForRMSHARNESS->GetMean());
	    endcHarnessMeanMap.SetBinError(ix_i,iy,histoForRMSHARNESS->GetMeanError());
	    endcHarnessRMSMap.SetBinContent(ix_i,iy,histoForRMSHARNESS->GetRMS());
	    endcHarnessRMSMap.SetBinError(ix_i,iy,histoForRMSHARNESS->GetRMSError());
	    endcHarnessAlphaMap.SetBinContent(ix_i,iy,fa1->GetParameter(0));
	    endcHarnessAlphaMap.SetBinError(ix_i,iy,fa1->GetParError(0));
	  }
	  
       
	if ( ( savePlots || histoForRMSHARNESS->GetRMS() > harnessRMSThreshold || histoForRMSHARNESS->GetMean() > harnessMeanHighThreshold || histoForRMSHARNESS->GetMean() < harnessMeanLowThreshold ) && (histoForRMSHARNESS->GetRMS() > 0 ||  histoForRMSHARNESS->GetMean() > 0 ) )
	  {

	    lc->SetMarkerColor(1);
	    lc->SetMarkerStyle(20);
	    lc->SetMarkerSize(0.7);
	    lc->SetLineWidth(2);

	    et->SetMarkerColor(kRed);
	    et->SetMarkerStyle(20);
	    et->SetMarkerSize(0.5);

	    etSumOverRef->SetMarkerColor(kRed);
	    etSumOverRef->SetMarkerStyle(20);
	    etSumOverRef->SetMarkerSize(0.5);
	    // 	   etNoCorr->SetMarkerColor(kViolet);
	    // 	   etNoCorr->SetMarkerStyle(20);
	    // 	   etNoCorr->SetMarkerSize(0.5);


	    TCanvas *c_monHARNESS = new TCanvas("c_monHARNESS","c_monHARNESS",1000,500);
	    c_monHARNESS->cd();
	    TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
	    pad1->SetMargin(0.1,0.01,0.1,0.1);
	    //	   pad1->SetMargin(0.1,0.01,0.1,0.14);
	    pad1->Draw();
	    pad1->cd();
	    b.Draw();
	    b.SetStats(kFALSE);

	    if (!useEtSumOverEtSumRef)
	      et->Draw("PSAME");
	    else
	      etSumOverRef->Draw("PSAME");
	    // 	   etNoCorr->Draw("PSAME");
	    lc->Draw("PSAME");
	   
	    line->Draw("same");
	    TLegend l(0.45,0.75,0.91,0.88);
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    l.AddEntry(lc,"1/laser corr iendcharness("+harness+")","P");
	    if (!useEtSumOverEtSumRef)
	      l.AddEntry(et,"<et corrected> iendcHarness("+harness+")","P");
	    else
	      l.AddEntry(etSumOverRef,"<et corrected/ et ref> iendcHarness("+harness+")","P");
	    // 	   l.AddEntry(etNoCorr,"<et uncorrected> harness("+harness+")","P");
	    l.Draw();

	    c_monHARNESS->cd();

	    gStyle->SetOptStat("mr");
	    TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
	    //           pad2->SetMargin(0.1,0,0.1,0.14);
	    pad2->Draw();
	    pad2->cd();
	    histoForRMSHARNESS->SetStats(kTRUE);
	    histoForRMSHARNESS->Draw("HBAR");


	    c_monHARNESS->cd();

	    c_monHARNESS->SaveAs("plots/monitor_"+harnessLabel+suffix);
	    histoForRMSHARNESS->Reset();
	    a.Draw();
	    a.GetYaxis()->SetTitle();
	    a.GetXaxis()->SetTitle("1/<lc>");
	    // 	   EtNoCorrvsTL->SetMarkerColor(kViolet);
	    // 	   EtNoCorrvsTL->SetMarkerStyle(20);
	    // 	   EtNoCorrvsTL->SetMarkerSize(0.5);
	   
	    // 	   EtNoCorrvsTL->Draw("PESAME");
	    // 	   c1->SaveAs("plots/EtNoCorrvsTL"+harnessLabel+suffix);
	    delete c_monHARNESS;
	    // 	   delete pad1;
	    // 	   delete pad2;
	  }
	delete histoForRMSHARNESS;

      }

    gPad->SetMargin(0.1,0.16,0.12,0.1);
    gStyle->SetOptStat(1111);
    endcHarnessAlpha.GetXaxis()->SetTitle("alpha correction");
    endcHarnessAlpha.SetMaximum(endcHarnessAlpha.GetMaximum()*1.3);
    endcHarnessAlpha.Draw();
    endcHarnessAlpha.SaveAs("plots/endcHarnessAlpha.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessAlpha"+suffix);

    gStyle->SetOptStat(0);
    endcHarnessAlphaMap.GetXaxis()->SetTitle("ix");
    endcHarnessAlphaMap.GetYaxis()->SetTitle("iy");
    endcHarnessAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
    endcHarnessAlphaMap.Draw("COLZ");
    endcHarnessAlphaMap.SaveAs("plots/endcHarnessAlphaMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessAlphaMap"+suffix);

    gStyle->SetOptStat(1111);
    endcHarnessMean.GetXaxis()->SetTitle("Mean of normalized endcHarness response");
    endcHarnessMean.SetMaximum(endcHarnessMean.GetMaximum()*1.3);
    endcHarnessMean.Draw();
    endcHarnessMean.SaveAs("plots/endcHarnessMean.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessMean"+suffix);

    gStyle->SetOptStat(0);
    endcHarnessMeanMap.GetXaxis()->SetTitle("ix");
    endcHarnessMeanMap.GetYaxis()->SetTitle("iy");
    endcHarnessMeanMap.GetZaxis()->SetRangeUser(endcHarnessMean.GetMean()-5*endcHarnessMean.GetRMS(),endcHarnessMean.GetMean()+5*endcHarnessMean.GetRMS());
    endcHarnessMeanMap.Draw("COLZ");
    endcHarnessMeanMap.SaveAs("plots/endcHarnessMeanMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessMeanMap"+suffix);

    gStyle->SetOptStat(0);
    endcHarnessMap.GetXaxis()->SetTitle("ix");
    endcHarnessMap.GetYaxis()->SetTitle("iy");
    //    endcHarnessMap.GetZaxis()->SetRangeUser(endcHarnessMean.GetMean()-5*endcHarnessMean.GetRMS(),endcHarnessMean.GetMean()+5*endcHarnessMean.GetRMS());
    endcHarnessMap.Draw("COLZ");
    endcHarnessMap.SaveAs("plots/endcHarnessMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessMap"+suffix);

    gStyle->SetOptStat(1111);
    endcHarnessRMS.GetXaxis()->SetTitle("RMS of normalized endcHarness response");
    endcHarnessRMS.SetMaximum(endcHarnessRMS.GetMaximum()*1.3);
    endcHarnessRMS.Draw();
    endcHarnessRMS.SaveAs("plots/endcHarnessRMS.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessRMS"+suffix);

    gStyle->SetOptStat(0);
    endcHarnessRMSMap.GetXaxis()->SetTitle("ix");
    endcHarnessRMSMap.GetYaxis()->SetTitle("iy");
    float rms_mean=endcHarnessRMS.GetMean(); 
    float rms_rms=endcHarnessRMS.GetRMS();
    endcHarnessRMSMap.GetZaxis()->SetRangeUser(TMath::Max((float)0.,(float)rms_mean-3*rms_rms), rms_mean+3*rms_rms);
    endcHarnessRMSMap.Draw("COLZ");
    endcHarnessRMSMap.SaveAs("plots/endcHarnessRMSMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcHarnessRMSMap"+suffix);

    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	//       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		// 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
		etBandGraph[ij][ii]=etBand[ii][ij];
		// 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	      }
	  
	  } 
      } 
      
  
    TGraphAsymmErrors* lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors* lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TGraphAsymmErrors* et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
    TGraphAsymmErrors* et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

    //   etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
    //   etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d/%m");


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

    //   if (savePlots)
    c1->SaveAs("plots/lcGraph_iendcHarness"+suffix);


    c.Draw();
    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kBlack);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   if (savePlots)
    c1->SaveAs("plots/etGraph_iendcHarness"+suffix);


    c.Draw();
    //   etNoCorr95Graph->SetFillColor(kYellow);
    //   etNoCorr95Graph->SetFillStyle(1001);
    //   etNoCorr95Graph->Draw("2same");

    //   etNoCorr68Graph->SetFillColor(kGreen);
    //   etNoCorr68Graph->SetFillStyle(1001);
    //   etNoCorr68Graph->SetMarkerColor(kBlack);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("2same");
    //   etNoCorr68Graph->Draw("pxsame");

    // //   if (savePlots)
    //     c1->SaveAs("plots/etNoCorrGraph_iendcHarness"+suffix);


    c.Draw();
    lc95Graph->SetFillColor(kYellow+1);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen+3);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");

    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kRed);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   etNoCorr68Graph->SetMarkerColor(kViolet);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("pxsame");

    line->Draw("same");

    //   if (savePlots)
    c1->SaveAs("plots/fullHistory_iendcHarness"+suffix);
  }

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      //       etNoCorrDist[ii]->Reset();
    }

  if(doAlsoXtalPlots){
    for (int i=1;i<=nXtals;++i)
      {
	if (quickTest && i%100 != 1)
	    continue;
	TString ixtalLabel="ixtal_";
	ixtalLabel+=i;
	TString ixtal="";
	ixtal+=i;

	TGraphErrors* lcxtal=(TGraphErrors*)_file2->Get("lc_"+ixtalLabel);
	TGraphErrors* etxtal=(TGraphErrors*)_file2->Get("et_"+ixtalLabel);
	TGraphErrors* etSumOverRefxtal=(TGraphErrors*)_file2->Get("etSumOverRef_"+ixtalLabel);
	//        TGraphErrors* etNoCorrxtal=(TGraphErrors*)_file2->Get("etNoCorr_"+ixtalLabel);
	//        TGraphErrors* EtNoCorrvsTLxtal= (TGraphErrors*)_file2->Get("EtNoCorrvsTL_"+ixtalLabel);
	TH1F* histoForRMSXtal=new TH1F("histoForRMSXtal","histoForRMSXtal",100,axisLowerXtal,axisUpXtal);
	Double_t * yAxisXtal;


	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lcxtal->GetY()+ii));
	    if (!useEtSumOverEtSumRef)
	      etDist[ii]->Fill(*(etxtal->GetY()+ii));
	    else
	      etDist[ii]->Fill(*(etSumOverRefxtal->GetY()+ii));
	    // 	   etNoCorrDist[ii]->Fill(*(etNoCorrxtal->GetY()+ii));
	  }

	//        EtNoCorrvsTLxtal->Fit(fa1,"R+","",0.9,1.01);
	xtalAlpha.Fill(fa1->GetParameter(0));
	int etaIndex=(i-1)/360+1;
	int phiIndex=(i-1)%360+1;
	if (etaIndex>85)
	  etaIndex++;
	else
	  etaIndex=86-etaIndex;

	std::cout <<  "monitor_"+ixtalLabel << std::endl;
	std::cout << "i " << i << " " << etaIndex << " " << phiIndex << std::endl;
	xtalAlphaMap.SetBinContent(phiIndex,etaIndex,fa1->GetParameter(0));
	xtalAlphaMap.SetBinError(phiIndex,etaIndex,fa1->GetParError(0));

	if (!useEtSumOverEtSumRef)
	  yAxisXtal=(Double_t*)etxtal->GetY();
	else
	  yAxisXtal=(Double_t*)etSumOverRefxtal->GetY();
       
	for (int ii=startInterval;ii<npoints;++ii){
	  histoForRMSXtal->Fill(yAxisXtal[ii]);
	}
	histoForRMSXtal->SetFillColor(kRed);

	xtalMean.Fill(histoForRMSXtal->GetMean());
	xtalRMS.Fill(histoForRMSXtal->GetRMS());
	xtalMap.SetBinContent(phiIndex,etaIndex,i);
	xtalMeanMap.SetBinContent(phiIndex,etaIndex,histoForRMSXtal->GetMean());
	xtalMeanMap.SetBinError(phiIndex,etaIndex,histoForRMSXtal->GetMeanError());
	xtalRMSMap.SetBinContent(phiIndex,etaIndex,histoForRMSXtal->GetRMS());
	xtalRMSMap.SetBinError(phiIndex,etaIndex,histoForRMSXtal->GetRMSError());

	if ( (savePlots || histoForRMSXtal->GetRMS() > xtalRMSThreshold || histoForRMSXtal->GetMean() > xtalMeanHighThreshold || histoForRMSXtal->GetMean() < xtalMeanLowThreshold ) 
	     && (histoForRMSXtal->GetRMS() > 0 ||  histoForRMSXtal->GetMean() > 0 ) 
	     )
	  {

	    lcxtal->SetMarkerColor(1);
	    lcxtal->SetMarkerStyle(20);
	    lcxtal->SetMarkerSize(0.7);
	    lcxtal->SetLineWidth(2);
	    etxtal->SetMarkerColor(kRed);
	    etxtal->SetMarkerStyle(20);
	    etxtal->SetMarkerSize(0.5);

	    etSumOverRefxtal->SetMarkerColor(kRed);
	    etSumOverRefxtal->SetMarkerStyle(20);
	    etSumOverRefxtal->SetMarkerSize(0.5);
	    // 	   etNoCorrxtal->SetMarkerColor(kViolet);
	    // 	   etNoCorrxtal->SetMarkerStyle(20);
	    // 	   etNoCorrxtal->SetMarkerSize(0.5);


	    TCanvas *c_mon_xtal = new TCanvas("c_mon_xtal","c_mon_xtal",1000,500);
	    c_mon_xtal->cd();
	    TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
	    pad1->SetMargin(0.1,0.01,0.1,0.1);
	    //pad1->SetMargin(0.1,0.01,0.1,0.14);
	    pad1->Draw();
	    pad1->cd();

	    bXtal.Draw();
	    bXtal.SetStats(kFALSE);



	    if (!useEtSumOverEtSumRef)
	      etxtal->Draw("PSAME");
	    else
	      etSumOverRefxtal->Draw("PSAME");
	    // 	   etNoCorrxtal->Draw("PSAME");
	    lcxtal->Draw("PSAME");
	    line->Draw("same");
	    TLegend l(0.45,0.75,0.91,0.88);
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    l.AddEntry(lcxtal,"1/laser corr ixtal("+ixtal+")","P");
	    if (!useEtSumOverEtSumRef)
	      l.AddEntry(etxtal,"<et corrected> ixtal("+ixtal+")","P");
	    else
	      l.AddEntry(etSumOverRefxtal,"<et corrected/ etref> ixtal("+ixtal+")","P");
	    // 	   l.AddEntry(etNoCorr,"<et uncorrected> ixtal("+ixtal+")","P");
	    l.Draw();

	    c_mon_xtal->cd();
	    gStyle->SetOptStat("mr");
	    TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
	    //	   pad2->SetMargin(0.1,0,0.1,0.14);
	    pad2->Draw();
	    pad2->cd();
	    histoForRMSXtal->SetStats(kTRUE);
	    histoForRMSXtal->Draw("HBAR");

	    c_mon_xtal->cd();


	    c_mon_xtal->SaveAs("plots/monitor_"+ixtalLabel+suffix);


	    // 	   a.Draw();
	    // 	   a.GetYaxis()->SetTitle("");
	    // 	   a.GetXaxis()->SetTitle("1/<lc>");
	    // 	   EtNoCorrvsTLxtal->SetMarkerColor(kViolet);
	    // 	   EtNoCorrvsTLxtal->SetMarkerStyle(20);
	    // 	   EtNoCorrvsTLxtal->SetMarkerSize(0.5);
	   
	    // 	   EtNoCorrvsTLxtal->Draw("PESAME");
	   
	    // 	   c1->SaveAs("plots/EtNoCorrvsTL"+ixtalLabel+suffix);
	    delete c_mon_xtal;
	    // 	   delete pad1;
	    // 	   delete pad2;
	  }

	delete histoForRMSXtal;

	//        delete lcxtal;
	//        delete etxtal;
	//        delete etNoCorrxtal;
	//        delete EtNoCorrvsTLxtal;
      }

    gPad->SetMargin(0.1,0.16,0.12,0.1);
    gStyle->SetOptStat(1111);
    xtalAlpha.GetXaxis()->SetTitle("alpha correction");
    xtalAlpha.SetMaximum(xtalAlpha.GetMaximum()*1.3);
    xtalAlpha.Draw();
    xtalAlpha.SaveAs("plots/xtalAlpha.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalAlpha"+suffix);

    gStyle->SetOptStat(0);
    xtalAlphaMap.GetXaxis()->SetTitle("xtal phi index");
    xtalAlphaMap.GetYaxis()->SetTitle("xtal eta index");
    xtalAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
    xtalAlphaMap.Draw("COLZ");
    xtalAlphaMap.SaveAs("plots/xtalAlphaMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalAlphaMap"+suffix);


    gStyle->SetOptStat(1111);
    xtalMean.GetXaxis()->SetTitle("Mean of normalized xtal response");
    xtalMean.SetMaximum(xtalMean.GetMaximum()*1.3);
    xtalMean.Draw();
    xtalMean.SaveAs("plots/xtalMean.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalMean"+suffix);

    gStyle->SetOptStat(0);
    xtalMeanMap.GetXaxis()->SetTitle("xtal phi index");
    xtalMeanMap.GetYaxis()->SetTitle("xtal eta index");
    xtalMeanMap.GetZaxis()->SetRangeUser(xtalMean.GetMean()-5*xtalMean.GetRMS(),xtalMean.GetMean()+5*xtalMean.GetRMS());
    xtalMeanMap.Draw("COLZ");
    xtalMeanMap.SaveAs("plots/xtalMeanMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalMeanMap"+suffix);

    gStyle->SetOptStat(0);
    xtalMap.GetXaxis()->SetTitle("xtal phi index");
    xtalMap.GetYaxis()->SetTitle("xtal eta index");
    //xtalMap.GetZaxis()->SetRangeUser(xtalMean.GetMean()-5*xtalMean.GetRMS(),xtalMean.GetMean()+5*xtalMean.GetRMS());
    xtalMap.Draw("COLZ");
    xtalMap.SaveAs("plots/xtalMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalMap"+suffix);

    gStyle->SetOptStat(1111);
    xtalRMS.GetXaxis()->SetTitle("RMS of normalized xtal response");
    xtalRMS.SetMaximum(xtalRMS.GetMaximum()*1.3);
    xtalRMS.Draw();
    xtalRMS.SaveAs("plots/xtalRMS.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalRMS"+suffix);

    gStyle->SetOptStat(0);
    xtalRMSMap.GetXaxis()->SetTitle("xtal phi index");
    xtalRMSMap.GetYaxis()->SetTitle("xtal eta index");
    float rms_mean=xtalRMS.GetMean(); 
    float rms_rms=xtalRMS.GetRMS();
    xtalRMSMap.GetZaxis()->SetRangeUser(TMath::Max((float)0.,(float)rms_mean-3*rms_rms), rms_mean+3*rms_rms);
    xtalRMSMap.Draw("COLZ");
    xtalRMSMap.SaveAs("plots/xtalRMSMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/xtalRMSMap"+suffix);
  
    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	//       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		// 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
		etBandGraph[ij][ii]=etBand[ii][ij];
		// 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	      }
	  
	  } 
      } 
      
  
    TGraphAsymmErrors* lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors* lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TGraphAsymmErrors* et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
    TGraphAsymmErrors* et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

    //   etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
    //   etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d/%m");


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

    //   if (savePlots)
    c1->SaveAs("plots/lcGraph_ixtal"+suffix);


    c.Draw();
    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kBlack);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   if (savePlots)
    c1->SaveAs("plots/etGraph_ixtal"+suffix);


    c.Draw();
    //   etNoCorr95Graph->SetFillColor(kYellow);
    //   etNoCorr95Graph->SetFillStyle(1001);
    //   etNoCorr95Graph->Draw("2same");

    //   etNoCorr68Graph->SetFillColor(kGreen);
    //   etNoCorr68Graph->SetFillStyle(1001);
    //   etNoCorr68Graph->SetMarkerColor(kBlack);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("2same");
    //   etNoCorr68Graph->Draw("pxsame");

    //   if (savePlots)
    //     c1->SaveAs("plots/etNoCorrGraph_ixtal"+suffix);


    c.Draw();
    lc95Graph->SetFillColor(kYellow+1);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen+3);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");

    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kRed);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   etNoCorr68Graph->SetMarkerColor(kViolet);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("pxsame");
    line->Draw("same");

    //   if (savePlots)
    c1->SaveAs("plots/fullHistory_ixtal"+suffix);
  }



  if(doAlsoEndcXtalPlots){
    for (int i=1;i<=nEndcXtals;++i)
      {
	if (quickTest && i%100 != 1)
	  continue;
	TString ixtalLabel="iendcxtal_";
	ixtalLabel+=i;
	TString ixtal="";
	ixtal+=i;

	TGraphErrors* lcxtal=(TGraphErrors*)_file5->Get("lc_"+ixtalLabel);
	TGraphErrors* etxtal=(TGraphErrors*)_file5->Get("et_"+ixtalLabel);
	TGraphErrors* etSumOverRefxtal=(TGraphErrors*)_file5->Get("etSumOverRef_"+ixtalLabel);
	//        TGraphErrors* etNoCorrxtal=(TGraphErrors*)_file2->Get("etNoCorr_"+ixtalLabel);
	//        TGraphErrors* EtNoCorrvsTLxtal= (TGraphErrors*)_file2->Get("EtNoCorrvsTL_"+ixtalLabel);
	TH1F* histoForRMSXtal=new TH1F("histoForRMSXtal","histoForRMSXtal",100,axisLowerXtal,axisUpXtal);
	Double_t * yAxisXtal;


	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lcxtal->GetY()+ii));
	    if (!useEtSumOverEtSumRef)
	      etDist[ii]->Fill(*(etxtal->GetY()+ii));
	    else
	      etDist[ii]->Fill(*(etSumOverRefxtal->GetY()+ii));
	    // 	   etNoCorrDist[ii]->Fill(*(etNoCorrxtal->GetY()+ii));
	  }

	//        EtNoCorrvsTLxtal->Fit(fa1,"R+","",0.9,1.01);
	endcXtalAlpha.Fill(fa1->GetParameter(0));
	std::cout <<  "monitor_"+ixtalLabel << std::endl;

	if (!useEtSumOverEtSumRef)
	  yAxisXtal=(Double_t*)etxtal->GetY();
	else
	  yAxisXtal=(Double_t*)etSumOverRefxtal->GetY();
       
	for (int ii=startInterval;ii<npoints;++ii){
	  histoForRMSXtal->Fill(yAxisXtal[ii]);
	}
	histoForRMSXtal->SetFillColor(kRed);

	endcXtalMean.Fill(histoForRMSXtal->GetMean());
	endcXtalRMS.Fill(histoForRMSXtal->GetRMS());

	for (int isign=0;isign<kSides;++isign)
	  for (int ix=0;ix<kEndcWedgesX;++ix)
	    for (int iy=0;iy<kEndcWedgesX;++iy)
	      {
		if (endcapXtals[ix][iy][isign]==(i-1))
		  {
		    int ix_i=ix;
		    if (isign==1)
		      ix_i+=100;       
		    endcXtalAlphaMap.SetBinContent(ix_i+1,iy+1,fa1->GetParameter(0));
		    endcXtalAlphaMap.SetBinError(ix_i+1,iy+1,fa1->GetParError(0));
		    endcXtalMap.SetBinContent(ix_i+1,iy+1,i);
		    endcXtalMeanMap.SetBinContent(ix_i+1,iy+1,histoForRMSXtal->GetMean());
		    endcXtalMeanMap.SetBinError(ix_i+1,iy+1,histoForRMSXtal->GetMeanError());
		    endcXtalRMSMap.SetBinContent(ix_i+1,iy+1,histoForRMSXtal->GetRMS());
		    endcXtalRMSMap.SetBinError(ix_i+1,iy+1,histoForRMSXtal->GetRMSError());
		    break;
		  }
	      }
	if ( (savePlots || histoForRMSXtal->GetRMS() > xtalRMSThreshold || histoForRMSXtal->GetMean() > xtalMeanHighThreshold || histoForRMSXtal->GetMean() < xtalMeanLowThreshold ) 
	     && (histoForRMSXtal->GetRMS() > 0 ||  histoForRMSXtal->GetMean() > 0 ) 
	     )
	  {

	    lcxtal->SetMarkerColor(1);
	    lcxtal->SetMarkerStyle(20);
	    lcxtal->SetMarkerSize(0.7);
	    lcxtal->SetLineWidth(2);
	    etxtal->SetMarkerColor(kRed);
	    etxtal->SetMarkerStyle(20);
	    etxtal->SetMarkerSize(0.5);

	    etSumOverRefxtal->SetMarkerColor(kRed);
	    etSumOverRefxtal->SetMarkerStyle(20);
	    etSumOverRefxtal->SetMarkerSize(0.5);
	    // 	   etNoCorrxtal->SetMarkerColor(kViolet);
	    // 	   etNoCorrxtal->SetMarkerStyle(20);
	    // 	   etNoCorrxtal->SetMarkerSize(0.5);


	    TCanvas *c_mon_xtal = new TCanvas("c_mon_xtal","c_mon_xtal",1000,500);
	    c_mon_xtal->cd();
	    TPad *pad1 = new TPad("pad1", "monitoring",0.03,0.03,0.75,0.92);
	    pad1->SetMargin(0.1,0.01,0.1,0.1);
	    //pad1->SetMargin(0.1,0.01,0.1,0.14);
	    pad1->Draw();
	    pad1->cd();

	    bXtal.Draw();
	    bXtal.SetStats(kFALSE);



	    if (!useEtSumOverEtSumRef)
	      etxtal->Draw("PSAME");
	    else
	      etSumOverRefxtal->Draw("PSAME");
	    // 	   etNoCorrxtal->Draw("PSAME");
	    lcxtal->Draw("PSAME");
	    line->Draw("same");
	    TLegend l(0.45,0.75,0.91,0.88);
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    l.AddEntry(lcxtal,"1/laser corr iendcxtal("+ixtal+")","P");
	    if (!useEtSumOverEtSumRef)
	      l.AddEntry(etxtal,"<et corrected> iendcxtal("+ixtal+")","P");
	    else
	      l.AddEntry(etSumOverRefxtal,"<et corrected/ etref> iendcxtal("+ixtal+")","P");
	    // 	   l.AddEntry(etNoCorr,"<et uncorrected> ixtal("+ixtal+")","P");
	    l.Draw();

	    c_mon_xtal->cd();
	    gStyle->SetOptStat("mr");
	    TPad *pad2 = new TPad("pad2", "rms",0.76,0.03,0.98,0.92);
	    //	   pad2->SetMargin(0.1,0,0.1,0.14);
	    pad2->Draw();
	    pad2->cd();
	    histoForRMSXtal->SetStats(kTRUE);
	    histoForRMSXtal->Draw("HBAR");

	    c_mon_xtal->cd();


	    c_mon_xtal->SaveAs("plots/monitor_"+ixtalLabel+suffix);


	    // 	   a.Draw();
	    // 	   a.GetYaxis()->SetTitle("");
	    // 	   a.GetXaxis()->SetTitle("1/<lc>");
	    // 	   EtNoCorrvsTLxtal->SetMarkerColor(kViolet);
	    // 	   EtNoCorrvsTLxtal->SetMarkerStyle(20);
	    // 	   EtNoCorrvsTLxtal->SetMarkerSize(0.5);
	   
	    // 	   EtNoCorrvsTLxtal->Draw("PESAME");
	   
	    // 	   c1->SaveAs("plots/EtNoCorrvsTL"+ixtalLabel+suffix);
	    delete c_mon_xtal;
	    // 	   delete pad1;
	    // 	   delete pad2;
	  }

	delete histoForRMSXtal;

	//        delete lcxtal;
	//        delete etxtal;
	//        delete etNoCorrxtal;
	//        delete EtNoCorrvsTLxtal;
      }

    gPad->SetMargin(0.1,0.16,0.12,0.1);
    gStyle->SetOptStat(1111);
    endcXtalAlpha.GetXaxis()->SetTitle("alpha correction");
    endcXtalAlpha.SetMaximum(endcXtalAlpha.GetMaximum()*1.3);
    endcXtalAlpha.Draw();
    endcXtalAlpha.SaveAs("plots/endcXtalAlpha.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalAlpha"+suffix);

    gStyle->SetOptStat(0);
    endcXtalAlphaMap.GetXaxis()->SetTitle("xtal ix");
    endcXtalAlphaMap.GetYaxis()->SetTitle("xtal iy");
    endcXtalAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
    endcXtalAlphaMap.Draw("COLZ");
    endcXtalAlphaMap.SaveAs("plots/endcXtalAlphaMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalAlphaMap"+suffix);


    gStyle->SetOptStat(1111);
    endcXtalMean.GetXaxis()->SetTitle("Mean of normalized xtal response");
    endcXtalMean.SetMaximum(endcXtalMean.GetMaximum()*1.3);
    endcXtalMean.Draw();
    endcXtalMean.SaveAs("plots/endcXtalMean.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalMean"+suffix);

    gStyle->SetOptStat(0);
    endcXtalMeanMap.GetXaxis()->SetTitle("xtal ix");
    endcXtalMeanMap.GetYaxis()->SetTitle("xtal iy");
    endcXtalMeanMap.GetZaxis()->SetRangeUser(endcXtalMean.GetMean()-5*endcXtalMean.GetRMS(),endcXtalMean.GetMean()+5*endcXtalMean.GetRMS());
    endcXtalMeanMap.Draw("COLZ");
    endcXtalMeanMap.SaveAs("plots/endcXtalMeanMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalMeanMap"+suffix);

    gStyle->SetOptStat(0);
    endcXtalMap.GetXaxis()->SetTitle("xtal ix");
    endcXtalMap.GetYaxis()->SetTitle("xtal iy");
    //    endcXtalMap.GetZaxis()->SetRangeUser(endcXtalMean.GetMean()-5*endcXtalMean.GetRMS(),endcXtalMean.GetMean()+5*endcXtalMean.GetRMS());
    endcXtalMap.Draw("COLZ");
    endcXtalMap.SaveAs("plots/endcXtalMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalMap"+suffix);

    gStyle->SetOptStat(1111);
    endcXtalRMS.GetXaxis()->SetTitle("RMS of normalized xtal response");
    endcXtalRMS.SetMaximum(endcXtalRMS.GetMaximum()*1.3);
    endcXtalRMS.Draw();
    endcXtalRMS.SaveAs("plots/endcXtalRMS.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalRMS"+suffix);

    gStyle->SetOptStat(0);
    endcXtalRMSMap.GetXaxis()->SetTitle("xtal ix");
    endcXtalRMSMap.GetYaxis()->SetTitle("xtal iy");
    float rms_mean=endcXtalRMS.GetMean(); 
    float rms_rms=endcXtalRMS.GetRMS();
    endcXtalRMSMap.GetZaxis()->SetRangeUser(TMath::Max((float)0.,(float)rms_mean-3*rms_rms), rms_mean+3*rms_rms);
    endcXtalRMSMap.Draw("COLZ");
    endcXtalRMSMap.SaveAs("plots/endcXtalRMSMap.root");
    //   if (savePlots)
    c1->SaveAs("plots/endcXtalRMSMap"+suffix);
  
    for (int ii=0;ii<npoints;++ii)
      {
	lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
	etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
	//       etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
	for (int ij=0;ij<5;++ij)
	  {
	    if (ij!=2)
	      {
		lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
		etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
		// 	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	      }
	    else
	      {
		lcBandGraph[ij][ii]=lcBand[ii][ij];
		etBandGraph[ij][ii]=etBand[ii][ij];
		// 	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	      }
	  
	  } 
      } 
      
  
    TGraphAsymmErrors* lc68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
    TGraphAsymmErrors* lc95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

    TGraphAsymmErrors* et68Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
    TGraphAsymmErrors* et95Graph=new TGraphAsymmErrors(npoints,lct->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

    //   etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
    //   etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

    c.Draw();
    c.GetXaxis()->SetTitle("Time");
    c.GetXaxis()->SetTimeDisplay(1);
    c.GetXaxis()->SetTimeFormat("%d/%m");


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

    //   if (savePlots)
    c1->SaveAs("plots/lcGraph_iendcxtal"+suffix);


    c.Draw();
    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kBlack);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   if (savePlots)
    c1->SaveAs("plots/etGraph_iendcxtal"+suffix);


    c.Draw();
    //   etNoCorr95Graph->SetFillColor(kYellow);
    //   etNoCorr95Graph->SetFillStyle(1001);
    //   etNoCorr95Graph->Draw("2same");

    //   etNoCorr68Graph->SetFillColor(kGreen);
    //   etNoCorr68Graph->SetFillStyle(1001);
    //   etNoCorr68Graph->SetMarkerColor(kBlack);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("2same");
    //   etNoCorr68Graph->Draw("pxsame");

    //   if (savePlots)
    //     c1->SaveAs("plots/etNoCorrGraph_ixtal"+suffix);


    c.Draw();
    lc95Graph->SetFillColor(kYellow+1);
    lc95Graph->SetFillStyle(1001);
    lc95Graph->Draw("2same");

    lc68Graph->SetFillColor(kGreen+3);
    lc68Graph->SetFillStyle(1001);
    lc68Graph->SetMarkerColor(kBlack);
    lc68Graph->SetMarkerStyle(20);
    lc68Graph->SetMarkerSize(0.4);
    lc68Graph->Draw("2same");
    lc68Graph->Draw("pxsame");

    et95Graph->SetFillColor(kYellow);
    et95Graph->SetFillStyle(1001);
    et95Graph->Draw("2same");

    et68Graph->SetFillColor(kGreen);
    et68Graph->SetFillStyle(1001);
    et68Graph->SetMarkerColor(kRed);
    et68Graph->SetMarkerStyle(20);
    et68Graph->SetMarkerSize(0.4);
    et68Graph->Draw("2same");
    et68Graph->Draw("pxsame");

    //   etNoCorr68Graph->SetMarkerColor(kViolet);
    //   etNoCorr68Graph->SetMarkerStyle(20);
    //   etNoCorr68Graph->SetMarkerSize(0.4);
    //   etNoCorr68Graph->Draw("pxsame");
    line->Draw("same");

    //   if (savePlots)
    c1->SaveAs("plots/fullHistory_iendcxtal"+suffix);
  }
}
