# Microsoft Developer Studio Project File - Name="임진록2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=임진록2 - WIN32 WINDOWED DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Gersang.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Gersang.mak" CFG="임진록2 - WIN32 WINDOWED DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "임진록2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "임진록2 - Win32 Windowed Debug" (based on "Win32 (x86) Application")
!MESSAGE "임진록2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/임진록2", IYDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "임진록2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "임진록2___Win32_Debug"
# PROP BASE Intermediate_Dir "임진록2___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../ImjinOnline/SCCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH" /D "_IMJIN2" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "..\JSInterface" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /I "..\..\GSLib\GameGuard" /D "_USE_HASHCODE_EFFECTSOUND" /D "_OGG_PLAY" /D "_DEBUG" /D "ENGLISH" /D "_ONLINE" /D "WIN32" /D "_WINDOWS" /D "_IMJIN2" /D "ZLIB_DLL" /D "_CRTDBG_MAP_ALLOC" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/Gersang_D.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib amstrmid.lib imm32.lib winmm.lib ddraw.lib dplayx.lib dsound.lib dxguid.lib wsock32.lib eaxguid.lib dxerr8.lib /nologo /subsystem:windows /machine:I386 /out:"C:\공유방\거상\Gersang.exe"
# SUBTRACT BASE LINK32 /incremental:yes /debug
# ADD LINK32 version.lib GSLCore_D.lib GSLDirectX_D.lib dxguid.lib eaxguid.lib WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib amstrmid.lib imm32.lib winmm.lib ddraw.lib dplayx.lib dsound.lib wsock32.lib dxerr8.lib MyMjollnir.lib NPGameLib.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /out:"Debug/Gersang_D.exe" /pdbtype:sept /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /mapinfo:lines /mapinfo:exports
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                 Debug\Gersang_D.exe                                 C:\공유방\거상\                                	copy                                 Debug\Gersang_D.map                                 ..\MapData\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "임진록2 - Win32 Windowed Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "임진록2___Win32_Windowed_Debug"
# PROP BASE Intermediate_Dir "임진록2___Win32_Windowed_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WindowedDebug"
# PROP Intermediate_Dir "WindowedDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /I "../ImjinOnline/SCCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH" /D "_IMJIN2" /D "_ONLINE" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\GSLib\include" /I "..\JSInterface" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /I "..\..\GSLib\GameGuard" /D "_USE_HASHCODE_EFFECTSOUND" /D "_OGG_PLAY" /D "_DEBUG" /D "ENGLISH" /D "_ONLINE" /D "_WINDOWED" /D "WIN32" /D "_WINDOWS" /D "_IMJIN2" /D "_LIB" /D "ZLIB_DLL" /D "_CRTDBG_MAP_ALLOC" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"WindowedDebug/Gersang_WD.bsc"
LINK32=link.exe
# ADD BASE LINK32 MFSvc.lib WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib amstrmid.lib imm32.lib winmm.lib ddraw.lib dplayx.lib dsound.lib dxguid.lib wsock32.lib eaxguid.lib dxerr8.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /out:"C:\공유방\거상\Gersang.exe"
# ADD LINK32 Version.lib GSLCore_WD.lib GSLDirectX_WD.lib dxguid.lib eaxguid.lib WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib amstrmid.lib imm32.lib winmm.lib ddraw.lib dplayx.lib dsound.lib wsock32.lib dxerr8.lib MyMjollnir.lib NPGameLib.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /out:"WindowedDebug/Gersang_WD.exe" /pdbtype:sept /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /mapinfo:lines /mapinfo:exports
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                WindowedDebug\Gersang_WD.exe                                C:\공유방\거상\                               	copy                                WindowedDebug\Gersang_WD.map                                ..\MapData\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "임진록2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "임진록2___Win32_Release"
# PROP BASE Intermediate_Dir "임진록2___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\JSInterface" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /I "..\..\GSLib\GameGuard" /D "NDEBUG" /D "ENGLISH" /D "_ONLINE" /D "WIN32" /D "_WINDOWS" /D "_IMJIN2" /D "ZLIB_DLL" /D "_GAMEGUARD" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\JSInterface" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /I "..\..\GSLib\GameGuard" /D "_USE_HASHCODE_EFFECTSOUND" /D "_OGG_PLAY" /D "NDEBUG" /D "ENGLISH" /D "_ONLINE" /D "WIN32" /D "_WINDOWS" /D "_IMJIN2" /D "ZLIB_DLL" /D "_GAMEGUARD" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib GSLCore.lib GSLDirectX.lib dxguid.lib eaxguid.lib WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib amstrmid.lib imm32.lib winmm.lib ddraw.lib dplayx.lib dsound.lib wsock32.lib dxerr8.lib MyMjollnir.lib NPGameLib.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /mapinfo:lines /mapinfo:exports
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib GSLCore.lib GSLDirectX.lib dxguid.lib eaxguid.lib WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib amstrmid.lib imm32.lib winmm.lib ddraw.lib dplayx.lib dsound.lib wsock32.lib dxerr8.lib NPGameLib.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /mapinfo:lines /mapinfo:exports
# SUBTRACT LINK32 /pdb:none /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                  Release\Gersang.exe                                  C:\공유방\거상\                                 	copy                                  Release\Gersang.map                                  ..\MapData\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "임진록2 - Win32 Debug"
# Name "임진록2 - Win32 Windowed Debug"
# Name "임진록2 - Win32 Release"
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\imjin2.rc
# End Source File
# Begin Source File

