#include "Calibration/EcalCalibAlgos/interface/PhiSymmetryCalibration.h"

// System include files
#include <memory>

// Framework
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibErrors.h"
#include "CondTools/Ecal/interface/EcalIntercalibConstantsXMLTranslator.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

// Geometry
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

//Channel status
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatusCode.h"

// HLT trigger
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include <FWCore/Common/interface/TriggerNames.h>
#include <DataFormats/Common/interface/TriggerResults.h>


#include "FWCore/Framework/interface/Run.h"

//MC
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//Vertex
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "boost/filesystem/operations.hpp"

// Trigger
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

using namespace std;
#include <fstream>
#include <iostream>
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TRegexp.h"

const float PhiSymmetryCalibration::kMiscalRangeEB = .05;
const float PhiSymmetryCalibration::kMiscalRangeEE = .10;


//_____________________________________________________________________________
// Class constructor

PhiSymmetryCalibration::PhiSymmetryCalibration(const edm::ParameterSet& iConfig) :
  ecalHitsProducer_(iConfig.getParameter<std::string>("ecalRecHitsProducer")),
  barrelHits_( iConfig.getParameter< std::string > ("barrelHitCollection")),
  endcapHits_( iConfig.getParameter< std::string > ("endcapHitCollection")),
  eCut_barl_( iConfig.getParameter< double > ("eCut_barrel") ),
  ap_( iConfig.getParameter<double> ("ap") ),
  b_( iConfig.getParameter<double> ("b") ),
  calculatePhikFactor_(iConfig.getParameter< bool > ("cPhiKFactor")), 
  eventSet_( iConfig.getParameter< int > ("eventSet") ),
 // statusThreshold_(iConfig.getUntrackedParameter<int>("statusThreshold",3)),
  statusThreshold_(iConfig.getUntrackedParameter<int>("statusThreshold",2)),
  reiteration_(iConfig.getUntrackedParameter< bool > ("reiteration",false)),
  oldcalibfile_(iConfig.getUntrackedParameter<std::string>("oldcalibfile",
							   "EcalintercalibConstants.xml"))

{

  dumpBeamSpot_=iConfig.getUntrackedParameter< bool > ("dumpBeamSpot",true);
  dumpL1_=iConfig.getUntrackedParameter< bool > ("dumpL1",true);
  dumpHLT_=iConfig.getUntrackedParameter< bool > ("dumpHLT",false);
  isfirstpass_=true;

  et_spectrum_b_histos.resize(kBarlRings);
  e_spectrum_b_histos.resize(kBarlRings);
  et_spectrum_e_histos.resize(kEndcEtaRings);
  e_spectrum_e_histos.resize(kEndcEtaRings); 
  
  spectra=true;

  nevents_EB_=0;
  nevents_EE_=0;
  nevents_=0;
  eventsinrun_=0;
  eventsinlb_=0;
}


//_____________________________________________________________________________
// Close files, etc.

PhiSymmetryCalibration::~PhiSymmetryCalibration()
{


  for(Int_t i=0;i<kBarlRings;i++){
    delete et_spectrum_b_histos[i];
    delete e_spectrum_b_histos[i];
    
  }
  for(Int_t i=0;i<kEndcEtaRings;i++){
    delete et_spectrum_e_histos[i];
    delete e_spectrum_e_histos[i];    
  }

  
}


//_____________________________________________________________________________
// Initialize algorithm

