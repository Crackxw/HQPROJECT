/************************************************************************************************
	だ橾貲 : OnlineVillageStruct-Hospital.cpp
	氬渡濠 : 檜彌戮

	葆擊 曖錳 籀葬
************************************************************************************************/
#include <GSL.h>

#include <Main.h>
#include <Mouse.h>
#include "charanimation.h"
#include <kindinfo.h>
#include <myfile.h>
#include <Player.h>
#include <debugtool.h>
#include <etc.h>


#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineHelp.h"
#include "OnlineVillage.h"
#include "OnlineVillageStruct-Hospital.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "OnlineETC.h"
#include "..\\Gersang\\Music.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "..\\Gersang\\CharInfo\\CharFileLoadManager.h"
#include "OnlineMercenary.h"

extern	_KindInfo				KI[];								// 議葛攪 謙盟滌 等檜攪 
extern	_InputDevice			IpD;
 

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureHospital::OnlineVillageStructureHospital()
{
	m_NowWork = HOSPITAL_NOWWORK_MAIN_START;
	m_Say     = 0;

	// Image	
	ZeroMemory(&m_Image_MainInterface_Back,  sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_EM,        sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_BM,        sizeof(XSPR));
	ZeroMemory(&m_Image_Portrait,             sizeof(XSPR));
//	m_Image_CharacterSlotInterface = NULL;

	// Solder Data
	ZeroMemory(m_CureSolderList, sizeof(SCureSolderData) * MAX_CURESOLDER_LIST);
	m_MaxSelectSolder = 0;
	m_NowSelectSolder = 0;
	m_Discount = 0;

	// Scroll
	m_NowScroll = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureHospital::~OnlineVillageStructureHospital()
{	
	Free();
}

BOOL	OnlineVillageStructureHospital::Action()
{
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureHospital::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{
	Free();

	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	// Load Image	
	clGrp.LoadXspr("Online\\GameSpr\\Village\\Town_Clinic.Spr",	       m_Image_MainInterface_Back);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_Image_Button_All_EM);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\main_window_button.Spr", m_Image_Button_All_BM);


//	m_Image_CharacterSlotInterface = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHARACTERSLOT);

	m_siX = 134;
	m_siY = 45;

	// Button
	BExit.Create(m_siX + 232, m_siY + 394,m_Image_Button_All_BM.Header.Xsize, m_Image_Button_All_BM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Cure.Create(m_siX + 132, m_siY + 358,       m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEAL),  BUTTON_PUT_LEFT, TRUE);
	m_Button_CureAll.Create(m_siX + 322, m_siY + 358,    m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_ALLHEAL), BUTTON_PUT_LEFT, TRUE);

	// Initialize
	m_NowWork         = HOSPITAL_NOWWORK_MAIN_START;
	m_Say             = 0;

	m_MaxSelectSolder = 0;
	m_NowSelectSolder = 0;
	m_NowScroll       = 0;
	m_Discount        = 0;

	siKind	          = ON_VIL_ST_KIND_HOSPITAL;
	m_pMyVillage	  = pVillageStructManager;


	pMyOnlineWorld->pOnlineClient->SendRequestHospitalGetCureList();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureHospital::Free()
{
	if(pMyOnlineWorld)
	{
		if(pMyOnlineWorld->pOnlineVillage)
		{
			pMyOnlineWorld->pOnlineVillage->fDead = FALSE;
		}
	}

	OnlineVillageBaseStructure::Free();
	if(m_Image_MainInterface_Back.Image)  {clGrp.FreeXspr(m_Image_MainInterface_Back);  ZeroMemory(&m_Image_MainInterface_Back,  sizeof(XSPR));}	

	if(m_Image_Button_All_EM.Image)        {clGrp.FreeXspr(m_Image_Button_All_EM);        ZeroMemory(&m_Image_Button_All_EM,        sizeof(XSPR));}
	if(m_Image_Button_All_BM.Image)        {clGrp.FreeXspr(m_Image_Button_All_BM);        ZeroMemory(&m_Image_Button_All_BM,        sizeof(XSPR));}

	if(m_Image_Portrait.Image)             {clGrp.FreeXspr(m_Image_Portrait);             ZeroMemory(&m_Image_Portrait,             sizeof(XSPR));}
//	m_Image_CharacterSlotInterface = NULL;

	ZeroMemory(m_CureSolderList, sizeof(SCureSolderData) * MAX_CURESOLDER_LIST);

	m_NowWork         = 0;
	m_Say             = 0;

	m_MaxSelectSolder = 0;
	m_NowSelectSolder = 0;
	m_NowScroll       = 0;
	m_Discount        = 0;

	siKind	          = 0;

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureHospital::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;
	BOOL			fLiveMouse;
	BOOL			TempbMouseDown;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// 葆辦蝶 謝ル蒂 掘и棻.

	TempbMouseDown = bMouseDown;
	fLiveMouse     = TRUE;
	if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE)
	{
		TempbMouseDown = FALSE;
		fLiveMouse     = FALSE;
	}

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(m_NowWork)
	{
		case HOSPITAL_NOWWORK_MAIN_START:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case HOSPITAL_NOWWORK_MAIN_STARTPOOL:
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			m_NowWork        = HOSPITAL_NOWWORK_MAIN_POOL;

			// 餌遴萄 轎溘
			SI32	TempCount;
			SI32	siSay;

			siSay = ON_TEXT_HOSPITAL_OWNERTEXT1;
			if(m_Say) siSay = ON_TEXT_HOSPITAL_OWNERTEXT5;
			if((m_MaxSelectSolder >= 1) && (m_MaxSelectSolder <= 3)) siSay = ON_TEXT_HOSPITAL_OWNERTEXT2;
			if(m_MaxSelectSolder >= 4)                               siSay = ON_TEXT_HOSPITAL_OWNERTEXT3;
			for(TempCount = 0; TempCount < m_MaxSelectSolder; TempCount++)
			{
				if(m_CureSolderList[TempCount].m_Life == 0)
				{
					siSay = ON_TEXT_HOSPITAL_OWNERTEXT4;
				}
			}

			switch(siSay)
			{
				case ON_TEXT_HOSPITAL_OWNERTEXT1:	
				{
					//棻耀濠紫 橈朝等 罹曹 鼠蝦 -_-;
					if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA)
						PushEffect(EFFECT_HOSPITAL00, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
						PushEffect(EFFECT_HOSPITAL00T, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
						PushEffect(EFFECT_HOSPITAL00J, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
						PushEffect(EFFECT_NOT_HOSPITAL_C, 0, 0);
				}
				break;

				case ON_TEXT_HOSPITAL_OWNERTEXT2:
				{
					//嬴Ъ濠陛 賃貲氈掘試.-_-;
					if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA)
						PushEffect(EFFECT_HOSPITAL10, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
						PushEffect(EFFECT_HOSPITAL10T, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
						PushEffect(EFFECT_HOSPITAL10J, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
						PushEffect(EFFECT_HOSPITAL_C, 0, 0);

				}
				break;

				case ON_TEXT_HOSPITAL_OWNERTEXT3:
				{
					if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA)
						PushEffect(EFFECT_HOSPITAL20, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
						PushEffect(EFFECT_HOSPITAL20T, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
						PushEffect(EFFECT_HOSPITAL20J, 0, 0);

				}
				break;

				case ON_TEXT_HOSPITAL_OWNERTEXT4:
				{
					//避橫陛朝 喫檜 氈掘試.
					if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA)
						PushEffect(EFFECT_HOSPITAL30, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
						PushEffect(EFFECT_HOSPITAL30T, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
						PushEffect(EFFECT_HOSPITAL30J, 0, 0);
					else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
						PushEffect(EFFECT_DIE_HOSPITAL_C,0,0);
				}
				break;
			}
			break;

		case HOSPITAL_NOWWORK_MAIN_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
				// 釭陛晦
				if(BExit.Process(TempbMouseDown) == TRUE || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )
				{
					m_NowWork = HOSPITAL_NOWWORK_MAIN_END;
				}

				// 纂猿
				m_Button_Cure.SetAction(TRUE);
				if(m_NowSelectSolder == -1) m_Button_Cure.SetAction(FALSE);
				if(m_Button_Cure.Process(TempbMouseDown) == TRUE)
				{
					if(pMyOnlineWorld->pMyData->GetMoney() == 0)
					{
						// ⑷營 絲檜 橈戲賊
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALMONEYNO));
					}
					else
					{
						if(pMyOnlineWorld->pMyData->GetMoney() < m_CureSolderList[m_NowSelectSolder].m_RevivalMoney)
						{
							// ⑷營 絲檜 睡�偕鬵� 檜ж檜賊
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALMONEYNO));
						}
						else
						{
							// Send Message
							pMyOnlineWorld->pOnlineClient->SendRequestHospitalCureSolder(m_CureSolderList[m_NowSelectSolder].m_Slot);

							m_NowWork = HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT;
						}
					}
				}

				// 瞪羹 纂猿
				m_Button_CureAll.SetAction(TRUE);
				if(m_MaxSelectSolder <= 0) m_Button_CureAll.SetAction(FALSE);
				if(m_Button_CureAll.Process(TempbMouseDown) == TRUE)
				{
					if(pMyOnlineWorld->pMyData->GetMoney() == 0)
					{
						// ⑷營 絲檜 橈戲賊
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALMONEYNO));
					}
					else
					{
						pMyOnlineWorld->pOnlineClient->SendRequestHospitalCureAllSolder();

						m_NowWork = HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT;
					}
				}

				if(TempbMouseDown)
				{
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
									goto SELECT_END;
								}
							}
						}
					}
