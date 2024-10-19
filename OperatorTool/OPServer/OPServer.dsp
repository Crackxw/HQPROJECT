# Microsoft Developer Studio Project File - Name="OPServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OPServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OPServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OPServer.mak" CFG="OPServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OPServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "OPServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/OperatorTool/OPServer", VTFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OPServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "OPServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\..\\ImjinOnline" /I "..\\..\\hqteaminc" /I "..\\..\\sccommon" /I "..\\..\\MegaBind" /I "..\\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_IMJINONLINESERVER" /D "ZLIB_DLL" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"C:\OP\OPServer.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OPServer - Win32 Release"
# Name "OPServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\OPServer.rc
# End Source File
# Begin Source File

SOURCE=.\OPServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\OPServer.h
# End Source File
# Begin Source File

SOURCE=.\OPServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\OPServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\OPServer.rc2
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "Common_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\Client.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Server.cpp
# End Source File
# End Group
# Begin Group "Common_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\Client.h
# End Source File
# Begin Source File

SOURCE=..\Common\Defines.h
# End Source File
# Begin Source File

SOURCE=..\Common\Server.h
# End Source File
# End Group
# End Group
# Begin Group "Parser"

# PROP Default_Filter ""
# Begin Group "Parser_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SCCommon\BuriedItemParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\GuildClassParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\JOSA.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineChar-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineEconomy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineFieldParser.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineGeneral-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnLineItem.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineLevelMark-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineMapIndex-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineMapStore-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineMercenary-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineMyWeapons.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineNPCIndex-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineObjectParser.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlinePortal-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineQuest-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineSkillParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineSmithParser.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineSoilderList-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineText.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineTicket-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineTip-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineTradeParser.cpp
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineVillage-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineWarpParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\Parametabox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\ProfitParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\ShipInfoParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\SMPathParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\SMPointParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\wansung2johap.cpp
# End Source File
# End Group
# Begin Group "Parser_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SCCommon\BuriedItemParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\GuildClassParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\JOSA.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineChar-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineEconomy.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineFieldParser.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineGeneral-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnLineItem.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineLevelMark-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineMapIndex-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineMapStore-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineMercenary-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineMyWeapons.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineNPCIndex-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineObjectParser.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlinePortal-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineQuest-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineSkillParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineSmithParser.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineSoilderList-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineText.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineTicket-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineTip-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineTradeParser.h
# End Source File
# Begin Source File

SOURCE="..\..\SCCommon\OnlineVillage-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\OnlineWarpParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\parametabox.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\ProfitParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\ShipInfoParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\SMPathParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\SMPointParser.h
# End Source File
# Begin Source File

SOURCE=..\..\SCCommon\wansung2johap.h
# End Source File
# End Group
# End Group
# Begin Group "MegaBind"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\MegaBind\bindJXFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\bindJXFile.h
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\JxHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\unzip.c
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\unzip.h
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\zip.h
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\MegaBind\zlib.lib
# End Source File
# End Group
# Begin Group "Catalog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConnectionUser.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserCatalog.cpp
# End Source File
# Begin Source File

SOURCE=.\UserCatalog.h
# End Source File
# End Group
# End Target
# End Project
