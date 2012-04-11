{
  int nVertexPosBins=21;
  int nVertexWidBins=21;

  TRandom3* g=new TRandom3(0);

  float bsWid=5.22;
  float bsCen=0.411;
  
  TH1F* generated;
  generated=new TH1F("gen","gen",300,-30,30);
  for (int ii=0;ii<5000000;++ii)
    generated->Fill(g->Gaus(bsCen,bsWid));  

  TH1F* gaussian[21];
  TH1F* gaussianWid[21];

  for (int i=0;i<nVertexPosBins;++i)
    {
      TString name="bSpotPosWeights_";
      name+=i;
      std::cout << "Producing " << name << std::endl;
      gaussian[i]=new TH1F(name,name,300,-30,30);
      float bsPos=-2. + i*0.2;

      for (int ii=0;ii<5000000;++ii)
	gaussian[i]->Fill(g->Gaus(bsPos,bsWid));
    }

  for (int i=0;i<nVertexWidBins;++i)
    {
      TString name="bSpotWidWeights_";
      name+=i;
      std::cout << "Producing " << name << std::endl;
      gaussianWid[i]=new TH1F(name,name,300,-30,30);
      float bsPos=0;
      float bsWid=4.5 + i*0.15;

      for (int ii=0;ii<5000000;++ii)
	gaussianWid[i]->Fill(g->Gaus(bsPos,bsWid));
    }

  TFile f("bsWeights.root","RECREATE");
  f.cd();
  for (int i=0;i<nVertexPosBins;++i)
    {
      gaussian[i]->Sumw2();
      gaussian[i]->Divide(generated);
      gaussian[i]->Write();
    }

  for (int i=0;i<nVertexWidBins;++i)
    {
      gaussianWid[i]->Sumw2();
      gaussianWid[i]->Divide(generated);
      gaussianWid[i]->Write();
    }

  f.Close();
}
