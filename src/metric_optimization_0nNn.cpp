// g++ metric_optimization_0nNn.cpp -o metric_optimization_0nNn $(root-config --cflags --libs)
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <vector>
#include "set_vars.h"
#include "set_vars_pfeval.h"
#include "set_vars_kine.h"
#include "set_vars_taggerbdt.h"
#include "common_funtions.h"
#include "set_vars_eval.h"


#include <string>
#include <sys/stat.h>
#include "TSystem.h"
#include <iomanip>   // <-- Needed for std::setw
#include "TH1.h"


// ====================================
// Event Selection from txt file
// ====================================

// Structure to hold event identifiers
struct EventID {
    int run;
    int subrun;
    int event;
};

// Function to load event IDs from file
std::vector<EventID> LoadEventList(const char* filename) {
    std::vector<EventID> filteredEvents;
    std::ifstream infile(filename);
    
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open event list file " << filename << std::endl;
        return filteredEvents;
    }
    
    std::string line;
    while (std::getline(infile, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        EventID id;
        
        if (iss >> id.run >> id.subrun >> id.event) {
            filteredEvents.push_back(id);
        }
    }
    
    infile.close();
    std::cout << "Loaded " << filteredEvents.size() << " events from selection list" << std::endl;
    return filteredEvents;
}

// Function to check if current event is in the list
bool IsEventSelected(int run, int subrun, int event, const std::vector<EventID>& filteredEvents) {
    for (const auto& id : filteredEvents) {
        if (id.run == run && id.subrun == subrun && id.event == event) {
            return true;
        }
    }
    return false;
}



//===============================


struct MeanRatioResult {
    double ratio;
    double ratio_err;
};

MeanRatioResult MeanRatio(TH1* h1, TH1* h2)
{
    MeanRatioResult R{0,0};

    if (!h1 || !h2) return R;

    double m1 = h1->GetMean();
    double e1 = h1->GetMeanError();
    double m2 = h2->GetMean();
    double e2 = h2->GetMeanError();

    if (m2 == 0) return R;

    R.ratio = m1 / m2;
    R.ratio_err = R.ratio * std::sqrt( (e1*e1)/(m1*m1) + (e2*e2)/(m2*m2) );

    return R;
}



// Helper function to extract file name without path or extension
std::string GetBaseName(const std::string& filepath) {
    size_t slash = filepath.find_last_of("/\\");
    std::string filename = (slash == std::string::npos) ? filepath : filepath.substr(slash + 1);

    size_t dot = filename.find_last_of(".");
    return (dot == std::string::npos) ? filename : filename.substr(0, dot);
}

bool ParseBool(const std::string& s) {
    if (s == "1" || s == "true" || s == "True" || s == "TRUE")
        return true;
    if (s == "0" || s == "false" || s == "False" || s == "FALSE")
        return false;

    std::cerr << "WARNING: Unrecognized boolean value '" << s
              << "'. Defaulting to false.\n";
    return false;
}



