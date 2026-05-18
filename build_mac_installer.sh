#!/bin/bash
set -e

# Create a temporary directory for packaging
mkdir -p mac_pkg/root/Library/Audio/Plug-Ins/VST3
mkdir -p mac_pkg/root/Library/Audio/Plug-Ins/Components
mkdir -p mac_pkg/root/Applications

# Copy built artifacts into the root structure
if [ -d "artifacts/LayerEngine.vst3" ]; then
    cp -R "artifacts/LayerEngine.vst3" mac_pkg/root/Library/Audio/Plug-Ins/VST3/
fi

if [ -d "artifacts/LayerEngine.component" ]; then
    cp -R "artifacts/LayerEngine.component" mac_pkg/root/Library/Audio/Plug-Ins/Components/
fi

if [ -d "artifacts/LayerEngine.app" ]; then
    cp -R "artifacts/LayerEngine.app" mac_pkg/root/Applications/
fi

# Build the flat installer package
pkgbuild --identifier com.eoinodowd.layerengine \
         --version 0.0.1 \
         --root mac_pkg/root \
         --install-location / \
         LayerEngine-Installer.pkg

# Clean up
rm -rf mac_pkg
echo "macOS Installer Package created: LayerEngine-Installer.pkg"