void PhiSymmetryCalibration::beginJob( )
{
  aHLTNames = new std::vector<std::string>;
  aHLTResults = new std::vector<bool>;

  //sistemare
  middle_cut_et_[0]=0.3475;
  middle_cut_et_[1]=0.3475;
  middle_cut_et_[2]=0.3475;
  middle_cut_et_[3]=0.3475;
  middle_cut_et_[4]=0.3475;
  middle_cut_et_[5]=0.345;
  middle_cut_et_[6]=0.345;
  middle_cut_et_[7]=0.345;
  middle_cut_et_[8]=0.345;
  middle_cut_et_[9]=0.3425;
  middle_cut_et_[10]=0.3425;
  middle_cut_et_[11]=0.34;
  middle_cut_et_[12]=0.34;
  middle_cut_et_[13]=0.34;
  middle_cut_et_[14]=0.3375;
  middle_cut_et_[15]=0.335;
  middle_cut_et_[16]=0.335;
  middle_cut_et_[17]=0.3325;
  middle_cut_et_[18]=0.3325;
  middle_cut_et_[19]=0.33;
  middle_cut_et_[20]=0.3275;
  middle_cut_et_[21]=0.3275;
  middle_cut_et_[22]=0.325;
  middle_cut_et_[23]=0.3225;
  middle_cut_et_[24]=0.32;
  middle_cut_et_[25]=0.32;
  middle_cut_et_[26]=0.3175;
  middle_cut_et_[27]=0.315;
  middle_cut_et_[28]=0.3125;
  middle_cut_et_[29]=0.31;
  middle_cut_et_[30]=0.3075;
  middle_cut_et_[31]=0.305;
  middle_cut_et_[32]=0.3025;
  middle_cut_et_[33]=0.3025;
  middle_cut_et_[34]=0.3;
  middle_cut_et_[35]=0.2975;
  middle_cut_et_[36]=0.295;
  middle_cut_et_[37]=0.2925;
  middle_cut_et_[38]=0.29;
  middle_cut_et_[39]=0.2875;
  middle_cut_et_[40]=0.285;
  middle_cut_et_[41]=0.2825;
  middle_cut_et_[42]=0.28;
  middle_cut_et_[43]=0.2775;
  middle_cut_et_[44]=0.2725;
  middle_cut_et_[45]=0.27;
  middle_cut_et_[46]=0.2675;
  middle_cut_et_[47]=0.265;
  middle_cut_et_[48]=0.2625;
  middle_cut_et_[49]=0.2575;
  middle_cut_et_[50]=0.255;
  middle_cut_et_[51]=0.2525;
  middle_cut_et_[52]=0.25;
  middle_cut_et_[53]=0.2475;
  middle_cut_et_[54]=0.245;
  middle_cut_et_[55]=0.24;
  middle_cut_et_[56]=0.2375;
  middle_cut_et_[57]=0.235;
  middle_cut_et_[58]=0.2325;
  middle_cut_et_[59]=0.23;
  middle_cut_et_[60]=0.2275;
  middle_cut_et_[61]=0.2225;
  middle_cut_et_[62]=0.22;
  middle_cut_et_[63]=0.215;
  middle_cut_et_[64]=0.2125;
  middle_cut_et_[65]=0.21;
  middle_cut_et_[66]=0.2075;
  middle_cut_et_[67]=0.205;
  middle_cut_et_[68]=0.2025;
  middle_cut_et_[69]=0.2;
  middle_cut_et_[70]=0.1975;
  middle_cut_et_[71]=0.195;
  middle_cut_et_[72]=0.1925;
  middle_cut_et_[73]=0.19;
  middle_cut_et_[74]=0.1875;
  middle_cut_et_[75]=0.185;
  middle_cut_et_[76]=0.1825;
  middle_cut_et_[77]=0.18;
  middle_cut_et_[78]=0.1775;
  middle_cut_et_[79]=0.175;
  middle_cut_et_[80]=0.1725;
  middle_cut_et_[81]=0.17;
  middle_cut_et_[82]=0.1675;
  middle_cut_et_[83]=0.165;
  middle_cut_et_[84]=0.16;


  treeFile= TFile::Open("variablesTree.root","recreate");
  treeFile->cd();
  //  variablesTree_barl_=new TTree("variablesTree_barl","variablesTree_barl");	      
  variablesTree_=new TTree("variablesTree","variablesTree");	      
  //  cout<<"TTree created"<<endl;						      
  variablesTree_->Branch("nhit_barl",&nhitBarl,"nhit_barl/i");  
  variablesTree_->Branch("HLTNamesLen",&hltNamesLen,"HLTNamesLen/I");
  variablesTree_->Branch("HLTNames",&aHLTNames);
  variablesTree_->Branch("HLTResults",&aHLTResults);
  variablesTree_->Branch("nL1bits",&nL1bits,"nL1bits/I");
  variablesTree_->Branch("L1bits",L1bits,"L1bits[nL1bits]/I");
  variablesTree_->Branch("nL1bitsTech",&nL1bitsTech,"nL1bitsTech/I");
  variablesTree_->Branch("L1bitsTech",L1bitsTech,"L1bitsTech[nL1bitsTech]/I");
  variablesTree_->Branch("vxMC",&vxMC,"vxMC/F");
  variablesTree_->Branch("vyMC",&vyMC,"vyMC/F");
  variablesTree_->Branch("vzMC",&vzMC,"vzMC/F");
  variablesTree_->Branch("nvertex",&nvertex,"nvertex/i");  
  variablesTree_->Branch("beamSpotX0",&beamSpotX0,"beamSpotX0/F");  
  variablesTree_->Branch("beamSpotY0",&beamSpotY0,"beamSpotY0/F");  
  variablesTree_->Branch("beamSpotZ0",&beamSpotZ0,"beamSpotZ0/F");  
  variablesTree_->Branch("beamSpotSigmaX",&beamSpotSigmaX,"beamSpotSigmaX/F");  
  variablesTree_->Branch("beamSpotSigmaY",&beamSpotSigmaY,"beamSpotSigmaY/F");  
  variablesTree_->Branch("beamSpotSigmaZ",&beamSpotSigmaZ,"beamSpotSigmaZ/F");  
  variablesTree_->Branch("ieta",etaBranch,"ieta[nhit_barl]/I");		      
  variablesTree_->Branch("iphi",phiBranch,"phi[nhit_barl]/I");		      
  variablesTree_->Branch("lc_barl",lc_barl_Branch,"lc_barl[nhit_barl]/F");		      
  variablesTree_->Branch("alpha_barl",alpha_barl_Branch,"alpha_barl[nhit_barl]/F");		      
  variablesTree_->Branch("et_barl",et_barl_Branch,"et_barl[nhit_barl]/F");		      
  variablesTree_->Branch("unixtime",&unixtime,"unixtime/i");
  variablesTree_->Branch("run",&run,"run/i");  
  variablesTree_->Branch("lumi",&lumi,"lumi/i");  
  variablesTree_->Branch("eventid",&eventid,"eventid/i");  

//   treeFile->cd();
//   variablesTree_endc_=new TTree("variablesTree_endc","variablesTree_endc");	      
//   //  cout<<"TTree created"<<endl;						      
  variablesTree_->Branch("nhit_endc",&nhitEndc,"nhit_endc/i");
//   variablesTree_endc_->Branch("HLTNamesLen",&hltNamesLen,"HLTNamesLen/I");
//   variablesTree_endc_->Branch("HLTNames",&aHLTNames);
//   variablesTree_endc_->Branch("HLTResults",&aHLTResults);
//   variablesTree_endc_->Branch("nL1bits",&nL1bits,"nL1bits/I");
//   variablesTree_endc_->Branch("L1bits",L1bits,"L1bits[nL1bits]/I");
//   variablesTree_endc_->Branch("nL1bitsTech",&nL1bitsTech,"nL1bitsTech/I");
//   variablesTree_endc_->Branch("L1bitsTech",L1bitsTech,"L1bitsTech[nL1bitsTech]/I");
//   variablesTree_endc_->Branch("vxMC",&vxMC,"vxMC/F");
//   variablesTree_endc_->Branch("vyMC",&vyMC,"vyMC/F");
//   variablesTree_endc_->Branch("vzMC",&vzMC,"vzMC/F");  
//   variablesTree_endc_->Branch("nvertex",&nvertex,"nvertex/i");  
//   variablesTree_endc_->Branch("beamSpotX0",&beamSpotX0,"beamSpotX0/F");  
//   variablesTree_endc_->Branch("beamSpotY0",&beamSpotY0,"beamSpotY0/F");  
//   variablesTree_endc_->Branch("beamSpotZ0",&beamSpotZ0,"beamSpotZ0/F");  
//   variablesTree_endc_->Branch("beamSpotSigmaX",&beamSpotSigmaX,"beamSpotSigmaX/F");  
//   variablesTree_endc_->Branch("beamSpotSigmaY",&beamSpotSigmaY,"beamSpotSigmaY/F");  
//   variablesTree_endc_->Branch("beamSpotSigmaZ",&beamSpotSigmaZ,"beamSpotSigmaZ/F");  
  variablesTree_->Branch("ix",xBranch,"ix[nhit_endc]/I");		      
  variablesTree_->Branch("iy",yBranch,"iy[nhit_endc]/I");		      
  variablesTree_->Branch("sign",sign_endc_Branch,"sign[nhit_endc]/i");
  variablesTree_->Branch("lc_endc",lc_endc_Branch,"lc_endc[nhit_endc]/F");		      
  variablesTree_->Branch("alpha_endc",alpha_endc_Branch,"alpha_endc[nhit_endc]/F");		      
  variablesTree_->Branch("et_endc",et_endc_Branch,"et_endc[nhit_endc]/F");		      
//   variablesTree_endc_->Branch("unixtime",&unixtime,"unixtime/i");
//   variablesTree_endc_->Branch("run",&run,"run/i");  
//   variablesTree_endc_->Branch("lumi",&lumi,"lumi/i");  
//   variablesTree_endc_->Branch("eventid",&eventid,"eventid/i");  

  kFactorFile=TFile::Open("PhiSymmetryCalibration_kFactors.root","recreate");
  /*  kFactorFile->cd();
  kFactorTree=new TTree("kFactorTree","kFactorTree");	      
  //  cout<<"TTree created"<<endl;						      
  kFactorTree->Branch("miscal",&miscal_var,"miscal/F");  
  kFactorTree->Branch("etsum",&etsum_var,"etsum/F");	
  kFactorTree->Branch("nhits",&nhits_var,"nhits/I");	
  kFactorTree->Branch("etsum_first",&etsum_first_var,"etsum_first/F");	
  kFactorTree->Branch("nhits_first",&nhits_first_var,"nhits_first/I");	
  kFactorTree->Branch("etsum_second",&etsum_second_var,"etsum_second/F");	
  kFactorTree->Branch("nhits_second",&nhits_second_var,"nhits_second/I");	
  kFactorTree->Branch("ieta",&eta_var,"ieta/I");		      
  kFactorTree->Branch("sign",&sign_var,"sign/I");		      
  kFactorTree->Branch("iphi",&phi_var,"iphi/I");		      
  */

  for(int itree=0;itree<NMISCALIBRATION;++itree){
    ostringstream tree_name;   
    tree_name<<"kFactorTree_"<<itree;
    kFactorFile->cd();
    kFactorTree[itree]=new TTree(tree_name.str().c_str(),tree_name.str().c_str());	      
    //  cout<<"TTree created"<<endl;						      
    kFactorTree[itree]->Branch("miscal",&miscal_var[itree],"miscal/F");  
    kFactorTree[itree]->Branch("etsum",&etsum_var[itree],"etsum/F");	
    kFactorTree[itree]->Branch("nhits",&nhits_var[itree],"nhits/I");	
    kFactorTree[itree]->Branch("etsum_first",&etsum_first_var[itree],"etsum_first/F");	
    kFactorTree[itree]->Branch("nhits_first",&nhits_first_var[itree],"nhits_first/I");	
    kFactorTree[itree]->Branch("etsum_second",&etsum_second_var[itree],"etsum_second/F");	
    kFactorTree[itree]->Branch("nhits_second",&nhits_second_var[itree],"nhits_second/I");	
    kFactorTree[itree]->Branch("ieta",&eta_var[itree],"ieta/I");		      
    kFactorTree[itree]->Branch("sign",&sign_var[itree],"sign/I");		      
    kFactorTree[itree]->Branch("iphi",&phi_var[itree],"iphi/I");		      
  }

  // initialize arrays
  for (int sign=0; sign<kSides; sign++) {
    for (int ieta=0; ieta<kBarlRings; ieta++) {
      for (int iphi=0; iphi<kBarlWedges; iphi++) {
	etsum_barl_[ieta][iphi][sign]=0.;
	nhits_barl_[ieta][iphi][sign]=0;
	laser_corr_barl_[ieta][iphi][sign]=0.;

      }
    }
    for (int ix=0; ix<kEndcWedgesX; ix++) {
      for (int iy=0; iy<kEndcWedgesY; iy++) {
	etsum_endc_[ix][iy][sign]=0.;
	nhits_endc_[ix][iy][sign]=0;
      }
    }
  }


  
  for (int imiscal=0; imiscal<kNMiscalBinsEB; imiscal++) {
    miscalEB_[imiscal]= (1-kMiscalRangeEB) + float(imiscal)* (2*kMiscalRangeEB/(kNMiscalBinsEB-1));
    for (int ieta=0; ieta<kBarlRings; ieta++) {
      etsum_barl_miscal_[imiscal][ieta]=0.;
      if(calculatePhikFactor_){
	for (int iphi=0; iphi<kBarlWedges;iphi++){
	  for(int isides=0;isides<kSides;isides++){
	    for(int inmiscalib =0; inmiscalib<NMISCALIBRATION;++inmiscalib){
	    etsum_barl_miscal_phi_[inmiscalib][imiscal][ieta][iphi][isides]=0.;
	    nhits_barl_miscal_[inmiscalib][imiscal][ieta][iphi][isides]=0;
	    etsum_barl_miscal_firstinterval_[inmiscalib][imiscal][ieta][iphi][isides]=0.;
	    etsum_barl_miscal_secondinterval_[inmiscalib][imiscal][ieta][iphi][isides]=0.;
	    nhits_barl_miscal_firstinterval_[inmiscalib][imiscal][ieta][iphi][isides]=0;
	    nhits_barl_miscal_secondinterval_[inmiscalib][imiscal][ieta][iphi][isides]=0;
	    }
	  }
	}
      }
    }
  }

  for (int imiscal=0; imiscal<kNMiscalBinsEE; imiscal++) {
    miscalEE_[imiscal]= (1-kMiscalRangeEE) + float(imiscal)* (2*kMiscalRangeEE/(kNMiscalBinsEE-1));
    for (int ring=0; ring<kEndcEtaRings; ring++) etsum_endc_miscal_[imiscal][ring]=0.;
  }



  // start spectra stuff
  if (eventSet_!=1) spectra = false;
  
  if(spectra)
    {
      ostringstream t;
      for(Int_t i=0;i<kBarlRings;i++)
	{
	  t << "et_spectrum_b_" << i+1;
	  et_spectrum_b_histos[i]=new TH1F(t.str().c_str(),";E_{T} [MeV]",50,0.,500.);
	  t.str("");
	  
	  t << "e_spectrum_b_" << i+1;
	  e_spectrum_b_histos[i]=new TH1F(t.str().c_str(),";E [MeV]",50,0.,500.);
	  t.str("");
	
	}
      for(Int_t i=0;i<kEndcEtaRings;i++)
	{
	  t << "et_spectrum_e_" << i+1;
	  et_spectrum_e_histos[i]=new TH1F(t.str().c_str(),";E_{T} [MeV]",75,0.,1500.);
	  t.str("");
	  
	  t << "e_spectrum_e_" << i+1;
	  e_spectrum_e_histos[i]=new TH1F(t.str().c_str(),";E [MeV]",75,0.,1500.);
	  t.str("");
	
	}
    }
  // end spectra stuff
}


