// MyCriticalSection.cpp: implementation of the CMyCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <assert.h>
#include "MyCriticalSection.h"


int				  CMyCriticalSection::m_nMaxCount		 = 0;
CRITICAL_SECTION* CMyCriticalSection::m_pCriticalSection = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCriticalSection::CMyCriticalSection() 
{
}

CMyCriticalSection::~CMyCriticalSection()
{
}

void CMyCriticalSection::InitializeCriticalSection(const int nCount)
{
	assert(nCount > 0);
	m_nMaxCount = nCount;
	
	m_pCriticalSection = new CRITICAL_SECTION[nCount];
	for( int i = 0; i < m_nMaxCount; ++i)
	{
		::InitializeCriticalSection(&m_pCriticalSection[i]);
	}
}

void CMyCriticalSection::DeleteCriticalSection()
{
	for( int i = 0; i < m_nMaxCount; ++i)
	{
		::DeleteCriticalSection(&m_pCriticalSection[i]);
	}

	if( m_pCriticalSection)
	{
		delete []m_pCriticalSection;
		m_pCriticalSection = NULL;
	}
}

void CMyCriticalSection::EnterCriticalSection(const int nIndex)
{
	::EnterCriticalSection(&m_pCriticalSection[nIndex]);
}

void CMyCriticalSection::LeaveCriticalSection(const int nIndex)
{
	::LeaveCriticalSection(&m_pCriticalSection[nIndex]);
}