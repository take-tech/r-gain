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

## インストール方法

GitHub Releases から、お使いのOSに合ったインストーラーをダウンロードしてください。

- macOS: `R-Gain-<version>-macOS.pkg`
- Windows: `R-Gain-<version>-Windows-x64-Setup.exe`

### macOS

1. `R-Gain-<version>-macOS.pkg` をダウンロードします。
2. ダウンロードした `.pkg` を開き、画面の案内に沿ってインストールします。
3. インストール後、DAWを起動し直します。
4. DAWのプラグイン一覧で `R-Gain` を探します。

macOS版では、VST3とAUがインストールされます。

インストール先:

```text
/Library/Audio/Plug-Ins/VST3/R-Gain.vst3
/Library/Audio/Plug-Ins/Components/R-Gain.component
```

### Windows

1. `R-Gain-<version>-Windows-x64-Setup.exe` をダウンロードします。
2. ダウンロードした `.exe` を開き、画面の案内に沿ってインストールします。
3. インストール後、DAWを起動し直します。
4. DAWのプラグイン一覧で `R-Gain` を探します。

Windows版では、VST3がインストールされます。

インストール先:

```text
C:\Program Files\Common Files\VST3\R-Gain.vst3
```

### DAWに表示されない場合

- DAWを再起動してください。
- DAWのプラグイン再スキャンを実行してください。
- Windowsの場合、DAWがVST3プラグインを読み込む設定になっているか確認してください。
- macOSの場合、AU対応DAWではAU、VST3対応DAWではVST3として表示されます。

## 使用方法

R-Gainは、音量調整用のシンプルなゲインプラグインです。トラック、バス、マスターなど、音量を少し調整したい場所に挿して使います。

### 基本的な使い方

1. DAWのオーディオトラック、インストゥルメントトラック、バス、またはマスターに `R-Gain` を挿します。
2. `Gain` ノブを動かして音量を調整します。
3. 右側の `Output` メーターで、プラグイン通過後の出力レベルを確認します。

### Gain

`Gain` は入力された音全体の音量を調整します。

- 範囲: `-60 dB` から `+24 dB`
- 初期値: `0 dB`
- `0 dB` のとき、音量は変わりません。
- プラス方向に上げると音量が大きくなります。
- マイナス方向に下げると音量が小さくなります。
- `-60 dB` 付近では、ほぼ無音になります。

### Output Meter

右側の `L` / `R` メーターは、Gain処理後の出力レベルを表示します。

- `L`: 左チャンネルの出力レベル
- `R`: 右チャンネルの出力レベル
- モノラル入力の場合は、左右に同じレベルが表示されます。

v0.1のメーターは簡易ピークメーターです。RMS、ラウドネス、クリップ表示にはまだ対応していません。

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

## Installers

GitHub Actions can build downloadable installers for Releases.

- macOS: `R-Gain-<version>-macOS.pkg`
- Windows: `R-Gain-<version>-Windows-x64-Setup.exe`

To create release installers, push a version tag:

```sh
git tag v0.1.0
git push origin v0.1.0
```

The generated installers are attached to the GitHub Release. The current setup is unsigned. For public distribution, macOS Developer ID signing/notarization and Windows code signing should be added before wider release.

## Development Status

R-Gain is currently at v0.1. The priority is a stable, lightweight gain utility that builds cleanly, loads in a DAW, applies gain correctly, and shows basic output metering.
