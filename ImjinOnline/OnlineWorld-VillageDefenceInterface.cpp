//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// ������ �����ϱ� �������̽� ���� OnlineWorld-VillageDefenceInterface.cpp (ĳ���Ͱ� �ʵ忡 �ִ� ��ó�� ó���Ѵ�. ��, �ʵ��Ȳ�� �� ���� �ִ�.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <GSL.h>
#include	<Mouse.h>
#include	<clGame.h>
#include "MainFunction.h"

#include "..\Gersang\maindata.h"
#include "..\Gersang\music.h"
#include <SoundManager.h>
#include "..\Gersang\directx.h"

#include	"OnlineVersion.h"
#include	"OnlineWorld.h"
#include	"OnlineImjin2I.h"
#include	"HqFileManager.h"
#include	"OnlineETC.h"


#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include	<OnlineHelp.h>
#include	<OnlineConfig.h>
#include	<OnlineMegaText.h>
#include	<OnlineNumberControl.h>
#include	<OnlineTradeBook.h>
#include	<OnlineVillageSave.h>
#include	<OnlineProductStructure.h>
#include	<OnlineTrade.h>
#include	<OnlineBooth.h>
#include	<OnlineTip.h>
//#include	<OnlineBasicTip.h>			// actdoll (2004/02/16 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
//#include	<OnlineBasicQuest.h>		// actdoll (2004/02/16 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
#include	<OnlineParty.h>
#include	<OnlineFollower.h>
#include	<IProfit.h>
#include	<OnlineDungeonParser.h>
#include	<OnlineTicket-Parser.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include	<OnlinePannel.h>
#include	<OnlineNoticeList.h>
#include	<OnlineKeyboard.h>
#include	<OnlineClient.h>
#include	<OnlineMyData.h>
#include	<OnlineVillage.h>
#include	<OnlineText.h>
#include	<OnlineResource.h>
#include	<OnlineMercenary-Parser.h>
#include	<OnlineFieldChat.h>
#include	<OnlineMiniMap.h>
#include	<OnlineMsgBox.h>
#include	<OnlineMap.h>
#include	<OnlineListControl.h>
#include	<OnlineInventory.h>
#include	<OnlineFieldArea.h>
#include	"MakePalette.h"
#include	"OnlineImoticon.h"
#include	"OnlineVillageManager.h"
#include	"OnlineCharActionCode.h"
#include	"OnlineCharUI.h"
#include	"OnlineVillageBaseStructure.h"
#include	"OnlineVillage-Parser.h"
#include	"clNumberRender.h"
#include	"OnlineTradeParser.h"
#include	"OnlineSkillParser.h"
#include	"OnlineGeneral-Parser.h"
#include	"OnlineMessenger.h"
#include	"OnlineQuest-Parser.h"
#include	"OnlineQuestPlay.h"
#include	"clItemInfoWindow.h"
#include	"slanderfilter.h"
#include	"OnlineSetting.h"
#include	"BuriedItemParser.h"
#include    "OnlineFieldAuction.h"
#include    "OnlineCharStatus.h"
//#include    "OnlineBanner.h"			// actdoll (2004/02/25 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
#include	"OnlineSmith.h"
#include	"OnlineSmithParser.h"
#include	"OnlineLevelMark-Parser.h"
#include	"OnlineMsgProc.h"
#include    "ddraw.h"
#include    "OnlineMFGS.h"
#include	"..\\gersang\\charinfo\\CharFileLoadManager.h"
#include	"MemMgr.h"
#include	<myfile.h>
#include	"bindJxFile.h"
#include	<DebugTool.h>
#include	"OnlineNPCTalk.h"
#include	"OnlineChatMacro.h"
#include	"OnlineQuestInterface.h"
#include	"OnlineMercenary.h"
#include	"OnlinePortrait.h"		// actdoll (031229)
#include	"stringcpy.h"
#include    "..\\SCCommon\\MacroCheckProcess.h"
#include	"OnlineMapSound.h"

#include	"..\EnumData\Enum_AttackAtb.h"			// AttackAtb
#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME ��Ʈ�ѷ� ����

#include "../SCCommon/OnlineCommonParser.h"	// Using Online Common Parser

#include	"FieldHeroInfoMgr.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

#include	"SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"

#include	<time.h>	// mktime()

//�ؼ����� �߰��κ� 
/*
#include	"JSWnd.h"
#include	"JSWndMgr.h"
#include	"JSTestWnd.h"
*/
//#include	"JSWnd.h"
//#include	"JSWndMgr.h"
//#include	"JSTestWnd.h"
#include	"tlhelp32.h"
#include	"GGuard.h"

// sagolboss �߰��κ� ->
#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif
// <- �߰� �� 

extern _clGame* pGame;
extern	_MainData				GD;									// ������ �ֵ����� 
extern _InputDevice				IpD;
extern	SoundManager			*g_pSoundManager;
extern	CCharFileLoadManager	g_FileLoadManager;					// ������ ĳ����
extern CItem					*g_pItem;
extern char						LogInID[128];

extern clock_t					CurrentClock, MonitorMacroClock1, MonitorMacroClock2;

extern int WindowModeSwitch;
extern int bActive;

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 

// robypark 2005/1/7 18:41
// ��ġ(10%������)�� ������ ���·� �׷��ش�.
// ������ ���� ü��, ���� ���������� ���
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

// ����׸� ���� ��������.
// HQDebug  GHQDebugTool;

// actdoll (2004/08/27 0:34) : �Է±� ���� ���� ����� â(_DEBUG�ø� ����. ���ٷ��� �ּ��ް�)
//#define _SHOW_INPUTSTATUS_

// �����ϱ� �������̽� ���� ��
UI16	cltOnlineWorld::VillageDefenceInterfacePoll(UI16 uiClientRet)
{
	UI16			uiNextMenu	=	0;		
//	CHAR			szMyCharSpeechBuffer[1024];
//	CHAR			TempBuffer[1024];
//	CHAR			TempBuffer1[1024];
	DWORD			dwPrevFrameStartTime, dwPrevFrameTimeRequire;	
	SI32			siX, siY;					
//	SI16			siTX, siTY;					
	SI32			siSetMouse = ON_MOUSE_BASIC;
	POINT			ptMouse;
	BOOL			fRender;
	SI16			siHeroX, siHeroY;
	static SI32		CheckObjectZone = 0;

	if((GetTickCount() % 50) == 0)
		pIOnlineMouse->Action();

	if(dwCurrentFrameStartTime == 0) dwCurrentFrameStartTime = timeGetTime();

	dwPrevFrameStartTime			=	dwCurrentFrameStartTime;	

	// ���� �������� ������ �� �ִ� ��Ȳ�� �ɶ����� ����Ѵ�.
	dwPrevFrameStartTime			-=	dwCurrentFrameAddTime;
	dwCurrentFrameAddTime			=	0;
	dwPrevFrameTimeRequire			=	timeGetTime() - dwPrevFrameStartTime;

	// �ʴ� �����Ӽ��� �����.
	if(dwPrevFrameTimeRequire < CPF)
	{
		Sleep(1);
		return uiNextMenu;
	}
	else
	{
		dwCurrentFrameAddTime	=	dwPrevFrameTimeRequire - CPF;
	}

	// ���� ��ǻ�Ͱ� ������ Render�� ���� �ʴ´�.
	fRender = TRUE;
	if(dwCurrentFrameAddTime > 1000)
	{
		dwCurrentFrameAddTime = 0;
	}
	if(dwCurrentFrameAddTime > CPF)
	{
		fRender = FALSE;
		dwCurrentFrameSkippingCount++;
	}
	if(fRender == TRUE)
	{
		dwCurrentFrameSkippingCount = 0;
	}
	if(dwCurrentFrameSkippingCount > 3)
	{
		fRender = TRUE;
		dwCurrentFrameSkippingCount = 0;
	}

	m_siMsgBoxStatus = pOnlineMsgBox->Process();

	/////////////////////////////////////////////////////
	if(m_WaitTickCount)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", pOnlineText->Get(ON_TEXT_WAIT));

		if((GetTickCount() - m_WaitTickCount) >= 3000)
		{
			// ������ ���ӽ� �����ߴ��� �����ϱ�
			pOnlineSetting->GameInit();

			// �α��� ȭ������
			SetMainPollStatus(ON_MAINPOLL_LOGINMGR);
			SetLogOut(0, ON_MAINPOLL_LOGINMGR);
			m_WaitTickCount = 0;
			return uiNextMenu;
		}
	}
	////////////////////////////////////////////////////
	// �������� ���� ��Ų��.
	uiTotalFrame++;


	siFPS	=	1000 / (timeGetTime() - dwPrevFrameStartTime);

	// �����ϱ⿡�� ���� �ݰ� ��� ��� �����ð� ����
	m_sVillageDefenceInterface.SetPastDelayTimeUsingVillageAttackType(dwPrevFrameTimeRequire);

	if (pOnlinePannel->GetSiegeWarfareUnitUIHandle())
	{
		pOnlinePannel->GetSiegeWarfareUnitUIHandle()->
			SetCurrentDelayTimeUsingVillageAttackType(
				m_sVillageDefenceInterface.m_siCurrentDelayTimeUsingVillageAttackType);
	}

	// ���� �ð��� ���´�.
	dwCurrentFrameStartTime		=	timeGetTime();		

	// ������ FrameSkipping ������ Up Check�� �����ϰ� ������ ��찡 �ֱ⶧���� ���⼭�� �ٽ� �˻�

	if(fLeftMemoryUpSwitch)
	{
		fLeftUpSwitch       = TRUE;
		fLeftMemoryUpSwitch = FALSE;
	}
	if(fRightMemoryUpSwitch)
	{
		fRightUpSwitch       = TRUE;
		fRightMemoryUpSwitch = FALSE;
	}

	// One Click Check
	if(IpD.LeftPressSwitch)
	{
		fLeftOneSwitch++;
	}
	else
	{
		fLeftOneSwitch = FALSE;
	}

	if(IpD.RightPressSwitch)
	{
		fRightOneSwitch++;
	}
	else
	{
		fRightOneSwitch = FALSE;
	}

	DWriteStringToFile("���� 2");

	// ���� ���	
	pIOnlineMouse->Action();
	pOnlineMousePoint->Action();

	// Ű������ �Է»��¸� ���´�.
	pOnlineKeyboard->Action();

	//�ؼ����� �߰��κ� 
	// ��� ���� ������
//	pJSWndMgr->Action();
	
	ptMouse.x	=	IpD.Mouse_X;
	ptMouse.y	=	IpD.Mouse_Y;

	pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

	// ���� ���� ó��
	if(pOnlineMap->m_MapInfo.uiVillageCode != pOnlineVillageInfoData->m_uiVillageCode)
	{
		pOnlineVillageInfoData->m_siFrame = 0;
		if(pOnlineMap->m_MapInfo.uiVillageCode != pOnlineVillageInfoData->m_siVillageSeriesWait)
		{
			pOnlineClient->SendRequestVillageInfo(pOnlineMap->m_MapInfo.uiVillageCode);
			pOnlineVillageInfoData->m_siVillageSeriesWait = pOnlineMap->m_MapInfo.uiVillageCode;
		}
	}
	else
	{
		pOnlineVillageInfoData->m_siFrame++;
		if(pOnlineVillageInfoData->m_siFrame > 16)
		{
			pOnlineVillageInfoData->m_siFrame = 16;
		}
	}

	if(pOnlineVillage == NULL)
	{
		// ���� ������ üũ�Ͽ� �� ������ ��������� �����Ѵ�.
		if(pOnlineMap->m_MapIndex == 0)
		{
			SetBackGroundMusic(CheckHeroZone());
		}
	}

	// �̸�Ƽ���� ó���Ѵ�.
	pOnlineImoticon->Pool(fLeftUpSwitch);

	//if(pOnlineVillage == NULL)
	{
		// ���콺 �ִ� ��ġ�� �� ĳ���Ͱ� ������ ���콺�� �����Ѵ�.
		// ��, �ʵ� �����̿��� �Ѵ�.	
		pOnlineHelp->Initialize();

		pOnlineMsgBoxPay->Process();
		if(pOnlineMsgBoxError->Process() == ON_MSGBOX_RETURN_OK)
		{
			pOnlineClientChatting->Close();
			return ON_MAINPOLL_FORCED_LOGOUT;
		}
	}
	
	DWriteStringToFile("���� 3");

	// robypark 2005/1/26 18:33
	// ���콺 Ŀ�� �Ʒ� ������Ʈ ó��(ǳ�� ���򸻵��� ����)
	ProcessFieldMouseMove(TRUE, ptMouse);

	// �⺻ ���콺�� �����Ѵ�.
//	pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);
/*
	// robypark 2004/12/21 16:39
	// ������ �߿� �Ϲ� ĳ���� ����(�� ������ �ʺ��� ������) ��Ű��
	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if(fRightOneSwitch == 1)	// ���콺 ������ ��ư Ŭ��
			{
				if( pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					switch(pMousePointInfo->GetPointKind())
					{
						case ON_MOUSE_POINT_CHAR:
						{
							UI16 uiUniqueID_UnderPoint = (UI16)pMousePointInfo->GetParam();

							if ( ( TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint) )	// ������ �����̰ų�,
								|| ( FALSE == pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind(uiUniqueID_UnderPoint) ) )	// �÷��̾ �ƴ� �����̰ų�,
								|| ( TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiUniqueID_UnderPoint) ) 	// ���� �������̽��� �ִ� ĳ���Ͱ� �̰ų�,
								|| ( TRUE == pIOnlineChar->GetBattleFlag(uiUniqueID_UnderPoint) ) )		// ���� ������� ĳ���Ͱ� �̰ų�,
							{
								// ���� ��ų �� ����.
								break;
							}

							// ������ �ð��̶��
							if (IsInTimeSiegeWarfare())
							{
								// ���� ��ܿ��� �ƴ϶��
								if (FALSE == IsMemberOfSameGuild(uiUniqueID_UnderPoint))
								{
									// ���� ��û
									// �� ��û ������ ������ ��� ��ɾ��� /w(����)�� ���� ����Ѵ�.(ON_RESPONSE_WARP)
									pOnlineClient->SendRequestSiegeWarfareWarp(uiUniqueID_UnderPoint);
									break;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
*/
	// ����Ű�� ó���Ѵ�.
	FieldShortCutKey();	

