//////////////////////////////////////////////////////////////////////
//
// ������ ������ �� ȭ�� ���� ��ܿ� �������� �������̽�
// ��������, ���� ���, ���� ��� ���, ������ ��ư ��
//
// SiegeWarfareUnitUI.cpp: implementation of the CSiegeWarfareUnitUI class.
//
//////////////////////////////////////////////////////////////////////

#include <GSL.h>
#include	<Mouse.h>

#include	"OnlineWorld.h"

#include	<OnlineHelp.h>
#include	<OnlineMegaText.h>
#include	<OnlineTradeBook.h>
#include	<OnlineTip.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include	<OnlinePannel.h>
#include	<OnlineClient.h>
#include	<OnlineMyData.h>
#include	<OnlineVillage.h>
#include	<OnlineText.h>
#include	"OnlineMiniMap.h"
#include	<OnlineMsgBox.h>
#include	<OnlineMap.h>
#include	<OnlineListControl.h>
#include	<OnlineInventory.h>
#include	<OnlineFieldArea.h>
#include	"OnlineVillageManager.h"
#include	"OnlineSetting.h"
#include	<DebugTool.h>
#include	"OnlineQuestInterface.h"
#include	"OnlineMercenary.h"
#include	"OnlineMsgBox.h"
#include	"OnlineCharUI.h"
#include	<OnlineTrade.h>
#include	"OnlineQuestPlay.h"

#include	<kindinfo.h>

#include	"SiegeWarfareUnitUI.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern _InputDevice				IpD;

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 

// robypark 2005/1/7 18:41
// ��ġ�� ������ ���·� �׷��ش�.
// ���� ����, ���� ������ ��¿��� ���
extern void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeWarfareUnitUI::CSiegeWarfareUnitUI()
{
	m_pMyOnlineWorld		= NULL;

	Init();
}

CSiegeWarfareUnitUI::~CSiegeWarfareUnitUI()
{
	SiegeWarfareUnitUI_DestroyWindow();

	m_pMyOnlineWorld		= NULL;
}

// �ʱ�ȭ
void	CSiegeWarfareUnitUI::Init(cltOnlineWorld *pOnlineWorld)
{
	Init();

	m_pMyOnlineWorld		= pOnlineWorld;
}

// �ʱ�ȭ
void	CSiegeWarfareUnitUI::Init()
{
	m_dwShowWindow			= SW_HIDE;
	m_bCreatedWindow		= FALSE;
	m_bDrawManaGauge		= FALSE;

	m_pConfirmToGiveUpBattleMsgBox	= NULL;

	ZeroMemory(m_szArmyKindName, sizeof(m_szArmyKindName));

	m_siNoFieldAreaHandle	= -1;
	m_siNoFieldAreaHandle_BtnMinimize	= -1;
	m_siCurrentDelayTimeUsingVillageAttackType	= 0;
	m_siMaxDelayTimeUsingVillageAttackType		= 0;
}

// ���� ���̱� ����
void	CSiegeWarfareUnitUI::ShowWindow(DWORD dwShowWindow)
{
	// ���̱� ���� ��ȭ�� ���� ���
	if (m_dwShowWindow == dwShowWindow)
		return;

	// �ʱ�ȭ ���� ���� ���
	if (!m_bCreatedWindow)
		return;

	m_dwShowWindow = dwShowWindow;

	// Ȱ��ȭ
	if (SW_NORMAL == m_dwShowWindow)
	{
		UpdateButton();

		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		// No Field Area ����
		m_siNoFieldAreaHandle = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 115, 200);

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}

		// No Field Area ����
		m_siNoFieldAreaHandle_BtnMinimize = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(10, 115 + 96, 10 + m_sprBtnMinimize.Header.Xsize, 115 + 96 + 3 + m_sprBtnMinimize.Header.Ysize + 3);
	}
	// �ּ�ȭ
	else if (SW_MINIMIZE == m_dwShowWindow)
	{
		UpdateButton();

		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		// No Field Area ����
		m_siNoFieldAreaHandle = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 13 + m_sprBtnMinimize.Header.Xsize + 3, 3 + m_sprBtnMinimize.Header.Ysize + 3);

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}
	}
	else	// SW_HIDE
	{
		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}
	}
}

