/************************************************************************************************
	���ϸ� : clItemInfoWindow.cpp
	����� : ������

	Item ������ ǥ���ϴ� â
************************************************************************************************/
#include <GSL.h>

#include	<clGame.h>


#include <main.h>
#include <clGame.h>
#include "clItemInfoWindow.h"
#include <OnlineFont.h>
#include <OnlineMegaText.h>


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Constructor
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
clItemInfoWindow::clItemInfoWindow()
{
	ZeroMemory(m_ItemInfoWindowData, sizeof(SItemInfoWindow) * MAXITEMINFOWINDOW);
	ZeroMemory(&m_ItemInfoWindowImage, sizeof(XSPR));
	m_lpGame = NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Destroyer
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
clItemInfoWindow::~clItemInfoWindow()
{
	Release();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Create
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL clItemInfoWindow::Create(_clGame* lpGame)
{
	ZeroMemory(m_ItemInfoWindowData, sizeof(SItemInfoWindow) * MAXITEMINFOWINDOW);
	m_lpGame = lpGame;

	clGrp.LoadXspr("Online\\GameSpr\\GetItem.Spr", m_ItemInfoWindowImage);

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Release
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
void clItemInfoWindow::Release(void)
{
	ZeroMemory(m_ItemInfoWindowData, sizeof(SItemInfoWindow) * MAXITEMINFOWINDOW);
	m_lpGame = NULL;

	if(m_ItemInfoWindowImage.Image)
	{
		clGrp.FreeXspr(m_ItemInfoWindowImage);
		ZeroMemory(&m_ItemInfoWindowImage, sizeof(XSPR));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Clear All Item Info Window
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
void clItemInfoWindow::ClearAllItemInfoWindow(void)
{
	ZeroMemory(m_ItemInfoWindowData, sizeof(SItemInfoWindow) * MAXITEMINFOWINDOW);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Add Item Info Window
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL clItemInfoWindow::AddItemInfoWindow(SI32 MaxFrame, SI32 x, SI32 y, UI32 ItemCode, char* lpTextData)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAXITEMINFOWINDOW; TempCount++)
	{
		if(m_ItemInfoWindowData[TempCount].m_Flag == FALSE)
		{
			ZeroMemory(&m_ItemInfoWindowData[TempCount], sizeof(SItemInfoWindow));
			
			m_ItemInfoWindowData[TempCount].m_Flag = TRUE;
			m_ItemInfoWindowData[TempCount].m_Type = 0;

			m_ItemInfoWindowData[TempCount].m_Frame    = 0;
			m_ItemInfoWindowData[TempCount].m_MaxFrame = MaxFrame;
			m_ItemInfoWindowData[TempCount].m_XPos     = x;
			m_ItemInfoWindowData[TempCount].m_YPos     = y;

			m_ItemInfoWindowData[TempCount].m_Code = ItemCode;
			strncpy((char*)m_ItemInfoWindowData[TempCount].m_TextData, lpTextData, 255);
			return TRUE;
		}
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Add ETC Info Window
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL clItemInfoWindow::AddETCInfoWindow(SI32 MaxFrame, SI32 x, SI32 y, UI32 ResourceCode, char* lpTextData)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAXITEMINFOWINDOW; TempCount++)
	{
		if(m_ItemInfoWindowData[TempCount].m_Flag == FALSE)
		{
			ZeroMemory(&m_ItemInfoWindowData[TempCount], sizeof(SItemInfoWindow));
			
			m_ItemInfoWindowData[TempCount].m_Flag = TRUE;
			m_ItemInfoWindowData[TempCount].m_Type = 1;

			m_ItemInfoWindowData[TempCount].m_Frame    = 0;
			m_ItemInfoWindowData[TempCount].m_MaxFrame = MaxFrame;
			m_ItemInfoWindowData[TempCount].m_XPos     = x;
			m_ItemInfoWindowData[TempCount].m_YPos     = y;

			m_ItemInfoWindowData[TempCount].m_Code = ResourceCode;
			strncpy((char*)m_ItemInfoWindowData[TempCount].m_TextData, lpTextData, 255);
			return TRUE;
		}
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Prepare
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL clItemInfoWindow::Prepare(void)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAXITEMINFOWINDOW; TempCount++)
	{
		if(m_ItemInfoWindowData[TempCount].m_Flag)
		{
			m_ItemInfoWindowData[TempCount].m_Frame++;
			if(m_ItemInfoWindowData[TempCount].m_Frame >= m_ItemInfoWindowData[TempCount].m_MaxFrame)
			{
				ZeroMemory(&m_ItemInfoWindowData[TempCount], sizeof(SItemInfoWindow));
			}
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Render
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL clItemInfoWindow::Render(LPDIRECTDRAWSURFACE7 lpSurface)
{
	SI32			TempCount;
	HDC				hDC;
	CItemHeader*	pItem;
	XSPR*			pImage;

	for(TempCount = 0; TempCount < MAXITEMINFOWINDOW; TempCount++)
	{
		if(m_ItemInfoWindowData[TempCount].m_Flag)
		{
			if(clGrp.LockSurface(lpSurface) == TRUE)
			{
				// Back Image
				clGrp.PutSpriteT(m_ItemInfoWindowData[TempCount].m_XPos, m_ItemInfoWindowData[TempCount].m_YPos, m_ItemInfoWindowImage.Header.Xsize, m_ItemInfoWindowImage.Header.Ysize, m_ItemInfoWindowImage.Image);

				// Item Image
				switch(m_ItemInfoWindowData[TempCount].m_Type)
				{
					case 0:
						if(m_ItemInfoWindowData[TempCount].m_Code)
						{
							pItem = m_lpGame->pOnlineWorld->pItemsInfo->GetItem(m_ItemInfoWindowData[TempCount].m_Code);

							if(pItem)
							{
								pImage = m_lpGame->pOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
								clGrp.PutSpriteT(m_ItemInfoWindowData[TempCount].m_XPos + 45 - (pImage->Header.Xsize / 2), m_ItemInfoWindowData[TempCount].m_YPos + 45 - (pImage->Header.Ysize / 2)
									, pImage->Header.Xsize, pImage->Header.Ysize
									, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
							}
						}
						break;

					case 1:
						if(m_ItemInfoWindowData[TempCount].m_Code)
						{
							pImage = m_lpGame->pOnlineWorld->pOnlineResource->GetxsprGame(m_ItemInfoWindowData[TempCount].m_Code);
							clGrp.PutSpriteT(m_ItemInfoWindowData[TempCount].m_XPos + 45 - (pImage->Header.Xsize / 2), m_ItemInfoWindowData[TempCount].m_YPos + 45 - (pImage->Header.Ysize / 2)
								, pImage->Header.Xsize, pImage->Header.Ysize
								, pImage->Image);
						}
						break;
				}

				clGrp.UnlockSurface(lpSurface);
			}
			if(lpSurface->GetDC(&hDC) == DD_OK)
			{
				SetBkMode(hDC, TRANSPARENT);
				SelectObject(hDC, m_lpGame->pOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

				// Text Data
				m_lpGame->pOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_ItemInfoWindowData[TempCount].m_XPos + 94, m_ItemInfoWindowData[TempCount].m_YPos + 24, 190, (char*)m_ItemInfoWindowData[TempCount].m_TextData);

				lpSurface->ReleaseDC(hDC);
			}
		}
	}

	return TRUE;
}

