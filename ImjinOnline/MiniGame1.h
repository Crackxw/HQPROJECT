#ifndef _MINIGAME1_H
#define _MINIGAME1_H	1

#include <Main.h>
#include <Mouse.h>
#include "..\Gersang\maindata.h"
#include <Etc.h>

#include "OnlineWorld.h"
#include "MiniGame1.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include "OnlineText.h"

#define MINIGAME1_STANDARD_SCORE    (40-10)   // 30
#define MINIGAME2_STANDARD_SCORE    (40+35)   // 75
#define MINIGAME3_STANDARD_SCORE    (20+30)   // 50

#define MINIGAME_MAX_KEY	1000

// �̴ϰ��ӿ��� ���Ǵ� �̹����� ����. 
#define MAX_MINIGAME_SPR_NUMER	10

//-------------------------------------------------------------
// �̴ϰ��� 1�� define
//-------------------------------------------------------------
#define MINIGAME1_MAX_XNUMBER    10
#define MINIGAME1_MAX_YNUMBER    10

#define MINIGAME1_CHIP_SHOW_DELAY   40 // 8
#define MINIGAME1_CHIP_ANI_DELAY    10 // 3
#define MINIGAME1_CHIP_FOREVER_DELAY 100


#define		MAX_FILE_NUMBER_MINIGAME1  2

#define		FILE_MINIGAME1_BACK			0
#define		FILE_MINIGAME1_CARD			1

//-------------------------------------------------------------
// �̴ϰ��� 2�� define
//-------------------------------------------------------------
#define MINIGAME2_MAX_XNUMBER    16		// Ĩ�� �ִ� ���� ����. 
#define MINIGAME2_MAX_YNUMBER    12		// Ĩ�� �ִ� ���� ����. 

#define MINIGAME2_CHIP_SHOW_DELAY   8
#define MINIGAME2_CHIP_ANI_DELAY    3
#define MINIGAME2_CHIP_FOREVER_DELAY 100


#define	MINIGAME2_BORDER_COLOR		191

#define		MAX_FILE_NUMBER_MINIGAME2  2

#define		FILE_MINIGAME2_BACK			0
#define		FILE_MINIGAME2_CARD			1

#define		MINIGAME2_SKIP				0
#define		MINIGAME2_WAIT				1
#define		MINIGAME2_NEXT				2

#define     MINIGAME2_TEXT_COMPARE_OK	0
#define		MINIGAME2_TEXT_COMPARE_NO	1


struct ChipData
{
	SI32	siIndex;
	SI08	siKind;
};

//-------------------------------------------------------------
// �̴ϰ��� 3�� define
//-------------------------------------------------------------

#define		MAX_FILE_NUMBER_MINIGAME3   5

#define		FILE_MINIGAME3_BACK			0
#define		FILE_MINIGAME3_FRUITS		1
#define		FILE_MINIGAME3_CHARACTER	2
#define		FILE_MINIGAME3_WORM			3
#define		FILE_MINIGAME3_SHOCK		4

// �������� ������ ����,. 
#define MINIGAME3_FRUIT_TYPE_GRAPE    0
#define MINIGAME3_FRUIT_TYPE_APPLE    1
#define MINIGAME3_FRUIT_TYPE_ORANGE   2
#define MINIGAME3_FRUIT_TYPE_PEAR     3
#define MINIGAME3_FRUIT_TYPE_PEACH    4


// �������� ������ �ӵ�. 
#define MINIGAME3_SPEED_1		1000
#define MINIGAME3_SPEED_2		900
#define MINIGAME3_SPEED_3		800
#define MINIGAME3_SPEED_4		700
#define MINIGAME3_SPEED_5		600
#define MINIGAME3_SPEED_6		500

// �� ȭ�鿡 ���� �� �ִ� �ִ� ���� ��. 
#define MINIGAME3_MAX_FRUIT_NUMBER  100

#define MINIGAME3_DEFAULT_JUMP_POWER 6

class	cltOnlineWorld;

void InitMiniGame(SI32 minigametype, cltOnlineWorld *pOnlineWorld);
BOOL MiniGame(SI32 minigametype);
void DrawMiniGame(SI32 minigametype, LPDIRECTDRAWSURFACE7 psurface);
void EndMiniGame(SI32 minigametype);
void ApplyHack();

// �̴ϰ��ӿ��� �̷���� �۾����� ���´�. 
SI32 GetMiniGameWork();
SI32 GetMiniGameLevel();
VOID SetMiniGameWork(SI32 work);
SI32 GetMiniGameType();

///////////�ӽ�///////////////
SI32 GetHackMiniGameWork();
VOID SetHackMiniGameWork(SI32 work);
SI32 GetHackMiniGamePoint();
VOID SetHackMiniGamePoint(SI32 point);
SI32 GetHackMiniGamePlayTime();
VOID SetHackMiniGamePlayTime(SI32 time);
SI32 GetHackMiniGameLevel();
VOID SetHackMiniGameLevel(SI32 level);


