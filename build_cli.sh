#!/bin/bash

# Build script for Business Scraper using g++

echo "Building Business Scraper with g++..."

# Check if required libraries are available
echo "Checking for required libraries..."

if ! pkg-config --exists libcurl; then
    echo "Error: libcurl development package not found. Please install libcurl4-openssl-dev"
    exit 1
fi

if ! pkg-config --exists jsoncpp; then
    echo "Error: jsoncpp development package not found. Please install libjsoncpp-dev"
    exit 1
fi

# Get library flags
CURL_FLAGS=$(pkg-config --cflags --libs libcurl)
JSONCPP_FLAGS=$(pkg-config --cflags --libs jsoncpp)

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

echo "Compiling with g++..."

# Compile with g++
g++ -std=c++17 -Wall -Wextra -Iinclude \
    src/core/Business.cpp \
    src/core/BusinessScraperEngine.cpp \
    src/scrapers/MapScraper.cpp \
    src/scrapers/WebScraper.cpp \
    src/output/Formatter.cpp \
    src/utils/ConfigManager.cpp \
    src/utils/FileUtils.cpp \
    src/main_cli.cpp \
    $CURL_FLAGS $JSONCPP_FLAGS \
    -o build/business_scraper

if [ $? -eq 0 ]; then
    echo "Build successful! Run ./build/business_scraper to start the application."
else
    echo "Build failed. Please check the error messages above."
    exit 1
fi
