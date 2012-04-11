{
  gRandom->SetSeed(0);
  // Exp + Pow fit model
  TF1 f("f","[0]*(TMath::Exp(x*[1])+[2]*TMath::Power(x,[3]))",0.14,1.4);
  TF1 fitF("fitF","[0]*(TMath::Exp(x*[1]*[4])+[2]*TMath::Power(x*[4],[3]))",0.25,1.25);

//  Info in <TCanvas::MakeDefCanvas>:  created default TCanvas with name c1
// FCN=508.171 FROM MIGRAD    STATUS=CONVERGED     905 CALLS         906 TOTAL
//                     EDM=1.94413e-10    STRATEGY= 1      ERROR MATRIX ACCURATE
//  EXT PARAMETER                                   STEP         FIRST
//  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE
//   1  p0           5.68108e+03   3.87893e+02   1.18213e-01   4.02729e-07
//   2  p1          -5.86915e+00   1.46021e-01   1.43788e-04  -5.46295e-05
//   3  p2           1.01454e-02   5.82677e-04   3.08487e-07   1.81270e-01
//   4  p3          -3.08101e+00   2.72977e-02   2.91613e-05  -2.49161e-03
  f.SetParameter(0,1);
  f.SetParameter(1,-5.86915e+00);
  f.SetParameter(2,1.01454e-02);
  f.SetParameter(3,-3.08101);

  fitF.SetParameter(0,5e3);
  fitF.FixParameter(1,-5.86915e+00);
  fitF.FixParameter(2,1.01454e-02);
  fitF.FixParameter(3,-3.08101);
  
  fitF.SetParameter(4,1.);

  
  TNtuple *toys = new TNtuple("toys","","et:et2:eta:etb:nhits:nhitsa:nhitsb:fitCalib:fitCalibErr");
  
  //   TH1F meanDist("meanDist","meanDist",1000,0.1,1.1);
  //   TH1F fitCalibDist("fitCalibDist","fitCalibDist",1000,0.9,1.1);
  //   TH1F rmsDist("rmsDist","rmsDist",1000,0.,0.05);
  //   TH1F* doubleDist[20];
  //   TH1F* doubleRMSDist[20];
  //   TH2F* corr[20];
  TH1F etDist("etDist","etDist",50,0.25,1.25);

//   for (int j=0;j<20;++j)
//     {
//       TString name="doubleDist_";
//       name+=j;
//       doubleDist[j]=new TH1F(name,name,1000,0.,10.);
//       name="doubleRMSDist_";
//       name+=j;
//       doubleRMSDist[j]=new TH1F(name,name,1000,0.,0.1);
//       name="corr_";
//       name+=j;
//       corr[j]=new TH2F(name,name,1000,0.,10.,1000,0.1,1.1);
//     }

  float a=0;
  float a2=0;
  float aA[20];
  float aA2[20];
  float aB[20];
  float aB2[20];
  int nhita=0;
  int nhitaA[20];
  int nhitaB[20];
  int i=0;
  int hitsPerToy=15000;
  int nToys=1000;
     
  for (int j=0;j<20;++j)
    {
      aA[j]=0.;
      aA2[j]=0.;
      aB[j]=0.;
      aB2[j]=0.;
      nhitaA[j]=0;
      nhitaB[j]=0;
    } 

  while (i<hitsPerToy*nToys)
    {
      if (i%100000==0)
	std::cout << i << std::endl;
      float hit=f.GetRandom();

      if (hit>0.25 && hit<1.25)
	{
	  etDist.Fill(hit);
	  a+=hit;
	  a2+=hit*hit;
	  for (int j=0;j<20;++j)
	    {
	      if (hit<(0.3+j*0.01))
		{
		  aA[j]+=hit;
		  aA2[j]+=hit*hit;
		  nhitaA[j]++;
		}
	      else
		{
		  aB[j]+=hit;
		  aB2[j]+=hit*hit;
		  nhitaB[j]++;
		}
	    }
	  ++nhita;
	}
      
      if ((nhita>0 && nhita%hitsPerToy==0))
	{
	  etDist.Fit(&fitF,"RL0");
	  //	  fitCalibDist.Fill(fitF.GetParameter(4));
	  toys->Fill(a,a2,aA[5],aB[5],nhita,nhitaA[5],nhitaB[5],fitF.GetParameter(4),fitF.GetParError(4));
	    // 	  meanDist.Fill(a/nhita);
	    // 	  rmsDist.Fill(sqrt((a2/nhita - (a*a/(nhita*nhita))))/sqrt(nhita));
	    // 	  for (int j=0;j<20;++j)
	    // 	    {
	    // 	      doubleDist[j]->Fill(aB[j]/aA[j]);
	    // 	      corr[j]->Fill(aB[j]/aA[j],a/nhita);
	    // 	      float errA=sqrt((aA2[j]/nhitaA[j] - (aA[j]*aA[j]/(nhitaA[j]*nhitaA[j]))));
	    // 	      float errB=sqrt((aB2[j]/nhitaB[j] - (aB[j]*aB[j]/(nhitaB[j]*nhitaB[j]))));
	    // 	    }
	  a=0.;
	  a2=0.;
	  for (int j=0;j<20;++j)
	    {
	      aA[j]=0.;
	      aA2[j]=0.;
	      aB[j]=0.;
	      aB2[j]=0.;
	      nhitaA[j]=0;
	      nhitaB[j]=0;
	    }
	  nhita=0;
	  etDist.Reset();
	}

      ++i;

    }

//   float x[20];
//   float y[20];
//   for (int j=0;j<20;++j)
//     {
//       x[j]=(0.3+j*0.01);
//       y[j]=doubleDist[j]->GetRMS()/doubleDist[j]->GetMean();
//     }
//   TGraph *gr=new TGraph(20,x,y);
//   gr->SetName("doubleRatioErr");
  TFile fo("toyKFactorMean.root","RECREATE");
  toys->Write();
//   meanDist.Write();
//   fitCalibDist.Write();
//   rmsDist.Write();
//   for (int j=0;j<20;++j)
//     {
//       doubleDist[j]->Write();
//       corr[j]->Write();
//     }
//   gr->Write();
  fo.Write();
  fo.Close();
}
