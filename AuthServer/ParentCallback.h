
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: ParentCallback.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#ifndef __PARENT_CALLBACK_H_INCLUDED__
#define __PARENT_CALLBACK_H_INCLUDED__


class CParentCallback
{
public:
	virtual void CallbackDisConnect() = 0;

public : 
	CParentCallback()		{}
	~CParentCallback()		{}
};

#endif // __PARENT_CALLBACK_H_INCLUDED__