//_____________________________________________________________________________
// Terminate algorithm

void PhiSymmetryCalibration::endJob()
{

  edm::LogInfo("Calibration") << "[PhiSymmetryCalibration] At end of job";
 
  cout<<"Events processed " << nevents_<< endl;
  
// cout<<"entriesnew tree "<<  variablesTree_barl_->GetEntries()<<endl;
  
  // start spectra stuff
  if(spectra)
    {
      TFile f("Espectra_plus.root","recreate");

      for(int i=0;i<kBarlRings;i++){
	et_spectrum_b_histos[i]->Write();
	e_spectrum_b_histos[i]->Write();
      }

      for(int i=0;i<kEndcEtaRings;i++){
	et_spectrum_e_histos[i]->Write();
	e_spectrum_e_histos[i]->Write();
      }

      f.Close();
    }
  



  if (eventSet_==1) {
    // calculate factors to convert from fractional deviation of ET sum from 
    // the mean to the estimate of the miscalibration factor
    getKfactors();



    std::ofstream k_barl_out("k_barl.dat", ios::out);
    for (int ieta=0; ieta<kBarlRings; ieta++){
      k_barl_out << ieta << " " << k_barl_[ieta] << endl;
    }    
    k_barl_out.close();
    

    std::ofstream k_barl_phi_out("k_phi_barl.dat", ios::out);
    for (int ieta=0; ieta<kBarlRings; ieta++){
      for (int iphi=0; iphi<kBarlWedges; iphi++){
	for(int isign=0;isign<kSides;isign++){
	k_barl_phi_out << ieta << " " << iphi<<" "<<isign<<" "<< k_barl_phi_[ieta][iphi][isign] << endl;
	}
      }
    }
    k_barl_phi_out.close();

    std::ofstream laser_corr_barl_out("laser_corr_barl.dat", ios::out);
    for (int ieta=0; ieta<kBarlRings; ieta++){
      for (int iphi=0; iphi<kBarlWedges; iphi++){
	for(int isign=0;isign<kSides;isign++){
	  laser_corr_barl_out << ieta << " " << iphi<<" "<<isign<<" "<< laser_corr_barl_[ieta][iphi][isign]<<" " << nhits_barl_[ieta][iphi][isign]<< endl;
	}
      }
    }
    laser_corr_barl_out.close();    

    if(nevents_EB_!=0) avgTime_=avgTime_/nevents_EB_;
    std::ofstream avg_time_barl_out("avg_time_barl.dat", ios::out);
    avg_time_barl_out<<avgTime_<<endl;
    avg_time_barl_out.close();


    std::ofstream k_endc_out("k_endc.dat", ios::out);
    for (int ring=0; ring<kEndcEtaRings; ring++){
      k_endc_out << ring << " " << k_endc_[ring] << endl;
    }
    k_endc_out.close();    



    if (eventSet_!=0) {

      

    // output ET sums


    stringstream etsum_file_barl;
    etsum_file_barl << "etsum_barl_"<<eventSet_<<".dat";



    std::ofstream etsum_barl_out(etsum_file_barl.str().c_str(),ios::out);

    for (int ieta=0; ieta<kBarlRings; ieta++) {
      for (int iphi=0; iphi<kBarlWedges; iphi++) {
	for (int sign=0; sign<kSides; sign++) {
	  etsum_barl_out << eventSet_ << " " << ieta << " " << iphi << " " << sign 
		         << " " << etsum_barl_[ieta][iphi][sign] << " " 
                         <<  nhits_barl_[ieta][iphi][sign] << endl;
	}
      }
    }
    etsum_barl_out.close();

    stringstream etsum_file_endc;
    etsum_file_endc << "etsum_endc_"<<eventSet_<<".dat";

    std::ofstream etsum_endc_out(etsum_file_endc.str().c_str(),ios::out);
    for (int ix=0; ix<kEndcWedgesX; ix++) {
      for (int iy=0; iy<kEndcWedgesY; iy++) {
	int ring = e_.endcapRing_[ix][iy];
	if (ring!=-1) {
	  for (int sign=0; sign<kSides; sign++) {
	    etsum_endc_out << eventSet_ << " " << ix << " " << iy << " " << sign 
			   << " " << etsum_endc_[ix][iy][sign] << " " 
			   << nhits_endc_[ix][iy][sign]<<" " 
                           << e_.endcapRing_[ix][iy]<<endl;
	  }
	}
      }
    }
    etsum_endc_out.close();

    //output of tree

    } 
 
  }
  //  std::cout << "entries: " << std::endl;
  //std::cout<<variablesTree_barl_->GetEntries() << std::endl;


  treeFile->cd();
  variablesTree_->Write();
  //  variablesTree_endc_->Write();
  //  variablesTree_endc_->Write();
  //  cout<<variablesTree_barl_<<endl;  

  treeFile->Close();
  
  
}


