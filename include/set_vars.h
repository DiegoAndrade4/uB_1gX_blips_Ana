   // --- Declare variables and branch pointers ---
   
    float reco_nu_vtx_x, reco_nu_vtx_y, reco_nu_vtx_z; //Reco NuVtx
    int crtveto ; 
    float crthitpe ; 
                                                       //
    int nblips_saved;
    std::vector<float>* blip_x = nullptr;
    std::vector<float>* blip_y = nullptr;
    std::vector<float>* blip_z = nullptr;
    std::vector<float>* blip_dx = nullptr;
    std::vector<float>* blip_dw = nullptr;
    std::vector<float>* blip_energy = nullptr;
    std::vector<float>* blip_charge = nullptr;
    std::vector<int>* blip_nplanes = nullptr;
    std::vector<float>* blip_proxtrkdist = nullptr;
    std::vector<int>* blip_proxtrkid = nullptr;
    std::vector<bool>* blip_touchtrk = nullptr;
    std::vector<bool>* blip_touchtrkid = nullptr;
    std::vector<int>* blip_pl0_nwires = nullptr;
    std::vector<int>* blip_pl1_nwires = nullptr;
    std::vector<int>* blip_pl2_nwires = nullptr;
    std::vector<bool>* blip_pl0_bydeadwire = nullptr;
    std::vector<bool>* blip_pl1_bydeadwire = nullptr;
    std::vector<bool>* blip_pl2_bydeadwire = nullptr;
    std::vector<int>* blip_true_g4id = nullptr;
    std::vector<int>* blip_true_pdg = nullptr;
    std::vector<float>* blip_true_energy = nullptr;

 

void setBranches(TTree *tree){
    // --- Set branch addresses ---
    tree->SetBranchAddress("reco_nu_vtx_x", &reco_nu_vtx_x);
    tree->SetBranchAddress("reco_nu_vtx_y", &reco_nu_vtx_y);
    tree->SetBranchAddress("reco_nu_vtx_z", &reco_nu_vtx_z); 
    
    tree->SetBranchAddress("crtveto", &crtveto);
    tree->SetBranchAddress("crthitpe", &crthitpe);

    tree->SetBranchAddress("nblips_saved", &nblips_saved);
    tree->SetBranchAddress("blip_x", &blip_x);
    tree->SetBranchAddress("blip_y", &blip_y);
    tree->SetBranchAddress("blip_z", &blip_z);
    tree->SetBranchAddress("blip_dx", &blip_dx);
    tree->SetBranchAddress("blip_dw", &blip_dw);
    tree->SetBranchAddress("blip_energy", &blip_energy);
    tree->SetBranchAddress("blip_charge", &blip_charge);
    tree->SetBranchAddress("blip_nplanes", &blip_nplanes);
    tree->SetBranchAddress("blip_proxtrkdist", &blip_proxtrkdist);
    tree->SetBranchAddress("blip_proxtrkid", &blip_proxtrkid);
    tree->SetBranchAddress("blip_touchtrk", &blip_touchtrk);
    tree->SetBranchAddress("blip_touchtrkid", &blip_touchtrkid);
    tree->SetBranchAddress("blip_pl0_nwires", &blip_pl0_nwires);
    tree->SetBranchAddress("blip_pl1_nwires", &blip_pl1_nwires);
    tree->SetBranchAddress("blip_pl2_nwires", &blip_pl2_nwires);
    tree->SetBranchAddress("blip_pl0_bydeadwire", &blip_pl0_bydeadwire);
    tree->SetBranchAddress("blip_pl1_bydeadwire", &blip_pl1_bydeadwire);
    tree->SetBranchAddress("blip_pl2_bydeadwire", &blip_pl2_bydeadwire);
    tree->SetBranchAddress("blip_proxtrkdist", &blip_proxtrkdist);
    tree->SetBranchAddress("blip_true_g4id", &blip_true_g4id);
    tree->SetBranchAddress("blip_true_pdg", &blip_true_pdg);
    tree->SetBranchAddress("blip_true_energy", &blip_true_energy);
    
   }
