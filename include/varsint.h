
  // Set max array sizes
	const int kMaxHits  = 3000000;
	const int kMaxBlips = 500000;
	const int kMaxTrks  = 50000;
	const int kMaxG4    = 2000000;
	const int kMaxEDeps = 10000000;
	const int kNplanes  = 3;  
  const int kMaxShwrs = 100000;    
  
  // --- Event information ---   
  int           event;                    // event number
  int           run;                      // run number
  int           subrun;                      // subrun number
  unsigned int  timestamp;                // unix time of event
  float         lifetime;                 // electron lifetime
 
  float nu_reco_vtx_x; // reconstructed Nu Vtx x
  float nu_reco_vtx_y; // reconstructed Nu Vtx y
  float nu_reco_vtx_z; // reconstructed Nu Vtx z		      

  float reco_nu_vtx_x; // reconstructed Nu Vtx x
  float reco_nu_vtx_y; // reconstructed Nu Vtx y
  float reco_nu_vtx_z; // reconstructed Nu Vtx z 

  // --- G4 information ---
  int   nparticles;               // number of G4 particles
  bool  part_isPrimary[kMaxG4];        // is primary particle
  int   part_trackID[kMaxG4];          // G4 track ID
  int   part_pdg[kMaxG4];              // PDG
  int   part_nDaughters[kMaxG4];       // number of daughters
  int   part_mother[kMaxG4];           // mother particle
  float part_E[kMaxG4];                // initial energy (MeV)
  float part_KE[kMaxG4];               // initial kinetic energy (MeV)
  float part_endE[kMaxG4];             // final energy (MeV)
  float part_endKE[kMaxG4];             // final energy (MeV)
  float part_mass[kMaxG4];             // mass (MeV)
  float part_P[kMaxG4];                // momentum (MeV)
  float part_Px[kMaxG4];               // momentum x (MeV)
  float part_Py[kMaxG4];               // momentum y (MeV)
  float part_Pz[kMaxG4];               // momentum z (MeV)
  float part_startPointx[kMaxG4];      // starting x (cm)
  float part_startPointy[kMaxG4];      // starting y (cm)
  float part_startPointz[kMaxG4];      // starting y (cm)
//  float part_endPointx[kMaxG4];        // ending x (cm)
//  float part_endPointy[kMaxG4];        // ending y (cm)
//  float part_endPointz[kMaxG4];        // ending y (cm)
  float   part_startT[kMaxG4];           // starting time (us)
  float part_endT[kMaxG4];             // ending time (us)
  float part_pathlen[kMaxG4];          // path length (cm)
  float part_depEnergy[kMaxG4];        // energy deposited in AV (MeV)
  int   part_depElectrons[kMaxG4];     // electrons deposited
//  float part_numElectrons[kMaxG4];     // electrons reaching anode wires
  std::vector<std::string> *part_process;// process name
  //float total_depEnergy;          // total deposited energy in AV
  //int   total_depElectrons;       // total deposited ionization electrons in AV
  //float total_numElectrons;       // total electrons reaching anode wires

  // --- True energy deposit info (derived from SimChannels and SimEnergyDeposits) ---
  int   nedeps;                   // number of true localized energy depositions
  bool  edep_isPrimary[kMaxEDeps]; // matched to a primary generated particle
  int   edep_tpc[kMaxEDeps];      // TPC
  int   edep_g4id[kMaxEDeps];     // leading G4 track ID
  int   edep_g4index[kMaxEDeps];  // leading G4 track index
  int   edep_pdg[kMaxEDeps];      // leading G4 track PDG
  int   edep_clustid[kMaxEDeps];  // hitclust ID
  int   edep_blipid[kMaxEDeps];   // reconstructed blip ID
  float edep_energy[kMaxEDeps];   // total energy deposited (MeV)
  int   edep_depne[kMaxEDeps];    // total ionization electrons deposited
  float edep_charge[kMaxEDeps];   // total electrons reaching anode wires
  float edep_x[kMaxEDeps];        // x (cm)
  float edep_y[kMaxEDeps];        // y (cm)
  float edep_z[kMaxEDeps];        // z (cm)
  int   edep_proc[kMaxEDeps];     // encodes particle process
  // --- Hit information ---
  int	  nhits;                    // number of hits
