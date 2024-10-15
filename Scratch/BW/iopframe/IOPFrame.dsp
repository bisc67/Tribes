# Microsoft Developer Studio Project File - Name="IOPFrame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=IOPFrame - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IOPFrame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IOPFrame.mak" CFG="IOPFrame - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IOPFrame - Win32 PS2 DevKit Release IOP" (based on "Win32 (x86) Console Application")
!MESSAGE "IOPFrame - Win32 PS2 DevKit Debug IOP" (based on "Win32 (x86) Console Application")
!MESSAGE "IOPFrame - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Scratch/BW/IOPFrame", CEPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Release IOP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-IOP-Release"
# PROP BASE Intermediate_Dir "PS2-IOP-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-IOP-Release"
# PROP Intermediate_Dir "PS2-IOP-Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /O2 /D "TARGET_PS2_IOP" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "..\x_files" /D "TARGET_PS2_IOP" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-IOP-Release\IOPFrame.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN
# ADD LINK32 /machine:IX86 /out:"PS2-IOP-Release\IOPFrame.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug IOP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-IOP-Debug"
# PROP BASE Intermediate_Dir "PS2-IOP-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-IOP-Debug"
# PROP Intermediate_Dir "PS2-IOP-Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /Zi /Od /D "TARGET_PS2_IOP" /D "VENDOR_SN" /FD
# ADD CPP /Zi /Od /I "..\x_files" /D "TARGET_PS2_IOP" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-IOP-Debug\IOPFrame.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN
# ADD LINK32 /incremental:no /machine:IX86 /out:"PS2-IOP-Debug\IOPFrame.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IOPFrame___Win32_PS2_DevKit_Debug"
# PROP BASE Intermediate_Dir "IOPFrame___Win32_PS2_DevKit_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-Devkit-Debug"
# PROP Intermediate_Dir "PS2-Devkit-Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /Zi /Od /I "..\x_files" /D "TARGET_PS2_IOP" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "..\x_files" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /incremental:no /machine:IX86 /out:"PS2-IOP-Debug\IOPFrame.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN
# ADD LINK32 /incremental:no /machine:IX86 /out:"PS2-IOP-Debug\IOPFrame.irx" /D:TARGET_PS2_DEV /D:VENDOR_SN
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "IOPFrame - Win32 PS2 DevKit Release IOP"
# Name "IOPFrame - Win32 PS2 DevKit Debug IOP"
# Name "IOPFrame - Win32 PS2 DevKit Debug"
# Begin Source File

SOURCE=.\iop_startup.cpp

!IF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Release IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iopaud_main.cpp

!IF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Release IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iopaud_main.hpp

!IF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Release IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iopaud_threads.cpp

!IF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Release IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iopaud_Transfer.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp

!IF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Release IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug IOP"

!ELSEIF  "$(CFG)" == "IOPFrame - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pager.cpp
# End Source File
# Begin Source File

SOURCE=.\pager.hpp
# End Source File
# Begin Source File

SOURCE=.\SimplePage.cpp
# End Source File
# Begin Source File

SOURCE=.\SimplePage.hpp
# End Source File
# Begin Source File

SOURCE=.\virtualmap.cpp
# End Source File
# Begin Source File

SOURCE=.\virtualmap.hpp
# End Source File
# End Target
# End Project
