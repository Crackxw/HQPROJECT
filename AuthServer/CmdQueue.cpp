

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: CmdQueue.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "CmdQueue.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////

CCmdQueue::CCmdQueue() : m_bInited(FALSE)
{
	Initialize();
}

//////////////////////////////////////////////////////////////////////

CCmdQueue::~CCmdQueue()
{
	Clear();

	::DeleteCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////

BOOL CCmdQueue::Initialize()
{
	::InitializeCriticalSection(&m_csLock);
	m_bInited = TRUE;

	Clear();
	return true;
}

//////////////////////////////////////////////////////////////////////

void CCmdQueue::Push(void* _pvData)
{
	Enter();
	assert(m_bInited);	
	m_Queue.push(_pvData);
	Leave();
}

//////////////////////////////////////////////////////////////////////

void CCmdQueue::Pop()
{
	Enter();
	assert(m_bInited);
	
	m_Queue.pop();	

	Leave();	
}

//////////////////////////////////////////////////////////////////////

void* CCmdQueue::Get()
{
	Enter();
	assert(m_bInited);

	void* pvData = NULL;

	if(IsEmpty()){
		pvData = m_Queue.front();
	}

	Leave();
	return pvData;
}

//////////////////////////////////////////////////////////////////////

BOOL CCmdQueue::IsEmpty()
{
	Enter();
	BOOL bRetVal = m_Queue.empty();
	Leave();

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

int CCmdQueue::GetCount()
{
	return (int)m_Queue.size();
}

//////////////////////////////////////////////////////////////////////

void CCmdQueue::Clear()
{	
	Enter();

	assert(m_bInited);

	while(!IsEmpty()){
//		void* pvData=m_Queue.front();
//		delete pvData;
		m_Queue.pop();
	}

	Leave();
}

//////////////////////////////////////////////////////////////////////