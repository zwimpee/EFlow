#ifndef Calibration_EcalCalibAlgos_PhiSymmetryCalibration_h
#define Calibration_EcalCalibAlgos_PhiSymmetryCalibration_h

//
// Package:    Calibration/EcalCalibAlgos
// Class:      PhiSymmetryCalibration
// 
//
// Description: performs phi-symmetry calibration
// 
//
// Original Author:  David Futyan
//
// Responsible    :  Stefano Argiro, Valentina Sola
//

#include <vector>

#include "Calibration/EcalCalibAlgos/interface/EcalGeomPhiSymHelper.h"

// Framework
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ProducerBase.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "DataFormats/DetId/interface/DetId.h"

//to get the laser correction                                                                                                               
#include "CalibCalorimetry/EcalLaserAnalyzer/interface/MEEBGeom.h"
#include "CalibCalorimetry/EcalLaserAnalyzer/interface/MEEEGeom.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"


#include "CondFormats/EcalObjects/interface/EcalLaserAlphas.h"
#include "CondFormats/EcalObjects/interface/EcalLaserAPDPNRatiosRef.h"
#include "CondFormats/EcalObjects/interface/EcalLaserAPDPNRatios.h"


#include "CondFormats/DataRecord/interface/EcalLaserAlphasRcd.h"
#include "CondFormats/DataRecord/interface/EcalLaserAPDPNRatiosRefRcd.h"
#include "CondFormats/DataRecord/interface/EcalLaserAPDPNRatiosRcd.h"

#define MAXHITS 1000
//intervals of energy
#define NMISCALIBRATION 1
#define MAXL1bits 200
#define MAXHLTbits 200


class TH1F;

class TTree;

class TFile;

class PhiSymmetryCalibration :  public edm::EDAnalyzer
{

 public:

  /// Constructor
  PhiSymmetryCalibration( const edm::ParameterSet& iConfig );
  
  /// Destructor
  ~PhiSymmetryCalibration();

  /// Called at beginning of job
  virtual void beginJob();
  virtual void endRun(edm::Run&, const edm::EventSetup&);
  void endLuminosityBlock(edm::LuminosityBlock const& ,edm::EventSetup const&);


  /// Called at end of job
  virtual void endJob();

  /// Called at each event 
  virtual void analyze( const edm::Event&, const edm::EventSetup& );

  /// 
  void setUp(const edm::EventSetup& setup);

 private:

  // private member functions

  void getKfactors();
  float getLaserCorrection(DetId const & xid, edm::Timestamp const & iTime, bool invertZSide) const;

  // private data members

  static const int  kNMiscalBinsEB = 11;
  static const float  kMiscalRangeEB;

  static const int  kNMiscalBinsEE = 41; 
  static const float  kMiscalRangeEE;

  EcalGeomPhiSymHelper e_; 

  // Transverse energy sum arrays
  double etsum_barl_[kBarlRings][kBarlWedges][kSides];
  double etsum_endc_[kEndcWedgesX][kEndcWedgesX][kSides];
  double etsum_endc_uncorr[kEndcWedgesX][kEndcWedgesX][kSides];
  double etsumMean_barl_[kBarlRings];
  double etsumMean_endc_[kEndcEtaRings];

  float laser_corr_barl_[kBarlRings][kBarlWedges][kSides];

  unsigned int nhits_barl_[kBarlRings][kBarlWedges][kSides];
  unsigned int nhits_endc_[kEndcWedgesX][kEndcWedgesX][kSides];

  double etsum_barl_miscal_[kNMiscalBinsEB][kBarlRings];
  double etsum_endc_miscal_[kNMiscalBinsEE][kEndcEtaRings];

  //miscalibrated et in eta and phi
  double etsum_barl_miscal_phi_[NMISCALIBRATION][kNMiscalBinsEB][kBarlRings][kBarlWedges][kSides];
  unsigned int nhits_barl_miscal_[NMISCALIBRATION][kNMiscalBinsEB][kBarlRings][kBarlWedges][kSides];


  float middle_cut_et_[kBarlRings];
  //  float middle_cut_et_[NMISCALIBRATION];//try different thresholds for the ratio
  //miscalibrated et in first interval of energy
  double etsum_barl_miscal_firstinterval_[NMISCALIBRATION][kNMiscalBinsEB][kBarlRings][kBarlWedges][kSides];
  unsigned int nhits_barl_miscal_firstinterval_[NMISCALIBRATION][kNMiscalBinsEB][kBarlRings][kBarlWedges][kSides];
  //miscalibrated et in second interval of energy
  double etsum_barl_miscal_secondinterval_[NMISCALIBRATION][kNMiscalBinsEB][kBarlRings][kBarlWedges][kSides];
  unsigned int nhits_barl_miscal_secondinterval_[NMISCALIBRATION][kNMiscalBinsEB][kBarlRings][kBarlWedges][kSides];


  double esumMean_barl_[kBarlRings];
  double esumMean_endc_[kEndcEtaRings];


