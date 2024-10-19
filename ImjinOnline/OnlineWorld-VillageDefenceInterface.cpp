//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// 공성전 수성하기 인터페이스 관련 OnlineWorld-VillageDefenceInterface.cpp (캐릭터가 필드에 있는 것처럼 처리한다. 단, 필드상황을 볼 수만 있다.)
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
//#include	<OnlineBasicTip.h>			// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
//#include	<OnlineBasicQuest.h>		// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
#include	<OnlineParty.h>
#include	<OnlineFollower.h>
#include	<IProfit.h>
#include	<OnlineDungeonParser.h>
#include	<OnlineTicket-Parser.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
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
//#include    "OnlineBanner.h"			// actdoll (2004/02/25 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
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
#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME 컨트롤러 세팅

#include "../SCCommon/OnlineCommonParser.h"	// Using Online Common Parser

#include	"FieldHeroInfoMgr.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

#include	"SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"

#include	<time.h>	// mktime()

//준석이형 추가부분 
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

// sagolboss 추가부분 ->
#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif
// <- 추가 끝 

extern _clGame* pGame;
extern	_MainData				GD;									// 게임의 주데이터 
extern _InputDevice				IpD;
extern	SoundManager			*g_pSoundManager;
extern	CCharFileLoadManager	g_FileLoadManager;					// 조합형 캐릭터
extern CItem					*g_pItem;
extern char						LogInID[128];

extern clock_t					CurrentClock, MonitorMacroClock1, MonitorMacroClock2;

extern int WindowModeSwitch;
extern int bActive;

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 

// robypark 2005/1/7 18:41
// 수치(10%단위로)를 게이지 형태로 그려준다.
// 공성전 유닛 체력, 보급 게이지에서 사용
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

// 디버그를 위한 전역변수.
// HQDebug  GHQDebugTool;

// actdoll (2004/08/27 0:34) : 입력기 상태 보는 디버그 창(_DEBUG시만 가동. 없앨려면 주석달것)
//#define _SHOW_INPUTSTATUS_

// 수성하기 인터페이스 메인 폴
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

	// 다음 프레임을 진행할 수 있는 상황이 될때까지 대기한다.
	dwPrevFrameStartTime			-=	dwCurrentFrameAddTime;
	dwCurrentFrameAddTime			=	0;
	dwPrevFrameTimeRequire			=	timeGetTime() - dwPrevFrameStartTime;

	// 초당 프레임수를 맞춘다.
	if(dwPrevFrameTimeRequire < CPF)
	{
		Sleep(1);
		return uiNextMenu;
	}
	else
	{
		dwCurrentFrameAddTime	=	dwPrevFrameTimeRequire - CPF;
	}

	// 만약 컴퓨터가 딸리면 Render를 하지 않는다.
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
			// 겜임폴 접속시 설정했던것 해제하기
			pOnlineSetting->GameInit();

			// 로그인 화면으로
			SetMainPollStatus(ON_MAINPOLL_LOGINMGR);
			SetLogOut(0, ON_MAINPOLL_LOGINMGR);
			m_WaitTickCount = 0;
			return uiNextMenu;
		}
	}
	////////////////////////////////////////////////////
	// 프레임을 증가 시킨다.
	uiTotalFrame++;


	siFPS	=	1000 / (timeGetTime() - dwPrevFrameStartTime);

	// 수성하기에서 마을 반격 기능 사용 지연시간 갱신
	m_sVillageDefenceInterface.SetPastDelayTimeUsingVillageAttackType(dwPrevFrameTimeRequire);

	if (pOnlinePannel->GetSiegeWarfareUnitUIHandle())
	{
		pOnlinePannel->GetSiegeWarfareUnitUIHandle()->
			SetCurrentDelayTimeUsingVillageAttackType(
				m_sVillageDefenceInterface.m_siCurrentDelayTimeUsingVillageAttackType);
	}

	// 현재 시간을 얻어온다.
	dwCurrentFrameStartTime		=	timeGetTime();		

	// 위에서 FrameSkipping 때문에 Up Check를 무시하고 지나갈 경우가 있기때문에 여기서만 다시 검사

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

	DWriteStringToFile("시작 2");

	// 오버 헤드	
	pIOnlineMouse->Action();
	pOnlineMousePoint->Action();

	// 키보드의 입력상태를 얻어온다.
	pOnlineKeyboard->Action();

	//준석이형 추가부분 
	// 모든 윈도 관리자
//	pJSWndMgr->Action();
	
	ptMouse.x	=	IpD.Mouse_X;
	ptMouse.y	=	IpD.Mouse_Y;

	pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

	// 마을 정보 처리
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
		// 현재 지역을 체크하여 그 지역의 배경음악을 설정한다.
		if(pOnlineMap->m_MapIndex == 0)
		{
			SetBackGroundMusic(CheckHeroZone());
		}
	}

	// 이모티콘을 처리한다.
	pOnlineImoticon->Pool(fLeftUpSwitch);

	//if(pOnlineVillage == NULL)
	{
		// 마우스 있는 위치에 적 캐릭터가 있으면 마우스를 변경한다.
		// 단, 필드 영역이여야 한다.	
		pOnlineHelp->Initialize();

		pOnlineMsgBoxPay->Process();
		if(pOnlineMsgBoxError->Process() == ON_MSGBOX_RETURN_OK)
		{
			pOnlineClientChatting->Close();
			return ON_MAINPOLL_FORCED_LOGOUT;
		}
	}
	
	DWriteStringToFile("시작 3");

	// robypark 2005/1/26 18:33
	// 마우스 커서 아래 오브젝트 처리(풍선 도움말등을 위한)
	ProcessFieldMouseMove(TRUE, ptMouse);

	// 기본 마우스로 세팅한다.
