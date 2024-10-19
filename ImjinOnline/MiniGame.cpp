
#include <GSL.h>

#include <clGame.h>
#include <MiniGame1.h>
#include <SoundManager.h>


_MiniGame* pMiniGame;

extern	_MainData				GD;									// ������ �ֵ����� 
extern	SoundManager	   *g_pSoundManager;
extern	_clGame				*pGame;
 
//-------------------------------------------------------------------------------
void InitMiniGame(SI32 minigametype, cltOnlineWorld *pOnlineWorld)
{
	switch(minigametype)
	{
	case MINIGAME_TYPE_1: 
		pMiniGame = new _MiniGame1(minigametype, MINIGAME1_STANDARD_SCORE, pOnlineWorld); 
		pMiniGame->Init();
		break;

	case MINIGAME_TYPE_2: 
		pMiniGame = new _MiniGame2(minigametype, MINIGAME2_STANDARD_SCORE, pOnlineWorld);
		pMiniGame->Init();
		break;
	case MINIGAME_TYPE_3: 
		pMiniGame = new _MiniGame3(minigametype, MINIGAME3_STANDARD_SCORE, pOnlineWorld); 
		pMiniGame->Init();
		break;

	default:
		clGrp.Error("fds93j", "MiniGame:%d", minigametype);
	}
}

void EndMiniGame(SI32 minigametype)
{
	if(pMiniGame == NULL) return ;

	// ������ �����Ѵ�.
	pMiniGame->StopMusic();

	// �޸𸮸� �����Ѵ�.
	pMiniGame->Quit();

	delete pMiniGame;
	pMiniGame = NULL;

	::SetFocus(pGame->Hwnd);
}

void ApplyHack()
{
	if(pMiniGame == NULL) return ;

	pMiniGame->ApplyHack();
}

BOOL MiniGame(SI32 minigametype)
{
	if(pMiniGame == NULL) return FALSE;
	return pMiniGame->Action();
}


void DrawMiniGame(SI32 minigametype, LPDIRECTDRAWSURFACE7 psurface)
{
	if(pMiniGame == NULL) return ;
	
	pMiniGame->Draw(psurface);
}

// �̴ϰ��ӿ��� �̷���� �۾����� ���´�. 
SI32 GetMiniGameWork()
{
	// ��ŷ�� ����� �ִٸ�, 
	if(pMiniGame->CheckCheckSum() == FALSE)return 0;

	return pMiniGame->GetMiniGameWork();
}

SI32 GetMiniGameLevel()
{
	// ��ŷ�� ����� �ִٸ�, 
	if(pMiniGame->CheckCheckSum() == FALSE)return 0;

	return pMiniGame->GetMiniGameLevel();
}

SI32 GetMiniGameType()
{
	// ��ŷ�� ����� �ִٸ�, 
	if(pMiniGame->CheckCheckSum() == FALSE)return 0;

	return pMiniGame->GetMiniGameType();
}

VOID SetMiniGameWork(SI32 work)
{
	pMiniGame->SetMiniGameWork(work);
}

///////////////�ӽ�//////////////////
SI32 GetHackMiniGameWork()
{
	return pMiniGame->GetHackMiniGameWork();
}

VOID SetHackMiniGameWork(SI32 work)
{
	pMiniGame->SetHackMiniGameWork(work);
}

SI32 GetHackMiniGamePoint()
{
	return pMiniGame->GetHackMiniGamePoint();
}

VOID SetHackMiniGamePoint(SI32 point)
{
	pMiniGame->SetHackMiniGamePoint(point);
}

SI32 GetHackMiniGamePlayTime()
{
	return pMiniGame->GetHackMiniGamePlayTime();
}

VOID SetHackMiniGamePlayTime(SI32 time)
{
	pMiniGame->SetHackMiniGamePlayTime(time);
}

SI32 GetHackMiniGameLevel()
{
	return pMiniGame->GetHackMiniGameLevel();
}

