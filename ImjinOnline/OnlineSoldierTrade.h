#pragma once

#include <iostream>
#include <vector>

using namespace std;


class OnlineListControl;
class cltOnlineWorld;
class OnlineSoldierTrade
{
public:
	OnlineSoldierTrade(cltOnlineWorld *pOnline, char *pImageFileName);
	~OnlineSoldierTrade();
	
	BOOL	IsAction()	{ return _nSoldierImageFont == -1 ? FALSE : TRUE; }
	void	SetHide();
	void	SetSoldier(OnFollowerInfo &Info, MyItemData *pItemData);	// 선택한 용병의 정보를 설정한다.
	void	Draw(LPDIRECTDRAWSURFACE7 pSurface);						// 선택한 용병의 정보를 출력한다.
	void	Actoin();

private:
	void	AddInventroyItem(MyItemData *pItemData);	// 인벤토리 아이템 삽입
	void	AddWearItem();								// 장착 아이템 삽입

private:
	cltOnlineWorld		*_pMyWorld;						// 온라인 월드.
	OnlineListControl	*_pSwordLCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(칼)
	OnlineListControl	*_pArmorLCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(갑옷)
	OnlineListControl	*_pRing1LCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(링1)
	OnlineListControl	*_pRing2LCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(링2)	
	OnlineListControl	*_pHelmetLCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(하이바)
	OnlineListControl	*_pBeltLCEx;						// 장착용(리스트 컨트롤 확장) 아이템 이다.(허리띠)
	OnlineListControl	*_pGlovesLCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(장갑)
	OnlineListControl	*_pBootsLCEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(부츠)	
	OnlineListControl	*_pGuardianEx;					// 장착용(리스트 컨트롤 확장) 아이템 이다.(수호부)	
	OnlineListControl	*_pItemLC;


	vector<OnlineListControl*>	_ListControl;
	OnFollowerInfo				_SoldierInfo;	
	XSPR						_Image;
	XSPR						m_But1Image;
	_Button						_BtExit;
 
	int							_nX, _nY;
	int							_nSoldierImageFont;
	BOOL						_bMouseDown;
	BOOL						m_bAction;
};