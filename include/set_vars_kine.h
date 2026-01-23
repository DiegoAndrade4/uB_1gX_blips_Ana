Float_t kine_reco_Enu;

  std::vector<float> *kine_energy_particle;
  std::vector<int> *kine_particle_type;


void setBranchesKINE(TTree *tree){
    // --- Set branch addresses ---

tree->SetBranchAddress("kine_reco_Enu", &kine_reco_Enu);

tree->SetBranchAddress("kine_energy_particle", &kine_energy_particle);
tree->SetBranchAddress("kine_particle_type", &kine_particle_type);

}