//_____________________________________________________________________________
// Called at each event

void PhiSymmetryCalibration::analyze( const edm::Event& event, const edm::EventSetup& setup )
{
  using namespace edm;
  using namespace reco;
  using namespace std;


  //  cout<<"start analyze"<<endl;
  eventid=event.id().event();
  run=event.id().run();
  lumi=event.id().luminosityBlock();

  bool isMC = !event.isRealData(); // separate MC processing
  // get MC info from GenParticleCandidates 
  Handle<GenParticleCollection> genParticles;
  vxMC=-9999.;
  vyMC=-9999.;
  vzMC=-9999.;
  if( isMC ) 
    {
      event.getByLabel("genParticles", genParticles);
      vxMC = genParticles->at(2).vx();
      vyMC = genParticles->at(2).vy();
      vzMC = genParticles->at(2).vz();
    }

  Timestamp ts(event.time().value());
  unixtime=ts.unixTime();

  if (isfirstpass_) {
    setUp(setup);
    isfirstpass_=false;
  }

  edm::ESHandle <EcalLaserAPDPNRatiosRef> apdPnRef;
  setup.get<EcalLaserAPDPNRatiosRefRcd>().get(apdPnRef);
  mAPDPNRatiosRef_=apdPnRef.product();

  edm::ESHandle <EcalLaserAPDPNRatios> apdPn;
  setup.get<EcalLaserAPDPNRatiosRcd>().get(apdPn);
  mAPDPNRatios_=apdPn.product();

  edm::ESHandle <EcalLaserAlphas> alpha;
  setup.get<EcalLaserAlphasRcd>().get(alpha);
  mAlphas_=alpha.product();


  nvertex=-1;
  edm::Handle<reco::VertexCollection> VertexHandle;
  event.getByLabel("offlinePrimaryVerticesWithBS", VertexHandle);
  if (VertexHandle.isValid())
      nvertex=VertexHandle->size();

  if (dumpBeamSpot_)
    {
      reco::BeamSpot vertexBeamSpot;
      edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
      event.getByLabel("offlineBeamSpot",recoBeamSpotHandle);
      
      beamSpotX0=-9999.;
      beamSpotY0=-9999.;
      beamSpotZ0=-9999.;
      beamSpotSigmaX=-9999.;
      beamSpotSigmaY=-9999.;
      beamSpotSigmaZ=-9999.;
      
      if (recoBeamSpotHandle.isValid())
	{
	  vertexBeamSpot = *recoBeamSpotHandle;
	  beamSpotX0=vertexBeamSpot.x0();
	  beamSpotY0=vertexBeamSpot.y0();
	  beamSpotZ0=vertexBeamSpot.z0();
	  
	  beamSpotSigmaX=vertexBeamSpot.BeamWidthX();
	  beamSpotSigmaY=vertexBeamSpot.BeamWidthY();
	  beamSpotSigmaZ=vertexBeamSpot.sigmaZ();
	}
    }

  if (dumpL1_)
    {
      // trigger Handles
      Handle< L1GlobalTriggerReadoutRecord > gtReadoutRecord;
      event.getByLabel( edm::InputTag("hltGtDigis"), gtReadoutRecord);
      
      ESHandle<L1GtTriggerMenu> menuRcd;
      setup.get<L1GtTriggerMenuRcd>().get(menuRcd);
      
      /// L1 trigger results for physics algorithms
      //const L1GtTriggerMenu* menu = menuRcd.product();
      const DecisionWord& gtDecisionWordBeforeMask = gtReadoutRecord->decisionWord();
      
      nL1bits = int( gtDecisionWordBeforeMask.size() );
      
      for (int iBit = 0; iBit < nL1bits; ++iBit)
	L1bits[iBit] = gtDecisionWordBeforeMask[iBit] ;
      /// L1 technical triggers
      const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = gtReadoutRecord->technicalTriggerWord();
      nL1bitsTech = int(technicalTriggerWordBeforeMask.size());
      for(int iBit = 0; iBit < nL1bitsTech; ++iBit) {
	L1bitsTech[iBit] = technicalTriggerWordBeforeMask.at(iBit);
      }
    }


  hltNamesLen = 0;
  if (dumpHLT_)
    {
      aHLTNames->clear();
      aHLTResults->clear();
      
      edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
      event.getByLabel(edm::InputTag("TriggerResults::HLT"), hltTriggerResultHandle);
      
      edm::TriggerNames HLTNames;
      HLTNames = event.triggerNames(*hltTriggerResultHandle);
      std::string tempnames;
      hltCount = hltTriggerResultHandle->size();
      std::vector<TRegexp> reg;
      reg.push_back(TRegexp(TString(".*BeamGas.*")));
      reg.push_back(TRegexp(TString(".*PreCollisions.*")));
      reg.push_back(TRegexp(TString(".*Interbunch.*")));
      reg.push_back(TRegexp(TString(".*halo.*")));
      reg.push_back(TRegexp(TString(".*Halo.*")));
      
      for (int i = 0 ; i != hltCount; ++i) {
	
	TString hltName_tstr(HLTNames.triggerName(i));
	std::string hltName_str(HLTNames.triggerName(i));
	
	for (unsigned int ireg=0;ireg<reg.size();ireg++)
	  {
	    if ( hltName_tstr.Contains(reg[ireg]) ) 
	      {
		aHLTNames->push_back(hltName_str);
		aHLTResults->push_back(hltTriggerResultHandle->accept(i));
	      }
	  }
      } 
      hltNamesLen = tempnames.length();
    }

  Handle<EBRecHitCollection> barrelRecHitsHandle;
  Handle<EERecHitCollection> endcapRecHitsHandle;
  
  event.getByLabel(ecalHitsProducer_,barrelHits_,barrelRecHitsHandle);
  if (!barrelRecHitsHandle.isValid()) {
    LogError("") << "[PhiSymmetryCalibration] Error! Can't get product!" << std::endl;
  }
  
  event.getByLabel(ecalHitsProducer_,endcapHits_,endcapRecHitsHandle);
  if (!endcapRecHitsHandle.isValid()) {
    LogError("") << "[PhiSymmetryCalibration] Error! Can't get product!" << std::endl;
  }
  




  // get the ecal geometry
  edm::ESHandle<CaloGeometry> geoHandle;
  setup.get<CaloGeometryRecord>().get(geoHandle);
  const CaloSubdetectorGeometry *barrelGeometry = 
    geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
  const CaloSubdetectorGeometry *endcapGeometry = 
    geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
 
  nhitBarl=0;
  nhitEndc=0;
  bool pass=false;
  // select interesting EcalRecHits (barrel)
  EBRecHitCollection::const_iterator itb;
  for (itb=barrelRecHitsHandle->begin(); itb!=barrelRecHitsHandle->end(); itb++) {
    EBDetId hit = EBDetId(itb->id());
    float eta = barrelGeometry->getGeometry(hit)->getPosition().eta();
    //float phi = barrelGeometry->getGeometry(hit)->getPosition().phi();
    float et = itb->energy()/cosh(eta);
    float e  = itb->energy();
  
//    cout<<"et"<<et<<"energy"<<e<<"eta"<<eta<<"phi"<<phi<<endl;  
    

    // if iterating, correct by the previous calib constants found,
    // which are supplied in the form of correction 
    if (reiteration_) {
      et= et  * oldCalibs_[hit];
      e = e  * oldCalibs_[hit];
    }

    float et_thr = eCut_barl_/cosh(eta) + 1.;

    int sign = hit.ieta()>0 ? 1 : 0;

    if (e >  eCut_barl_ && et < et_thr && e_.goodCell_barl[abs(hit.ieta())-1][hit.iphi()-1][sign]) {
      //cout<<"eta "<<abs(hit.ieta())<<" phi "<<hit.iphi()<<" sign "<<sign<<endl;
      std::pair<float,float> lc=getLaserCorrection(itb->id(),event.time(),false);
      etsum_barl_[abs(hit.ieta())-1][hit.iphi()-1][sign] += et;
      nhits_barl_[abs(hit.ieta())-1][hit.iphi()-1][sign] ++;
      laser_corr_barl_[abs(hit.ieta())-1][hit.iphi()-1][sign]+=lc.first;
      Timestamp ts(event.time().value());
      nevents_EB_++;
      avgTime_+=ts.unixTime();
      pass =true;
      
      etaBranch[nhitBarl]=hit.ieta();
      phiBranch[nhitBarl]=hit.iphi();
      et_barl_Branch[nhitBarl]=et;
      lc_barl_Branch[nhitBarl]=lc.first;
      alpha_barl_Branch[nhitBarl]=lc.second;
      ++nhitBarl;
      if(nhitBarl>1000){
	cout<<"exceeded maximum size of hit array"<<endl;
	break;
      }
      
    }//if energy
    


    
    if (eventSet_==1) {
      // apply a miscalibration to all crystals and increment the 
      // ET sum, combined for all crystals
      for (int imiscal=0; imiscal<kNMiscalBinsEB; imiscal++) {
	//	long long int eventidint=(long long int) eventid;
	//	if(eventidint%kNMiscalBinsEB != imiscal) continue;

	//	cout<<"eventid "<<eventid<<"eventidint"<<eventidint<<" "<<imiscal<<endl;
	//et cut low 0.25!!!!!!!!remember it
	if (miscalEB_[imiscal]*et >  eCut_barl_ && miscalEB_[imiscal]*et < et_thr && e_.goodCell_barl[abs(hit.ieta())-1][hit.iphi()-1][sign]) {
	  etsum_barl_miscal_[imiscal][abs(hit.ieta())-1] += miscalEB_[imiscal]*et;
	  
	  if(calculatePhikFactor_){
	    for(int inmiscalib =0; inmiscalib<NMISCALIBRATION;++inmiscalib){
	      etsum_barl_miscal_phi_[inmiscalib][imiscal][abs(hit.ieta())-1][hit.iphi()-1][sign] += miscalEB_[imiscal]*et;
	      nhits_barl_miscal_[inmiscalib][imiscal][abs(hit.ieta())-1][hit.iphi()-1][sign] ++;
	      if(miscalEB_[imiscal]*et <=middle_cut_et_[abs(hit.ieta())-1]){
		etsum_barl_miscal_firstinterval_[inmiscalib][imiscal][abs(hit.ieta())-1][hit.iphi()-1][sign] += miscalEB_[imiscal]*et;
		nhits_barl_miscal_firstinterval_[inmiscalib][imiscal][abs(hit.ieta())-1][hit.iphi()-1][sign] ++;
	      }else {
		etsum_barl_miscal_secondinterval_[inmiscalib][imiscal][abs(hit.ieta())-1][hit.iphi()-1][sign] += miscalEB_[imiscal]*et;
		nhits_barl_miscal_secondinterval_[inmiscalib][imiscal][abs(hit.ieta())-1][hit.iphi()-1][sign] ++;
	      }
	    }
	  }
	}
      
      }
      
      // spectra stuff
      if(spectra && hit.ieta()>0) //POSITIVE!!!
	//      if(spectra && hit.ieta()<0) //NEGATIVE!!!
	{
	  et_spectrum_b_histos[abs(hit.ieta())-1]->Fill(et*1000.);
	  e_spectrum_b_histos[abs(hit.ieta())-1]->Fill(e*1000.);
	}//if spectra
      
    }//if eventSet_==1
  }//for barl


  // select interesting EcalRecHits (endcaps)
  EERecHitCollection::const_iterator ite;
  for (ite=endcapRecHitsHandle->begin(); ite!=endcapRecHitsHandle->end(); ite++) {
    EEDetId hit = EEDetId(ite->id());
    float eta = abs(endcapGeometry->getGeometry(hit)->getPosition().eta());
    float phi = endcapGeometry->getGeometry(hit)->getPosition().phi();

    float et = ite->energy()/cosh(eta);
    float e  = ite->energy();

    // if iterating, multiply by the previous correction factor
    if (reiteration_) {
      et= et * oldCalibs_[hit];
      e = e * oldCalibs_[hit];
    }

    int sign = hit.zside()>0 ? 1 : 0;


    // changes of eCut_endc_ -> variable linearthr 
    // e_cut = ap + eta_ring*b

    double eCut_endc=0;
    for (int ring=0; ring<kEndcEtaRings; ring++) {

      if(eta>e_.etaBoundary_[ring] && eta<e_.etaBoundary_[ring+1])
	{  
	  float eta_ring= abs(e_.cellPos_[ring][50].eta())  ;
	  eCut_endc = ap_ + eta_ring*b_;

	}
    }


    float et_thr = eCut_endc/cosh(eta) + 1.;
   
    if (e > eCut_endc && et < et_thr && e_.goodCell_endc[hit.ix()-1][hit.iy()-1][sign]){
      std::pair<float,float> lc=getLaserCorrection(ite->id(),event.time(),false);
      etsum_endc_[hit.ix()-1][hit.iy()-1][sign] += et;
      nhits_endc_[hit.ix()-1][hit.iy()-1][sign] ++;
      
      

      nevents_EE_++;


      //fill the endc vectors

      xBranch[nhitEndc]=hit.ix();
      yBranch[nhitEndc]=hit.iy();
      et_endc_Branch[nhitEndc]=et;
      lc_endc_Branch[nhitEndc]=lc.first;
      alpha_endc_Branch[nhitEndc]=lc.second;
      sign_endc_Branch[nhitEndc]=sign;

      ++nhitEndc;

      pass=true;
     }
 
   

    if (eventSet_==1) {
      // apply a miscalibration to all crystals and increment the 
      // ET sum, combined for all crystals
      for (int imiscal=0; imiscal<kNMiscalBinsEE; imiscal++) {
	if (miscalEE_[imiscal]*e> eCut_endc && et*miscalEE_[imiscal] < et_thr && e_.goodCell_endc[hit.ix()-1][hit.iy()-1][sign]){
	  int ring = e_.endcapRing_[hit.ix()-1][hit.iy()-1];
	  etsum_endc_miscal_[imiscal][ring] += miscalEE_[imiscal]*et;
	}
      }

      // spectra stuff
      if(spectra && hit.zside()>0) //POSITIVE!!!

	{
	  int ring = e_.endcapRing_[hit.ix()-1][hit.iy()-1];

	  et_spectrum_e_histos[ring]->Fill(et*1000.);
	  e_spectrum_e_histos[ring]->Fill(e*1000.);

	  if(ring==16)
	    {
	      int iphi_endc = 0;
	      for (int ip=0; ip<e_.nRing_[ring]; ip++) {
		if (phi==e_.phi_endc_[ip][ring]) iphi_endc=ip;
	      }

	    }
	}//if spectra

    }//if eventSet_==1
  }//for endc

  if (pass) {
    nevents_++;
    eventsinrun_++;
    eventsinlb_++;

  }

  if(nhitBarl>0 || nhitEndc>0 )     
    variablesTree_->Fill();

}