// ó��
BOOL	CSiegeWarfareUnitUI::Action(void)
{
	// ���� ������ ���
	if (SW_HIDE == m_dwShowWindow)
		return FALSE;

	// �ʱ�ȭ ���� ���� ���
	if (!m_bCreatedWindow)
		return FALSE;

	UpdateButton();

	// ���� ���� ���� �޽��� �ڽ� ó��
	if (m_pConfirmToGiveUpBattleMsgBox)
	{
		SI32 siResult = m_pConfirmToGiveUpBattleMsgBox->Process();

		if (ON_MSGBOX_RETURN_OK == siResult)
		{
			// ������ ���� ���� ���� ��û
			m_pMyOnlineWorld->pOnlineClient->SendRequestNormalCharacter(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID());

			// �޽��� �ڽ� ����
			delete m_pConfirmToGiveUpBattleMsgBox;
			m_pConfirmToGiveUpBattleMsgBox = NULL;

			// ����� �Է� Ȱ��ȭ
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}
		else if (ON_MSGBOX_RETURN_CANCEL == siResult)
		{
			// �޽��� �ڽ� ����
			delete m_pConfirmToGiveUpBattleMsgBox;
			m_pConfirmToGiveUpBattleMsgBox = NULL;

			// ����� �Է� Ȱ��ȭ
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}
	}

	// ���� �ּ�ȭ ���
	if (SW_MINIMIZE == m_dwShowWindow)
	{
		// ���� ��ư ó��
		if (m_btnRestore.Process(m_bMouseDown))
		{
			ShowWindow(SW_NORMAL);
		}

		if (m_btnRestore.IsInMouse())
		{
			// ǳ�� ���� ó���ϱ�
			char szTooltipText[1024];
			ZeroMemory(szTooltipText, sizeof(szTooltipText));

//			sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get());
			sprintf(szTooltipText, m_pMyOnlineWorld->pOnlineText->Get(8100132));

			m_pMyOnlineWorld->pOnlineHelp->SetText( szTooltipText, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_RIGHT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
		}

		if( IpD.LeftPressSwitch )
			m_bMouseDown	=	TRUE;
		else						
			m_bMouseDown	=	FALSE;

		return TRUE;
	}

	// �������̽� ��ư ó��
	// ������ ���� ���� ���� ��ư ó��
	SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
	if (m_btnShowUnitInformation.Process(m_bMouseDown))
	{
		// ���� ���� ���� ���� ���� ���̱� ���
		if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		{
			// ���� ���� ���� ���� ���� �Ⱥ��̱�� ����
			scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
			m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
		}
		else
		{
			// ���� ���� ���� ���� ���� ���̱�� ����
			scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
			m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
		}
	}
	/*
	// ���� ���  ����Ʈ ����
	else if (m_btnShowEmemyGuildList.Process(m_bMouseDown))
	{
	}
	*/
	// ���� ���� ���
	else if (m_btnDefenceUp.Process(m_bMouseDown))
	{
		// ���� ������ ����, ���� �����ð� ��� ���
		if ((m_uiVillageCode)
			&& (0 == m_siCurrentDelayTimeUsingVillageAttackType)
			&& (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()))
		{
			m_pMyOnlineWorld->pOnlineClient->SendVillageDefenceUp(m_uiVillageCode);
		}
	}
	// �����������̽�=>������ ��ư
	else if (m_btnEnterVillage.Process(m_bMouseDown))
	{
		// �����ڴٰ� ��û
		if (m_uiVillageCode)
			m_pMyOnlineWorld->pOnlineClient->SendLeaveVillageDefenceInterface(m_uiVillageCode);
	}
	// ���� ����â �ּ�ȭ ��ư ó��
	else if (m_btnMinimize.Process(m_bMouseDown))
	{
		ShowWindow(SW_MINIMIZE);
	}
	// ���� ���� ����
	else if (m_btnToNormalChar.Process(m_bMouseDown))
	{
		if ((FALSE == m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive())				// �ŷ� ��û ���� �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pOnlineTrade->IsActive())					// �ŷ� ���� �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pMyData->IsWaitBattleResponse())			// ���� ������� ���� �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pMyData->IsGotoVillage()) 				// ������ ���� ���̰ų� �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse())		// ���� ���� ���� ��� ���̶�� �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pIOnlineChar->IsMoving(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))	// �̵� ���̶�� �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pOnlineMsgBoxUsePortal->IsActive())		// ��Ż�� �̿��Ϸ��� ��찡 �ƴϰ�
				&& (FALSE == m_pMyOnlineWorld->pOnlineQuestPlay->IsAction())				// NPC���� ����Ʈ�� �޴� ���� �ƴϰ�
				&& (NULL == m_pConfirmToGiveUpBattleMsgBox))					// ���� ���� �޽��� �ڽ��� ������ ���� ���
		{
			m_pConfirmToGiveUpBattleMsgBox = new OnlineMsgBox;
			m_pConfirmToGiveUpBattleMsgBox->Init(m_pMyOnlineWorld);

			m_pConfirmToGiveUpBattleMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, m_pMyOnlineWorld->pOnlineText->Get(8100128), m_pMyOnlineWorld->pOnlineText->Get(8100130));

			// ����� �Է� ��Ȱ��ȭ
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);
		}
	}

	// ���� �������̽� ��ư ǳ�� ���� ó���ϱ�
	char szTooltipText[1024];
	ZeroMemory(szTooltipText, sizeof(szTooltipText));

	// ������ ���� ���� ���� ��ư ó��
	if (m_btnShowUnitInformation.IsInMouse())
	{
		sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOL_TIP_SHOW_SIEGEWARFAREUNIT_INFO));
	}
	/*
	// ���� ��� ����Ʈ ����
	else if (m_btnShowEmemyGuildList.IsInMouse())
	{
	}
	*/
	// ���� ���� ���
	else if (m_btnDefenceUp.IsInMouse())
	{
		char szCost[32];
		char szDefenceUpAmount[32];
		char szCoolTime[32];

		LPSOptionSiegeWarfare pSOptionSiegeWarfare = m_pMyOnlineWorld->GetSOptionSiegeWarfare();
		MakeMoneyText(pSOptionSiegeWarfare->siVillageCounterAttack_DefenceUp_Amount, szDefenceUpAmount);	// ���� ���Ǵ� ��
		MakeMoneyText(pSOptionSiegeWarfare->siVillageCounterAttack_DefenceUp_Cost, szCost);	// ���
		MakeMoneyText(pSOptionSiegeWarfare->siVillageCounterAttack_DefenceUp_CoolTime, szCoolTime);	// cool time

		sprintf(szTooltipText, 
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_VILLAGE_DEFENCE_UP),
						szDefenceUpAmount,
						szCost, 
						szCoolTime);
	}
	// �����������̽�=>���� ���� ��ư
	else if (m_btnEnterVillage.IsInMouse())
	{
		sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_VILLAGEDEFENCEINTERFACE_OUT));
	}
	// ���� ����â �ּ�ȭ ��ư
	else if (m_btnMinimize.IsInMouse())
	{
//		sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get());
		sprintf(szTooltipText, m_pMyOnlineWorld->pOnlineText->Get(8100131));
	}
	else if (m_btnToNormalChar.IsInMouse())
	{
		sprintf(szTooltipText, m_pMyOnlineWorld->pOnlineText->Get(8100129));
	}

	// ǳ�� ���� ǥ���� �� ������, 
	if ('\0' != szTooltipText[0])
		m_pMyOnlineWorld->pOnlineHelp->SetText( szTooltipText, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_RIGHT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );

	if( IpD.LeftPressSwitch )
		m_bMouseDown	=	TRUE;
	else						
		m_bMouseDown	=	FALSE;

	return TRUE;
}