//	pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);
/*
	// robypark 2004/12/21 16:39
	// 공성전 중에 일반 캐릭터 워프(각 국가의 초보자 수련장) 시키기
	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if(fRightOneSwitch == 1)	// 마우스 오른쪽 버튼 클릭
			{
				if( pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					switch(pMousePointInfo->GetPointKind())
					{
						case ON_MOUSE_POINT_CHAR:
						{
							UI16 uiUniqueID_UnderPoint = (UI16)pMousePointInfo->GetParam();

							if ( ( TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint) )	// 공성전 유닛이거나,
								|| ( FALSE == pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind(uiUniqueID_UnderPoint) ) )	// 플레이어가 아닌 몬스터이거나,
								|| ( TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiUniqueID_UnderPoint) ) 	// 수성 인터페이스에 있는 캐릭터가 이거나,
								|| ( TRUE == pIOnlineChar->GetBattleFlag(uiUniqueID_UnderPoint) ) )		// 전투 대기중인 캐릭터가 이거나,
							{
								// 워프 시킬 수 없다.
								break;
							}

							// 공성전 시간이라면
							if (IsInTimeSiegeWarfare())
							{
								// 같은 상단원이 아니라면
								if (FALSE == IsMemberOfSameGuild(uiUniqueID_UnderPoint))
								{
									// 워프 요청
									// 이 요청 응답은 기존의 운영자 명령어인 /w(워프)의 것을 사용한다.(ON_RESPONSE_WARP)
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
	// 단축키를 처리한다.
	FieldShortCutKey();	

//	if(pOnlineVillage == NULL)
	{
//		m_pNpcTalk->Action();

		m_pQuestInterface->Action();

		m_pChatMacro->Action();

		pOnlineParty->Action();

		// 판넬
		pOnlinePannel->Poll();

		// 인벤토리.
		pOnlineInventory->SetFlagVillage(FALSE);
		pOnlineInventory->Action();

		// 거래창
		pOnlineTrade->Action();

		// 장부
		pOnlineTradeBook->Poll();


		// 좌판
		pOnlineBooth->ActionBench();

		// 팁
		pOnlineTip->Action();

		// Quest Play
		pOnlineQuestPlay->Poll();

//		//초보자 도움말.
//		pOnlineBasicTip->Action();

//		pOnlineBasicQuest->Action();		

		// 경매 
		pOnlineFieldAuction->Process();

		// 접수
		pOnlineReception->Process();

		// 대장장이
		pOnlineSmith->Poll();

//		pJSTestWnd->Action();

		// 아이템 획득 창 관련 행동을 한다. 
		pGame->pItemInfoWindow->Prepare();

		// 메신저
		pOnlineMessenger->Action();

		m_pMercenary->Action();

		if( bFindTrade )
			FindTradeBook();

		// 미니맵
		pOnlineMiniMap->Pool(FALSE);

		// 내 캐릭터의 명령을 처리한다.(이동, 공격, 마을 들어감, 아이템 줍기)
		ProcessMyCharOrder();
	}
		
	// 보낼 메시지가 있으면 보내준다.
	RPI.Send();

	// Resource 관리
//	if( timeGetTime() - m_ImageFreeTimer > 5000 )
	{
		pOnlineResource->Prepare();
//		FreeOldImageByTime();

//		m_ImageFreeTimer = timeGetTime();
	}

	// 오버헤드 End
	// 오브젝트 액션.	
	pIOnlineObject->Action();	

	// 캐릭터 액션.		
	pIOnlineChar->Action();		

	// 따라다니는 용병들 처리
	pIOnlineChar->ActionByVMercenary();

	pOnlineMap->Action();

	// robypark 2004/10/25 13:28
	// 공성전 전투 관련 효과 처리
	m_SiegeWarfare_BulletMgr.Action();
	m_SiegeWarfare_SmokeMgr.Action();

	//퀘스트 아이콘을 출력한다.
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
	// 화면에 그린다.	

	pOnlineFieldEffect->Pool();

	
	DWriteStringToFile("그리기 1");
	if(fRender)
	{
		VillageDefenceInterfaceDraw();	
	}
	DWriteStringToFile("그리기 2");
	

	if( bExitOnlineGame )
	{
		siMainPollStatus = ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}

	DWriteStringToFile("끝");

	return uiNextMenu;
}

// 수성하기 인터페이 그리기
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
		// 타일, 캐릭터, 오브젝트를 그린다.	
		pOnlineMap->Draw(GETSCREENSURFACE(this));	
		
		// robypark 2004/10/25 13:27
		// 공성전 전투 효과 그리기
		m_SiegeWarfare_BulletMgr.Draw();
		m_SiegeWarfare_SmokeMgr.Draw();

		pOnlineFieldEffect->Render(GETSCREENSURFACE(this));

		pOnlineMousePoint->Draw();

		pIOnlineChar->AllDrawSpeech(GETSCREENSURFACE(this));

		// NPC 말하기
		m_pNpcTalk->Action();
		m_pNpcTalk->Draw(GETSCREENSURFACE(this));

		// Macro
		m_pChatMacro->Draw(GETSCREENSURFACE(this));
		
		// robypark 2004/11/13 16:37
		// 공성전 유닛 정보 창들을 모두 표시하는 경우 처리
		if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		{
			// HDC를 이용하여 찍어야 할 캐릭터에 대한 정보를 찍어준다.
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				SetBkMode(hdc, TRANSPARENT);

				hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
				SetTextColor(hdc, RGB(0, 0, 0));
		
				// 모든 공성전 유닛의 캐릭터명 그리기
				pIOnlineChar->AllDrawName(hdc);

				SelectObject(hdc, hOldFont);
				GETSCREENSURFACE(this)->ReleaseDC(hdc);

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// 모든 공성전 유닛 체력 및 보급 게이지 그리기
					pIOnlineChar->AllDrawSiegeWarfareUnit_StatusBar();

					// 모든 공성 유닛 상단 깃발 그리기
					pIOnlineChar->AllDrawSiegeWarfareUnit_GuildFlag();

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}	// if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)

		// 현재 마우스가 캐릭터를 가르키고 있으면
		if(m_SelectCharacterIndex != -1)
		{
			// 공성전 유닛일 경우 체력 게이지 출력하기
			if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
			{
				// HDC를 이용하여 찍어야 할 캐릭터에 대한 정보를 찍어준다.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// 공성전 유닛 캐릭터명 그리기
					pIOnlineChar->DrawName(m_SelectCharacterIndex, hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);
				}

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// 모든 공성전 유닛 체력 및 보급 게이지 그리기
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(m_SelectCharacterIndex);

					// 모든 공성 유닛 상단 깃발 그리기
					pIOnlineChar->DrawGuildFlag(m_SelectCharacterIndex);

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}
		}

		// HDC를 이용하여 찍어야 할 캐릭터에 대한 정보를 찍어준다.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);

			hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
			SetTextColor(hdc, RGB(0, 0, 0));
			
			pIOnlineChar->DrawName(pMyData->GetMyCharUniqueID(), hdc);		

			SelectObject(hdc, hOldFont);
			GETSCREENSURFACE(this)->ReleaseDC(hdc);

			// robypark 2004/11/12 21:28
			// 공성전 유닛 체력 및 보급 게이지 그리기
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// 공성전 유닛 체력 및 보급 게이지 그리기
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(pMyData->GetMyCharUniqueID());

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

		}	
		
		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// 상단 깃발을 찍는다.
			pIOnlineChar->DrawGuildFlag(pMyData->GetMyCharUniqueID());

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	
	}

	// 필드에서 건물 작업을 하고 있을때
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
				// 마을 간판
				clGrp.PutSpriteT(-1 + SignStartPos, 0, lpBoard->Header.Xsize, lpBoard->Header.Ysize, lpBoard->Image);

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

			// 마을 이름을 출력해준다.
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
		// 일반 상태일때
		// 미니맵 창이 활성화 되어 있다면 출력~~
		if(pOnlineMiniMap->IsActive())
			pOnlineMiniMap->Draw(GETSCREENSURFACE(this));

		pOnlineParty->Draw(GETSCREENSURFACE(this));

		pOnlineTradeBook->Draw( GETSCREENSURFACE(this) );

		// 경매접수창 
		pOnlineReception->Draw(GETSCREENSURFACE(this));

		// 대장장이
		pOnlineSmith->Draw(GETSCREENSURFACE(this));

		// 판넬을 그린다.
		pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_MINIMAP/*mini map*/ | ONLINEPANNEL_DISABLEFLAG_TIP/*Quest*/ | ONLINEPANNEL_DISABLEFLAG_OPTION/*system*/ | ONLINEPANNEL_DISABLEFLAG_IMOTICON/*imoticon*/ | ONLINEPANNEL_DISABLEFLAG_CHARINFO/*mercenary*/);
		pOnlinePannel->Draw(GETSCREENSURFACE(this));

		// 거래창이 활성화 되어 있따면
		if( pOnlineTrade->IsActive() )
		{	
			// 내 인벤토리에서 아이템이 드래그 중이라면 거래창을 먼저 찍어 주고 인벤토리를 찍어 준다.
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
			// 용병 관련 출력			
			m_pMercenary->Draw(GETSCREENSURFACE(this));

			// 좌판
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

		// 이모티콘을 출력한다.
		pOnlineImoticon->Draw(GETSCREENSURFACE(this));	

		// 화면으로 옮긴다.
		// 정보를 화면에 찍어준다.
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

					// 현재 Tile Buffer 사용률
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

					// 현재 선택한 개체 출력
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					switch(pMousePointInfo->GetPointKind())
					{
					case ON_MOUSE_POINT_CHAR:
						// 선택한 캐릭터 표시
						if(pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam())))
						{
							// Player인지

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

								// 길드원일 경우

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
									// 길드 깃발이 있는경우

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d, Flag : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								}
								else
								{
									// 길드 깃발이 없는경우

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()));
								}
							}
							else
							{
								// 길드원이 아닐경우

								sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\"", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()));
							}
						}
						else
						{
							// Monster인지

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

					// 원하는 자판의 Data를 얻어온다.
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

		// 경매창 
		pOnlineFieldAuction->Draw(GETSCREENSURFACE(this));

		POINT pt;
		pt.x	= IpD.Mouse_X;
		pt.y	= IpD.Mouse_Y;


		// 도움말 출력
		SI16 siDrawedHelpPosX = 0;
		SI16 siDrawedHelpPosY = 0;
		int helpx = pOnlineHelp->Show(GETSCREENSURFACE(this), &siDrawedHelpPosX, &siDrawedHelpPosY);

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			XSPR*		lpXSPR;

			// 현재 마우스가 캐릭터를 가르키고 있으면
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
				// 공성전 유닛일 경우 체력, 보급품 게이지 출력하기
				if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
				{
					// 공성전 유닛 체력 게이지만 출력
					XSPR	*psprGaugeHealth = NULL;

					// 사용자와 같은 상단인가?
					if (IsMemberOfSameGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
					}
					// 적대 상단원인가?
					else if (pIOnlineChar->GetIsHostilityGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
					}
					else	// 그 밖의 경우 안그린다.
						psprGaugeHealth = NULL;

					if (psprGaugeHealth)
					{
						// 그리기
						DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
												siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
												m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct + 1,
												psprGaugeHealth);
					}

					// 공성전 유닛 보급게이지를 출력해야 하는 경우라면
					if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods)
					{
						// 공성전 유닛 보급품 게이지 출력
						XSPR	*psprGaugeSupplyGoodsAmount = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

						if (psprGaugeSupplyGoodsAmount)
						{
							// 그리기
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

		// 메세지 박스 출력
		pOnlineMsgBox->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxUsePortal->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxError->Draw(GETSCREENSURFACE(this));

		// TIP 출력
		pOnlineTip->Draw(GETSCREENSURFACE(this));

		// 메신저 출력
		pOnlineMessenger->Draw(GETSCREENSURFACE(this));

		// 숫자판 드로우
		pOnlineNumPad->pNumberControl->Draw( GETSCREENSURFACE(this) );

		// 환경설정 드로우
		pOnlineSetting->Draw(GETSCREENSURFACE(this));

		//준석이형 추가부분 
		// 모든 윈도들을 드로우
//		pJSWndMgr->Draw(GETSCREENSURFACE(this));

		pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(this));

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// 마우스를 찍는다.
			pIOnlineMouse->Draw();			

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}
		
		// for debug		

		UpdateScreen(GETSCREENSURFACE(this));
	}

	YetTempTickCount = GetTickCount() - TempTickCount;
}

