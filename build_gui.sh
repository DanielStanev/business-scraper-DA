#!/bin/bash

# Build script for Business Scraper GUI using CMake

echo "Building Business Scraper GUI..."

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for required tools
echo "Checking for required tools..."

if ! command_exists cmake; then
    echo "Error: CMake not found."
    exit 1
fi

if ! command_exists make; then
    echo "Error: Make not found."
    exit 1
fi

if ! command_exists g++; then
    echo "Error: g++ compiler not found."
    exit 1
fi

# Check for required libraries
echo "Checking for required libraries..."

if ! pkg-config --exists libcurl; then
    echo "Error: libcurl development package not found."
    exit 1
fi

if ! pkg-config --exists jsoncpp; then
    echo "Error: jsoncpp development package not found."
    exit 1
fi

if ! pkg-config --exists Qt6Core Qt6Widgets; then
    echo "Error: Qt6 development packages not found."
    exit 1
fi

echo "All dependencies found!"

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Change to build directory
cd build

# Configure with CMake
echo "Configuring project with CMake..."
if ! cmake ..; then
    echo "Error: CMake configuration failed!"
    exit 1
fi

# Build the project
echo "Building project..."
if ! make -j$(nproc 2>/dev/null || echo 4); then
    echo "Error: Build failed!"
    exit 1
fi

# Check if GUI executable was created
if [ -f "bin/business_scraper_gui" ]; then
    echo ""
    echo "Build successful!"
    echo ""
    echo "To run the application:"
    echo "  cd build && ./bin/business_scraper_gui"
    echo ""
    echo "Note: Make sure to configure your Google Maps API key in config.ini before running."
else
    echo "Error: GUI executable not found after build!"
    exit 1
fi
