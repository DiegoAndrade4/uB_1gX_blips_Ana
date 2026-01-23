

 Float_t match_completeness_energy;

 Float_t truth_energyInside;

 Bool_t truth_vtxInside;

 Int_t truth_nuPdg;

void setBranchesEval(TTree *tree){
    // --- Set branch addresses ---

tree->SetBranchAddress("match_completeness_energy", &match_completeness_energy); 
tree->SetBranchAddress("truth_energyInside", &truth_energyInside);
//tree0->Branch("truth_energyInside", &eval_info.truth_energyInside,"data/F");
tree->SetBranchAddress("truth_vtxInside", &truth_vtxInside);
tree->SetBranchAddress("truth_nuPdg", &truth_nuPdg);

}
