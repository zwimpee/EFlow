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

double * X;
double * Y;
double * errorX;
double * errorY;

int nPoints;
int nDeltaParameters;
int nXtalsInRegion;

int * intervalStart;
int * intervalEnd;

int det;

int* ieta;
int* iphi;
int* badXtals;

int* ix;
int* iy;
int* sign;

#define kBarlWedges 360

// return an index between 1 and 61200
int iXtal(int eta, int phi, int isign)
{
  int etaIndex=eta-1;
  if (isign==1)
    etaIndex+=85;
  return (etaIndex*kBarlWedges+phi);
}

void findXtalsinTT(int iTT, int *xtalsInReg)
{
  int smTT=(iTT-1)%68+1;
  int sm=(iTT-1)/68+1;
  int ie=(smTT-1)/4+1;
  int ip=(smTT-1)%4+1;
  int sig=1;
  if (sm>18)
    sig=0;
  //  std::cout << sm << "," << smTT << "," << ie << "," << ip << "," << sig << std::endl;
  int ixtal=0; 
  for (int i1=(ie-1)*5+1;i1<=ie*5;++i1)
    for (int i2=(((sm-1)%18))*20+(ip-1)*5+1;i2<=(((sm-1)%18))*20+(ip*5);++i2)
      {
	xtalsInReg[ixtal]=iXtal(i1,i2,sig);
	++ixtal;
      }
}

void findXtalsinHV(int iHV, int *xtalsInReg)
{
  findXtalsinTT((iHV-1)*2+1,&xtalsInReg[0]);
  findXtalsinTT((iHV-1)*2+2,&xtalsInReg[25]);
}

void findIetaIphiEB(int index, int ixtal)
{
  ieta[index]=(ixtal-1)/360+1;
  iphi[index]=(ixtal-1)%360+1;
  if (ieta[index]>85)
    ieta[index]=ieta[index]-85;
  else
    ieta[index]=-ieta[index];
  std::cout << "+++++ Xtal @ " << ieta[index] << "," << iphi[index] <<  std::endl;
}
  
bool isChinese(int index)
{
  if( (ieta[index]>-76 ) && (ieta[index]<76) )
    return false;

  if(ieta[index]>75){
    if(iphi[index]>300 && iphi[index]<=340) return true;
    if(iphi[index]>120 && iphi[index]<=140) return true;
    if(iphi[index]>231 && iphi[index]<=240) return true;
    if(ieta[index]>=81){
      if(iphi[index]>220 && iphi[index]<=240) return true;
      if(iphi[index]>180 && iphi[index]<=200) return true;
    }
  }

  if(ieta[index]<-76)
    {
      if(iphi[index]>200 && iphi[index]<=220) return true;
      if(iphi[index]>240 && iphi[index]<=300) return true;
      if(ieta[index]<= -81){
	if(iphi[index]>20 && iphi[index]<=40) return true;
	if(iphi[index]>60 && iphi[index]<=100) return true;
      }
    }
  return false;
}



