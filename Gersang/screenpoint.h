#ifndef _SCREENPOINT_H
#define _SCREENPOINT_H

class _ScreenPoint {
public:
	// 메뉴 
	SHORT	MaxMenuXCount;                     // 메뉴가 한줄에 몇개 까지 들어갈 수 있는가?
	SHORT	MaxMenuYCount;                     // 메뉴가 세로로 몇개 까지 들어갈 수 있는가?
    SHORT	MenuButtonXsize, MenuButtonYsize;  // 메뉴 버튼의 가로 세로 크기 
	SHORT	MenuMarginX, MenuMarginY;          // 메뉴끼리의 간격 

	SHORT	MenuStartX, MenuStartY;            // 메뉴의 시작 위치 

	// 단축아이콘 
	SHORT	MaxShortCutMenuXCount;             // 메뉴가 한줄에 몇개 까지 들어갈 수 있는가?
	SHORT	MaxShortCutMenuYCount;                     // 메뉴가 세로로 몇개 까지 들어갈 수 있는가?
    SHORT	ShortCutMenuButtonXsize, ShortCutMenuButtonYsize;  // 메뉴 버튼의 가로 세로 크기 
	SHORT	ShortCutMenuMarginX, ShortCutMenuMarginY;          // 메뉴끼리의 간격 

	SHORT	ShortCutMenuStartX, ShortCutMenuStartY;            // 메뉴의 시작 위치 


	SHORT	GeneralMenuStartX, GeneralMenuStartY;
	SHORT	GeneralMenuMarginX;


	// 초상화 
	SHORT	PortraitStartX, PortraitStartY;

	SHORT	TrainUnitPortraintX, TrainUnitPortraintY;

	// 캐릭터 정보 
	SHORT	HealthInfoStartX, HealthInfoStartY;
	SHORT	CharInfoYInterval;

	// 게임 메뉴 버튼 
	SHORT	GameMenuButtonStartX, GameMenuButtonStartY;
	SHORT	GameMenuButtonXsize, GameMenuButtonYsize; 

	//동맹창 윈도우 타이틀
	SHORT	GameAllyWindowX, GameAllyWindowY;
	SHORT   GameAllyTitleX,  GameAllyTitleY;

	// 승무원 정보 
	SHORT	CrewStartX, CrewStartY;
	SHORT	CrewXsize, CrewYsize;

	// 인벤토리 메뉴 
	SI32	InventoryMenuStartX, InventoryMenuStartY, InventoryMenuMarginX, InventoryMenuXsize;

	// 시계 
	SI32	ClockStartX, ClockStartY;


	//---------------------------------
	// 온라인 관련

	// 전투모드에서 캐릭터 정보창
	SI32	OnlineCharInfoStartX, OnlineCharInfoStartY;
	// 전투모드에서 체력등 게이지 표시 좌표
	SI32	HealthGageStartX, HealthGageStartY;

	void	Init();
};

#endif