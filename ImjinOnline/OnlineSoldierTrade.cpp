#include <GSL.h>

#include "OnlineWorld.h"
#include "OnlineSoldierTrade.h"
#include "charanimation.h"
#include <kindinfo.h>
#include "DirectX.h"
#include "myfile.h"
#include <mouse.h>

#include "OnlineListControl.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineMyData.h"
#include "OnlineHelp.h"
#include "OnlineInventory.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlinePortrait.h"


extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 

OnlineSoldierTrade::OnlineSoldierTrade(cltOnlineWorld *pOnline, char *pImageFileName)
{
	_pMyWorld = pOnline;
	clGrp.LoadXspr(pImageFileName, _Image);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr", m_But1Image);

	// 출력 좌표를 얻어 온다.
	
	_nX = 100;
	_nY = 45;

	_nSoldierImageFont = -1;
//	m_bAction		   = FALSE;

	// 장착용 아이템을 넣을 컨트롤 생성
	_pSwordLCEx		= new OnlineListControl(pOnline);	
	_pArmorLCEx		= new OnlineListControl(pOnline);	
	_pRing1LCEx		= new OnlineListControl(pOnline);	
	_pRing2LCEx		= new OnlineListControl(pOnline);	
	_pHelmetLCEx	= new OnlineListControl(pOnline);	
	_pBeltLCEx		= new OnlineListControl(pOnline);		
	_pGlovesLCEx	= new OnlineListControl(pOnline);	
	_pBootsLCEx		= new OnlineListControl(pOnline);
	_pGuardianEx	= new OnlineListControl(pOnline);

	// 장착용 리스트 컨트롤을 생성한다.	
	_pGuardianEx->Init( 60, 60, 1, 1, _nX + 121, _nY + 35, ON_ITEM_GUARDIAN );
	_pHelmetLCEx->Init( 60, 60, 1, 1, _nX + 185, _nY + 35, ON_ITEM_HELMAT );					// 헬멧
	_pGlovesLCEx->Init( 60, 60, 1, 1, _nX + 249, _nY + 35, ON_ITEM_GLOVES );					// 장갑

	_pSwordLCEx->Init ( 60, 60, 1, 1, _nX + 121, _nY + 99, ON_ITEM_SWORD );					// 무기
	_pArmorLCEx->Init ( 60, 60, 1, 1, _nX + 185, _nY + 99, ON_ITEM_ARMOR | ON_ITEM_DRESS);		// 갑옷
	_pBeltLCEx->Init  ( 60, 60, 1, 1, _nX + 249, _nY + 99, ON_ITEM_BELT );						// 벨트	

	_pRing1LCEx->Init ( 60, 60, 1, 1, _nX + 121, _nY + 163, ON_ITEM_RING | ON_ITEM_LRING );		// 악세사리
	_pBootsLCEx->Init ( 60, 60, 1, 1, _nX + 185, _nY + 163, ON_ITEM_BOOTS );					// 부츠
	_pRing2LCEx->Init ( 60, 60, 1, 1, _nX + 249, _nY + 163, ON_ITEM_RING | ON_ITEM_RRING );		// 악세사리


	// 인벤토리
	_pItemLC	= new OnlineListControl(pOnline);
	_pItemLC->Init(60, 60, 5, 2, _nX + 10, _nY + 235, 0);



	_BtExit.Create( _nX+139, _nY+394, m_But1Image.Header.Xsize, m_But1Image.Header.Ysize, _pMyWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT);


	
	_ListControl.push_back(_pItemLC);
	_ListControl.push_back(_pHelmetLCEx);
	_ListControl.push_back(_pSwordLCEx);
	_ListControl.push_back(_pBeltLCEx);
	_ListControl.push_back(_pArmorLCEx);
	_ListControl.push_back(_pGlovesLCEx);
	_ListControl.push_back(_pBootsLCEx);	
	_ListControl.push_back(_pRing1LCEx);
	_ListControl.push_back(_pRing2LCEx);
	_ListControl.push_back(_pGuardianEx);


	_bMouseDown = FALSE;
}