//	if(pOnlineVillage == NULL)
	{
//		m_pNpcTalk->Action();

		m_pQuestInterface->Action();

		m_pChatMacro->Action();

		pOnlineParty->Action();

		// �ǳ�
		pOnlinePannel->Poll();

		// �κ��丮.
		pOnlineInventory->SetFlagVillage(FALSE);
		pOnlineInventory->Action();

		// �ŷ�â
		pOnlineTrade->Action();

		// ���
		pOnlineTradeBook->Poll();


		// ����
		pOnlineBooth->ActionBench();

		// ��
		pOnlineTip->Action();

		// Quest Play
		pOnlineQuestPlay->Poll();

//		//�ʺ��� ����.
//		pOnlineBasicTip->Action();

//		pOnlineBasicQuest->Action();		

		// ��� 
		pOnlineFieldAuction->Process();

		// ����
		pOnlineReception->Process();

		// ��������
		pOnlineSmith->Poll();

//		pJSTestWnd->Action();

		// ������ ȹ�� â ���� �ൿ�� �Ѵ�. 
		pGame->pItemInfoWindow->Prepare();

		// �޽���
		pOnlineMessenger->Action();

		m_pMercenary->Action();

		if( bFindTrade )
			FindTradeBook();

		// �̴ϸ�
		pOnlineMiniMap->Pool(FALSE);

		// �� ĳ������ ����� ó���Ѵ�.(�̵�, ����, ���� ��, ������ �ݱ�)
		ProcessMyCharOrder();
	}
		
	// ���� �޽����� ������ �����ش�.
	RPI.Send();

	// Resource ����
//	if( timeGetTime() - m_ImageFreeTimer > 5000 )
	{
		pOnlineResource->Prepare();
//		FreeOldImageByTime();

//		m_ImageFreeTimer = timeGetTime();
	}

	// ������� End
	// ������Ʈ �׼�.	
	pIOnlineObject->Action();	

	// ĳ���� �׼�.		
	pIOnlineChar->Action();		

	// ����ٴϴ� �뺴�� ó��
	pIOnlineChar->ActionByVMercenary();

	pOnlineMap->Action();

	// robypark 2004/10/25 13:28
	// ������ ���� ���� ȿ�� ó��
	m_SiegeWarfare_BulletMgr.Action();
	m_SiegeWarfare_SmokeMgr.Action();

	//����Ʈ �������� ����Ѵ�.
//	if(pOnlineTradeBook->IsAction())
//		pOnlineBasicQuest->SetAction(FALSE);
//	else 
//		pOnlineBasicQuest->SetAction(TRUE);
		


	CheckObjectZone++;
	if(CheckObjectZone > 15)
	{
		SI16		TempX;
		SI16		TempY;

		TempX = siHeroX;
		TempY = siHeroY;
		pIOnlineChar->GetPos(pOnlineMap->GetCurrentFocusCharID(), &TempX, &TempY);
		pIOnlineObject->CheckZone((TempX - (ON_MAP_VIEW_XSIZE_HALF * 2)), (TempY - (ON_MAP_VIEW_YSIZE_HALF * 2)), (TempX + (ON_MAP_VIEW_XSIZE_HALF * 2)), (TempY + (ON_MAP_VIEW_YSIZE_HALF * 2)));
		CheckObjectZone = 0;
	}
	// ȭ�鿡 �׸���.	

	pOnlineFieldEffect->Pool();

	
	DWriteStringToFile("�׸��� 1");
	if(fRender)
	{
		VillageDefenceInterfaceDraw();	
	}
	DWriteStringToFile("�׸��� 2");
	

	if( bExitOnlineGame )
	{
		siMainPollStatus = ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}

	DWriteStringToFile("��");

	return uiNextMenu;
}

// �����ϱ� �������� �׸���
VOID	cltOnlineWorld::VillageDefenceInterfaceDraw(VOID)
{
	HDC		hdc;
	HFONT	hOldFont;
	SI32	siX, siY;
	CHAR	szBuffer[1024];
	CHAR	szBuffer2[1024];
	CHAR	szChatBuffer[1024];	
	SI16	siHeroX, siHeroY;
	DWORD	TempTickCount;
	BOOL	fFieldRender;
	static DWORD	YetTempTickCount = 0;

	TempTickCount = GetTickCount();

	fFieldRender = TRUE;
	if(pOnlineVillage)
	{
		if(pOnlineVillage->CheckFieldRender() == FALSE)
		{
			fFieldRender = FALSE;
		}
	}

	if(fFieldRender)
	{
		// Ÿ��, ĳ����, ������Ʈ�� �׸���.	
		pOnlineMap->Draw(GETSCREENSURFACE(this));	
		
		// robypark 2004/10/25 13:27
		// ������ ���� ȿ�� �׸���
		m_SiegeWarfare_BulletMgr.Draw();
		m_SiegeWarfare_SmokeMgr.Draw();

		pOnlineFieldEffect->Render(GETSCREENSURFACE(this));

		pOnlineMousePoint->Draw();

		pIOnlineChar->AllDrawSpeech(GETSCREENSURFACE(this));

		// NPC ���ϱ�
		m_pNpcTalk->Action();
		m_pNpcTalk->Draw(GETSCREENSURFACE(this));

		// Macro
		m_pChatMacro->Draw(GETSCREENSURFACE(this));
		
		// robypark 2004/11/13 16:37
		// ������ ���� ���� â���� ��� ǥ���ϴ� ��� ó��
		if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		{
			// HDC�� �̿��Ͽ� ���� �� ĳ���Ϳ� ���� ������ ����ش�.
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				SetBkMode(hdc, TRANSPARENT);

				hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
				SetTextColor(hdc, RGB(0, 0, 0));
		
				// ��� ������ ������ ĳ���͸� �׸���
				pIOnlineChar->AllDrawName(hdc);

				SelectObject(hdc, hOldFont);
				GETSCREENSURFACE(this)->ReleaseDC(hdc);

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// ��� ������ ���� ü�� �� ���� ������ �׸���
					pIOnlineChar->AllDrawSiegeWarfareUnit_StatusBar();

					// ��� ���� ���� ��� ��� �׸���
					pIOnlineChar->AllDrawSiegeWarfareUnit_GuildFlag();

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}	// if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)

		// ���� ���콺�� ĳ���͸� ����Ű�� ������
		if(m_SelectCharacterIndex != -1)
		{
			// ������ ������ ��� ü�� ������ ����ϱ�
			if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
			{
				// HDC�� �̿��Ͽ� ���� �� ĳ���Ϳ� ���� ������ ����ش�.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// ������ ���� ĳ���͸� �׸���
					pIOnlineChar->DrawName(m_SelectCharacterIndex, hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);
				}

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// ��� ������ ���� ü�� �� ���� ������ �׸���
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(m_SelectCharacterIndex);

					// ��� ���� ���� ��� ��� �׸���
					pIOnlineChar->DrawGuildFlag(m_SelectCharacterIndex);

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}
		}

		// HDC�� �̿��Ͽ� ���� �� ĳ���Ϳ� ���� ������ ����ش�.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);

			hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
			SetTextColor(hdc, RGB(0, 0, 0));
			
			pIOnlineChar->DrawName(pMyData->GetMyCharUniqueID(), hdc);		

			SelectObject(hdc, hOldFont);
			GETSCREENSURFACE(this)->ReleaseDC(hdc);

			// robypark 2004/11/12 21:28
			// ������ ���� ü�� �� ���� ������ �׸���
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// ������ ���� ü�� �� ���� ������ �׸���
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(pMyData->GetMyCharUniqueID());

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

		}	
		
		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// ��� ����� ��´�.
			pIOnlineChar->DrawGuildFlag(pMyData->GetMyCharUniqueID());

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	
	}

	// �ʵ忡�� �ǹ� �۾��� �ϰ� ������
	if(pOnlineMap->m_MapInfo.uiVillageCode)
	{
		if(pOnlineMap->m_MapInfo.uiVillageCode == pOnlineVillageInfoData->m_uiVillageCode)
		{
			float		TempFloat;
			SI16		SignStartPos;
			XSPR		*lpBoard;

			SignStartPos = 0;
			if(pOnlineVillageInfoData->m_siFrame < 16)
			{
				TempFloat    = (float)sin(((float)pOnlineVillageInfoData->m_siFrame / 5.625f) * 0.017444f);
				SignStartPos = -222 + (int)(TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * 222.0f);
			}

			lpBoard = pOnlineResource->GetxsprGame(ON_GAMESPR_DOWNTOWN_BOARDER);
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// ���� ����
				clGrp.PutSpriteT(-1 + SignStartPos, 0, lpBoard->Header.Xsize, lpBoard->Header.Ysize, lpBoard->Image);

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

			// ���� �̸��� ������ش�.
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				::SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAT));
				::SetBkMode(hdc, TRANSPARENT);
				::SetTextColor(hdc, RGB(240, 240, 240));

				strcpy(szBuffer, pOnlineText->Get(pOnlineVillageInfoData->m_lpVillageHeader->siVillageName));
				pOnlineMegaText->DrawTextCenter(hdc, 23 + SignStartPos, 12, 120, 17, szBuffer);		

				MakeMoneyText(pOnlineVillageInfoData->m_VillageInfo.uiPopulation, (char*)szBuffer);
				sprintf((char*)szBuffer2, pOnlineText->Get(ON_TEXT_VILINFO_PEOPLE), szBuffer);

				pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 63, szBuffer2, RGB(10, 10, 10));		
				pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 62, szBuffer2, RGB(210, 210, 210));		

				if(pOnlineVillageInfoData->m_VillageInfo.m_MaxInvestmentID[0] == NULL) sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_MAXINVEST_NONE));
				else                                                                   sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_MAXINVEST_NAME), pOnlineVillageInfoData->m_VillageInfo.m_MaxInvestmentID);
				pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 83, szBuffer, RGB(10, 10, 10));
				pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 82, szBuffer, RGB(210, 210, 210));

				if(pOnlineVillageInfoData->m_VillageInfo.szGuildID[0] == NULL) sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_GUILD_NONE));
				else                                                           sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_GUILD_NAME), pOnlineVillageInfoData->m_VillageInfo.szGuildID);
				pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 103, szBuffer, RGB(10, 10, 10));
				pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 102, szBuffer, RGB(210, 210, 210));

				GETSCREENSURFACE(this)->ReleaseDC(hdc);
			}
		}
	}