SOURCE=.\plus.ico
# End Source File
# End Group
# Begin Group "ImjinOnline"

# PROP Default_Filter ""
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BgmMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\FieldHeroInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\FieldHeroInfoMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineChar.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineCharKI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineObject.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineObjectKI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\MakePalette.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\MemMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBanner.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBaseParser.cpp
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineChar-DrawChar.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineChar-Move.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineChar-Order.cpp"
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineChar.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharActionCode.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharKI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharUI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineChatList.cpp
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineClient-CommandProc.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineClient-Send.cpp"
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineClient.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineDungeonParser.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineETC.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldArea.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_Bullet.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_BulletMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_Smoke.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_SmokeMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFindPath.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFollower.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFont.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineHelp.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\Onlineimjin2I.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMap.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMapBoothInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMapSound.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMegaText.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMemoryProtected.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMFGS.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMyData.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineObject.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineObjectKI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineResource.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineRPI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVersion.cpp
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-FieldShortCutKey.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-IntroScreen.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-PacketProc.cpp"

!IF  "$(CFG)" == "임진록2 - Win32 Debug"

!ELSEIF  "$(CFG)" == "임진록2 - Win32 Windowed Debug"

!ELSEIF  "$(CFG)" == "임진록2 - Win32 Release"

# ADD BASE CPP /Zi /O2 /Ob1
# ADD CPP /Zi /O2 /Ob1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-ProcessMyCharOrder.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-ReceiveGoOutVillage.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-ReceiveReturnOnline.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-VillageDefenceInterface.cpp"
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineWorld.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\SiegeUnitTableMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\slanderfilter.cpp
# End Source File
# End Group
# Begin Group "Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BgmMgr.h
# End Source File
# Begin Source File

SOURCE=.\BgmMgr_PreHeader.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\FieldHeroInfo.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\FieldHeroInfoMgr.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineChar.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineCharKI.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineObject.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineObjectKI.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\MakePalette.h
# End Source File
# Begin Source File

SOURCE=..\imjinonline\MemMgr.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\Online.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBanner.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBaseParser.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineChar.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharActionCode.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharKI.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharUI.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineChatList.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineClient.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineConfig.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineDungeonParser.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineEtc.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldArea.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_Bullet.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_BulletMgr.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_Smoke.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldEffect_SiegeWarfare_SmokeMgr.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFindPath.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFollower.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFont.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineHelp.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\Onlineimjin2I.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMap.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMapBoothInfo.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMapSound.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMath.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMegaText.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMemoryProtected.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMFGS.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMyData.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineObject.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineObjectKI.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineOptionWindow.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineResource.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineRPI.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVersion.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineWorld.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\SiegeUnitTableMgr.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\slanderfilter.h
# End Source File
# Begin Source File

