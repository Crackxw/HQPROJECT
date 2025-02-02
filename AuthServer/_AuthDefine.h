#ifndef __AUTH__
#define __AUTH__

#include "_LoginDefine.h"




namespace AUTH {
																

	enum {

		REQUEST_RESET								  ,			// 로그인 상태를 초기화 한다 

		RESPONSE_RESET								  ,			// 로그인 상태 초기화 요청에 응답 한다 

		REPORT_HEARTBEAT							  ,			// 허트 비트를 보고 한다 

		REQUEST_LOGIN_END							  ,			// 로그인 완료를 요청 한다 		
		RESPONSE_LOGIN_END										// 로그인 완료요청에 응답 한다 
	};

	const int MAX_PACKETSIZE					= 3000;			// 인증서버와 게임서버간의 
																// 패킷의 최대 크기 

	
















//--------------------------------------------------------------------------------------------
//  name :
//  desc : 패킷 송신자를 구별해낼 유일한 아이디 
//--------------------------------------------------------------------------------------------
struct sUniqueID
{
	DWORD			orderKey;						// 송신자의 OrderKey
	DWORD			arrayIndex;						// 송신자의 ArrayIndex



	// -- UniqueID 설정 
	void			set( DWORD key,
						 DWORD index )
	{			
		orderKey   = key;		
		arrayIndex = index;
	}

	bool operator == (const sUniqueID& b) const { 
	
		return (orderKey == b.orderKey && arrayIndex == b.arrayIndex); 
	};

};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 게임서버와의 패킷 통신에 쓰이는 공통 헤더 
//--------------------------------------------------------------------------------------------
struct sPacketHeader 
{
	WORD			packetSize;							 
	short			checksum;						

	sUniqueID		userUniqID;					
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인을 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestLogin: public LOGIN::sRequestLogin
{		
	char			ip[16];						// 유저의 아이피 

	WORD			world_id;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseLogin: public LOGIN::sResponseLogin
{		
	DWORD			dbAccount;						
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그아웃을 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestLogout
{
	DWORD			command;

	char			id		 [LOGIN::ID_MAX_LENGTH];			
	char			password [LOGIN::PW_MAX_LENGTH];	
	char			ip		 [16];
	
	DWORD			dbAccount;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그아웃 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseLogout
{
	DWORD			command;

	char			responseKind;			
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 게임서버가 재시작 될때 로그인 상태 초기화를 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestReset
{
	DWORD			command;

	WORD			world_id;					
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인 상태 초기화 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseReset
{
	DWORD		   command;

	char		   responseKind;			 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 허트 비트를 보고 한다 
//--------------------------------------------------------------------------------------------
struct sReportHeartbeat
{
	DWORD			command;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인 완료를 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestLoginEnd
{
	DWORD			command;

	DWORD			dbAccount;					
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인 완료 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseLoginEnd
{
	DWORD			command;

	char			responseKind;							
};



};

#endif