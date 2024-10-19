#ifndef __FRONT_GAMESERVER__
#define __FRONT_GAMESERVER__


#include "_FrontDefine.h"


namespace FRONT_GAMESERVER {


	using namespace FRONT;


	enum {
				
		REPORT_CONCURRENT							= 100	 	// ���� ���� �����ڼ��� �˸��� 		

	};


	const int  MAX_PACKETSIZE					= 3000;












//--------------------------------------------------------------------------------------------
//  name :
//  desc : ���� �������� ��Ŷ ��ſ� ���̴� ���� ��� 
//--------------------------------------------------------------------------------------------
struct sPacketHeader 
{
	WORD		packetSize;						 
	short	    checksum;						 
};




//--------------------------------------------------------------------------------------------
//  name :
//  desc : ���� ���� �����ڼ��� �˸��� 
//--------------------------------------------------------------------------------------------
struct sReportConcurrent
{
	DWORD					command;	
	WORD					world_id;			// �� ��Ŷ�� ���� ������ ���̵� 
	WORD					concurrent;			// ���� ������ �� 
};
	



};


#endif