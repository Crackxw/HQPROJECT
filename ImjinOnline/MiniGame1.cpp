#include <GSL.h>
#include <MiniGame1.h>
#include <clGame.h>
#include "..\Gersang\music.h"
#include "..\Gersang\text.h"
#include "OnlineMsgBox.h"

#ifdef _OGG_PLAY
#include <list>
#include <queue>
#include "BgmMgr_PreHeader.h"
#endif
//--------------------------------------------------------------
// 공통 extern
//--------------------------------------------------------------
extern _clGame		*pGame;
extern	_MainData				GD;									// 게임의 주데이터 
extern _InputDevice			IpD;
extern clock_t StartClock, CurrentClock, PrevClock; 

extern	SHORT 				GameMouseX, GameMouseY;
extern	DWORD 				GameWParam;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern _Text					Text;										// 텍스트 클래스 선언 
extern  int                 WindowModeSwitch;
extern BOOL                 bActive;

_MiniGame1::_MiniGame1(SI32 minigametype, SI32 standardscore, cltOnlineWorld *pOnlineWorld):_MiniGame(minigametype, standardscore, pOnlineWorld)
{

}


void _MiniGame1::Init()
{
	SI32 index;
	SI32 i;

	SI32 data1[]= { 8,   9,  10,  11,  12,  13,  14,  15, 16, 17, 
		           18,  19,  20,  21,  22,  23,  24,  25, 26, 27, 
		          -1};

	SI32 data2[] = {	4, 3, 12*6/2,
						4, 3, 12*6/2,
						4, 4, 16*6/2,
						4, 4, 16*6/2,
						6, 3, 18*6/2,
						6, 3, 18*6/2,
						6, 4, 24*6/2,
						6, 4, 24*6/2,
						6, 5, 30*6/2,
						6, 5, 30*6/2,
						8, 3, 24*6/2, 
						8, 3, 24*6/2, 
						8, 4, 32*6/2, 
						8, 4, 32*6/2, 
						8, 5, 40*6/2, 
						8, 5, 40*6/2, 
						-1};

	char  *data3[] = 
	{
		"online\\minigame\\minigame1\\minigame1_back.spr",
		"online\\minigame\\minigame1\\minigame1_card.spr",
		""
	};

	ChipXInterval = 5;
	ChipYInterval = 3;

	ChipInMouse= -1;
	PressedChip= -1;

	ShowChipNumber		= 0;
	ReadyToHideNumber	= 0;

	index=0;
	while(1)
	{
		ImagePool[index] = data1[index];
		if(data1[index] == -1)break;
		index++;


	}

	index=0;
	while(1)
	{
		DifficultyLevel[index] = data2[index]; 
		if(data2[index] == -1)break;
		index++;
	}

	index=0;
	while(strcmp(data3[index], "")  != 0)
	{
		strcpy(MiniGameFileName[index], data3[index]);
		index++;
	}


	// 키버퍼를 초기화하여 이전의 입력을 무시한다. 
	GD.KB.InitKeyBuffer();


	// 필요한 화일을 불러온다. 
	for(i = 0;i < MAX_FILE_NUMBER_MINIGAME1;i++)
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

	if(pMiniGameSpr[FILE_MINIGAME1_CARD])
	{
		ChipXsize = pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Xsize;
		ChipYsize = pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize;
	}

	MiniGame1StartSwitch =  FALSE;

	// 이미지의 갯수를 파악한다. 
	ImageNumberInPool = 0;

	index = 0;
	while(1)
	{
		if(ImagePool[index] >= 0)
		{
			ImageNumberInPool++;
		}
		else
		{
			break;
		}

		index++;
	}

	ShowChipNumber		= 0;
	ReadyToHideNumber	= 0;
	bSendHackMsg = FALSE;

	// 레벨에 따라 변수를 설정한다. 
	DecideVaryByLevel();

	// 칩을 초기화한다. 
	InitChip();

	// 칩에 이미지를 배열한다. 
	ArrangeRandomImage();

	// 나가기 버튼 그림
	pExitSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_BOOKEXIT );		
	BExit.Create( 498, 504, pExitSpr->Header.Xsize, pExitSpr->Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );

