// Clients.h: interface for the CClients class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTS_H__F740629F_9A03_442D_B848_4BD0AD02B966__INCLUDED_)
#define AFX_CLIENTS_H__F740629F_9A03_442D_B848_4BD0AD02B966__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Socket.h"
#include "XOverlapped.h"
#include "Scriptparser.h"

extern Version	g_stVer;
extern Version2	g_stVer2;
extern Version3 g_stVer3;
extern IP		g_stIp;
extern IP2		g_stIp2;

class CClients : public CSocket
{
public:
	CClients();
	~CClients();

	// 소켓...
public:
	inline SOCKET	GetSocket() { return m_sock; }
	inline void		SetSocket(SOCKET socket) { m_sock = socket; }
	inline void		SetSockaddr(SOCKADDR_IN *sockaddr) { memcpy(&m_sockaddr, sockaddr, sizeof(SOCKADDR_IN)); }
	void			RegisterClient(SOCKET, SOCKADDR_IN *);

private:
	XOverlapped		*m_pOv;
	// 전송버퍼...
	char			m_SendBuf[MAX_BUFFERSIZE];
	char			m_RecvBuf[MAX_BUFFERSIZE];

	// 상태...
private:
	int				m_siStatus;
public:
	void			SendInformation1(void);
	void			SendInformation2(void);
	void			SendInformation3(VersionInf *pVersion);
	void			Logout();
	void			RequestRecv();
	XOverlapped		*GetOverlapped();
	enum			STATUS {CONNECT,DISCONNECT};
	inline int		GetStatus() { return m_siStatus; }
	inline void		SetStatus(STATUS Status) { m_siStatus = Status; }

public:
	bool isConnect();
	CRITICAL_SECTION	m_cs;
	void	VerLock();
	void	VerUnlock();

};

#endif // !defined(AFX_CLIENTS_H__F740629F_9A03_442D_B848_4BD0AD02B966__INCLUDED_)
