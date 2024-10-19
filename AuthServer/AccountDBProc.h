#ifndef __CACCOUNTDBPROC__
#define __CACCOUNTDBPROC__

typedef DWORD	_dword;
typedef WORD	_word;


#include "_WorkDefine.h"

class CMSSQL;
class CAuthorizer;




//������������������������������������������������������������������������������������������������������������������������
//
// class : AccountDB ���� �۾��� ó�� �ϴ� Ŭ���� ( ���� �ڵ� ������ ���� ���� �۾� ó�� )
//
//������������������������������������������������������������������������������������������������������������������������
class CAccountDBProc
{
private:
	char*						m_queryBuffer;
	CMSSQL*						m_AccountDB;
	CAuthorizer*				m_pAuthorizer;

public:
	CAccountDBProc();
	~CAccountDBProc();

	
	void						processPoll();

private:
	void						processQuery();
	void						autoRecovery();
	
		
	
	//--------------------------------------------------------------------------------		
	int							DB_ForceLogout  ( _dword dbAccount );



	//--------------------------------------------------------------------------------		
	void						recvRequestReset  ( WORK::sPacketHeader* header, AUTH::sRequestReset*   data );
	


	//--------------------------------------------------------------------------------		
	void						sendResponseReset ( AUTH::sUniqueID gameServerUniqID, 
													AUTH::sUniqueID userUniqID,
													int responseKind );





public:

	//--------------------------------------------------------------------------------
	static unsigned int _stdcall _AccountDBProcThread(void* pParam)
	{
		printf(" [CAccountDBProc] AccountDBProcThread Start.....\n");
					
		CAccountDBProc   AccountDBProc;
		
		AccountDBProc.processPoll();

		printf(" [CAccountDBProc] AccountDBProcThread Ended.\n");

		return 0;
	}
};



#endif