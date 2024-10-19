#ifndef __WORK__
#define __WORK__

#include "_AuthDefine.h"



namespace WORK {


	const int MAX_PACKETSIZE					= 3100;			// �α��� �۾� ��û�� 
																// ���信 ���̴� ��Ŷ�� 
																// �ִ� ũ�� 






//--------------------------------------------------------------------------------------------
//  name :
//  desc : ��� ��û�� ���信 ���������� ���̴� ��Ŷ ��� 
//--------------------------------------------------------------------------------------------
struct sPacketHeader
{
	WORD				packetSize;
	
	AUTH::sUniqueID		userUniqID;
	AUTH::sUniqueID		gameServerUniqID;	
};



};

#endif