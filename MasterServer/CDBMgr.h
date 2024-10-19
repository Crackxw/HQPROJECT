#ifndef __CDBMGR_H__
#define __CDBMGR_H__

#include "CODBC.h"

class CDBMgr
{
protected: 
	static CDBMgr *pInstance;
	CODBC         *pODBC;             //odbc클래스

	void ReleaseInstance();

public:
	CDBMgr();
	~CDBMgr();
	static CDBMgr *GetInstance();

	bool Init();
	bool ConnectDB();                //DB에 접속한다.
	bool DisConnectDB();             //DB와 연결을 끊는다.
	bool SendQuary(bool bStatus, int nServerIndex); //쿼리문을 전송한다.
};

#endif