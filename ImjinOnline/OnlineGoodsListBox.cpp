#include <Main.h>
#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineGoodsListBox.h"

extern _InputDevice			IpD;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ʈ �ڽ��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GoodsListBox::Create(SHORT x, SHORT y, SHORT width, SHORT height, 
							SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight, 
							XSPR *pBorder, XSPR *pHandle, SI16 siHandleAxis, SI16 siHandleTop, SI16 siHandleBottom, cltOnlineWorld *pOnlineWorld)
{
	_YListBox::Create(x, y, width, height, 
					textx, texty, textwidth, textheight, SellWidth, SellHeight,
					0, 0, 0, 0, 0);
	
	siBuySellGoods			=	-1;
	pBorderXspr				=	pBorder;
	pHandleXspr				=	pHandle;
	
	m_rcBarInfo.left		=	siHandleAxis - (pHandle->Header.Xsize / 2);
	m_rcBarInfo.top			=	siHandleTop;
	m_rcBarInfo.right		=	siHandleAxis + (pHandle->Header.Xsize / 2);
	m_rcBarInfo.bottom		=	siHandleBottom; 

	// ó������ siHandleBottom�� ����.
	siHandleY				=	siHandleTop;
	pMyOnlineWorld			=	pOnlineWorld;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	����Ʈ �ڽ��� ����ϴ� �Լ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::Draw(LPDIRECTDRAWSURFACE7	pSurface, CItem *pItemList)
{
	SI16			i;
	SI16			siItemNum;
	SI16			siVisibleSellNum;
	SI16			siStartSellNumber;
	SI16			siX, siY;

	if(pBorderXspr != NULL)	
		clGrp.PutSpriteCT(m_x, m_y, pBorderXspr->Header.Xsize, pBorderXspr->Header.Ysize, &pBorderXspr->Image[pBorderXspr->Header.Start[0]]);	

	siItemNum				= _YListBox::GetSellNum();		
	siVisibleSellNum		= _YListBox::GetVisibleSellNum();
	siStartSellNumber		= _YListBox::GetStartSellNumber();		

	for(i=0; ((i+siStartSellNumber) < siItemNum) && (i < siVisibleSellNum); i++)					
	{			 
		siX = m_PutTextX ;
		siY = m_PutTextY + i * GetSellHeight() ;

		Buttons[i + siStartSellNumber].Put(siX, siY, pSurface);				
	}			

	DrawScrollBar(pSurface);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	����Ʈ �ڽ��� ��ũ�ѹٸ� ����ϴ� �Լ�. 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::DrawScrollBar(LPDIRECTDRAWSURFACE7 pSurface)
{		
	if(pHandleXspr != NULL)
	{
		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			clGrp.PutSpriteCT(m_rcBarInfo.left, siHandleY, pHandleXspr->Header.Xsize, pHandleXspr->Header.Ysize, &pHandleXspr->Image[pHandleXspr->Header.Start[0]]);
			clGrp.UnlockSurface(pSurface);
		}
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ����Ʈ �ڽ��� �߰��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SHORT	GoodsListBox::AddSell(CItemHeader *pItem, MONEY uiPrice, UI16 uiQuantity)
{
	CHAR	szTextBuffer[1024];
	SI32	siItemNum;

	siItemNum	=	_YListBox::GetSellNum();			

	sprintf(szTextBuffer, "[%s] [%dKg] [%I64d��]", pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName), pItem->m_uiWeight, uiPrice, uiQuantity);
	Buttons[siItemNum].Create(GetSellWidth(), GetSellHeight(), szTextBuffer, pItem, uiPrice, uiQuantity, pMyOnlineWorld);
	
	if(_YListBox::AddSell(pItem, szTextBuffer) != -1)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ����Ʈ �ڽ����� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::DelSell(SHORT SellIndex)
{	
	SI32	siItemNum;
	SI32	i;

	siItemNum	=	_YListBox::GetSellNum();		

	if(IsValidIndex(SellIndex)==TRUE)
	{	
		// siIndex ���� �� ĭ�� ������ ����.		
		for(i = SellIndex; i < (m_SellNum - 1); i++)
		{
			Buttons[i].pItem		=	Buttons[i + 1].pItem;
			Buttons[i].uiPrice		=	Buttons[i + 1].uiPrice;
			Buttons[i].uiQuantity	=	Buttons[i + 1].uiQuantity;		
		}		
	}	
	
	_YListBox::DelSell(SellIndex);	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�� �������� �׼��� ����
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::Action(BOOL bMousePushStatus)
{	
	SI16	i;
	SI32	siItemNum;
	SI16	siVisibleSellNum;
	SI16	siStartSellNumber;	
	SI16	siX, siY;
	BOOL	IsDoubleClicking;
	POINT	pt;
	SI32	siTotalBarHeight;
	SI32	siCurrentHandleYPos;	

	if(m_bAction != FALSE && m_bProcess != FALSE)
	{
		siItemNum				= _YListBox::GetSellNum();
		siVisibleSellNum		= _YListBox::GetVisibleSellNum();
		siStartSellNumber		= _YListBox::GetStartSellNumber();	
			
		for(i = 0; ((i + siStartSellNumber) < siItemNum) && (i < siVisibleSellNum); i++)
		{
			siX = m_PutTextX;
			siY = m_PutTextY + i * GetSellHeight();

			if(Buttons[i + siStartSellNumber].Process(siX, siY, bMousePushStatus, &IsDoubleClicking)==TRUE)
				SetBuySellGoods(i + siStartSellNumber);				
		}		

		pt.x	=	IpD.Mouse_X;
		pt.y	=	IpD.Mouse_Y;		
		if((bMousePushStatus == TRUE) && (PtInRect(&m_rcBarInfo, pt) == TRUE))	
		{
			siHandleY			=	min(max(IpD.Mouse_Y - (pHandleXspr->Header.Ysize / 2), m_rcBarInfo.top), m_rcBarInfo.bottom - pHandleXspr->Header.Ysize);
			siTotalBarHeight	=	m_rcBarInfo.bottom - m_rcBarInfo.top;
			siCurrentHandleYPos	=	siHandleY - m_rcBarInfo.top;

			// �����ϴ� SellNumber�� ������ ���Ѵ�.
			m_StartSellNumber	=	max((siCurrentHandleYPos * (siItemNum - (siVisibleSellNum - 1))) / siTotalBarHeight, 0);			
		}
	}	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���ο� �������� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::SetPosition(SI16 siNewPosition)
{
	if(IsValidIndex(_YListBox::GetCurrentPosition())==TRUE)
		Buttons[_YListBox::GetCurrentPosition()].SetSelected(FALSE);

	_YListBox::SetPosition(siNewPosition);

	if(IsValidIndex(_YListBox::GetCurrentPosition())==TRUE)
		Buttons[_YListBox::GetCurrentPosition()].SetSelected(TRUE);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ID�� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	GoodsListBox::GetID(SI16 siSellIndex)
{
	return Buttons[siSellIndex].pItem->m_uiID;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�����ۿ��� ������ ���ҽ�Ų��.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::DecreaseQuantity(UI16 uiID, UI16 uiQuantity)
{
	SI32	i;
	SI32	siItemNum;

	siItemNum			= _YListBox::GetSellNum();

	for(i = 0; i < siItemNum; i++)
	{
		if(Buttons[i].pItem->m_uiID == uiID)
		{
			Buttons[i].uiQuantity	-=	uiQuantity;
			
			if(Buttons[i].uiQuantity == 0)
			{
				// �ش� �׸��� ���ش�.
				DelSell(i);
			}

			break;
		}	
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ID�� ����Ͽ� �ش� �������� ����, ������ �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsListBox::Modify(UI16 uiID, UI16 uiQuantity, MONEY uiPrice)
{
	SI32	i;
	SI32	siItemNum;

	siItemNum			= _YListBox::GetSellNum();

	for(i = 0; i < siItemNum; i++)
	{
		if(Buttons[i].pItem->m_uiID == uiID)
		{
			Buttons[i].uiQuantity	=	uiQuantity;		
			Buttons[i].uiPrice		=	uiPrice;			
		}	
	}		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����Ʈ �ڽ��� ó������ �ʴ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GoodsListBox::SetProcess(BOOL bProcess)
{
	SI32	i;
	SI32	siItemNum;

	siItemNum			= _YListBox::GetSellNum();

	for(i = 0; i < siItemNum; i++)	
		Buttons[i].SetProcess(bProcess);			

	_YListBox::SetProcess(bProcess);	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����Ʈ �ڽ��� ó������ �ʴ´�.(�ð������ε� ��Ÿ����.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GoodsListBox::SetAction(BOOL bAction)
{
	SI32	i;
	SI32	siItemNum;

	siItemNum		= _YListBox::GetSellNum();

	for(i = 0; i < siItemNum; i++)	
		Buttons[i].SetAction(bAction);			

	_YListBox::SetAction(bAction);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����Ʈ �ڽ��� �������� Ŭ���ؼ� ��ų� �ȷ��� �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GoodsListBox::SetBuySellGoods(SI32 siNewBuySellGoods)
{
	siBuySellGoods	=	siNewBuySellGoods;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����Ʈ �ڽ��� �������� Ŭ���ؼ� ��ų� �ȷ��� �ϴ��� �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	GoodsListBox::GetBuySellGoods()
{
	SI32	siTempBuySellGoods;

	siTempBuySellGoods		=	siBuySellGoods;	
	siBuySellGoods			=	-1;

	return siTempBuySellGoods;
}



