#ifndef __FRONT_GAMESERVER__
#define __FRONT_GAMESERVER__


#include "_FrontDefine.h"


namespace FRONT_GAMESERVER {


	using namespace FRONT;


	enum {
				
		REPORT_CONCURRENT							= 100	 	// 현재 동시 접속자수를 알린다 		

	};


	const int  MAX_PACKETSIZE					= 3000;












//--------------------------------------------------------------------------------------------
//  name :
//  desc : 게임 서버와의 패킷 통신에 쓰이는 공통 헤더 
//--------------------------------------------------------------------------------------------
struct sPacketHeader 
{
	WORD		packetSize;						 
	short	    checksum;						 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : 현재 동시 접속자수를 알린다 
//--------------------------------------------------------------------------------------------
struct sReportConcurrent
{
	DWORD					command;	
	WORD					world_id;			// 이 패킷을 보낸 월드의 아이디 
	WORD					concurrent;			// 동시 접속자 수 
};
	



};


#endif