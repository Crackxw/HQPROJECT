# Microsoft Developer Studio Project File - Name="SO Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SO Server - Win32 Jeremy Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SO Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SO Server.mak" CFG="SO Server - Win32 Jeremy Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SO Server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SO Server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "SO Server - Win32 Jeremy Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SO Server/SO Server", YDAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SO Server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SO_Server___Win32_Release"
# PROP BASE Intermediate_Dir "SO_Server___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\SCCommon" /I "..\\ImjinOnline" /I "SOGame" /I "..\\GCommon" /I "SOGame/VillageStructure" /I "..\\MegaBind" /I "..\..\GSLib\Include" /I "..\..\GSLib\GameGuard" /D "NDEBUG" /D "FD_SETSIZE#6000" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "WIN32_LEAN_AND_MEAN" /D "_IMJINONLINESERVER" /D "ZLIB_DLL" /D "_WINDOWS" /D "_GAMEGUARD" /FR /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib GSLCore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ntwdblib.lib ws2_32.lib winmm.lib CSAuth.lib zlib.lib /nologo /subsystem:console /machine:I386 /out:"C:\GameServer\SO Server\Data\SO Server.exe" /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /libpath:"..\..\HQProject\MegaBind" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SO Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SO_Server___Win32_Debug0"
# PROP BASE Intermediate_Dir "SO_Server___Win32_Debug0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\SCCommon" /I "." /I "..\SCCommon" /I "..\\ImjinOnline" /I "SOGame" /I "..\\GCommon" /I "SOGame/VillageStructure" /I "..\\MegaBind" /I "..\..\GSLib\Include" /I "..\..\GSLib\GameGuard" /D "_DEBUG" /D "FD_SETSIZE#6000" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_IMJINONLINESERVER" /D "ZLIB_DLL" /D "_LIB" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GSLCore_D.lib ntwdblib.lib ws2_32.lib winmm.lib CSAuth.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"c:\GameServer\SO Server\Data\SO Server.exe" /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SO Server - Win32 Jeremy Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SO_Server___Win32_Jeremy_Debug0"
# PROP BASE Intermediate_Dir "SO_Server___Win32_Jeremy_Debug0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SO_Server___Win32_Jeremy_Debug"
# PROP Intermediate_Dir "SO_Server___Win32_Jeremy_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\SCCommon" /I "..\\ImjinOnline" /I "SOGame" /I "..\\hqteaminc" /I "..\\GCommon" /I "SOGame/VillageStructure" /I "..\\SCCommon" /I "..\\MegaBind" /D "_DEBUG" /D "FD_SETSIZE#6000" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_IMJINONLINESERVER" /D "ZLIB_DLL" /D "_LIB" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\SCCommon" /I "." /I "..\SCCommon" /I "..\\ImjinOnline" /I "SOGame" /I "..\\GCommon" /I "SOGame/VillageStructure" /I "..\\MegaBind" /I "..\..\GSLib\Include" /I "..\..\GSLib\GameGuard" /D "_DEBUG" /D "FD_SETSIZE#6000" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_IMJINONLINESERVER" /D "ZLIB_DLL" /D "_LIB" /D "_WINDOWS" /D "_GAMEGUARD" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ntwdblib.lib ws2_32.lib winmm.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"C:\GameServer\SO Server\Data\SO Server.exe" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 GSLCore_JD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ntwdblib.lib ws2_32.lib winmm.lib CSAuth.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"D:\GameServer\SO Server\Data\SO Server.exe" /libpath:"..\..\GSLib\Lib" /libpath:"..\..\GSLib\GameGuard" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SO Server - Win32 Release"
# Name "SO Server - Win32 Debug"
# Name "SO Server - Win32 Jeremy Debug"
# Begin Group "_JEWAN'S WORK FOLDER"

# PROP Default_Filter ""
# Begin Group "JX Network Library Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocketMgr.h
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
# Begin Group "JX Network Library Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\JXAcceptSocketList.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\XCommon\JXConnectSocketMgr.cpp
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
# Begin Group "XPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\XCommon\XPacketCmd.h
# End Source File
# Begin Source File

