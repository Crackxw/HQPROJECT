#ifndef _XUSERMGR_H
#define _XUSERMGR_H

#include "../JWCommon/JWList.h"

class XUser;

class XUserMgr : public JWList
{
public:
	XUserMgr();
	~XUserMgr();

	void				Create( SI32 siMaxUser );

	JWList*				GetUserPtrList() { return m_pUserPtrList; };

	XUser*				GetUserByID( char *pID );
	XUser*				GetUserByName( char *pName );

public:
	XUser				*m_pUser;
	
	JWObjectPtr			*m_pUserPtr;
	JWList				*m_pUserPtrList;

};


#endif