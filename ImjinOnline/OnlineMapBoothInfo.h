/********************************************************************************************
	파일명	:	OnlineMapBoothInfo.h
	작성자	:	이 윤 석
	담당자	:	이 준 석
	작성일	:	2002.**.**
********************************************************************************************/

#ifndef	_ONLINE_MAP_BOOTHINFO_H
#define _ONLINE_MAP_BOOTHINFO_H

#include "Online.h"
#include "OnlineCommonStruct.h"


#define MAX_BOOTHDATA		32


typedef struct
{
	SI16					sifCreate;													// 생성여부
	SI32					sifData;													// Data가 있는지 여부(없으면 서버에서 연락이 오기를 기다리는중)

	char					szItemDescript[ON_MAX_BOOTH_NOTE + 1];						// 자판 설명
} SMapBoothDescriptData;

typedef struct
{
	SI32					sifCreate;													// 생성 여부
	SI32					sifData;													// Data가 있는지 여부(없으면 서버에서 연락이 오기를 기다리는중)

	UI16					uiBoothID;													// ID
	SI32					siTileX;													// 맵 위치
	SI32					siTileY;
	SI32					siMapIndex;

	UI16					uiItemID[ON_MAX_BOOTH_ITEM];								// 자판 아이템
	SMapBoothDescriptData	stItemDescript[ON_MAX_BOOTH_ITEM];							// 자판 설명
} SMapBoothData;


class OnlineMapBoothInfo
{
public:	
	BOOL					m_fCreate;

	SMapBoothData			m_MapBoothData[MAX_BOOTHDATA];

public:	
	OnlineMapBoothInfo();																		// 생성자
	virtual ~OnlineMapBoothInfo();																// 소멸자.

	VOID			Init();																		// 초기화
	VOID			Free();																		// 해제.

	VOID			Prepare(SI32 siTileX, SI32 siTileY, SI32 siMapIndex, SI32 Range);			// Prepare
	VOID			Clear(void);																// Booth Data 모두 초기화
	VOID			AddBoothData(UI16 uiBoothID, SI32 siTileX, SI32 siTileY, SI32 siMapIndex);	// Booth Data 를 설정한다.
	VOID			DelBoothData(UI16 uiBoothID);												// Booth Data 를 지운다.
	VOID			SetBoothData(UI16 uiBoothID, SI32 siTileX, SI32 siTileY, SI32 siMapIndex, 
					OnBaseBoothItem* pOnBaseBoothItem, UI08 uiBoothItemNum);					// Booth Data 를 설정한다.
	SMapBoothData*	GetBoothData(UI16 uiBoothID);												// Booth Data 를 설정한다.

	VOID					AddBoothDataDescript(UI16 uiBoothID, UI08 SlotNum);							// Booth Data 에 Descript 설정한다.
	VOID					DelBoothDataDescript(UI16 uiBoothID, UI08 SlotNum);							// Booth Data 에 Descript 삭제한다.
	VOID					SetBoothDataDescript(UI16 uiBoothID, UI08 SlotNum, char* lpDescript);		// Booth Data 에 Descript 설정
	SMapBoothDescriptData*	GetBoothDataDescript(UI16 uiBoothID, UI08 SlotNum);							// Booth Data 에 Descript 설정
};


#endif