SELECT_END:;
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
								pMyOnlineWorld->pOnlineHelp->SetTextHospital((SI16)ptMouse.x, (SI16)ptMouse.y, 
									(char*)m_CureSolderList[TempCount].m_Name
									, m_CureSolderList[TempCount].m_Money
									, m_CureSolderList[TempCount].m_RevivalMoney
									, m_CureSolderList[TempCount].m_Life
									, m_CureSolderList[TempCount].m_MaxLife
									, m_CureSolderList[TempCount].m_Mana
									, m_CureSolderList[TempCount].m_MaxMana);
								goto HELP_END;
							}
						}
					}
				}
HELP_END:

				if(IpD.IsDoubleClick() == TRUE)
				{
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

									if(pMyOnlineWorld->pMyData->GetMoney() == 0)
									{
										// ⑷營 絲檜 橈戲賊
										pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALMONEYNO));
									}
									else
									{
										if(pMyOnlineWorld->pMyData->GetMoney() < m_CureSolderList[m_NowSelectSolder].m_RevivalMoney)
										{
											// ⑷營 絲檜 睡�偕鬵� 檜ж檜賊
											pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALMONEYNO));
										}
										else
										{
											pMyOnlineWorld->pOnlineClient->SendRequestHospitalCureSolder(m_CureSolderList[m_NowSelectSolder].m_Slot);

											IpD.PrevLeftUp1Clock = 0;
											IpD.PrevLeftUp2Clock = 0;
											m_NowWork = HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT;
										}
									}
									goto SELECT1_END;
								}
							}
						}
					}
