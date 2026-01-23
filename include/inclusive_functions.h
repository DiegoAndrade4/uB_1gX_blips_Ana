bool LEEana::is_0p(TaggerInfo& tagger_info, KineInfo& kine, PFevalInfo& pfeval){
  bool flag = false;

  if (tagger_info.numu_cc_flag>=0){
      // 1 lepton <=1 proton 0 charged pion
      // 1 lepton guaranteed by numu cc flag
      // using pi0 flag to remove pi0 component in channel definition
      int Nproton = 0;
      int Npion = 0;
      for(size_t i=0; i<kine.kine_energy_particle->size(); i++)
      {
          int pdgcode = kine.kine_particle_type->at(i);
          if(abs(pdgcode)==2212 && kine.kine_energy_particle->at(i)>35) Nproton++; // KE threshold: 50 MeV, 1.5 cm?
          //if(abs(pdgcode)==2212 && kine.kine_energy_particle->at(i)>0) Nproton++; //Erin: CHANGE, "actual 0p" aka no 35 MeV threshold
          if(abs(pdgcode)==211 && kine.kine_energy_particle->at(i)>10) Npion++; // KE threshold: 10 MeV
      }
      if(Nproton==0) flag = true;
  }

  return flag;
} 



//=============================================================================

int get_Nproton(TaggerInfo& tagger_info, KineInfo& kine, PFevalInfo& pfeval) {
    int Nproton = 0;

    if (tagger_info.numu_cc_flag >= 0) {
        for (size_t i = 0; i < kine.kine_energy_particle->size(); i++) {
            int pdgcode = kine.kine_particle_type->at(i);
            // Count protons above 35 MeV kinetic energy
            if (abs(pdgcode) == 2212 && kine.kine_energy_particle->at(i) > 35)
                Nproton++;
        }
    }

    return Nproton;
}


//=============================================================================