// �׸���
void	CSiegeWarfareUnitUI::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	// ������ ������ �ʴ� ���
	if (SW_HIDE == m_dwShowWindow)
		return;

	// �ʱ�ȭ ���� ���� ���
	if (!m_bCreatedWindow)
		return;

	// ���� �̹����� �׸���
	DrawImage(pSurface);

	// ���� �ؽ�Ʈ�� �׸���
	DrawText(pSurface);

	// �޽��� �ڽ��� ���� ��� �׸���
	if (m_pConfirmToGiveUpBattleMsgBox)
	{
		m_pConfirmToGiveUpBattleMsgBox->Draw(pSurface);
	}
}

// �׸���
void	CSiegeWarfareUnitUI::DrawImage(LPDIRECTDRAWSURFACE7 pSurface)
{
	// �̹����� ���
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		// Ȱ��ȭ�� ������ ���
		if (SW_NORMAL == m_dwShowWindow)
		{
			// ���� �̹��� ���
			clGrp.PutSpriteT(0, 0, m_sprStatusMain.Header.Xsize, m_sprStatusMain.Header.Ysize, m_sprStatusMain.Image);

			clGrp.PutSpriteT(0, 96, m_sprMainInterface.Header.Xsize, m_sprMainInterface.Header.Ysize, m_sprMainInterface.Image);

			// ������ ���� �ʻ�ȭ �׸���
			clGrp.PutSpriteT(8, 8, m_sprPortrait.Header.Xsize, m_sprPortrait.Header.Ysize, m_sprPortrait.Image);

			// ��ư �̹��� ���
			// �ּ�ȭ ��ư
			m_btnMinimize.Put(&m_sprBtnMinimize, 0, 2, 1, BUTTON_PUT_NOMOVE);

			UI16 uiMyCharUniqueID = m_pMyOnlineWorld->pMyData->GetMyCharUniqueID();

			// ������ ���
			// ü��
			SI32 siRate = ((float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiMyCharUniqueID)
							/ (float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_MaximumHP(uiMyCharUniqueID)) * 100;

			DrawGaugeVillage(99, 10, siRate, &m_sprGaugeRed);

			// ����
			if (m_bDrawManaGauge)
			{
				siRate = ((float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID)
							/ (float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(uiMyCharUniqueID)) * 100;

				DrawGaugeVillage(99, 34, siRate, &m_sprGaugeBlue);
			}

			SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();

			// ������ ���� ���� ����
			// ���콺 Ŀ���� ��ư���� �ִٸ�,
			if (m_btnShowUnitInformation.IsInMouse())
			{
				m_btnShowUnitInformation.PutSpriteButton(
														m_btnShowUnitInformation.GetX(), 
														m_btnShowUnitInformation.GetY(), 
														&m_sprShowUnitInformation, 
														1, FALSE, FALSE);
			}
			// ���� ������ ���� ���̱� ���
			else if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
			{
				m_btnShowUnitInformation.PutSpriteButton(
														m_btnShowUnitInformation.GetX(), 
														m_btnShowUnitInformation.GetY(), 
														&m_sprShowUnitInformation, 
														2, FALSE, FALSE);
			}
			else if (0 == (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT))
			{
				m_btnShowUnitInformation.PutSpriteButton(
														m_btnShowUnitInformation.GetX(), 
														m_btnShowUnitInformation.GetY(), 
														&m_sprShowUnitInformation, 
														0, FALSE, FALSE);
			}
			////////////////////////////////////////

			/*
			// ���� ��� ����Ʈ ����
			m_btnShowEmemyGuildList.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			*/
			// ���� ���� ���
			m_btnDefenceUp.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			// ������
			m_btnEnterVillage.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			// ���� ���� ����
			m_btnToNormalChar.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

			// ���� ���� �ݰ� ��� ����� ���Ͽ� ���� �ð� ǥ��
			if ((m_siMaxDelayTimeUsingVillageAttackType) && (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()))
			{
				// ����� �ð� ���� ���
				float fRemainTimePct = (float)m_siCurrentDelayTimeUsingVillageAttackType / (float)m_siMaxDelayTimeUsingVillageAttackType;

				if (0 == m_siCurrentDelayTimeUsingVillageAttackType)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[7]]);
				else if (fRemainTimePct > 0.858f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[6]]);
				else if (fRemainTimePct > 0.715f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[5]]);
				else if (fRemainTimePct > 0.572f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[4]]);
				else if (fRemainTimePct > 0.429f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[3]]);
				else if (fRemainTimePct > 0.286f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[2]]);
				else if (fRemainTimePct > 0.143f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[1]]);
				else
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[0]]);
			}
	//		else
	//			clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[7]]);
		}
		else	// minimize
		{
			clGrp.PutSprite(0, 0,
								m_sprMainInterface.Header.Xsize,
								m_sprMainInterface.Header.Ysize,
								0,
								110,
								m_sprMainInterface.Header.Xsize,
								m_sprMainInterface.Header.Ysize - 110,
								m_sprMainInterface.Image);

			// ���� ��ư �̹��� �׸���
			m_btnRestore.Put(&m_sprBtnMinimize, 0, 2, 1, BUTTON_PUT_NOMOVE);
		}

		clGrp.UnlockSurface( pSurface );
	}
}

