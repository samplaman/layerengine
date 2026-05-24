#!/bin/bash
# Universal Installer for LayerEngine on Linux
set -e

echo "======================================"
echo "Installing LayerEngine..."
echo "======================================"

install_standalone="y"
read -r -p "Do you want to install the standalone application? [Y/n] " response
if [[ "$response" =~ ^[Nn] ]]; then
  install_standalone="n"
fi

# Determine install directories
if [ "$EUID" -ne 0 ]; then
  # Local user install
  echo "Installing for current user only..."
  INSTALL_BIN="$HOME/.local/bin"
  INSTALL_VST3="$HOME/.vst3"
  INSTALL_DESKTOP="$HOME/.local/share/applications"
  INSTALL_ICON="$HOME/.local/share/icons/hicolor/scalable/apps"
else
  # System-wide install
  echo "Installing system-wide..."
  INSTALL_BIN="/usr/local/bin"
  INSTALL_VST3="/usr/lib/vst3"
  INSTALL_DESKTOP="/usr/share/applications"
  INSTALL_ICON="/usr/share/pixmaps"
fi

mkdir -p "$INSTALL_VST3"
if [ "$install_standalone" = "y" ]; then
  mkdir -p "$INSTALL_BIN"
  mkdir -p "$INSTALL_DESKTOP"
  mkdir -p "$INSTALL_ICON"
fi

if [ "$install_standalone" = "y" ]; then
  # Copy Standalone
  if [ -f "LayerEngine" ]; then
    cp "LayerEngine" "$INSTALL_BIN/"
    chmod +x "$INSTALL_BIN/LayerEngine"
    echo "✓ Standalone application installed to: $INSTALL_BIN/LayerEngine"
  else
    echo "⚠ Standalone application not found in current folder."
  fi

  # Copy Icon
  if [ -f "logo.png" ]; then
    cp "logo.png" "$INSTALL_ICON/layerengine.png"
    echo "✓ Application icon installed."
  fi

  # Create Desktop Entry
  cat <<EOF > "$INSTALL_DESKTOP/layerengine.desktop"
[Desktop Entry]
Version=1.0
Type=Application
Name=LayerEngine
Comment=Quad-Layer Granular Synthesizer
Exec=LayerEngine
Icon=layerengine
Terminal=false
Categories=AudioVideo;Audio;Synthesis;
EOF
  chmod +x "$INSTALL_DESKTOP/layerengine.desktop"
  echo "✓ Desktop launcher created."
else
  echo "ℹ Skipping standalone application installation."
fi

# Copy VST3
if [ -d "LayerEngine.vst3" ]; then
  rm -rf "$INSTALL_VST3/LayerEngine.vst3"
  cp -r "LayerEngine.vst3" "$INSTALL_VST3/"
  echo "✓ VST3 plugin installed to: $INSTALL_VST3/LayerEngine.vst3"
else
  echo "⚠ VST3 plugin not found in current folder."
fi

echo "======================================"
echo "LayerEngine installation complete!"
echo "======================================"