/*	if(pOnlineVillage)
	{
		if(pOnlineVillage->PollField() == FALSE)
		{
			pOnlineVillage->FreeMapStruct();
			delete pOnlineVillage;
			pOnlineVillage = NULL;
		}
	}
	else
*/
	{
		// �Ϲ� �����϶�
		// �̴ϸ� â�� Ȱ��ȭ �Ǿ� �ִٸ� ���~~
		if(pOnlineMiniMap->IsActive())
			pOnlineMiniMap->Draw(GETSCREENSURFACE(this));

		pOnlineParty->Draw(GETSCREENSURFACE(this));

		pOnlineTradeBook->Draw( GETSCREENSURFACE(this) );

		// �������â 
		pOnlineReception->Draw(GETSCREENSURFACE(this));

		// ��������
		pOnlineSmith->Draw(GETSCREENSURFACE(this));

		// �ǳ��� �׸���.
		pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_MINIMAP/*mini map*/ | ONLINEPANNEL_DISABLEFLAG_TIP/*Quest*/ | ONLINEPANNEL_DISABLEFLAG_OPTION/*system*/ | ONLINEPANNEL_DISABLEFLAG_IMOTICON/*imoticon*/ | ONLINEPANNEL_DISABLEFLAG_CHARINFO/*mercenary*/);
		pOnlinePannel->Draw(GETSCREENSURFACE(this));

		// �ŷ�â�� Ȱ��ȭ �Ǿ� �ֵ���
		if( pOnlineTrade->IsActive() )
		{	
			// �� �κ��丮���� �������� �巡�� ���̶�� �ŷ�â�� ���� ��� �ְ� �κ��丮�� ��� �ش�.
			if(pOnlineInventory->GetDragItemInfo() != -1 )
			{
				pOnlineTrade->Draw(GETSCREENSURFACE(this));
				m_pMercenary->Draw(GETSCREENSURFACE(this));
				if(pOnlineInventory->IsActive())	pOnlineInventory->Draw(GETSCREENSURFACE(this));
			}

			else if( pOnlineTrade->GetList()->GetDragSlot() != -1 )
			{
				m_pMercenary->Draw(GETSCREENSURFACE(this));
				if(pOnlineInventory->IsActive())	pOnlineInventory->Draw(GETSCREENSURFACE(this));
				pOnlineTrade->Draw(GETSCREENSURFACE(this));
			}
			
			else
			{
				pOnlineTrade->Draw(GETSCREENSURFACE(this));
				m_pMercenary->Draw(GETSCREENSURFACE(this));
				if(pOnlineInventory->IsActive())	pOnlineInventory->Draw(GETSCREENSURFACE(this));				
			}
		}
		else
		{
			// �뺴 ���� ���			
			m_pMercenary->Draw(GETSCREENSURFACE(this));

			// ����
			if(pOnlineBooth->IsActionIn())
			{
				pOnlineBooth->Draw(GETSCREENSURFACE(this));	
				if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));
				pOnlineBooth->pOnlineMsgBoxBench->Draw(GETSCREENSURFACE(this));
			}
			else if(pOnlineBooth->IsActionOut())
			{
				if(pOnlineBooth->GetList()->GetDragSlot() != -1)
				{
					if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));
					pOnlineBooth->Draw(GETSCREENSURFACE(this));
				}
				else
				{
					pOnlineBooth->Draw(GETSCREENSURFACE(this));
					if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));
				}
				pOnlineBooth->pOnlineMsgBoxBench->Draw(GETSCREENSURFACE(this));
			}
			else
				if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));

			pOnlineTrade->Draw(GETSCREENSURFACE(this));
		}

		// �̸�Ƽ���� ����Ѵ�.
		pOnlineImoticon->Draw(GETSCREENSURFACE(this));	

		// ȭ������ �ű��.
		// ������ ȭ�鿡 ����ش�.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SI32 siScreenDotX, siScreenDotY;

			pOnlineMap->TransformMapXYToScreenDotCenterXY(2, 2, siScreenDotX, siScreenDotY);
			pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		

			if((m_GMFlag != ON_USERGRADE_NORMAL) && (m_InfoFlag == FALSE))
			{
				if(pIOnlineChar->GetShow(pMyData->GetMyCharUniqueID()))
				{
					pIOnlineChar->GetPos(pOnlineMap->GetCurrentFocusCharID(), &siHeroX, &siHeroY);
					sprintf((char*)szChatBuffer, "Player Pos(Tile) : %d, %d | Mouse Position(Pixel) : %d, %d", siHeroX, siHeroY, IpD.Mouse_X, IpD.Mouse_Y );
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 11, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 10, szChatBuffer, PaletteWhiteColor);			

					sprintf((char*)szChatBuffer, "Now Time : %dY %dM %dD %dH %dM", m_RealTime.uiYear, m_RealTime.uiMonth, m_RealTime.uiDay, m_RealTime.uiHour, m_RealTime.uiMinute);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 31, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 30, szChatBuffer, PaletteWhiteColor);
					
					
					sprintf((char*)szChatBuffer, "Now FPS : %d, Render Count : %d, Left Message Count : %d, Recv Data = %d K", YetTempTickCount == 0 ? 1000 : 1000 / YetTempTickCount, YetTempTickCount, pOnlineClient->m_pMsgBuffer.size(), pOnlineClient->m_RecvDataSize / 1024);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 51, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 50, szChatBuffer, PaletteWhiteColor);			

					// ���� Tile Buffer ����
					SI32		TempTempCount;
					SI32		TempTempBaseCount;

					TempTempBaseCount = 0;
					for(TempTempCount = 0; TempTempCount < MAX_ONLINE_TILE_BASEDATAFILE; TempTempCount++)
					{
						if(pOnlineResource->ImageBaseTileData[TempTempCount].m_TileIndex != -1) TempTempBaseCount++;
					}
					sprintf((char*)szChatBuffer, "Now Tile Buffer P, Base : %d/%d", TempTempBaseCount, MAX_ONLINE_TILE_BASEDATAFILE);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 71, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 70, szChatBuffer, PaletteWhiteColor);			


					TempTempBaseCount = 0;
					for(TempTempCount = 0; TempTempCount < MAX_ONLINE_OBJECT_BASEDATAFILE; TempTempCount++)
					{
						if(pOnlineResource->ImageBaseObjectData[TempTempCount].m_TileIndex != -1) TempTempBaseCount++;
					}
					sprintf((char*)szChatBuffer, "Now Object Buffer P, Base : %d/%d", TempTempBaseCount, MAX_ONLINE_OBJECT_BASEDATAFILE);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 91, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 90, szChatBuffer, PaletteWhiteColor);			

					// ���� ������ ��ü ���
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					switch(pMousePointInfo->GetPointKind())
					{
					case ON_MOUSE_POINT_CHAR:
						// ������ ĳ���� ǥ��
						if(pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam())))
						{
							// Player����

							char*		lpGuild = pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam());

							if(lpGuild)
							{
								if(lpGuild[0] == NULL)
								{
									lpGuild = NULL;
								}
							}

							if(lpGuild != NULL)
							{
								XSPR*		lpXSPR;

								// ������ ���

								lpXSPR = pOnlineResource->GetxsprFlagM(pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								if(lpXSPR)
								{
									if(lpXSPR->Image == NULL)
									{
										lpXSPR = NULL;
									}
								}

								if(lpXSPR)
								{
									// ��� ����� �ִ°��

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d, Flag : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								}
								else
								{
									// ��� ����� ���°��

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()));
								}
							}
							else
							{
								// ������ �ƴҰ��

								sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\"", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()));
							}
						}
						else
						{
							// Monster����

							sprintf((char*)szChatBuffer, "Now Select : Monster, UNID : %d", (UI32)pMousePointInfo->GetParam());
						}
						pOnlineMegaText->DrawText( hdc, 11, 111, szChatBuffer, PaletteDataBlack);			
						pOnlineMegaText->DrawText( hdc, 10, 110, szChatBuffer, PaletteWhiteColor);			
						break;				
					case ON_MOUSE_POINT_OBJECT:
						sprintf((char*)szChatBuffer, "Now Select : Object, ID : %d", pMousePointInfo->GetParam());
						break;
					case ON_MOUSE_POINT_ITEM:
						UI32		uiItemID;

						uiItemID		=	pMousePointInfo->GetParam();				

						sprintf((char*)szChatBuffer, "Now Select : Item, ID : %d", uiItemID);
						break;
					case ON_MOUSE_POINT_MILEPOST:
						sprintf((char*)szChatBuffer, "Now Select : MilePost");
						break;
					case ON_MOUSE_POINT_INTERFACE:
						sprintf((char*)szChatBuffer, "Now Select : Interface");
						break;

					case ON_MOUSE_POINT_NPC:
						sprintf((char*)szChatBuffer, "Now Select : NPC");
						break;			

					default:
						sprintf((char*)szChatBuffer, "Now Select : Field, x : %d, y : %d", siX, siY);
						break;
					}
					pOnlineMegaText->DrawText( hdc, 11, 111, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 110, szChatBuffer, PaletteWhiteColor);			

					MapStoreHeader*			lpMapStoreHeader;
					SI16					MPointFlag;

					// ���ϴ� ������ Data�� ���´�.
					MPointFlag = 0;
					lpMapStoreHeader = pOnlineMap->GetMapStoreParser(siX, siY, &MPointFlag);
					if(lpMapStoreHeader)
					{
						sprintf((char*)szChatBuffer, "Booth : Field, x : %d, y : %d, Pos : %d", siX, siY, MPointFlag);
						pOnlineMegaText->DrawText( hdc, 11, 131, szChatBuffer, PaletteDataBlack);			
						pOnlineMegaText->DrawText( hdc, 10, 130, szChatBuffer, PaletteWhiteColor);			
					}

					// PK Flag
					sprintf((char*)szChatBuffer, "PK Flag : %s", CheckPKFieldMap() ? "TRUE" : "FALSE");
					pOnlineMegaText->DrawText( hdc, 11, 151, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 150, szChatBuffer, PaletteWhiteColor);

					// Tile Draw Time
					sprintf((char*)szChatBuffer, "Tile Draw Time : %I64d", dwMapTileDrawTime);
					pOnlineMegaText->DrawText( hdc, 11, 171, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 170, szChatBuffer, PaletteWhiteColor);

					char tempbuf[1024];
					sprintf( tempbuf, "Work(%3d) WorkComplete(%3d) Free(%3d) FreeComplete(%3d)",
					pCharFileLoadManager->pOnlineLoadXSprProc->GetWorkQueueLength(),
					pCharFileLoadManager->pOnlineLoadXSprProc->GetCompleteQueueLength(),
					pCharFileLoadManager->pOnlineLoadXSprProc->GetFreeListLength(),
					pCharFileLoadManager->pOnlineLoadXSprProc->GetFreeQueueLength() );

					pOnlineMegaText->DrawText( hdc, 11, 191, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 190, tempbuf, PaletteWhiteColor );
				}
			}

			GETSCREENSURFACE(this)->ReleaseDC(hdc);
		}	

		ShowMessage(100, 35, GETSCREENSURFACE(this));

		// ���â 
		pOnlineFieldAuction->Draw(GETSCREENSURFACE(this));

		POINT pt;
		pt.x	= IpD.Mouse_X;
		pt.y	= IpD.Mouse_Y;


		// ���� ���
		SI16 siDrawedHelpPosX = 0;
		SI16 siDrawedHelpPosY = 0;
		int helpx = pOnlineHelp->Show(GETSCREENSURFACE(this), &siDrawedHelpPosX, &siDrawedHelpPosY);

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			XSPR*		lpXSPR;

			// ���� ���콺�� ĳ���͸� ����Ű�� ������
			if(m_SelectCharacterIndex != -1)
			{
				lpXSPR = pOnlineResource->GetxsprFlagM(pIOnlineChar->GetGuildFlag(m_SelectCharacterIndex));
				if(lpXSPR)
				{
					if(lpXSPR->Image)
					{
						clGrp.PutSpriteT(m_SelectCharacterNameDrawX + 5 + helpx, m_SelectCharacterNameDrawY + 7, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[(GetTickCount() / 120) % lpXSPR->Header.font_number]]);
					}
				}

				/*
				// robypark 2005/1/8 12:26
				// ������ ������ ��� ü��, ����ǰ ������ ����ϱ�
				if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
				{
					// ������ ���� ü�� �������� ���
					XSPR	*psprGaugeHealth = NULL;

					// ����ڿ� ���� ����ΰ�?
					if (IsMemberOfSameGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
					}
					// ���� ��ܿ��ΰ�?
					else if (pIOnlineChar->GetIsHostilityGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
					}
					else	// �� ���� ��� �ȱ׸���.
						psprGaugeHealth = NULL;

					if (psprGaugeHealth)
					{
						// �׸���
						DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
												siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
												m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct + 1,
												psprGaugeHealth);
					}

					// ������ ���� ���ް������� ����ؾ� �ϴ� �����
					if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods)
					{
						// ������ ���� ����ǰ ������ ���
						XSPR	*psprGaugeSupplyGoodsAmount = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

						if (psprGaugeSupplyGoodsAmount)
						{
							// �׸���
							DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
													siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY + 3 + 1,
													m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeSupplyGoodsPct + 1,
													psprGaugeSupplyGoodsAmount);
						}
					}
				}
				*/
			}

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	

		// �޼��� �ڽ� ���
		pOnlineMsgBox->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxUsePortal->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxError->Draw(GETSCREENSURFACE(this));

		// TIP ���
		pOnlineTip->Draw(GETSCREENSURFACE(this));

		// �޽��� ���
		pOnlineMessenger->Draw(GETSCREENSURFACE(this));

		// ������ ��ο�
		pOnlineNumPad->pNumberControl->Draw( GETSCREENSURFACE(this) );

		// ȯ�漳�� ��ο�
		pOnlineSetting->Draw(GETSCREENSURFACE(this));

		//�ؼ����� �߰��κ� 
		// ��� �������� ��ο�
//		pJSWndMgr->Draw(GETSCREENSURFACE(this));

		pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(this));

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// ���콺�� ��´�.
			pIOnlineMouse->Draw();			

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}
		
		// for debug		

		UpdateScreen(GETSCREENSURFACE(this));
	}

	YetTempTickCount = GetTickCount() - TempTickCount;
}

// �����ϱ� �������̽� �ʱ�ȭ
VOID	cltOnlineWorld::InitVillageDefenceInterface(VOID)
{
	// ���� �������̽� ��� �߿� �Ϻ� ����� ����� �� ����.
	pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_MINIMAP/*mini map*/ | ONLINEPANNEL_DISABLEFLAG_TIP/*Quest*/ | ONLINEPANNEL_DISABLEFLAG_OPTION/*system*/ | ONLINEPANNEL_DISABLEFLAG_IMOTICON/*imoticon*/ | ONLINEPANNEL_DISABLEFLAG_CHARINFO/*mercenary*/);

	// ���� �ִ� â�� �ݱ� //////////////////////////////////////
	// �뺴â
	if (m_pMercenary->IsAction())
		m_pMercenary->OnButtonPush();

	// �κ��丮�� ���� ������ �ݾ��ش�.
	if(pOnlineInventory->IsActive())
		pOnlineInventory->SetActive(FALSE);
	
	// ��ΰ� ���� ������ �ݾ��ش�.
	if(pOnlineTradeBook->IsAction())
		pOnlineTradeBook->SetAction(FALSE);

	// �̴ϸʵ� �ݴ´�!!
	if(pOnlineMiniMap->IsActive())
		pOnlineMiniMap->SetActive(FALSE);

	// �ý��� �޴�(�ɼ�) �ݱ�
	if(pOnlineSetting->IsAction())
		pOnlineSetting->SetAction(FALSE);
	///////////////////////////////////////////////////////////
}

// �����ϱ� �������̽� ����
VOID	cltOnlineWorld::FreeVillageDefenceInterface(VOID)
{
	// �������̽� �κ� No Field Area ����
	pMyData->SetInVillageDefenceInterface(FALSE);

	m_sVillageDefenceInterface.m_uiVillageCode = 0;

	CSiegeWarfareUnitUI *pSiegeWarfareUnitUI = pOnlinePannel->GetSiegeWarfareUnitUIHandle();

	if (pSiegeWarfareUnitUI)
	{
		pSiegeWarfareUnitUI->SetCurrentDelayTimeUsingVillageAttackType(0);

		pSiegeWarfareUnitUI->SetMaxDelayTimeUsingVillageAttackType(0);

		pSiegeWarfareUnitUI->SetVillageCode(m_sVillageDefenceInterface.m_uiVillageCode);
	}
}

