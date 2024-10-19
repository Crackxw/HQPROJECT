//������������������������������������������������������������������������������
// �ۼ���      : ������			
// ���� ������ : 2002. 6. 20	
//������������������������������������������������������������������������������

#ifndef _JXLIST_H
#define _JXLIST_H

#include "JXObject.h"

#define		BEGIN_JXLIST_LOOP( i, list, objtype, objinst )		(objinst) = (objtype *)((list)->GetFirstNode()); for( i = 0; i < (list)->GetNumberNodes(); ++(i) ) { (objinst) = (objtype *)( (objinst)->GetNext() );
#define		END_JXLIST_LOOP										}

class JXList : public JXObject
{
public:
	JXList();
	virtual ~JXList();

public:
	void			SetParentList( JXList *pList ) { m_pParentList = pList; };

	JXObject*		PushFront();
	JXObject*		PushBack();

	JXObject*		RemoveFront();
	JXObject*		RemoveBack();

	void			Remove( JXObject *pNode );
	void			RemoveAll();

	JXObject*		GetFirstNode(){ return m_pMasterFirst; };
	JXObject*		GetLastNode(){ return m_pMasterLast; };

	JXObject*		GetFreeFirst(){ return m_pFreeFirst; };
	JXObject*		GetFreeLast(){ return m_pFreeLast; };

	UI32			GetNumberNodes(){ return m_uiMasterNumber; };

public:
	JXObject*		AddToFree( JXObject *pNode );

	UI32			PushFront( JXList *pList );

public:
	UI32			Wait();
	UI32			Wait( UI32 uiMilliseconds );
	BOOL			Release();

public:
	UI32			m_uiMasterNumber;
	UI32			m_uiFreeNumber;

public:
//private:
	JXObject		*m_pMasterFirst;
	JXObject		*m_pMasterLast;

	JXObject		*m_pFreeFirst;
	JXObject		*m_pFreeLast;

private:
	// ��� �Լ����� ���� ������ ���� ������
	JXObject		*local_pNodeA;
	JXObject		*local_pNodeB;

protected:
	// Thread ����
	HANDLE			m_hMutex;

	// ��带 ������ �� ����Ʈ 
	JXList			*m_pParentList;

};

#endif
