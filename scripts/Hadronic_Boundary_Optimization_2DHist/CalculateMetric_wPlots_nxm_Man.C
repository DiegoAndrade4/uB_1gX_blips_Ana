// Macro to compare two files and calculate metric
// Usage: root -l 'CalculateMetric_wPlots_nxm_Man.C("file1_BNBNuOverlay.txt", "file2_EXTUnb.txt")'

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TMarker.h"
#include "TMultiGraph.h"
#include "TFile.h"

struct EventData {
    double SigEvts, R, Multiplicity, ESum, EvtNn, Evt0n;
    
    // Hash function for use as map key
    size_t hash() const {
        // Combine hashes of the matching fields
        size_t h1 = std::hash<double>{}(SigEvts);
        size_t h2 = std::hash<double>{}(R);
        size_t h3 = std::hash<double>{}(Multiplicity);
        size_t h4 = std::hash<double>{}(ESum);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
    
    // Equality operator for matching
    bool matches(const EventData& other, double tolerance = 1e-9) const {
        return (std::abs(SigEvts - other.SigEvts) < tolerance) &&
               (std::abs(R - other.R) < tolerance) &&
               (std::abs(Multiplicity - other.Multiplicity) < tolerance) &&
               (std::abs(ESum - other.ESum) < tolerance);
    }
};

struct MinMaxInfo {
    double minMetric, maxMetric;
    double minMult, maxMult;
    double minESum, maxESum;
    int minIndex, maxIndex;
    
    MinMaxInfo() : minMetric(1e99), maxMetric(-1e99), 
                   minMult(0), maxMult(0), minESum(0), maxESum(0),
                   minIndex(-1), maxIndex(-1) {}
};

// Function to extract base name from file path
std::string GetBaseName(const std::string& filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    size_t lastDot = filepath.find_last_of(".");
    
    std::string filename;
    if (lastSlash != std::string::npos) {
        filename = filepath.substr(lastSlash + 1);
    } else {
        filename = filepath;
    }
    
    if (lastDot != std::string::npos && lastDot > lastSlash) {
        return filename.substr(0, lastDot);
    }
    return filename;
}

// Function to create output directory
std::string CreateOutputDirectory(const std::string& file1, const std::string& file2) {
    std::string base1 = GetBaseName(file1);
    std::string base2 = GetBaseName(file2);
    std::string dirName = base1 + "_vs_" + base2 + "_results";
    
    mkdir(dirName.c_str(), 0755);
    
    return dirName;
}

// Function to read event data from file
std::vector<EventData> ReadFile(const char* filename) {
    std::vector<EventData> events;
    std::ifstream infile(filename);
    
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return events;
    }
    
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        EventData evt;
        
        if (iss >> evt.SigEvts >> evt.R >> evt.Multiplicity >> evt.ESum >> evt.EvtNn >> evt.Evt0n) {
            events.push_back(evt);
        }
    }
    
    infile.close();
    std::cout << "Read " << events.size() << " events from " << filename << std::endl;
    return events;
}

