#ifndef __FRONT_USER__
#define __FRONT_USER__


#include "_FrontDefine.h"


namespace FRONT_USER {


	using namespace FRONT;


	enum {

		REQUEST_CHECK_VERSION					=	0 ,			// 버전 체크를	   요청 한다 
		REQUEST_WORLDS_INFO							  ,			// 월드들의 정보를 요청 한다 
		REQUEST_SELECT_WORLD						  ,			// 월드 선택을	   요청 한다 		
		REQUEST_NOTICE								  ,			// 공지 사항을	   요청 한다 

		RESPONSE_CHECK_VERSION						  ,			// 버전 체크 요청에    응답 한다 
		RESPONSE_WORLDS_INFO						  ,			// 월드들의 정보요청에 응답 한다 
		RESPONSE_SELECT_WORLD						  ,			// 월드 선택 요청에    응답 한다 
		RESPONSE_NOTICE											// 공지 사항 요청에    응답 한다 

	};


	
	// -- 버전 체크에 대한 응답 종류 
	enum {

		INVALID_VERSION							= -98			// 버전이 틀리다 
	};


	const int  MAX_PACKETSIZE					=  3000;		




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 클라이언트와의 패킷 통신에 쓰이는 공통 헤더 
//--------------------------------------------------------------------------------------------
#pragma						pack(push, 1)

struct  sPacketHeader 
{
	WORD		packetSize;						 
	char	    checksum;						 
};

#pragma						pack(pop)




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 버전 체크를 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestCheckVersion
{
	DWORD			command;							
	WORD			version;					// 접속한 클라이언트의 버전 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 버전 체크 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseCheckVersion
{
	DWORD			command;					
	char			responseKind;				// 응답 종류 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 월드들의 정보를 요청 한다  
//--------------------------------------------------------------------------------------------
struct sRequestWorldsInfo
{
	DWORD			command;					
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 월드들의 정보 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseWorldsInfoFront		
{
	DWORD			command;					
	WORD			numWorld;					 					
};

struct sResponseWorldsInfo: public sResponseWorldsInfoFront 
{

	sWorldInfo		worldsInfo[MAX_WORLD];	    
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 월드의 선택을 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestSelectWorld
{
	DWORD		command;						
	WORD		world_id;						
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 월드 선택에 응답한다  
//--------------------------------------------------------------------------------------------
struct sResponseSelectWorldFront
{
	DWORD		command;						
	WORD		world_id;																
};

struct sResponseSelectWorld: public sResponseSelectWorldFront				
{
	char		gateIP[16];						
	WORD		gatePort;

	char		chatIP[16];							
	WORD		chatPort;

	

	void		setGateIP(const char* a)
	{
		if( a ) {

			memset(gateIP, 0, sizeof(gateIP));
			strcpy(gateIP, a);
		}
	}


	void		setChatIP(const char* a)
	{
		if( a ) {

			memset(chatIP, 0, sizeof(chatIP));
			strcpy(chatIP, a);
		}
	}

};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 공지 사항을 요청 한다 
//--------------------------------------------------------------------------------------------
struct sRequestNotice
{
	DWORD		command;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 공지 사항 요청에 응답 한다 
//--------------------------------------------------------------------------------------------
struct sResponseNoticeFront
{
	DWORD		command;
};

struct sResponseNotice: public sResponseNoticeFront
{

	char		notice[NOTICE_MAX_LENGTH];	    // 공지 사항 
};




};


#endif
