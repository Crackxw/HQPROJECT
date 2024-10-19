#ifndef __CDBMGR_H__
#define __CDBMGR_H__

#include "CODBC.h"

class CDBMgr
{
protected: 
	static CDBMgr *pInstance;
	CODBC         *pODBC;             //odbcŬ����

	void ReleaseInstance();

public:
	CDBMgr();
	~CDBMgr();
	static CDBMgr *GetInstance();

	bool Init();
	bool ConnectDB();                //DB�� �����Ѵ�.
	bool DisConnectDB();             //DB�� ������ ���´�.
	bool SendQuary(bool bStatus, int nServerIndex); //�������� �����Ѵ�.
};

#endif