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


	// Ű���۸� �ʱ�ȭ�Ͽ� ������ �Է��� �����Ѵ�. 
	GD.KB.InitKeyBuffer();


	// �ʿ��� ȭ���� �ҷ��´�. 
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

	// �̹����� ������ �ľ��Ѵ�. 
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

	// ������ ���� ������ �����Ѵ�. 
	DecideVaryByLevel();

	// Ĩ�� �ʱ�ȭ�Ѵ�. 
	InitChip();

	// Ĩ�� �̹����� �迭�Ѵ�. 
	ArrangeRandomImage();

	// ������ ��ư �׸�
	pExitSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_BOOKEXIT );		
	BExit.Create( 498, 504, pExitSpr->Header.Xsize, pExitSpr->Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );

#ifdef _OGG_PLAY
	 GetBgmMgr()->Play(OGGBGM_FILENAME_MINIGAME, TRUE);
#else
	// ������ �����Ѵ�. 
	PlayMusic("music\\MiniGame1.YAV");
#endif
}

void _MiniGame1::InitChip()
{

	SI32 i, j;

	// �̴ϰ��� ���� ȿ����. 
	PushEffect(EFFECT_MINIGAME1_START, 0, 0);

	for(i = 0;i<MINIGAME1_MAX_YNUMBER;i++)
		for(j=0;j<MINIGAME1_MAX_XNUMBER;j++)
		{
			Chip[j][i] = -1;
			ChipStatus[j][i] = 0;
		}
}

// �´� ¦�� �ִ��� Ȯ���Ѵ�. 
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
		
		// ������� �ִ�. 
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

	// �ʿ��� �̹��� ������ ��´�. 
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

	// ���콺�� ���� Ĩ�� ã�´�. 
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
	
	// ���콺�� ������ Ĩ�� ã�´�. 
	if(GameLeftPressSwitch == FALSE && PressedChip >=0 && (PressedChip == ChipInMouse) 
		&& ChipStatus[ChipInMouse%MINIGAME1_MAX_XNUMBER][ChipInMouse/MINIGAME1_MAX_XNUMBER] == 0 )
	{
		// ������ �۵����̴�. 
		MiniGame1StartSwitch = TRUE;
		
		if(ShowChipNumber < 2)
		{
			ChipStatus[ChipInMouse%MINIGAME1_MAX_XNUMBER][ChipInMouse/MINIGAME1_MAX_XNUMBER] = MINIGAME1_CHIP_SHOW_DELAY;
			ShowChipNumber++;
			
			// ī�� �ڵ����� ȿ����
			PushEffect(EFFECT_MINIGAME1_CARD, 0, 0);
			
		}
	}
	
	if(GameLeftPressSwitch == TRUE && ChipInMouse >= 0)
		PressedChip = ChipInMouse;
	else
		PressedChip = -1;

	if( BExit.Process( pMyOnlineWorld->fLeftUpSwitch ) == TRUE )
		return FALSE;

	// ���콺�� ����Ѵ�. 
	POINT point;
	GetCursorPos(&point);
	if (WindowModeSwitch)
		ScreenToClient(pGame->Hwnd,&point);
	
	IpD.Mouse_X=(SHORT)point.x;
	IpD.Mouse_Y=(SHORT)point.y;
	DrawMousePrimary();

	// ���� �������� �����Ѵ�. 
	SI32 diff = abs(CurrentClock - PrevClock);
	//if( diff < 30)return TRUE;
	
	if(GetPlayTime() >= diff)
	{
		if(MiniGame1StartSwitch == TRUE)
		{
			SI32 OldTime = GetPlayTime();

			SetPlayTime(GetPlayTime() - min(30*2, diff));
			SetHackMiniGamePlayTime(GetHackMiniGamePlayTime() - min(30*2, diff));

			// ī��Ʈ �ٿ� ȿ����. 
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
		
		// ������ �۾����� ��ȯ�Ѵ�. 
//		MakeWork();
		return FALSE;
	}

	// �� Ŭ�����ߴ��� Ȯ���Ѵ�. 
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
		
		// ������ �۾����� ��ȯ�Ѵ�. 
		MakeWork();
		
		SetMiniGameLevel(GetMiniGameLevel() + 1);
		SetHackMiniGameLevel(GetHackMiniGameLevel() + 1);
		
		// �����ܰ���� ���� �̴ϰ��� ������.
		if(DifficultyLevel[GetMiniGameLevel() * 3] == -1)		return FALSE;
		
		// ������ ���� ������ �����Ѵ�. 
		DecideVaryByLevel();
		// Ĩ�� �ʱ�ȭ�Ѵ�. 
		InitChip();
		// Ĩ�� �̹����� �迭�Ѵ�. 
		ArrangeRandomImage();
	}

	
	// ����� Ĩ�� ���� �ð��� ���δ�. 
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
		// �´� ¦�� �ִ��� Ȯ���Ѵ�. 
		POINT chip1, chip2;
		if(IsCheckPairs(&chip1, &chip2) == TRUE)
		{
			// ī��¦�� �¾Ҵ�. 
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
			// �����Ѵ�. 
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
						// ī�� �ڵ����� ȿ����
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

	// �ǳ��� �׸���. 
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

	// ���� �ð��� ǥ���Ѵ�. 
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

	// ������ ǥ���Ѵ�. 
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
				// ����Ǿ� ������, 
				if(ChipStatus[j][i] == MINIGAME1_CHIP_FOREVER_DELAY)
				{
				}
				else if(ChipStatus[j][i])
				{

					// ī�带 �׸���. 
					clGrp.PutSpriteT(ChipStartX + j*(ChipXsize+ChipXInterval),				ChipStartY + i*(ChipYsize+ChipYInterval), 
									pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Xsize,
									pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Ysize,
									&pMiniGameSpr[FILE_MINIGAME1_CARD]->Image[pMiniGameSpr[FILE_MINIGAME1_CARD]->Header.Start[ ChipBoard[j][i] ]]);

					// �̹����� �׸���. 
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

	// ���콺�� �׸���. 
	DrawMouse(psurface);
}


void _MiniGame1::Quit()
{
	SI32 i;

	// �ε��� ȭ�ϵ��� �����Ѵ�. 
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


