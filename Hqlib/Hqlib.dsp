# Microsoft Developer Studio Project File - Name="Hqlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Hqlib - Win32 Windowed Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Hqlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Hqlib.mak" CFG="Hqlib - Win32 Windowed Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Hqlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Hqlib - Win32 Windowed Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Hqlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/HQProject/Hqlib", IDFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Hqlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\JSInterface" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /D "_USE_HASHCODE_EFFECTSOUND" /D "_OGG_PLAY" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_IMJIN2" /D "ZLIB_DLL" /D "_CRTDBG_MAP_ALLOC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/Hqlib_D.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug/Hqlib_D.lib"
# Begin Special Build Tool
TargetPath=.\Debug\Hqlib_D.lib
SOURCE="$(InputPath)"
PostBuild_Cmds=copy   $(TargetPath)   ..\..\GSLib\lib\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Hqlib - Win32 Windowed Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Hqlib___Win32_Windowed_Debug"
# PROP BASE Intermediate_Dir "Hqlib___Win32_Windowed_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WindowedDebug"
# PROP Intermediate_Dir "WindowedDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\SCCommon" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\JSInterface" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_IMJIN2" /D "ZLIB_DLL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\JSInterface" /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /D "_USE_HASHCODE_EFFECTSOUND" /D "_OGG_PLAY" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_IMJIN2" /D "ZLIB_DLL" /D "_CRTDBG_MAP_ALLOC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"WindowedDebug/Hqlib_WD.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Gersang\Hqlib.lib"
# ADD LIB32 /nologo /out:"WindowedDebug/Hqlib_WD.lib"
# Begin Special Build Tool
TargetPath=.\WindowedDebug\Hqlib_WD.lib
SOURCE="$(InputPath)"
PostBuild_Cmds=copy   $(TargetPath)   ..\..\GSLib\lib\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Hqlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Hqlib___Win32_Release"
# PROP BASE Intermediate_Dir "Hqlib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_IMJIN2" /D "ENGLISH" /D "_ONLINE" /D "ZLIB_DLL" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\MegaBind" /I "..\FF2D" /I "..\GCommon" /I "..\SCCommon" /I "..\ImjinOnline" /I "..\hqteaminc" /I "..\AuthServer" /I "..\FrontServer" /I "..\Gersang" /I "..\..\GSLib\include" /D "_USE_HASHCODE_EFFECTSOUND" /D "_OGG_PLAY" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_IMJIN2" /D "ENGLISH" /D "_ONLINE" /D "ZLIB_DLL" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
TargetPath=.\Release\Hqlib.lib
SOURCE="$(InputPath)"
PostBuild_Cmds=copy   $(TargetPath)   ..\..\GSLib\lib\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Hqlib - Win32 Debug"
# Name "Hqlib - Win32 Windowed Debug"
# Name "Hqlib - Win32 Release"
# Begin Group "MiniGame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImjinOnline\MiniGame.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\MiniGame1.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\MiniGame1.h
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\MiniGame2.cpp
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\MiniGame3.cpp
# End Source File
# End Group
# Begin Group "SCommon"

# PROP Default_Filter ""
# Begin Group "SCommon_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SCCommon\_AuctionDefine.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\_BuriedItemDefine.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\_SkillDefine.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BattleProtocol.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BlackPig.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BuriedItemParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\CEncrypt.h
# End Source File
# Begin Source File

SOURCE=..\sccommon\cltList.h
# End Source File
# Begin Source File

SOURCE=..\sccommon\Common.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\CSeed.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Defines.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Friend.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\GBServer.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\GSCDefine.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\GSCProtocol.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\GuildClassParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IGuildClass.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IOnlineFollower.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IOnlineShipKI.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IProfit.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ItemUpgrade.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ItemUpgradeList.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ItemUpgradeParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JODebug.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JOSA.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\LoginData.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\MacroCheckProcess.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseChar.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseCharKI.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseMap.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\onlinebattlemsg.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineCGSHeader.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineChar-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineClientCallback.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineCommonParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineCommonStruct.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineEconomy.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineFieldParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineGameMsg.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineGameOption.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineGeneral-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnLineItem.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineLevelMark-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMap-Bf.h"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMapIndex-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMapStore-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMercenary-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineMyWeapons.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineNPCIndex-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineObjectParser.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlinePortal-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineProductStructure.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineProtocol.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineQuest-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineShipKI.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineShipMoveInfo.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineSkillParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineSmithParser.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineSoilderList-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineText.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineTicket-Parser.h"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineTip-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineTradeParser.h
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineVillage-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineWarpParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\parametabox.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ProfitParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\protocol.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ReadIP.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SeedX.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ShipInfoParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SMPathParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SMPointParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Timer.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\VillageAttackInfo.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\wansung2johap.h
# End Source File
# End Group
# Begin Group "SCommon_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SCCommon\BlackPig.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BuriedItemParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\CEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\sccommon\Common.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\CSeed.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Friend.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\GuildClassParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IGuildClass.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IOnlineFollower.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IOnlineShipKI.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\IProfit.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ItemUpgrade.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ItemUpgradeList.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ItemUpgradeParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JODebug.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JOSA.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\MacroCheckProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseCharKI.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseMap.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\onlinebattlemsg.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineChar-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineCommonParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineEconomy.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineFieldParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineGameOption.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineGeneral-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnLineItem.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineLevelMark-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMapIndex-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMapStore-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineMercenary-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineMyWeapons.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineNPCIndex-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineObjectParser.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlinePortal-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineProductStructure.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineQuest-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineShipKI.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineShipMoveInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineSkillParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineSmithParser.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineSoilderList-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineText.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineTicket-Parser.cpp"
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineTip-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineTradeParser.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineVillage-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineWarpParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Parametabox.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ProfitParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ReadIP.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\ShipInfoParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SMPathParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SMPointParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Timer.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\VillageAttackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\wansung2johap.cpp
# End Source File
# End Group
# Begin Group "FrontServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\FrontServer\_FrontDefine.h
# End Source File
# Begin Source File

