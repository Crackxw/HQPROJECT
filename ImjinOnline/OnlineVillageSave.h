/****************************************************************************************
	�� �� �� : OnlineVillageSave.h
	�� �� �� : ������
	�� �� �� : ���ؼ�
	�� �� �� : 2001.11.12
	�� �� �� : 2002.01.16
****************************************************************************************/


#ifndef	__VILLAGESAVE_H
#define __VILLAGESAVE_H


struct SAVEFILE_HEADER
{
	UI16							m_uiVillageCode;												// ���� Code
	CHAR							m_SaveFileName[10];												// �� �ش� ������ ���� �̸�	
};

struct SAVEFILE_DATA
{
	UI16							m_uiItemID;														// �� �������� ����ϴ� ������ �Ƶ�
	UI16							m_siItemQuantity;												// �� �������� ����
	UI32							m_uiBuyValue;													// �� �������� �Ǹ� ����
	UI32							m_uiSellValue;													// �� �������� ���� ����
};
struct SAVE_DATE
{
	SI16							m_siYear;
	SI16							m_siMonth;
	SI16							m_siDay;
	SI16							m_siHour;
};

struct SAVEFILE_DATAHEADER
{
	SAVE_DATE						m_Date;															// ��¥
	SI16							m_siItemNum;													// �� �������� ����ϴ� ������ ��
	SAVEFILE_DATA					m_pData[20];
};

struct SAVEFILE_SEARCH
{
	SAVE_DATE						m_Date;															// ��¥
	SAVEFILE_DATA					m_Data;															// ����Ÿ
	UI16							m_uiVillageCode;												// ���� Code	
};

struct SAVEFILE_SORT
{
	UI16							m_uiVillageCode;												// ���� Code
	UI16							m_uiBuyVillageCode;												// �� �� ����
	UI32							m_uiItemID;
	__int64							m_nProfit;														// ����
	__int64							m_nSell, m_nBuy;
	SAVE_DATE						m_Date;															// ��¥
};

struct SAVEFILE_ITEMTABLE
{
	UI32							*m_pItemTable;
	SI16							m_siItemNum;
};

struct SASAVEFILE_SEARCH
{
	UI32							m_uiKey;
	SI16							m_siIndex;
};

struct ITemSearchTable
{
	SAVE_DATE						m_Date;
	UI16							m_uiItemID;
	UI32							m_uiBuy, m_uiSell;
	UI16							m_uiBuyVillageCode;
	UI16							m_uiSellVillageCode;
	SI32							m_siProfit;
};

// robypark 2004/9/21 15:00
// 2004/9/19 �߱��� ������ �߰��Ǿ� ������ 300�� ������ ������ �������� ���� �� ������ ���ļ� ������ �߻��Ѵ�.
// �̿� ���� ���� �����Ѵ�. ���Ŀ� ������ �� �߰��Ǿ� �Ʒ��� ������ Ŀ�� ��� �ݵ�� �����ؾ� �Ѵ�.
// ��Ȥ �� �鰳�� ������ ���ƴٴϸ� �̺��� ū ���� ����� ����� �����Ѵ�.
#define COUNT_TRADE_DATA			512		// ����� ����Ÿ ����
#define COUNT_SEARCH_DATA			512		// �˻� ������ ����


class  OnlineVillageSave
{
private:
	SAVEFILE_HEADER					*m_pSaveHeader;													// �� ������ ID�� �� ���� ������ ���� �̸��� ���� �ϰ� �ִ�
	SAVEFILE_DATAHEADER				m_pTradeData[COUNT_TRADE_DATA];												// ����� ����Ÿ�� �����ϰ� �ֵ�
	SASAVEFILE_SEARCH				m_pSearchData[COUNT_SEARCH_DATA];

	ITemSearchTable					*m_pITemSearchTable;

	SAVEFILE_SEARCH					*m_pSearchHeader;
	SAVEFILE_SEARCH					*m_pTempSeach;
	SAVEFILE_SORT					*m_pSort;														// ������ �˻�	
	SAVEFILE_SORT					*m_pAllSearch;
	cltOnlineWorld					*m_pMyOnlineWorld;
	SI32							m_siVillageIndex;												// ���� �ε� �ϰ� �ִ� ���̺� ����� �ε���
	SI32							m_siTotalVillage;												// ����� �Ѽ�
	CHAR							m_szPath[100];
	SI16							m_siVersion;													// ��� ����
	BOOL							m_bSearch;
	SI16							m_siSortVillageNum, m_siCount, m_siAllCount;
	SI16							m_siSearchNum;
	UI32							m_uiSortItemID;
	SAVEFILE_ITEMTABLE				*m_pItemIDTable;												// ��� ������ �˻��� �ӽ÷� ������ �Ƶ� ��� �����ϰ� �ִ� ���̺�
	BOOL							m_bError;
	UI08							m_uiPrecent01;
	BOOL							m_bSuccess;

public:
	OnlineVillageSave();
	~OnlineVillageSave();


	VOID							Init( cltOnlineWorld *pMyOnlineWorld );
	VOID							Initialize();
	VOID							Free();
	VOID							Load();		
	VOID							Save();
	VOID							SearchSave( SAVEFILE_SORT *pSort, SI16 siNum );


	BOOL							SaveInfo( UI16 uiVillageCode, SAVEFILE_DATAHEADER *pHeader );		// ����
	SAVEFILE_DATAHEADER				*GetData( UI16 uiVillageCode );									// �ε�
	SI32							GetVillageIndex( UI16 uiVillageCode );							// ����� ���� ã�� ����ü�� �ε����� ����
	SI32							GetTotal();														// �Ѽ��� �����Ե�
	SAVEFILE_HEADER					*GetData();														// ����� ���� ��
	SAVEFILE_SEARCH					*Search( UI32 uiItemID, SI16 &siVillageNum );					// �������� �Ĵ� ���� ������ �ش�.
	SAVEFILE_SORT					*SortSearch( UI32 uiItemID, SI16 &siVillageNum );				// �������� �����ؼ� ���� �̵��� ���� ���� ������ ������ ���ش�.
	SAVEFILE_SORT					*SortSearch(SI16 &siSearchNum );								// ��ü �˻�
	VOID							StopSearch();													// �˻� ����
	SAVEFILE_SORT					*GetSearchData( SI16 &siVillageNum );							// �˻� �Ǿ� �ִ� ����Ÿ�� ������ ���ش�.
	VOID							CheckSearchItem( UI16 uiVillageCode );							// ���ο� ����Ÿ�� ��� ���� �˻��� ���� ����Ÿ�� �񱳸� �Ѵ�.
	BOOL							DeleteTrade();													// ��� ������ �����.
	VOID							GetLoadingPercent(CHAR *pTemp);									// ���� �� % �ε��� �ߴ��� �˾� �´�.
	BOOL							GetSuccess()	{ return m_bSuccess; };							// �˻� ����
	SI32							GetVillageNum()	{ return (m_siTotalVillage-1);};
};


#endif