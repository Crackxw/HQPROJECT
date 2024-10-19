/************************************************************************************************
	���ϸ� : OnlineVillageStruct-GBarrack.cpp
	����� : ������

	���� �Ʒü� ó��
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
#include "OnlineVillageStruct-GBarrack.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "..\\Gersang\\Music.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
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
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 

extern _InputDevice				IpD;


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageStructureGBarrack::OnlineVillageStructureGBarrack()
{
	m_NowWork = GBARRACK_NOWWORK_BUY_START;
	m_Say     = 0;

	// Image
	ZeroMemory(&m_Image_MainInterface_Back1,    sizeof(XSPR));
	ZeroMemory(&m_Image_CheckInterface,         sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_MM,          sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_EM,          sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_BM,          sizeof(XSPR));
	ZeroMemory(&m_Image_Portrait,               sizeof(XSPR));
//	m_Image_CharacterSlotInterface = NULL;
	
	// Solder Data
	ZeroMemory(m_BuySolderList,       sizeof(OnInnBuySolderInfo) * MAX_BUYGSOLDER_LIST);
	m_BuyMaxSelectSolder = 0;
	m_BuyNowSelectSolder = 0;
	ZeroMemory(m_ChangeJobSolderList, sizeof(OnChangeJobSolderInfo) * MAX_BUYGSOLDER_LIST);
	m_ChangeJobMaxSelectSolder = 0;
	m_ChangeJobNowSelectSolder = 0;
	ZeroMemory(m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYGSOLDER_LIST);
	m_DismissalMaxSelectSolder = 0;
	m_DismissalNowSelectSolder = 0;
	m_Discount = 0;

	// Scroll
	m_NowScroll = 0;
	SetGeneralExMode(FALSE);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageStructureGBarrack::~OnlineVillageStructureGBarrack()
{	
	Free();
}

BOOL	OnlineVillageStructureGBarrack::Action()
{
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillageStructureGBarrack::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{	
	Free();

	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	// Load Image
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_hireling.spr",	       m_Image_MainInterface_Back1);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW.Spr", m_Image_CheckInterface);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_tab.Spr",  m_Image_Button_All_MM);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_Image_Button_All_EM);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\main_window_button.Spr", m_Image_Button_All_BM);
	clGrp.LoadXspr("Online\\GameSpr\\inventory\\Sub_Button.Spr",m_Image_Check_Button);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW_LINE.Spr",m_Image_Line_Check_Button);

	m_siX = 134;
	m_siY = 45;

	m_siCheckX = (800 - m_Image_CheckInterface.Header.Xsize)/2;
	m_siCheckY = (600 - m_Image_CheckInterface.Header.Ysize)/2;

	// Button
	m_Button_Main_Buy.Create(m_siX + 63, m_siY + 31,       m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_EMPOLYMENT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_ChangeJob.Create(m_siX + 205, m_siY + 31, m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_CHANGEJOB), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_Dismissal.Create(m_siX + 348, m_siY + 31, m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE), BUTTON_PUT_LEFT, TRUE);

	m_Button_Main_General.Create(m_siX + 130,m_siY + 31,m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_NORMAL_GENERAL), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_GeneralEx.Create(m_siX + 276,m_siY + 31,m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize + 5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_UPGRADE_GENERAL), BUTTON_PUT_LEFT, TRUE);

	BExit.Create(m_siX + 232, m_siY + 394,						m_Image_Button_All_BM.Header.Xsize, m_Image_Button_All_BM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_EXIT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Buy.Create(m_siX + 227, m_siY + 366,				m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_EMPOLYMENT),  BUTTON_PUT_LEFT, TRUE);
	m_Button_BuyOK.Create(m_siCheckX + 50 , m_siCheckY + 148,	m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_EMPOLYMENT),  BUTTON_PUT_LEFT, TRUE);
	m_Button_BuyCancel.Create(m_siCheckX +170 , m_siCheckY + 148,m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_ChangeJob.Create(m_siX + 227, m_siY + 366,			m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_CHANGEJOB),  BUTTON_PUT_LEFT, TRUE);
	m_Button_ChangeJobOK.Create(m_siCheckX + 50 , m_siCheckY + 148,m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_CHANGEJOB),  BUTTON_PUT_LEFT, TRUE);
	m_Button_ChangeJobCancel.Create(m_siCheckX +170 , m_siCheckY + 148,m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_Dismissal.Create(m_siX + 227, m_siY + 366,			m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_DismissalOK.Create(m_siCheckX +50 , m_siCheckY + 148,	m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE),  BUTTON_PUT_LEFT, TRUE);
	m_Button_DismissalCancel.Create(m_siCheckX +170 , m_siCheckY + 148,	m_Image_Check_Button.Header.Xsize, m_Image_Check_Button.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE),  BUTTON_PUT_LEFT, TRUE);	

	// Initialize
	m_NowWork         = GBARRACK_NOWWORK_CHANGEGENERAL_SELECT;
	m_Say             = 0;

	m_BuyMaxSelectSolder = 0;
	m_BuyNowSelectSolder = 0;
	m_ChangeJobMaxSelectSolder = 0;
	m_ChangeJobNowSelectSolder = 0;
	m_DismissalMaxSelectSolder = 0;
	m_DismissalNowSelectSolder = 0;

	m_Discount = 0;

	m_NowScroll       = 0;

	siKind	          = ON_VIL_ST_KIND_GBARRACK;
	SetGeneralExMode(FALSE);

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillageStructureGBarrack::Free()
{
	OnlineVillageBaseStructure::Free();

	if(m_Image_MainInterface_Back1.Image)  {clGrp.FreeXspr(m_Image_MainInterface_Back1);  ZeroMemory(&m_Image_MainInterface_Back1,  sizeof(XSPR));}
	if(m_Image_CheckInterface.Image)       {clGrp.FreeXspr(m_Image_CheckInterface);       ZeroMemory(&m_Image_CheckInterface,       sizeof(XSPR));}

	if(m_Image_Button_All_MM.Image)        {clGrp.FreeXspr(m_Image_Button_All_MM);        ZeroMemory(&m_Image_Button_All_MM,        sizeof(XSPR));}
	if(m_Image_Button_All_EM.Image)        {clGrp.FreeXspr(m_Image_Button_All_EM);        ZeroMemory(&m_Image_Button_All_EM,        sizeof(XSPR));}
	if(m_Image_Button_All_BM.Image)        {clGrp.FreeXspr(m_Image_Button_All_BM);        ZeroMemory(&m_Image_Button_All_BM,        sizeof(XSPR));}

	if(m_Image_Check_Button.Image)		   {clGrp.FreeXspr(m_Image_Check_Button);		  ZeroMemory(&m_Image_Check_Button,			sizeof(XSPR));}
	if(m_Image_Line_Check_Button.Image)	   {clGrp.FreeXspr(m_Image_Line_Check_Button);	  ZeroMemory(&m_Image_Line_Check_Button,	sizeof(XSPR));}

	if(m_Image_Portrait.Image)             {clGrp.FreeXspr(m_Image_Portrait);             ZeroMemory(&m_Image_Portrait,             sizeof(XSPR));}

	ZeroMemory(m_BuySolderList,       sizeof(OnInnBuySolderInfo) * MAX_BUYGSOLDER_LIST);
	ZeroMemory(m_ChangeJobSolderList, sizeof(OnChangeJobSolderInfo) * MAX_BUYGSOLDER_LIST);
	ZeroMemory(m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYGSOLDER_LIST);

	m_NowWork         = 0;
	m_Say             = 0;

	m_BuyMaxSelectSolder = 0;
	m_BuyNowSelectSolder = 0;
	m_ChangeJobMaxSelectSolder = 0;
	m_ChangeJobNowSelectSolder = 0;
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�׼�.	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Poll(LPDIRECTDRAWSURFACE7 pSurface)
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

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// ���콺 ��ǥ�� ���Ѵ�.

	TempbMouseDown = bMouseDown;
	fLiveMouse     = TRUE;
	if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE)
	{
		TempbMouseDown = FALSE;
		fLiveMouse     = FALSE;
	}

	m_Button_Main_Buy.SetAction(FALSE);
	m_Button_Main_ChangeJob.SetAction(FALSE);
	m_Button_Main_Dismissal.SetAction(FALSE);

	if(m_Button_Main_Buy.IsInMouse())       pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_BUY),       (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE); 
	if(m_Button_Main_ChangeJob.IsInMouse()) pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_CHANGEJOB), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE);
	if(m_Button_Main_Dismissal.IsInMouse()) pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_DISMISSAL), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE);


	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(m_NowWork)
	{
		case GBARRACK_NOWWORK_BUY_START:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_BUY_STARTPOOL:
			Draw_BuySelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			m_NowWork        = GBARRACK_NOWWORK_BUY_POOL;
			break;
		case GBARRACK_NOWWORK_CHANGEGENERAL_SELECT:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_Buy.SetAction(FALSE);
			else
			{
				m_Button_Main_General.SetAction(TRUE);
				m_Button_Main_GeneralEx.SetAction(TRUE);

				if((BExit.Process(TempbMouseDown) == TRUE) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_bGeneralExMode = FALSE;
					m_NowWork = GBARRACK_NOWWORK_BUY_END;
				}
				if(m_Button_Main_General.Process(TempbMouseDown) == TRUE)
				{
					m_bGeneralExMode = FALSE;
					pMyOnlineWorld->pOnlineClient->SendRequestGBarrackEnter();
				}
				if(m_Button_Main_GeneralEx.Process(TempbMouseDown) == TRUE)
				{
					m_bGeneralExMode  = TRUE;
					//pMyOnlineWorld->pOnlineClient->SendRequestGBarrackGeneralExEnter();
					pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeGeneralExEnter();
				}
			}
			Draw_ChangeGeneralExWait(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;
		case GBARRACK_NOWWORK_BUY_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_Buy.SetAction(FALSE);
			else
			{
				m_Button_Main_ChangeJob.SetAction(TRUE);
				m_Button_Main_Dismissal.SetAction(TRUE);
				
				// ������
				if((BExit.Process(TempbMouseDown) == TRUE) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_bGeneralExMode = FALSE;
					m_NowWork = GBARRACK_NOWWORK_BUY_END;
				}

				if(m_Button_Main_ChangeJob.Process(TempbMouseDown) == TRUE)
				{
					//��� ������ ����Ʈ�� ��û.
					if(GetGeneralExMode() == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeJobEnter();
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeGeneralExEnter();

					m_NowWork = GBARRACK_NOWWORK_CHANGEJOB_GETLIST;
				}				
				if(m_Button_Main_Dismissal.Process(TempbMouseDown) == TRUE)
				{
					// �ذ� �� �� �ִ� �뺴�� List ��û
					if(GetGeneralExMode() == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackDismissalEnter();
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackDismissalGeneralExEnter();

					m_NowWork = GBARRACK_NOWWORK_DISMISSAL_GETLIST;
				}

				// ���
				m_Button_Buy.SetAction(TRUE);
				if(m_BuyNowSelectSolder == -1) m_Button_Buy.SetAction(FALSE);
				switch(pMyOnlineWorld->pOnlineVillage->GetNation())
				{
					case ON_COUNTRY_JAPAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
						m_Button_Buy.SetAction(FALSE);
						break;

					case ON_COUNTRY_CHINA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
						m_Button_Buy.SetAction(FALSE);
						break;

					case ON_COUNTRY_TAIWAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
						m_Button_Buy.SetAction(FALSE);
						break;

					case ON_COUNTRY_KOREA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
						m_Button_Buy.SetAction(FALSE);
						break;
				}
				if(m_Button_Buy.Process(TempbMouseDown) == TRUE)
				{
					// ���� Level�� ���Ҽ� ������ OK
					switch(CheckLevel(m_BuySolderList[m_BuyNowSelectSolder].siCode, m_BuySolderList[m_BuyNowSelectSolder].siLevel, m_BuySolderList[m_BuyNowSelectSolder].siCredit))
					{
						case 0:		// ���԰���
							// ���� ������ �ڸ��� ������ ��� ������ Error Message
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
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOSOLIDER));
							}
							else
							{
								SI16		MaxFollower;
								char		TempBuffer[256];

								MaxFollower = pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
								if((11 - TempUnitCount + 1) > MaxFollower)
								{
									sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOWSOLIDER), MaxFollower);
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), (char*)TempBuffer);
								}
								else
								{
									if(pMyOnlineWorld->pMyData->GetMoney() < m_BuySolderList[m_BuyNowSelectSolder].siMoney)
									{
										pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_NOMONEY));
									}
									else
									{
										m_NowWork = GBARRACK_NOWWORK_BUYCHECK_POOL;
									}
								}
							}
							break;

						case 2:		// ���� ����� ������ ����
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_ISSAMEGENERAL));
							break;
						case 1:		// �ſ뵵 ����
						default:
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_TRADELIMIT), m_BuySolderList[m_BuyNowSelectSolder].siCredit);
							break;
					}
				}

				if(TempbMouseDown)
				{
					// �뺴 List
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_BuyMaxSelectSolder)
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
									m_BuyNowSelectSolder = TempCount;
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
						if(TempCount < m_BuyMaxSelectSolder)
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
								MakeName( m_BuySolderList[TempCount].SolderInfo.uiKind, m_BuySolderList[TempCount].uiCharNameAddOption, (BYTE*)TempBuffer, 256 );

								pMyOnlineWorld->pOnlineHelp->SetTextBuySolder((SI16)ptMouse.x, (SI16)ptMouse.y, (char*)TempBuffer
									, m_BuySolderList[TempCount].siMoney
									, m_BuySolderList[TempCount].SolderInfo.siLife, m_BuySolderList[TempCount].SolderInfo.siMana, m_BuySolderList[TempCount].SolderInfo.siStr, m_BuySolderList[TempCount].SolderInfo.siDex, m_BuySolderList[TempCount].SolderInfo.siVit, m_BuySolderList[TempCount].SolderInfo.siInt
									, m_BuySolderList[TempCount].siLevel, pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)
									, m_BuySolderList[TempCount].siCredit, pMyOnlineWorld->pMyData->GetTradeGrade());
								goto END_HELP;
							}
						}
					}
				}
END_HELP:

				if(IpD.IsDoubleClick() == TRUE)
				{
					// �뺴 List
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_BuyMaxSelectSolder)
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
									m_BuyNowSelectSolder = TempCount;

									if(m_Button_Buy.IsAction())
									{
										// ���� Level�� ���Ҽ� ������ OK
										switch(CheckLevel(m_BuySolderList[m_BuyNowSelectSolder].siCode, m_BuySolderList[m_BuyNowSelectSolder].siLevel, m_BuySolderList[m_BuyNowSelectSolder].siCredit))
										{
											case 0:		// ���԰���
												// ���� ������ �ڸ��� ������ ��� ������ Error Message
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
													pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOSOLIDER));
												}
												else
												{
													SI16		MaxFollower;
													char		TempBuffer[256];

													MaxFollower = pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
													if((11 - TempUnitCount + 1) > MaxFollower)
													{
														sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOWSOLIDER), MaxFollower);
														pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), (char*)TempBuffer);
													}
													else
													{
														if(pMyOnlineWorld->pMyData->GetMoney() < m_BuySolderList[m_BuyNowSelectSolder].siMoney)
														{
															pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_NOMONEY));
														}
														else
														{
															m_NowWork = GBARRACK_NOWWORK_BUYCHECK_POOL;
														}
													}
												}
												break;

											case 2:		// ���� ����� ������ ����
												pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_ISSAMEGENERAL));
												break;

//											case 3:		// ����� �ʰ�
//												pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_MANYGENERAL), pMyOnlineWorld->pMyData->siMaxGeneral);
//												break;

											case 1:		// �ſ뵵 ����
											default:
												pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_TRADELIMIT), m_BuySolderList[m_BuyNowSelectSolder].siCredit);
												break;
										}
									}
									break;
								}
							}
						}
					}
				}
			}

			Draw_BuySelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_BUYCHECK_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_Buy.SetAction(FALSE);
			else
			{
				m_Button_Buy.SetAction(FALSE);

				if(m_Button_BuyOK.Process(TempbMouseDown) == TRUE)
				{
					BYTE		TempBuffer[256];

					// Send Message
					MakeName( m_BuySolderList[m_BuyNowSelectSolder].SolderInfo.uiKind, m_BuySolderList[m_BuyNowSelectSolder].uiCharNameAddOption, (BYTE*)TempBuffer, 256 );
					if(m_bGeneralExMode == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackBuySolder(m_BuySolderList[m_BuyNowSelectSolder].siCode, (BYTE*)TempBuffer);
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackBuyGeneralEx(m_BuySolderList[m_BuyNowSelectSolder].siCode, (BYTE*)TempBuffer);

					m_NowWork = GBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT;
				}
				else
				{
					if(m_Button_BuyCancel.Process(TempbMouseDown) == TRUE)
					{
						m_NowWork = GBARRACK_NOWWORK_BUY_POOL;
					}
				}
//				}
			}

			Draw_BuySelect(pSurface);
			Draw_BuyCheck(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_BuySelect(pSurface);
			Draw_BuyCheck(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_BUY_END:
			Draw_BuySelect(pSurface);

			m_NowWork        = GBARRACK_NOWWORK_BUY_ENDPOOL;
//			m_StartTickCount = GetTickCount();
			break;

		case GBARRACK_NOWWORK_BUY_ENDPOOL:
			Draw_BuySelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			Free();
			return TRUE;
			break;

		case GBARRACK_NOWWORK_CHANGEJOB_GETLIST:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_ChangeJobSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_CHANGEJOB_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_ChangeJob.SetAction(FALSE);
			else
			{
				m_Button_Main_Dismissal.SetAction(TRUE);
				if(m_bGeneralExMode == FALSE)
					m_Button_Main_Buy.SetAction(TRUE);
				else if(m_bGeneralExMode == TRUE)
					m_Button_Main_Buy.SetAction(FALSE);

				// ������
				if((BExit.Process(TempbMouseDown) == TRUE) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_NowWork = GBARRACK_NOWWORK_CHANGEJOB_END;
				}
				if(m_Button_Main_Buy.Process(TempbMouseDown) == TRUE)
				{
					//��� ��� ��û.
					if(GetGeneralExMode() == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackEnter();
					//else
					//	pMyOnlineWorld->pOnlineClient->SendRequestGBarrackGeneralExEnter();
					m_NowWork = GBARRACK_NOWWORK_BUY_POOL;
				}
				if(m_Button_Main_Dismissal.Process(TempbMouseDown) == TRUE)
				{
					if(GetGeneralExMode() == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackDismissalEnter();
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackDismissalGeneralExEnter();
					m_NowWork = GBARRACK_NOWWORK_DISMISSAL_GETLIST;
				}
				// ����
				m_Button_ChangeJob.SetAction(TRUE);
				if(m_ChangeJobNowSelectSolder == -1)
				{
					m_Button_ChangeJob.SetAction(FALSE);
				}
				else
				{
					if(CheckChangeJob(m_ChangeJobNowSelectSolder) == FALSE)
					{
						m_Button_ChangeJob.SetAction(FALSE);
					}
				}
				switch(pMyOnlineWorld->pOnlineVillage->GetNation())
				{
					case ON_COUNTRY_JAPAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
						{
							m_Button_ChangeJob.SetAction(FALSE);
						}
						break;

					case ON_COUNTRY_CHINA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
						{
							m_Button_ChangeJob.SetAction(FALSE);
						}
						break;

					case ON_COUNTRY_TAIWAN:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
						{
							m_Button_ChangeJob.SetAction(FALSE);
						}
						break;

					case ON_COUNTRY_KOREA:
						if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
						{
							m_Button_ChangeJob.SetAction(FALSE);
						}
						break;
				}
				if(m_Button_ChangeJob.Process(TempbMouseDown) == TRUE)
				{
					m_ChangeJobSelectType = 0;
					m_NowWork             = GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL;
				}

				if(TempbMouseDown)
				{
					// �뺴 List
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_ChangeJobMaxSelectSolder)
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
									m_ChangeJobNowSelectSolder = TempCount;
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
						if(TempCount < m_ChangeJobMaxSelectSolder)
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
								if(!m_ChangeJobSolderList[TempCount].siJobListFlag[0])
									pMyOnlineWorld->pOnlineHelp->SetTextChangeJobSolder((SI16)ptMouse.x, (SI16)ptMouse.y, m_ChangeJobSolderList[TempCount].siSlot, m_ChangeJobSolderList[TempCount].siJobList, m_ChangeJobSolderList[TempCount].siJobListFlag, m_ChangeJobSolderList[TempCount].siJobListChangeLevel);
								else
								{
									UI08 uiSlot = m_ChangeJobSolderList[TempCount].siSlot;
									pMyOnlineWorld->pOnlineHelp->SetTextChangeJobGeneral((SI16)ptMouse.x, (SI16)ptMouse.y
										, pMyOnlineWorld->pMyData->GetFollowerCharName(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterLevel(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterLife(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterMana(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterTotalStr(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterTotalDex(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterTotalVit(uiSlot)
										, pMyOnlineWorld->pMyData->GetFollowerParameterTotalInt(uiSlot));
								}

								goto END_HELP_CHANGEJOB;
							}
						}
					}
				}
END_HELP_CHANGEJOB:

				if(IpD.IsDoubleClick() == TRUE)
				{
					// �뺴 List
					for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
					{
						for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
						{
							TempCount = (TempCount2 * 6) + TempCount1;
							if(TempCount < m_ChangeJobMaxSelectSolder)
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
									m_ChangeJobNowSelectSolder = TempCount;

									if(CheckChangeJob(m_ChangeJobNowSelectSolder) == FALSE)
									{
										m_Button_ChangeJob.SetAction(FALSE);
									}
									if(m_Button_ChangeJob.IsAction())
									{
										m_ChangeJobSelectType = 0;
										m_NowWork             = GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL;
									}
									break;
								}
							}
						}
					}
				}
			}

			Draw_ChangeJobSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_ChangeJob.SetAction(FALSE);
			else
			{
				SI32		TempMaxGeneralSelect;
				SI32		TempCount;
				SI32		TempPos;
				SI32		TempStartPos;
				SI32		TempSize;

				m_Button_ChangeJob.SetAction(FALSE);

				// ��ư ó��
				if(m_ChangeJobSelectType != -1)
				{
					if(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobListFlag[m_ChangeJobSelectType])
					{
						//GetAsyncKeyState(VK_RETURN) || 
						if((m_Button_ChangeJobOK.Process(TempbMouseDown) == TRUE))
						{
							// ���� �������� �˻�
							switch(CheckChangeJobG(m_ChangeJobNowSelectSolder, m_ChangeJobSelectType))
							{
								case 0:			// ����
									// Send Message
									if(m_bGeneralExMode == FALSE)
										pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeJobSolder(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siSlot, m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[m_ChangeJobSelectType]);
									else
										pMyOnlineWorld->pOnlineClient->SendRequestGBarrackBuyChangeGeneralEx(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siSlot, m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[m_ChangeJobSelectType]);

									m_NowWork = GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL_RETURNWAIT;
									break;
									
								case 2:			// ���� ����� ������ ����
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_ISSAMEGENERAL));
									break;
									
//								case 3:			// ����� �ʰ�
//									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_MANYGENERAL), pMyOnlineWorld->pMyData->siMaxGeneral);
//									break;
									
								case 1:			// ��������
								default:
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
									break;
							}
						}
					}
				}

				if(m_Button_ChangeJobCancel.Process(TempbMouseDown) == TRUE)
				{
					m_NowWork = GBARRACK_NOWWORK_CHANGEJOB_POOL;
				}

				// ���� �ٲܼ� �ִ� ����� �˻�
				TempMaxGeneralSelect = 0;
				for(TempCount = 0; TempCount < 4; TempCount++)
				{
					if(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount] != 0)
					{
						TempMaxGeneralSelect++;
					}
				}

				// ��ġ �˻�
				TempSize     = 250 / TempMaxGeneralSelect;
				TempStartPos = 315 + (TempSize / 2) - (68 / 2);

				// ���콺 ����
				TempPos = TempStartPos;
				for(TempCount = 0; TempCount < TempMaxGeneralSelect; TempCount++)
				{
					if(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount] != 0)
					{
						if((ptMouse.x >= TempPos) && (ptMouse.x < (TempPos + 68)) && (ptMouse.y >= 205) && (ptMouse.y < (205 + 68)))
						{
							if(TempbMouseDown)
							{
								m_ChangeJobSelectType = TempCount;
							}

							// Help
							CharHead*	lpCharHead = pMyOnlineWorld->pIOnlineCharKI->GetCharHeader(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount]);

							pMyOnlineWorld->pOnlineHelp->SetTextChangeJobGeneral((SI16)ptMouse.x, (SI16)ptMouse.y
								, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->pIOnlineCharKI->GetName(pMyOnlineWorld->pIOnlineCharKI->GetIndex(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount])))
								, 0, lpCharHead->Parameta.GetLife(), lpCharHead->siManaMax
								, lpCharHead->Parameta.GetStr()
								, lpCharHead->Parameta.GetDex()
								, lpCharHead->Parameta.GetVit()
								, lpCharHead->Parameta.GetInt());
						}

						TempPos += TempSize;
					}
				}
			}

			Draw_ChangeJobSelect(pSurface);
			Draw_ChangeJobCheck(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_ChangeJobSelect(pSurface);
			Draw_ChangeJobCheck(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_CHANGEJOB_END:
			Draw_ChangeJobSelect(pSurface);

			m_NowWork        = GBARRACK_NOWWORK_CHANGEJOB_ENDPOOL;
//			m_StartTickCount = GetTickCount();
			break;

		case GBARRACK_NOWWORK_CHANGEJOB_ENDPOOL:
			Draw_ChangeJobSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			Free();
			return TRUE;
			break;

		case GBARRACK_NOWWORK_DISMISSAL_GETLIST:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_DismissalSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_DISMISSAL_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_Dismissal.SetAction(FALSE);
			else
			{
				m_Button_Main_ChangeJob.SetAction(TRUE);
				if(m_bGeneralExMode == FALSE)
					m_Button_Main_Buy.SetAction(TRUE);
				else if(m_bGeneralExMode == TRUE)
					m_Button_Main_Buy.SetAction(FALSE);

				// ������
				if((BExit.Process(TempbMouseDown) == TRUE) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_NowWork = GBARRACK_NOWWORK_DISMISSAL_END;
				}
				if(m_Button_Main_Buy.Process(TempbMouseDown) == TRUE)
				{
					if(GetGeneralExMode() == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackEnter();
					//else
					//	pMyOnlineWorld->pOnlineClient->SendRequestGBarrackGeneralExEnter();
					m_NowWork = GBARRACK_NOWWORK_BUY_POOL;
				}				
				if(m_Button_Main_ChangeJob.Process(TempbMouseDown) == TRUE)
				{
					// ���� �� �� �ִ� �뺴�� List ��û
					if(GetGeneralExMode() == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeJobEnter();
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeGeneralExEnter();
					m_NowWork = GBARRACK_NOWWORK_CHANGEJOB_GETLIST;
				}				
/*
				if(m_Button_Main_Dismissal.Process(TempbMouseDown) == TRUE)
				{
					// �ذ� �� �� �ִ� �뺴�� List ��û
					m_bGeneralExMode = FALSE;
					pMyOnlineWorld->pOnlineClient->SendRequestGBarrackDismissalEnter();
					m_NowWork = GBARRACK_NOWWORK_DISMISSAL_GETLIST;
				}
				if(m_Button_Main_ChangeGeneral.Process(TempbMouseDown) == TRUE)
				{
					m_bGeneralExMode = TRUE;
					m_NowWork = GBARRACK_NOWWORK_CHANGEGENERAL_POOL;
				}
*/
				//////////////////////////////////////////////////////////////////////////////////
				//2�� ���� ����
				//////////////////////////////////////////////////////////////////////////////////
				/*
				if(m_Button_GeneralExChange.Process(TempbMouseDown) == TRUE)
				{
					// 2�� ��� ���� (����)�� ����Ʈ�� ��û.
					m_bGeneralExMode = TRUE;
					pMyOnlineWorld->pOnlineClient->SendRequestGBarrackChangeGeneralExEnter();
				}
				if(m_Button_GeneralExBuy.Process(TempbMouseDown) == TRUE)
				{
					// 2�� ��� ���� (���)�� ����Ʈ�� ��û.
					m_bGeneralExMode = TRUE;
					pMyOnlineWorld->pOnlineClient->SendRequestGBarrackGeneralExEnter();
				}*/

				// �ذ�
				m_Button_Dismissal.SetAction(TRUE);
				if(m_DismissalNowSelectSolder == -1) m_Button_Dismissal.SetAction(FALSE);
				if(m_Button_Dismissal.Process(TempbMouseDown) == TRUE)
				{
					// ���� Level�� ���Ҽ� ������ OK
					m_NowWork = GBARRACK_NOWWORK_DISMISSALCHECK_POOL;					
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

								if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68))))
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

							if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68))))
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

									m_NowWork = GBARRACK_NOWWORK_DISMISSALCHECK_POOL;
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

		case GBARRACK_NOWWORK_DISMISSALCHECK_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
				m_Button_Dismissal.SetAction(FALSE);
			else
			{
				m_Button_Dismissal.SetAction(FALSE);

				if(m_Button_DismissalOK.Process(TempbMouseDown) == TRUE)
				{
					// Send Message
					if(m_bGeneralExMode == FALSE)
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackDismissalSolder(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);
					else
						pMyOnlineWorld->pOnlineClient->SendRequestGBarrackSellDismissalGeneralEx(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot);

					m_NowWork = GBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT;
				}
				else
				{
					if(m_Button_DismissalCancel.Process(TempbMouseDown) == TRUE)
					{
						m_NowWork = GBARRACK_NOWWORK_DISMISSAL_POOL;
					}
				}
			}

			Draw_DismissalSelect(pSurface);
			Draw_DismissalCheck(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			
			Draw_DismissalSelect(pSurface);
			Draw_DismissalCheck(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GBARRACK_NOWWORK_DISMISSAL_END:
			Draw_DismissalSelect(pSurface);

			m_NowWork        = GBARRACK_NOWWORK_DISMISSAL_ENDPOOL;
//			m_StartTickCount = GetTickCount();
			break;

		case GBARRACK_NOWWORK_DISMISSAL_ENDPOOL:
			Draw_DismissalSelect(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			Free();
			return TRUE;
			break;
	}

	// �뺴
	if(pMyOnlineWorld->m_pMercenary->IsAction())
	{
		pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// �κ��丮.
	if(pMyOnlineWorld->pOnlineInventory->IsActive())
	{
		pMyOnlineWorld->pOnlineInventory->SetFlagVillage(TRUE);
		pMyOnlineWorld->pOnlineInventory->Action();
		pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// ���
	if(pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
		pMyOnlineWorld->pOnlineTradeBook->Poll();
		pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// �̴ϸ�
	if(pMyOnlineWorld->pOnlineMiniMap->IsActive())
	{
		pMyOnlineWorld->pOnlineMiniMap->Pool();
		pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// Pannel�� ����Ѵ�.
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_OPTION
		| ONLINEPANNEL_DISABLEFLAG_ATTACK | ONLINEPANNEL_DISABLEFLAG_IMOTICON);

	if(IpD.LeftPressSwitch) bMouseDown = TRUE;
	else				    bMouseDown = FALSE;

	return FALSE;
}

BOOL	OnlineVillageStructureGBarrack::Draw_ChangeGeneralExWait(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI32			TempSelectCharacterIndex = -1;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// ���콺 ��ǥ�� ���Ѵ�.


	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// ���� ���
		// ��ũ�� ���
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_Back1.Header.Xsize, m_Image_MainInterface_Back1.Header.Ysize, m_Image_MainInterface_Back1.Image);

		// actdoll (2004/01/06 17:08) : �ٸ� ������� NPC �ʻ�׸���
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GBARRACK, pMyOnlineWorld->pOnlineVillage->GetNation() );

		m_Button_Main_General.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_GeneralEx.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);		

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		// �Ʒü� ���� ���
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207 ,m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), RGB(200, 200, 200));

		// ������ ��ư Text ���
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		m_Button_Main_General.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_GeneralEx.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw Main
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Draw_BuySelect(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			StartPos;
	SI32			TempCount;
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;
	char			TempBuffer[256];
	BYTE			fFlag;				// 0 : Normal ���, 1 : ������ ��ũ��, 2 : �������� ��ũ��
	SI32			TempSelectCharacterIndex = -1;
	SI32			TempSay;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// ���콺 ��ǥ�� ���Ѵ�.

	// ��ġ ���
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case GBARRACK_NOWWORK_BUY_STARTPOOL:
			fFlag     = 1;
			break;

		case GBARRACK_NOWWORK_BUY_ENDPOOL:
			fFlag     = 0;
			break;
	}

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// ���� ���
		// ��ũ�� ���
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_Back1.Header.Xsize, m_Image_MainInterface_Back1.Header.Ysize, m_Image_MainInterface_Back1.Image);

		// actdoll (2004/01/06 17:08) : �ٸ� ������� NPC �ʻ�׸���
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GBARRACK, pMyOnlineWorld->pOnlineVillage->GetNation() );
		
		// ����ϱ� ��ư ���
		if(m_NowWork == GBARRACK_NOWWORK_BUY_POOL) m_Button_Buy.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                       m_Button_Buy.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);

		// ����ִ� �뺴 ���
		for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
		{
			for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
			{
				TempCount = (TempCount2 * 6) + TempCount1;
				if(TempCount < m_BuyMaxSelectSolder)
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
						TempSelectCharacterIndex = TempCount;
					}

					// ���õ� �뺴�� ���� ��� ���̶���Ʈ ����ش�.
					SI32	siDrawFlag = 0;
					if( m_BuyNowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					switch(pMyOnlineWorld->pOnlineVillage->GetNation())
					{
						case ON_COUNTRY_KOREA:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if(CheckLevel(m_BuySolderList[TempCount].siCode, m_BuySolderList[TempCount].siLevel, m_BuySolderList[TempCount].siCredit) != 0)	
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_BuySolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;

						case ON_COUNTRY_JAPAN:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if(CheckLevel(m_BuySolderList[TempCount].siCode, m_BuySolderList[TempCount].siLevel, m_BuySolderList[TempCount].siCredit) != 0)	
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_BuySolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;

						case ON_COUNTRY_CHINA:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if(CheckLevel(m_BuySolderList[TempCount].siCode, m_BuySolderList[TempCount].siLevel, m_BuySolderList[TempCount].siCredit) != 0)	
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_BuySolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;

						case ON_COUNTRY_TAIWAN:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if(CheckLevel(m_BuySolderList[TempCount].siCode, m_BuySolderList[TempCount].siLevel, m_BuySolderList[TempCount].siCredit) != 0)	
									siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, m_BuySolderList[TempCount].SolderInfo.uiKind, siDrawFlag );
							break;
					}
				}
			}
		}

		// ������ ��ư ���
		if(m_NowWork == BARRACK_NOWWORK_BUY_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                      BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		

		if(m_bGeneralExMode == FALSE)
		{
			m_Button_Main_Buy.SetAction(TRUE);
			m_Button_Main_Buy.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
			m_Button_Main_Buy.SetAction(FALSE);
		}
		m_Button_Main_ChangeJob.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
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

				// ���� ��� ���
				TempSay = 0;
				if(TempSelectCharacterIndex != -1)
				{
					TempSay = GetFollowerGBarrackText(m_BuySolderList[TempSelectCharacterIndex].SolderInfo.uiKind);
				}

				if(TempSay == 0)
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_BUY));
				}
				else
				{
					char			TempBufferOwnerText1[256];
					char			TempBufferOwnerText2[256];

					// �뺴��뿡 �ʿ��� �ɷ�ġ�� ����
					ZeroMemory(TempBufferOwnerText1, 256);
					ZeroMemory(TempBufferOwnerText2, 256);
					sprintf((char*)TempBufferOwnerText1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_TRADEHELP)
						, m_BuySolderList[TempSelectCharacterIndex].siCredit);
					sprintf((char*)TempBufferOwnerText2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_BUYSELECT)
						, m_BuySolderList[TempSelectCharacterIndex].siMoney);

					// �� �뺴�� ����
					sprintf((char*)TempBuffer, "%s %s %s", pMyOnlineWorld->pOnlineText->Get(TempSay), TempBufferOwnerText1, TempBufferOwnerText2);
				}

				pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, (char*)TempBuffer);

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

				// ����ϱ� ��ư Text ���
				m_Button_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;
		}

		// �Ʒü� ���� ���
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207 ,m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), RGB(200, 200, 200));

		// ������ ��ư Text ���
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		m_Button_Main_ChangeJob.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC(hDC);
	}
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw Main Check
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Draw_BuyCheck(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hDC;
	BYTE	TempBuffer[256];
	BYTE	TempBuffer1[256];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siCheckX, m_siCheckY, m_Image_Line_Check_Button.Header.Xsize, m_Image_Line_Check_Button.Header.Ysize, m_Image_Line_Check_Button.Image);
		// ���� ���
		clGrp.PutSpriteJin(m_siCheckX, m_siCheckY, m_Image_CheckInterface.Header.Xsize, m_Image_CheckInterface.Header.Ysize, m_Image_CheckInterface.Image);

		// ����ҷ��� �뺴 �� ���
		//clGrp.PutSpriteT(337, 200, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[0]]);
		if(m_BuyNowSelectSolder != -1)
		{
//			RenderCharacter(pSurface, (UI16)m_BuySolderList[m_BuyNowSelectSolder].SolderInfo.uiKind, m_siCheckX + 25, m_siCheckY + 45, FALSE, 100, 0);
			// actdoll (2004/01/10 14:15) : �ʻ����
			RenderCharacter( pSurface, m_siCheckX + 25, m_siCheckY + 45, m_BuySolderList[m_BuyNowSelectSolder].SolderInfo.uiKind, DF_FLAG_PORTRAIT_HIGHLIGHT );
		}

		// ����ϱ� ��ư ���
		m_Button_BuyOK.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// ����ϱ� ��ư ���
		m_Button_BuyCancel.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);

		// ����ҷ��� �뺴 ���� ���
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		if(m_BuyNowSelectSolder != -1)
		{
			// Name
			MakeName( m_BuySolderList[m_BuyNowSelectSolder].SolderInfo.uiKind, m_BuySolderList[m_BuyNowSelectSolder].uiCharNameAddOption, (BYTE*)TempBuffer, 256 );
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 50, (char*)TempBuffer, RGB(200, 200, 200));

			// Money
			MakeMoneyText(m_BuySolderList[m_BuyNowSelectSolder].siMoney, (char*)TempBuffer1);
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INFO_MONEY), TempBuffer1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 70, (char*)TempBuffer, RGB(200, 200, 200));

			// LIFE
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INFO_HP), m_BuySolderList[m_BuyNowSelectSolder].SolderInfo.siLife);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 90, (char*)TempBuffer, RGB(200, 200, 200));

			// MANA
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INFO_MP), m_BuySolderList[m_BuyNowSelectSolder].SolderInfo.siMana);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 128, m_siCheckY + 110, (char*)TempBuffer, RGB(200, 200, 200));
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// ����ϱ� ��ư Text ���
		m_Button_BuyOK.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// ����ϱ� ��ư Text ���
		m_Button_BuyCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw ChangeJob
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Draw_ChangeJobSelect(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			StartPos;
	SI32			TempCount;
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;
	char			TempBuffer[256];
	SI32			TempSelectCharacterIndex = -1;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// ���콺 ��ǥ�� ���Ѵ�.

	// ��ġ ���
//	if(TickCount > 450) TickCount = 450;
	StartPos = 0;
	switch(m_NowWork)
	{
		case GBARRACK_NOWWORK_CHANGEJOB_ENDPOOL:
//			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
//			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			break;
	}

//	BExit.SetX(BExit.GetX());
//	m_Button_ChangeJob.SetX(m_Button_ChangeJob.GetX());
//	m_Button_Main_Buy.SetX(m_Button_Main_Buy.GetX());
//	m_Button_Main_ChangeJob.SetX(m_Button_Main_ChangeJob.GetX());
//	m_Button_Main_Dismissal.SetX(m_Button_Main_Dismissal.GetX());

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// ���� ���
		// ��ũ�� ���
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_Back1.Header.Xsize, m_Image_MainInterface_Back1.Header.Ysize, m_Image_MainInterface_Back1.Image);

		// actdoll (2004/01/06 17:08) : �ٸ� ������� NPC �ʻ�׸���
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GBARRACK, pMyOnlineWorld->pOnlineVillage->GetNation() );
		
		// �����ϱ� ��ư ���
		if(m_NowWork == GBARRACK_NOWWORK_CHANGEJOB_POOL) m_Button_ChangeJob.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                             m_Button_ChangeJob.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);

		// �����ִ� �뺴 ���
		for(TempCount1 = 0; TempCount1 < 6; TempCount1++)
		{
			for(TempCount2 = 0; TempCount2 < 2; TempCount2++)
			{
				TempCount = (TempCount2 * 6) + TempCount1;
				if(TempCount < m_ChangeJobMaxSelectSolder)
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
					if(m_ChangeJobNowSelectSolder == TempCount)
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
					}*/

					if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + 68)) && (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + 68)))
					{
						TempSelectCharacterIndex = TempCount;
					}

					// ���õ� �뺴�� ���� ��� ���̶���Ʈ ����ش�.
					SI32	siDrawFlag = 0;
					if( m_ChangeJobNowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					switch(pMyOnlineWorld->pOnlineVillage->GetNation())
					{
						case ON_COUNTRY_KOREA:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if( !CheckChangeJob(TempCount) )	siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, pMyOnlineWorld->pMyData->GetFollowerCharID(m_ChangeJobSolderList[TempCount].siSlot), siDrawFlag );
							break;

						case ON_COUNTRY_JAPAN:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if( !CheckChangeJob(TempCount) )	siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, pMyOnlineWorld->pMyData->GetFollowerCharID(m_ChangeJobSolderList[TempCount].siSlot), siDrawFlag );
							break;

						case ON_COUNTRY_CHINA:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if( !CheckChangeJob(TempCount) )	siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, pMyOnlineWorld->pMyData->GetFollowerCharID(m_ChangeJobSolderList[TempCount].siSlot), siDrawFlag );
							break;

						case ON_COUNTRY_TAIWAN:
							// actdoll (2004/01/10 14:15) : ������ Ʋ�� ���
							if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
							{
								siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							// ���� ���
							else
							{
								// ���� ������ �������� �ʾҴٸ� ���úҰ�
								if( !CheckChangeJob(TempCount) )	siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
							}
							RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, pMyOnlineWorld->pMyData->GetFollowerCharID(m_ChangeJobSolderList[TempCount].siSlot), siDrawFlag );
							break;
					}
				}
			}
		}

		// ������ ��ư ���
		if(m_NowWork == BARRACK_NOWWORK_BUY_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                      BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		

		m_Button_Main_ChangeJob.SetAction(TRUE);
		m_Button_Main_ChangeJob.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_ChangeJob.SetAction(FALSE);

		m_Button_Main_Buy.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Dismissal.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		switch(pMyOnlineWorld->pOnlineVillage->GetNation())
		{
			case ON_COUNTRY_JAPAN:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 10)
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_NOTCAN));
				}
				else
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_CHANGEJOB));
				}
				break;

			case ON_COUNTRY_CHINA:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 100)
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_NOTCAN));
				}
				else
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_CHANGEJOB));
				}
				break;

			case ON_COUNTRY_TAIWAN:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1000)
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_NOTCAN));
				}
				else
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_CHANGEJOB));
				}
				break;

			case ON_COUNTRY_KOREA:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) != 1)
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_NOTCAN));
				}
				else
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXTHELP_CHANGEJOB));
				}
				break;
		}
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 70, 402, (char*)TempBuffer);

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// ����ϱ� ��ư Text ���
		m_Button_ChangeJob.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// ����缺�� ���� ���
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207, m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), RGB(200, 200, 200));

		// ������ ��ư Text ���
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	
		m_Button_Main_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_ChangeJob.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw ChangeJob Check
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Draw_ChangeJobCheck(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC			hDC;
	SI32		TempMaxGeneralSelect;
	SI32		TempCount;
	SI32		TempPos;
	SI32		TempStartPos;
	SI32		TempSize;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siCheckX, m_siCheckY, m_Image_Line_Check_Button.Header.Xsize, m_Image_Line_Check_Button.Header.Ysize, m_Image_Line_Check_Button.Image);
		// ���� ���
		clGrp.PutSpriteJin(m_siCheckX, m_siCheckY, m_Image_CheckInterface.Header.Xsize, m_Image_CheckInterface.Header.Ysize, m_Image_CheckInterface.Image);

		// �����ҷ��� �뺴 �� ���
		if(m_ChangeJobNowSelectSolder != -1)
		{
			// ���� �ٲܼ� �ִ� ����� �˻�
			TempMaxGeneralSelect = 0;
			for(TempCount = 0; TempCount < 4; TempCount++)
			{
				if(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount] != 0)
				{
					TempMaxGeneralSelect++;
				}
			}

			// ��ġ �˻�
			TempSize     = 250 / TempMaxGeneralSelect;
			TempStartPos = 315 + (TempSize / 2) - (68 / 2);

			// ���콺 ����
			TempPos = TempStartPos;
			for(TempCount = 0; TempCount < TempMaxGeneralSelect; TempCount++)
			{
				if(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount] != 0)
				{
					/*
					if(m_ChangeJobSelectType == TempCount)
					{
//						clGrp.PutSpriteT(m_siCheckX + 25, m_siCheckY + 45, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[1]]);
					}
					else
					{
						if((ptMouse.x >= TempPos) && (ptMouse.x < (TempPos + 68)) && (ptMouse.y >= 205) && (ptMouse.y < (205 + 68)))
						{
//							clGrp.PutSpriteT(m_siCheckX + 25, m_siCheckY + 45, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[1]]);
						}
						else
						{
//							clGrp.PutSpriteT(m_siCheckX + 25, m_siCheckY + 45, m_Image_CharacterSlotInterface->Header.Xsize, m_Image_CharacterSlotInterface->Header.Ysize, &m_Image_CharacterSlotInterface->Image[m_Image_CharacterSlotInterface->Header.Start[0]]);
						}
					}*/

					// ���õ� �뺴�� ���� ��� ���̶���Ʈ ����ش�.
					SI32	siDrawFlag = 0;
					if( m_ChangeJobNowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					// actdoll (2004/01/10 14:15) : �ʻ����
					if(m_NowWork != GBARRACK_NOWWORK_CHANGEJOB_GETLIST)
					{
//						if(m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobListFlag[TempCount]) RenderCharacter(pSurface, (UI16)m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount], m_siCheckX + 25, m_siCheckY + 45, FALSE, 100, 100);
//						else                                                                           RenderCharacter(pSurface, (UI16)m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount], m_siCheckX + 25, m_siCheckY + 45, TRUE,  100, 100);
						if(!m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobListFlag[TempCount])	siDrawFlag |= DF_FLAG_PORTRAIT_ICON_DIEOVER;
						RenderCharacter( pSurface, m_siCheckX + 25, m_siCheckY + 45, m_ChangeJobSolderList[m_ChangeJobNowSelectSolder].siJobList[TempCount], siDrawFlag );
					}
					
					TempPos += TempSize;
				}
			}
		}

		// ����ϱ� ��ư ���
		m_Button_ChangeJobOK.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// ����ϱ� ��ư ���
		m_Button_ChangeJobCancel.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);

		// ����ҷ��� �뺴 ���� ���
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		// ����
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 25, m_siCheckY + 25, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_CHANGEGENERAL), RGB(200, 200, 200));

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// ����ϱ� ��ư Text ���
		m_Button_ChangeJobOK.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// ����ϱ� ��ư Text ���
		m_Button_ChangeJobCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw Dismissal
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Draw_DismissalSelect(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			StartPos;
	SI32			TempCount;
	SI32			TempCount1;
	SI32			TempCount2;
	SI32			TempXPos;
	SI32			TempYPos;
	BYTE			fFlag;				// 0 : Normal ���, 1 : ������ ��ũ��, 2 : �������� ��ũ��

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// ���콺 ��ǥ�� ���Ѵ�.

	// ��ġ ���
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case GBARRACK_NOWWORK_DISMISSAL_ENDPOOL:
			fFlag     = 0;
			break;
	}

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// ���� ���
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_Back1.Header.Xsize, m_Image_MainInterface_Back1.Header.Ysize, m_Image_MainInterface_Back1.Image);

		// actdoll (2004/01/06 17:08) : �ٸ� ������� NPC �ʻ�׸���
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GBARRACK, pMyOnlineWorld->pOnlineVillage->GetNation() );
		
		// �ذ��ϱ� ��ư ���
		if(m_NowWork == GBARRACK_NOWWORK_DISMISSAL_POOL) m_Button_Dismissal.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                             m_Button_Dismissal.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);

		// �� �뺴 ���
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
					
					// ���õ� �뺴�� ���� ��� ���̶���Ʈ ����ش�.
					SI32	siDrawFlag = 0;
					if( m_DismissalNowSelectSolder == TempCount )	siDrawFlag = DF_FLAG_PORTRAIT_HIGHLIGHT;

					// actdoll (2004/01/10 14:15) : �ʻ����
					if(m_NowWork != GBARRACK_NOWWORK_DISMISSAL_GETLIST)
					{					
						RenderCharacter( pSurface, TempXPos + 4, TempYPos + 5, pMyOnlineWorld->pMyData->GetFollowerCharID(m_DismissalSolderList[TempCount].siSlot), siDrawFlag );
					}
				}
			}
		}

		// ������ ��ư ���
		if(m_NowWork == GBARRACK_NOWWORK_DISMISSAL_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                             BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		

		m_Button_Main_Dismissal.SetAction(TRUE);
		m_Button_Main_Dismissal.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_Dismissal.SetAction(FALSE);
		m_Button_Main_Buy.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_ChangeJob.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		// ���� ��� ���
		m_Say             = 0;
		switch(pMyOnlineWorld->pOnlineVillage->GetNation())
		{
			case ON_COUNTRY_JAPAN:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 10)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_JAPAN));
				}
				else
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_DISMISSALDS));
				}
				break;

			case ON_COUNTRY_CHINA:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 100)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_TAIWAN));
				}
				else
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_DISMISSALDS));
				}
				break;

			case ON_COUNTRY_TAIWAN:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 1000)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_TAIWAN));
				}
				else
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_DISMISSALDS));
				}
				break;

			case ON_COUNTRY_KOREA:
				if(pMyOnlineWorld->pMyData->GetFollowerParameterNation(0) == 1)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_OWNERDISMISSALTEXT_KOREA));
				}
				else
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 70, 402, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_DISMISSALDS));
				}
				break;
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// ����ϱ� ��ư Text ���
		m_Button_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// �Ʒü� ���� ���
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207, m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK), RGB(200, 200, 200));

		// ������ ��ư Text ���
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_ChangeJob.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_Main_Dismissal.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw Dismissal Check
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::Draw_DismissalCheck(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hDC;
	BYTE	TempBuffer[256];
	BYTE	TempBuffer1[256];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{

		clGrp.PutSpriteT(m_siCheckX, m_siCheckY, m_Image_Line_Check_Button.Header.Xsize, m_Image_Line_Check_Button.Header.Ysize, m_Image_Line_Check_Button.Image);
		// ���� ���
		clGrp.PutSpriteJin(m_siCheckX, m_siCheckY, m_Image_CheckInterface.Header.Xsize, m_Image_CheckInterface.Header.Ysize, m_Image_CheckInterface.Image);

		// �ذ��ҷ��� �뺴 �� ���
		if(m_DismissalNowSelectSolder != -1)
		{
			
			// actdoll (2004/01/10 14:15) : �ʻ����
			RenderCharacter( pSurface, m_siCheckX + 25, m_siCheckY + 45, pMyOnlineWorld->pMyData->GetFollowerCharID(m_DismissalSolderList[m_DismissalNowSelectSolder].siSlot), DF_FLAG_PORTRAIT_HIGHLIGHT );
		}

		// �ذ��ϱ� ��ư ���
		m_Button_DismissalOK.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// ����ϱ� ��ư ���
		m_Button_DismissalCancel.Put(&m_Image_Check_Button, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);

		// ����ҷ��� �뺴 ���� ���
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

		// ����ϱ� ��ư Text ���
		m_Button_DismissalOK.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// ����ϱ� ��ư Text ���
		m_Button_DismissalCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Check Level(Return Value : 0 : ����, 1 : �ſ뵵 ����, 2 : ���� ����� ������ ����, 3 : ����� �ʰ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	OnlineVillageStructureGBarrack::CheckLevel(SI32 Kind, SI32 Lv, SI32 Credit)
{
	SI32	TempCount;
	SI32	TempGeneralCount;

	// �켱 ���� ����� �ִ��� �˻�
	TempGeneralCount = 0;
	for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(pMyOnlineWorld->pMyData->IsValidFollower(TempCount))
		{
			if(pMyOnlineWorld->pMyData->GetFollowerParameterKind(TempCount) == Kind)
			{
				return 2;
			}

			if(pMyOnlineWorld->pIOnlineCharKI->GetGeneral(pMyOnlineWorld->pMyData->GetFollowerParameterKind(TempCount)) == 1)
			{
				TempGeneralCount++;
			}
		}
	}

	// �ſ뵵�� �������� �˻�
	if(pMyOnlineWorld->pMyData->GetTradeGrade() < Credit) return 1;

	// ������� �ʰ��ϴ��� �˻�
//	if(TempGeneralCount >= pMyOnlineWorld->pMyData->siMaxGeneral) return 3;

	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Check Change Job
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageStructureGBarrack::CheckChangeJob(SI32 Index)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < 4; TempCount++)
	{
		if(m_ChangeJobSolderList[Index].siJobList[TempCount])
		{
			if(m_ChangeJobSolderList[Index].siJobListFlag[TempCount])
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Check Change Job(Return Value : 0 : ����, 1 : ��������, 2 : ���� ����� ������ ����, 3 : ����� �ʰ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	OnlineVillageStructureGBarrack::CheckChangeJobG(SI32 Index, SI32 Slot)
{
	if(m_ChangeJobSolderList[Index].siJobList[Slot])
	{
		if(m_ChangeJobSolderList[Index].siJobListFlag[Slot])
		{
			SI32	TempCount;
			SI32	TempGeneralCount;

			// �켱 ���� ����� �ִ��� �˻�
			TempGeneralCount = 0;
			for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
			{
				if(pMyOnlineWorld->pMyData->IsValidFollower(TempCount))
				{
					if(pMyOnlineWorld->pMyData->GetFollowerParameterKind(TempCount) == m_ChangeJobSolderList[Index].siJobList[Slot])
					{
						return 2;
					}

					if(pMyOnlineWorld->pIOnlineCharKI->GetGeneral(pMyOnlineWorld->pMyData->GetFollowerParameterKind(TempCount)) == 1)
					{
						TempGeneralCount++;
					}
				}
			}

			// ������� �ʰ��ϴ��� �˻�
//			if(TempGeneralCount >= pMyOnlineWorld->pMyData->siMaxGeneral) return 3;

			return 0;
		}
	}

	return 1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�� �뺴���� Text ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	OnlineVillageStructureGBarrack::GetFollowerGBarrackText(UI16 Kind)
{
	return pMyOnlineWorld->pIOnlineCharKI->GetDescText(pMyOnlineWorld->pIOnlineCharKI->GetIndex(Kind));
}