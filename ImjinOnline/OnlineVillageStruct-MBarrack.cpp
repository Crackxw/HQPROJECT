/************************************************************************************************
	だ橾貲 : OnlineVillageStruct-MBarrack.cpp
	氬渡濠 : 檜彌戮

	葆擊 �こ羹� 籀葬
************************************************************************************************/
#include <GSL.h>

#include <Main.h>
#include <Mouse.h>
#include "OnlineETC.h"
#include <etc.h>

#include "OnlineWorld.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineVillage.h"
#include "OnlineVillageStruct-MBarrack.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "..\\Gersang\\Music.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "Onlinetext.h"
#include "OnlineMercenary-Parser.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "..\\Gersang\\CharInfo\\CharFileLoadManager.h"
#include "OnlineMercenary.h"

#include "charanimation.h"
#include <kindinfo.h>
#include <Player.h>
#include <myfile.h>
extern	_KindInfo				KI[];								// 議葛攪 謙盟滌 等檜攪 

extern _InputDevice				IpD;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureMBarrack::OnlineVillageStructureMBarrack()
{
	m_NowWork = MBARRACK_NOWWORK_BUY_START;
	m_Say     = 0;
	ZeroMemory(m_MonsterBuyItemName, sizeof(m_MonsterBuyItemName));

	// Image
	ZeroMemory(&m_Image_MainInterface_Back1,    sizeof(XSPR));
	ZeroMemory(&m_Image_CheckInterface,         sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_MM,          sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_EM,          sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_BM,          sizeof(XSPR));
	ZeroMemory(&m_Image_Portrait,               sizeof(XSPR));
//	m_Image_CharacterSlotInterface = NULL;
	
	// Solder Data
	ZeroMemory(m_SolderList,          sizeof(OnInnBuyMonsterInfo) * MAX_BUYSOLDER_LIST);
	m_MaxSelectSolder = 0;
	m_NowSelectSolder = 0;
	ZeroMemory(m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYSOLDER_LIST);
	m_DismissalMaxSelectSolder = 0;
	m_DismissalNowSelectSolder = 0;
	m_Discount = 0;

	// Scroll
	m_NowScroll = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureMBarrack::~OnlineVillageStructureMBarrack()
{	
	Free();
}

BOOL	OnlineVillageStructureMBarrack::Action()
{
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMBarrack::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{	
	Free();

	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	// Load Image
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_hireling.spr",m_Image_MainInterface_Back1);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW.Spr", m_Image_CheckInterface);
//	m_Image_CharacterSlotInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHARACTERSLOT);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_tab.Spr",  m_Image_Button_All_MM);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_Image_Button_All_EM);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\main_window_button.Spr", m_Image_Button_All_BM);
	clGrp.LoadXspr("Online\\GameSpr\\inventory\\Sub_Button.Spr",m_Image_Check_Button);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW_LINE.Spr",m_Image_Line_Check_Button);



	m_siX = 134;
	m_siY = 45;

	m_siCheckX =  (800 - m_Image_CheckInterface.Header.Xsize)/2;
	m_siCheckY =  (600 - m_Image_CheckInterface.Header.Ysize)/2;

	// Button
	m_Button_Main_Buy.Create(m_siX + 130,m_siY + 31,m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_EMPOLYMENT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_Dismissal.Create(m_siX + 276,m_siY + 31, m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE), BUTTON_PUT_LEFT, TRUE);

	BExit.Create(m_siX + 232, m_siY + 394,								 m_Image_Button_All_BM.Header.Xsize, m_Image_Button_All_BM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_EXIT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Buy.Create(m_siX + 227, m_siY + 366,						 m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_EMPOLYMENT),  BUTTON_PUT_LEFT, TRUE);
	m_Button_BuyOK.Create(m_siCheckX + 50 , m_siCheckY + 148,            m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_EMPOLYMENT),  BUTTON_PUT_LEFT, TRUE);
	m_Button_BuyCancel.Create(m_siCheckX +170 , m_siCheckY + 148,        m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_Dismissal.Create(m_siX + 227 , m_siY + 366,				 m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_DismissalOK.Create(m_siCheckX + 50, m_siCheckY + 148,		 m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_DismissalCancel.Create(m_siCheckX + 170, m_siCheckY + 148,	 m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE),  BUTTON_PUT_LEFT, TRUE);

	// Initialize
	m_NowWork         = MBARRACK_NOWWORK_BUY_START;
	m_Say             = 0;
	ZeroMemory(m_MonsterBuyItemName, sizeof(m_MonsterBuyItemName));
	CItemHeader*	pItem;
	for(SI32 TempCount = 0; TempCount < pOnlineWorld->pItemsInfo->GetCount(); TempCount++)
	{
		pItem = pOnlineWorld->pItemsInfo->GetItemHeader(TempCount);

		if((pItem->m_siGroupCode & ON_ITEM_SPECIAL) && (pItem->m_siSpecial == ON_ITEM_SPECIAL_SEALINGBEAD))
		{
			strcpy((char*)m_MonsterBuyItemName, pOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
			break;
		}
	}

	m_MaxSelectSolder = 0;
	m_NowSelectSolder = 0;

	m_DismissalMaxSelectSolder = 0;
	m_DismissalNowSelectSolder = 0;

	m_Discount = 0;

	m_NowScroll       = 0;

	siKind	          = ON_VIL_ST_KIND_MBARRACK;

	pMyOnlineWorld->pOnlineClient->SendRequestMBarrackEnter();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMBarrack::Free()
{
	OnlineVillageBaseStructure::Free();

	if(m_Image_MainInterface_Back1.Image)  {clGrp.FreeXspr(m_Image_MainInterface_Back1);  ZeroMemory(&m_Image_MainInterface_Back1,  sizeof(XSPR));}
	if(m_Image_CheckInterface.Image)       {clGrp.FreeXspr(m_Image_CheckInterface);       ZeroMemory(&m_Image_CheckInterface,       sizeof(XSPR));}
//	m_Image_CharacterSlotInterface = NULL;

	if(m_Image_Button_All_MM.Image)        {clGrp.FreeXspr(m_Image_Button_All_MM);        ZeroMemory(&m_Image_Button_All_MM,        sizeof(XSPR));}
	if(m_Image_Button_All_EM.Image)        {clGrp.FreeXspr(m_Image_Button_All_EM);        ZeroMemory(&m_Image_Button_All_EM,        sizeof(XSPR));}
	if(m_Image_Button_All_BM.Image)        {clGrp.FreeXspr(m_Image_Button_All_BM);        ZeroMemory(&m_Image_Button_All_BM,        sizeof(XSPR));}
	if(m_Image_Check_Button.Image)		   {clGrp.FreeXspr(m_Image_Check_Button);		  ZeroMemory(&m_Image_Check_Button,			sizeof(XSPR));}
	if(m_Image_Line_Check_Button.Image)		   {clGrp.FreeXspr(m_Image_Line_Check_Button);    ZeroMemory(&m_Image_Line_Check_Button,    sizeof(XSPR));}

	if(m_Image_Portrait.Image)             {clGrp.FreeXspr(m_Image_Portrait);             ZeroMemory(&m_Image_Portrait,             sizeof(XSPR));}

	ZeroMemory(m_SolderList,          sizeof(OnInnBuyMonsterInfo) * MAX_BUYSOLDER_LIST);
	ZeroMemory(m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYSOLDER_LIST);

	m_NowWork         = 0;
	m_Say             = 0;
	ZeroMemory(m_MonsterBuyItemName, sizeof(m_MonsterBuyItemName));

	m_MaxSelectSolder = 0;
	m_NowSelectSolder = 0;

	m_DismissalMaxSelectSolder = 0;
	m_DismissalNowSelectSolder = 0;

	m_Discount = 0;

	m_NowScroll       = 0;

	siKind	          = 0;

	if(pMyOnlineWorld)
	{
		if( pMyOnlineWorld->m_pMercenary->IsAction() )
			pMyOnlineWorld->m_pMercenary->OnButtonPush();
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	char			TempBuffer[256];
	SI32			TempCount;
	SI32			TempUnitCount;
	BOOL			fLiveMouse;
	BOOL			TempbMouseDown;
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// 葆辦蝶 謝ル蒂 掘и棻.

	TempbMouseDown = bMouseDown;
	fLiveMouse     = TRUE;
	if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE)
	{
		TempbMouseDown = FALSE;
		fLiveMouse     = FALSE;
	}

	m_Button_Main_Buy.SetAction(FALSE);
	m_Button_Main_Dismissal.SetAction(FALSE);

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(m_NowWork)
	{
		case MBARRACK_NOWWORK_BUY_START:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_BUY_STARTPOOL:
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			// 橾薑 衛除檜 雖釭賊 Pool 煎 檜翕			
			m_NowWork        = MBARRACK_NOWWORK_BUY_POOL;

			if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
				PushEffect(EFFECT_MBARRACK_C, 0, 0);

			break;

		case MBARRACK_NOWWORK_BUY_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_Buy.SetAction(FALSE);
			else
			{
				m_Button_Main_Dismissal.SetAction(TRUE);

				// 釭陛晦
				if((BExit.Process(TempbMouseDown) == TRUE) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_NowWork = MBARRACK_NOWWORK_BUY_END;
				}
				if(m_Button_Main_Dismissal.Process(TempbMouseDown) == TRUE)
				{
					// つ熱 氈朝 辨煽菟 List 蹂羶
					pMyOnlineWorld->pOnlineClient->SendRequestMBarrackDismissalEnter();

					m_NowWork = MBARRACK_NOWWORK_DISMISSAL_GETLIST;
										
				}				

				// 堅辨
				m_Button_Buy.SetAction(TRUE);
				if(m_NowSelectSolder == -1) m_Button_Buy.SetAction(FALSE);
				switch(pMyOnlineWorld->pOnlineVillage->GetNation())
				{
					case ON_COUNTRY_JAPAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
						{
							m_Button_Buy.SetAction(FALSE);
						}
						break;

					case ON_COUNTRY_CHINA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
						{
							m_Button_Buy.SetAction(FALSE);
						}
						break;

					case ON_COUNTRY_TAIWAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
						{
							m_Button_Buy.SetAction(FALSE);
						}
						break;

					case ON_COUNTRY_KOREA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
						{
							m_Button_Buy.SetAction(FALSE);
						}
						break;
				}
				if(m_Button_Buy.Process(TempbMouseDown) == TRUE)
				{
					// ⑷營 Level煎 掘й熱 氈戲賊 OK
					if(CheckLevel(m_SolderList[m_NowSelectSolder].siLevel, m_SolderList[m_NowSelectSolder].siCredit))
					{
						if(CheckItem(m_SolderList[m_NowSelectSolder].siSealingBeadNum))
						{
							if(CheckMonsterNum())
							{
								// ⑷營 盪濰й 濠葬陛 氈戲賊 鱔婁 橈戲賊 Error Message
								TempUnitCount = 0;
								for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
								{
									if(pMyOnlineWorld->pMyData->IsValidFollower(TempCount) == FALSE)
									{
										TempUnitCount++;
									}
								}
								
								if(TempUnitCount <= 0)
								{
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOSOLIDER));
								}
								else
								{
									SI16		MaxFollower;
									char		TempBuffer[256];

									MaxFollower = pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
									if((ON_MAX_FOLLOWER_NUM - TempUnitCount) > MaxFollower)
									{
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOWSOLIDER), MaxFollower);
										pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), (char*)TempBuffer);
									}
									else
									{
										if(pMyOnlineWorld->pMyData->GetMoney() < m_SolderList[m_NowSelectSolder].siMoney)
										{
											pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_NOMONEY));
										}
										else
										{
											m_NowWork = MBARRACK_NOWWORK_BUYCHECK_POOL;
										}
									}
								}
							}
							else
							{
								// 跨蝶攪 熱陛 剩耀棻堅 轎溘
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK)
									, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_OVERMONSTERBUY)
									, ON_MAXMONSTER);
							}
						}
						else
						{
							// 嬴檜蠱 橈棻堅 轎溘ж朝 詭撮雖
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK)
								, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_NOITEM)
								, GetMonsterBuyItemName(), m_SolderList[m_NowSelectSolder].siSealingBeadNum);
						}
					}
					else
					{
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_SOLIDEREMPOLY), m_SolderList[m_NowSelectSolder].siLevel, m_SolderList[m_NowSelectSolder].siCredit);
					}
				}

				if(TempbMouseDown)
				{
					// 辨煽 List
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_MaxSelectSolder)
							{
								if(TempCount2 == 0)
								{
									TempXPos = (m_siX + 48) + (TempCount1 * 68);
									TempYPos = m_siY + 180;
								}
								else
								{
									TempXPos = (m_siX + 84) + (TempCount1 * 68);
									TempYPos = m_siY + 248;
								}

								if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
								{
									m_NowSelectSolder = TempCount;
								}
							}
						}
					}
				}

				// Help
				for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
				{
					for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
					{
						TempCount = (TempCount2 * 6) + TempCount1;
						if(TempCount < m_MaxSelectSolder)
						{
							if(TempCount2 == 0)
							{
								TempXPos = (m_siX + 48) + (TempCount1 * 68);
								TempYPos = m_siY + 180;
							}
							else
							{
								TempXPos = (m_siX + 84) + (TempCount1 * 68);
								TempYPos = m_siY + 248;
							}

							if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
							{
								MakeName( m_SolderList[TempCount].SolderInfo.uiKind, m_SolderList[TempCount].uiCharNameAddOption, (BYTE*)TempBuffer, 256 );

								pMyOnlineWorld->pOnlineHelp->SetTextBuyMonster((SI16)ptMouse.x, (SI16)ptMouse.y, (char*)TempBuffer
									, m_SolderList[TempCount].siMoney
									, m_SolderList[TempCount].SolderInfo.siLife, m_SolderList[TempCount].SolderInfo.siMana, m_SolderList[TempCount].SolderInfo.siStr, m_SolderList[TempCount].SolderInfo.siDex, m_SolderList[TempCount].SolderInfo.siVit, m_SolderList[TempCount].SolderInfo.siInt
									, m_SolderList[TempCount].siLevel, pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)
									, m_SolderList[TempCount].siCredit, pMyOnlineWorld->pMyData->GetTradeGrade()
									, GetMonsterBuyItemName(), m_SolderList[TempCount].siSealingBeadNum);
								goto END_HELP;
							}
						}
					}
				}
