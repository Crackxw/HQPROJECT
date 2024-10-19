# Microsoft Developer Studio Project File - Name="AuthServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AuthServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AuthServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AuthServer.mak" CFG="AuthServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AuthServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AuthServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/AuthServer", YVDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AuthServer - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "AuthServer - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Lib/raslib.lib ntwdblib.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /nodefaultlib:"LIBC" /out:"D:\Auth Server\AuthServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AuthServer - Win32 Release"
# Name "AuthServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_AuthDefine.h
# End Source File
# Begin Source File

SOURCE=.\_LoginDefine.h
# End Source File
# Begin Source File

SOURCE=.\_WorkDefine.h
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# End Group
# Begin Group "Kojimodo"

# PROP Default_Filter ""
# Begin Group "IOCP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\kojimodo\IOCP\AcceptSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\kojimodo\IOCP\AcceptSocket.h
# End Source File
# Begin Source File

SOURCE=..\kojimodo\IOCP\ListenSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\kojimodo\IOCP\ListenSocket.h
# End Source File
# End Group
# Begin Group "Utility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\kojimodo\Utility\MSSQL.cpp
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\MSSQL.h
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\Queue.cpp
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\Queue.h
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\Singleton.h
# End Source File
# End Group
# End Group
# Begin Group "GameServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.h
# End Source File
# Begin Source File

SOURCE=.\GameServerMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServerMgr.h
# End Source File
# End Group
# Begin Group "Temp"

# PROP Default_Filter ""
# Begin Group "Authorizer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Authorizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Authorizer.h
# End Source File
# Begin Source File

SOURCE=.\Gash.cpp
# End Source File
# Begin Source File

SOURCE=.\Gash.h
# End Source File
# Begin Source File

SOURCE=.\Ras.cpp
# End Source File
# Begin Source File

SOURCE=.\Ras.h
# End Source File
# Begin Source File

SOURCE=.\Raslib.h
# End Source File
# End Group
# Begin Group "Work"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WorkMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkMgr.h
# End Source File
# Begin Source File

SOURCE=.\WorkQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkQueue.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AccountDBProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountDBProc.h
# End Source File
# Begin Source File

SOURCE=.\AuthProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthProc.h
# End Source File
# End Group
# Begin Group "AuthInfoParser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AuthInfoParser.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthInfoParser.h
# End Source File
# End Group
# Begin Group "GCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\JOConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\JOConfig.h
# End Source File
# End Group
# Begin Group "SCCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SCCommon\JOGersangConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JOGersangConfig.h
# End Source File
# End Group
# Begin Group "China Auth"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Accounting.cpp
# End Source File
# Begin Source File

SOURCE=.\Accounting.h
# End Source File
# Begin Source File

SOURCE=.\AuthCallback.h
# End Source File
# Begin Source File

SOURCE=.\AuthConnectSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthConnectSocket.h
# End Source File
# Begin Source File

SOURCE=.\Authentication.cpp
# End Source File
# Begin Source File

SOURCE=.\Authentication.h
# End Source File
# Begin Source File

SOURCE=.\Authorization.cpp
# End Source File
# Begin Source File

SOURCE=.\Authorization.h
# End Source File
# Begin Source File

SOURCE=.\AuthSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthSocket.h
# End Source File
# Begin Source File

SOURCE=.\CmdQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdQueue.h
# End Source File
# Begin Source File

SOURCE=.\GashClient.cpp
# End Source File
# Begin Source File

SOURCE=.\GashClient.h
# End Source File
# Begin Source File

SOURCE=.\JOChinaAuthServerConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\JOChinaAuthServerConfig.h
# End Source File
# Begin Source File

SOURCE=.\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\LogWriter.h
# End Source File
# Begin Source File

SOURCE=.\ParentCallback.h
# End Source File
# End Group
# End Target
# End Project
