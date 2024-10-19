#ifndef __WORK__
#define __WORK__

#include "_AuthDefine.h"



namespace WORK {


	const int MAX_PACKETSIZE					= 3100;			// 로그인 작업 요청과 
																// 응답에 쓰이는 패킷의 
																// 최대 크기 






//--------------------------------------------------------------------------------------------
//  name :
//  desc : 모든 요청및 응답에 공통적으로 쓰이는 패킷 헤더 
//--------------------------------------------------------------------------------------------
struct sPacketHeader
{
	WORD				packetSize;
	
	AUTH::sUniqueID		userUniqID;
	AUTH::sUniqueID		gameServerUniqID;	
};



};

#endif