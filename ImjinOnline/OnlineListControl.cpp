/********************************************************************************************
	파일명	:	OnlineListControl.cpp
	작성자	:	정 진 욱
	작성일	:	2001.05.31
	수정일	:	2002.06.04
	내  용  :	리스트 컨드롤
********************************************************************************************/
#include <GSL.h>

#include <Mouse.h>

#include "OnlineWorld.h"
#include "MakePalette.h"
#include "OnlineListControl.h"
#include "OnlineItem.h"
#include "OnlineMegaText.h"
#include "OnlinePortrait.h"
#include "OnlineMercenary.h"

extern _InputDevice				IpD;


// 한 칸의 크기와 총 수를 결정를 한다.
void OnlineListControl::Init(SI16 siWidth, SI16 siHeight, SI16 siXsize, SI16 siYsize, SI16 siXpos, SI16 siYpos, SI32 siType)
{
	m_siWidth = siWidth;		// 한 칸의 너비
	m_siHeight = siHeight;		// 한 칸의 높이
	m_siXnum = siXsize;			// 총 가로 칸수
	m_siYnum = siYsize;			// 총 세로 칸수
	m_siXpos = siXpos;			// 기준 X 좌표
	m_siYpos = siYpos;			// 기준 Y 좌표
	m_siType = siType;			// 무슨 용도 인가? 장비창 & 인벤
	m_bNowDrag = false; m_siDragSlot = -1;

	ItemInfo	TempSlot;		// 슬롯를 초기화 하기 위한 변수
	SI16		i, y;			// for문용

	// 슬롯을 초기화 한다.(m_siXnum*m_siYnum 수 만큼 초기화를 해준다.)
	for( i=0 ; i<(m_siXnum*m_siYnum); i++ )	m_SlotInfo.push_back(TempSlot);


	// 슬롯의 영역을 구분해 준다.
	RECT	rect;						// 임시로 사각영역을 잡기 위한 변수
	SI16	siTempLeft=0, siTempTop=0;	// 임시로 기준 좌표를 저장할 변수

	for( i=0 ; i<m_siXnum; i++ )
	{
		siTempLeft = m_siXpos + (i * m_siWidth);	// 임시로 left값을 구해 준다.
		siTempLeft += i * 4;

		for( y=0 ; y<m_siYnum; y++ )
		{
			siTempTop = m_siYpos + (y * m_siHeight);
			siTempTop += y * 4;
			SetRect(&rect, siTempLeft, siTempTop, siTempLeft + m_siWidth, siTempTop + m_siHeight);
			m_SlotRect.push_back(rect);
		}
	}
}

// 현재 선택한 Slot를 알아 낸다.
const SI16 OnlineListControl::GetSelectSlot() const
{
	POINT pt;

	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	for( SI16 i=0 ; i<(SI16)m_SlotRect.size(); i++ )
		if(PtInRect(&m_SlotRect[i], pt))	return i;	

	return -1;
}

