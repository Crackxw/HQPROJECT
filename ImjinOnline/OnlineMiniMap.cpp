/********************************************************************************************
	파일명	:	OnlineMiniMap.cpp
	작성자	:	정 진 욱
	담당자	:	이 준 석
	작성일	:	2001.07.20
	수정일	:	2001.07.26
	내  용  :	미니맵 출력
********************************************************************************************/

#include <GSL.h>

#include	<Mouse.h>
#include	<clGame.h>


#include	"OnlineMegaText.h"
#include	"OnlineWorld.h"
#include	"OnlineMiniMap.h"
#include	"OnlineVillageSave.h"
#include	"OnlineTradeBook.h"
#include	"OnlineConfig.h"
#include	"OnlineFont.h"
#include	"OnlineDungeonParser.h"
#include	"OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include	"OnlinePannel.h"
#include	"OnlineKeyboard.h"
#include	"OnlineMyData.h"
#include	"OnlineItem.h"
#include	"OnlineResource.h"
#include	"OnlineFieldChat.h"
#include	"OnlineClient.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineFieldArea.h"
#include	"OnlineVillage.h"
#include	"OnlineVillageManager.h"
#include	"OnlineVillage-Parser.h"
#include	"OnlineNPCIndex-Parser.h"
#include	"OnlineMap.h"
#include	"OnlineHelp.h"

#include	"FieldHeroInfoMgr.h"

extern	_clGame* pGame;
extern _InputDevice			IpD;


extern INT		qsort_CompareForMiniMapTarget(const void* arg1, const void* arg2);


OnlineMiniMap::OnlineMiniMap()
{
	// 기본 정보
	siNoFieldHandle = -1;
	siFullMode      = TRUE;
	pMyOnlineWorld  = NULL;
	bMouseDown      = FALSE;

	m_FanfLock     = FALSE;
	m_FanXPosition = 0;
	m_FanYPosition = 0;

	m_YetHeroXPos = 0;
	m_YetHeroYPos = 0;

	// Mini Map 정보 (Mini Map Image 에 관한 부분)
	MiniMap         = NULL;
	MiniMapBack     = NULL;
	m_MiniMapWidth  = 0;
	m_MiniMapHeight = 0;
	m_MapIndex = 0;

	// 현재 보여지는 Map 정보(현재 보여지는 MiniMap의 위치와 확대 비율)
	m_MPointX  = 0;
	m_MPointY  = 0;
	m_Percent  = 0;
	m_MovePositionFlag = FALSE;
	m_MovePositionX    = 0;
	m_MovePositionY    = 0;

	// Target(만약 선택된 Target이 있을 경우 그 위치)
	m_SelectTargetVillageFlag = FALSE;
	m_SelectTargetX    = 0;
	m_SelectTargetY    = 0;

	m_SelectTargetItemFlag  = FALSE;
	m_SelectTargetItemCount = 0;
	m_SelectTargetItemData  = NULL;


	m_SelectTargetstructFlag  = FALSE;
	m_SelectTargetstructCount = 0;
	m_SelectTargetstructData  = NULL;

	// Target List(Target들 List)
	m_TargetType = 0;
	ZeroMemory(m_TargetList, sizeof(SMiniMapTargetList) * 5);
	ZeroMemory(&m_TargetTemp,sizeof(SMiniMapTargetList));

	// Target Find(Target 이름을 손으로 쳐서 찾을때)
//	m_pTargetFindEditBox = NULL;
	ZeroMemory(m_TargetFindEditBoxName, 65);
	m_YetEditHWND         = NULL;
	m_YetEditLockShortKey = 0;

	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_map_area.Spr", MiniMapBorder );
	pImage = new BYTE[MiniMapBorder.Header.Xsize*MiniMapBorder.Header.Ysize];
	clGrp.FreeXspr( MiniMapBorder );

	ZeroMemory(m_CharStatus, sizeof(m_CharStatus));
	m_bVillage = FALSE;
}

OnlineMiniMap::~OnlineMiniMap()
{
	Free();
} 

/********************************************************************************************
	지금 활성화 되어 있는가 알아 본다.
********************************************************************************************/
BOOL	OnlineMiniMap::IsActive()
{
	return m_bAction;
}

/********************************************************************************************
	초기화
********************************************************************************************/
VOID	OnlineMiniMap::Initialize()
{
	SetActive(FALSE);
	m_siStatus = ON_MINIMAP_INIT;
	m_siSubStatus = ON_MINIMAP_SUB_INIT;
	siFullMode	= TRUE;
	m_Percent	= pMyOnlineWorld->pConfig->GetMiniMapSIze();
//	m_pTargetFindEditBox->Initialize();
	m_bVillage = FALSE;
}

VOID	OnlineMiniMap::Init(cltOnlineWorld *pOnlineWorld)
{
	// 기본 정보
	m_siX = 611;
	m_siY = 5;
	m_siSubX = 174;
	m_siSubY = 121;
	m_siStatus = ON_MINIMAP_INIT;
	m_siSubStatus = ON_MINIMAP_SUB_INIT;

	siNoFieldHandle = -1;
	siFullMode      = TRUE;
	pMyOnlineWorld  = pOnlineWorld;
	bMouseDown      = FALSE;

	SetRect(&m_rtScroll, 775, 74, 775 + 9, 74 + 372);
	m_siScroll = m_siSelect = 0;
	m_siScrollPos = 74;

	m_FanfLock     = FALSE;
	m_FanXPosition = 0;
	m_FanYPosition = 0;

	m_YetHeroXPos = 0;
	m_YetHeroYPos = 0;

	// Mini Map 정보 (Mini Map Image 에 관한 부분)
	if(MiniMap) MiniMap->Release();
	MiniMap         = NULL;
	if(MiniMapBack) MiniMapBack->Release();
	MiniMapBack     = NULL;
	m_MiniMapWidth  = 0;
	m_MiniMapHeight = 0;
	m_MapIndex = 0;

	// 현재 보여지는 Map 정보(현재 보여지는 MiniMap의 위치와 확대 비율)
	m_MPointX  = 0;
	m_MPointY  = 0;
	m_Percent  = 0;
	m_MovePositionFlag = FALSE;
	m_MovePositionX    = 0;
	m_MovePositionY    = 0;

	// Target(만약 선택된 Target이 있을 경우 그 위치)
	m_SelectTargetVillageFlag = FALSE;
	m_SelectTargetX    = 0;
	m_SelectTargetY    = 0;
	m_SelectTargetItemFlag  = FALSE;
	m_SelectTargetItemCount = 0;
	m_SelectTargetstructFlag  = FALSE;
	m_SelectTargetstructCount = 0;

	m_YetEditHWND         = FALSE;
	m_YetEditLockShortKey = 0;
	ZeroMemory(m_TargetFindEditBoxName, 65);

	// Image 정보(창을 뛰우는데 필요한 Image)
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\MinimapTarget.Spr", TargetSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\Minimap_Village.Spr", VillageSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\MinimapWharfVillage.Spr", WharfVillageSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\Minimap_CharFace.Spr", CharFaceSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\MinimapDungeon.Spr", DungeonSpr);

	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\book_scr_left.Spr", m_ScrHUp);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\book_scr_right.Spr", m_ScrHDown);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\book_scr_up.Spr", m_ScrVUp);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\book_scr_down.Spr", m_ScrVDown);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\book_scr_handle.Spr", m_ScrHandle);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_scr_bar.Spr", m_ScrBack);

	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_small_back.Spr", m_SmallBack);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_big_back.Spr", m_BigBack);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_map_area.Spr", MiniMapBorder);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_bigmap_button.Spr", m_BigMap);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_plus_button.Spr", m_PlusToggle);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\minimap_minus_button.Spr", m_MinusToggle);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_ButMain);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\enter_button.Spr", m_ButEnter);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr", m_ButExit);

	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_mag_base.Spr", m_MagBase);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_mag_button_here.Spr", m_Mag1);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_mag_button_plus.Spr", m_Mag2);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_mag_button_minus.Spr", m_Mag3);

	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_iteminfo.Spr", m_SubBack);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\bigmap_iteminfo_border.Spr", m_SubBackBorder);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\small_window_push_button.Spr", m_SubExit);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\small_window_scr_bar.Spr", m_SubScrBack);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\small_window_scr_handle.Spr", m_SubScrHandle);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\small_window_scr_up.Spr", m_SubScrUp);
	clGrp.LoadXspr("Online\\GameSpr\\Minimap\\small_window_scr_down.Spr", m_SubScrDown);

	// Mini Map 정보 (Mini Map Image 에 관한 부분)
	MiniMap         = NULL;
	MiniMapBack     = NULL;
	m_MiniMapWidth  = 0;
	m_MiniMapHeight = 0;
	m_MapIndex = 0;

	// 현재 보여지는 Map 정보(현재 보여지는 MiniMap의 위치와 확대 비율)
	m_MPointX  = 0;
	m_MPointY  = 0;
	m_Percent  = pMyOnlineWorld->pConfig->GetMiniMapSIze();
	m_MovePositionFlag = FALSE;
	m_MovePositionX    = 0;
	m_MovePositionY    = 0;

	m_SelectTargetItemFlag  = FALSE;
	m_SelectTargetItemCount = 0;
	m_SelectTargetItemData  = new SMiniMapTargetDataV[1024];

	m_SelectTargetstructFlag  = FALSE;
	m_SelectTargetstructCount = 0;
	m_SelectTargetstructData  = new SMiniMapTargetData[1024];

	// Target List(Target들 List)
	m_TargetType = 0;
	ZeroMemory(m_TargetList, sizeof(SMiniMapTargetList) * 5);
	m_TargetList[0].m_pList = new SMiniMapTargetData[1024];
	m_TargetList[1].m_pList = new SMiniMapTargetData[1024];
	m_TargetList[2].m_pList = new SMiniMapTargetData[1024];
	m_TargetList[3].m_pList = new SMiniMapTargetData[1024];
	m_TargetList[4].m_pList = new SMiniMapTargetData[1024];

	m_TargetTemp.m_pList = new SMiniMapTargetData[1024];

	ZeroMemory(m_TargetList[4].m_pList, sizeof(m_TargetList[4].m_pList));

	// Target Find(Target 이름을 손으로 쳐서 찾을때)
