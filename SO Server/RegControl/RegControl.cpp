#include "RegControl.h"

// 생성자
RegControl::RegControl(SOServer *pServer)
{
	RegKeyInit();
	RegKeyServerInfo();

	m_clServer	= pServer;
	m_clGate	= m_clServer->clGate;
	m_clWorld	= m_clServer->clWorld;
}

// 소멸자
RegControl::~RegControl()
{
	SetRegControlReady(FALSE);
	RegKeyClose();
}

// 레지스트리 키의 초기화
BOOL RegControl::RegKeyInit(VOID)
{

	DWORD	dw;
	char	cSubKey[256]	= "SOFTWARE\\Online_Server";
	m_uiHeartBeatCount		= 0;

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, cSubKey, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dw) != ERROR_SUCCESS)
		return FALSE;
	else
		return TRUE;
}

// 레지스트리에 정보를 저장한다.
BOOL RegControl::RegKeyServerInfo(VOID)
{
	char	cServerName[256]	= "GameServer";
	char	cFullPath[256];
	char	cWorkDir[256];

	GetCurrentDirectory(256, cWorkDir);
	sprintf(cFullPath, "%s\\%s.exe", cWorkDir, cServerName);

	if(RegSetValueEx(m_hKey, "ServerName", 0, REG_SZ, (CONST BYTE *)cServerName, strlen(cServerName)) != ERROR_SUCCESS)
		return FALSE;
	if(RegSetValueEx(m_hKey, "FullPath", 0, REG_SZ, (CONST BYTE *)cFullPath, strlen(cFullPath)) != ERROR_SUCCESS)
		return FALSE;
	if(RegSetValueEx(m_hKey, "WorkDir", 0, REG_SZ, (CONST BYTE *)cWorkDir, strlen(cWorkDir)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// 일정주기로 정보를 저장한다.
BOOL RegControl::RegKeyHeartBeat(SI32 siLoopRate, SI32 siCurrentPlayerNumber, SI32 siMaxPlayerNumber)
{
	if(m_uiHeartBeatCount == 0xffffffff)
		m_uiHeartBeatCount = 0;
	else
		++m_uiHeartBeatCount;

//	SI32 siLoopRate = (SI32)m_clServer->ServerLoad();

//	RegSetValueEx(m_hKey, "HeartBeat" , 0 , REG_DWORD , (BYTE *) &m_uiHeartBeatCount, sizeof(UI32));
//	RegSetValueEx(m_hKey, "LoopRate" , 0 , REG_DWORD , (BYTE *) &siLoopRate, sizeof(UI32));
//	RegSetValueEx(m_hKey, "CurrentPlayer" , 0 , REG_DWORD , (BYTE *) &m_clGate->siTotalConnectPlayer, sizeof(UI32));
//	RegSetValueEx(m_hKey, "MaxPlayer" , 0 , REG_DWORD , (BYTE *) &m_clGate->siMaxConnectedPlayer, sizeof(UI32));

	RegSetValueEx(m_hKey, "HeartBeat" , 0 , REG_DWORD , (BYTE *) &m_uiHeartBeatCount, sizeof(UI32));
	RegSetValueEx(m_hKey, "LoopRate" , 0 , REG_DWORD , (BYTE *) &siLoopRate, sizeof(UI32));
	RegSetValueEx(m_hKey, "CurrentPlayer" , 0 , REG_DWORD , (BYTE *) &siCurrentPlayerNumber, sizeof(UI32));
	RegSetValueEx(m_hKey, "MaxPlayer" , 0 , REG_DWORD , (BYTE *) &siMaxPlayerNumber, sizeof(UI32));
	return TRUE;
}

// 종료한다.
BOOL RegControl::RegKeyClose(VOID)
{
	RegCloseKey(m_hKey);
	return TRUE;
}

VOID RegControl::SetRegControlReady(BOOL bReady)
{
	DWORD dwReady;
	dwReady = (bReady)? 1: 0;
	RegSetValueEx(m_hKey, "CheckReady" , 0 , REG_DWORD , (BYTE *) &dwReady, sizeof(DWORD));
}