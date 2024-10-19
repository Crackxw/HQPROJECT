# Microsoft Developer Studio Project File - Name="XGateServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=XGateServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XGateServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XGateServer.mak" CFG="XGateServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XGateServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "XGateServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/XGateServer", PWCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XGateServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "FD_SETSIZE#6000" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "XGateServer - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "FD_SETSIZE#6000" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib ntwdblib.lib /nologo /subsystem:console /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "XGateServer - Win32 Release"
# Name "XGateServer - Win32 Debug"
# Begin Group "PacketCmd Define Header File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\XPacketCmd.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\XGateServer.cpp
# End Source File
# Begin Source File

SOURCE=.\XGateServer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XControlServer.cpp
# End Source File
# Begin Source File

SOURCE=.\XDBProc.cpp
# End Source File
# Begin Source File

SOURCE=.\XGameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\XMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\XPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\XPlayerMgr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\XControlServer.h
# End Source File
# Begin Source File

SOURCE=.\XDBProc.h
# End Source File
# Begin Source File

SOURCE=.\XGameServer.h
# End Source File
# Begin Source File

SOURCE=.\XMonitor.h
# End Source File
# Begin Source File

SOURCE=.\XPlayer.h
# End Source File
# Begin Source File

SOURCE=.\XPlayerMgr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "XCommon Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXDBController.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\jxdef.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXGateSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXObject.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketQueue.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocketMgr.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXWorkManager.h
# End Source File
# End Group
# Begin Group "XCommon Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXDBController.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXGateSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocketMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXWorkManager.cpp
# End Source File
# End Group
# End Target
# End Project