//	m_pTargetFindEditBox = new OnlineEditControl;
//	m_pTargetFindEditBox->CreateEditBox(1000, 1000, 200, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 14 );
//	m_pTargetFindEditBox->EnableWindow(FALSE);
//	SendMessage(m_pTargetFindEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)14, 0);
//	m_pTargetFindEditBox->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

	OnlineEditControlEx	*pOecEx;
	if( m_hOecFindEdit )	Handler_OnlineEditControlEx::KillControl( m_hOecFindEdit );
	RECT	rcRect  = { 652, 40, 746, 55};
	m_hOecFindEdit	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, 14);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecFindEdit );
	pOecEx->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(200, 200, 200) );

	m_YetEditHWND         = NULL;
	m_YetEditLockShortKey = 0;
	ZeroMemory(m_TargetFindEditBoxName, 65);


	// 작은맵 큰맵으로 전환, 확대, 축소
	m_BBigmap.Create(m_siX + 144, m_siY + 19,  m_BigMap.Header.Xsize, m_BigMap.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BSEnlarge.Create(m_siX + 133, m_siY + 155, m_PlusToggle.Header.Xsize, m_PlusToggle.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BSReduce.Create(m_siX + 152, m_siY + 139,  m_MinusToggle.Header.Xsize, m_MinusToggle.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_siX = m_siY = 0;
	// 큰맵 메인메뉴
	m_BVillage.Create(m_siX + 26, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_VILLAGE), BUTTON_PUT_LEFT, TRUE);
	m_BItem.Create(m_siX + 113, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_ITEM), BUTTON_PUT_LEFT, TRUE);
	m_BStruct.Create(m_siX + 200, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_STRUCTER), BUTTON_PUT_LEFT, TRUE);
	m_BDungeon.Create(m_siX + 287, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_SPECIAL), BUTTON_PUT_LEFT, TRUE);
	m_BShortCut.Create(m_siX + 374, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_SHORTCUT), BUTTON_PUT_LEFT, TRUE);
	m_BNpc.Create(m_siX + 461, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_MISSION), BUTTON_PUT_LEFT, TRUE);
	m_BChar.Create(m_siX + 548, m_siY + 39, m_ButMain.Header.Xsize, m_ButMain.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_CHARACTER), BUTTON_PUT_LEFT, TRUE);

	// 큰맵 서브 메뉴
	m_BCharCenter.Create(m_siX + 554, m_siY + 75, m_Mag1.Header.Xsize, m_Mag1.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BBEnlarge.Create(m_siX + 577, m_siY + 75, m_Mag2.Header.Xsize, m_Mag2.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BBReduce.Create(m_siX + 600, m_siY + 75, m_Mag3.Header.Xsize, m_Mag3.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BFind.Create(m_siX + 748, m_siY + 39, m_ButEnter.Header.Xsize, m_ButEnter.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BExit.Create(m_siX + 678, m_siY + 473, m_ButExit.Header.Xsize, m_ButExit.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(2281), BUTTON_PUT_LEFT, TRUE);

	// 스크롤 버튼
	m_BScrBut[0].Create(m_siX + 627, m_siY + 65, m_ScrVUp.Header.Xsize, m_ScrVUp.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BScrBut[1].Create(m_siX + 627, m_siY + 471, m_ScrVDown.Header.Xsize, m_ScrVDown.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BScrBut[2].Create(m_siX + 18, m_siY + 480, m_ScrHUp.Header.Xsize, m_ScrHUp.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BScrBut[3].Create(m_siX + 618, m_siY + 480, m_ScrHDown.Header.Xsize, m_ScrHDown.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BTargetScrBut[0].Create(m_siX + 775, m_siY + 65, m_ScrVUp.Header.Xsize, m_ScrVUp.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BTargetScrBut[1].Create(m_siX + 775, m_siY + 446, m_ScrVDown.Header.Xsize, m_ScrVDown.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	// 아이템 정보창 버튼
	m_BSubExit.Create(m_siSubX + 124, m_siSubY + 265, m_SubExit.Header.Xsize, m_SubExit.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(2281), BUTTON_PUT_LEFT, TRUE);
	m_BSubScrUp.Create(m_siSubX + 291, m_siSubY + 37, m_SubScrUp.Header.Xsize, m_SubScrUp.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BSubScrDown.Create(m_siSubX + 291, m_siSubY + 245, m_SubScrDown.Header.Xsize, m_SubScrDown.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_siX = 611;
	m_siY = 5;
}

/********************************************************************************************

********************************************************************************************/
VOID	OnlineMiniMap::Free()
{
	// 기본 정보
	siNoFieldHandle = -1;
	siFullMode      = TRUE;
	pMyOnlineWorld  = NULL;
	bMouseDown      = FALSE;

	m_FanfLock     = FALSE;
	m_FanXPosition = 0;
	m_FanYPosition = 0;

	m_YetHeroXPos = 0;
	m_YetHeroYPos = 0;

	// Image 정보(창을 뛰우는데 필요한 Image)
	clGrp.FreeXspr(TargetSpr);		clGrp.FreeXspr(VillageSpr);		clGrp.FreeXspr(WharfVillageSpr);
	clGrp.FreeXspr(CharFaceSpr);	clGrp.FreeXspr(DungeonSpr);

	clGrp.FreeXspr(m_ScrHUp);		clGrp.FreeXspr(m_ScrHDown);		clGrp.FreeXspr(m_ScrVUp);
	clGrp.FreeXspr(m_ScrVDown);		clGrp.FreeXspr(m_ScrHandle);	clGrp.FreeXspr(m_ScrBack);

	clGrp.FreeXspr(m_SmallBack);	clGrp.FreeXspr(m_BigBack);		clGrp.FreeXspr(MiniMapBorder);
	clGrp.FreeXspr(m_BigMap);		clGrp.FreeXspr(m_PlusToggle);	clGrp.FreeXspr(m_MinusToggle);

	clGrp.FreeXspr(m_ButMain);		clGrp.FreeXspr(m_ButEnter);		clGrp.FreeXspr(m_ButExit);
	clGrp.FreeXspr(m_MagBase);		clGrp.FreeXspr(m_Mag1);			clGrp.FreeXspr(m_Mag2);
	clGrp.FreeXspr(m_Mag3);

	clGrp.FreeXspr(m_SubBack);		clGrp.FreeXspr(m_SubBackBorder);clGrp.FreeXspr(m_SubExit);	
	clGrp.FreeXspr(m_SubScrBack);	clGrp.FreeXspr(m_SubScrHandle);	clGrp.FreeXspr(m_SubScrUp);		
	clGrp.FreeXspr(m_SubScrDown);


	// Mini Map 정보 (Mini Map Image 에 관한 부분)
	if(MiniMap) MiniMap->Release();
	MiniMap         = NULL;
	if(MiniMapBack) MiniMapBack->Release();
	MiniMapBack     = NULL;
	m_MiniMapWidth  = 0;
	m_MiniMapHeight = 0;
	m_MapIndex = 0;

	// 현재 보여지는 Map 정보(현재 보여지는 MiniMap의 위치와 확대 비율)
	m_MPointX  = 0;
	m_MPointY  = 0;
	m_Percent  = 0;
	m_MovePositionFlag = FALSE;
	m_MovePositionX    = 0;
	m_MovePositionY    = 0;

	// Target(만약 선택된 Target이 있을 경우 그 위치)
	m_SelectTargetVillageFlag = FALSE;
	m_SelectTargetX    = 0;
	m_SelectTargetY    = 0;
	m_SelectTargetItemFlag  = FALSE;
	m_SelectTargetItemCount = 0;
	m_SelectTargetstructFlag  = FALSE;
	m_SelectTargetstructCount = 0;

	if(m_SelectTargetItemData)
	{
		delete[] m_SelectTargetItemData;
		m_SelectTargetItemData = NULL;
	}
	if(m_SelectTargetstructData)
	{
		delete[] m_SelectTargetstructData;
		m_SelectTargetstructData = NULL;
	}

	// Target List(Target들 List)
	m_TargetType = 0;
	if(m_TargetList[0].m_pList)
	{
		delete[] m_TargetList[0].m_pList;
		m_TargetList[0].m_pList = NULL;
	}
	if(m_TargetList[1].m_pList)
	{
		delete[] m_TargetList[1].m_pList;
		m_TargetList[1].m_pList = NULL;
	}
	if(m_TargetList[2].m_pList)
	{
		delete[] m_TargetList[2].m_pList;
		m_TargetList[2].m_pList = NULL;
	}
	if(m_TargetList[3].m_pList)
	{
		delete[] m_TargetList[3].m_pList;
		m_TargetList[3].m_pList = NULL;
	}
	if(m_TargetList[4].m_pList)
	{
		delete[] m_TargetList[4].m_pList;
		m_TargetList[4].m_pList = NULL;
	}

	if(m_TargetTemp.m_pList)
	{
		delete[] m_TargetTemp.m_pList;
		m_TargetTemp.m_pList = NULL;
	}

	ZeroMemory(m_TargetList, sizeof(SMiniMapTargetList) * 5);
	ZeroMemory(&m_TargetTemp,sizeof(SMiniMapTargetList));

	// Target Find(Target 이름을 손으로 쳐서 찾을때)
/*	if(m_pTargetFindEditBox)
	{
		delete m_pTargetFindEditBox;
		m_pTargetFindEditBox = NULL;
	}
*/
	Handler_OnlineEditControlEx::KillControl( m_hOecFindEdit );

	m_YetEditHWND         = FALSE;
	m_YetEditLockShortKey = 0;
	ZeroMemory(m_TargetFindEditBoxName, 65);

	if(pImage)
	{
		delete []pImage;	
		pImage = NULL;
	}
}

/********************************************************************************************
	Load Mini Map
********************************************************************************************/
BOOL	OnlineMiniMap::LoadMiniMap(CHAR* lpMiniMap, SI32 MapIndex)
{
	XSPR					TempMiniMap;

	//================================================================
	// Image Load
	if(MiniMap) MiniMap->Release();

	clGrp.LoadXspr(lpMiniMap, TempMiniMap);
	clGrp.CreateOffScreen(MiniMap, TempMiniMap.Header.Xsize, TempMiniMap.Header.Ysize, TRUE, FALSE);

	clGrp.ClearSurface(MiniMap);

	int	ClipLeft, ClipTop, ClipRight, ClipBottom;
	//클리핑 영역을 바꾼다.
	clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);

	if(clGrp.LockSurface(MiniMap))
	{
		clGrp.SetClipArea(0, 0, TempMiniMap.Header.Xsize-1, TempMiniMap.Header.Ysize-1);

		clGrp.PutSpriteT(0, 0, TempMiniMap.Header.Xsize, TempMiniMap.Header.Ysize, TempMiniMap.Image);
		clGrp.UnlockSurface(MiniMap);
	}

	clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);

	m_MiniMapWidth  = TempMiniMap.Header.Xsize;
	m_MiniMapHeight = TempMiniMap.Header.Ysize;
	clGrp.FreeXspr(TempMiniMap);

	clGrp.CreateOffScreen(MiniMapBack, MINIMAP_SIZE_MAX_WIDTH, MINIMAP_SIZE_MAX_HEIGHT, TRUE, FALSE);
	clGrp.ClearSurface(MiniMapBack);
	
	m_MapIndex = MapIndex;

	//================================================================
	// 변수 초기화
	m_FanfLock     = FALSE;
	m_FanXPosition = 0;
	m_FanYPosition = 0;

	m_YetHeroXPos = 0;
	m_YetHeroYPos = 0;

	m_MPointX  = 0;
	m_MPointY  = 0;
	m_MovePositionFlag = FALSE;
	m_MovePositionX    = 0;
	m_MovePositionY    = 0;

	m_SelectTargetVillageFlag = FALSE;
	m_SelectTargetX = 0;
	m_SelectTargetY = 0;
	m_SelectTargetItemFlag  = FALSE;
	m_SelectTargetItemCount = 0;

	if(!m_MapIndex)
	{
		m_BVillage.SetAction(TRUE);
		m_BItem.SetAction(TRUE);
		m_BStruct.SetAction(TRUE);
		m_BChar.SetAction(TRUE);
		m_BDungeon.SetAction(TRUE);
		m_BShortCut.SetAction(TRUE);
		m_BNpc.SetAction(TRUE);

		ButtonStatus(m_siStatus, ON_MINIMAP_VILLAGE);
		m_siStatus = ON_MINIMAP_VILLAGE;
	}

	//================================================================
	// Target List 만들기
	MakeTargetList();

//	m_pTargetFindEditBox->SetString("");
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl(m_hOecFindEdit);
	pOecEx->Clear();

	ZeroMemory(m_TargetFindEditBoxName, 65);

	return TRUE;
}

/********************************************************************************************
	미니맵을 처리한다.
********************************************************************************************/
VOID	OnlineMiniMap::Pool(BOOL bVillage)
{
	m_bVillage = bVillage;

	if(!IsActive())		return;

	ActionCommon();

	if(siFullMode)		return;

	switch(m_siStatus)
	{
	case ON_MINIMAP_INIT:
		break;
	case ON_MINIMAP_VILLAGE:
		ActionVillage();
		break;
	case ON_MINIMAP_ITEM:
		MinimapMove();
		if(bMouseDown)
		{
			SelectTarget(1);
			ActionTargetScroll(1);
		}
		ActionEditBox(1);
		break;
	case ON_MINIMAP_STRUCT:
		MinimapMove();
		if(bMouseDown)
		{
			SelectTarget(3);
			ActionTargetScroll(3);
		}
		ActionEditBox(3);
		break;
	case ON_MINIMAP_DUNGEON:
		MinimapMove();
		if(bMouseDown)
		{
			SelectTarget(2);
			ActionTargetScroll(2);
		}
		ActionEditBox(2);
		break;
	case ON_MINIMAP_SHORTCUT:
		MinimapMove();
		if(bMouseDown)
		{
			SelectTarget(2);
			ActionTargetScroll(2);
		}
		ActionEditBox(2);
		break;
	case ON_MINIMAP_NPC:
		MinimapMove();
		if(bMouseDown)
		{
			SelectTarget(2);
			ActionTargetScroll(2);
		}
		ActionEditBox(2);
		break;
	case ON_MINIMAP_CHAR:
		MinimapMove();
		ActionEditBox(4);
		break;
	case ON_MINIMAP_WAIT:
		break;
	case ON_MINIMAP_EXIT:
		break;
	}

	if(IpD.LeftPressSwitch) bMouseDown = TRUE;
	else				    bMouseDown = FALSE;
}

/********************************************************************************************
	출력
********************************************************************************************/
VOID	OnlineMiniMap::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	if(!IsActive())		return;

	SI32 m_siXsize, m_siYsize, TempXSP, TempYSP, TempXSS, TempYSS;
	float TempXFloat, TempYFloat;
	DWORD TextColor = RGB(250, 250, 250);

	pMyOnlineWorld->pOnlineMap->GetSize(&m_siXsize, &m_siYsize);
	
	// 미니맵의 시작위치 검사
	if(siFullMode)
	{
		TempXSP = m_siX;
		TempYSP = m_siY;
		
		TempXSS = MINIMAP_SIZE_MIN_WIDTH;
		TempYSS = MINIMAP_SIZE_MIN_HEIGHT;
	}
	else
	{
		TempXSP = m_siX + 21;
		TempYSP = m_siY + 68;
		
		TempXSS = MINIMAP_SIZE_MAX_WIDTH;
		TempYSS = MINIMAP_SIZE_MAX_HEIGHT;
	}
	
	// 미니맵 출력
	DrawMiniMap(pSurface, TempXSP, TempYSP, TempXSS, TempYSS);

	SI16 siType = -1;
	switch(m_siStatus)
	{
	case ON_MINIMAP_VILLAGE:	siType = 0;		break;
	case ON_MINIMAP_ITEM:		siType = 1;		break;
	case ON_MINIMAP_STRUCT:		siType = 3;		break;
	case ON_MINIMAP_DUNGEON:
	case ON_MINIMAP_SHORTCUT:
	case ON_MINIMAP_NPC:		siType = 2;		break;
		break;
	}
	
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		if(siFullMode)
		{
			// 배경 출력
			clGrp.PutSpriteT(m_siX, m_siY, m_SmallBack.Header.Xsize, m_SmallBack.Header.Ysize, m_SmallBack.Image);		
			
			// 전체모드로 교체, 확대, 축소 버튼
			m_BBigmap.Put(&m_BigMap, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BSEnlarge.Put(&m_PlusToggle, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BSReduce.Put(&m_MinusToggle, 0, 2, 1, BUTTON_PUT_NOMOVE);
		}
		else
		{
			// 배경 출력
			clGrp.PutSpriteT(m_siX, m_siY, m_BigBack.Header.Xsize, m_BigBack.Header.Ysize, m_BigBack.Image);
			clGrp.PutSpriteT(m_siX+551, m_siY+72, m_MagBase.Header.Xsize, m_MagBase.Header.Ysize, m_MagBase.Image);

			// Target List 스크롤
			if(siType != -1)
			{
				if(m_TargetList[siType].m_siMaxCount > MINIMAP_LIST_MAX_LINE)
				{
					clGrp.PutSpriteT(m_siX + 771, m_siY + 65, m_ScrBack.Header.Xsize, m_ScrBack.Header.Ysize, m_ScrBack.Image );
					clGrp.PutSpriteT(m_siX + 775, m_siScrollPos, m_ScrHandle.Header.Xsize, m_ScrHandle.Header.Ysize, m_ScrHandle.Image );
					m_BTargetScrBut[0].Put(&m_ScrVUp, 0, 1, 0, BUTTON_PUT_NOMOVE);
					m_BTargetScrBut[1].Put(&m_ScrVDown, 0, 1, 0, BUTTON_PUT_NOMOVE);
				}
			}

			// 맵 스크롤
			m_BScrBut[0].Put(&m_ScrVUp, 0, 1, 0, BUTTON_PUT_NOMOVE);
			m_BScrBut[1].Put(&m_ScrVDown, 0, 1, 0, BUTTON_PUT_NOMOVE);
			m_BScrBut[2].Put(&m_ScrHUp, 0, 1, 0, BUTTON_PUT_NOMOVE);
			m_BScrBut[3].Put(&m_ScrHDown, 0, 1, 0, BUTTON_PUT_NOMOVE);
			CheckScrollPercent(m_MPointX, m_MPointY, TempXFloat, TempYFloat);
			TempXFloat = (TempXFloat * 591.0f) + (m_siX + 27);
			TempYFloat = (TempYFloat * 397.0f) + (m_siY + 74);
			clGrp.PutSpriteT((SI32)TempXFloat, m_siY + 480, m_ScrHandle.Header.Xsize, m_ScrHandle.Header.Ysize, m_ScrHandle.Image);
			clGrp.PutSpriteT(m_siX + 627, (SI32)TempYFloat, m_ScrHandle.Header.Xsize, m_ScrHandle.Header.Ysize, m_ScrHandle.Image);

			// 마을취급 아이템
			if(m_siStatus == ON_MINIMAP_VILLAGE && m_siSubStatus == ON_MINIMAP_SUB_ITEMINFO)
			{
				// 배경창
				clGrp.PutSpriteJin(m_siSubX, m_siSubY, m_SubBack.Header.Xsize, m_SubBack.Header.Ysize, m_SubBack.Image);
				clGrp.PutSpriteT(m_siSubX-1, m_siSubY-1, m_SubBackBorder.Header.Xsize, m_SubBackBorder.Header.Ysize, m_SubBackBorder.Image);

				if(m_pVillageData)
				{
					// 스크롤
					if(m_pVillageData->m_siItemNum > MINIMAP_SUB_LIST_MAX_LINE)
					{
						clGrp.PutSpriteT(m_siSubX + 291, m_siSubY + 37, m_SubScrBack.Header.Xsize, m_SubScrBack.Header.Ysize, m_SubScrBack.Image );
						clGrp.PutSpriteT(m_siSubX + 291, m_siSubScrollPos, m_SubScrHandle.Header.Xsize, m_SubScrHandle.Header.Ysize, m_SubScrHandle.Image );
						m_BSubScrUp.Put(&m_SubScrUp, 0, 1, 0, BUTTON_PUT_NOMOVE);
						m_BSubScrDown.Put(&m_SubScrDown, 0, 1, 0, BUTTON_PUT_NOMOVE);
					}
				}
				m_BSubExit.Put(&m_SubExit, 0, 2, 1, BUTTON_PUT_NOMOVE);
			}

			// 버튼
			m_BVillage.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BItem.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BStruct.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BDungeon.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BShortCut.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BNpc.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BChar.Put(&m_ButMain, 0, 2, 1, BUTTON_PUT_NOMOVE);

			m_BCharCenter.Put(&m_Mag1, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BBEnlarge.Put(&m_Mag2, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BBReduce.Put(&m_Mag3, 0, 2, 1, BUTTON_PUT_NOMOVE);

			m_BFind.Put(&m_ButEnter, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_BExit.Put(&m_ButExit, 0, 2, 1, BUTTON_PUT_NOMOVE);
		}
		
		clGrp.UnlockSurface(pSurface);
	}

	HDC hDC;
//	CHAR szTemp1[256], szTemp2[256];
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		
		if(siFullMode == FALSE)
		{
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 341, m_siY + 6, 118, 13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP), RGB(250, 250, 250));

			// Target List 출력
			for(SI16 i = 0; i < MINIMAP_LIST_MAX_LINE; i++)
			{
				SI16 index = i + m_siScroll;
				
				if(index >= m_TargetList[siType].m_siMaxCount)	continue;

				if(index == m_siSelect) TextColor = RGB(0, 250, 0);
				else					TextColor = RGB(250, 250, 250);

				if(m_siStatus == ON_MINIMAP_VILLAGE)
				{
					SAVEFILE_DATAHEADER	*pVillageData = pMyOnlineWorld->pOnlineVillageSave->GetData(m_TargetList[siType].m_pList[index].m_uiVillageCode);
					if(!pVillageData)
					{
						if(index == m_siSelect) TextColor = RGB(0, 250, 0);
						else					TextColor = RGB(150, 150, 150);
					}
				}

/*				SI16 temp = strlen(m_TargetList[siType].m_pList[index].m_TargetName);
				if(temp > 18)
				{
					ZeroMemory(szTemp1, sizeof(szTemp1));
					if(UI08(m_TargetList[siType].m_pList[index].m_TargetName[17]) > 127)
					{
						strncpy(szTemp1, m_TargetList[siType].m_pList[index].m_TargetName, 17);
						szTemp1[17] = 0;
					}
					else
						strncpy(szTemp1, m_TargetList[siType].m_pList[index].m_TargetName, 18);
					sprintf(szTemp2, "%s..", szTemp1);
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 651, m_siY + 74 + i * 20, (char*)szTemp2, TextColor);
				}
				else*/
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 651, m_siY + 74 + i * 20, (char*)m_TargetList[siType].m_pList[index].m_TargetName, TextColor);
			}

			if(m_siStatus == ON_MINIMAP_CHAR)
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 651, m_siY + 74, (char*)m_CharStatus, RGB(0, 250, 0));
				
			// 마을 찾기 Text 출력
			//if(m_pTargetFindEditBox->IsFocus())
			//m_pTargetFindEditBox->DrawText(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), 
			//	RGB(250, 250, 250), m_siX + 655, m_siY + 42, NULL, bFocus);

			if(Handler_OnlineEditControlEx::IsFocus(m_hOecFindEdit))
			{
				OnlineEditControlEx	*pOecEx;
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecFindEdit );
				pOecEx->Draw( hDC );
			}

			// 마을취급 아이템
			if(m_siStatus == ON_MINIMAP_VILLAGE && m_siSubStatus == ON_MINIMAP_SUB_ITEMINFO)
				DrawItemList(hDC);

			// 버튼
			m_BVillage.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BItem.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BStruct.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BDungeon.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BShortCut.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BNpc.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BChar.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		pSurface->ReleaseDC(hDC);
	}
}

/********************************************************************************************
	미니맵만을 출력한다.
********************************************************************************************/
VOID	OnlineMiniMap::DrawMiniMap(LPDIRECTDRAWSURFACE7 pSurface, SI16 XPos, SI16 YPos, SI16 XSize, SI16 YSize)
{
	if(MiniMap == NULL) return;

	char TempBuffer[256];
	SI32 siXPos, siYPos, siCXPos, siCYPos, siCPXPos, siCPYPos;
	SI32 siXPSize, siYPSize, siTempX, siTempY, TempCount;
	RECT TempDestRect, TempSrcRect;
	float TempXPercent, TempYPercent, TempCXPercent, TempCYPercent;
	HDC		hDC;
	XSPR	TempMiniMap;

	// 서피스 잃어버리는지 체크
	if(MiniMap->IsLost() != DD_OK)
	{
		MiniMap->Restore();
		
		// actdoll (2004/07/09 11:09) : 만약 본맵과 관련된 데이터가 왔을 경우 중국용 미니맵을 따로 갱신한다.
		if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_CHINA )	clGrp.LoadXspr("Online\\Map\\Minimap_China.Spr",	TempMiniMap);
		else													clGrp.LoadXspr("Online\\Map\\Minimap.Spr",			TempMiniMap);

		int	ClipLeft, ClipTop, ClipRight, ClipBottom;
		clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
		
		if(clGrp.LockSurface(MiniMap))
		{
			clGrp.SetClipArea(0, 0, TempMiniMap.Header.Xsize-1, TempMiniMap.Header.Ysize-1);
			clGrp.PutSpriteT(0, 0, TempMiniMap.Header.Xsize, TempMiniMap.Header.Ysize, TempMiniMap.Image);
			clGrp.UnlockSurface(MiniMap);
		}
		
		clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);

		clGrp.FreeXspr( TempMiniMap );
	}

	if(MiniMapBack->IsLost() != DD_OK)
	{
		MiniMapBack->Restore();
	}

	// 우선 미니맵이 나올장소를 초기화
	TempDestRect.left   = 0;
	TempDestRect.top    = 0;
	TempDestRect.right  = XSize;
	TempDestRect.bottom = YSize;
	clGrp.ClearSurface(MiniMapBack);

	// 출력될 MiniMap의 크기 검사
	CheckScrollPercent(m_MPointX, m_MPointY, TempXPercent, TempYPercent);
	siXPos   = (SI32)(TempXPercent * (float)m_MiniMapWidth);
	siYPos   = (SI32)(TempYPercent * (float)m_MiniMapHeight);
	siXPSize = (SI32)((float)(XSize / 2) * (1.0f + (float)(m_Percent) / 2.0f));
	siYPSize = (SI32)((float)(YSize / 2) * (1.0f + (float)(m_Percent) / 2.0f));

	TempSrcRect.left   = siXPos - siXPSize;
	TempSrcRect.top    = siYPos - siYPSize;
	TempSrcRect.right  = siXPos + siXPSize;
	TempSrcRect.bottom = siYPos + siYPSize;

	// Clipping
	if(TempSrcRect.left   >= m_MiniMapWidth)  return;
	if(TempSrcRect.top    >= m_MiniMapHeight) return;
	if(TempSrcRect.right  <  0)               return;
	if(TempSrcRect.bottom <  0)               return;

	if(TempSrcRect.left   <  0)
	{
		TempDestRect.left += (SI32)((float)XSize * ((float)(-TempSrcRect.left) / (float)(TempSrcRect.right - TempSrcRect.left)));
		TempSrcRect.left  =  0;
	}
	if(TempSrcRect.top    <  0)
	{
		TempDestRect.top += (SI32)((float)YSize * ((float)(-TempSrcRect.top) / (float)(TempSrcRect.bottom - TempSrcRect.top)));
		TempSrcRect.top  =  0;
	}
	if(TempSrcRect.right  >= m_MiniMapWidth) 
	{
		TempDestRect.right -= (SI32)((float)XSize * ((float)(TempSrcRect.right - m_MiniMapWidth) / (float)(TempSrcRect.right - TempSrcRect.left)));
		TempSrcRect.right  =  m_MiniMapWidth;
	}
	if(TempSrcRect.bottom >= m_MiniMapHeight)
	{
		TempDestRect.bottom -= (SI32)((float)YSize * ((float)(TempSrcRect.bottom - m_MiniMapHeight) / (float)(TempSrcRect.bottom - TempSrcRect.top)));
		TempSrcRect.bottom  =  m_MiniMapHeight;
	}

	// Blt
	MiniMapBack->Blt(&TempDestRect, MiniMap, &TempSrcRect, DDBLT_WAIT, NULL);
	
	// 전체 지도의 크기를 구함
	TempDestRect.left   = (XSize / 2) - (SI32)((float)siXPos / (1.0f + (float)(m_Percent) / 2.0f));
	TempDestRect.top    = (YSize / 2) - (SI32)((float)siYPos / (1.0f + (float)(m_Percent) / 2.0f));
	TempDestRect.right  = (XSize / 2) + (SI32)((float)(m_MiniMapWidth - siXPos)  / (1.0f + (float)(m_Percent) / 2.0f));
	TempDestRect.bottom = (YSize / 2) + (SI32)((float)(m_MiniMapHeight - siYPos) / (1.0f + (float)(m_Percent) / 2.0f));
	clGrp.SetClipArea(0, 0, XSize - 1, YSize - 1);

	if(MiniMapBack->GetDC(&hDC) == DD_OK)
	{
		switch(pGame->LanguageCode)
		{
		case ELANGAGECODE_TAIWAN:	SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_MINIMAP2));		break;
		default:					SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_MINIMAP1));		break;
		}

		// 미니맵에서 선택한 Target출력
		switch(m_siStatus)
		{
		case ON_MINIMAP_VILLAGE:
			// 모든 마을 이름 출력
			if(m_Percent <= 1 || !siFullMode)
			{
				for(TempCount = 0; TempCount < m_TargetList[0].m_siMaxCount ; TempCount++)
				{
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[0].m_pList[TempCount].m_PositionX , 
						m_TargetList[0].m_pList[TempCount].m_PositionY, siTempX, siTempY);
					CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
					
					siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent) + 7;
					siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent) - 7;
					
					sprintf((char*)TempBuffer, "%s", m_TargetList[0].m_pList[TempCount].m_TargetName);

					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos + 1, siCYPos + 1, (char*)TempBuffer, RGB(1, 1, 1));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos, siCYPos, (char*)TempBuffer, RGB(255, 255, 255));
				}
			}
			break;
		case ON_MINIMAP_ITEM:
			if(m_SelectTargetItemFlag && m_SelectTargetItemCount)
			{		
				if(m_Percent <= 1 || !siFullMode)
				{
					for(TempCount = 0; TempCount < m_SelectTargetItemCount; TempCount++)
					{
						CheckScrollPercent(m_SelectTargetItemData[TempCount].m_TargetX, m_SelectTargetItemData[TempCount].m_TargetY, TempCXPercent, TempCYPercent);
						
						siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent) + 7;
						siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent) - 12;
						
						sprintf((char*)TempBuffer, "%s", m_SelectTargetItemData[TempCount].m_TargetName);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos + 1, siCYPos + 1, (char*)TempBuffer, RGB(1, 1, 1));
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos, siCYPos, (char*)TempBuffer, RGB(255, 255, 255));
					}
				}
			}
			break;
		case ON_MINIMAP_STRUCT:
			// 선택된 건물 이름 출력
			if(m_SelectTargetstructFlag && m_SelectTargetstructCount)
			{		
				if(m_Percent <= 1 || !siFullMode)
				{
					for(TempCount = 0; TempCount < m_SelectTargetstructCount; TempCount++)
					{
						pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_SelectTargetstructData[TempCount].m_PositionX , 
							m_SelectTargetstructData[TempCount].m_PositionY, siTempX, siTempY);
						CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
						
						siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent) + 7;
						siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent) - 7;
						
						sprintf((char*)TempBuffer, "%s", m_SelectTargetstructData[TempCount].m_TargetName);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos + 1, siCYPos + 1, (char*)TempBuffer, RGB(1, 1, 1));
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos, siCYPos, (char*)TempBuffer, RGB(255, 255, 255));
					}
				}
			}
			break;
		case ON_MINIMAP_DUNGEON:
		case ON_MINIMAP_SHORTCUT:
		case ON_MINIMAP_NPC:
			// 특수지형 이름 출력
			if(m_Percent <= 1 || !siFullMode)
			{
				for(TempCount = 0; TempCount < m_TargetList[2].m_siMaxCount ; TempCount++)
				{
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[2].m_pList[TempCount].m_PositionX , 
						m_TargetList[2].m_pList[TempCount].m_PositionY, siTempX, siTempY);
					CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
					
					siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent) + 7;
					siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent) - 7;

					
					if(m_TargetList[2].m_pList[TempCount].m_iDungeonCode == 1)
						sprintf((char*)TempBuffer, "%s(%d)", m_TargetList[2].m_pList[TempCount].m_TargetName, m_TargetList[2].m_pList[TempCount].m_BattlePoint);
					else
						sprintf((char*)TempBuffer, "%s", m_TargetList[2].m_pList[TempCount].m_TargetName);

					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos + 1, siCYPos + 1, (char*)TempBuffer, RGB(1, 1, 1));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos, siCYPos, (char*)TempBuffer, RGB(255, 255, 255));
				}
			}
			break;
		case ON_MINIMAP_CHAR:
			// 찾고자 하는 캐릭터 출력
			pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX ,
				m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
			CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
						
			siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent) + 7;
			siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent) - 7;
			
			sprintf((char*)TempBuffer, "%s(%d, %d)", m_TargetList[4].m_pList[0].m_TargetName, m_TargetList[4].m_pList[0].m_PositionX, m_TargetList[4].m_pList[0].m_PositionY);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos + 1, siCYPos + 1, (char*)TempBuffer, RGB(1, 1, 1));
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, siCXPos, siCYPos, (char*)TempBuffer, RGB(255, 255, 255));
			break;
		}
		
		MiniMapBack->ReleaseDC(hDC);
	}	
	
	if(clGrp.LockSurface(MiniMapBack) == TRUE) 
	{
		// 미니맵에서 선택한 Target출력
		switch(m_siStatus)
		{
		case ON_MINIMAP_VILLAGE:
			if(m_SelectTargetVillageFlag)
			{
				// 모든 마을 출력
				for(TempCount = 0; TempCount < m_TargetList[0].m_siMaxCount ; TempCount++)
				{
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[0].m_pList[TempCount].m_PositionX , 
						m_TargetList[0].m_pList[TempCount].m_PositionY, siTempX, siTempY);
					CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
					
					siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
					siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
					
					if(m_Percent <= 1)
					{
						if(m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_WHARF])		// 항구도시라면
							clGrp.PutSpriteT(siCXPos - WharfVillageSpr.Header.Xsize/2, siCYPos - WharfVillageSpr.Header.Ysize/2 , 
							WharfVillageSpr.Header.Xsize, WharfVillageSpr.Header.Ysize, &WharfVillageSpr.Image[WharfVillageSpr.Header.Start[0]]);
						else 
						{
							clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , VillageSpr.Header.Xsize, 
								VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[m_TargetList[0].m_pList[TempCount].m_Size]]);
						}
					}
					else
					{
						clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , 
							VillageSpr.Header.Xsize, VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[2]]);
					}
				}
				
				// 선택한 마을만 출력
				CheckScrollPercent(m_SelectTargetX, m_SelectTargetY, TempCXPercent, TempCYPercent);
				
				siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
				siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
				clGrp.PutSpriteT(siCXPos - TargetSpr.Header.Xsize/2, siCYPos - TargetSpr.Header.Ysize, 
					TargetSpr.Header.Xsize, TargetSpr.Header.Ysize, &TargetSpr.Image[TargetSpr.Header.Start[0]]);
			}
			else
			{
				// 모든 마을 출력
				for(TempCount = 0; TempCount < m_TargetList[0].m_siMaxCount ; TempCount++)
				{
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[0].m_pList[TempCount].m_PositionX , 
						m_TargetList[0].m_pList[TempCount].m_PositionY, siTempX, siTempY);
					CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
					
					siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
					siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
					
					if(m_Percent <= 1 || !siFullMode)
					{
						if(m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_WHARF])		// 항구도시라면
							clGrp.PutSpriteT(siCXPos - WharfVillageSpr.Header.Xsize/2, siCYPos - WharfVillageSpr.Header.Ysize/2 , 
							WharfVillageSpr.Header.Xsize, WharfVillageSpr.Header.Ysize, &WharfVillageSpr.Image[0]);
						else 
						{
							clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , VillageSpr.Header.Xsize, 
								VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[m_TargetList[0].m_pList[TempCount].m_Size]]);
						}
					}
					else
					{
						clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , 
							VillageSpr.Header.Xsize, VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[2]]);
					}
				}
			}
			break;
		case ON_MINIMAP_ITEM:
			// 선택한 아이템이 있는 Target 출력
			if(m_SelectTargetItemFlag && m_SelectTargetItemCount)
			{		
				for(TempCount = 0; TempCount < m_SelectTargetItemCount; TempCount++)
				{
					CheckScrollPercent(m_SelectTargetItemData[TempCount].m_TargetX, m_SelectTargetItemData[TempCount].m_TargetY, TempCXPercent, TempCYPercent);
					
					siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
					siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
					
					if(m_Percent <= 1 || !siFullMode)
					{
						if(m_SelectTargetItemData[TempCount].m_bWharf)		// 항구도시라면
							clGrp.PutSpriteT(siCXPos - WharfVillageSpr.Header.Xsize/2, siCYPos - WharfVillageSpr.Header.Ysize/2 , 
							WharfVillageSpr.Header.Xsize, WharfVillageSpr.Header.Ysize, &WharfVillageSpr.Image[0]);
						else 
						{
							clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , VillageSpr.Header.Xsize, 
								VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[m_SelectTargetItemData[TempCount].m_Size]]);
						}
					}
					else
					{
						clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , VillageSpr.Header.Xsize, 
							VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[2]]);
					}
				}
			}
			break;
		case ON_MINIMAP_STRUCT:
			// 선택된 건물 출력
			if(m_SelectTargetstructFlag && m_SelectTargetstructCount)
			{
				for(TempCount = 0; TempCount < m_SelectTargetstructCount; TempCount++)
				{
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_SelectTargetstructData[TempCount].m_PositionX , 
						m_SelectTargetstructData[TempCount].m_PositionY, siTempX, siTempY);
					CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
					
					siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
					siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
					
					if(m_Percent <= 1 || !siFullMode)
					{
						if(m_SelectTargetstructData[TempCount].m_IsStructure[ON_VIL_ST_KIND_WHARF])		// 항구도시라면
							clGrp.PutSpriteT(siCXPos - WharfVillageSpr.Header.Xsize/2, siCYPos - WharfVillageSpr.Header.Ysize/2 , 
							WharfVillageSpr.Header.Xsize, WharfVillageSpr.Header.Ysize, &WharfVillageSpr.Image[0]);
						else 
						{
							clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , VillageSpr.Header.Xsize, 
								VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[m_SelectTargetstructData[TempCount].m_Size]]);
						}
					}
					else
					{
						clGrp.PutSpriteT(siCXPos - VillageSpr.Header.Xsize/2, siCYPos - VillageSpr.Header.Ysize/2 , VillageSpr.Header.Xsize, 
							VillageSpr.Header.Ysize, &VillageSpr.Image[VillageSpr.Header.Start[2]]);
					}
				}
			}
			break;
		case ON_MINIMAP_DUNGEON:
		case ON_MINIMAP_SHORTCUT:
		case ON_MINIMAP_NPC:
			// 특수지형 출력
			for(TempCount = 0; TempCount < m_TargetList[2].m_siMaxCount ; TempCount++)
			{
				pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[2].m_pList[TempCount].m_PositionX , 
					m_TargetList[2].m_pList[TempCount].m_PositionY, siTempX, siTempY);
				CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
				
				siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
				siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
				
				if(m_TargetList[2].m_pList[TempCount].m_iDungeonCode == 3)
					clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
					CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[0]]);
				else
					clGrp.PutSpriteT(siCXPos - DungeonSpr.Header.Xsize/2, siCYPos - DungeonSpr.Header.Ysize/2 , 
					DungeonSpr.Header.Xsize, DungeonSpr.Header.Ysize, &DungeonSpr.Image[0]);
			}
			// 선택한 특수지형 가리키는 이미지
			CheckScrollPercent(m_SelectTargetX, m_SelectTargetY, TempCXPercent, TempCYPercent);
			
			siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
			siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
			clGrp.PutSpriteT(siCXPos - TargetSpr.Header.Xsize/2, siCYPos - TargetSpr.Header.Ysize, 
				TargetSpr.Header.Xsize, TargetSpr.Header.Ysize, &TargetSpr.Image[TargetSpr.Header.Start[0]]);

			break;
		case ON_MINIMAP_CHAR:
			// 찾고자 하는 캐릭터 출력
			pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX , 
				m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
			CheckScrollPercent(siTempX, siTempY, TempCXPercent, TempCYPercent);
			
			siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
			siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
			
			// robypark 2005/1/14 17:27
			// 주인공 관련 데이터 관리자 얻기
			CFieldHeroInfo *pFieldHeroInfo = NULL;
			if (pMyOnlineWorld->GetFieldHeroInfoMgr())
				pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(m_TargetList[4].m_pList[0].m_CharKind);

			if (pFieldHeroInfo)
			{
				clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
				CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[pFieldHeroInfo->GetMinmapIconFontNo()]]);
			}
			else if(m_TargetList[4].m_pList[0].m_CharKind == 23107 )	// 운영자	
			{
				clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
				CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[0]]);
			}

			break;
		}

		// 미니맵에서 현재 자신의 위치 출력
		GetHeroPosition(siCPXPos, siCPYPos);
		CheckScrollPercent(siCPXPos, siCPYPos, TempCXPercent, TempCYPercent);
 
		siCXPos = TempDestRect.left + (SI32)((float)(TempDestRect.right  - TempDestRect.left) * TempCXPercent);
		siCYPos = TempDestRect.top  + (SI32)((float)(TempDestRect.bottom - TempDestRect.top)  * TempCYPercent);
		
		// 사용자가 공성전 유닛이라면
		if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			// 변신전 사용자의 원래 캐릭터 종류에 따른 미니맵에 그리기
			// robypark 2005/1/14 17:27
			// 주인공 관련 데이터 관리자 얻기
			CFieldHeroInfo *pFieldHeroInfo = NULL;
			if (pMyOnlineWorld->GetFieldHeroInfoMgr())
				pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(pMyOnlineWorld->pMyData->GetPrevCharOnlineKind());

			if (pFieldHeroInfo)
			{
				clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
				CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[pFieldHeroInfo->GetMinmapIconFontNo()]]);
			}
			else if(pMyOnlineWorld->pMyData->GetPrevCharOnlineKind() == 23107 )		// 운영자	
				clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
				CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[0]]);
		}
		else	// 공성전 유닛이 아닌 경우
		{
			// robypark 2005/1/14 17:27
			// 주인공 관련 데이터 관리자 얻기
			CFieldHeroInfo *pFieldHeroInfo = NULL;
			if (pMyOnlineWorld->GetFieldHeroInfoMgr())
				pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(pMyOnlineWorld->pMyData->GetMyCharKind());

			if (pFieldHeroInfo)
			{
				clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
				CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[pFieldHeroInfo->GetMinmapIconFontNo()]]);
			}
			else if(pMyOnlineWorld->pMyData->GetMyCharKind() == 23107 )		// 운영자	
				clGrp.PutSpriteT(siCXPos - CharFaceSpr.Header.Xsize/2, siCYPos - CharFaceSpr.Header.Ysize/2 , 
				CharFaceSpr.Header.Xsize, CharFaceSpr.Header.Ysize, &CharFaceSpr.Image[CharFaceSpr.Header.Start[0]]);
		}

		clGrp.UnlockSurface(MiniMapBack);
	}
	
	// 출력한 것을 화면에 출력
	TempDestRect.left   = XPos;
	TempDestRect.top    = YPos;
	TempDestRect.right  = XPos + XSize;
	TempDestRect.bottom = YPos + YSize;
	TempSrcRect.left    = 0;
	TempSrcRect.top     = 0;
	TempSrcRect.right   = XSize;
	TempSrcRect.bottom  = YSize;

	// jin 수정!
	if(siFullMode)
	{
		if(clGrp.LockSurface(MiniMapBack) == TRUE)
		{
			clGrp.PutSpriteT(0, 0, MiniMapBorder.Header.Xsize, MiniMapBorder.Header.Ysize, MiniMapBorder.Image);
			clGrp.GetImage(0, 0, MiniMapBorder.Header.Xsize, MiniMapBorder.Header.Ysize, pImage);			
			clGrp.UnlockSurface(MiniMapBack);
		}

		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			clGrp.PutSpriteZin(TempDestRect.left, TempDestRect.top, MiniMapBorder.Header.Xsize, MiniMapBorder.Header.Ysize, pImage);			
			clGrp.UnlockSurface(pSurface);
		}		
	}
	else
	{
		if(pSurface->Blt(&TempDestRect, MiniMapBack, &TempSrcRect, DDBLT_WAIT , NULL) != DD_OK)
		{
			// actdoll (2004/07/09 11:09) : 만약 본맵과 관련된 데이터가 왔을 경우 중국용 미니맵을 따로 갱신한다.
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_CHINA )	LoadMiniMap("Online\\Map\\Minimap_China.Spr",	m_MapIndex);
			else													LoadMiniMap("Online\\Map\\Minimap.Spr",			m_MapIndex);
			
			m_Percent	= pMyOnlineWorld->pConfig->GetMiniMapSIze();
			return;
		}
	}

	clGrp.SetClipArea(0, 0, 799, 599);
}

