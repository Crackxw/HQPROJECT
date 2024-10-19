//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>
#include <main.h>
#include <clGame.h>

#include "..\Gersang\music.h"
#include "..\Gersang\message.h"
#include "..\Gersang\help.h"	

#include <myfile.h>	// hojae_append
#include "..\Gersang\DirectX.h"
#include <gamebutton.h>

extern _clGame		*pGame;
extern	SHORT 					GameMouseX, GameMouseY;
extern	int 					MapDragSwitch;
extern	SHORT 					GameLeftPressSwitch;
extern	_Help 					Help;

SI32 ButtonOfMouse = -1;
SI32 SelectedButton;
BOOL ButtonLeftPressSelectedSwitch;
BOOL ButtonSelectedSwitch;


void _GameButton::Init()
{
	bValidSwitch = FALSE;
	strcpy(pString, "");
}

// 버튼을 만든다.
void _GameButton::SetGameButton(SI32 ButtonType,			SI32 X,					SI32 Y, 
								 SI32 PrePressBackFile,		SI32 PrePressBackFont,	BOOL PrePressBackDraw32Switch,
								 SI32 PressBackFile,		SI32 PressBackFont,		BOOL PressBackDraw32Switch,
								 SI32 MouseBackFile,		SI32 MouseBackFont,		BOOL MouseBackDraw32Switch,
								 SI32 ButtonImageFile,		SI32 ButtonImageFont,	BOOL ButtonImageDraw32Switch,
								 SI32 Effect,
								 SI32 Para1, 
								 SI32 Para2, 
								 SI32 (*Func)(SI32, SI32),
								 SI32 (*HelpFunc)(SI32, SI32))
{


	// 버튼이 유효하게 되었다. 
	bValidSwitch  = TRUE;

	// 버튼의 유형. 
	siButtonType = ButtonType;

		// 버튼 시작 위치 
	siX = X;
	siY = Y;

	// 버튼의 가로 세로 크기 
	if(PrePressBackFile != -1)
	{
		siButtonXsize = GetFileXsize(PrePressBackFile);
		siButtonYsize = GetFileYsize(PrePressBackFile);
	}
	else if(ButtonImageFile != -1)
	{
		siButtonXsize = GetFileXsize(ButtonImageFile);
		siButtonYsize = GetFileYsize(ButtonImageFile);
	}


	// 눌리기 이전의 버튼 배경 그림. 
	siPrePressBackFile = PrePressBackFile;
	siPrePressBackFont = PrePressBackFont;
	bPrePressBackDraw32Switch = PrePressBackDraw32Switch;

	// 눌린 상태의 버튼 배경 그림. 
	siPressBackFile		= PressBackFile;
	siPressBackFont		= PressBackFont;
	bPressBackDraw32Switch = PressBackDraw32Switch;


	// 마우스가 버튼 위에 있을 때의 배경 그림. 
	siMouseBackFile		= MouseBackFile;
	siMouseBackFont		= MouseBackFont;
	bMouseBackDraw32Switch	= MouseBackDraw32Switch;

	// 버튼 내용 그림. 
	siButtonImageFile	=ButtonImageFile;
	siButtonImageFont	= ButtonImageFont;
	bButtonImageDraw32Switch = ButtonImageDraw32Switch;

	// 버튼이 눌렸을때의 효과음.
	siEffect = Effect;

	siPara1 = Para1;
	siPara2 = Para2;

	pFunc = Func;
	pHelpFunc = HelpFunc;
}

// 버튼을 만든다.
void _GameButton::SetGameButton(SI32 ButtonType,			SI32 X,					SI32 Y, 
								 SI32 PrePressBackFile,		SI32 PrePressBackFont,	BOOL PrePressBackDraw32Switch,
								 SI32 PressBackFile,		SI32 PressBackFont,		BOOL PressBackDraw32Switch,
								 SI32 MouseBackFile,		SI32 MouseBackFont,		BOOL MouseBackDraw32Switch,
								 char *pstr,				char *phelpstr,
								 SI32 Effect,
								 SI32 Para1, 
								 SI32 Para2, 
								 SI32 (*Func)(SI32, SI32),
								 SI32 (*HelpFunc)(SI32, SI32))
{


	// 버튼이 유효하게 되었다. 
	bValidSwitch  = TRUE;

	// 버튼의 유형. 
	siButtonType = ButtonType;

		// 버튼 시작 위치 
	siX = X;
	siY = Y;

	// 버튼의 가로 세로 크기 
	if(PrePressBackFile != -1)
	{
		siButtonXsize = GetFileXsize(PrePressBackFile);
		siButtonYsize = GetFileYsize(PrePressBackFile);
	}

	// 눌리기 이전의 버튼 배경 그림. 
	siPrePressBackFile = PrePressBackFile;
	siPrePressBackFont = PrePressBackFont;
	bPrePressBackDraw32Switch = PrePressBackDraw32Switch;

	// 눌린 상태의 버튼 배경 그림. 
	siPressBackFile		= PressBackFile;
	siPressBackFont		= PressBackFont;
	bPressBackDraw32Switch = PressBackDraw32Switch;


	// 마우스가 버튼 위에 있을 때의 배경 그림. 
	siMouseBackFile		= MouseBackFile;
	siMouseBackFont		= MouseBackFont;
	bMouseBackDraw32Switch	= MouseBackDraw32Switch;

	// 버튼 내용 그림. 
	siButtonImageFile	= -1;
	siButtonImageFont	= -1;
	bButtonImageDraw32Switch = FALSE;

	if(pstr)
		strcpy(pString, pstr);
	if(phelpstr)
		strcpy(pHelpString, phelpstr);

#ifndef _USE_HASHCODE_EFFECTSOUND
	// 버튼이 눌렸을때의 효과음.
	siEffect	= Effect;
#else
	
#endif
	siPara1		= Para1;
	siPara2		= Para2;

	pFunc		= Func;
	pHelpFunc	= HelpFunc;
}

	// 버튼을 그려준다. 
