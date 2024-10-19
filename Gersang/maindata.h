#ifndef _MAINDATA_H
#define _MAINDATA_H   1

#include <movemark.h>
#include "rain.h"
#include "mytime.h"
#include "earthquake.h"
#include "order.h"
#include <ridedata.h>
#include <player.h>
#include "message.h"
#include <KeyBuffer.h>

#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character

#define MAX_ORDER_BUFFER_NUMBER       3      // 프레임당 입력 가능한 최대 명령 개수 

#define SELECTEDORDEROPTION_KEYPRESS   1     // 단축키를 사용한 명령입력 
#define SELECTEDORDEROPTION_LEFTPRESS  2     // 왼쪽   마우스 버튼을 사용한 명령 입력 
#define SELECTEDORDEROPTION_RIGHTPRESS 4     // 오른쪽 마우스 버튼을 사용한 명령 입력 
#define SELECTEDORDEROPTION_AUTO       8     // 자동 입력 

class _ObjectBuffer{
public:
	int X, Y;
	_NewID ID;
};

class _MainData {

public:

	unsigned int Frame;
	unsigned int GameFrame;
	unsigned int RandomCount;
	unsigned int RandomSeed;
	unsigned int Rand;
	unsigned int OldRand;

	unsigned short ProducedCharNumber;

	_Time Time;

	_clKeyBuffer KB;


	// 화면 흔들림 관련
	_EarthQuake EarthQuake;

	_MoveMark MoveMark;

	SHORT Reserved16[5];

	// 메시지 관련
	SHORT MessageIndex;
	SHORT MessageDelay;
	char		Message[MAX_MESSAGE_NUMBER][MAX_MESSAGE_TEXT_LENGTH];
	COLORREF	MessageColor[MAX_MESSAGE_NUMBER];
	SHORT Message2Delay;
	char  Message2[MAX_MESSAGE_TEXT_LENGTH];
	SHORT PlayerMessageDelay;
	char  ObjectiveMessage[MAX_MESSAGE_TEXT_LENGTH];
	BOOL  bObjectiveMessageSwitch;

	// 작은 지도 관련 
	SHORT SmapModeChangedSwitch;
	SHORT SmapModeFullSwitch;

	// 게임에서 승리 했는가?
	SHORT WinSwitch;

	// 도움말관련 
	SHORT HelpMenuOfMouse;
	DWORD HelpMenuClock;

	SHORT HelpShortCutMenuOfMouse;
	DWORD HelpShortCutMenuClock;


	SHORT MoveCharNumber;
	SHORT MoveCharID;

	char ObjectNeedSwitch;
	SHORT SelectedOrder;            // 선택된 명령 
	SHORT SelectedOrderOption;      // 명령 선택에 관계된 자세한 정보 
	SHORT ObjectNeedType;           // 필요한 오브젝트의 갯수 

	SHORT DarkLayor;           

	///// 메뉴 관련 정보 //////////////////////////////
	SHORT Menu[MAX_MENU_NUMBER];
	SHORT MenuMode[MAX_MENU_NUMBER];
	SHORT MenuReserve[MAX_MENU_NUMBER];

	SHORT SelectedTeam;        // 현재 선택되어진 팀번호 
	SHORT PressedTeam;         // 단축키가 눌려진 팀번호 
	_Team Team;

	_OrderData OrderBuffer[MAX_ORDER_BUFFER_NUMBER];
	SHORT OrderBufferIndex;

	_OrderData OrderQue[MAX_MULTI_PLAYER][MAX_ORDER_QUE];

	
	char OrderBufferSwitch;

	SHORT ObjectBufferIndex;
	_ObjectBuffer ObjectBuffer[MAX_MULTI_ORDER_NUMBER];


	// 캐릭터 선택관련 변수 //
	SI32 SelectedNumber;           // 선택된 캐릭터의 갯수 
	_NewID SelectedID[MAX_SELECTED_CHAR_NUMBER];
	SHORT   IDPara1[MAX_SELECTED_CHAR_NUMBER]; // 각 아이디 값의 파라메타 
	SHORT   IDPara2[MAX_SELECTED_CHAR_NUMBER]; // 각 아이디 값의 파라메타 

	SHORT CharInfo[MAX_CHAR_NUMBER];

	SHORT CharNumber[MAX_MULTI_PLAYER][KIND_ON_MAX];
	_NewID AirCharID[MAX_CHAR_NUMBER];                      // 비행 캐릭터의 아이디 저장 
	SHORT AirCharNumber;                                   // 비행 캐릭터의 갯수 
	_NewID LandCharID[MAX_CHAR_NUMBER];
	SHORT LandCharNumber;