/********************************************************************************************
	활성화 시킬 것인가?
********************************************************************************************/
VOID	OnlineMiniMap::SetActive(BOOL bActive)
{
	bMouseDown = FALSE;
	m_bAction = bActive;
	
	m_YetHeroXPos = 0;
	m_YetHeroYPos = 0;
	
	if(bActive == TRUE) 
	{	
//		if(m_bVillage)
//			siFullMode = TRUE;

		if(siFullMode)
		{
			m_siX = 611;
			m_siY = 5;
		}
		else
		{
			m_siX = 0;
			m_siY = 0;
		}
		
		// 노 필드 지역을 설정한다.
		ChangePos(siFullMode);
		
		m_MovePositionFlag = FALSE;
		m_FanfLock		   = FALSE;
	}
	else
	{
		// 노 필드 지역을 해제한다.
		if(siNoFieldHandle != -1)
		{
			pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(siNoFieldHandle);
			siNoFieldHandle = -1;
		}
		
		//if(m_pTargetFindEditBox->IsFocus())
		if(Handler_OnlineEditControlEx::IsFocus(m_hOecFindEdit))
			RemoveEditFocus();
	}

	// 장부가 켜져 있으면
	if(pMyOnlineWorld->pOnlineTradeBook->IsAction() && ISFullMode() == TRUE)
		pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

	// 현재 마을에 있으면
	if(pMyOnlineWorld->pOnlineVillage)
		pMyOnlineWorld->pOnlineVillage->LeaveMiniMapFlag = bActive;
}