SOURCE=.\XPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\XPlayer.h
# End Source File
# Begin Source File

SOURCE=.\XPlayerMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\XPlayerMgr.h
# End Source File
# End Group
# End Group
# Begin Group "Character"

# PROP Default_Filter ""
# Begin Group "SOPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SOPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\SOPlayer.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_AuditBook.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_CommonlyMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_Credit.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_Field.cpp
# End Source File
# Begin Source File

SOURCE=.\SOPlayer_ItemBankList.cpp
# End Source File
# Begin Source File

SOURCE=.\SOPlayer_ItemBankList.h
# End Source File
# Begin Source File

SOURCE=.\SOPlayer_Login.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_MoveMap.cpp
# End Source File
# Begin Source File

SOURCE=.\party\SOPlayer_Party.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_PrivateMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\SOPlayer_ProcessMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_SendFiledMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\SOPlayer_Trade.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOPlayer_Village.cpp
# End Source File
# Begin Source File

SOURCE=.\SOPlayer_War.cpp
# End Source File
# End Group
# Begin Group "Monster"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SOGame\ISOMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\ISOMonster.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOMonster.h
# End Source File
# Begin Source File

SOURCE=.\State.cpp
# End Source File
# Begin Source File

SOURCE=.\State.h
# End Source File
# Begin Source File

SOURCE=.\StateMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\StateMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\SOGame\SOBaseChar.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOBaseChar.h
# End Source File
# Begin Source File

SOURCE=.\SOBaseChar_Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOBaseChar_SendFiledMsg.cpp
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Server World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SOServer.cpp
# End Source File
# Begin Source File

SOURCE=.\SOServer.h
# End Source File
# Begin Source File

SOURCE=.\SOWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\SOWorld.h
# End Source File
# Begin Source File

SOURCE=.\XMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\XMonitor.h
# End Source File
# Begin Source File

SOURCE=..\XCommon\XMonitorCmd.h
# End Source File
# End Group
# Begin Group "Database"

# PROP Default_Filter ""
# Begin Group "DBQuery"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_DBQueryDefine.h
# End Source File
# Begin Source File

SOURCE=.\DBGateQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\DBGateQueue.h
# End Source File
# Begin Source File

SOURCE=.\DBQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\DBQueue.h
# End Source File
# Begin Source File

SOURCE=.\DBQueueProc.cpp
# End Source File
# Begin Source File

SOURCE=.\DBQueueProc.h
# End Source File
# Begin Source File

SOURCE=.\DBSubQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSubQueue.h
# End Source File
# End Group
# Begin Group "DBQueryMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DBQueryMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DBQueryMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MSSQLDB.cpp
# End Source File
# Begin Source File

SOURCE=.\MSSQLDB.h
# End Source File
# Begin Source File

SOURCE=.\SODatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\SODatabase.h
# End Source File
# Begin Source File

SOURCE=.\SODBStructure.h
# End Source File
# End Group
# Begin Group "ExternalComm"

# PROP Default_Filter ""
# Begin Group "AuthServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\AuthServer\_AuthDefine.h
# End Source File
# Begin Source File

SOURCE=.\AuthServerProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthServerProc.h
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\MSSQL.cpp
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\MSSQL.h
# End Source File
# End Group
# Begin Group "FrontServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\FrontServer\_FrontDefine.h
# End Source File
# Begin Source File

SOURCE=..\FrontServer\_FrontGameServerDefine.h
# End Source File
# Begin Source File

SOURCE=.\FrontServerProc.cpp
# End Source File
# Begin Source File

SOURCE=.\FrontServerProc.h
# End Source File
# End Group
# Begin Group "Winsock"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\kojimodo\Utility\Winsock.cpp
# End Source File
# Begin Source File

SOURCE=..\kojimodo\Utility\Winsock.h
# End Source File
# End Group
# Begin Group "Auth & Login Thread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\AuthServer\_LoginDefine.h
# End Source File
# Begin Source File

SOURCE=.\AuthProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthProc.h
# End Source File
# Begin Source File