// 수성하기 인터페이스 초기화
VOID	cltOnlineWorld::InitVillageDefenceInterface(VOID)
{
	// 수성 인터페이스 사용 중에 일부 기능을 사용할 수 없다.
	pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_MINIMAP/*mini map*/ | ONLINEPANNEL_DISABLEFLAG_TIP/*Quest*/ | ONLINEPANNEL_DISABLEFLAG_OPTION/*system*/ | ONLINEPANNEL_DISABLEFLAG_IMOTICON/*imoticon*/ | ONLINEPANNEL_DISABLEFLAG_CHARINFO/*mercenary*/);

	// 열려 있는 창들 닫기 //////////////////////////////////////
	// 용병창
	if (m_pMercenary->IsAction())
		m_pMercenary->OnButtonPush();

	// 인벤토리가 열려 있으면 닫아준다.
	if(pOnlineInventory->IsActive())
		pOnlineInventory->SetActive(FALSE);
	
	// 장부가 열려 있으면 닫아준다.
	if(pOnlineTradeBook->IsAction())
		pOnlineTradeBook->SetAction(FALSE);

	// 미니맵도 닫는다!!
	if(pOnlineMiniMap->IsActive())
		pOnlineMiniMap->SetActive(FALSE);

	// 시스템 메뉴(옵션) 닫기
	if(pOnlineSetting->IsAction())
		pOnlineSetting->SetAction(FALSE);
	///////////////////////////////////////////////////////////
}

// 수성하기 인터페이스 해제
VOID	cltOnlineWorld::FreeVillageDefenceInterface(VOID)
{
	// 인터페이스 부분 No Field Area 해제
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

// 수성하기 인터페이스로 들어가기 요청에 대한 응답 처리
// ON_RESPONSE_WAR_VILLAGE_DEFENCE
UI16	cltOnlineWorld::OnResponseEnterVillageDefenceInterface(BYTE *pMsg)
{
	OnResWarVillageDefence	*pOnResWarVillageDefence = (OnResWarVillageDefence *)pMsg;

	// 수성하기 인터페이스에 들어갈 수 있다.
	if (ON_RET_OK == pOnResWarVillageDefence->dwResponse)
	{
		// 수성 인터페이스에 들어간 사용자가 자신이라면
		if (pMyData->GetMyCharUniqueID() == pOnResWarVillageDefence->uiUniqueID)
		{
			// 인터페이스 리소스 초기화 및 로드
			InitVillageDefenceInterface();

			// 현재 수성하기 인터페이스에 있다고 설정
			pMyData->SetInVillageDefenceInterface(TRUE);

			// 수성하기 Poll 설정
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

			// 마을 안으로 들어간다.	Village Code만 저장									
			pMyData->SetGotoVillage(FALSE, m_sVillageDefenceInterface.m_uiVillageCode);

			/*
			// 마을 정보를 얻어온다.
			VillageHeader*	lpVillageHeader = pOnlineVillageManager->GetVillageParser()->
												GetVillageHeaderCode(m_sVillageDefenceInterface.m_uiVillageCode);

			if (lpVillageHeader)
			{
				// 마을의 위치를 기준으로 맵을 설정한다.
				pOnlineMap->SetXY(lpVillageHeader->siX, lpVillageHeader->siY);
				pOnlineMap->SetCurrentFocusCharID(pMyData->GetMyCharUniqueID());
			}
			*/
		}
		else	// 다른 사용자라면 
		{
		}

		// 수성인터페이스에 들어갔다고 캐릭터 설정
		pIOnlineChar->SetInVillageDefenceInterface(pOnResWarVillageDefence->uiUniqueID, TRUE);
	}
	// 수성하기 인터페이스에 이미 다른 사용자(상단원)가 들어가 있다.
	else if (ON_RET_GUILD_VILLAGE_OFF_LIMITS == pOnResWarVillageDefence->dwResponse)
	{
		// 수성인터페이스를 거치지 않고 바로 마을로 들어간다.
		// 마을 안으로 들어간다.										
		pMyData->SetGotoVillage(FALSE, m_sVillageDefenceInterface.m_uiVillageCode);
		// 서버에게 마을로 들어간다고 요청한다.		
		pOnlineClient->SendGotoVillage(m_sVillageDefenceInterface.m_uiVillageCode);

		// 수성하기 인터페이스에 들어갈 수없다는 메시지 출력
		SetMessage((char*)pOnlineText->Get(ON_TEXT_ALREADY_USED_VILLAGE_DEFENCE_INTERFACE));
		pOnlinePannel->AddChat((char*)pOnlineText->Get(ON_TEXT_ALREADY_USED_VILLAGE_DEFENCE_INTERFACE), ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);

		m_sVillageDefenceInterface.m_uiVillageCode = 0;
	}
	// 그 밖의 에러들
	else
	{
		// 이미 사용자가 수성인터페이스에 있는 경우 return; 
		// 간혹 요청메시지를 여러차례보내는 경우 문제가 된다.
		if (pMyData->IsInVillageDefenceInterface())
			return 0;

		// 더 이상 서버로 부터 마을에 들어간다는 메시지를 기다리지 않도록 설정한다.
		pMyData->SetWaitGotoVillageResponse(FALSE);		
			
		// 마을에 들어간다는 변수를 초기화 한다.
		pMyData->SetGotoVillage(FALSE, 0);
		
		// Field Poll 설정
		SetGamePollStatus( ON_GAMEPOLL_FIELD );

		pOnlineCharUI->SetAction(TRUE);
		return 0;

		// 수성인터페이스를 거치지 않고 바로 마을로 들어간다.
		// 마을 안으로 들어간다.										
		pMyData->SetGotoVillage(FALSE, m_sVillageDefenceInterface.m_uiVillageCode);

		// 서버에게 마을로 들어간다고 요청한다.		
		pOnlineClient->SendGotoVillage(m_sVillageDefenceInterface.m_uiVillageCode);

		m_sVillageDefenceInterface.m_uiVillageCode = 0;
	}

	return 0;
}

/*
// 수성하기 인터페이스에서 마을로 들어가기 요청에 대한 응답 처리
// ON_RESPONSE_WAR_VILLAGE_IN
UI16	cltOnlineWorld::OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg)
{
	OnResWarVillageIn	*pOnResWarVillageIn = (OnResWarVillageIn *)pMsg;

	// 수성하기 인터페이스에서 마을로 들어가는 캐릭터가 사용자인 경우
	if (pMyData->GetMyCharUniqueID() == pOnResWarVillageIn->uiUniqueID)
	{
		// 수성하기 인터페이스 리소스 해제
		FreeVillageDefenceInterface();

		// 수성인터페이스에서 마을로 들어가는 경우에 모든 공성 유닛 정보 보기 옵션이 활성화된 경우 다시 비활성화하기.
		// 공성전 유닛이 아닌 상태에서 마을로 들어간 경우에만.
		if ((m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT) 
			&& (0 == (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)))
		{
			m_scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
		}

		UI16 uiVillageCode;

		// 인구 처리
		OnRecvVillageInfo*		lpOnRecvVillageInfo = (OnRecvVillageInfo*)((UI08*)(pOnResWarVillageIn) + sizeof(OnResWarVillageIn));

		// 서버로 부터 마을에 들어갈 수 있또록 허가를 받았다.

		// 빌리지 객체를 생성한다.
		pOnlineVillage	=	new OnlineVillage;
			
		// 마을로 이동해야 한다.
		uiVillageCode	=	pMyData->GetGotoVillage();						
			
		pOnlineVillage->Init(uiVillageCode, lpOnRecvVillageInfo, this);

		SetGamePollStatus( ON_GAMEPOLL_VILLAGE );
			
		// 모든 캐릭터들을 없앤다.
		pIOnlineChar->AllFree();
			
		// 오브젝트를 모두 해제한다.
		pIOnlineObject->FreeAll();

		// 더 이상 서버로 부터 마을에 들어간다는 메시지를 기다리지 않도록 설정한다.
		pMyData->SetWaitGotoVillageResponse(FALSE);		
			
		// 마을에 들어간다는 변수를 초기화 한다.
		pMyData->SetGotoVillage(FALSE, 0);			

		pOnlineClientChatting->MessageSend_NotIsMap();

		if(m_VillageInStruct != -1)
		{
			pOnlineVillage->MoveInStruct(GetVillageInStruct(), TRUE, TRUE);
		}
	}
	// 수성하기 인터페이스에서 마을로 들어가는 캐릭터가 다른 사용자인 경우
	else
		pIOnlineChar->Free(pOnResWarVillageIn->uiUniqueID);	// 캐릭터 삭제

	return 0;
}
*/

// 수성하기 인터페이스에 나온다. 필드로
// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
UI16	cltOnlineWorld::OnResponseLeaveVillageDefenceInterface(BYTE *pMsg)
{
	OnResWarVillageIn	*pOnResWarVillageIn = (OnResWarVillageIn *)pMsg;

	// 나가기 실패한 경우
	if (ON_RET_OK != pOnResWarVillageIn->dwResponse)
		return 0;

	// 수성하기 인터페이스에서 마을로 들어가는 캐릭터가 사용자인 경우
	if (pMyData->GetMyCharUniqueID() == pOnResWarVillageIn->uiUniqueID)
	{
		// 수성하기 인터페이스 리소스 해제
		FreeVillageDefenceInterface();

		// 더 이상 서버로 부터 마을에 들어간다는 메시지를 기다리지 않도록 설정한다.
		pMyData->SetWaitGotoVillageResponse(FALSE);		
			
		// 마을에 들어간다는 변수를 초기화 한다.
		pMyData->SetGotoVillage(FALSE, 0);

		// 수성 인터페이스에서 나왔다고 데이터들 초기화
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
		
		// Field Poll 설정
		SetGamePollStatus( ON_GAMEPOLL_FIELD );

		pOnlineCharUI->SetAction(TRUE);
	}

	pIOnlineChar->SetInVillageDefenceInterface(pOnResWarVillageIn->uiUniqueID, FALSE);

	return 0;
}

// 수성하기 인터페이스에서 공격 속성 등의 초기화 정보 얻기
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
// 공성전 시간인지를 검사한다.
BOOL	cltOnlineWorld::IsInTimeSiegeWarfare(void)
{
	// 공성하는 날인가?
	if (IsInDaySiegeWarfare())
	{
		if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN)
				&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END))
		{
			// 공성 시간이다.
			return TRUE;
		}
	}

	// 공성 시간이 아니다.
	return FALSE;
}

