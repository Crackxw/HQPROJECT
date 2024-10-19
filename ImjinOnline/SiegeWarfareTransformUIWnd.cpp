// SiegeWarfareTransformUIWnd.cpp: implementation of the CSiegeWarfareTransformUIWnd class.
//
//////////////////////////////////////////////////////////////////////


#include	<GSL.h>
#include	<Mouse.h>

#include	"OnlineWorld.h"

#include	<OnlineHelp.h>
#include	<OnlineMegaText.h>
#include	<OnlineTradeBook.h>
#include	<OnlineTip.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
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

#include	"SiegeWarfareTransformUIWnd.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

extern _InputDevice				IpD;

extern	unsigned char				DarkTable[][256];					// 어두운 단계 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeWarfareTransformUIWnd::CSiegeWarfareTransformUIWnd()
{
	m_pMyOnlineWorld		= NULL;
	m_bSendedMsg			= FALSE;
	m_bVisibleWindow		= FALSE;
	m_bDownloadedGuildCount = FALSE;
	m_bMouseDown			= FALSE;

	m_siSelectArmyKindIdx		= -1;
	m_siPrevSelectArmyKindIdx	= -1;
	m_siHoverArmyKindIdx		= -1;

//	m_pSurfaceSprArmy		= NULL;

	ZeroMemory(m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);
}

CSiegeWarfareTransformUIWnd::~CSiegeWarfareTransformUIWnd()
{
	FreeResource();
}

// 초기화
void	CSiegeWarfareTransformUIWnd::Init(cltOnlineWorld *pOnlineWorld)
{
	m_pMyOnlineWorld = pOnlineWorld;

	m_bSendedMsg			= FALSE;
	m_bVisibleWindow		= FALSE;
	m_bDownloadedGuildCount = FALSE;
	m_bMouseDown			= FALSE;

	m_siSelectArmyKindIdx		= -1;
	m_siPrevSelectArmyKindIdx	= -1;
	m_siHoverArmyKindIdx		= -1;

//	m_pSurfaceSprArmy		= NULL;

	ZeroMemory(m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);
}

// 윈도 보이기 설정
BOOL	CSiegeWarfareTransformUIWnd::ShowWindow(BOOL bVisibleWindow)
{
	m_bVisibleWindow = bVisibleWindow;

	// 이미 변신 메시지를 보낸 경우
	if (TRUE == m_bSendedMsg)
	{
		if (TRUE == m_bVisibleWindow)
		{
			m_bVisibleWindow = FALSE;

			return FALSE;
		}
	}

	// 활성화
	if (FALSE == m_bVisibleWindow)
	{
		// 리소스 해제
		FreeResource();

		if (-1 != m_siNoFieldAreaIdx)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaIdx);
			m_siNoFieldAreaIdx = -1;
		}
	}
	else
	{
		// 상단 내 공성 유닛 목록 요청
		m_pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
		m_bDownloadedGuildCount = FALSE;

		LoadResource();

		if (-1 == m_siNoFieldAreaIdx)
		{
			// No Field Area 설정
			m_siNoFieldAreaIdx = m_pMyOnlineWorld->
				pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY,
													m_siX + m_sprBackground.Header.Xsize,
													m_siY + m_sprBackground.Header.Ysize);
		}
	}

	return TRUE;
}

