
#include "JWList.h"

JWList::JWList()
{

	m_pMasterFirst = new JWObject;
	m_pMasterLast = new JWObject;

	m_pMasterFirst->m_pPrev = NULL;
	m_pMasterFirst->m_pNext = m_pMasterLast;

	m_pMasterLast->m_pPrev = m_pMasterFirst;
	m_pMasterLast->m_pNext = NULL;

	m_pFreeFirst = new JWObject;
	m_pFreeLast = new JWObject;

	m_pFreeFirst->m_pPrev = NULL;
	m_pFreeFirst->m_pNext = m_pFreeLast;

	m_pFreeLast->m_pPrev = m_pFreeFirst;
	m_pFreeLast->m_pNext = NULL;

	m_uiMasterNumber = 0;
	m_uiFreeNumber = 0;

	m_pSourceList = this;

	m_hMutex = CreateMutex( NULL, FALSE, NULL );

}

JWList::~JWList()
{
	delete m_pMasterFirst;
	delete m_pMasterLast;
	delete m_pFreeFirst;
	delete m_pFreeLast;

	CloseHandle( m_hMutex );

}

BOOL JWList::CanPush()
{
	return ( m_pSourceList->m_uiFreeNumber > 0 ) ? TRUE : FALSE;
}

JWObject* JWList::PushFront()
{

	if( m_pSourceList->m_uiFreeNumber == 0 ) return NULL;

	// FreeList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pSourceList->m_pFreeFirst->m_pNext;

	m_pSourceList->m_pFreeFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pSourceList->m_pFreeFirst;

	--m_pSourceList->m_uiFreeNumber;

	// MasterList로 연결한다

	local_pNodeA->m_pPrev = m_pMasterFirst;
	local_pNodeA->m_pNext = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext->m_pPrev = local_pNodeA;
	m_pMasterFirst->m_pNext = local_pNodeA;

	++m_uiMasterNumber;

	return local_pNodeA;
}


JWObject* JWList::PushBack()
{			

	if( m_pSourceList->m_uiFreeNumber == 0 ) return NULL;


	// FreeList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pSourceList->m_pFreeFirst->m_pNext;

	m_pSourceList->m_pFreeFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pSourceList->m_pFreeFirst;

	--m_pSourceList->m_uiFreeNumber;

	// MasterList로 연결한다

	local_pNodeA->m_pPrev = m_pMasterLast->m_pPrev;
	local_pNodeA->m_pNext = m_pMasterLast;

	m_pMasterLast->m_pPrev->m_pNext = local_pNodeA;
	m_pMasterLast->m_pPrev = local_pNodeA;
	
	++m_uiMasterNumber;

	return local_pNodeA;
}

JWObject* JWList::RemoveFront()
{

	if( m_uiMasterNumber == 0 ) return NULL;

	// MasterList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext = local_pNodeA->m_pNext;
	local_pNodeA->m_pNext->m_pPrev = m_pMasterFirst;

	--m_uiMasterNumber;

	// FreeList로 연결한다

	local_pNodeB = ((JWList *)(local_pNodeA->m_pParent))->m_pFreeLast; 
		
	local_pNodeA->m_pPrev = local_pNodeB->m_pPrev;
	local_pNodeA->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = local_pNodeA;
	local_pNodeB->m_pPrev = local_pNodeA;

	++( ((JWList *)(local_pNodeA->m_pParent))->m_uiFreeNumber );

	return local_pNodeA;
}