/********************************************************************************************
	Warp
********************************************************************************************/
BOOL	OnlineMiniMap::Warp(SI32 siX, SI32 siY)
{
	if(pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(siX, siY, m_MPointX, m_MPointY) == FALSE)
	{
		m_MPointX = 0;
		m_MPointY = 0;
		return FALSE;
	}

	m_MovePositionFlag = FALSE;
	m_FanfLock		   = FALSE;

	m_YetHeroXPos = m_MPointX;
	m_YetHeroYPos = m_MPointY;

	return TRUE;
}

/********************************************************************************************
	주인공 위치를 얻어옴
********************************************************************************************/
BOOL	OnlineMiniMap::GetHeroPosition(SI32& xPos, SI32& yPos)
{
	SI16		siHeroX;
	SI16		siHeroY;
	SI32		siHeroXI;
	SI32		siHeroYI;

	if(pMyOnlineWorld->pOnlinePannel->GetfIsVillage())
	{
		pMyOnlineWorld->pOnlinePannel->GetVillagePosition(siHeroXI, siHeroYI);

		if(pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(siHeroXI, siHeroYI, xPos, yPos) == FALSE)
		{
			xPos = 0;
			xPos = 0;
			return FALSE;
		}
	}
	else
	{
		pMyOnlineWorld->pIOnlineChar->GetPos(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID(), &siHeroX, &siHeroY);

		if(pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(siHeroX, siHeroY, xPos, yPos) == FALSE)
		{
			xPos = 0;
			xPos = 0;
			return FALSE;
		}
	}

	return TRUE;
}

/********************************************************************************************
	Edit Box로 Focus 이동
********************************************************************************************/
BOOL	OnlineMiniMap::SetEditFocus(void)
{
//	if(m_pTargetFindEditBox->IsFocus() == FALSE)
//	{
		// 현재 Focus 저장
//		m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
//		m_YetEditHWND         = GetFocus();
//		m_YetEditHWNDEnable   = IsWindowEnabled(m_YetEditHWND);

//		pMyOnlineWorld->SetShortKey(TRUE);
//		m_pTargetFindEditBox->EnableWindow(TRUE);
//		m_pTargetFindEditBox->SetFocus();

		//pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->EnableWindow(FALSE);
//	}
	if(Handler_OnlineEditControlEx::IsFocus(m_hOecFindEdit) == FALSE)
	{
		m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
		pMyOnlineWorld->SetShortKey(TRUE);
		Handler_OnlineEditControlEx::SetFocus( m_hOecFindEdit );
	}

	return TRUE;
}

/********************************************************************************************
	Edit Box Focus 해제
********************************************************************************************/
BOOL	OnlineMiniMap::RemoveEditFocus(void)
{
	//if(m_pTargetFindEditBox->IsFocus() == TRUE)
	if(Handler_OnlineEditControlEx::IsFocus(m_hOecFindEdit) == TRUE)
	{
		//pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->EnableWindow(TRUE);
		//pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->SetFocus();
		//m_pTargetFindEditBox->EnableWindow(FALSE);
		// Focus 지정
		//SetFocus(m_YetEditHWND);
		//EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

		//Handler_OnlineEditControlEx::SetFocus(pGame->Hwnd, pMyOnlineWorld->pOnlinePannel->GetHOECChat());
		Handler_OnlineEditControlEx::ClearCurrentFocus();
		pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
	}
	return TRUE;
}

