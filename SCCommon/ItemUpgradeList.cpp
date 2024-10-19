#include <GSL.h>
#include "ItemUpgradeList.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ��
//--------------------------------------------------------------------------------------------
CItemUpgradeList::CItemUpgradeList()
{
	m_pFirst = NULL;
	m_pLast = NULL;
	Initialize();	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ��
//--------------------------------------------------------------------------------------------
CItemUpgradeList::~CItemUpgradeList()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ȭ
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
//	Desc : ���� ó��
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
//	Desc : ������ ����Ʈ�� �߰� ( ����� ��ũ�� ����Ʈ ) �Ŀ� ��ȹ ������ ������ ���׷��̵� �ݴ�� �� ���׷��� �ϴ� ��찡 �߻��� ���� ����ؼ�
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::AddItemList( CItemUpgrade * pItemUpgrade )
{
	if ( ! m_pFirst )					//��尡 �ϳ��� ���� ���
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
//	Desc : CItemUpgrade ������ �Ŀ� ����Ʈ�� �߰�
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
//	Desc : m_pFirst ��ȯ
//--------------------------------------------------------------------------------------------
CItemUpgrade *CItemUpgradeList::GetFirst()
{
	return m_pFirst;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : m_pLast ��ȯ
//--------------------------------------------------------------------------------------------
CItemUpgrade *CItemUpgradeList::GetLast()
{
	return m_pLast;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ���� ItemCode�� �޾Ƽ� ���� �������� ���� CItemUpgrade ��ü�� ã�Ƽ� �� ��ü�� �ּҸ� ��ȯ
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
//	Desc : ���� ItemCode�� �޾Ƽ� ���� �������� ���� CItemUpgrade ��ü�� ã�Ƽ� �� ��ü�� ������ �ڵ常 ��ȯ
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
//	Desc : �������� �ε��� �ڵ尪 ����
//--------------------------------------------------------------------------------------------
int CItemUpgradeList::GetCode()
{
	return m_Code;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �������� �ε��� �ڵ尪 ����
//--------------------------------------------------------------------------------------------
void CItemUpgradeList::SetCode( int Code )
{
	m_Code = Code;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ������ �̸� ����
//--------------------------------------------------------------------------------------------
char* CItemUpgradeList::GetItemName()
{
	return m_strItemName;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ������ �̸� ����
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
