#include <GSL.h>

#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineClient.h"
#include "OnlineResource.h"
#include "OnlineFieldArea.h"
#include "OnlineImoticon.h"

extern _InputDevice			IpD;


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineImoticon::OnlineImoticon()
{
	m_lpWorld     = NULL;
	m_fRenderList = FALSE;

	siNoFieldHandle = -1;

	ZeroMemory(m_UseImoticonList, sizeof(UI16) * 9);

	m_lpPannel = NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineImoticon::~OnlineImoticon()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineImoticon::Init(cltOnlineWorld* lpWorld)
{
	SI32	TempCount;

	m_lpWorld     = lpWorld;
	m_fRenderList = FALSE;

	siNoFieldHandle = -1;

	ZeroMemory(m_UseImoticonList, sizeof(UI16) * 9);

	m_lpPannel = m_lpWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_IMOTICON_SLOT);

	// ����ϴ� Imoticon ��(���� ���⼭ ���� ����Ҽ� �ִ� Imoticon�� ���� �۾��� �Ѵ�. ������ ������ �ӽ÷� �����Ƿ� �Ѵ�.)
	for(TempCount = 0; TempCount < 9; TempCount++)
	{
		m_UseImoticonList[TempCount] = TempCount;
//		if(TempCount == 8) m_UseImoticonList[TempCount] = 0xfff;
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineImoticon::Free(void)
{
	m_lpWorld     = NULL;
	m_fRenderList = FALSE;

	ZeroMemory(m_UseImoticonList, sizeof(UI16) * 9);

	m_lpPannel = NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�̸�Ƽ�� ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineImoticon::SetImoticonKey(SI16 ImoticonKey)
{
	UI16			ImoticonKind;

	if(m_lpWorld != NULL)
	{
		ImoticonKind = 0;

		ImoticonKind = m_UseImoticonList[ImoticonKey];
		if(ImoticonKind != 0xfff)
		{
			m_lpWorld->pOnlineClient->SendRequestImoticon(ImoticonKind);
			m_lpWorld->pIOnlineChar->SetImoticon(m_lpWorld->pMyData->GetMyCharUniqueID(), ImoticonKind);
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�̸�Ƽ�� Active
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineImoticon::SetActive(BOOL fActive)
{
	if(fActive == FALSE)
	{
		// �������
		if(siNoFieldHandle != -1)
		{
			m_lpWorld->pOnlineFieldArea->SetFieldArea(siNoFieldHandle);
			siNoFieldHandle = -1;
		}
	}
	else
	{
		// ��Ÿ����
		if(siNoFieldHandle != -1)
		{
			m_lpWorld->pOnlineFieldArea->SetFieldArea(siNoFieldHandle);
			siNoFieldHandle = -1;
		}
		siNoFieldHandle = m_lpWorld->pOnlineFieldArea->SetNoFieldArea(598, 505 - ((m_lpPannel->Header.Ysize - 5) * 3), 598 + ((m_lpPannel->Header.Xsize - 2) * 3), 505);
	}

	m_fRenderList = fActive;

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Active ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineImoticon::IsActive(void)
{
	return m_fRenderList;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ó��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineImoticon::Pool(BOOL fLMouseUp)
{
	int			TempCount;
	int			TempXCount;
	int			TempYCount;
	int			TempXPos;
	int			TempYPos;
	POINT		ptMouse;

	if(m_fRenderList)
	{
		// ���콺 ��ǥ�� ���Ѵ�.	
		m_lpWorld->pIOnlineMouse->GetPos(&ptMouse);

		// ����ϴ� Imoticon ��(���� ���⼭ ���� ����Ҽ� �ִ� Imoticon�� ���� �۾��� �Ѵ�. ������ ������ �ӽ÷� �����Ƿ� �Ѵ�.)
		for(TempCount = 0; TempCount < 9; TempCount++)
		{
			m_UseImoticonList[TempCount] = TempCount;
//			if(TempCount == 8) m_UseImoticonList[TempCount] = 0xfff;
		}

		// ��ư�� �����Ͽ����� �˻�
		if(fLMouseUp)
		{
			for(TempYCount = 0; TempYCount < 3; TempYCount++)
			{
				for(TempXCount = 0; TempXCount < 3; TempXCount++)
				{
					TempXPos = 598 + ((m_lpPannel->Header.Xsize - 2) * TempXCount);
					TempYPos = 505 - ((m_lpPannel->Header.Ysize - 5) * 3) + ((m_lpPannel->Header.Ysize - 5) * TempYCount);

					if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + m_lpPannel->Header.Xsize - 2))
						&& (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + m_lpPannel->Header.Ysize - 5)))
					{
						SetImoticonKey((TempYCount * 3) + TempXCount);
						SetActive(FALSE);
						return TRUE;
					}
				}
			}
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineImoticon::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	SI32		TempCount;
	SI32		TempXCount;
	SI32		TempYCount;
	SI32		TempXPos;
	SI32		TempYPos;
	SI32		TempImoticonXSize = 40;
	SI32		TempImoticonYSize = 40;
	XSPR*		pImoticon;
	POINT		ptMouse;

	if(m_fRenderList)
	{
		// ���콺 ��ǥ�� ���Ѵ�.	
		m_lpWorld->pIOnlineMouse->GetPos(&ptMouse);

		// Image�� ����Ѵ�.
		if(clGrp.LockSurface(lpSurface) == TRUE)
		{
			for(TempYCount = 0; TempYCount < 3; TempYCount++)
			{
				for(TempXCount = 0; TempXCount < 3; TempXCount++)
				{
					TempXPos = 598 + ((m_lpPannel->Header.Xsize - 2) * TempXCount);
					TempYPos = 505 - ((m_lpPannel->Header.Ysize - 5) * 3) + ((m_lpPannel->Header.Ysize - 5) * TempYCount);

					// �켱 ���� ���
					if((ptMouse.x >= TempXPos) && (ptMouse.x < (TempXPos + m_lpPannel->Header.Xsize - 2))
						&& (ptMouse.y >= TempYPos) && (ptMouse.y < (TempYPos + m_lpPannel->Header.Ysize - 5)))
					{
						if(IpD.LeftPressSwitch)
						{
							clGrp.PutSpriteT(TempXPos, TempYPos, m_lpPannel->Header.Xsize, m_lpPannel->Header.Ysize, 
													&m_lpPannel->Image[m_lpPannel->Header.Start[2]]);
						}
						else
						{
							clGrp.PutSpriteT(TempXPos, TempYPos, m_lpPannel->Header.Xsize, m_lpPannel->Header.Ysize, 
													&m_lpPannel->Image[m_lpPannel->Header.Start[1]]);
						}
					}
					else
					{
						clGrp.PutSpriteT(TempXPos, TempYPos, m_lpPannel->Header.Xsize, m_lpPannel->Header.Ysize, 
												&m_lpPannel->Image[m_lpPannel->Header.Start[0]]);
					}

					// ������ Imoticon ���
					TempCount = (TempYCount * 3) + TempXCount;
					if(m_UseImoticonList[TempCount] != 0xfff)
					{
						pImoticon = m_lpWorld->pOnlineResource->GetxsprImoticon(m_UseImoticonList[TempCount]);
						clGrp.PutSpriteT(TempXPos + (m_lpPannel->Header.Xsize / 2) - (pImoticon->Header.Xsize / 2)
							, TempYPos + (m_lpPannel->Header.Ysize / 2) - (pImoticon->Header.Ysize / 2)
							, pImoticon->Header.Xsize, pImoticon->Header.Ysize, 
												&pImoticon->Image[pImoticon->Header.Start[(GetTickCount() / 50) % pImoticon->Header.font_number]]);
					}
				}
			}

			clGrp.UnlockSurface(lpSurface);
		}
	}

	return TRUE;
}

