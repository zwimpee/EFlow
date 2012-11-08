void drawFitResults(
		    TString inputFile="",
		    int det=0,
		    float maxAlphaError=999,
		    float chi2ProbCut=0,
		    float chi2MinCut=0,
		    TString additionalCuts=""
		    )
{
  TFile *_file0 = TFile::Open(inputFile);

  TString cut="status==0 && badXtal==0 && alpha0+delta_alpha>0.3 && err_alpha<";
  cut+=maxAlphaError;
  if (chi2ProbCut>0)
    {
      cut+=" && TMath::Prob(chi2Min,ndof)>";
      cut+=chi2ProbCut;
    }
  if (chi2MinCut>0)
    {
      cut+=" && chi2Min/ndof<";
      cut+=chi2MinCut;
    }
  if (additionalCuts!="")
    {
      cut+=" && ";
      cut+=additionalCuts;
    }

  std::cout << "CUT STRING " << cut << std::endl;
  TString BTCP="&& alpha0>1.05";
  TString SIC="&& alpha0<1.05";

  TString minus="&& index<30601";
  TString plus="&& index>30600";
  if (det ==1)
    {
      minus="&& sign==0";
      plus="&& sign==1";
    }
  TProfile2D* hh;
  if (det==0)
    {
      hh=new TProfile2D("hh","hh",360,0.5,360.5,171,-85.5,85.5);
      hh->GetXaxis()->SetTitle("i#phi");
      hh->GetYaxis()->SetTitle("i#eta");
    }
  else if (det==1)
    {
      hh=new TProfile2D("hh","hh",200,0.5,200.5,100,0.5,100.5);
      hh->GetXaxis()->SetTitle("ix");
      hh->GetYaxis()->SetTitle("iy");
    }

  TProfile* hhh;
  if (det==0)
    {
      hhh=new TProfile("hhh","hhh",85,0.5,85.5);
      hhh->GetXaxis()->SetTitle("i#eta");
    }
  else if (det==1)
    {
      hhh=new TProfile("hhh","hhh",40,0.5,40.5);
      hhh->GetXaxis()->SetTitle("iring");
    }

  TString variablesToPlot[13] =
    {
      "alpha0+delta_alpha",
      "delta[0]",
      "delta[1]",
      "delta[2]",
      "delta[3]",
      "err_alpha",
      "err_delta[0]",
      "err_delta[1]",
      "err_delta[2]",
      "err_delta[3]",
      "chi2Min/ndof",
      "ndof",
      "index1+0.001"
    };

  TString varName[13] =
    {
      "alpha",
      "delta_0",
      "delta_1",
      "delta_2",
      "delta_3",
      "err_alpha",
      "err_delta_0",
      "err_delta_1",
      "err_delta_2",
      "err_delta_3",
      "chi2MinOverNDOF",
      "NDOF",
      "iring"
    };

  int nBins[13] =
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
      1000,
      2000,
      100
    };

  float minHisto[13] =
    {
      -1,
      -0.02,
      -0.02,
      -0.02,
      -0.02,
      0.,
      0.,
      0.,
      0.,
      0.,
      0.,
      -0.5,
      -0.5
    };

  float maxHisto[13] =
    {
      4.,
      0.02,
      0.02,
      0.02,
      0.02,
      0.8,
      0.01,
      0.01,
      0.01,
      0.01,
      3,
      1999.5,
      99.5
    };


  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(1111);      

  TString detString="_EB";
  if (det==1)
    detString="_EE";
  
  for (int ivar=0;ivar<12;++ivar)
    { 
      TH1F h(varName[ivar],varName[ivar],nBins[ivar],minHisto[ivar],maxHisto[ivar]);
      TString plotString=variablesToPlot[ivar];
      plotString+=">>";
      plotString+=varName[ivar];
      fitResults->Draw(plotString,cut);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+".png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+".root");

      float mean=h.GetMean();
      float rms=h.GetRMS();


      fitResults->Draw(plotString,cut+plus);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"+.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"+.root");

      fitResults->Draw(plotString,cut+minus);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"-.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"-.root");

      fitResults->Draw(plotString,cut+BTCP);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"_BTCP.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"_BTCP.root");

      fitResults->Draw(plotString,cut+SIC);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"_SIC.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"_SIC.root");


      fitResults->Draw(plotString,cut+BTCP+plus);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"+_BTCP.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"+_BTCP.root");

      fitResults->Draw(plotString,cut+SIC+plus);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"+_SIC.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"+_SIC.png");


      fitResults->Draw(plotString,cut+BTCP+minus);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"-_BTCP.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"-_BTCP.png");

      fitResults->Draw(plotString,cut+SIC+minus);
      h.GetXaxis()->SetTitle(varName[ivar]);
      gStyle->SetOptStat(1111);      
      h.Draw();
      h.SetStats(true);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"-_SIC.png");
      h.SaveAs("plotsFit/"+varName[ivar]+detString+"-_SIC.png");




      plotString=variablesToPlot[ivar];
      if (det==0)
	plotString+=":ieta:iphi>>hh";
      else if (det==1)
	plotString+=":iphi:ieta+sign*100>>hh";
      fitResults->Draw(plotString,cut);

      gPad->SetRightMargin(0.15);
      if (ivar>5 && ivar<10)
	hh->GetZaxis()->SetRangeUser(0.,3*rms);
      else if (ivar<5)
	hh->GetZaxis()->SetRangeUser(mean-3*rms,mean+3*rms);
      else if (ivar==12)
	hh->GetZaxis()->SetRangeUser(0.,100);
      else 
	hh->GetZaxis()->SetRangeUser(0.,mean+3*rms);
      gStyle->SetOptStat(0);      
      hh->Draw("COLZ");
      c1->SaveAs("plotsFit/"+varName[ivar]+"_Map"+detString+".png");

      plotString=variablesToPlot[ivar];
      if (det==0)
	plotString+=":ieta:iphi>>hh";
      else if (det==1)
	plotString+=":iphi:ieta+sign*100>>hh";
      fitResults->Draw(plotString,cut+BTCP);

      gPad->SetRightMargin(0.15);
      if (ivar>5 && ivar<10)
	hh->GetZaxis()->SetRangeUser(0.,3*rms);
      else if (ivar<5)
	hh->GetZaxis()->SetRangeUser(mean-3*rms,mean+3*rms);
      else if (ivar==11)
	hh->GetZaxis()->SetRangeUser(0.,100);
      else 
	hh->GetZaxis()->SetRangeUser(0.,mean+3*rms);
      gStyle->SetOptStat(0);      
      hh->Draw("COLZ");
      c1->SaveAs("plotsFit/"+varName[ivar]+"_Map"+detString+"_BTCP.png");

      plotString=variablesToPlot[ivar];
      if (det==0)
	plotString+=":ieta:iphi>>hh";
      else if (det==1)
	plotString+=":iphi:ieta+sign*100>>hh";
      fitResults->Draw(plotString,cut+SIC);

      gPad->SetRightMargin(0.15);
      if (ivar>5 && ivar<10)
	hh->GetZaxis()->SetRangeUser(0.,3*rms);
      else if (ivar<5)
	hh->GetZaxis()->SetRangeUser(mean-3*rms,mean+3*rms);
      else if (ivar==11)
	hh->GetZaxis()->SetRangeUser(0.,100);
      else 
	hh->GetZaxis()->SetRangeUser(0.,mean+3*rms);
      gStyle->SetOptStat(0);      
      hh->Draw("COLZ");
      c1->SaveAs("plotsFit/"+varName[ivar]+"_Map"+detString+"_SIC.png");

      plotString=variablesToPlot[ivar];
      if (det==0)
	plotString+=":index1>>hhh";
      else if (det==1)
	plotString+=":index1>>hhh";

      fitResults->Draw(plotString,cut);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"_vsEta.png");

      fitResults->Draw(plotString,cut+BTCP);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"_vsEta_BTCP.png");

      fitResults->Draw(plotString,cut+SIC);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"_vsEta_SIC.png");


      fitResults->Draw(plotString,cut+plus);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"+_vsEta.png");

      fitResults->Draw(plotString,cut+BTCP+plus);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"+_vsEta_BTCP.png");

      fitResults->Draw(plotString,cut+SIC+plus);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"+_vsEta_SIC.png");


      fitResults->Draw(plotString,cut+minus);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"-_vsEta.png");

      fitResults->Draw(plotString,cut+BTCP+minus);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"-_vsEta_BTCP.png");

      fitResults->Draw(plotString,cut+SIC+minus);
      hhh->Draw();
      hhh->SetStats(false);
      c1->Update();
      c1->SaveAs("plotsFit/"+varName[ivar]+detString+"-_vsEta_SIC.png");

    }
}