//  int	  hit_tpc[kMaxHits];        // tpc number
  int	  hit_plane[kMaxHits];      // plane number
  int	  hit_wire[kMaxHits];       // wire number
  int	  hit_channel[kMaxHits];    // channel ID
  float	hit_peakT[kMaxHits];      // raw peak time (tick)
  float	hit_time[kMaxHits];       // corrected peak time (tick)
  float hit_rms[kMaxHits];        // shape RMS
  float	hit_amp[kMaxHits];         // amplitude
  float	hit_area[kMaxHits];       // charge (area) in ADC units
  float hit_sumadc[kMaxHits];     // summed ADC
  float hit_charge[kMaxHits];     // reconstructed number of electrons
  int   hit_mult[kMaxHits];       // multiplicity
  int	  hit_trkid[kMaxHits];      // is this hit associated with a reco track?
  int   hit_ismatch[kMaxHits];    // does hit have time match on another plane?
  int	  hit_g4id[kMaxHits];       // G4 TrackID of leading particle
  float hit_g4frac[kMaxHits];     // fraction of hit energy from leading MCParticle
  float hit_g4energy[kMaxHits];   // true energy
  float hit_g4charge[kMaxHits];   // true number of electrons (drift-attenuated)
  int   hit_clustid[kMaxHits];    // key of HitClust in which hit was included
  int   hit_blipid[kMaxHits];     // key of Blip in which hit was included
  float hit_gof[kMaxHits];        // goodness of fit (default -1)

  // --- Track information ---
  int   ntrks;                    // number tracks
  int   longtrks;                    // number of long tracks
  int   trk_id[kMaxTrks];         // trackID
  int   trk_npts[kMaxTrks];       // number 3D trajectory points
  float trk_length[kMaxTrks];     // track length [cm]
  float trk_startx[kMaxTrks];     // starting X coordinate
  float trk_starty[kMaxTrks];     // starting Y coordinate
  float trk_startz[kMaxTrks];     // starting Z coordinate
  float trk_startd[kMaxTrks];     // starting distance to boundary
  float trk_endx[kMaxTrks];       // ending X coordinate
  float trk_endy[kMaxTrks];       // ending Y coordinate
  float trk_endz[kMaxTrks];       // ending Z coordinate
  float trk_endd[kMaxTrks];       // ending distance to boundary
  bool trk_isMC[kMaxTrks];         //is a MC track
  // --- Hit cluster information ---
  int   nclusts;                      // total clusters made
  int   clust_id[kMaxHits];           // cluster ID (index)
//  int   clust_tpc[kMaxHits];          // cluster TPC ID
  int   clust_plane[kMaxHits];        // cluster plane
  int   clust_wire[kMaxHits];         // central-most wire of cluster
  int   clust_startwire[kMaxHits];    // starting wire
  int   clust_endwire[kMaxHits];      // ending wire
  int   clust_nwires[kMaxHits];       // number of wires in this cluster
  int   clust_nhits[kMaxHits];        // number of hits
  int   clust_time[kMaxHits];         // charge-weighted time
  float clust_timespan[kMaxHits];     // cluster timespan
  float clust_rms[kMaxHits];          // charge-weighted RMS
  float clust_starttime[kMaxHits];    // cluster start tick
  float clust_endtime[kMaxHits];      // cluster end tick
  float clust_amp[kMaxHits];          // maximum hit amplitude [ADC]
  int   clust_charge[kMaxHits];       // cluster charge at anode [e-]
