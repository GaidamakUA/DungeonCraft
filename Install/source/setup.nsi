# Dungeon Craft installation script
#
# Used with NullSoft Installation System (NSIS)
# Created with NSIS version 2.15
# http://nsis.sourceforge.net

Name DungeonCraft

# application version
!define APP_VERSION_TEXT "0.914"
!define APP_VERSION 0.9.1.4

!define APP_NAME DungeonCraft

# This variable points to your local DC installation folder
# and is used as the source from which files are pulled
# into the compressed installation file.
!define APP_SRC "D:\UAF"

# source folders for the various file types
!define SRC_DEFAULT_ART ${APP_SRC}\DefaultArt
!define DST_DEFAULT_ART $INSTDIR\DefaultArt

!define SRC_GPDL ${APP_SRC}\src\GPDL
!define DST_GPDL $INSTDIR\GPDL

!define SRC_HELP ${APP_SRC}\help
!define DST_HELP $INSTDIR\help

# source images used in the installation
!define APP_ICON "${APP_SRC}\src\UAFWinEd\res\UAFWinEd.ico"
!define APP_SPLASH_IMAGE "${APP_SRC}\src\Launch\res\uaf_banner.bmp"


# filenames for the resulting install and uninstall executables
!define UNINSTALL_OUT_FILE DC_uninstall_0914.exe
!define INSTALL_OUT_FILE DC_install_0914.exe

!define REGKEY "SOFTWARE\$(^Name)"
!define VERSION ${APP_VERSION_TEXT}
!define COMPANY ""
!define URL http:\\uaf.sourceforge.net

# maximum compression
SetCompressor /FINAL /SOLID LZMA
#SetCompressorDictSize 31
SetDatablockOptimize on

# Included files
!include Sections.nsh

# Reserved Files
ReserveFile "${NSISDIR}\Plugins\StartMenu.dll"
ReserveFile "${NSISDIR}\Plugins\AdvSplash.dll"

# Variables
Var StartMenuGroup

# Installer pages
Page directory
Page custom StartMenuGroupSelect "" ": Start Menu Folder"
Page instfiles

# Installer attributes
OutFile ${INSTALL_OUT_FILE}
InstallDir $PROGRAMFILES\${APP_NAME}
AllowRootDirInstall true
ShowInstDetails hide
ShowUninstDetails hide
CRCCheck on
XPStyle on
Icon "${APP_ICON}"
AutoCloseWindow false
VIProductVersion ${APP_VERSION}
VIAddVersionKey ProductName ${APP_NAME}
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey CompanyWebsite "${URL}"
VIAddVersionKey Comments "Long Live Gold Box Gaming"
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey FileDescription "${APP_NAME} Installer"
VIAddVersionKey LegalCopyright "Licensed under GPL"
InstallDirRegKey HKLM "${REGKEY}" Path
UninstallIcon "${APP_ICON}"


# Installer sections
Section -Main SEC0000
    
    # size needed for install of all files, in Kilobytes
    AddSize 19360
    
    # always install these files, regardless of timestamp
    SetOverwrite on
    
    Call BackupExistingFiles
    
    Call InstallAppFiles
    Call InstallConfigFiles   
    Call InstallHelpFiles 
    Call InstallGPDLFiles
    
    Call InstallDefaultArtFiles
#    Call InstallRes800Files
#    Call InstallRes1024Files
    
    Call InstallSoundFiles
    
    Call UpdateConfigFile
    Call UpdateRegistry
            
    Call InstallStartMenuShortcuts
    
    WriteRegStr HKLM "${REGKEY}\Components" Main 1
SectionEnd

Section -post SEC0001
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
    WriteRegStr HKLM "${REGKEY}" StartMenuGroup $StartMenuGroup
    WriteUninstaller $INSTDIR\${UNINSTALL_OUT_FILE}
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "$(^Name)"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" URLInfoAbout "${URL}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon $INSTDIR\${UNINSTALL_OUT_FILE}
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString $INSTDIR\${UNINSTALL_OUT_FILE}
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" RegOwner "cocoaspud@hotmail.com"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" HelpLink "${URL}"
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd

# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKLM "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    GoTo done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend

# Uninstaller sections
Section /o un.Main UNSEC0000  
    Call un.InstallConfigFiles    
    Call un.InstallAppFiles
    Call un.InstallHelpFiles
    Call un.InstallGPDLFiles

#    Call un.InstallRes800Files
#    Call un.InstallRes1024Files
    Call un.InstallSoundFiles
    Call un.InstallDefaultArtFiles


    DeleteRegValue HKLM "${REGKEY}\Components" Main
    
    Call un.CleanRegistry
    Call un.RemoveMainDir
SectionEnd

Section un.post UNSEC0001
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"    
    Delete /REBOOTOK $INSTDIR\${UNINSTALL_OUT_FILE}
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKLM "${REGKEY}\Components"
    DeleteRegKey /IfEmpty HKLM "${REGKEY}"
    
    Call un.InstallStartMenuShortcuts
    
SectionEnd

# Installer functions
Function StartMenuGroupSelect
    Push $R1
    StartMenu::Select /autoadd /text "Select the Start Menu folder in which you would like to create the program's shortcuts:" /lastused $StartMenuGroup ${APP_NAME}
    Pop $R1
    StrCmp $R1 success success
    StrCmp $R1 cancel done
    MessageBox MB_OK $R1
    Goto done
success:
    Pop $StartMenuGroup
done:
    Pop $R1
FunctionEnd

Function .onInit
    # allow only one instance of the installer to run
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "DCInstallerMutex") i .r1 ?e'
    Pop $R0 
    StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
    Abort

    InitPluginsDir
    Push $R1
    File /oname=$PLUGINSDIR\spltmp.bmp "${APP_SPLASH_IMAGE}"
    advsplash::show 1000 1000 1000 -1 $PLUGINSDIR\spltmp
    Pop $R1
    Pop $R1
FunctionEnd

# Uninstaller functions
Function un.onInit
    MessageBox MB_YESNO "Uninstall ${APP_NAME}?" IDYES yes IDNO no
    yes:
        ReadRegStr $INSTDIR HKLM "${REGKEY}" Path
        ReadRegStr $StartMenuGroup HKLM "${REGKEY}" StartMenuGroup
        !insertmacro SELECT_UNSECTION Main ${UNSEC0000}
        Goto finish
    no:
        Abort
        Goto finish
        
    finish:
    
FunctionEnd

Function UpdateConfigFile
    Push "$INSTDIR\config.txt" ; file to modify
    Push "Install_Dir" ; string that a line must begin with
    Push "Install_Dir = $INSTDIR" ; string to replace whole line with
    Call ReplaceLineStr        

    Push "$INSTDIR\config.txt" ; file to modify
    Push "Default_Resources" ; string that a line must begin with
    Push "Default_Resources = $INSTDIR\DefaultArt" ; string to replace whole line with
    Call ReplaceLineStr        

    Push "$INSTDIR\config.txt" ; file to modify
    Push "CommonCharFolder" ; string that a line must begin with
    Push "CommonCharFolder = $INSTDIR\CharSave" ; string to replace whole line with
    Call ReplaceLineStr        

    Push "$INSTDIR\config.txt" ; file to modify
    Push "Game_Exec" ; string that a line must begin with
    Push "Game_Exec = $INSTDIR\UAFWin\UAFWin.exe" ; string to replace whole line with
    Call ReplaceLineStr       
FunctionEnd

Function UpdateRegistry
    WriteRegStr HKLM "Software\UAFEditor" "InstallDir" "$INSTDIR"   
    # force 640x480 mode in editor 
    WriteRegDWORD HKLM "Software\UAFEditor" "ScreenRes" 0
    # let the operating system default the initial editor window position
    DeleteRegValue HKLM "Software\UAFEditor" "WindowPositions"
FunctionEnd

Function un.CleanRegistry
    DeleteRegKey HKLM "Software\UAFEditor"
    DeleteRegKey HKCU "Software\UAF Editor"
FunctionEnd

