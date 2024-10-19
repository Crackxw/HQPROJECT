#include <winsock2.h>
#include <stdio.h>


// CUser
#include "../kojimodo/iocp/AcceptSocket.h"
#include "User.h"


// CUserMgr
#include "../kojimodo/iocp/ListenSocket.h"
#include "../kojimodo/utility/Singleton.h"
#include "UserMgr.h"


//
#include "_FrontUserDefine.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CUserMgr::CUserMgr(char* ip, int port, int max_socket): 
CListenSocket( ip, port, max_socket )
{							
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CUserMgr::~CUserMgr()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CAcceptSocket* CUserMgr::_CallBack_CreateSocket(int index)
{	
	return new CUser( index, FRONT_USER::MAX_PACKETSIZE, 
							 FRONT_USER::MAX_PACKETSIZE, 50000, 50000 );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CUser* CUserMgr::getUser(int index)
{
	return (CUser*)getSocket(index);
}