// 공성전 대방이 소집권 아이템 변환(생산) 시간인지를 검사
BOOL	cltOnlineWorld::IsInTimeRecruitArmyItem(void)
{
	/*
	if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_RECRUITARMYITEM_BEGIN)
			&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siSIEGEWARFARETIME_RECRUITARMYITEM_END))
	{
		// 소집권 아이템 변환 가능 시간이다.
		return TRUE;
	}
	*/

	// 불가능한 시간이다.
	return FALSE;
}

// 공성전 대방이 소집권 아이템을 상단원에게 나줘 줄 수 있는 시간인지를 검사
BOOL	cltOnlineWorld::IsInTimeGiveArmyItem(void)
{
	/*
	if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siGiveArmyItem_BeginTime)
			&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siGiveArmyItem_EndTime))
	{
		// 소집권 아이템을 상단원에게 나줘 줄 수 있는 시간이다.
		return TRUE;
	}
	*/

	// 불가능한 시간이다.
	return FALSE;
}

// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 불가능 시간인지 검사
BOOL	cltOnlineWorld::IsInTimeDisabledRecruitArmyInGovernment(void)
{
	// 공성하는 날인가?
	if (IsInDaySiegeWarfare())
	{
		if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_Begin)
				&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_End))
		{
			// 관청에서 병력모집 및 보급품 생산이 불가능한 시간이다.
			return TRUE;
		}
	}

	// 가능한 시간이다.
	return FALSE;
}

// 사용자들이 상단 가입이 불가능한 시간인지를 검사
BOOL	cltOnlineWorld::IsInTimeDisabledTimeToJoin(void)
{
	// 공성하는 날인가?
	if (IsInDaySiegeWarfare())
	{
		if ((m_RealTime.uiHour >= m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_Begin)
				&& (m_RealTime.uiHour < m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_End))
		{
			// 상단 가입 신청 및 가입 허가가 불가능한 시간이다
			return TRUE;
		}
	}

	// 가능한 시간이다.
	return FALSE;
}

// 사용자들이 상단에서 탈퇴가 불가능한 시간(대방이 상단원 해고도 불가능하다) 인지 검사
BOOL	cltOnlineWorld::IsInTimeDisabledTimeToWithdraw(void)
{
	// 현재 시간
	UI08 uiHour = m_RealTime.uiHour;

	// 자정(밤 12시)은 24로 변경하여 처리한다.
	if (0 == m_RealTime.uiHour)
		uiHour = 24;

	// 공성하는 날인가?
	if (IsInDaySiegeWarfare())
	{
		if ((uiHour >= m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_Begin)
				&& (uiHour < m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_End))
		{
			// 상단에서 탈퇴가 불가능한 시간이다.
			return TRUE;
		}
	}

	// 가능한 시간이다.
	return FALSE;
}

// 폭탄투자 권한 처리 시간인지 검사
BOOL	cltOnlineWorld::IsInTimeAllInInvestment(void)
{
	// 현재 시간
	UI08 uiHour = m_RealTime.uiHour;

	// 자정(밤 12시)은 24로 변경하여 처리한다.
	if (0 == m_RealTime.uiHour)
		uiHour = 24;

	// 공성하는 날인가?
	if (IsInDaySiegeWarfare())
	{
		if ((uiHour >= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_Begin)
				&& (uiHour < m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End))
		{
			// 대방이 상단원들에게 폭탄투자 권한을 줄 수 있는 시간 혹은 마을에 폭탄 투자할 수 있는 시간이다.
			return TRUE;
		}
	}

	// 불가능한 시간이다.
	return FALSE;
}

// robypark 2005/1/31 16:53
// 공성하는 날인지 검사
BOOL	cltOnlineWorld::IsInDaySiegeWarfare(void)
{
    struct tm tmToday;
	ZeroMemory(&tmToday, sizeof(tmToday));

	tmToday.tm_min	= m_RealTime.uiMinute;			/* minutes after the hour - [0,59] */
	tmToday.tm_hour	= m_RealTime.uiHour;			/* hours since midnight - [0,23] */
	tmToday.tm_mday	= m_RealTime.uiDay;				/* day of the month - [1,31] */
	tmToday.tm_mon	= m_RealTime.uiMonth - 1;		/* months since January - [0,11] */
	tmToday.tm_year	= m_RealTime.uiYear - 1900;		/* years since 1900 */

	// 오늘이 무슨 요일인지 알아 내기
    if( mktime( &tmToday ) == (time_t)-1 )
		return FALSE;	// 요일 변환 실패

	// 공성 요일 리스트 검사
	for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
	{
		if (-1 != m_SOptionSiegeWarfare.scSiegeDay[i])
		{
			if (tmToday.tm_wday == m_SOptionSiegeWarfare.scSiegeDay[i])
			{
				// 오늘은 공성하는 날이다.
				return TRUE;
			}
		}
	}

	// 오늘은 공성하는 날이 아니다.
	return FALSE;
}

