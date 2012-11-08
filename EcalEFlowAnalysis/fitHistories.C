#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include <iostream>
#include <algorithm>

#define MAXHITS 1000
#define kBarlRings 85
#define kEndcRings 39

#define kBarlWedges 360
#define kEndcWedgesX 100
#define kEndcWedgesY 100

#define kSides 2

#define kTowerPerSM 68
#define kXtalPerSM 1700
#define kSM 36

#define kEndcSCs 624
#define kEndcXtals 14648

#define alphaFromHistory

using namespace std;

double * X;
double * Y;
double * Z;
double * errorX;
double * errorY;
double * errorZ;
int * excludedPoint;

int nPoints;
int nDeltaParameters;
int nXtalsInRegion;

int * intervalStart;
int * intervalEnd;

int det;

int* ieta;
int* iphi;
int* badXtals;

// int* ix;
// int* iy;
int* sign;

int endcapRings[kEndcWedgesX][kEndcWedgesY][kSides];
int endcapXtals[kEndcWedgesX][kEndcWedgesY][kSides];
int endcapSCs[kEndcWedgesX][kEndcWedgesY][kSides];
int endcapLMs[kEndcWedgesX][kEndcWedgesY][kSides];

int oneAlphaPerRegion;


bool iterativeFit;
bool excludeHighChi2Points;
float chi2CutForIterativeFit;
int nPointsUsed;

// return an index between 1 and 61200
int iXtal(int eta, int phi, int isign)
{
  int etaIndex=eta-1;
  if (isign==1)
    etaIndex+=85;
  return (etaIndex*kBarlWedges+phi);
}


// return an index between 1 and 36
int iSM(int eta, int phi, int isign)
{
  int id = ( phi - 1 ) / 20 + 1;
  if ( isign == 0 ) id += 18;
  return id;
}

// return an index between 1 and 68
int iTower(int eta, int phi, int isign)
{
  int ttEta=(eta-1)/5+1;
  int ttPhi=((phi-1)%20)/5+1;
  return (ttEta-1)*4+ttPhi;
}

