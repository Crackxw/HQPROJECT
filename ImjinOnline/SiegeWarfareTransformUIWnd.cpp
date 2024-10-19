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

#include	"SiegeWarfareTransformUIWnd.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern _InputDevice				IpD;

extern	unsigned char				DarkTable[][256];					// ��ο� �ܰ� 

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

// �ʱ�ȭ
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

// ���� ���̱� ����
BOOL	CSiegeWarfareTransformUIWnd::ShowWindow(BOOL bVisibleWindow)
{
	m_bVisibleWindow = bVisibleWindow;

	// �̹� ���� �޽����� ���� ���
	if (TRUE == m_bSendedMsg)
	{
		if (TRUE == m_bVisibleWindow)
		{
			m_bVisibleWindow = FALSE;

			return FALSE;
		}
	}

	// Ȱ��ȭ
	if (FALSE == m_bVisibleWindow)
	{
		// ���ҽ� ����
		FreeResource();

		if (-1 != m_siNoFieldAreaIdx)
		{
			// No Field Area ����
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaIdx);
			m_siNoFieldAreaIdx = -1;
		}
	}
	else
	{
		// ��� �� ���� ���� ��� ��û
		m_pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
		m_bDownloadedGuildCount = FALSE;

		LoadResource();

		if (-1 == m_siNoFieldAreaIdx)
		{
			// No Field Area ����
			m_siNoFieldAreaIdx = m_pMyOnlineWorld->
				pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY,
													m_siX + m_sprBackground.Header.Xsize,
													m_siY + m_sprBackground.Header.Ysize);
		}
	}

	return TRUE;
}

// ó��
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
		// ���� �ִϸ��̼� ��ư ����
		UpdateButton();
	}

	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// ���� ���� �ؽ�Ʈ �ε��� ���
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
			continue;

		// ��� ���� �̹��� ���� ��ư�� �ִ°�?
		if (PtInRect(&m_sAnimationButton[i].m_rtDraw, ptMouse))
		{
			m_siHoverArmyKindIdx = i;

			if (m_bMouseDown)
			{
				m_siSelectArmyKindIdx = i;

				// ������ �����ߴ� ������ �ƴ϶��
				if (m_siPrevSelectArmyKindIdx != m_siSelectArmyKindIdx)
				{
					// ���� �� �ִϸ��̼� ����
					if (-1 != m_siPrevSelectArmyKindIdx)
					{
						// �ش� ������ ���� ������ ���´�
						LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siPrevSelectArmyKindIdx);
						m_sAnimationButton[m_siPrevSelectArmyKindIdx].m_scCurrentAnimationIdx = ANIBTN_WAIT;
						m_sAnimationButton[m_siPrevSelectArmyKindIdx].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionStartFont;
						m_sAnimationButton[m_siPrevSelectArmyKindIdx].m_scCurrentDelay = 0;
					}

					m_siPrevSelectArmyKindIdx = m_siSelectArmyKindIdx;

					// ���� ������ �����̹Ƿ� �ִϸ��̼� ����
					// �ش� ������ ���� ������ ���´�
					LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siSelectArmyKindIdx);
					m_sAnimationButton[m_siSelectArmyKindIdx].m_scCurrentAnimationIdx = ANIBTN_MOVE;
					m_sAnimationButton[m_siSelectArmyKindIdx].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[ANIBTN_MOVE].m_siAniamtionStartFont;
					m_sAnimationButton[m_siSelectArmyKindIdx].m_scCurrentDelay = 0;
				}
			}
		}
	}

	// ������ ������ �ִ�.
	if (-1 != m_siSelectArmyKindIdx)
	{
		// ���� ��ư Ŭ�� ó��
		if ((m_btnConfirm.Process(m_bMouseDown)) && (m_bDownloadedGuildCount))
		{
			// �ش� ������ ���� ������ ���´�
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siSelectArmyKindIdx);

			// ���������� ��� ����� �� ���̴�.
			SI08 scCaptainFactor = 1;

			if (NULL != lpsSiegeUnitTable)
			{
				// ��õ� ĳ���ͷ� ���� �������� �˻�
				if (FALSE == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
				{
					// ���� �޽��� �ڽ� ���
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100119));
					goto NEXT_STATEMENT;
				}
				// ����Ҷ� ����̰ų� ����ϰ�쿣 ���� ������ �ɼ� �ִ�.
				if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
						m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
				{
					// ���������� ��� ����� �� ���̴�.
					scCaptainFactor = 2;

					// ��õ� ĳ���ͷ� �� ���� �������� ���� �Ұ���(��� �� ��� ��� �Ұ���)
					if (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
					{
						// ���� �޽��� �ڽ� ���
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100120));
						goto NEXT_STATEMENT;
					}
				}

				// ������ ����Ѱ�?
				if (m_siGuildCount[m_siSelectArmyKindIdx] < scCaptainFactor)
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100121));
					goto NEXT_STATEMENT;
				}

				// �ش� ������ �����ϱ� ���� ������ �Ǵ��� �˻�
				if (m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) < lpsSiegeUnitTable->m_scTransformLevelLimit)
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100122));
					goto NEXT_STATEMENT;
				}

				// �ش� ������ �����ϱ� ���� �������� �Ǵ��� �˻�
				if (m_pMyOnlineWorld->pMyData->GetMoney() < (lpsSiegeUnitTable->m_siTransformCost * scCaptainFactor))
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTMONEY));
					goto NEXT_STATEMENT;
				}

				// ���� �������� �˻�
				if (FALSE == m_pMyOnlineWorld->SiegeWarfare_IsInAreaDisabledTransform())
				{
					// ���� �������� ���� ��û
					m_pMyOnlineWorld->pOnlineClient->SendTransformToSiegeUnit(lpsSiegeUnitTable->m_uiKind);

					// ���� �޽����� ���´ٰ� ����
					m_bSendedMsg = TRUE;

					return FALSE;
				}
			}
		}
	}

	// ���(�ݱ�) ��ư ó��
	if (m_btnCancel.Process(m_bMouseDown))
	{
		return FALSE;
	}

