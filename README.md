# R-Gain

R-Gain is a lightweight gain utility plugin designed for simple level control and output metering.

## v0.1 Scope

v0.1 intentionally includes only Gain and an Output Meter. Phase invert, pan, width, mono, preset management, and advanced metering are not implemented yet.

## Features

- Gain control from -60 dB to +24 dB
- Smoothed gain changes to avoid clicks
- Simple L/R output peak meter
- APVTS-based parameter management
- DSP processing separated into `rgain::dsp::GainProcessor`

## Supported Formats

- VST3
- AU on macOS

## Build

Clone with submodules so JUCE is available at `external/juce`.

```sh
git clone --recurse-submodules <repository-url>
cd r-gain
```

If you already cloned without submodules:

```sh
git submodule update --init --recursive
```

### macOS

```sh
cmake -S . -B build
cmake --build build --config Release
```

This builds VST3 and AU.

### Windows

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

This builds VST3.

Generated plugin artifacts are placed in the CMake build output managed by JUCE.

## Development Status

R-Gain is currently at v0.1. The priority is a stable, lightweight gain utility that builds cleanly, loads in a DAW, applies gain correctly, and shows basic output metering.
