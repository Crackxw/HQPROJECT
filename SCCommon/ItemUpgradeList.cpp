#include <GSL.h>
#include "ItemUpgradeList.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 생 성 자
//--------------------------------------------------------------------------------------------
CItemUpgradeList::CItemUpgradeList()
{
	m_pFirst = NULL;
	m_pLast = NULL;
	Initialize();	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 소 멸 자
//--------------------------------------------------------------------------------------------
CItemUpgradeList::~CItemUpgradeList()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 초 기 화
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::Initialize()
{
	if ( m_pFirst )
	{
		delete m_pFirst;
		m_pFirst = NULL;
		return;
	}

	if ( m_pLast )
	{
		delete m_pLast;
		m_pLast = NULL;
		return;
	}
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 종료 처리
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::Destroy()
{
	if ( m_pFirst )
	{
		delete m_pFirst;
		m_pFirst = NULL;
		return;
	}

	if ( m_pLast )
	{
		delete m_pLast;
		m_pLast = NULL;
		return;
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템 리스트에 추가 ( 양방향 링크드 리스트 ) 후에 기획 상으로 아이템 업그레이드 반대로 언 업그레드 하는 경우가 발생할 때를 대비해서
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::AddItemList( CItemUpgrade * pItemUpgrade )
{
	if ( ! m_pFirst )					//노드가 하나도 없을 경우
	{
		m_pFirst = pItemUpgrade;
		m_pLast = m_pFirst;
	}

	m_pLast->SetNext( pItemUpgrade );	
	pItemUpgrade->SetPre( m_pLast );

	m_pLast = pItemUpgrade;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : CItemUpgrade 생성한 후에 리스트에 추가
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::AddUpgradeItem( int ItemCode, short Level, int Success, int Broken)
{
	CItemUpgrade *pItemUpgrade = new CItemUpgrade();
	
	pItemUpgrade->SetItemInfo( ItemCode, Level, Success, Broken );

	AddItemList( pItemUpgrade );

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : m_pFirst 반환
//--------------------------------------------------------------------------------------------
CItemUpgrade *CItemUpgradeList::GetFirst()
{
	return m_pFirst;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : m_pLast 반환
//--------------------------------------------------------------------------------------------
CItemUpgrade *CItemUpgradeList::GetLast()
{
	return m_pLast;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 현재 ItemCode를 받아서 다음 아이템을 가진 CItemUpgrade 객체를 찾아서 그 객체의 주소를 반환
//--------------------------------------------------------------------------------------------
CItemUpgrade *CItemUpgradeList::pFindNextItemCode( int CurrentItemCode )
{
	CItemUpgrade *pNextItem = m_pFirst;

	int CurrentLevel = ( CurrentItemCode - 10000 ) % 10;

	int i = 0;
	
	while ( i < CurrentLevel + 1 )
	{
		++i;
		
		pNextItem = pNextItem->GetNext();
	}

	return pNextItem;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 현재 ItemCode를 받아서 다음 아이템을 가진 CItemUpgrade 객체를 찾아서 그 객체의 아이템 코드만 반환
//--------------------------------------------------------------------------------------------
int CItemUpgradeList::iFindNextItemCode( int CurrentItemCode )
{
	CItemUpgrade *pNextItem = m_pFirst;

	int CurrentLevel = ( CurrentItemCode - 10000 ) % 10;

	int i = 0;
	
	while ( i < CurrentLevel + 1 )
	{
		++i;
		
		pNextItem = pNextItem->GetNext();
	}

	return pNextItem->GetItemCode();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템의 인덱스 코드값 얻어옮
//--------------------------------------------------------------------------------------------
int CItemUpgradeList::GetCode()
{
	return m_Code;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템의 인덱스 코드값 설정
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::SetCode( int Code )
{
	m_Code = Code;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템 이름 얻어옮
//--------------------------------------------------------------------------------------------
char* CItemUpgradeList::GetItemName()
{
	return m_strItemName;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템 이름 설정
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::SetItemName( char* ItemName)
{
	strcpy( m_strItemName,  ItemName );
	return;
}

short CItemUpgradeList::GetResult( int CurrentLevel, int Percentage )
{
	CItemUpgrade *pTemp = m_pFirst;

	for( int i=0; i< CurrentLevel; ++i )
	{
		pTemp = pTemp->GetNext();
	}

	return pTemp->GetResult( Percentage );
}