void _GameButton::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	CHAR buffer[1024];
	SI32 siFile = siPrePressBackFile;
	SI32 siFont = siPrePressBackFont;
	BOOL bDraw32Switch = bPrePressBackDraw32Switch;

	// 클리핑 영역을 기억한다. 
	SI32 clipx1, clipx2, clipy1, clipy2;
	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);


	// 현재 버튼의 상태를 알아야 한다. (눌린상태, 마우스 댐, 누르기전)
	if(bLeftPressSwitch == TRUE)
	{
		siFile			= siPressBackFile;
		siFont			= siPressBackFont;
		bDraw32Switch	= bPressBackDraw32Switch;
	}
	else if(bMouseSwitch == TRUE)
	{
		siFile			= siMouseBackFile;
		siFont			= siMouseBackFont;
		bDraw32Switch	= bMouseBackDraw32Switch;
	}
	else
	{
		siFile			= siPrePressBackFile;
		siFont			= siPrePressBackFont;
		bDraw32Switch	= bPrePressBackDraw32Switch;
		
	}
	
	if(clGrp.LockSurface(surface)==TRUE)
	{
		if(siFile != -1 && siFont !=-1)
		{
			if(bDraw32Switch  == TRUE)
			{
				Draw_PutImage32(siX, siY, siButtonXsize, siButtonYsize, siFile, siFont);	// hojae_append

			}
			else
			{
				Draw_PutSpriteT(siX, siY, siButtonXsize, siButtonYsize, siFile, siFont);	// hojae_append
			}
		}


		// 버튼의 정보를 표시해준다. (그림일때..)
		if(pString 
		&& siButtonImageFile != -1 && siButtonImageFont != -1)
		{
			if(bButtonImageDraw32Switch == TRUE)
			{
				Draw_PutImage32(siX, siY, siButtonImageFile, siButtonImageFont);	// hojae_append
			}
			else
			{
				Draw_PutSpriteT(siX, siY, siButtonImageFile, siButtonImageFont);	// hojae_append
			}
		}


	   clGrp.UnlockSurface(surface);
	}

	// 게임 메뉴 를 그린다. (정보가 텍스트로 표시될때)
	HDC hdc;
	SIZE		Size;
	if( pString && surface->GetDC(&hdc)==DD_OK)
	{
	    SelectObject(hdc, pGame->pBattle->HelpFont);
		SetTextColor( hdc, NORMAL_TEXT_COLOR);
		SetBkMode(hdc, TRANSPARENT);

		GetTextExtentPoint32(hdc, pString, strlen(pString), &Size);
	
		TextOut(hdc, siX + (siButtonXsize - Size.cx)/2, 
					 siY + (siButtonYsize - Size.cy)/2,
					 pString, strlen(pString) );

		surface->ReleaseDC(hdc);
	}

	//-----------------------------
	// 마우스가 버튼 위에 있다면


	if( bMouseSwitch == TRUE && strcmp(pHelpString, "") != 0  )
	{
		wsprintf(buffer, "%s", pHelpString);

		Help.SetHelp(siX, siY, 
			buffer,
			NULL,
			0,
			0,
			0,
			0,
			0,
			NULL);
	}


	// 클리핑 영역을 재 설정한다. 
	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
}

// 마우스가 버튼위에 있는지 확인한다. 
BOOL _GameButton::IsMouseInButton()
{
	if(bValidSwitch == FALSE)return FALSE;

	if(siX	<	GameMouseX && GameMouseX < (siX + siButtonXsize)
	&& siY	<	GameMouseY && GameMouseY < (siY + siButtonYsize))
	{
		return TRUE;
	}
	return FALSE;
}