Function ReplaceLineStr
 Exch $R0 ; string to replace that whole line with
 Exch
 Exch $R1 ; string that line should start with
 Exch
 Exch 2
 Exch $R2 ; file
 Push $R3 ; file handle
 Push $R4 ; temp file
 Push $R5 ; temp file handle
 Push $R6 ; global
 Push $R7 ; input string length
 Push $R8 ; line string length
 Push $R9 ; global
 
  StrLen $R7 $R1
 
  GetTempFileName $R4
 
  FileOpen $R5 $R4 w
  FileOpen $R3 $R2 r
 
  ReadLoop:
  ClearErrors
   FileRead $R3 $R6
    IfErrors Done
 
   StrLen $R8 $R6
   StrCpy $R9 $R6 $R7 -$R8
   StrCmp $R9 $R1 0 +3
 
    FileWrite $R5 "$R0$\r$\n"
    Goto ReadLoop
 
    FileWrite $R5 $R6
    Goto ReadLoop
 
  Done:
 
  FileClose $R3
  FileClose $R5
 
  SetDetailsPrint none
   Delete $R2
   Rename $R4 $R2
  SetDetailsPrint both
 
 Pop $R9
 Pop $R8
 Pop $R7
 Pop $R6
 Pop $R5
 Pop $R4
 Pop $R3
 Pop $R2
 Pop $R1
 Pop $R0
FunctionEnd

Function BackupExistingFiles
    CreateDirectory $INSTDIR\backup    
    CopyFiles /SILENT /FILESONLY $INSTDIR\*.txt $INSTDIR\backup
    CopyFiles /SILENT /FILESONLY $INSTDIR\UAFWin\*.exe $INSTDIR\backup
    CopyFiles /SILENT /FILESONLY $INSTDIR\UAFWinEd\*.exe $INSTDIR\backup    
FunctionEnd

Function InstallAppFiles    
    SetOutPath $INSTDIR\UAFWin    
    File ${APP_SRC}\UAFWin\UAFWin.exe
    
    SetOutPath $INSTDIR\UAFWinEd
    File ${APP_SRC}\UAFWinEd\UAFWinEd.exe
FunctionEnd

Function un.InstallAppFiles
    Delete /REBOOTOK $INSTDIR\UAFWinEd\UAFWinEd.exe
    Delete /REBOOTOK $INSTDIR\UAFWinEd\UafErr_Edit.txt
    Delete /REBOOTOK $INSTDIR\UAFWin\UAFWin.exe
    Delete /REBOOTOK $INSTDIR\UAFWin\UafErr_Eng.txt   
    
    RmDir /r /REBOOTOK $INSTDIR\backup
    RmDir /REBOOTOK $INSTDIR\UAFWinEd
    RmDir /REBOOTOK $INSTDIR\UAFWin
    
FunctionEnd

Function InstallConfigFiles
    SetOutPath $INSTDIR
    
    File ${APP_SRC}\ability.txt
    File ${APP_SRC}\baseclass.txt
    File ${APP_SRC}\bass.dll
    File ${APP_SRC}\classes.txt
    File ${APP_SRC}\config.txt
    File ${APP_SRC}\items.txt
    File ${APP_SRC}\monsters.txt
    File ${APP_SRC}\races.txt
    File ${APP_SRC}\skills.txt
    File ${APP_SRC}\SpellGroups.txt
    File ${APP_SRC}\spells.txt
    File ${APP_SRC}\traits.txt
    File ${APP_SRC}\ReadMe.txt         
FunctionEnd

Function un.InstallConfigFiles
    Delete /REBOOTOK $INSTDIR\ReadMe.txt
    Delete /REBOOTOK $INSTDIR\traits.txt
    Delete /REBOOTOK $INSTDIR\spells.txt
    Delete /REBOOTOK $INSTDIR\SpellGroups.txt
    Delete /REBOOTOK $INSTDIR\skills.txt
    Delete /REBOOTOK $INSTDIR\races.txt
    Delete /REBOOTOK $INSTDIR\monsters.txt
    Delete /REBOOTOK $INSTDIR\items.txt
    Delete /REBOOTOK $INSTDIR\config.txt
    Delete /REBOOTOK $INSTDIR\classes.txt
    Delete /REBOOTOK $INSTDIR\bass.dll
    Delete /REBOOTOK $INSTDIR\baseclass.txt
    Delete /REBOOTOK $INSTDIR\ability.txt