// 처리
BOOL	CSiegeWarfareTransformUIWnd::Action(void)
{
	if (FALSE == m_bVisibleWindow)
		return FALSE;

	POINT ptMouse;

	ptMouse.x	=	IpD.Mouse_X;
	ptMouse.y	=	IpD.Mouse_Y;

	m_siHoverArmyKindIdx = -1;

	if (m_bDownloadedGuildCount)
	{
		// 병과 애니메이션 버튼 갱신
		UpdateButton();
	}

	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// 병과 설명 텍스트 인덱스 얻기
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
			continue;

		// 어느 병과 이미지 위에 버튼이 있는가?
		if (PtInRect(&m_sAnimationButton[i].m_rtDraw, ptMouse))
		{
			m_siHoverArmyKindIdx = i;

			if (m_bMouseDown)
			{
				m_siSelectArmyKindIdx = i;

				// 이전에 선택했던 병과가 아니라면
				if (m_siPrevSelectArmyKindIdx != m_siSelectArmyKindIdx)
				{
					// 이전 것 애니메이션 갱신
					if (-1 != m_siPrevSelectArmyKindIdx)
					{
						// 해당 병과에 대한 정보를 얻어온다
						LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siPrevSelectArmyKindIdx);
						m_sAnimationButton[m_siPrevSelectArmyKindIdx].m_scCurrentAnimationIdx = ANIBTN_WAIT;
						m_sAnimationButton[m_siPrevSelectArmyKindIdx].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionStartFont;
						m_sAnimationButton[m_siPrevSelectArmyKindIdx].m_scCurrentDelay = 0;
					}

					m_siPrevSelectArmyKindIdx = m_siSelectArmyKindIdx;

					// 새로 선택한 병과이므로 애니메이션 갱신
					// 해당 병과에 대한 정보를 얻어온다
					LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siSelectArmyKindIdx);
					m_sAnimationButton[m_siSelectArmyKindIdx].m_scCurrentAnimationIdx = ANIBTN_MOVE;
					m_sAnimationButton[m_siSelectArmyKindIdx].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[ANIBTN_MOVE].m_siAniamtionStartFont;
					m_sAnimationButton[m_siSelectArmyKindIdx].m_scCurrentDelay = 0;
				}
			}
		}
	}

	// 선택한 병과가 있다.
	if (-1 != m_siSelectArmyKindIdx)
	{
		// 변신 버튼 클릭 처리
		if ((m_btnConfirm.Process(m_bMouseDown)) && (m_bDownloadedGuildCount))
		{
			// 해당 병과에 대한 정보를 얻어온다
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siSelectArmyKindIdx);

			// 대장유닛을 경우 비용은 두 배이다.
			SI08 scCaptainFactor = 1;

			if (NULL != lpsSiegeUnitTable)
			{
				// 명시된 캐릭터로 변신 가능한지 검사
				if (FALSE == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
				{
					// 에러 메시지 박스 출력
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100119));
					goto NEXT_STATEMENT;
				}
				// 사용할때 대방이거나 행수일경우엔 대장 유닛이 될수 있다.
				if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
						m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
				{
					// 대장유닛을 경우 비용은 두 배이다.
					scCaptainFactor = 2;

					// 명시된 캐릭터로 된 대장 유닛으로 변신 불가능(대방 및 행수 사용 불가능)
					if (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
					{
						// 에러 메시지 박스 출력
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100120));
						goto NEXT_STATEMENT;
					}
				}

				// 병력이 충분한가?
				if (m_siGuildCount[m_siSelectArmyKindIdx] < scCaptainFactor)
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100121));
					goto NEXT_STATEMENT;
				}

				// 해당 병과로 변신하기 위한 레벨이 되는지 검사
				if (m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) < lpsSiegeUnitTable->m_scTransformLevelLimit)
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100122));
					goto NEXT_STATEMENT;
				}

				// 해당 병과로 변신하기 위한 소지금이 되는지 검사
				if (m_pMyOnlineWorld->pMyData->GetMoney() < (lpsSiegeUnitTable->m_siTransformCost * scCaptainFactor))
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTMONEY));
					goto NEXT_STATEMENT;
				}

				// 변신 가능한지 검사
				if (FALSE == m_pMyOnlineWorld->SiegeWarfare_IsInAreaDisabledTransform())
				{
					// 공성 유닛으로 변신 요청
					m_pMyOnlineWorld->pOnlineClient->SendTransformToSiegeUnit(lpsSiegeUnitTable->m_uiKind);

					// 변신 메시지를 보냈다고 설정
					m_bSendedMsg = TRUE;

					return FALSE;
				}
			}
		}
	}

	// 취소(닫기) 버튼 처리
	if (m_btnCancel.Process(m_bMouseDown))
	{
		return FALSE;
	}

NEXT_STATEMENT:

	// 버튼 애니메이션 갱신
	UpdateAnimation();

	// 풍선 도움말 처리
	if (-1 != m_siHoverArmyKindIdx)
	{
		// 해당 병과에 대한 정보를 얻어온다
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siHoverArmyKindIdx);

		BOOL bCaptain = FALSE;

		// 대방이거나 행수일경우(대장 유닛)에는 대장 유닛 변신 가능
		if(m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)
			|| m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		{
			bCaptain = TRUE;
		}

		m_pMyOnlineWorld->pOnlineHelp->SetTextSiegeUnit(
				lpsSiegeUnitTable,
				ptMouse.x, ptMouse.y, 
				bCaptain,
				ON_HELP_LEFT | ON_HELP_UP);
	}

	if( IpD.LeftPressSwitch )
		m_bMouseDown	=	TRUE;
	else						
		m_bMouseDown	=	FALSE;

	return TRUE;
}

