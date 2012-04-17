{
  TChain c("mapTree_barl");
  c.Add("/xrootdfs/cms/local/meridian/EFlow/hitsMaps/makeMapOut_AlCaPhiSym_Run2012A-v1_RAW*root");
//   TFile* f= TFile::Open("map_barl2011A_new.root");
//   TTree* intree= (TTree*)f->Get("mapTree_barl");
//  gSystem->Load("libJSON.so");
  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L readMap.C++");
  readMap t(&c);
  //  t.setJSON("Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt");
  t.outFileName="readMap_Run2012A_v1_190456_191277_test.root";
  t.setJSON("analyzed_AlCaPhiSym_Run2012A-v1_RAW.json");
  t.setHitsMaxPerXtal(1500);
  t.setMaxStopHours(12);  
  t.Loop();
}
