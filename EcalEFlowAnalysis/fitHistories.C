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
 
int nPoints;
double * X;
double * Y;
double * errorX;
double * errorY;

int nDeltaParameters;
int * intervalStart;
int * intervalEnd;

int det;

int ieta;
int iphi;

int ix;
int iy;
int sign;

// is the xtal Chinese?

void findIetaIphiEB(int ixtal)
{
  ieta=(ixtal-1)/360+1;
  iphi=(ixtal-1)%360+1;
  if (ieta>85)
    ieta=ieta-85;
  else
    ieta=-ieta;
  std::cout << "Xtal @ " << ieta << "," << iphi <<  std::endl;
}
  
bool isChinese()
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



// return alpha as from the simple russian/chinese DB                                                                                    
float alphaDB()
{
  if (isChinese())
    {
      //      std::cout << "Alpha is SIC " << 1.0 << std::endl;
      return 1.0;
    }
  else
    {
      //      std::cout << "Alpha is BTCP " << 1.52 << std::endl;
      return 1.52;
    }
}

//function is (LC+delta_i)^alpha
double correction(double x, const double& par0, const double& par1){
  double  transparencyLoss=TMath::Power(x,1/alphaDB());
  //  cout<<(x/(pow((transparencyLoss),(alphaDB(109,297,1)-0.3))))<<" ";
  return (x/(TMath::Power((transparencyLoss+par0),(alphaDB()+par1))));
  //  return transparencyLoss+par[0]+0*par[1];
}

double corrFunction(int i, double x, const double* par){
  int interval=-1;
    for (int ii=0;ii<nDeltaParameters;++ii)
      {
	if (i>=intervalStart[ii] && i<= intervalEnd[ii])
	  {
	    interval=ii;
	    break;
	  }
      }
    return correction(x,par[interval],par[nDeltaParameters]);
}

double chi2(const double *par )
{
  Double_t chisq = 0;
  Double_t delta;
  //  cout<<nPoints<<endl;
  for (int i=0;i<nPoints; i++) {
    delta  = ( Y[i]*corrFunction(i,X[i],par) - 1. )/errorY[i];
    //    cout<<delta<<" ";
    chisq += delta*delta;
    //cout<<delta<<" "<<chisq<<"e ";
  }
  return chisq;
}
 
