#ifndef __CAUTHPROC__
#define __CAUTHPROC__

typedef DWORD	_dword;
typedef WORD	_word;


class CAuthorizer;
class CMSSQL;
class CGashClient;

#include "AuthCallback.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CAuthProc : public CAuthCallback
{
private:
	char*				m_queryBuffer;
	CAuthorizer*		m_pAuthorizer;
	CMSSQL*				m_AccountDB;


	////////////////////////////////////////////////////////////////////
	// 중국	서비스를 위한 Gash server의 Connection Object.
	// 현재 이 객체는 Config파일을 읽어 Nation code가 CHINA 일때만
	// 생성한다. 즉 new를 해주는 것은 nation code가 CHINA일때만이다.
	// Gash Client가 생성하는 Thread 수 = Auth Thread * 3;
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