// return alpha as from the simple russian/chinese DB                                                                                    
float alphaDB(int index)
{
  if (isChinese(index))
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
double correction(int index, double x, const double& par0, const double& par1){
  double  transparencyLoss=TMath::Power(x,1/alphaDB(index));
  if (transparencyLoss<0.01 || transparencyLoss>2. )
    {
      std::cout << "***** RETURNING 1. TO AVOID MORE ISSUES" << std::endl;
      return 1.;
    }
  //  cout<<(x/(pow((transparencyLoss),(alphaDB(109,297,1)-0.3))))<<" ";
  return (x/(TMath::Power((transparencyLoss+par0),(alphaDB(index)+par1))));
  //  return transparencyLoss+par[0]+0*par[1];
}

double corrFunction(int index, int i, double x, const double* par){
  int interval=-1;
    for (int ii=0;ii<nDeltaParameters;++ii)
      {
	if (i>=intervalStart[ii] && i<= intervalEnd[ii])
	  {
	    interval=ii;
	    break;
	  }
      }
    return correction(index,x,par[interval],par[nDeltaParameters+index]);
}

double chi2(const double *par )
{
  Double_t chisq = 0;
  Double_t delta;
  //  cout<<nPoints<<endl;
  for (int ixtal=0;ixtal<nXtalsInRegion; ixtal++) 
    {
      if (badXtals[ixtal]>0)
	continue;
      for (int i=0;i<nPoints; i++) 
	{
	  
	  //skipping bad points
	  if (errorY[ixtal*nPoints+i]==0)
	      continue;
	  
	  delta  = ( Y[ixtal*nPoints+i] *corrFunction(ixtal,i, X[ixtal*nPoints+i],par) - 1. )/errorY[ixtal*nPoints+i];

	  if ( delta != delta )
	    continue;
	  
	  //    cout<<delta<<" ";
	  chisq += delta*delta;
	  //cout<<delta<<" "<<chisq<<"e ";
	}
    }
  return chisq;
}
 
int fitHistories(
		 int nDelta=4,
		 int* intStart=0,
		 int* intEnd=0,
		 int excl_intervals=0,
		 int * fixedPar=0,
		 double *startValue=0,
		 double *stepValue=0,
		 TString regionType="xtal",
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

  int status,n_delta,ietaVar,iphiVar,signVar,indexVar,indexType,badXtalVar;
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
  fitResults->Branch("badXtal", &badXtalVar, "badXtalVar/I");
  fitResults->Branch("index", &indexVar, "index/I");
  fitResults->Branch("indexType", &indexType, "indexType/I");
  fitResults->Branch("delta", delta, deltaBranch);
  fitResults->Branch("err_delta", err_delta, err_deltaBranch);
  fitResults->Branch("delta_alpha", &delta_alpha, "delta_alpha/D");
  fitResults->Branch("err_alpha", &err_alpha, "err_alpha/D");
  fitResults->Branch("chi2Min", &chi2Min, "chi2Min/D");
  fitResults->Branch("ndof", &ndof, "ndof/D");
  fitResults->Branch("status", &status, "status/I");
  
  std::cout << "Opening " << historiesFile << std::endl;

  TFile* inputFile= TFile::Open(historiesFile);
  if (!inputFile)
    {
      std::cout << historiesFile << " cannot be opened " << std::endl;
      return -1;
    }

  intervalStart= new int[nDeltaParameters];
  intervalEnd= new int[nDeltaParameters];
  
  std::cout << "RegionType is " << regionType << std::endl;

  for (int i=0;i<nDeltaParameters;++i)
    {
      std::cout << "INTERVAL " << i << "[" << intStart[i] << "-" << intEnd[i] << "]" << std::endl;
      intervalStart[i]=intStart[i];
      intervalEnd[i]=intEnd[i];
    }

  //  
  //first four intervals for 2011 not used

  int nRegions;
  if (regionType == "xtal" )
    nRegions=61200;
  else if (regionType == "TT" )
    nRegions=36*68;
  else if (regionType == "HV" )
    nRegions=36*34;
  else 
    {
      std::cout << "RegionType is unknown" << std::endl;
      return -1;
    }
  
  for (int ireg=0;ireg<nRegions;++ireg)
    {
      if (regionType == "xtal")
	nXtalsInRegion=1;
      else if (regionType == "TT")
	nXtalsInRegion=25;
      else if (regionType == "HV")
	nXtalsInRegion=50;
      
      int xtalsInRegion[nXtalsInRegion];
      TGraphErrors* lcGraph[nXtalsInRegion];
      TGraphErrors* etGraph[nXtalsInRegion];

      std::cout << "iregion " << ireg+1 << std::endl;
      
      if (regionType == "xtal")
	xtalsInRegion[0]=ireg+1;
      else if (regionType == "TT")
	findXtalsinTT(ireg+1,&xtalsInRegion[0]);
      else if (regionType == "HV")
	findXtalsinHV(ireg+1,&xtalsInRegion[0]);

      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	{
	  //  etFile= TFile::Open("etSumOverRef_ixtal_38457.root");
	  TString nameGraph="lc_ixtal_";
	  nameGraph+=xtalsInRegion[ixtal];
	  //	  std::cout << nameGraph << std::endl;
	  lcGraph[ixtal]=(TGraphErrors*)inputFile->Get(nameGraph);
	  nameGraph="etSumOverRef_ixtal_";
	  nameGraph+=xtalsInRegion[ixtal];
	  etGraph[ixtal]=(TGraphErrors*)inputFile->Get(nameGraph);
      
	  if (!lcGraph[ixtal] || !etGraph[ixtal])
	    {
	      std::cout << "histories not found" << std::endl;
	      continue;
	    }
      
	}

      nPoints=lcGraph[0]->GetN()-excl_intervals-1; //removing also last point
  
      //      npoints=npoints;
      //      nPoints=npoints-excl_intervals;
      //  nPoints=57;
  
      X = new double[nXtalsInRegion*nPoints];
      Y = new double[nXtalsInRegion*nPoints];
      errorX = new double[nXtalsInRegion*nPoints];
      errorY = new double[nXtalsInRegion*nPoints];
      ieta = new int[nXtalsInRegion];
      iphi = new int[nXtalsInRegion];
      badXtals = new int[nXtalsInRegion];

      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	badXtals[ixtal]=0;

      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	{
	  findIetaIphiEB(ixtal,xtalsInRegion[ixtal]);
	  for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	    {
	      int jj=ii-excl_intervals;
	      if ( 
		  *(lcGraph[ixtal]->GetY()+ii)<0.1 || *(lcGraph[ixtal]->GetY()+ii)>1.9 || *(etGraph[ixtal]->GetY()+ii)<0.1 || *(etGraph[ixtal]->GetY()+ii)>1.9 || TMath::Abs(*(lcGraph[ixtal]->GetEY()+ii))>0.05 
		   || TMath::Abs(*(etGraph[ixtal]->GetEY()+ii))>0.05 || *(etGraph[ixtal]->GetEY()+ii)==0. 
		   || *(lcGraph[ixtal]->GetY()+ii) != *(lcGraph[ixtal]->GetY()+ii) 
		   || *(etGraph[ixtal]->GetY()+ii) != *(etGraph[ixtal]->GetY()+ii)
		   )
		{ 
		  std::cout << "+++++ FOUND A BAD XTAL " << xtalsInRegion[ixtal] << ". Excluding it ++++++ " << std::endl;
		  badXtals[ixtal]=1;
		}
	      else
		{
		  X[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetY()+ii);
		  //      Y[jj]=rand->Gaus(TMath::Power(TMath::Power(X[jj],1/1.52)+injDelta,1.52+injDeltaAlpha)/X[jj],sigma);
		  Y[ixtal*nPoints+jj]=*(etGraph[ixtal]->GetY()+ii);
		  //errorY[jj]=sigma;
		  errorX[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetEY()+ii);
		  errorY[ixtal*nPoints+jj]=*(etGraph[ixtal]->GetEY()+ii);
		}
	      //      std::cout << jj << "," << X[jj] << "," << Y[jj] << std::endl;
	    }
	}
      
      int goodXtals=0;
      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	{
	  if (badXtals[ixtal]>0)
	    {
	      for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
		{
		  int jj=ii-excl_intervals;
		  X[ixtal*nPoints+jj]=1.;
		  Y[ixtal*nPoints+jj]=1.;
		  errorX[ixtal*nPoints+jj]=0.;
		  errorY[ixtal*nPoints+jj]=0.;
		}
	    }
	  else
	    goodXtals++;
	}

      ROOT::Math::Functor f(&chi2,nDeltaParameters+nXtalsInRegion); 
      
      min->SetFunction(f);
      
      // Set the free variables to be minimized!
      for (int i=0;i<nDeltaParameters;++i)
	{
	  TString name="delta_";
	  name+=i;
	  std::cout << name << "," << fixedPar[i] << "," << startValue[i] << "," << stepValue[i] << std::endl; 
	  if (!fixedPar[i])
	    min->SetLimitedVariable(i,name.Data(),startValue[i], stepValue[i],-0.05,0.05);
	  else
	    min->SetFixedVariable(i,name.Data(),startValue[i]);
	  //min->SetFixedVariable(i,name.Data(),variable[i]);
	}
      
      for (int ixtal=0;ixtal<nXtalsInRegion;++ixtal)
	{
	  TString name="delta_alpha_";
	  name+=ixtal;
	  std::cout << name << "," << fixedPar[nDeltaParameters+ixtal] << "," << startValue[nDeltaParameters+ixtal] << "," << stepValue[nDeltaParameters+ixtal] << std::endl; 
	  if (!fixedPar[nDeltaParameters+ixtal] && badXtals[ixtal]==0 )
	    min->SetLimitedVariable(nDeltaParameters+ixtal,name.Data(),startValue[nDeltaParameters+ixtal], stepValue[nDeltaParameters+ixtal],-1.52,5.);
	  else
	    min->SetFixedVariable(nDeltaParameters+ixtal,name.Data(),startValue[nDeltaParameters+ixtal]);
	}
      //      min->SetFixedVariable(nDeltaParameters,"delta_alpha",variable[nDeltaParameters]);
  
      min->Minimize(); 

      const double *xs = min->X();
      const double *err_xs = min->Errors();  
      status=min->Status();
      n_delta=nDeltaParameters;
      indexType=2;
      for (int i=0;i<nDeltaParameters;++i)
	{
	  delta[i]=xs[i];
	  err_delta[i]=err_xs[i];
	}
      chi2Min=min->MinValue();
      ndof=nPoints*goodXtals-min->NFree();
      for (int ixtal=0;ixtal<nXtalsInRegion;++ixtal)
	{
	  //       cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): " 
	  // 	   << chi2(xs) << " Status "  << min->Status() << endl;
  
	  ietaVar=ieta[ixtal];
	  iphiVar=iphi[ixtal];
	  signVar=(ieta>0) ? 1 : 0;
	  badXtalVar=badXtals[ixtal];
	  indexVar=xtalsInRegion[ixtal];
	  delta_alpha=xs[nDeltaParameters+ixtal];
	  err_alpha=err_xs[nDeltaParameters+ixtal];

	  fitResults->Fill();
      
	  fitOut->cd();
      
	  //Writing corrected laser correction
	  for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	    {
	      int jj=ii-excl_intervals;

	      Y[ixtal*nPoints+jj]=(*(lcGraph[ixtal]->GetY()+ii))/corrFunction(ixtal,jj,*(lcGraph[ixtal]->GetY()+ii),xs);
	      errorY[ixtal*nPoints+jj]=0;
	      X[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetX()+ii);
	      //errorY[jj]=sigma;
	      errorX[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetEX()+ii);
	    }

	  TGraphErrors* corr_lc=new TGraphErrors(nPoints,&X[ixtal*nPoints],&Y[ixtal*nPoints],&errorX[ixtal*nPoints],&errorY[ixtal*nPoints]);
	  corr_lc->SetName(TString(lcGraph[ixtal]->GetName())+"_corr");
	  corr_lc->Write();
	  //Writing corrected et correction
	  for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	    {
	      int jj=ii-excl_intervals;
	      Y[ixtal*nPoints+jj]=(*(etGraph[ixtal]->GetY()+ii))*corrFunction(ixtal,jj,*(lcGraph[ixtal]->GetY()+ii),xs);
	      errorY[ixtal*nPoints+jj]=*(etGraph[ixtal]->GetEY()+ii);
	      X[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetX()+ii);
	      //errorY[ixtal*nPoints+jj]=sigma;
	      errorX[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetEX()+ii);
	    }

	  TGraphErrors* corr_et=new TGraphErrors(nPoints,&X[ixtal*nPoints],&Y[ixtal*nPoints],&errorX[ixtal*nPoints],&errorY[ixtal*nPoints]);
	  corr_et->SetName(TString(etGraph[ixtal]->GetName())+"_corr");
	  corr_et->Write();

	  lcGraph[ixtal]->Write();
	  etGraph[ixtal]->Write();
	}

      delete X; 
      delete Y; 
      delete errorX;
      delete errorY;
      delete ieta;
      delete iphi;
      delete badXtals;
  
    }//end of loop over regions
  
  
  fitResults->Write();
  fitOut->Write();
  
  delete intervalStart;
  delete intervalEnd;
  
  return 0;
}
