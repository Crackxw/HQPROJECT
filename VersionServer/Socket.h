// Socket.h: interface for the CSocket class.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKET_H__60894DB8_CAF5_441C_821D_DD702232D5C6__INCLUDED_)
#define AFX_SOCKET_H__60894DB8_CAF5_441C_821D_DD702232D5C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "Ws2_32")

#include <winsock2.h>
#include <windows.h>
#include <iostream.h>

class CSocket  
{
public:	// �����ڿ� �Ҹ���.
	CSocket();
	~CSocket();

public:
	VOID	ViewErrMsg(int);
	enum	{TCP,UDP};

public:	// ���� ���̺귯�� 2.2�����ʱ�ȭ�� ����.
	VOID	StartWinsock();
	VOID	EndWinsock();

public:
	VOID	ReleaseSocket();
	int		Recv(VOID *, int);
	BOOL	Connect();
	VOID	InitSocket(int socktype, char *cip, WORD port);
	VOID	Bind();
	VOID	Listen(int backlog);
	SOCKET	Accept();
	int		Send(VOID *, int);

protected:
	SOCKET		m_sock;
	SOCKADDR_IN	m_sockaddr;
};

#endif // !defined(AFX_SOCKET_H__60894DB8_CAF5_441C_821D_DD702232D5C6__INCLUDED_)