SOURCE=.\stringcpy.h
# End Source File
# End Group
# Begin Group "ChattingServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImjinOnline\ChattingServer\HQChattingServerClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\ChattingServer\HQChattingServerClientSocket.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\ChattingServer\HQChattingServerPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\ChattingServer\HQChattingServerPacket.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\ChattingServer\HQChattingServerPacketCMD.h
# End Source File
# End Group
# Begin Group "InputDevice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\IOnlineMouse.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\mouse.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\mouse.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMouse.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMousePoint.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMousePoint.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMousePointInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMousePointInfo.h
# End Source File
# End Group
# Begin Group "MsgProc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldMsgProc.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineGlobal.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMsgProc.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMsgProc.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineSystemMsgProc.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageMsgProc.cpp
# End Source File
# End Group
# Begin Group "Village"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernData.h
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernGuild.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernInvestment.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernMgr.h
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernMsg.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineGovernVillageInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillage.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillage.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageBaseBarrack.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageBaseStructure.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageBaseStructure.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageDescriptList.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageDescriptList.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageManager.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageManager.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageSave.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageSave.h
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Bank.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Bank.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Barrack.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Barrack.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Farm.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Farm.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-GBarrack.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-GBarrack.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Hospital.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Hospital.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Inn.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Inn.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Make.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Make.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Market.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Market.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-MBarrack.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-MBarrack.h"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Wharf.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineVillageStruct-Wharf.h"
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageStructManager.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineVillageStructManager.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBooth.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBooth.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharStatus.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineCharStatus.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineChatMacro.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineChatMacro.h
# End Source File
# Begin Source File

SOURCE=.\OnlineFieldAuction.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineFieldAuction.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldChat.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineFieldChat.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineImoticon.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineImoticon.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineInventory.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineInventory.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineItemList.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineItemList.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMercenary.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMercenary.h
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineMessenger.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineMessenger.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMiniMap.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMiniMap.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNotice.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNotice.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNoticeList.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNoticeList.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNPCTalk.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNPCTalk.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlinePannel.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlinePannel.h
# End Source File
# Begin Source File

SOURCE=.\OnlineParty.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineParty.h
# End Source File
# Begin Source File

SOURCE=.\OnlinePortrait.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlinePortrait.h
# End Source File
# Begin Source File

SOURCE=.\OnlineQuestInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineQuestInterface.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineQuestPlay.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineQuestPlay.h
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineSetting.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineSetting.h
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineSmith.cpp
# End Source File
# Begin Source File

SOURCE=..\imjinonline\OnlineSmith.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineSMS.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineSMS.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineSoldierTrade.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineSoldierTrade.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineTip.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineTip.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineTrade.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineTrade.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineTradeBook.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineTradeBook.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\SiegeWarfareTransformUIWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\SiegeWarfareTransformUIWnd.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\SiegeWarfareUnitUI.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\SiegeWarfareUnitUI.h
# End Source File
# End Group
# Begin Group "control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImjinOnline\GS_IMEBase.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\GS_IMEBase.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\Handler_OnlineEditControlEx.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\Handler_OnlineEditControlEx.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBaseLB.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineBaseLB.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineEditControlEx.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineEditControlEx.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineInputStatusControl.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineInputStatusControl.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineListControl.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineListControl.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMsgBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineMsgBox.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNumberControl.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\OnlineNumberControl.h
# End Source File
# End Group
# Begin Group "Login"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-ConnectServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-CreateCharacter.cpp"
# End Source File
# Begin Source File

SOURCE="..\imjinonline\OnlineWorld-LoginMgr.cpp"
# End Source File
# Begin Source File

SOURCE="..\imjinonline\OnlineWorld-LogInNotice.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-LogInOut.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-ReceiveMyData.cpp"
# End Source File
# Begin Source File

SOURCE="..\ImjinOnline\OnlineWorld-SelectCharacter.cpp"
# End Source File
# End Group
# End Group
# Begin Group "Imjin2"

# PROP Default_Filter ""
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\calcattack.cpp
# End Source File
# Begin Source File

SOURCE=".\char-absorb.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-airmagic.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-attack.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-capturebuild.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-clon.cpp"
# End Source File
# Begin Source File

SOURCE=".\Char-Draw2.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-earthquake.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-FireAttack.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-generaleffect.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-groundexp.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-heal.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-lightning.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-longspear.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-mana.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-mine.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-patrol.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-rainmake.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-recall.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-RunningFire.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-selfexp.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-shield.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-stealexp.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-updatemenu.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-usebuda.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-warp.cpp"
# End Source File
# Begin Source File

