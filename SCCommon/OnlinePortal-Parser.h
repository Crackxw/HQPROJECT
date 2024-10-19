#ifndef __ONLINEPORTAL_PARSER_H
#define __ONLINEPORTAL_PARSER_H

struct	PortalHeader
{
	SI16	siPortalCode;																	// Code

	SI16	siPortalType;																	// Portal Type(0 : ��, 1 : ����, 2 : ���� �ⱸ)
	SI16	siPortalImage;																	// Portal Image 
	SI16	siPortalMapXPosition;															// Map X Position
	SI16	siPortalMapYPosition;															// Map Y Position
	SI16	siPortalMapXEPosition;															// Map X End Position
	SI16	siPortalMapYEPosition;															// Map Y End Position
	SI16	siPortalMapIndex;																// Map Index
	SI16	siPortalMoveMapXPosition;														// Move Map X Position
	SI16	siPortalMoveMapYPosition;														// Move Map Y Position
	SI16	siPortalMoveMapIndex;															// Move Map Index
	SI32	siPortalDescriptionCode;														// Description Code
	SI32	siReqBattlePower;																// ���� ����ϴµ� �ʿ��� �ּ� ������
	SI32	siReqMaxLv;																		// ���� ����ϴµ� �ʿ��� �ִ� Level
	SI16	siStateSound;																	// �� ���� ������� ���� ����� ���� Type
	SI16	siTileFlag;																		// �� ���� Tile ������� �׷��������� Flag

	__int64	i64PortalMoney;																	// Portal �̿�� ��� �ݾ�
	SI32	siNeedLV;																		// Portal �̿�� �ʿ��� LV(�ݾ��� ������츸 ����)
	SI32	siNeedCredit;																	// Portal �̿�� �ʿ��� Credit(�ݾ��� ������츸 ����)

	SI32	siLimitNum;
	SI32	siStartTime;
	SI32	siEndTime;
};


class OnlinePortalParser
{
public:
	PortalHeader			*pPortalHeader;														// Portal�� ��� ������ ���� ����ü
	PortalHeader			VilliagePortalHeader;												// Village Portal�� ����(�� �ϳ��� ����)
	SI16					siPortalNum;														// �� Portal
	
public:
	OnlinePortalParser();
	virtual	~OnlinePortalParser();

	BOOL					ReadText(char *pFileName);											// Portal �ε�
	VOID					Free();																// �����

	PortalHeader*			GetPortal(SI16 XPos, SI16 YPos, SI16 MapIndex);						// Get Portal
	PortalHeader*			GetPortal(SI16 PortalCode);													// Get Portal
	SI32					GetPortalIndex(UI16 PortalCode);									// ��Ż�� �ε����� ���´�.
};


#endif
