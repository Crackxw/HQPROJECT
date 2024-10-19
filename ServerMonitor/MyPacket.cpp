// MyPacket.cpp: implementation of the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerMonitor.h"
#include "MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPacket::CMyPacket()
{
	m_pPrev = NULL;
	m_pNext = NULL;

	m_pbBuf = NULL;


}

CMyPacket::~CMyPacket()
{
	if (m_pbBuf)
	{
		free(m_pbBuf);
		m_pbBuf = NULL;
	}

}

WORD CMyPacket::GetLength()
{
	if (m_pbBuf == NULL) return 0; 
	WORD *pLength = (WORD *)m_pbBuf;
	return *pLength;
}

void CMyPacket::LinkNext(CMyPacket *pPacket)
{
	

	m_pNext = pPacket;



	
	
}

void CMyPacket::LinkPrev(CMyPacket *pPacket)
{
	

	m_pPrev = pPacket;



	
	
}

void CMyPacket::SetLength(int length)
{
	if (m_pbBuf) {
		free(m_pbBuf);
	}
	m_pbBuf = (char *)malloc(length + 2);
	WORD *pLength = (WORD *)m_pbBuf;
	*pLength = length + 2;

	
	
}

void CMyPacket::Pack(void *szBuf,int len)
{
	if (len <= 0) return;

	if (m_pbBuf) {
		free(m_pbBuf);
	}
	m_pbBuf = (char *)malloc(len + 2);

	memcpy(m_pbBuf,&(((char *)szBuf)[2]),len);
	

}

void CMyPacket::Pack(void *szBuf,WORD command,WORD len)
{
	if (len < 0) return;

	if (m_pbBuf) {
		free(m_pbBuf);
	}
	m_pbBuf = (char *)malloc(len + 4);	
	memcpy(&m_pbBuf[4],szBuf,len);
	WORD *pLength = (WORD *)m_pbBuf;
	*pLength = len + 4;
    pLength++;
	*pLength = command;



}

void CMyPacket::Pack(FILE *fp,WORD command,WORD len)
{
	if (len < 0) return;

	if (m_pbBuf) {
		free(m_pbBuf);
	}
	m_pbBuf = (char *)malloc(len + 4);	
	fread(&m_pbBuf[4],len,1,fp);
	//memcpy(&m_pbBuf[4],szBuf,len);
	WORD *pLength = (WORD *)m_pbBuf;
	*pLength = len + 4;
    pLength++;
	*pLength = command;



}

void CMyPacket::Expand(char *szBuf)
{
	WORD *temp =(WORD *)m_pbBuf;
	memcpy(szBuf,m_pbBuf,*temp);
}


