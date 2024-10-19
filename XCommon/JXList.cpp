//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// JXList class
//
// JXObject를 상속 받은 객체를 Node로 하는 Linked List를 구현
// 
// 노드를 추가하고 삭제 할 때마다 메모리를 할당하고 해제하는 방식이 아니라,
// 최대 노드 개수 만큼 메모리를 미리 할당하여 동작하는 방식으로 구현되었다
// 
// JXList는 "Free" List와 "Master" List를 가지고 있다
//
// "Free"는 미리 메모리에 할당된 객체들의 리스트이다
// "Free"의 메모리 생성 및 해제는 JXList를 상속받은 클래스에서 책임진다
//
// "Master"은 실제로 클래스 외부에서 사용하는 리스트이다
// 노드 추가시에 "Free"에서 노드를 분리한 후 "Master"에 삽입하는 방식이다
//
// 작 성 자    : 이 제 완
// 최종 수정일 : 2002. 6. 20
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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

	// FreeList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pParentList->m_pFreeFirst->m_pNext;

	m_pParentList->m_pFreeFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pParentList->m_pFreeFirst;

	--m_pParentList->m_uiFreeNumber;

	// MasterList로 연결한다

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


	// FreeList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pParentList->m_pFreeFirst->m_pNext;

	m_pParentList->m_pFreeFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pParentList->m_pFreeFirst;

	--m_pParentList->m_uiFreeNumber;

	// MasterList로 연결한다

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

	// MasterList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pMasterFirst;

	--m_uiMasterNumber;

	// FreeList로 연결한다

	local_pNodeB = ((JXList *)(local_pNodeA->m_pParent))->m_pFreeLast; 
		
	local_pNodeA->m_pPrev = local_pNodeB->m_pPrev;
	local_pNodeA->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = local_pNodeA;
	local_pNodeB->m_pPrev = local_pNodeA;

	++( ((JXList *)(local_pNodeA->m_pParent))->m_uiFreeNumber );

	// 노드를 더 이상 유효하지 않도록 설정한다 
	local_pNodeA->m_bValid = FALSE;

	return local_pNodeA;
}

JXObject* JXList::RemoveBack()
{

	if( m_uiMasterNumber == 0 ) return NULL;

 	// MasterList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pMasterLast->m_pPrev;

	m_pMasterLast->m_pPrev = local_pNodeA->m_pPrev;
	local_pNodeA->m_pPrev->m_pNext = m_pMasterLast;

	--m_uiMasterNumber;

	// FreeList로 연결한다

	local_pNodeB = ((JXList *)(local_pNodeA->m_pParent))->m_pFreeLast; 
	
	local_pNodeA->m_pPrev = local_pNodeB->m_pPrev;
	local_pNodeA->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = local_pNodeA;
	local_pNodeB->m_pPrev = local_pNodeA;

	++( ((JXList *)(local_pNodeA->m_pParent))->m_uiFreeNumber );

	// 노드를 더 이상 유효하지 않도록 설정한다 
	local_pNodeA->m_bValid = FALSE;

	return local_pNodeA;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  노드의 포인터를 넘겨 받아서 노드를 제거한다( 실제로는 Free 리스트에 넣는다 )
//  노드 포인터에 대한 유효성 검사를 하지 않는다 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void JXList::Remove( JXObject *pNode )
{

	// MasterList에서 분리

	pNode->m_pPrev->m_pNext = pNode->m_pNext;
	pNode->m_pNext->m_pPrev = pNode->m_pPrev;

	--m_uiMasterNumber;

	// FreeList로 연결

	local_pNodeB = ((JXList *)(pNode->m_pParent))->m_pFreeLast; 

	pNode->m_pPrev = local_pNodeB->m_pPrev;
	pNode->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = pNode;
	local_pNodeB->m_pPrev = pNode;

	++( ((JXList *)(pNode->m_pParent))->m_uiFreeNumber );

	// 노드를 더 이상 유효하지 않도록 설정한다 
	pNode->m_bValid = FALSE;

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  모든 노드를 제거 한다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void JXList::RemoveAll()
{

	JXObject *pCurrent, *pTemp;

	pCurrent = m_pMasterFirst->m_pNext;

	while( pCurrent != m_pMasterLast ){

		pTemp = pCurrent;
		pCurrent = pTemp->m_pNext;

		// MasterList에서 분리

		pTemp->m_pPrev->m_pNext = pTemp->m_pNext;
		pTemp->m_pNext->m_pPrev = pTemp->m_pPrev;

		// FreeList로 연결

		local_pNodeB = ((JXList *)(pTemp->m_pParent))->m_pFreeLast; 

		pTemp->m_pPrev = local_pNodeB->m_pPrev;
		pTemp->m_pNext = local_pNodeB;

		local_pNodeB->m_pPrev->m_pNext = pTemp;
		local_pNodeB->m_pPrev = pTemp;

		++( ((JXList *)(pTemp->m_pParent))->m_uiFreeNumber );

		// 노드를 더 이상 유효하지 않도록 설정한다 
		pTemp->m_bValid = FALSE;
	}

	m_uiMasterNumber = 0;
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 설 명 : "Free"에 새로운 노드를 추가한다 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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

	// 노드들을 분리한다 

	pList->m_pMasterFirst->m_pNext = pList->m_pMasterLast;
	pList->m_pMasterLast->m_pPrev = pList->m_pMasterFirst;

	pList->m_uiMasterNumber = 0;

	// 노드들을 삽입한다

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