OnlineSoldierTrade::~OnlineSoldierTrade()
{
	clGrp.FreeXspr( _Image );
	clGrp.FreeXspr( m_But1Image );

	for(vector<OnlineListControl*>::iterator i=_ListControl.begin(); i!=_ListControl.end(); ++i)
	{
		if(*i)
		{
			(*i)->Clear();
			delete (*i);
		}
	}
	
	_ListControl.clear();
}

void OnlineSoldierTrade::Actoin()
{
//	if(m_bAction == FALSE) return;

	if( _nSoldierImageFont == -1 )	return;

	if(_BtExit.Process(_bMouseDown))
	{
//		m_bAction = FALSE;
		_nSoldierImageFont = -1;
		ZeroMemory(&_SoldierInfo, sizeof(_SoldierInfo));

		for(vector<OnlineListControl*>::iterator i=_ListControl.begin(); i!=_ListControl.end(); ++i)
			(*i)->Clear();
		
		if(_pMyWorld->pOnlineTrade->IsActive())
		{
			_pMyWorld->pOnlineTrade->SetStatus(1);
			_pMyWorld->pOnlineTrade->SetMouseStats(FALSE);
		}
		if(_pMyWorld->pOnlineBooth->IsActionOut() && (_pMyWorld->pOnlineBooth->GetStatusOut() == ON_BENCH_OUT_SOLDIER_INFO))
		{
			_pMyWorld->pOnlineBooth->SetStatusOut(ON_BENCH_OUT_BUY_FOLLOWER);
		}
		
	}

	int siIndex;

	CItemHeader		*pItem;
	const ItemInfo	*pItemSlot;

	for(vector<OnlineListControl*>::iterator i=_ListControl.begin(); i!=_ListControl.end(); ++i)
	{
		if((siIndex = (*i)->GetSelectSlot()) != -1)
		{
			pItemSlot	= (*i)->GetSlotInfo((const)siIndex);
			pItem		= pItemSlot == NULL ? NULL : _pMyWorld->pItemsInfo->GetItem(pItemSlot->uiItemID);
			
			if(!pItem)				continue;			

			_pMyWorld->pOnlineHelp->SetTextItem(pItem, const_cast<ItemInfo*>(pItemSlot), (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y, ON_HELP_DOWN, FALSE, FALSE);			
		}
	}

	_bMouseDown = IpD.LeftPressSwitch;
}


void OnlineSoldierTrade::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
//	if(m_bAction == FALSE) return;

	if( _nSoldierImageFont == -1 )	return;

	if( clGrp.LockSurface(pSurface) )
	{
		clGrp.PutSprite8T(_nX, _nY, _Image.Header.Xsize, _Image.Header.Ysize, _Image.Image);	// 보더

		SI32 siDrawFlag = 0;
		if( _SoldierInfo.siLife <= 0 )		siDrawFlag = DF_FLAG_PORTRAIT_HALFALPHA;	// 죽어있다면 반투명으로 그려라
		_pMyWorld->m_pPortrait->DrawAsNormal( _nX + 25, _nY + 35, _SoldierInfo.uiKind, true, siDrawFlag);

		_BtExit.Put(&m_But1Image, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	HDC hDC;
	if(pSurface->GetDC(&hDC) == DD_OK)
	{	
		char szTemp[128];

		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, _pMyWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );
		
		// 이름
		if(_SoldierInfo.Name[10] != NULL)
			_SoldierInfo.Name[10] = NULL;
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+35, _nY+129, 60, 13, _SoldierInfo.Name);
		//_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+163, _nY+12, , RGB( 216, 201, 183 ));

		// 
		sprintf(szTemp, "%s : %d", _pMyWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_LV ),_SoldierInfo.siLevel);		
		//_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+25, _nY+146, 36, 12, , RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+25, _nY+146, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%s:%d", _pMyWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_EXP ),_SoldierInfo.siExp);
		//_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+124, _nY+29, 36, 12, , RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+25, _nY+160, szTemp, RGB(216, 201, 183));
/*
		sprintf(szTemp, "%d", _SoldierInfo.siLife);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+124, _nY+46, 36, 12, _pMyWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_LIFE ), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+163, _nY+46, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", _SoldierInfo.siMana);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+232, _nY+46, 36, 12, _pMyWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_MANA ), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+272, _nY+46, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", _SoldierInfo.siStr);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+124, _nY+63, 46, 12, _pMyWorld->pOnlineText->Get(6000007), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+163, _nY+64, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", _SoldierInfo.siDex);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+232, _nY+63, 36, 12, _pMyWorld->pOnlineText->Get(6000008), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+272, _nY+64, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", _SoldierInfo.siVit);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+124, _nY+80, 36, 12, _pMyWorld->pOnlineText->Get(6000009), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+163, _nY+81, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", _SoldierInfo.siInt);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+232, _nY+80, 36, 12, _pMyWorld->pOnlineText->Get(6000010), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+272, _nY+81, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", 0);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+124, _nY+97, 36, 12, _pMyWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_ATTACKRATE), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+163, _nY+97, szTemp, RGB(216, 201, 183));

		sprintf(szTemp, "%d", 0);
		_pMyWorld->pOnlineMegaText->DrawTextCenter(hDC, _nX+232, _nY+97, 36, 12, _pMyWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEFENCERATE), RGB(216, 201, 183));
		_pMyWorld->pOnlineMegaText->DrawText(hDC, _nX+272, _nY+97, szTemp, RGB(216, 201, 183));
*/
		_BtExit.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}
	

	for(vector<OnlineListControl*>::iterator i=_ListControl.begin(); i!=_ListControl.end(); ++i)
		(*i)->Draw(pSurface);
}