// �����ϱ� �������̽��� ���� ��û�� ���� ���� ó��
// ON_RESPONSE_WAR_VILLAGE_DEFENCE
UI16	cltOnlineWorld::OnResponseEnterVillageDefenceInterface(BYTE *pMsg)
{
	OnResWarVillageDefence	*pOnResWarVillageDefence = (OnResWarVillageDefence *)pMsg;

	// �����ϱ� �������̽��� �� �� �ִ�.
	if (ON_RET_OK == pOnResWarVillageDefence->dwResponse)
	{
		// ���� �������̽��� �� ����ڰ� �ڽ��̶��
		if (pMyData->GetMyCharUniqueID() == pOnResWarVillageDefence->uiUniqueID)
		{
			// �������̽� ���ҽ� �ʱ�ȭ �� �ε�
			InitVillageDefenceInterface();

			// ���� �����ϱ� �������̽��� �ִٰ� ����
			pMyData->SetInVillageDefenceInterface(TRUE);

			// �����ϱ� Poll ����
			SetGamePollStatus( ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE );

			m_sVillageDefenceInterface.SetDelayTimeUsingVillageAttackType(pOnResWarVillageDefence->siLimitTime);

			CSiegeWarfareUnitUI *pSiegeWarfareUnitUI = pOnlinePannel->GetSiegeWarfareUnitUIHandle();

			if (pSiegeWarfareUnitUI)
			{
				pSiegeWarfareUnitUI->SetCurrentDelayTimeUsingVillageAttackType(
						m_sVillageDefenceInterface.m_siCurrentDelayTimeUsingVillageAttackType);

				pSiegeWarfareUnitUI->SetMaxDelayTimeUsingVillageAttackType(
						m_sVillageDefenceInterface.m_siMaxDelayTimeUsingVillageAttackType);

				pSiegeWarfareUnitUI->SetVillageCode(m_sVillageDefenceInterface.m_uiVillageCode);
			}

			pOnlineMsgBox->Initialize();

			// ���� ������ ����.	Village Code�� ����									
			pMyData->SetGotoVillage(FALSE, m_sVillageDefenceInterface.m_uiVillageCode);

			/*
			// ���� ������ ���´�.
			VillageHeader*	lpVillageHeader = pOnlineVillageManager->GetVillageParser()->
												GetVillageHeaderCode(m_sVillageDefenceInterface.m_uiVillageCode);

			if (lpVillageHeader)
			{
				// ������ ��ġ�� �������� ���� �����Ѵ�.
				pOnlineMap->SetXY(lpVillageHeader->siX, lpVillageHeader->siY);
				pOnlineMap->SetCurrentFocusCharID(pMyData->GetMyCharUniqueID());
			}
			*/
		}
		else	// �ٸ� ����ڶ�� 
		{
		}

		// �����������̽��� ���ٰ� ĳ���� ����
		pIOnlineChar->SetInVillageDefenceInterface(pOnResWarVillageDefence->uiUniqueID, TRUE);
	}
	// �����ϱ� �������̽��� �̹� �ٸ� �����(��ܿ�)�� �� �ִ�.
	else if (ON_RET_GUILD_VILLAGE_OFF_LIMITS == pOnResWarVillageDefence->dwResponse)
	{
		// �����������̽��� ��ġ�� �ʰ� �ٷ� ������ ����.
		// ���� ������ ����.										
		pMyData->SetGotoVillage(FALSE, m_sVillageDefenceInterface.m_uiVillageCode);
		// �������� ������ ���ٰ� ��û�Ѵ�.		
		pOnlineClient->SendGotoVillage(m_sVillageDefenceInterface.m_uiVillageCode);

		// �����ϱ� �������̽��� �� �����ٴ� �޽��� ���
		SetMessage((char*)pOnlineText->Get(ON_TEXT_ALREADY_USED_VILLAGE_DEFENCE_INTERFACE));
		pOnlinePannel->AddChat((char*)pOnlineText->Get(ON_TEXT_ALREADY_USED_VILLAGE_DEFENCE_INTERFACE), ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);

		m_sVillageDefenceInterface.m_uiVillageCode = 0;
	}
	// �� ���� ������
	else
	{
		// �̹� ����ڰ� �����������̽��� �ִ� ��� return; 
		// ��Ȥ ��û�޽����� �������ʺ����� ��� ������ �ȴ�.
		if (pMyData->IsInVillageDefenceInterface())
			return 0;

		// �� �̻� ������ ���� ������ ���ٴ� �޽����� ��ٸ��� �ʵ��� �����Ѵ�.
		pMyData->SetWaitGotoVillageResponse(FALSE);		
			
		// ������ ���ٴ� ������ �ʱ�ȭ �Ѵ�.
		pMyData->SetGotoVillage(FALSE, 0);
		
		// Field Poll ����
		SetGamePollStatus( ON_GAMEPOLL_FIELD );

		pOnlineCharUI->SetAction(TRUE);
		return 0;

		// �����������̽��� ��ġ�� �ʰ� �ٷ� ������ ����.
		// ���� ������ ����.										
		pMyData->SetGotoVillage(FALSE, m_sVillageDefenceInterface.m_uiVillageCode);

		// �������� ������ ���ٰ� ��û�Ѵ�.		
		pOnlineClient->SendGotoVillage(m_sVillageDefenceInterface.m_uiVillageCode);

		m_sVillageDefenceInterface.m_uiVillageCode = 0;
	}

	return 0;
}

/*
// �����ϱ� �������̽����� ������ ���� ��û�� ���� ���� ó��
// ON_RESPONSE_WAR_VILLAGE_IN
UI16	cltOnlineWorld::OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg)
{
	OnResWarVillageIn	*pOnResWarVillageIn = (OnResWarVillageIn *)pMsg;

	// �����ϱ� �������̽����� ������ ���� ĳ���Ͱ� ������� ���
	if (pMyData->GetMyCharUniqueID() == pOnResWarVillageIn->uiUniqueID)
	{
		// �����ϱ� �������̽� ���ҽ� ����
		FreeVillageDefenceInterface();

		// �����������̽����� ������ ���� ��쿡 ��� ���� ���� ���� ���� �ɼ��� Ȱ��ȭ�� ��� �ٽ� ��Ȱ��ȭ�ϱ�.
		// ������ ������ �ƴ� ���¿��� ������ �� ��쿡��.
		if ((m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT) 
			&& (0 == (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)))
		{
			m_scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
		}

		UI16 uiVillageCode;

		// �α� ó��
		OnRecvVillageInfo*		lpOnRecvVillageInfo = (OnRecvVillageInfo*)((UI08*)(pOnResWarVillageIn) + sizeof(OnResWarVillageIn));

		// ������ ���� ������ �� �� �ֶǷ� �㰡�� �޾Ҵ�.

		// ������ ��ü�� �����Ѵ�.
		pOnlineVillage	=	new OnlineVillage;
			
		// ������ �̵��ؾ� �Ѵ�.
		uiVillageCode	=	pMyData->GetGotoVillage();						
			
		pOnlineVillage->Init(uiVillageCode, lpOnRecvVillageInfo, this);

		SetGamePollStatus( ON_GAMEPOLL_VILLAGE );
			
		// ��� ĳ���͵��� ���ش�.
		pIOnlineChar->AllFree();
			
		// ������Ʈ�� ��� �����Ѵ�.
		pIOnlineObject->FreeAll();

		// �� �̻� ������ ���� ������ ���ٴ� �޽����� ��ٸ��� �ʵ��� �����Ѵ�.
		pMyData->SetWaitGotoVillageResponse(FALSE);		
			
		// ������ ���ٴ� ������ �ʱ�ȭ �Ѵ�.
		pMyData->SetGotoVillage(FALSE, 0);			

		pOnlineClientChatting->MessageSend_NotIsMap();

		if(m_VillageInStruct != -1)
		{
			pOnlineVillage->MoveInStruct(GetVillageInStruct(), TRUE, TRUE);
		}
	}
	// �����ϱ� �������̽����� ������ ���� ĳ���Ͱ� �ٸ� ������� ���
	else
		pIOnlineChar->Free(pOnResWarVillageIn->uiUniqueID);	// ĳ���� ����

	return 0;
}
*/

// �����ϱ� �������̽��� ���´�. �ʵ��
// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
UI16	cltOnlineWorld::OnResponseLeaveVillageDefenceInterface(BYTE *pMsg)
{
	OnResWarVillageIn	*pOnResWarVillageIn = (OnResWarVillageIn *)pMsg;

	// ������ ������ ���
	if (ON_RET_OK != pOnResWarVillageIn->dwResponse)
		return 0;

	// �����ϱ� �������̽����� ������ ���� ĳ���Ͱ� ������� ���
	if (pMyData->GetMyCharUniqueID() == pOnResWarVillageIn->uiUniqueID)
	{
		// �����ϱ� �������̽� ���ҽ� ����
		FreeVillageDefenceInterface();

		// �� �̻� ������ ���� ������ ���ٴ� �޽����� ��ٸ��� �ʵ��� �����Ѵ�.
		pMyData->SetWaitGotoVillageResponse(FALSE);		
			
		// ������ ���ٴ� ������ �ʱ�ȭ �Ѵ�.
		pMyData->SetGotoVillage(FALSE, 0);

		// ���� �������̽����� ���Դٰ� �����͵� �ʱ�ȭ
		pMyData->SetInVillageDefenceInterface(FALSE);

		m_sVillageDefenceInterface.SetDelayTimeUsingVillageAttackType(0);
		m_sVillageDefenceInterface.m_uiVillageCode = 0;

		CSiegeWarfareUnitUI *pSiegeWarfareUnitUI = pOnlinePannel->GetSiegeWarfareUnitUIHandle();

		if (pSiegeWarfareUnitUI)
		{
			pSiegeWarfareUnitUI->SetCurrentDelayTimeUsingVillageAttackType(
					m_sVillageDefenceInterface.m_siCurrentDelayTimeUsingVillageAttackType);

			pSiegeWarfareUnitUI->SetMaxDelayTimeUsingVillageAttackType(
					m_sVillageDefenceInterface.m_siMaxDelayTimeUsingVillageAttackType);

			pSiegeWarfareUnitUI->SetVillageCode(m_sVillageDefenceInterface.m_uiVillageCode);
		}
		
		// Field Poll ����
		SetGamePollStatus( ON_GAMEPOLL_FIELD );

		pOnlineCharUI->SetAction(TRUE);
	}

	pIOnlineChar->SetInVillageDefenceInterface(pOnResWarVillageIn->uiUniqueID, FALSE);

	return 0;
}

// �����ϱ� �������̽����� ���� �Ӽ� ���� �ʱ�ȭ ���� ���
// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK
UI16	cltOnlineWorld::OnResponseVullageAttackTypeCheck(BYTE *pMsg)
{
	OnResVillageAttackTypeCheck *pOnResVillageAttackTypeCheck = (OnResVillageAttackTypeCheck *)pMsg;

	if (ON_RET_ATTACK_TYPE_OK == pOnResVillageAttackTypeCheck->dwResponse)
	{
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// robypark 2004/12/27 10:46
// ������ �ð������� �˻��Ѵ�.
BOOL	cltOnlineWorld::IsInTimeSiegeWarfare(void)
{
	// �����ϴ� ���ΰ�?
	if (IsInDaySiegeWarfare())
	{
		if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN)
				&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END))
		{
			// ���� �ð��̴�.
			return TRUE;
		}
	}

	// ���� �ð��� �ƴϴ�.
	return FALSE;
}

// ������ ����� ������ ������ ��ȯ(����) �ð������� �˻�
BOOL	cltOnlineWorld::IsInTimeRecruitArmyItem(void)
{
	/*
	if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_RECRUITARMYITEM_BEGIN)
			&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siSIEGEWARFARETIME_RECRUITARMYITEM_END))
	{
		// ������ ������ ��ȯ ���� �ð��̴�.
		return TRUE;
	}
	*/

	// �Ұ����� �ð��̴�.
	return FALSE;
}

// ������ ����� ������ �������� ��ܿ����� ���� �� �� �ִ� �ð������� �˻�
BOOL	cltOnlineWorld::IsInTimeGiveArmyItem(void)
{
	/*
	if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siGiveArmyItem_BeginTime)
			&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siGiveArmyItem_EndTime))
	{
		// ������ �������� ��ܿ����� ���� �� �� �ִ� �ð��̴�.
		return TRUE;
	}
	*/

	// �Ұ����� �ð��̴�.
	return FALSE;
}

// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� �Ұ��� �ð����� �˻�
BOOL	cltOnlineWorld::IsInTimeDisabledRecruitArmyInGovernment(void)
{
	// �����ϴ� ���ΰ�?
	if (IsInDaySiegeWarfare())
	{
		if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_Begin)
				&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_End))
		{
			// ��û���� ���¸��� �� ����ǰ ������ �Ұ����� �ð��̴�.
			return TRUE;
		}
	}

	// ������ �ð��̴�.
	return FALSE;
}

// ����ڵ��� ��� ������ �Ұ����� �ð������� �˻�
BOOL	cltOnlineWorld::IsInTimeDisabledTimeToJoin(void)
{
	// �����ϴ� ���ΰ�?
	if (IsInDaySiegeWarfare())
	{
		if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_Begin)
				&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_End))
		{
			// ��� ���� ��û �� ���� �㰡�� �Ұ����� �ð��̴�
			return TRUE;
		}
	}

	// ������ �ð��̴�.
	return FALSE;
}

// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð�(����� ��ܿ� �ذ� �Ұ����ϴ�) ���� �˻�
BOOL	cltOnlineWorld::IsInTimeDisabledTimeToWithdraw(void)
{
	// ���� �ð�
	UI08 uiHour = m_RealTime.uiHour;

	// ����(�� 12��)�� 24�� �����Ͽ� ó���Ѵ�.
	if (0 == m_RealTime.uiHour)
		uiHour = 24;

	// �����ϴ� ���ΰ�?
	if (IsInDaySiegeWarfare())
	{
		if ((uiHour >= m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_Begin)
				&& (uiHour < m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_End))
		{
			// ��ܿ��� Ż�� �Ұ����� �ð��̴�.
			return TRUE;
		}
	}

	// ������ �ð��̴�.
	return FALSE;
}

// ��ź���� ���� ó�� �ð����� �˻�
BOOL	cltOnlineWorld::IsInTimeAllInInvestment(void)
{
	// ���� �ð�
	UI08 uiHour = m_RealTime.uiHour;

	// ����(�� 12��)�� 24�� �����Ͽ� ó���Ѵ�.
	if (0 == m_RealTime.uiHour)
		uiHour = 24;

	// �����ϴ� ���ΰ�?
	if (IsInDaySiegeWarfare())
	{
		if ((uiHour >= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_Begin)
				&& (uiHour < m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End))
		{
			// ����� ��ܿ��鿡�� ��ź���� ������ �� �� �ִ� �ð� Ȥ�� ������ ��ź ������ �� �ִ� �ð��̴�.
			return TRUE;
		}
	}

	// �Ұ����� �ð��̴�.
	return FALSE;
}

