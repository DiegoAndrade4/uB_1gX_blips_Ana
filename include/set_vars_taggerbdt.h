    // numu vs. nc tagger
    float numu_cc_flag;

    bool flag_single_photon_bdt;
    float single_photon_numu_score;
    float single_photon_other_score;
    float single_photon_ncpi0_score;
    float single_photon_nue_score;

    float shw_sp_n_20mev_showers;
    float shw_sp_n_20br1_showers;

    Int_t run;
    Int_t subrun;
    Int_t event;


void setBranchesBDT(TTree *tree){
    // --- Set branch addresses ---
   tree->SetBranchAddress("single_photon_numu_score", &single_photon_numu_score);
   tree->SetBranchAddress("single_photon_other_score", &single_photon_other_score);
   tree->SetBranchAddress("single_photon_ncpi0_score", &single_photon_ncpi0_score);
   tree->SetBranchAddress("single_photon_nue_score", &single_photon_nue_score);

   tree->SetBranchAddress("shw_sp_n_20mev_showers",&shw_sp_n_20mev_showers);
   tree->SetBranchAddress("shw_sp_n_20br1_showers",&shw_sp_n_20br1_showers);


  tree->SetBranchAddress("run",&run);
  tree->SetBranchAddress("subrun",&subrun);
  tree->SetBranchAddress("event",&event);



  tree->SetBranchAddress("numu_cc_flag",&numu_cc_flag);


}
