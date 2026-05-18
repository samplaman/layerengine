!define APP_NAME "LayerEngine"
!define COMP_NAME "Eoin O Dowd"
!define VERSION "0.0.1"

Name "${APP_NAME}"
OutFile "LayerEngine-Installer.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
RequestExecutionLevel admin

Page directory
Page instfiles

Section "Install"
    ; Install Standalone Application
    SetOutPath "$INSTDIR"
    File "artifacts\LayerEngine.exe"
    
    ; Create Start Menu Shortcuts
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\LayerEngine.exe"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
    
    ; Install VST3 Plugin
    SetOutPath "$COMMONFILES\VST3"
    File /r "artifacts\LayerEngine.vst3"

    ; Uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"
    
    ; Registry entries for Add/Remove Programs
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
    RMDir /r "$COMMONFILES\VST3\LayerEngine.vst3"
    
    ; Remove Registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
SectionEnd
