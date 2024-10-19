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

#define MAX_ORDER_BUFFER_NUMBER       3      // �����Ӵ� �Է� ������ �ִ� ��� ���� 

#define SELECTEDORDEROPTION_KEYPRESS   1     // ����Ű�� ����� ����Է� 
#define SELECTEDORDEROPTION_LEFTPRESS  2     // ����   ���콺 ��ư�� ����� ��� �Է� 
#define SELECTEDORDEROPTION_RIGHTPRESS 4     // ������ ���콺 ��ư�� ����� ��� �Է� 
#define SELECTEDORDEROPTION_AUTO       8     // �ڵ� �Է� 

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


	// ȭ�� ��鸲 ����
	_EarthQuake EarthQuake;

	_MoveMark MoveMark;

	SHORT Reserved16[5];

	// �޽��� ����
	SHORT MessageIndex;
	SHORT MessageDelay;
	char		Message[MAX_MESSAGE_NUMBER][MAX_MESSAGE_TEXT_LENGTH];
	COLORREF	MessageColor[MAX_MESSAGE_NUMBER];
	SHORT Message2Delay;
	char  Message2[MAX_MESSAGE_TEXT_LENGTH];
	SHORT PlayerMessageDelay;
	char  ObjectiveMessage[MAX_MESSAGE_TEXT_LENGTH];
	BOOL  bObjectiveMessageSwitch;

	// ���� ���� ���� 
	SHORT SmapModeChangedSwitch;
	SHORT SmapModeFullSwitch;

	// ���ӿ��� �¸� �ߴ°�?
	SHORT WinSwitch;

	// ���򸻰��� 
	SHORT HelpMenuOfMouse;
	DWORD HelpMenuClock;

	SHORT HelpShortCutMenuOfMouse;
	DWORD HelpShortCutMenuClock;


	SHORT MoveCharNumber;
	SHORT MoveCharID;

	char ObjectNeedSwitch;
	SHORT SelectedOrder;            // ���õ� ��� 
	SHORT SelectedOrderOption;      // ��� ���ÿ� ����� �ڼ��� ���� 
	SHORT ObjectNeedType;           // �ʿ��� ������Ʈ�� ���� 

	SHORT DarkLayor;           

	///// �޴� ���� ���� //////////////////////////////
	SHORT Menu[MAX_MENU_NUMBER];
	SHORT MenuMode[MAX_MENU_NUMBER];
	SHORT MenuReserve[MAX_MENU_NUMBER];

	SHORT SelectedTeam;        // ���� ���õǾ��� ����ȣ 
	SHORT PressedTeam;         // ����Ű�� ������ ����ȣ 
	_Team Team;

	_OrderData OrderBuffer[MAX_ORDER_BUFFER_NUMBER];
	SHORT OrderBufferIndex;

	_OrderData OrderQue[MAX_MULTI_PLAYER][MAX_ORDER_QUE];

	
	char OrderBufferSwitch;

	SHORT ObjectBufferIndex;
	_ObjectBuffer ObjectBuffer[MAX_MULTI_ORDER_NUMBER];


	// ĳ���� ���ð��� ���� //
	SI32 SelectedNumber;           // ���õ� ĳ������ ���� 
	_NewID SelectedID[MAX_SELECTED_CHAR_NUMBER];
	SHORT   IDPara1[MAX_SELECTED_CHAR_NUMBER]; // �� ���̵� ���� �Ķ��Ÿ 
	SHORT   IDPara2[MAX_SELECTED_CHAR_NUMBER]; // �� ���̵� ���� �Ķ��Ÿ 

	SHORT CharInfo[MAX_CHAR_NUMBER];

	SHORT CharNumber[MAX_MULTI_PLAYER][KIND_ON_MAX];
	_NewID AirCharID[MAX_CHAR_NUMBER];                      // ���� ĳ������ ���̵� ���� 
	SHORT AirCharNumber;                                   // ���� ĳ������ ���� 
	_NewID LandCharID[MAX_CHAR_NUMBER];
	SHORT LandCharNumber;

	SHORT BlackCheckStartID;
	char BlackMap[MAX_MULTI_PLAYER][MAX_MAP_XSIZE][MAX_MAP_YSIZE];	// ���α׷����� �迭 
	UI16 BlackMap2[MAX_MAP_XSIZE][MAX_MAP_YSIZE];// �׷��� ó���� ���� �迭. 
	char BlackMap2ChangedSwitch[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
	char LastLevel[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
	unsigned char SmallMapBase[MAX_MAP_XSIZE][MAX_MAP_YSIZE];

	_Player Player[MAX_MULTI_PLAYER];
    SHORT BulletInfo[MAX_BULLET_NUMBER]; // �Ѿ˿� ���� ������ ������ ���� 
	SHORT SmokeInfo[MAX_SMOKE_NUMBER]; // ���⿡ ���� ������ ������ ���� 

	_Rain Rain;

	SHORT BattleFieldX, BattleFieldY;
	int ScreenMoveSwitch;
	int ScreenMoveX, ScreenMoveY;
	SHORT HomeX, HomeY;


	// ���� ���� ���� 
	SHORT CharOrder[MAX_CHAR_NUMBER];
	SHORT CharOrderIndex;

	int VictoryDelay;        // �¸����� �����ð� 
	int DefeatDelay;         // �й� ���� �����ð� 

	// ���� ��� ���� ���� 
	SHORT GameMode; 

	DWORD AccumulatedPlaySecond;     // ������ �ð��� �����ش�. 
	DWORD TempAccumulatedPlayClock; // �ð� ������ ���ؼ� �ʱ�ȭ�Ѵ�.  

	DWORD CheaterProtectCrop[MAX_MULTI_PLAYER] ;
	DWORD CheaterProtectTree[MAX_MULTI_PLAYER] ;

	DWORD SyncSignal;          // ���� ��ȣ 

	// ���� ���� �׸��� ����. 
	BOOL IsSmallMapValidSwitch;
	BOOL SmallMapDrawSwitch;
	DWORD DrawSmallMapFrame;

	// ��� ���׷��̵带 �Ϸ��ų ���ΰ�
	BOOL bUpgradeAllSwitch;

	/// �ڵ� ���� ��밡���Ѱ�  ?
	BOOL bAutoMagicEnableSwitch;

	// �¶��� ���� �ӵ��� ������ ���ΰ�?
	BOOL bOnlineSpeedSwitch;

	// ���� �þ� ����� ����ϴ°�?
	BOOL StraightSightMode;

	SI32 InitInitMenuCount;
	SI32 InitQuitCount;

	DWORD PlayTime_CurrentTime;

	DWORD Reserved[78];

	BOOL m_bBlackCheck;		//������忡�� ESC��ư�� �������� �þ�üũ ��ƾ�� ���� �ʴ´�.

	BOOL OnlineAllyNumSwitch[MAX_MULTI_PLAYER][MAX_MULTI_PLAYER];  // ���ͻ���(0: ���� ����, 1: ���� ��û��û, 2: ���� ��û����, 3: ���� ����).


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

	// �þ߸� ó���Ѵ�. 
    void BlackCheck(SHORT mode,BOOL bPKFieldMap);

	// ������ ��ο� ���׼ӿ� �ִ°�? 
    BOOL IsInBlackFog(SHORT x, SHORT y);
    BOOL IsInMidFog(SHORT x, SHORT y);
	// �߰� ���׳� �׺��� ��ο� ���� �ִ°�?


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

    // ����� ���� �����ܿ� �ִ´�. 
    void PushShortCutMenu(_NewID id, SHORT item);


	void DeleteAirCharID(_NewID id);
	void DeleteLandCharID(_NewID id);

	void SetBattleField(SHORT x, SHORT y);
	void SetScreenMove(int x, int y);

	// ���� ����� �����Ѵ�. 
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

	// ĳ���͵��� �þ߸� �������� ó���ϴ��� ���θ� �����Ѵ�. 
	BOOL SetStraightSightMode(BOOL mode)
	{
		StraightSightMode = mode;
		return TRUE;
	}

	BOOL GetStraightSightMode()
	{
		return StraightSightMode;
	}


	// ���õ� ĳ������ �÷��̾� ��ȣ�� �����ش�. (���õ� ĳ������ ù��° ĳ������ �÷��̾� ��ȣ��. )
	SI32 FindSelectedCharPlayer();

	// pnumber1�� pnumber2���� ģ���Ѱ� ?
	BOOL IsFriendTo(SI32 pnumber1, SI32 pnumber2);

};


#endif
