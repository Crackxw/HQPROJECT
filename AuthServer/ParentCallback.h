
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: ParentCallback.h

Author: �̹μ� <nshangukin@joyon.com>
Currently Managed by: �̹μ�

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