void PhiSymmetryCalibration::endRun(edm::Run& run, const edm::EventSetup&){
 
  
  std::cout  << "PHIREPRT : run "<< run.run() 
             << " start " << (run.beginTime().value()>>32)
             << " end "   << (run.endTime().value()>>32) 
             << " dur "   << (run.endTime().value()>>32)- (run.beginTime().value()>>32)
	  
             << " npass "      << eventsinrun_  << std::endl;
  eventsinrun_=0;        
 
  return ;

}

//_____________________________________________________________________________

void PhiSymmetryCalibration::getKfactors()
{

  /*  float epsilon_T_eb[kNMiscalBinsEB];
      float epsilon_M_eb[kNMiscalBinsEB];*/
  float epsilon_T_ee[kNMiscalBinsEE];
  float epsilon_M_ee[kNMiscalBinsEE];


  std::vector<TGraph*>  k_barl_graph(kBarlRings);
  std::vector<TCanvas*> k_barl_plot(kBarlRings);


    
 

  
  for (int ieta=0; ieta<kBarlRings; ieta++) {
    
    if(calculatePhikFactor_){
      for (int iphi=0; iphi<kBarlWedges; iphi++){
	for(int isign=0;isign<kSides; isign++){
	  for (int imiscal=0; imiscal<kNMiscalBinsEB; imiscal++) {
	    int middlebin =  int (kNMiscalBinsEB/2); 

	    for(int itree=0;itree<NMISCALIBRATION;++itree){
	      eta_var[itree]=ieta+1;
	      phi_var[itree]=iphi+1;
	      sign_var[itree]=isign;
	      miscal_var[itree]=-1;
	      etsum_var[itree]=-1;
	      etsum_first_var[itree]=-1;
	      etsum_second_var[itree]=-1;
	      nhits_var[itree]=-1;
	      nhits_first_var[itree]=-1;
	      nhits_second_var[itree]=-1;
	      
	    if(etsum_barl_miscal_phi_[itree][middlebin][ieta][iphi][isign]!=0) {
	      miscal_var[itree]= miscalEB_[imiscal] - 1.;
	      etsum_var[itree] = etsum_barl_miscal_phi_[itree][imiscal][ieta][iphi][isign];
	      nhits_var[itree]=nhits_barl_miscal_[itree][imiscal][ieta][iphi][isign];
	    }	  
	    
	    if(etsum_barl_miscal_firstinterval_[itree][middlebin][ieta][iphi][isign]!=0){
	      etsum_first_var[itree] = etsum_barl_miscal_firstinterval_[itree][imiscal][ieta][iphi][isign];
	      nhits_first_var[itree] = nhits_barl_miscal_firstinterval_[itree][imiscal][ieta][iphi][isign];
	    }
	    
	    if(etsum_barl_miscal_secondinterval_[itree][middlebin][ieta][iphi][isign]!=0){
	      etsum_second_var[itree] = etsum_barl_miscal_secondinterval_[itree][imiscal][ieta][iphi][isign];
	      nhits_second_var[itree] = nhits_barl_miscal_secondinterval_[itree][imiscal][ieta][iphi][isign];
	      
	    }
	    if(miscal_var[itree] != -1 || etsum_var[itree] != -1 || nhits_var[itree] != -1 || etsum_first_var[itree] != -1 || nhits_first_var[itree] != -1 ||  etsum_second_var[itree] != -1 || nhits_second_var[itree] != -1){
	      //	      cout<<"filled tree 2"<< miscal_var[itree]<<" "<<etsum_var[itree]<<" "<<etsum_first_var[itree]<<" "<<etsum_second_var[itree]<<" "<<
	      //		nhits_var[itree]<<" "<<nhits_first_var[itree]<<" "<<nhits_second_var[itree]<<" "<<eta_var[itree]<<" "<<phi_var[itree]<<" "<<sign_var[itree]<<endl;

	      kFactorTree[itree]->Fill();   
	      //	      cout<<"fill ok"<<endl;
	    }
	    }//ciclo su tree
	    //	    cout<<"fill ok"<<endl; 
	  }
	  
	  
	}//endisign
      }//endif iphi
      
    }//endif calculate phikfactor
    
  }


  std::vector<TGraph*>  k_endc_graph(kEndcEtaRings);
  std::vector<TCanvas*> k_endc_plot(kEndcEtaRings);

  for (int ring=0; ring<kEndcEtaRings; ring++) {
    for (int imiscal=0; imiscal<kNMiscalBinsEE; imiscal++) {
      int middlebin =  int (kNMiscalBinsEE/2);
      epsilon_T_ee[imiscal] = etsum_endc_miscal_[imiscal][ring]/etsum_endc_miscal_[middlebin][ring] - 1.;
      epsilon_M_ee[imiscal] = miscalEE_[imiscal] - 1.;
    }
    k_endc_graph[ring] = new TGraph (kNMiscalBinsEE,epsilon_M_ee,epsilon_T_ee);
    k_endc_graph[ring]->Fit("pol1");

    ostringstream t;
    t<< "k_endc_"<< ring+1;
    k_endc_plot[ring] = new TCanvas(t.str().c_str(),"");
    k_endc_plot[ring]->SetFillColor(10);
    k_endc_plot[ring]->SetGrid();
    k_endc_graph[ring]->SetMarkerSize(1.);
    k_endc_graph[ring]->SetMarkerColor(4);
    k_endc_graph[ring]->SetMarkerStyle(20);
    k_endc_graph[ring]->GetXaxis()->SetLimits(-1*kMiscalRangeEE,kMiscalRangeEE);
    k_endc_graph[ring]->GetXaxis()->SetTitleSize(.05);
    k_endc_graph[ring]->GetYaxis()->SetTitleSize(.05);
    k_endc_graph[ring]->GetXaxis()->SetTitle("#epsilon_{M}");
    k_endc_graph[ring]->GetYaxis()->SetTitle("#epsilon_{T}");
    k_endc_graph[ring]->Draw("AP");

    k_endc_[ring] = k_endc_graph[ring]->GetFunction("pol1")->GetParameter(1);
    //    std::cout << "k_endc_[" << ring << "]=" << k_endc_[ring] << std::endl;
  }
 
  kFactorFile->cd();
  for(int itree=0;itree<NMISCALIBRATION;++itree){
  kFactorTree[itree]->Write();
  }
  kFactorFile->Close();

  for (int ieta=0; ieta<kBarlRings; ieta++) { 
    delete k_barl_plot[ieta]; 
    delete k_barl_graph[ieta];

  }
  for (int ring=0; ring<kEndcEtaRings; ring++) { 
    //k_endc_plot[ring]->Write();
    delete k_endc_plot[ring];
    delete k_endc_graph[ring];
  }
  // f.Close();

}