/********************************************************************************************
	Edit Box를 사용하는지 Flag를 얻음
********************************************************************************************/
BOOL	OnlineMiniMap::GetfEditBox(void)
{
	//if(m_pTargetFindEditBox->IsFocus()) return TRUE;
	if(Handler_OnlineEditControlEx::IsFocus(m_hOecFindEdit) == TRUE)
		return TRUE;
	return FALSE;
}

/********************************************************************************************
	맵 좌표로 미니맵의 스크롤비율을 구한다.
********************************************************************************************/
BOOL	OnlineMiniMap::CheckScrollPercent(SI32 siXPos, SI32 siYPos, float& refXP, float& refYP)
{
	SI32		TempMapMaxX;
	SI32		TempMapMaxY;

	pMyOnlineWorld->pOnlineMap->GetMapRealSize(TempMapMaxX, TempMapMaxY);

	refXP = (float)siXPos / (float)TempMapMaxX;
	refYP = (float)siYPos / (float)TempMapMaxY;

	return TRUE;
}

/********************************************************************************************
	스크롤 비율로 맵 좌표를 구한다.
********************************************************************************************/
BOOL	OnlineMiniMap::CheckScrollPercent(float fXP, float fYP, SI32& resiXPos, SI32& resiYPos)
{
	SI32		TempMapMaxX;
	SI32		TempMapMaxY;

	pMyOnlineWorld->pOnlineMap->GetMapRealSize(TempMapMaxX, TempMapMaxY);
	resiXPos = (SI32)(fXP * (float)TempMapMaxX);
	resiYPos = (SI32)(fYP * (float)TempMapMaxY);

	return TRUE;
}

/********************************************************************************************
	미니맵을 선택하고 있는지 검사
********************************************************************************************/
BOOL	OnlineMiniMap::CheckMiniMapScreen(POINT* lpPoint)
{
	SI32 TempXP, TempYP, TempXS, TempYS;
	
	TempXP = m_siX;
	TempYP = m_siY;

	if(siFullMode)
	{
		TempXS = MINIMAP_SIZE_MIN_WIDTH;
		TempYS = MINIMAP_SIZE_MIN_HEIGHT;
	}
	else
	{
		TempXS = MINIMAP_SIZE_MAX_WIDTH;
		TempYS = MINIMAP_SIZE_MAX_HEIGHT;
	}

	if((TempXP < lpPoint->x) && ((TempXP + TempXS) >= lpPoint->x) && (TempYP < lpPoint->y) && ((TempYP + TempYS) >= lpPoint->y))
	{
		TempXP = m_BCharCenter.GetX();
		TempYP = m_BCharCenter.GetY();
		TempXS = m_BCharCenter.GetWidth();
		TempYS = m_BCharCenter.GetHeight();

		if((TempXP < lpPoint->x) && ((TempXP + TempXS) >= lpPoint->x) && (TempYP < lpPoint->y) && ((TempYP + TempYS) >= lpPoint->y))
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}

/********************************************************************************************
	Make Target List
********************************************************************************************/
BOOL	OnlineMiniMap::MakeTargetList(void)
{
	if(pMyOnlineWorld->pOnlineVillageSave == NULL) return FALSE;

	OnlineVillageParser*	lpOnlineVillageParser;
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempCount2;
	SI32					MaxCount;
	SAVEFILE_HEADER*		lpSAVEFILE_HEADER;
	SAVEFILE_DATAHEADER*	pData;
	VillageHeader*			TempVillageHeader;
	CItemHeader*			pItem;
	DungeonHeader*			pDungeonHeader;
	char*					lpText;


	//============================================================================
	// 마을 Target

	m_TargetList[0].m_siMaxCount = 0;

	// Village
	lpOnlineVillageParser = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser();
	MaxCount = lpOnlineVillageParser->GetTotalVillageNum();

	for(TempCount = 0; TempCount < MaxCount; TempCount++)
	{
		TempVillageHeader = lpOnlineVillageParser->GetVillageHeaderIndex(TempCount);
		if(TempVillageHeader->siMapIndex != m_MapIndex)		break;

		ZeroMemory(&m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount], sizeof(SMiniMapTargetData));
		strcpy((char*)m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(TempVillageHeader->siVillageName));
		m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_PositionX = TempVillageHeader->siX;
		m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_PositionY = TempVillageHeader->siY;
		m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_uiVillageCode = TempVillageHeader->uiVillageCode;

		// 건물 사이즈
		switch(TempVillageHeader->szSize) 
		{
		case 'L':
			m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_Size = 0;	break;
		case 'M':
			m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_Size = 1;	break;
		case 'S':
			m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_Size = 2;	break;
		}

		// 해당 건물 존재여부
		for(int i = 0; i < TOT_STRUCTURE_NUM; i++) {
			m_TargetList[0].m_pList[m_TargetList[0].m_siMaxCount].m_IsStructure[i] = lpOnlineVillageParser->IsStructure(TempCount, i);
		}
		m_TargetList[0].m_siMaxCount++;
	}

	// 우선 순위로 정렬
	qsort((void*)m_TargetList[0].m_pList, m_TargetList[0].m_siMaxCount, sizeof(SMiniMapTargetData), qsort_CompareForMiniMapTarget);

	//============================================================================
	// Item Target

	m_TargetList[1].m_siMaxCount = 0;

	MaxCount          = pMyOnlineWorld->pOnlineVillageSave->GetTotal() - 1;
	lpSAVEFILE_HEADER = pMyOnlineWorld->pOnlineVillageSave->GetData();
	if(lpSAVEFILE_HEADER)
	{
		for(TempCount = 0; TempCount < MaxCount; TempCount++)
		{
			pData = pMyOnlineWorld->pOnlineVillageSave->GetData(lpSAVEFILE_HEADER[TempCount+1].m_uiVillageCode);

			if(!pData)			continue;

			for(TempCount1 = 0; TempCount1 < pData->m_siItemNum; TempCount1++)
			{
				if(TempCount1 < 20)
				{
					if(!pMyOnlineWorld->pItemsInfo) continue;
					if(!(&pData->m_pData[TempCount1])) continue;
					if(!pData->m_pData[TempCount1].m_uiItemID) continue;

					pItem = pMyOnlineWorld->pItemsInfo->GetItem(pData->m_pData[TempCount1].m_uiItemID);
					if(!pItem)		continue;

					lpText = pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);
					if(!lpText)		continue;

					// 우선 현재 Item이 등록돼어 있는지 검사
					for(TempCount2 = 0; TempCount2 < m_TargetList[1].m_siMaxCount; TempCount2++)
					{
						if(strcmp((char*)lpText, (char*)m_TargetList[1].m_pList[TempCount2].m_TargetName) == 0)
						{
							// 등록돼어 있다면 무시
							break;
						}
					}
					
					// 만약 등록돼어 있지 않다면, List에 등록한다.
					if(TempCount2 == m_TargetList[1].m_siMaxCount)
					{
						ZeroMemory(&m_TargetList[1].m_pList[m_TargetList[1].m_siMaxCount], sizeof(SMiniMapTargetData));
						strcpy((char*)m_TargetList[1].m_pList[m_TargetList[1].m_siMaxCount].m_TargetName, lpText);
						
						m_TargetList[1].m_pList[m_TargetList[1].m_siMaxCount].m_ItemCode = pData->m_pData[TempCount1].m_uiItemID;
						
						m_TargetList[1].m_siMaxCount++;
					}
				}
			}
		}
	}

	// 우선 순위로 정렬
	qsort((void*)m_TargetList[1].m_pList, m_TargetList[1].m_siMaxCount, sizeof(SMiniMapTargetData), qsort_CompareForMiniMapTarget);

	//============================================================================
	// Dungeon Target

	m_TargetTemp.m_siMaxCount = 0;
	MaxCount = pMyOnlineWorld->pOnlineDungeon->GetDungeonNum();
	
	for(TempCount = 0; TempCount < MaxCount; TempCount++)
	{
		if(m_MapIndex == 0)
		{
			pDungeonHeader	= pMyOnlineWorld->pOnlineDungeon->GetDungeonHeader(TempCount);
			if( pDungeonHeader->siMapIndex == m_MapIndex)
			{
				ZeroMemory(&m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount], sizeof(SMiniMapTargetData));
				strcpy((char*)m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_TargetName, pDungeonHeader->szName);

				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionX = pDungeonHeader->siMapXPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionY = pDungeonHeader->siMapYPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_BattlePoint = pDungeonHeader->siBattlePoint;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_bQuestNPC = FALSE;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_iDungeonCode = pDungeonHeader->siSelect;
				m_TargetTemp.m_siMaxCount++;
			}
		}
	}
	//특수지형의 경우.
	if(m_MapIndex != 0) 
	{
		for(TempCount = 0; TempCount < MaxCount; TempCount++)
		{
			pDungeonHeader	= pMyOnlineWorld->pOnlineDungeon->GetDungeonHeader(TempCount);
			if( pDungeonHeader->siMapIndex == m_MapIndex)
			{
				ZeroMemory(&m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount], sizeof(SMiniMapTargetData));
				strcpy((char*)m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_TargetName, pDungeonHeader->szName);
				
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionX = pDungeonHeader->siMapXPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionY = pDungeonHeader->siMapYPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_BattlePoint = pDungeonHeader->siBattlePoint;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_bQuestNPC = FALSE;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_iDungeonCode = 4;
				m_TargetTemp.m_siMaxCount++;
			}
		}
		FindDungeonStructure(4);
	}

	MaxCount = pMyOnlineWorld->pOnlineMap->m_NPCParser.GetNPCNum();
	for(TempCount = 0; TempCount < MaxCount; TempCount++)
	{
		NPCIndexHeader *pNPCIndexHeader	= pMyOnlineWorld->pOnlineMap->m_NPCParser.GetNPCByIndex(TempCount);
		if(!pNPCIndexHeader)	continue;

		if(pNPCIndexHeader->siType == 1 && pNPCIndexHeader->siMapIndex == 0 && m_MapIndex == 0)
		{
			ZeroMemory(&m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount], sizeof(SMiniMapTargetData));
			strcpy((char*)m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(pNPCIndexHeader->siName));
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionX = pNPCIndexHeader->siMapXPos;
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionY = pNPCIndexHeader->siMapYPos;
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_bQuestNPC = TRUE;
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_iDungeonCode = 3;
			m_TargetTemp.m_siMaxCount++;
		}
		else if(pNPCIndexHeader->siType == 1 && pNPCIndexHeader->siMapIndex != 0)
		{
			if(m_MapIndex == 0)
			{
				SI16	siCode;
				switch(pNPCIndexHeader->siMapIndex)
				{
				case 1:	 case 3:  case 4:						siCode = 0;		break;
				case 5:	 case 6:  case 7:						siCode = 6;		break;
				case 8:	 case 9:  case 10: case 11: case 12:	siCode = 13;	break;
				case 13:										siCode = 21;	break;
				case 14: case 15: case 16:						siCode = 23;	break;
				case 17:										siCode = 30;	break;
				case 18:										siCode = 32;	break;
				case 19: case 20:								siCode = 41;	break;
				case 21: case 22:								siCode = 45;	break;
				case 23: case 24:								siCode = 49;	break;
				case 25:										siCode = 59;	break;
				case 26:										siCode = 62;	break;
				case 27:										siCode = 63;	break;
				case 43:										siCode = 124;	break;
				case 44:										siCode = 124;	break;
				}
				
				DungeonHeader*	pDungeonHeader = pMyOnlineWorld->pOnlineDungeon->GetDungeonHeader(siCode);
				ZeroMemory(&m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount], sizeof(SMiniMapTargetData));
				strcpy((char*)m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(pNPCIndexHeader->siName));
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionX = pDungeonHeader->siMapXPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionY = pDungeonHeader->siMapYPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_bQuestNPC = TRUE;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_iDungeonCode = 3;
				m_TargetTemp.m_siMaxCount++;
			}
			else if(m_MapIndex == pNPCIndexHeader->siMapIndex)
			{
				ZeroMemory(&m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount], sizeof(SMiniMapTargetData));
				strcpy((char*)m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(pNPCIndexHeader->siName));
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionX = pNPCIndexHeader->siMapXPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionY = pNPCIndexHeader->siMapYPos;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_bQuestNPC = TRUE;
				m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_iDungeonCode = 3;
				m_TargetTemp.m_siMaxCount++;
			}
		}
		else if(pNPCIndexHeader->siType == 3 && pNPCIndexHeader->siMapIndex == 0 && m_MapIndex == 0)
		{
			ZeroMemory(&m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount], sizeof(SMiniMapTargetData));
			strcpy((char*)m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(pNPCIndexHeader->siName));
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionX = pNPCIndexHeader->siMapXPos;
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_PositionY = pNPCIndexHeader->siMapYPos;
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_bQuestNPC = TRUE;
			m_TargetTemp.m_pList[m_TargetTemp.m_siMaxCount].m_iDungeonCode = 3;
			m_TargetTemp.m_siMaxCount++;
		}
	}

	//============================================================================
	// 건물 Target

	SI32 TempStructureKinds[TOT_STRUCTURE_NUM];
	m_TargetList[3].m_siMaxCount = 0;

	for(TempCount = 0; TempCount < m_TargetList[0].m_siMaxCount; TempCount++)
	{
		for(TempCount1 = 1; TempCount1 < TOT_STRUCTURE_NUM; TempCount1++)
		{
			if(m_TargetList[0].m_pList[TempCount].m_IsStructure[TempCount1])
			{
				// 우선 현재 건물이 등록돼어 있는지 검사
				for(TempCount2 = 0; TempCount2 < m_TargetList[3].m_siMaxCount; TempCount2++)
				{
					if(TempCount1 == TempStructureKinds[TempCount2])
					{
						// 등록돼어 있다면 무시
						break;
					}
				}
				// 만약 등록돼어 있지 않다면, List에 등록한다.
				if(TempCount2 == m_TargetList[3].m_siMaxCount)
				{
					switch(TempCount1) 
					{
					case ON_VIL_ST_KIND_BARRACK:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BARRACK ));		break;
					case ON_VIL_ST_KIND_MARKET:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MARKET ));		break;
					case ON_VIL_ST_KIND_BANK:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK ));			break;
					case ON_VIL_ST_KIND_HOSPITAL:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HOSPITAL ));		break;
					case ON_VIL_ST_KIND_INN:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INN ));			break;
					case ON_VIL_ST_KIND_WHARF:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WHARF ));		break;
					case ON_VIL_ST_KIND_FARM:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FARM ));			break;
					case ON_VIL_ST_KIND_GOVERNMENT:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_GOVERNMENT ));	break;
					case ON_VIL_ST_KIND_WSTORE:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WSTORE ));		break;
					case ON_VIL_ST_KIND_FSTORE:		
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FSTORE ));		break;
					case ON_VIL_ST_KIND_DSTORE:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_DSTORE ));		break;
					case ON_VIL_ST_KIND_WEAPON:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WEAPON ));		break;
					case ON_VIL_ST_KIND_ARMOR:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ARMOR ));		break;
					case ON_VIL_ST_KIND_PASTURE:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_PASTURE ));		break;
					case ON_VIL_ST_KIND_FACTORY:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FACTORY ));		break;
					case ON_VIL_ST_KIND_GBARRACK:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_GBARRACK ));		break;
					case ON_VIL_ST_KIND_MBARRACK:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MBARRACK ));		break;
					case ON_VIL_ST_KIND_RING:
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MBARRACK ));		break;
					default:	
						sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_UNKNOWN_STRUCTURE ));		break;
					}
					m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_StructCode = TempCount1;
					TempStructureKinds[m_TargetList[3].m_siMaxCount] = TempCount1;
					m_TargetList[3].m_siMaxCount++;
				}
			}
		}
	}
	// 생산시설 항목을 리스트에 추가
	sprintf(m_TargetList[3].m_pList[m_TargetList[3].m_siMaxCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_MAKE_STRUCT));
	m_TargetList[3].m_siMaxCount++;

	return TRUE;
}