FunctionEnd

Function InstallHelpFiles
    SetOutPath ${DST_HELP}
    
    File ${SRC_HELP}\ChangeHistory.txt
    File ${SRC_HELP}\UAForever.hlp
    File ${SRC_HELP}\UAForever.cnt
FunctionEnd

Function un.InstallHelpFiles    
    Delete /REBOOTOK ${DST_HELP}\ChangeHistory.txt
    Delete /REBOOTOK ${DST_HELP}\UAForever.hlp
    Delete /REBOOTOK ${DST_HELP}\UAForever.cnt
    # this file is created when the help file is opened the first time
    Delete /REBOOTOK ${DST_HELP}\UAForever.gid
    RmDir /REBOOTOK ${DST_HELP}
FunctionEnd

Function InstallGPDLFiles
    SetOutPath ${DST_GPDL}
    
    File ${SRC_GPDL}\GPDLcomp.exe
    File ${SRC_GPDL}\language.txt    
    File ${SRC_GPDL}\README.txt
    File ${SRC_GPDL}\talk.txt
FunctionEnd

Function un.InstallGPDLFiles    
    Delete /REBOOTOK ${DST_GPDL}\GPDLcomp.exe
    Delete /REBOOTOK ${DST_GPDL}\language.txt    
    Delete /REBOOTOK ${DST_GPDL}\README.txt
    Delete /REBOOTOK ${DST_GPDL}\talk.txt
    RmDir /REBOOTOK ${DST_GPDL}
FunctionEnd

Function InstallSoundFiles
    SetOutPath ${DST_DEFAULT_ART}
    
    File ${SRC_DEFAULT_ART}\bump.wav
    File ${SRC_DEFAULT_ART}\cast.wav
    File ${SRC_DEFAULT_ART}\CharDeath.wav
    File ${SRC_DEFAULT_ART}\launch.wav
    File ${SRC_DEFAULT_ART}\CharMove.wav
    File ${SRC_DEFAULT_ART}\death.wav
    File ${SRC_DEFAULT_ART}\hit.wav
    File ${SRC_DEFAULT_ART}\intro.mp3
    File ${SRC_DEFAULT_ART}\miss.wav
    File ${SRC_DEFAULT_ART}\step.wav    
FunctionEnd

Function un.InstallSoundFiles    
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bump.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cast.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\CharDeath.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\launch.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\CharMove.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\death.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\hit.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\intro.mp3
    Delete /REBOOTOK ${DST_DEFAULT_ART}\miss.wav
    Delete /REBOOTOK ${DST_DEFAULT_ART}\step.wav    
FunctionEnd

