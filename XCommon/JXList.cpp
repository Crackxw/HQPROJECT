//������������������������������������������������������������������������������
// JXList class
//
// JXObject�� ��� ���� ��ü�� Node�� �ϴ� Linked List�� ����
// 
// ��带 �߰��ϰ� ���� �� ������ �޸𸮸� �Ҵ��ϰ� �����ϴ� ����� �ƴ϶�,
// �ִ� ��� ���� ��ŭ �޸𸮸� �̸� �Ҵ��Ͽ� �����ϴ� ������� �����Ǿ���
// 
// JXList�� "Free" List�� "Master" List�� ������ �ִ�
//
// "Free"�� �̸� �޸𸮿� �Ҵ�� ��ü���� ����Ʈ�̴�
// "Free"�� �޸� ���� �� ������ JXList�� ��ӹ��� Ŭ�������� å������
//
// "Master"�� ������ Ŭ���� �ܺο��� ����ϴ� ����Ʈ�̴�
// ��� �߰��ÿ� "Free"���� ��带 �и��� �� "Master"�� �����ϴ� ����̴�
//
// �� �� ��    : �� �� ��
// ���� ������ : 2002. 6. 20
//������������������������������������������������������������������������������

#include "JXList.h"

JXList::JXList()
{

	m_pMasterFirst = new JXObject;
	m_pMasterLast = new JXObject;

	m_pMasterFirst->m_pPrev = NULL;
	m_pMasterFirst->m_pNext = m_pMasterLast;

	m_pMasterLast->m_pPrev = m_pMasterFirst;
	m_pMasterLast->m_pNext = NULL;

	m_pFreeFirst = new JXObject;
	m_pFreeLast = new JXObject;

	m_pFreeFirst->m_pPrev = NULL;
	m_pFreeFirst->m_pNext = m_pFreeLast;

	m_pFreeLast->m_pPrev = m_pFreeFirst;
	m_pFreeLast->m_pNext = NULL;

	m_uiMasterNumber = 0;
	m_uiFreeNumber = 0;

	m_pParentList = this;

	m_hMutex = CreateMutex( NULL, FALSE, NULL );

}

JXList::~JXList()
{
	delete m_pMasterFirst;
	delete m_pMasterLast;
	delete m_pFreeFirst;
	delete m_pFreeLast;

	CloseHandle( m_hMutex );

}

JXObject* JXList::PushFront()
{

	if( m_pParentList->m_uiFreeNumber == 0 ) return NULL;

	// FreeList�� ���� Node�� �и� ��Ų�� 

	local_pNodeA = m_pParentList->m_pFreeFirst->m_pNext;

	m_pParentList->m_pFreeFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pParentList->m_pFreeFirst;

	--m_pParentList->m_uiFreeNumber;

	// MasterList�� �����Ѵ�

	local_pNodeA->m_pPrev = m_pMasterFirst;
	local_pNodeA->m_pNext = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext->m_pPrev = local_pNodeA;
	m_pMasterFirst->m_pNext = local_pNodeA;

	++m_uiMasterNumber;

	local_pNodeA->m_bValid = TRUE;

	return local_pNodeA;
}


JXObject* JXList::PushBack()
{			

	if( m_pParentList->m_uiFreeNumber == 0 ) return NULL;


	// FreeList�� ���� Node�� �и� ��Ų�� 

	local_pNodeA = m_pParentList->m_pFreeFirst->m_pNext;

	m_pParentList->m_pFreeFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pParentList->m_pFreeFirst;

	--m_pParentList->m_uiFreeNumber;

	// MasterList�� �����Ѵ�

	local_pNodeA->m_pPrev = m_pMasterLast->m_pPrev;
	local_pNodeA->m_pNext = m_pMasterLast;

	m_pMasterLast->m_pPrev->m_pNext = local_pNodeA;
	m_pMasterLast->m_pPrev = local_pNodeA;
	
	++m_uiMasterNumber;

	local_pNodeA->m_bValid = TRUE;

	return local_pNodeA;
}

JXObject* JXList::RemoveFront()
{

	if( m_uiMasterNumber == 0 ) return NULL;

	// MasterList�� ���� Node�� �и� ��Ų�� 

	local_pNodeA = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pMasterFirst;

	--m_uiMasterNumber;

	// FreeList�� �����Ѵ�

	local_pNodeB = ((JXList *)(local_pNodeA->m_pParent))->m_pFreeLast; 
		
	local_pNodeA->m_pPrev = local_pNodeB->m_pPrev;
	local_pNodeA->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = local_pNodeA;
	local_pNodeB->m_pPrev = local_pNodeA;

	++( ((JXList *)(local_pNodeA->m_pParent))->m_uiFreeNumber );

	// ��带 �� �̻� ��ȿ���� �ʵ��� �����Ѵ� 
	local_pNodeA->m_bValid = FALSE;

	return local_pNodeA;
}