NEXT_STATEMENT:

	// ��ư �ִϸ��̼� ����
	UpdateAnimation();

	// ǳ�� ���� ó��
	if (-1 != m_siHoverArmyKindIdx)
	{
		// �ش� ������ ���� ������ ���´�
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_siHoverArmyKindIdx);

		BOOL bCaptain = FALSE;

		// ����̰ų� ����ϰ��(���� ����)���� ���� ���� ���� ����
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

// �׸���
void	CSiegeWarfareTransformUIWnd::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if (FALSE == m_bVisibleWindow)
		return ;

	// ���� �̹����� �׸���
	DrawImage(pSurface);

	// ���� �ؽ�Ʈ�� �׸���
	DrawText(pSurface);
}

// �׸���
void	CSiegeWarfareTransformUIWnd::DrawImage(LPDIRECTDRAWSURFACE7 pSurface)
{
	// �̹����� ���
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		// ���� �̹��� ���
		clGrp.PutSpriteT(m_siX, m_siY, m_sprBackground.Header.Xsize, m_sprBackground.Header.Ysize, m_sprBackground.Image);

/*
		// ���� �ʻ�ȭ �׸���
		for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
		{
			// ����
			if (i == 4)
				continue;

			// actdoll (2004/01/07 16:33) : 
			//	�ణ ���ŷο� ����̴�.
			//	�ܺο��� ���ǽ��� ��ŷ�ϰ� �ִ� ��쿡 �ʻ��� �� ������� �׳� ������ �ϸ� DDERR_SURFACEBUSY ������ ��� �߻��ϰ� �ȴ�.
			//	������ ���ǽ��� ��ŷ ���� ��� �Ѿ�� ���ǽ��� ���� ��� ������ �ϰ� ���� �� �ٽ� �⵵�� �Ѵ�.
			//	��ŷ���� ���ǽ��� ��κ� �������� �Ѱ��ִ� pSurface�� ��찡 ���� ������ �̳��� ��� ������ �ϵ��� �Ѵ�.
			//	���� pSurface�� ��ŷ ���ǽ��� �ƴ϶�� ���������̴�. 
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

			// ����ڰ� ������ ������� �׵θ��� �׷��ش�.
			if (i == m_siSelectArmyKindIdx)
			{
				clGrp.Box(m_rtArmyKind[i].left - 1, m_rtArmyKind[i].top - 1, m_rtArmyKind[i].right + 1, m_rtArmyKind[i].bottom + 1, 0x27, 2);
			}
		}
*/
		// ���� ��ư �ִϸ��̼� �׸���
		for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
		{
			// ���� ���� �ؽ�Ʈ �ε��� ���
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

			if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
				continue;

			unsigned char *tempDarkTable = DarkTable[0];

			// ��Ӱ� �����Ѵٸ�
			if (m_sAnimationButton[i].m_scGrayed)
				tempDarkTable = DarkTable[4];

			SI32 siDrawX = m_sAnimationButton[i].m_rtDraw.left + ((m_sAnimationButton[i].m_rtDraw.right - m_sAnimationButton[i].m_rtDraw.left) / 2);
			SI32 siDrawY = m_sAnimationButton[i].m_rtDraw.top + ((m_sAnimationButton[i].m_rtDraw.bottom - m_sAnimationButton[i].m_rtDraw.top) / 2);

			clGrp.PutSpriteT(siDrawX - (m_sAnimationButton[i].m_sprAnimation.Header.Xsize / 2),
								siDrawY - (m_sAnimationButton[i].m_sprAnimation.Header.Ysize / 2), 
								m_sAnimationButton[i].m_sprAnimation.Header.Xsize, m_sAnimationButton[i].m_sprAnimation.Header.Ysize,
								&m_sAnimationButton[i].m_sprAnimation.Image[m_sAnimationButton[i].m_sprAnimation.Header.Start[m_sAnimationButton[i].m_ssCurrentFont]], tempDarkTable);

			/*
			// ����ڰ� ������ ������� �׵θ��� �׷��ش�.
			if (i == m_siSelectArmyKindIdx)
			{
				clGrp.Box(m_sAnimationButton[i].m_rtDraw.left - 1, m_sAnimationButton[i].m_rtDraw.top - 1, m_sAnimationButton[i].m_rtDraw.right + 1, m_sAnimationButton[i].m_rtDraw.bottom + 1, 0x27, 2);
			}
			*/
			// ����ڰ� ������ �������
			if (i == m_siSelectArmyKindIdx)
			{
				clGrp.PutSpriteT(siDrawX - (m_sprSelect.Header.Xsize / 2),
									siDrawY - (m_sprSelect.Header.Ysize / 2), 
									m_sprSelect.Header.Xsize, m_sprSelect.Header.Ysize,
									m_sprSelect.Image);
			}
		}

		// ���� ��ư �׸���
		m_btnConfirm.Put(&m_sprOkCancel, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// ��� ��ư �׸���
		m_btnCancel.Put(&m_sprOkCancel, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}
}

// �׸���
void	CSiegeWarfareTransformUIWnd::DrawText(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC			hDC;

	// �ؽ�Ʈ ���
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// Ÿ��Ʋ �ؽ�Ʈ ���
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 257, m_siY + 7, 118, 13, m_szTitle, RGB(250, 250, 250));

		// ������ ����
		for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
		{
			// ���� ���� �ؽ�Ʈ �ε��� ���
			LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

			if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
				continue;

			// comma�� �� ���� ���ڿ��� ����
			char szCount[16];
			MakeMoneyText(m_siGuildCount[i], szCount);

			m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox(hDC, m_sAnimationButton[i].m_rtDraw.left, m_sAnimationButton[i].m_rtDraw.top, m_sAnimationButton[i].m_rtDraw.right - m_sAnimationButton[i].m_rtDraw.left - 4, m_sAnimationButton[i].m_rtDraw.bottom - m_sAnimationButton[i].m_rtDraw.top, NULL, szCount, RGB(0, 255, 0), DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
		}

		/*
		// ������ ���� ���� �ؽ�Ʈ ���
		if (-1 != m_siSelectArmyKindIdx)
		{
			// ���� ���� �ؽ�Ʈ �ε��� ���
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

		// ���� ��ư
		m_btnConfirm.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// ��� ��ư
		m_btnCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hDC );
	}
}

// ��ư ����
void	CSiegeWarfareTransformUIWnd::UpdateButton(void)
{
	// ���� ���� ��ư�� ����
	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// ���������� ��� ����� �� ���̴�.
		SI08 scCaptainFactor = 1;

		// �ش� ������ ���� ������ ���´�
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		// ��õ� ĳ���ͷ� ���� �������� �˻�
		if (FALSE == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
		{
			// ��Ȱ��ȭ
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}
		// ����Ҷ� ����̰ų� ����ϰ�쿣 ���� ������ �ɼ� �ִ�.
		if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
				m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		{
			// ���������� ��� ����� �� ���̴�.
			scCaptainFactor = 2;

			// ��õ� ĳ���ͷ� �� ���� �������� ���� �Ұ���(��� �� ��� ��� �Ұ���)
			if (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
			{
				// ��Ȱ��ȭ
				m_sAnimationButton[i].m_scGrayed = 1;
				continue;
			}
		}

		// ������ ������� �˻�
		if (m_siGuildCount[i] < scCaptainFactor)
		{
			// ��Ȱ��ȭ
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}

		// �ش� ������ �����ϱ� ���� ������ �Ǵ��� �˻�
		if (m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) < lpsSiegeUnitTable->m_scTransformLevelLimit)
		{
			// ��Ȱ��ȭ
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}

		// �ش� ������ �����ϱ� ���� �������� �Ǵ��� �˻�
		if (m_pMyOnlineWorld->pMyData->GetMoney() < (lpsSiegeUnitTable->m_siTransformCost * scCaptainFactor))
		{
			// ��Ȱ��ȭ
			m_sAnimationButton[i].m_scGrayed = 1;
			continue;
		}

		// ��Ȱ��ȭ
		m_sAnimationButton[i].m_scGrayed = 0;
	}
}

// ���ҽ� �б�
void	CSiegeWarfareTransformUIWnd::LoadResource(void)
{
	// ��� �̹��� �ε�
	clGrp.LoadXspr("Online\\GameSpr\\Book\\SIEGE_TRANSFER_BASE.Spr", m_sprBackground);

	// ���� �� ��� ��ư �̹��� �ε�
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_sprOkCancel);

	// ������ ���� ���� ���׸� �̹��� �ε�
	clGrp.LoadXspr("Online\\GameSpr\\Book\\SIEGE_TRANSFER_SELECT.Spr", m_sprSelect);

/*
	XSPR sprArmyKind;

	// ���� �̹��� �ε�
	clGrp.LoadXspr("Online\\GameSpr\\mercenary\\PORTRAIT_NPC.Spr", sprArmyKind);

	// �� ��Ʈ�� �ش�Ǵ� �̹����� ũ��
	m_siPortraitWidth	= sprArmyKind.Header.Xsize;
	m_siPortraitHeight	= sprArmyKind.Header.Ysize;

	SI32					ClipLeft, ClipTop, ClipRight, ClipBottom;

	clGrp.CreateOffScreen	( m_pSurfaceSprArmy, m_siPortraitWidth * (ON_MAX_GUILD_UNIT - 1), m_siPortraitHeight, TRUE, FALSE );	// ���ǽ� ����
	clGrp.ClearSurface		( m_pSurfaceSprArmy );		// ���ǽ� Ŭ��

	// �׸��� ���ǽ��� ����
	clGrp.GetClipArea		( ClipLeft, ClipTop, ClipRight, ClipBottom );

	if(clGrp.LockSurface( m_pSurfaceSprArmy ))
	{
		for (SI32 i = 0; i < (ON_MAX_GUILD_UNIT - 1); i++)
		{
			// ���� ��ȹ ���
			clGrp.SetClipArea	( i * m_siPortraitWidth, 0, 
									((i + 1) * m_siPortraitWidth) - 1,	m_siPortraitHeight - 1 );
			
			// �÷�Ű���� �����ϴ� ������ ���� ������ ����� �÷�Ű�� �ѹ� ĥ���ش�.
			clGrp.FillBox		( i * m_siPortraitWidth, 0,
									((i + 1) * m_siPortraitWidth) - 1,	m_siPortraitHeight - 1,
									TRANSCOLOR );

			// �׸� ����
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
	// �����̽��� ���� ����(ũ�� �� ��ġ) ����
	SI32 left = 0;
	SI32 top = 0;
	SI32 right = left + m_siPortraitWidth;
	SI32 bottom = top + m_siPortraitHeight;

	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// ���� ����
		if (i == 4)
			continue;

		SetRect(&m_rtSurfaceArmyKind[i], left, top, right, bottom);
		left = right;
		right = left + m_siPortraitWidth;
	}

	SI32 siRealWidth = 54;
	SI32 siRealHeight = 54;

	// ���� ���� ��ư�� ����
	left = m_siX + 41;
	top = m_siY + 92;
	right = left + siRealWidth;
	bottom = top + siRealHeight;

	for (i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// ���� ����
		if (i == 4)
			continue;

		SetRect(&m_rtArmyKind[i], left, top, right, bottom);
		left = right + 12;
		right = left + siRealWidth;
	}
*/
	SI32 siRealWidth = 80;
	SI32 siRealHeight = 80;

	// ���� ���� ��ư�� ����
	SI32 left = m_siX + 18;
	SI32 top = m_siY + 40;
	SI32 right = left + siRealWidth;
	SI32 bottom = top + siRealHeight;

	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// �ش� ������ ���� ������ ���´�
		LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(i);

		if (0 == (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
			continue;

		SetRect(&m_sAnimationButton[i].m_rtDraw, left, top, right, bottom);
		left = right + 6;
		right = left + siRealWidth;

		if (strcmp("NULL", lpsSiegeUnitTable->m_sAniamtionInfo[0].m_szAnimationFilename) != 0)
			clGrp.LoadXspr(lpsSiegeUnitTable->m_sAniamtionInfo[0].m_szAnimationFilename, m_sAnimationButton[i].m_sprAnimation);

		m_sAnimationButton[i].m_scCurrentAnimationIdx = ANIBTN_WAIT;	// ���� �׷����� �ִϸ��̼� �ε���. ���
		m_sAnimationButton[i].m_ssCurrentFont = lpsSiegeUnitTable->m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionStartFont;			// ���� �׷����� ��Ʈ ��ȣ
		m_sAnimationButton[i].m_scCurrentDelay = 0;			// �ִϸ��̼� ������
		m_sAnimationButton[i].m_scGrayed = 1;
	}

	// ���� ��ư ����
	m_btnConfirm.Create(m_siX + 181, m_siY + 135, m_sprOkCancel.Header.Xsize, m_sprOkCancel.Header.Ysize,
							m_pMyOnlineWorld->pOnlineText->Get(8100107),
							BUTTON_PUT_LEFT, TRUE);

	// ��� ��ư ����
	m_btnCancel.Create(m_siX + 371, m_siY + 135, m_sprOkCancel.Header.Xsize, m_sprOkCancel.Header.Ysize,
							m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_CANCEL),
							BUTTON_PUT_LEFT, TRUE);

	m_bMouseDown			= FALSE;

	m_siSelectArmyKindIdx		= -1;
	m_siPrevSelectArmyKindIdx	= -1;
	m_siHoverArmyKindIdx		= -1;

	// ���� Ÿ��Ʋ
	sprintf(m_szTitle, "%s", m_pMyOnlineWorld->pOnlineText->Get(8100124));

	// ���� ���� �ؽ�Ʈ �ڽ�
	SetRect(&m_rtArmyDesc, m_siX + 33, m_siY + 219, m_siX + 33 + 466, m_siY + 219 + 88);
}

// ���ҽ� ����
void	CSiegeWarfareTransformUIWnd::FreeResource(void)
{
	// ��� �̹��� ����
	clGrp.FreeXspr(m_sprBackground);
/*
	if (m_pSurfaceSprArmy)
	{
		// �������� ��ư �̹��� ����
		m_pSurfaceSprArmy->Release();
		m_pSurfaceSprArmy = NULL;
	}
*/
	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		clGrp.FreeXspr(m_sAnimationButton[i].m_sprAnimation);
	}

	// ���� �� ��� ��ư �̹��� ����
	clGrp.FreeXspr(m_sprOkCancel);

	// ������ ���� ���� ���׸� �̹��� ����
	clGrp.FreeXspr(m_sprSelect);
}

