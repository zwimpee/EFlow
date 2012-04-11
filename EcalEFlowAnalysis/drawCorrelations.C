{
  TFile *_file0 = TFile::Open("ttAlphaMap.root");
  TFile *_file1 = TFile::Open("EBMapPerTT.root");
  TH2F* alphaMap=(TH2F*) _file0->Get("ttAlphaMap");
  TH2F* icMap=(TH2F*) _file1->Get("EBMapCoeff1AveTT");


  double alpha[36*68];
  double alphaErr[36*68];
  double ic[36*68];
  double icErr[36*68];

  for (int ix=1;ix<73;++ix)
    {
      for (int iy=1;iy<35;++iy)
	{
	  std::cout << (ix-1)*34+(iy-1) << std::endl;
	  int iy_alpha=iy;
	  if (iy>17)
	    iy_alpha++;
	  alpha[(ix-1)*34+(iy-1)]=alphaMap->GetBinContent(ix,iy_alpha);
	  alphaErr[(ix-1)*34+(iy-1)]=alphaMap->GetBinError(ix,iy_alpha);
	  ic[(ix-1)*34+(iy-1)]=icMap->GetBinContent(ix,iy);
	  icErr[(ix-1)*34+(iy-1)]=0.003;
	  std::cout << alpha[(ix-1)*34+(iy-1)] << " " << ic[(ix-1)*34+(iy-1)] << std::endl;
	}
    }

  
  gStyle->SetOptStat(0);
  //  gStyle->SetOptFit(1111111);
  gStyle->SetOptTitle(0);
  TH2F a("a","a",10,0.7,1.4,10,0.7,2.1);
  a.GetXaxis()->SetTitle("ic");
  a.GetYaxis()->SetTitle("alpha correction");
  a.Draw();
  TGraphErrors* alphaIcCorr=new TGraphErrors(36*68,ic,alpha,icErr,alphaErr);
  alphaIcCorr->SetMarkerStyle(20);
  alphaIcCorr->SetMarkerColor(kGreen);
  alphaIcCorr->SetMarkerSize(0.8);
  alphaIcCorr->Draw("PSAME");
  c1->SaveAs("alphaIcCorr.png");
  alphaIcCorr->SaveAs("alphaIcCorr.root");

}