// 그리기
void OnlineListControl::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(clGrp.LockSurface(pSurface))
	{
		CItemHeader *pItem;
		XSPR *pImage;

		for( SI16 i=0; i != (SI16)m_SlotInfo.size(); ++i )
		{
			if(m_siDragSlot != i )
			{
				if(m_SlotInfo[i].siType == LIST_FOLLOWER)
				{
					// actdoll (2004/01/02 22:10) : 초상 그리는 방법 변경
					m_pMyOnlineWorld->m_pPortrait->DrawAsNormalEx(pSurface, m_SlotRect[i].left, m_SlotRect[i].top, 
						m_SlotInfo[i].Follower.uiKind, TRUE, 60, 60);
				}
				else if(!m_SlotInfo[i].bHide && m_SlotInfo[i].uiItemID)
				{
					if(m_SlotInfo[i].siType == LIST_ITEM)
					{
						pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SlotInfo[i].uiItemID);

						if(pItem)
						{
							pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
							switch(m_SlotInfo[i].siColor)
							{
								case LIST_COLOR_NOT:
									clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
									break;

								case LIST_COLOR_RED:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed));
									break;

								case LIST_COLOR_PINK:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow));
									break;
							}					
						}
					}
					else if(m_SlotInfo[i].siType == LIST_PROPERTY)
					{
						pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SlotInfo[i].uiItemID);

						if(pItem)
						{
							pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
							switch(m_SlotInfo[i].siColor)
							{
								case LIST_COLOR_NOT:
									clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
									break;

								case LIST_COLOR_RED:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed));
									break;

								case LIST_COLOR_PINK:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow));
									break;
							}					
						}
					}
				}
			}
			//clGrp.Box(m_SlotRect[i].left, m_SlotRect[i].top, m_SlotRect[i].right, m_SlotRect[i].bottom, 255);
		}
		clGrp.UnlockSurface(pSurface);
	}

	HDC hDC;

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC,TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_ITEM));

		char szNumber[10];
		for( SI16 i=0; i != (SI16)m_SlotInfo.size(); ++i )
		{
			if(!m_SlotInfo[i].bHide && m_SlotInfo[i].uiItemID && m_siType == 0)
			{
				if(m_SlotInfo[i].siType == LIST_ITEM && m_siDragSlot != i)
				{
					sprintf(szNumber, "%d", m_SlotInfo[i].ItemCount);
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_SlotRect[i].right-1, m_SlotRect[i].bottom - 10, szNumber, RGB(250, 250, 250));
					
					// actdoll (2004/07/21 21:59) : 시간제 아이템일 경우 좌상단에 베이스 날자를 찍어준다.
					if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( m_SlotInfo[i].uiItemID ) )
					{
						sprintf( szNumber, "[%d]", m_pMyOnlineWorld->pItemsInfo->GetItemUsingDay( m_SlotInfo[i].uiItemID ) );
//						m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_SlotRect[i].left+2, m_SlotRect[i].top+2, szNumber, RGB(0, 0, 0) );
						m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_SlotRect[i].left+1, m_SlotRect[i].top+1, szNumber, RGB(0, 255, 0) );
					}
				}
				else if(m_SlotInfo[i].siType == LIST_FOLLOWER && m_siDragSlot != i)
				{
					SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
					sprintf(szNumber, "LV.%d", m_SlotInfo[i].Follower.siLevel);
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_SlotRect[i].right-1, m_SlotRect[i].bottom - 10, szNumber, RGB(250, 250, 250));
				}
			}
		}

		pSurface->ReleaseDC(hDC);
	}

	if(clGrp.LockSurface(pSurface))
	{
		CItemHeader *pItem;
		XSPR *pImage;
		
		if(m_siDragSlot != -1)
		{
			pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SlotInfo[m_siDragSlot].uiItemID);
			
			if(pItem)
			{
				pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
				switch(m_SlotInfo[m_siDragSlot].siColor)
				{
				case LIST_COLOR_NOT:
					clGrp.PutSpriteT(IpD.Mouse_X - (pImage->Header.Xsize/2), IpD.Mouse_Y - (pImage->Header.Ysize/2), pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
					break;
					
				case LIST_COLOR_RED:
					if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed))
						clGrp.PutSpriteT(IpD.Mouse_X - (pImage->Header.Xsize/2), IpD.Mouse_Y - (pImage->Header.Ysize/2), pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
					break;
					
				case LIST_COLOR_PINK:
					if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow))
						clGrp.PutSpriteT(IpD.Mouse_X - (pImage->Header.Xsize/2), IpD.Mouse_Y - (pImage->Header.Ysize/2), pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
					break;
				}
			}				
		}
		
		clGrp.UnlockSurface(pSurface);
	}	

	
}