// return an index between 1 and 2448
int iTT(int eta, int phi, int isign)
{
  return (iSM(eta,phi,isign)-1)*68+iTower(eta,phi,isign);
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


void findXtalsinSC(int iSC, int *xtalsInReg)
{
  nXtalsInRegion=0;
  for (int isign=0;isign<kSides;++isign)
    for (int ix=0;ix<kEndcWedgesX;++ix)
      for (int iy=0;iy<kEndcWedgesX;++iy)
	{
	  if (endcapSCs[ix][iy][isign]==iSC)
	    {
	      std::cout << "+++++ " << nXtalsInRegion << " EE Xtal @ " << ix+1 << "," << iy+1 <<  std::endl;
	      xtalsInReg[nXtalsInRegion]=endcapXtals[ix][iy][isign]+1;
 	      ieta[nXtalsInRegion]=ix+1;
 	      iphi[nXtalsInRegion]=iy+1;
 	      sign[nXtalsInRegion]=isign;
	      nXtalsInRegion++;
	    }
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

void findIetaIphiEE(int index, int ixtal)
{
  for (int isign=0;isign<kSides;++isign)
    for (int ix=0;ix<kEndcWedgesX;++ix)
      for (int iy=0;iy<kEndcWedgesX;++iy)
	{
	  if (endcapXtals[ix][iy][isign]==ixtal-1)
	    {
	      ieta[index]=ix+1;
	      iphi[index]=iy+1;
	      sign[index]=isign;
	      break;
	    }
	}
  std::cout << "+++++ EE Xtal @ " << ieta[index] << "," << iphi[index] <<  std::endl;
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
double correction(int index, double x, const double& par0, const double& par1, float alpha=0){
  float alphaXtal;
  if (alpha==0)
    alphaXtal=alphaDB(index);
  else
    alphaXtal=alpha;

  double  transparencyLoss=TMath::Power(x,1/alphaXtal);
  if (transparencyLoss<0.01 || transparencyLoss>2. )
    {
      std::cout << "***** RETURNING 1. TO AVOID MORE ISSUES" << std::endl;
      return 1.;
    }
  //  cout<<(x/(pow((transparencyLoss),(alphaDB(109,297,1)-0.3))))<<" ";
  return (x/(TMath::Power((transparencyLoss+par0),(alphaXtal+par1))));
  //  return transparencyLoss+par[0]+0*par[1];
}

double corrFunction(int index=0, int i=0, double x=0, const double* par=0, float alpha=0){
  int interval=-1;
    for (int ii=0;ii<nDeltaParameters;++ii)
      {
	if (i>=intervalStart[ii] && i<= intervalEnd[ii])
	  {
	    interval=ii;
	    break;
	  }
      }
    if (!oneAlphaPerRegion)
      return correction(index,x,par[interval],par[nDeltaParameters+index],alpha);
    else
      return correction(index,x,par[interval],par[nDeltaParameters],alpha);
}

double chi2(const double *par )
{
  Double_t chisq = 0;
  Double_t delta;
  nPointsUsed=0;
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
#ifndef alphaFromHistory	  
	  delta  = ( Y[ixtal*nPoints+i] *corrFunction(ixtal,i, X[ixtal*nPoints+i],par,0) - 1. )/errorY[ixtal*nPoints+i];
#else
	  delta  = ( Y[ixtal*nPoints+i] *corrFunction(ixtal,i, X[ixtal*nPoints+i],par,Z[ixtal*nPoints+i]) - 1. )/errorY[ixtal*nPoints+i];
#endif
	  
	  if ( delta != delta )
	    continue;

	  if (excludeHighChi2Points && excludedPoint[ixtal*nPoints+i])
	    continue;

	  nPointsUsed++;
	  //    cout<<delta<<" ";
	  chisq += delta*delta;
	  //cout<<delta<<" "<<chisq<<"e ";
	}
    }
  return chisq;
}

void excludePoints(const double *par )
{
  Double_t delta;
  for (int ixtal=0;ixtal<nXtalsInRegion; ixtal++) 
    {
      if (badXtals[ixtal]>0)
	continue;
      for (int i=0;i<nPoints; i++) 
	{
	  
	  //skipping bad points
	  if (errorY[ixtal*nPoints+i]==0)
	      continue;
#ifndef alphaFromHistory	  
	  delta  = ( Y[ixtal*nPoints+i] *corrFunction(ixtal,i, X[ixtal*nPoints+i],par,0) - 1. )/errorY[ixtal*nPoints+i];
#else
	  delta  = ( Y[ixtal*nPoints+i] *corrFunction(ixtal,i, X[ixtal*nPoints+i],par,Z[ixtal*nPoints+i]) - 1. )/errorY[ixtal*nPoints+i];
#endif
	  
	  if ( delta != delta )
	    continue;

	  if (excludeHighChi2Points && delta*delta>chi2CutForIterativeFit)
	    excludedPoint[ixtal*nPoints+i]=1;

	}
    }
}


void readEEIndices(TString& eeIndicesFile)
{
  std::cout << "Opening eeIndicesMap  " << eeIndicesFile << std::endl;
  TFile *fee=TFile::Open(eeIndicesFile);
  if (!fee->IsOpen())
    std::cout << "Cannot open " << eeIndicesFile << std::endl;
  TTree* eeIndicesTree= (TTree*)fee->Get("eeIndices");

  int ixVar,iyVar,signVar,iringVar,ixtalVar,iSCVar,iLMVar;
  TBranch *b_ix=eeIndicesTree->GetBranch("ix");
  TBranch *b_iy=eeIndicesTree->GetBranch("iy");
  TBranch *b_sig=eeIndicesTree->GetBranch("sign");
  TBranch *b_iring=eeIndicesTree->GetBranch("iring");
  TBranch *b_ixtal=eeIndicesTree->GetBranch("ixtal");
  TBranch *b_isc=eeIndicesTree->GetBranch("isc");
  TBranch *b_ilm=eeIndicesTree->GetBranch("ilm");

  eeIndicesTree->SetBranchAddress("ix", &ixVar, &b_ix);
  eeIndicesTree->SetBranchAddress("iy", &iyVar, &b_iy);
  eeIndicesTree->SetBranchAddress("sign", &signVar, &b_sig);
  eeIndicesTree->SetBranchAddress("iring", &iringVar, &b_iring);
  eeIndicesTree->SetBranchAddress("ixtal", &ixtalVar, &b_ixtal);
  eeIndicesTree->SetBranchAddress("isc", &iSCVar, &b_isc);
  eeIndicesTree->SetBranchAddress("ilm", &iLMVar, &b_ilm);

  int nentries_ee = eeIndicesTree->GetEntries();

  for (int isign=0;isign<kSides;++isign)
    for (int ix=0;ix<kEndcWedgesX;++ix)
      for (int iy=0;iy<kEndcWedgesX;++iy)
	{
	  endcapRings[ix][iy][isign]=-1;
	  endcapXtals[ix][iy][isign]=-1;
	  endcapSCs[ix][iy][isign]=-1;
	  endcapLMs[ix][iy][isign]=-1;
	}

  std::cout << "Found mapping for " << nentries_ee << " xtals " << std::endl;

  for(int jentry=0;jentry<nentries_ee;++jentry)
    {
      eeIndicesTree->GetEntry(jentry);
      endcapRings[ixVar-1][iyVar-1][signVar]=iringVar;
      endcapXtals[ixVar-1][iyVar-1][signVar]=ixtalVar;
      endcapSCs[ixVar-1][iyVar-1][signVar]=iSCVar;
      endcapLMs[ixVar-1][iyVar-1][signVar]=iLMVar;
    }
}
 
int fitHistories(
		 int nDelta=4,
		 int* intStart=0,
		 int* intEnd=0,
		 int excl_intervals=0,
		 int nPointsForFit=999999,
		 int * fixedPar=0,
		 double *startValue=0,
		 double *stepValue=0,
		 double errScaleFactor=1.,
		 int alphaPerRegion=0,
		 int toy=0,
		 int useDataPointForToy=0,
		 float toy_delta=0.,
		 float toy_shift=0.,
		 float toy_sigma=0.006,
		 int iterative=0,
		 float chi2Cut=9999.,
		 float minChi2=2.3,
		 TString regionType="xtal",
		 TString historiesFile="",
		 TString fitResultsFile="fitResultsEB.root",
		 TString eeIndicesMap="data/eeIndicesMap.root"
		 )
{
  oneAlphaPerRegion=alphaPerRegion;
  iterativeFit=iterative;
  chi2CutForIterativeFit=chi2Cut;

  readEEIndices(eeIndicesMap);

  if (toy)
    {
      std::cout << "[INFO]: This is just for toys experiments.\n Generating toys with average shift " << toy_delta << " and error on each measurement " << toy_sigma << std::endl; 
    }
  TRandom3 gen(0);
  
  ROOT::Math::Minimizer* min = 
    ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");

  min->SetMaxFunctionCalls(1000000);
  min->SetMaxIterations(100000);
  min->SetTolerance(0.0005);
  min->SetPrintLevel(2);

  nDeltaParameters=nDelta;

  TFile* fitOut=TFile::Open(fitResultsFile,"recreate");
  fitOut->cd();

  int status,n_delta,ietaVar,iphiVar,signVar,indexVar,index1Var,index2Var,indexType,badXtalVar;
  double alpha0,delta_alpha,delta[nDeltaParameters], err_alpha, err_delta[nDeltaParameters], chi2Min,ndof;


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
  fitResults->Branch("index1", &index1Var, "index1/I");
  fitResults->Branch("index2", &index2Var, "index2/I");
  fitResults->Branch("indexType", &indexType, "indexType/I");
  fitResults->Branch("delta", delta, deltaBranch);
  fitResults->Branch("err_delta", err_delta, err_deltaBranch);
  fitResults->Branch("alpha0", &alpha0, "alpha0/D");
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
    //    nRegions=10000;
  else if (regionType == "eextal" )
    nRegions=14648;
  else if (regionType == "TT" )
    nRegions=36*68;
  else if (regionType == "HV" )
    nRegions=36*34;
  else if (regionType == "EESC" )
    nRegions=628;
  else 
    {
      std::cout << "RegionType is unknown" << std::endl;
      return -1;
    }
  
  for (int ireg=0;ireg<nRegions;++ireg)
    {
      if (regionType == "xtal")
	nXtalsInRegion=1;
      if (regionType == "eextal")
	nXtalsInRegion=1;
      else if (regionType == "TT")
	nXtalsInRegion=25;
      else if (regionType == "HV")
	nXtalsInRegion=50;
      else if (regionType == "EESC")
	nXtalsInRegion=25;
      
      int xtalsInRegion[nXtalsInRegion];
      TGraphErrors* lcGraph[nXtalsInRegion];
#ifdef alphaFromHistory
      TGraphErrors* alphaGraph[nXtalsInRegion];
#endif
      TGraphErrors* etGraph[nXtalsInRegion];

      std::cout << "iregion " << ireg+1 << std::endl;

      ieta = new int[nXtalsInRegion];
      iphi = new int[nXtalsInRegion];
      sign = new int[nXtalsInRegion];
      badXtals = new int[nXtalsInRegion];        

      det=0; //EB
      TString xtalType="ixtal_"; //EB
      if (regionType == "xtal")
	xtalsInRegion[0]=ireg+1;
      else if (regionType == "eextal")
	{
	  xtalsInRegion[0]=ireg+1;
	  xtalType="iendcxtal_";
	  det=1;
	}
      else if (regionType == "TT")
	findXtalsinTT(ireg+1,&xtalsInRegion[0]);
      else if (regionType == "HV")
	findXtalsinHV(ireg+1,&xtalsInRegion[0]);
      else if (regionType == "EESC")
	{
	  findXtalsinSC(ireg+1,&xtalsInRegion[0]);
	  xtalType="iendcxtal_";
	  det=1;
	}

      std::cout << "[INFO]::xtals in this region " << nXtalsInRegion << std::endl;

      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	badXtals[ixtal]=0;      

      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	{
	  //  etFile= TFile::Open("etSumOverRef_ixtal_38457.root");
	  TString nameGraph="lc_";
	  nameGraph+=xtalType;
	  nameGraph+=xtalsInRegion[ixtal];
	  //	  std::cout << nameGraph << std::endl;
	  lcGraph[ixtal]=(TGraphErrors*)inputFile->Get(nameGraph);
	  nameGraph="etSumOverRef_";
	  nameGraph+=xtalType;
	  nameGraph+=xtalsInRegion[ixtal];
	  etGraph[ixtal]=(TGraphErrors*)inputFile->Get(nameGraph);

#ifdef alphaFromHistory
	  nameGraph="alpha_";
	  nameGraph+=xtalType;
	  nameGraph+=xtalsInRegion[ixtal];
	  alphaGraph[ixtal]=(TGraphErrors*)inputFile->Get(nameGraph);
#endif

#ifndef alphaFromHistory      
	  if (!lcGraph[ixtal] || !etGraph[ixtal])
#else
	  if (!lcGraph[ixtal] || !etGraph[ixtal] || !alphaGraph[ixtal])
#endif
	    {
	      std::cout << ixtal << " ixtal histories not found " << std::endl;
	      badXtals[ixtal]=1;
	      continue;
	    }
	  
	}
      
      
      for (int ixtal=0;ixtal<nXtalsInRegion;++ixtal)
	{
	  if (badXtals[ixtal]==1)
	    continue;
	  nPoints=std::min(lcGraph[ixtal]->GetN()-excl_intervals,nPointsForFit); //removing also last point
	  break;
	}
      
      X = new double[nXtalsInRegion*nPoints];
      Y = new double[nXtalsInRegion*nPoints];
      excludedPoint = new int[nXtalsInRegion*nPoints];
 
#ifdef alphaFromHistory
      Z = new double[nXtalsInRegion*nPoints];
#endif
      errorX = new double[nXtalsInRegion*nPoints];
      errorY = new double[nXtalsInRegion*nPoints];
#ifdef alphaFromHistory
      errorZ = new double[nXtalsInRegion*nPoints];
#endif

      //      npoints=npoints;
      //      nPoints=npoints-excl_intervals;
      //  nPoints=57;
  
      float xtal_shift[nDeltaParameters];
      if (toy==1)
	{
	  for (int id=0;id<nDeltaParameters;++id)
	    {
	      xtal_shift[id]=gen.Gaus()*toy_shift;
	      std::cout << "RANDOM SHIFT FOR THIS REGION IS " << xtal_shift[id] << std::endl;
	    }
	}

      for (int ixtal=0; ixtal<nXtalsInRegion; ++ixtal)
	{
	  if (det==0)
	    findIetaIphiEB(ixtal,xtalsInRegion[ixtal]);
	  if (det==1 && nXtalsInRegion==1)
	    findIetaIphiEE(ixtal,xtalsInRegion[ixtal]);
	  if (badXtals[ixtal]==1)
	    continue;
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
		  *(&X[0]+ixtal*nPoints+jj)=*(lcGraph[ixtal]->GetY()+ii);
		  errorX[ixtal*nPoints+jj]=*(lcGraph[ixtal]->GetEY()+ii);
		  //      Y[jj]=rand->Gaus(TMath::Power(TMath::Power(X[jj],1/1.52)+injDelta,1.52+injDeltaAlpha)/X[jj],sigma);
		  if (toy == 0 )
		    {
		      Y[ixtal*nPoints+jj]=*(etGraph[ixtal]->GetY()+ii);
		      errorY[ixtal*nPoints+jj]=*(etGraph[ixtal]->GetEY()+ii)*errScaleFactor;
		    }
		  else
		    {
		      float shift=0.;
		      if (toy==1)
			{
			  int interval=-1;
			  for (int id=0;id<nDeltaParameters;++id)
			    {
			      if (jj>=intervalStart[id] && jj<= intervalEnd[id])
				{
				  interval=id;
				  break;
				}
			    }
			  shift=xtal_shift[interval];
			}
		      float data=1;
		      float err_data=toy_sigma;
		      if (useDataPointForToy==1)
			{
			  data=*(etGraph[ixtal]->GetY()+ii);
			  err_data=*(etGraph[ixtal]->GetEY()+ii)*errScaleFactor;
			  err_data=TMath::Sqrt(err_data*err_data+toy_sigma*toy_sigma);
			}
#ifndef alphaFromHistory
			  Y[ixtal*nPoints+jj]=data/correction(xtalsInRegion[ixtal], X[ixtal*nPoints+jj], shift, toy_delta, 0.) + gen.Gaus()*toy_sigma;
#else
			  Y[ixtal*nPoints+jj]=data/correction(xtalsInRegion[ixtal], X[ixtal*nPoints+jj], shift, toy_delta, *(alphaGraph[ixtal]->GetY()+ii)) +gen.Gaus()*toy_sigma;
#endif
			  errorY[ixtal*nPoints+jj]=err_data;

		    }
		  //errorY[jj]=sigma;


#ifdef alphaFromHistory
		  Z[ixtal*nPoints+jj]=*(alphaGraph[ixtal]->GetY()+ii);
		  //Z[ixtal*nPoints+jj]=1.;
		  //		  std::cout << "+++++++ " << ixtal << " jj " << jj << " " << &Z[ixtal*nPoints+jj] << " " << Z[ixtal*nPoints+jj] << " " << &X[ixtal*nPoints+jj] << " " << &Y[ixtal*nPoints+jj] << std::endl;
		  
		  errorZ[ixtal*nPoints+jj]=*(alphaGraph[ixtal]->GetEY()+ii);
#endif
		  excludedPoint[ixtal*nPoints+jj]=0;
		}
	      //      std::cout << jj << "," << X[jj] << "," << Y[jj] << std::endl;
	    }
	}

      //      std::cout << "+++++++ " << 0 << " jj " << 0 << " " << &Z[0] << " " << Z[0] << std::endl;
      
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
#ifdef alphaFromHistory
		  Z[ixtal*nPoints+jj]=1.;
		  errorZ[ixtal*nPoints+jj]=0.;
#endif
		}
	    }
	  else
	    goodXtals++;
	}

      int nTotPars=nDeltaParameters;
      if (!oneAlphaPerRegion)
	nTotPars+=nXtalsInRegion;
      else
	nTotPars++;

      ROOT::Math::Functor f(&chi2,nTotPars);
      min->SetFunction(f);
      
      // Set the free variables to be minimized!
      for (int i=0;i<nDeltaParameters;++i)
	{
	  TString name="delta_";
	  name+=i;
	  std::cout << name << "," << fixedPar[i] << "," << startValue[i] << "," << stepValue[i] << std::endl; 
	  if (!fixedPar[i])
	    min->SetLimitedVariable(i,name.Data(),startValue[i], stepValue[i],-0.2,0.2);
	  else
	    min->SetFixedVariable(i,name.Data(),startValue[i]);
	  //min->SetFixedVariable(i,name.Data(),variable[i]);
	}
      
      if (!oneAlphaPerRegion)
	{
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
	}
      else
	{
	  TString name="delta_alpha";
	  std::cout << name << "," << fixedPar[nDeltaParameters] << "," << startValue[nDeltaParameters] << "," << stepValue[nDeltaParameters] << std::endl; 
	  if (!fixedPar[nDeltaParameters] )
	    min->SetLimitedVariable(nDeltaParameters,name.Data(),startValue[nDeltaParameters], stepValue[nDeltaParameters],-1.52,5.);
	  else
	    min->SetFixedVariable(nDeltaParameters,name.Data(),startValue[nDeltaParameters]);
	}
      //      min->SetFixedVariable(nDeltaParameters,"delta_alpha",variable[nDeltaParameters]);


      excludeHighChi2Points=false;
      min->Minimize(); 
      double chi2=999.;
      chi2=(float) min->MinValue()/(float) (nPointsUsed-min->NFree()) ;
      std::cout << "ITER 0 CHI2 " << chi2 << " NPOINTS " << nPointsUsed << std::endl;

      if (iterativeFit && chi2>minChi2)
 	{
 	  excludeHighChi2Points=true;
 	  int niter=1;
	  double deltaChi2=100;
	  int nDeltaPoints=999;
 	  while ( (chi2>minChi2 || abs(deltaChi2)>0.05 ) && niter<10 && nDeltaPoints!=0 )
 	    {
	      nDeltaPoints=nPointsUsed;
	      excludePoints(min->X());
 	      min->Minimize();
	      nDeltaPoints=nDeltaPoints-nPointsUsed;
	      deltaChi2=chi2;
 	      chi2=(float) min->MinValue()/(float) (nPointsUsed-min->NFree()) ;
	      deltaChi2=(deltaChi2-chi2)/chi2;
	      std::cout << "ITER " << niter << " CHI2 " << chi2 << " DELTACHI2 " << deltaChi2 <<  " NPOINTS " << nPointsUsed << " NDELTAPOINTS " << nDeltaPoints << std::endl;
	      ++niter;
 	    }
	}

      const double *xs = min->X();
      const double *err_xs = min->Errors();  
      err_xs = min->Errors();  
      status=min->Status();
      n_delta=nDeltaParameters;
      if (det==0)
	indexType=2;
      else if (det==1)
	indexType=5;
      for (int i=0;i<nDeltaParameters;++i)
	{
	  delta[i]=xs[i];
	  err_delta[i]=err_xs[i];
	}
      chi2Min=min->MinValue();
      ndof=nPointsUsed-min->NFree();
      for (int ixtal=0;ixtal<nXtalsInRegion;++ixtal)
	{
	  //       cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): " 
	  // 	   << chi2(xs) << " Status "  << min->Status() << endl;
  
	  ietaVar=ieta[ixtal];
	  iphiVar=iphi[ixtal];
	  if (det==0)
	    signVar=(ieta>0) ? 1 : 0;
	  else if (det==1)
	    signVar=sign[ixtal];
#ifndef alphaFromHistory
	  alpha0=alphaFromDB(ixtal);
#else
	  alpha0=Z[ixtal*nPoints+3];
#endif	  
	  badXtalVar=badXtals[ixtal];
	  indexVar=xtalsInRegion[ixtal];

	  if (det==0)
	    {
	      index1Var=TMath::Abs(ietaVar);
	      index2Var=iTT(ietaVar,iphiVar,signVar);
	    }
	  else if (det==1)
	    {
	      index1Var=endcapRings[ietaVar-1][iphiVar-1][signVar];
	      index2Var=endcapSCs[ietaVar-1][iphiVar-1][signVar];
	    }

	  if (!oneAlphaPerRegion)
	    {
	      delta_alpha=xs[nDeltaParameters+ixtal];
	      err_alpha=err_xs[nDeltaParameters+ixtal];
	    }
	  else
	    {
	      delta_alpha=xs[nDeltaParameters];
	      err_alpha=err_xs[nDeltaParameters];
	    }

	  fitResults->Fill();

	  if (badXtals[ixtal]==1)
	    continue;

	  fitOut->cd();


	  //Writing corrected laser correction
	  for (int ii=excl_intervals;ii<excl_intervals+nPoints;++ii)
	    {
	      int jj=ii-excl_intervals;
#ifndef alphaFromHistory	  
	      Y[ixtal*nPoints+jj]=(*(lcGraph[ixtal]->GetY()+ii))/corrFunction(ixtal,jj,*(lcGraph[ixtal]->GetY()+ii),xs);
#else
	      Y[ixtal*nPoints+jj]=(*(lcGraph[ixtal]->GetY()+ii))/corrFunction(ixtal,jj,*(lcGraph[ixtal]->GetY()+ii),xs,*(alphaGraph[ixtal]->GetY()+ii));
#endif
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
#ifndef alphaFromHistory	  
	      Y[ixtal*nPoints+jj]=(*(etGraph[ixtal]->GetY()+ii))*corrFunction(ixtal,jj,*(lcGraph[ixtal]->GetY()+ii),xs);
#else
	      Y[ixtal*nPoints+jj]=(*(etGraph[ixtal]->GetY()+ii))*corrFunction(ixtal,jj,*(lcGraph[ixtal]->GetY()+ii),xs,*(alphaGraph[ixtal]->GetY()+ii));
#endif
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

#ifdef alphaFromHistory
      delete Z;
      delete errorZ;
#endif
      delete excludedPoint;
      delete ieta;
      delete iphi;
      delete sign;
      delete badXtals;
  
    }//end of loop over regions
  
  
  fitResults->Write();
  fitOut->Write();
  
  delete intervalStart;
  delete intervalEnd;
  
  return 0;
}
