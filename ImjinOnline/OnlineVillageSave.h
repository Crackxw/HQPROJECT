/****************************************************************************************
	파 일 명 : OnlineVillageSave.h
	작 성 자 : 정진욱
	담 담 자 : 이준석
	작 성 일 : 2001.11.12
	수 정 일 : 2002.01.16
****************************************************************************************/


#ifndef	__VILLAGESAVE_H
#define __VILLAGESAVE_H


struct SAVEFILE_HEADER
{
	UI16							m_uiVillageCode;												// 마을 Code
	CHAR							m_SaveFileName[10];												// 각 해당 마을의 파일 이름	
};

struct SAVEFILE_DATA
{
	UI16							m_uiItemID;														// 이 마을에서 취급하는 아이템 아디
	UI16							m_siItemQuantity;												// 이 아이템의 수량
	UI32							m_uiBuyValue;													// 이 아이템의 판매 가격
	UI32							m_uiSellValue;													// 이 아이템의 구매 가격
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
	SAVE_DATE						m_Date;															// 날짜
	SI16							m_siItemNum;													// 이 마을에서 취급하는 아이템 수
	SAVEFILE_DATA					m_pData[20];
};

struct SAVEFILE_SEARCH
{
	SAVE_DATE						m_Date;															// 날짜
	SAVEFILE_DATA					m_Data;															// 데이타
	UI16							m_uiVillageCode;												// 마을 Code	
};

struct SAVEFILE_SORT
{
	UI16							m_uiVillageCode;												// 마을 Code
	UI16							m_uiBuyVillageCode;												// 잴 싼 마을
	UI32							m_uiItemID;
	__int64							m_nProfit;														// 이익
	__int64							m_nSell, m_nBuy;
	SAVE_DATE						m_Date;															// 날짜
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
// 2004/9/19 중국에 마을이 추가되어 기존에 300개 정도의 마을을 기준으로 만든 이 정보가 넘쳐서 에러가 발생한다.
// 이에 따라 값을 변경한다. 추후에 마을이 더 추가되어 아래의 값보다 커질 경우 반드시 수정해야 한다.
// 간혹 몇 백개의 마을을 돌아다니며 이보다 큰 값을 만드는 사람이 존재한다.
#define COUNT_TRADE_DATA			512		// 장부의 데이타 개수
#define COUNT_SEARCH_DATA			512		// 검색 데이터 개수


class  OnlineVillageSave
{
private:
	SAVEFILE_HEADER					*m_pSaveHeader;													// 각 마을의 ID와 각 마을 마다의 파을 이름을 저장 하고 있다
	SAVEFILE_DATAHEADER				m_pTradeData[COUNT_TRADE_DATA];												// 장부의 데이타를 저장하고 있따
	SASAVEFILE_SEARCH				m_pSearchData[COUNT_SEARCH_DATA];

	ITemSearchTable					*m_pITemSearchTable;

	SAVEFILE_SEARCH					*m_pSearchHeader;
	SAVEFILE_SEARCH					*m_pTempSeach;
	SAVEFILE_SORT					*m_pSort;														// 아이템 검색	
	SAVEFILE_SORT					*m_pAllSearch;
	cltOnlineWorld					*m_pMyOnlineWorld;
	SI32							m_siVillageIndex;												// 지금 로드 하고 있는 세이브 헤더의 인덱스
	SI32							m_siTotalVillage;												// 헤더의 총수
	CHAR							m_szPath[100];
	SI16							m_siVersion;													// 장부 버전
	BOOL							m_bSearch;
	SI16							m_siSortVillageNum, m_siCount, m_siAllCount;
	SI16							m_siSearchNum;
	UI32							m_uiSortItemID;
	SAVEFILE_ITEMTABLE				*m_pItemIDTable;												// 모든 아이템 검색시 임시로 아이템 아디를 모두 저장하고 있는 테이블
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


	BOOL							SaveInfo( UI16 uiVillageCode, SAVEFILE_DATAHEADER *pHeader );		// 저장
	SAVEFILE_DATAHEADER				*GetData( UI16 uiVillageCode );									// 로드
	SI32							GetVillageIndex( UI16 uiVillageCode );							// 헤더로 부터 찾아 구조체를 인덱스를 리턴
	SI32							GetTotal();														// 총수를 리턴함둥
	SAVEFILE_HEADER					*GetData();														// 헤더를 리턴 함
	SAVEFILE_SEARCH					*Search( UI32 uiItemID, SI16 &siVillageNum );					// 아이템이 파는 마을 리턴해 준다.
	SAVEFILE_SORT					*SortSearch( UI32 uiItemID, SI16 &siVillageNum );				// 아이템을 정렬해서 가장 이득이 많이 나는 순으로 정렬을 해준다.
	SAVEFILE_SORT					*SortSearch(SI16 &siSearchNum );								// 전체 검색
	VOID							StopSearch();													// 검색 중지
	SAVEFILE_SORT					*GetSearchData( SI16 &siVillageNum );							// 검색 되어 있는 데이타를 리턴을 해준다.
	VOID							CheckSearchItem( UI16 uiVillageCode );							// 새로운 데이타가 들어 오면 검색을 해준 데이타와 비교를 한다.
	BOOL							DeleteTrade();													// 장부 파일을 지운다.
	VOID							GetLoadingPercent(CHAR *pTemp);									// 현재 몇 % 로딩을 했는지 알아 온다.
	BOOL							GetSuccess()	{ return m_bSuccess; };							// 검색 성공
	SI32							GetVillageNum()	{ return (m_siTotalVillage-1);};
};


#endif