END_HELP:

				if(IpD.IsDoubleClick() == TRUE)
				{
					// 辨煽 List
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_MaxSelectSolder)
							{
								if(TempCount2 == 0)
								{
									TempXPos = (m_siX + 48) + (TempCount1 * 68);
									TempYPos = m_siY + 180;
								}
								else
								{
									TempXPos = (m_siX + 84) + (TempCount1 * 68);
									TempYPos = m_siY + 248;
								}

								if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
								{
									m_NowSelectSolder = TempCount;

									if(m_Button_Buy.IsAction())
									{
										// ⑷營 Level煎 掘й熱 氈戲賊 OK
										if(CheckLevel(m_SolderList[m_NowSelectSolder].siLevel, m_SolderList[m_NowSelectSolder].siCredit))
										{
											if(CheckItem(m_SolderList[m_NowSelectSolder].siSealingBeadNum))
											{
												if(CheckMonsterNum())
												{
													// ⑷營 盪濰й 濠葬陛 氈戲賊 鱔婁 橈戲賊 Error Message
													TempUnitCount = 0;
													for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
													{
														if(pMyOnlineWorld->pMyData->IsValidFollower(TempCount) == FALSE)
														{
															TempUnitCount++;
														}
													}
													
													if(TempUnitCount <= 0)
													{
														pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOSOLIDER));
													}
													else
													{
														SI16		MaxFollower;
														char		TempBuffer[256];

														MaxFollower = pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
														if((ON_MAX_FOLLOWER_NUM - TempUnitCount) > MaxFollower)
														{
															sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOWSOLIDER), MaxFollower);
															pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), (char*)TempBuffer);
														}
														else
														{
															if(pMyOnlineWorld->pMyData->GetMoney() < m_SolderList[m_NowSelectSolder].siMoney)
															{
																pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_NOMONEY));
															}
															else
															{
																m_NowWork = MBARRACK_NOWWORK_BUYCHECK_POOL;
															}
														}
													}
												}
												else
												{
													// 跨蝶攪 熱陛 剩耀棻堅 轎溘
													pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK)
														, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_OVERMONSTERBUY)
														, ON_MAXMONSTER);
												}
											}
											else
											{
												// 嬴檜蠱 橈棻堅 轎溘ж朝 詭撮雖
												pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK)
													, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_NOITEM)
													, GetMonsterBuyItemName(), m_SolderList[m_NowSelectSolder].siSealingBeadNum);
											}
										}
										else
										{
											pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_SOLIDEREMPOLY), m_SolderList[m_NowSelectSolder].siLevel, m_SolderList[m_NowSelectSolder].siCredit);
										}
									}
									break;
								}
							}
						}
					}
				}
			}

			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_BUYCHECK_POOL:
			m_Button_Buy.SetAction(FALSE);

			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