void OnlineSoldierTrade::SetSoldier(OnFollowerInfo &Info, MyItemData *pItemData)
{	
//	m_bAction			= TRUE;
	_SoldierInfo		= Info;
	_nSoldierImageFont	= KI[TransKindOnlineToImjin(_SoldierInfo.uiKind)].GetOnlinePortraitFont();	



	// 인벤토리 아이템을 너어 준다.
	if(pItemData)	AddInventroyItem(pItemData);

	// 장착용 아이템을 너어 준다.
	AddWearItem();
}

void OnlineSoldierTrade::AddWearItem()
{
	ItemInfo	AddItemInfo;	

	for( int i=0; i<WEAR_ITEM_NUM ; i++ )
	{
		if(_SoldierInfo.stWearItemInfo[i].uiItemID != 0)
		{
			AddItemInfo.uiItemID	= _SoldierInfo.stWearItemInfo[i].uiItemID;
			AddItemInfo.ItemCount	= 1;
			AddItemInfo.siColor		= LIST_COLOR_NOT;
			
			if( i == ON_WEAR_HELMET_ITEM )					_pHelmetLCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_ARMOR_ITEM )				_pArmorLCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_BELT_ITEM )				_pBeltLCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_GLOVE_ITEM )				_pGlovesLCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_SHOES_ITEM )				_pBootsLCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_RRING_ITEM )				_pRing1LCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_LRING_ITEM )				_pRing2LCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_WEAPON_ITEM )				_pSwordLCEx->AddItem(AddItemInfo, 0);
			else if( i == ON_WEAR_GUARDIAN_ITEM )			_pGuardianEx->AddItem(AddItemInfo, 0);
		}
	}
}

void OnlineSoldierTrade::AddInventroyItem(MyItemData *pItemData)
{
	ItemInfo	AddItemInfo;	

	// 인벤토리 아이템을 너어 준다.
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
	{
		if(pItemData[i].uiID != -1)
		{
			AddItemInfo.uiItemID	= pItemData[i].uiID;
			AddItemInfo.ItemCount	= pItemData[i].uiQuantity;

			if(_pMyWorld->pItemsInfo->GetItem(AddItemInfo.uiItemID))
			{
				AddItemInfo.siColor = LIST_COLOR_NOT;
				_pItemLC->AddItem(AddItemInfo, pItemData[i].siPosInInventory);
			}			
		}
	}
}

void OnlineSoldierTrade::SetHide()
{
	_nSoldierImageFont = -1; 

/*	for(vector<OnlineListControl*>::iterator i=_ListControl.begin(); i!=_ListControl.end(); ++i)
		(*i)->Clear();*/
}