VOID SetHackMiniGameLevel(SI32 level)
{
	pMiniGame->SetHackMiniGameLevel(level);
}


//--------------------------------------------------------------
// ���� extern
//--------------------------------------------------------------
extern _InputDevice			IpD;
extern clock_t StartClock, CurrentClock, PrevClock;
extern  HFONT				SaveInfoFont;
extern  HFONT				HelpFont;

extern	SHORT 				GameMouseX, GameMouseY;
extern	DWORD 				GameWParam;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;



//----------------------------------------------------------------------------------------------------
_MiniGame::_MiniGame(SI32 minigametype, SI32 standarscore, cltOnlineWorld *pOnlineWorld)
{
	MiniGameType	=   minigametype;
	pMyOnlineWorld	=	pOnlineWorld;

	BackgroundMusic		=  0;

	siPlayTime		= new SI32;
	siMiniGameLevel	= new SI32;
	siMiniGameWork	= new SI32;
	siMiniGamePoint	= new SI32;
	siStandardScore	= new SI32;

	siHackMiniGameWork		= new SI32;
	siHackMiniGamePoint		= new SI32;
	siHackMiniGamePlayTime	= new SI32;
	siHackMiniGameLevel		= new SI32;

	siKey		= new SI32;
	siHackKey	= new SI32;
	m_siKey		= new SI32;

	// ��ȣȭ Ű���� �����Ѵ�.
	srand(timeGetTime());
	*siKey = rand() % MINIGAME_MAX_KEY;
	*siHackKey = rand() % MINIGAME_MAX_KEY;

	SetMiniGameLevel(0);
	SetMiniGamePoint(0);
	SetMiniGameWork(0);
	SetPlayTime(0);

	SetHackMiniGameLevel(0);
	SetHackMiniGamePoint(0);
	SetHackMiniGameWork(0);
	SetHackMiniGamePlayTime(0);

	SetStandardScore(standarscore);
}


void _MiniGame::MakeWork()
{
	bSendHackMsg = FALSE;

	// ��ŷ�� ����� �ִٸ�, 
	if(CheckCheckSum() == FALSE)return ;


	// ������ Ȯ���Ѵ�. (�۾��� ������ �����Ѵ�.)
	if(GetMiniGamePoint() > GetStandardScore())
	{
		SetMiniGameWork(GetMiniGameWork() + (GetMiniGamePoint() / GetStandardScore()) );
		SetMiniGamePoint(GetMiniGamePoint() % GetStandardScore());
		
		SetHackMiniGameWork(GetHackMiniGameWork() + (GetHackMiniGamePoint() / GetStandardScore()) );
		SetHackMiniGamePoint(GetHackMiniGamePoint() % GetStandardScore());
	}
}

