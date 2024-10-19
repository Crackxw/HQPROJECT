# Microsoft Developer Studio Project File - Name="GWebChatServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=GWebChatServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GWebChatServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GWebChatServer.mak" CFG="GWebChatServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GWebChatServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "GWebChatServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/GWebChatServer", YSEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GWebChatServer - Win32 Release"

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

!ELSEIF  "$(CFG)" == "GWebChatServer - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
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

# Name "GWebChatServer - Win32 Release"
# Name "GWebChatServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\XChatServer.cpp
# End Source File
# Begin Source File

SOURCE=.\XLobby.cpp
# End Source File
# Begin Source File

SOURCE=.\XRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\XRoomMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\XUser.cpp
# End Source File
# Begin Source File

SOURCE=.\XUserMgr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\chatprotocol.h
# End Source File
# Begin Source File

SOURCE=.\XChatServer.h
# End Source File
# Begin Source File

SOURCE=.\XLobby.h
# End Source File
# Begin Source File

SOURCE=.\XRoom.h
# End Source File
# Begin Source File

SOURCE=.\XRoomMgr.h
# End Source File
# Begin Source File

SOURCE=.\XUser.h
# End Source File
# Begin Source File

SOURCE=.\XUserMgr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "_Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\JWCommon\jwbase.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWConnect.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWConnect.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWFQueueMISO.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWFQueueMISO.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWIOCPServer.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWIOCPServer.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWList.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWList.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWListen.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWListen.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWObject.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWPacket.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWRecvBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWRecvBuffer.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWSendBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWSendBuffer.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWServer.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWServer.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWSocket.h
# End Source File
# Begin Source File

SOURCE=..\JWCommon\JWSocketMgr.h
# End Source File
# End Group
# End Target
# End Project