Function InstallDefaultArtFiles
    SetOutPath ${DST_DEFAULT_ART}
    
    File ${SRC_DEFAULT_ART}\config.txt

    File ${SRC_DEFAULT_ART}\AreaViewZoneArt.png
    File ${SRC_DEFAULT_ART}\AreaViewArt.png
    File ${SRC_DEFAULT_ART}\Defcd.png
    File ${SRC_DEFAULT_ART}\Defcdi.png
    File ${SRC_DEFAULT_ART}\Defcp.png
    File ${SRC_DEFAULT_ART}\Defib.png
    File ${SRC_DEFAULT_ART}\Mapart.png
    File ${SRC_DEFAULT_ART}\Title.png

    File ${SRC_DEFAULT_ART}\bd_Background1.png
    File ${SRC_DEFAULT_ART}\bd_Background2.png
    File ${SRC_DEFAULT_ART}\bd_Background3.png
    File ${SRC_DEFAULT_ART}\bd_Background4.png
    File ${SRC_DEFAULT_ART}\bd_Background4.png
    File ${SRC_DEFAULT_ART}\bd_blue-brick.png
    File ${SRC_DEFAULT_ART}\bd_sewer.png

    File ${SRC_DEFAULT_ART}\bo_Defvb.png
    File ${SRC_DEFAULT_ART}\bo_Deffr.png
    File ${SRC_DEFAULT_ART}\bo_DefHb.png
    File ${SRC_DEFAULT_ART}\bp_BPic1.png
    File ${SRC_DEFAULT_ART}\bp_village.png

    File ${SRC_DEFAULT_ART}\cm_AirElemental.png
    File ${SRC_DEFAULT_ART}\cm_Beholder.png
    File ${SRC_DEFAULT_ART}\cm_BlackPudding.png
    File ${SRC_DEFAULT_ART}\cm_BlueDragon.png
    File ${SRC_DEFAULT_ART}\cm_BoringBeetle.png
    File ${SRC_DEFAULT_ART}\cm_BugBear.png
    File ${SRC_DEFAULT_ART}\cm_Bulette.png
    File ${SRC_DEFAULT_ART}\cm_CloudGiant.png
    File ${SRC_DEFAULT_ART}\cm_DefMI.png
    File ${SRC_DEFAULT_ART}\cm_DisplacerBeast.png
    File ${SRC_DEFAULT_ART}\cm_EarthElemental.png
    File ${SRC_DEFAULT_ART}\cm_EnormousSpider.png
    File ${SRC_DEFAULT_ART}\cm_Ettin.png
    File ${SRC_DEFAULT_ART}\cm_FireElemental.png
    File ${SRC_DEFAULT_ART}\cm_FireGiant.png
    File ${SRC_DEFAULT_ART}\cm_FrostGiant.png
    File ${SRC_DEFAULT_ART}\cm_GelatinousCube.png
    File ${SRC_DEFAULT_ART}\cm_GiantRat.png
    File ${SRC_DEFAULT_ART}\cm_Gnoll.png
    File ${SRC_DEFAULT_ART}\cm_Goblin.png
    File ${SRC_DEFAULT_ART}\cm_HillGiant.png
    File ${SRC_DEFAULT_ART}\cm_Hobgoblin.png
    File ${SRC_DEFAULT_ART}\cm_HobgoblinChief.png
    File ${SRC_DEFAULT_ART}\cm_IronGolem.png
    File ${SRC_DEFAULT_ART}\cm_Kobold.png
    File ${SRC_DEFAULT_ART}\cm_LizardMan.png
    File ${SRC_DEFAULT_ART}\cm_LizardManKing.png
    File ${SRC_DEFAULT_ART}\cm_Minotaur.png
    File ${SRC_DEFAULT_ART}\cm_mobat.png
    File ${SRC_DEFAULT_ART}\cm_Mummy.png
    File ${SRC_DEFAULT_ART}\cm_Ogre.png
    File ${SRC_DEFAULT_ART}\cm_Orc.png
    File ${SRC_DEFAULT_ART}\cm_OrcChieftain.png
    File ${SRC_DEFAULT_ART}\cm_PurpleWorm.png
    File ${SRC_DEFAULT_ART}\cm_Skeleton.png
    File ${SRC_DEFAULT_ART}\cm_spider.png
    File ${SRC_DEFAULT_ART}\cm_StoneGiant.png
    File ${SRC_DEFAULT_ART}\cm_Troll.png
    File ${SRC_DEFAULT_ART}\cm_Vampire.png
    File ${SRC_DEFAULT_ART}\cm_WaterElemental.png
    File ${SRC_DEFAULT_ART}\cm_WhiteDragon.png
    File ${SRC_DEFAULT_ART}\cm_Worg.png
    File ${SRC_DEFAULT_ART}\cm_Wraith.png
    File ${SRC_DEFAULT_ART}\cm_zombie.png

    File ${SRC_DEFAULT_ART}\cn_DwarfFight01.png
    File ${SRC_DEFAULT_ART}\cn_DwarfFight02.png
    File ${SRC_DEFAULT_ART}\cn_ElfFight01.png
    File ${SRC_DEFAULT_ART}\cn_ElfMage01.png
    File ${SRC_DEFAULT_ART}\cn_GnomeFight01.png
    File ${SRC_DEFAULT_ART}\cn_GnomeMage01.png
    File ${SRC_DEFAULT_ART}\cn_HalfFight01.png
    File ${SRC_DEFAULT_ART}\cn_HalfFight02.png
    File ${SRC_DEFAULT_ART}\cn_HumCleric01.png
    File ${SRC_DEFAULT_ART}\cn_HumFight01.png
    File ${SRC_DEFAULT_ART}\cn_HumMage01.png
    File ${SRC_DEFAULT_ART}\cn_HumThief01.png
    File ${SRC_DEFAULT_ART}\cn_Icon1.png
    File ${SRC_DEFAULT_ART}\cn_Icon2.png
    File ${SRC_DEFAULT_ART}\cn_Icon3.png
    File ${SRC_DEFAULT_ART}\cn_Icon4.png
    File ${SRC_DEFAULT_ART}\cn_Icon5.png
    File ${SRC_DEFAULT_ART}\cn_Icon6.png

    File ${SRC_DEFAULT_ART}\cu_cursor.png
    File ${SRC_DEFAULT_ART}\cu_defcc.png
    File ${SRC_DEFAULT_ART}\cw_Defcw.png
    File ${SRC_DEFAULT_ART}\cw_sewer.png
    File ${SRC_DEFAULT_ART}\cww_DefCWW.png
    File ${SRC_DEFAULT_ART}\cww_Outdoor.png

    File ${SRC_DEFAULT_ART}\dr_Door2.png
    File ${SRC_DEFAULT_ART}\dr_Door3.png
    File ${SRC_DEFAULT_ART}\it_DefIIRA.png
    File ${SRC_DEFAULT_ART}\it_DefIIRS.png
    File ${SRC_DEFAULT_ART}\it_DefIIRT.png
    File ${SRC_DEFAULT_ART}\overart.png
    File ${SRC_DEFAULT_ART}\ovl_overland.png

    File ${SRC_DEFAULT_ART}\prt_barkeep.png
    File ${SRC_DEFAULT_ART}\prt_CityOfficial.png
    File ${SRC_DEFAULT_ART}\prt_priest.png
    File ${SRC_DEFAULT_ART}\prt_sage.png
    File ${SRC_DEFAULT_ART}\prt_shipjourney.png
    File ${SRC_DEFAULT_ART}\prt_shopkeep1.png
    File ${SRC_DEFAULT_ART}\prt_shopkeep2.png
    File ${SRC_DEFAULT_ART}\prt_smithy.png
    File ${SRC_DEFAULT_ART}\prt_SPic1.png
    File ${SRC_DEFAULT_ART}\prt_SPic2.png
    File ${SRC_DEFAULT_ART}\prt_SPic3.png
    File ${SRC_DEFAULT_ART}\prt_SPic4.png
    File ${SRC_DEFAULT_ART}\prt_SPic5.png
    File ${SRC_DEFAULT_ART}\prt_SPic6.png
    File ${SRC_DEFAULT_ART}\prt_trainer.png
    File ${SRC_DEFAULT_ART}\sp_Deftr.png
    File ${SRC_DEFAULT_ART}\sp_Defca.png

    File ${SRC_DEFAULT_ART}\spl_DefSIR.png
    File ${SRC_DEFAULT_ART}\spl_DefSTC.png
    File ${SRC_DEFAULT_ART}\spl_DefSTH.png
    File ${SRC_DEFAULT_ART}\spl_DefSTL.png
    File ${SRC_DEFAULT_ART}\spl_cast_fireball.png
    File ${SRC_DEFAULT_ART}\spl_cast_lightning.png
    File ${SRC_DEFAULT_ART}\spl_cover_fireball.png
    File ${SRC_DEFAULT_ART}\spl_cover_lightning.png
    File ${SRC_DEFAULT_ART}\spl_missile_fireball.png
    File ${SRC_DEFAULT_ART}\spl_missile_icestorm.png
    File ${SRC_DEFAULT_ART}\spl_missile_lightning.png
    File ${SRC_DEFAULT_ART}\spl_stars_blue.png

    File ${SRC_DEFAULT_ART}\spr_Sprite1.png

    File ${SRC_DEFAULT_ART}\wa_Wall1.png
    File ${SRC_DEFAULT_ART}\wa_Wall2.png
    File ${SRC_DEFAULT_ART}\wa_Wall3.png
    File ${SRC_DEFAULT_ART}\wa_Wall4.png
    File ${SRC_DEFAULT_ART}\wa_Wall5.png