	SHORT BlackCheckStartID;
	char BlackMap[MAX_MULTI_PLAYER][MAX_MAP_XSIZE][MAX_MAP_YSIZE];	// 프로그래밍적 배열 
	UI16 BlackMap2[MAX_MAP_XSIZE][MAX_MAP_YSIZE];// 그래픽 처리를 위한 배열. 
	char BlackMap2ChangedSwitch[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
	char LastLevel[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
	unsigned char SmallMapBase[MAX_MAP_XSIZE][MAX_MAP_YSIZE];

	_Player Player[MAX_MULTI_PLAYER];
    SHORT BulletInfo[MAX_BULLET_NUMBER]; // 총알에 대한 정보를 저장할 공간 
	SHORT SmokeInfo[MAX_SMOKE_NUMBER]; // 연기에 대한 정보를 저장할 공간 

	_Rain Rain;

	SHORT BattleFieldX, BattleFieldY;
	int ScreenMoveSwitch;
	int ScreenMoveX, ScreenMoveY;
	SHORT HomeX, HomeY;


	// 유닛 생성 순서 
	SHORT CharOrder[MAX_CHAR_NUMBER];
	SHORT CharOrderIndex;

	int VictoryDelay;        // 승리후의 지연시간 
	int DefeatDelay;         // 패배 후의 지연시간 

	// 게임 모드 관련 변수 
	SHORT GameMode; 

	DWORD AccumulatedPlaySecond;     // 누적된 시간을 보여준다. 
	DWORD TempAccumulatedPlayClock; // 시간 누적을 위해서 초기화한다.  

	DWORD CheaterProtectCrop[MAX_MULTI_PLAYER] ;
	DWORD CheaterProtectTree[MAX_MULTI_PLAYER] ;

	DWORD SyncSignal;          // 동기 신호 

	// 작은 지도 그리기 관련. 
	BOOL IsSmallMapValidSwitch;
	BOOL SmallMapDrawSwitch;
	DWORD DrawSmallMapFrame;

	// 모든 업그레이드를 완료시킬 것인가
	BOOL bUpgradeAllSwitch;

	/// 자동 마법 사용가능한가  ?
	BOOL bAutoMagicEnableSwitch;

	// 온라인 게임 속도로 진행할 것인가?
	BOOL bOnlineSpeedSwitch;

	// 직선 시야 방식을 사용하는가?
	BOOL StraightSightMode;

	SI32 InitInitMenuCount;
	SI32 InitQuitCount;

	DWORD PlayTime_CurrentTime;

	DWORD Reserved[78];

	BOOL m_bBlackCheck;		//전투모드에서 ESC버튼을 눌렀을때 시야체크 루틴에 들어가지 않는다.

	BOOL OnlineAllyNumSwitch[MAX_MULTI_PLAYER][MAX_MULTI_PLAYER];  // 동맹상태(0: 동맹 없음, 1: 동맹 신청요청, 2: 동맹 신청받음, 3: 동맹 상태).


	void Init();
	unsigned int Random()
	{
        RandomSeed=(RandomSeed*65411)%65531;
        return RandomSeed;
	}

	BOOL PushObject(SHORT x, SHORT y, _NewID id);
	BOOL IncreaseCharNumber(SHORT pnumber, SHORT kind);
	BOOL DecreaseCharNumber(SHORT pnumber, SHORT kind);
	void IncreaseFlyCharNumber(_NewID id);
	void DecreaseFlyCharNumber(_NewID id);

	// 시야를 처리한다. 
    void BlackCheck(SHORT mode,BOOL bPKFieldMap);

	// 완전히 어두운 포그속에 있는가? 
    BOOL IsInBlackFog(SHORT x, SHORT y);
    BOOL IsInMidFog(SHORT x, SHORT y);
	// 중간 포그나 그보다 어두운 곳에 있는가?


	SHORT GetBlackMap2(SHORT x, SHORT y);
	BOOL SetBlackMap2(SI32 tilex, SI32 tiley, SI32 pos, SI32 data);
	UI16 GetBlackMap2(SI32 tilex, SI32 tiley, SI32 pos);
	BOOL IsCompleteBlackBlackMap2(SI32 tilex, SI32 tiley);
	BOOL IsCompleteWhiteBlackMap2(SI32 tilex, SI32 tiley);
	BOOL IsDarkFogBlackMap2(SI32 tilex, SI32 tiley);

	SHORT GetMenu(SHORT index, int unique);
	SHORT GetGeneralMenu(SHORT index);
	void SetMenu(SHORT index, SHORT menu, SHORT reservenumber, SHORT mode, SHORT menupercent);
	void SetGeneralMenu(SHORT index, SHORT menu);

	void PushOrderBuffer(_OrderData order);
	void PopOrderBuffer();

    // 명령을 단축 아이콘에 넣는다. 
    void PushShortCutMenu(_NewID id, SHORT item);


	void DeleteAirCharID(_NewID id);
	void DeleteLandCharID(_NewID id);

	void SetBattleField(SHORT x, SHORT y);
	void SetScreenMove(int x, int y);

	// 전투 결과를 저장한다. 
    BOOL SaveResult(SHORT winswitch);

	void SetSmallMapValidSwitch(BOOL bMode)
	{
		IsSmallMapValidSwitch = bMode;
	}

	BOOL GetSmallMapValidSwitch()
	{
		return IsSmallMapValidSwitch ;
	}


	VOID SetUpgradeAllSwitch(BOOL mode) { bUpgradeAllSwitch = mode; }
	BOOL GetUpgradeAllSwitch() { return bUpgradeAllSwitch; }

	VOID SetAutoMagicEnableSwitch(BOOL mode);
	BOOL GetAutoMagicEnableSwitch() { return bAutoMagicEnableSwitch; }

	VOID SetOnlineSpeedSwitch(BOOL mode){bOnlineSpeedSwitch = mode;}
	BOOL GetOnlineSpeedSwitch() { return bOnlineSpeedSwitch; }

	// 캐릭터들의 시야를 직선으로 처리하는지 여부를 결정한다. 
	BOOL SetStraightSightMode(BOOL mode)
	{
		StraightSightMode = mode;
		return TRUE;
	}

	BOOL GetStraightSightMode()
	{
		return StraightSightMode;
	}


	// 선택된 캐릭터의 플레이어 번호를 돌려준다. (선택된 캐릭터중 첫번째 캐릭터의 플레이어 번호다. )
	SI32 FindSelectedCharPlayer();

	// pnumber1가 pnumber2에게 친절한가 ?
	BOOL IsFriendTo(SI32 pnumber1, SI32 pnumber2);

};


#endif
