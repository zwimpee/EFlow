void drawFitResults(
		    TString inputFile=""
)
{
  TFile *_file0 = TFile::Open(inputFile);
  TProfile2D hh("hh","hh",360,0.5,360.5,171,-85.5,85.5);
  hh.GetXaxis()->SetTitle("i#phi");
  hh.GetYaxis()->SetTitle("i#eta");

  TString variablesToPlot[11] =
    {
      "delta_alpha",
      "delta[0]",
      "delta[1]",
      "delta[2]",
      "delta[3]",
      "err_alpha",
      "err_delta[0]",
      "err_delta[1]",
      "err_delta[2]",
      "err_delta[3]",
      "chi2Min/ndof"
    };

  TString varName[11] =
    {
      "delta_alpha",
      "delta_0",
      "delta_1",
      "delta_2",
      "delta_3",
      "err_alpha",
      "err_delta_0",
      "err_delta_1",
      "err_delta_2",
      "err_delta_3",
      "chi2MinOverNDOF"
    };

  int nBins[11] =
    {
      1000,
      1000,
      1000,
      1000,
      1000,
      1000,
      1000,
      1000,
      1000,
      1000,
      1000
    };

  float minHisto[11] =
    {
      -2.,
      -0.02,
      -0.02,
      -0.02,
      -0.02,
      0.,
      0.,
      0.,
      0.,
      0.,
      0.
    };

  float maxHisto[11] =
    {
      2.,
      0.02,
      0.02,
      0.02,
      0.02,
      0.8,
      0.01,
      0.01,
      0.01,
      0.01,
      3
    };


  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(1111);      
  for (int ivar=0;ivar<11;++ivar)
    { 
      TH1F h(varName[ivar],varName[ivar],nBins[ivar],minHisto[ivar],maxHisto[ivar]);
      TString plotString=variablesToPlot[ivar];
      plotString+=">>";
      plotString+=varName[ivar];
      fitResults->Draw(plotString,"status==0");
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      c1->SaveAs("plotsFit/"+varName[ivar]+".png");

      float mean=h.GetMean();
      float rms=h.GetRMS();
      plotString=variablesToPlot[ivar];
      plotString+=":ieta:iphi>>hh";
      fitResults->Draw(plotString,"status==0");

      gPad->SetRightMargin(0.15);
      if (ivar>5 && ivar<10)
	  hh.GetZaxis()->SetRangeUser(0.,3*rms);
      else if (ivar<5)
	  hh.GetZaxis()->SetRangeUser(-3*rms,3*rms);
      else 
	  hh.GetZaxis()->SetRangeUser(0.,mean+3*rms);
      gStyle->SetOptStat(0);      
      hh.Draw("COLZ");
      c1->SaveAs("plotsFit/"+varName[ivar]+"_Map.png");
    }
}