//int   clust_g4charge[kMaxHits];     // true cluster charge at anode
//  float clust_g4energy[kMaxHits];     // true cluster energy from G4
  int   clust_g4id[kMaxHits];         // true MCParticle ID (index for particle branches)
  int   clust_blipid[kMaxHits];       // blip ID for this nlusteer (if it was made into one)
  int   clust_edepid[kMaxHits];       // true energy dep ID
  bool  clust_ismatch[kMaxHits];      // was this cluster plane-matched?
  //int   clust_lhit_wire[kMaxHits];    // cluster wire (lead hit wire)
  //int   clust_lhit_chan[kMaxHits];    // cluster channel (lead hit wire)
  //int   clust_lhit_id[kMaxHits];      // lead hit ID (index for hit_X[i] branches)
  //float clust_lhit_amp[kMaxHits];     // lead hit peak amplitude [ADC]
  //float clust_lhit_rms[kMaxHits];     // lead hit RMS [ADC]
  //float clust_lhit_time[kMaxHits];    // lead hit time [ticks]
  //float clust_lhit_gof[kMaxHits];     // lead hit goodness-of-fit; pulse train = -1
  //bool  clust_lhit_isfit[kMaxHits];   // is there a valid goodness of fit for lead hit?

  // --- 3D Blip information ---
  int   nblips;                       // number of blips in event
  int   blip_id[kMaxBlips];           // blip ID / index
  //int   blip_tpc[kMaxBlips];          // blip TPC
  int   blip_nplanes[kMaxBlips];      // number of planes matched (2 or 3)
  float blip_x[kMaxBlips];            // X position [cm]
  float blip_y[kMaxBlips];            // Y position [cm]
  float blip_z[kMaxBlips];            // Z position [cm]
  float blip_sigmayz[kMaxBlips];      // difference in wire intersection points
  float blip_dx[kMaxBlips];           // dX [cm]
  float blip_dyz[kMaxBlips];
  float blip_dw[kMaxBlips];
  float blip_size[kMaxBlips];
  float blip_sumadc[kMaxBlips];          // integrated ADCs 
  int blip_charge[kMaxBlips];       // blip charge at anode [e-]
  float blip_energy[kMaxBlips];       // blip energy [MeV]
  int   blip_edepid[kMaxBlips];       // true energy dep ID
  float blip_trkdist[kMaxBlips];      // distance to nearest track
  int   blip_trkid[kMaxBlips];        // index of nearest trk
  float blip_trklength[kMaxBlips];
  bool  blip_incylinder[kMaxBlips];   // is blip within a cylinder near a track
  float blip_time[kMaxBlips];                   // blip drift time [ticks]
  int   blip_clustid[kNplanes][kMaxBlips];     // cluster ID per plane
int   blip_pl2_clustid[kMaxBlips];     // cluster ID per plane
int   blip_pl1_clustid[kMaxBlips];     // cluster ID per plane
int   blip_pl0_clustid[kMaxBlips];     // cluster ID per plane					     

float blip_proxtrkdist[kMaxBlips];
int blip_proxtrkid[kMaxBlips];
bool  blip_touchtrk[kMaxBlips];   // is blip touching a track
bool clust_bydeadwire[kMaxBlips]; // is cluster in blip by a dead wire

int blip_true_g4id[kMaxBlips];
int blip_true_pdg[kMaxBlips]; 
float blip_true_energy[kMaxBlips];
bool blip_pl2_bydeadwire[kMaxBlips];


