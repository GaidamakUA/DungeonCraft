# Microsoft Developer Studio Project File - Name="ConvertFRUA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ConvertFRUA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConvertFRUA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConvertFRUA.mak" CFG="ConvertFRUA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ConvertFRUA - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ConvertFRUA - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ConvertFRUA - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "ConvertFRUA - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "ConvertFRUA - Win32 Release"
# Name "ConvertFRUA - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Class.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvertFRUA.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvertFRUA.rc
# End Source File
# Begin Source File

SOURCE=.\Event_AddNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Camp.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Chain.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Combat.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_CombatTreasure.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Encounter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_GainExperience.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_GiveDamage.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_GiveTreasure.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_GuidedTour.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_HealParty.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Journal.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_NPCSays.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Null.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_PassTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Password.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Quest.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_QuestionButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_QuestionList.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_QuestionYesNo.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Random.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_RemoveNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Rest.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Shop.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_SmallTown.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_SpecialItems.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_TakePartyItems.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Tavern.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_TavernTales.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Teleporter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Temple.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_TextStatement.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Tour.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_TrainingHall.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Vault.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_WhoPays.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_WhoTries.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\J.cpp
# End Source File
# Begin Source File

SOURCE=.\Levels.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Miscellaneous.cpp
# End Source File
# Begin Source File

SOURCE=.\Monsters.cpp
# End Source File
# Begin Source File

SOURCE=.\Spells.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StepEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferModule.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConvertFRUA.h
# End Source File
# Begin Source File

SOURCE=.\Events.h
# End Source File
# Begin Source File

SOURCE=.\J.h
# End Source File
# Begin Source File

SOURCE=.\Monster.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Translate.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\cursor2.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
