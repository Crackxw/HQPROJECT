#include <windows.h>
#include <stdio.h>
#include "Authorizer.h"
#include "Gash.h"

#include "_LoginDefine.h"


// AuthInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "AuthInfoParser.h"


#import  "lib/GD2sAAA.dll"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CGash::CGash():m_clsContext(CAuthInfoParser::P()->getAuthorizer_clsCtx())
{
	CoInitialize(NULL);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CGash::~CGash()
{
	CoUninitialize();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CGash::checkResultString(char* str)
{
	int ret = LOGIN::UNKNOWN_FAIL;


	if( strcmp(str, "Right_Service_Account") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "Right_Service_Account_And_Record") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "Accounting_Request_Has_Insert_MSMQ") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "Service_Account_Never_Login") == 0 )
	{
		ret = LOGIN::OK;
	}

	else if( strcmp(str, "Wrong_Password") == 0 )
	{
		ret = LOGIN::INVALID_PASSWORD;
	}

	else if( strcmp(str, "No_Enough_Point") == 0 )
	{
		ret = LOGIN::NOT_ENOUGHT_POINT; 
	}

	else if( strcmp(str, "Main_Account_Has_Been_Locked") == 0 )
	{
		ret = LOGIN::LOCK_MAIN_ACCOUNT;
	}

	else if( strcmp(str, "Service_Account_Has_Been_Locked") == 0 )
	{
		ret = LOGIN::LOCK_SERVICE_ACCOUNT;
	}

	else if( strcmp(str, "Service_Account_Has_Been_Login") == 0 )
	{
		ret = LOGIN::ALREADY_LOGIN;
	}

	else if( strcmp(str, "Service_Account_Not_Exist") == 0 )
	{
		ret = LOGIN::INVALID_ID;
	}
	
	else if( strcmp(str, "Logon_Failures_In_Short_Time") == 0 )
	{
		ret = LOGIN::FAIL_SHORT_TIME;
	}


	return ret;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CGash::reqLogin(char *id, char* password, char*ip, DWORD* idNumber)
{

	GD2sAAA::_Authentication*	pAuth	     = NULL;
	int							loginResult  = LOGIN::UNKNOWN_FAIL;



	//-- CoCreateInstance
	HRESULT	  hResult;
	hResult = CoCreateInstance( __uuidof(GD2sAAA::Authentication),
							   NULL,
							   m_clsContext,
							   __uuidof(GD2sAAA::_Authentication),
							   (void**)&pAuth );
	
	
	if( hResult == S_OK )
	{ 

		if( pAuth )
		{

			_bstr_t			rvalue;
			_variant_t		strDescription(rvalue);
			

			//-- ServiceAuth
			short			authResult = 0;
			authResult =	pAuth->ServiceAuth( id, password, "600062", 
												ip, "C", &strDescription );
							pAuth->Release();


			if( authResult != -1 )
			{

				//-- Out string
				_bstr_t Error(strDescription);

				char Text[128];
				strcpy(Text,Error);
				printf("-- GASH_LOGIN: %s(%s) \n",id,Text);

				loginResult = checkResultString( Text );

			}

		}

	}

	return loginResult;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CGash::reqLogout(char *id, char* password, char*ip, DWORD* idNumber)
{

	GD2sAAA::_Accounting *pAccount		= NULL;
	int					 logoutResult	= LOGIN::UNKNOWN_FAIL;



	//-- CoCreateInstance
	HRESULT hResult;						
	hResult = CoCreateInstance( __uuidof(GD2sAAA::Accounting),
							   NULL,
							   m_clsContext,
							   __uuidof(GD2sAAA::_Accounting),
							   (void**)&pAccount );


	if( hResult == S_OK )
	{

		if( pAccount ) 
		{

			_bstr_t			rvalue;
			_variant_t		strDescription(rvalue);
			

			//-- ServiceAccount
			short			accountResult = 0;
			accountResult = pAccount->ServiceAccounting( id, "600062", "R", "","", 
														 ip, &strDescription);
							pAccount->Release();

			
			if( accountResult != -1 )
			{

				//-- Out string
				_bstr_t Error(strDescription);

				char Text[128];
				strcpy(Text,Error);
				printf("-- GASH_LOGOUT: %s(%s) \n",id,Text);

				logoutResult = checkResultString( Text );

			}

		}
	}

	return logoutResult;
}



				
//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
int CGash::reqRecCash(char *id, char* password, char*ip, DWORD* idNumber)
{

	GD2sAAA::_Authentication*	pAuth	   = NULL;
	int							recCashResult = LOGIN::UNKNOWN_FAIL;



	//-- CoCreateInstance
	HRESULT	  hResult;
	hResult = CoCreateInstance( __uuidof(GD2sAAA::Authentication),
							   NULL,
							   m_clsContext,
							   __uuidof(GD2sAAA::_Authentication),
							   (void**)&pAuth );
	
	
	if( hResult == S_OK )
	{ 

		if( pAuth )
		{

			_bstr_t			rvalue;
			_variant_t		strDescription(rvalue);
			

			//-- ServiceAuth
			short			authResult = 0;
			authResult =	pAuth->ServiceAuth( id, password, "600062", 
												ip, "X", &strDescription );
							pAuth->Release();


			if( authResult != -1 )
			{

				//-- Out string
				_bstr_t Error(strDescription);

				char Text[128];
				strcpy(Text,Error);
				printf("-- GASH_REC_CASH: %s(%s) \n",id,Text);

				recCashResult = checkResultString( Text );

			}

		}

	}

	return recCashResult;
}
