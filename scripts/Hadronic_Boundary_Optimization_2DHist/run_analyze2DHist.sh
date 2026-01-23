#!/bin/bash


#########################################
# Function to create and verify directory
#########################################

create_directory() {
    local OUTPUT_DIR="$1"
    
    # Create directory 
    mkdir -p "$OUTPUT_DIR"
    
    # Verify it was created
    if [ -d "$OUTPUT_DIR" ]; then
        echo "Directory '$OUTPUT_DIR' is ready"
        return 0
    else
        echo "ERROR: Failed to create directory '$OUTPUT_DIR'"
        return 1
    fi
}

#--------------------------------------------


#root -l -b -q 'analyze2DHist.C("MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 75, 0, 30, 1, 0, 30, 1, "output_analyze2DHist_R75_regA.txt")'
#root -l -b -q 'analyze2DHist.C("MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip", 75, 0, 30, 1, 0, 30, 1, "output_analyze2DHist_R75_all.txt")'



#BNB Files

create_directory "RunAnaOutOptim_BNB"


root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R70_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 70, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R70_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R75_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 75, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R75_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R80_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 80, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R80_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R85_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 85, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R85_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R90_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 90, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R90_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R95_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 95, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R95_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R100_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 100, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R100_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R105_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 105, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R105_regA.txt")'

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R110_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 110, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R110_regA.txt")' 

root -l -b -q 'analyze2DHist.C("SIGNAL_CRT_R115_M10_E8/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 115, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_BNB/output_analyze2DHist_R115_regA.txt")'

#EXT Files

create_directory "RunAnaOutOptim_EXT"


root -l -b -q 'analyze2DHist.C("EXT_CRT_R70_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R70.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 70, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R70_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R75_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R75.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 75, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R75_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R80_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R80.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 80, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R80_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R85_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R85.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 85, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R85_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R90_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R90.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 90, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R90_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R95_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R95.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 95, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R95_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R100_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R100.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 100, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R100_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R105_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R105.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 105, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R105_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R110_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R110.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 110, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R110_regA.txt")'

root -l -b -q 'analyze2DHist.C("EXT_CRT_R115_M10_E8/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist_Optimization_R115.000_NBlips10.000_SumE8.000_Data.root", "h2D_1gX_BlipMultiplicity_SumEblip_regA", 115, 0, 30, 1, 0, 30, 1, "RunAnaOutOptim_EXT/output_analyze2DHist_R115_regA.txt")'

