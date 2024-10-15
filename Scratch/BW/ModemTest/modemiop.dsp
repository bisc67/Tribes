# Microsoft Developer Studio Project File - Name="modemiop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=modemiop - Win32 PS2 DevKit Debug IOP
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "modemiop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "modemiop.mak" CFG="modemiop - Win32 PS2 DevKit Debug IOP"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "modemiop - Win32 PS2 DevKit Release IOP" (based on "Win32 (x86) Console Application")
!MESSAGE "modemiop - Win32 PS2 DevKit Debug IOP" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "modemiop - Win32 PS2 DevKit Release IOP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-IOP-Release"
# PROP BASE Intermediate_Dir "PS2-IOP-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-IOP-Release"
# PROP Intermediate_Dir "PS2-IOP-Release"
# PROP Target_Dir ""
# ADD BASE CPP /O2 /D "TARGET_PS2_IOP" /D "VENDOR_SN" /FD
# ADD CPP /O2 /D "TARGET_PS2_IOP" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-IOP-Release\modemiop.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN
# ADD LINK32 /machine:IX86 /out:"PS2-IOP-Release\modemiop.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "modemiop - Win32 PS2 DevKit Debug IOP"

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
# ADD CPP /Zi /Od /D "TARGET_PS2_IOP" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-IOP-Debug\modemiop.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN
# ADD LINK32 inetctl.ilb inet.ilb netcnf.ilb /machine:IX86 /out:"c:\projects\t2\xcore\entropy\ps2\modules\modemiop.irx" /D:TARGET_PS2_IOP /D:VENDOR_SN

!ENDIF 

# Begin Target

# Name "modemiop - Win32 PS2 DevKit Release IOP"
# Name "modemiop - Win32 PS2 DevKit Debug IOP"
# Begin Source File

SOURCE=.\IOPTYPES.H
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\netlayer.c
# End Source File
# Begin Source File

SOURCE=.\netlayer.h
# End Source File
# End Target
# End Project