//____________________________________________________________________________

std::pair<float,float> PhiSymmetryCalibration::getLaserCorrection(DetId const & xid, edm::Timestamp const & iTime, bool invertZSide) const
{
  std::pair<float,float> correctionFactor = std::make_pair<float,float>(1.0,1.0);

  if (!mAPDPNRatios_ || !mAPDPNRatiosRef_)
    {
      edm::LogError("EcalLaserDbService") << "Laser DB info not found" << endl;
      return correctionFactor;
    }

  const EcalLaserAPDPNRatios::EcalLaserAPDPNRatiosMap& laserRatiosMap =  mAPDPNRatios_->getLaserMap();
  const EcalLaserAPDPNRatios::EcalLaserTimeStampMap& laserTimeMap =  mAPDPNRatios_->getTimeMap();
  const EcalLaserAPDPNRatiosRefMap& laserRefMap =  mAPDPNRatiosRef_->getMap();
  const EcalLaserAlphaMap& laserAlphaMap =  mAlphas_->getMap();

  EcalLaserAPDPNRatios::EcalLaserAPDPNpair apdpnpair;
  EcalLaserAPDPNRatios::EcalLaserTimeStamp timestamp;
  EcalLaserAPDPNref apdpnref;
  EcalLaserAlpha alpha;

  if (xid.det()==DetId::Ecal) {
    //    std::cout << " XID is in Ecal : ";
  } else {
    //    std::cout << " XID is NOT in Ecal : ";
    edm::LogError("EcalLaserDbService") << " DetId is NOT in ECAL" << endl;
    return correctionFactor;
  } 
  
  int hi = -1;
  if (xid.subdetId()==EcalBarrel) {
    //    std::cout << "EcalBarrel" << std::endl;
    //    std::cout << "--> rawId() = " << xid.rawId() << "   id() = " << EBDetId( xid ).hashedIndex() << std::endl;
    hi = EBDetId( xid ).hashedIndex();
  } else if (xid.subdetId()==EcalEndcap) {
    //    std::cout << "EcalEndcap" << std::endl;
    hi = EEDetId( xid  ).hashedIndex() + EBDetId::MAX_HASH + 1;
  } else {
    //    std::cout << "NOT EcalBarrel or EcalEndCap" << std::endl;
    edm::LogError("EcalLaserDbService") << " DetId is NOT in ECAL Barrel or Endcap" << endl;
    return correctionFactor;
  }
  
  int iLM;
  if (xid.subdetId()==EcalBarrel) {
    EBDetId ebid( xid.rawId() );
    iLM = MEEBGeom::lmr(ebid.ieta(), ebid.iphi());
  } else if (xid.subdetId()==EcalEndcap) {
    EEDetId eeid( xid.rawId() );
    // SuperCrystal coordinates
    MEEEGeom::SuperCrysCoord iX = (eeid.ix()-1)/5 + 1;
    MEEEGeom::SuperCrysCoord iY = (eeid.iy()-1)/5 + 1;    
    iLM = MEEEGeom::lmr(iX, iY, eeid.zside());    
  } else {
    edm::LogError("EcalLaserDbService") << " DetId is NOT in ECAL Barrel or Endcap" << endl;
    return correctionFactor;
  }
  //  std::cout << " LM num ====> " << iLM << endl;
  
  // get alpha, apd/pn ref, apd/pn pairs and timestamps for interpolation
  DetId myId;
  if (xid.subdetId()==EcalEndcap && invertZSide)
    myId=EEDetId(EEDetId(xid).ix(),EEDetId(xid).iy(),-1*EEDetId(xid).zside());
  else
    myId=xid;

  EcalLaserAPDPNRatios::EcalLaserAPDPNRatiosMap::const_iterator itratio = laserRatiosMap.find(myId);
  if (itratio != laserRatiosMap.end()) {
    apdpnpair = (*itratio);
  } else {
    edm::LogError("EcalLaserDbService") << "error with laserRatiosMap!" << endl;     
    return correctionFactor;
  }
  
  if (iLM-1< (int)laserTimeMap.size()) {
    timestamp = laserTimeMap[iLM-1];  
  } else {
    edm::LogError("EcalLaserDbService") << "error with laserTimeMap!" << endl;     
    return correctionFactor;
  }
  
  EcalLaserAPDPNRatiosRefMap::const_iterator itref = laserRefMap.find(xid);
  if ( itref != laserRefMap.end() ) {
    apdpnref = (*itref);
  } else { 
    edm::LogError("EcalLaserDbService") << "error with laserRefMap!" << endl;     
    return correctionFactor;
  }
  
  EcalLaserAlphaMap::const_iterator italpha = laserAlphaMap.find(xid);
  if ( italpha != laserAlphaMap.end() ) {
    alpha = (*italpha);
  } else {
    edm::LogError("EcalLaserDbService") << "error with laserAlphaMap!" << endl;     
    return correctionFactor;
  }

  //   std::cout << " APDPN pair " << apdpnpair.p1 << " , " << apdpnpair.p2 << std::endl; 
  //   std::cout << " TIME pair " << timestamp.t1.value() << " , " << timestamp.t2.value() << " iLM " << iLM << std::endl; 
  //   std::cout << " LM module " << iLM << std::endl;
  //   std::cout << " APDPN ref " << apdpnref << std::endl; 
  
  //    std::cout << " ALPHA " << alpha << std::endl; 
  
  // should implement some default in case of error...
  
  // should do some quality checks first
  // ...
  
  // we will need to treat time differently...
  // is time in DB same format as in MC?  probably not...
  
  // interpolation
  
  edm::TimeValue_t t = iTime.value();
  edm::TimeValue_t t_i = 0, t_f = 0;
  float p_i = 0, p_f = 0;
  
  if ( t >= timestamp.t1.value() && t < timestamp.t2.value() ) {
    t_i = timestamp.t1.value();
    t_f = timestamp.t2.value();
    p_i = apdpnpair.p1;
    p_f = apdpnpair.p2;
  } else if ( t >= timestamp.t2.value() && t <= timestamp.t3.value() ) {
    t_i = timestamp.t2.value();
    t_f = timestamp.t3.value();
    p_i = apdpnpair.p2;
    p_f = apdpnpair.p3;
  } else if ( t < timestamp.t1.value() ) {
    t_i = timestamp.t1.value();
    t_f = timestamp.t2.value();
    p_i = apdpnpair.p1;
    p_f = apdpnpair.p2;
    //edm::LogWarning("EcalLaserDbService") << "The event timestamp t=" << t 
    //        << " is lower than t1=" << t_i << ". Extrapolating...";
  } else if ( t > timestamp.t3.value() ) {
    t_i = timestamp.t2.value();
    t_f = timestamp.t3.value();
    p_i = apdpnpair.p2;
    p_f = apdpnpair.p3;
    //edm::LogWarning("EcalLaserDbService") << "The event timestamp t=" << t 
    //        << " is greater than t3=" << t_f << ". Extrapolating...";
  }

  if ( apdpnref != 0 && (t_i - t_f) != 0) {
    float interpolatedLaserResponse = p_i/apdpnref + (t-t_i)*(p_f-p_i)/apdpnref/(t_f-t_i);
    if ( interpolatedLaserResponse <= 0 ) {
      edm::LogError("EcalLaserDbService") << "The interpolated laser correction is <= zero! (" 
					  << interpolatedLaserResponse << "). Using 1. as correction factor.";
      return correctionFactor;
    } else {
      //correctionFactor = interpolatedLaserResponse;
      correctionFactor =std::make_pair<float,float>( 1/pow(interpolatedLaserResponse,alpha) , (float) alpha );
    }
    //  std::cout << "correction factor " << correctionFactor << std::endl;
  } else {
    edm::LogError("EcalLaserDbService") 
      << "apdpnref (" << apdpnref << ") "
      << "or t_i-t_f (" << (t_i - t_f) << " is zero!";
    return correctionFactor;
  }
 
  return correctionFactor;
}




