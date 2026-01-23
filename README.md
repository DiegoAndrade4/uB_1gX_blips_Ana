
# uB_1gX_blips_Ana

Macros for signal event selection and optimization of hadronic boundary for 0n/Nn split.  BNB Nu and EXT unb. 

Macros for signal/sideband event selection and optimization of hadronic boundary for 0n/Nn split.



## Description and usage
At directory `uB_1gX_blips_Ana/scr`, use the following script to compile: 

* `source compile.sh anamacro_1gX_blips_signal.cpp`
* `source compile.sh metric_optimization_0nNn_EXT.cpp`

Compiled files will be at directory`uB_1gX_blips_Ana/build`. Then, to run such executables go to `uB_1gX_blips_Ana/scripts`, which contains bash scripts.  


* `source run_anamacro_1gX_blips_signal_R70-115_optInputs.sh`
* `source run_metric_optimization_0nNn_EXT_R70-115_optInputs.sh`


Move output directories (or update paths) `EXT_CRT_RXX_M10_E8` and `SIGNAL_CRT_RXX_M10_E8` to directory `Hadronic_Boundary_Optimization_2DHist`.  

* `source run_analyze2DHist.sh`

```
#EXT Files 
cd /RunAnaOutOptim_EXT
cat output_analyze2DHist_R70_regA.txt output_analyze2DHist_R75_regA.txt output_analyze2DHist_R80_regA.txt output_analyze2DHist_R85_regA.txt output_analyze2DHist_R90_regA.txt output_analyze2DHist_R95_regA.txt output_analyze2DHist_R100_regA.txt output_analyze2DHist_R105_regA.txt output_analyze2DHist_R110_regA.txt output_analyze2DHist_R115_regA.txt > EXT_param_R_70-115_E_0-30_N_0-30_2DHist.txt

#EXT Files
cd ../RunAnaOutOptim_BNB
cat output_analyze2DHist_R70_regA.txt output_analyze2DHist_R75_regA.txt output_analyze2DHist_R80_regA.txt output_analyze2DHist_R85_regA.txt output_analyze2DHist_R90_regA.txt output_analyze2DHist_R95_regA.txt output_analyze2DHist_R100_regA.txt output_analyze2DHist_R105_regA.txt output_analyze2DHist_R110_regA.txt output_analyze2DHist_R115_regA.txt > BNBnuoverlay_param_R_70-115_E_0-30_N_0-30_2DHist.txt
```



Merged txt files are the inputs of the following macro which will calculate the metric and retrieve the min/max metric values: 
```
root -l 'CalculateMetric_wPlots_nxm_Man.C("RunAnaOutOptim_BNB/BNBnuoverlay_param_R_70-115_E_0-30_N_0-30_2DHist.txt","RunAnaOutOptim_EXT/EXT_param_R_70-115_E_0-30_N_0-30_2DHist.txt")'
```

An output directory will be created containing a summary file, figures and a root file with plots. 
