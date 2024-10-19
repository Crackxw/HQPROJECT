/********************************************************************************************************
	파일명	:	OnlineWorld-ConnectServer.cpp
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2003.01.21
********************************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>
#include "OnlineWorld.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineText.h"
#include "OnlineMsgBox.h"
#include "OnlineNotice.h"
#include "OnlineFont.h"
#include "OnlineKeyboard.h"
#include "OnlineConfig.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineVersion.h"

extern _clGame* pGame;

#define WORLD_DISPLAY_MAX_COUNT		13

enum
{   
	ON_SELECT_SERVER_INIT		= 0,
	ON_SELECT_SERVER_POLL,
	ON_SELECT_SERVER_WAIT,
};


VOID cltOnlineWorld::InitSelectServer()
{
	pOnlineLogInMgr->m_SelectServerData.pScrollBarImage = pOnlineResource->GetxsprGame(ON_GMAESPR_LOGIN_SCROLL_BAR);

	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_server_box.Spr", pOnlineLogInMgr->m_SelectServerData.ServerBoxImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_server_select.Spr", pOnlineLogInMgr->m_SelectServerData.ServerSelImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_server_sts.Spr", pOnlineLogInMgr->m_SelectServerData.PingImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_server_sts_ring.Spr", pOnlineLogInMgr->m_SelectServerData.PingRingImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_server_sts_Scroll.Spr", pOnlineLogInMgr->m_SelectServerData.ScrollAreaImage);

	pOnlineLogInMgr->m_SelectServerData.siSelectStatus		= ON_SELECT_SERVER_INIT;
	pOnlineLogInMgr->m_SelectServerData.pWorldInfo			= NULL;
	pOnlineLogInMgr->m_SelectServerData.siWorldCount		= 0;

	pOnlineLogInMgr->m_SelectServerData.m_siScroll			= 0;
	pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY		= 224;
	pOnlineLogInMgr->m_SelectServerData.m_fOldScrollY		= 224;
	SetRect(&pOnlineLogInMgr->m_SelectServerData.m_rtScrollArea, 716, 224, 730, 455);

	pOnlineLogInMgr->m_SelectServerData.siSelect			= pConfig->GetServerType() - 1;
	if(pOnlineLogInMgr->m_SelectServerData.siSelect < 0)	pOnlineLogInMgr->m_SelectServerData.siSelect = 0;

	pOnlineLogInMgr->m_SelectServerData.OkButton.Create(530, 502, pOnlineLogInMgr->SButtonImage.Header.Xsize, pOnlineLogInMgr->SButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_INFO_CONNECT), BUTTON_PUT_LEFT, TRUE);
	pOnlineLogInMgr->m_SelectServerData.ExitButton.Create(651, 502, pOnlineLogInMgr->SButtonImage.Header.Xsize, pOnlineLogInMgr->SButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_QUESTINFO_EXIT), BUTTON_PUT_LEFT, TRUE);

	if(g_fTestVersion == FALSE)
	{
		switch(pGame->LanguageCode)
		{
		case ELANGAGECODE_TAIWAN:
		case ELANGAGECODE_CHINA:
		case ELANGAGECODE_HONGKONG:
			g_PingPerUserCount[0] = 1000;
			g_PingPerUserCount[1] = 1500;
			break;
		case ELANGAGECODE_JAPAN:
		default:
			g_PingPerUserCount[0] = 2000;
			g_PingPerUserCount[1] = 2500;
			break;
		}
	}
	else
	{
		g_PingPerUserCount[0] = 2000;
		g_PingPerUserCount[1] = 2500;
	}
}

SI08 cltOnlineWorld::SelectServer()
{
	BOOL			bSelect = FALSE;
	SI32			TempCount;
	
	switch(pOnlineLogInMgr->m_SelectServerData.siSelectStatus)
	{
	case ON_SELECT_SERVER_INIT:
		{
			pOnlineLogInMgr->m_StartTickCount = GetTickCount();
			pOnlineLogInMgr->m_SelectServerData.OkButton.SetAction(TRUE);
			pOnlineLogInMgr->m_SelectServerData.ExitButton.SetAction(TRUE);
			pOnlineLogInMgr->m_SelectServerData.siSelectStatus = ON_SELECT_SERVER_POLL;
		}
		break;
	case ON_SELECT_SERVER_POLL:
		{
			if(g_AutoServerCode != -1)
			{
				pOnlineLogInMgr->m_SelectServerData.siSelectStatus = ON_SELECT_SERVER_WAIT;
				break;
			}

			//========================================================================================
			// 처리
			// 버튼 처리
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_UP)))
			{
				pOnlineLogInMgr->m_SelectServerData.siSelect--;
				if(pOnlineLogInMgr->m_SelectServerData.siSelect < 0)		pOnlineLogInMgr->m_SelectServerData.siSelect = 0;
			}
			
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_DOWN)))
			{
				pOnlineLogInMgr->m_SelectServerData.siSelect++;
				if(pOnlineLogInMgr->m_SelectServerData.siSelect > pOnlineLogInMgr->m_SelectServerData.siWorldCount - 1)	pOnlineLogInMgr->m_SelectServerData.siSelect = pOnlineLogInMgr->m_SelectServerData.siWorldCount - 1;
			}
			
			if(pOnlineLogInMgr->m_SelectServerData.OkButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
			{
				if(pOnlineLogInMgr->m_SelectServerData.pWorldInfo && pOnlineLogInMgr->m_SelectServerData.siWorldCount > 0)
				{
					if(pOnlineLogInMgr->m_SelectServerData.pWorldInfo[pOnlineLogInMgr->m_SelectServerData.siSelect].world_state == FRONT::OK)
					{
						pConfig->SetServerType(pOnlineLogInMgr->m_SelectServerData.siSelect + 1);
						pConfig->SaveConfig();
//						pConfig->OpenConfig();
						pOnlineClient->SendWorldID(pOnlineLogInMgr->m_SelectServerData.pWorldInfo[pOnlineLogInMgr->m_SelectServerData.siSelect].world_id);
						pOnlineLogInMgr->m_SelectServerData.OkButton.SetAction(FALSE);
						
						pOnlineLogInMgr->m_SelectServerData.siSelectStatus = ON_SELECT_SERVER_WAIT;
					}
				}
			}

			if(pOnlineLogInMgr->m_SelectServerData.ExitButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
				return ON_RETURN_EXIT;

			DWORD dwTempTickCount = GetTickCount();
			if( (dwTempTickCount - pOnlineLogInMgr->m_StartTickCount) > 100 )
			{
				if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_RETURN)))
				{
					if(pOnlineLogInMgr->m_SelectServerData.pWorldInfo && pOnlineLogInMgr->m_SelectServerData.siWorldCount > 0)
					{
						if(pOnlineLogInMgr->m_SelectServerData.pWorldInfo[pOnlineLogInMgr->m_SelectServerData.siSelect].world_state == FRONT::OK)
						{
							pConfig->SetServerType(pOnlineLogInMgr->m_SelectServerData.siSelect + 1);
							pConfig->SaveConfig();
//							pConfig->OpenConfig();
							pOnlineClient->SendWorldID(pOnlineLogInMgr->m_SelectServerData.pWorldInfo[pOnlineLogInMgr->m_SelectServerData.siSelect].world_id);
							pOnlineLogInMgr->m_SelectServerData.OkButton.SetAction(FALSE);
							
							pOnlineLogInMgr->m_SelectServerData.siSelectStatus = ON_SELECT_SERVER_WAIT;
						}
					}
				}
			}

			ScrollAction();

			if(pOnlineLogInMgr->bMouseDown)
			{
				// 서버 선택 동작
				for(TempCount = 0; TempCount < WORLD_DISPLAY_MAX_COUNT; TempCount++)
				{
					if((TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll) < pOnlineLogInMgr->m_SelectServerData.siWorldCount)
					{
						if((pOnlineLogInMgr->ptMouse.x >= 556) && (pOnlineLogInMgr->ptMouse.x < 705) && (pOnlineLogInMgr->ptMouse.y >= (220 + (TempCount * 20))) && (pOnlineLogInMgr->ptMouse.y < (240 + (TempCount * 20))))
						{
							if(pOnlineLogInMgr->m_SelectServerData.pWorldInfo[TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll].world_state == FRONT::OK)
								pOnlineLogInMgr->m_SelectServerData.siSelect = TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll;
						}
					}
				}
			}
		}
		break;
	case ON_SELECT_SERVER_WAIT:			// 서버선택 OK 메세지를 기다린다.
		{
			FRONT_USER::sResponseSelectWorld *pSelectWorldMsg = (FRONT_USER::sResponseSelectWorld*)pOnlineClient->GetMsgBuffer((DWORD)FRONT_USER::RESPONSE_SELECT_WORLD);
			if(!pSelectWorldMsg)		break;

			if(pSelectWorldMsg->world_id != FRONT::UNKNOWN_FAIL && pSelectWorldMsg->world_id == pOnlineLogInMgr->m_SelectServerData.pWorldInfo[pOnlineLogInMgr->m_SelectServerData.siSelect].world_id)
			{
				bServerSelect = TRUE;

				// FrontServer와의 접속을 끊는다.
				pOnlineClient->Close();
				
				// 해당 서버아이피를 얻어온다.			
				sprintf(pOnlineClient->IPAddress, pSelectWorldMsg->gateIP); 
				sprintf(pOnlineClientChatting->IPAddress, pSelectWorldMsg->chatIP);

				// 선택한 서버정보를 저장한다.
				sprintf(g_stConnectServerInfo.m_ServerName, pOnlineLogInMgr->m_SelectServerData.pWorldInfo[pOnlineLogInMgr->m_SelectServerData.siSelect].name);
				g_stConnectServerInfo.m_WorldID = pSelectWorldMsg->world_id;
				g_stConnectServerInfo.m_ChatPort = pSelectWorldMsg->chatPort;
				g_stConnectServerInfo.m_GatePort = pSelectWorldMsg->gatePort;

				pOnlineLogInMgr->m_StartTickCount = timeGetTime();
				pOnlineLogInMgr->m_SelectServerData.siSelectStatus = ON_SELECT_SERVER_INIT;
				return ON_RETURN_OK;
			}
			else
			{
				pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DISCONNECT_SELECTED_SERVER));
				pOnlineLogInMgr->m_SelectServerData.siSelectStatus = ON_SELECT_SERVER_INIT;
			}
		}
		break;
	}
	return ON_RETURN_NONE;
}

VOID	cltOnlineWorld::DrawSelectServer()
{
	SI32				TempCount;
	char				TempBuffer[256];
	HDC					hDC;
	FRONT::sWorldInfo	TempWorldInfo;

	pOnlineHelp->Initialize();
	
	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{
		// 바탕
		clGrp.PutSpriteT(512, 195, pOnlineLogInMgr->m_SelectServerData.ServerBoxImage.Header.Xsize, pOnlineLogInMgr->m_SelectServerData.ServerBoxImage.Header.Ysize,  pOnlineLogInMgr->m_SelectServerData.ServerBoxImage.Image);

		if(pOnlineLogInMgr->m_SelectServerData.siWorldCount > WORLD_DISPLAY_MAX_COUNT)
		{
			clGrp.PutSpriteT(719, 224, pOnlineLogInMgr->m_SelectServerData.ScrollAreaImage.Header.Xsize, pOnlineLogInMgr->m_SelectServerData.ScrollAreaImage.Header.Ysize, pOnlineLogInMgr->m_SelectServerData.ScrollAreaImage.Image);		
			clGrp.PutSpriteT(716, pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY, pOnlineLogInMgr->m_SelectServerData.pScrollBarImage->Header.Xsize, pOnlineLogInMgr->m_SelectServerData.pScrollBarImage->Header.Ysize, pOnlineLogInMgr->m_SelectServerData.pScrollBarImage->Image);		
		}
		
		// 서버 리스트 Ping
		SI16 TempPing;
		for(TempCount = 0; TempCount < WORLD_DISPLAY_MAX_COUNT; TempCount++)
		{
			if((TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll) < pOnlineLogInMgr->m_SelectServerData.siWorldCount)
			{
				TempWorldInfo = pOnlineLogInMgr->m_SelectServerData.pWorldInfo[TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll];

				if(TempWorldInfo.world_state == FRONT::OK)
				{
					if(TempWorldInfo.concurrent < g_PingPerUserCount[0])      TempPing = 2;
					else if(TempWorldInfo.concurrent < g_PingPerUserCount[1]) TempPing = 1;
					else                                                      TempPing = 0;
				}
				else
					TempPing = 0;

				clGrp.PutSpriteT(538, 219 + (TempCount * 20), pOnlineLogInMgr->m_SelectServerData.PingRingImage.Header.Xsize, pOnlineLogInMgr->m_SelectServerData.PingRingImage.Header.Ysize, pOnlineLogInMgr->m_SelectServerData.PingRingImage.Image);
				clGrp.PutSpriteT(540, 221 + (TempCount * 20), pOnlineLogInMgr->m_SelectServerData.PingImage.Header.Xsize, pOnlineLogInMgr->m_SelectServerData.PingImage.Header.Ysize, &pOnlineLogInMgr->m_SelectServerData.PingImage.Image[pOnlineLogInMgr->m_SelectServerData.PingImage.Header.Start[TempPing]]);

				if((TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll) == pOnlineLogInMgr->m_SelectServerData.siSelect)
					clGrp.PutSpriteT(556, 216 + TempCount * 20, pOnlineLogInMgr->m_SelectServerData.ServerSelImage.Header.Xsize, pOnlineLogInMgr->m_SelectServerData.ServerSelImage.Header.Ysize,  pOnlineLogInMgr->m_SelectServerData.ServerSelImage.Image);
			}
		}
		
		// 버튼
		pOnlineLogInMgr->m_SelectServerData.OkButton.Put(&pOnlineLogInMgr->SButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pOnlineLogInMgr->m_SelectServerData.ExitButton.Put(&pOnlineLogInMgr->SButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}	
	
	if(GETSCREENSURFACE(this)->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, pOnlineFonts->GetFont(ON_FONT_CHAT));
		
		for(TempCount = 0; TempCount < WORLD_DISPLAY_MAX_COUNT; TempCount++)
		{
			if((TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll) < pOnlineLogInMgr->m_SelectServerData.siWorldCount)
			{
				TempWorldInfo = pOnlineLogInMgr->m_SelectServerData.pWorldInfo[TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll];

				sprintf((char*)TempBuffer, "%2d. %s (%d.%d.%d)", TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll + 1, TempWorldInfo.name
					, TempWorldInfo.getYear(), TempWorldInfo.getMonth(), TempWorldInfo.getDay());

				// 서버리스트
				if(TempWorldInfo.world_state == FRONT::OK)
				{
					pOnlineMegaText->DrawText(hDC, 562 + 1, 219 + (TempCount * 20) + 1, (char*)TempBuffer, RGB(8, 8, 8));
					if((TempCount + pOnlineLogInMgr->m_SelectServerData.m_siScroll) == pOnlineLogInMgr->m_SelectServerData.siSelect)
					{
						// 선택된 서버
						pOnlineMegaText->DrawText(hDC, 562, 220 + (TempCount * 20), (char*)TempBuffer, RGB(250, 250, 250));
					}
					else
					{
						// 마우스 올려진 서버
						if((pOnlineLogInMgr->ptMouse.x >= 556) && (pOnlineLogInMgr->ptMouse.x < 705) && (pOnlineLogInMgr->ptMouse.y >= (220 + (TempCount * 20))) && (pOnlineLogInMgr->ptMouse.y < (240 + (TempCount * 20))))
							pOnlineMegaText->DrawText(hDC, 562, 220 + (TempCount * 20), (char*)TempBuffer, RGB(250, 250, 250));
						// 선택안된 서버
						else
							pOnlineMegaText->DrawText(hDC, 562, 220 + (TempCount * 20), (char*)TempBuffer, RGB(210, 210, 210));
					}
				}
				else
					pOnlineMegaText->DrawText(hDC, 562, 220 + (TempCount * 20), (char*)TempBuffer, RGB(210, 210, 210));
				
				// 서버상태 표시 도움말
				if((pOnlineLogInMgr->ptMouse.x >= 556) && (pOnlineLogInMgr->ptMouse.x < 705) && (pOnlineLogInMgr->ptMouse.y >= (220 + (TempCount * 20))) && (pOnlineLogInMgr->ptMouse.y < (240 + (TempCount * 20))))
				{
					if(TempWorldInfo.world_id == 98)
					{
						// 이벤트 월드
						pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_SERVER_STATE_EVENT), pOnlineLogInMgr->ptMouse.x, pOnlineLogInMgr->ptMouse.y, ON_HELP_UP, ON_HELP_WHITE, TRUE);
					}
					else
					{
						if(TempWorldInfo.world_state == FRONT::OK)
						{
							if(TempWorldInfo.concurrent < g_PingPerUserCount[0])      pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_SERVER_STATE_OK),       pOnlineLogInMgr->ptMouse.x, pOnlineLogInMgr->ptMouse.y, ON_HELP_UP, ON_HELP_WHITE, TRUE);
							else if(TempWorldInfo.concurrent < g_PingPerUserCount[1]) pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_SERVER_STATE_SLOW),     pOnlineLogInMgr->ptMouse.x, pOnlineLogInMgr->ptMouse.y, ON_HELP_UP, ON_HELP_WHITE, TRUE);
							else                                                      pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_SERVER_STATE_VERYSLOW), pOnlineLogInMgr->ptMouse.x, pOnlineLogInMgr->ptMouse.y, ON_HELP_UP, ON_HELP_WHITE, TRUE);
						}
						else
							pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_SERVER_INSPECT_WAITING), pOnlineLogInMgr->ptMouse.x, pOnlineLogInMgr->ptMouse.y, ON_HELP_UP, ON_HELP_WHITE, TRUE);
					}
				}
			}
		}
		
		// 버튼
		SelectObject(hDC, pOnlineFonts->GetFont(ON_FONT_CHAT));
		pOnlineLogInMgr->m_SelectServerData.OkButton.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_SelectServerData.ExitButton.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		
		GETSCREENSURFACE(this)->ReleaseDC(hDC);
	}
}

VOID cltOnlineWorld::FreeSelectServer()
{
	if(pOnlineLogInMgr->m_SelectServerData.pWorldInfo)
	{
		delete [] pOnlineLogInMgr->m_SelectServerData.pWorldInfo;
		pOnlineLogInMgr->m_SelectServerData.pWorldInfo = NULL;
	}
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectServerData.ServerBoxImage);
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectServerData.ServerSelImage);
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectServerData.PingImage);
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectServerData.PingRingImage);
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectServerData.ScrollAreaImage);
}

VOID cltOnlineWorld::ScrollAction()
{
	SI32	totalScrollNum, scrollNum, endY, ScrollAreaHeight;
	FLOAT	indent;
	
	if(!pOnlineLogInMgr->bMouseDown)	return;

	if(pOnlineLogInMgr->m_SelectServerData.siWorldCount <= WORLD_DISPLAY_MAX_COUNT)		return;

	if(PtInRect(&pOnlineLogInMgr->m_SelectServerData.m_rtScrollArea, pOnlineLogInMgr->ptMouse) != TRUE)	return;

	if(pOnlineLogInMgr->m_SelectServerData.siWorldCount <= WORLD_DISPLAY_MAX_COUNT)		return;

	totalScrollNum = pOnlineLogInMgr->m_SelectServerData.siWorldCount - WORLD_DISPLAY_MAX_COUNT + 1;

	ScrollAreaHeight = pOnlineLogInMgr->m_SelectServerData.m_rtScrollArea.bottom - pOnlineLogInMgr->m_SelectServerData.m_rtScrollArea.top;
	indent = (FLOAT)(ScrollAreaHeight - pOnlineLogInMgr->m_SelectServerData.pScrollBarImage->Header.Ysize) / (FLOAT)totalScrollNum;

	if(indent <= 0)		return;
	
	// 스크롤바의 위치(m_siScrollBarY)를 변경한다.
	pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY = pOnlineLogInMgr->ptMouse.y - pOnlineLogInMgr->m_SelectServerData.pScrollBarImage->Header.Ysize/2;
	endY = pOnlineLogInMgr->m_SelectServerData.m_rtScrollArea.top + ScrollAreaHeight - pOnlineLogInMgr->m_SelectServerData.pScrollBarImage->Header.Ysize;
	if(pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY > endY)	pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY = endY;
	if(pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY < 222)	pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY = 222;
	
	// 화면의 첫번째에 나와야 할 서버의 인덱스를 설정한다.
	scrollNum = (SI32)(((FLOAT)pOnlineLogInMgr->m_SelectServerData.m_siScrollBarY - pOnlineLogInMgr->m_SelectServerData.m_fOldScrollY) / indent);	// 몇 칸을 스크롤 할 것인가?
	if( scrollNum != 0 )
	{
		pOnlineLogInMgr->m_SelectServerData.m_siScroll += scrollNum;
		if(pOnlineLogInMgr->m_SelectServerData.m_siScroll < 0 ) 
			pOnlineLogInMgr->m_SelectServerData.m_siScroll = 0;
		else if(pOnlineLogInMgr->m_SelectServerData.m_siScroll > (pOnlineLogInMgr->m_SelectServerData.siWorldCount - WORLD_DISPLAY_MAX_COUNT))
			pOnlineLogInMgr->m_SelectServerData.m_siScroll = pOnlineLogInMgr->m_SelectServerData.siWorldCount - WORLD_DISPLAY_MAX_COUNT;
		
		pOnlineLogInMgr->m_SelectServerData.m_fOldScrollY += (indent * (FLOAT)scrollNum); 
	}
}