/*				if(GetAsyncKeyState(VK_RETURN))
				{
					BYTE		TempBuffer[256];

					// Send Message
					ZeroMemory(TempBuffer, 256);
					MakeName(m_SolderList[m_NowSelectSolder].SolderInfo.uiKind, m_SolderList[m_NowSelectSolder].uiCharNameAddOption, (BYTE*)TempBuffer);
					pMyOnlineWorld->pOnlineClient->SendRequestMBarrackBuySolder(m_SolderList[m_NowSelectSolder].siCode, (BYTE*)TempBuffer);

					m_NowWork = MBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT;

					m_DeleteItemCount = m_SolderList[m_NowSelectSolder].siSealingBeadNum;
				}
				else
				{
*/
				if(m_Button_BuyOK.Process(TempbMouseDown) == TRUE)
				{
					BYTE		TempBuffer[256];

					// Send Message
					MakeName( m_SolderList[m_NowSelectSolder].SolderInfo.uiKind, m_SolderList[m_NowSelectSolder].uiCharNameAddOption, (BYTE*)TempBuffer, 256 );
					pMyOnlineWorld->pOnlineClient->SendRequestMBarrackBuySolder(m_SolderList[m_NowSelectSolder].siCode, (BYTE*)TempBuffer);

					m_NowWork = MBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT;

					m_DeleteItemCount = m_SolderList[m_NowSelectSolder].siSealingBeadNum;
				}
				else
				{
					if(m_Button_BuyCancel.Process(TempbMouseDown) == TRUE)
					{
						m_NowWork = MBARRACK_NOWWORK_BUY_POOL;
					}
				}