BOOL _MiniGame::CheckHacking()
{
	if(GetMiniGameKey() == GetMiniGameHackKey())
	{
		if(!bSendHackMsg)
			pMyOnlineWorld->pOnlineClient->SendFarmWorkHack(GetMiniGameType()*10 + 7, GetMiniGameLevel(), GetHackMiniGameWork(), GetMiniGameKey());
		SetPlayTime(0);
		SetMiniGamePoint(0);
		SetMiniGameWork(0);
		bSendHackMsg = TRUE;
		return FALSE;
	}
	
	if(GetHackMiniGamePlayTime() != GetPlayTime())			// �۾��ð�(time) ��ŷ
	{
		if(!bSendHackMsg)
			pMyOnlineWorld->pOnlineClient->SendFarmWorkHack(GetMiniGameType()*10, GetMiniGameLevel(), GetPlayTime(), GetHackMiniGamePlayTime());
		SetPlayTime(0);
		SetMiniGamePoint(0);
		SetMiniGameWork(0);
		bSendHackMsg = TRUE;
		return FALSE;
	}
	if(GetHackMiniGamePoint() != GetMiniGamePoint())		// �۾�����Ʈ(score) ��ŷ
	{
		if(!bSendHackMsg)
			pMyOnlineWorld->pOnlineClient->SendFarmWorkHack(GetMiniGameType()*10 + 1, GetMiniGameLevel(), GetMiniGamePoint(), GetHackMiniGamePoint());
		SetPlayTime(0);
		SetMiniGamePoint(0);
		SetMiniGameWork(0);
		bSendHackMsg = TRUE;
		return FALSE;
	}
	if(GetHackMiniGameWork() != GetMiniGameWork())			// �۾���(work) ��ŷ
	{
		if(!bSendHackMsg)
			pMyOnlineWorld->pOnlineClient->SendFarmWorkHack(GetMiniGameType()*10 + 2, GetMiniGameLevel(), GetMiniGameWork(), GetHackMiniGameWork());
		SetPlayTime(0);
		SetMiniGamePoint(0);
		SetMiniGameWork(0);
		bSendHackMsg = TRUE;
		return FALSE;
	}
	if(GetHackMiniGameLevel() != GetMiniGameLevel())		// ����(level) ��ŷ
	{
		if(!bSendHackMsg)
			pMyOnlineWorld->pOnlineClient->SendFarmWorkHack(GetMiniGameType()*10 + 3, GetMiniGameLevel(), GetMiniGameLevel(), GetHackMiniGameLevel());
		SetPlayTime(0);
		SetMiniGamePoint(0);
		SetMiniGameWork(0);
		bSendHackMsg = TRUE;
		return FALSE;
	}

	if(CheckCheckSum() == FALSE)								// üũ������ �ɸ�	
	{
		if(!bSendHackMsg)
			pMyOnlineWorld->pOnlineClient->SendFarmWorkHack(GetMiniGameType()*10 + 6, GetMiniGameLevel(), GetMiniGameWork(), GetHackMiniGameWork());
		SetPlayTime(0);
		SetMiniGamePoint(0);
		SetMiniGameWork(0);
		bSendHackMsg = TRUE;
		return FALSE;
	}

	return TRUE;
}

void _MiniGame::InputOperate()
{
	DWORD wparam;
	SHORT leftpressswitch=FALSE, rightpressswitch=FALSE;
	SHORT mousex, mousey;
	char moveswitch;

	if(GD.KB.PopKeyBuffer(leftpressswitch, rightpressswitch, mousex, mousey, wparam, moveswitch) == TRUE)
	{

		GameMouseX = mousex;
		GameMouseY = mousey;
    	GameWParam = wparam;
		if(leftpressswitch >=0)
			GameLeftPressSwitch=leftpressswitch;

		if(rightpressswitch >= 0)
			GameRightPressSwitch=rightpressswitch;
	}

}

BOOL _MiniGame::PlayMusic(char* filename)
{

	BackgroundMusic	= g_pSoundManager->AddPlayID( filename, FALSE, TRUE, FALSE );

	g_pSoundManager->PlaySound( BackgroundMusic );

	return TRUE;
}

void _MiniGame::StopMusic()
{
	if(g_pSoundManager)
	{
		g_pSoundManager->StopSound(BackgroundMusic );	
		g_pSoundManager->ReleaseSound(BackgroundMusic);
	}
}

void _MiniGame::Quit()
{
	if(siPlayTime)		delete siPlayTime;
	if(siMiniGameLevel)	delete siMiniGameLevel;
	if(siMiniGameWork)	delete siMiniGameWork;
	if(siMiniGamePoint)	delete siMiniGamePoint;
	if(siStandardScore)	delete siStandardScore;

	if(siHackMiniGameWork)		delete siHackMiniGameWork;
	if(siHackMiniGamePoint)		delete siHackMiniGamePoint;
	if(siHackMiniGamePlayTime)	delete siHackMiniGamePlayTime;
	if(siHackMiniGameLevel)		delete siHackMiniGameLevel;

	if(siKey)		delete siKey;
	if(siHackKey)	delete siHackKey;
	if(m_siKey)		delete m_siKey;
}
