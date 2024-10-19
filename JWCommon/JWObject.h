#ifndef _JWOBJECT_H
#define _JWOBJECT_H

#include "jwbase.h"

class JWObject
{
friend class JWList;

public:
	JWObject() : m_pPrev( 0 ), m_pNext( 0 ), m_pParent( 0 ), m_pFriend( 0 ), m_bAction( 0 ) {};
	virtual ~JWObject() {};

public:
	virtual void	Init() {};
	virtual void	Close() {};

public:
	void			SetParent( JWObject *pParent ) { m_pParent = pParent; };
	JWObject*		GetParent() { return m_pParent; };
	
	void			SetFriend( JWObject *pFriend ) { m_pFriend = pFriend; };
	JWObject*		GetFriend() { return m_pFriend; };

	JWObject*		GetPrev() { return m_pPrev; };
	JWObject*		GetNext() { return m_pNext; };

	void			SetAction() { InterlockedExchange( (long *)&m_bAction, TRUE ); };
	void			ResetAction() { InterlockedExchange( (long *)&m_bAction, FALSE ); };
	BOOL			IsOnAction() { return m_bAction; };

protected:
	JWObject*		m_pPrev;
	JWObject*		m_pNext;

	JWObject*		m_pParent;
	JWObject*		m_pFriend;

	BOOL			m_bAction;
};



class JWObjectPtr : public JWObject
{
public:
	JWObjectPtr() : m_pObject( 0 ) {};
	virtual ~JWObjectPtr() {};

	void			Clear() { m_pObject = NULL; };

	void			SetObject( JWObject *pObject ) { m_pObject = pObject; };

	JWObject*		GetObject() { return m_pObject; };

protected:
	JWObject*		m_pObject;

};


#endif