//				}
			}

			Draw_Select(pSurface);
			Draw_Check(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_Select(pSurface);
			Draw_Check(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_BUY_END:
			Draw_Select(pSurface);
			m_NowWork        = MBARRACK_NOWWORK_BUY_ENDPOOL;
			break;

		case MBARRACK_NOWWORK_BUY_ENDPOOL:
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			Free();
			return TRUE;
		case MBARRACK_NOWWORK_DISMISSAL_GETLIST:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_DismissalSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_DISMISSAL_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
				m_Button_Main_Buy.SetAction(TRUE);

				// 釭陛晦
				if((BExit.Process(TempbMouseDown) == TRUE) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_NowWork = MBARRACK_NOWWORK_DISMISSAL_END;
				}
				if(m_Button_Main_Buy.Process(TempbMouseDown) == TRUE)
				{
					m_NowWork = MBARRACK_NOWWORK_BUY_POOL;
					if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA ||
						pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN )
						PushEffect(EFFECT_BARRACK00, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
						PushEffect(EFFECT_BARRACK00J, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
						PushEffect(EFFECT_MBARRACK_C, 0, 0);
				}				

				// п堅
				m_Button_Dismissal.SetAction(TRUE);
				if(m_DismissalNowSelectSolder == -1) m_Button_Dismissal.SetAction(FALSE);
				if(m_Button_Dismissal.Process(TempbMouseDown) == TRUE)
				{
					m_NowWork = MBARRACK_NOWWORK_DISMISSALCHECK_POOL;
				}

				if(TempbMouseDown)
				{
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_DismissalMaxSelectSolder)
							{
								if(TempCount2 == 0)
								{
									TempXPos = (m_siX + 48) + (TempCount1 * 68);
									TempYPos = m_siY + 180;
								}
								else
								{
									TempXPos = (m_siX + 84) + (TempCount1 * 68);
									TempYPos = m_siY + 248;
								}

								if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
								{
									m_DismissalNowSelectSolder = TempCount;
									goto END_SELECT1;
								}
							}
						}
					}
				}