void setBranches(TTree *tree){
  
  // for use on saved vectors of strings, for example:
  // tree->SetBranchAddress("processname",&_processname,&br);
  TBranch *br = 0; 
  tree->SetBranchAddress("event",&event);                         
  tree->SetBranchAddress("run",&run);
  tree->SetBranchAddress("subrun",&subrun);  
  tree->SetBranchAddress("timestamp",&timestamp);                            
  tree->SetBranchAddress("nu_reco_vtx_x",&nu_reco_vtx_x); 
  tree->SetBranchAddress("nu_reco_vtx_y",&nu_reco_vtx_y);
  tree->SetBranchAddress("nu_reco_vtx_z",&nu_reco_vtx_z);

  tree->SetBranchAddress("reco_nu_vtx_x",&reco_nu_vtx_x);
  tree->SetBranchAddress("reco_nu_vtx_y",&reco_nu_vtx_y);
  tree->SetBranchAddress("reco_nu_vtx_z",&reco_nu_vtx_z);
//  tree->SetBranchAddress("total_depEnergy",&total_depEnergy);     
//  tree->SetBranchAddress("total_numElectrons",&total_numElectrons);
  tree->SetBranchAddress("nparticles",&nparticles);               
  tree->SetBranchAddress("part_isPrimary",&part_isPrimary);                  
  tree->SetBranchAddress("part_trackID",&part_trackID);                      
  tree->SetBranchAddress("part_pdg",&part_pdg);                              
  tree->SetBranchAddress("part_nDaughters",&part_nDaughters);                
  tree->SetBranchAddress("part_mother",&part_mother);                        
  tree->SetBranchAddress("part_KE",&part_KE);                                  
 // tree->SetBranchAddress("part_endE",&part_endE);                            
 // tree->SetBranchAddress("part_mass",&part_mass);                            
//  tree->SetBranchAddress("part_P",&part_P);                                  
  tree->SetBranchAddress("part_Px",&part_Px);                                
  tree->SetBranchAddress("part_Py",&part_Py);                                
  tree->SetBranchAddress("part_Pz",&part_Pz);                                
  tree->SetBranchAddress("part_startPointx",&part_startPointx);              
  tree->SetBranchAddress("part_startPointy",&part_startPointy);              
  tree->SetBranchAddress("part_startPointz",&part_startPointz);              
//  tree->SetBranchAddress("part_endPointx",&part_endPointx);                  
//  tree->SetBranchAddress("part_endPointy",&part_endPointy);                  
//  tree->SetBranchAddress("part_endPointz",&part_endPointz);                  
  tree->SetBranchAddress("part_startT",&part_startT);                        
  tree->SetBranchAddress("part_endT",&part_endT);                            
  tree->SetBranchAddress("part_pathlen",&part_pathlen);                      
//  tree->SetBranchAddress("part_numElectrons",&part_numElectrons);            
  tree->SetBranchAddress("part_depEnergy",&part_depEnergy);                  
//  tree->SetBranchAddress("part_process",&part_process,&br);
  tree->SetBranchAddress("nedeps",&nedeps);                       
  tree->SetBranchAddress("edep_g4id",&edep_g4id); 
  tree->SetBranchAddress("edep_isPrimary",&edep_isPrimary);  
  tree->SetBranchAddress("edep_blipid",&edep_blipid);
  tree->SetBranchAddress("edep_pdg",&edep_pdg);  
  tree->SetBranchAddress("edep_clustid",&edep_clustid);            
  tree->SetBranchAddress("edep_energy",&edep_energy);              
  tree->SetBranchAddress("edep_charge",&edep_charge);              
  tree->SetBranchAddress("edep_x",&edep_x);                        
  tree->SetBranchAddress("edep_y",&edep_y);                        
  tree->SetBranchAddress("edep_z",&edep_z);
  tree->SetBranchAddress("edep_proc",&edep_proc);  
  //tree->SetBranchAddress("edep_ds",&edep_ds);
  tree->SetBranchAddress("nhits",&nhits);                         
//  tree->SetBranchAddress("hit_tpc",&hit_tpc);                      
  tree->SetBranchAddress("hit_plane",&hit_plane);                  
  tree->SetBranchAddress("hit_wire",&hit_wire);                    
  tree->SetBranchAddress("hit_peakT",&hit_peakT);                  
  tree->SetBranchAddress("hit_time",&hit_time);                    
  tree->SetBranchAddress("hit_rms",&hit_rms);                      
  tree->SetBranchAddress("hit_amp",&hit_amp);                        
  tree->SetBranchAddress("hit_area",&hit_area);                    
  tree->SetBranchAddress("hit_charge",&hit_charge);                
  //tree->SetBranchAddress("hit_isreal",&hit_isreal);                
  tree->SetBranchAddress("hit_trkid",&hit_trkid);                  
  tree->SetBranchAddress("hit_g4id",&hit_g4id);                    
  tree->SetBranchAddress("hit_g4frac",&hit_g4frac);                
  tree->SetBranchAddress("hit_g4energy",&hit_g4energy);            
  tree->SetBranchAddress("hit_g4charge",&hit_g4charge);            
  tree->SetBranchAddress("hit_clustid",&hit_clustid);              
  tree->SetBranchAddress("nclusts",&nclusts);                     
//  tree->SetBranchAddress("clust_tpc",&clust_tpc);                  
  tree->SetBranchAddress("clust_plane",&clust_plane);              
  tree->SetBranchAddress("clust_wire",&clust_wire);                
  tree->SetBranchAddress("clust_nwires",&clust_nwires);            
  tree->SetBranchAddress("clust_nhits",&clust_nhits);              
  tree->SetBranchAddress("clust_charge",&clust_charge);            
  tree->SetBranchAddress("clust_time",&clust_time);                
//  tree->SetBranchAddress("clust_g4charge",&clust_g4charge);        
//  tree->SetBranchAddress("clust_g4energy",&clust_g4energy);        
  tree->SetBranchAddress("clust_edepid",&clust_edepid);           
 // tree->SetBranchAddress("clust_lhit_rms",&clust_lhit_rms);           
 // tree->SetBranchAddress("clust_lhit_ph",&clust_lhit_ph);           
 //tree->SetBranchAddress("clust_lhit_peakT",&clust_lhit_peakT);          
 //tree->SetBranchAddress("clust_lhit_gof",&clust_lhit_gof);          
 // tree->SetBranchAddress("clust_timespan",&clust_timespan);          
  tree->SetBranchAddress("clust_blipid",&clust_blipid);            
  tree->SetBranchAddress("nblips",&nblips);                       
//  tree->SetBranchAddress("blip_tpc",&blip_tpc);                   
  tree->SetBranchAddress("blip_nplanes",&blip_nplanes);
//  tree->SetBranchAddress("blip_maxdiff",&blip_maxdiff);
  tree->SetBranchAddress("blip_x",&blip_x);                        
  tree->SetBranchAddress("blip_y",&blip_y);                        
  tree->SetBranchAddress("blip_z",&blip_z);                        
  tree->SetBranchAddress("blip_charge",&blip_charge);              
  //tree->SetBranchAddress("blip_charge_pl2",&blip_charge_pl2);              
  tree->SetBranchAddress("blip_edepid",&blip_edepid);
 tree->SetBranchAddress("blip_pl2_clustid",&blip_pl2_clustid);
 tree->SetBranchAddress("blip_pl1_clustid",&blip_pl1_clustid);
 tree->SetBranchAddress("blip_pl0_clustid",&blip_pl0_clustid);
 tree->SetBranchAddress("part_process",&part_process,&br);
 tree->SetBranchAddress("blip_dx",&blip_dx);
 tree->SetBranchAddress("blip_dyz",&blip_dyz);
 tree->SetBranchAddress("blip_dw",&blip_dw);
 tree->SetBranchAddress("blip_energy",&blip_energy);
 tree->SetBranchAddress("blip_size",&blip_size);
 tree->SetBranchAddress("blip_trklength",&blip_trklength);
tree->SetBranchAddress("blip_trkid",&blip_trkid);
tree->SetBranchAddress("blip_proxtrkdist",&blip_proxtrkdist);
tree->SetBranchAddress("blip_proxtrkid",&blip_proxtrkid);
tree->SetBranchAddress("blip_touchtrk",&blip_touchtrk);
tree->SetBranchAddress("clust_bydeadwire",&clust_bydeadwire);

tree->SetBranchAddress("ntrks",&ntrks);
tree->SetBranchAddress("longtrks",&longtrks);
tree->SetBranchAddress("trk_isMC",&trk_isMC);
tree->SetBranchAddress("trk_length",&trk_length);
tree->SetBranchAddress("trk_id",&trk_id);

tree->SetBranchAddress("blip_true_pdg",&blip_true_pdg);
tree->SetBranchAddress("blip_true_energy",&blip_true_energy);
tree->SetBranchAddress("blip_true_g4id",&blip_true_g4id);
tree->SetBranchAddress("blip_pl2_bydeadwire",&blip_pl2_bydeadwire);


}









