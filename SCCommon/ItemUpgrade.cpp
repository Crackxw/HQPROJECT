#include <GSL.h>
#include "ItemUpgrade.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 생 성 자
//--------------------------------------------------------------------------------------------
CItemUpgrade::CItemUpgrade()
{
	m_pPre = NULL;
	m_pNext = NULL;

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 소 멸 자
//--------------------------------------------------------------------------------------------
CItemUpgrade::~CItemUpgrade()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 초 기 화
//--------------------------------------------------------------------------------------------
void CItemUpgrade::Initialize()
{
	m_ItemCode = -1;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 종료 처리
//--------------------------------------------------------------------------------------------
void CItemUpgrade::Destroy()
{
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Next Node 설정
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetNext( CItemUpgrade  *pNode )
{
	m_pNext = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Next Node 얻어옮
//--------------------------------------------------------------------------------------------
CItemUpgrade  * CItemUpgrade::GetNext( )
{
	return  m_pNext;	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Pre Node 설정
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetPre( CItemUpgrade *pNode )
{
	m_pPre = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Pre Node 얻어옮
//--------------------------------------------------------------------------------------------
CItemUpgrade  * CItemUpgrade::GetPre( )
{
	return m_pPre;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템 코드 반환
//--------------------------------------------------------------------------------------------
int CItemUpgrade::GetItemCode()
{
	return m_ItemCode;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템 코드 설정
//--------------------------------------------------------------------------------------------
void CItemUpgrade::SetItemCode( int ItemCode )
{
	m_ItemCode = ItemCode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 아이템 정보 설정
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
//	Desc : 아이템 확률 계산
//--------------------------------------------------------------------------------------------
short CItemUpgrade::GetResult( int Percentage )
{
	if ( Percentage < m_Success )		//성공
	{
		return ITEM_UPGRADE_SUCCESS;
	}
	else if ( Percentage < m_Fail )	//실패
	{
		return ITEM_UPGRADE_FAIL;
	}
	else							//Broken
	{
		return ITEM_UPGRADE_BROKEN;
	}
	
}