// 그리기
void	CSiegeWarfareTransformUIWnd::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if (FALSE == m_bVisibleWindow)
		return ;

	// 윈도 이미지들 그리기
	DrawImage(pSurface);

	// 윈도 텍스트들 그리기
	DrawText(pSurface);
}

// 그리기
void	CSiegeWarfareTransformUIWnd::DrawImage(LPDIRECTDRAWSURFACE7 pSurface)
{
	// 이미지들 출력
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		// 메인 이미지 출력
		clGrp.PutSpriteT(m_siX, m_siY, m_sprBackground.Header.Xsize, m_sprBackground.Header.Ysize, m_sprBackground.Image);

/*
		// 병과 초상화 그리기
		for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
		{
			// 수비병
			if (i == 4)
				continue;

			// actdoll (2004/01/07 16:33) : 
			//	약간 번거로운 방법이다.
			//	외부에서 서피스를 락킹하고 있는 경우에 초상을 블릿 명령으로 그냥 찍으려 하면 DDERR_SURFACEBUSY 에러가 계속 발생하게 된다.
			//	때문에 서피스가 락킹 중일 경우 넘어온 서피스에 대해 잠깐 해제를 하고 찍은 뒤 다시 잡도록 한다.
			//	락킹중인 서피스는 대부분 이쪽으로 넘겨주는 pSurface일 경우가 높기 때문에 이놈을 잡고 해제를 하도록 한다.
			//	만약 pSurface가 락킹 서피스가 아니라면 안찍힐것이다. 
			if( clGrp.IsLockSurface() )
			{
				RECT	rcClip;
				clGrp.GetClipRect( &rcClip );
				clGrp.UnlockSurface( pSurface );

				pSurface->Blt( &m_rtArmyKind[i], m_pSurfaceSprArmy, &m_rtSurfaceArmyKind[i], DDBLT_WAIT | DDBLT_KEYSRC, NULL );

				clGrp.SetClipArea( rcClip.left, rcClip.top, rcClip.right, rcClip.bottom );
				clGrp.LockSurface( pSurface );
			}
			else
			{
				pSurface->Blt( &m_rtArmyKind[i], m_pSurfaceSprArmy, &m_rtSurfaceArmyKind[i], DDBLT_WAIT | DDBLT_KEYSRC, NULL );
			}

			// 사용자가 선택한 병과라면 테두리를 그려준다.
			if (i == m_siSelectArmyKindIdx)
			{
				clGrp.Box(m_rtArmyKind[i].left - 1, m_rtArmyKind[i].top - 1, m_rtArmyKind[i].right + 1, m_rtArmyKind[i].bottom + 1, 0x27, 2);
			}
		}
*/
		// 병과 버튼 애니메이션 그리기
		for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
		{
			// 병과 설명 텍스트 인덱스 얻기
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

			if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
				continue;

			unsigned char *tempDarkTable = DarkTable[0];

			// 어둡게 찍어야한다면
			if (m_sAnimationButton[i].m_scGrayed)
				tempDarkTable = DarkTable[4];

			SI32 siDrawX = m_sAnimationButton[i].m_rtDraw.left + ((m_sAnimationButton[i].m_rtDraw.right - m_sAnimationButton[i].m_rtDraw.left) / 2);
			SI32 siDrawY = m_sAnimationButton[i].m_rtDraw.top + ((m_sAnimationButton[i].m_rtDraw.bottom - m_sAnimationButton[i].m_rtDraw.top) / 2);

			clGrp.PutSpriteT(siDrawX - (m_sAnimationButton[i].m_sprAnimation.Header.Xsize / 2),
								siDrawY - (m_sAnimationButton[i].m_sprAnimation.Header.Ysize / 2), 
								m_sAnimationButton[i].m_sprAnimation.Header.Xsize, m_sAnimationButton[i].m_sprAnimation.Header.Ysize,
								&m_sAnimationButton[i].m_sprAnimation.Image[m_sAnimationButton[i].m_sprAnimation.Header.Start[m_sAnimationButton[i].m_ssCurrentFont]], tempDarkTable);

			/*
			// 사용자가 선택한 병과라면 테두리를 그려준다.
			if (i == m_siSelectArmyKindIdx)
			{
				clGrp.Box(m_sAnimationButton[i].m_rtDraw.left - 1, m_sAnimationButton[i].m_rtDraw.top - 1, m_sAnimationButton[i].m_rtDraw.right + 1, m_sAnimationButton[i].m_rtDraw.bottom + 1, 0x27, 2);
			}
			*/
			// 사용자가 선택한 병과라면
			if (i == m_siSelectArmyKindIdx)
			{
				clGrp.PutSpriteT(siDrawX - (m_sprSelect.Header.Xsize / 2),
									siDrawY - (m_sprSelect.Header.Ysize / 2), 
									m_sprSelect.Header.Xsize, m_sprSelect.Header.Ysize,
									m_sprSelect.Image);
			}
		}

		// 변신 버튼 그리기
		m_btnConfirm.Put(&m_sprOkCancel, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 취소 버튼 그리기
		m_btnCancel.Put(&m_sprOkCancel, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}
}

// 그리기
void	CSiegeWarfareTransformUIWnd::DrawText(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC			hDC;

	// 텍스트 출력
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// 타이틀 텍스트 출력
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 257, m_siY + 7, 118, 13, m_szTitle, RGB(250, 250, 250));

		// 병과별 개수
		for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
		{
			// 병과 설명 텍스트 인덱스 얻기
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

			if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
				continue;

			// comma가 들어간 숫자 문자열로 변형
			char szCount[16];
			MakeMoneyText(m_siGuildCount[i], szCount);

			m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox(hDC, m_sAnimationButton[i].m_rtDraw.left, m_sAnimationButton[i].m_rtDraw.top, m_sAnimationButton[i].m_rtDraw.right - m_sAnimationButton[i].m_rtDraw.left - 4, m_sAnimationButton[i].m_rtDraw.bottom - m_sAnimationButton[i].m_rtDraw.top, NULL, szCount, RGB(0, 255, 0), DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
		}

		/*
		// 선택한 병과 설명 텍스트 출력
		if (-1 != m_siSelectArmyKindIdx)
		{
			// 병과 설명 텍스트 인덱스 얻기
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siSelectArmyKindIdx);

			if (lpsSiegeUnitTable)
			{
				char szBuffer[2048];
				sprintf(szBuffer, "%s\n\n%s",
								m_pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siUnitNameTextIdx),
								m_pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siArmyKindDescTextID));

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox(hDC, 0, 0, 0, 0, &m_rtArmyDesc, 
															szBuffer,
															RGB(250, 250, 250));
			}
		}
		*/

		// 변신 버튼
		m_btnConfirm.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 취소 버튼
		m_btnCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hDC );
	}
}

