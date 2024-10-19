// MyPacketQueue.h: interface for the CMyPacketQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKETQUEUE_H__DEDBC4AD_6486_4BE1_A53E_C13287CF5C5D__INCLUDED_)
#define AFX_MYPACKETQUEUE_H__DEDBC4AD_6486_4BE1_A53E_C13287CF5C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyPacket;

class CMyPacketQueue  
{
public:
	void Clear();

	CMyPacket *Dequeue();
	int Enqueue(CMyPacket *);

	CMyPacketQueue();
	virtual ~CMyPacketQueue();
private:

	CRITICAL_SECTION m_crit;

	int m_siPacketNum;
	CMyPacket *m_pHead;
	CMyPacket *m_pTail;
	

};

#endif // !defined(AFX_MYPACKETQUEUE_H__DEDBC4AD_6486_4BE1_A53E_C13287CF5C5D__INCLUDED_)