/********************************************************************************************
	Change Pos
********************************************************************************************/
BOOL	OnlineMiniMap::ChangePos(BOOL fFullMode)
{
//	if(m_bVillage)
//		fFullMode = TRUE;

	siFullMode  = fFullMode;

	if(fFullMode)
	{
		m_siX = 611;
		m_siY = 5;
	}
	else
	{
		m_siX = 0;
		m_siY = 0;
	}

	m_siScroll = m_siSelect = 0;
	m_siScrollPos = 74;

	// Clip 지역 지정
	if(siNoFieldHandle != -1)
	{
		pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(siNoFieldHandle);
		siNoFieldHandle = -1;
	}
	if(fFullMode)
		siNoFieldHandle = pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(611, 5, 611 + 186, 5 + 184-1);
	else
		siNoFieldHandle = pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 599);

	if(pMyOnlineWorld->pOnlineInventory->IsActive()) pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

	return TRUE;
}


/********************************************************************************************
	Find Target Iten
********************************************************************************************/
BOOL	OnlineMiniMap::FindTargetItem(UI16 ItemCode)
{
	OnlineVillageParser*	lpOnlineVillageParser;
	SI32					TempCount;
	SI32					TempCount1;
	SI32					MaxCount;
	SAVEFILE_HEADER*		lpSAVEFILE_HEADER;

	SAVEFILE_DATAHEADER*	pData;
	VillageHeader*			TempVillageHeader;

	m_SelectTargetItemFlag  = TRUE;
	m_SelectTargetItemCount = 0;

	if(pMyOnlineWorld->pOnlineVillageSave == NULL) return FALSE;

	//============================================================================
	// Item Target
	MaxCount              = pMyOnlineWorld->pOnlineVillageSave->GetTotal() - 1;
	lpSAVEFILE_HEADER     = pMyOnlineWorld->pOnlineVillageSave->GetData();
	lpOnlineVillageParser = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser();

	if(!lpSAVEFILE_HEADER)		return FALSE;

	for(TempCount = 0; TempCount < MaxCount; TempCount++)
	{
		pData             = pMyOnlineWorld->pOnlineVillageSave->GetData(lpSAVEFILE_HEADER[TempCount+1].m_uiVillageCode);
		TempVillageHeader = lpOnlineVillageParser->GetVillageHeaderCode(lpSAVEFILE_HEADER[TempCount+1].m_uiVillageCode);
		
		if(!pData)				return FALSE;
		if(!TempVillageHeader)	return FALSE;

		for(TempCount1 = 0; TempCount1 < pData->m_siItemNum; TempCount1++)
		{
			if(pData->m_pData[TempCount1].m_uiItemID == ItemCode)
			{
				// List에 추가
				ZeroMemory(&m_SelectTargetItemData[m_SelectTargetItemCount], sizeof(SMiniMapTargetDataV));
				strcpy((char*)m_SelectTargetItemData[m_SelectTargetItemCount].m_TargetName, pMyOnlineWorld->pOnlineText->Get(TempVillageHeader->siVillageName));
				
				pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(TempVillageHeader->siX, TempVillageHeader->siY, m_SelectTargetItemData[m_SelectTargetItemCount].m_TargetX, m_SelectTargetItemData[m_SelectTargetItemCount].m_TargetY);
				
				// 건물 사이즈
				switch(TempVillageHeader->szSize) 
				{
				case 'L':
					m_SelectTargetItemData[m_SelectTargetItemCount].m_Size = 0;	break;
				case 'M':
					m_SelectTargetItemData[m_SelectTargetItemCount].m_Size = 1;	break;
				case 'S':
					m_SelectTargetItemData[m_SelectTargetItemCount].m_Size = 2;	break;
				}
				
				// 항구 마을 판단
				m_SelectTargetItemData[m_SelectTargetItemCount].m_bWharf = false;
				for(int i = 0; i < ON_VILLAGE_MAX_BUILDING; i++) 
				{
					if(TempVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_WHARF)
						m_SelectTargetItemData[m_SelectTargetItemCount].m_bWharf = true;
				}
				
				m_SelectTargetItemCount++;
				break;
			}
		}
	}

	return TRUE;
}

/********************************************************************************************
	Find Target Structure
********************************************************************************************/
BOOL	OnlineMiniMap::FindTargetStructure(UI16 StructCode, BOOL bAllStruct)
{
	SI32 TempCount;
	m_SelectTargetstructFlag  = TRUE;
	m_SelectTargetstructCount = 0;

	for(TempCount = 0; TempCount < m_TargetList[0].m_siMaxCount; TempCount++)
	{
		if(bAllStruct)
		{
			if(m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_FARM] || m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_WEAPON] || 
				m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_ARMOR] || m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_PASTURE]|| 
				m_TargetList[0].m_pList[TempCount].m_IsStructure[ON_VIL_ST_KIND_FACTORY])
			{
				ZeroMemory(&m_SelectTargetstructData[m_SelectTargetstructCount], sizeof(SMiniMapTargetData));
				strcpy((char*)m_SelectTargetstructData[m_SelectTargetstructCount].m_TargetName, m_TargetList[0].m_pList[TempCount].m_TargetName);
				m_SelectTargetstructData[m_SelectTargetstructCount].m_PositionX = m_TargetList[0].m_pList[TempCount].m_PositionX;
				m_SelectTargetstructData[m_SelectTargetstructCount].m_PositionY = m_TargetList[0].m_pList[TempCount].m_PositionY;
				m_SelectTargetstructData[m_SelectTargetstructCount].m_Size = m_TargetList[0].m_pList[TempCount].m_Size;
				for(int i=0; i<TOT_STRUCTURE_NUM; i++) 
					m_SelectTargetstructData[m_SelectTargetstructCount].m_IsStructure[i] = m_TargetList[0].m_pList[TempCount].m_IsStructure[i];
				m_SelectTargetstructCount++;
			}
		}
		else
		{
			if(m_TargetList[0].m_pList[TempCount].m_IsStructure[StructCode])
			{
				ZeroMemory(&m_SelectTargetstructData[m_SelectTargetstructCount], sizeof(SMiniMapTargetData));
				strcpy((char*)m_SelectTargetstructData[m_SelectTargetstructCount].m_TargetName, m_TargetList[0].m_pList[TempCount].m_TargetName);
				m_SelectTargetstructData[m_SelectTargetstructCount].m_PositionX = m_TargetList[0].m_pList[TempCount].m_PositionX;
				m_SelectTargetstructData[m_SelectTargetstructCount].m_PositionY = m_TargetList[0].m_pList[TempCount].m_PositionY;
				m_SelectTargetstructData[m_SelectTargetstructCount].m_Size = m_TargetList[0].m_pList[TempCount].m_Size;
				for(int i=0; i<TOT_STRUCTURE_NUM; i++) 
					m_SelectTargetstructData[m_SelectTargetstructCount].m_IsStructure[i] = m_TargetList[0].m_pList[TempCount].m_IsStructure[i];
				m_SelectTargetstructCount++;
			}
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Find Target Structure
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL OnlineMiniMap::FindDungeonStructure(UI16 DungeonCode)
{
	SI32 TempCount;
	int m_SelectTargetDungeonCount = 0;

	for(TempCount = 0; TempCount < m_TargetTemp.m_siMaxCount; TempCount++)
	{
		if(m_TargetTemp.m_pList[TempCount].m_iDungeonCode == DungeonCode)
		{
			ZeroMemory(&m_TargetList[2].m_pList[m_SelectTargetDungeonCount], sizeof(SMiniMapTargetData));
			strcpy((char*)m_TargetList[2].m_pList[m_SelectTargetDungeonCount].m_TargetName, m_TargetTemp.m_pList[TempCount].m_TargetName);
			m_TargetList[2].m_pList[m_SelectTargetDungeonCount].m_PositionX = m_TargetTemp.m_pList[TempCount].m_PositionX;
			m_TargetList[2].m_pList[m_SelectTargetDungeonCount].m_PositionY = m_TargetTemp.m_pList[TempCount].m_PositionY;
			m_TargetList[2].m_pList[m_SelectTargetDungeonCount].m_iDungeonCode = DungeonCode;
			m_TargetList[2].m_pList[m_SelectTargetDungeonCount].m_BattlePoint = m_TargetTemp.m_pList[TempCount].m_BattlePoint;
			m_SelectTargetDungeonCount++;
			m_TargetList[2].m_siMaxCount      = m_SelectTargetDungeonCount;
			
		}
						
	}

	return TRUE;
}
/********************************************************************************************
	미니맵 확대
********************************************************************************************/
BOOL	OnlineMiniMap::MiniMap_AddView(void)
{
	m_Percent--;

	if(m_Percent <  0) m_Percent = 0;
	if(m_Percent >= 8) m_Percent = 8;

	return TRUE;
}

/********************************************************************************************
	미니맵 축소
********************************************************************************************/
BOOL	OnlineMiniMap::MiniMap_SubTractView(void)
{
	m_Percent++;

	if(m_Percent <  0) m_Percent = 0;
	if(m_Percent >= 8) m_Percent = 8;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	VillageSearch을 퀵소트하기 위한 함수이다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INT		qsort_CompareForMiniMapTarget(const void* arg1, const void* arg2)
{
	SMiniMapTargetData			*pSMiniMapTargetData1;
	SMiniMapTargetData			*pSMiniMapTargetData2;
	int							TempResult;

	pSMiniMapTargetData1 = (SMiniMapTargetData*)arg1;
	pSMiniMapTargetData2 = (SMiniMapTargetData*)arg2;
		
	TempResult = strcmp((char*)pSMiniMapTargetData1->m_TargetName, (char*)pSMiniMapTargetData2->m_TargetName);
	if(TempResult < 0)
	{
		return -1;
	}
	else if(TempResult > 0)
	{
		return 1;
	}

	return 0;
}

/********************************************************************************************
	Make Character Target 
********************************************************************************************/
BOOL	OnlineMiniMap::MakeCharTarget(UI16 status, UI16 charkind, UI16 PosX, UI16 PosY, UI16 MapID, UI16 VillageCode)
{
	VillageHeader*		TempVillageHeader;
	MapIndexHeader*		pTempMapIndexHeader;
	PortalHeader*		pTempPortalHeader;
	SI32				siTempX, siTempY;
	m_TargetList[4].m_siMaxCount = 1;

	//ㅡ,.ㅡ
	/////////////////////////////////////////////
	if( (PosX >= 1470 && PosX <= 1500) 
		&& (PosY >= 1048 && PosY <= 1084))
		status = ON_RET_NOT_CONNECTED;
	if( (PosX >= 1687 && PosX <= 1730) 
		&& (PosY >= 548 && PosY <= 600))
		status = ON_RET_NOT_CONNECTED;

	if(MapID == 43)
		status = ON_RET_NOT_CONNECTED;
	/////////////////////////////////////////////

	switch(status) 
	{
	case ON_RET_ON_FIELD:
		{
			ZeroMemory(&m_TargetList[4].m_pList[0], sizeof(SMiniMapTargetData));
			strcpy((char*)m_TargetList[4].m_pList[0].m_TargetName, m_FindCharName);
			m_TargetList[4].m_pList[0].m_CharKind  = charkind;
			
			ZeroMemory(m_CharStatus, 64);
			if(MapID == 0)
			{
				if(PosX == 0 && PosY == 0)
				{
					strcpy((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_FIND_CHAR));
				}
				else
				{
					m_TargetList[4].m_pList[0].m_PositionX = PosX;
					m_TargetList[4].m_pList[0].m_PositionY = PosY;
					strcpy((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_USERFIELD));
					m_MovePositionFlag = TRUE;
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX ,
						m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
					m_MovePositionX    = siTempX;
					m_MovePositionY    = siTempY;
				}
			}
			else
			{
				pTempMapIndexHeader	= pMyOnlineWorld->pOnlineMap->MapIndexParser.GetMapIndexData(MapID);
				pTempPortalHeader	= pMyOnlineWorld->pOnlineMap->PortalParser.GetPortal(pTempMapIndexHeader->siPortalCode);
				m_TargetList[4].m_pList[0].m_PositionX = pTempPortalHeader->siPortalMapXPosition;
				m_TargetList[4].m_pList[0].m_PositionY = pTempPortalHeader->siPortalMapYPosition;
				
				m_MovePositionFlag = TRUE;
				pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX ,
					m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
				m_MovePositionX    = siTempX;
				m_MovePositionY    = siTempY;
				
				strcpy((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(pTempMapIndexHeader->siNameCode));
			}
		}
		break;
		
	case ON_RET_IN_BATTLE:
		{
			ZeroMemory(&m_TargetList[4].m_pList[0], sizeof(SMiniMapTargetData));
			strcpy((char*)m_TargetList[4].m_pList[0].m_TargetName, m_FindCharName);
			m_TargetList[4].m_pList[0].m_CharKind  = charkind;
			
			ZeroMemory(m_CharStatus, 64);
			if(MapID == 0)
			{
				m_TargetList[4].m_pList[0].m_PositionX = PosX;
				m_TargetList[4].m_pList[0].m_PositionY = PosY;
				strcpy((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BATTLE_IN_FIELD));
				m_MovePositionFlag = TRUE;
				pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX ,
					m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
				m_MovePositionX    = siTempX;
				m_MovePositionY    = siTempY;
			}
			else
			{
				pTempMapIndexHeader	= pMyOnlineWorld->pOnlineMap->MapIndexParser.GetMapIndexData(MapID);
				pTempPortalHeader	= pMyOnlineWorld->pOnlineMap->PortalParser.GetPortal(pTempMapIndexHeader->siPortalCode);
				m_TargetList[4].m_pList[0].m_PositionX = pTempPortalHeader->siPortalMapXPosition;
				m_TargetList[4].m_pList[0].m_PositionY = pTempPortalHeader->siPortalMapYPosition;
				
				m_MovePositionFlag = TRUE;
				pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX ,
					m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
				m_MovePositionX    = siTempX;
				m_MovePositionY    = siTempY;
				
				sprintf((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BATTLE_IN_SOMEWHERE), pMyOnlineWorld->pOnlineText->Get(pTempMapIndexHeader->siNameCode));
			}
		}
		break;

	case ON_RET_NOT_CONNECTED:
		ZeroMemory(m_CharStatus, sizeof(m_CharStatus));
		strcpy((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_NOTLOGIN));
		break;

	case ON_RET_IN_VILLAGE:
		TempVillageHeader = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(VillageCode); 
		ZeroMemory(&m_TargetList[4].m_pList[0], sizeof(SMiniMapTargetData));
		strcpy((char*)m_TargetList[4].m_pList[0].m_TargetName, m_FindCharName);
		m_TargetList[4].m_pList[0].m_PositionX = TempVillageHeader->siX;
		m_TargetList[4].m_pList[0].m_PositionY = TempVillageHeader->siY;
		m_TargetList[4].m_pList[0].m_CharKind  = charkind;
		ZeroMemory(m_CharStatus, sizeof(m_CharStatus));
		sprintf((char*)m_CharStatus, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_VILIN), pMyOnlineWorld->pOnlineText->Get(TempVillageHeader->siVillageName));
		m_MovePositionFlag = TRUE;
		pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[4].m_pList[0].m_PositionX ,
			m_TargetList[4].m_pList[0].m_PositionY, siTempX, siTempY);
		m_MovePositionX    = siTempX;
		m_MovePositionY    = siTempY;
		break;
	}

	return TRUE;
}

BOOL	OnlineMiniMap::ISFullMode()
{
	BOOL	tempMode;
	if(siFullMode)
		tempMode = FALSE;
	else
		tempMode = TRUE;

	return tempMode;
}

VOID OnlineMiniMap::ChangeMode(BOOL FullMode)
{
	if(FullMode)
		ChangePos(FALSE);
	else
		ChangePos(TRUE);
}

VOID OnlineMiniMap::MovePosition()
{
	m_MovePositionFlag = TRUE;
	GetHeroPosition(m_MovePositionX, m_MovePositionY);
}

VOID OnlineMiniMap::ActionCommon()
{
	if(m_MapIndex)
	{
		FindDungeonStructure(1);
		m_siStatus = ON_MINIMAP_DUNGEON;

		m_BVillage.SetAction(FALSE);
		m_BItem.SetAction(FALSE);
		m_BStruct.SetAction(FALSE);
		m_BChar.SetAction(FALSE);
		m_BDungeon.SetAction(FALSE);
		m_BShortCut.SetAction(FALSE);
		m_BNpc.SetAction(FALSE);
	}

	pMyOnlineWorld->pIOnlineMouse->GetPos(&m_ptMouse);
	pMyOnlineWorld->pOnlineMap->GetMapRealSize(m_siXsize, m_siYsize);

	// Target의 위치 조정
	if(m_MPointX <  0)       m_MPointX = 0;
	if(m_MPointY <  0)       m_MPointY = 0;
	if(m_MPointX >= m_siXsize) m_MPointX = m_siXsize - 1;
	if(m_MPointY >= m_siYsize) m_MPointY = m_siYsize - 1;

	if(siFullMode)
	{
		// 큰미니맵으로 전환
		if(m_BBigmap.Process(bMouseDown) == TRUE)
		{
			if(pMyOnlineWorld->pOnlineTradeBook->IsAction() )
				pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
			ChangePos(FALSE);
		}
		else if(m_BSEnlarge.Process(bMouseDown) == TRUE)
			m_Percent -= 2;
		else if(m_BSReduce.Process(bMouseDown) == TRUE)
			m_Percent += 2;
		
		if(m_Percent <  0) m_Percent = 0;
		if(m_Percent >= 8) m_Percent = 8;

		MinimapMove();
		pMyOnlineWorld->pConfig->SetMiniMapSize(m_Percent);
		
		if(IpD.LeftPressSwitch) bMouseDown = TRUE;
		else				    bMouseDown = FALSE;
		return;
	}
	
	// Target Edit Box
	if(bMouseDown)
	{
		// 스크롤바 처리
		ActionMapScroll();
		
		if(m_ptMouse.x >= (m_siX + 652) && m_ptMouse.x <= (m_siX + 746) && m_ptMouse.y >= (m_siY + 40) && m_ptMouse.y <= (m_siY + 55))
			SetEditFocus();
		else
			RemoveEditFocus();
	}
	
	// Main 메뉴
	if(m_BVillage.Process(bMouseDown) == TRUE)			// 마을
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_VILLAGE);
		m_siStatus = ON_MINIMAP_VILLAGE;
		m_siSubStatus = ON_MINIMAP_SUB_INIT;
	}
	else if(m_BItem.Process(bMouseDown) == TRUE)		// 아이템
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_ITEM);
		m_siStatus = ON_MINIMAP_ITEM;
	}
	else if(m_BStruct.Process(bMouseDown) == TRUE)		// 건물
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_STRUCT);
		m_siStatus = ON_MINIMAP_STRUCT;
	}
	else if(m_BDungeon.Process(bMouseDown) == TRUE)		// 특수지형
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_DUNGEON);
		FindDungeonStructure(1);
		m_siStatus = ON_MINIMAP_DUNGEON;
	}
	else if(m_BShortCut.Process(bMouseDown) == TRUE)	// 지름길
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_SHORTCUT);
		FindDungeonStructure(2);
		m_siStatus = ON_MINIMAP_SHORTCUT;
	}
	else if(m_BNpc.Process(bMouseDown) == TRUE)			// 임무수행원
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_NPC);
		FindDungeonStructure(3);
		m_siStatus = ON_MINIMAP_NPC;
	}
	else if(m_BChar.Process(bMouseDown) == TRUE)		// 캐릭터검색
	{
		ButtonStatus(m_siStatus, ON_MINIMAP_CHAR);
		m_siStatus = ON_MINIMAP_CHAR;
	}
	
	// Sub 메뉴
	if(m_BCharCenter.Process(bMouseDown) == TRUE)
	{
		m_MovePositionFlag = TRUE;
		GetHeroPosition(m_MovePositionX, m_MovePositionY);
	}
	else if(m_BBEnlarge.Process(bMouseDown) == TRUE)
		m_Percent -= 2;
	else if(m_BBReduce.Process(bMouseDown) == TRUE)
		m_Percent += 2;
	if(m_Percent <  0) m_Percent = 0;
	if(m_Percent >= 8) m_Percent = 8;
	
	// Mini Mode 버튼
	if(m_BExit.Process(bMouseDown) == TRUE)
		ChangePos(TRUE);
	
	pMyOnlineWorld->pConfig->SetMiniMapSize(m_Percent);
}