// 버튼 갱신
void	CSiegeWarfareTransformUIWnd::UpdateButton(void)
{
	// 병과 선택 버튼들 갱신
	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// 대장유닛을 경우 비용은 두 배이다.
		SI08 scCaptainFactor = 1;

		// 해당 병과에 대한 정보를 얻어온다
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		// 명시된 캐릭터로 변신 가능한지 검사
		if (FALSE == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
		{
			// 비활성화
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}
		// 사용할때 대방이거나 행수일경우엔 대장 유닛이 될수 있다.
		if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
				m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		{
			// 대장유닛을 경우 비용은 두 배이다.
			scCaptainFactor = 2;

			// 명시된 캐릭터로 된 대장 유닛으로 변신 불가능(대방 및 행수 사용 불가능)
			if (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
			{
				// 비활성화
				m_sAnimationButton[i].m_scGrayed = 1;
				continue;
			}
		}

		// 병력이 충분한지 검사
		if (m_siGuildCount[i] < scCaptainFactor)
		{
			// 비활성화
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}

		// 해당 병과로 변신하기 위한 레벨이 되는지 검사
		if (m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) < lpsSiegeUnitTable->m_scTransformLevelLimit)
		{
			// 비활성화
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}

		// 해당 병과로 변신하기 위한 소지금이 되는지 검사
		if (m_pMyOnlineWorld->pMyData->GetMoney() < (lpsSiegeUnitTable->m_siTransformCost * scCaptainFactor))
		{
			// 비활성화
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}

		// 비활성화
		m_sAnimationButton[i].m_scGrayed = 0;
	}
}

// 리소스 읽기
void	CSiegeWarfareTransformUIWnd::LoadResource(void)
{
	// 배경 이미지 로드
	clGrp.LoadXspr("Online\\GameSpr\\Book\\SIEGE_TRANSFER_BASE.Spr", m_sprBackground);

	// 변신 및 취소 버튼 이미지 로드
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_sprOkCancel);

	// 선택한 병과 위에 덧그림 이미지 로드
	clGrp.LoadXspr("Online\\GameSpr\\Book\\SIEGE_TRANSFER_SELECT.Spr", m_sprSelect);

