// GersangServerStatus.h: interface for the CGersangServerStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GERSANGSERVERSTATUS_H__C33A5A84_C580_4224_AC01_E859C2778355__INCLUDED_)
#define AFX_GERSANGSERVERSTATUS_H__C33A5A84_C580_4224_AC01_E859C2778355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#define MAX_SERVER_NAME 128
#define MAX_SERVER_IP  128

class CServerMonitorDlg;
class CGersangServerMonitor;
class CGersangServerPatch;

enum server_state
{
	SERVER_STATE_NOTCONNECTED = 0,
	SERVER_STATE_CONNECTING,
	SERVER_STATE_LIVE,
	SERVER_STATE_DEATH,
	SERVER_STATE_TIMEOUT,
	SERVER_STATE_UNKNOWN

};

class CGersangServerStatus  
{
	friend CServerMonitorDlg;
	friend CGersangServerMonitor;
	friend CGersangServerPatch;
public:

	
	

	int Process();
	
	CGersangServerStatus();
	virtual ~CGersangServerStatus();

	int SetServerName(const char *szName);
	const TCHAR *GetServerName();
	void GetServerInfo(TCHAR *szString);
	void SetServerOnTime(LPSYSTEMTIME pSystemTime);

	

	int GetState(int serverkind);	

	
	
	int GetUserNum() {return m_siUserNum;} ;
	int GetMaxUserNum() {return m_siMaxUserNum;};
	int GetDifferance() {return m_siDifferance;};
	int GetSameUserCount() {return m_siSameUserCount;};	
	int GetKickCount() {return m_siKickCount;};
	void SetKickCount(int num) {m_siKickCount = num;};
	int GetMute() {return m_siMute;};

	void SetMyServerNum(int num) {m_siMyServerNum = num;};

	static void SetServerNum(int num);
	static int GetServerNum() {return siServerNum;}

	void Init(int num);


	

private:

	static int siServerNum;

	
	TCHAR m_szName[MAX_SERVER_NAME];

	
	int *m_psiServerStates;
	SYSTEMTIME m_ServerOnTime;
	int m_siUserNum;
	int m_siMaxUserNum;
	DWORD m_dwHeartBeatTime;
	int m_siSameUserCount;
	int m_siDifferance;
	int m_siKickCount;
	int m_siMyServerNum;
	int m_siGateQueue;
	int m_siSubQueue;

	int m_siMute;

};

#endif // !defined(AFX_GERSANGSERVERSTATUS_H__C33A5A84_C580_4224_AC01_E859C2778355__INCLUDED_)