void CalculateMetric_wPlots_nxm_Man(const char* filename1, const char* filename2) {
    
    std::cout << "========================================" << std::endl;
    std::cout << "Starting Metric Calculation" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::vector<EventData> file1_data = ReadFile(filename1);
    std::vector<EventData> file2_data = ReadFile(filename2);
    
    if (file1_data.empty() || file2_data.empty()) {
        std::cerr << "Error: One or both files are empty" << std::endl;
        return;
    }
    
    std::string outputDir = CreateOutputDirectory(filename1, filename2);
    std::cout << "Output directory: " << outputDir << std::endl;
    
    std::string outfile1_path = outputDir + "/output_short.txt";
    std::string outfile2_path = outputDir + "/output_detailed.txt";
    std::string summary_path = outputDir + "/summary_statistics.txt";
    std::string rootfile_path = outputDir + "/plots.root";
    
    std::ofstream outfile1(outfile1_path.c_str());
    std::ofstream outfile2(outfile2_path.c_str());
    std::ofstream summaryFile(summary_path.c_str());
    
    if (!outfile1.is_open() || !outfile2.is_open() || !summaryFile.is_open()) {
        std::cerr << "Error: Cannot create output files" << std::endl;
        return;
    }
    
    outfile1 << "# R Multiplicity ESum metric" << std::endl;
    outfile2 << "# SigEvts R Multiplicity ESum EvtNn1 Evt0n1 EvtNn2 Evt0n2 metric" << std::endl;
    
    // Write header to summary file
    summaryFile << "========================================" << std::endl;
    summaryFile << "Metric Calculation Summary" << std::endl;
    summaryFile << "========================================" << std::endl;
    summaryFile << "File 1: " << filename1 << std::endl;
    summaryFile << "File 2: " << filename2 << std::endl;
    summaryFile << "========================================" << std::endl << std::endl;
    
    size_t nEvents = std::min(file1_data.size(), file2_data.size());
    int validEvents = 0;
    int skippedEvents = 0;
    
    // Global min/max tracking
    MinMaxInfo globalMinMax;
    double globalMinR = 0, globalMaxR = 0;
    
    // Vectors for plotting
    std::vector<double> metricValues, eventIndices, rValues, multiplicityValues, eSumValues;
    
    // Maps for grouping by R
    std::map<double, std::vector<double>> rGroupedMult, rGroupedESum, rGroupedMetric;
    std::map<double, MinMaxInfo> rMinMaxMap;
    
    std::cout << "\nProcessing " << nEvents << " events..." << std::endl;
    summaryFile << "Processing events..." << std::endl;
    summaryFile << "File 1: " << file1_data.size() << " events" << std::endl;
    summaryFile << "File 2: " << file2_data.size() << " events" << std::endl << std::endl;
    
    // Match events from file1 with file2
    for (size_t i = 0; i < file1_data.size(); i++) {
        const EventData& evt1 = file1_data[i];
        
        // Find matching event in file2
        bool found = false;
        EventData evt2;
        
        for (size_t j = 0; j < file2_data.size(); j++) {
            if (evt1.matches(file2_data[j])) {
                evt2 = file2_data[j];
                found = true;
                break;
            }
        }
        
        if (!found) {
            if (skippedEvents < 5) {
                std::cout << "WARNING: Event " << i << " from file1 has no match in file2 - skipping" << std::endl;
                summaryFile << "WARNING: Event " << i << " from file1 has no match in file2 - skipping" << std::endl;
            }
            skippedEvents++;
            continue;
        }
       // Metric calculation 
        double denominator = std::sqrt( evt1.EvtNn +  evt2.EvtNn );
        double metric = 0.0;
        
        if (denominator > 0) {
            double ratio = (evt1.EvtNn - evt2.EvtNn) / denominator;
            metric = ratio * ratio;
        }
        
        outfile1 << evt1.R << " " << evt1.Multiplicity << " " << evt1.ESum << " " << metric << std::endl;
        outfile2 << evt1.SigEvts << " " << evt1.R << " " << evt1.Multiplicity << " " << evt1.ESum << " "
                 << evt1.EvtNn << " " << evt1.Evt0n << " " << evt2.EvtNn << " " << evt2.Evt0n << " "
                 << metric << std::endl;
        
        metricValues.push_back(metric);
        eventIndices.push_back(validEvents);
        rValues.push_back(evt1.R);
        multiplicityValues.push_back(evt1.Multiplicity);
        eSumValues.push_back(evt1.ESum);
        
        // Group by R
        rGroupedMult[evt1.R].push_back(evt1.Multiplicity);
        rGroupedESum[evt1.R].push_back(evt1.ESum);
        rGroupedMetric[evt1.R].push_back(metric);
        
        // Track global min/max
        if (metric < globalMinMax.minMetric) {
            globalMinMax.minMetric = metric;
            globalMinMax.minMult = evt1.Multiplicity;
            globalMinMax.minESum = evt1.ESum;
            globalMinMax.minIndex = validEvents;
            globalMinR = evt1.R;
        }
        
        if (metric > globalMinMax.maxMetric) {
            globalMinMax.maxMetric = metric;
            globalMinMax.maxMult = evt1.Multiplicity;
            globalMinMax.maxESum = evt1.ESum;
            globalMinMax.maxIndex = validEvents;
            globalMaxR = evt1.R;
        }
        
        // Track min/max per R group
        if (rMinMaxMap.find(evt1.R) == rMinMaxMap.end()) {
            rMinMaxMap[evt1.R] = MinMaxInfo();
        }
        
        MinMaxInfo& rInfo = rMinMaxMap[evt1.R];
        if (metric < rInfo.minMetric) {
            rInfo.minMetric = metric;
            rInfo.minMult = evt1.Multiplicity;
            rInfo.minESum = evt1.ESum;
            rInfo.minIndex = rGroupedMetric[evt1.R].size() - 1;
        }
        
        if (metric > rInfo.maxMetric) {
            rInfo.maxMetric = metric;
            rInfo.maxMult = evt1.Multiplicity;
            rInfo.maxESum = evt1.ESum;
            rInfo.maxIndex = rGroupedMetric[evt1.R].size() - 1;
        }
        
        validEvents++;
        
        if ((i + 1) % 1000 == 0) {
            std::cout << "Processed " << (i + 1) << " / " << file1_data.size() << " events..." << std::endl;
        }
    }
    
    outfile1.close();
    outfile2.close();
    
    // Summary output (both to console and file)
    std::string separator = "========================================";
    std::string output;
    
    output = "\n" + separator + "\n";
    output += "Processing Complete!\n";
    output += separator + "\n";
    std::cout << output;
    summaryFile << output;
    
    output = "Valid events processed: " + std::to_string(validEvents) + "\n";
    output += "Skipped events (mismatch): " + std::to_string(skippedEvents) + "\n";
    std::cout << output;
    summaryFile << output;
    
    if (validEvents > 0) {
        output = "\n--- GLOBAL Metric Statistics ---\n";
        std::cout << output;
        summaryFile << output;
        
        output = "MINIMUM metric = " + std::to_string(globalMinMax.minMetric) + "\n";
        output += "  at event index: " + std::to_string(globalMinMax.minIndex) + "\n";
        output += "  R = " + std::to_string(globalMinR) + "\n";
        output += "  Multiplicity = " + std::to_string(globalMinMax.minMult) + "\n";
        output += "  ESum = " + std::to_string(globalMinMax.minESum) + "\n";
        std::cout << output;
        summaryFile << output;
        
        output = "\nMAXIMUM metric = " + std::to_string(globalMinMax.maxMetric) + "\n";
        output += "  at event index: " + std::to_string(globalMinMax.maxIndex) + "\n";
        output += "  R = " + std::to_string(globalMaxR) + "\n";
        output += "  Multiplicity = " + std::to_string(globalMinMax.maxMult) + "\n";
        output += "  ESum = " + std::to_string(globalMinMax.maxESum) + "\n";
        std::cout << output;
        summaryFile << output;
        
        // Print min/max for each R group
        output = "\n--- Per-R Metric Statistics ---\n";
        std::cout << output;
        summaryFile << output;
        
        for (auto const& entry : rMinMaxMap) {
            double R_val = entry.first;
            const MinMaxInfo& info = entry.second;
            
            output = "\nR = " + std::to_string(R_val) + " (" + std::to_string(rGroupedMetric[R_val].size()) + " events)\n";
            output += "  Min metric = " + std::to_string(info.minMetric) + 
                      " at Mult=" + std::to_string(info.minMult) + 
                      ", ESum=" + std::to_string(info.minESum) + "\n";
            output += "  Max metric = " + std::to_string(info.maxMetric) + 
                      " at Mult=" + std::to_string(info.maxMult) + 
                      ", ESum=" + std::to_string(info.maxESum) + "\n";
            std::cout << output;
            summaryFile << output;
        }
        
        // Create ROOT file for plots
        TFile* rootFile = new TFile(rootfile_path.c_str(), "RECREATE");
        
        // Create plots
        output = "\nCreating plots...\n";
        std::cout << output;
        summaryFile << output;
        
        gStyle->SetOptStat(0);
        
        // Plot 1: Metric vs Event Index
        TCanvas* c1 = new TCanvas("c1", "Metric vs Event", 1200, 800);
        c1->SetGrid();
        
        TGraph* grMetric = new TGraph(validEvents, &eventIndices[0], &metricValues[0]);
        grMetric->SetTitle("Metric vs Event Index;Event Index;Metric = [(EvtNn_{1} - EvtNn_{2}) / #sqrt{EvtNn_{1} + EvtNn_{2}}]^{2}");
        grMetric->SetMarkerStyle(20);
        grMetric->SetMarkerSize(0.5);
        grMetric->SetMarkerColor(kBlue);
        grMetric->SetName("grMetric");
        grMetric->Draw("AP");
        
        TMarker* minMarker = new TMarker(globalMinMax.minIndex, globalMinMax.minMetric, 29);
        minMarker->SetMarkerColor(kRed);
        minMarker->SetMarkerSize(2);
        minMarker->Draw();
        
        TMarker* maxMarker = new TMarker(globalMinMax.maxIndex, globalMinMax.maxMetric, 29);
        maxMarker->SetMarkerColor(kGreen+2);
        maxMarker->SetMarkerSize(2);
        maxMarker->Draw();
        
        TLatex* minLabel = new TLatex(globalMinMax.minIndex, globalMinMax.minMetric * 1.05, 
                                       Form("Min = %.4f", globalMinMax.minMetric));
        minLabel->SetTextColor(kRed);
        minLabel->SetTextSize(0.03);
        minLabel->Draw();
        
        TLatex* maxLabel = new TLatex(globalMinMax.maxIndex, globalMinMax.maxMetric * 0.95, 
                                       Form("Max = %.4f", globalMinMax.maxMetric));
        maxLabel->SetTextColor(kGreen+2);
        maxLabel->SetTextSize(0.03);
        maxLabel->Draw();
        
        std::string plot1_path = outputDir + "/metric_vs_event.png";
        c1->SaveAs(plot1_path.c_str());
        c1->Write();
        std::cout << "Saved: " << plot1_path << std::endl;
        summaryFile << "Saved: " << plot1_path << std::endl;
        
        // Plot 2: Metric histogram
        TCanvas* c2 = new TCanvas("c2", "Metric Distribution", 1200, 800);
        
        TH1D* hMetric = new TH1D("hMetric", "Metric Distribution;Metric;Number of Events", 
                                 50, globalMinMax.minMetric * 0.95, globalMinMax.maxMetric * 1.05);
        for (size_t i = 0; i < metricValues.size(); i++) {
            hMetric->Fill(metricValues[i]);
        }
        hMetric->SetFillColor(kBlue-9);
        hMetric->SetLineColor(kBlue);
        hMetric->Draw();
        
        std::string plot2_path = outputDir + "/metric_histogram.png";
        c2->SaveAs(plot2_path.c_str());
        c2->Write();
        std::cout << "Saved: " << plot2_path << std::endl;
        summaryFile << "Saved: " << plot2_path << std::endl;
        
        // Plot 3: Metric vs R, Multiplicity, ESum
        TCanvas* c3 = new TCanvas("c3", "Metric Correlations", 1800, 600);
        c3->Divide(3, 1);
        
        c3->cd(1);
        gPad->SetGrid();
        TGraph* grMetricR = new TGraph(validEvents, &rValues[0], &metricValues[0]);
        grMetricR->SetTitle("Metric vs R;R;Metric");
        grMetricR->SetMarkerStyle(20);
        grMetricR->SetMarkerSize(0.5);
        grMetricR->SetMarkerColor(kBlue);
        grMetricR->SetName("grMetricR");
        grMetricR->Draw("AP");
        
        c3->cd(2);
        gPad->SetGrid();
        TGraph* grMetricMult = new TGraph(validEvents, &multiplicityValues[0], &metricValues[0]);
        grMetricMult->SetTitle("Metric vs Multiplicity;Multiplicity;Metric");
        grMetricMult->SetMarkerStyle(20);
        grMetricMult->SetMarkerSize(0.5);
        grMetricMult->SetMarkerColor(kRed);
        grMetricMult->SetName("grMetricMult");
        grMetricMult->Draw("AP");
        
        c3->cd(3);
        gPad->SetGrid();
        TGraph* grMetricESum = new TGraph(validEvents, &eSumValues[0], &metricValues[0]);
        grMetricESum->SetTitle("Metric vs ESum;ESum;Metric");
        grMetricESum->SetMarkerStyle(20);
        grMetricESum->SetMarkerSize(0.5);
        grMetricESum->SetMarkerColor(kGreen+2);
        grMetricESum->SetName("grMetricESum");
        grMetricESum->Draw("AP");
        
        std::string plot3_path = outputDir + "/metric_correlations.png";
        c3->SaveAs(plot3_path.c_str());
        c3->Write();
        std::cout << "Saved: " << plot3_path << std::endl;
        summaryFile << "Saved: " << plot3_path << std::endl;
        
        // Create 3D plots for each R value
        output = "\n--- Creating 3D plots for each R value ---\n";
        output += "Found " + std::to_string(rGroupedMult.size()) + " unique R values\n";
        std::cout << output;
        summaryFile << output;
        
        int plotIndex = 4;
        for (auto const& entry : rGroupedMult) {
            double R_val = entry.first;
            const std::vector<double>& mult = entry.second;
            const std::vector<double>& esum = rGroupedESum[R_val];
            const std::vector<double>& metric = rGroupedMetric[R_val];
            const MinMaxInfo& info = rMinMaxMap[R_val];
            
            if (mult.size() > 0) {
                TCanvas* canvas = new TCanvas(Form("c%d_R_%.1f", plotIndex, R_val), 
                                              Form("3D Plot at R = %.1f", R_val), 1200, 800);
                
                TGraph2D* gr3D = new TGraph2D(mult.size());
                gr3D->SetTitle(Form("Metric vs Multiplicity vs ESum at R = %.1f;Multiplicity;ESum;Metric", R_val));
                gr3D->SetName(Form("gr3D_R_%.1f", R_val));
                
                for (size_t i = 0; i < mult.size(); i++) {
                    gr3D->SetPoint(i, mult[i], esum[i], metric[i]);
                }
                
                gr3D->SetMarkerStyle(20);
                gr3D->SetMarkerSize(1);
                gr3D->SetMarkerColor(kBlue);
                gr3D->Draw("PCOL");
                
                // Mark min and max points on this 3D plot
                TGraph2D* minPoint = new TGraph2D(1);
                minPoint->SetPoint(0, info.minMult, info.minESum, info.minMetric);
                minPoint->SetMarkerStyle(29);
                minPoint->SetMarkerSize(2);
                minPoint->SetMarkerColor(kRed);
                minPoint->SetName(Form("minPoint_R_%.1f", R_val));
                minPoint->Draw("P SAME");
                
                TGraph2D* maxPoint = new TGraph2D(1);
                maxPoint->SetPoint(0, info.maxMult, info.maxESum, info.maxMetric);
                maxPoint->SetMarkerStyle(29);
                maxPoint->SetMarkerSize(2);
                maxPoint->SetMarkerColor(kGreen+2);
                maxPoint->SetName(Form("maxPoint_R_%.1f", R_val));
                maxPoint->Draw("P SAME");
                
                std::string plotPath = outputDir + Form("/3D_R_%.1f.png", R_val);
                canvas->SaveAs(plotPath.c_str());
                canvas->Write();
                
                output = "Saved: " + plotPath + " (" + std::to_string(mult.size()) + " points)\n";
                output += "  Min metric = " + std::to_string(info.minMetric) + 
                          ", Max metric = " + std::to_string(info.maxMetric) + "\n";
                std::cout << output;
                summaryFile << output;
                
                plotIndex++;
            }
        }
        
        rootFile->Close();
        delete rootFile;
        
        output = "\nROOT file saved: " + rootfile_path + "\n";
        std::cout << output;
        summaryFile << output;
    }
    
    summaryFile.close();
    
    output = "\nOutput files created:\n";
    output += "  1. " + outfile1_path + "\n";
    output += "  2. " + outfile2_path + "\n";
    output += "  3. " + summary_path + "\n";
    output += "  4. " + rootfile_path + "\n";
    output += separator + "\n";
    std::cout << output;
}