// robypark 2005/1/31 16:53
// �����ϴ� ������ �˻�
BOOL	cltOnlineWorld::IsInDaySiegeWarfare(void)
{
    struct tm tmToday;
	ZeroMemory(&tmToday, sizeof(tmToday));

	tmToday.tm_min	= m_RealTime.uiMinute;			/* minutes after the hour - [0,59] */
	tmToday.tm_hour	= m_RealTime.uiHour;			/* hours since midnight - [0,23] */
	tmToday.tm_mday	= m_RealTime.uiDay;				/* day of the month - [1,31] */
	tmToday.tm_mon	= m_RealTime.uiMonth - 1;		/* months since January - [0,11] */
	tmToday.tm_year	= m_RealTime.uiYear - 1900;		/* years since 1900 */

	// ������ ���� �������� �˾� ����
    if( mktime( &tmToday ) == (time_t)-1 )
		return FALSE;	// ���� ��ȯ ����

	// ���� ���� ����Ʈ �˻�
	for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
	{
		if (-1 != m_SOptionSiegeWarfare.scSiegeDay[i])
		{
			if (tmToday.tm_wday == m_SOptionSiegeWarfare.scSiegeDay[i])
			{
				// ������ �����ϴ� ���̴�.
				return TRUE;
			}
		}
	}

	// ������ �����ϴ� ���� �ƴϴ�.
	return FALSE;
}