SOURCE=..\FrontServer\_FrontUserDefine.h
# End Source File
# End Group
# Begin Group "Login"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\AuthServer\_LoginDefine.h
# End Source File
# End Group
# End Group
# Begin Group "Imjin2"

# PROP Default_Filter ""
# Begin Group "Imjin2_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Gersang\directx.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\map.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\mapedit.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\message.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\mine.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\music.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\ycontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YListBoxControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YListButton.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YListControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\ymemoryfree.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YMIControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YOkCancel.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YOnOffControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\yoption.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\yoptionmenu.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\yprg.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YProgressBarControl.Cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YScrSpeedControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\YSelectControl.cpp
# End Source File
# End Group
# Begin Group "Imjin2_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Gersang\directx.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\mapedit.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\merchant.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\message.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\mine.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\music.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\myio.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\mytime.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\ycdmessage.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\ycontrol.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YListBoxControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YListButton.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YListControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\ymemoryfree.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YMIControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\yobjective.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YOkCancel.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YOnOffControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\yoption.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\yoptionmenu.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\yprg.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YProgressBarControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YScrSpeedControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\YSelectControl.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\yselectstage.h
# End Source File
# End Group
# Begin Group "Chardraw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Gersang\CharInfo\CharFileLoadManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\CharInfo\CharFileLoadManager.h
# End Source File
# Begin Source File

SOURCE=..\Gersang\CharInfo\CharFileLoadManager_Hero.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\CharInfo\CharFileLoadManager_Item.cpp
# End Source File
# Begin Source File

SOURCE=..\Gersang\CharInfo\CharFileLoadManagerDefine.h
# End Source File
# End Group
# End Group
# Begin Group "GCommon"

# PROP Default_Filter ""
# Begin Group "GCommon_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\ddutil.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\findpath5.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\gamebutton.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\keybuffer.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\map.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\mapinterface.h
# End Source File
# Begin Source File

SOURCE="..\GCommon\mynet-game.h"
# End Source File
# Begin Source File

SOURCE=..\GCommon\mynet.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\SPrg.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\wavread.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\yprg2.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\yPutGetResource.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\ysmap.h
# End Source File
# End Group
# Begin Group "GCommon_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\findpath5.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\gamebutton.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\keybuffer.cpp
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-array.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-block.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-crop.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-level.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-ruin.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-save.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-tile.cpp"
# End Source File
# Begin Source File

SOURCE="..\GCommon\map-water.cpp"
# End Source File
# Begin Source File

SOURCE=..\GCommon\map2.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\mapcastle.CPP
# End Source File
# Begin Source File

SOURCE=..\GCommon\mapdraw.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\mapinterface.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\maprandom.CPP
# End Source File
# Begin Source File

SOURCE="..\GCommon\MyNet-game.cpp"
# End Source File
# Begin Source File

SOURCE=..\GCommon\mynet.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\SPrg.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\wavread.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\yprg2.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\yPutGetResource.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\ysmap.cpp
# End Source File
# End Group
# Begin Group "Electronshock"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\CHARDRAW\CharDrawInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\CHARDRAW\CharDrawInfo.h
# End Source File
# End Group
# Begin Group "Poisoneffect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\ELECTRONSHOCK\ElectronShock.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\ELECTRONSHOCK\ElectronShock.h
# End Source File
# End Group
# Begin Group "Charinfo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\POISONEFFECT\PoisonEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\POISONEFFECT\PoisonEffect.h
# End Source File
# End Group
# End Group
# End Target
# End Project