//################################
// Print G4 particle
//################################
void printG4Particle(int i) {
// printf("  %5i  trkID: %-6i PDG: %-8i XYZ= %7.1f %7.1f %7.1f, dL=%7.1f, KE0=%9.3f, Edep=%9.3f, T=%8.1f --> %8.1f, moth=%5i, %12s, ND=%i\n",
  printf("  %5i   PDG: %-8i , KE0=%9.3f, Edep=%9.3f,  moth=%5i, %12s, ND=%i\n",
  i,
//  part_trackID[i],
  part_pdg[i],
//  part_startPointx[i],
//  part_startPointy[i],
//  part_startPointz[i],
//  part_pathlen[i],
  part_KE[i],
  part_depEnergy[i],
//  part_startT[i],
//  part_endT[i],
  part_mother[i],
  part_process->at(i).c_str(),
  part_nDaughters[i]
  );
}



void printG4ParticleI(int i) {
  printf("KE0=%9.3f, Edep=%9.3f, %12s\n",
 
 part_KE[i],
  part_depEnergy[i],
 part_process->at(i).c_str()
 );
}


//======================================
// Funtions fot Track-back particles
// ===================================

  std::vector<int>  FindAllAncestors(int);
  std::vector<int>  FindAllAncestorPDGs(int);
  void              MakeTrackIDMap();


