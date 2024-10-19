// MyPacketQueue.cpp: implementation of the CMyPacketQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerMonitor.h"
#include "MyPacketQueue.h"
#include "MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPacketQueue::CMyPacketQueue()
{

	InitializeCriticalSection(&m_crit);
	m_pHead = NULL;
	m_pTail = NULL;

	m_siPacketNum = 0;

}

CMyPacketQueue::~CMyPacketQueue()
{
	DeleteCriticalSection(&m_crit);

}


int CMyPacketQueue::Enqueue(CMyPacket *pPacket)
{
	if (pPacket == NULL) return 0 ;

	EnterCriticalSection(&m_crit);

	if (m_pHead == NULL && m_pTail == NULL)
	{
		m_pHead = pPacket;
		m_pTail = pPacket;
	}
	else 
	{
		m_pHead->LinkPrev(pPacket);
		pPacket->LinkNext(m_pHead);

		m_pHead = pPacket;

	}

	m_siPacketNum++;

	LeaveCriticalSection(&m_crit);
	return 1;

}

CMyPacket *CMyPacketQueue::Dequeue()
{

	EnterCriticalSection(&m_crit);
	if (m_pTail == NULL) {
		LeaveCriticalSection(&m_crit);
		return NULL;
	}

	CMyPacket *ret = m_pTail;
	

	// 하나남았으면
	if (m_pTail == m_pHead)
	{
		m_pHead = NULL;
		m_pTail = NULL;
		m_siPacketNum--;
		LeaveCriticalSection(&m_crit);
		return ret;
	}
	else
	{
		CMyPacket *prev = m_pTail->GetPrev();
		m_pTail = prev;
		// 연결 끊음
		m_pTail->LinkNext(NULL);
		ret->LinkPrev(NULL);

		m_siPacketNum--;
		LeaveCriticalSection(&m_crit);
		return ret;
		
	}

	

}

void CMyPacketQueue::Clear()
{
	EnterCriticalSection(&m_crit);
	CMyPacket *node = m_pHead;
	CMyPacket *temp;

	while (node != NULL)
	{
		temp = node->GetNext();
		delete node;
		node = temp;

	}

	m_siPacketNum = 0;

	m_pHead = NULL;
	m_pTail = NULL;

	LeaveCriticalSection(&m_crit);


}

