#include <GSL.h>

#include "OnlineBaseLB.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	리스트 박스를 생성한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineSelectLB::Create(SHORT x, SHORT y, SHORT width, SHORT height, 
							SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight, 
							XSPR *pBorder, XSPR *pHandle, SI16 siHandleAxis, SI16 siHandleTop, SI16 siHandleBottom,BOOL bUpDnBttn)
{
	// 기본 베이스 클래스도 생성을 해준다.
	_YListBox::Create(x, y, width, height, textx, texty, textwidth, textheight, SellWidth, SellHeight, 0, 0, 0, 0, 0);

	pBorderXspr	=	pBorder;
	pHandleXspr	=	pHandle;



	m_rcBarInfo.left		=	siHandleAxis - (pHandle != NULL ? (pHandle->Header.Xsize / 2) : 0);
	m_rcBarInfo.top			=	siHandleTop;
	m_rcBarInfo.right		=	siHandleAxis + (pHandle != NULL ? (pHandle->Header.Xsize / 2) : 0);
	m_rcBarInfo.bottom		=	siHandleBottom; 

	rcDragArea.left			=	m_rcBarInfo.left - 15;
	rcDragArea.top			=	m_rcBarInfo.top;
	rcDragArea.right		=	m_rcBarInfo.right + 15;
	rcDragArea.bottom		=	m_rcBarInfo.bottom;


	// 처음에는 siHandleBottom과 같다.
	siHandleY				=	siHandleTop;

	m_bUpDnBttn				=   bUpDnBttn;

	ZeroMemory(fDisable, sizeof(UI08) * MAX_LIST_BOX_SELL);
	
	// actdoll (2004/01/13 14:17) : 색상 박스를 전부 0으로 초기화
	ZeroMemory( m_Color, sizeof(COLORREF) * MAX_LIST_BOX_SELL );
	m_bDrawItemColor		= false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	인자로 넘어온 폰트를 사용하여 텍스트를 출력하며 리스트 박스를 그린다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineSelectLB::Draw(LPDIRECTDRAWSURFACE7 pSurface, HGDIOBJ hFont, COLORREF Color, COLORREF DisableColor, BOOL fSelectColor, COLORREF SelectColor, COLORREF SelectSellColor)
{
	SI16			i;
	SI16			siItemNum;
	SI16			siVisibleSellNum;
	SI16			siX, siY;
	SI16			SelectSell;
	HDC				hdc;

	if(fSelectColor == FALSE)
	{
		SelectColor     = Color;
		SelectSellColor = Color;
	}
	if(pBorderXspr != NULL)	
	{
		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			clGrp.PutSpriteT(m_x, m_y, pBorderXspr->Header.Xsize, pBorderXspr->Header.Ysize, &pBorderXspr->Image[pBorderXspr->Header.Start[0]]);	
			clGrp.UnlockSurface(pSurface);
		}
	}

	siItemNum				= _YListBox::GetSellNum();		
	siVisibleSellNum		= _YListBox::GetVisibleSellNum();
	SelectSell				= GetCurrentPosition();

	//리스트 박스 안에 있는 텍스트를 출력한다.
	if(pSurface->GetDC(&hdc) == DD_OK)	
	{
		SetBkMode(hdc, TRANSPARENT);

		if(hFont != NULL) 
			SelectObject(hdc, hFont);

		for(i=0; ((i+m_StartSellNumber) < m_SellNum) && (i < m_VisibleSellNum); i++)					
		{				
			siX = m_PutTextX ;
			siY = m_PutTextY + i * GetSellHeight() ;

			// 선택한 셀이라면
			if(SelectSell == (i + m_StartSellNumber))
			{
				SetTextColor(hdc, SelectColor);
			}
			// 일반 셀이라면
			else
			{
				// 비활성 셀이라면
				if(fDisable[i + m_StartSellNumber])
				{
					SetTextColor(hdc, DisableColor);
				}
				else
				{
					// 마우스 위치가 셀 위로 올라와 있는 상태라면
					if((siNowMouseX >= siX) && (siNowMouseX <= siX + _YListBox::GetSellWidth()) && (siNowMouseY >= siY) && (siNowMouseY <= siY + _YListBox::GetSellHeight()))			
					{
						SetTextColor(hdc, SelectSellColor);
					}
					// 평범한 상태라면
					else
					{
						// 각 아이템별 개별 컬러로 찍으라면
						if( m_bDrawItemColor )	
						{
							if( m_Color[i + m_StartSellNumber] == 0 )	m_Color[i + m_StartSellNumber] = Color;// 셀 값이 0일 경우는 기본값으로 찍으라는 말이다.
							SetTextColor(hdc, m_Color[i + m_StartSellNumber]);
						}
						// 그냥 찍으라면
						else
						{
							SetTextColor(hdc, Color);
						}
							
					}
				}
			}
				
			TextOut(hdc, siX, siY , m_lplpSellName[i + m_StartSellNumber], strlen(m_lplpSellName[i + m_StartSellNumber]));				
		}
		pSurface->ReleaseDC(hdc);
	}

	// 스크롤바를 그린다.
	DrawScrollBar(pSurface);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	리스트 박스의 스크롤바를 출력하는 함수. 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineSelectLB::DrawScrollBar(LPDIRECTDRAWSURFACE7 pSurface)
{		
	if(pHandleXspr != NULL)
	{
		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			clGrp.PutSpriteT(m_rcBarInfo.left, siHandleY, pHandleXspr->Header.Xsize, pHandleXspr->Header.Ysize, &pHandleXspr->Image[pHandleXspr->Header.Start[0]]);
			clGrp.UnlockSurface(pSurface);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명		각 아이템의 액션을 수행
//----------:----------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineSelectLB::Action(SI16 siMouseX, SI16 siMouseY, BOOL bMousePushStatus, SI32 *psiSelectSell, BOOL bSelect)
{	
	SI16	i;
	SI32	siItemNum;
	SI16	siVisibleSellNum;
	SI16	siStartSellNumber;	
	SI16	siX, siY;
	POINT	pt;
	SI32	siTotalBarHeight;
	SI32	siCurrentHandleYPos;

	*psiSelectSell	=	-1;
	siNowMouseX     = siMouseX;
	siNowMouseY     = siMouseY;	


	//if(pHandleXspr != NULL)

	if(m_bUpDnBttn == TRUE  && (GetSellNum() > GetVisibleSellNum()) )
		siHandleY = m_rcBarInfo.top + this->GetStartSellNumber() * ((m_rcBarInfo.bottom - m_rcBarInfo.top - pHandleXspr->Header.Ysize) / (GetSellNum() - GetVisibleSellNum()) );
//	else
//		siHandleY = m_rcBarInfo.top;

	if(m_bAction != FALSE && m_bProcess != FALSE && bMousePushStatus == TRUE)
	{
		siItemNum				= _YListBox::GetSellNum();
		siVisibleSellNum		= _YListBox::GetVisibleSellNum();
		siStartSellNumber		= _YListBox::GetStartSellNumber();				
		
		for(i = 0; ((i + siStartSellNumber) < siItemNum) && (i < siVisibleSellNum); i++)
		{
			if(fDisable[i + siStartSellNumber] == FALSE)
			{
				siX = m_PutTextX;
				siY = m_PutTextY + i * GetSellHeight();

				if(bSelect)
				{
					if((siMouseX >= siX) && (siMouseX <= siX + _YListBox::GetSellWidth()) && (siMouseY >= siY) && (siMouseY <= siY + _YListBox::GetSellHeight()))			
					{
						SetPosition(i + siStartSellNumber);			
						*psiSelectSell	=	i + siStartSellNumber;
					}
				}
			}
		}		

		pt.x	=	siMouseX;
		pt.y	=	siMouseY;		

		if(PtInRect(&m_rcBarInfo, pt) == TRUE || (PtInRect(&rcDragArea, pt) == TRUE && (bDrag == TRUE)))
		{
			if( siItemNum > siVisibleSellNum )
			{
//				if(pHandleXspr != NULL)
					siHandleY = min(max(siMouseY - (pHandleXspr->Header.Ysize / 2), m_rcBarInfo.top), m_rcBarInfo.bottom - pHandleXspr->Header.Ysize);
//				else
//					siHandleY = m_rcBarInfo.top;
				//siHandleY			=	min(max(siMouseY - (pHandleXspr->Header.Ysize / 2), m_rcBarInfo.top), m_rcBarInfo.bottom - pHandleXspr->Header.Ysize);

				siTotalBarHeight	=	m_rcBarInfo.bottom - m_rcBarInfo.top - pHandleXspr->Header.Ysize;
				siCurrentHandleYPos	=	siHandleY - m_rcBarInfo.top;

				// 시작하는 SellNumber를 새로이 구한다.
	//			m_StartSellNumber	=	max((siCurrentHandleYPos * (siItemNum - (siVisibleSellNum - 1))) / siTotalBarHeight, 0);
				m_StartSellNumber	=	siCurrentHandleYPos / ( siTotalBarHeight / ( ( siItemNum - siVisibleSellNum ) ) );
				bDrag				=	TRUE;
			}
		}				
	}	
	else
	{
		bDrag	=	FALSE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	특정 Item만 Disable함
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineSelectLB::SetItemDisable(SI32 ItemIndex, BOOL Disable)
{
	fDisable[ItemIndex] = Disable;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	특정 Item에 대한 색깔 찍기시 색깔을 지정해준다.(기본값 흰색)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineSelectLB::SetItemColor( SI32 ItemIndex, COLORREF color )
{
	m_Color[ItemIndex] = color;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	특정 Item에 대한 색깔 찍기를 모두 클린시킨다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineSelectLB::ClearItemColor()
{
	ZeroMemory( m_Color, sizeof(COLORREF) * MAX_LIST_BOX_SELL );
}
