#include <GSL.h>
#include <MiniGame1.h>
#include <clGame.h>
#include <colortable.h>
#include "..\Gersang\music.h"
#include "..\Gersang\text.h"
#include "..\Gersang\clNumberRender.h"
#include "OnlineMsgBox.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif
//--------------------------------------------------------------
// ���� extern
//--------------------------------------------------------------
extern _clGame		*pGame;
extern	_MainData				GD;									// ������ �ֵ����� 
extern _InputDevice			IpD;
extern clock_t StartClock, CurrentClock, PrevClock;

extern	SHORT 				GameMouseX, GameMouseY;
extern	DWORD 				GameWParam;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern _Text					Text;										// �ؽ�Ʈ Ŭ���� ���� 
extern	unsigned char			TeamLightTable[];
extern  int                 WindowModeSwitch;
extern BOOL                 bActive;

_MiniGame3::_MiniGame3(SI32 minigametype, SI32 standardscore, cltOnlineWorld *pOnlineWorld):_MiniGame(minigametype, standardscore, pOnlineWorld)
{
	// Para1 : �޺�����. 

}


void _MiniGame3::Init()
{
	SI32 index;
	SI32 i;

	pGame->pNumberRender->ClearAllNumber();

	// ���������� ����. 
	MaxLevel = 10;

	ShockDelay = 0;

	HaltDelay  = 0;

	// �޺� ���ڸ� �ʱ�ȭ�Ѵ�. 
	SetPara1(0);

	                // ��������,�������� ����,�޾ƾ� �ϴ� ����, �������� �ӵ�,
    SI32 data1[] = {MINIGAME3_FRUIT_TYPE_APPLE,  50, 40, MINIGAME3_SPEED_1,
					MINIGAME3_FRUIT_TYPE_APPLE,  50, 40, MINIGAME3_SPEED_1,
					MINIGAME3_FRUIT_TYPE_GRAPE,  60, 48, MINIGAME3_SPEED_2,
					MINIGAME3_FRUIT_TYPE_GRAPE,  60, 48, MINIGAME3_SPEED_2,
					MINIGAME3_FRUIT_TYPE_ORANGE, 70, 56, MINIGAME3_SPEED_3,
					MINIGAME3_FRUIT_TYPE_ORANGE, 70, 56, MINIGAME3_SPEED_3,
					MINIGAME3_FRUIT_TYPE_PEAR,   80, 64, MINIGAME3_SPEED_4,
					MINIGAME3_FRUIT_TYPE_PEAR,   80, 64, MINIGAME3_SPEED_4,
					MINIGAME3_FRUIT_TYPE_PEACH,  90, 72, MINIGAME3_SPEED_5,
					MINIGAME3_FRUIT_TYPE_PEACH,  90, 72, MINIGAME3_SPEED_5,
					-1};
	
	char  *data3[] = 
	{
		"online\\minigame\\minigame3\\minigame3_back.spr",
		"online\\minigame\\minigame3\\fruits.spr",
		"online\\minigame\\minigame3\\character.spr",
		"online\\minigame\\minigame3\\worm.spr",
		"online\\minigame\\minigame3\\shock.spr",
		""
	};

	index=0;
	while(1)
	{
		StageInfo[index] = data1[index];
		if(data1[index] == -1)break;
		index++;
	}


	index=0;
	while(strcmp(data3[index], "")  != 0)
	{
		strcpy(MiniGameFileName[index], data3[index]);
		index++;
	}


	// Ű���۸� �ʱ�ȭ�Ͽ� ������ �Է��� �����Ѵ�. 
	GD.KB.InitKeyBuffer();


	// �ʿ��� ȭ���� �ҷ��´�. 
	for(i = 0;i < MAX_FILE_NUMBER_MINIGAME3;i++)
	{
		pMiniGameSpr[i] = new XSPR;
		if(pMiniGameSpr[i]  == NULL)
		{
			clGrp.Error("fd93722jf", "Fd398575:%s", MiniGameFileName[i]);
			break;
		}
		
		if(clGrp.LoadXspr(MiniGameFileName[i], *pMiniGameSpr[i])==FALSE)
		{
			clGrp.Error("12fd93722jf", "Fd398575:%s", MiniGameFileName[i]);
			break;
		}
	}

	// ������ �������� ������ �����Ѵ�. 
	PlayArea.left	= (clGrp.GetScreenXsize() - pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Xsize)/ 2 ;
	PlayArea.right	= PlayArea.left + pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Xsize				;
	PlayArea.top	= (clGrp.GetScreenYsize() - pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Ysize)/ 2 + 40;
	PlayArea.bottom	= PlayArea.top + pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Ysize - 100;

	// ������ ���� �ٱ��� ������ �����Ѵ�. 
	ToolSize.cx = 50;
	ToolSize.cy = 60;

	// �ٱ����� �ʱ� ��ġ�� �����Ѵ�. 
	ToolPosition.x = PlayArea.left + (PlayArea.right - PlayArea.left) / 2;
	ToolPosition.y = PlayArea.bottom - (ToolSize.cy/2) + 5;

	MiniGame1StartSwitch =  FALSE;

	// ������ ��ư �׸�
	pExitSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_BOOKEXIT );		
	BExit.Create( 547, 510, pExitSpr->Header.Xsize, pExitSpr->Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );

	Dir = 0;

	JumpSwitch  = FALSE;
	JumpPower = MINIGAME3_DEFAULT_JUMP_POWER;

	WormDir		= WEST;
	WormPosition.x = -1;
	WormPosition.y = -1;
	bSendHackMsg = FALSE;

	// ������ ��Ÿ���� �����ϴ� ��ġ. 
	StartWormPosition.x = PlayArea.left;
	StartWormPosition.y = ToolPosition.y+ ToolSize.cy/2 - 2;

	// ������ ������� ��ġ. 
	EndWormPosition.x = PlayArea.right;
	EndWormPosition.y = ToolPosition.y + ToolSize.cy/2 - 2;

	InitFruitInfo();