SOURCE=..\AuthServer\LoginProc.cpp
# End Source File
# Begin Source File

SOURCE=..\AuthServer\LoginProc.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\FieldBattleParseer.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestEventItemParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGameServerParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGate.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGMList.cpp
# End Source File
# Begin Source File

SOURCE=.\SOMain.cpp
# End Source File
# Begin Source File

SOURCE=.\Stack.cpp
# End Source File
# Begin Source File

SOURCE=.\WarpParser.cpp
# End Source File
# Begin Source File

SOURCE=.\WarTimeParser.cpp
# End Source File
# End Group
# Begin Group "SCCommonSource"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=..\SCCommon\BlackPig.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Common.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Friend.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\GuildClassParser.cpp
# End Source File
# Begin Source File

SOURCE=..\GCommon\HQFileManager.cpp
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

SOURCE=..\SCCommon\JOSA.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseCharKI.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseMap.cpp
# End Source File
# Begin Source File

SOURCE="..\SCCommon\OnlineChar-Parser.cpp"
# End Source File
# Begin Source File

SOURCE=..\GCommon\OnlineCredit.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineEconomy.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineFieldParser.cpp
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

SOURCE=..\sccommon\OnlineSmithParser.cpp
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

SOURCE=..\SCCommon\SORand.cpp
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
# Begin Group "SCCommonHeader"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\SCCommon\BattleProtocol.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BlackPig.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\cltList.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Common.h
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

SOURCE=..\GCommon\HQFileManager.h
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

SOURCE=.\JODebug.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JOSA.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseCharKI.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineBaseMap.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\onlinebattlemsg.cpp
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

SOURCE=..\SCCommon\OnlineCommonParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineCommonParser.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineCommonStruct.h
# End Source File
# Begin Source File

SOURCE=..\GCommon\OnlineCredit.h
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

SOURCE="..\SCCommon\OnlineGeneral-Parser.h"
# End Source File
# Begin Source File

SOURCE=..\ImjinOnline\Onlineimjin2I.h
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

SOURCE=..\sccommon\OnlineSmithParser.h
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

SOURCE=..\SCCommon\ReadIP.h
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

SOURCE=..\SCCommon\SORand.h
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
# Begin Group "SOGameHeader"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\SOGame\Command.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\ExpoRank.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\FieldMonster.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\Guild.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\GuildMemberManager.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\IFieldMonster.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\IGuild.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\IOnlineMap.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\IPortal.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\ISONPCKI.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\ISOShip.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\OnlineMonsterParser.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\PlayersManager.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\Point.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\PrivateMarket.h
# End Source File
# Begin Source File

SOURCE=.\QuestManager.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOCharKI.h
# End Source File
# Begin Source File

SOURCE=.\SOCommonHeader.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOFindPath.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOMap.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SONPCKI.h
# End Source File
# Begin Source File

SOURCE=.\SOPlant.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOShip.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOVillage.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\Trade.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\User.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\UserCatalog.h
# End Source File
# End Group
# Begin Group "RegControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RegControl\RegControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RegControl\RegControl.h
# End Source File
# End Group
# Begin Group "BattleMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\Battle.h
# End Source File
# Begin Source File

SOURCE=.\BattleMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleMgr.h
# End Source File
# Begin Source File

SOURCE=.\Element.cpp
# End Source File
# Begin Source File

SOURCE=.\Element.h
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# End Group
# Begin Group "Farm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SOPlayer_Farm.cpp
# End Source File
# Begin Source File

SOURCE=.\SOVillage_Farm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FieldBattleParser.h
# End Source File
# Begin Source File

SOURCE=.\QuestEventItemParser.h
# End Source File
# Begin Source File

SOURCE=.\SODefine.h
# End Source File
# Begin Source File

SOURCE=.\SOGameServerParser.h
# End Source File
# Begin Source File

SOURCE=.\SOGate.h
# End Source File
# Begin Source File

SOURCE=.\SOGMList.h
# End Source File
# Begin Source File

