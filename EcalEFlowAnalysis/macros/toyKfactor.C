{
  gRandom->SetSeed(0);

  float lowEnergyCut=0.25;
  float miscalibRange=0.1;
  float noiseInGeV=0.1;
  int nmiscalib=20;
  int nCuts=10;
  int nToys=1;
  int hitsPerToy=100000;
  bool addNoise=true;
  bool addSignal=true;
  bool miscalibOnlyResponse=true;
  

  TH1F* h_Et[nmiscalib];
  TF1* f;
  f = new TF1("f","[0]*(TMath::Exp(x*[1])+[2]*TMath::Power(x,[3]))",0.05,3.);
      
  // From a fit to the data ieta==1
  //  Info in <TCanvas::MakeDefCanvas>:  created default TCanvas with name c1
  // FCN=508.171 FROM MIGRAD    STATUS=CONVERGED     905 CALLS         906 TOTAL
  //                     EDM=1.94413e-10    STRATEGY= 1      ERROR MATRIX ACCURATE
  //  EXT PARAMETER                                   STEP         FIRST
  //  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE
  //   1  p0           5.68108e+03   3.87893e+02   1.18213e-01   4.02729e-07
  //   2  p1          -5.86915e+00   1.46021e-01   1.43788e-04  -5.46295e-05
  //   3  p2           1.01454e-02   5.82677e-04   3.08487e-07   1.81270e-01
  //   4  p3          -3.08101e+00   2.72977e-02   2.91613e-05  -2.49161e-03
      
  f->SetParameter(0,1);
  f->SetParameter(1,-5.86915e+00);
  f->SetParameter(2,1.01454e-02);
  f->SetParameter(3,-3.08101);

  for (int i=0;i<nmiscalib;++i)
    {
      TString histoName="h_miscalib_";
      histoName+=i;
      h_Et[i]=new TH1F(histoName,histoName,75,0.,1.5);
      // Exp + Pow fit model
      float miscal = (1. - ((float) miscalibRange / 2.) ) + ( ( (float) miscalibRange / (float) nmiscalib )* (float) i );	  
    } 

  //   float a=0;
  //   float am[nCuts];
  //   float ap[nCuts];
  //   int nhita=0;

  TRandom3 g;
  TFile *fOut=new TFile(Form("toysKfactor_miscalibOnlyResponse%d_addNoise%d_noise%3.2f_lowCut%3.2f.root",int(miscalibOnlyResponse),int(addNoise),noiseInGeV,lowEnergyCut),"RECREATE");
  fOut->cd();
  TNtuple *toys = new TNtuple("toys","","kFactorMean:kFactorMeanErr:kFactorSum:kFactorSumErr:kFactorNHits:kFactorNHitsErr:kFactorDouble:kFactorDoubleErr");


  for (int itoy=0; itoy < nToys ; ++ itoy)
    {

      float miscalib[nmiscalib];
      float ratioMean[nmiscalib];
      float ratioDouble[nCuts][nmiscalib];
      float ratioDoubleHits[nCuts][nmiscalib];
      float ratioHits[nmiscalib];
      float ratioSum[nmiscalib];
 
      float b[nmiscalib];
      int nhitb[nmiscalib];
      float bm[nmiscalib][nCuts];
      float bp[nmiscalib][nCuts];
      int nhitm[nmiscalib][nCuts];
      int nhitp[nmiscalib][nCuts];

      int i=0;  


	  //       am[j]=0.;
	  //       ap[j]=0.;
      
      for (int imis=0;imis<nmiscalib;++imis)
	{
	  b[imis]=0;
	  nhitb[imis]=0;
	  miscalib[imis] = (1. - ((float) miscalibRange / 2.) ) + ( ( (float) miscalibRange / (float) nmiscalib )* (float) imis );	  
	  for (int j=0;j<nCuts;++j)
	    {
	      bm[imis][j]=0.;
	      bp[imis][j]=0.;
	      nhitm[imis][j]=0;
	      nhitp[imis][j]=0;
	    }
	}
      
      while ((nhitb[nmiscalib/2]<hitsPerToy))
	{
	  if (nhitb[nmiscalib/2]%20000==0)
	    std::cout << nhitb[nmiscalib/2] << " " << i << std::endl;

	  //       if (hit>lowEnergyCut && hit<1.25)
	  // 	{
	  // 	  a+=hit;
	  // 	  for (int j=0;j<nCuts;++j)
	  // 	    {
	  // 	      if (hit<(0.3+j*0.02))
	  // 		am[j]+=hit;
	  // 	      else
	  // 		ap[j]+=hit;
	  // 	    }
	  // 	  ++nhita;
	  // 	}

	  float noise=0;
	  if (addNoise)
	    noise=g.Gaus(0,noiseInGeV);
	  //	  float noise=0.;
	  float hit=0;
	  if (addSignal)
	    hit=f->GetRandom();	

	  if (!miscalibOnlyResponse)
	    hit+=noise;
	  for (int imis=0;imis<nmiscalib;++imis)
	    {
//  	      if (i%nmiscalib != imis)
//  		continue;
	      float energy=0;
	      if (miscalibOnlyResponse)
		energy=hit*miscalib[imis]+noise;
	      else
		energy=hit*miscalib[imis];

	      if (energy > lowEnergyCut && energy<1.25)
		{
		  h_Et[imis]->Fill(energy);
		  b[imis]+=energy;
		  ++nhitb[imis];
		  for (int j=0;j<nCuts;++j)
		    {
		      if (energy<(lowEnergyCut+0.05+j*0.01))
			{
			  bm[imis][j]+=energy;
			  nhitm[imis][j]++;
			}
		      else
			{
			  bp[imis][j]+=energy;
			  nhitp[imis][j]++;
			}
		    }
		}
	    }
	  ++i;
	}
      
      
      for (int imis=0;imis<nmiscalib;++imis)
	{      
	  std::cout << "+++++++++++++++++++  " << imis <<  "  +++++++++++++++++++" << std::endl;
	  miscalib[imis]=(miscalib[imis])-1;
	  ratioMean[imis]=( (b[imis]/nhitb[imis]) - lowEnergyCut ) / ( (b[nmiscalib/2]/nhitb[nmiscalib/2]) - lowEnergyCut ) - 1.;
	  ratioHits[imis]=(float)nhitb[imis]/(float)nhitb[nmiscalib/2]-1;

	  for (int j=0;j<nCuts;++j)
	    {
	      ratioDouble[j][imis]=((bp[imis][j]/bm[imis][j])/(bp[nmiscalib/2][j]/bm[nmiscalib/2][j]))-1;
	      //	  ratioDoubleHits[j][imis]= ( (nhitp[imis][j]/nhitm[imis][j] )/ (nhitp[nmiscalib/2][j]/nhitm[nmiscalib/2][j]) )-1;
	      ratioDoubleHits[j][imis]= ( (float) (nhitp[imis][j]/ (float) nhitm[imis][j] ) / ( (float) (nhitp[nmiscalib/2][j]/ (float) nhitm[nmiscalib/2][j] ) ) ) - 1. ;
	      std::cout << ratioDoubleHits[j][imis] << std::endl;
	      std::cout << imis << " " <<   j << " " <<  (float) nhitp[imis][j]/(float) nhitm[imis][j] << " " << (bp[imis][j]/bm[imis][j])/((bp[nmiscalib/2][j]/bm[nmiscalib/2][j]))-1 << std::endl;
	    }
      
	  //	ratioDouble[j][imis]=(bm[j]/bp[j])/((am[j]/ap[j]))-1;
	  ratioSum[imis]=(b[imis])/(b[nmiscalib/2])-1;
	}
  

      gStyle->SetOptFit(111111);
      gStyle->SetOptTitle(0);

      if (itoy == nToys-1)
	{
	  for (int i=0;i<nmiscalib;++i)
	    {
	      TString histo="h_Et_miscalib_";
	      histo+=i;      

	      if (i==nmiscalib/2)
		continue;

	      if (i==0)
		{
		  h_Et[nmiscalib/2]->Sumw2();
		  //	  h_Et[nmiscalib/2]->Rebin(4);
		  h_Et[nmiscalib/2]->Scale(1./(float)h_Et[nmiscalib/2]->GetEntries());
		  h_Et[nmiscalib/2]->SetLineWidth(2);
		  h_Et[nmiscalib/2]->GetXaxis()->SetTitle("Et [GeV]");
		}

	      h_Et[i]->Sumw2();
	      //      h_Et[i]->Rebin(4);
	      h_Et[i]->Scale(1./(float)h_Et[i]->GetEntries());
	      h_Et[i]->GetXaxis()->SetTitle("Et [GeV]");

	      h_Et[nmiscalib/2]->SetMaximum(TMath::Max(h_Et[nmiscalib/2]->GetMaximum(),h_Et[i]->GetMaximum()));
	      h_Et[nmiscalib/2]->Draw("H");
	      h_Et[i]->SetMarkerColor(2);
	      h_Et[i]->SetMarkerStyle(20);
	      h_Et[i]->SetMarkerSize(0.7);
	      h_Et[i]->Draw("EPSAME");

	      c1->SaveAs(histo+".png");

	      h_Et[i]->Divide(h_Et[nmiscalib/2]);
	      h_Et[i]->SetMarkerColor(1);
	      h_Et[i]->SetMarkerSize(1.2);

	      h_Et[i]->Fit("pol1");
	      h_Et[i]->SetMaximum(1.5);
	      h_Et[i]->SetMinimum(0.5);
	      h_Et[i]->Draw("EP");
	      c1->SaveAs(histo+"_ratio.png");
	    }
	}

      float kFactorMean;
      float kFactorMeanErr;
      float kFactorSum;
      float kFactorSumErr;
      float kFactorNHits;
      float kFactorNHitsErr;
      float kFactorDouble;
      float kFactorDoubleErr;

      TF1 slope("slope","[0]*x",-0.4,0.4);
      TGraph* aGr=new TGraph(nmiscalib,&ratioMean[0],&miscalib[0]);
      aGr->SetName("aGr");
      aGr->GetYaxis()->SetTitle("miscalib-1");
      aGr->GetXaxis()->SetTitle("<Et Observed>/<Et True>-1");
      aGr->Print();
      aGr->Draw("AP*");
      aGr->Fit("slope");
      kFactorMean=aGr->GetFunction("slope")->GetParameter(0);
      kFactorMeanErr=aGr->GetFunction("slope")->GetParError(0);
      if (itoy==nToys-1)
	{
	  aGr->Write("kFactorMean");
	  c1->SaveAs("kFactorMean.png");
	}
  
      TGraph* aGr=new TGraph(nmiscalib,&miscalib[0],&ratioSum[0]);
      aGr->SetName("aGr");
      aGr->GetXaxis()->SetTitle("miscalib-1");
      aGr->GetYaxis()->SetTitle("EtSum Observed/EtSum True-1");
      aGr->Print();
      aGr->Draw("AP*");
      aGr->Fit("slope");
      kFactorSum=aGr->GetFunction("slope")->GetParameter(0);
      kFactorSumErr=aGr->GetFunction("slope")->GetParError(0);
      if (itoy==nToys-1)
	{
	  aGr->Write("kFactorSum");
	  c1->SaveAs("kFactorSum.png");
	}

      TGraph* aGr=new TGraph(nmiscalib,&miscalib[0],&ratioHits[0]);
      aGr->SetName("aGr");
      aGr->GetXaxis()->SetTitle("miscalib-1");
      aGr->GetYaxis()->SetTitle("NHits Observed/NHits True-1");
      aGr->Print();
      aGr->Draw("AP*");
      aGr->Fit("slope");
      kFactorNHits=aGr->GetFunction("slope")->GetParameter(0);
      kFactorNHitsErr=aGr->GetFunction("slope")->GetParError(0);
      if (itoy==nToys-1)
	{
	  aGr->Write("kFactorNHits");
	  c1->SaveAs("kFactorNHits.png");
	}

      //   TGraph* aGr=new TGraph(nmiscalib,&ratioSum[0],&ratioMean[0]);
      //   aGr->SetName("aGr");
      //   aGr->GetXaxis()->SetTitle("miscalib-1");
      //   aGr->GetYaxis()->SetTitle("NHits Observed/NHits True-1");
      //   aGr->Print();
      //   aGr->Draw("AP*");
      //   aGr->Fit("slope");
      //   aGr->SaveAs("kFactorDoubleRatio.root");
      //   c1->SaveAs("kFactorDoubleRatio.png");

      for (int j=0;j<nCuts;++j)
	{
	  TGraph* aGr=new TGraph(nmiscalib,&miscalib[0],&ratioDouble[j][0]);
	  aGr->SetName("aGr");
	  aGr->GetXaxis()->SetTitle("miscalib-1");
	  aGr->GetYaxis()->SetTitle("EtPoM/EtPoM True-1");
	  aGr->Print();
	  aGr->Draw("AP*");
	  aGr->Fit("slope");
	  TString name="kFactorDouble_";
	  name+=j;
	  if (j==5)
	    {
	      kFactorDouble=aGr->GetFunction("slope")->GetParameter(0);
	      kFactorDoubleErr=aGr->GetFunction("slope")->GetParError(0);
	    }
	  if (itoy==nToys-1)
	    {
	      aGr->Write(name);
	      c1->SaveAs(name+".png");
	    }
	}

      for (int j=0;j<nCuts;++j)
	{
	  TGraph* aGr=new TGraph(nmiscalib,&miscalib[0],&ratioDoubleHits[j][0]);
	  aGr->SetName("aGr");
	  aGr->GetXaxis()->SetTitle("miscalib-1");
	  aGr->GetYaxis()->SetTitle("NHitsPoM/NHitsPoM True-1");
	  aGr->Print();
	  aGr->Draw("AP*");
	  aGr->Fit("slope");
	  TString name="kFactorDoubleHits_";
	  name+=j;
	  if (itoy==nToys-1)
	    {
	      aGr->Write(name);
	      c1->SaveAs(name+".png");
	    }
	}
      toys->Fill(kFactorMean,kFactorMeanErr,kFactorSum,kFactorSumErr,kFactorNHits,kFactorNHitsErr,kFactorDouble,kFactorDoubleErr);
    }
  

  toys->Write();
  fOut->Close();
}
