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
	void	SetSoldier(OnFollowerInfo &Info, MyItemData *pItemData);	// ������ �뺴�� ������ �����Ѵ�.
	void	Draw(LPDIRECTDRAWSURFACE7 pSurface);						// ������ �뺴�� ������ ����Ѵ�.
	void	Actoin();

private:
	void	AddInventroyItem(MyItemData *pItemData);	// �κ��丮 ������ ����
	void	AddWearItem();								// ���� ������ ����

private:
	cltOnlineWorld		*_pMyWorld;						// �¶��� ����.
	OnlineListControl	*_pSwordLCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(Į)
	OnlineListControl	*_pArmorLCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(����)
	OnlineListControl	*_pRing1LCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(��1)
	OnlineListControl	*_pRing2LCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(��2)	
	OnlineListControl	*_pHelmetLCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(���̹�)
	OnlineListControl	*_pBeltLCEx;						// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(�㸮��)
	OnlineListControl	*_pGlovesLCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(�尩)
	OnlineListControl	*_pBootsLCEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(����)	
	OnlineListControl	*_pGuardianEx;					// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(��ȣ��)	
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