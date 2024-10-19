
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: QuestManager.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

////////////////////////////////////////////////////////////////////

#ifndef __QUEST_MANAGER_H_INCLUDED__
#define __QUEST_MANAGER_H_INCLUDED__

#include <windows.h>
#include <list>
#include "..\\XCommon\\jxdef.h"

class CQuestData
{
	friend class CQuestManager;

public : 
	CQuestData();
	CQuestData(const CQuestData& rhs);
	~CQuestData();

private : 
	CQuestData& operator=(const CQuestData& rhs);

public : 
	void	Set(UI32 uiAccount, UI08 uiSlot, SI16 siQuestCode);

private : 
	SI16				m_siQuestCode;
	UI32				m_uiAccount;
	UI08				m_uiSlot;
};

////////////////////////////////////////////////////////////////////

class CQuestManager
{
public : 
	CQuestManager();
	~CQuestManager();

public : 
	UI32	GetCompletedQuestCount	(void);
	void	AddCompletedQuest		(UI32 uiAccount, UI08 uiSlot, SI16 siQuestCode);
	void	ClearAllQuest			(void);

private : 
	void	Enter()		{ ::EnterCriticalSection(&m_csLock); }
	void	Leave()		{ ::LeaveCriticalSection(&m_csLock); }

private : 	
	std::list<CQuestData>	m_listCompletedQuest;

private : 
	CRITICAL_SECTION		m_csLock;
};

////////////////////////////////////////////////////////////////////

#endif // __QUEST_MANAGER_H_INCLUDED__

////////////////////////////////////////////////////////////////////