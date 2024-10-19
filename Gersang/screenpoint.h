#ifndef _SCREENPOINT_H
#define _SCREENPOINT_H

class _ScreenPoint {
public:
	// �޴� 
	SHORT	MaxMenuXCount;                     // �޴��� ���ٿ� � ���� �� �� �ִ°�?
	SHORT	MaxMenuYCount;                     // �޴��� ���η� � ���� �� �� �ִ°�?
    SHORT	MenuButtonXsize, MenuButtonYsize;  // �޴� ��ư�� ���� ���� ũ�� 
	SHORT	MenuMarginX, MenuMarginY;          // �޴������� ���� 

	SHORT	MenuStartX, MenuStartY;            // �޴��� ���� ��ġ 

	// ��������� 
	SHORT	MaxShortCutMenuXCount;             // �޴��� ���ٿ� � ���� �� �� �ִ°�?
	SHORT	MaxShortCutMenuYCount;                     // �޴��� ���η� � ���� �� �� �ִ°�?
    SHORT	ShortCutMenuButtonXsize, ShortCutMenuButtonYsize;  // �޴� ��ư�� ���� ���� ũ�� 
	SHORT	ShortCutMenuMarginX, ShortCutMenuMarginY;          // �޴������� ���� 

	SHORT	ShortCutMenuStartX, ShortCutMenuStartY;            // �޴��� ���� ��ġ 


	SHORT	GeneralMenuStartX, GeneralMenuStartY;
	SHORT	GeneralMenuMarginX;


	// �ʻ�ȭ 
	SHORT	PortraitStartX, PortraitStartY;

	SHORT	TrainUnitPortraintX, TrainUnitPortraintY;

	// ĳ���� ���� 
	SHORT	HealthInfoStartX, HealthInfoStartY;
	SHORT	CharInfoYInterval;

	// ���� �޴� ��ư 
	SHORT	GameMenuButtonStartX, GameMenuButtonStartY;
	SHORT	GameMenuButtonXsize, GameMenuButtonYsize; 

	//����â ������ Ÿ��Ʋ
	SHORT	GameAllyWindowX, GameAllyWindowY;
	SHORT   GameAllyTitleX,  GameAllyTitleY;

	// �¹��� ���� 
	SHORT	CrewStartX, CrewStartY;
	SHORT	CrewXsize, CrewYsize;

	// �κ��丮 �޴� 
	SI32	InventoryMenuStartX, InventoryMenuStartY, InventoryMenuMarginX, InventoryMenuXsize;

	// �ð� 
	SI32	ClockStartX, ClockStartY;


	//---------------------------------
	// �¶��� ����

	// ������忡�� ĳ���� ����â
	SI32	OnlineCharInfoStartX, OnlineCharInfoStartY;
	// ������忡�� ü�µ� ������ ǥ�� ��ǥ
	SI32	HealthGageStartX, HealthGageStartY;

	void	Init();
};

#endif