JXObject* JXList::RemoveBack()
{

	if( m_uiMasterNumber == 0 ) return NULL;

 	// MasterList�� ���� Node�� �и� ��Ų�� 

	local_pNodeA = m_pMasterLast->m_pPrev;

	m_pMasterLast->m_pPrev = local_pNodeA->m_pPrev;
	local_pNodeA->m_pPrev->m_pNext = m_pMasterLast;

	--m_uiMasterNumber;

	// FreeList�� �����Ѵ�

	local_pNodeB = ((JXList *)(local_pNodeA->m_pParent))->m_pFreeLast; 
	
	local_pNodeA->m_pPrev = local_pNodeB->m_pPrev;
	local_pNodeA->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = local_pNodeA;
	local_pNodeB->m_pPrev = local_pNodeA;

	++( ((JXList *)(local_pNodeA->m_pParent))->m_uiFreeNumber );

	// ��带 �� �̻� ��ȿ���� �ʵ��� �����Ѵ� 
	local_pNodeA->m_bValid = FALSE;

	return local_pNodeA;
}

//������������������������������������������������������������������������������
//  ����� �����͸� �Ѱ� �޾Ƽ� ��带 �����Ѵ�( �����δ� Free ����Ʈ�� �ִ´� )
//  ��� �����Ϳ� ���� ��ȿ�� �˻縦 ���� �ʴ´� 
//������������������������������������������������������������������������������

void JXList::Remove( JXObject *pNode )
{

	// MasterList���� �и�

	pNode->m_pPrev->m_pNext = pNode->m_pNext;
	pNode->m_pNext->m_pPrev = pNode->m_pPrev;

	--m_uiMasterNumber;

	// FreeList�� ����

	local_pNodeB = ((JXList *)(pNode->m_pParent))->m_pFreeLast; 

	pNode->m_pPrev = local_pNodeB->m_pPrev;
	pNode->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = pNode;
	local_pNodeB->m_pPrev = pNode;

	++( ((JXList *)(pNode->m_pParent))->m_uiFreeNumber );

	// ��带 �� �̻� ��ȿ���� �ʵ��� �����Ѵ� 
	pNode->m_bValid = FALSE;

}

//������������������������������������������������������������������������������
//  ��� ��带 ���� �Ѵ�
//������������������������������������������������������������������������������

void JXList::RemoveAll()
{

	JXObject *pCurrent, *pTemp;

	pCurrent = m_pMasterFirst->m_pNext;

	while( pCurrent != m_pMasterLast ){

		pTemp = pCurrent;
		pCurrent = pTemp->m_pNext;

		// MasterList���� �и�

		pTemp->m_pPrev->m_pNext = pTemp->m_pNext;
		pTemp->m_pNext->m_pPrev = pTemp->m_pPrev;

		// FreeList�� ����

		local_pNodeB = ((JXList *)(pTemp->m_pParent))->m_pFreeLast; 

		pTemp->m_pPrev = local_pNodeB->m_pPrev;
		pTemp->m_pNext = local_pNodeB;

		local_pNodeB->m_pPrev->m_pNext = pTemp;
		local_pNodeB->m_pPrev = pTemp;

		++( ((JXList *)(pTemp->m_pParent))->m_uiFreeNumber );

		// ��带 �� �̻� ��ȿ���� �ʵ��� �����Ѵ� 
		pTemp->m_bValid = FALSE;
	}

	m_uiMasterNumber = 0;
	
}

//������������������������������������������������������������������������������
// �� �� : "Free"�� ���ο� ��带 �߰��Ѵ� 
//������������������������������������������������������������������������������

JXObject* JXList::AddToFree( JXObject *pNode )
{

	pNode->m_pParent = this;

	pNode->m_pPrev = m_pFreeLast->m_pPrev;
	pNode->m_pNext = m_pFreeLast;

	m_pFreeLast->m_pPrev->m_pNext = pNode;
	m_pFreeLast->m_pPrev = pNode;

	++m_uiFreeNumber;

	return pNode;
}

UI32 JXList::PushFront( JXList *pList )
{
	UI32 uiResult = pList->m_uiMasterNumber;

	if( uiResult == 0 ) return uiResult;

	local_pNodeA = pList->m_pMasterFirst->m_pNext;
	local_pNodeB = pList->m_pMasterLast->m_pPrev;

	// ������ �и��Ѵ� 

	pList->m_pMasterFirst->m_pNext = pList->m_pMasterLast;
	pList->m_pMasterLast->m_pPrev = pList->m_pMasterFirst;

	pList->m_uiMasterNumber = 0;

	// ������ �����Ѵ�

	local_pNodeA->m_pPrev = m_pMasterFirst;
	local_pNodeB->m_pNext = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext->m_pPrev = local_pNodeB;
	m_pMasterFirst->m_pNext = local_pNodeA;

	m_uiMasterNumber += uiResult;

	return uiResult;
}

UI32 JXList::Wait()
{
	return ::WaitForSingleObject( m_hMutex, INFINITE );
}


UI32 JXList::Wait( UI32 uiMilliseconds )
{
	return ::WaitForSingleObject( m_hMutex, uiMilliseconds );
}

BOOL JXList::Release()
{
	return ::ReleaseMutex( m_hMutex );
}