class _MiniGame{
protected:
	SI32 MiniGameType;
	cltOnlineWorld	*pMyOnlineWorld;

	// ��ȣȭ �ؾ� �ϴ� ������. 
	///////////////////////////////////////////////////////
	SI32 siCheckSum;		// ��ȣȭ ����. 

	SI32 *siPlayTime;		// ���� �ð� 
	SI32 *siMiniGameLevel;
	SI32 Para1;				// �������� ����ϱ� ���� ���� 1
	SI32 *siMiniGameWork;	// �۾���
	SI32 Para2;				// �������� ����ϱ� ���� ���� 2
	SI32 *siMiniGamePoint;	// ����. 
	SI32 *siStandardScore;

	SI32 *siHackMiniGameWork;
	SI32 *siHackMiniGamePoint;
	SI32 *siHackMiniGamePlayTime;
	SI32 *siHackMiniGameLevel;
	SI32 *siKey, *siHackKey;

	BOOL bSendHackMsg;
	SI16 m_siSelect;
	BOOL m_bSkip;
	BOOL m_bFinishedswitch;
	SI32 *m_siKey;	
	
	BOOL m_bHackMonitor;	

	///////////////////////////////////////////////////////

	SI32 BackgroundMusic;	// ������� �ڵ�. 

	XSPR *pMiniGameSpr[MAX_MINIGAME_SPR_NUMER];
	char  MiniGameFileName[MAX_MINIGAME_SPR_NUMER][128] ;

	void UpdateCheckSum()
	{
		siCheckSum = ~( GetMiniGamePoint() + GetMiniGameWork() + GetPlayTime() + GetStandardScore()+ GetMiniGameLevel() + GetPara1() + GetPara2() );
	}

	SI32 EncodePassword(SI32 number)
	{
		return (number ^ *siKey);
	}
	
	SI32 DecodePassword(SI32 number)
	{
		return (number ^ *siKey);
	}
	void MakeWork();
	BOOL CheckHacking();

	SI32 HackEncodePassword(SI32 number)
	{
		return (number ^ *siHackKey);
	}
	
	SI32 HackDecodePassword(SI32 number)
	{
		return (number ^ *siHackKey);
	}

public:
	_MiniGame(SI32 minigametype, SI32 standartscore, cltOnlineWorld *pOnlineWorld);

	virtual void Init(){};
	virtual BOOL Action(){return FALSE;};
	virtual void Draw(LPDIRECTDRAWSURFACE7 psurface){};
	virtual void Quit();

	void InputOperate();

	SI32 GetMiniGameType(){return MiniGameType;}

	SI32 GetMiniGameKey()	{ return *siKey; };
	SI32 GetMiniGameHackKey()	{ return *siHackKey; };

	void SetMiniGamePoint(SI32 point){*siMiniGamePoint = EncodePassword(point);UpdateCheckSum();};
	SI32 GetMiniGamePoint(){return DecodePassword(*siMiniGamePoint);}

	void SetMiniGameWork(SI32 work){*siMiniGameWork = EncodePassword(work);UpdateCheckSum();};
	SI32 GetMiniGameWork(){return DecodePassword(*siMiniGameWork);}

	void SetPlayTime(SI32 time){*siPlayTime = EncodePassword(time);UpdateCheckSum();};
	SI32 GetPlayTime(){return DecodePassword(*siPlayTime);}

	void SetStandardScore(SI32 score){*siStandardScore = EncodePassword(score);UpdateCheckSum();};
	SI32 GetStandardScore(){return DecodePassword(*siStandardScore);}

	void SetMiniGameLevel(SI32 level){*siMiniGameLevel = EncodePassword(level);UpdateCheckSum();};
	SI32 GetMiniGameLevel(){return DecodePassword(*siMiniGameLevel);}

	void SetPara1(SI32 para1){Para1 = EncodePassword(para1);UpdateCheckSum();}
	SI32 GetPara1(){return DecodePassword(Para1);}

	void SetPara2(SI32 para2){Para2 = EncodePassword(para2);UpdateCheckSum();}
	SI32 GetPara2(){return DecodePassword(Para2);}

	void SetMiniGameCheckNumber(SI32 point){*m_siKey = EncodePassword(point);};
	SI32 GetMiniGameCheckNumber(){return DecodePassword(*m_siKey);};


	//////////////�ӽ�///////////////////
	void SetHackMiniGameWork(SI32 work){*siHackMiniGameWork = HackEncodePassword(work);};
	SI32 GetHackMiniGameWork(){return HackDecodePassword(*siHackMiniGameWork);}

