
////////////////////////////////////////////////////////////////

#ifndef __CRASH_INFO_H_INCLUDED__
#define __CRASH_INFO_H_INCLUDED__

////////////////////////////////////////////////////////////////

#include <list>

////////////////////////////////////////////////////////////////

#include "..\\CrashMailProtocol\\CrashMailProtocol.h"

////////////////////////////////////////////////////////////////

class CCrashData
{
	friend class CCrashInfo;

public : 
	CCrashData()	{}	
	~CCrashData()	{}

	CCrashData(const CCrashData& rhs);

public :
	CCrashData& operator=(const CCrashData& rhs);	
	void		Set(const ON_RESP_SEARCHCRASHINFO& rhs);	

public :
	DWORD	dwID;
	SHORT	nCrashType;	
	char	szCrashContents[64];
	char	szOSType[64];
	char	szProcessType[64];	
	char	szIP[20];
	SHORT	nProcessNumber;
	SHORT	nProcessLevel;
	LONG	nPageSize;
	char	szsTime[64];
	char	szeTime[64];
	char	szCrashAddress[16];
	char	szLineAddress[16];
	char	szNationCode[64];
	char	szSupposition[256];
};

////////////////////////////////////////////////////////////////

class CCrashInfo
{
public : 
	CCrashInfo();
	virtual ~CCrashInfo();

public : 
	virtual void		Add			(CCrashData Data);
	virtual BOOL		Remove		(DWORD dwKey);
	virtual void		RemoveAll	();
	virtual CCrashData	Find		(DWORD dwKey);
	virtual int			Count		();	
	
	virtual std::list<CCrashData>::iterator& 
		GetNext(std::list<CCrashData>::iterator& it);

	std::list<CCrashData>::iterator& GetFirst();
	virtual BOOL		IsEnd		(std::list<CCrashData>::iterator& it);
	CCrashData			Get			(std::list<CCrashData>::iterator& it);
	BOOL				Gets		(CCrashData* pData, int& nCount, int nMax, int nPage);

private : 
	void			Enter()		{ ::EnterCriticalSection(&m_csLock); }
	void			Leave()		{ ::LeaveCriticalSection(&m_csLock); }

private : 
	CRITICAL_SECTION		m_csLock;

private : 
	std::list<CCrashData>	m_list;
};

////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////