END_SELECT1:

				// Help
				for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
				{
					for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
					{
						TempCount = (TempCount2 * 6) + TempCount1;
						if(TempCount < m_DismissalMaxSelectSolder)
						{
							if(TempCount2 == 0)
							{
								TempXPos = (m_siX + 48) + (TempCount1 * 68);
								TempYPos = m_siY + 180;
							}
							else
							{
								TempXPos = (m_siX + 84) + (TempCount1 * 68);
								TempYPos = m_siY + 248;
							}

							if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
							{
								pMyOnlineWorld->pOnlineHelp->SetTextDismissalSolder((SI16)ptMouse.x, (SI16)ptMouse.y, pMyOnlineWorld->pMyData->GetFollowerCharName(m_DismissalSolderList[TempCount].siSlot)
									, m_DismissalSolderList[TempCount].siMoney
									, pMyOnlineWorld->pMyData->GetFollowerParameterLevel(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterLife(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterMana(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterTotalStr(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterTotalDex(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterTotalVit(m_DismissalSolderList[TempCount].siSlot)
									, pMyOnlineWorld->pMyData->GetFollowerParameterTotalInt(m_DismissalSolderList[TempCount].siSlot));
								goto END_HELP1;
							}
						}
					}
				}
END_HELP1:

				if(IpD.IsDoubleClick() == TRUE)
				{
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_DismissalMaxSelectSolder)
							{
								if(TempCount2 == 0)
								{
									TempXPos = (m_siX + 48) + (TempCount1 * 68);
									TempYPos = m_siY + 180;
								}
								else
								{
									TempXPos = (m_siX + 84) + (TempCount1 * 68);
									TempYPos = m_siY + 248;
								}

								if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
								{
									m_DismissalNowSelectSolder = TempCount;

									m_NowWork = MBARRACK_NOWWORK_DISMISSALCHECK_POOL;
									break;
								}
							}
						}
					}
				}
			}

			Draw_DismissalSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_DISMISSALCHECK_POOL:
			m_Button_Dismissal.SetAction(FALSE);

			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
/*				if(GetAsyncKeyState(VK_RETURN))
				{
					// Send Message
					pMyOnlineWorld->pOnlineClient->SendRequestMBarrackDismissalSolder(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);

					m_NowWork = MBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT;
				}
				else
				{
*/
				if(m_Button_DismissalOK.Process(TempbMouseDown) == TRUE)
				{
					// Send Message
					pMyOnlineWorld->pOnlineClient->SendRequestMBarrackDismissalSolder(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);

					m_NowWork = MBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT;
				}
				else
				{
					if(m_Button_DismissalCancel.Process(TempbMouseDown) == TRUE)
					{
						m_NowWork = MBARRACK_NOWWORK_DISMISSAL_POOL;
					}
				}
//				}
			}

			Draw_DismissalSelect(pSurface);
			Draw_DismissalCheck(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_DismissalSelect(pSurface);
			Draw_DismissalCheck(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case MBARRACK_NOWWORK_DISMISSAL_END:
			Draw_DismissalSelect(pSurface);
			m_NowWork        = MBARRACK_NOWWORK_DISMISSAL_ENDPOOL;
			break;

		case MBARRACK_NOWWORK_DISMISSAL_ENDPOOL:
			Draw_DismissalSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			Free();
			return TRUE;
			break;
	}

	// 辨煽
	pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	// 檣漸饜葬.
	if(pMyOnlineWorld->pOnlineInventory->IsActive())
	{
		pMyOnlineWorld->pOnlineInventory->SetFlagVillage(TRUE);
		pMyOnlineWorld->pOnlineInventory->Action();
		pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 濰睡
	if(pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
		pMyOnlineWorld->pOnlineTradeBook->Poll();
		pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 嘐棲裘
	if(pMyOnlineWorld->pOnlineMiniMap->IsActive())
	{
		pMyOnlineWorld->pOnlineMiniMap->Pool();
		pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// Pannel擊 轎溘и棻.
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON);

	if(IpD.LeftPressSwitch) bMouseDown = TRUE;
	else				    bMouseDown = FALSE;

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw Main
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::Draw_Select(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			StartPos;
	SI32			TempCount;
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;
	char			TempBuffer[1024];
	BYTE			fFlag;				// 0 : Normal 轎溘, 1 : 殘⑵霞 蝶觼煤, 2 : 雲橫雖朝 蝶觼煤
	SI32			TempSelectCharacterIndex = -1;
	SI32			TempSay;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// 葆辦蝶 謝ル蒂 掘и棻.

	// 嬪纂 啗骯
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case MBARRACK_NOWWORK_BUY_STARTPOOL:
			fFlag     = 1;
			break;

		case MBARRACK_NOWWORK_BUY_ENDPOOL:
			fFlag     = 0;
			break;
	}

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 蝶觼煤 轎溘
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_Back1.Header.Xsize, m_Image_MainInterface_Back1.Header.Ysize, m_Image_MainInterface_Back1.Image);

		// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_MBARRACK, pMyOnlineWorld->pOnlineVillage->GetNation() );
				
		// 堅辨ж晦 幗が 轎溘
		if(m_NowWork == MBARRACK_NOWWORK_BUY_POOL) m_Button_Buy.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                       m_Button_Buy.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);

		// 髦熱氈朝 辨煽 轎溘
		for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
		{
			for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
			{
				TempCount = (TempCount2 * 6) + TempCount1;
				if(TempCount < m_MaxSelectSolder)
				{
					if(TempCount2 == 0)
					{
						TempXPos = (m_siX + 48) + (TempCount1 * 68);
						TempYPos = m_siY + 180;
					}
					else
					{
						TempXPos = (m_siX + 84) + (TempCount1 * 68);
						TempYPos = m_siY + 248;
					}

					/*
					if(m_NowSelectSolder == TempCount)
					{
//						clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[1]]);
					}
					else
					{
						if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
						{
							if(bMouseDown)
							{
//								clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[1]]);
							}
							else
							{
//								clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[2]]);
							}
						}
						else
						{
//							clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[0]]);
						}
					}
					*/
					if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
					{
						TempSelectCharacterIndex = TempCount;
					}

					// 摹鷗脹 辨煽檜 氈擊 唳辦 ж檜塭檜お 鎰橫遽棻.
					SI32	siDrawFlag = 0;
					if( m_NowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					switch(pMyOnlineWorld->pOnlineVillage->GetNation())
					{
						case ON_COUNTRY_KOREA:
							// actdoll (2004/01/10 14:15) : 措陛陛 ぎ萵 唳辦
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// 蜃擊 唳辦
							else
							{
								// 溯漣 敘薑縑 紫殖ж雖 彊懊棻賊 摹鷗碳陛
								if(	(CheckLevel(m_SolderList[TempCount].siLevel, m_SolderList[TempCount].siCredit) == FALSE)	|| 
									(CheckMonsterNum() == FALSE)																|| 
									(CheckItem(m_SolderList[TempCount].siSealingBeadNum) == FALSE) )
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_SolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;

						case ON_COUNTRY_JAPAN:
							// actdoll (2004/01/10 14:15) : 措陛陛 ぎ萵 唳辦
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// 蜃擊 唳辦
							else
							{
								// 溯漣 敘薑縑 紫殖ж雖 彊懊棻賊 摹鷗碳陛
								if(	(CheckLevel(m_SolderList[TempCount].siLevel, m_SolderList[TempCount].siCredit) == FALSE)	|| 
									(CheckMonsterNum() == FALSE)																|| 
									(CheckItem(m_SolderList[TempCount].siSealingBeadNum) == FALSE) )
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_SolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;

						case ON_COUNTRY_CHINA:
							// actdoll (2004/01/10 14:15) : 措陛陛 ぎ萵 唳辦
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// 蜃擊 唳辦
							else
							{
								// 溯漣 敘薑縑 紫殖ж雖 彊懊棻賊 摹鷗碳陛
								if(	(CheckLevel(m_SolderList[TempCount].siLevel, m_SolderList[TempCount].siCredit) == FALSE)	|| 
									(CheckMonsterNum() == FALSE)																|| 
									(CheckItem(m_SolderList[TempCount].siSealingBeadNum) == FALSE) )
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_SolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;

						case ON_COUNTRY_TAIWAN:
							// actdoll (2004/01/10 14:15) : 措陛陛 ぎ萵 唳辦
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// 蜃擊 唳辦
							else
							{
								// 溯漣 敘薑縑 紫殖ж雖 彊懊棻賊 摹鷗碳陛
								if(	(CheckLevel(m_SolderList[TempCount].siLevel, m_SolderList[TempCount].siCredit) == FALSE)	|| 
									(CheckMonsterNum() == FALSE)																|| 
									(CheckItem(m_SolderList[TempCount].siSealingBeadNum) == FALSE) )
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_SolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;
					}
				}
			}
		}

		// 釭陛晦 幗が 轎溘
		if(m_NowWork == MBARRACK_NOWWORK_BUY_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                       BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		
		m_Button_Main_Buy.SetAction(TRUE);
		m_Button_Main_Buy.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_Buy.SetAction(FALSE);
		m_Button_Main_Dismissal.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		switch(fFlag)
		{
			case 0:
				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

				switch(pMyOnlineWorld->pOnlineVillage->GetNation())
				{
					case ON_COUNTRY_JAPAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
						{
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYNOTEQUITSTATETEXT2_JAPAN));
						}
						else
						{
							// 鼻檣 渠餌 轎溘
							TempSay = 0;
							if(TempSelectCharacterIndex != -1)
							{
								TempSay = GetFollowerBarrackText(m_SolderList[TempSelectCharacterIndex].SolderInfo.uiKind);
							}

							if(TempSay == 0)
							{
								if(m_Discount > 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISCOUNT_JAPAN), pMyOnlineWorld->pMyData->GetGuildName());
								}
								else if(m_Discount < 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERINCREASE_JAPAN), pMyOnlineWorld->pMyData->GetGuildName());
								}				
								else
								{
									sprintf((char*)TempBuffer, "%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYTEXT_JAPAN));
									m_Say             = 0;
								}
							}
							else
							{
								char			TempBufferOwnerText1[1024];
								char			TempBufferOwnerText2[1024];

								// 辨煽堅辨縑 в蹂и 棟溘纂諦 陛問
								ZeroMemory(TempBufferOwnerText1, 1024);
								ZeroMemory(TempBufferOwnerText2, 1024);
								switch(pMyOnlineWorld->GetVillageStateCode((UI32)pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode))
								{
									case ON_COUNTRY_JAPAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_CHINA:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_TAIWAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_KOREA:
									default:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;
								}

								// 陝 辨煽曖 撲貲
								sprintf((char*)TempBuffer, "%s %s %s", pMyOnlineWorld->pOnlineText->Get(TempSay), TempBufferOwnerText1, TempBufferOwnerText2);
							}
						}
						break;

					case ON_COUNTRY_CHINA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
						{
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYNOTEQUITSTATETEXT2_KOREA));
						}
						else
						{
							// 鼻檣 渠餌 轎溘
							TempSay = 0;
							if(TempSelectCharacterIndex != -1)
							{
								TempSay = GetFollowerBarrackText(m_SolderList[TempSelectCharacterIndex].SolderInfo.uiKind);
							}

							if(TempSay == 0)
							{
								if(m_Discount > 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISCOUNT_KOREA), pMyOnlineWorld->pMyData->GetGuildName());
								}
								else if(m_Discount < 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERINCREASE_KOREA), pMyOnlineWorld->pMyData->GetGuildName());
								}				
								else
								{
									sprintf((char*)TempBuffer, "%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYTEXT_KOREA));
									m_Say             = 0;
								}
							}
							else
							{
								char			TempBufferOwnerText1[1024];
								char			TempBufferOwnerText2[1024];

								// 辨煽堅辨縑 в蹂и 棟溘纂諦 陛問
								ZeroMemory(TempBufferOwnerText1, 1024);
								ZeroMemory(TempBufferOwnerText2, 1024);
								switch(pMyOnlineWorld->GetVillageStateCode((SI16)pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode))
								{
									case ON_COUNTRY_JAPAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_CHINA:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_TAIWAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_KOREA:
									default:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;
								}

								// 陝 辨煽曖 撲貲
								sprintf((char*)TempBuffer, "%s %s %s", pMyOnlineWorld->pOnlineText->Get(TempSay), TempBufferOwnerText1, TempBufferOwnerText2);
							}
						}
						break;

					case ON_COUNTRY_TAIWAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
						{
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYNOTEQUITSTATETEXT2_KOREA));
						}
						else
						{
							// 鼻檣 渠餌 轎溘
							TempSay = 0;
							if(TempSelectCharacterIndex != -1)
							{
								TempSay = GetFollowerBarrackText(m_SolderList[TempSelectCharacterIndex].SolderInfo.uiKind);
							}

							if(TempSay == 0)
							{
								if(m_Discount > 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISCOUNT_KOREA), pMyOnlineWorld->pMyData->GetGuildName());
								}
								else if(m_Discount < 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERINCREASE_KOREA), pMyOnlineWorld->pMyData->GetGuildName());
								}				
								else
								{
									sprintf((char*)TempBuffer, "%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYTEXT_KOREA));
									m_Say             = 0;
								}
							}
							else
							{
								char			TempBufferOwnerText1[1024];
								char			TempBufferOwnerText2[1024];

								// 辨煽堅辨縑 в蹂и 棟溘纂諦 陛問
								ZeroMemory(TempBufferOwnerText1, 1024);
								ZeroMemory(TempBufferOwnerText2, 1024);
								switch(pMyOnlineWorld->GetVillageStateCode((SI16)pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode))
								{
									case ON_COUNTRY_JAPAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_CHINA:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_TAIWAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_KOREA:
									default:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;
								}

								// 陝 辨煽曖 撲貲
								sprintf((char*)TempBuffer, "%s %s %s", pMyOnlineWorld->pOnlineText->Get(TempSay), TempBufferOwnerText1, TempBufferOwnerText2);
							}
						}
						break;

					case ON_COUNTRY_KOREA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
						{
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYNOTEQUITSTATETEXT2_KOREA));
						}
						else
						{
							// 鼻檣 渠餌 轎溘
							TempSay = 0;
							if(TempSelectCharacterIndex != -1)
							{
								TempSay = GetFollowerBarrackText(m_SolderList[TempSelectCharacterIndex].SolderInfo.uiKind);
							}

							if(TempSay == 0)
							{
								if(m_Discount > 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISCOUNT_KOREA), pMyOnlineWorld->pMyData->GetGuildName());
								}
								else if(m_Discount < 0)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERINCREASE_KOREA), pMyOnlineWorld->pMyData->GetGuildName());
								}				
								else
								{
									sprintf((char*)TempBuffer, "%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYTEXT_KOREA));
									m_Say             = 0;
								}
							}
							else
							{
								char			TempBufferOwnerText1[1024];
								char			TempBufferOwnerText2[1024];

								// 辨煽堅辨縑 в蹂и 棟溘纂諦 陛問
								ZeroMemory(TempBufferOwnerText1, 1024);
								ZeroMemory(TempBufferOwnerText2, 1024);
								switch(pMyOnlineWorld->GetVillageStateCode((SI16)pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode))
								{
									case ON_COUNTRY_JAPAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_JAPAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_CHINA:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_TAIWAN:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_TAIWAN)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;

									case ON_COUNTRY_KOREA:
									default:
										sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYLEVELCREDIT_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siLevel, m_SolderList[TempSelectCharacterIndex].siCredit);
										sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERBUYMONEY_KOREA)
											, m_SolderList[TempSelectCharacterIndex].siMoney);
										break;
								}

								// 陝 辨煽曖 撲貲
								sprintf((char*)TempBuffer, "%s %s %s", pMyOnlineWorld->pOnlineText->Get(TempSay), TempBufferOwnerText1, TempBufferOwnerText2);
							}
						}
						break;
				}

				pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, (char*)TempBuffer);

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

				// 堅辨ж晦 幗が Text 轎溘
				m_Button_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;
		}

		// �こ羹� 旋濠 轎溘
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207, m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), RGB(200, 200, 200));

		// 釭陛晦 幗が Text 轎溘
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