// �׸���
void	CSiegeWarfareUnitUI::DrawText(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC			hDC;

	// �ؽ�Ʈ ���
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// �Ϲ� Ȱ��ȭ�� ����
		if (SW_NORMAL == m_dwShowWindow)
		{
			char szTemp1[64], szTemp2[64], szTemp3[128];

			UI16 uiMyCharUniqueID = m_pMyOnlineWorld->pMyData->GetMyCharUniqueID();

			// ������ ���� ���� ���
			MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiMyCharUniqueID), szTemp1);
			MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_MaximumHP(uiMyCharUniqueID), szTemp2);
			sprintf(szTemp3, "%s / %s", szTemp1, szTemp2);

			// ü��
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99 + 1,	10 + 1,	122, 15, szTemp3, RGB(0, 0, 0));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99,		10,		122, 15, szTemp3, RGB(250, 250, 250));

			// ����
			if (m_bDrawManaGauge)
			{
				MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID), szTemp1);
				MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(uiMyCharUniqueID), szTemp2);
				sprintf(szTemp3, "%s / %s", szTemp1, szTemp2);

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99 + 1,	34 + 1, 122, 15, szTemp3, RGB(0, 0, 0));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99,		34,		122, 15, szTemp3, RGB(250, 250, 250));
			}

			// ������ ���� �̸�
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 102, 76, 86, 15, m_szArmyKindName, RGB(250, 250, 250));

			// ������ ���� ���� ����
			m_btnShowUnitInformation.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			/*
			// ���� ��� ����Ʈ ����
			m_btnShowEmemyGuildList.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			*/
			// ���� ���� ���
			m_btnDefenceUp.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			// ���� ����
			m_btnEnterVillage.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			// ���� ���� ����
			m_btnToNormalChar.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		else	// minimize
		{
			/*
			// ���� ��ư
			m_btnRestore.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			*/
		}

		pSurface->ReleaseDC( hDC );
	}
}