VOID OnlineMiniMap::ActionMapScroll()
{
	SI32 TempCount;
	float TempXFloat, TempYFloat;
	BOOL fChange = FALSE;
	
	if(m_BScrBut[0].Process(bMouseDown) == TRUE)
	{
		CheckScrollPercent(m_MPointX, m_MPointY, TempXFloat, TempYFloat);
		
		TempYFloat -= 0.02f;
		if(TempYFloat < 0.0f) TempYFloat = 0.0f;
		if(TempYFloat > 1.0f) TempYFloat = 1.0f;
		
		CheckScrollPercent(TempXFloat, TempYFloat, m_MPointX, m_MPointY);
	}
	else if(m_BScrBut[1].Process(bMouseDown) == TRUE)
	{
		CheckScrollPercent(m_MPointX, m_MPointY, TempXFloat, TempYFloat);
		
		TempYFloat += 0.02f;
		if(TempYFloat < 0.0f) TempYFloat = 0.0f;
		if(TempYFloat > 1.0f) TempYFloat = 1.0f;
		
		CheckScrollPercent(TempXFloat, TempYFloat, m_MPointX, m_MPointY);
	}
	else if(m_BScrBut[2].Process(bMouseDown) == TRUE)
	{
		CheckScrollPercent(m_MPointX, m_MPointY, TempXFloat, TempYFloat);
		
		TempXFloat -= 0.02f;
		if(TempXFloat < 0.0f) TempXFloat = 0.0f;
		if(TempXFloat > 1.0f) TempXFloat = 1.0f;
		
		CheckScrollPercent(TempXFloat, TempYFloat, m_MPointX, m_MPointY);
	}
	else if(m_BScrBut[3].Process(bMouseDown) == TRUE)
	{
		CheckScrollPercent(m_MPointX, m_MPointY, TempXFloat, TempYFloat);
		
		TempXFloat += 0.02f;
		if(TempXFloat < 0.0f) TempXFloat = 0.0f;
		if(TempXFloat > 1.0f) TempXFloat = 1.0f;
		
		CheckScrollPercent(TempXFloat, TempYFloat, m_MPointX, m_MPointY);
	}

	CheckScrollPercent(m_MPointX, m_MPointY, TempXFloat, TempYFloat);
	
	if((m_ptMouse.x >= (m_siX + 27)) && (m_ptMouse.x <= (m_siX + 618)) && (m_ptMouse.y >= (m_siY + 480)) && (m_ptMouse.y <= (m_siY + 489)))
	{
		TempCount = m_ptMouse.x;
		if(TempCount < (m_siX + 27))	TempCount = m_siX + 27;
		if(TempCount > (m_siX + 618))	TempCount = m_siX + 618;
		
		TempCount  = TempCount - (m_siX + 27);
		TempXFloat = (float)TempCount / 591.0f;
		fChange    = TRUE;
		
		m_MovePositionFlag = FALSE;
	}
	
	if((m_ptMouse.x >= (m_siX + 627)) && (m_ptMouse.x <= (m_siX + 636)) && (m_ptMouse.y >= (m_siY + 74)) && (m_ptMouse.y <= (m_siY + 471)))
	{
		TempCount = m_ptMouse.y;
		if(TempCount < (m_siY + 74))  TempCount = (m_siY + 74);
		if(TempCount > (m_siY + 471)) TempCount = (m_siY + 471);
		
		TempCount  = TempCount - (m_siY + 74);
		TempYFloat = (float)TempCount / 397.0f;
		fChange    = TRUE;
		
		m_MovePositionFlag = FALSE;
	}
	
	if(fChange)
		CheckScrollPercent(TempXFloat, TempYFloat, m_MPointX, m_MPointY);
}

VOID OnlineMiniMap::SelectTarget(SI16 siType)
{
	SI16 siYCount, TempSelect;
	for(siYCount = 0; siYCount < MINIMAP_LIST_MAX_LINE; siYCount++)
	{
		if(m_ptMouse.x >= m_siX+647 && m_ptMouse.x < m_siX+773 && m_ptMouse.y >= (m_siY+75 + siYCount * 20) && m_ptMouse.y < (m_siY+75 + (siYCount + 1) * 20))
		{
			TempSelect = siYCount + m_siScroll;
			if(TempSelect >= m_TargetList[siType].m_siMaxCount)		continue;

			if(m_siSelect != TempSelect)
			{
				m_siSelect = TempSelect;
				if(m_siStatus == ON_MINIMAP_VILLAGE && m_siSubStatus == ON_MINIMAP_SUB_ITEMINFO)
				{
					// 장부데이터를 받아온다.
					m_pVillageData = pMyOnlineWorld->pOnlineVillageSave->GetData(m_TargetList[0].m_pList[m_siSelect].m_uiVillageCode);
					m_siSubScroll = m_siSubSelect = 0;
					m_siSubScrollPos = m_siSubY + 49;
				}
				break;
			}
			else
				return;
		}
	}
	if(siYCount == MINIMAP_LIST_MAX_LINE)
		return;

	switch(siType)
	{
	case 0:
		m_SelectTargetVillageFlag = TRUE;
		
		pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[siType].m_pList[m_siSelect].m_PositionX
			, m_TargetList[siType].m_pList[m_siSelect].m_PositionY, m_SelectTargetX, m_SelectTargetY);
		
		m_MovePositionFlag = TRUE;
		m_MovePositionX    = m_SelectTargetX;
		m_MovePositionY    = m_SelectTargetY;
		break;
	case 1:
		FindTargetItem(m_TargetList[siType].m_pList[m_siSelect].m_ItemCode);
		break;
	case 2:
		pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[siType].m_pList[m_siSelect].m_PositionX
			, m_TargetList[siType].m_pList[m_siSelect].m_PositionY, m_SelectTargetX, m_SelectTargetY);
		
		m_MovePositionFlag = TRUE;
		m_MovePositionX    = m_SelectTargetX;
		m_MovePositionY    = m_SelectTargetY;
		break;
	case 3:
		if(m_siSelect == (m_TargetList[siType].m_siMaxCount-1))
			FindTargetStructure(m_TargetList[siType].m_pList[m_siSelect].m_StructCode, TRUE);
		else
			FindTargetStructure(m_TargetList[siType].m_pList[m_siSelect].m_StructCode);
		break;
	}
}

VOID OnlineMiniMap::ActionTargetScroll(SI16 siType)
{
	SI16 siHeight, siListCount;

	siListCount = m_TargetList[siType].m_siMaxCount;

	// 스크롤 처리
	if(siListCount > MINIMAP_LIST_MAX_LINE)
	{
		if(m_BTargetScrBut[0].Process(bMouseDown))
		{
			m_siScroll--;
			if(m_siScroll < 0)		m_siScroll = 0;
			
			siHeight = (SI16)(m_rtScroll.bottom - m_rtScroll.top - m_ScrHandle.Header.Ysize);
			m_siScrollPos = m_siY + 74 + (SI16)(((float)siHeight / (float)(siListCount - MINIMAP_LIST_MAX_LINE)) * (float)m_siScroll);
		}		
		else if(m_BTargetScrBut[1].Process(bMouseDown))
		{
			m_siScroll++;
			if(m_siScroll > (siListCount - MINIMAP_LIST_MAX_LINE))	m_siScroll = siListCount - MINIMAP_LIST_MAX_LINE;

			siHeight		=	(SI16)((m_rtScroll.bottom - m_rtScroll.top - m_ScrHandle.Header.Ysize));
			m_siScrollPos = m_siY + 74 + (SI16)(((float)siHeight / (float)(siListCount - MINIMAP_LIST_MAX_LINE))  * (float)m_siScroll);
		}
		else if(PtInRect(&m_rtScroll, m_ptMouse) == TRUE)
		{
			siHeight		=	(SI16)(m_rtScroll.bottom - m_rtScroll.top - m_ScrHandle.Header.Ysize);
			m_siScrollPos = m_ptMouse.y;
			if(m_siScrollPos > (m_rtScroll.bottom - m_ScrHandle.Header.Ysize))
				m_siScrollPos = m_rtScroll.bottom - m_ScrHandle.Header.Ysize;

			m_siScroll = max(0, (m_siScrollPos - (m_siY + 74)) / ((float)siHeight / (float)(siListCount - MINIMAP_LIST_MAX_LINE)));
		}
	}
}

