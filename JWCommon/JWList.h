#ifndef _JWLIST_H
#define _JWLIST_H

#include "JWObject.h"

#define		JWLIST_LOOP( i, list, objtype, objinst )		(objinst) = (objtype *)((list)->GetActualFirstNode()); for( i = 0; i < (list)->GetNumberOfNodes(); ++(i), (objinst) = (objtype *)( (objinst)->GetNext() ) )

class JWList : public JWObject
{
public:
	JWList();
	virtual ~JWList();

public:
	void			SetSourceList( JWList *pList ) { m_pSourceList = pList; };

	BOOL			CanPush();

	JWObject*		PushFront();
	JWObject*		PushBack();

	JWObject*		RemoveFront();
	JWObject*		RemoveBack();

	void			Remove( JWObject *pNode );
	void			RemoveAll();

	JWObject*		GetActualFirstNode() { return m_pMasterFirst->m_pNext; };
	JWObject*		GetActualLastNode() { return m_pMasterLast->m_pPrev; };

	JWObject*		GetFirstNode() { return m_pMasterFirst; };
	JWObject*		GetLastNode() { return m_pMasterLast; };

	JWObject*		GetFreeFirst() { return m_pFreeFirst; };
	JWObject*		GetFreeLast() { return m_pFreeLast; };

	UI32			GetNumberOfNodes() { return m_uiMasterNumber; };

public:
	JWObject*		NewObject( JWObject *pNode );

	UI32			PushFrontAll( JWList *pSrcList );
	UI32			PushBackAll( JWList *pSrcList );

public:
	UI32			Wait();
	UI32			Wait( UI32 uiMilliseconds );
	BOOL			Release();

public:
	UI32			m_uiMasterNumber;
	UI32			m_uiFreeNumber;

public:
	JWObject		*m_pMasterFirst;
	JWObject		*m_pMasterLast;

	JWObject		*m_pFreeFirst;
	JWObject		*m_pFreeLast;

private:
	// 멤버 함수에서 지역 변수로 사용될 변수들
	JWObject		*local_pNodeA;
	JWObject		*local_pNodeB;

private:
	// Thread 관련
	HANDLE			m_hMutex;

	// 노드를 가지고 올 리스트 
	JWList			*m_pSourceList;

};

#endif
