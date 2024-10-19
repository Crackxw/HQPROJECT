

#include "StdAfx.h"
#include "CrashInfo.h"

////////////////////////////////////////////////////////////////
// Crash Data

CCrashData::CCrashData(const CCrashData& rhs)
{
	dwID = rhs.dwID;
	nCrashType = rhs.nCrashType;
	strncpy(szCrashContents, rhs.szCrashContents, sizeof(szCrashContents));
	strncpy(szOSType, rhs.szOSType, sizeof(szOSType));
	strncpy(szProcessType, rhs.szProcessType, sizeof(szProcessType));
	strncpy(szIP, rhs.szIP, sizeof(szIP));
	nProcessNumber = rhs.nProcessNumber;
	nProcessLevel = rhs.nProcessLevel;
	nPageSize = rhs.nPageSize;
	strncpy(szsTime, rhs.szsTime, sizeof(szsTime));
	strncpy(szeTime, rhs.szeTime, sizeof(szeTime));
	strncpy(szCrashAddress, rhs.szCrashAddress, sizeof(szCrashAddress));
	strncpy(szLineAddress, rhs.szLineAddress, sizeof(szLineAddress));
	strncpy(szNationCode, rhs.szNationCode, sizeof(szNationCode));
	strncpy(szSupposition, rhs.szSupposition, sizeof(szSupposition));
}

////////////////////////////////////////////////////////////////

CCrashData& CCrashData::operator=(const CCrashData& rhs)
{
	if(this == &rhs)
		return *this;

	dwID = rhs.dwID;
	nCrashType = rhs.nCrashType;
	strncpy(szCrashContents, rhs.szCrashContents, sizeof(szCrashContents));
	strncpy(szOSType, rhs.szOSType, sizeof(szOSType));
	strncpy(szProcessType, rhs.szProcessType, sizeof(szProcessType));
	strncpy(szIP, rhs.szIP, sizeof(szIP));
	nProcessNumber = rhs.nProcessNumber;
	nProcessLevel = rhs.nProcessLevel;
	nPageSize = rhs.nPageSize;
	strncpy(szsTime, rhs.szsTime, sizeof(szsTime));
	strncpy(szeTime, rhs.szeTime, sizeof(szeTime));
	strncpy(szCrashAddress, rhs.szCrashAddress, sizeof(szCrashAddress));
	strncpy(szLineAddress, rhs.szLineAddress, sizeof(szLineAddress));
	strncpy(szNationCode, rhs.szNationCode, sizeof(szNationCode));
	strncpy(szSupposition, rhs.szSupposition, sizeof(szSupposition));
	
	return *this;
}

////////////////////////////////////////////////////////////////

void CCrashData::Set(const ON_RESP_SEARCHCRASHINFO& rhs)
{
	dwID = rhs.dwID;
	nCrashType = rhs.nCrashType;
	strncpy(szCrashContents, rhs.szCrashContents, sizeof(szCrashContents));
	strncpy(szOSType, rhs.szOSType, sizeof(szOSType));
	strncpy(szProcessType, rhs.szProcessType, sizeof(szProcessType));
	strncpy(szIP, rhs.szIP, sizeof(szIP));
	nProcessNumber = rhs.nProcessNumber;
	nProcessLevel = rhs.nProcessLevel;
	nPageSize = rhs.nPageSize;
	strncpy(szsTime, rhs.szsTime, sizeof(szsTime));
	strncpy(szeTime, rhs.szeTime, sizeof(szeTime));
	strncpy(szCrashAddress, rhs.szCrashAddress, sizeof(szCrashAddress));
	strncpy(szLineAddress, rhs.szLineAddress, sizeof(szLineAddress));
	strncpy(szNationCode, rhs.szNationCode, sizeof(szNationCode));
	strncpy(szSupposition, rhs.szSupposition, sizeof(szSupposition));
}

////////////////////////////////////////////////////////////////
// CCrashInfo
CCrashInfo::CCrashInfo()
{
	::InitializeCriticalSection(&m_csLock);
}

////////////////////////////////////////////////////////////////

CCrashInfo::~CCrashInfo()
{
	RemoveAll();

	::DeleteCriticalSection(&m_csLock);
}

////////////////////////////////////////////////////////////////

void CCrashInfo::Add(CCrashData Data)
{
	Enter();
	m_list.push_back(Data);
	Leave();
}

////////////////////////////////////////////////////////////////

CCrashData CCrashInfo::Find(DWORD dwKey)
{
	Enter();
	std::list<CCrashData>::iterator	itCur = m_list.begin();
	CCrashData Data;

	while(itCur != m_list.end())
	{
		Data = (*itCur);
		if(Data.dwID == dwKey)
		{
			Leave();
			return Data;
		}
	}
	
	Data.dwID = 0;
	Leave();
	return Data;
}

////////////////////////////////////////////////////////////////

CCrashData CCrashInfo::Get(std::list<CCrashData>::iterator& it)
{
	Enter();
	CCrashData Data;
	
	Data = (*it);
		
	Data.dwID = 0;
	Leave();
	return Data;
}

////////////////////////////////////////////////////////////////

BOOL CCrashInfo::Gets(CCrashData* pData, int& nCount, int nMax, int nPage)
{
	Enter();

	std::list<CCrashData>::iterator	itCur;
	int		nMove = 0;

	///////////////////////////////////////////////////////
	// 시작 위치를 구한다.
	if(nPage == 1)
		itCur = m_list.begin();
	
	///////////////////////////////////////////////////////

	else
	{
		itCur = m_list.begin();

		while(itCur != m_list.end())
		{
			++itCur;

			if(++nMove >= nMax*nPage)
				break;
		}
	}

	///////////////////////////////////////////////////////

	BOOL bRetVal = TRUE;
	nCount = 0;
	
	while(itCur != m_list.end())
	{
		pData[nCount] = (*itCur);

		++itCur;

		if(++nCount >= nMax)
			break;
	}

	Leave();

	return bRetVal;
}

////////////////////////////////////////////////////////////////

std::list<CCrashData>::iterator& CCrashInfo::GetNext(std::list<CCrashData>::iterator& it)
{
	Enter();
	++it;
	Leave();

	return it;
}

////////////////////////////////////////////////////////////////

std::list<CCrashData>::iterator& CCrashInfo::GetFirst()
{
	Enter();
	std::list<CCrashData>::iterator itCur = m_list.begin();
	Leave();

	return itCur;
}

////////////////////////////////////////////////////////////////

BOOL CCrashInfo::IsEnd(std::list<CCrashData>::iterator& it)
{
	return m_list.end() == it ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////

BOOL CCrashInfo::Remove(DWORD dwKey)
{
	Enter();
	std::list<CCrashData>::iterator	itCur = m_list.begin();
	CCrashData Data;
	BOOL bRetVal = FALSE;

	while(itCur != m_list.end())
	{
		Data = (*itCur);
		if(Data.dwID == dwKey)
		{
			itCur = m_list.erase(itCur);
			bRetVal = TRUE;
			break;
		}
		else
		{
			++itCur;
		}
	}
	
	Leave();	
	return bRetVal;
}

////////////////////////////////////////////////////////////////

void CCrashInfo::RemoveAll()
{
	Enter();

	m_list.erase(m_list.begin(), m_list.end());

	Leave();
}

////////////////////////////////////////////////////////////////

int CCrashInfo::Count()
{
	Enter();

	int nCount = (int)m_list.size();

	Leave();
	
	return nCount;
}

////////////////////////////////////////////////////////////////