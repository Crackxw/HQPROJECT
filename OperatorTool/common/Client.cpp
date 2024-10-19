// Client.cpp : implementation file
//

#include <afxsock.h>
#include "defines.h"
#include "Client.h"

#include "Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClient

CClient::CClient()
{
	CAsyncSocket::CAsyncSocket();
}

CClient::~CClient()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClient, CAsyncSocket)
	//{{AFX_MSG_MAP(CClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClient member functions
void CClient::OnConnect(int nErrorCode)
{
	SendMessage(m_hWnd, WM_CONNECT, nErrorCode == 0 ? TRUE : FALSE, 0);
}

void CClient::OnReceive(int nErrorCode)
{
	char TempBuffer[MSG_SIZE];

	Receive((void*)TempBuffer, MSG_SIZE);

	OnBaseMsg	*pMsg = (OnBaseMsg*)TempBuffer;

	SendMessage(m_hWnd, pMsg->GetType(), (WPARAM)this, (LPARAM)TempBuffer);
}