SOURCE=.\char_soul.cpp
# End Source File
# Begin Source File

SOURCE=.\cheat.cpp
# End Source File
# Begin Source File

SOURCE=.\clItemInfoWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\clNumberRender.cpp
# End Source File
# Begin Source File

SOURCE=.\earthquake.cpp
# End Source File
# Begin Source File

SOURCE=.\gamemenu.cpp
# End Source File
# Begin Source File

SOURCE=.\gmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\help.cpp
# End Source File
# Begin Source File

SOURCE=.\io.cpp
# End Source File
# Begin Source File

SOURCE=.\item.cpp
# End Source File
# Begin Source File

SOURCE=.\kindinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\maindata.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\mapedit.cpp
# End Source File
# Begin Source File

SOURCE=.\message.cpp
# End Source File
# Begin Source File

SOURCE=.\mine.cpp
# End Source File
# Begin Source File

SOURCE=.\music.cpp
# End Source File
# Begin Source File

SOURCE=.\order.cpp
# End Source File
# Begin Source File

SOURCE=.\rain.cpp
# End Source File
# Begin Source File

SOURCE=.\screenpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\stage.cpp
# End Source File
# Begin Source File

SOURCE=.\text.cpp
# End Source File
# Begin Source File

SOURCE=.\time.cpp
# End Source File
# Begin Source File

SOURCE=.\winlose.cpp
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\buttonssprmanagement.h
# End Source File
# Begin Source File

SOURCE=.\calcattack.h
# End Source File
# Begin Source File

SOURCE=".\char-mana.h"
# End Source File
# Begin Source File

SOURCE=".\char-recall.h"
# End Source File
# Begin Source File

SOURCE=".\char-RunningFire.h"
# End Source File
# Begin Source File

SOURCE=.\cheat.h
# End Source File
# Begin Source File

SOURCE=.\choosenation.h
# End Source File
# Begin Source File

SOURCE=.\clItemInfoWindow.h
# End Source File
# Begin Source File

SOURCE=.\clNumberRender.h
# End Source File
# Begin Source File

SOURCE=.\crop.h
# End Source File
# Begin Source File

SOURCE=.\earthquake.h
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=.\gamemenu.h
# End Source File
# Begin Source File

SOURCE=.\gmenu.h
# End Source File
# Begin Source File

SOURCE=.\help.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\kindinfo.h
# End Source File
# Begin Source File

SOURCE=.\loadgameinitmenu.h
# End Source File
# Begin Source File

SOURCE=.\maindata.h
# End Source File
# Begin Source File

SOURCE=.\mapedit.h
# End Source File
# Begin Source File

SOURCE=.\message.h
# End Source File
# Begin Source File

SOURCE=.\mine.h
# End Source File
# Begin Source File

SOURCE=.\music.h
# End Source File
# Begin Source File

SOURCE=.\myio.h
# End Source File
# Begin Source File

SOURCE=.\mytime.h
# End Source File
# Begin Source File

SOURCE=.\order.h
# End Source File
# Begin Source File

SOURCE=.\rain.h
# End Source File
# Begin Source File

SOURCE=.\redrawscreen.h
# End Source File
# Begin Source File

SOURCE=.\reservetrain.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resultsprmanagement.h
# End Source File
# Begin Source File

SOURCE=.\screenpoint.h
# End Source File
# Begin Source File

SOURCE=.\SoundManager.h
# End Source File
# Begin Source File

SOURCE=.\stage.h
# End Source File
# Begin Source File

SOURCE=.\syncro.h
# End Source File
# Begin Source File

SOURCE=.\text.h
# End Source File
# Begin Source File

SOURCE=.\winlose.h
# End Source File
# Begin Source File

SOURCE=.\ytitlefile.h
# End Source File
# End Group
# End Group
# Begin Group "Gcommon"

# PROP Default_Filter ""
# Begin Group "Header No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\bullet.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\button.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\capture.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\charanimation.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\charinterface.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\charorg.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\chat.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\clgame.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\Client_LogIn.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\config.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\etc.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\extern.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\findpathstraight.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\game2.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\gamefunction.h
# End Source File
# Begin Source File

SOURCE=.\GGuard.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\inventorybox.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\item.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\magic.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\main.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\mainfunction.h
# End Source File
# Begin Source File