SELECT1_END:;
				}
			}
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT1:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case HOSPITAL_NOWWORK_MAIN_END:
			Draw_Select(pSurface);

			m_NowWork        = HOSPITAL_NOWWORK_MAIN_ENDPOOL;
			break;

		case HOSPITAL_NOWWORK_MAIN_ENDPOOL:
			Draw_Select(pSurface);
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
BOOL	OnlineVillageStructureHospital::Draw_Select(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hDC;
	SI16	StartPos;
//	DWORD	TempTick;
	SI32	TempXPos;
	SI32	TempYPos;
	SI32	TempCount;
	SI32	TempCount1;
	SI32	TempCount2;
	BYTE	fFlag;				// 0 : Normal 轎溘, 1 : 殘⑵霞 蝶觼煤, 2 : 雲橫雖朝 蝶觼煤
	SI32	siSay;
	char	TempBuffer[256];

	// 嬪纂 啗骯
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case HOSPITAL_NOWWORK_MAIN_STARTPOOL:
			fFlag     = 1;
			break;

		case HOSPITAL_NOWWORK_MAIN_ENDPOOL:
			fFlag     = 0;
			break;
	}


	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 蝶觼煤 轎溘
		clGrp.PutSpriteT(m_siX , m_siY, m_Image_MainInterface_Back.Header.Xsize,  m_Image_MainInterface_Back.Header.Ysize,  m_Image_MainInterface_Back.Image);
		
		// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 45, NPC_KIND_DOCTOR, pMyOnlineWorld->pOnlineVillage->GetNation() );
		
		// 堅辨ж晦 幗が 轎溘
		if(m_NowWork == HOSPITAL_NOWWORK_MAIN_POOL) m_Button_Cure.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                        m_Button_Cure.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);
		
		m_Button_CureAll.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		// 纂猿й熱 辨煽 轎溘
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
					