//	BExit.SetX(BExit.GetX());
//	m_Button_Buy.SetX(m_Button_Buy.GetX());
//	m_Button_Main_Buy.SetX(m_Button_Main_Buy.GetX());
//	m_Button_Main_Dismissal.SetX(m_Button_Main_Dismissal.GetX());

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw Main Check
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::Draw_Check(LPDIRECTDRAWSURFACE7 pSurface, DWORD TickCount)
{
	HDC		hDC;
	BYTE	TempBuffer[256];
	BYTE	TempBuffer1[256];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siCheckX, m_siCheckY, m_Image_Line_Check_Button.Header.Xsize, m_Image_Line_Check_Button.Header.Ysize, m_Image_Line_Check_Button.Image);
		// 夥鰻 轎溘
		clGrp.PutSpriteJin(m_siCheckX, m_siCheckY, m_Image_CheckInterface.Header.Xsize, m_Image_CheckInterface.Header.Ysize, m_Image_CheckInterface.Image);

		// 堅辨й溥朝 辨煽 橡掉 轎溘
//		clGrp.PutSpriteT(337, 200, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[0]]);
//		RenderCharacter(pSurface, (UI16)m_SolderList[m_NowSelectSolder].SolderInfo.uiKind, m_siCheckX + 25, m_siCheckY + 45, FALSE, 100, 0);
		
		// actdoll (2004/01/10 14:15) : 蟾鼻鎰晦
		RenderCharacter( pSurface, m_siCheckX + 25, m_siCheckY + 45, m_SolderList[m_NowSelectSolder].SolderInfo.uiKind, DF_FLAG_PORTRAIT_HIGHLIGHT );

		// 堅辨ж晦 幗が 轎溘
		m_Button_BuyOK.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 鏃模ж晦 幗が 轎溘
		m_Button_BuyCancel.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);

		// 堅辨й溥朝 辨煽 薑爾 轎溘
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		if(m_NowSelectSolder != -1)
		{
			// Name
			MakeName( m_SolderList[m_NowSelectSolder].SolderInfo.uiKind, m_SolderList[m_NowSelectSolder].uiCharNameAddOption, (BYTE*)TempBuffer, 256 );
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 50, (char*)TempBuffer, RGB(200, 200, 200));

			// Money
			MakeMoneyText(m_SolderList[m_NowSelectSolder].siMoney, (char*)TempBuffer1);
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INFO_MONEY), TempBuffer1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 65, (char*)TempBuffer, RGB(200, 200, 200));

			// LIFE
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INFO_HP), m_SolderList[m_NowSelectSolder].SolderInfo.siLife);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 80, (char*)TempBuffer, RGB(200, 200, 200));

			// MANA
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INFO_MP), m_SolderList[m_NowSelectSolder].SolderInfo.siMana);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 95, (char*)TempBuffer, RGB(200, 200, 200));

			// 餌辨ж朝 嬴檜蠱
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_TEXT_BUY_USEITEM), GetMonsterBuyItemName(), m_SolderList[m_NowSelectSolder].siSealingBeadNum);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 110, (char*)TempBuffer, RGB(200, 200, 200));
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// 堅辨ж晦 幗が Text 轎溘
		m_Button_BuyOK.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 鏃模ж晦 幗が Text 轎溘
		m_Button_BuyCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw Dismissal
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::Draw_DismissalSelect(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			StartPos;
	SI32			TempCount;
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;
	BYTE			fFlag;				// 0 : Normal 轎溘, 1 : 殘⑵霞 蝶觼煤, 2 : 雲橫雖朝 蝶觼煤

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// 葆辦蝶 謝ル蒂 掘и棻.

	// 嬪纂 啗骯
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case MBARRACK_NOWWORK_DISMISSAL_ENDPOOL:
			fFlag     = 0;
			break;
	}

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 蝶觼煤 轎溘
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_Back1.Header.Xsize, m_Image_MainInterface_Back1.Header.Ysize, m_Image_MainInterface_Back1.Image);

		// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_MBARRACK, pMyOnlineWorld->pOnlineVillage->GetNation() );
				
		// п堅ж晦 幗が 轎溘
		if(m_NowWork == MBARRACK_NOWWORK_DISMISSAL_POOL) m_Button_Dismissal.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                             m_Button_Dismissal.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);

		// 頂 辨煽 轎溘
		for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
		{
			for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
			{
				TempCount = (TempCount2 * 6) + TempCount1;
				if(TempCount < m_DismissalMaxSelectSolder)
				{
					if(TempCount2 == 0)
					{
						TempXPos = (m_siX + 48) + (TempCount1 * 68);
						TempYPos = m_siY + 180;
					}
					else
					{
						TempXPos = (m_siX + 84) + (TempCount1 * 68);
						TempYPos = m_siY + 248;
					}
/*
					if(m_DismissalNowSelectSolder == TempCount)
					{
//						clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[1]]);
					}
					else
					{
						if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
						{
							if(bMouseDown)
							{
//								clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[2]]);
							}
							else
							{
//								clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[1]]);
							}
						}
						else
						{
//							clGrp.PutSpriteT(TempXPos, TempYPos, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[0]]);
						}
					}
*/
//					SI32	CurLife	= pMyOnlineWorld->pMyData->GetFollowerParameterLife(m_DismissalSolderList[TempCount].siSlot);
//					SI32	MaxLife	= pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife(m_DismissalSolderList[TempCount].siSlot);
//					SI32	CurExp	= pMyOnlineWorld->pMyData->GetFollowerParameterCurExp(m_DismissalSolderList[TempCount].siSlot);
//					SI32	NextExp	= pMyOnlineWorld->pMyData->GetFollowerParameterNextExp(m_DismissalSolderList[TempCount].siSlot);
//					SI32	PrevExp	= pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp(m_DismissalSolderList[TempCount].siSlot);
					
					// 摹鷗脹 辨煽檜 氈擊 唳辦 ж檜塭檜お 鎰橫遽棻.
					SI32	siDrawFlag = 0;
					if( m_DismissalNowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					// actdoll (2004/01/10 14:15) : 蟾鼻鎰晦
					if(m_NowWork != MBARRACK_NOWWORK_DISMISSAL_GETLIST)
					{
//						RenderCharacter(pSurface, pMyOnlineWorld->pMyData->GetFollowerCharID(m_DismissalSolderList[TempCount].siSlot), TempXPos + 4, TempYPos + 5, FALSE
//							, (SI16)(((R32)CurLife / (R32)MaxLife) * 100.0f), (SI16)(((R32)(CurExp - PrevExp) / (R32)(NextExp - PrevExp)) * 100.0f));
						RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, pMyOnlineWorld->pMyData->GetFollowerCharID(m_DismissalSolderList[TempCount].siSlot), siDrawFlag );
					}
				}
			}
		}

		// 釭陛晦 幗が 轎溘
		if(m_NowWork == MBARRACK_NOWWORK_DISMISSAL_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                             BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		
		m_Button_Main_Buy.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Dismissal.SetAction(TRUE);
		m_Button_Main_Dismissal.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_Dismissal.SetAction(FALSE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		switch(fFlag)
		{
			case 0:
				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

				// 鼻檣 渠餌 轎溘
				m_Say = 0;
				switch(pMyOnlineWorld->pOnlineVillage->GetNation())
				{
					case ON_COUNTRY_JAPAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 10)
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_JAPAN));
						}
						else
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_JAPANAS));
						}
						break;

					case ON_COUNTRY_CHINA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 100)
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_TAIWAN));
						}
						else
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_TAIWANAS));
						}
						break;

					case ON_COUNTRY_TAIWAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 1000)
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_TAIWAN));
						}
						else
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_TAIWANAS));
						}
						break;

					case ON_COUNTRY_KOREA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 1)
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_KOREA));
						}
						else
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_KOREAAS));
						}
						break;
				}

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

				// 堅辨ж晦 幗が Text 轎溘
				m_Button_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;
		}

		// �こ羹� 旋濠 轎溘
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207, m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK), RGB(200, 200, 200));

		// 釭陛晦 幗が Text 轎溘
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw Dismissal Check
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::Draw_DismissalCheck(LPDIRECTDRAWSURFACE7 pSurface, DWORD TickCount)
{
	HDC		hDC;
	BYTE	TempBuffer[256];
	BYTE	TempBuffer1[256];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siCheckX, m_siCheckY, m_Image_Line_Check_Button.Header.Xsize, m_Image_Line_Check_Button.Header.Ysize, m_Image_Line_Check_Button.Image);
		// 夥鰻 轎溘
		clGrp.PutSpriteJin(m_siCheckX, m_siCheckY, m_Image_CheckInterface.Header.Xsize, m_Image_CheckInterface.Header.Ysize, m_Image_CheckInterface.Image);

		// п堅й溥朝 辨煽 橡掉 轎溘
		if(m_DismissalNowSelectSolder != -1)
		{
//			SI32	CurLife	= pMyOnlineWorld->pMyData->GetFollowerParameterLife(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);
//			SI32	MaxLife	= pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);
//			SI32	CurExp	= pMyOnlineWorld->pMyData->GetFollowerParameterCurExp(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);
//			SI32	NextExp	= pMyOnlineWorld->pMyData->GetFollowerParameterNextExp(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);
//			SI32	PrevExp	= pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);
		
//			RenderCharacter(pSurface, (UI16)pMyOnlineWorld->pMyData->GetFollowerCharID(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot), m_siCheckX + 25, m_siCheckY + 45, FALSE
//				, (SI16)(((R32)CurLife / (R32)MaxLife) * 100.0f), (SI16)(((R32)(CurExp - PrevExp) / (R32)(NextExp - PrevExp)) * 100.0f));

			// actdoll (2004/01/10 14:15) : 蟾鼻鎰晦
			RenderCharacter( pSurface, m_siCheckX + 25, m_siCheckY + 45, pMyOnlineWorld->pMyData->GetFollowerCharID(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot), DF_FLAG_PORTRAIT_HIGHLIGHT );
		}

		// 堅辨ж晦 幗が 轎溘
		m_Button_DismissalOK.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 鏃模ж晦 幗が 轎溘
		m_Button_DismissalCancel.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);

		// 堅辨й溥朝 辨煽 薑爾 轎溘
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		if(m_DismissalNowSelectSolder != -1)
		{
			// Name
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 50, (char*)pMyOnlineWorld->pMyData->GetFollowerCharName(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot), RGB(200, 200, 200));

			// Money
			MakeMoneyText(m_DismissalSolderList[m_DismissalNowSelectSolder].siMoney, (char*)TempBuffer1);
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_BOSANGMONEY), TempBuffer1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 70, (char*)TempBuffer, RGB(200, 200, 200));

			// LIFE
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_STRENTH), pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot));
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 90, (char*)TempBuffer, RGB(200, 200, 200));

			// MANA
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_MAGIG), pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot));
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 110, (char*)TempBuffer, RGB(200, 200, 200));
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// 堅辨ж晦 幗が Text 轎溘
		m_Button_DismissalOK.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 鏃模ж晦 幗が Text 轎溘
		m_Button_DismissalCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Check Level
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::CheckLevel(SI32 Lv, SI32 Credit)
{
	if(pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) >= Lv)     return TRUE;
	if(pMyOnlineWorld->pMyData->GetTradeGrade()              >= Credit) return TRUE;

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	餌辨й 嬴檜蠱檜 醱碟и雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::CheckItem(SI32 UseItemCount)
{
	MyItemData*		lpItemList = pMyOnlineWorld->pMyData->GetItemList(0);
	CItemHeader*	pItemHeader;
	
	for(SI32 TempCount = 0; TempCount < ON_MYDATA_MAX_ITEM_NUM; TempCount++)
	{
		if(lpItemList[TempCount].uiID != 0)
		{
			pItemHeader = pMyOnlineWorld->pItemsInfo->GetItem(lpItemList[TempCount].uiID);

			if(pItemHeader)
			{
				if((pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL) && (pItemHeader->m_siSpecial == ON_ITEM_SPECIAL_SEALINGBEAD))
				{
					if(UseItemCount <= lpItemList[TempCount].uiQuantity)
					{
						return TRUE;
					}
					return FALSE;
				}
			}
		}
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	堅辨й熱 氈朝 跨蝶攪 熱陛 腎朝雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMBarrack::CheckMonsterNum(void)
{
	SI32		NowMonsterCount;
	SI32		FollowerCount;

	NowMonsterCount = 0;
	for(FollowerCount = 0; FollowerCount < ON_MAX_FOLLOWER_NUM; FollowerCount++)
	{
		if(pMyOnlineWorld->pMyData->IsValidFollower(FollowerCount))
		{
			if(pMyOnlineWorld->pIOnlineCharKI->GetGeneral(pMyOnlineWorld->pMyData->GetFollowerParameterKind(FollowerCount)) == 2)
			{
				NowMonsterCount++;
			}
		}
	}

	if(NowMonsterCount > (ON_MAXMONSTER - 1)) return FALSE;

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	陝 辨煽滌煎 Text 雖薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnlineVillageStructureMBarrack::GetFollowerBarrackText(UI16 Kind)
{
	return pMyOnlineWorld->pIOnlineCharKI->GetDescText(pMyOnlineWorld->pIOnlineCharKI->GetIndex(Kind));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪 堅辨衛 餌辨ж朝 嬴檜蠱 檜葷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
char*	OnlineVillageStructureMBarrack::GetMonsterBuyItemName(void)
{
	return (char*)m_MonsterBuyItemName;
}

