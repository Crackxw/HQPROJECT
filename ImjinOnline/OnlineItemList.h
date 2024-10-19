/*****************************************************************************************************************
	���ϸ�	:	OnlineItemList.h
	�ۼ���  :	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.12.21
*****************************************************************************************************************/

#ifndef _ON_ITEMLIST__H
#define _ON_ITEMLIST__H

struct SAVEFILE_SEARCH;
struct SAVEFILE_DATA;

struct SortList
{
	UI16	m_uiID;
	SI32	m_siData;
};

#define ON_ITEMLIST_TYPE_NOACTION		0
#define ON_ITEMLIST_TYPE_VILLAGELIST	1
#define ON_ITEMLIST_TYPE_ITEMLIST		2


class OnlineItemList
{
private:
	XSPR				m_SubImage, m_SButImage, m_SScrBackImage, m_SScrBarImage, m_SScrUpImage, m_SScrDownImage;
	_Button				m_BExit, m_BArmor, m_BDress, m_BUp, m_BDown;

	UI08				m_uiType;
	UI16				m_uiSearchItemID;
	SI16				m_siVillageNum;
	SAVEFILE_SEARCH		*m_pSearchHeader;
	cltOnlineWorld		*m_pMyOnlineWorld;
	SI16				m_siX, m_siY, m_siNoField;
	SI16				m_siScroll;
	SI16				m_siScrollPos;
	RECT				m_rtScroll;
	BOOL				m_bMouseDown;
	
	UI16				*m_pWeapon, *m_pArmor, *m_pBelt, *m_pBoots, *m_pGloves, *m_pHelmat, *m_pRing, *m_pDress,*m_pGuardian;
	UI16				*m_ShowItemList,  *m_SwordItemList;
	SI16				m_siShowItemNum, m_siSwordItemNum;

public:
	OnlineItemList(cltOnlineWorld *pOnlineWorld);
	~OnlineItemList();

	VOID					Init();														// �ʱ�ȭ
	VOID					Free();
	VOID					Action();													// �׼�	
	VOID					Draw(LPDIRECTDRAWSURFACE7 pSurface);						// ��ο�
	BOOL					IsAction();													// ���� Ȱ�� ���ΰ�? 
	VOID					SetAction(UI08 uiType, SI32 siAttri, UI32 uiItemID);

private:
	VOID					Initialize();												// �ʱ�ȭ
};
#endif
