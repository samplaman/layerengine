

# Layer Engine

<p align="center">
  <img src="assets/logo.png" alt="Layer Engine Logo" width="300">
</p>

<p align="center">
  <a href="https://github.com/samplaman/layerengine/actions/workflows/build.yml">
    <img src="https://github.com/samplaman/layerengine/actions/workflows/build.yml/badge.svg" alt="Build Status">
  </a>
  <a href="https://github.com/samplaman/layerengine/actions/workflows/release.yml">
    <img src="https://github.com/samplaman/layerengine/actions/workflows/release.yml/badge.svg" alt="Release Status">
  </a>
</p>

---

**Layer Engine** is a high-performance, professional-grade quad-layer granular synthesizer built with C++ and the JUCE framework. Designed for deep sound design and atmospheric textures, Yabba allows users to stack up to four independent granular engines, each with its own modulation, filtering, and spatialization controls.

## 🚀 Key Features

- **Quad-Layer Architecture**: Mix and layer four independent granular synthesis engines simultaneously for complex, evolving soundscapes.
- **Advanced Granular Engine**:
  - Precision control over **Position**, **Grain Size**, **Density**, and **Pitch**.
  - **Randomness/Jitter**: Add organic movement with per-parameter jitter controls for position, pitch, and size.
  - **Scan Speed**: Automatically scan through samples at varying rates.
- **Dynamic Windowing**: Choose between multiple grain window shapes: `Sine`, `Gaussian`, `Hann`, and `Triangular`.
- **Integrated Synthesis Chain**:
  - **ADSR Envelope**: Dedicated envelope per layer for precise amplitude shaping.
  - **State Variable Filter (SVF)**: High-quality TPT filter with Cutoff and Resonance controls per layer.
- **Stereo Imaging**: Integrated **Pan** and **Stereo Spread** controls per layer to create wide, immersive soundscapes.
- **Visual Feedback**: Real-time grain visualization and sample waveform display.
- **Modern UI**: High-contrast, premium dark interface featuring a cinematic mountain background and glassmorphism design for professional studio environments.

## 🛠 Tech Stack

- **Core**: C++17
- **Framework**: JUCE 7
- **DSP**: JUCE DSP Module (TPT Filters, ADSR)
- **Build System**: CMake
- **Cross-Compilation**: LLVM-MinGW (for Linux-to-Windows VST3 deployment)
- **CI/CD**: GitHub Actions for multi-platform automated builds (macOS, Windows, Linux)

## 📦 Installation & Building

### Prerequisites
- **CMake** (3.15 or higher)
- **C++ Compiler** (GCC, Clang, or MSVC)
- **JUCE Dependencies** (Automatically fetched via CMake FetchContent)

### Local Build
```bash
git clone https://github.com/samplaman/layerengine.git
cd layerengine
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
```

### Cross-Compilation (Linux to Windows)
```bash
cmake -B build-win -DCMAKE_TOOLCHAIN_FILE=mingw-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build-win --config Release
```

## 🎹 Usage

1. **Load Samples**: Drag and drop audio files onto any of the four Layer tabs.
2. **Shape Grains**: Use the **Size** and **Density** knobs to define the texture.
3. **Modulate**: Map the **Mod Wheel** to grain position for manual scanning or use **Scan Speed** for automation.
4. **Mix & FX**: Use the dedicated **Mixer** and **FX** tabs to balance the four layers and add final spatial processing.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
*Developed by [Eoin ODowd](https://github.com/eoinodowd)*
