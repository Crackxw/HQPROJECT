// ClientSocket.h: interface for the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKET_H__E1288145_7A60_4E13_A603_87930E147434__INCLUDED_)
#define AFX_CLIENTSOCKET_H__E1288145_7A60_4E13_A603_87930E147434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClientSocket  
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	bool IsConnected();

private:
	bool m_bConnect;
};

#endif // !defined(AFX_CLIENTSOCKET_H__E1288145_7A60_4E13_A603_87930E147434__INCLUDED_)
