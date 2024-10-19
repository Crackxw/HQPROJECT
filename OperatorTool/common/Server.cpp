// Server.cpp : implementation file
//

#include <afxsock.h>
#include "defines.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServer

CServer::CServer()
{
	CAsyncSocket::CAsyncSocket();
}

CServer::~CServer()
{
	for(list<CClient*>::iterator i=m_ClientSocket.begin(); i!=m_ClientSocket.end(); ++i)
	{
		(*i)->Close();
		delete (*i);
	}

	m_ClientSocket.clear();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServer, CAsyncSocket)
	//{{AFX_MSG_MAP(CServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServer member functions

void CServer::OnAccept(int nErrorCode)
{
	// 새로운 client가 연결됨
	CClient *pNewClient = new CClient;

	Accept(*pNewClient);

	pNewClient->SetHwnd(m_hWnd);

	m_ClientSocket.push_back(pNewClient);
}
