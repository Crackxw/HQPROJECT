# Microsoft Developer Studio Project File - Name="FrontServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=FrontServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FrontServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FrontServer.mak" CFG="FrontServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FrontServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FrontServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/FrontServer", OLDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FrontServer - Win32 Release"

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

!ELSEIF  "$(CFG)" == "FrontServer - Win32 Debug"

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
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"D:\Front Server\FrontServer.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FrontServer - Win32 Release"
# Name "FrontServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_FrontDefine.h
# End Source File
# Begin Source File

SOURCE=.\_FrontGameServerDefine.h
# End Source File
# Begin Source File

SOURCE=.\_FrontUserDefine.h
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

SOURCE=..\kojimodo\Utility\Singleton.h
# End Source File
# End Group
# End Group
# Begin Group "User"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMgr.h
# End Source File
# End Group
# Begin Group "GameInfoParser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameInfoParser.cpp
# End Source File
# Begin Source File

SOURCE=.\GameInfoParser.h
# End Source File
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
# Begin Group "WorldInfo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WorldInfoMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldInfoMgr.h
# End Source File
# End Group
# Begin Group "Global"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WorldInfoParser.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldInfoParser.h
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
# Begin Group "GCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\JOConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\JOConfig.h
# End Source File
# End Group
# End Target
# End Project
