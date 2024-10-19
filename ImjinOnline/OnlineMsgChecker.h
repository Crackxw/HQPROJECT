#pragma once

#pragma warning ( disable : 4786 )

//#include <iostream>
#include <set>

using namespace std;


class OnlineMsgChecker
{
private:
	OnlineMsgChecker()	{ _MsgChecker = NULL; }

public:
	~OnlineMsgChecker()	{}

	static OnlineMsgChecker	*GetInstance();

	void	AddMsg( DWORD dw )		{ _Msg.insert( dw ); }
	void	removeAll();
	BOOL	IsMsg( DWORD dw );

private:
	static OnlineMsgChecker			*_MsgChecker;
	set< DWORD, less<DWORD> >		_Msg;
};