// ��ư ����
void	CSiegeWarfareUnitUI::UpdateButton(void)
{
	if (SW_NORMAL != m_dwShowWindow)
		return;

	// ���� ���� ���
	m_btnDefenceUp.SetAction(FALSE);

	// ���� �������̽� �� ĳ���Ͷ��
	if (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
	{
		// �����������̽����� ������
		m_btnEnterVillage.ShowWindow(TRUE);

		// ���� ���� ����
		m_btnToNormalChar.ShowWindow(FALSE);
	}
	else
	{
		// �����������̽����� ������
		m_btnEnterVillage.ShowWindow(FALSE);

		// ���� ���� ����
		m_btnToNormalChar.ShowWindow(TRUE);
	}

	/*
	// ���� ��� ����Ʈ ����
	m_btnShowEmemyGuildList.SetAction(FALSE);
	*/

	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetMyCharKind());

	// ������ ���� �� ������ ���� ���� ������ ������ �������� �˻�
	if ((lpsSiegeUnitTable)
		&& (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE))
	{
		// ���� �������̽� �� ĳ���Ͷ��
		if (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
		{
			if (0 == m_siCurrentDelayTimeUsingVillageAttackType)
			{
				// ���� ���� ���
				m_btnDefenceUp.SetAction(TRUE);
			}
		}
	}
}

// ���ҽ� �б�
void	CSiegeWarfareUnitUI::LoadResource(void)
{
	// ��� ���� �����ΰ�?
	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetMyCharKind());

	// ������ ������ �ִ� ĳ�����ΰ�?
	if (KI[TransKindOnlineToImjin(lpsSiegeUnitTable->m_uiKind, FALSE)].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
	{
		m_bDrawManaGauge = TRUE;
		clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE.Spr",	m_sprStatusMain);			// ���� ���� ���� ���� ���� ��� �̹���
	}
	else
	{
		m_bDrawManaGauge = FALSE;
		clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE_X.Spr",	m_sprStatusMain);			// ���� ���� ���� ���� ���� ��� �̹���
	}
	
	// ���� �̸� ���
	ZeroMemory(m_szArmyKindName, sizeof(m_szArmyKindName));
	strcpy(m_szArmyKindName, m_pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siUnitNameTextIdx));

	// �̹��� �ε� /////////////////////////////////////////
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_B.Spr",			m_sprShowUnitInformation);	// ��� ������ ���� ���� ����
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_A.Spr",			m_sprFunctionBtnImage);		// ��� ��ư �̹���
//	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_C.Spr",			m_sprEnterVillage);			// �����������̽�=>���� ���� ��ư �̹���
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_DEFENSE_BASE.Spr",	m_sprMainInterface);		// �������̽� ���� �̹���
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_TIME_GAGE.Spr",	m_sprTimer);				// ���� ��� ��� ���� �ð� ǥ�� �̹���

	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE_GAUGE_0.Spr",	m_sprGaugeRed);			// ������ red
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE_GAUGE_1.Spr",	m_sprGaugeBlue);		// ������ blue
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_X.Spr",				m_sprBtnMinimize);		// �ּ�ȭ ��ư �̹���
	clGrp.LoadXspr(lpsSiegeUnitTable->m_szPortraitFileName,					m_sprPortrait);			// �ʻ�ȭ �̹���

	// ��ư ���� /////////////////////////////////////////
	// �ּ�ȭ ��ư
	m_btnMinimize.Create(13, 115 + 96 + 3, 
					m_sprBtnMinimize.Header.Xsize,	m_sprBtnMinimize.Header.Ysize,
					"Minimize", BUTTON_PUT_LEFT, TRUE );
	// ���� ��ư
	m_btnRestore.Create(13, 5 + 3, 
					m_sprBtnMinimize.Header.Xsize,	m_sprBtnMinimize.Header.Ysize,
					"Restore", BUTTON_PUT_LEFT, TRUE );

	// ��� ������ ���� ����
	m_btnShowUnitInformation.Create(12, 19 + 96, 
					m_sprShowUnitInformation.Header.Xsize,	m_sprShowUnitInformation.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SHOW_SIEGEWARFAREUNIT_INFORMATION), BUTTON_PUT_LEFT, TRUE );
	/*
	// ���� ��� ����Ʈ ���� ����
	m_btnShowEmemyGuildList.Create(12, 49, 
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_COUNTERATTACK_STONE), BUTTON_PUT_LEFT, TRUE );
	*/

	// ���� ���� ���
	m_btnDefenceUp.Create(12, 49 + 96,
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_DEFUP), BUTTON_PUT_LEFT, TRUE );

	// �����������̽�=>���� ����
	m_btnEnterVillage.Create(12, 79 + 96,
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,	
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXITTEXT), BUTTON_PUT_LEFT, TRUE );

	// ��������=>�Ϲ� ���� ���� ��ư
	m_btnToNormalChar.Create(12, 79 + 96,
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,	
					m_pMyOnlineWorld->pOnlineText->Get(8100128), BUTTON_PUT_LEFT, TRUE );

	// �������̽� ��� �߿� �Ϻ� ����� ����� �� ����.
	m_pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_MINIMAP/*mini map*/ | ONLINEPANNEL_DISABLEFLAG_TIP/*Quest*/ | ONLINEPANNEL_DISABLEFLAG_OPTION/*system*/ | ONLINEPANNEL_DISABLEFLAG_IMOTICON/*imoticon*/ | ONLINEPANNEL_DISABLEFLAG_CHARINFO/*mercenary*/);

	// ���� �ִ� â�� �ݱ� //////////////////////////////////////
	// �뺴â
	if (m_pMyOnlineWorld->m_pMercenary->IsAction())
		m_pMyOnlineWorld->m_pMercenary->OnButtonPush();

	// �κ��丮�� ���� ������ �ݾ��ش�.
	if(m_pMyOnlineWorld->pOnlineInventory->IsActive())
		m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
	
	// ��ΰ� ���� ������ �ݾ��ش�.
	if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

	// �̴ϸʵ� �ݴ´�!!
	if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive())
		m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);

	// �ý��� �޴�(�ɼ�) �ݱ�
	if(m_pMyOnlineWorld->pOnlineSetting->IsAction())
		m_pMyOnlineWorld->pOnlineSetting->SetAction(FALSE);
	///////////////////////////////////////////////////////////

	m_bMouseDown = FALSE;
}