// 그리기, 이미지(psprOverImage)파일 중에 폰트번호(siFontNumber)에 해당되는 그림을 덧그려준다. 소집권 아이템을 위함.
void OnlineListControl::Draw(LPDIRECTDRAWSURFACE7 pSurface, XSPR *psprOverImage, SI32 siFontNumber/* = 0*/)
{
	if(clGrp.LockSurface(pSurface))
	{
		CItemHeader *pItem;
		XSPR *pImage;

		for( SI16 i=0; i != (SI16)m_SlotInfo.size(); ++i )
		{
			if(m_siDragSlot != i )
			{
				if(m_SlotInfo[i].siType == LIST_FOLLOWER)
				{
					// actdoll (2004/01/02 22:10) : 초상 그리는 방법 변경
					m_pMyOnlineWorld->m_pPortrait->DrawAsNormalEx(pSurface, m_SlotRect[i].left, m_SlotRect[i].top, 
						m_SlotInfo[i].Follower.uiKind, TRUE, 60, 60);
				}
				else if(!m_SlotInfo[i].bHide && m_SlotInfo[i].uiItemID)
				{
					if(m_SlotInfo[i].siType == LIST_ITEM)
					{
						pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SlotInfo[i].uiItemID);

						if(pItem)
						{
							pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
							switch(m_SlotInfo[i].siColor)
							{
								case LIST_COLOR_NOT:
									clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
									break;

								case LIST_COLOR_RED:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed));
									break;

								case LIST_COLOR_PINK:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow));
									break;
							}
							
							// 소집권 아이템이며,
							if (pItem->m_pImperium)
							{
								// 덧그려야할 이미지가 있다면,
								if (psprOverImage)
								{
									// 반 투명으로 그려준다.
									clGrp.PutSpriteLightT(m_SlotRect[i].left, m_SlotRect[i].top, psprOverImage->Header.Xsize, psprOverImage->Header.Ysize, &psprOverImage->Image[psprOverImage->Header.Start[siFontNumber]]);
								}
							}
						}
					}
					else if(m_SlotInfo[i].siType == LIST_PROPERTY)
					{
						pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SlotInfo[i].uiItemID);

						if(pItem)
						{
							pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
							switch(m_SlotInfo[i].siColor)
							{
								case LIST_COLOR_NOT:
									clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
									break;

								case LIST_COLOR_RED:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed));
									break;

								case LIST_COLOR_PINK:
									if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow))
										clGrp.PutSpriteT(m_SlotRect[i].left, m_SlotRect[i].top, pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]], m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow));
									break;
							}					
						}
					}
				}
			}
			//clGrp.Box(m_SlotRect[i].left, m_SlotRect[i].top, m_SlotRect[i].right, m_SlotRect[i].bottom, 255);
		}
		clGrp.UnlockSurface(pSurface);
	}

	HDC hDC;

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC,TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_ITEM));

		char szNumber[10];
		for( SI16 i=0; i != (SI16)m_SlotInfo.size(); ++i )
		{
			if(!m_SlotInfo[i].bHide && m_SlotInfo[i].uiItemID && m_siType == 0)
			{
				if(m_SlotInfo[i].siType == LIST_ITEM && m_siDragSlot != i)
				{
					sprintf(szNumber, "%d", m_SlotInfo[i].ItemCount);
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_SlotRect[i].right-1, m_SlotRect[i].bottom - 10, szNumber, RGB(250, 250, 250));
					
					// actdoll (2004/07/21 21:59) : 시간제 아이템일 경우 좌상단에 베이스 날자를 찍어준다.
					if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( m_SlotInfo[i].uiItemID ) )
					{
						sprintf( szNumber, "[%d]", m_pMyOnlineWorld->pItemsInfo->GetItemUsingDay( m_SlotInfo[i].uiItemID ) );
//						m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_SlotRect[i].left+2, m_SlotRect[i].top+2, szNumber, RGB(0, 0, 0) );
						m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_SlotRect[i].left+1, m_SlotRect[i].top+1, szNumber, RGB(0, 255, 0) );
					}
				}
				else if(m_SlotInfo[i].siType == LIST_FOLLOWER && m_siDragSlot != i)
				{
					SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
					sprintf(szNumber, "LV.%d", m_SlotInfo[i].Follower.siLevel);
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_SlotRect[i].right-1, m_SlotRect[i].bottom - 10, szNumber, RGB(250, 250, 250));
				}
			}
		}

		pSurface->ReleaseDC(hDC);
	}

	if(clGrp.LockSurface(pSurface))
	{
		CItemHeader *pItem;
		XSPR *pImage;
		
		if(m_siDragSlot != -1)
		{
			pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SlotInfo[m_siDragSlot].uiItemID);
			
			if(pItem)
			{
				pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
				switch(m_SlotInfo[m_siDragSlot].siColor)
				{
				case LIST_COLOR_NOT:
					clGrp.PutSpriteT(IpD.Mouse_X - (pImage->Header.Xsize/2), IpD.Mouse_Y - (pImage->Header.Ysize/2), pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
					break;
					
				case LIST_COLOR_RED:
					if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataRed))
						clGrp.PutSpriteT(IpD.Mouse_X - (pImage->Header.Xsize/2), IpD.Mouse_Y - (pImage->Header.Ysize/2), pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
					break;
					
				case LIST_COLOR_PINK:
					if(m_pMyOnlineWorld->pPaletteManager->GetPalette(m_pMyOnlineWorld->PaletteDataYellow))
						clGrp.PutSpriteT(IpD.Mouse_X - (pImage->Header.Xsize/2), IpD.Mouse_Y - (pImage->Header.Ysize/2), pImage->Header.Xsize, pImage->Header.Ysize, &pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
					break;
				}
			}				
		}
		
		clGrp.UnlockSurface(pSurface);
	}	
}

