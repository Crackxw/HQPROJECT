
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: CmdQueue.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __COMMAND_QUEUE_H_INCLUDED__
#define __COMMAND_QUEUE_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <queue>
#include <list>

//////////////////////////////////////////////////////////////////////

class CCmdQueue
{
public : 
	CCmdQueue();
	~CCmdQueue();

public : 
	void	Push(void* vpData);
	void	Pop();
	void*	Get();
	BOOL	IsEmpty();
	BOOL	Initialize();
	void	Clear();
	int		GetCount();

private : 
	void	Enter()		{ ::EnterCriticalSection(&m_csLock); }
	void	Leave()		{ ::LeaveCriticalSection(&m_csLock); }

private : 
	std::queue<void*>	m_Queue;

	CRITICAL_SECTION	m_csLock;
	BOOL				m_bInited;
};

//////////////////////////////////////////////////////////////////////

#endif // __COMMAND_QUEUE_H_INCLUDED__

//////////////////////////////////////////////////////////////////////