#ifdef _OGG_PLAY
	GetBgmMgr()->Play(OGGBGM_FILENAME_MINIGAME, TRUE);
#else
	// ������ �����Ѵ�. 
	PlayMusic("music\\MiniGame1.YAV");
#endif
}


// ȭ�鿡 ��Ÿ���� ������ �ʱ�ȭ�Ѵ�. 
void _MiniGame3::InitFruitInfo()
{
	SI32 i;

	// �������� �� ������ ������ ��´�. 
	RemainFruitNumber	= GetStageMaxDropNumber( GetMiniGameLevel() );

	GotFruitNumber		= 0;

	for(i = 0;i < MINIGAME3_MAX_FRUIT_NUMBER;i++)
	{
		FruitInfo[i].x = -1;
		FruitInfo[i].y = -1;
	}
}

	// ������ �ٱ��Ͽ� ����ִ°�?
BOOL _MiniGame3::IsFruitInTool(SI32 fruitid)
{
	if( (FruitInfo[fruitid].x >= (ToolPosition.x - (ToolSize.cx/2))) 
	&& (FruitInfo[fruitid].x <= (ToolPosition.x + (ToolSize.cx/2)))
	&& (FruitInfo[fruitid].y >= (ToolPosition.y - (ToolSize.cy/2))) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _MiniGame3::Action()
{
	SI32 i;

	if(!CheckHacking())	return FALSE;

	InputOperate();

	if(ShockDelay)
	{
		ShockDelay--;
	}

	if(HaltDelay > -20)
	{
		HaltDelay--;
	}
	if(HaltDelay<0)
	{
		HaltDelay++;
	}

	pGame->pNumberRender->Prepare();

	//������ ������. 
	if(RemainFruitNumber )
	{
		if(abs(CurrentClock - LastDropTime)  > GetStageSpeed( GetMiniGameLevel() ))
		{
			LastDropTime = CurrentClock;
			
			// ������� ã�´�. 
			for(i = 0;i < MINIGAME3_MAX_FRUIT_NUMBER;i++)
			{
				if(FruitInfo[i].x < 0)
				{
					RemainFruitNumber--;

					FruitInfo[i].x	=	PlayArea.left + 10 + GD.Random() % (PlayArea.right-PlayArea.left-20);
					FruitInfo[i].y	=	PlayArea.top;
					FruitTime[i]	=	CurrentClock;
					break;
				}
			}
		}
	}

	// ������ �Ʒ��� ������. 
	for(i = 0;i < MINIGAME3_MAX_FRUIT_NUMBER;i++)
	{
		if(FruitInfo[i].x >= 0)
		{
			// ���ϼӵ��� ���Ѵ�. 
			DWORD elapsedtime = CurrentClock - FruitTime[i];
			SI32 length = 6 * elapsedtime*elapsedtime / 1000000;

			FruitInfo[i].y = PlayArea.top + length;

			if(FruitInfo[i].y > PlayArea.bottom)
			{
				DeleteFruit(i);

				// ���� ������ ȿ����  ȿ����
				PushEffect(EFFECT_MINIGAME3_FRUITFAIL, 0, 0);

				// �޺� ���ڸ� �ʱ�ȭ�Ѵ�. 
				SetPara1(0);
			}

			// �ٱ��Ͽ� ���� �Ǹ� ���ش�.
			if(IsFruitInTool(i) == TRUE)
			{
				GotFruitNumber++;
				SetMiniGamePoint(GetMiniGamePoint() + GetMiniGameLevel() + 1 + GetPara1()/3);
				SetHackMiniGamePoint(GetHackMiniGamePoint() + GetMiniGameLevel() + 1 + GetPara1()/3);
				DeleteFruit(i);

				// ���� ���� ȿ����
				PushEffect(EFFECT_MINIGAME3_FRUITOK, 0, 0);
				ShockDelay = 5;

				SetPara1(GetPara1() + 1);

				pGame->pNumberRender->AddNumber(NUMBERTYPE_UP, ToolPosition.x, ToolPosition.y, 1.0, GetPara1(), NULL);			// Add Number
			}
		}
	}

	// ������ ��� ��������, �������� �ִ� �͵� ������ ������ ����.
	if(RemainFruitNumber == 0)
	{
		BOOL remainswitch = FALSE;

		for(i = 0;i < MINIGAME3_MAX_FRUIT_NUMBER;i++)
		{
			if(FruitInfo[i].x >= 0)
			{
				remainswitch = TRUE;
			}
		}

		if(remainswitch == FALSE)
		{
			// ���������� �Ѿ�� �й�� �������� �����Ѵ�. 

			// �ʿ䷮ �̻��� ������, 
			if(GotFruitNumber >= GetStageNeedDropNumber(GetMiniGameLevel() ))
			{
				// ������ �۾����� ��ȯ�Ѵ�. 
				MakeWork();
				
				SetMiniGameLevel(GetMiniGameLevel() + 1);
				SetHackMiniGameLevel(GetHackMiniGameLevel() + 1);

				if(GetMiniGameLevel() >= MaxLevel)
				{
					// �����ܰ���� ���� �̴ϰ��� ������.
					return FALSE;
				}
				
				SetPara1(0);
				InitFruitInfo();
			}
			else
			{
				return FALSE;
			}
		}
	}

	// ������ ������ ������ �������� �����.
	if(WormPosition.x < 0 )
	{
		if(rand() %2 )
		{
		   WormDir = EAST;
		   WormPosition = EndWormPosition;
		}
		else
		{
		   WormDir = WEST;
		   WormPosition = StartWormPosition;
		}
	}

	if(WormPosition.x >= 0 )
	{
		if(WormDir == WEST)
		{
			WormPosition.x ++;
			
			if(WormPosition.x == EndWormPosition.x)
			{
				WormPosition.x = -1;
			}
		}
		else
		{
			WormPosition.x --;
			
			if(WormPosition.x == StartWormPosition.x)
			{
				WormPosition.x = -1;
			}
		}
	}

	// ������ ������ ���߰� �Ѵ�.
	if(IsCollWithWorm() == TRUE)
	{

		if(HaltDelay == 0)
		{
			// �����Ѵ�. 
			SetMiniGamePoint(max(0, GetMiniGamePoint() - 2 - 4 - 4));
			SetHackMiniGamePoint(max(0, GetHackMiniGamePoint() - 2 - 4 - 4));

			HaltDelay = 40;
		}
	}

	if( BExit.Process( pMyOnlineWorld->fLeftUpSwitch ) == TRUE )
	{
		return FALSE;
	}

	// ���콺�� ����Ѵ�. 
	POINT point;
	GetCursorPos(&point);
	if (WindowModeSwitch)
	{
		ScreenToClient(pGame->Hwnd,&point);

		if (bActive) 
		{
		}
	}
	IpD.Mouse_X=(SHORT)point.x;
	IpD.Mouse_Y=(SHORT)point.y;
	DrawMousePrimary();

	// �ٱ��ϸ� �����δ�. 
	if(HaltDelay <= 0)
	{
		OldToolPosition = ToolPosition;
		ToolPosition.x = min( max(PlayArea.left, IpD.Mouse_X), PlayArea.right);
		
/*		if(ToolPosition.x >= OldToolPosition.x)
		{
			//ReverseSwitch = TRUE;
		}
		else 
		{
			//Dir = 0;
		}
*/

		Dir = (CurrentClock / 400) % 2;

		// ĳ���� ������ ���� �ٸ� �̹����� ����Ѵ�. 
		SI32 onlinekind = pGame->pOnlineWorld->pMyData->GetMyCharKind();
		SI32 battlekind = TransKindOnlineToImjin(onlinekind);

		switch(battlekind)
		{
		case KIND_ON_KEON:
			break;
		case KIND_ON_MYUNGHWA:
			Dir += 2;
			break;
		case KIND_ON_RYO:
			Dir += 4;
			break;
		case KIND_ON_HANAHINE:
			Dir += 6;
			break;
		default:
			break;
		}
	}

	// ���콺 ���� ��ư�� ������ �����Ѵ�. 
	if(JumpSwitch== FALSE && GameLeftPressSwitch == TRUE)
	{
		// ������ ���¿����� ������ �Ұ����ϴ�.
		if(HaltDelay <= 0)
		{
			JumpSwitch = TRUE;
			JumpDirection = NORTH;
		}
	}

	if(JumpSwitch == TRUE)
	{
		// ���� �ö󰡴� ���̸�, 
		if(JumpDirection == NORTH)
		{
			
			ToolPosition.y -= (JumpPower*3/2);
			
			JumpPower--;

			if(JumpPower == 0)
			{
				JumpDirection = SOUTH;
			}
		}
		else
		{
			JumpPower++;
			ToolPosition.y += (JumpPower*3/2);
			
			if(JumpPower == MINIGAME3_DEFAULT_JUMP_POWER)
			{
				JumpSwitch = FALSE;
			}
		}
	}

	return TRUE;
}

void _MiniGame3::Draw(LPDIRECTDRAWSURFACE7 psurface)
{

	SI32 i;
	SI32 file;
	HDC hdc;
	char buffer[128];
	SI32 startx, starty;


	// �ǳ��� �׸���. 
	if(clGrp.LockSurface(psurface) == TRUE)
	{

		SI32 startx = (clGrp.GetScreenXsize() - pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Xsize)/ 2;
		SI32 starty = (clGrp.GetScreenYsize() - pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Ysize)/ 2;

		clGrp.PutSpriteT(startx, starty, 
						pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Xsize,
						pMiniGameSpr[FILE_MINIGAME3_BACK]->Header.Ysize,
						pMiniGameSpr[FILE_MINIGAME3_BACK]->Image);

		
		clGrp.UnlockSurface(psurface);
	}

	

	// ���� ������ ǥ������.
	wsprintf(buffer, "%d", RemainFruitNumber);

	SI32 xsize, ysize;

	xsize = 12 ;
	ysize = 8  ;

	RECT destrect = {430-xsize, 120 - ysize, 430+xsize, 120 + ysize};
	DrawTextInRect(psurface, pGame->pBattle->SaveInfoFont, buffer, destrect);


	// �޾ƾ� �� ���� ���� ǥ���Ѵ�. 
	wsprintf(buffer, "%d/%d", GotFruitNumber, GetStageNeedDropNumber( GetMiniGameLevel() ));

	xsize = 24 ;
	ysize = 8  ;

	RECT destrect2 = {551 - xsize, 120 - ysize, 551 + xsize, 120 + ysize};
	DrawTextInRect(psurface, pGame->pBattle->SaveInfoFont, buffer, destrect2);

	// ������ ǥ���Ѵ�. 
	if(psurface->GetDC(&hdc)==DD_OK)
	{
		SelectObject(hdc, pGame->pBattle->HelpFont);
		
		SetTextColor( hdc, NORMAL_TEXT_COLOR );
		SetBkMode(hdc, TRANSPARENT);

		wsprintf(buffer, Text.RemainFruit.Get());
		DrawTextInCenter(hdc, 368, 120, buffer);

		wsprintf(buffer, Text.GotFruit.Get());
		DrawTextInCenter(hdc, 490, 120, buffer);

		wsprintf(buffer, Text.Scores.Get());
		DrawTextInCenter(hdc, 229, 499, buffer);
		wsprintf(buffer, "%d",GetMiniGamePoint());
		DrawTextInCenter(hdc, 291, 499, buffer);

		wsprintf(buffer, Text.Difficulty.Get());
		DrawTextInCenter(hdc, 365, 499, buffer);
		wsprintf(buffer, "%d",GetMiniGameLevel());
		DrawTextInCenter(hdc, 417, 499, buffer);
		
		wsprintf(buffer, Text.Work.Get());
		DrawTextInCenter(hdc, 486, 499, buffer);
		wsprintf(buffer, "%d",GetMiniGameWork());
		DrawTextInCenter(hdc, 554, 499, buffer);


//		wsprintf(buffer, "%d %d",IpD.Mouse_X, IpD.Mouse_Y);
//		TextOut(hdc, 0, 550, buffer, lstrlen(buffer));

		psurface->ReleaseDC(hdc);
	}

	if(clGrp.LockSurface(psurface) == TRUE)
	{
		//clGrp.Box(PlayArea.left, PlayArea.top, PlayArea.right, PlayArea.bottom, 255);
		
		// �������� ������ �׸���. 
		for(i = 0;i < MINIGAME3_MAX_FRUIT_NUMBER;i++)
		{
			if(FruitInfo[i].x >= 0)
			{
				file = FILE_MINIGAME3_FRUITS;

				clGrp.PutSpriteT(FruitInfo[i].x, FruitInfo[i].y, 
					pMiniGameSpr[file]->Header.Xsize,
					pMiniGameSpr[file]->Header.Ysize,
					&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[ GetStageFruit( GetMiniGameLevel() ) ]]);
			}
		}

		// ������ �׸���. 
		if(WormPosition.x >= 0)
		{
			file = FILE_MINIGAME3_WORM;
			startx = WormPosition.x - (pMiniGameSpr[file]->Header.Xsize/2); 
			starty = WormPosition.y - (pMiniGameSpr[file]->Header.Ysize/2);
			
			if(WormDir == EAST)
			{
				clGrp.PutSpriteT(startx, starty, 
					pMiniGameSpr[file]->Header.Xsize,
					pMiniGameSpr[file]->Header.Ysize,
					&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[ (CurrentClock/200)%2 ]]);
			}
			else
			{
				clGrp.PutSpriteRT(startx, starty, 
					pMiniGameSpr[file]->Header.Xsize,
					pMiniGameSpr[file]->Header.Ysize,
					&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[ (CurrentClock/200)%2 ]]);
			}
		}


		// ����� �׸���. 
		if(ShockDelay )
		{
			file = FILE_MINIGAME3_SHOCK;
			
			startx = ToolPosition.x - (pMiniGameSpr[file]->Header.Xsize/2); 
			starty = ToolPosition.y - (pMiniGameSpr[file]->Header.Ysize/2);
			
			clGrp.PutSpriteT(startx, starty, 
				pMiniGameSpr[file]->Header.Xsize,
				pMiniGameSpr[file]->Header.Ysize,
				&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[0]]);
		}


		// �ٱ��ϸ� �׸���.
		startx = ToolPosition.x - (ToolSize.cx/2); 
		starty = ToolPosition.y - (ToolSize.cy/2);

		file = FILE_MINIGAME3_CHARACTER;

		if(HaltDelay>0)
		{
			if(HaltDelay%2==0)
			{
				
				clGrp.PutSpriteT(startx, starty, 
					pMiniGameSpr[file]->Header.Xsize,
					pMiniGameSpr[file]->Header.Ysize,
					&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[Dir]], TeamLightTable);
			}
			else
			{
				clGrp.PutSpriteT(startx, starty, 
					pMiniGameSpr[file]->Header.Xsize,
					pMiniGameSpr[file]->Header.Ysize,
					&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[Dir]]);
			}
		}
		else
		{
				clGrp.PutSpriteT(startx, starty, 
					pMiniGameSpr[file]->Header.Xsize,
					pMiniGameSpr[file]->Header.Ysize,
					&pMiniGameSpr[file]->Image[ pMiniGameSpr[file]->Header.Start[Dir]]);
		}
		

		BExit.Put( pExitSpr,  0, 2, 1, BUTTON_PUT_NOMOVE );

		clGrp.UnlockSurface(psurface);
	}


	if(psurface->GetDC(&hdc)==DD_OK)
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		BExit.Put( hdc );
		psurface->ReleaseDC(hdc);
	}


	pGame->pNumberRender->Render(psurface);

	pMyOnlineWorld->pOnlineMsgBoxError->Draw(psurface);

	// ���콺�� �׸���. 
	DrawMouse(psurface);

}


void _MiniGame3::Quit()
{
	SI32 i;

	// �ε��� ȭ�ϵ��� �����Ѵ�. 
	for(i = 0;i < MAX_FILE_NUMBER_MINIGAME3;i++)
	{
		if(pMiniGameSpr[i])
		{
			if(pMiniGameSpr[i]->Image)
			{
				clGrp.FreeXspr(*pMiniGameSpr[i]);
				pMiniGameSpr[i]->Image = NULL;
			}

			delete pMiniGameSpr[i];
			pMiniGameSpr[i] = NULL;

		}
	}
	_MiniGame::Quit();
}


BOOL _MiniGame3::IsCollWithWorm()
{

	if(ToolPosition.x - (ToolSize.cx/2) <= WormPosition.x && WormPosition.x <= ToolPosition.x + (ToolSize.cx/2)
	&& ToolPosition.y - (ToolSize.cy/2) <= WormPosition.y && WormPosition.y <= ToolPosition.y + (ToolSize.cy/2))
	{
		return TRUE;
	}

	return FALSE;

}
//----------------------------------------------------------------------------------------------------


