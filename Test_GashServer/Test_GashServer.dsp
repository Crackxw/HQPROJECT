# Microsoft Developer Studio Project File - Name="Test_GashServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Test_GashServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Test_GashServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Test_GashServer.mak" CFG="Test_GashServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Test_GashServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Test_GashServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/Test_GashServer", QXGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Test_GashServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Test_GashServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Test_GashServer - Win32 Release"
# Name "Test_GashServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GashServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GashServerMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Test_GashServer.cpp
# End Source File
# Begin Source File

SOURCE=.\XClient.cpp
# End Source File
# Begin Source File

SOURCE=.\XClientMgr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GashServer.h
# End Source File
# Begin Source File

SOURCE=.\GashServerMgr.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XClient.h
# End Source File
# Begin Source File

SOURCE=.\XClientMgr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "XCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocketMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocketMgr.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\jxdef.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXGateSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXGateSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXObject.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketQueue.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocketMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocketMgr.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXWorkManager.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXWorkManager.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\XMonitorCmd.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\XPacketCmd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
