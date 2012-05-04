#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include <iostream>

using namespace std;
 
// double RosenBrock(const double *xx )
// {
//   const Double_t x = xx[0];
//   const Double_t y = xx[1];
//   const Double_t tmp1 = y-x*x;
//   const Double_t tmp2 = 1-x;
//   return 100*tmp1*tmp1+tmp2*tmp2;
// }
int nPoints;
double * X;
double * Y;
double * errorY;

int nDeltaParameters;
int * intervalStart;
int * intervalEnd;

//function is (LC+delta_i)^alpha
double corrFunction(double x, const double& par0, const double& par1){
  double  transparencyLoss=TMath::Power(x,1/1.52);
  //  cout<<(x/(pow((transparencyLoss),(alphaDB(109,297,1)-0.3))))<<" ";
  return (x/(TMath::Power((transparencyLoss+par0),(1.52+par1))));
  //  return transparencyLoss+par[0]+0*par[1];
}

double chi2(const double *par )
{
  Double_t chisq = 0;
  Double_t delta;

  //  cout<<nPoints<<endl;
  for (int i=0;i<nPoints; i++) {
    //    cout<<X[i]<<" "<<Y[i]<<" "<<errorY[i]<<" e ";
    int interval=-1;
    for (int ii=0;ii<nDeltaParameters;++ii)
      {
	if (i>=intervalStart[ii] && i<= intervalEnd[ii])
	  {
	    interval=ii;
	    break;
	  }
      }

    delta  = ( Y[i]*corrFunction(X[i],par[interval],par[nDeltaParameters]) - 1. )/errorY[i];
    //    cout<<delta<<" ";
    chisq += delta*delta;
    //cout<<delta<<" "<<chisq<<"e ";
  }

  return chisq;
}
 
int testMinimizer(
		  int nToys=5000,
		  float sigma=0.004,
		  float injDelta=0.,
		  float injDeltaAlpha=0.,
		  TString toyMCFile="toyMCFile.root"
		  )
{
  ROOT::Math::Minimizer* min = 
    ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("Minuit2", "Simplex");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("Minuit2", "Scan");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("Minuit2", "Fumili");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugateFR");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugatePR");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS2");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "SteepestDescent");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLMultiFit", "");
  //   ROOT::Math::Minimizer* min = 
  //          ROOT::Math::Factory::CreateMinimizer("GSLSimAn", "");
 
  min->SetMaxFunctionCalls(1000000);
  min->SetMaxIterations(100000);
  min->SetTolerance(0.0005);
  min->SetPrintLevel(2);

  nDeltaParameters=4;

  TFile* toyMcFile=TFile::Open(toyMCFile,"recreate");
  toyMcFile->cd();

  int status;
  double delta_alpha,delta[nDeltaParameters], err_alpha, err_delta[nDeltaParameters], delta_true, delta_alpha_true,chi2Min,ndof;

  TTree* toyOut=new TTree("toyOut","toyOut");

  toyOut->Branch("delta", delta, "delta[4]/D");
  toyOut->Branch("delta_true", &delta_true, "delta_true/D");
  toyOut->Branch("err_delta", err_delta, "err_delta[4]/D");
  toyOut->Branch("delta_alpha", &delta_alpha, "delta_alpha/D");
  toyOut->Branch("delta_alpha_true", &delta_alpha_true, "delta_alpha_true/D");
  toyOut->Branch("err_alpha", &err_alpha, "err_alpha/D");
  toyOut->Branch("chi2Min", &chi2Min, "chi2Min/D");
  toyOut->Branch("ndof", &ndof, "ndof/D");
  toyOut->Branch("status", &status, "status/I");
  
  TRandom3 * rand=new TRandom3(0);
  
  TFile* lcFile= TFile::Open("lc_ixtal_61030.root");
  
  //  etFile= TFile::Open("etSumOverRef_ixtal_38457.root");
  TGraphErrors* lcGraph=(TGraphErrors*)lcFile->Get("lc_ixtal_61030");
  
  //  int npoints=lcGraph->GetN();
  int excl_intervals=4;//first four intervals for 2011 not used
  //      npoints=npoints;
  //      nPoints=npoints-excl_intervals;
  nPoints=57;

  X = new double[nPoints];
  Y = new double[nPoints];
  errorY = new double[nPoints];

  intervalStart= new int[nDeltaParameters];
  intervalEnd= new int[nDeltaParameters];
  
  intervalStart[0]=0;
  intervalEnd[0]=10;
  intervalStart[1]=11;
  intervalEnd[1]=20;
  intervalStart[2]=21;
  intervalEnd[2]=30;
  intervalStart[3]=31;
  intervalEnd[3]=nPoints-1;

  
  //  float sigma=0.002;

  for (int iToy=0;iToy<nToys;++iToy)
    {
      for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	{
	  int jj=ii-excl_intervals;
	  X[jj]=*(lcGraph->GetY()+ii);
	  Y[jj]=rand->Gaus(TMath::Power(TMath::Power(X[jj],1/1.52)+injDelta,1.52+injDeltaAlpha)/X[jj],sigma);
	  errorY[jj]=sigma;
	  //      std::cout << jj << "," << X[jj] << "," << Y[jj] << std::endl;
	}
      
      ROOT::Math::Functor f(&chi2,nDeltaParameters+1); 
      double step[nDeltaParameters+1];
      double variable[nDeltaParameters+1];
      for (int i=0;i<nDeltaParameters;++i)
	{
	  step[i]=0.000001;
	  variable[i]=0.5;
	}
      step[nDeltaParameters]=0.001;
      variable[nDeltaParameters]=0.5;

      min->SetFunction(f);
      
      // Set the free variables to be minimized!
      for (int i=0;i<nDeltaParameters;++i)
	{
	  TString name="delta_";
	  name+=i;
	  min->SetVariable(i,name.Data(),variable[i], step[i]);
	}
      min->SetVariable(nDeltaParameters,"delta_alpha",variable[nDeltaParameters], step[nDeltaParameters]);
      
      min->Minimize(); 
      
      const double *xs = min->X();
      const double *err_xs = min->Errors();
//       cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): " 
// 	   << chi2(xs) << " Status "  << min->Status() << endl;
      status=min->Status();

      delta_true=injDelta;
      delta_alpha_true=injDeltaAlpha;
      delta_alpha=xs[nDeltaParameters];
      err_alpha=err_xs[nDeltaParameters];
      chi2Min=min->MinValue();
      ndof=nPoints-min->NFree();
      for (int i=0;i<nDeltaParameters;++i)
	{
	  delta[i]=xs[i];
	  err_delta[i]=err_xs[i];
	}
      toyOut->Fill();
    } 
  
  toyMcFile->cd();
  toyOut->Write();
  toyMcFile->Write();

  return 0;
}