// ���ҽ� ����
void	CSiegeWarfareUnitUI::FreeResource(void)
{
	// �̹��� ���� /////////////////////////////////////////
	clGrp.FreeXspr(m_sprShowUnitInformation);		// ��� ���� ���� �������� ��ư �̹���
	clGrp.FreeXspr(m_sprEnterVillage);				// ���� ���� ��ư �̹���
	clGrp.FreeXspr(m_sprFunctionBtnImage);			// ���� ��� ��ư �̹���
	clGrp.FreeXspr(m_sprMainInterface);				// �����������̽� ���� �̹���
	clGrp.FreeXspr(m_sprTimer);						// �����������̽� ���� ���� ��� ��� ���� �ð� ǥ�� �̹���

	clGrp.FreeXspr(m_sprStatusMain);				// ���� ���� ���� ���� ���� ��� �̹���
	clGrp.FreeXspr(m_sprGaugeRed);					// ������ red
	clGrp.FreeXspr(m_sprGaugeBlue);					// ������ blue
	clGrp.FreeXspr(m_sprBtnMinimize);				// �ּ�ȭ ��ư �̹���
	clGrp.FreeXspr(m_sprPortrait);					// �ʻ�ȭ �̹���
}

// ���� ���̴°�?
BOOL	CSiegeWarfareUnitUI::IsVisibleWindow(void)
{
	if (SW_NORMAL == m_dwShowWindow)
		return TRUE;

	return FALSE;
}

