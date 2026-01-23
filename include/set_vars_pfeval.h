   // --- Declare variables and branch pointers ---

//std::vector<float>* reco_showerMomentum = nullptr;


  Float_t reco_nuvtxX;
  Float_t reco_nuvtxY;
  Float_t reco_nuvtxZ;

  Float_t reco_showervtxX;
  Float_t reco_showervtxY;
  Float_t reco_showervtxZ;

Float_t reco_showerMomentum[4];



 Int_t   truth_single_photon;
 Bool_t truth_isCC;
 Int_t truth_NCDelta;

  Int_t   truth_showerPdg;
  Int_t   truth_showerMother;


 Float_t truth_muonMomentum[4];
Int_t truth_Npi0;






  // PF particle
  Int_t truth_Ntrack;  // number of tracks in MC
  Int_t truth_id[10000];  // track id; size == truth_Ntrack
  Int_t truth_pdg[10000];  // track particle pdg; size == truth_Ntrack
  std::vector<std::string > *truth_process;
  Int_t truth_mother[10000];  // mother id of this track; size == truth_Ntrack
  Float_t truth_startXYZT[10000][4];  // start position of this track; size == truth_Ntrack
  Float_t truth_endXYZT[10000][4];  // end position of this track; size == truth_Ntrack
  Float_t truth_startMomentum[10000][4];  // start momentum of this track; size == truth_Ntrack
  Float_t truth_endMomentum[10000][4];  // end momentum of this track; size == truth_Ntrack
  std::vector<std::vector<Int_t> > *truth_daughters;  // daughters id of this track; vector









void setBranchesPFEval(TTree *tree){
    // --- Set branch addresses ---
    tree->SetBranchAddress("reco_showerMomentum", &reco_showerMomentum);
  
  tree->SetBranchAddress("reco_nuvtxX", &reco_nuvtxX);
  tree->SetBranchAddress("reco_nuvtxY", &reco_nuvtxY);
  tree->SetBranchAddress("reco_nuvtxZ", &reco_nuvtxZ);

  tree->SetBranchAddress("reco_showervtxX", &reco_showervtxX);
  tree->SetBranchAddress("reco_showervtxY", &reco_showervtxY);
  tree->SetBranchAddress("reco_showervtxZ", &reco_showervtxZ);

 tree->SetBranchAddress("truth_single_photon",&truth_single_photon);
 tree->SetBranchAddress("truth_isCC", &truth_isCC);
 tree->SetBranchAddress("truth_NCDelta",&truth_NCDelta);
 tree->SetBranchAddress("truth_Npi0",&truth_Npi0);

 tree->SetBranchAddress("truth_showerPdg", &truth_showerPdg);
 tree->SetBranchAddress("truth_showerMother", &truth_showerMother);
  
 tree->SetBranchAddress("truth_muonMomentum", &truth_muonMomentum);







/*

    //if (tree->GetBranch("truth_Ntrack")){
    //flag_pf_truth = true;
    //if (!flag_init_pointers){
    //  init_pointers(tagger_info);
    //  flag_init_pointers = true;
    //}

    tree->SetBranchAddress("truth_Ntrack", &truth_Ntrack);
    tree->SetBranchAddress("truth_id", &truth_id);
    tree->SetBranchAddress("truth_pdg", &truth_pdg);
    tree->SetBranchAddress("truth_process", &truth_process);
    tree->SetBranchAddress("truth_mother", &truth_mother);
    tree->SetBranchAddress("truth_startXYZT", &truth_startXYZT);
    tree->SetBranchAddress("truth_endXYZT", &truth_endXYZT);
    tree->SetBranchAddress("truth_startMomentum", &truth_startMomentum);
    tree->SetBranchAddress("truth_endMomentum", &truth_endMomentum);
    tree->SetBranchAddress("truth_daughters", &truth_daughters);
    tree->SetBranchAddress("fMC_trackPosition", &fMC_trackPosition);

    tree->SetBranchAddress("mc_isnu", &mc_isnu);
    tree->SetBranchAddress("mc_nGeniePrimaries", &mc_nGeniePrimaries);
    tree->SetBranchAddress("mc_nu_pdg", &mc_nu_pdg);
    tree->SetBranchAddress("mc_nu_ccnc", &mc_nu_ccnc);
    tree->SetBranchAddress("mc_nu_mode", &mc_nu_mode);
    tree->SetBranchAddress("mc_nu_intType", &mc_nu_intType);
    tree->SetBranchAddress("mc_nu_target", &mc_nu_target);
    tree->SetBranchAddress("mc_hitnuc", &mc_hitnuc);
    tree->SetBranchAddress("mc_hitquark", &mc_hitquark);
    tree->SetBranchAddress("mc_nu_Q2", &mc_nu_Q2);
    tree->SetBranchAddress("mc_nu_W", &mc_nu_W);
    tree->SetBranchAddress("mc_nu_X", &mc_nu_X);
    tree->SetBranchAddress("mc_nu_Y", &mc_nu_Y);
    tree->SetBranchAddress("mc_nu_Pt", &mc_nu_Pt);
    tree->SetBranchAddress("mc_nu_Theta", &mc_nu_Theta);
    tree->SetBranchAddress("mc_nu_pos", &mc_nu_pos);
    tree->SetBranchAddress("mc_nu_mom", &mc_nu_mom);

//  }


*/











   }
