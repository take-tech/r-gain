#define MyAppName "R-Gain"
#define MyAppVersion GetEnv("RGAIN_VERSION")
#if MyAppVersion == ""
#define MyAppVersion "0.2.0"
#endif
#define MyAppPublisher "Ranze"
#define MyAppURL "https://github.com/take-tech/r-gain"
#define SourceVst3 GetEnv("RGAIN_VST3_DIR")
#if SourceVst3 == ""
#define SourceVst3 "..\\..\\build\\RGain_artefacts\\Release\\VST3\\R-Gain.vst3"
#endif

[Setup]
AppId={{D7CB9D86-69E0-4D8A-9D3F-8F8B8C1D9485}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={commoncf}\VST3
DisableDirPage=yes
DisableProgramGroupPage=yes
OutputBaseFilename=R-Gain-{#MyAppVersion}-Windows-x64-Setup
OutputDir=..\..\build\packages
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=admin
UninstallDisplayName={#MyAppName}

[Files]
Source: "{#SourceVst3}\*"; DestDir: "{commoncf}\VST3\R-Gain.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

[InstallDelete]
Type: filesandordirs; Name: "{commoncf}\VST3\R-Gain.vst3"
