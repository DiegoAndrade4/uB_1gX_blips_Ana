// g++ anamacro_1gX_blips_signal.cpp -o anamacro_1gX_blips_signal $(root-config --cflags --libs)

#include <fstream> 
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <vector>
#include "../include/set_vars.h"
#include "../include/set_vars_pfeval.h"
#include "../include/set_vars_kine.h"
#include "../include/set_vars_taggerbdt.h"
#include "../include/common_funtions.h"
#include "../include/set_vars_eval.h"
#include "../include/pfeval.h"

#include <string>
#include <sys/stat.h>
#include "TSystem.h"


//using namespace std;


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
    float Radius          = std::atof(argv[6]);
    int NBlips_Cut        = std::atoi(argv[7]);
    float SumE_Cut        = std::atof(argv[8]);

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
    std::cout << " Radius          : " << Radius << "\n";
    std::cout << " NBlips_Cut      : " << NBlips_Cut << "\n";
    std::cout << " SumE_Cut        : " << SumE_Cut << "\n";
    std::cout << " ------------------\n";

    // Open input file
    TFile* fInputFile = TFile::Open(inputFile.c_str());
    if (!fInputFile || fInputFile->IsZombie()) {
        std::cerr << "ERROR: Could not open input ROOT file!\n";
        return 1;
    }

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





    

    // --- Define histograms ---

   TH1F *h_1gX_Nprotons = new TH1F("h_1gX_Nprotons", "Inclusive single shower selection; Number of Protons; Events", 10, 0, 10);
   TH1F *h_1gX_Nprotons_0n = new TH1F("h_1gX_Nprotons_0n", "Inclusive single shower selection - 0n; Number of Protons; Events", 10, 0, 10);
   TH1F *h_1gX_Nprotons_Nn = new TH1F("h_1gX_Nprotons_Nn", "Inclusive single shower selection - Nn; Number of Protons; Events", 10, 0, 10);

    //Signal events histograms
    TH1F *h_SPNCDeltaSig_Nprotons = new TH1F("h_SPNCDeltaSig_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCPi0Sig_Nprotons = new TH1F("h_SPNCPi0Sig_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCOtherSig_Nprotons = new TH1F("h_SPNCOtherSig_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSig_Nprotons = new TH1F("h_SPNumuCCSig_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPOutFVSig_Nprotons = new TH1F("h_SPOutFVSig_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSigGen_Nprotons = new TH1F("h_SPNumuCCSigGen_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);


    //Background events histograms 
    TH1F *h_SPoutFVBkg_Nprotons = new TH1F("h_SPoutFVBkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnumuCCBkg_Nprotons = new TH1F("h_SPnumuCCBkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnumuCCpi0Bkg_Nprotons = new TH1F("h_SPnumuCCpi0Bkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnueCCBkg_Nprotons = new TH1F("h_SPnueCCBkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCBkg_Nprotons = new TH1F("h_SPNCBkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCpi0Bkg_Nprotons = new TH1F("h_SPNCpi0Bkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_CosmicBkg_Nprotons = new TH1F("h_CosmicBkg_Nprotons", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);


   //0n/Nn split
    //Signal events histograms
    TH1F *h_SPNCDeltaSig_Nprotons_0n = new TH1F("h_SPNCDeltaSig_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCPi0Sig_Nprotons_0n = new TH1F("h_SPNCPi0Sig_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCOtherSig_Nprotons_0n = new TH1F("h_SPNCOtherSig_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSig_Nprotons_0n = new TH1F("h_SPNumuCCSig_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPOutFVSig_Nprotons_0n = new TH1F("h_SPOutFVSig_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSigGen_Nprotons_0n = new TH1F("h_SPNumuCCSigGen_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);


    //Background events histograms
    TH1F *h_SPoutFVBkg_Nprotons_0n = new TH1F("h_SPoutFVBkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnumuCCBkg_Nprotons_0n = new TH1F("h_SPnumuCCBkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnumuCCpi0Bkg_Nprotons_0n = new TH1F("h_SPnumuCCpi0Bkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnueCCBkg_Nprotons_0n = new TH1F("h_SPnueCCBkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCBkg_Nprotons_0n = new TH1F("h_SPNCBkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCpi0Bkg_Nprotons_0n = new TH1F("h_SPNCpi0Bkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_CosmicBkg_Nprotons_0n = new TH1F("h_CosmicBkg_Nprotons_0n", "Inclusive 1g signal events - 0n;Number of Protons; Event counts", 10, 0, 10);




//Nn
    //Signal events histograms
    TH1F *h_SPNCDeltaSig_Nprotons_Nn = new TH1F("h_SPNCDeltaSig_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCPi0Sig_Nprotons_Nn = new TH1F("h_SPNCPi0Sig_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCOtherSig_Nprotons_Nn = new TH1F("h_SPNCOtherSig_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSig_Nprotons_Nn = new TH1F("h_SPNumuCCSig_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPOutFVSig_Nprotons_Nn = new TH1F("h_SPOutFVSig_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSigGen_Nprotons_Nn = new TH1F("h_SPNumuCCSigGen_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);


    //Background events histograms
    TH1F *h_SPoutFVBkg_Nprotons_Nn = new TH1F("h_SPoutFVBkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnumuCCBkg_Nprotons_Nn = new TH1F("h_SPnumuCCBkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnumuCCpi0Bkg_Nprotons_Nn = new TH1F("h_SPnumuCCpi0Bkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPnueCCBkg_Nprotons_Nn = new TH1F("h_SPnueCCBkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCBkg_Nprotons_Nn = new TH1F("h_SPNCBkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCpi0Bkg_Nprotons_Nn = new TH1F("h_SPNCpi0Bkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_CosmicBkg_Nprotons_Nn = new TH1F("h_CosmicBkg_Nprotons_Nn", "Inclusive 1g signal events - Nn;Number of Protons; Event counts", 10, 0, 10);







  //Signal true0p/Np split
    TH1F *h_SPNCDeltaSig_Nprotons_0p = new TH1F("h_SPNCDeltaSig_Nprotons_0p", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCPi0Sig_Nprotons_0p = new TH1F("h_SPNCPi0Sig_Nprotons_0p", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCOtherSig_Nprotons_0p = new TH1F("h_SPNCOtherSig_Nprotons_0p", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSig_Nprotons_0p = new TH1F("h_SPNumuCCSig_Nprotons_0p", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPOutFVSig_Nprotons_0p = new TH1F("h_SPOutFVSig_Nprotons_0p", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSigGen_Nprotons_0p = new TH1F("h_SPNumuCCSigGen_Nprotons_0p", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);

    TH1F *h_SPNCDeltaSig_Nprotons_Np = new TH1F("h_SPNCDeltaSig_Nprotons_Np", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCPi0Sig_Nprotons_Np = new TH1F("h_SPNCPi0Sig_Nprotons_Np", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNCOtherSig_Nprotons_Np = new TH1F("h_SPNCOtherSig_Nprotons_Np", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSig_Nprotons_Np = new TH1F("h_SPNumuCCSig_Nprotons_Np", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPOutFVSig_Nprotons_Np = new TH1F("h_SPOutFVSig_Nprotons_Np", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);
    TH1F *h_SPNumuCCSigGen_Nprotons_Np = new TH1F("h_SPNumuCCSigGen_Nprotons_Np", "Inclusive 1g signal events;Number of Protons; Event counts", 10, 0, 10);



// -- 0p/Np ---
TH1F *h_1gX_BlipMultiplicity = new TH1F("h_1gX_BlipMultiplicity", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F* h_1gX_SumEblip         = new TH1F("h_1gX_SumEblip", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; Counts", 100, 0, 100);
//0p
TH1F *h_1gX_BlipMultiplicity_0p = new TH1F("h_1gX_BlipMultiplicity_0p", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F* h_1gX_SumEblip_0p         = new TH1F("h_1gX_SumEblip_0p", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; Counts", 100, 0, 100);
//Np
TH1F *h_1gX_BlipMultiplicity_Np = new TH1F("h_1gX_BlipMultiplicity_Np", "Inclusive 1g signal events; Blip multiplicity ; counts", 100, 0, 100);
TH1F* h_1gX_SumEblip_Np         = new TH1F("h_1gX_SumEblip_Np", "Inclusive 1g signal events; Summed blip energy [MeV_{ee}]; Counts", 100, 0, 100);

//2D BlipMultiplicity vs SumEblip
TH2F *h2D_1gX_BlipMultiplicity_SumEblip = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_0p = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_0p", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_Np = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_Np", "Inclusive 1g signal events; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);

TH2F *h2D_1gX_BlipMultiplicity_SumEblip_0n = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_0n", "Inclusive 1g signal events - 0n; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_Nn = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_Nn", "Inclusive 1g signal events - Nn; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);

TH2F *h2D_1gX_BlipMultiplicity_SumEblip_regA = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_regA", "Inclusive 1g signal events - regA; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);
TH2F *h2D_1gX_BlipMultiplicity_SumEblip_regB = new TH2F("h2D_1gX_BlipMultiplicity_SumEblip_regB", "Inclusive 1g signal events - regB; Blip multiplicity ; Sum E blip", 100, 0, 100,  100, 0, 100);

// ---  All sphere --- 
TH1F* h_Blip_sphere_all_energy    = new TH1F("h_Blip_sphere_all_energy",   "All sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx  = new TH1F("h_Blip_sphere_all_dist2vtx", "All sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle     = new TH1F("h_Blip_sphere_all_angle",    "All sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle  = new TH1F("h_Blip_sphere_all_cosangle", "All sphere blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC sphere 
TH1F* h_Blip_sphere_all_energy_mc    = new TH1F("h_Blip_sphere_all_energy_mc",   "MC sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx_mc  = new TH1F("h_Blip_sphere_all_dist2vtx_mc", "MC sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle_mc     = new TH1F("h_Blip_sphere_all_angle_mc",    "MC sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle_mc  = new TH1F("h_Blip_sphere_all_cosangle_mc", "MC sphere blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC sphere protons
TH1F* h_Blip_sphere_all_energy_mc_p    = new TH1F("h_Blip_sphere_all_energy_mc_p",   " MC-p sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx_mc_p  = new TH1F("h_Blip_sphere_all_dist2vtx_mc_p", " MC-p sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle_mc_p     = new TH1F("h_Blip_sphere_all_angle_mc_p",    " MC-p sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle_mc_p  = new TH1F("h_Blip_sphere_all_cosangle_mc_p", " MC-p sphere blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC sphere e+/e-
TH1F* h_Blip_sphere_all_energy_mc_e    = new TH1F("h_Blip_sphere_all_energy_mc_e",   " MC-e sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx_mc_e  = new TH1F("h_Blip_sphere_all_dist2vtx_mc_e", " MC-e sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle_mc_e     = new TH1F("h_Blip_sphere_all_angle_mc_e",    " MC-e sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle_mc_e  = new TH1F("h_Blip_sphere_all_cosangle_mc_e", " MC-e sphere blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC sphere heavy nuclei
TH1F* h_Blip_sphere_all_energy_mc_HN    = new TH1F("h_Blip_sphere_all_energy_mc_HN",   " MC-HN sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx_mc_HN  = new TH1F("h_Blip_sphere_all_dist2vtx_mc_HN", " MC-HN sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle_mc_HN     = new TH1F("h_Blip_sphere_all_angle_mc_HN",    " MC-HN sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle_mc_HN  = new TH1F("h_Blip_sphere_all_cosangle_mc_HN", " MC-HN sphere blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC sphere other
TH1F* h_Blip_sphere_all_energy_mc_other    = new TH1F("h_Blip_sphere_all_energy_mc_other",   " MC-other sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx_mc_other  = new TH1F("h_Blip_sphere_all_dist2vtx_mc_other", " MC-other sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle_mc_other     = new TH1F("h_Blip_sphere_all_angle_mc_other",    " MC-other sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle_mc_other  = new TH1F("h_Blip_sphere_all_cosangle_mc_other", " MC-other sphere blips ; Cos(#alpha); Counts", 800, -1, 1);
// Overlay sphere
TH1F* h_Blip_sphere_all_energy_over    = new TH1F("h_Blip_sphere_all_energy_over",   "All sphere blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_sphere_all_dist2vtx_over  = new TH1F("h_Blip_sphere_all_dist2vtx_over", "All sphere blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_sphere_all_angle_over     = new TH1F("h_Blip_sphere_all_angle_over",    "All sphere blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_sphere_all_cosangle_over  = new TH1F("h_Blip_sphere_all_cosangle_over", "All sphere blips ; Cos(#alpha); Counts", 800, -1, 1);



// -- All Signal blips ---
TH1F* h_Blip_signal_all_energy    = new TH1F("h_Blip_signal_all_energy",   "All signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_all_dist2vtx  = new TH1F("h_Blip_signal_all_dist2vtx", "All signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_all_angle     = new TH1F("h_Blip_signal_all_angle",    "All signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_all_cosangle  = new TH1F("h_Blip_signal_all_cosangle", "All signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// All Reg A Signal blips
TH1F* h_Blip_signal_all_regA_energy    = new TH1F("h_Blip_signal_all_regA_energy",   "All regA signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_all_regA_dist2vtx  = new TH1F("h_Blip_signal_all_regA_dist2vtx", "All regA signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_all_regA_angle     = new TH1F("h_Blip_signal_all_regA_angle",    "All regA signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_all_regA_cosangle  = new TH1F("h_Blip_signal_all_regA_cosangle", "All regA signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// All Reg B Signal blips
TH1F* h_Blip_signal_all_regB_energy    = new TH1F("h_Blip_signal_all_regB_energy",   "All regB signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_all_regB_dist2vtx  = new TH1F("h_Blip_signal_all_regB_dist2vtx", "All regB signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_all_regB_angle     = new TH1F("h_Blip_signal_all_regB_angle",    "All regB signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_all_regB_cosangle  = new TH1F("h_Blip_signal_all_regB_cosangle", "All regB signal blips ; Cos(#alpha); Counts", 800, -1, 1);


// -- MC Signal blips ---
TH1F* h_Blip_signal_mc_energy    = new TH1F("h_Blip_signal_mc_energy",   "MC signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_dist2vtx  = new TH1F("h_Blip_signal_mc_dist2vtx", "MC signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_angle     = new TH1F("h_Blip_signal_mc_angle",    "MC signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_cosangle  = new TH1F("h_Blip_signal_mc_cosangle", "MC signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC signal p
TH1F* h_Blip_signal_mc_energy_p    = new TH1F("h_Blip_signal_mc_energy_p",   "MC-p signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_dist2vtx_p  = new TH1F("h_Blip_signal_mc_dist2vtx_p", "MC-p signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_angle_p     = new TH1F("h_Blip_signal_mc_angle_p",    "MC-p signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_cosangle_p  = new TH1F("h_Blip_signal_mc_cosangle_p", "MC-p signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC signal e+/e-
TH1F* h_Blip_signal_mc_energy_e    = new TH1F("h_Blip_signal_mc_energy_e",   " MC-e signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_dist2vtx_e  = new TH1F("h_Blip_signal_mc_dist2vtx_e", " MC-e signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_angle_e     = new TH1F("h_Blip_signal_mc_angle_e",    " MC-e signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_cosangle_e  = new TH1F("h_Blip_signal_mc_cosangle_e", " MC-e signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC signal heavy nuclei
TH1F* h_Blip_signal_mc_energy_HN    = new TH1F("h_Blip_signal_mc_energy_HN",   " MC-HN signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_dist2vtx_HN  = new TH1F("h_Blip_signal_mc_dist2vtx_HN", " MC-HN signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_angle_HN     = new TH1F("h_Blip_signal_mc_angle_HN",    " MC-HN signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_cosangle_HN  = new TH1F("h_Blip_signal_mc_cosangle_HN", " MC-HN signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC signal other
TH1F* h_Blip_signal_mc_energy_other    = new TH1F("h_Blip_signal_mc_energy_other",   " MC-other signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_dist2vtx_other  = new TH1F("h_Blip_signal_mc_dist2vtx_other", " MC-other signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_angle_other     = new TH1F("h_Blip_signal_mc_angle_other",    " MC-other signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_cosangle_other  = new TH1F("h_Blip_signal_mc_cosangle_other", " MC-other signal blips ; Cos(#alpha); Counts", 800, -1, 1);


// -- MC Region A Signal blips ---
TH1F* h_Blip_signal_mc_regA_energy    = new TH1F("h_Blip_signal_mc_regA_energy",   "MC regA signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regA_dist2vtx  = new TH1F("h_Blip_signal_mc_regA_dist2vtx", "MC regA signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regA_angle     = new TH1F("h_Blip_signal_mc_regA_angle",    "MC regA signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regA_cosangle  = new TH1F("h_Blip_signal_mc_regA_cosangle", "MC regA signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regA signal p
TH1F* h_Blip_signal_mc_regA_energy_p    = new TH1F("h_Blip_signal_mc_regA_energy_p",   "MC regA-p signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regA_dist2vtx_p  = new TH1F("h_Blip_signal_mc_regA_dist2vtx_p", "MC regA-p signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regA_angle_p     = new TH1F("h_Blip_signal_mc_regA_angle_p",    "MC regA-p signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regA_cosangle_p  = new TH1F("h_Blip_signal_mc_regA_cosangle_p", "MC regA-p signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regA signal e+/e-
TH1F* h_Blip_signal_mc_regA_energy_e    = new TH1F("h_Blip_signal_mc_regA_energy_e",   " MC regA-e signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regA_dist2vtx_e  = new TH1F("h_Blip_signal_mc_regA_dist2vtx_e", " MC regA-e signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regA_angle_e     = new TH1F("h_Blip_signal_mc_regA_angle_e",    " MC regA-e signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regA_cosangle_e  = new TH1F("h_Blip_signal_mc_regA_cosangle_e", " MC regA-e signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regA signal heavy nuclei
TH1F* h_Blip_signal_mc_regA_energy_HN    = new TH1F("h_Blip_signal_mc_regA_energy_HN",   " MC regA-HN signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regA_dist2vtx_HN  = new TH1F("h_Blip_signal_mc_regA_dist2vtx_HN", " MC regA-HN signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regA_angle_HN     = new TH1F("h_Blip_signal_mc_regA_angle_HN",    " MC regA-HN signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regA_cosangle_HN  = new TH1F("h_Blip_signal_mc_regA_cosangle_HN", " MC regA-HN signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regA signal other
TH1F* h_Blip_signal_mc_regA_energy_other    = new TH1F("h_Blip_signal_mc_regA_energy_other",   " MC regA-other signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regA_dist2vtx_other  = new TH1F("h_Blip_signal_mc_regA_dist2vtx_other", " MC regA-other signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regA_angle_other     = new TH1F("h_Blip_signal_mc_regA_angle_other",    " MC regA-other signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regA_cosangle_other  = new TH1F("h_Blip_signal_mc_regA_cosangle_other", " MC regA-other signal blips ; Cos(#alpha); Counts", 800, -1, 1);


// -- MC Region B Signal blips ---
TH1F* h_Blip_signal_mc_regB_energy    = new TH1F("h_Blip_signal_mc_regB_energy",   "MC regB signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regB_dist2vtx  = new TH1F("h_Blip_signal_mc_regB_dist2vtx", "MC regB signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regB_angle     = new TH1F("h_Blip_signal_mc_regB_angle",    "MC regB signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regB_cosangle  = new TH1F("h_Blip_signal_mc_regB_cosangle", "MC regB signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regB signal p
TH1F* h_Blip_signal_mc_regB_energy_p    = new TH1F("h_Blip_signal_mc_regB_energy_p",   "MC regB-p signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regB_dist2vtx_p  = new TH1F("h_Blip_signal_mc_regB_dist2vtx_p", "MC regB-p signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regB_angle_p     = new TH1F("h_Blip_signal_mc_regB_angle_p",    "MC regB-p signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regB_cosangle_p  = new TH1F("h_Blip_signal_mc_regB_cosangle_p", "MC regB-p signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regB signal e+/e-
TH1F* h_Blip_signal_mc_regB_energy_e    = new TH1F("h_Blip_signal_mc_regB_energy_e",   " MC regB-e signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regB_dist2vtx_e  = new TH1F("h_Blip_signal_mc_regB_dist2vtx_e", " MC regB-e signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regB_angle_e     = new TH1F("h_Blip_signal_mc_regB_angle_e",    " MC regB-e signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regB_cosangle_e  = new TH1F("h_Blip_signal_mc_regB_cosangle_e", " MC regB-e signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regB signal heavy nuclei
TH1F* h_Blip_signal_mc_regB_energy_HN    = new TH1F("h_Blip_signal_mc_regB_energy_HN",   " MC regB-HN signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regB_dist2vtx_HN  = new TH1F("h_Blip_signal_mc_regB_dist2vtx_HN", " MC regB-HN signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regB_angle_HN     = new TH1F("h_Blip_signal_mc_regB_angle_HN",    " MC regB-HN signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regB_cosangle_HN  = new TH1F("h_Blip_signal_mc_regB_cosangle_HN", " MC regB-HN signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// MC regB signal other
TH1F* h_Blip_signal_mc_regB_energy_other    = new TH1F("h_Blip_signal_mc_regB_energy_other",   " MC regB-other signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_mc_regB_dist2vtx_other  = new TH1F("h_Blip_signal_mc_regB_dist2vtx_other", " MC regB-other signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_mc_regB_angle_other     = new TH1F("h_Blip_signal_mc_regB_angle_other",    " MC regB-other signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_mc_regB_cosangle_other  = new TH1F("h_Blip_signal_mc_regB_cosangle_other", " MC regB-other signal blips ; Cos(#alpha); Counts", 800, -1, 1);



// -- Overlay Signal blips ---
TH1F* h_Blip_signal_over_energy    = new TH1F("h_Blip_signal_over_energy",   "Overlay signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_over_dist2vtx  = new TH1F("h_Blip_signal_over_dist2vtx", "Overlay signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_over_angle     = new TH1F("h_Blip_signal_over_angle",    "Overlay signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_over_cosangle  = new TH1F("h_Blip_signal_over_cosangle", "Overlay signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// Overlay Reg A Signal blips
TH1F* h_Blip_signal_over_regA_energy    = new TH1F("h_Blip_signal_over_regA_energy",   "Overlay regA signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_over_regA_dist2vtx  = new TH1F("h_Blip_signal_over_regA_dist2vtx", "Overlay regA signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_over_regA_angle     = new TH1F("h_Blip_signal_over_regA_angle",    "Overlay regA signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_over_regA_cosangle  = new TH1F("h_Blip_signal_over_regA_cosangle", "Overlay regA signal blips ; Cos(#alpha); Counts", 800, -1, 1);
// Overlay Reg B Signal blips
TH1F* h_Blip_signal_over_regB_energy    = new TH1F("h_Blip_signal_over_regB_energy",   "Overlay regB signal blips ; Blip energy [MeV_{ee}]; Counts", 100, 0, 50);
TH1F* h_Blip_signal_over_regB_dist2vtx  = new TH1F("h_Blip_signal_over_regB_dist2vtx", "Overlay regB signal blips ; Blip distance to shower vertex [cm]; Counts", 160, 0, 800);
TH1F* h_Blip_signal_over_regB_angle     = new TH1F("h_Blip_signal_over_regB_angle",    "Overlay regB signal blips ; #alpha [degrees]; Counts", 36, 0, 180);
TH1F* h_Blip_signal_over_regB_cosangle  = new TH1F("h_Blip_signal_over_regB_cosangle", "Overlay regB signal blips ; Cos(#alpha); Counts", 800, -1, 1);







    // --- Event loop ---   
   int WC_0p_wBB = 0,  WC_Np_wBB = 0; 
    int signal_events = 0;
    int NoSigNorBkg = 0; int NoSigNorBkg0n = 0; int NoSigNorBkgNn = 0; 
    int dummycounter = 0;  
    int Events_Nn_all = 0 ;
    int Events_0n_all = 0 ; 
   // int nevents = fTree->GetEntries(); 

    int nevents = IsData ?  fTree->GetEntries() * 0.5   :  fTree->GetEntries() ;
    std::cout << "fTree->GetEntries() " << fTree->GetEntries() << std::endl;
    std::cout << "Total events: " << nevents << std::endl;

    for (int iEvent = 0; iEvent < nevents ; ++iEvent) {
                fTree->GetEntry(iEvent);
	            T_PFeval->GetEntry(iEvent);
	            T_KINEvars->GetEntry(iEvent);
                T_BDTvars->GetEntry(iEvent);
                T_eval->GetEntry(iEvent);


                TVector3 NuVtx(reco_nu_vtx_x, reco_nu_vtx_y, reco_nu_vtx_z); // NuVtx
                TVector3 ShVtx(reco_showervtxX, reco_showervtxY, reco_showervtxZ); // ShVtx
                TVector3 ShowerMomentum(reco_showerMomentum[0], reco_showerMomentum[1], reco_showerMomentum[2]); // ShowerDir
   

                int N_rec_protons = 0 , WC_N_rec_protons = 0 ;
                int n_sig_blips = 0, n_sig_blips_mc = 0, n_sig_blips_overlay = 0 ; 
                float SumE_sig_blips = 0, SumE_sig_blips_mc = 0, SumE_sig_blips_overlay = 0  ;          

		int n_sig_all_blips = 0 , n_sig_all_regA_blips = 0 ,n_sig_all_regB_blips = 0 ,n_sig_mc_blips = 0 ,n_sig_mc_regA_blips = 0 ,n_sig_mc_regB_blips = 0 ,n_sig_over_blips = 0 ,n_sig_over_regA_blips = 0 ,n_sig_over_regB_blips = 0 ;
		float SumE_sig_all_blips = 0 , SumE_sig_all_regA_blips = 0 ,SumE_sig_all_regB_blips = 0 ,SumE_sig_mc_blips = 0 ,SumE_sig_mc_regA_blips = 0 ,SumE_sig_mc_regB_blips = 0 ,SumE_sig_over_blips = 0 ,SumE_sig_over_regA_blips = 0 ,SumE_sig_over_regB_blips = 0 ;
   
                        if (iEvent < 0)
                         std::cout << "Event " << iEvent << " nblips_saved=" << nblips_saved
                            << " blip_energy size=" << (blip_energy ? blip_energy->size() : 0)
                            <<"\n  NuSelFil-reco_nuvtx(x,y,z): "<<reco_nu_vtx_x<<","<<reco_nu_vtx_y<<","<<reco_nu_vtx_z 
		                    <<"\n  PFEv-reco_nuvtx(x,y,z): "<<reco_nuvtxX<<","<<reco_nuvtxY<<","<<reco_nuvtxZ
		                    <<"\n  PFEv-reco_showervtx(x,y,z): "<<reco_showervtxX<<","<<reco_showervtxY<<","<<reco_showervtxZ
		                    <<"\n  PFEv-ShDirection(x,y,z): "<<reco_showerMomentum[0]<<","<<reco_showerMomentum[1]<<","<<reco_showerMomentum[2]
		                    <<"\n KINE-kine_reco_Enu: "<<kine_reco_Enu 
		                    <<"\n BDT-single_photon_numu_score: "<<single_photon_numu_score
                            <<"\n BDT-single_photon_other_score: "<<single_photon_other_score
		                    <<"\n BDT-single_photon_ncpi0_score: "<<single_photon_ncpi0_score
		                    <<"\n BDT-single_photon_nue_score: "<<single_photon_nue_score

                            <<"\n Eval-match_completeness_energy: "<<match_completeness_energy
                            <<"\n Eval-truth_energyInside: "<<truth_energyInside
		                    <<"\n PFEv-truth_single_photon: "<<truth_single_photon
		                    <<"\n Eval-truth_isCC: "<<truth_isCC
			                <<"\n PFEv-truth_NCDelta: "<<truth_NCDelta
			                <<"\n Eval-truth_vtxInside: "<<truth_vtxInside
		                    << std::endl;

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

                                    if (signal_events < 0)
                                    std::cout << "Signal Event " << iEvent << " nblips_saved=" << nblips_saved
                                    << " blip_energy size=" << (blip_energy ? blip_energy->size() : 0)
                                    <<"\n  NuSelFil-reco_nuvtx(x,y,z): "<<reco_nu_vtx_x<<","<<reco_nu_vtx_y<<","<<reco_nu_vtx_z
                                    <<"\n  PFEv-reco_nuvtx(x,y,z): "<<reco_nuvtxX<<","<<reco_nuvtxY<<","<<reco_nuvtxZ
                                    <<"\n  PFEv-reco_showervtx(x,y,z): "<<reco_showervtxX<<","<<reco_showervtxY<<","<<reco_showervtxZ
                                    <<"\n  PFEv-ShDirection(x,y,z): "<<reco_showerMomentum[0]<<","<<reco_showerMomentum[1]<<","<<reco_showerMomentum[2]
                                    <<"\n KINE-kine_reco_Enu: "<<kine_reco_Enu
                                    <<"\n BDT-single_photon_numu_score: "<<single_photon_numu_score
                                    <<"\n BDT-single_photon_other_score: "<<single_photon_other_score
                                    <<"\n BDT-single_photon_ncpi0_score: "<<single_photon_ncpi0_score
                                    <<"\n BDT-single_photon_nue_score: "<<single_photon_nue_score

                                    <<"\n Eval-match_completeness_energy: "<<match_completeness_energy
                                    <<"\n Eval-truth_energyInside: "<<truth_energyInside
                                    <<"\n PFEv-truth_single_photon: "<<truth_single_photon
                                    <<"\n Eval-truth_isCC: "<<truth_isCC
                                    <<"\n PFEv-truth_NCDelta: "<<truth_NCDelta
                                    <<"\n Eval-truth_vtxInside: "<<truth_vtxInside
                                    << std::endl;

                                    

                                    //-- Blip section --
                                        
                                     int backtracked_blip = 0 ;  
                                     // --- Blip loop ---
                                     for (size_t iBlip = 0; iBlip < nblips_saved ; ++iBlip) {

                                        TVector3 BlipVtx(blip_x->at(iBlip), blip_y->at(iBlip) , blip_z->at(iBlip));
                                        TVector3 ShowerVtx2Blip = BlipVtx - ShVtx; // Vector
		                                float blip_dist2vtx = (BlipVtx - ShVtx).Mag(); // Blip dist w.r.t ShVtx


                                        if (blip_nplanes->at(iBlip) > 1 &&          // 2&3 matched-planes (3D-blips)
                                            blip_touchtrk->at(iBlip) == 0 &&        // No blips touching tracks
                                            blip_pl2_bydeadwire->at(iBlip) == 0 &&  // No blips by dead wires in collection plane (pl2)
                                            blip_proxtrkdist->at(iBlip) > 15 &&      // Distance to closest track > 15 [cm]
                                            blip_dist2vtx < Radius                      // Blip distance from ShVtx
                                            //blip_energy->at(iBlip) > 0.6            // Reco blip energy > 0.6 [MeVee}
                                            //IsWithinSphereOutsideConic(ShVtx, ShowerMomentum, BlipVtx, Radius) //Qsphere

		                                    ){  

                                         double blip_angle_sh = calculateAngleBetweenVectors(ShowerVtx2Blip, ShowerMomentum);
                                         double cos_blip_angle_sh = calculateCosineAngleBetweenVectors(ShowerVtx2Blip, ShowerMomentum); 



                                         //-- Sphere blips
                                         //--All
                                         h_Blip_sphere_all_energy  ->Fill(blip_energy->at(iBlip));
                                         h_Blip_sphere_all_dist2vtx->Fill(blip_dist2vtx);
                                         h_Blip_sphere_all_angle   ->Fill(blip_angle_sh);
                                         h_Blip_sphere_all_cosangle->Fill(cos_blip_angle_sh);
                                         
                                         //Local MC-Overlay split
                                         //-- MC
                                         if (blip_true_g4id->at(iBlip) > 0) { 
                                             h_Blip_sphere_all_energy_mc  ->Fill(blip_energy->at(iBlip));
                                             h_Blip_sphere_all_dist2vtx_mc->Fill(blip_dist2vtx);
                                             h_Blip_sphere_all_angle_mc   ->Fill(blip_angle_sh);
                                             h_Blip_sphere_all_cosangle_mc->Fill(cos_blip_angle_sh);
                                             // Truth match 
                                             if (blip_true_pdg->at(iBlip) == 2212){ // truth matched proton blips
                                                 h_Blip_sphere_all_energy_mc_p  ->Fill(blip_energy->at(iBlip));
                                                 h_Blip_sphere_all_dist2vtx_mc_p->Fill(blip_dist2vtx);
                                                 h_Blip_sphere_all_angle_mc_p   ->Fill(blip_angle_sh);
                                                 h_Blip_sphere_all_cosangle_mc_p->Fill(cos_blip_angle_sh);      

                                                } else if (blip_true_pdg->at(iBlip) ==   11 || blip_true_pdg->at(iBlip) ==   -11) {// truth matched e+/e- blips
                                                            h_Blip_sphere_all_energy_mc_e  ->Fill(blip_energy->at(iBlip));
                                                            h_Blip_sphere_all_dist2vtx_mc_e->Fill(blip_dist2vtx);
                                                            h_Blip_sphere_all_angle_mc_e   ->Fill(blip_angle_sh);
                                                            h_Blip_sphere_all_cosangle_mc_e->Fill(cos_blip_angle_sh);          
                                                        }else if (blip_true_pdg->at(iBlip) == 1000010020 || // truth matched heavy nuclei-blips 1000010020-D 
                                                                  blip_true_pdg->at(iBlip) == 1000010030 || // 1000010030-T
                                                                  blip_true_pdg->at(iBlip) == 1000020030 || // 1000020030-He3
			                                          blip_true_pdg->at(iBlip) == 1000020040 ){ // 1000020040-alpha
                                            
                                                                            h_Blip_sphere_all_energy_mc_HN  ->Fill(blip_energy->at(iBlip));
                                                                            h_Blip_sphere_all_dist2vtx_mc_HN->Fill(blip_dist2vtx);
                                                                            h_Blip_sphere_all_angle_mc_HN   ->Fill(blip_angle_sh);
                                                                            h_Blip_sphere_all_cosangle_mc_HN->Fill(cos_blip_angle_sh);
                                                                }else{ // other truth match
                                                                    h_Blip_sphere_all_energy_mc_other  ->Fill(blip_energy->at(iBlip));
                                                                    h_Blip_sphere_all_dist2vtx_mc_other->Fill(blip_dist2vtx);
                                                                    h_Blip_sphere_all_angle_mc_other   ->Fill(blip_angle_sh);
                                                                    h_Blip_sphere_all_cosangle_mc_other->Fill(cos_blip_angle_sh);

                                                                }// End Truth match


                                                //-- Overlay
                                             }else{ h_Blip_sphere_all_energy_over  ->Fill(blip_energy->at(iBlip));
                                                    h_Blip_sphere_all_dist2vtx_over->Fill(blip_dist2vtx);
                                                    h_Blip_sphere_all_angle_over   ->Fill(blip_angle_sh);
                                                    h_Blip_sphere_all_cosangle_over->Fill(cos_blip_angle_sh); }
                                    
                                          //-- End Sphere blips


                                         //--------------------------
                                         //-- Signal blips section --
                                         //--------------------------
                                           if( IsWithinSphereOutsideConic(ShVtx, ShowerMomentum, BlipVtx, Radius) ){ //Qsphere
                                                
                                                 //--Signal All
                                                  n_sig_all_blips++ ; SumE_sig_all_blips+= blip_energy->at(iBlip) ;

                                                    h_Blip_signal_all_energy  ->Fill(blip_energy->at(iBlip));
                                                    h_Blip_signal_all_dist2vtx->Fill(blip_dist2vtx);
                                                    h_Blip_signal_all_angle   ->Fill(blip_angle_sh);
                                                    h_Blip_signal_all_cosangle->Fill(cos_blip_angle_sh);

                                                    

                                                    // Region B all
                                                    if( IsBackTrackedBlip( blip_dist2vtx , cos_blip_angle_sh ) ) { 
                                                        n_sig_all_regB_blips++ ; SumE_sig_all_regB_blips+= blip_energy->at(iBlip) ;
                                                        h_Blip_signal_all_regB_energy  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_all_regB_dist2vtx->Fill(blip_dist2vtx);
                                                        h_Blip_signal_all_regB_angle   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_all_regB_cosangle->Fill(cos_blip_angle_sh);



                                                     //Region A all
                                                    }else{  
                                                        n_sig_all_regA_blips++ ; SumE_sig_all_regA_blips+= blip_energy->at(iBlip) ;
                                                        h_Blip_signal_all_regA_energy  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_all_regA_dist2vtx->Fill(blip_dist2vtx);
                                                        h_Blip_signal_all_regA_angle   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_all_regA_cosangle->Fill(cos_blip_angle_sh);
                                                    }







                                                 //--Signal MC
                                                if (blip_true_g4id->at(iBlip) > 0) {
                                                     n_sig_mc_blips++ ; SumE_sig_mc_blips+= blip_energy->at(iBlip) ;
                                                                                              

                                                     //All MC
                                                     h_Blip_signal_mc_energy  ->Fill(blip_energy->at(iBlip));
                                                     h_Blip_signal_mc_dist2vtx->Fill(blip_dist2vtx);
                                                     h_Blip_signal_mc_angle   ->Fill(blip_angle_sh);
                                                     h_Blip_signal_mc_cosangle->Fill(cos_blip_angle_sh);

                                                     // Truth match 
                                                     if (blip_true_pdg->at(iBlip) == 2212){ // truth matched proton blips
                                                        h_Blip_signal_mc_energy_p  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_mc_dist2vtx_p->Fill(blip_dist2vtx);
                                                        h_Blip_signal_mc_angle_p   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_mc_cosangle_p->Fill(cos_blip_angle_sh);
                                                    
                                                        } else if (blip_true_pdg->at(iBlip) ==   11 || blip_true_pdg->at(iBlip) ==   -11) {// truth matched e+/e- blips
                                                                   h_Blip_signal_mc_energy_e  ->Fill(blip_energy->at(iBlip));
                                                                   h_Blip_signal_mc_dist2vtx_e->Fill(blip_dist2vtx);
                                                                   h_Blip_signal_mc_angle_e   ->Fill(blip_angle_sh);
                                                                   h_Blip_signal_mc_cosangle_e->Fill(cos_blip_angle_sh);

                                                                } else if (blip_true_pdg->at(iBlip) == 1000010020 || // truth matched heavy nuclei-blips 1000010020-D 
                                                                           blip_true_pdg->at(iBlip) == 1000010030 || // 1000010030-T
                                                                           blip_true_pdg->at(iBlip) == 1000020030 || // 1000020030-He3
			                                                   blip_true_pdg->at(iBlip) == 1000020040 ){ // 1000020040-alpha
                                                                                    h_Blip_signal_mc_energy_HN  ->Fill(blip_energy->at(iBlip));
                                                                                    h_Blip_signal_mc_dist2vtx_HN->Fill(blip_dist2vtx);
                                                                                    h_Blip_signal_mc_angle_HN   ->Fill(blip_angle_sh);
                                                                                    h_Blip_signal_mc_cosangle_HN->Fill(cos_blip_angle_sh);
                                                                            
                                                                        } else{ // other truth match
                                                                                h_Blip_signal_mc_energy_other  ->Fill(blip_energy->at(iBlip));
                                                                                h_Blip_signal_mc_dist2vtx_other->Fill(blip_dist2vtx);
                                                                                h_Blip_signal_mc_angle_other   ->Fill(blip_angle_sh);
                                                                                h_Blip_signal_mc_cosangle_other->Fill(cos_blip_angle_sh);
                                                        
                                                                                }// End Truth match





                                                    // MC Region B 
                                                    if( IsBackTrackedBlip( blip_dist2vtx , cos_blip_angle_sh ) ) { 
                                                        n_sig_mc_regB_blips++ ; SumE_sig_mc_regB_blips+= blip_energy->at(iBlip) ;

                                                        h_Blip_signal_mc_regB_energy  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_mc_regB_dist2vtx->Fill(blip_dist2vtx);
                                                        h_Blip_signal_mc_regB_angle   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_mc_regB_cosangle->Fill(cos_blip_angle_sh);

                                                        

                                                     // Truth match Region B 
                                                     if (blip_true_pdg->at(iBlip) == 2212){ // truth matched proton blips
                                                        h_Blip_signal_mc_regB_energy_p  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_mc_regB_dist2vtx_p->Fill(blip_dist2vtx);
                                                        h_Blip_signal_mc_regB_angle_p   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_mc_regB_cosangle_p->Fill(cos_blip_angle_sh);
                                                    
                                                        } else if (blip_true_pdg->at(iBlip) ==   11 || blip_true_pdg->at(iBlip) ==   -11) {// truth matched e+/e- blips
                                                                   h_Blip_signal_mc_regB_energy_e  ->Fill(blip_energy->at(iBlip));
                                                                   h_Blip_signal_mc_regB_dist2vtx_e->Fill(blip_dist2vtx);
                                                                   h_Blip_signal_mc_regB_angle_e   ->Fill(blip_angle_sh);
                                                                   h_Blip_signal_mc_regB_cosangle_e->Fill(cos_blip_angle_sh);

                                                                } else if (blip_true_pdg->at(iBlip) == 1000010020 || // truth matched heavy nuclei-blips 1000010020-D, 
                                                                           blip_true_pdg->at(iBlip) == 1000010030 || //1000010030-T,
                                                                           blip_true_pdg->at(iBlip) == 1000020030 || //1000020030-He3
			                                                   blip_true_pdg->at(iBlip) == 1000020040 ){ //1000020040-alpha
                                                                                    h_Blip_signal_mc_regB_energy_HN  ->Fill(blip_energy->at(iBlip));
                                                                                    h_Blip_signal_mc_regB_dist2vtx_HN->Fill(blip_dist2vtx);
                                                                                    h_Blip_signal_mc_regB_angle_HN   ->Fill(blip_angle_sh);
                                                                                    h_Blip_signal_mc_regB_cosangle_HN->Fill(cos_blip_angle_sh);
                                                                            
                                                                        } else{ // other truth match
                                                                                h_Blip_signal_mc_regB_energy_other  ->Fill(blip_energy->at(iBlip));
                                                                                h_Blip_signal_mc_regB_dist2vtx_other->Fill(blip_dist2vtx);
                                                                                h_Blip_signal_mc_regB_angle_other   ->Fill(blip_angle_sh);
                                                                                h_Blip_signal_mc_regB_cosangle_other->Fill(cos_blip_angle_sh);
                                                        
                                                                                 }// End Truth match Region B






                                                     // MC Region A 
                                                    }else{  
                                                        n_sig_mc_regA_blips++ ; SumE_sig_mc_regA_blips+= blip_energy->at(iBlip) ;
                                                        h_Blip_signal_mc_regA_energy  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_mc_regA_dist2vtx->Fill(blip_dist2vtx);
                                                        h_Blip_signal_mc_regA_angle   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_mc_regA_cosangle->Fill(cos_blip_angle_sh);



                                                        // Truth match Region A
                                                     if (blip_true_pdg->at(iBlip) == 2212){ // truth matched proton blips
                                                        h_Blip_signal_mc_regA_energy_p  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_mc_regA_dist2vtx_p->Fill(blip_dist2vtx);
                                                        h_Blip_signal_mc_regA_angle_p   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_mc_regA_cosangle_p->Fill(cos_blip_angle_sh);
                                                    
                                                        } else if (blip_true_pdg->at(iBlip) ==   11 || blip_true_pdg->at(iBlip) ==   -11) {// truth matched e+/e- blips
                                                                   h_Blip_signal_mc_regA_energy_e  ->Fill(blip_energy->at(iBlip));
                                                                   h_Blip_signal_mc_regA_dist2vtx_e->Fill(blip_dist2vtx);
                                                                   h_Blip_signal_mc_regA_angle_e   ->Fill(blip_angle_sh);
                                                                   h_Blip_signal_mc_regA_cosangle_e->Fill(cos_blip_angle_sh);

                                                                } else if (blip_true_pdg->at(iBlip) == 1000010020 || // truth matched heavy nuclei-blips 1000010020-D, 
                                                                           blip_true_pdg->at(iBlip) == 1000010030 || // 1000010030-T
                                                                           blip_true_pdg->at(iBlip) == 1000020030 || // 1000020030-He3
			                                                   blip_true_pdg->at(iBlip) == 1000020040 ){ // 1000020040-alpha
                                                                                    h_Blip_signal_mc_regA_energy_HN  ->Fill(blip_energy->at(iBlip));
                                                                                    h_Blip_signal_mc_regA_dist2vtx_HN->Fill(blip_dist2vtx);
                                                                                    h_Blip_signal_mc_regA_angle_HN   ->Fill(blip_angle_sh);
                                                                                    h_Blip_signal_mc_regA_cosangle_HN->Fill(cos_blip_angle_sh);
                                                                            
                                                                        } else{ // other truth match
                                                                                h_Blip_signal_mc_regA_energy_other  ->Fill(blip_energy->at(iBlip));
                                                                                h_Blip_signal_mc_regA_dist2vtx_other->Fill(blip_dist2vtx);
                                                                                h_Blip_signal_mc_regA_angle_other   ->Fill(blip_angle_sh);
                                                                                h_Blip_signal_mc_regA_cosangle_other->Fill(cos_blip_angle_sh);
                                                        
                                                                                }// End Truth match Region A


                                                    }//End MC Region A


                                                                                         
                                                 


                                                 
                                                 
                                                 //--Signal Overlay
                                                 }else{ //All Overlay
                                                 n_sig_over_blips++ ; SumE_sig_over_blips+= blip_energy->at(iBlip) ; 
                                                
                                                    h_Blip_signal_over_energy  ->Fill(blip_energy->at(iBlip));
                                                    h_Blip_signal_over_dist2vtx->Fill(blip_dist2vtx);
                                                    h_Blip_signal_over_angle   ->Fill(blip_angle_sh);
                                                    h_Blip_signal_over_cosangle->Fill(cos_blip_angle_sh);

                                                    

                                                    // Overlay Region B
                                                    if( IsBackTrackedBlip( blip_dist2vtx , cos_blip_angle_sh ) ) { 
                                                        n_sig_over_regB_blips++ ; SumE_sig_over_regB_blips+= blip_energy->at(iBlip) ;
                                                        h_Blip_signal_over_regB_energy  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_over_regB_dist2vtx->Fill(blip_dist2vtx);
                                                        h_Blip_signal_over_regB_angle   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_over_regB_cosangle->Fill(cos_blip_angle_sh);




                                                    }else{ // Overlay Region A 
                                                        n_sig_over_regA_blips++ ; SumE_sig_over_regA_blips+= blip_energy->at(iBlip) ;
                                                        h_Blip_signal_over_regA_energy  ->Fill(blip_energy->at(iBlip));
                                                        h_Blip_signal_over_regA_dist2vtx->Fill(blip_dist2vtx);
                                                        h_Blip_signal_over_regA_angle   ->Fill(blip_angle_sh);
                                                        h_Blip_signal_over_regA_cosangle->Fill(cos_blip_angle_sh);
                                                    }


                                                }//--Endoverlay Blips


                                            } //--- End Signal blips
			
                                               
                                        } //<--End Sphere blips
                                    



                                    } //<-- End Blip Loop 











                                    //Inclusive Single Photon channels


                                    WC_N_rec_protons = Get_Nproton(numu_cc_flag , kine_energy_particle,  kine_particle_type) ; 
			                        //std::cout<<"Signal Event: R/S/E: "<<run<<"/"<<subrun<<"/"<<event<< std::endl; 
                                    //std::cout<<"With:"<< Get_Nproton(numu_cc_flag , kine_energy_particle,  kine_particle_type)<<" reconstructed protons"<< std::endl;  

                                    N_rec_protons = AddBacktrackedBlips ? WC_N_rec_protons + n_sig_all_regB_blips  : WC_N_rec_protons;				    
                                                                          

                                    h_1gX_Nprotons->Fill( N_rec_protons); 

                                    // breakdown categories for single photon analysis
                                    if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_NCDelta==1 && truth_vtxInside==1) h_SPNCDeltaSig_Nprotons->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && (truth_showerMother==111) && truth_vtxInside==1) h_SPNCPi0Sig_Nprotons->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_showerMother!=111 && truth_NCDelta==0 && truth_vtxInside==1)  h_SPNCOtherSig_Nprotons->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1 && truth_vtxInside==1)  h_SPNumuCCSig_Nprotons->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && (truth_isCC==0 || (truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1)) && truth_vtxInside==0)  h_SPOutFVSig_Nprotons->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 ) h_SPNumuCCSigGen_Nprotons->Fill(N_rec_protons) ;


                                    // Background categories
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_vtxInside==0) h_SPoutFVBkg_Nprotons->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==14 && truth_isCC==1 && truth_vtxInside==1 && truth_Npi0==0) h_SPnumuCCBkg_Nprotons->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==14 && truth_isCC==1 && truth_vtxInside==1 && truth_Npi0>0) h_SPnumuCCpi0Bkg_Nprotons->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==12 && truth_isCC==1 && truth_vtxInside==1 ) h_SPnueCCBkg_Nprotons->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_isCC==0 && truth_vtxInside==1 && truth_Npi0==0)  h_SPNCBkg_Nprotons->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_isCC==0 && truth_vtxInside==1 && truth_Npi0>0) h_SPNCpi0Bkg_Nprotons->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy<=0.1*truth_energyInside ) h_CosmicBkg_Nprotons->Fill(N_rec_protons) ; //bad match
                                    else NoSigNorBkg++; 




				    h2D_1gX_BlipMultiplicity_SumEblip_regA->Fill(n_sig_all_regA_blips, SumE_sig_all_regA_blips);
			   	    h2D_1gX_BlipMultiplicity_SumEblip_regB->Fill(n_sig_all_regB_blips, SumE_sig_all_regB_blips);
				//0n/Nn designations
				    if(/*n_sig_all_blips*/ n_sig_all_regA_blips  <=  NBlips_Cut &&  /*SumE_sig_all_blips*/  SumE_sig_all_regA_blips  <= SumE_Cut ){ //0n
				    Events_0n_all++;
                                    h_1gX_Nprotons_0n->Fill( N_rec_protons);
			            h2D_1gX_BlipMultiplicity_SumEblip_0n->Fill(n_sig_all_regA_blips, SumE_sig_all_regA_blips);
				    //	if(n_sig_all_regA_blips == 0){std::cout<<"n_sig_all_regA_blips: "<<n_sig_all_regA_blips<< ", "<<"SumE_sig_all_regA_blips: "<<SumE_sig_all_regA_blips<<std::endl;
							           // std::cout<<"n_sig_all_regB_blips: "<<n_sig_all_regB_blips<< ", "<<"SumE_sig_all_regB_blips: "<<SumE_sig_all_regB_blips<<std::endl;}		 

                                    // breakdown categories for single photon analysis
                                    if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_NCDelta==1 && truth_vtxInside==1) h_SPNCDeltaSig_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && (truth_showerMother==111) && truth_vtxInside==1) h_SPNCPi0Sig_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_showerMother!=111 && truth_NCDelta==0 && truth_vtxInside==1)  h_SPNCOtherSig_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1 && truth_vtxInside==1)  h_SPNumuCCSig_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && (truth_isCC==0 || (truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1)) && truth_vtxInside==0)  h_SPOutFVSig_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 ) h_SPNumuCCSigGen_Nprotons_0n->Fill(N_rec_protons) ;


                                    // Background categories
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_vtxInside==0) h_SPoutFVBkg_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==14 && truth_isCC==1 && truth_vtxInside==1 && truth_Npi0==0) h_SPnumuCCBkg_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==14 && truth_isCC==1 && truth_vtxInside==1 && truth_Npi0>0) h_SPnumuCCpi0Bkg_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==12 && truth_isCC==1 && truth_vtxInside==1 ) h_SPnueCCBkg_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_isCC==0 && truth_vtxInside==1 && truth_Npi0==0)  h_SPNCBkg_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_isCC==0 && truth_vtxInside==1 && truth_Npi0>0) h_SPNCpi0Bkg_Nprotons_0n->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy<=0.1*truth_energyInside ) h_CosmicBkg_Nprotons_0n->Fill(N_rec_protons) ; //bad match
                                    else NoSigNorBkg0n++;	

				   }else{ //Nn
				   Events_Nn_all++;	
                                   h_1gX_Nprotons_Nn->Fill( N_rec_protons);
				   h2D_1gX_BlipMultiplicity_SumEblip_Nn->Fill(n_sig_all_regA_blips, SumE_sig_all_regA_blips);

                                    // breakdown categories for single photon analysis
                                    if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_NCDelta==1 && truth_vtxInside==1) h_SPNCDeltaSig_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && (truth_showerMother==111) && truth_vtxInside==1) h_SPNCPi0Sig_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_showerMother!=111 && truth_NCDelta==0 && truth_vtxInside==1)  h_SPNCOtherSig_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1 && truth_vtxInside==1)  h_SPNumuCCSig_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && (truth_isCC==0 || (truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1)) && truth_vtxInside==0)  h_SPOutFVSig_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 ) h_SPNumuCCSigGen_Nprotons_Nn->Fill(N_rec_protons) ;


                                    // Background categories
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_vtxInside==0) h_SPoutFVBkg_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==14 && truth_isCC==1 && truth_vtxInside==1 && truth_Npi0==0) h_SPnumuCCBkg_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==14 && truth_isCC==1 && truth_vtxInside==1 && truth_Npi0>0) h_SPnumuCCpi0Bkg_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && abs(truth_nuPdg)==12 && truth_isCC==1 && truth_vtxInside==1 ) h_SPnueCCBkg_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_isCC==0 && truth_vtxInside==1 && truth_Npi0==0)  h_SPNCBkg_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if(match_completeness_energy>0.1*truth_energyInside && truth_isCC==0 && truth_vtxInside==1 && truth_Npi0>0) h_SPNCpi0Bkg_Nprotons_Nn->Fill(N_rec_protons) ;
                                    else if (match_completeness_energy<=0.1*truth_energyInside ) h_CosmicBkg_Nprotons_Nn->Fill(N_rec_protons) ; //bad match
                                    else NoSigNorBkgNn++;

			            }



/*
                                    // breakdown of 1g-signal into true 0p/Np for single photon analysis
                                    if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_NCDelta==1 && truth_vtxInside==1) { if(is_true_0p) h_SPNCDeltaSig_Nprotons_true0p->Fill(N_rec_protons) ; 
																							 else   h_SPNCDeltaSig_Nprotons_trueNp->Fill(N_rec_protons) ;}
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && (truth_showerMother==111) && truth_vtxInside==1){ if(is_true_0p) h_SPNCPi0Sig_Nprotons_true0p->Fill(N_rec_protons) ; 
																								        else h_SPNCPi0Sig_Nprotons_trueNp->Fill(N_rec_protons) ;}
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 && truth_showerMother!=111 && truth_NCDelta==0 && truth_vtxInside==1)  { if(is_true_0p) h_SPNCOtherSig_Nprotons_true0p->Fill(N_rec_protons) ;
																											   else  h_SPNCOtherSig_Nprotons_trueNp->Fill(N_rec_protons) ;}
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1 && truth_vtxInside==1) { if(is_true_0p) h_SPNumuCCSig_Nprotons_true0p->Fill(N_rec_protons) ; 
																													  else h_SPNumuCCSig_Nprotons_trueNp->Fill(N_rec_protons) ; }
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && (truth_isCC==0 || (truth_isCC==1 && abs(truth_nuPdg)==14 && truth_muonMomentum[3]-0.105658<0.1)) && truth_vtxInside==0)  { if(is_true_0p)  h_SPOutFVSig_Nprotons_true0p->Fill(N_rec_protons) ; 
																																 else h_SPOutFVSig_Nprotons_trueNp->Fill(N_rec_protons) ; }
                                    else if (match_completeness_energy>0.1*truth_energyInside && truth_single_photon==1 && truth_isCC==0 )  { if(is_true_0p) h_SPNumuCCSigGen_Nprotons_true0p->Fill(N_rec_protons) ; else h_SPNumuCCSigGen_Nprotons_trueNp->Fill(N_rec_protons) ;}
			            else dummycounter++;


*/



                                    //----------------------
                                    // --- 0p/Np split ---
                                    //----------------------
                                    h_1gX_BlipMultiplicity->Fill(n_sig_all_blips);
                                    h_1gX_SumEblip->Fill(SumE_sig_all_blips); 
			            h2D_1gX_BlipMultiplicity_SumEblip->Fill(n_sig_all_blips, SumE_sig_all_blips);


                                    if (is_0p(numu_cc_flag , kine_energy_particle,  kine_particle_type) == true) {
                                     // 0p-dataset 
                                     
                                        h_1gX_BlipMultiplicity_0p->Fill(n_sig_all_blips);
                                        h_1gX_SumEblip_0p->Fill(SumE_sig_all_blips);
					h2D_1gX_BlipMultiplicity_SumEblip_0p->Fill(n_sig_all_blips, SumE_sig_all_blips);
					
					/* //0n/Nn split
					if(n_sig_all_blips < 10 &&  SumE_sig_all_blips <= 8 ){
                                        h_1gX_Nprotons_0p0n->Fill( N_rec_protons); //0p0n
                                        }else{  h_1gX_Nprotons_0pNn->Fill( N_rec_protons); //0pNn
                                             }*/
						                                    
                                    }else{ //Np-dataset
                                        h_1gX_BlipMultiplicity_Np->Fill(n_sig_all_blips);
                                        h_1gX_SumEblip_Np->Fill(SumE_sig_all_blips); 
					h2D_1gX_BlipMultiplicity_SumEblip_Np->Fill(n_sig_all_blips, SumE_sig_all_blips);

					/*if(n_sig_all_blips < 10 &&  SumE_sig_all_blips <= 8 ){
                                        h_1gX_Nprotons_Np0n->Fill( N_rec_protons); //Np0n
                                        }else{  h_1gX_Nprotons_Np0pNn->Fill( N_rec_protons); //NpNn
                                             } */
                                    }


                                        if( Get_Nproton(numu_cc_flag , kine_energy_particle,  kine_particle_type)== 0 && backtracked_blip > 0 )WC_0p_wBB++ ; 
                                        if( Get_Nproton(numu_cc_flag , kine_energy_particle,  kine_particle_type) > 0 && backtracked_blip > 0 )WC_Np_wBB++ ; 
	                                    backtracked_blip = 0 ; 


                                }// Signal Events
                            
                            
                            
                            }//<--End Event Loop



    
   Float_t a;
   TTree tree("evd_tree", "TTree with a single float branch for number of signal events");
   TBranch* branch = tree.Branch("total_signal_events", &a, "total_signal_events/F");
   a = signal_events ; 
   tree.Fill();


    std::cout << "signal events/total events = "<< signal_events <<" / "<<nevents <<" = " << (float) signal_events / nevents<<std::endl; 
    std::cout << "WC_0p_wBB: "<< WC_0p_wBB <<std::endl; 
    std::cout << "WC_Np_wBB: "<< WC_Np_wBB <<std::endl;
    std::cout << "Events_0n_all = "<<Events_0n_all <<std::endl;
    std::cout << "Events_Nn_all = "<<Events_Nn_all <<std::endl;
   
/* 
    std::cout <<"backtracked_blip_mc: "<< backtracked_blip_mc <<std::endl; 
    std::cout <<"backtracked_blip_mc_proton: "<< backtracked_blip_mc_proton <<std::endl;
    std::cout <<"backtracked_blip_mc_electron: "<< backtracked_blip_mc_electron <<std::endl;
    std::cout <<"backtracked_blip_mc_heavyN: "<<backtracked_blip_mc_heavyN  <<std::endl;
    std::cout <<"backtracked_blip_mc_othe: "<<backtracked_blip_mc_other  <<std::endl;
    std::cout <<"backtracked_blip_overlay: "<< backtracked_blip_overlay  <<std::endl;
*/
    std::cout <<"\n"<<std::endl; 
    std::cout<<" h_1gX_Nprotons :"<< h_1gX_Nprotons->GetEntries()<<std::endl; 
    std::cout <<"Signal Categories\n"<<std::endl;
    std::cout <<"h_SPNCDeltaSig_Nprotons :"<< h_SPNCDeltaSig_Nprotons->GetEntries()<<std::endl;
    std::cout <<"h_SPNCPi0Sig_Nprotons   :"<< h_SPNCPi0Sig_Nprotons  ->GetEntries()<<std::endl; 
    std::cout <<"h_SPNCOtherSig_Nprotons :"<< h_SPNCOtherSig_Nprotons->GetEntries()<<std::endl; 
    std::cout <<"h_SPNumuCCSig_Nprotons  :"<< h_SPNumuCCSig_Nprotons ->GetEntries()<<std::endl;
    std::cout <<"h_SPNumuCCSigGen_Nprotons  :"<< h_SPNumuCCSigGen_Nprotons ->GetEntries()<<std::endl;
    std::cout <<"h_SPOutFVSig_Nprotons   :"<< h_SPOutFVSig_Nprotons  ->GetEntries()<<std::endl;
    std::cout <<"Background Categories :"<<std::endl;
    std::cout <<"h_SPoutFVBkg_Nprotons   :"<<h_SPoutFVBkg_Nprotons   ->GetEntries()<<std::endl;
    std::cout <<"h_SPnumuCCBkg_Nprotons  :"<<h_SPnumuCCBkg_Nprotons  ->GetEntries()<<std::endl;
    std::cout <<"h_SPnumuCCpi0Bkg_Nprotons:"<<h_SPnumuCCpi0Bkg_Nprotons->GetEntries()<<std::endl;
    std::cout <<"h_SPnueCCBkg_Nprotons  :"<<h_SPnueCCBkg_Nprotons  ->GetEntries()<<std::endl;
    std::cout <<"h_SPNCBkg_Nprotons       :"<<h_SPNCBkg_Nprotons     ->GetEntries()<<std::endl;
    std::cout <<"h_SPNCpi0Bkg_Nprotons    :"<<h_SPNCpi0Bkg_Nprotons ->GetEntries()<<std::endl;
    std::cout <<"h_CosmicBkg_Nprotons     :"<<h_CosmicBkg_Nprotons ->GetEntries()<<std::endl;
    std::cout <<"No Signal nor Background:"<<NoSigNorBkg<<std::endl; 
   
//     std::cout << "Histograms saved to " << outputFile << std::endl;
    fOutFile->Write();
    fOutFile->Close();
    fInputFile->Close();
    return 0;













}//<--End main
