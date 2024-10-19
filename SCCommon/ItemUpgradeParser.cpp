#include <GSL.h>
#include "ItemUpgradeList.h"
#include "ItemUpgradeParser.h"
#include <fstream.h>

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ��
//--------------------------------------------------------------------------------------------
CItemUpgradeParser::CItemUpgradeParser()
{
	m_pUpgradeItemList = NULL;
	srand( time( NULL ) );

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ��
//--------------------------------------------------------------------------------------------
CItemUpgradeParser::~CItemUpgradeParser()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : �� �� ȭ
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
//	Desc : ���� ó��
//--------------------------------------------------------------------------------------------
void CItemUpgradeParser::Destroy()
{
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : Script ���� �о List�� �־��
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
		inFile>>temp;					//�ʿ� ���� ���� �� �� �Ѿ
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
//	Desc : ���� �����ۿ��� ���� ���׷��̵� �� �������� ������ �����
//--------------------------------------------------------------------------------------------
OnItemUpgradeResultInfo* CItemUpgradeParser::GetUpgradeResult( int CurrentItemCode, int magicstonekind )
{
	memset ( &m_stItemUpgradeResult, 0, sizeof(OnItemUpgradeResultInfo) );
	
	switch( magicstonekind ) 
	{
	case MAGIC_STONE_BLUE:										//Ǫ�� ���� ���
		if ( ( CurrentItemCode - 10000 ) % 10 > 5 )				//Ǫ�� ���� ��� �Ұ���
		{
			m_stItemUpgradeResult.siResult = ITEM_UPGRADE_IMPOSSIBLE;
		}
		else													//Ǫ�� ���� ��� ����
		{
			m_stItemUpgradeResult.siResult = ITEM_UPGRADE_SUCCESS;
			m_stItemUpgradeResult.uiItemID = GetNextItemCode( CurrentItemCode );
		}
		break;
	case MAGIC_STONE_RED:										//���� ���� ���
		m_stItemUpgradeResult = GetNextItemCodeAndResult( CurrentItemCode );
		break;
	case MAGIC_STONE_BLACK:										//���� ���� ���
		break;
	}

	return &m_stItemUpgradeResult;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ���� ���׷��̵� �� �������� �ڵ� return ��
//--------------------------------------------------------------------------------------------
int CItemUpgradeParser::GetNextItemCode( int CurrentItemCode )
{
	int ListCode = ( CurrentItemCode - 10000 ) / 10 - 1;		//�迭�� 0 ���� �����̹Ƿ� -1 �� ����

	return m_pUpgradeItemList[ListCode].iFindNextItemCode( CurrentItemCode );
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : ���� ���׷��̵� �� �������� �ڵ� return �� ( ��, �� �Լ��� ȣ���� ���� Ȯ���� ��� �ؼ� ���� ���и� ��ȯ ����. ����, ������ ����� ������ ������ �ڵ带 ��ȯ �Ѵ�.  )
//		   �� �Լ��� �Դٴ� �� Ǫ�� ������ �ƴ϶�, ���������� ���� �������� Upgrade �� ��쿡�� �� �� �ִ�.
//--------------------------------------------------------------------------------------------
OnItemUpgradeResultInfo CItemUpgradeParser::GetNextItemCodeAndResult( int CurrentItemCode )
{
	OnItemUpgradeResultInfo ItemUpgradeResultInfo;
	
	if ( CurrentItemCode > 19999 )								//���׷��̵� 10�ܰ���� �� �����Ƿ�
	{
		OnItemUpgradeResultInfo ItemUpgradeResultInfo;
		ItemUpgradeResultInfo.siResult = ITEM_UPGRADE_IMPOSSIBLE;

		return ItemUpgradeResultInfo;
	}

	int ListCode = ( CurrentItemCode - 10000 ) / 10 - 1;		//�迭�� 0 ���� �����̹Ƿ� -1 �� ����

	//Ȯ�� ��� �κ� ������ �߰� ���ָ� ��
	int CurrentLevel = CurrentItemCode % 10;

	ItemUpgradeResultInfo.siResult = m_pUpgradeItemList[ListCode].GetResult( CurrentLevel, rand() % 100000 );

	ItemUpgradeResultInfo.uiItemID = m_pUpgradeItemList[ListCode].iFindNextItemCode( CurrentItemCode );

	return ItemUpgradeResultInfo;
}