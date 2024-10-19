// MyCriticalSection.h: interface for the CMyCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCRITICALSECTION_H__21D74BAD_0A1F_4C3B_B099_B7C0AA95920C__INCLUDED_)
#define AFX_MYCRITICALSECTION_H__21D74BAD_0A1F_4C3B_B099_B7C0AA95920C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyCriticalSection  
{
public:
	CMyCriticalSection();
	virtual ~CMyCriticalSection();

public:
	static void InitializeCriticalSection(const int nCount);
	static void EnterCriticalSection(const int nIndex);
	static void LeaveCriticalSection(const int nIndex);
	static void DeleteCriticalSection();


private:
	static CRITICAL_SECTION* m_pCriticalSection;
	static int			     m_nMaxCount;
};

#endif // !defined(AFX_MYCRITICALSECTION_H__21D74BAD_0A1F_4C3B_B099_B7C0AA95920C__INCLUDED_)
