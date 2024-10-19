#include <windows.h>
#include <conio.h>
#include <stdio.h>


// CWorldInfoMgr
#include "../kojimodo/utility/Singleton.h"
#include "WorldInfoMgr.h"






//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CWorldInfoMgr::CWorldInfoMgr(): m_numWorld(0)
{
	using namespace FRONT;

	
	m_WorldList = new sWorldInfo2[MAX_WORLD];
	memset(m_WorldList, 0, sizeof(sWorldInfo2) * MAX_WORLD);		
	

	InitializeCriticalSection(&m_csWorldList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CWorldInfoMgr::~CWorldInfoMgr()
{
	
	if( m_WorldList != NULL ) {

		delete m_WorldList;
	}


	DeleteCriticalSection(&m_csWorldList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� �����ϰ� �ִ� ������� ������ ��´� 
//--------------------------------------------------------------------------------------------
int CWorldInfoMgr::getWorldsInfo(FRONT::sWorldInfo* worldsInfo)
{
	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i)
	{
						
		worldsInfo[i].world_id	  = m_WorldList[i].world_id;
		worldsInfo[i].world_state = m_WorldList[i].world_state;
		worldsInfo[i].concurrent  = m_WorldList[i].concurrent;		
		worldsInfo[i].create_date = m_WorldList[i].create_date;				
		worldsInfo[i].setName( (char*)m_WorldList[i].name );
				
	}
	
	int retValue = m_numWorld;

	LeaveCriticalSection(&m_csWorldList);

	return retValue;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ������ ����Ʈ�� �߰� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::addWorld(FRONT::sWorldInfo2* pWorldInfo)
{	
	EnterCriticalSection(&m_csWorldList);

	if( m_numWorld < FRONT::MAX_WORLD )
	{

		/* �̹� �ش� ������ ������ ���� ��� ���� */

		if( isValidWorld( pWorldInfo->world_id ) == FALSE )  
		{				
			memcpy(&m_WorldList[m_numWorld], pWorldInfo, sizeof(FRONT::sWorldInfo2));

			printf( "--------------------------- WORLD ADD ---------------------------\n" );
			printf( "ID: %d\n",					m_WorldList[m_numWorld].world_id );
			printf( "NAME: %s\n",				m_WorldList[m_numWorld].name );
			
			printf( "CREATE_DAY: %d:%d:%d\n",	m_WorldList[m_numWorld].getYear(), 
												m_WorldList[m_numWorld].getMonth(),
												m_WorldList[m_numWorld].getDay() );
			
			printf( "GATE_IP: %s\n",			m_WorldList[m_numWorld].gateIP );
			printf( "GATE_PORT: %d\n",			m_WorldList[m_numWorld].gatePort );
			printf( "CHAT_IP: %s\n",			m_WorldList[m_numWorld].chatIP );
			printf( "CHAT_PORT: %d\n",			m_WorldList[m_numWorld].chatPort );
			printf( "-----------------------------------------------------------------\n" );

			++m_numWorld;

			LeaveCriticalSection(&m_csWorldList);

			return TRUE;
		}
	}

	LeaveCriticalSection(&m_csWorldList);

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ������ ����Ʈ���� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CWorldInfoMgr::removeWorld(int world_id)
{
	int index = -1; 
	
	EnterCriticalSection(&m_csWorldList);

	if( (index = isValidWorld(world_id)) >= 0 )
	{					
		int size = sizeof(m_WorldList[0]) *  ( m_numWorld -  index - 1 );

		if( size > 0) {

			memcpy(&m_WorldList[index], &m_WorldList[index + 1], size);
		}

		--m_numWorld;		
	}

	LeaveCriticalSection(&m_csWorldList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ������ ������ ���´� 
//--------------------------------------------------------------------------------------------
WORD CWorldInfoMgr::getNumWorld()
{
	return m_numWorld;	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ش��ϴ� ���尡 ����Ʈ�� �ִ��� �˾� ���� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::isValidWorld(int world_id)
{
	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {
			
			LeaveCriticalSection(&m_csWorldList);
			
			return TRUE; 
		}
	}

	LeaveCriticalSection(&m_csWorldList);

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ش� ������ ����Ʈ ���� �����Ǹ� ���´� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::getGateIP(int world_id, char* gateIP)
{	
	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {
			
			/* value copy, because thread sync */
			memcpy( gateIP, m_WorldList[i].gateIP, sizeof(m_WorldList[i].gateIP) );

			LeaveCriticalSection(&m_csWorldList);

			return TRUE;
		}
	}

	LeaveCriticalSection(&m_csWorldList);

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ش� ������ ����Ʈ ���� ��Ʈ�� ��� �´� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::getGatePort(int world_id, WORD* gatePort)
{	
	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {
			
			/* value copy, because thread sync */
			*gatePort = m_WorldList[i].gatePort;
						
			LeaveCriticalSection(&m_csWorldList);

			return TRUE;
		}
	}

	LeaveCriticalSection(&m_csWorldList);

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ش� ������ äƮ ���� �����Ǹ� ���´� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::getChatIP(int world_id, char* chatIP)
{
	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {

			/* value copy, because thread sync */
			memcpy( chatIP, m_WorldList[i].chatIP, sizeof(m_WorldList[i].chatIP) );
			
			LeaveCriticalSection(&m_csWorldList);

			return TRUE;
		}
	}

	LeaveCriticalSection(&m_csWorldList);

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ش� ������ äƮ ���� ��Ʈ�� ��� �´� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::getChatPort(int world_id, WORD* chatPort)
{	
	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {
			
			/* value copy, because thread sync */
			*chatPort = m_WorldList[i].chatPort;
						
			LeaveCriticalSection(&m_csWorldList);

			return TRUE;
		}
	}

	LeaveCriticalSection(&m_csWorldList);

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ش� ������ ���� �����ڼ��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::setConcurrent(int world_id, int concurrent)
{

	EnterCriticalSection(&m_csWorldList);

	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {
		
			m_WorldList[i].concurrent = concurrent;			
						

			LeaveCriticalSection(&m_csWorldList);			
			return TRUE;
		}
	}

	LeaveCriticalSection(&m_csWorldList);
	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
bool CWorldInfoMgr::setWorldState(int world_id, int state)
{

	EnterCriticalSection(&m_csWorldList);
	
	for(int i=0; i < m_numWorld; ++i) {

		if( m_WorldList[i].world_id == world_id ) {
		
			m_WorldList[i].world_state = state;


			LeaveCriticalSection(&m_csWorldList);
			return TRUE;		
		}
	}

	LeaveCriticalSection(&m_csWorldList);
	return FALSE;
}