#ifdef _OGG_PLAY
	 GetBgmMgr()->Play(OGGBGM_FILENAME_MINIGAME, TRUE);
#else
	// 음악을 연주한다. 
	PlayMusic("music\\MiniGame1.YAV");
#endif
}

void _MiniGame1::InitChip()
{

	SI32 i, j;

	// 미니게임 시작 효과음. 
	PushEffect(EFFECT_MINIGAME1_START, 0, 0);

	for(i = 0;i<MINIGAME1_MAX_YNUMBER;i++)
		for(j=0;j<MINIGAME1_MAX_XNUMBER;j++)
		{
			Chip[j][i] = -1;
			ChipStatus[j][i] = 0;
		}
}

// 맞는 짝이 있는지 확인한다. 
BOOL _MiniGame1::IsCheckPairs(POINT* pchip1, POINT* pchip2)
{
	SI32 i, j;
	SI32 Chip1 = -1, Chip2= -1;
	SI32 Chip1X = 0, Chip1Y = 0;
	SI32 Chip2X = 0, Chip2Y = 0;
	
	for(i = 0; i < YNumber;i++)
		for(j = 0;j < XNumber;j++)
		{
			
			if(ChipStatus[j][i] == (MINIGAME1_CHIP_SHOW_DELAY-MINIGAME1_CHIP_ANI_DELAY))
			{
				if(Chip1 == -1)
				{
					Chip1 = Chip[j][i];
					Chip1X = j;
					Chip1Y = i;
				}
				else if(Chip2 == -1)
				{
					Chip2 = Chip[j][i];
					Chip2X = j;
					Chip2Y = i;
					
				}
				
			}
		}
		
		// 맞춘것이 있다. 
		if(Chip1 >= 0 && Chip1 == Chip2 )
		{
			pchip1->x = Chip1X;
			pchip1->y = Chip1Y;
			pchip2->x = Chip2X;
			pchip2->y = Chip2Y;
			return TRUE;
		}

		return FALSE;

}

void _MiniGame1::ArrangeRandomImage()
{
	SI32 i, j;

	// 필요한 이미지 갯수를 얻는다. 
	SI32 needimagenumber = XNumber * YNumber / 2;

	SI32 xpos, ypos;



	while(needimagenumber)
	{
		while(1)
		{
			xpos = rand() % XNumber;
			ypos = rand() % YNumber;
			
			if(Chip[xpos][ypos] == -1)
			{
				Chip[xpos][ypos] = ImagePool[needimagenumber - 1];
				break;
			}
		}
		
		while(1)
		{
			xpos = rand() % XNumber;
			ypos = rand() % YNumber;
			
			if(Chip[xpos][ypos] == -1)
			{
				Chip[xpos][ypos] = ImagePool[needimagenumber - 1];
				break;
			}
		}
		
		needimagenumber --;
	}

	for(i = 0; i < YNumber;i++)
		for(j = 0;j < XNumber;j++)
		{
			ChipBoard[j][i] = rand() % 4;
		}

}

void _MiniGame1::DecideVaryByLevel()
{
	if(DifficultyLevel[GetMiniGameLevel() * 3] == -1)
	{
		SetMiniGameLevel(0);
	}

	XNumber		= DifficultyLevel[GetMiniGameLevel() * 3];
	YNumber		= DifficultyLevel[GetMiniGameLevel() * 3 + 1];
	SetPlayTime(DifficultyLevel[GetMiniGameLevel() * 3 + 2] * 1000);
	SetHackMiniGamePlayTime(DifficultyLevel[GetMiniGameLevel() * 3 + 2] * 1000);

	ChipStartX = (clGrp.GetScreenXsize() - (ChipXsize + ChipXInterval) * XNumber) / 2;
	ChipStartY = (clGrp.GetScreenYsize() - (ChipYsize + ChipYInterval) * YNumber) / 2;

}