int main(int argc, char** argv) {

    if (argc < 9) {
        std::cerr << "Usage:\n"
                  << argv[0]
                  << " <inputFile.root> <label> <bool1> <bool2> <outputDir>\n";
        std::cerr << "Example:\n"
                  << argv[0]
                  << " data.root MyTag 1 0 Results\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string label     = argv[2];
    bool IsData           = ParseBool(argv[3]);
    bool AddBacktrackedBlips = ParseBool(argv[4]);
    std::string outDir    = argv[5]; 


    float SphRadius = std::atof(argv[6]);
    float NBlips_Cut = std::atof(argv[7]);
    float SumE_Cut = std::atof(argv[8]);

    // Create output directory if missing
    if (gSystem->AccessPathName(outDir.c_str())) {
        std::cout << "Directory does not exist. Creating: " << outDir << std::endl;
        gSystem->mkdir(outDir.c_str(), true);
    } else {
        std::cout << "Directory exists: " << outDir << std::endl;
    }

    // Base filename (input file name without extension)
    std::string baseName  = GetBaseName(inputFile);

    // Start forming output filename
    std::string outputName = baseName + "_" + label; 
    outputName += Form("_R%.3f_NBlips%.3f_SumE%.3f", SphRadius, NBlips_Cut, SumE_Cut);

    // Append flags if true (customize text below if desired)
    if (IsData) outputName += "_Data";
    if (AddBacktrackedBlips) outputName += "_BacktrkBlips";

    // Add ROOT extension and path
    std::string outputFile = outDir + "/" + outputName + ".root";

    std::cout << " ---- SETTINGS ----\n";
    std::cout << " Input ROOT File : " << inputFile << "\n";
    std::cout << " Signal/Sideband : " << label << "\n";
    std::cout << " IsData          : " << (IsData ? "true" : "false") << "\n";
    std::cout << " AddBackTrkBlips : " << (AddBacktrackedBlips ? "true" : "false") << "\n";
    std::cout << " Output File     : " << outputFile << "\n";

    std::cout << " SphRadius       : " << SphRadius << "\n";
    std::cout << " NBlips_Cut      : " << NBlips_Cut << "\n";
    std::cout << " SumE_Cut        : " << SumE_Cut << "\n";

    std::cout << " ------------------\n";

    // Open input file
    TFile* fInputFile = TFile::Open(inputFile.c_str());
    if (!fInputFile || fInputFile->IsZombie()) {
        std::cerr << "ERROR: Could not open input ROOT file!\n";
        return 1;
    }


bool SaveOutputFile = false ; 
    // Create output ROOT file
    TFile* fOutFile = new TFile(outputFile.c_str(), "RECREATE");




    // --- Get TTree ---
    TTree* fTree = (TTree*)fInputFile->Get("nuselection/NeutrinoSelectionFilter"); // PeLEE TTree
    TTree *T_PFeval   = (TTree*)fInputFile->Get("wcpselection/T_PFeval");
    TTree *T_BDTvars  = (TTree*)fInputFile->Get("wcpselection/T_BDTvars");
    TTree *T_eval     = (TTree*)fInputFile->Get("wcpselection/T_eval");
    TTree *T_pot      = (TTree*)fInputFile->Get("wcpselection/T_pot");
    TTree *T_KINEvars = (TTree*)fInputFile->Get("wcpselection/T_KINEvars");


    if (!T_PFeval || !T_eval || !T_BDTvars || !T_KINEvars || !T_pot) {
            std::cerr << "Error: one or more trees not found!" << std::endl;
        fInputFile->Close();
        return 1;
    }

    if (!fTree) {
        std::cerr << "Error: cannot find tree 'nuselection/NeutrinoSelectionFilter'" << std::endl;
        fInputFile->Close();
        return 1;
    }


    // Set the branches you plan on using; this function is in the "set_vars.h"
    // file sourced at the top, along with all the variables.
    setBranches(fTree); // blip info
    setBranchesPFEval(T_PFeval);
    setBranchesKINE(T_KINEvars);
    setBranchesBDT(T_BDTvars);
    setBranchesEval(T_eval);
/*
float SphRadius = 100; 
float NBlips_Cut = 10;
float SumE_Cut = 8;
*/

// --  ---
TH1F *h_1gX_BlipMultiplicity_all = new TH1F("h_1gX_BlipMultiplicity_all", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_all         = new TH1F("h_1gX_SumEblip_all", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_all = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_all", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);


TH1F *h_1gX_BlipMultiplicity_all_0n = new TH1F("h_1gX_BlipMultiplicity_all_0n", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_all_0n         = new TH1F("h_1gX_SumEblip_all_0n", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_all_0n = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_all_0n", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);

TH1F *h_1gX_BlipMultiplicity_mc_0n = new TH1F("h_1gX_BlipMultiplicity_mc_0n", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_mc_0n         = new TH1F("h_1gX_SumEblip_mc_0n", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_mc_0n = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_mc_0n", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);

TH1F *h_1gX_BlipMultiplicity_over_0n = new TH1F("h_1gX_BlipMultiplicity_over_0n", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_over_0n         = new TH1F("h_1gX_SumEblip_over_0n", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_over_0n = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_over_0n", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);


TH1F *h_1gX_BlipMultiplicity_all_Nn = new TH1F("h_1gX_BlipMultiplicity_all_Nn", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_all_Nn         = new TH1F("h_1gX_SumEblip_all_Nn", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_all_Nn = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_all_Nn", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);

TH1F *h_1gX_BlipMultiplicity_mc_Nn = new TH1F("h_1gX_BlipMultiplicity_mc_Nn", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_mc_Nn         = new TH1F("h_1gX_SumEblip_mc_Nn", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_mc_Nn = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_mc_Nn", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);

TH1F *h_1gX_BlipMultiplicity_over_Nn = new TH1F("h_1gX_BlipMultiplicity_over_Nn", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F *h_1gX_SumEblip_over_Nn         = new TH1F("h_1gX_SumEblip_over_Nn", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; counts", 100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_over_Nn = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_over_Nn", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip [MeV_{ee}]", 100, 0, 100,  100, 0, 100);




//std::vector<EventID> filteredEvents = LoadEventList("SignalEvents_Run4b_BNBNuOverlay_RSE.txt");



int signal_events = 0 ;
int Events_Nn_all = 0 ; 
int Events_0n_all = 0 ; 

    int nevents = IsData ?  fTree->GetEntries() * 0.5   :  fTree->GetEntries() ;
    std::cout << "fTree->GetEntries() " << fTree->GetEntries() << std::endl;
    std::cout << "Total events: " << nevents << std::endl;

    for (int iEvent = 0; iEvent < nevents; ++iEvent) {
                fTree->GetEntry(iEvent);
                    T_PFeval->GetEntry(iEvent);
                    T_KINEvars->GetEntry(iEvent);
                T_BDTvars->GetEntry(iEvent);
                T_eval->GetEntry(iEvent);

/*
	//Filtered events
	// Get current event identifiers from your tree
    	int currentRun = run;      // Your branch variable
    	int currentSubrun = subrun; // Your branch variable  
    	int currentEvent = event;   // Your branch variable
    
    	// Check if this event is in the filtered list
    	if (!IsEventSelected(currentRun, currentSubrun, currentEvent, filteredEvents)) {
        	continue;  // Skip this event
    	}

        std::cout << "Processing filtered event: Run=" << currentRun  << " Subrun=" << currentSubrun << " Event=" << currentEvent << std::endl;

*/
                TVector3 NuVtx(reco_nu_vtx_x, reco_nu_vtx_y, reco_nu_vtx_z); // NuVtx
                TVector3 ShVtx(reco_showervtxX, reco_showervtxY, reco_showervtxZ); // ShVtx
                TVector3 ShowerMomentum(reco_showerMomentum[0], reco_showerMomentum[1], reco_showerMomentum[2]); // ShowerDir


 int BacktrackedBlips = 0 ; 
int NBlips_sig_all_blips = 0, NBlips_sig_mc_blips = 0 , NBlips_sig_over_blips = 0 ; 
int   SumE_sig_all_blips = 0,   SumE_sig_mc_blips = 0 ,   SumE_sig_over_blips = 0 ; 

                                     //********************************
                                     //  1gX- selection + generic neutrino selection
                                     //********************************
                                        if (
                                    crtveto == 0 &&
                                    kine_reco_Enu > 0   &&  /* LEEana::is_singlephoton_sel(tagger, pfeval)*/

                                    shw_sp_n_20mev_showers > 0 &&
                                    reco_nuvtxX>5.0 && reco_nuvtxX<250.0 &&
                                    single_photon_numu_score > 0.4 &&
                                    single_photon_other_score > 0.2 &&
                                    single_photon_ncpi0_score > -0.05 &&
                                    single_photon_nue_score > -1.0 &&
                                    shw_sp_n_20br1_showers==1

                                     ){//Signal Events

                                                signal_events++ ; 



				     //-- Blip section --

                                 
                                     // --- Blip loop ---
                                     for (size_t iBlip = 0; iBlip < nblips_saved ; ++iBlip) {

                                        TVector3 BlipVtx(blip_x->at(iBlip), blip_y->at(iBlip) , blip_z->at(iBlip));
                                        TVector3 ShowerVtx2Blip = BlipVtx - ShVtx; // Vector
                                                float blip_dist2vtx = (BlipVtx - ShVtx).Mag(); // Blip dist w.r.t ShVtx


                                        if (blip_nplanes->at(iBlip) > 1 &&          // 2&3 matched-planes (3D-blips)
                                            blip_touchtrk->at(iBlip) == 0 &&        // No blips touching tracks
                                            blip_pl2_bydeadwire->at(iBlip) == 0 &&  // No blips by dead wires in collection plane (pl2)
                                            blip_proxtrkdist->at(iBlip) > 15 &&     // Distance to closest track > 15 [cm]
                                            blip_dist2vtx < SphRadius  &&                 // Blip distance from ShVtx
                                            IsWithinSphereOutsideConic(ShVtx, ShowerMomentum, BlipVtx, SphRadius)  //Qsphere
					     
                                                    ){

                                         double blip_angle_sh = calculateAngleBetweenVectors(ShowerVtx2Blip, ShowerMomentum);
                                         double cos_blip_angle_sh = calculateCosineAngleBetweenVectors(ShowerVtx2Blip, ShowerMomentum);

						if( ! IsBackTrackedBlip( blip_dist2vtx , cos_blip_angle_sh ) ) { // IsotropicBlips		

						NBlips_sig_all_blips++ ;
				        	SumE_sig_all_blips += blip_energy->at(iBlip);	 
			
                                        	//Local MC-Overlay split
                                         	//-- MC
                                         	if (blip_true_g4id->at(iBlip) > 0) { 

					 	NBlips_sig_mc_blips++ ;
					 	SumE_sig_mc_blips += blip_energy->at(iBlip);

						}else{ //Overlay blips
					 
					 	NBlips_sig_over_blips++ ;
					 	SumE_sig_over_blips += blip_energy->at(iBlip);

                                         	}

                                                } //EndIsotropicBlips
						else { BacktrackedBlips++ ; 
                                                      //std::cout <<"signal_event # "<<signal_events<<" , with "<<BacktrackedBlips<<" BackTrackedBlips"<<std::endl; 
						     }
				            }//End signal blip
					}//End blip loop
					
				
				

				     //----------------------
                                    // --- 0n/Nn split ---
                                    //----------------------
                                    h_1gX_BlipMultiplicity_all->Fill(NBlips_sig_all_blips);
                                    h_1gX_SumEblip_all->Fill(SumE_sig_all_blips);
                                    h2D_1gX_BlipMultiplicity_SumEblip_all->Fill(NBlips_sig_all_blips, SumE_sig_all_blips);
				

				     if(NBlips_sig_all_blips <= NBlips_Cut &&  SumE_sig_all_blips <= SumE_Cut ){//0n
					//All 
					Events_0n_all++; 
					h_1gX_BlipMultiplicity_all_0n->Fill(NBlips_sig_all_blips);
                                    	h_1gX_SumEblip_all_0n->Fill(SumE_sig_all_blips);
                                    	h2D_1gX_BlipMultiplicity_SumEblip_all_0n->Fill(NBlips_sig_all_blips, SumE_sig_all_blips);
				        // MC
					h_1gX_BlipMultiplicity_mc_0n->Fill(NBlips_sig_mc_blips);
                                        h_1gX_SumEblip_mc_0n->Fill(SumE_sig_mc_blips);
                                        h2D_1gX_BlipMultiplicity_SumEblip_mc_0n->Fill(NBlips_sig_mc_blips, SumE_sig_mc_blips);
					//Over
					h_1gX_BlipMultiplicity_over_0n->Fill(NBlips_sig_over_blips);
                                        h_1gX_SumEblip_over_0n->Fill(SumE_sig_over_blips);
                                        h2D_1gX_BlipMultiplicity_SumEblip_over_0n->Fill(NBlips_sig_over_blips, SumE_sig_over_blips);
				      }else{//Nn
					 //All
					Events_Nn_all++;
                                        h_1gX_BlipMultiplicity_all_Nn->Fill(NBlips_sig_all_blips);
                                        h_1gX_SumEblip_all_Nn->Fill(SumE_sig_all_blips);
                                        h2D_1gX_BlipMultiplicity_SumEblip_all_Nn->Fill(NBlips_sig_all_blips, SumE_sig_all_blips);
                                        // MC
                                        h_1gX_BlipMultiplicity_mc_Nn->Fill(NBlips_sig_mc_blips);
                                        h_1gX_SumEblip_mc_Nn->Fill(SumE_sig_mc_blips);
                                        h2D_1gX_BlipMultiplicity_SumEblip_mc_Nn->Fill(NBlips_sig_mc_blips, SumE_sig_mc_blips);
                                        //Over
                                        h_1gX_BlipMultiplicity_over_Nn->Fill(NBlips_sig_over_blips);
                                        h_1gX_SumEblip_over_Nn->Fill(SumE_sig_over_blips);
                                        h2D_1gX_BlipMultiplicity_SumEblip_over_Nn->Fill(NBlips_sig_over_blips, SumE_sig_over_blips); 
					}
		
// if (BacktrackedBlips=! 0 ) std::cout <<"signal_event # "<<signal_events<<" , with "<<BacktrackedBlips<<" BackTrackedBlips"<<std::endl;  
 BacktrackedBlips = 0 ;
 NBlips_sig_all_blips = 0;  NBlips_sig_mc_blips = 0 ; NBlips_sig_over_blips = 0 ;
   SumE_sig_all_blips = 0;    SumE_sig_mc_blips = 0 ;   SumE_sig_over_blips = 0 ;

 }//End Signal events

}//End event loop


 std::cout <<"Total signal_event "<<signal_events<<std::endl;

    // Open output file in APPEND mode
    //std::ofstream outfile("Metric_Optimization_0nNn_FILES/REN_pars_Nn0n_opt.txt", std::ios::app);
    std::ofstream outfile(outDir + "/REN_pars_Nn0n_opt_5.txt", std::ios::app);

    if (!outfile.is_open()) std::cerr << "Error REN_pars_Nn0n_opt_5.txt for writing!\n";


   outfile << std::setw(12) << signal_events
            << std::setw(12) << SphRadius
            << std::setw(12) << NBlips_Cut
            << std::setw(12) << SumE_Cut  
	    << std::setw(12) << Events_Nn_all
	    << std::setw(12) << Events_0n_all
	    << std::endl;






     outfile.close();


//     std::cout << "Histograms saved to " << outputFile << std::endl;
if (SaveOutputFile){
    fOutFile->Write(); }
    fOutFile->Close();
    fInputFile->Close();
				

} //main end