// ����ڿ� ���� ����� ��ܿ������� �˻�
BOOL	cltOnlineWorld::IsMemberOfSameGuild(UI16 uiDestUniqueID)
{
	if (pMyData->GetGuildName())	// ����ڰ� ��ܿ� ����������,
	{
		if (NULL == pIOnlineChar->GetGuildName(uiDestUniqueID))	// ���� ĳ���Ͱ� ��ܿ� �������� ���� ���
			return FALSE;
		else
		{
			// ����ڰ� ��� �����ڶ��
			if (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) == pMyData->GetGuildClass())
				return FALSE;

			// ���� ��ܿ��� �ƴϸ�
			if (0 != strcmp(pMyData->GetGuildName(), pIOnlineChar->GetGuildName(uiDestUniqueID)))
				return FALSE;
			else
			{
				// ��� ���������� ��
				if (pIOnlineChar->GetGuildInGrade(uiDestUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
					return FALSE;
				else
					return TRUE;	// ���� ��ܿ��̴�.
			}
		}
	}

	return FALSE;
}

// ����ڿ� ���� ����� ��ܿ������� �˻�
// char *pszGuildName:	���� ��� ��
// SI32 siClassInGuild: ���� ��ܳ� ����
BOOL	cltOnlineWorld::IsMemberOfSameGuild(char *pszGuildName, SI32 siClassInGuild)
{
	if (pMyData->GetGuildName())	// ����ڰ� ��ܿ� ����������,
	{
		if (NULL == pszGuildName)	// ���� ĳ���Ͱ� ��ܿ� �������� ���� ���
			return FALSE;
		else
		{
			// ����ڰ� ��� �����ڶ��
			if (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) == pMyData->GetGuildClass())
				return FALSE;

			// ���� ��ܿ��� �ƴϸ�
			if (0 != strcmp(pMyData->GetGuildName(), pszGuildName))
				return FALSE;
			else
			{
				// ��� ���������� ��
				if (siClassInGuild == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
					return FALSE;
				else
					return TRUE;	// ���� ��ܿ��̴�.
			}
		}
	}

	return FALSE;
}

// ĳ���� A�� ĳ����B�� ���� ����� ��ܿ������� �˻�
BOOL	cltOnlineWorld::IsMemberOfSameGuild(UI16 uiUniqueID, UI16 uiDestUniqueID)
{
	if (pIOnlineChar->GetGuildName(uiUniqueID))	// ĳ����A�� ��ܿ� ����������,
	{
		if (NULL == pIOnlineChar->GetGuildName(uiDestUniqueID))	// ĳ����B�� ��ܿ� �������� ���� ���
			return FALSE;
		else
		{
			// ĳ����A�� ��� �����ڶ��
			if (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) == pIOnlineChar->GetGuildInGrade(uiUniqueID))
				return FALSE;

			// ĳ����A�� ĳ����B�� ���� ��ܿ��� �ƴϸ�
			if (0 != strcmp(pIOnlineChar->GetGuildName(uiUniqueID), pIOnlineChar->GetGuildName(uiDestUniqueID)))
				return FALSE;
			else	// ĳ����A�� ĳ����B�� ���� ��ܸ��̴�.
			{
				// ĳ����B�� ��� ���������� ��
				if (pIOnlineChar->GetGuildInGrade(uiDestUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
					return FALSE;
				else
					return TRUE;	// ���� ��ܿ��̴�.
			}
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

// robypark 2005/2/15 15:30
// ������ ������ ġ�ᰡ �������� �˻��Ͽ� ���콺 Ŀ�� ������ ���� ���콺 Ÿ�� ���
// UI16 uiTargetUniqueID: �ǰ����� UniqueID
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanHeal(UI16 uiUniqueID)
{
	UI16 uiMyCharUniqueID = pMyData->GetMyCharUniqueID();

/*
	// �ٸ� ������ ġ���� �� �ִ°�?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiMyCharUniqueID)].IsAttribute2(ATB2_HEAL))
		return ON_MOUSE_BATTLE_SUB_NONE;
*/

	// ġ���ϱ� ���� ����(����ǰ)�� �ִ°�?
	if (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID) <= 0)
		return ON_MOUSE_BATTLE_SUB_NONE;

	// �ش� ĳ���Ͱ� ġ�� �����Ѱ�?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiUniqueID)].IsAttribute1(ATB_PATIENT))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// �ش� ĳ���Ͱ� �λ���Ͽ��°�?
	if (pIOnlineChar->GetSiegeWarfare_MaximumHP(uiUniqueID) <= pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// ��� ĳ���Ͱ� ���� ��ܿ��ΰ�?
	if (FALSE == IsMemberOfSameGuild(uiUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// ġ�� ����
	return ON_MOUSE_BATTLE_HEAL;
}

// robypark 2005/1/25 18:18
// �ٸ� ������ ������ ġ�� �����Ѱ�?
BOOL	cltOnlineWorld::SiegeWarfare_CanHeal(UI16 uiUniqueID)
{
	UI16 uiMyCharUniqueID = pMyData->GetMyCharUniqueID();

	// �ٸ� ������ ġ���� �� �ִ°�?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiMyCharUniqueID)].IsAttribute2(ATB2_HEAL))
		return FALSE;

	// ġ���ϱ� ���� ����(����ǰ)�� �ִ°�?
	if (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID) <= 0)
		return FALSE;

	// �ش� ĳ���Ͱ� ġ�� �����Ѱ�?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiUniqueID)].IsAttribute1(ATB_PATIENT))
		return FALSE;

	// �ش� ĳ���Ͱ� �λ���Ͽ��°�?
	if (pIOnlineChar->GetSiegeWarfare_MaximumHP(uiUniqueID) <= pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID))
		return FALSE;

	// ��� ĳ���Ͱ� ���� ��ܿ��ΰ�?
	if (FALSE == IsMemberOfSameGuild(uiUniqueID))
		return FALSE;

	// ġ�� ����
	return TRUE;
}

// robypark 2005/1/26 18:22
// �ʵ忡 ���� �� ���콺 Ŀ�� �Ʒ� ������Ʈ�鿡 ���� ó��(Mouse Move)
void	cltOnlineWorld::ProcessFieldMouseMove(BOOL bIsSiegeWarfareUnit, POINT ptMouse)
{
	CHAR			szMyCharSpeechBuffer[1024];
	CHAR			TempBuffer[1024];
	CHAR			TempBuffer1[1024];
	SI32			siSetMouse = ON_MOUSE_BASIC;
	bool			bEnabledCursor = true;		// ���� ���콺 �� ��� Į, Ȱ, ���� ���� ���� Ŀ�� �̹����� �̿��ϱ� ����

	// robypark 2005/1/8 11:43
	// ������ ������ ���콺 Ŀ���� ����Ű�� �ִ� ĳ������
	// ǳ�� ����(ü��, ����ǰ ������) ǥ���� �� ��ǥ�� �� % ���� ����ü �ʱ�ȭ
	ZeroMemory(&m_sSiegeWarfareUnitGaugeInfo, sizeof(SSiegeWarfareUnitGaugeInfo));

	m_SelectCharacterIndex     = -1;
	m_SelectCharacterNameDrawX = -1000;
	m_SelectCharacterNameDrawY = -1000;

	if(pOnlineVillage == NULL)
	{
		if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
		{	
			siSetMouse	=	ON_MOUSE_BASIC;

			switch(pMousePointInfo->GetPointKind())
			{
			case ON_MOUSE_POINT_CHAR:		
				// ���� ���콺�� ĳ���� ���� �ִ�.
				SI32		TempColor;
				XSPR*		lpXSPR;	// ��� ��� ��������Ʈ
				BOOL		fFlag;	// ��� ����� �ִ���

				// ���� ĳ������ ������ �ִ��� �˻�

				if(pIOnlineChar->GetDataType((UI16)pMousePointInfo->GetParam()) == 1)
				{
					pOnlineClient->SendPlayerDetailInfo((UI16)pMousePointInfo->GetParam());
					pIOnlineChar->SetDataType((UI16)pMousePointInfo->GetParam(), 2);
				}

				TempColor = ON_HELP_WHITE;
				if(pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam())))
				{
					if(pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()) && !m_bEvent)
					{
						TempColor = ON_HELP_WHITERED;
					}
				}
				// robypark 2004/12/24 15:33 ���� ��ܿ� ������ ������ ��� �ؽ�Ʈ Į�� ����
				else if(pIOnlineChar->IsSiegeWarfareUnit((UI16)pMousePointInfo->GetParam()))
				{
					if(pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()) && !m_bEvent)
					{
						TempColor = ON_HELP_WHITERED;
					}
				}

				// Ŀ�� �Ʒ� ĳ���Ͱ� ��忡 ������ ĳ�������� �˻�
				if(pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam()) && (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())))
				{
					fFlag = FALSE;
					// ��� ����� �ִ��� �˻�
					lpXSPR = pOnlineResource->GetxsprFlagM(pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
					if(lpXSPR)
					{
						if(lpXSPR->Image)
						{
							fFlag = TRUE;
						}
					}

					char *pPlayerID = pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam());

					// ĳ���� �̸� ���
					if( pPlayerID )
					{
						if(m_bEvent)
						{
							if(!pIOnlineChar->IsInvalidBattle((UI16)pMousePointInfo->GetParam()))		TempColor = ON_HELP_RED;
							sprintf((char*)TempBuffer, "%s(%d)(%d)", pPlayerID, pIOnlineChar->GetRank((UI16)pMousePointInfo->GetParam()), pIOnlineChar->GetScore((UI16)pMousePointInfo->GetParam()));
						}
						else
						{
							// ����� ĳ���Ͱ� ��� ������ ���� ��� ��ī��Ʈ ǥ��
							if(m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)TempBuffer, "%s(%d)", pPlayerID, pIOnlineChar->GetFightPower((UI16)pMousePointInfo->GetParam()));
							else                                sprintf((char*)TempBuffer, "%s(%d)(%d)", pPlayerID, pIOnlineChar->GetFightPower((UI16)pMousePointInfo->GetParam()), (UI16)pMousePointInfo->GetParam());

							// ��ܸ�� ����
							sprintf((char*)TempBuffer1, "[%s:%s]", pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam()), pOnlineText->Get(m_cltIGuildClass.GetClassName(pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()))));
						}

						// robypark 2004/11/9 13:33
						// ���콺 Ŀ�� �Ʒ� ĳ���� ���� ���
						SI16 siKindIdx = pIOnlineChar->GetImjinKind( (UI16)pMousePointInfo->GetParam() );
						SI16 siCharacterUniqueID = (UI16)pMousePointInfo->GetParam();

						// ���콺 Ŀ�� �Ʒ� ĳ���Ͱ� ������ �������� �˻�
						if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(siCharacterUniqueID))
						{
							if(fFlag)
							{
								// ��� ��� ���� ����
								pOnlineHelp->SetTextID((char*)TempBuffer1, (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &m_SelectCharacterNameDrawX, &m_SelectCharacterNameDrawY);
								m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
							}
							else
							{
								// ��� ��� ����
								pOnlineHelp->SetTextID((char*)TempBuffer1, (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor);
							}
						}
						// ������ �����̴�
						else
						{
							if(fFlag)
							{
								m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth = true;
								m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct = (pIOnlineChar->GetSiegeWarfare_Hp(siCharacterUniqueID) * 10)
																						/ (float)pIOnlineChar->GetSiegeWarfare_MaximumHP(siCharacterUniqueID);
								// ��� ��� ���� ����
								// ������ ������ ��� ü�� �� ����ǰ(���޼�����)�� �߰��� ǥ���ϵ��� �Ѵ�.
								/*
								// ���� ������ ���
								if (KI[siKindIdx].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
								{
									m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods = true;
									m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeSupplyGoodsPct = (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(siCharacterUniqueID) * 10)
																								/ (float)pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(siCharacterUniqueID);

									pOnlineHelp->SetSiegeWarfareUnitToolTipText((char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor,
																					m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
																					true,
																					lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &m_SelectCharacterNameDrawX, &m_SelectCharacterNameDrawY);
									
									m_SelectCharacterNameDrawY += 4;
								}
								else
								*/
								{
									pOnlineHelp->SetSiegeWarfareUnitToolTipText((char*)TempBuffer, (char *)TempBuffer1, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_CENTER | ON_HELP_UP), TempColor,
																					m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
																					false,
																					lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &m_SelectCharacterNameDrawX, &m_SelectCharacterNameDrawY);
								}
								// pOnlineHelp->SetTextID((char*)TempBuffer1, (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &m_SelectCharacterNameDrawX, &m_SelectCharacterNameDrawY);
								// ���콺�� ĳ���͸� ����Ű�� �ִ�.
								m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
							}
							else
							{
								m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth = true;
								m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct = (pIOnlineChar->GetSiegeWarfare_Hp(siCharacterUniqueID) * 10)
																						/ (float)pIOnlineChar->GetSiegeWarfare_MaximumHP(siCharacterUniqueID);

								// ��� ��� ����
								// ������ ������ ��� ü�� �� ����ǰ(���޼�����)�� �߰��� ǥ���ϵ��� �Ѵ�.
								/*
								// ���� ������ ���
								if (KI[siKindIdx].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
								{
									m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods = true;
									m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeSupplyGoodsPct = (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(siCharacterUniqueID) * 10)
																								/ (float)pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(siCharacterUniqueID);

									pOnlineHelp->SetSiegeWarfareUnitToolTipText((char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor,
																					m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
																					true);
								}
								else
								*/
								{
									pOnlineHelp->SetSiegeWarfareUnitToolTipText((char*)TempBuffer, (char *)TempBuffer1, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_CENTER | ON_HELP_UP), TempColor,
																					m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
																					false);
								}
								// pOnlineHelp->SetTextID((char*)TempBuffer1, (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor);
								// ���콺�� ĳ���͸� ����Ű�� �ִ�.
								m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
							}
						}
					}
				}
				// ��ܿ� �������� ���� ĳ���͸� ����Ű�� �ִ� ���
				else
				{
					char *pPlayerID = pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam());

					if( pPlayerID )
					{
						if(m_bEvent)
						{
							if(!pIOnlineChar->IsInvalidBattle((UI16)pMousePointInfo->GetParam()))		TempColor = ON_HELP_RED;
							sprintf((char*)TempBuffer, "%s(%d)(%d)", pPlayerID, pIOnlineChar->GetRank((UI16)pMousePointInfo->GetParam()), pIOnlineChar->GetScore((UI16)pMousePointInfo->GetParam()));
						}
						else
						{
							if(m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)TempBuffer, "%s(%d)", pPlayerID, pIOnlineChar->GetFightPower((UI16)pMousePointInfo->GetParam()));
							else                                sprintf((char*)TempBuffer, "%s(%d)(%d)", pPlayerID, pIOnlineChar->GetFightPower((UI16)pMousePointInfo->GetParam()), (UI16)pMousePointInfo->GetParam());
						}

						SI16 siKindIdx = pIOnlineChar->GetImjinKind( (UI16)pMousePointInfo->GetParam() );
						SI16 siCharacterUniqueID = (UI16)pMousePointInfo->GetParam();

						// robypark 2004/11/9 13:33
						// ���콺 Ŀ�� �Ʒ� ĳ���� ���� ���
						// ���콺 Ŀ�� �Ʒ� ĳ���Ͱ� ������ �������� �˻�
						if (FALSE == IsSiegeWarfareUnit(siKindIdx))
							pOnlineHelp->SetText((char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor );
						else
						{
							m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth = true;
							m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct = (pIOnlineChar->GetSiegeWarfare_Hp(siCharacterUniqueID) * 10)
																					/ (float)pIOnlineChar->GetSiegeWarfare_MaximumHP(siCharacterUniqueID);

							// ������ ������ ��� ü�� �� ����ǰ(���޼�����)�� �߰��� ǥ���ϵ��� �Ѵ�.
							/*
							// ���� ������ ���
							if (KI[siKindIdx].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
							{
								m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods = true;
								m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeSupplyGoodsPct = (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(siCharacterUniqueID) * 10)
																							/ (float)pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(siCharacterUniqueID);

								pOnlineHelp->SetSiegeWarfareUnitToolTipText((char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor,
																					m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
																					true);

							}
							else
							*/
							{
								pOnlineHelp->SetSiegeWarfareUnitToolTipText((char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_CENTER | ON_HELP_UP), TempColor,
																					m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
																					false);
							}

							// ���콺�� ĳ���͸� ����Ű�� �ִ�.
							m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
						}
					}
				}

				if(m_ShowMessagBoxCharInfoFlag)
				{
					if(pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam()) && (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())))
					{
						clGrp.Error("Charactor Info", "UNID : %d, ID : %s, Guild : %s, GuildInClass : %d, GuildFlag : %d", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
							, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()), pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
					}
					else
					{
						clGrp.Error("Charactor Info", "UNID : %d, ID : %s", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()));
					}
				}
				if(m_GameOption->m_Option_PKFlag == TRUE)
				{
					siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
				}
				else
				{
					// �ڽ��� ������ ������ ���
					if (TRUE == bIsSiegeWarfareUnit)
					{
						UI16 uiTargetUniqueID = (UI16)pMousePointInfo->GetParam();

						// ��� ĳ���Ͱ� ������ ������ �ƴ϶��
						if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
						{
							break;
						}

						// Ŀ�� �Ʒ� ĳ���� ���
						pIOnlineChar->SetCursorHoverCharacter(uiTargetUniqueID, TRUE);

						// �ٸ� ������ ġ���� �� �ִ°�?
						if (FALSE == KI[pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID())].IsAttribute2(ATB2_HEAL))
						{
							// ���콺 �Ʒ� ĳ���͸� ���� �����Ѱ�?
							SI16 ssSetMouse_BattleSub = SiegeWarfare_CheckCursorCanAttackUnit(uiTargetUniqueID);

							// ������ �������� �˻�
							if (ON_MOUSE_BATTLE_SUB_NONE != ssSetMouse_BattleSub)
							{
								switch (ssSetMouse_BattleSub)
								{
									case ON_MOUSE_BATTLE_SUB_SHORTRANGE:
										siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
										break;
									case ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE:
										siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
										bEnabledCursor = false;
										break;
									case ON_MOUSE_BATTLE_SUB_LONGRANGE:
										siSetMouse	=	ON_MOUSE_FIELD_BATTLE_BOW;
										break;
									case ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE:
										siSetMouse	=	ON_MOUSE_FIELD_BATTLE_BOW;
										bEnabledCursor = false;
										break;
									default:
										siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
										break;
								}
							}
						}
						else
						{
							// ġ�ᰡ ���� ���� ���� ���
							if (ON_MOUSE_BATTLE_HEAL != SiegeWarfare_CheckCursorCanHeal(uiTargetUniqueID))
								bEnabledCursor = false;

							siSetMouse	=	ON_MOUSE_BATTLE_HEAL;
						}
					}
					else	// ���� �Ϲ� ����
					{
						// �ش� ĳ���Ͱ� ������ ������ �ƴ� ���
						if (FALSE == pIOnlineChar->IsSiegeWarfareUnit((UI16)pMousePointInfo->GetParam()))
						{
							// �������� ���
							if(pOnlineMap->m_MapInfo.fWarFlag == 2)
							{
								if( !bPeace )
								{
									siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
								}
							}
							// ������ ���
							else if (FALSE == pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind( (UI16)pMousePointInfo->GetParam())))
							{
								siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
							}
							else
							{
								// �������
								if( !bPeace )
								{
									// ���� ��ܿ�
									if(pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()))
									{
										//siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
									}
									else if(pIOnlineChar->GetBattleFlag((UI16)pMousePointInfo->GetParam()))
									{
										siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
									}
								}
							}
						}
					}
				}
				break;

			case ON_MOUSE_POINT_ITEM:
				// ������ ���� �ִ�
				UI32		uiItemID;
				SI16		siItemX, siItemY;
				CItemHeader *pItem;
				char*		lpBuffer;

				pMousePointInfo->GetPos(&siItemX, &siItemY);
				uiItemID		=	pMousePointInfo->GetParam();
				if(uiItemID == 0)
				{
					// ���� Item�� ���

					MapStoreHeader*			lpMapStoreHeader;
					SI16					MPointFlag;
					SMapBoothData*			lpSMapBoothData;
	//				SMapBoothDescriptData*	lpSMapBoothDescriptData;				

					// ���ϴ� ������ Data�� ���´�.
					lpMapStoreHeader = pOnlineMap->GetMapStoreParser(siItemX, siItemY, &MPointFlag);
					if(MPointFlag > 0)
					{
						MPointFlag--;

						lpSMapBoothData = pOnlineMap->m_MapBoothInfo.GetBoothData(lpMapStoreHeader->siMapStoreCode);
						if(lpSMapBoothData)
						{
							if(lpSMapBoothData->sifData)
							{
								if(lpSMapBoothData->uiItemID[MPointFlag])
								{
									pItem	                = pItemsInfo->GetItem(lpSMapBoothData->uiItemID[MPointFlag]);
									
									lpBuffer = NULL;
									pOnlineHelp->SetText(pOnlineText->Get(pItem->m_siItemName)
										, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_GREEN | 0xff);
								}
							}
						}
					}
				}
				else
				{
					// �Ϲ� Item�� ���
					pOnlineHelp->SetTextFieldItem( pItemsInfo->GetItem( uiItemID ), pOnlineMap->GetItemQuantity( siItemX, siItemY ), (SI16)ptMouse.x, (SI16)ptMouse.y );
				}
				siSetMouse	=	ON_MOUSE_GETITEM;
				break;

			case ON_MOUSE_POINT_OBJECT:
				{				
					CHAR						szVillageName[1024];
					SVillageDescriptData*		lpSVillageDescriptData;
					SI16						VillageCode;
					char*						lpDesText;
					SI32						ReqBattlePower;
					SI32						ReqMaxLV;
					MONEY						ReqMoney;
					SI32						ReqMoLV;
					SI32						ReqMoCredit;
					PortalHeader*				lpPortalHeader;

					// ������Ʈ�� �켱 �������� �˾ƿ;� �Ѵ�.
					if(pIOnlineObject->IsAtb(UI16(pMousePointInfo->GetParam()), ATB_OBJECT_VILLAGE) == TRUE)	
					{
						if(pIOnlineObject->IsPortal(UI16(pMousePointInfo->GetParam())))
						{
							szVillageName[0] = 0;
							if(pIOnlineObject->GetPortalName(SI16(pMousePointInfo->GetParam())))
							{
								strcpy((char*)szVillageName, pIOnlineObject->GetPortalName(SI16(pMousePointInfo->GetParam())));
							}
							ReqBattlePower = 0;
							ReqMaxLV       = 0;
							ReqMoney       = 0;
							ReqMoLV        = 0;
							ReqMoCredit    = 0;
							if(pIOnlineObject->GetPortalID(SI16(pMousePointInfo->GetParam())) != -1)
							{
								lpPortalHeader = pOnlineMap->PortalParser.GetPortal(pIOnlineObject->GetPortalID(SI16(pMousePointInfo->GetParam())));
								if(lpPortalHeader)
								{
									ReqBattlePower = lpPortalHeader->siReqBattlePower;
									ReqMaxLV       = lpPortalHeader->siReqMaxLv;
									ReqMoney       = lpPortalHeader->i64PortalMoney;
									ReqMoLV        = lpPortalHeader->siNeedLV;
									ReqMoCredit    = lpPortalHeader->siNeedCredit;
								}
							}

							if(ReqBattlePower)
							{
								sprintf((char*)szMyCharSpeechBuffer, pOnlineText->Get(ON_TEXT_PORTALMSG_LOWLEVEL), ReqBattlePower);
								pOnlineHelp->SetText(szVillageName, (char*)szMyCharSpeechBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
							}
							else if(ReqMaxLV)
							{
								sprintf((char*)szMyCharSpeechBuffer, pOnlineText->Get(ON_TEXT_PORTALMSG_HIGHLEVEL), ReqMaxLV);
								pOnlineHelp->SetText(szVillageName, (char*)szMyCharSpeechBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
							}
							else
							{
								if(szVillageName[0])
								{
									if(ReqMoney)
									{
										sprintf((char*)szMyCharSpeechBuffer, pOnlineText->Get(ON_TEXT_PORTAL_DATA), ReqMoLV, ReqMoCredit);
										pOnlineHelp->SetText(szVillageName, (char*)szMyCharSpeechBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
									}
									else
									{
										if(ReqMoLV)
										{
											sprintf((char*)szMyCharSpeechBuffer, pOnlineText->Get(ON_TEXT_PORTALMSG_PKFIELD_LOWLEVEL), ReqMoLV);
											pOnlineHelp->SetText(szVillageName, (char*)szMyCharSpeechBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
										}
										else
										{
											pOnlineHelp->SetText(szVillageName, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
										}
									}
								}
							}
						}
						else
						{
							// ���� �̸��� �˾ƿ´�
							pIOnlineObject->GetName(SI16(pMousePointInfo->GetParam()), szVillageName);

							// ���� �������� �˻��ؼ� ���콺 Ŀ�� ���
							SI16 ssSetMouse_BattleSub = SiegeWarfare_CheckCursorCanAttackVillage(SI16(pMousePointInfo->GetParam()));

							// robypark 2004/11/9 13:27
							// ���� ����(�����ϴ�)�̸�, ����ڰ� ���� ���� ������ ������ ������ ��� ���콺 Ŀ�� ���������� ����
							if ((GetVillageWarDayGuild(pIOnlineObject->GetVillageCode((UI16)pMousePointInfo->GetParam())))
								&& (ON_MOUSE_BATTLE_SUB_NONE != ssSetMouse_BattleSub))
							{
								pOnlineHelp->SetText(szVillageName, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_RED);

								// ������ �������� �˻�
								if (ON_MOUSE_BATTLE_SUB_NONE != ssSetMouse_BattleSub)
								{
									switch (ssSetMouse_BattleSub)
									{
										case ON_MOUSE_BATTLE_SUB_SHORTRANGE:
											siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
											break;
										case ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE:
											siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
											bEnabledCursor = false;
											break;
										case ON_MOUSE_BATTLE_SUB_LONGRANGE:
											siSetMouse	=	ON_MOUSE_FIELD_BATTLE_BOW;
											break;
										case ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE:
											siSetMouse	=	ON_MOUSE_FIELD_BATTLE_BOW;
											bEnabledCursor = false;
											break;
										default:
											siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
											break;
									}
								}
								break;
							}

							// ���� ������ ���´�.
							VillageCode = pIOnlineObject->GetVillageCode(SI16(pMousePointInfo->GetParam()));
							lpDesText = NULL;
							lpSVillageDescriptData = pOnlineVillageDescriptList->GetDescriptData(VillageCode);
							if(lpSVillageDescriptData == NULL)
							{
								pOnlineVillageDescriptList->AddDescriptData(VillageCode);
								pOnlineClient->SendRequestVillageDescript_Read(VillageCode);
							}
							else
							{
								if(lpSVillageDescriptData->m_fWait == FALSE)
								{
									lpDesText = (char*)lpSVillageDescriptData->m_DescriptText;
								}
							}

							if(lpDesText)
							{
								if(lpDesText[0])
								{
									pOnlineHelp->SetText(szVillageName, lpDesText, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
								}
								else
								{
									pOnlineHelp->SetText(szVillageName, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
								}
							}
							else
							{
								pOnlineHelp->SetText(szVillageName, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
							}
						}
						// ����� ĳ���Ͱ� ���� �������̽��� �ִ°�?
						if (FALSE == pIOnlineChar->IsInVillageDefenceInterface(pMyData->GetMyCharUniqueID()))
							siSetMouse	=	ON_MOUSE_GOVIL;		// �Ϲ� ���� ����� ���콺 Ŀ�� ����
					}
					else
					{
						// ��ȭ����� ���� ������Ʈ�� ���콺 Ŀ���� ��ġ�� ��� ������Ʈ ���� �ؽ�Ʈ ǥ���ϱ�
						if(pIOnlineObject->GetMark(pMousePointInfo->GetParam()) != NULL)
						{
							pOnlineHelp->SetText(pIOnlineObject->GetMark(pMousePointInfo->GetParam()), (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE, TRUE);
							TRACE("%s", pIOnlineObject->GetMark(pMousePointInfo->GetParam()));
						}
					}
				}
				break;
			case ON_MOUSE_POINT_MILEPOST:
				break;		

			case ON_MOUSE_POINT_NPC:
				{
					NPCIndexHeader*		lpNPCIndexHeader;

					if(SI16(pMousePointInfo->GetParam()))
					{
						lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(SI16(pMousePointInfo->GetParam()));
						
						if((lpNPCIndexHeader))
//						if((lpNPCIndexHeader) && (lpNPCIndexHeader->bTalkOneself))
						{
							pOnlineHelp->SetText(pOnlineText->Get(lpNPCIndexHeader->siName), (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
						}
					}
				}
				break;			
			}		
			pIOnlineMouse->SetMouse(siSetMouse, bEnabledCursor);
		}
		else
		{		
			// �⺻ ���콺�� �����Ѵ�.
			pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);
		}
	}
	else
	{		
		// �⺻ ���콺�� �����Ѵ�.
		pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);
	}
}

// robypark 2005/1/26 18:22
// �ʵ忡 ���� �� ���콺 Ŀ�� ���� ��ư Ŭ�� ó��
void	cltOnlineWorld::ProcessFieldMouseLeftBtnClick(BOOL bIsSiegeWarfareUnit, SI32 siCursorX, SI32 siCursorY)
{
	SI16			siTX, siTY;

	// ������ ���� ��� ���� ����
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	switch(pMousePointInfo->GetPointKind())
	{
		case ON_MOUSE_POINT_CHAR:
			// ���� �������̽� ���� ���
			m_sVillageDefenceInterface.m_uiVillageCode = 0;

			if(bCharMove == TRUE)
			{
				// �ʹ� ���� ���� ������ �������� ��� ������ �޼����� �ٿ�
				if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
				{
					SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
				}

				// ������ ĳ���� ǥ��
				pIOnlineChar->SetNewSelectedFlag((UI16)pMousePointInfo->GetParam(), TRUE);

				// ĳ���͸� �����ϰ� �Ѵ�.
				if(pOnlineTrade->IsWait())
					break;

				if(pOnlineBooth->IsActionIn())
					break;

				UI16 uiUniqueID_UnderPoint = (UI16)pMousePointInfo->GetParam();

				// robypark 2005/1/25 18:45
				// �ٸ� ĳ���� ġ����
				// ������ ĳ���Ͱ� ������ �������� �˻�
				if (TRUE == bIsSiegeWarfareUnit)
				{
					// ��� ĳ���Ͱ� ������ �����̶��,
					if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
					{
						// �����ð����� �˻�
						if(IsInTimeSiegeWarfare())
						{
							// ġ�ᰡ �����Ѱ�?
							if (SiegeWarfare_CanHeal(uiUniqueID_UnderPoint))
							{
								// ���� ������ ġ��õ� ��û
								pOnlineCharUI->SetOrderHeal(uiUniqueID_UnderPoint);
								break;
							}
							// ���� �����ϴٸ�
							else if (SiegeWarfare_CheckCanAttackUnit(uiUniqueID_UnderPoint))
							{
								// ���� ��� ����
								m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_CHARACTER;
								m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = uiUniqueID_UnderPoint;
								break;
							}
						}
					}

					// ������ ĳ���� ǥ�� ���
					pIOnlineChar->SetNewSelectedFlag((UI16)pMousePointInfo->GetParam(), FALSE);

					// ������ ������ �¶��� ������ �� �� ����.
					break;
				}

				// ��� ĳ���Ͱ� ������ �����̶��,
				if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
				{
					// ������ ĳ���� ǥ�� ���
					pIOnlineChar->SetNewSelectedFlag((UI16)pMousePointInfo->GetParam(), FALSE);

					// ������ ���ְ� �¶��� ������ �� �� ����.
					break;
				}

				// ������ ���� ���� �޽����� ������ ���
				if (pOnlinePannel->GetSiegeWarfareTransformUIWnd()->IsSendedMsg())
				{
					// �¶��� ������ �� �� ����.
					break;
				}

				if ((m_GameOption->m_Option_PKFlag == TRUE) && (!bPeace && pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()))
					&& (pOnlineMap->GetPKMapFlag() != 1) && (!pIOnlineChar->IsBoothOpen((UI16)pMousePointInfo->GetParam())))
				{
					pOnlineCharUI->SetAttack((UI16)pMousePointInfo->GetParam());
					// ȿ���� ���
					// robypark 2005/1/14 17:27
					// ���ΰ� ���� ������ ������ ���
					CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

					if (pFieldHeroInfo)
					{
						if (pFieldHeroInfo->GetSndTryBattle())
							HashCodePushEffect(pFieldHeroInfo->GetSndTryBattle(), 0, 0);
					}
				}
				else if((pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam()))) == FALSE)
				{
					pOnlineCharUI->SetAttack((UI16)pMousePointInfo->GetParam());
					// ȿ���� ���
					// robypark 2005/1/14 17:27
					// ���ΰ� ���� ������ ������ ���
					CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

					if (pFieldHeroInfo)
					{
						if (pFieldHeroInfo->GetSndTryBattle())
							HashCodePushEffect(pFieldHeroInfo->GetSndTryBattle(), 0, 0);
					}
				}
				else if(pIOnlineChar->GetBattleFlag((UI16)pMousePointInfo->GetParam()))
				{
					pOnlineCharUI->SetAttack((UI16)pMousePointInfo->GetParam());
					// ȿ���� ���
					// robypark 2005/1/14 17:27
					// ���ΰ� ���� ������ ������ ���
					CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

					if (pFieldHeroInfo)
					{
						if (pFieldHeroInfo->GetSndTryBattle())
							HashCodePushEffect(pFieldHeroInfo->GetSndTryBattle(), 0, 0);
					}
				}
				else
				{
					if(pOnlineMap->m_MapInfo.fWarFlag == 0)
					{
						if( !bPeace )
						{
							if(pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()))
							{
							}
							else if(pIOnlineChar->GetBattleFlag((UI16)pMousePointInfo->GetParam()))
							{
								pOnlineCharUI->SetAttack((UI16)pMousePointInfo->GetParam());

								// ȿ���� ���
								// robypark 2005/1/14 17:27
								// ���ΰ� ���� ������ ������ ���
								CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

								if (pFieldHeroInfo)
								{
									if (pFieldHeroInfo->GetSndTryBattle())
										HashCodePushEffect(pFieldHeroInfo->GetSndTryBattle(), 0, 0);
								}
							}
						}
					}
					else if(pOnlineMap->m_MapInfo.fWarFlag == 2)
					{
						if( !bPeace )
						{
							pOnlineCharUI->SetAttack((UI16)pMousePointInfo->GetParam());

							// ȿ���� ���
							// robypark 2005/1/14 17:27
							// ���ΰ� ���� ������ ������ ���
							CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

							if (pFieldHeroInfo)
							{
								if (pFieldHeroInfo->GetSndTryBattle())
									HashCodePushEffect(pFieldHeroInfo->GetSndTryBattle(), 0, 0);
							}
						}
					}
					else
					{
						if( !bPeace )
						{
							if(pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()))
							{
								SetMessage(pOnlineText->Get(ON_TEXT_NOTATTACK));								
							}
						}
					}
				}
			}
			break;

		case ON_MOUSE_POINT_OBJECT:
			{
				if(bCharMove == TRUE)
				{
					// �ʹ� ���� ���� ������ �������� ��� ������ �޼����� �ٿ�

					if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
					{
						SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
					}

					pIOnlineObject->GetPos(UI16(pMousePointInfo->GetParam()), &siTX, &siTY);

					// �����̳� ��Ż�� ���
					if(pIOnlineObject->IsAtb(UI16(pMousePointInfo->GetParam()), ATB_OBJECT_VILLAGE) == TRUE)
					{
						// robypark 2004/11/12 13:35
						// ������ ������ ���� ���� �����ϱ�
						// ������ ������ �ƴ� ���
						if (FALSE == bIsSiegeWarfareUnit)
						{
							// ������Ʈ�� ��ġ �˻�
							pOnlineCharUI->SetUseObject(siTX, siTY);
						}
						// ������ ������ ���
						else
						{
							// �������� ��Ż���� ����
							if(FALSE == pIOnlineObject->IsPortal((UI16)pMousePointInfo->GetParam()))
							{
								// ������ ���
								// �����ϴ� �������� �˻�
								// �����ð����� �˻�
								if(IsInTimeSiegeWarfare())
								{
									if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
									{
										// ���� ������ ������ ĳ�����ΰ�?
										if (SiegeWarfare_CheckCanAttackVillage())
										{
											// ������ ���� ��� ���� ����
											m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_VILLAGE;
											m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()));
										}
									}	// if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
									else
									{
										// ���� ������ ���
										if (GetVillageEnemyGuild( pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ) ) )
										{
											// ���� ���������� ���� �� ������ ��ܰ� �������� ����.
											char szErrorMsg[512];
											SVillageData*		lpSVillageData;
											VillageHeader*		lpVillageHeader;

											lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));
											lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));

											if ((lpSVillageData) && (lpVillageHeader))
											{
												// ����� �ִ� ������ ���
												if (NULL != lpSVillageData->m_VillageGuildName[0])
												{
													sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
																lpSVillageData->m_VillageGuildName, 
																pOnlineText->Get(lpVillageHeader->siVillageName ));
												}
												// ����� ���� ������ ���
												else
												{
													sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY),
																pOnlineText->Get(lpVillageHeader->siVillageName ));
												}

												pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);
											}
										}
										// �ش� ������ ĳ���Ͱ� �Ҽӵ� ��� ������ ������ ��� 
										// ����ǰ ���� ó��
										else if (TRUE == GetVillageOurGuild(pIOnlineObject->GetVillageCode((UI16)pMousePointInfo->GetParam())))
										{
											// ���� ���� ���� ��� ���
											LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(pMyData->GetMyCharKind());

											// ������ ������ ���� ���
											if (NULL != lpsSiegeUnitTable)
											{
												// ������ ���� �� ������ ���� ���� ������ ������ �������� �˻�
												if ((lpsSiegeUnitTable) && (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE))
												{
													// ���� �� ĳ���Ͱ� �����ϴ¸���� ���´�.
													OnOrder Order	=	pIOnlineChar->GetCurrentOrder(pMyData->GetMyCharUniqueID());
													//  ������ �ð��̸�, 
													if ((IsInTimeSiegeWarfare())
														&& (Order.siOrder != ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE))	// ������ ������ ���ڴٰ� ��û���� ���� ���
													{
														// ���� ���� �������̽��� ���ڴٰ� ĳ���Ϳ� ��� �� ������ ����
														pOnlineCharUI->SetUseObject_VillageDefence(siTX, siTY);
													}
												}
												else
												{
													pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "",
																				pOnlineText->Get(ON_TEXT_CANNOT_ENTER_VILLAGE_SIEGEWARFARE_UNIT));
												}
											}
										}
										else
										{
											// ���� ������ �ƴҰ��
											// ���� �Ұ��� �޽��� ����
											pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
										}
									}	// else [ if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam())))) ]
								}	// if((m_RealTime.uiHour >= SIEGEWARFARETIME_BEGIN) && (m_RealTime.uiHour < SIEGEWARFARETIME_END))
								else
								{
									char szErrorMsg[256];
									sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_VILLAGE_ATTACK_TIME), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);
									pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);												
								}
							}
							else
							{
								// ������ ������ �� �� �̵������� ��Ż �̿밡��
								// ��Ż ID ã��
								if(pIOnlineObject->GetPortalID(SI16(pMousePointInfo->GetParam())) != -1)
								{
									PortalHeader*				lpPortalHeader = NULL;
									
									// ��Ż ���� ����ü ã��
									lpPortalHeader = pOnlineMap->PortalParser.GetPortal(pIOnlineObject->GetPortalID(SI16(pMousePointInfo->GetParam())));

									if(lpPortalHeader)
									{
										// �� �ε��� 0�� �� �����θ� ��Ż �̿� ����
										if (0 == lpPortalHeader->siPortalMoveMapIndex)
										{
											// ������Ʈ�� ��ġ �˻�
											pOnlineCharUI->SetUseObject(siTX, siTY);
										}
										else
										{
											pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
																		pOnlineText->Get(ON_TEXT_CANNOT_ENTER_VILLAGE_SIEGEWARFARE_UNIT));
										}
									}
								}
							}
						}
					}
					else
						pOnlineCharUI->SetMove(siCursorX, siCursorY);
				}
			}
			break;

		case ON_MOUSE_POINT_ITEM:
			{				
				UI32	uiItemID;
				SI16	siItemX, siItemY;

				// ���� �������̽� ���� ���
				m_sVillageDefenceInterface.m_uiVillageCode = 0;

				if(bCharMove == TRUE)
				{
					// �ʹ� ���� ���� ������ �������� ��� ������ �޼����� �ٿ�

					if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
					{
						SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
					}

					pMousePointInfo->GetPos(&siItemX, &siItemY);
					uiItemID		=	pMousePointInfo->GetParam();				

					if(uiItemID != 0)
					{
						// Item �� �̵���Ű�� �κ�����
						pOnlineCharUI->SetPickUpItem(siItemX, siItemY, uiItemID);						
					}
					else
					{
						// ���� Item�� ���
					}
				}
			}
			break;

		case ON_MOUSE_POINT_MILEPOST:
			break;

		case ON_MOUSE_POINT_INTERFACE:
			break;

		case ON_MOUSE_POINT_NPC:
			{
				// ���� �������̽� ���� ���
				m_sVillageDefenceInterface.m_uiVillageCode = 0;

				if(SI16(pMousePointInfo->GetParam()))
				{
					NPCIndexHeader*		lpNPCIndexHeader;

					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(SI16(pMousePointInfo->GetParam()));

					// robypark 2004/9/9
					// ȥ�㸻�� ��Ȱ��ȭ�� NPC�� Ŭ���Ͽ� ������ �� ������ ����, ������ ������ �ƴϸ�(robypark 2004/11/22 11:44)
					if ((lpNPCIndexHeader) && (lpNPCIndexHeader->bTalkOneself) && (FALSE == bIsSiegeWarfareUnit))
					{
						if(bCharMove == TRUE)
						{
							// �ʹ� ���� ���� ������ �������� ��� ������ �޼����� �ٿ�

							if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
							{
								SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
							}

							// ������Ʈ�� ��ġ �˻�

							pOnlineCharUI->SetUseNPC(lpNPCIndexHeader->siMapXPos, lpNPCIndexHeader->siMapYPos, lpNPCIndexHeader->siCode);
						}
					}
				}
			}
			break;

		default:
			// ���� �������̽� ���� ���
			m_sVillageDefenceInterface.m_uiVillageCode = 0;

			// ���� ������ �������� üũ
			if(pOnlineBooth->IsActionIn() && !pOnlineInventory->IsActive() && !pOnlineMap->m_fHeroSelect && 
				pOnlineBooth->GetStatus() == ON_BENCH_IN_POLL && !pOnlineBooth->GetFollowerMode())
			{
				if(pOnlineBooth->IsField())
					pOnlineBooth->pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "",pOnlineText->Get(ON_TEXT_TRADE_LEAVEQ));
				else
					pOnlineBooth->pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "",pOnlineText->Get(ON_TEXT_TRADE2_LEAVEQ));
				pOnlineBooth->SetStatusIn(ON_BENCH_IN_EXIT_INIT);
				//pOnlineBooth->GetList()->DeleteSlot(pOnlineBooth->GetBoothSlot());
			}

			if(bCharMove == TRUE)
			{
				// �ʹ� ���� ���� ������ �������� ��� ������ �޼����� �ٿ�
				if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
				{
					SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
				}

				pOnlineCharUI->SetMove(siCursorX, siCursorY);
			}

			if( rand()%100 > 99 )
			{
				// ȿ���� ���
				// robypark 2005/1/14 17:27
				// ���ΰ� ���� ������ ������ ���
				CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

				if (pFieldHeroInfo)
				{
					if (pFieldHeroInfo->GetSndMove())
						HashCodePushEffect(pFieldHeroInfo->GetSndMove(), 0, 0);
				}
			}

			if(fLeftOneSwitch == 1)
			{
				pOnlineMousePoint->SetMouse(siCursorX, siCursorY, pOnlineMap->m_MapIndex);
			}

			// ����ä�� üũ
			if(pCBuriedItemParser->isBuriedArea(pOnlineMap->m_MapIndex))
			{
				SI16 siHeroX, siHeroY;
				if(!pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY))
					break;

				// ������ ������ Ž�� ��û���� ��ġ�� ���� ��ġ�� ���̰�
				UI16 DifX = abs(m_CharPosition_X - siHeroX);
				UI16 DifY = abs(m_CharPosition_Y - siHeroY);
				if(DifX > 7 || DifY > 7)
				{
					if(!pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &m_CharPosition_X, &m_CharPosition_Y))
						break;
					pOnlineClient->SendRequestProbeArea();
				}
			}				
			break;
	}
}