bool OnlineListControl::DragUp()
{
	// 우선 무슨 슬롯을 선택을 했는지 알아 본다.
	const SI16 siSelectSolt = GetSelectSlot();	
	if( siSelectSolt == -1 )				return false;
	if( IsEmptySlot(siSelectSolt) == -1 )	return false;
	
	if(m_SlotInfo[siSelectSolt].siType == LIST_FOLLOWER)	return false;
	m_bNowDrag = true;
	m_siDragSlot = siSelectSolt;
	return true;
}

SI16 OnlineListControl::DragDown()	
{
	if( !m_bNowDrag )			return -1;
	if( m_siDragSlot == -1 )	return -1;

	const SI16 siSelectSlot = GetSelectSlot();	

	m_siOldDragSlot = m_siDragSlot;
	if( siSelectSlot != -1 )
		IsEmptySlot(siSelectSlot) == -1 ? AddDragItem(siSelectSlot) : -1;

	StopDrag();
	return siSelectSlot;
}

void OnlineListControl::SetPos(SI16 siX,SI16 siY)	
{

	int i,y;

	m_siXpos = siX;			// 기준 X 좌표
	m_siYpos = siY;			// 기준 Y 좌표
	

	// 슬롯의 영역을 구분해 준다.

	m_SlotRect.clear();

	RECT	rect;					// 임시로 사각영역을 잡기 위한 변수
	SI16	siTempLeft, siTempTop;	// 임시로 기준 좌표를 저장할 변수

	for( i=0 ; i<m_siXnum; i++ )
	{
		siTempLeft = m_siXpos + (i * m_siWidth);	// 임시로 left값을 구해 준다.
		siTempLeft += i * 4;

		for( y=0 ; y<m_siYnum; y++ )
		{
			siTempTop = m_siYpos + (y * m_siHeight);
			siTempTop += y * 4;
			SetRect(&rect, siTempLeft, siTempTop, siTempLeft + m_siWidth, siTempTop + m_siHeight);
			m_SlotRect.push_back(rect);
		}
	}

	
}

const ItemInfo* OnlineListControl::GetSlotInfo(const SI16 siSlot) const
{
	if(siSlot < 0)	return NULL;

	if(m_SlotInfo[siSlot].siType == LIST_FOLLOWER)
		return (const ItemInfo*)&m_SlotInfo[siSlot];
	else
	{
		if(m_SlotInfo[siSlot].uiItemID)	return (const ItemInfo*)&m_SlotInfo[siSlot];
		else							return NULL;
	}
}

SI16 OnlineListControl::IsEmptySlot(const SI16 siSlot) const
{ 
	if(m_SlotInfo[siSlot].siType == LIST_ITEM)
	{
		if(m_SlotInfo[siSlot].uiItemID)						return siSlot;
		else												return -1;
	}
	else if(m_SlotInfo[siSlot].siType == LIST_FOLLOWER)		return siSlot;
	else													return -1;
}