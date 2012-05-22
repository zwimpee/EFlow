#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TMath.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <TChain.h>
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TF1.h"
#include "TH1.h"
#include "THStack.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTreeIndex.h"
#include "TChainIndex.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include "lumiIntervals.h"

#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define nRings 85
#define nTowers 36*68
#define nXtals 36*1700
#define kTowerPerSM 68
#define kXtalPerSM 1700
#define kSM 36



class makeLaserRatio {
public :

  TString firstFileName;
  TString secondFileName;
  TString outFileName;


  makeLaserRatio();
  virtual ~makeLaserRatio();
  void setFiles(const char* firstFile, const char* secondFile);
  void setOutFile(const char* file);
  void Loop();
};

makeLaserRatio::makeLaserRatio()
{

}

makeLaserRatio::~makeLaserRatio()
{

}

void makeLaserRatio::setFiles(const char* firstFile, const char* secondFile){
  firstFileName=TString(firstFile);  
  secondFileName=TString(secondFile);  
}

void makeLaserRatio::setOutFile(const char* file)
{
   outFileName=TString(file);
}

