#ifndef SODATABASE_H
#define SODATABASE_H

#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"
#include "DBGateQueue.h"
#include "DBSubQueue.h"
#include "DBQueryMgr.h"

#include "SOCommonHeader.h"
#include "MSSQLDB.h"
#include "SOUtility.h"
#include "SOPlayer.h"
#include "SOVillage.h"
#include "ParametaBox.h"
#include "ISOMonster.h"
#include "IGuild.h"
#include "SODBStructure.h"




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 데이타베이스 총 관리 클래스  
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CDBQueryMgr;	  
class SODatabase :  public CDBQueryMgr
{
private:
	HQSQLDB		SQLDB;

public:
	SODatabase();																	
	~SODatabase();																	

public:	
	BOOL	ConnectDatabase();
	BOOL	ConnectDatabase2();

	bool	GetItemAuctions(void* pParam);
	bool	GetLoginInfo( DWORD dbAccount, char slot, char *ip, void* outPut );
	BOOL	GetDBInfo(SI32 siInfoKind, LPVOID pIn, LPVOID pOut, UI16 Receiver = 0);		
	BOOL	GetDBInfo2(SI32 siInfoKind, LPVOID pIn, LPVOID pOut, UI16 Receiver = 0);
	BOOL	GetDBInfo3(SI32 siInfoKind, LPVOID pIn, LPVOID pOut, UI16 Receiver = 0);

	void	writeDBQuery(int  command, void* pIn, int pInSize);							

	BOOL	m_bConnect;

	int		getGateQueueSize()	{ return m_pGateQueue->getSize(); }
	int		getSubQueueSize()	{ return m_pSubQueue->getSize(); }
	int		getDespositorySize(){ return m_depositoryPos; }


	CRITICAL_SECTION	m_cs1;
	CRITICAL_SECTION	m_cs2;
	CRITICAL_SECTION	m_cs3;


	int		m_countWrite;
	int		m_countGetDB;
};


#endif