JWObject* JWList::RemoveBack()
{

	if( m_uiMasterNumber == 0 ) return NULL;

 	// MasterList로 부터 Node를 분리 시킨다 

	local_pNodeA = m_pMasterLast->m_pPrev;

	m_pMasterLast->m_pPrev = local_pNodeA->m_pPrev;
	local_pNodeA->m_pPrev->m_pNext = m_pMasterLast;

	--m_uiMasterNumber;

	// FreeList로 연결한다

	local_pNodeB = ((JWList *)(local_pNodeA->m_pParent))->m_pFreeLast; 
	
	local_pNodeA->m_pPrev = local_pNodeB->m_pPrev;
	local_pNodeA->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = local_pNodeA;
	local_pNodeB->m_pPrev = local_pNodeA;

	++( ((JWList *)(local_pNodeA->m_pParent))->m_uiFreeNumber );

	return local_pNodeA;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  노드의 포인터를 넘겨 받아서 노드를 제거한다( 실제로는 Free 리스트에 넣는다 )
//  노드 포인터에 대한 유효성 검사를 하지 않는다 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void JWList::Remove( JWObject *pNode )
{

	// MasterList에서 분리

	pNode->m_pPrev->m_pNext = pNode->m_pNext;
	pNode->m_pNext->m_pPrev = pNode->m_pPrev;

	--m_uiMasterNumber;

	// FreeList로 연결

	local_pNodeB = ((JWList *)(pNode->m_pParent))->m_pFreeLast; 

	pNode->m_pPrev = local_pNodeB->m_pPrev;
	pNode->m_pNext = local_pNodeB;

	local_pNodeB->m_pPrev->m_pNext = pNode;
	local_pNodeB->m_pPrev = pNode;

	++( ((JWList *)(pNode->m_pParent))->m_uiFreeNumber );

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  모든 노드를 제거 한다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void JWList::RemoveAll()
{

	JWObject *pCurrent, *pTemp;

	pCurrent = m_pMasterFirst->m_pNext;

	while( pCurrent != m_pMasterLast ){

		pTemp = pCurrent;
		pCurrent = pTemp->m_pNext;

		// MasterList에서 분리

		pTemp->m_pPrev->m_pNext = pTemp->m_pNext;
		pTemp->m_pNext->m_pPrev = pTemp->m_pPrev;

		// FreeList로 연결

		local_pNodeB = ((JWList *)(pTemp->m_pParent))->m_pFreeLast; 

		pTemp->m_pPrev = local_pNodeB->m_pPrev;
		pTemp->m_pNext = local_pNodeB;

		local_pNodeB->m_pPrev->m_pNext = pTemp;
		local_pNodeB->m_pPrev = pTemp;

		++( ((JWList *)(pTemp->m_pParent))->m_uiFreeNumber );

	}

	m_uiMasterNumber = 0;
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 설 명 : "Free"에 새로운 노드를 추가한다 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

JWObject* JWList::NewObject( JWObject *pNode )
{

	pNode->m_pParent = this;

	pNode->m_pPrev = m_pFreeLast->m_pPrev;
	pNode->m_pNext = m_pFreeLast;

	m_pFreeLast->m_pPrev->m_pNext = pNode;
	m_pFreeLast->m_pPrev = pNode;

	++m_uiFreeNumber;

	return pNode;

}

UI32 JWList::PushFrontAll( JWList *pSrcList )
{
	UI32 uiResult = pSrcList->m_uiMasterNumber;

	if( uiResult == 0 ) return uiResult;

	local_pNodeA = pSrcList->m_pMasterFirst->m_pNext;
	local_pNodeB = pSrcList->m_pMasterLast->m_pPrev;

	// 노드들을 분리한다 

	pSrcList->m_pMasterFirst->m_pNext = pSrcList->m_pMasterLast;
	pSrcList->m_pMasterLast->m_pPrev = pSrcList->m_pMasterFirst;

	pSrcList->m_uiMasterNumber = 0;

	// 노드들을 삽입한다

	local_pNodeA->m_pPrev = m_pMasterFirst;
	local_pNodeB->m_pNext = m_pMasterFirst->m_pNext;

	m_pMasterFirst->m_pNext->m_pPrev = local_pNodeB;
	m_pMasterFirst->m_pNext = local_pNodeA;

	m_uiMasterNumber += uiResult;

	return uiResult;
}

UI32 JWList::PushBackAll( JWList *pSrcList )
{
	UI32 uiResult = pSrcList->m_uiMasterNumber;

	if( uiResult == 0 ) return uiResult;

	local_pNodeA = pSrcList->m_pMasterFirst->m_pNext;
	local_pNodeB = pSrcList->m_pMasterLast->m_pPrev;

	// 노드들을 분리한다 

	pSrcList->m_pMasterFirst->m_pNext = pSrcList->m_pMasterLast;
	pSrcList->m_pMasterLast->m_pPrev = pSrcList->m_pMasterFirst;

	pSrcList->m_uiMasterNumber = 0;

	// 노드들을 삽입한다

	local_pNodeA->m_pPrev = m_pMasterLast->m_pPrev;
	local_pNodeB->m_pNext = m_pMasterLast;

	m_pMasterLast->m_pPrev->m_pNext = local_pNodeA;
	m_pMasterLast->m_pPrev = local_pNodeB;

	m_uiMasterNumber += uiResult;

	return uiResult;
}

UI32 JWList::Wait()
{
	return ::WaitForSingleObject( m_hMutex, INFINITE );
}


UI32 JWList::Wait( UI32 uiMilliseconds )
{
	return ::WaitForSingleObject( m_hMutex, uiMilliseconds );
}

BOOL JWList::Release()
{
	return ::ReleaseMutex( m_hMutex );
}

