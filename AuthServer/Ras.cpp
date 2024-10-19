#include <windows.h>
#include <stdio.h>
#include "Authorizer.h"
#include "Ras.h"

#include "_LoginDefine.h"
#include "RasLib.h"


// CAuthInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "AuthInfoParser.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CRas::CRas()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CRas::~CRas()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CRas::checkResultString(char* str)
{
	int ret = LOGIN::UNKNOWN_FAIL;


	// SERVICE ERROR
	//-------------------------------------------------------------------
	if( strcmp(str, "SERVICE_OK") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "SERVICE_LOGIN_OK") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "SERVICE_LOGOUT_OK") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "SERVICE_LOCK_WICKDNESS") == 0 )
	{
		ret = LOGIN::LOCK_MAIN_ACCOUNT;
	}

	else if( strcmp(str, "SERVICE_LOCK_ACCOUNT") == 0 )
	{
		ret = LOGIN::LOCK_SERVICE_ACCOUNT;
	}



	// ACCESS ERROR
	//-------------------------------------------------------------------
	else if( strcmp(str, "ACCESS_WRONG_ID") == 0 )
	{
		ret = LOGIN::INVALID_ID;
	}

	else if( strcmp(str, "ACCESS_WRONG_PASSWORD") == 0 )
	{
		ret = LOGIN::INVALID_PASSWORD;
	}

	else if( strcmp(str, "ACCESS_ALREADY_IN_CONTENTS") == 0 )
	{
		ret = LOGIN::ALREADY_LOGIN;
	}

	else if( strcmp(str, "ACCESS_NOT_IN_CONTENTS") == 0 )
	{
		ret = LOGIN::OK;
	}



	// PAYMENT ERROR
	//-------------------------------------------------------------------
	else if( strcmp(str, "PAYMENT_OK") == 0 )
	{
		ret = LOGIN::OK;
	}
	
	else if( strcmp(str, "PAYMENT_NO_NEED_CASH") == 0 )
	{
		ret = LOGIN::NOT_ENOUGHT_POINT;
	}


	return ret;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CRas::reqLogin(char *id, char* password, char*ip, DWORD* idNumber)
{


	int					 loginResult  = LOGIN::UNKNOWN_FAIL;
	

	char				 rasResult[1024];
	memset(rasResult, 0, sizeof(rasResult));




	Joy_RAS_Function( 100002, 'B', -1, 						  
					  id	   , strlen(id), 
					  password , strlen(password), 
					  ip	   , strlen(ip),						   
					  CAuthInfoParser::P()->getAuthorizer_IP() ,
					  CAuthInfoParser::P()->getAuthorizer_Port() ,					  					  
					  rasResult,
					  1024 );


	{
		char*	msg			= strtok( rasResult , "," );
		char*	strIdNumber = strtok( NULL	    , "," );

		printf("-- RAS_LOGIN: %s\n", msg);

		if( strIdNumber ) *idNumber = atoi(strIdNumber);
		
		loginResult = checkResultString( msg );
	}


	return loginResult;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CRas::reqLogout(char *id, char* password, char*ip, DWORD* idNumber)
{

	int					 logoutResult	= LOGIN::UNKNOWN_FAIL;


	char				 rasResult[1024];
	memset(rasResult, 0, sizeof(rasResult));




	Joy_RAS_Function( 100002, 'D', -1, 						  
					  id	   , strlen(id), 
					  password , strlen(password), 
					  ip	   , strlen(ip),						   
					  CAuthInfoParser::P()->getAuthorizer_IP() ,
					  CAuthInfoParser::P()->getAuthorizer_Port() ,					  					  
					  rasResult,
					  1024 );

	{
		char*	msg			= strtok( rasResult , "," );
		char*	strIdNumber = strtok( NULL	    , "," );

		printf("-- RAS_LOGOUT: %s\n", msg);

		if( strIdNumber ) *idNumber = atoi(strIdNumber);
	
		logoutResult = checkResultString( msg );
	}


	return logoutResult;
}



				
//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CRas::reqRecCash(char *id, char* password, char*ip, DWORD* idNumber)
{

	int					 recCashResult = LOGIN::UNKNOWN_FAIL;


	char				 rasResult[1024];
	memset(rasResult, 0, sizeof(rasResult));




	Joy_RAS_Function( 100002, 'C', -1, 						  
					  id	   , strlen(id), 
					  password , strlen(password), 
					  ip	   , strlen(ip),						   
					  CAuthInfoParser::P()->getAuthorizer_IP() ,
					  CAuthInfoParser::P()->getAuthorizer_Port() ,					  					  
					  rasResult,
					  1024 );

	{

		char*	msg			= strtok( rasResult , "," );
		char*	strIdNumber = strtok( NULL	    , "," );

		printf("-- RAS_REC_CASH: %s\n", msg);

		if( strIdNumber ) *idNumber = atoi(strIdNumber);
		
		recCashResult = checkResultString( msg );
	}


	return recCashResult;
}