int fitHistories(
		 int nDelta=4,
		 int* intStart=0,
		 int* intEnd=0,
		 int excl_intervals=0,
		 TString historiesFile="",
		 TString fitResultsFile="fitResultsEB.root"
		 )
{
  ROOT::Math::Minimizer* min = 
    ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

  min->SetMaxFunctionCalls(1000000);
  min->SetMaxIterations(100000);
  min->SetTolerance(0.0005);
  min->SetPrintLevel(2);

  nDeltaParameters=nDelta;

  TFile* fitOut=TFile::Open(fitResultsFile,"recreate");
  fitOut->cd();

  int status,n_delta,ietaVar,iphiVar,signVar,indexVar,indexType;
  double delta_alpha,delta[nDeltaParameters], err_alpha, err_delta[nDeltaParameters], chi2Min,ndof;

  TTree* fitResults=new TTree("fitResults","fitResults");

  TString deltaBranch="delta[";
  deltaBranch+=nDeltaParameters;
  deltaBranch+="]/D";
  TString err_deltaBranch="err_delta[";
  err_deltaBranch+=nDeltaParameters;
  err_deltaBranch+="]/D";

  fitResults->Branch("n_delta", &n_delta, "n_delta/I");
  fitResults->Branch("ieta", &ietaVar, "ieta/I");
  fitResults->Branch("iphi", &iphiVar, "iphi/I");
  fitResults->Branch("sign", &signVar, "sign/I");
  fitResults->Branch("index", &indexVar, "index/I");
  fitResults->Branch("indexType", &indexType, "indexType/I");
  fitResults->Branch("delta", delta, deltaBranch);
  fitResults->Branch("err_delta", err_delta, err_deltaBranch);
  fitResults->Branch("delta_alpha", &delta_alpha, "delta_alpha/D");
  fitResults->Branch("err_alpha", &err_alpha, "err_alpha/D");
  fitResults->Branch("chi2Min", &chi2Min, "chi2Min/D");
  fitResults->Branch("ndof", &ndof, "ndof/D");
  fitResults->Branch("status", &status, "status/I");
  
  TFile* inputFile= TFile::Open(historiesFile);
  if (!inputFile)
    {
      std::cout << historiesFile << " cannot be opened " << std::endl;
      return -1;
    }



  intervalStart= new int[nDeltaParameters];
  intervalEnd= new int[nDeltaParameters];
  
  for (int i=0;i<nDeltaParameters;++i)
    {
      intervalStart[i]=intStart[i];
      intervalEnd[i]=intEnd[i];
    }


  //  
  //first four intervals for 2011 not used


  for (int ixtal=1; ixtal<=61200; ++ixtal)
    {
      //  etFile= TFile::Open("etSumOverRef_ixtal_38457.root");
      TString nameGraph="lc_ixtal_";
      nameGraph+=ixtal;
      TGraphErrors* lcGraph=(TGraphErrors*)inputFile->Get(nameGraph);
      nameGraph="etSumOverRef_ixtal_";
      nameGraph+=ixtal;
      TGraphErrors* etGraph=(TGraphErrors*)inputFile->Get(nameGraph);
      findIetaIphiEB(ixtal);
      
      if (!lcGraph || !etGraph)
	{
	  std::cout << "histories not found" << std::endl;
	  continue;
	}
      
      nPoints=lcGraph->GetN()-excl_intervals-1; //removing also last point
      //      npoints=npoints;
      //      nPoints=npoints-excl_intervals;
      //  nPoints=57;
      
      X = new double[nPoints];
      Y = new double[nPoints];
      errorX = new double[nPoints];
      errorY = new double[nPoints];
      
      for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	{
	  int jj=ii-excl_intervals;
	  X[jj]=*(lcGraph->GetY()+ii);
	  //      Y[jj]=rand->Gaus(TMath::Power(TMath::Power(X[jj],1/1.52)+injDelta,1.52+injDeltaAlpha)/X[jj],sigma);
	  Y[jj]=*(etGraph->GetY()+ii);
	  //errorY[jj]=sigma;
	  errorX[jj]=*(lcGraph->GetEY()+ii);
	  errorY[jj]=*(etGraph->GetEY()+ii);
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
      n_delta=nDeltaParameters;
      ietaVar=ieta;
      iphiVar=iphi;
      signVar=(ieta>0) ? 1 : 0;
      indexVar=ixtal;
      indexType=2;
      delta_alpha=xs[nDeltaParameters];
      err_alpha=err_xs[nDeltaParameters];
      chi2Min=min->MinValue();
      ndof=nPoints-min->NFree();

      for (int i=0;i<nDeltaParameters;++i)
	{
	  delta[i]=xs[i];
	  err_delta[i]=err_xs[i];
	}
      fitResults->Fill();
  
      fitOut->cd();

      //Writing corrected laser correction
      for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	{
	  int jj=ii-excl_intervals;

	  Y[jj]=(*(lcGraph->GetY()+ii))/corrFunction(jj,*(lcGraph->GetY()+ii),xs);
	  errorY[jj]=0;
	  X[jj]=*(lcGraph->GetX()+ii);
	  //errorY[jj]=sigma;
	  errorX[jj]=*(lcGraph->GetEX()+ii);
	}

      TGraphErrors* corr_lc=new TGraphErrors(nPoints,X,Y,errorX,errorY);
      corr_lc->SetName(TString(lcGraph->GetName())+"_corr");
      corr_lc->Write();
      //Writing corrected et correction
      for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	{
	  int jj=ii-excl_intervals;
	  Y[jj]=(*(etGraph->GetY()+ii))*corrFunction(jj,*(lcGraph->GetY()+ii),xs);
	  errorY[jj]=*(etGraph->GetEY()+ii);
	  X[jj]=*(lcGraph->GetX()+ii);
	  //errorY[jj]=sigma;
	  errorX[jj]=*(lcGraph->GetEX()+ii);
	}

      TGraphErrors* corr_et=new TGraphErrors(nPoints,X,Y,errorX,errorY);
      corr_et->SetName(TString(etGraph->GetName())+"_corr");
      corr_et->Write();

      lcGraph->Write();
      etGraph->Write();

      delete X; 
      delete Y; 
      delete errorX;
      delete errorY;

    }

  fitResults->Write();
  fitOut->Write();

  delete intervalStart;
  delete intervalEnd;

  return 0;
}
