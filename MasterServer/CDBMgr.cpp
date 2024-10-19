#include "CDBMgr.h"

CDBMgr *CDBMgr::pInstance = NULL;
CDBMgr::CDBMgr()
{
	pODBC = NULL;
}

CDBMgr::~CDBMgr()
{
	if(pODBC != NULL)
		delete pODBC;

	pODBC = NULL;

	ReleaseInstance();
}

CDBMgr *CDBMgr::GetInstance()
{
	if(pInstance == NULL)
		pInstance = new CDBMgr;

	return pInstance;
}

void CDBMgr::ReleaseInstance()
{
	if(pInstance != NULL)
		delete pInstance;

	pInstance = NULL;
}

bool CDBMgr::Init()
{
	if(pODBC == NULL)
		pODBC = new CODBC;

	if(pODBC == NULL) return FALSE;

	return TRUE;
}

bool CDBMgr::ConnectDB() //DB에 접속한다.
{
	if(!pODBC->Connect("ServerMonitor119", "db_team", "ds1npx"))
	{
		printf("DB Connect Failed!!");
		return FALSE;
	}

	return TRUE;
}

bool CDBMgr::DisConnectDB() //DB와 연결을 끊는다.
{
	if(pODBC != NULL)
		pODBC->DisConnect();

	return TRUE;
}

// bStatus-> 0:dead, 1:live
bool CDBMgr::SendQuary(bool bStatus, int nServerIndex) //쿼리문을 전송한다.
{
	if(nServerIndex < 0) return FALSE;
	if(pODBC == NULL)
	{
		if(!Init()) return FALSE;
	}

	char pSql[1024] = {0,};
	sprintf(pSql, "UPDATE servermonitor SET status = %d where server = %d", bStatus, nServerIndex);
	if(pODBC->Query(pSql) < 0)
	{
		printf("SendQuery Failed");
		return FALSE;
	}
	else
		printf("SendQuery [ServerNum:%d][Status:%d]", nServerIndex, bStatus);
   
	return TRUE;
}