void _GameButton::PUSHEFFECT()
{
	if(siEffect)
#ifndef _USE_HASHCODE_EFFECTSOUND
		::PushEffect(siEffect, 0, 0);
#else
		::HashCodePushEffect(siEffect, 0, 0);
#endif
}




// 빈 버튼을 찾는다. 
SI32 _clBattle::FindEmptyGameButton()
{

	SI32 i;

	for( i = 1; i < MAX_GAME_BUTTON;i++)
	{
		if(GameButton[i].bValidSwitch == FALSE)return i;
	}

	return 0;
}

// 빈 버튼을 찾는다. (중복된 버튼이 있으면 찾지 않는다. )
SI32 _clBattle::FindEmptyGameButton(SI32 ButtonType)
{

	SI32 i;
	SI32 siValidSlot = 0;

	for( i = 1; i < MAX_GAME_BUTTON;i++)
	{
		if(GameButton[i].bValidSwitch == FALSE)
		{
			siValidSlot = i;
		}
		// 이미 그 버튼이 있으면, 
		else if(GameButton[i].siButtonType == ButtonType)
		{
			return 0;
		}
	}

	return siValidSlot;
}


// 게임 버튼을 종류에 의거하여 삭제한다. 
void _clBattle::DeleteGameButtonByType(SI32 ButtonType)
{
	SI32 i;

	for( i = 1; i < MAX_GAME_BUTTON;i++)
	{
		if(GameButton[i].bValidSwitch == TRUE)
		{
			if(GameButton[i].siButtonType == ButtonType)
			{
				GameButton[i].bValidSwitch = FALSE;
			}

		}
	}
}

void _clBattle::InitGameButton()
{

	SI32 i;

	ButtonOfMouse = -1;

	for( i = 1; i < MAX_GAME_BUTTON;i++)
	{
		if(GameButton[i].bValidSwitch == TRUE)
		{
			GameButton[i].Init();
		}
	}
}

// 게임중에 나오는 버튼들을 모아서 그려준다. 
void _clBattle::DrawGameButton(LPDIRECTDRAWSURFACE7 surface)
{
	SI32 i;

	for( i = 1; i < MAX_GAME_BUTTON;i++)
	{
		if(GameButton[i].bValidSwitch == TRUE)
		{
			GameButton[i].Draw(surface);
		}
	}
}


BOOL _clBattle::OperateGameButton()
{

	SI32 i;
	// 마우스가 버튼 위에 없다. 로 초기화 
	BOOL IsMouseInGameButtonSwitch = FALSE;

	for( i = 1;i < MAX_GAME_BUTTON;i++)
	{
		// 버튼의 상태를 초기화한다. 
		GameButton[i].bMouseSwitch		= FALSE;
		GameButton[i].bLeftPressSwitch	= FALSE;

		if(GameButton[i].IsMouseInButton() == TRUE
		&& MapDragSwitch == FALSE)
		{

			// 마우스가 게임 버튼위에 있다는 사실을 설정한다. 
			IsMouseInGameButtonSwitch = TRUE;
			
			ButtonOfMouse = i;
			// 마우스가 버튼 위에 있음을 알려준다. 
			GameButton[i].bMouseSwitch = TRUE;

			// 버튼이 눌렸음을 알려준다. 
			if(i == SelectedButton)
			{
				GameButton[i].bLeftPressSwitch = TRUE;
			}


			if(GameButton[i].pHelpFunc)
			{
				GameButton[i].pHelpFunc(GameButton[i].siPara1 , GameButton[i].siPara2 );
			}
			
			// 이미 선택된 메뉴라면, 
			if(SelectedButton == i)
			{
				// 최종 선택 
				if(ButtonLeftPressSelectedSwitch == TRUE)
				{
					
					if(GameLeftPressSwitch==FALSE)
					{

						// 필요한 조치를 취하는 함수를 호출한다. 
						if(GameButton[i].pFunc)
							GameButton[i].pFunc(GameButton[i].siPara1, GameButton[i].siPara2);

						// 선택된 상태를 초기화한다. 
						SelectedButton=-1;
						ButtonSelectedSwitch=FALSE;
						
					}
					
				}
			}
			// 선택되지 않은 다른 메뉴위라면, 
			else
			{
				// 왼쪽 버튼이 눌린 것이라면, 
				if(GameLeftPressSwitch==TRUE)
				{
					GameButton[i].PUSHEFFECT();
					
					ButtonLeftPressSelectedSwitch=TRUE;
					SelectedButton=i;
					ButtonSelectedSwitch=TRUE;
				}
			}
			
		}
		
	}

	return IsMouseInGameButtonSwitch;
}