  // factors to convert from ET sum deviation to miscalibration
  double k_barl_[kBarlRings];
  double k_barl_phi_[kBarlRings][kBarlWedges][kSides];
  double k_endc_[kEndcEtaRings];

  double miscalEB_[kNMiscalBinsEB];
  double miscalEE_[kNMiscalBinsEE]; 

  std::vector<DetId> barrelCells;
  std::vector<DetId> endcapCells;

  // input calibration constants
  double oldCalibs_barl[kBarlRings  ][kBarlWedges][kSides];
  double oldCalibs_endc[kEndcWedgesX][kEndcWedgesY][kSides];

  // new calibration constants
  double newCalibs_barl[kBarlRings  ][kBarlWedges][kSides];
  double newCalibs_endc[kEndcWedgesX][kEndcWedgesX][kSides];

  // calibration constants not multiplied by old ones
  float epsilon_M_barl[kBarlRings][kBarlWedges][kSides];
  float epsilon_M_endc[kEndcWedgesX][kEndcWedgesY][kSides];

  // calibration const not corrected for k
  float rawconst_barl[kBarlRings][kBarlWedges][kSides];
  float rawconst_endc[kEndcWedgesX][kEndcWedgesX][kSides];   

 
  // steering parameters

  std::string ecalHitsProducer_;
  std::string barrelHits_;
  std::string endcapHits_;

  // energy cut in the barrel
  double eCut_barl_;
  

  // parametrized energy cut EE : e_cut = ap + eta_ring*b
  double ap_;
  double b_;

  //k-factor in phi switch
  bool calculatePhikFactor_;


  int eventSet_;
  /// threshold in channel status beyond which channel is marked bad
  int statusThreshold_; 


  static const int kMaxEndciPhi = 360;
   
  float phi_endc[kMaxEndciPhi][kEndcEtaRings]; 
  

  bool reiteration_;
  std::string oldcalibfile_; //searched for in Calibration/EcalCalibAlgos/data
  
  /// the old calibration constants (when reiterating, the last ones derived)
  EcalIntercalibConstants oldCalibs_;

  bool isfirstpass_;


  // Et and E spectra
  std::vector<TH1F*> et_spectrum_b_histos; //kBarlEtaRings
  std::vector<TH1F*> e_spectrum_b_histos;
  std::vector<TH1F*> et_spectrum_e_histos; //kEndcEtaRings
  std::vector<TH1F*> e_spectrum_e_histos;

  bool spectra;
  int  nevents_; 
  int  nevents_EB_; 
  int  nevents_EE_; 
  int  eventsinrun_;
  int  eventsinlb_;

  //laser correction stuff
  const EcalLaserAlphas* mAlphas_;
  const EcalLaserAPDPNRatiosRef* mAPDPNRatiosRef_;
  const EcalLaserAPDPNRatios* mAPDPNRatios_;  

  unsigned long long int avgTime_;

  //TTree
  //tree for the barrel
  int etaBranch[MAXHITS],phiBranch[MAXHITS];
  float lc_barl_Branch[MAXHITS],et_barl_Branch[MAXHITS];
  unsigned int unixtime,eventid, run, lumi,nhitBarl, nvertex;
  float beamSpotX0,beamSpotY0,beamSpotZ0;
  float beamSpotSigmaX,beamSpotSigmaY,beamSpotSigmaZ;

  std::vector<std::string>*  aHLTNames;
  std::vector<bool>*  aHLTResults;
  int nHLT;
  Bool_t   hltPass;
  Int_t    hltNamesLen;
  Int_t    hltCount;

  // Vertex distribution at MC truth level
  Float_t vxMC;
  Float_t vyMC;
  Float_t vzMC;

  //tree for the endcap
  int xBranch[MAXHITS],yBranch[MAXHITS];
  float lc_endc_Branch[MAXHITS],et_endc_Branch[MAXHITS];
  unsigned int sign_endc_Branch[MAXHITS],nhitEndc;

  /*  float miscal_var,etsum_var,etsum_first_var,etsum_second_var;
  int nhits_var,nhits_first_var,nhits_second_var,eta_var,phi_var,sign_var;
  */


  int nL1bits;
  int L1bits[MAXL1bits];
  int nL1bitsTech;
  int L1bitsTech[MAXL1bits];


  TFile* treeFile;

  TTree* variablesTree_;
  //  TTree* variablesTree_endc_;

  TFile* kFactorFile;
  //  TTree* kFactorTree;

  float miscal_var[NMISCALIBRATION],etsum_var[NMISCALIBRATION],etsum_first_var[NMISCALIBRATION],etsum_second_var[NMISCALIBRATION];
  int nhits_var[NMISCALIBRATION],nhits_first_var[NMISCALIBRATION],nhits_second_var[NMISCALIBRATION],eta_var[NMISCALIBRATION],phi_var[NMISCALIBRATION],sign_var[NMISCALIBRATION];

  TTree* kFactorTree[NMISCALIBRATION];

};

#endif
