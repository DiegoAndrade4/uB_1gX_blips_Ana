// Usage: root -l 'analyze2DHist.C("input.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 100, 0, 20, 1, 0, 50, 1, "output.txt")'

#include <TFile.h>
#include <TH2F.h>
#include <iostream>
#include <fstream>

void analyze2DHist(
    const char* inputFile,
    const char* histName,
    float R,                    // Input parameter R
    float X_cut_min,            // X_cut range start
    float X_cut_max,            // X_cut range end
    float X_cut_step,           // X_cut step size
    float Y_cut_min,            // Y_cut range start
    float Y_cut_max,            // Y_cut range end
    float Y_cut_step,           // Y_cut step size
    const char* outputFile = "output.txt"
) {
    
    // Open input ROOT file
    TFile* fInput = TFile::Open(inputFile);
    if (!fInput || fInput->IsZombie()) {
        std::cerr << "ERROR: Could not open input file: " << inputFile << std::endl;
        return;
    }
    
    // Get 2D histogram
    TH2F* h2D = (TH2F*)fInput->Get(histName);
    if (!h2D) {
        std::cerr << "ERROR: Could not find histogram: " << histName << std::endl;
        fInput->Close();
        return;
    }
    
    // Get total number of entries
    int TotEv = h2D->GetEntries();
    
    std::cout << "Histogram: " << histName << std::endl;
    std::cout << "Total Entries: " << TotEv << std::endl;
    std::cout << "Radius [cm]: R parameter: " << R << std::endl;
    std::cout << "Multiplicity [#blips]: X_cut range : [" << X_cut_min << ", " << X_cut_max << "] step=" << X_cut_step << std::endl;
    std::cout << "SumEb [MeVee]:  Y_cut range : [" << Y_cut_min << ", " << Y_cut_max << "] step=" << Y_cut_step << std::endl;
    
    // Open output text file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Could not open output file: " << outputFile << std::endl;
        fInput->Close();
        return;
    }
    
    // Write header
    outFile << "TotEv\tR\tX_cut\tY_cut\tOuter_Ev\tInner_Ev" << std::endl;
    
    // Get histogram axis information
    TAxis* xaxis = h2D->GetXaxis();
    TAxis* yaxis = h2D->GetYaxis();
    
    // Nested loop over X_cut and Y_cut
    for (float X_cut = X_cut_min; X_cut <= X_cut_max; X_cut += X_cut_step) {
        for (float Y_cut = Y_cut_min; Y_cut <= Y_cut_max; Y_cut += Y_cut_step) {
            
            // Find bin numbers corresponding to the cuts
            int X_bin = xaxis->FindBin(X_cut);
            int Y_bin = yaxis->FindBin(Y_cut);
            
            // Count events inside the boundary (X <= X_cut AND Y <= Y_cut)
            int Inner_Ev = 0;
            for (int i = 1; i <= X_bin; ++i) {
                for (int j = 1; j <= Y_bin; ++j) {
                    Inner_Ev += h2D->GetBinContent(i, j);
                }
            }
            
            // Calculate events outside the boundary
            int Outer_Ev = TotEv - Inner_Ev;
            
            // Write to output file (tab-separated)
            outFile << TotEv << "\t" 
                    << R << "\t" 
                    << X_cut << "\t" 
                    << Y_cut << "\t" 
                    << Outer_Ev << "\t" 
                    << Inner_Ev << std::endl;
        }
    }
    
    outFile.close();
    fInput->Close();
    
    std::cout << "Analysis complete. Output written to: " << outputFile << std::endl;
}
