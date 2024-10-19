//--------------------------------------------------------------------
//  ��� : ���°�
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

// ��ư�� �����.
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


	// ��ư�� ��ȿ�ϰ� �Ǿ���. 
	bValidSwitch  = TRUE;

	// ��ư�� ����. 
	siButtonType = ButtonType;

		// ��ư ���� ��ġ 
	siX = X;
	siY = Y;

	// ��ư�� ���� ���� ũ�� 
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


	// ������ ������ ��ư ��� �׸�. 
	siPrePressBackFile = PrePressBackFile;
	siPrePressBackFont = PrePressBackFont;
	bPrePressBackDraw32Switch = PrePressBackDraw32Switch;

	// ���� ������ ��ư ��� �׸�. 
	siPressBackFile		= PressBackFile;
	siPressBackFont		= PressBackFont;
	bPressBackDraw32Switch = PressBackDraw32Switch;


	// ���콺�� ��ư ���� ���� ���� ��� �׸�. 
	siMouseBackFile		= MouseBackFile;
	siMouseBackFont		= MouseBackFont;
	bMouseBackDraw32Switch	= MouseBackDraw32Switch;

	// ��ư ���� �׸�. 
	siButtonImageFile	=ButtonImageFile;
	siButtonImageFont	= ButtonImageFont;
	bButtonImageDraw32Switch = ButtonImageDraw32Switch;

	// ��ư�� ���������� ȿ����.
	siEffect = Effect;

	siPara1 = Para1;
	siPara2 = Para2;

	pFunc = Func;
	pHelpFunc = HelpFunc;
}

// ��ư�� �����.
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


	// ��ư�� ��ȿ�ϰ� �Ǿ���. 
	bValidSwitch  = TRUE;

	// ��ư�� ����. 
	siButtonType = ButtonType;

		// ��ư ���� ��ġ 
	siX = X;
	siY = Y;

	// ��ư�� ���� ���� ũ�� 
	if(PrePressBackFile != -1)
	{
		siButtonXsize = GetFileXsize(PrePressBackFile);
		siButtonYsize = GetFileYsize(PrePressBackFile);
	}

	// ������ ������ ��ư ��� �׸�. 
	siPrePressBackFile = PrePressBackFile;
	siPrePressBackFont = PrePressBackFont;
	bPrePressBackDraw32Switch = PrePressBackDraw32Switch;

	// ���� ������ ��ư ��� �׸�. 
	siPressBackFile		= PressBackFile;
	siPressBackFont		= PressBackFont;
	bPressBackDraw32Switch = PressBackDraw32Switch;


	// ���콺�� ��ư ���� ���� ���� ��� �׸�. 
	siMouseBackFile		= MouseBackFile;
	siMouseBackFont		= MouseBackFont;
	bMouseBackDraw32Switch	= MouseBackDraw32Switch;

	// ��ư ���� �׸�. 
	siButtonImageFile	= -1;
	siButtonImageFont	= -1;
	bButtonImageDraw32Switch = FALSE;

	if(pstr)
		strcpy(pString, pstr);
	if(phelpstr)
		strcpy(pHelpString, phelpstr);

#ifndef _USE_HASHCODE_EFFECTSOUND
	// ��ư�� ���������� ȿ����.
	siEffect	= Effect;
#else
	
#endif
	siPara1		= Para1;
	siPara2		= Para2;

	pFunc		= Func;
	pHelpFunc	= HelpFunc;
}

	// ��ư�� �׷��ش�. 
void _GameButton::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	CHAR buffer[1024];
	SI32 siFile = siPrePressBackFile;
	SI32 siFont = siPrePressBackFont;
	BOOL bDraw32Switch = bPrePressBackDraw32Switch;

	// Ŭ���� ������ ����Ѵ�. 
	SI32 clipx1, clipx2, clipy1, clipy2;
	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);


	// ���� ��ư�� ���¸� �˾ƾ� �Ѵ�. (��������, ���콺 ��, ��������)
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


		// ��ư�� ������ ǥ�����ش�. (�׸��϶�..)
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

	// ���� �޴� �� �׸���. (������ �ؽ�Ʈ�� ǥ�õɶ�)
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
	// ���콺�� ��ư ���� �ִٸ�


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


	// Ŭ���� ������ �� �����Ѵ�. 
	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
}

// ���콺�� ��ư���� �ִ��� Ȯ���Ѵ�. 
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




// �� ��ư�� ã�´�. 
SI32 _clBattle::FindEmptyGameButton()
{

	SI32 i;

	for( i = 1; i < MAX_GAME_BUTTON;i++)
	{
		if(GameButton[i].bValidSwitch == FALSE)return i;
	}

	return 0;
}

// �� ��ư�� ã�´�. (�ߺ��� ��ư�� ������ ã�� �ʴ´�. )
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
		// �̹� �� ��ư�� ������, 
		else if(GameButton[i].siButtonType == ButtonType)
		{
			return 0;
		}
	}

	return siValidSlot;
}


// ���� ��ư�� ������ �ǰ��Ͽ� �����Ѵ�. 
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

// �����߿� ������ ��ư���� ��Ƽ� �׷��ش�. 
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
	// ���콺�� ��ư ���� ����. �� �ʱ�ȭ 
	BOOL IsMouseInGameButtonSwitch = FALSE;

	for( i = 1;i < MAX_GAME_BUTTON;i++)
	{
		// ��ư�� ���¸� �ʱ�ȭ�Ѵ�. 
		GameButton[i].bMouseSwitch		= FALSE;
		GameButton[i].bLeftPressSwitch	= FALSE;

		if(GameButton[i].IsMouseInButton() == TRUE
		&& MapDragSwitch == FALSE)
		{

			// ���콺�� ���� ��ư���� �ִٴ� ����� �����Ѵ�. 
			IsMouseInGameButtonSwitch = TRUE;
			
			ButtonOfMouse = i;
			// ���콺�� ��ư ���� ������ �˷��ش�. 
			GameButton[i].bMouseSwitch = TRUE;

			// ��ư�� �������� �˷��ش�. 
			if(i == SelectedButton)
			{
				GameButton[i].bLeftPressSwitch = TRUE;
			}


			if(GameButton[i].pHelpFunc)
			{
				GameButton[i].pHelpFunc(GameButton[i].siPara1 , GameButton[i].siPara2 );
			}
			
			// �̹� ���õ� �޴����, 
			if(SelectedButton == i)
			{
				// ���� ���� 
				if(ButtonLeftPressSelectedSwitch == TRUE)
				{
					
					if(GameLeftPressSwitch==FALSE)
					{

						// �ʿ��� ��ġ�� ���ϴ� �Լ��� ȣ���Ѵ�. 
						if(GameButton[i].pFunc)
							GameButton[i].pFunc(GameButton[i].siPara1, GameButton[i].siPara2);

						// ���õ� ���¸� �ʱ�ȭ�Ѵ�. 
						SelectedButton=-1;
						ButtonSelectedSwitch=FALSE;
						
					}
					
				}
			}
			// ���õ��� ���� �ٸ� �޴������, 
			else
			{
				// ���� ��ư�� ���� ���̶��, 
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