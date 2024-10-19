

#ifndef __ONLINE_CLIENT_CALLBACK_H__
#define __ONLINE_CLIENT_CALLBACK_H__

class COnlineClientCallback
{
public : 
	COnlineClientCallback()			{}
	virtual ~COnlineClientCallback()	{}

public : 
	virtual BOOL	SendMacroUsingUserInfo(const char* szMacroProgName) = 0;
};

#endif // __ONLINE_CLIENT_CALLBACK_H__