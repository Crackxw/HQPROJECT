#include <GSL.h>
#include "ItemUpgrade.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ��
//--------------------------------------------------------------------------------------------
CItemUpgrade::CItemUpgrade()
{
	m_pPre = NULL;
	m_pNext = NULL;

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ��
//--------------------------------------------------------------------------------------------
CItemUpgrade::~CItemUpgrade()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ȭ
//--------------------------------------------------------------------------------------------
void CItemUpgrade::Initialize()
{
	m_ItemCode = -1;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ���� ó��
//--------------------------------------------------------------------------------------------
void CItemUpgrade::Destroy()
{
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Next Node ����
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetNext( CItemUpgrade  *pNode )
{
	m_pNext = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Next Node ����
//--------------------------------------------------------------------------------------------
CItemUpgrade  * CItemUpgrade::GetNext( )
{
	return  m_pNext;	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Pre Node ����
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetPre( CItemUpgrade *pNode )
{
	m_pPre = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Pre Node ����
//--------------------------------------------------------------------------------------------
CItemUpgrade  * CItemUpgrade::GetPre( )
{
	return m_pPre;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ������ �ڵ� ��ȯ
//--------------------------------------------------------------------------------------------
int CItemUpgrade::GetItemCode()
{
	return m_ItemCode;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ������ �ڵ� ����
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetItemCode( int ItemCode )
{
	m_ItemCode = ItemCode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ������ ���� ����
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetItemInfo( int ItemCode, short Level, int Success, int Broken)
{
	m_ItemCode = ItemCode;
	m_Level = Level;
	m_Success = Success;
	m_Broken = Broken;
	m_Fail = 100000 - Success - Broken;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ������ Ȯ�� ���
//--------------------------------------------------------------------------------------------
short CItemUpgrade::GetResult( int Percentage )
{
	if ( Percentage < m_Success )		//����
	{
		return ITEM_UPGRADE_SUCCESS;
	}
	else if ( Percentage < m_Fail )	//����
	{
		return ITEM_UPGRADE_FAIL;
	}
	else							//Broken
	{
		return ITEM_UPGRADE_BROKEN;
	}
	
}