SOURCE=.\SOMain.h
# End Source File
# Begin Source File

SOURCE=.\SOUtility.h
# End Source File
# Begin Source File

SOURCE=.\Stack.h
# End Source File
# Begin Source File

SOURCE=.\WarpParser.h
# End Source File
# Begin Source File

SOURCE=.\WarTimeParser.h
# End Source File
# Begin Source File

SOURCE=.\XDBProcDefine.h
# End Source File
# End Group
# Begin Group "SkillMgr"

# PROP Default_Filter ""
# Begin Group "BuriedItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SCCommon\_BuriedItemDefine.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BuriedItemParser.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\BuriedItemParser.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\SCCommon\_SkillDefine.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Skill.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\Skill.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SkillMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\SkillMgr.h
# End Source File
# End Group
# Begin Group "AuctionMarket"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SCCommon\_AuctionDefine.h
# End Source File
# Begin Source File

SOURCE=.\AuctionList.cpp
# End Source File
# Begin Source File

SOURCE=.\AuctionList.h
# End Source File
# Begin Source File

SOURCE=.\AuctionMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\AuctionMarket.h
# End Source File
# Begin Source File

SOURCE=.\AuctionMarketProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AuctionMarketProc.h
# End Source File
# Begin Source File

SOURCE=.\PlayerList.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerList.h
# End Source File
# End Group
# Begin Group "VillageStructure"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_BankStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_BankStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_BarrackStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_BarrackStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_BaseStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_BaseStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_GovernmentStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_GovernmentStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_HospitalStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_HospitalStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_IndustrialComplex.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_IndustrialComplex.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_IndustrialComplex_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_IndustrialComplex_Factory.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_IndustrialComplexsStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_IndustrialComplexsStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_MarketStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_MarketStructure.h
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_WharfStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\VillageStructure\SOVillage_WharfStructure.h
# End Source File
# End Group
# Begin Group "Smith"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Smith.cpp
# End Source File
# Begin Source File

SOURCE=.\Smith.h
# End Source File
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
# Begin Group "ChallengeMode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChallengeMode.cpp
# End Source File
# Begin Source File

SOURCE=.\ChallengeMode.h
# End Source File
# End Group
# Begin Group "Megabind"

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
# Begin Group "party_system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\party\IPartyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\party\IPartyMgr.h
# End Source File
# End Group
# Begin Group "SOGameSource"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\SOGame\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\ExpoRank.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\FieldMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\Guild.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\GuildMemberManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\IFieldMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\IGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\IOnlineMap.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\IPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\ISONPCKI.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\ISOShip.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\OnlineMonsterParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\PlayersManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\Point.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\PrivateMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOCharKI.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOFindPath.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOMap.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SONPCKI.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOShip.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOShip_SendFieldMsg.cpp
# End Source File
# Begin Source File

SOURCE=".\SOGame\SOVillage-Market.cpp"
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOVillage.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOVillage_Bank.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOVillage_Inn.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\SOVillage_Investment.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\Trade.cpp
# End Source File
# Begin Source File

SOURCE=.\SOGame\UserCatalog.cpp
# End Source File
# End Group
# Begin Group "GCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GCommon\clgame.h
# End Source File
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

SOURCE=..\SCCommon\JODebug.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JODebug.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JOGersangConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\JOGersangConfig.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\MacroCheckProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\SCCommon\MacroCheckProcess.h
# End Source File
# Begin Source File

SOURCE=..\SCCommon\OnlineClientCallback.h
# End Source File
# End Group
# Begin Group "Crypt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\CEncrypt.h
# End Source File
# Begin Source File

SOURCE=.\CSeed.cpp
# End Source File
# Begin Source File

SOURCE=.\CSeed.h
# End Source File
# Begin Source File

SOURCE=.\SeedX.h
# End Source File
# End Group
# Begin Group "SiegeUint"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSiegeUniteTable.cpp
# End Source File
# Begin Source File

SOURCE=.\CSiegeUniteTable.h
# End Source File
# End Group
# End Target
# End Project
