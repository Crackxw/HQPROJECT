// VSManager.h: interface for the VSManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSMANAGER_H__D7A8E9F7_009E_4BA8_96DE_DA8FE07D4293__INCLUDED_)
#define AFX_VSMANAGER_H__D7A8E9F7_009E_4BA8_96DE_DA8FE07D4293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Socket.h"
#include "Clients.h"
#include "XOverlapped.h"
#include "ScriptParser.h"
#include "VersionInf.h"

class VSManager  : public CSocket
{
public:
	VSManager();
	~VSManager();

private:
	HANDLE	m_hIOCP;

private:	// 쓰레드 관련...
	HANDLE	m_hWorkerThread[256];
	UINT	m_uiWorkerID[256];
	UINT	m_uiWorkerIndex;

	HANDLE	m_hAcceptThread;
	UINT	m_uiAcceptThreadID;

public:		// 쓰레드 관련...
	inline static unsigned __stdcall WorkerProc(void* lpArg);
	void WorkerThread();
	void StartWorker();

public:		// 쓰레드 관련...
	inline static unsigned __stdcall AcceptProc(void* lpArg);
	void AcceptThread();
	void StartAccept();

public:
	int GetEmptyClient();
	void SendHeartBeat();
	void CutAllConnection();
	bool ConsoleProcess();
	BOOL Process();

private:
	bool VServerParsing(char *pFileName, VersionInf *pVersion);
	void AcceptClient();
	bool		bNotExist;
	CClients	*pClients;

public:		// Accept 동기화.
	CRITICAL_SECTION	m_AcceptCS;
public:		// 파서...
	void LoadDataFromScriptFile();
	ScriptParser *pParser;
	VersionInf m_versionInf;
};

#endif // !defined(AFX_VSMANAGER_H__D7A8E9F7_009E_4BA8_96DE_DA8FE07D4293__INCLUDED_)
