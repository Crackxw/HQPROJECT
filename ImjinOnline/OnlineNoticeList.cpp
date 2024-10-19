/************************************************************************************************
	���ϸ� : OnlineNoticeList.cpp
	����� : ������

	Game�� �������� Data�� �����ϰ�, ����Ѵ�.
************************************************************************************************/
#include <GSL.h>

#include <Main.h>
#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineFont.h"
#include "OnlineNoticeList.h"


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineNoticeList::OnlineNoticeList()
{
	m_lpcltOnlineWorld = NULL;

	m_lpNoticeData        = NULL;
	m_NoticeDataMaxCount  = 0;
	m_NoticeDataWaitCount = 0;

	m_RenderXPos  = 0;
	m_RenderYPos  = 0;
	m_RenderSize  = 0;
	m_RenderCount = 0;

	m_DrawNoticeIndex   = -1;
	m_DrawNoticeScroll  = 0;
	m_DrawNoticeMaxSize = 0;
	m_DrawNoticeRenderStartTime = 0;

	m_RectHRGN    = NULL;
	m_RectBigHRGN = NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineNoticeList::~OnlineNoticeList()
{
	Free();
}

VOID OnlineNoticeList::Initialize()
{
	for(SI32 TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
	{
		ZeroMemory(&m_lpNoticeData[TempCount], sizeof(SNoticeData));
	}

	m_DrawNoticeIndex   = -1;
	m_DrawNoticeScroll  = 0;
	m_DrawNoticeMaxSize = 0;
	m_DrawNoticeRenderStartTime = 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineNoticeList::Init(cltOnlineWorld* lpcltOnlineWorld, SI32 MaxNoticeCount, SI32 WaitNoticeCount, SI32 RenderXPos, SI32 RenderYPos, SI32 RenderSize, SI32 DrawCount)
{
	Free();

	m_lpcltOnlineWorld = lpcltOnlineWorld;

	m_lpNoticeData        = new SNoticeData[MaxNoticeCount + WaitNoticeCount];
	ZeroMemory(m_lpNoticeData, sizeof(SNoticeData) * (MaxNoticeCount + WaitNoticeCount));
	m_NoticeDataMaxCount  = MaxNoticeCount;
	m_NoticeDataWaitCount = WaitNoticeCount;

	m_RenderXPos  = RenderXPos;
	m_RenderYPos  = RenderYPos;
	m_RenderSize  = RenderSize;
	m_RenderCount = DrawCount;

	m_DrawNoticeIndex   = -1;
	m_DrawNoticeScroll  = 0;
	m_DrawNoticeMaxSize = 0;
	m_DrawNoticeRenderStartTime = 0;

	m_RectHRGN    = CreateRectRgn(m_RenderXPos, m_RenderYPos, m_RenderXPos + m_RenderSize, m_RenderYPos + 16);
	m_RectBigHRGN = CreateRectRgn(0, 0, 799, 599);

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID OnlineNoticeList::Free(void)
{
	m_lpcltOnlineWorld = NULL;

	if(m_lpNoticeData)
	{
		delete[] m_lpNoticeData;
		m_lpNoticeData        = NULL;
	}
	m_NoticeDataMaxCount  = 0;
	m_NoticeDataWaitCount = 0;

	m_RenderXPos  = 0;
	m_RenderYPos  = 0;
	m_RenderSize  = 0;
	m_RenderCount = 0;

	m_DrawNoticeIndex   = -1;
	m_DrawNoticeScroll  = 0;
	m_DrawNoticeMaxSize = 0;
	m_DrawNoticeRenderStartTime = 0;

	if(m_RectHRGN)    DeleteObject(m_RectHRGN);
	if(m_RectBigHRGN) DeleteObject(m_RectBigHRGN);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ��ġ ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineNoticeList::SetPosition(SI32 RenderXPos, SI32 RenderYPos, SI32 RenderSize, SI32 DrawCount)
{
	m_RenderXPos  = RenderXPos;
	m_RenderYPos  = RenderYPos;
	m_RenderSize  = RenderSize;
	m_RenderCount = DrawCount;

	m_DrawNoticeScroll  = 0;

	if(m_RectHRGN)
	{
		DeleteObject(m_RectHRGN);
		m_RectHRGN    = CreateRectRgn(m_RenderXPos, m_RenderYPos, m_RenderXPos + m_RenderSize, m_RenderYPos + 16);
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ �߰��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineNoticeList::AddNoticeData(char* lpNotice, COLORREF rgb)
{
	SI32		TempCount;
	SI32		TempSelectIndex;
	SI32		TempSelectIndexCount;

	if(m_lpNoticeData)
	{
		ListSort();

		// ������ ĭ�� �����ִ��� �˻�
		for(TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
		{
			if(m_lpNoticeData[TempCount].m_fCreate == FALSE)
			{
				// �������� �߰�
				ZeroMemory(&m_lpNoticeData[TempCount], sizeof(SNoticeData));

				m_lpNoticeData[TempCount].m_fCreate     = TRUE;
				strncpy((char*)m_lpNoticeData[TempCount].m_Notice, lpNotice, 256);
				m_lpNoticeData[TempCount].m_RGB         = rgb;
				m_lpNoticeData[TempCount].m_DrawCount   = 0;				
				return TRUE;
			}
		}

		// ���� ������ĭ�� ������, ���� ���� ��µ� ���������� ã��
		TempSelectIndex      = -1;
		TempSelectIndexCount = 0;
		for(TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
		{
			if(m_lpNoticeData[TempCount].m_fCreate == FALSE)
			{
				if(TempCount != m_DrawNoticeIndex)
				{
					if(m_lpNoticeData[TempCount].m_DrawCount > TempSelectIndexCount)
					{
						TempSelectIndex      = TempCount;
						TempSelectIndexCount = m_lpNoticeData[TempCount].m_DrawCount;
					}
				}
			}
		}
		if(TempSelectIndex != -1)
		{
			// ���� ���� ��µ� ���������� �����, ���� �߰��õ��� �Ѵ�.
			ZeroMemory(&m_lpNoticeData[TempSelectIndex], sizeof(SNoticeData));
			AddNoticeData(lpNotice, rgb);
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Poll
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineNoticeList::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32		TempCount;
	SI32		TempCount1;
	BOOL		TempfDrawNotice;

	if(m_DrawNoticeIndex == -1)
	{
		// ���� ���� ��µŴ� Notice�� ������ ���� Notice�� ����Ѵ�.
		for(TempCount = 0; TempCount < m_NoticeDataMaxCount; TempCount++)
		{
			if(m_lpNoticeData[TempCount].m_fCreate)
			{
				m_DrawNoticeIndex   = TempCount;
				m_DrawNoticeMaxSize = m_DrawNoticeScroll = GetNoticeSize(m_DrawNoticeIndex, pSurface);
				m_DrawNoticeRenderStartTime = GetTickCount();
				break;
			}
		}
	}
	else
	{
		// ����Ŵ� Notice�� �������
		m_DrawNoticeScroll = m_DrawNoticeMaxSize - ((GetTickCount() - m_DrawNoticeRenderStartTime) / 22);

		if(m_DrawNoticeScroll <= 0)
		{
			// ���� ����� ���Ͽ�����
			m_lpNoticeData[m_DrawNoticeIndex].m_DrawCount++;
			if(m_lpNoticeData[m_DrawNoticeIndex].m_DrawCount >= m_RenderCount)
			{
				// ����� �Ҹ�ŭ �Ͽ����� ����
				ZeroMemory(&m_lpNoticeData[m_DrawNoticeIndex], sizeof(SNoticeData));
				ListSort();		// <<- ListSoft()���� ���� ����ϴ� Index�� ������� �ٸ� Notice�� ������, ���� Index(�ּ� 0)���� �ϰ�, ������ -1�� �����Ѵ�.
			}
			else
			{
				// ���� ���� ���۰� ���ڶ���쵵 ����
				TempCount1 = 0;
				for(TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
				{
					if(m_lpNoticeData[TempCount].m_fCreate) TempCount1++;
				}
				if(TempCount1 >= m_NoticeDataMaxCount)
				{
					ZeroMemory(&m_lpNoticeData[m_DrawNoticeIndex], sizeof(SNoticeData));
					ListSort();		// <<- ListSoft()���� ���� ����ϴ� Index�� ������� �ٸ� Notice�� ������, ���� Index(�ּ� 0)���� �ϰ�, ������ -1�� �����Ѵ�.
				}
			}

			if(m_DrawNoticeIndex == -1)
			{
				// ���� ����Ҳ� �������
				m_DrawNoticeScroll  = 0;
				m_DrawNoticeMaxSize = 0;
				m_DrawNoticeRenderStartTime = 0;
			}
			else
			{
				// ���� ����Ұ� �˻��Ͽ� ���
				TempfDrawNotice = FALSE;
				for(TempCount = 0; TempCount < m_NoticeDataMaxCount; TempCount++)
				{
					if(m_lpNoticeData[TempCount].m_fCreate)
					{
						TempfDrawNotice = TRUE;
						break;
					}
				}

				// ��°� ã��
				if(TempfDrawNotice)
				{
					for(TempCount = 0; TempCount < m_NoticeDataMaxCount; TempCount++)
					{
						if(m_lpNoticeData[(TempCount + m_DrawNoticeIndex + 1) % m_NoticeDataMaxCount].m_fCreate)
						{
							m_DrawNoticeIndex   = (TempCount + m_DrawNoticeIndex + 1) % m_NoticeDataMaxCount;
							m_DrawNoticeMaxSize = m_DrawNoticeScroll = GetNoticeSize(m_DrawNoticeIndex, pSurface);
							m_DrawNoticeRenderStartTime = GetTickCount();
							break;
						}
					}
				}
				else
				{
					m_DrawNoticeIndex   = -1;
					m_DrawNoticeScroll  = 0;
					m_DrawNoticeMaxSize = 0;
					m_DrawNoticeRenderStartTime = 0; 
				}
			}
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ListSort(���� ����ϴ� Index�� ������� �ٸ� Notice�� ������, ���� Index(�ּ� 0)���� �ϰ�, ������ -1�� �����Ѵ�.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineNoticeList::ListSort(void)
{
	SI32		TempCount;
	SI32		TempMaxCount;

	TempMaxCount = 0;
	for(TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
	{
		if(m_lpNoticeData[TempCount].m_fCreate == FALSE)
		{
			if(TempCount == m_DrawNoticeIndex)
			{
				m_DrawNoticeIndex--;
			}
			else if(TempCount < m_DrawNoticeIndex)
			{
				m_DrawNoticeIndex--;
			}

			if(TempCount != ((m_NoticeDataMaxCount + m_NoticeDataWaitCount) - 1))
			{
				CopyMemory(&m_lpNoticeData[TempCount], &m_lpNoticeData[TempCount + 1], (sizeof(SNoticeData) * ((m_NoticeDataMaxCount + m_NoticeDataWaitCount) - (TempCount + 1))));
				ZeroMemory(&m_lpNoticeData[(m_NoticeDataMaxCount + m_NoticeDataWaitCount) - 1], sizeof(SNoticeData));
			}
		}
		else
		{
			TempMaxCount++;
		}
	}

	if(TempMaxCount <= 0)
	{
		m_DrawNoticeIndex  = -1;
		m_DrawNoticeScroll = 0;
		m_DrawNoticeRenderStartTime = 0;
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get Notice Draw Size
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 OnlineNoticeList::GetNoticeSize(SI32 Index, LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32		TempTextSize;
	HDC			hDC;
	SIZE		TextSize;	
	SI32		TempDrawSize;

	if(Index != -1)
	{
		if(m_lpNoticeData[Index].m_fCreate)
		{
			TempTextSize = strlen((char*)m_lpNoticeData[Index].m_Notice);
			if(TempTextSize > 0)
			{
				if(pSurface->GetDC(&hDC) == DD_OK)
				{
					SelectObject(hDC, m_lpcltOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

					GetTextExtentPoint32(hDC, (char*)m_lpNoticeData[Index].m_Notice, TempTextSize, &TextSize);
					//TempDrawSize = TextSize.cx + 240;
					TempDrawSize = TextSize.cx + m_RenderSize;

					pSurface->ReleaseDC(hDC);

					return TempDrawSize;
				}
			}
		}
	}

	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Draw
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineNoticeList::Draw(HDC hDC)
{
	SI32		TempSize;

	if(m_DrawNoticeIndex >= 0)
	{
		if(m_lpNoticeData[m_DrawNoticeIndex].m_fCreate)
		{
			TempSize = strlen((char*)m_lpNoticeData[m_DrawNoticeIndex].m_Notice);

			//TempHRGN    = CreateRectRgn(133, 553, 371, 569);
			SelectObject(hDC, m_RectHRGN);

			SetBkMode(hDC, TRANSPARENT);	
			SetTextColor(hDC, m_lpNoticeData[m_DrawNoticeIndex].m_RGB);
			//TextOut(hDC, (133 + 240) - (m_DrawNoticeMaxSize - m_DrawNoticeScroll), 556, (char*)m_lpNoticeData[m_DrawNoticeIndex].m_Notice, TempSize);
			TextOut(hDC, (m_RenderXPos + m_RenderSize) - (m_DrawNoticeMaxSize - m_DrawNoticeScroll), m_RenderYPos, (char*)m_lpNoticeData[m_DrawNoticeIndex].m_Notice, TempSize);

			SelectObject(hDC, m_RectBigHRGN);
		}
	}

	return TRUE;
}

