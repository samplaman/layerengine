!include "MUI2.nsh"
!include "Sections.nsh"

!define APP_NAME "LayerEngine"
!define COMP_NAME "Eoin O Dowd"
!define VERSION "0.0.1"

Name "${APP_NAME}"
OutFile "LayerEngine-Installer.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
RequestExecutionLevel admin

Var VST3_DIR

; Initialization
Function .onInit
  StrCpy $VST3_DIR "$COMMONFILES\VST3"
FunctionEnd

; Pages
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

; VST3 Directory selection
!define MUI_PAGE_HEADER_TEXT "Choose VST3 Location"
!define MUI_PAGE_HEADER_SUBTEXT "Choose the folder in which to install the VST3 plugin."
!define MUI_DIRECTORYPAGE_TEXT_TOP "The setup will install the VST3 plugin in the following folder.$\r$\n$\r$\nTo install in a different folder, click Browse and select another folder."
!define MUI_DIRECTORYPAGE_VARIABLE $VST3_DIR
!define MUI_PAGE_CUSTOMFUNCTION_PRE VST3DirPre
!insertmacro MUI_PAGE_DIRECTORY

!insertmacro MUI_PAGE_INSTFILES

; Uninstaller Pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"


Section "Standalone Application" SecStandalone
    SetOutPath "$INSTDIR"
    File "artifacts\LayerEngine.exe"
    
    ; Create Start Menu Shortcuts
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\LayerEngine.exe"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

Section "VST3 Plugin" SecVST3
    SetOutPath "$VST3_DIR"
    File /r "artifacts\LayerEngine.vst3"
    
    ; Save VST3 path to registry for uninstaller
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "VST3Dir" "$VST3_DIR"
SectionEnd

; Post install section (always runs) to create uninstaller
Section -Post
    SetOutPath "$INSTDIR"
    WriteUninstaller "$INSTDIR\uninstall.exe"
    
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "Publisher" "${COMP_NAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayVersion" "${VERSION}"
SectionEnd

Section "Uninstall"
    ; Remove Standalone
    Delete "$INSTDIR\LayerEngine.exe"
    Delete "$INSTDIR\uninstall.exe"
    RMDir "$INSTDIR"
    
    ; Remove Shortcuts
    Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
    Delete "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${APP_NAME}"
    
    ; Remove VST3
    ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "VST3Dir"
    StrCmp $0 "" no_vst3
    RMDir /r "$0\LayerEngine.vst3"
  no_vst3:
    
    ; Remove Registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
SectionEnd

; Descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStandalone} "Installs the standalone application."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecVST3} "Installs the VST3 plugin."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function VST3DirPre
  SectionGetFlags ${SecVST3} $0
  IntOp $0 $0 & ${SF_SELECTED}
  StrCmp $0 ${SF_SELECTED} continue_page
  Abort ; Skip page if VST3 not selected
continue_page:
FunctionEnd
