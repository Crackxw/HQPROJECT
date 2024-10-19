#include <GSL.h>
#include "ItemUpgradeList.h"
#include "ItemUpgradeParser.h"
#include <fstream.h>

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 생 성 자
//--------------------------------------------------------------------------------------------
CItemUpgradeParser::CItemUpgradeParser()
{
	m_pUpgradeItemList = NULL;
	srand( time( NULL ) );

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 소 멸 자
//--------------------------------------------------------------------------------------------
CItemUpgradeParser::~CItemUpgradeParser()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 초 기 화
//--------------------------------------------------------------------------------------------
void CItemUpgradeParser::Initialize()
{
	if ( m_pUpgradeItemList )
	{
		delete [] m_pUpgradeItemList;

		m_pUpgradeItemList = NULL;
	}
	
	memset(&m_stItemUpgradeResult, 0, sizeof (OnItemUpgradeResultInfo)) ;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 종료 처리
//--------------------------------------------------------------------------------------------
void CItemUpgradeParser::Destroy()
{
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Script 파일 읽어서 List에 넣어둠
//--------------------------------------------------------------------------------------------
bool CItemUpgradeParser::ReadScriptFile()
{
	char Programmer[ 128 ] = "";
	char ChangeDate[ 128 ] = "";
	char Version[ 128 ] = "";
	char Architecture [ 128 ] = "";
	int Total_Item_Num = 0;

	int Count = 0;
	short Level = -1;
	int Success = 0;
	int Broken = 0;

	int Code = 0;
	int ItemCode = 0;
	char ItemName[128] = "";

	ifstream inFile( "upgrade.txt", ios::in );

	if ( ! inFile )
	{
		cerr << "File coule not be opened\n";
		return false;
	}

	inFile>>Programmer>>ChangeDate>>Version>>Architecture>>Total_Item_Num;

	char temp[32] = "";

	for ( int j = 0; j < 44; ++j )
	{
		inFile>>temp;					//필요 없는 내용 한 줄 넘어감
	}

	m_pUpgradeItemList = new CItemUpgradeList[Total_Item_Num];
	
	while ( ! inFile.eof() )
	{
		
		Count++;

		inFile >> Code >> ItemName;

		m_pUpgradeItemList[ Count ].SetCode( Code );
		m_pUpgradeItemList[ Count ].SetItemName( ItemName );

		for ( int i = 0; i < 10; ++i )
		{
			inFile >> ItemCode >> Level >> Success >> Broken;
			m_pUpgradeItemList[ Count ].AddUpgradeItem( ItemCode, Level, Success, Broken );
		}

		inFile>>ItemCode>>Level;
		m_pUpgradeItemList[ Count ].AddUpgradeItem( ItemCode, Level );

	}

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 현재 아이템에서 다음 업그레이드 될 아이템의 정보를 얻어줌
//--------------------------------------------------------------------------------------------
OnItemUpgradeResultInfo* CItemUpgradeParser::GetUpgradeResult( int CurrentItemCode, int magicstonekind )
{
	memset ( &m_stItemUpgradeResult, 0, sizeof(OnItemUpgradeResultInfo) );
	
	switch( magicstonekind ) 
	{
	case MAGIC_STONE_BLUE:										//푸른 마석 사용
		if ( ( CurrentItemCode - 10000 ) % 10 > 5 )				//푸른 마석 사용 불가능
		{
			m_stItemUpgradeResult.siResult = ITEM_UPGRADE_IMPOSSIBLE;
		}
		else													//푸른 마석 사용 가능
		{
			m_stItemUpgradeResult.siResult = ITEM_UPGRADE_SUCCESS;
			m_stItemUpgradeResult.uiItemID = GetNextItemCode( CurrentItemCode );
		}
		break;
	case MAGIC_STONE_RED:										//붉은 마석 사용
		m_stItemUpgradeResult = GetNextItemCodeAndResult( CurrentItemCode );
		break;
	case MAGIC_STONE_BLACK:										//검은 마석 사용
		break;
	}

	return &m_stItemUpgradeResult;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 다음 업그레이드 될 아이템의 코드 return 함
//--------------------------------------------------------------------------------------------
int CItemUpgradeParser::GetNextItemCode( int CurrentItemCode )
{
	int ListCode = ( CurrentItemCode - 10000 ) / 10 - 1;		//배열은 0 부터 시작이므로 -1 을 해줌

	return m_pUpgradeItemList[ListCode].iFindNextItemCode( CurrentItemCode );
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 다음 업그레이드 될 아이템의 코드 return 함 ( 단, 이 함수를 호출할 경우는 확률로 계산 해서 성공 실패를 반환 해줌. 물론, 성공일 경우라면 성공한 아이템 코드를 반환 한다.  )
//		   이 함수로 왔다는 건 푸른 마석이 아니라, 절대적으로 붉은 마석으로 Upgrade 할 경우에만 올 수 있다.
//--------------------------------------------------------------------------------------------
OnItemUpgradeResultInfo CItemUpgradeParser::GetNextItemCodeAndResult( int CurrentItemCode )
{
	OnItemUpgradeResultInfo ItemUpgradeResultInfo;
	
	if ( CurrentItemCode > 19999 )								//업그레이드 10단계까지 다 했으므로
	{
		OnItemUpgradeResultInfo ItemUpgradeResultInfo;
		ItemUpgradeResultInfo.siResult = ITEM_UPGRADE_IMPOSSIBLE;

		return ItemUpgradeResultInfo;
	}

	int ListCode = ( CurrentItemCode - 10000 ) / 10 - 1;		//배열은 0 부터 시작이므로 -1 을 해줌

	//확률 계산 부분 빠졌음 추가 해주면 됨
	int CurrentLevel = CurrentItemCode % 10;

	ItemUpgradeResultInfo.siResult = m_pUpgradeItemList[ListCode].GetResult( CurrentLevel, rand() % 100000 );

	ItemUpgradeResultInfo.uiItemID = m_pUpgradeItemList[ListCode].iFindNextItemCode( CurrentItemCode );

	return ItemUpgradeResultInfo;
}