// ��ư �ִϸ��̼� ����
void	CSiegeWarfareTransformUIWnd::UpdateAnimation(void)
{
	for (SI32 i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		// �ش� ������ ���� ������ ���´�
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

// ���� ���̴°�?
BOOL	CSiegeWarfareTransformUIWnd::IsVisibleWindow(void)
{
	return m_bVisibleWindow;
}

// ������ ��� ���� ���� ����Ʈ ��û ����
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 CSiegeWarfareTransformUIWnd::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;

	ZeroMemory(m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	// ���� �˺� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN];
	// ���� â�� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN];
	// ���� �ú� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN];
	// ���� �⸶�� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN];
	// ���� ���� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER]  = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER];
	// ���� �߼��� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT]	 = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT];
	// ���� ���޼��� �δ�� ����
	m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON];

	m_bDownloadedGuildCount = TRUE;

	return 0;
}

// ���� �޽����� ������ �����ߴ°�?
BOOL CSiegeWarfareTransformUIWnd::IsSendedMsg(void)
{
	return m_bSendedMsg;
}

// ���� �޽����� ������ �����ߴ°��� ����
void CSiegeWarfareTransformUIWnd::SetSendedMsg(BOOL bSendedMsg)
{
	m_bSendedMsg = bSendedMsg;
}
