/********************************************************************************************
	���ϸ�	:	OnlineMapBoothInfo.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.**.**
********************************************************************************************/

#ifndef	_ONLINE_MAP_BOOTHINFO_H
#define _ONLINE_MAP_BOOTHINFO_H

#include "Online.h"
#include "OnlineCommonStruct.h"


#define MAX_BOOTHDATA		32


typedef struct
{
	SI16					sifCreate;													// ��������
	SI32					sifData;													// Data�� �ִ��� ����(������ �������� ������ ���⸦ ��ٸ�����)

	char					szItemDescript[ON_MAX_BOOTH_NOTE + 1];						// ���� ����
} SMapBoothDescriptData;

typedef struct
{
	SI32					sifCreate;													// ���� ����
	SI32					sifData;													// Data�� �ִ��� ����(������ �������� ������ ���⸦ ��ٸ�����)

	UI16					uiBoothID;													// ID
	SI32					siTileX;													// �� ��ġ
	SI32					siTileY;
	SI32					siMapIndex;

	UI16					uiItemID[ON_MAX_BOOTH_ITEM];								// ���� ������
	SMapBoothDescriptData	stItemDescript[ON_MAX_BOOTH_ITEM];							// ���� ����
} SMapBoothData;


class OnlineMapBoothInfo
{
public:	
	BOOL					m_fCreate;

	SMapBoothData			m_MapBoothData[MAX_BOOTHDATA];

public:	
	OnlineMapBoothInfo();																		// ������
	virtual ~OnlineMapBoothInfo();																// �Ҹ���.

	VOID			Init();																		// �ʱ�ȭ
	VOID			Free();																		// ����.

	VOID			Prepare(SI32 siTileX, SI32 siTileY, SI32 siMapIndex, SI32 Range);			// Prepare
	VOID			Clear(void);																// Booth Data ��� �ʱ�ȭ
	VOID			AddBoothData(UI16 uiBoothID, SI32 siTileX, SI32 siTileY, SI32 siMapIndex);	// Booth Data �� �����Ѵ�.
	VOID			DelBoothData(UI16 uiBoothID);												// Booth Data �� �����.
	VOID			SetBoothData(UI16 uiBoothID, SI32 siTileX, SI32 siTileY, SI32 siMapIndex, 
					OnBaseBoothItem* pOnBaseBoothItem, UI08 uiBoothItemNum);					// Booth Data �� �����Ѵ�.
	SMapBoothData*	GetBoothData(UI16 uiBoothID);												// Booth Data �� �����Ѵ�.

	VOID					AddBoothDataDescript(UI16 uiBoothID, UI08 SlotNum);							// Booth Data �� Descript �����Ѵ�.
	VOID					DelBoothDataDescript(UI16 uiBoothID, UI08 SlotNum);							// Booth Data �� Descript �����Ѵ�.
	VOID					SetBoothDataDescript(UI16 uiBoothID, UI08 SlotNum, char* lpDescript);		// Booth Data �� Descript ����
	SMapBoothDescriptData*	GetBoothDataDescript(UI16 uiBoothID, UI08 SlotNum);							// Booth Data �� Descript ����
};


#endif