	void SetHackMiniGamePoint(SI32 point){*siHackMiniGamePoint = HackEncodePassword(point);};
	SI32 GetHackMiniGamePoint(){return HackDecodePassword(*siHackMiniGamePoint);}

	void SetHackMiniGamePlayTime(SI32 time){*siHackMiniGamePlayTime = HackEncodePassword(time);};
	SI32 GetHackMiniGamePlayTime(){return HackDecodePassword(*siHackMiniGamePlayTime);}

	void SetHackMiniGameLevel(SI32 level){*siHackMiniGameLevel = HackEncodePassword(level);};
	SI32 GetHackMiniGameLevel(){return HackDecodePassword(*siHackMiniGameLevel);}

	// ��ŷ�� ���ɼ��� Ž���Ѵ�. 
	// TRUE : �̻����. 
	// FALSE : �̻�����. 
	BOOL CheckCheckSum()
	{
		if((~siCheckSum) == ( GetMiniGamePoint() + GetMiniGameWork() + GetPlayTime() + GetStandardScore() + GetMiniGameLevel() + GetPara1() + GetPara2()) )
		{
			return TRUE;
		}
		
		return FALSE;
	}


	BOOL PlayMusic(char* filename);
	void StopMusic();
	
	virtual void ApplyHack()	{};
};

class _MiniGame1: public _MiniGame{
protected:
	SI32 Chip[MINIGAME1_MAX_XNUMBER][MINIGAME1_MAX_YNUMBER];
	SI32 ChipStatus[MINIGAME1_MAX_XNUMBER][MINIGAME1_MAX_YNUMBER];
	SI32 ChipBoard[MINIGAME1_MAX_XNUMBER][MINIGAME1_MAX_YNUMBER];
	
	SI32 ChipXsize ;
	SI32 ChipYsize ;

	SI32 ChipXInterval ;
	SI32 ChipYInterval ;

	SI32 XNumber;
	SI32 YNumber;
	
	SI32 ChipStartX, ChipStartY;
	SI32 ChipInMouse, PressedChip;

	SI32 ShowChipNumber;
	SI32 ReadyToHideNumber;

	SI32 ImageNumberInPool ;
	BOOL MiniGame1StartSwitch;
	
	// ������  ��ư
	_Button			BExit;
	XSPR			*pExitSpr;

	SI32 ImagePool[100]; 
	SI32 DifficultyLevel[100] ;

	// �´� ¦�� �ִ��� Ȯ���Ѵ�. 
	BOOL IsCheckPairs(POINT* pchip1, POINT* pchip2);
	void ArrangeRandomImage();
	void DecideVaryByLevel();
	void InitChip();
	BOOL IsMouseInChip(SI32 x, SI32 y);

public:
	_MiniGame1(SI32 minigametype, SI32 standartscore, cltOnlineWorld *pOnlineWorld);
	void Init();
	BOOL Action();
	void Draw(LPDIRECTDRAWSURFACE7 psurface);
	void Quit();



};

class _MiniGame2: public _MiniGame{
protected:
	ChipData Chip[MINIGAME2_MAX_XNUMBER][MINIGAME2_MAX_YNUMBER];
	BOOL TempPath[MINIGAME2_MAX_XNUMBER][MINIGAME2_MAX_YNUMBER];
	
	SI32 ChipXsize ;
	SI32 ChipYsize ;
	
	POINT SelectedChip1, SelectedChip2;
	
	SI32 XNumber;
	SI32 YNumber;
	SI32 ChipStartX, ChipStartY;
	SI32 ChipInMouse, PressedChip;

	SI32 ShowChipNumber;
	SI32 ReadyToHideNumber;
	
	POINT MiniGame2Path[MINIGAME2_MAX_XNUMBER*MINIGAME2_MAX_YNUMBER];
	SI32 MiniGame2PathIndex;
	SI32 MiniGame2DrawPathIndex;
	
	SI32 ImageNumberInPool ;
	UI16 m_siCount;
	OnlineMsgBox*      pOnlineMsgBox;
//	OnlineEditControl* m_pMinigameEditBox;
	// actdoll (2004/05/05 16:00) : [OECEX]
	HOEC				m_hOecMiniGame;

	SI32 ImagePool[100] ;
	SI32 DifficultyLevel[100];
	
	BOOL					MiniGame2StartSwitch;
	CHAR					*m_szMiniGameEditBoxString;
	HWND					m_YetEditHWND;
	BOOL					m_YetEditHWNDEnable;
	SI16					m_YetEditLockShortKey;
	SI16					m_siX,m_siY;

	UI16					m_siFont;
	UI16					m_siSize;
	UI16					m_siDir;

