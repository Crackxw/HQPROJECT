#ifndef __CAUTHPROC__
#define __CAUTHPROC__

typedef DWORD	_dword;
typedef WORD	_word;


class CAuthorizer;
class CMSSQL;
class CGashClient;

#include "AuthCallback.h"


//������������������������������������������������������������������������������������������������������������������������
//
// class : 
//
//������������������������������������������������������������������������������������������������������������������������
class CAuthProc : public CAuthCallback
{
private:
	char*				m_queryBuffer;
	CAuthorizer*		m_pAuthorizer;
	CMSSQL*				m_AccountDB;


	////////////////////////////////////////////////////////////////////
	// �߱�	���񽺸� ���� Gash server�� Connection Object.
	// ���� �� ��ü�� Config������ �о� Nation code�� CHINA �϶���
	// �����Ѵ�. �� new�� ���ִ� ���� nation code�� CHINA�϶����̴�.
	// Gash Client�� �����ϴ� Thread �� = Auth Thread * 3;
private : 
	CGashClient*		m_pGashClient;
	BOOL				m_bGashClientUsed;
private : 
	void		 IsGashClientUsed();
	virtual	void CallbackSendResponseLogin
		(
			AUTH::sUniqueID gameServerUniqID, 
			AUTH::sUniqueID userUniqID, 
			int responseKind,
			DWORD dbAccount
		);
	////////////////////////////////////////////////////////////////////


public:
	CAuthProc();
	~CAuthProc();

	void				processPoll();

private:
	void				processQuery();






private:

	//--------------------------------------------------------------------------------
	void				recvRequestLogin    ( WORK::sPacketHeader* header, AUTH::sRequestLogin*    data );
	void				recvRequestLogout   ( WORK::sPacketHeader* header, AUTH::sRequestLogout*   data );
	void				recvRequestLoginEnd ( WORK::sPacketHeader* header, AUTH::sRequestLoginEnd* data );


	



	//--------------------------------------------------------------------------------
	void				sendResponseLogin	  ( AUTH::sUniqueID gameServerUniqID, 
												AUTH::sUniqueID userUniqID, 
												int responseKind, _dword dbAccount );

	void				sendResponseLoginEnd  ( AUTH::sUniqueID gameServerUniqID, 
												AUTH::sUniqueID userUniqID, 
												int responseKind );



	//--------------------------------------------------------------------------------	
	int					DB_Login    ( char* id, char* password, char* ip, _word world_id, _dword idNumber, _dword* dbAccount );
	int					DB_Logout   ( _dword dbAccount );
	int					DB_LoginEnd ( _dword dbAccount );




public:

	//--------------------------------------------------------------------------------
	static unsigned int _stdcall _AuthProcThread(void* pParam)
	{
		printf(" [CAuthProc] CAuthProcThread Start.....\n");
					
		CAuthProc CAuthProc;
		
		CAuthProc.processPoll();

		printf(" [CAuthProc] CAuthProcThread Ended.\n");

		return 0;
	}
};



#endif