BOOL _MiniGame1::IsMouseInChip(SI32 x, SI32 y)
{
	if( ChipStartX + x*(ChipXsize+ChipXInterval) <= GameMouseX && GameMouseX <= ChipStartX + x*(ChipXsize+ChipXInterval) + ChipXsize
	&&  ChipStartY + y*(ChipYsize+ChipYInterval) <= GameMouseY && GameMouseY <= ChipStartY + y*(ChipYsize+ChipYInterval) + ChipYsize)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _MiniGame1::Action()
{
	SI32 i, j; 

	if(!CheckHacking())	return FALSE;

	InputOperate();

	// 마우스가 눌린 칩을 찾는다. 
	ChipInMouse = -1;
	for(i = 0; i < YNumber;i++)
	{
		for(j = 0;j < XNumber;j++)
		{
			
			if(IsMouseInChip(j, i) == TRUE)
			{
				
				ChipInMouse = j + i * MINIGAME1_MAX_XNUMBER;
			}

		}
	}
	
	// 마우스로 선택한 칩을 찾는다. 
	if(GameLeftPressSwitch == FALSE && PressedChip >=0 && (PressedChip == ChipInMouse) 
		&& ChipStatus[ChipInMouse%MINIGAME1_MAX_XNUMBER][ChipInMouse/MINIGAME1_MAX_XNUMBER] == 0 )
	{
		// 게임이 작동중이다. 
		MiniGame1StartSwitch = TRUE;
		
		if(ShowChipNumber < 2)
		{
			ChipStatus[ChipInMouse%MINIGAME1_MAX_XNUMBER][ChipInMouse/MINIGAME1_MAX_XNUMBER] = MINIGAME1_CHIP_SHOW_DELAY;
			ShowChipNumber++;
			
			// 카드 뒤뒤집기 효과음
			PushEffect(EFFECT_MINIGAME1_CARD, 0, 0);
			
		}
	}
	
	if(GameLeftPressSwitch == TRUE && ChipInMouse >= 0)
		PressedChip = ChipInMouse;
	else
		PressedChip = -1;

	if( BExit.Process( pMyOnlineWorld->fLeftUpSwitch ) == TRUE )
		return FALSE;

	// 마우스를 출력한다. 
	POINT point;
	GetCursorPos(&point);
	if (WindowModeSwitch)
		ScreenToClient(pGame->Hwnd,&point);
	
	IpD.Mouse_X=(SHORT)point.x;
	IpD.Mouse_Y=(SHORT)point.y;
	DrawMousePrimary();

	// 일정 프레임을 유지한다. 
	SI32 diff = abs(CurrentClock - PrevClock);
	//if( diff < 30)return TRUE;
	
	if(GetPlayTime() >= diff)
	{
		if(MiniGame1StartSwitch == TRUE)
		{
			SI32 OldTime = GetPlayTime();

			SetPlayTime(GetPlayTime() - min(30*2, diff));
			SetHackMiniGamePlayTime(GetHackMiniGamePlayTime() - min(30*2, diff));

			// 카운트 다운 효과음. 
			if(GetPlayTime()/1000 < 10)
			{
				if( (OldTime/1000) - (GetPlayTime()/1000) )
					PushEffect(EFFECT_MINIGAME1_TIME, 0, 0);
			}
		}
	}
	else
	{
		SetPlayTime(0);
		SetHackMiniGamePlayTime(0);
		
		// 점수를 작업으로 전환한다. 
//		MakeWork();
		return FALSE;
	}

	// 다 클리어했는지 확인한다. 
	BOOL finishedswitch = TRUE;
	for(i = 0; i < YNumber;i++)
	{
		for(j = 0;j < XNumber;j++)
		{
			if(ChipStatus[j][i] != MINIGAME1_CHIP_FOREVER_DELAY)
			{
				finishedswitch = FALSE;
			}
		}
	}
	
	if(finishedswitch == TRUE)
	{
		MiniGame1StartSwitch = FALSE;
		
		// 점수를 작업으로 전환한다. 
		MakeWork();
		
		SetMiniGameLevel(GetMiniGameLevel() + 1);
		SetHackMiniGameLevel(GetHackMiniGameLevel() + 1);
		
		// 최종단계까지 가면 미니겜을 끝낸다.
		if(DifficultyLevel[GetMiniGameLevel() * 3] == -1)		return FALSE;
		
		// 레벨에 따라 변수를 설정한다. 
		DecideVaryByLevel();
		// 칩을 초기화한다. 
		InitChip();
		// 칩에 이미지를 배열한다. 
		ArrangeRandomImage();
	}

	
	// 노출된 칩의 노출 시간을 줄인다. 
	ReadyToHideNumber = 0;
	for(i = 0; i < YNumber;i++)
	{
		for(j = 0;j < XNumber;j++)
		{
			if(ChipStatus[j][i] > 0 && ChipStatus[j][i] != MINIGAME1_CHIP_FOREVER_DELAY)
			{
				if(ChipStatus[j][i] > (MINIGAME1_CHIP_SHOW_DELAY-MINIGAME1_CHIP_ANI_DELAY))
				{
					ChipStatus[j][i]--;
				}
				else
				{
					ReadyToHideNumber++;
				}
			}
			
		}
	}

	if(ReadyToHideNumber == 2)
	{
		// 맞는 짝이 있는지 확인한다. 
		POINT chip1, chip2;
		if(IsCheckPairs(&chip1, &chip2) == TRUE)
		{
			// 카드짝이 맞았다. 
			PushEffect(EFFECT_MINIGAME1_PAIR, 0, 0);
			
			ChipStatus[chip1.x][chip1.y]	= MINIGAME1_CHIP_FOREVER_DELAY;
			ChipStatus[chip2.x][chip2.y]	= MINIGAME1_CHIP_FOREVER_DELAY;
			ShowChipNumber		= 0;
			ReadyToHideNumber	= 0;
			
			SetMiniGamePoint(GetMiniGamePoint()+(GetPlayTime()/1000));
			SetHackMiniGamePoint(GetHackMiniGamePoint()+(GetPlayTime()/1000));
		}
		else
		{
			// 감점한다. 
			SetMiniGamePoint( max(0, GetMiniGamePoint() - 1) );
			SetHackMiniGamePoint( max(0, GetHackMiniGamePoint() - 1) );
		}
		
		for(i = 0; i < YNumber;i++)
		{
			for(j = 0;j < XNumber;j++)
			{
				if(ChipStatus[j][i] > 0 && ChipStatus[j][i] != MINIGAME1_CHIP_FOREVER_DELAY)
				{
					ChipStatus[j][i]--;
					if(ChipStatus[j][i] == 0)
					{
						// 카드 뒤뒤집기 효과음
						PushEffect(EFFECT_MINIGAME1_CARDCOVER, 0, 0);
						
						ShowChipNumber--;
						ReadyToHideNumber--;
					}
				}
			}
		}
	}

	return TRUE;
}

void _MiniGame1::Draw(LPDIRECTDRAWSURFACE7 psurface)
{

	SI32 i, j;
	HDC hdc;
	char buffer[128];

	// 판넬을 그린다. 
	if(clGrp.LockSurface(psurface) == TRUE)
	{

		SI32 startx = (clGrp.GetScreenXsize() - pMiniGameSpr[FILE_MINIGAME1_BACK]->Header.Xsize)/ 2;
		SI32 starty = (clGrp.GetScreenYsize() - pMiniGameSpr[FILE_MINIGAME1_BACK]->Header.Ysize)/ 2;

		clGrp.PutSpriteT(startx, starty, 
						pMiniGameSpr[FILE_MINIGAME1_BACK]->Header.Xsize,
						pMiniGameSpr[FILE_MINIGAME1_BACK]->Header.Ysize,
						pMiniGameSpr[FILE_MINIGAME1_BACK]->Image);

		
		clGrp.UnlockSurface(psurface);
	}

	// 남은 시간을 표시한다. 
	wsprintf(buffer, "%d", GetPlayTime()/1000);

	SI32 percent = (GetPlayTime() % 1000) / 10;
	
	SI32 xsize, ysize;
	if(GetPlayTime()/1000 > 10)
	{
		xsize = 15 ;
		ysize = 8  ;
	}
	else
	{
		xsize = 22  * percent / 100; 
		ysize = 12  * percent / 100; 
	}

	RECT destrect = {400-xsize, 96 - ysize, 400+xsize, 96 + ysize};
	DrawTextInRect(psurface, pGame->pBattle->SaveInfoFont, buffer, destrect);

	// 점수를 표시한다. 
	if(psurface->GetDC(&hdc)==DD_OK)
	{
		SelectObject(hdc, pGame->pBattle->HelpFont);
		
		SetTextColor( hdc, NORMAL_TEXT_COLOR );
		SetBkMode(hdc, TRANSPARENT);

		wsprintf(buffer, Text.Scores.Get());
		DrawTextInCenter(hdc, 203, 484, buffer);
		wsprintf(buffer, "%d",GetMiniGamePoint());
		DrawTextInCenter(hdc, 278, 484, buffer);

		wsprintf(buffer, Text.Difficulty.Get());
		DrawTextInCenter(hdc, 345, 484, buffer);
		wsprintf(buffer, "%d",GetMiniGameLevel());
		DrawTextInCenter(hdc, 409, 484, buffer);
		
		wsprintf(buffer, Text.Work.Get());
		DrawTextInCenter(hdc, 474, 484, buffer);
		wsprintf(buffer, "%d",GetMiniGameWork());
		DrawTextInCenter(hdc, 567, 484, buffer);

		psurface->ReleaseDC(hdc);
	}

	if(clGrp.LockSurface(psurface) == TRUE)
	{
		for(i = 0; i < YNumber;i++)
			for(j = 0;j < XNumber;j++)
			{
				// 노출되어 있으면, 
				if(ChipStatus[j][i] == MINIGAME1_CHIP_FOREVER_DELAY)
				{
				}
				else if(ChipStatus[j][i])
				{

					// 카드를 그린다. 
					clGrp.PutSpriteT(ChipStartX + j*(ChipXsize+ChipXInterval),				ChipStartY + i*(ChipYsize+ChipYInterval), 
									pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Xsize,
									pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize,
									&pMiniGameSpr[FILE_MINIGAME1_CARD]->Image[pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Start[ ChipBoard[j][i] ]]);

					// 이미지를 그린다. 
					clGrp.PutSpriteT(ChipStartX + j*(ChipXsize+ChipXInterval),				ChipStartY + i*(ChipYsize+ChipYInterval), 
									pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Xsize,
									pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize,
									&pMiniGameSpr[FILE_MINIGAME1_CARD]->Image[pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Start[ Chip[j][i] ]]);

				}
				
				SI32 data = ChipStatus[j][i];
				SI32 ysize = pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize;
				
				if(data == MINIGAME1_CHIP_FOREVER_DELAY)
				{
					ysize = 0;
				}
				else if(data)
				{
					ysize = 0;

					if(data >= (MINIGAME1_CHIP_SHOW_DELAY-MINIGAME1_CHIP_ANI_DELAY))
					{
						ysize = pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize * (data-(MINIGAME1_CHIP_SHOW_DELAY-MINIGAME1_CHIP_ANI_DELAY)) / MINIGAME1_CHIP_ANI_DELAY;
					}
					else if(data <= MINIGAME1_CHIP_ANI_DELAY)
					{
						ysize = pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize * (MINIGAME1_CHIP_ANI_DELAY - data) / MINIGAME1_CHIP_ANI_DELAY;
					}
					
				}
				else
				{
					ysize = pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize;
				}
				
				clGrp.PutSpriteT(ChipStartX + j*(ChipXsize+ChipXInterval),				ChipStartY + i*(ChipYsize+ChipYInterval) + (pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize - ysize), 
					pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Xsize,
					ysize,
					&pMiniGameSpr[FILE_MINIGAME1_CARD]->Image[pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Start[ 4 + ChipBoard[j][i] ]]);


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

	pMyOnlineWorld->pOnlineMsgBoxError->Draw(psurface);

	// 마우스를 그린다. 
	DrawMouse(psurface);
}


void _MiniGame1::Quit()
{
	SI32 i;

	// 로드한 화일들을 해제한다. 
	for(i = 0;i < MAX_FILE_NUMBER_MINIGAME1;i++)
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


//----------------------------------------------------------------------------------------------------


