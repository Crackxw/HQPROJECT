// XOverlapped.h: interface for the XOverlapped class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XOVERLAPPED_H__3295FE60_DED2_4566_826F_EAC06CC34781__INCLUDED_)
#define AFX_XOVERLAPPED_H__3295FE60_DED2_4566_826F_EAC06CC34781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "VSDefine.h"

struct XOverlapped  
{
public:
	XOverlapped();
	~XOverlapped();
public:
	WSAOVERLAPPED	m_ov;
	DWORD			m_type;
	WSABUF			m_wsaBuf;
	char			m_cBuf[MAX_BUFFERSIZE];

	enum _type { SEND_STATUS, RECV_STATUS, ACCEPT_STATUS, END_STATUS };
public:
	void SetSend();
	void SetRecv();
	void SetAccept();
	void SetEnd();

	bool IsRecv();
	bool IsSend();
	bool IsAccept();
	bool IsEnd();

	void Clear();
};

#endif // !defined(AFX_XOVERLAPPED_H__3295FE60_DED2_4566_826F_EAC06CC34781__INCLUDED_)