/*
	XSPR sprArmyKind;

	// 병과 이미지 로드
	clGrp.LoadXspr("Online\\GameSpr\\mercenary\\PORTRAIT_NPC.Spr", sprArmyKind);

	// 한 폰트에 해당되는 이미지의 크기
	m_siPortraitWidth	= sprArmyKind.Header.Xsize;
	m_siPortraitHeight	= sprArmyKind.Header.Ysize;

	SI32					ClipLeft, ClipTop, ClipRight, ClipBottom;

	clGrp.CreateOffScreen	( m_pSurfaceSprArmy, m_siPortraitWidth * (ON_MAX_GUILD_UNIT - 1), m_siPortraitHeight, TRUE, FALSE );	// 서피스 생성
	clGrp.ClearSurface		( m_pSurfaceSprArmy );		// 서피스 클린

	// 그림을 서피스에 복사
	clGrp.GetClipArea		( ClipLeft, ClipTop, ClipRight, ClipBottom );

	if(clGrp.LockSurface( m_pSurfaceSprArmy ))
	{
		for (SI32 i = 0; i < (ON_MAX_GUILD_UNIT - 1); i++)
		{
			// 복사 구획 잡고
			clGrp.SetClipArea	( i * m_siPortraitWidth, 0, 
									((i + 1) * m_siPortraitWidth) - 1,	m_siPortraitHeight - 1 );
			
			// 컬러키까지 복사하는 구문이 없기 때문에 배경을 컬러키로 한번 칠해준다.
			clGrp.FillBox		( i * m_siPortraitWidth, 0,
									((i + 1) * m_siPortraitWidth) - 1,	m_siPortraitHeight - 1,
									TRANSCOLOR );

			// 그림 복사
			clGrp.PutSpriteT	( i * m_siPortraitWidth, 0, m_siPortraitWidth, m_siPortraitHeight,	
									&sprArmyKind.Image[sprArmyKind.Header.Start[i + 1]] );
		}

		clGrp.UnlockSurface	( m_pSurfaceSprArmy );
	}

	clGrp.SetClipArea( ClipLeft, ClipTop, ClipRight, ClipBottom );

	clGrp.FreeXspr(sprArmyKind);
*/
	m_siX = 84;
	m_siY = 5;

/*
	// 서페이스상 병과 영역(크기 및 위치) 설정
	SI32 left = 0;
	SI32 top = 0;
	SI32 right = left + m_siPortraitWidth;
	SI32 bottom = top + m_siPortraitHeight;

	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// 수비병 제외
		if (i == 4)
			continue;

		SetRect(&m_rtSurfaceArmyKind[i], left, top, right, bottom);
		left = right;
		right = left + m_siPortraitWidth;
	}

	SI32 siRealWidth = 54;
	SI32 siRealHeight = 54;

	// 병과 선택 버튼들 생성
	left = m_siX + 41;
	top = m_siY + 92;
	right = left + siRealWidth;
	bottom = top + siRealHeight;

	for (i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// 수비병 제외
		if (i == 4)
			continue;

		SetRect(&m_rtArmyKind[i], left, top, right, bottom);
		left = right + 12;
		right = left + siRealWidth;
	}
*/
	SI32 siRealWidth = 80;
	SI32 siRealHeight = 80;

	// 병과 선택 버튼들 생성
	SI32 left = m_siX + 18;
	SI32 top = m_siY + 40;
	SI32 right = left + siRealWidth;
	SI32 bottom = top + siRealHeight;

	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// 해당 병과에 대한 정보를 얻어온다
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
			continue;

		SetRect(&m_sAnimationButton[i].m_rtDraw, left, top, right, bottom);
		left = right + 6;
		right = left + siRealWidth;

		if (strcmp("NULL", lpsSiegeUnitTable->m_sAniamtionInfo[0].m_szAnimationFilename) != 0)
			clGrp.LoadXspr(lpsSiegeUnitTable->m_sAniamtionInfo[0].m_szAnimationFilename, m_sAnimationButton[i].m_sprAnimation);

		m_sAnimationButton[i].m_scCurrentAnimationIdx = ANIBTN_WAIT;	// 현재 그려지는 애니메이션 인덱스. 대기
		m_sAnimationButton[i].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionStartFont;			// 현재 그려지는 폰트 번호
		m_sAnimationButton[i].m_scCurrentDelay = 0;			// 애니메이션 딜레이
		m_sAnimationButton[i].m_scGrayed = 1;
	}

	// 변신 버튼 생성
	m_btnConfirm.Create(m_siX + 181, m_siY + 135, m_sprOkCancel.Header.Xsize, m_sprOkCancel.Header.Ysize,
							m_pMyOnlineWorld->pOnlineText->Get(8100107),
							BUTTON_PUT_LEFT, TRUE);

	// 취소 버튼 생성
	m_btnCancel.Create(m_siX + 371, m_siY + 135, m_sprOkCancel.Header.Xsize, m_sprOkCancel.Header.Ysize,
							m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_CANCEL),
							BUTTON_PUT_LEFT, TRUE);

	m_bMouseDown			= FALSE;

	m_siSelectArmyKindIdx		= -1;
	m_siPrevSelectArmyKindIdx	= -1;
	m_siHoverArmyKindIdx		= -1;

	// 윈도 타이틀
	sprintf(m_szTitle, "%s", m_pMyOnlineWorld->pOnlineText->Get(8100124));

	// 병과 설명 텍스트 박스
	SetRect(&m_rtArmyDesc, m_siX + 33, m_siY + 219, m_siX + 33 + 466, m_siY + 219 + 88);
}

