	
#include <windows.h>
#include "OnlineMsgChecker.h"


OnlineMsgChecker *OnlineMsgChecker::_MsgChecker = NULL;

OnlineMsgChecker *OnlineMsgChecker::GetInstance()
{
	if( _MsgChecker )	_MsgChecker = new OnlineMsgChecker;
	return _MsgChecker;
}

BOOL OnlineMsgChecker::IsMsg( DWORD dw )
{
	set< DWORD, less<DWORD> >::iterator find = _Msg.find( dw );

	if( find != _Msg.end() )
	{
		_Msg.erase( find );
		return TRUE;
	}

	return FALSE;
}

void OnlineMsgChecker::removeAll()
{
	if( _MsgChecker )
	{
		_Msg.clear();
		delete _MsgChecker;
	}
	_MsgChecker = NULL;
}