VOID OnlineMiniMap::ActionEditBox(SI16 siType)
{
	//m_pTargetFindEditBox->GetString(m_TargetFindEditBoxName);
	OnlineEditControlEx	*pOecEx;
	pOecEx	= Handler_OnlineEditControlEx::GetControl(m_hOecFindEdit);
	strcpy(m_TargetFindEditBoxName,pOecEx->GetString());


	BOOL bFindAction = FALSE;
	//if(m_pTargetFindEditBox->IsDoneMessage() == TRUE && m_pTargetFindEditBox->IsFocus() == TRUE)
	if(pOecEx->IsPushEnter() == TRUE && Handler_OnlineEditControlEx::IsFocus(m_hOecFindEdit) == TRUE)
		bFindAction = TRUE;
	if(m_BFind.Process(bMouseDown) == TRUE)
		bFindAction = TRUE;
	if(!bFindAction)	return;

	// 캐릭터 검색
	if(siType == 4) 
	{
		if(m_TargetFindEditBoxName[0] == 0)		return;
		
		ZeroMemory(m_FindCharName, sizeof(m_FindCharName));
		strcpy((char*)m_FindCharName, (char*)m_TargetFindEditBoxName);
		
		pMyOnlineWorld->pOnlineClient->SendRPIInMinimap(m_FindCharName);
	}
	else
	{
		for(int i = 0; i < m_TargetList[siType].m_siMaxCount; i++)
		{
			if(strcmp((char*)m_TargetFindEditBoxName, (char*)m_TargetList[siType].m_pList[i].m_TargetName) == 0)
			{
				switch(siType)
				{
				case 0:
					m_SelectTargetVillageFlag = TRUE;
					pMyOnlineWorld->pOnlineMap->TransformMapXYToRealXY(m_TargetList[siType].m_pList[i].m_PositionX, m_TargetList[siType].m_pList[i].m_PositionY, m_SelectTargetX, m_SelectTargetY);
					m_MovePositionFlag = TRUE;
					m_MovePositionX    = m_SelectTargetX;
					m_MovePositionY    = m_SelectTargetY;
					break;
				case 1:
					FindTargetItem(m_TargetList[siType].m_pList[i].m_ItemCode);
					break;
				}

				// 스크롤 변경
				if(i > MINIMAP_LIST_MAX_LINE/2)
					m_siScroll = min(i-MINIMAP_LIST_MAX_LINE/2, m_TargetList[siType].m_siMaxCount-MINIMAP_LIST_MAX_LINE);
				m_siSelect = i;
				SI16 siHeight = (SI16)((m_rtScroll.bottom - m_rtScroll.top - m_ScrHandle.Header.Ysize));
				m_siScrollPos = m_siY + 74 + (SI16)(((float)siHeight / (float)(m_TargetList[siType].m_siMaxCount - MINIMAP_LIST_MAX_LINE))  * (float)m_siScroll);
				break;
			}
		}
	}
	// Edit Box 초기화
	//m_pTargetFindEditBox->SetString("");
	pOecEx->Clear();
	ZeroMemory(m_TargetFindEditBoxName, 65);
	RemoveEditFocus();
	//m_pTargetFindEditBox->InitDoneMessage();
}

VOID OnlineMiniMap::ButtonStatus(SI32 siPreStatus, SI32 siNextStatus)
{
	m_siScroll = m_siSelect = 0;
	m_siScrollPos = 74;

	switch(siPreStatus)
	{
	case ON_MINIMAP_VILLAGE:	m_BVillage.SetAction(TRUE);		break;
	case ON_MINIMAP_ITEM:		m_BItem.SetAction(TRUE);		break;
	case ON_MINIMAP_STRUCT:		m_BStruct.SetAction(TRUE);		break;
	case ON_MINIMAP_DUNGEON:	m_BDungeon.SetAction(TRUE);		break;
	case ON_MINIMAP_SHORTCUT:	m_BShortCut.SetAction(TRUE);	break;
	case ON_MINIMAP_NPC:		m_BNpc.SetAction(TRUE);			break;
	case ON_MINIMAP_CHAR:		m_BChar.SetAction(TRUE);		break;
	}
	switch(siNextStatus)
	{
	case ON_MINIMAP_VILLAGE:	m_BVillage.SetAction(FALSE);	break;
	case ON_MINIMAP_ITEM:		m_BItem.SetAction(FALSE);		break;
	case ON_MINIMAP_STRUCT:		m_BStruct.SetAction(FALSE);		break;
	case ON_MINIMAP_DUNGEON:	m_BDungeon.SetAction(FALSE);	break;
	case ON_MINIMAP_SHORTCUT:	m_BShortCut.SetAction(FALSE);	break;
	case ON_MINIMAP_NPC:		m_BNpc.SetAction(FALSE);		break;
	case ON_MINIMAP_CHAR:		m_BChar.SetAction(FALSE);		break;
	}
}

VOID OnlineMiniMap::MinimapMove()
{
	SI32 TempNowHeroXPos, TempNowHeroYPos, TempsiXsize, TempsiYsize;

	if(bMouseDown && CheckMiniMapScreen(&m_ptMouse))
	{
		if(m_FanfLock)
		{
			// 이동
			m_MPointX -= (SI32)((float)(m_ptMouse.x - m_FanXPosition) * (1.0f + (float)(m_Percent) / 2.0f) * ((float)m_siXsize / (float)m_MiniMapWidth));
			m_MPointY -= (SI32)((float)(m_ptMouse.y - m_FanYPosition) * (1.0f + (float)(m_Percent) / 2.0f) * ((float)m_siYsize / (float)m_MiniMapHeight));

			m_FanXPosition = m_ptMouse.x;
			m_FanYPosition = m_ptMouse.y;
		}
		else
		{
			m_FanfLock     = TRUE;
			m_FanXPosition = m_ptMouse.x;
			m_FanYPosition = m_ptMouse.y;
		}

		m_MovePositionFlag = FALSE;
	}
	else
	{
		m_FanfLock = FALSE;

		if(m_MovePositionFlag == FALSE)
		{
			// 주인공이 출력돼는 위치가 미니맵 범위의 안쪽 반인지 검사
			GetHeroPosition(TempNowHeroXPos, TempNowHeroYPos);
			TempsiXsize = (SI32)((float)(198 / 2) * (1.0f + (float)(m_Percent) / 2.0f) * ((float)m_siXsize / (float)m_MiniMapWidth));
			TempsiYsize = (SI32)((float)(176 / 2) * (1.0f + (float)(m_Percent) / 2.0f) * ((float)m_siYsize / (float)m_MiniMapHeight));

			if(((m_YetHeroXPos >= (m_MPointX - TempsiXsize)) && (m_YetHeroXPos <= (m_MPointX + TempsiXsize)) && (m_YetHeroYPos >= (m_MPointY - TempsiYsize)) && (m_YetHeroYPos <= (m_MPointY + TempsiYsize))))
			{
				// 새로 이동하는 위치에 맞게 미니맵 이동
				m_MPointX += TempNowHeroXPos - m_YetHeroXPos;
				m_MPointY += TempNowHeroYPos - m_YetHeroYPos;
			}

			GetHeroPosition(m_YetHeroXPos, m_YetHeroYPos);
		}
	}

	if(m_MovePositionFlag)
	{
		if(abs(m_MovePositionX - m_MPointX) > 16)
			m_MPointX += (m_MovePositionX - m_MPointX) / 16;
		else
			m_MPointX = m_MovePositionX;
		
		if(abs(m_MovePositionY - m_MPointY) > 16)
			m_MPointY += (m_MovePositionY - m_MPointY) / 16;
		else
			m_MPointY = m_MovePositionY;
		
		if((abs(m_MovePositionX - m_MPointX) == 0) && (abs(m_MovePositionY - m_MPointY) == 0))
			m_MovePositionFlag = FALSE;
	}
}

VOID OnlineMiniMap::ActionVillage()
{
	switch(m_siSubStatus)
	{
	case ON_MINIMAP_SUB_INIT:
		{
			m_siSubScroll = m_siSubSelect = 0;
			m_siSubScrollPos = m_siSubY + 49;
			SetRect(&m_rtSubScroll, m_siSubX+292, m_siSubY+49, m_siSubX+292+12, m_siSubY+49+196);
			SetRect(&m_rtVillageHelp, m_siX+647, m_siY+70, m_siX+647+126, m_siY+70+380);
			
			m_siSubStatus = ON_MINIMAP_SUB_POLL;
		}
		break;
	case ON_MINIMAP_SUB_POLL:
		{
			MinimapMove();

			if(bMouseDown)
			{
				SelectTarget(0);
				ActionTargetScroll(0);
			}
			ActionEditBox(0);

			// 마을의 취급품목을 볼수있는 Help를 띄워준다.
			if(PtInRect(&m_rtVillageHelp, m_ptMouse) == TRUE)
			{
				pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(8000510), m_ptMouse.x, m_ptMouse.y+50, ON_HELP_UP, RGB(250, 250, 250), TRUE);
			}

			// 더블클릭시 그마을의 아이템 정보를 보여준다.
			if(IpD.IsDoubleClick())
			{
				SI16 siYCount = m_siSelect - m_siScroll;
				if(m_ptMouse.x >= m_siX+647 && m_ptMouse.x < m_siX+773 && m_ptMouse.y >= (m_siY+75 + siYCount * 20) && m_ptMouse.y < (m_siY+75 + (siYCount + 1) * 20))
				{
					// 장부데이터를 받아온다.
					m_pVillageData = pMyOnlineWorld->pOnlineVillageSave->GetData(m_TargetList[0].m_pList[m_siSelect].m_uiVillageCode);
					m_siSubStatus = ON_MINIMAP_SUB_ITEMINFO;
				}
			}
		}
		break;
	case ON_MINIMAP_SUB_ITEMINFO:
		{
			MinimapMove();
			m_FanfLock = FALSE;

			if(bMouseDown)
			{
				SelectTarget(0);
				ActionTargetScroll(0);
				ActionSubItemScroll();
			}

			if(m_BSubExit.Process(bMouseDown))
				m_siSubStatus = ON_MINIMAP_SUB_POLL;
		}
		break;
	}
}

VOID OnlineMiniMap::ActionSubItemScroll()
{
	SI16 siHeight, siListCount;

	if(!m_pVillageData)		return;

	siListCount = m_pVillageData->m_siItemNum;

	// 스크롤 처리
	if(siListCount > MINIMAP_SUB_LIST_MAX_LINE)
	{
		if(m_BSubScrUp.Process(bMouseDown))
		{
			m_siSubScroll--;
			if(m_siSubScroll < 0)		m_siSubScroll = 0;
			
			siHeight = (SI16)(m_rtSubScroll.bottom - m_rtSubScroll.top - m_SubScrHandle.Header.Ysize);
			m_siSubScrollPos = m_siSubY + 49 + (SI16)(((float)siHeight / (float)(siListCount - MINIMAP_SUB_LIST_MAX_LINE)) * (float)m_siSubScroll);
		}		
		else if(m_BSubScrDown.Process(bMouseDown))
		{
			m_siSubScroll++;
			if(m_siSubScroll > (siListCount - MINIMAP_SUB_LIST_MAX_LINE))	m_siSubScroll = siListCount - MINIMAP_SUB_LIST_MAX_LINE;

			siHeight		=	(SI16)((m_rtSubScroll.bottom - m_rtSubScroll.top - m_SubScrHandle.Header.Ysize));
			m_siSubScrollPos = m_siSubY + 49 + (SI16)(((float)siHeight / (float)(siListCount - MINIMAP_SUB_LIST_MAX_LINE))  * (float)m_siSubScroll);
		}
		else if(PtInRect(&m_rtSubScroll, m_ptMouse) == TRUE)
		{
			siHeight		=	(SI16)(m_rtSubScroll.bottom - m_rtSubScroll.top - m_SubScrHandle.Header.Ysize);
			m_siSubScrollPos = m_ptMouse.y;
			if(m_siSubScrollPos > (m_rtSubScroll.bottom - m_SubScrHandle.Header.Ysize))
				m_siSubScrollPos = m_rtSubScroll.bottom - m_SubScrHandle.Header.Ysize;

			m_siSubScroll = max(0, (m_siSubScrollPos - (m_siSubY + 49)) / ((float)siHeight / (float)(siListCount - MINIMAP_SUB_LIST_MAX_LINE)));
		}
	}
}

VOID OnlineMiniMap::DrawItemList(HDC hdc)
{
	CHAR szTemp1[256], szTemp2[256];
	DWORD TextColor = RGB(250, 250, 250);

	m_BSubExit.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

	// 제목(마을이름, 취급품목, 수량, 무게, 판매, 구매)
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+29, m_siSubY+2, 106, 14, (char*)m_TargetList[0].m_pList[m_siSelect].m_TargetName, TextColor);

	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+10, m_siSubY+21, 84, 13, (char*)pMyOnlineWorld->pOnlineText->Get(ON_TEXT_USEGOOODS), TextColor);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+99, m_siSubY+21, 37, 13, (char*)pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_COUNT), TextColor);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+145, m_siSubY+21, 27, 13, (char*)pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WEIGHT), TextColor);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+175, m_siSubY+21, 47, 13, (char*)pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SOLD), TextColor);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+231, m_siSubY+21, 47, 13, (char*)pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BUY), TextColor);
	
	if(!m_pVillageData)		
	{
		sprintf(szTemp1, pMyOnlineWorld->pOnlineText->Get(8000511));
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine(hdc, m_siSubX+11, m_siSubY+50, 250, (char*)szTemp1, TextColor);
		return;
	}

	// 날짜
	sprintf(szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TIMETEXT), m_pVillageData->m_Date.m_siYear, m_pVillageData->m_Date.m_siMonth, m_pVillageData->m_Date.m_siDay, m_pVillageData->m_Date.m_siHour);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+148, m_siSubY+2, 112, 14, (char*)szTemp1, TextColor);
	
	// 아이템리스트 출력
	for(SI16 i = 0; i < MINIMAP_SUB_LIST_MAX_LINE; i++)
	{
		SI16 index = i + m_siSubScroll;

		if(index >= m_pVillageData->m_siItemNum)	continue;
		
		// 아이템이름
		CItemHeader *pItem = pMyOnlineWorld->pItemsInfo->GetItem(m_pVillageData->m_pData[index].m_uiItemID);
		if(!pItem)	continue;

		SI16 temp = strlen(pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
		if(temp > 8)
		{
			ZeroMemory(szTemp1, sizeof(szTemp1));
			strncpy(szTemp1, pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName), 8);
			sprintf(szTemp2, "%s..", szTemp1);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+11, m_siSubY+40 + (i * 18), 84, 15, (char*)szTemp2, TextColor);
		}
		else
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+11, m_siSubY+40 + (i * 18), 84, 15, (char*)pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName), TextColor);

		// 수량
		sprintf(szTemp1, "%d", m_pVillageData->m_pData[index].m_siItemQuantity);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+99, m_siSubY+40 + (i * 18), 37, 13, szTemp1, TextColor);

		// 무게
		sprintf(szTemp1, "%d", pItem->m_uiWeight);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+145, m_siSubY+40 + (i * 18), 27, 13, szTemp1, TextColor);

		// 판매
		sprintf(szTemp1, "%d", m_pVillageData->m_pData[index].m_uiBuyValue);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+175, m_siSubY+40 + (i * 18), 47, 13, szTemp1, TextColor);
		
		// 구매
		sprintf(szTemp1, "%d", m_pVillageData->m_pData[index].m_uiSellValue);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siSubX+231, m_siSubY+40 + (i * 18), 47, 13, szTemp1, TextColor);
	}
}