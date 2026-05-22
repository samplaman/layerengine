#!/bin/bash
set -e

# Create Debian directory structure
DEB_DIR="linux_deb/LayerEngine-Linux"
mkdir -p "$DEB_DIR/DEBIAN"
mkdir -p "$DEB_DIR/usr/bin"
mkdir -p "$DEB_DIR/usr/lib/vst3"
mkdir -p "$DEB_DIR/usr/share/applications"
mkdir -p "$DEB_DIR/usr/share/pixmaps"

# Write the debian control file
cat <<EOF > "$DEB_DIR/DEBIAN/control"
Package: layerengine
Version: 0.0.1
Architecture: amd64
Maintainer: Eoin O Dowd
Description: LayerEngine Quad-Layer Granular Synthesizer
 Section: sound
 Priority: optional
EOF

# Copy artifacts
if [ -f "artifacts/LayerEngine" ]; then
    cp "artifacts/LayerEngine" "$DEB_DIR/usr/bin/LayerEngine"
    chmod +x "$DEB_DIR/usr/bin/LayerEngine"
fi

if [ -d "artifacts/LayerEngine.vst3" ]; then
    cp -r "artifacts/LayerEngine.vst3" "$DEB_DIR/usr/lib/vst3/"
fi

# Copy logo as application icon
if [ -f "assets/icon.png" ]; then
    cp "assets/icon.png" "$DEB_DIR/usr/share/pixmaps/layerengine.png"
fi

# Create desktop entry for standalone app
cat <<EOF > "$DEB_DIR/usr/share/applications/layerengine.desktop"
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

# Build DEB package
dpkg-deb --build "$DEB_DIR" LayerEngine-Installer.deb

# Clean up
rm -rf linux_deb
echo "Linux DEB package created: LayerEngine-Installer.deb"
