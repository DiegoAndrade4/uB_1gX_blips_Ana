#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <source.cpp> [target_name]"
    exit 1
fi

SOURCE=$1
BUILD_DIR="../build"

# If target not provided, use source filename without extension
if [ $# -eq 2 ]; then
    TARGET=$2
else
    TARGET=$(basename "$SOURCE" .cpp)
fi

mkdir -p "$BUILD_DIR"

echo "Compiling $SOURCE -> $BUILD_DIR/$TARGET..."
g++ "$SOURCE" -o "$BUILD_DIR/$TARGET" $(root-config --cflags --libs)

if [ $? -eq 0 ]; then
    echo "✓ Success: $BUILD_DIR/$TARGET"
else
    echo "✗ Failed"
    exit 1
fi
