# Microsoft Developer Studio Project File - Name="GersangCrashSender" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GersangCrashSender - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GersangCrashSender.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GersangCrashSender.mak" CFG="GersangCrashSender - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GersangCrashSender - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GersangCrashSender - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GersangCrashSender", IBHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GersangCrashSender - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
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
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"D:\Server\HQProject\gersangcrashsender\Bin\GCS.exe"

!ELSEIF  "$(CFG)" == "GersangCrashSender - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"D:\Server\HQProject\gersangcrashsender\Bin\GCS.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GersangCrashSender - Win32 Release"
# Name "GersangCrashSender - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GersangCrashSender.cpp
# End Source File
# Begin Source File

SOURCE=.\GersangCrashSender.rc
# End Source File
# Begin Source File

SOURCE=.\GersangCrashSenderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GersangCrashSender.h
# End Source File
# Begin Source File

SOURCE=.\GersangCrashSenderDlg.h
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

SOURCE=.\res\Background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GersangCrashSender.ico
# End Source File
# Begin Source File

SOURCE=.\res\GersangCrashSender.rc2
# End Source File
# End Group
# Begin Group "e-mai libarary"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EMailLib\AppOctetStream.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\AppOctetStream.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\Base64.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\Base64.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MailMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MailMessage.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MIMECode.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MIMECode.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MIMEContentAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MIMEContentAgent.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MIMEMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\MIMEMessage.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\SMTP.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\SMTP.h
# End Source File
# Begin Source File

SOURCE=.\EMailLib\TextPlain.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailLib\TextPlain.h
# End Source File
# End Group
# Begin Group "CrashMailProtocol"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CrashMailProtocol\CrashMailProtocol.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