// 리소스 해제
void	CSiegeWarfareTransformUIWnd::FreeResource(void)
{
	// 배경 이미지 삭제
	clGrp.FreeXspr(m_sprBackground);
/*
	if (m_pSurfaceSprArmy)
	{
		// 병과선택 버튼 이미지 삭제
		m_pSurfaceSprArmy->Release();
		m_pSurfaceSprArmy = NULL;
	}
*/
	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		clGrp.FreeXspr(m_sAnimationButton[i].m_sprAnimation);
	}

	// 변신 및 취소 버튼 이미지 삭제
	clGrp.FreeXspr(m_sprOkCancel);

	// 선택한 병과 위에 덧그림 이미지 해제
	clGrp.FreeXspr(m_sprSelect);
}

// 버튼 애니메이션 갱신
void	CSiegeWarfareTransformUIWnd::UpdateAnimation(void)
{
	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// 해당 병과에 대한 정보를 얻어온다
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
			continue;

		if (m_sAnimationButton[i].m_scCurrentDelay > lpsSiegeUnitTable->m_sAniamtionInfo[m_sAnimationButton[i].m_scCurrentAnimationIdx].m_siAnimationDelay)
		{
			m_sAnimationButton[i].m_ssCurrentFont++;
			m_sAnimationButton[i].m_scCurrentDelay = 0;

			if (m_sAnimationButton[i].m_ssCurrentFont >= 
				(lpsSiegeUnitTable->m_sAniamtionInfo[m_sAnimationButton[i].m_scCurrentAnimationIdx].m_siAniamtionStartFont + lpsSiegeUnitTable->m_sAniamtionInfo[m_sAnimationButton[i].m_scCurrentAnimationIdx].m_siAniamtionFontNumber))
			{
				m_sAnimationButton[i].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[m_sAnimationButton[i].m_scCurrentAnimationIdx].m_siAniamtionStartFont;
			}
		}
		else
		{
			m_sAnimationButton[i].m_scCurrentDelay++;
		}
	}
}

// 윈도 보이는가?
BOOL	CSiegeWarfareTransformUIWnd::IsVisibleWindow(void)
{
	return m_bVisibleWindow;
}

// 공성전 상단 공성 유닛 리스트 요청 응답
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 CSiegeWarfareTransformUIWnd::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;

	ZeroMemory(m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	// 공성 검병 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN];
	// 공성 창병 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN];
	// 공성 궁병 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN];
	// 공성 기마병 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN];
	// 공성 충차 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER]  = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER];
	// 공성 발석거 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT];
	// 공성 보급수레 부대수 설정
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON];

	m_bDownloadedGuildCount = TRUE;

	return 0;
}

// 변신 메시지를 서버에 전송했는가?
BOOL CSiegeWarfareTransformUIWnd::IsSendedMsg(void)
{
	return m_bSendedMsg;
}

// 변신 메시지를 서버에 전송했는가를 설정
void CSiegeWarfareTransformUIWnd::SetSendedMsg(BOOL bSendedMsg)
{
	m_bSendedMsg = bSendedMsg;
}