//============================================
// Make a map from trackID to index in array
  std::map<int,int> map_g4trkid_index;


  //==================================================
  // Make a map of all TrackIDs to their array index
  //==================================================

  void MakeTrackIDMap(){
    map_g4trkid_index.clear();
    for(int j=0; j<nparticles; j++) 
      map_g4trkid_index[part_trackID[j]] = j;      
  }



  //===============================================================
  // Returns list of array index values corresponding to all the 
  // particle's ancestors in the list ("part_[index]"...)
  //===============================================================
  std::vector<int> FindAllAncestors(int index){
    std::vector<int> output;
    bool flag = true;
    while(flag){
      int mother = part_mother[index];
      if( !map_g4trkid_index.count(mother) ) return output;
      int mother_index = map_g4trkid_index[mother];
      output.push_back(mother_index);
      if( mother <= 0 ) { break;}
      index = mother_index;
    }
    return output;
  }





  //===============================================================
  // Returns a list of PDGs for the ancestors of this particle
  // in descending order
  //==============================================================
  std::vector<int> FindAllAncestorPDGs(int index){
    std::vector<int> output;
    std::vector<int> ancestors = FindAllAncestors(index);
    for(auto v : ancestors ) output.push_back(part_pdg[v]);
    return output;
  }



  //===============================================================
  // Returns a list of KEs for the ancestors of this particle
  // in descending order (Diego)
  //==============================================================
  std::vector<float> FindAllAncestorKEs(int index){
    std::vector<float> output;
    std::vector<int> ancestors = FindAllAncestors(index);
    for(auto v : ancestors ) output.push_back(part_KE[v]);
    return output;
  }








/*
  //===============================================================
  // Main Macro for testing out functions above
  //===============================================================
  void ancestor(){
  
    TFile* file = new TFile("files/BlipAna_20240124_BiPo_Overlay.root","READ");
    TTree* tree = (TTree*)file->Get("blipanaTrkMask/anatree");
    tree->SetBranchAddress("nparticles",&nparticles);
    tree->SetBranchAddress("part_trackID",&part_trackID);
    tree->SetBranchAddress("part_mother",&part_mother);
    tree->SetBranchAddress("part_pdg",&part_pdg);
    // loop over 10 EVENTS 
    for(int i=0; i<10; i++){
      
      tree->GetEntry(i);
      MakeTrackIDMap();
      
      std::cout<<"Event "<<i<<" has nparticles = "<<nparticles<<"\n";
      
      // loop over PARTICLES in this event
      for(int ip=0; ip<nparticles; ip++){
        std::cout<<"    "<<ip<<"  TrackID "<<part_trackID[ip]<<"   PDG "<<part_pdg[ip]<<"     mother TrkID "<<part_mother[ip]<<"\n";
        
        // get a list of all the PDGs leading up to this
        // particle, in *descending* order
        std::vector<int> ancestorPDGs = FindAllAncestorPDGs(ip);
        std::cout<<"      --> Found "<<ancestorPDGs.size()<<" ancestors in lineage: ";
        for(auto pdg : ancestorPDGs ) { 
          std::cout<<pdg<<"  ";
        }
        std::cout<<"\n";
        
      }//end particle loop

    }//end event loop

}
*/