//_____________________________________________________________________________



void PhiSymmetryCalibration::setUp(const edm::EventSetup& setup){

  edm::ESHandle<EcalChannelStatus> chStatus;
  setup.get<EcalChannelStatusRcd>().get(chStatus);

  edm::ESHandle<CaloGeometry> geoHandle;
  setup.get<CaloGeometryRecord>().get(geoHandle);

  e_.setup(&(*geoHandle), &(*chStatus), statusThreshold_);
 
  
  if (reiteration_){   
    
    EcalCondHeader h;
    // namespace fs = boost::filesystem;
//     fs::path p(oldcalibfile_.c_str(),fs::native);
//     if (!fs::exists(p)) edm::LogError("PhiSym") << "File not found: " 
// 						<< oldcalibfile_ <<endl;
    
    edm::FileInPath fip("Calibration/EcalCalibAlgos/data/"+oldcalibfile_);
    

    
    int ret=
    EcalIntercalibConstantsXMLTranslator::readXML(fip.fullPath(),h,oldCalibs_);    
    if (ret) edm::LogError("PhiSym")<<"Error reading XML files"<<endl;;
    
  } else {
    // in fact if not reiterating, oldCalibs_ will never be used
    edm::ESHandle<EcalIntercalibConstants> pIcal;      
    setup.get<EcalIntercalibConstantsRcd>().get(pIcal);
    oldCalibs_=*pIcal;

  }
  
}


void PhiSymmetryCalibration::endLuminosityBlock(edm::LuminosityBlock const& lb, edm::EventSetup const&){

  
  if ((lb.endTime().value()>>32)- (lb.beginTime().value()>>32) <60 ) 
    return;

  std::cout  << "PHILB : run "<< lb.run()
             << " id " << lb.id() 
             << " start " << (lb.beginTime().value()>>32)
             << " end "   << (lb.endTime().value()>>32) 
             << " dur "   << (lb.endTime().value()>>32)- (lb.beginTime().value()>>32)
    
             << " npass "      << eventsinlb_  << std::endl;
  
  eventsinlb_=0;

}