	SI32					m_siFailTimes;					// Ĩ���ῡ ������ Ƚ��
	SI32					m_siPrevConnectTime;			// Ĩ�� ���� ���������� �ð�
	SI32					m_siTotConnectDiff;				// �ѽ��� Ĩ�� �����Ű�µ� �ɸ� �ð����� ��
	SI32					m_siAverage;					// �ѽ��� Ĩ�� �����Ű�µ� �ɸ� �ð��� ���
	SI32					m_siDelayConnectTimes;			// Ĩ�� �����Ű�µ� �ɸ� �ð��� ��պ��� �ξ� ���� �ɸ� Ƚ��
	SI32					m_siTextNum;

	// ������  ��ư
	_Button			BExit;
	XSPR			*pExitSpr;
	XSPR			m_ImgCheck;

	SI32			m_siPreMouseX, m_siPreMouseY;
	SI16			m_siCenterHackCount, m_siRepeatHackCount;

	BOOL CheckConnect(POINT pt1, POINT pt2, SI32 dir, SI32 count);
	void InitCheckNumber();
	SI32 GetRandKey();
	void ArrangeRandomImage_MiniGame2();;
	void DecideVaryByLevel_MiniGame2();
	void InitChip_MiniGame2();
	BOOL IsMouseInChip_MiniGame2(SI32 x, SI32 y);
	void CompareNumber(UI32 uiTextID,UI32 uiEditTextID);

	void ApplyHack();

public:
	_MiniGame2(SI32 minigametype, SI32 standartscore, cltOnlineWorld *pOnlineWorld);
	~_MiniGame2();
	void Init();
	BOOL Action();
	void Draw(LPDIRECTDRAWSURFACE7 psurface);
	void Quit();
	void Free();
};


class _MiniGame3: public _MiniGame{
protected:

	// �����̺��� �������� �� ������ ����. 
    SI32 StageInfo[100];

	// ȭ�鿡 ��Ÿ�� ������ ��ġ ����. 
	POINT FruitInfo[MINIGAME3_MAX_FRUIT_NUMBER];
	// ȭ�鿡 ��Ÿ�� ������ �ð� ����. 
	DWORD  FruitTime[MINIGAME3_MAX_FRUIT_NUMBER];
	
	// ������ ���������� ������ �ð�. 
	DWORD  LastDropTime;

	//  ����� �������� ����. 
	RECT PlayArea;

	// �ٱ��� ����. 
	SIZE ToolSize;

	// �ٱ��� ��ġ 
	POINT ToolPosition;
	POINT OldToolPosition;

	// ������ ���� ���� ��. 
	SI32 RemainFruitNumber;

	// �̹� �������� ���� ������ ��. 
	SI32 GotFruitNumber;


	// �ְ� ����. 
	SI32 MaxLevel;
	
	// �ٱ����� ����. 
	SI32 Dir;

	// ������ ����. 
	SI32 WormDir;
	// ������ ��ġ 
	POINT WormPosition;
	POINT StartWormPosition, EndWormPosition;  // ������ ��Ÿ���� ��ġ, ������� ��ġ. 


	BOOL MiniGame1StartSwitch;
	
	// ���� ����. 
	BOOL JumpSwitch;
	SI32 JumpDirection;
	SI32 JumpPower;

	// ���� �޾��� �� �׷��� ȿ���� �󸶳� ���� ǥ�� �ϴ°� ?
	SI32 ShockDelay;

	// ĳ���Ͱ� �����ϰ��־�� �ϴ� �ð�. 
	SI32 HaltDelay;


	// ������  ��ư
	_Button			BExit;
	XSPR			*pExitSpr;

	SI32 GetStageFruit(SI32 stagelevel)
	{
		return StageInfo[stagelevel * 4];
	}
	SI32 GetStageMaxDropNumber(SI32 stagelevel)
	{
		return StageInfo[stagelevel * 4 + 1];
	}
	SI32 GetStageNeedDropNumber(SI32 stagelevel)
	{
		return StageInfo[stagelevel * 4 + 2];
	}
	SI32 GetStageSpeed(SI32 stagelevel)
	{
		return StageInfo[stagelevel * 4 + 3];
	}

	// ȭ�鿡 ��Ÿ���� ������ �ʱ�ȭ�Ѵ�. 
	void InitFruitInfo();

	void DeleteFruit(SI32 fruitid)
	{
		FruitInfo[fruitid].x = -1;
		FruitInfo[fruitid].y = -1;
	}

	// ������ �ٱ��Ͽ� ����ִ°�?
	BOOL IsFruitInTool(SI32 fruitid);


public:
	_MiniGame3(SI32 minigametype, SI32 standartscore, cltOnlineWorld *pOnlineWorld);
	void Init();
	BOOL Action();
	void Draw(LPDIRECTDRAWSURFACE7 psurface);
	void Quit();
	BOOL IsCollWithWorm();






};

#endif