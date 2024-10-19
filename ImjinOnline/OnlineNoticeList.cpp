/************************************************************************************************
	だ橾貲 : OnlineNoticeList.cpp
	氬渡濠 : 檜彌戮

	Game醞 奢雖餌о Data蒂 婦葬ж堅, 轎溘и棻.
************************************************************************************************/
#include <GSL.h>

#include <Main.h>
#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineFont.h"
#include "OnlineNoticeList.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	轎溘 嬪纂 雖薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 撲貲擊 蹺陛и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineNoticeList::AddNoticeData(char* lpNotice, COLORREF rgb)
{
	SI32		TempCount;
	SI32		TempSelectIndex;
	SI32		TempSelectIndexCount;

	if(m_lpNoticeData)
	{
		ListSort();

		// 盪濰й 蘊檜 陴嬴氈朝雖 匐餌
		for(TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
		{
			if(m_lpNoticeData[TempCount].m_fCreate == FALSE)
			{
				// 奢雖餌о 蹺陛
				ZeroMemory(&m_lpNoticeData[TempCount], sizeof(SNoticeData));

				m_lpNoticeData[TempCount].m_fCreate     = TRUE;
				strncpy((char*)m_lpNoticeData[TempCount].m_Notice, lpNotice, 256);
				m_lpNoticeData[TempCount].m_RGB         = rgb;
				m_lpNoticeData[TempCount].m_DrawCount   = 0;				
				return TRUE;
			}
		}

		// 虜擒 盪濰й蘊檜 橈戲賊, 陛濰 號檜 轎溘脹 奢雖餌о擊 瓊擠
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
			// 陛濰 號檜 轎溘脹 奢雖餌о擊 雖辦堅, 億煎 蹺陛衛紫蒂 и棻.
			ZeroMemory(&m_lpNoticeData[TempSelectIndex], sizeof(SNoticeData));
			AddNoticeData(lpNotice, rgb);
		}
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Poll
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineNoticeList::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32		TempCount;
	SI32		TempCount1;
	BOOL		TempfDrawNotice;

	if(m_DrawNoticeIndex == -1)
	{
		// 虜擒 ⑷營 轎溘腋朝 Notice陛 橈擊陽 棻擠 Notice蒂 轎溘и棻.
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
		// 轎溥腋朝 Notice陛 氈擊唳辦
		m_DrawNoticeScroll = m_DrawNoticeMaxSize - ((GetTickCount() - m_DrawNoticeRenderStartTime) / 22);

		if(m_DrawNoticeScroll <= 0)
		{
			// 虜擒 轎溘擊 棻ж艘戲賊
			m_lpNoticeData[m_DrawNoticeIndex].m_DrawCount++;
			if(m_lpNoticeData[m_DrawNoticeIndex].m_DrawCount >= m_RenderCount)
			{
				// 轎溘擊 й虜躑 ж艘戲賊 餉薯
				ZeroMemory(&m_lpNoticeData[m_DrawNoticeIndex], sizeof(SNoticeData));
				ListSort();		// <<- ListSoft()縑憮 ⑷營 轎溘ж朝 Index陛 橈擊唳辦 棻艇 Notice陛 氈戲賊, 瞪曖 Index(譆模 0)戲煎 ж堅, 橈戲賊 -1煎 熱薑и棻.
			}
			else
			{
				// 虜擒 ⑷營 幗ぷ陛 賅濠塒唳辦紫 餉薯
				TempCount1 = 0;
				for(TempCount = 0; TempCount < (m_NoticeDataMaxCount + m_NoticeDataWaitCount); TempCount++)
				{
					if(m_lpNoticeData[TempCount].m_fCreate) TempCount1++;
				}
				if(TempCount1 >= m_NoticeDataMaxCount)
				{
					ZeroMemory(&m_lpNoticeData[m_DrawNoticeIndex], sizeof(SNoticeData));
					ListSort();		// <<- ListSoft()縑憮 ⑷營 轎溘ж朝 Index陛 橈擊唳辦 棻艇 Notice陛 氈戲賊, 瞪曖 Index(譆模 0)戲煎 ж堅, 橈戲賊 -1煎 熱薑и棻.
				}
			}

			if(m_DrawNoticeIndex == -1)
			{
				// 棻擠 轎溘й眷 橈擊唳辦
				m_DrawNoticeScroll  = 0;
				m_DrawNoticeMaxSize = 0;
				m_DrawNoticeRenderStartTime = 0;
			}
			else
			{
				// 棻擠 轎溘й勘 匐餌ж罹 轎溘
				TempfDrawNotice = FALSE;
				for(TempCount = 0; TempCount < m_NoticeDataMaxCount; TempCount++)
				{
					if(m_lpNoticeData[TempCount].m_fCreate)
					{
						TempfDrawNotice = TRUE;
						break;
					}
				}

				// 轎溘勘 瓊晦
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ListSort(⑷營 轎溘ж朝 Index陛 橈擊唳辦 棻艇 Notice陛 氈戲賊, 瞪曖 Index(譆模 0)戲煎 ж堅, 橈戲賊 -1煎 熱薑и棻.)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Get Notice Draw Size
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

