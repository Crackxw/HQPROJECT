
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AuthCallback.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#ifndef __AUTH_CALLBACK_H_INCLUDED__
#define __AUTH_CALLBACK_H_INCLUDED__


#include "_AuthDefine.h"

class CAuthCallback
{
public:
	virtual void CallbackSendResponseLogin
		( 
			AUTH::sUniqueID gameServerUniqID, 
			AUTH::sUniqueID userUniqID, 
			int responseKind,
			DWORD dbAccount
		) = 0;

public : 
	CAuthCallback()	{}
	~CAuthCallback()	{}
};

#endif // __AUTH_CALLBACK_H_INCLUDED__
