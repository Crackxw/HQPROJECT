//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완					
// 최종 수정일 : 2002. 7. 2		
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXOBJECT_H
#define _JXOBJECT_H

#include "jxdef.h"

class JXObject
{
friend class JXList;

public:
	JXObject() : m_pPrev( 0 ), m_pNext( 0 ), m_pParent( 0 ), m_bValid( 0 ) {};
	virtual ~JXObject() {};

public:
	BOOL			IsValid() { return m_bValid; };

	void			SetValid( BOOL bValid ) { m_bValid = bValid; };

	JXObject*		GetParent() { return m_pParent; };

	JXObject*		GetPrev() { return m_pPrev; };
	JXObject*		GetNext() { return m_pNext; };

protected:
	BOOL			m_bValid;

	JXObject*		m_pPrev;
	JXObject*		m_pNext;

	JXObject*		m_pParent;

};


class JXObjectPtr : public JXObject
{
public:
	JXObjectPtr() : m_pObject( 0 ) {};
	virtual ~JXObjectPtr() {};

	void			Clear() { 
		
					m_bValid = FALSE; 
					m_pObject = NULL; 
	};

	void			SetObject( JXObject *pObject ) {

					m_pObject = pObject;
					m_bValid = TRUE;
	};

	JXObject*		GetObject() { 

					if( m_bValid ) return m_pObject;
					else return NULL;
	};

protected:
	JXObject*		m_pObject;

};

#endif

