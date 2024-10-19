# Microsoft Developer Studio Project File - Name="CrashMonitorClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CrashMonitorClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CrashMonitorClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CrashMonitorClient.mak" CFG="CrashMonitorClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CrashMonitorClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CrashMonitorClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/CrashMonitorClient", YAIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

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

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

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
# ADD LINK32 ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"D:\Server\HQProject\CRASHMONITORCLIENT\Bin\CrashMonitorClient.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CrashMonitorClient - Win32 Release"
# Name "CrashMonitorClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConnectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashClientFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashInfoRecvProcessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClient.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClient.rc
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClientDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClientView.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashTopFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\JOCrashClientConfig.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConnectDlg.h
# End Source File
# Begin Source File

SOURCE=.\CrashClientFormView.h
# End Source File
# Begin Source File

SOURCE=.\CrashInfo.h
# End Source File
# Begin Source File

SOURCE=.\CrashInfoRecvProcessDlg.h
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClient.h
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClientDoc.h
# End Source File
# Begin Source File

SOURCE=.\CrashMonitorClientView.h
# End Source File
# Begin Source File

SOURCE=.\CrashTopFormView.h
# End Source File
# Begin Source File

SOURCE=.\Defines.h
# End Source File
# Begin Source File

SOURCE=.\JOCrashClientConfig.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsFormView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CrashMonitorClient.ico
# End Source File
# Begin Source File

SOURCE=.\res\CrashMonitorClient.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CrashMonitorClientDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ctrl\AW_CMultiViewSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl\AW_CMultiViewSplitter.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl\TitleTip.h
# End Source File
# End Group
# Begin Group "CrashMailProtocol"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CrashMailProtocol\CrashMailProtocol.h
# End Source File
# End Group
# Begin Group "XCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXCommon.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocketMgr.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocketMgr.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\jxdef.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXGateSocket.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXGateSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXList.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXObject.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacket.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketList.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketQueue.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXPacketQueue.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocket.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocketMgr.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\XCommon\JXSocketMgr.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXWorkManager.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

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
# Begin Group "JOCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\JOCommon\JOConfig.cpp

!IF  "$(CFG)" == "CrashMonitorClient - Win32 Release"

!ELSEIF  "$(CFG)" == "CrashMonitorClient - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\JOCommon\JOConfig.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
