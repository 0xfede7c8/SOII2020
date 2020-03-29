#!/bin/bash

# Create build directories if not already built
mkdir -p build/client
mkdir -p build/server

# Build the system using the makefile
cd src
make
cd ..

# Run the system
./build/client/usb_writer