FunctionEnd

Function un.InstallDefaultArtFiles
    Delete /REBOOTOK ${DST_DEFAULT_ART}\config.txt

    Delete /REBOOTOK ${DST_DEFAULT_ART}\AreaViewZoneArt.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\AreaViewArt.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\Defcd.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\Defcdi.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\Defcp.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\Defib.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\Mapart.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\Title.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_Background1.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_Background2.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_Background3.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_Background4.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_Background4.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_blue-brick.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bd_sewer.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\bo_Defvb.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bo_Deffr.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bo_DefHb.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bp_BPic1.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\bp_village.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_AirElemental.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Beholder.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_BlackPudding.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_BlueDragon.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_BoringBeetle.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_BugBear.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Bulette.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_CloudGiant.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_DefMI.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_DisplacerBeast.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_EarthElemental.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_EnormousSpider.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Ettin.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_FireElemental.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_FireGiant.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_FrostGiant.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_GelatinousCube.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_GiantRat.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Gnoll.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Goblin.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_HillGiant.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Hobgoblin.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_HobgoblinChief.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_IronGolem.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Kobold.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_LizardMan.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_LizardManKing.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Minotaur.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_mobat.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Mummy.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Ogre.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Orc.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_OrcChieftain.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_PurpleWorm.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Skeleton.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_spider.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_StoneGiant.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Troll.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Vampire.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_WaterElemental.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_WhiteDragon.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Worg.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_Wraith.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cm_zombie.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_DwarfFight01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_DwarfFight02.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_ElfFight01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_ElfMage01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_GnomeFight01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_GnomeMage01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_HalfFight01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_HalfFight02.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_HumCleric01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_HumFight01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_HumMage01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_HumThief01.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_Icon1.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_Icon2.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_Icon3.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_Icon4.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_Icon5.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cn_Icon6.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\cu_cursor.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cu_defcc.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cw_Defcw.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cw_sewer.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cww_DefCWW.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\cww_Outdoor.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\dr_Door2.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\dr_Door3.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\it_DefIIRA.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\it_DefIIRS.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\it_DefIIRT.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\overart.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\ovl_overland.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_barkeep.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_CityOfficial.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_priest.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_sage.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_shipjourney.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_shopkeep1.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_shopkeep2.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_smithy.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_SPic1.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_SPic2.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_SPic3.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_SPic4.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_SPic5.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_SPic6.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\prt_trainer.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\sp_Deftr.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\sp_Defca.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_DefSIR.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_DefSTC.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_DefSTH.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_DefSTL.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_cast_fireball.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_cast_lightning.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_cover_fireball.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_cover_lightning.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_missile_fireball.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_missile_icestorm.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_missile_lightning.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\spl_stars_blue.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\spr_Sprite1.png

    Delete /REBOOTOK ${DST_DEFAULT_ART}\wa_Wall1.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\wa_Wall2.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\wa_Wall3.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\wa_Wall4.png
    Delete /REBOOTOK ${DST_DEFAULT_ART}\wa_Wall5.png
    
    RmDir /REBOOTOK ${DST_DEFAULT_ART}

FunctionEnd


Function InstallStartMenuShortcuts
    SetOutPath $SMPROGRAMS\$StartMenuGroup    
    
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${APP_NAME} Uninstall.lnk" $INSTDIR\${UNINSTALL_OUT_FILE}    
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${APP_NAME} Help.lnk" ${DST_HELP}\UAForever.hlp
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${APP_NAME} Editor.lnk" $INSTDIR\UAFWinEd\UAFWinEd.exe    
FunctionEnd

Function un.InstallStartMenuShortcuts    
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${APP_NAME} Editor.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${APP_NAME} Help.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${APP_NAME} Uninstall.lnk"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
FunctionEnd

Function un.RemoveMainDir
    RmDir /REBOOTOK $INSTDIR
FunctionEnd