SOURCE=..\gcommon\movemark.h
# End Source File
# Begin Source File

SOURCE=..\gcommon\myfile.h
# End Source File
# Begin Source File

SOURCE=..\gcommon\mymath.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\player.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\PReg.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\ridedata.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\setchar.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\smoke.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\Stack.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\team.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\volcano.h
# End Source File
# End Group
# Begin Group "Source No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\bullet.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\button.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\capture.cpp
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-decidefont.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-draw.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-dying.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianChicken.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianDog.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianHorse.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianRabbit.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianRecall.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianSheep.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-guardianTiger.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-move.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-order.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-setfilenfont.cpp"
# End Source File
# Begin Source File

SOURCE=".\char-SiegeMode.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\char-wait.cpp"
# End Source File
# Begin Source File

SOURCE=..\GCommon\charanimation.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\charinterface.cpp
# End Source File
# Begin Source File

SOURCE="..\GCommon\charorg-magic.cpp"
# End Source File
# Begin Source File

SOURCE=..\GCommon\charorg.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\chat.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\Client_LogIn.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\config.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\etc.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\file.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\game2.cpp
# End Source File
# Begin Source File

SOURCE=.\GGuard.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\magic.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\main.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\mainfunction.cpp
# End Source File
# Begin Source File

SOURCE=..\gcommon\movemark.cpp
# End Source File
# Begin Source File

SOURCE=..\gcommon\mymath.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\player.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\PReg.Cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\ridedata.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\setchar.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\smoke.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\team.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\volcano.cpp
# End Source File
# End Group
# End Group
# Begin Group "BindJx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MegaBind\bindJXFile.cpp
# End Source File
# Begin Source File

SOURCE=..\MegaBind\bindJXFile.h
# End Source File
# Begin Source File

SOURCE=..\MegaBind\JxHeader.h
# End Source File
# Begin Source File

SOURCE=..\MegaBind\unzip.c
# End Source File
# Begin Source File

SOURCE=..\MegaBind\unzip.h
# End Source File
# Begin Source File

SOURCE=..\MegaBind\zconf.h
# End Source File
# Begin Source File

SOURCE=..\MegaBind\zip.h
# End Source File
# Begin Source File

SOURCE=..\MegaBind\zlib.h
# End Source File
# Begin Source File

SOURCE=..\MegaBind\zlib.lib
# End Source File
# End Group
# Begin Group "LoadXSprProc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cltFreeXsprList.cpp
# End Source File
# Begin Source File

SOURCE=.\cltFreeXsprList.h
# End Source File
# Begin Source File

SOURCE=.\cltLoadXsprProc.cpp
# End Source File
# Begin Source File

SOURCE=.\cltLoadXsprProc.h
# End Source File
# Begin Source File

SOURCE=.\cltXsprQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\cltXsprQueue.h
# End Source File
# End Group
# Begin Group "EnumData"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\EnumData\Enum_AttackAtb.cpp
# End Source File
# Begin Source File

SOURCE=..\EnumData\Enum_AttackAtb.h
# End Source File
# Begin Source File

SOURCE=..\EnumData\Enum_DefenceAtb.cpp
# End Source File
# Begin Source File

SOURCE=..\EnumData\Enum_DefenceAtb.h
# End Source File
# Begin Source File

SOURCE=..\EnumData\Enum_KI_Character.cpp
# End Source File
# Begin Source File

SOURCE=..\EnumData\Enum_KI_Character.h
# End Source File
# Begin Source File

SOURCE=..\EnumData\MouseTypeEnum.cpp
# End Source File
# Begin Source File

SOURCE=..\EnumData\MouseTypeEnum.h
# End Source File
# End Group
# Begin Group "SoundPlayer"

# PROP Default_Filter ""
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SoundPlayer\DxSndPlayer.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\IPlayer.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\iSoundData.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\MyGBM.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\MyOgg.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\SoundPlayer_Common.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\StaticSound.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\StreamSound.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\Wav.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SoundPlayer\DxSndPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\IPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\iSoundData.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCriticalSection.h
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\MyGBM.cpp
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\MyOgg.cpp
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\StaticSound.cpp
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\StreamSound.cpp
# End Source File
# Begin Source File

SOURCE=..\SoundPlayer\Wav.cpp
# End Source File
# End Group
# End Group
# End Target
# End Project