/*					if(m_NowSelectSolder == TempCount)
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
					// actdoll (2004/01/10 14:15) : 蟾鼻鎰晦(羹溘/葆溘 ル衛)
					SI32	CurLife	= pMyOnlineWorld->pMyData->GetFollowerParameterLife(m_CureSolderList[TempCount].m_Slot);
					SI32	MaxLife	= pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife(m_CureSolderList[TempCount].m_Slot);
					SI32	CurMana	= pMyOnlineWorld->pMyData->GetFollowerParameterMana(m_CureSolderList[TempCount].m_Slot);
					SI32	MaxMana	= pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana(m_CureSolderList[TempCount].m_Slot);
					R32		fHPRate	= ( MaxLife > 0 )	?	( (R32)CurLife / (R32)MaxLife )	:	0.0f;
					R32		fMPRate	= ( MaxMana > 0 )	?	( (R32)CurMana / (R32)MaxMana ) :	0.0f;
//					RenderCharacter(pSurface, (UI16)m_CureSolderList[TempCount].m_Kind, TempXPos + 4, TempYPos + 5, (CurLife == 0 ? TRUE : FALSE)
//						, (SI16)(((R32)CurLife / (R32)MaxLife) * 100.0f), (SI16)(((R32)CurMana / (R32)MaxMana) * 100.0f));

					// 摹鷗脹 辨煽檜 氈擊 唳辦 ж檜塭檜お 鎰橫遽棻.
					SI32	siDrawFlag = 0;
					if( m_NowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_CureSolderList[TempCount].m_Kind, siDrawFlag, true, fHPRate, fMPRate );
				}
			}
		}
		
		
		
		// 釭陛晦 幗が 轎溘
		if(m_NowWork == HOSPITAL_NOWWORK_MAIN_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                        BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		
		
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

				// 曖錳 渠餌 轎溘
				if(pMyOnlineWorld->pOnlineVillage->fDead)
				{
					siSay = ON_TEXT_HOSPITAL_OWNERTEXT4;
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 50, 402, pMyOnlineWorld->pOnlineText->Get(siSay));
				}
				else
				{
					siSay = ON_TEXT_HOSPITAL_OWNERTEXT1;
					if(m_Say) siSay = ON_TEXT_HOSPITAL_OWNERTEXT5;
					if((m_MaxSelectSolder >= 1) && (m_MaxSelectSolder <= 3)) siSay = ON_TEXT_HOSPITAL_OWNERTEXT2;
					if(m_MaxSelectSolder >= 4)                               siSay = ON_TEXT_HOSPITAL_OWNERTEXT3;
					for(TempCount = 0; TempCount < m_MaxSelectSolder; TempCount++)
					{
						if(m_CureSolderList[TempCount].m_Life == 0)
						{
							siSay = ON_TEXT_HOSPITAL_OWNERTEXT4;
						}
					}

					if(siSay == ON_TEXT_HOSPITAL_OWNERTEXT5)
					{
						pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 50, 402, pMyOnlineWorld->pOnlineText->Get(siSay));
					}
					else
					{
						if(m_Discount > 0)
						{
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_OWNERTEXT_DISCOUNT));
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 50, 402, (char*)TempBuffer);
						}
						else if(m_Discount < 0)
						{
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_OWNERTEXT_INCREASE));
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 50, 402, (char*)TempBuffer);
						}					
						else
						{
							pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 50, 402, pMyOnlineWorld->pOnlineText->Get(siSay));
						}
					}
				}
				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

				// 堅辨ж晦 幗が Text 轎溘
				m_Button_Cure.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_Button_CureAll.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;
		}

		// 曖錳 旋濠 轎溘
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207 ,m_siY + 7,118,13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), RGB(200, 200, 200));

		// 釭陛晦 幗が Text 轎溘
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 Follower曖 鼻鷓蒂 匐儀
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureHospital::FindFollowerData(void)
{
	SI32					TempCount;
	IOnlineFollower*		lpIOnlineFollower;
	OnlineFollower*			lpOnlineFollower;

	lpIOnlineFollower = pMyOnlineWorld->pMyData->GetFollowersInterface();
	for(TempCount = 0; TempCount < m_MaxSelectSolder; TempCount++)
	{
		lpOnlineFollower = lpIOnlineFollower->GetFollower(m_CureSolderList[TempCount].m_Slot);

		m_CureSolderList[TempCount].m_Kind                   = pMyOnlineWorld->pMyData->GetFollowerCharID(m_CureSolderList[TempCount].m_Slot);
		m_CureSolderList[TempCount].m_Life					 = lpOnlineFollower->ParametaBox.IP.GetLife();
		m_CureSolderList[TempCount].m_MaxLife				 = lpOnlineFollower->ParametaBox.GetMaxLife();
		m_CureSolderList[TempCount].m_Mana					 = lpOnlineFollower->ParametaBox.IP.GetMana();
		m_CureSolderList[TempCount].m_MaxMana				 = lpOnlineFollower->ParametaBox.GetMaxMana();		

		ZeroMemory(m_CureSolderList[TempCount].m_Name, ON_ID_LENGTH + 1);
		if(pMyOnlineWorld->pIOnlineCharKI->GetClass(pMyOnlineWorld->pIOnlineCharKI->GetIndex(m_CureSolderList[TempCount].m_Kind)) == ON_CHAR_KI_CLASS_PLAYER)
		{
			sprintf((char*)m_CureSolderList[TempCount].m_Name, "%s", pMyOnlineWorld->pMyData->GetMyCharName());
		}
		else
		{			
			sprintf((char*)m_CureSolderList[TempCount].m_Name, "%s", lpOnlineFollower->Name);
		}
	}
}
