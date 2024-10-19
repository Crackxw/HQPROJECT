// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__E4A4A588_23E0_4A5B_9F64_E47A43AA0014__INCLUDED_)
#define AFX_MYPACKET_H__E4A4A588_23E0_4A5B_9F64_E47A43AA0014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyPacket  
{
public:
	CMyPacket();
	virtual ~CMyPacket();

	void Pack(FILE *fp,WORD command,WORD len);
	void Pack(void *buf,WORD command,WORD len);
	void Pack(void *buf,int len);
	void Expand(char *buf);

	

	void LinkNext(CMyPacket *);
	void LinkPrev(CMyPacket *);

	void SetLength(int length);
	WORD GetLength();
	char *GetBuffer() {return m_pbBuf;};


	CMyPacket *GetPrev() {return m_pPrev;};
	CMyPacket *GetNext() {return m_pNext;};


private:

	
	char *m_pbBuf;
	CMyPacket * m_pPrev;
	CMyPacket * m_pNext;
	

};

#endif // !defined(AFX_MYPACKET_H__E4A4A588_23E0_4A5B_9F64_E47A43AA0014__INCLUDED_)
