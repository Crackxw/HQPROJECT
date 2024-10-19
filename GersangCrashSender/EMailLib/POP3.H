/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: POP3.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __SMPT_H_INCLUDED__
#define __SMPT_H_INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxsock.h>
#include "MailMessage.h"
#include <list>
#include <queue>
#include <string>

#define SMTP_PORT 25		// Standard port for SMTP servers
#define RESPONSE_BUFFER_SIZE 1024

class pairValue 
{
public : 
	pairValue()		{}
	~pairValue()	{}
	pairValue(const pairValue& rhs)
	{
		idx = rhs.idx;
		size = rhs.size;
		from = rhs.from;
		message = rhs.message;
		subject = rhs.subject;
	}
public : 
    int idx;
    int size;
	std::string from;
	std::string message;
	std::string subject;
};

class CPOP3  
{
public:
	CPOP3( LPCTSTR szSMTPServerName, UINT nPort = SMTP_PORT );
	virtual ~CPOP3();

	void SetServerProperties( LPCTSTR szSMTPServerName, UINT nPort = SMTP_PORT );
	CString GetLastError();
	UINT GetPort();
	BOOL Disconnect();
	BOOL Connect(TCHAR szID[256], TCHAR szPass[256]);
	virtual BOOL FormatMailMessage( CMailMessage* msg );
	BOOL SendMessage( CMailMessage* msg );
	CString GetServerHostName();

   void Cancel(); // <JFO>

private:
	BOOL get_response( UINT response_expected );
	CString cook_body( CMailMessage* msg );

	CString m_sError;
	BOOL m_bConnected;
	UINT m_nPort;
	CString m_sSMTPServerHostName;
	CSocket m_wsSMTPServer;

protected:
	virtual BOOL transmit_message( CMailMessage* msg );

public : 
	std::queue<pairValue>	m_queData;
	void MailContents(const char* pPacket);


// Helper Code
	struct response_code
	{
		UINT nResponse;	
		TCHAR* sMessage;
	};

	enum eResponse
	{
		GENERIC_SUCCESS = 0,
		CONNECT_SUCCESS,
		DATA_SUCCESS,
		QUIT_SUCCESS,		
		LAST_RESPONSE
	};
	TCHAR *response_buf;
	static response_code response_table[];
};

#endif