// robypark 2005/1/26 18:22
// �ʵ忡 ���� �� ���콺 Ŀ�� ������ ��ư Ŭ�� ó��
void	cltOnlineWorld::ProcessFieldMouseRightBtnClick(BOOL bIsSiegeWarfareUnit)
{
	if(pOnlineParty->GetPartyMsg() == 1) //PARTY_CHECK_MSG
	{
		pOnlineParty->SetPartyMsg(5);
	}

	if( pOnlineTrade->m_pMsgBox->IsActive() == FALSE )
	{
		switch(pMousePointInfo->GetPointKind())
		{
			case ON_MOUSE_POINT_CHAR:
				//-------------------------------------------------//
				// robypark 2005/1/14 15:28
				// �ŷ� ���̰ų�
				if (pOnlineTrade->IsActive())
				{
					// ������ �� �ŷ� ��û, ������ ���� �� ���� �Ұ����ϴ�
					break;
				}
				//-------------------------------------------------//

				// �������� Ŭ������ ���
				if(pIOnlineChar->IsBoothOpen((UI16)pMousePointInfo->GetParam()) && pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind( (UI16)pMousePointInfo->GetParam())))
				{
					if(!pOnlineBooth->IsActionIn())
					{
						// ��ſ� �������� �ƴҶ����� ������ �����ִ�
						if (!pOnlineFieldAuction->IsEntered())
						{											
							pOnlineBooth->SetActionOut(TRUE);
							pOnlineBooth->SetStatusOut(ON_BENCH_OUT_BENCH_CLICK);
							pOnlineBooth->SetUniqueID((UI16)pMousePointInfo->GetParam());
							pOnlineClient->SendGetBoothItemInfo((UI16)pMousePointInfo->GetParam());
						}
					}
				}
				else
				{
					if(bCharMove == TRUE)
					{
						// ���� ������� �� �ŷ�����
						//OnOrder	Order =	pIOnlineChar->GetCurrentOrder(pMyData->GetMyCharUniqueID());
						if(pMyData->IsWaitBattleResponse())
							break;

						UI16 uiUniqueID_UnderPoint = (UI16)pMousePointInfo->GetParam();

						// robypark 2005/1/13 22:2
						// ������ �߿� �Ϲ� ����(�ǳ���)�� �ʺ��� ���������� ������ ��ų �� �ִ��� �˻�
						if (SiegeWarfare_CheckAbledWarfRefugee(uiUniqueID_UnderPoint))
						{
							// ���� ��û
							// �� ��û ������ ������ ��� ��ɾ��� /w(����)�� ���� ����Ѵ�.(ON_RESPONSE_WARP)
							pOnlineClient->SendRequestSiegeWarfareWarp(uiUniqueID_UnderPoint);
							break;
						}

						// robypark 2005/1/25 18:45
						// �ٸ� ĳ���� ġ����
						// ������ ĳ���Ͱ� ������ �������� �˻�
						if (TRUE == bIsSiegeWarfareUnit)
						{
							/*
							// ��� ĳ���Ͱ� ������ �����̶��,
							if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
							{
								// ġ�ᰡ �����Ѱ�?
								if (SiegeWarfare_CanHeal(uiUniqueID_UnderPoint))
								{
									// ���� ������ ġ��õ� ��û
									pOnlineCharUI->SetOrderHeal(uiUniqueID_UnderPoint);
									break;
								}
								// ���� �����ϴٸ�
								else if (SiegeWarfare_CheckCanAttackUnit(uiUniqueID_UnderPoint))
								{
									// ���� ��� ����
									m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_CHARACTER;
									m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = uiUniqueID_UnderPoint;
									break;
								}
							}
							*/

							// ������ ������ �ŷ� �� ��Ƽ��û�� �� �� ����.
							break;
						}

						// �����������̽� �ȿ� �ִ� ĳ���Ͱ� �ƴϸ�
						if (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiUniqueID_UnderPoint))
						{							
							// robypark 2004/12/24 15:19
							// �ŷ� ��� ĳ���Ͱ� ������ �����̶��,
							if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
							{
								// �ŷ� �� ��Ƽ ��û�� �� �� ����.
								break;
							}
							// ���콺 ������ ��ư���� ������ ĳ���Ͱ� �÷��̾��̰ų�
							else if (pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind(uiUniqueID_UnderPoint)))
							{
								// ���� ����� ��� �ŷ� ��û ����.
								if (IsPeace() == FALSE)
									break;

								if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()))
									break;

								ZeroMemory(pOnlineTrade->FriendInfo.szCharName, sizeof(pOnlineTrade->FriendInfo.szCharName));
								strncpy(pOnlineTrade->FriendInfo.szCharName, pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()), ON_ID_LENGTH);
								pOnlineTrade->FriendInfo.uiDestUniqueID = uiUniqueID_UnderPoint;	// �ŷ� ��� ĳ���� ����ũ ���̵� ����
							
								// ��ȿ�� ĳ���� �����̶��
								if(( pOnlineTrade->FriendInfo.szCharName[0] != 0) && (pOnlineTrade->FriendInfo.uiDestUniqueID))
								{
									// �ŷ� ��û�� ĳ���Ͱ� ����� ���
									if(!strcmp(pOnlineTrade->FriendInfo.szCharName, pOnlineText->Get(8000520)))
									{
										pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_OPERATOR_WARNING));
									}							
									else
									{
										if(pOnlineParty->GetPartyMsg() == 5) //PARTY_NO_MSG
										{
											pOnlineParty->SetPartyMode(IpD.Mouse_X,IpD.Mouse_Y,(UI16)pMousePointInfo->GetParam());
											pOnlineParty->SetCharID(pOnlineTrade->FriendInfo.szCharName);
											bCharMove = FALSE;
											break;
										}
									}
								}
							}
						}
					}
				}
				break;

			case ON_MOUSE_POINT_OBJECT:
				if(bCharMove == TRUE)
				{
					// �ʹ� ���� ���� ������ �������� ��� ������ �޼����� �ٿ�
					if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
					{
						SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
					}

					/*
					// �������� �˻�
					if(pIOnlineObject->IsAtb(UI16(pMousePointInfo->GetParam()), ATB_OBJECT_VILLAGE) == TRUE)	
					{
						if(pIOnlineObject->IsPortal(UI16(pMousePointInfo->GetParam())) == FALSE)
						{
							// �����ϴ� �������� �˻�
							// �����ð����� �˻�
							if(IsInTimeSiegeWarfare())
							{
								if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
								{
									// ���� ������ ������ ĳ�����ΰ�?
									if (SiegeWarfare_CheckCanAttackVillage())
									{
										// ������ ���� ��� ���� ����
										m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_VILLAGE;
										m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()));
									}
								}	// if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
								else
								{
									// ���� ������ ���
									if (GetVillageEnemyGuild( pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ) ) )
									{
										// ���� ���������� ���� �� ������ ��ܰ� �������� ����.
										char szErrorMsg[512];
										SVillageData*		lpSVillageData;
										VillageHeader*		lpVillageHeader;

										lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));
										lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));

										if ((lpSVillageData) && (lpVillageHeader))
										{
											// ����� �ִ� ������ ���
											if (NULL != lpSVillageData->m_VillageGuildName[0])
											{
												sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
															lpSVillageData->m_VillageGuildName, 
															pOnlineText->Get(lpVillageHeader->siVillageName ));
											}
											// ����� ���� ������ ���
											else
											{
												sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY),
															pOnlineText->Get(lpVillageHeader->siVillageName ));
											}

											pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);
										}
									}
									// �ش� ������ ĳ���Ͱ� �Ҽӵ� ��� ������ ������ ��� 
									// ����ǰ ���� ó��
									else if (TRUE == GetVillageOurGuild(pIOnlineObject->GetVillageCode((UI16)pMousePointInfo->GetParam())))
									{
										// �ڽ��� ĳ���Ͱ� ���� ����(����ǰ �����ߴٰ� ���� �� �� �ִ�)�� ���
										if (KI[pIOnlineChar->GetImjinKind( pMyData->GetMyCharUniqueID() )].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
										{
											SI16	siVillagePosX,siVillagePosY;

											// ���� ��ġ ���
											pIOnlineObject->GetPos((UI16)pMousePointInfo->GetParam(), &siVillagePosX, &siVillagePosY);

											// ������ ���� ���� ����ǰ ���� ��û�ϱ�
											pOnlineCharUI->SetRequestSupplyGoodsFromVillage(siVillagePosX, siVillagePosY);
										}
										else
										{
											// ���� ������ �ƴҰ��
											// ���� �Ұ��� �޽��� ����
											pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
										}										
									}
									else
									{
										// ���� ������ �ƴҰ��
										// ���� �Ұ��� �޽��� ����
										pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
									}
								}	// else [ if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam())))) ]
							}	// if((m_RealTime.uiHour >= SIEGEWARFARETIME_BEGIN) && (m_RealTime.uiHour < SIEGEWARFARETIME_END))
							else
							{
								char szErrorMsg[256];
								sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_VILLAGE_ATTACK_TIME), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);
								pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);												
							}
						}
					}
					*/
				}
				break;
		}
	}
}
