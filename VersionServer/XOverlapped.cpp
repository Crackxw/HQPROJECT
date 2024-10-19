// XOverlapped.cpp: implementation of the XOverlapped class.
//
//////////////////////////////////////////////////////////////////////

#include "XOverlapped.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XOverlapped::XOverlapped()
{
	m_wsaBuf.buf = m_cBuf;
	m_wsaBuf.len = MAX_BUFFERSIZE;
}

XOverlapped::~XOverlapped()
{

}

void XOverlapped::Clear()
{
	ZeroMemory(&m_ov, sizeof(WSAOVERLAPPED));
}

void XOverlapped::SetRecv()
{
	m_type = RECV_STATUS;
}

void XOverlapped::SetSend()
{
	m_type = SEND_STATUS;
}

void XOverlapped::SetAccept()
{
	m_type = ACCEPT_STATUS;
}

void XOverlapped::SetEnd()
{
	m_type = END_STATUS;
}

bool XOverlapped::IsRecv()
{
	return (m_type == RECV_STATUS) ? true: false;
}


bool XOverlapped::IsSend()
{
	return (m_type == SEND_STATUS) ? true: false;
}


bool XOverlapped::IsAccept()
{
	return (m_type == ACCEPT_STATUS) ? true: false;
}

bool XOverlapped::IsEnd()
{
	return (m_type == END_STATUS) ? true: false;
}
