{
  TFile *_file0 = TFile::Open("beamSpotInterpolatedCorrections.root");

  bool doVertexWidth=false;

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TH2F a("a","a",10,-2.2,2.2,10,0.97,1.03);
  a.GetXaxis()->SetTitle("beam Spot position (cm)");
  a.GetYaxis()->SetTitle("bsPos relative correction");

  for (int iieta=0;iieta<85;++iieta)
    //    for (int iside=0;iside<2;++iside)
      {
	
	TString name="bsPosCorrectionInterpolated_ieta_";
	name+=(iieta+1);
// 	name+="_side_";
// 	name+=iside;
	TGraph* g_i=(TGraph*)_file0->Get(name);
	
	std::cout << g_i->GetN() << std::endl;
	std::cout << TMath::MaxElement(g_i->GetN(),g_i->GetX()) << std::endl;
	std::cout << TMath::MinElement(g_i->GetN(),g_i->GetX()) << std::endl;

	std::cout <<   (TMath::MaxElement(g_i->GetN(),g_i->GetX())-TMath::MinElement(g_i->GetN(),g_i->GetX()))/(g_i->GetN()-1) << std::endl;
	name="bsPosCorrection_ieta_";
 	name+=(iieta+1);
// 	name+="_side_";
// 	name+=iside;
	TGraph* g_o=(TGraph*)_file0->Get(name);

	// 	if (iside==0)
	a.Draw();
	g_o->SetMarkerStyle(20);
	g_o->SetMarkerSize(1.2);
// 	if (iside==0)
	  g_o->SetMarkerColor(4);
// 	else
// 	  g_o->SetMarkerColor(3);
	g_o->Draw("PSAME");

	g_i->SetLineWidth(2);
// 	if (iside==0)
	g_i->SetLineColor(1);
// 	else
// 	  g_i->SetLineColor(2);
	g_i->Draw("LSAME");

// 	if (iside==1)
	c1->SaveAs(name+".png");
      }


  if (doVertexWidth)
    {
  TH2F a("a","a",10,4.,8.,10,0.97,1.03);
  a.GetXaxis()->SetTitle("beam Spot Width (cm)");
  a.GetYaxis()->SetTitle("bsPos relative correction");

  for (int iieta=0;iieta<85;++iieta)
    //    for (int iside=0;iside<2;++iside)
      {
	
	TString name="bsWidCorrectionInterpolated_ieta_";
	name+=(iieta+1);
// 	name+="_side_";
// 	name+=iside;
	TGraph* g_i=(TGraph*)_file0->Get(name);
	
	name="bsWidCorrection_ieta_";
 	name+=(iieta+1);
// 	name+="_side_";
// 	name+=iside;
	TGraph* g_o=(TGraph*)_file0->Get(name);

	// 	if (iside==0)
	a.Draw();
	g_o->SetMarkerStyle(20);
	g_o->SetMarkerSize(1.2);
// 	if (iside==0)
	  g_o->SetMarkerColor(4);
// 	else
// 	  g_o->SetMarkerColor(3);
	g_o->Draw("PSAME");

	g_i->SetLineWidth(2);
// 	if (iside==0)
	g_i->SetLineColor(1);
// 	else
// 	  g_i->SetLineColor(2);
	g_i->Draw("LSAME");

// 	if (iside==1)
	c1->SaveAs(name+".png");
      }
    }
}
