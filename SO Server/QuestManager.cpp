

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: QuestManager.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "QuestManager.h"

////////////////////////////////////////////////////////////////////

CQuestData::CQuestData() : 
m_uiAccount(0),
m_uiSlot(0),
m_siQuestCode(0)
{

}

////////////////////////////////////////////////////////////////////

CQuestData::CQuestData(const CQuestData& rhs)
{
	m_uiAccount		= rhs.m_uiAccount;
	m_uiSlot		= rhs.m_uiSlot;
	m_siQuestCode	= rhs.m_siQuestCode;	
}

////////////////////////////////////////////////////////////////////

CQuestData::~CQuestData()
{

}

////////////////////////////////////////////////////////////////////

CQuestData& CQuestData::operator=(const CQuestData& rhs)
{
	if(this == &rhs)
		return *this;

	m_uiAccount		= rhs.m_uiAccount;
	m_uiSlot		= rhs.m_uiSlot;
	m_siQuestCode	= rhs.m_siQuestCode;	

	return *this;
}

////////////////////////////////////////////////////////////////////

void CQuestData::Set(UI32 uiAccount, UI08 uiSlot, SI16 siQuestCode)
{
	m_uiAccount		= uiAccount;
	m_uiSlot		= uiSlot;
	m_siQuestCode	= siQuestCode;
}

////////////////////////////////////////////////////////////////////

CQuestManager::CQuestManager()
{
	::InitializeCriticalSection(&m_csLock);
}

////////////////////////////////////////////////////////////////////

CQuestManager::~CQuestManager()
{
	ClearAllQuest();

	::DeleteCriticalSection(&m_csLock);
}

////////////////////////////////////////////////////////////////////

UI32 CQuestManager::GetCompletedQuestCount()
{
	Enter();
	UI32 uiCount = (UI32)m_listCompletedQuest.size();
	Leave();

	return uiCount;
}

////////////////////////////////////////////////////////////////////

void CQuestManager::AddCompletedQuest(UI32 uiAccount, UI08 uiSlot, SI16 siQuestCode)
{
	Enter();
	CQuestData Data;
	Data.Set(uiAccount, uiSlot, siQuestCode);

	m_listCompletedQuest.push_back(Data);
	Leave();
}

////////////////////////////////////////////////////////////////////

void CQuestManager::ClearAllQuest()
{
	Enter();
	m_listCompletedQuest.erase(m_listCompletedQuest.begin(), 
		m_listCompletedQuest.end());
	Leave();
}

////////////////////////////////////////////////////////////////////