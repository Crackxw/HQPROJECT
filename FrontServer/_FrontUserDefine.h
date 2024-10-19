#ifndef __FRONT_USER__
#define __FRONT_USER__


#include "_FrontDefine.h"


namespace FRONT_USER {


	using namespace FRONT;


	enum {

		REQUEST_CHECK_VERSION					=	0 ,			// ���� üũ��	   ��û �Ѵ� 
		REQUEST_WORLDS_INFO							  ,			// ������� ������ ��û �Ѵ� 
		REQUEST_SELECT_WORLD						  ,			// ���� ������	   ��û �Ѵ� 		
		REQUEST_NOTICE								  ,			// ���� ������	   ��û �Ѵ� 

		RESPONSE_CHECK_VERSION						  ,			// ���� üũ ��û��    ���� �Ѵ� 
		RESPONSE_WORLDS_INFO						  ,			// ������� ������û�� ���� �Ѵ� 
		RESPONSE_SELECT_WORLD						  ,			// ���� ���� ��û��    ���� �Ѵ� 
		RESPONSE_NOTICE											// ���� ���� ��û��    ���� �Ѵ� 

	};


	
	// -- ���� üũ�� ���� ���� ���� 
	enum {

		INVALID_VERSION							= -98			// ������ Ʋ���� 
	};


	const int  MAX_PACKETSIZE					=  3000;		




//--------------------------------------------------------------------------------------------
//  name :
//  desc : Ŭ���̾�Ʈ���� ��Ŷ ��ſ� ���̴� ���� ��� 
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
//  desc : ���� üũ�� ��û �Ѵ� 
//--------------------------------------------------------------------------------------------
struct sRequestCheckVersion
{
	DWORD			command;							
	WORD			version;					// ������ Ŭ���̾�Ʈ�� ���� 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : ���� üũ ��û�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
struct sResponseCheckVersion
{
	DWORD			command;					
	char			responseKind;				// ���� ���� 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : ������� ������ ��û �Ѵ�  
//--------------------------------------------------------------------------------------------
struct sRequestWorldsInfo
{
	DWORD			command;					
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : ������� ���� ��û�� ���� �Ѵ� 
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
//  desc : ������ ������ ��û �Ѵ� 
//--------------------------------------------------------------------------------------------
struct sRequestSelectWorld
{
	DWORD		command;						
	WORD		world_id;						
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : ���� ���ÿ� �����Ѵ�  
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
//  desc : ���� ������ ��û �Ѵ� 
//--------------------------------------------------------------------------------------------
struct sRequestNotice
{
	DWORD		command;
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : ���� ���� ��û�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
struct sResponseNoticeFront
{
	DWORD		command;
};

struct sResponseNotice: public sResponseNoticeFront
{

	char		notice[NOTICE_MAX_LENGTH];	    // ���� ���� 
};




};


#endif