// ������ �ּ�ȭ �Ǿ��°�?
BOOL	CSiegeWarfareUnitUI::IsMinimizeWindow(void)
{
	if (SW_MINIMIZE == m_dwShowWindow)
		return TRUE;

	return FALSE;
}

// �������̽� ��� ����� �� �־����� ������ ����
void	CSiegeWarfareUnitUI::SetCurrentDelayTimeUsingVillageAttackType(SI32 siDelay)
{
	m_siCurrentDelayTimeUsingVillageAttackType = siDelay;
}

void	CSiegeWarfareUnitUI::SetMaxDelayTimeUsingVillageAttackType(SI32 siDelay)
{
	m_siMaxDelayTimeUsingVillageAttackType = siDelay;
}

// ���� �ڵ�
void	CSiegeWarfareUnitUI::SetVillageCode(UI16 uiVillageCode)
{
	m_uiVillageCode = uiVillageCode;
}

// ���� ����
BOOL	CSiegeWarfareUnitUI::SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow)
{
	// �̹� ������ ���
	if (m_bCreatedWindow)
	{
		// ���� ������ ���� �Ŀ� �ٽ� �����ϵ��� �Ѵ�.
		SiegeWarfareUnitUI_DestroyWindow();

		Init();
	}

	// ���ҽ� �ε�
	LoadResource();

	// ���� �Ϸ�
	m_bCreatedWindow = TRUE;

	return TRUE;
}

// ���� �ı�
BOOL	CSiegeWarfareUnitUI::SiegeWarfareUnitUI_DestroyWindow(void)
{
	// �������� ���� ���
	if (FALSE == m_bCreatedWindow)
		return FALSE;

	if (m_pConfirmToGiveUpBattleMsgBox)
	{
		delete m_pConfirmToGiveUpBattleMsgBox;
		m_pConfirmToGiveUpBattleMsgBox = NULL;
	}

	// ���ҽ� ����
	FreeResource();

	// No Field Area ����
	if (m_pMyOnlineWorld->pOnlineFieldArea)
	{
		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}
	}

	m_dwShowWindow = SW_HIDE;

	// ���� �Ϸ�
	m_bCreatedWindow = FALSE;

	return TRUE;
}
