#!/bin/bash

#------------------------------------------------------
MyPATHS="setup_paths.sh"

if [ ! -f "$MyPATHS" ]; then
    echo "ERROR: File $MyPATHS not found"
    exit 1
fi

if source "$MyPATHS"; then
    echo "Successfully sourced $MyPATHS"
else
    echo "ERROR: Failed to source $MyPATHS"
    exit 1
fi
#------------------------------------------------------
