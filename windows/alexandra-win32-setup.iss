#define MyAppName "Alexandra Video Library"
#define MyAppNameShort "Alexandra"
#define MyAppVersion "0.9.5"
#define MyAppPublisher "The Higggs Development"
#define MyAppURL "https://sourceforge.net/projects/alexandra-qt/"
#define MyAppExeName "alexandra.exe"

[Setup]
; The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
AppId={{DBAF1834-C3A1-4884-A639-045750269A5C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL="https://sourceforge.net/projects/alexandra-qt/support/"
AppUpdatesURL="https://sourceforge.net/projects/alexandra-qt/files/latest/download"
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile=..\LICENSE
;InfoBeforeFile=..\README.md
OutputDir=.\
OutputBaseFilename=alexandra-{#MyAppVersion}-win32-setup
SetupIconFile=..\icons\windows\main24.ico
Compression=lzma2/ultra
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "..\src\alexandra.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\lib\MediaInfo.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\icudt52.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\icuin52.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\icuuc52.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\qminimal.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\qoffscreen.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\qt-mini-deploy\qwindows.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppNameShort}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