// 사용자와 같은 상단의 상단원인지를 검사
BOOL	cltOnlineWorld::IsMemberOfSameGuild(UI16 uiDestUniqueID)
{
	if (pMyData->GetGuildName())	// 사용자가 상단에 가입했으며,
	{
		if (NULL == pIOnlineChar->GetGuildName(uiDestUniqueID))	// 상대방 캐릭터가 상단에 가입하지 않은 경우
			return FALSE;
		else
		{
			// 사용자가 상단 지원자라면
			if (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) == pMyData->GetGuildClass())
				return FALSE;

			// 같은 상단원이 아니며
			if (0 != strcmp(pMyData->GetGuildName(), pIOnlineChar->GetGuildName(uiDestUniqueID)))
				return FALSE;
			else
			{
				// 상단 지원자인지 비교
				if (pIOnlineChar->GetGuildInGrade(uiDestUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
					return FALSE;
				else
					return TRUE;	// 같은 상단원이다.
			}
		}
	}

	return FALSE;
}

// 사용자와 같은 상단의 상단원인지를 검사
// char *pszGuildName:	상대방 상단 명
// SI32 siClassInGuild: 상대방 상단내 직급
BOOL	cltOnlineWorld::IsMemberOfSameGuild(char *pszGuildName, SI32 siClassInGuild)
{
	if (pMyData->GetGuildName())	// 사용자가 상단에 가입했으며,
	{
		if (NULL == pszGuildName)	// 상대방 캐릭터가 상단에 가입하지 않은 경우
			return FALSE;
		else
		{
			// 사용자가 상단 지원자라면
			if (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) == pMyData->GetGuildClass())
				return FALSE;

			// 같은 상단원이 아니며
			if (0 != strcmp(pMyData->GetGuildName(), pszGuildName))
				return FALSE;
			else
			{
				// 상단 지원자인지 비교
				if (siClassInGuild == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
					return FALSE;
				else
					return TRUE;	// 같은 상단원이다.
			}
		}
	}

	return FALSE;
}

// 캐릭터 A와 캐릭터B가 같은 상단의 상단원인지를 검사
BOOL	cltOnlineWorld::IsMemberOfSameGuild(UI16 uiUniqueID, UI16 uiDestUniqueID)
{
	if (pIOnlineChar->GetGuildName(uiUniqueID))	// 캐릭터A가 상단에 가입했으며,
	{
		if (NULL == pIOnlineChar->GetGuildName(uiDestUniqueID))	// 캐릭터B가 상단에 가입하지 않은 경우
			return FALSE;
		else
		{
			// 캐릭터A가 상단 지원자라면
			if (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) == pIOnlineChar->GetGuildInGrade(uiUniqueID))
				return FALSE;

			// 캐릭터A와 캐릭터B가 같은 상단원이 아니며
			if (0 != strcmp(pIOnlineChar->GetGuildName(uiUniqueID), pIOnlineChar->GetGuildName(uiDestUniqueID)))
				return FALSE;
			else	// 캐릭터A와 캐릭터B가 같은 상단명이다.
			{
				// 캐릭터B가 상단 지원자인지 비교
				if (pIOnlineChar->GetGuildInGrade(uiDestUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
					return FALSE;
				else
					return TRUE;	// 같은 상단원이다.
			}
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

// robypark 2005/2/15 15:30
// 공성전 유닛이 치료가 가능한지 검사하여 마우스 커서 설정을 위한 마우스 타입 얻기
// UI16 uiTargetUniqueID: 피격자의 UniqueID
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanHeal(UI16 uiUniqueID)
{
	UI16 uiMyCharUniqueID = pMyData->GetMyCharUniqueID();

/*
	// 다른 유닛을 치료할 수 있는가?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiMyCharUniqueID)].IsAttribute2(ATB2_HEAL))
		return ON_MOUSE_BATTLE_SUB_NONE;
*/

	// 치료하기 위한 마나(보급품)가 있는가?
	if (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID) <= 0)
		return ON_MOUSE_BATTLE_SUB_NONE;

	// 해당 캐릭터가 치료 가능한가?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiUniqueID)].IsAttribute1(ATB_PATIENT))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// 해당 캐릭터가 부상당하였는가?
	if (pIOnlineChar->GetSiegeWarfare_MaximumHP(uiUniqueID) <= pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// 대상 캐릭터가 같은 상단원인가?
	if (FALSE == IsMemberOfSameGuild(uiUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// 치료 가능
	return ON_MOUSE_BATTLE_HEAL;
}

// robypark 2005/1/25 18:18
// 다른 공성전 유닛을 치료 가능한가?
BOOL	cltOnlineWorld::SiegeWarfare_CanHeal(UI16 uiUniqueID)
{
	UI16 uiMyCharUniqueID = pMyData->GetMyCharUniqueID();

	// 다른 유닛을 치료할 수 있는가?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiMyCharUniqueID)].IsAttribute2(ATB2_HEAL))
		return FALSE;

	// 치료하기 위한 마나(보급품)가 있는가?
	if (pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID) <= 0)
		return FALSE;

	// 해당 캐릭터가 치료 가능한가?
	if (FALSE == KI[pIOnlineChar->GetImjinKind(uiUniqueID)].IsAttribute1(ATB_PATIENT))
		return FALSE;

	// 해당 캐릭터가 부상당하였는가?
	if (pIOnlineChar->GetSiegeWarfare_MaximumHP(uiUniqueID) <= pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID))
		return FALSE;

	// 대상 캐릭터가 같은 상단원인가?
	if (FALSE == IsMemberOfSameGuild(uiUniqueID))
		return FALSE;

	// 치료 가능
	return TRUE;
}

// robypark 2005/1/26 18:22
// 필드에 있을 때 마우스 커서 아래 오브젝트들에 관한 처리(Mouse Move)
void	cltOnlineWorld::ProcessFieldMouseMove(BOOL bIsSiegeWarfareUnit, POINT ptMouse)
{
	CHAR			szMyCharSpeechBuffer[1024];
	CHAR			TempBuffer[1024];
	CHAR			TempBuffer1[1024];
	SI32			siSetMouse = ON_MOUSE_BASIC;
	bool			bEnabledCursor = true;		// 전투 마우스 일 경우 칼, 활, 힐과 같은 세부 커서 이미지를 이용하기 위함

	// robypark 2005/1/8 11:43
	// 공성전 유닛을 마우스 커서로 가리키고 있는 캐릭터의
	// 풍선 도움말(체력, 보급품 게이지) 표시할 때 좌표값 및 % 저장 구조체 초기화
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
				// 현재 마우스가 캐릭터 위에 있다.
				SI32		TempColor;
				XSPR*		lpXSPR;	// 상단 깃발 스프라이트
				BOOL		fFlag;	// 상단 깃발이 있는지

				// 현재 캐릭터의 정보가 있는지 검사

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
				// robypark 2004/12/24 15:33 적대 상단원 공성전 유닛일 경우 텍스트 칼라 변경
				else if(pIOnlineChar->IsSiegeWarfareUnit((UI16)pMousePointInfo->GetParam()))
				{
					if(pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()) && !m_bEvent)
					{
						TempColor = ON_HELP_WHITERED;
					}
				}

				// 커서 아래 캐릭터가 길드에 가입한 캐릭터인지 검사
				if(pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam()) && (m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())))
				{
					fFlag = FALSE;
					// 상단 깃발이 있는지 검사
					lpXSPR = pOnlineResource->GetxsprFlagM(pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
					if(lpXSPR)
					{
						if(lpXSPR->Image)
						{
							fFlag = TRUE;
						}
					}

					char *pPlayerID = pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam());

					// 캐릭터 이름 얻기
					if( pPlayerID )
					{
						if(m_bEvent)
						{
							if(!pIOnlineChar->IsInvalidBattle((UI16)pMousePointInfo->GetParam()))		TempColor = ON_HELP_RED;
							sprintf((char*)TempBuffer, "%s(%d)(%d)", pPlayerID, pIOnlineChar->GetRank((UI16)pMousePointInfo->GetParam()), pIOnlineChar->GetScore((UI16)pMousePointInfo->GetParam()));
						}
						else
						{
							// 사용자 캐릭터가 운영자 권한이 있을 경우 어카운트 표시
							if(m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)TempBuffer, "%s(%d)", pPlayerID, pIOnlineChar->GetFightPower((UI16)pMousePointInfo->GetParam()));
							else                                sprintf((char*)TempBuffer, "%s(%d)(%d)", pPlayerID, pIOnlineChar->GetFightPower((UI16)pMousePointInfo->GetParam()), (UI16)pMousePointInfo->GetParam());

							// 상단명과 직급
							sprintf((char*)TempBuffer1, "[%s:%s]", pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam()), pOnlineText->Get(m_cltIGuildClass.GetClassName(pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()))));
						}

						// robypark 2004/11/9 13:33
						// 마우스 커서 아래 캐릭터 정보 얻기
						SI16 siKindIdx = pIOnlineChar->GetImjinKind( (UI16)pMousePointInfo->GetParam() );
						SI16 siCharacterUniqueID = (UI16)pMousePointInfo->GetParam();

						// 마우스 커서 아래 캐릭터가 공성전 유닛인지 검사
						if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(siCharacterUniqueID))
						{
							if(fFlag)
							{
								// 상단 깃발 포함 설정
								pOnlineHelp->SetTextID((char*)TempBuffer1, (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &m_SelectCharacterNameDrawX, &m_SelectCharacterNameDrawY);
								m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
							}
							else
							{
								// 상단 깃발 없음
								pOnlineHelp->SetTextID((char*)TempBuffer1, (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor);
							}
						}
						// 공성전 유닛이다
						else
						{
							if(fFlag)
							{
								m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth = true;
								m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct = (pIOnlineChar->GetSiegeWarfare_Hp(siCharacterUniqueID) * 10)
																						/ (float)pIOnlineChar->GetSiegeWarfare_MaximumHP(siCharacterUniqueID);
								// 상단 깃발 포함 설정
								// 공성전 유닛일 경우 체력 및 보급품(보급수레만)을 추가로 표시하도록 한다.
								/*
								// 보급 수레인 경우
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
								// 마우스가 캐릭터를 가리키고 있다.
								m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
							}
							else
							{
								m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth = true;
								m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct = (pIOnlineChar->GetSiegeWarfare_Hp(siCharacterUniqueID) * 10)
																						/ (float)pIOnlineChar->GetSiegeWarfare_MaximumHP(siCharacterUniqueID);

								// 상단 깃발 없음
								// 공성전 유닛일 경우 체력 및 보급품(보급수레만)을 추가로 표시하도록 한다.
								/*
								// 보급 수레인 경우
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
								// 마우스가 캐릭터를 가리키고 있다.
								m_SelectCharacterIndex = (UI16)pMousePointInfo->GetParam();
							}
						}
					}
				}
				// 상단에 가입하지 않은 캐릭터를 가리키고 있는 경우
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
						// 마우스 커서 아래 캐릭터 정보 얻기
						// 마우스 커서 아래 캐릭터가 공성전 유닛인지 검사
						if (FALSE == IsSiegeWarfareUnit(siKindIdx))
							pOnlineHelp->SetText((char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_UP), TempColor );
						else
						{
							m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth = true;
							m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct = (pIOnlineChar->GetSiegeWarfare_Hp(siCharacterUniqueID) * 10)
																					/ (float)pIOnlineChar->GetSiegeWarfare_MaximumHP(siCharacterUniqueID);

							// 공성전 유닛일 경우 체력 및 보급품(보급수레만)을 추가로 표시하도록 한다.
							/*
							// 보급 수레인 경우
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

							// 마우스가 캐릭터를 가리키고 있다.
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
					// 자신이 공성전 유닛인 경우
					if (TRUE == bIsSiegeWarfareUnit)
					{
						UI16 uiTargetUniqueID = (UI16)pMousePointInfo->GetParam();

						// 상대 캐릭터가 공성전 유닛이 아니라면
						if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
						{
							break;
						}

						// 커서 아래 캐릭터 밝게
						pIOnlineChar->SetCursorHoverCharacter(uiTargetUniqueID, TRUE);

						// 다른 유닛을 치료할 수 있는가?
						if (FALSE == KI[pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID())].IsAttribute2(ATB2_HEAL))
						{
							// 마우스 아래 캐릭터를 공격 가능한가?
							SI16 ssSetMouse_BattleSub = SiegeWarfare_CheckCursorCanAttackUnit(uiTargetUniqueID);

							// 전투가 가능한지 검사
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
							// 치료가 가능 하지 않은 경우
							if (ON_MOUSE_BATTLE_HEAL != SiegeWarfare_CheckCursorCanHeal(uiTargetUniqueID))
								bEnabledCursor = false;

							siSetMouse	=	ON_MOUSE_BATTLE_HEAL;
						}
					}
					else	// 내가 일반 유닛
					{
						// 해당 캐릭터가 공성전 유닛이 아닌 경우
						if (FALSE == pIOnlineChar->IsSiegeWarfareUnit((UI16)pMousePointInfo->GetParam()))
						{
							// 무도장인 경우
							if(pOnlineMap->m_MapInfo.fWarFlag == 2)
							{
								if( !bPeace )
								{
									siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
								}
							}
							// 몬스터인 경우
							else if (FALSE == pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind( (UI16)pMousePointInfo->GetParam())))
							{
								siSetMouse	=	ON_MOUSE_BATTLE_SWORD;
							}
							else
							{
								// 전투모드
								if( !bPeace )
								{
									// 적대 상단원
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
				// 아이템 위에 있다
				UI32		uiItemID;
				SI16		siItemX, siItemY;
				CItemHeader *pItem;
				char*		lpBuffer;

				pMousePointInfo->GetPos(&siItemX, &siItemY);
				uiItemID		=	pMousePointInfo->GetParam();
				if(uiItemID == 0)
				{
					// 자판 Item일 경우

					MapStoreHeader*			lpMapStoreHeader;
					SI16					MPointFlag;
					SMapBoothData*			lpSMapBoothData;
	//				SMapBoothDescriptData*	lpSMapBoothDescriptData;				

					// 원하는 자판의 Data를 얻어온다.
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
					// 일반 Item일 경우
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

					// 오브젝트라도 우선 마을인지 알아와야 한다.
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
							// 마을 이름을 알아온다
							pIOnlineObject->GetName(SI16(pMousePointInfo->GetParam()), szVillageName);

							// 공격 가능한지 검사해서 마우스 커서 얻기
							SI16 ssSetMouse_BattleSub = SiegeWarfare_CheckCursorCanAttackVillage(SI16(pMousePointInfo->GetParam()));

							// robypark 2004/11/9 13:27
							// 적대 마을(공성하는)이며, 사용자가 마을 공격 가능한 공성전 유닛일 경우 마우스 커서 전투형으로 변경
							if ((GetVillageWarDayGuild(pIOnlineObject->GetVillageCode((UI16)pMousePointInfo->GetParam())))
								&& (ON_MOUSE_BATTLE_SUB_NONE != ssSetMouse_BattleSub))
							{
								pOnlineHelp->SetText(szVillageName, (SI16)ptMouse.x, (SI16)ptMouse.y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_RED);

								// 전투가 가능한지 검사
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

							// 마을 설명을 얻어온다.
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
						// 사용자 캐릭터가 수성 인터페이스에 있는가?
						if (FALSE == pIOnlineChar->IsInVillageDefenceInterface(pMyData->GetMyCharUniqueID()))
							siSetMouse	=	ON_MOUSE_GOVIL;		// 일반 마을 가기용 마우스 커서 설정
					}
					else
					{
						// 문화유산과 같은 오브젝트에 마우스 커서가 위치할 경우 오브젝트 설명 텍스트 표시하기
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
			// 기본 마우스로 세팅한다.
			pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);
		}
	}
	else
	{		
		// 기본 마우스로 세팅한다.
		pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);
	}
}

// robypark 2005/1/26 18:22
// 필드에 있을 때 마우스 커서 왼쪽 버튼 클릭 처리
void	cltOnlineWorld::ProcessFieldMouseLeftBtnClick(BOOL bIsSiegeWarfareUnit, SI32 siCursorX, SI32 siCursorY)
{
	SI16			siTX, siTY;

	// 공성전 공격 모드 설정 변수
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	switch(pMousePointInfo->GetPointKind())
	{
		case ON_MOUSE_POINT_CHAR:
			// 수성 인터페이스 들어가기 취소
			m_sVillageDefenceInterface.m_uiVillageCode = 0;

			if(bCharMove == TRUE)
			{
				// 너무 많은 양의 무게의 아이템을 들고 있으면 메세지를 뛰움
				if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
				{
					SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
				}

				// 선택한 캐릭터 표시
				pIOnlineChar->SetNewSelectedFlag((UI16)pMousePointInfo->GetParam(), TRUE);

				// 캐릭터를 공격하게 한다.
				if(pOnlineTrade->IsWait())
					break;

				if(pOnlineBooth->IsActionIn())
					break;

				UI16 uiUniqueID_UnderPoint = (UI16)pMousePointInfo->GetParam();

				// robypark 2005/1/25 18:45
				// 다른 캐릭터 치료라면
				// 공격자 캐릭터가 공성전 유닛인지 검사
				if (TRUE == bIsSiegeWarfareUnit)
				{
					// 대상 캐릭터가 공성전 유닛이라면,
					if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
					{
						// 공성시간인지 검사
						if(IsInTimeSiegeWarfare())
						{
							// 치료가 가능한가?
							if (SiegeWarfare_CanHeal(uiUniqueID_UnderPoint))
							{
								// 보급 수레가 치료시도 요청
								pOnlineCharUI->SetOrderHeal(uiUniqueID_UnderPoint);
								break;
							}
							// 공격 가능하다면
							else if (SiegeWarfare_CheckCanAttackUnit(uiUniqueID_UnderPoint))
							{
								// 공격 모드 설정
								m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_CHARACTER;
								m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = uiUniqueID_UnderPoint;
								break;
							}
						}
					}

					// 선택한 캐릭터 표시 취소
					pIOnlineChar->SetNewSelectedFlag((UI16)pMousePointInfo->GetParam(), FALSE);

					// 공성전 유닛은 온라인 전투를 할 수 없다.
					break;
				}

				// 대상 캐릭터가 공성전 유닛이라면,
				if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
				{
					// 선택한 캐릭터 표시 취소
					pIOnlineChar->SetNewSelectedFlag((UI16)pMousePointInfo->GetParam(), FALSE);

					// 공성전 유닛과 온라인 전투를 할 수 없다.
					break;
				}

				// 공성전 유닛 변신 메시지를 전송한 경우
				if (pOnlinePannel->GetSiegeWarfareTransformUIWnd()->IsSendedMsg())
				{
					// 온라인 전투를 할 수 없다.
					break;
				}

				if ((m_GameOption->m_Option_PKFlag == TRUE) && (!bPeace && pIOnlineChar->GetIsHostilityGuild((UI16)pMousePointInfo->GetParam()))
					&& (pOnlineMap->GetPKMapFlag() != 1) && (!pIOnlineChar->IsBoothOpen((UI16)pMousePointInfo->GetParam())))
				{
					pOnlineCharUI->SetAttack((UI16)pMousePointInfo->GetParam());
					// 효과음 출력
					// robypark 2005/1/14 17:27
					// 주인공 관련 데이터 관리자 얻기
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
					// 효과음 출력
					// robypark 2005/1/14 17:27
					// 주인공 관련 데이터 관리자 얻기
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
					// 효과음 출력
					// robypark 2005/1/14 17:27
					// 주인공 관련 데이터 관리자 얻기
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

								// 효과음 출력
								// robypark 2005/1/14 17:27
								// 주인공 관련 데이터 관리자 얻기
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

							// 효과음 출력
							// robypark 2005/1/14 17:27
							// 주인공 관련 데이터 관리자 얻기
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
					// 너무 많은 양의 무게의 아이템을 들고 있으면 메세지를 뛰움

					if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
					{
						SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
					}

					pIOnlineObject->GetPos(UI16(pMousePointInfo->GetParam()), &siTX, &siTY);

					// 마을이나 포탈일 경우
					if(pIOnlineObject->IsAtb(UI16(pMousePointInfo->GetParam()), ATB_OBJECT_VILLAGE) == TRUE)
					{
						// robypark 2004/11/12 13:35
						// 공성전 유닛의 마을 출입 제한하기
						// 공성전 유닛이 아닐 경우
						if (FALSE == bIsSiegeWarfareUnit)
						{
							// 오브젝트의 위치 검사
							pOnlineCharUI->SetUseObject(siTX, siTY);
						}
						// 공성전 유닛일 경우
						else
						{
							// 마을인지 포탈인지 구분
							if(FALSE == pIOnlineObject->IsPortal((UI16)pMousePointInfo->GetParam()))
							{
								// 마을인 경우
								// 공성하는 마을인지 검사
								// 공성시간인지 검사
								if(IsInTimeSiegeWarfare())
								{
									if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
									{
										// 마을 공격이 가능한 캐릭터인가?
										if (SiegeWarfare_CheckCanAttackVillage())
										{
											// 공성전 공격 모드 설정 변수
											m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_VILLAGE;
											m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()));
										}
									}	// if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
									else
									{
										// 적대 마을일 경우
										if (GetVillageEnemyGuild( pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ) ) )
										{
											// 적대 마을이지만 오늘 이 마을의 상단과 공성전이 없다.
											char szErrorMsg[512];
											SVillageData*		lpSVillageData;
											VillageHeader*		lpVillageHeader;

											lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));
											lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));

											if ((lpSVillageData) && (lpVillageHeader))
											{
												// 상단이 있는 마을인 경우
												if (NULL != lpSVillageData->m_VillageGuildName[0])
												{
													sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
																lpSVillageData->m_VillageGuildName, 
																pOnlineText->Get(lpVillageHeader->siVillageName ));
												}
												// 상단이 없는 마을인 경우
												else
												{
													sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY),
																pOnlineText->Get(lpVillageHeader->siVillageName ));
												}

												pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);
											}
										}
										// 해당 마을이 캐릭터가 소속된 상단 소유의 마을일 경우 
										// 보급품 관련 처리
										else if (TRUE == GetVillageOurGuild(pIOnlineObject->GetVillageCode((UI16)pMousePointInfo->GetParam())))
										{
											// 공성 유닛 정보 헤더 얻기
											LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(pMyData->GetMyCharKind());

											// 공성전 유닛이 맞을 경우
											if (NULL != lpsSiegeUnitTable)
											{
												// 공성전 유닛 중 수성을 위한 마을 출입이 가능한 유닛인지 검사
												if ((lpsSiegeUnitTable) && (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE))
												{
													// 현재 내 캐릭터가 수행하는명령을 얻어온다.
													OnOrder Order	=	pIOnlineChar->GetCurrentOrder(pMyData->GetMyCharUniqueID());
													//  공성전 시간이며, 
													if ((IsInTimeSiegeWarfare())
														&& (Order.siOrder != ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE))	// 이전에 수성에 들어가겠다고 요청하지 않은 경우
													{
														// 마을 수성 인터페이스에 들어가겠다고 캐릭터에 명령 및 서버에 전달
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
											// 적대 마을이 아닐경우
											// 공격 불가능 메시지 띄우기
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
								// 공성전 유닛은 본 맵 이동가능한 포탈 이용가능
								// 포탈 ID 찾기
								if(pIOnlineObject->GetPortalID(SI16(pMousePointInfo->GetParam())) != -1)
								{
									PortalHeader*				lpPortalHeader = NULL;
									
									// 포탈 정보 구조체 찾기
									lpPortalHeader = pOnlineMap->PortalParser.GetPortal(pIOnlineObject->GetPortalID(SI16(pMousePointInfo->GetParam())));

									if(lpPortalHeader)
									{
										// 맵 인덱스 0인 본 맵으로만 포탈 이용 가능
										if (0 == lpPortalHeader->siPortalMoveMapIndex)
										{
											// 오브젝트의 위치 검사
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

				// 수성 인터페이스 들어가기 취소
				m_sVillageDefenceInterface.m_uiVillageCode = 0;

				if(bCharMove == TRUE)
				{
					// 너무 많은 양의 무게의 아이템을 들고 있으면 메세지를 뛰움

					if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
					{
						SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
					}

					pMousePointInfo->GetPos(&siItemX, &siItemY);
					uiItemID		=	pMousePointInfo->GetParam();				

					if(uiItemID != 0)
					{
						// Item 을 이동시키는 부분으로
						pOnlineCharUI->SetPickUpItem(siItemX, siItemY, uiItemID);						
					}
					else
					{
						// 자판 Item일 경우
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
				// 수성 인터페이스 들어가기 취소
				m_sVillageDefenceInterface.m_uiVillageCode = 0;

				if(SI16(pMousePointInfo->GetParam()))
				{
					NPCIndexHeader*		lpNPCIndexHeader;

					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(SI16(pMousePointInfo->GetParam()));

					// robypark 2004/9/9
					// 혼잣말이 비활성화된 NPC는 클릭하여 선택할 수 없도록 수정, 공성전 유닛이 아니며(robypark 2004/11/22 11:44)
					if ((lpNPCIndexHeader) && (lpNPCIndexHeader->bTalkOneself) && (FALSE == bIsSiegeWarfareUnit))
					{
						if(bCharMove == TRUE)
						{
							// 너무 많은 양의 무게의 아이템을 들고 있으면 메세지를 뛰움

							if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
							{
								SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
							}

							// 오브젝트의 위치 검사

							pOnlineCharUI->SetUseNPC(lpNPCIndexHeader->siMapXPos, lpNPCIndexHeader->siMapYPos, lpNPCIndexHeader->siCode);
						}
					}
				}
			}
			break;

		default:
			// 수성 인터페이스 들어가기 취소
			m_sVillageDefenceInterface.m_uiVillageCode = 0;

			// 좌판 밖으로 나가는지 체크
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
				// 너무 많은 양의 무게의 아이템을 들고 있으면 메세지를 뛰움
				if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
				{
					SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
				}

				pOnlineCharUI->SetMove(siCursorX, siCursorY);
			}

			if( rand()%100 > 99 )
			{
				// 효과음 출력
				// robypark 2005/1/14 17:27
				// 주인공 관련 데이터 관리자 얻기
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

			// 약초채집 체크
			if(pCBuriedItemParser->isBuriedArea(pOnlineMap->m_MapIndex))
			{
				SI16 siHeroX, siHeroY;
				if(!pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY))
					break;

				// 이전에 아이템 탐사 요청시의 위치와 지금 위치의 차이값
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
// 필드에 있을 때 마우스 커서 오른쪽 버튼 클릭 처리
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
				// 거래 중이거나
				if (pOnlineTrade->IsActive())
				{
					// 노점상 및 거래 신청, 공성전 워프 및 공격 불가능하다
					break;
				}
				//-------------------------------------------------//

				// 노점상을 클릭했을 경우
				if(pIOnlineChar->IsBoothOpen((UI16)pMousePointInfo->GetParam()) && pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind( (UI16)pMousePointInfo->GetParam())))
				{
					if(!pOnlineBooth->IsActionIn())
					{
						// 경매에 참여중이 아닐때에만 노점에 들어갈수있당
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
						// 전투 대기중일 때 거래막음
						//OnOrder	Order =	pIOnlineChar->GetCurrentOrder(pMyData->GetMyCharUniqueID());
						if(pMyData->IsWaitBattleResponse())
							break;

						UI16 uiUniqueID_UnderPoint = (UI16)pMousePointInfo->GetParam();

						// robypark 2005/1/13 22:2
						// 공성전 중에 일반 유저(피난민)을 초보자 수련장으로 워프를 시킬 수 있는지 검사
						if (SiegeWarfare_CheckAbledWarfRefugee(uiUniqueID_UnderPoint))
						{
							// 워프 요청
							// 이 요청 응답은 기존의 운영자 명령어인 /w(워프)의 것을 사용한다.(ON_RESPONSE_WARP)
							pOnlineClient->SendRequestSiegeWarfareWarp(uiUniqueID_UnderPoint);
							break;
						}

						// robypark 2005/1/25 18:45
						// 다른 캐릭터 치료라면
						// 공격자 캐릭터가 공성전 유닛인지 검사
						if (TRUE == bIsSiegeWarfareUnit)
						{
							/*
							// 대상 캐릭터가 공성전 유닛이라면,
							if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
							{
								// 치료가 가능한가?
								if (SiegeWarfare_CanHeal(uiUniqueID_UnderPoint))
								{
									// 보급 수레가 치료시도 요청
									pOnlineCharUI->SetOrderHeal(uiUniqueID_UnderPoint);
									break;
								}
								// 공격 가능하다면
								else if (SiegeWarfare_CheckCanAttackUnit(uiUniqueID_UnderPoint))
								{
									// 공격 모드 설정
									m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_CHARACTER;
									m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = uiUniqueID_UnderPoint;
									break;
								}
							}
							*/

							// 공성전 유닛은 거래 및 파티신청을 할 수 없다.
							break;
						}

						// 수성인터페이스 안에 있는 캐릭터가 아니며
						if (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiUniqueID_UnderPoint))
						{							
							// robypark 2004/12/24 15:19
							// 거래 대상 캐릭터가 공성전 유닛이라면,
							if (pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID_UnderPoint))
							{
								// 거래 및 파티 요청을 할 수 없다.
								break;
							}
							// 마우스 오른쪽 버튼으로 선택한 캐릭터가 플레이어이거나
							else if (pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind(uiUniqueID_UnderPoint)))
							{
								// 전투 모드일 경우 거래 신청 못함.
								if (IsPeace() == FALSE)
									break;

								if(pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()))
									break;

								ZeroMemory(pOnlineTrade->FriendInfo.szCharName, sizeof(pOnlineTrade->FriendInfo.szCharName));
								strncpy(pOnlineTrade->FriendInfo.szCharName, pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()), ON_ID_LENGTH);
								pOnlineTrade->FriendInfo.uiDestUniqueID = uiUniqueID_UnderPoint;	// 거래 대상 캐릭터 유니크 아이디 설정
							
								// 유효한 캐릭터 선택이라면
								if(( pOnlineTrade->FriendInfo.szCharName[0] != 0) && (pOnlineTrade->FriendInfo.uiDestUniqueID))
								{
									// 거래 신청한 캐릭터가 운영자일 경우
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
					// 너무 많은 양의 무게의 아이템을 들고 있으면 메세지를 뛰움
					if(pMyData->GetFollowersInterface()->IsItemWeightOverflowed())
					{
						SetMessage(pOnlineText->Get(ON_TEXT_ITEMHELP_MOVE_HELP_HEAVYITEM));
					}

					/*
					// 마을인지 검사
					if(pIOnlineObject->IsAtb(UI16(pMousePointInfo->GetParam()), ATB_OBJECT_VILLAGE) == TRUE)	
					{
						if(pIOnlineObject->IsPortal(UI16(pMousePointInfo->GetParam())) == FALSE)
						{
							// 공성하는 마을인지 검사
							// 공성시간인지 검사
							if(IsInTimeSiegeWarfare())
							{
								if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
								{
									// 마을 공격이 가능한 캐릭터인가?
									if (SiegeWarfare_CheckCanAttackVillage())
									{
										// 공성전 공격 모드 설정 변수
										m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_VILLAGE;
										m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()));
									}
								}	// if(GetVillageWarDayGuild(pIOnlineObject->GetVillageCode(UI16(pMousePointInfo->GetParam()))))
								else
								{
									// 적대 마을일 경우
									if (GetVillageEnemyGuild( pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ) ) )
									{
										// 적대 마을이지만 오늘 이 마을의 상단과 공성전이 없다.
										char szErrorMsg[512];
										SVillageData*		lpSVillageData;
										VillageHeader*		lpVillageHeader;

										lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));
										lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pIOnlineObject->GetVillageCode( (UI16)pMousePointInfo->GetParam() ));

										if ((lpSVillageData) && (lpVillageHeader))
										{
											// 상단이 있는 마을인 경우
											if (NULL != lpSVillageData->m_VillageGuildName[0])
											{
												sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
															lpSVillageData->m_VillageGuildName, 
															pOnlineText->Get(lpVillageHeader->siVillageName ));
											}
											// 상단이 없는 마을인 경우
											else
											{
												sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY),
															pOnlineText->Get(lpVillageHeader->siVillageName ));
											}

											pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);
										}
									}
									// 해당 마을이 캐릭터가 소속된 상단 소유의 마을일 경우 
									// 보급품 관련 처리
									else if (TRUE == GetVillageOurGuild(pIOnlineObject->GetVillageCode((UI16)pMousePointInfo->GetParam())))
									{
										// 자신의 캐릭터가 보급 수레(보급품 저장했다가 나눠 줄 수 있는)인 경우
										if (KI[pIOnlineChar->GetImjinKind( pMyData->GetMyCharUniqueID() )].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
										{
											SI16	siVillagePosX,siVillagePosY;

											// 마을 위치 얻기
											pIOnlineObject->GetPos((UI16)pMousePointInfo->GetParam(), &siVillagePosX, &siVillagePosY);

											// 마을에 보급 수레 보급품 보충 요청하기
											pOnlineCharUI->SetRequestSupplyGoodsFromVillage(siVillagePosX, siVillagePosY);
										}
										else
										{
											// 적대 마을이 아닐경우
											// 공격 불가능 메시지 띄우기
											pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
										}										
									}
									else
									{
										// 적대 마을이 아닐경우
										// 공격 불가능 메시지 띄우기
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
