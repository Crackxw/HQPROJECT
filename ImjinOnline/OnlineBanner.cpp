//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 歜霞煙 螞塭檣 [剪鼻]
//	File Name		: OnlineBanner.cpp
//	Birth Date		: 2004. 02. 25.
//	Creator			: 檜 彌 戮
//	Editer			: 褻 ⑷ 遽 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 餌辨ж雖 彊朝 模蝶殮棲棻.
//		=== Disable Source Files Comment ===
//		晦襄 模蝶縑憮 檜 贗楚蝶蒂 餌辨ж朝 в熱 睡碟檜 瞪睡 輿戮籀葬 腎橫 氈歷蝗棲棻.
//		評塭憮 褒чだ橾曖 敘賅蒂 還檜晦 嬪п п渡 囀萄蒂 輿戮籀葬м棲棻.
//		釭醞縑 в蹂и 睡碟檜 氈剪釭 檜匙 陽僥縑 僥薯陛 嫦儅й 唳辦 棻衛 犒掘衛酈賊 腌棲棻.
//
//===================================================================================================


/************************************************************************************************
	だ橾貲 : OnlineBanner.cpp
	氬渡濠 : 檜彌戮

	Game醞 釭螃朝 Banner蒂 籀葬и棻.
************************************************************************************************/

/*
#include <Main.h>
#include <Mouse.h>
#include <clGame.h>
#include "OnlineWorld.h"
#include "OnlineBanner.h"
#include "OnlineNumberControl.h"
#include "BindJXFile.h"

#define BANNERSHOWTIME				60000					// 1碟葆棻 ж釭噶 雖釭除棻.


extern _InputDevice				IpD;
extern _clGame*			pGame;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineBanner::OnlineBanner()
{
	m_lpcltOnlineWorld = NULL;

	m_BannerDataCount = 0;
	m_BannerData      = NULL;

	m_NowBanner          = 0;
	m_BannerShowTimeLast = 0;

	m_bMouseDown = FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineBanner::~OnlineBanner()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineBanner::Init(cltOnlineWorld* lpcltOnlineWorld)
{
	FILE*		fp;
	char		szBuffer[1024];
	char		szFileName[1024];
	int			BannerCount;

	Free();

	if(pGame->LanguageCode != ELANGAGECODE_KOREAN)
		return FALSE;

	// Text 檗晦	
	fp = BindJXFile::GetInstance()->GetFile( "Online\\Banner\\Banner.txt" );
	if(fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	m_BannerDataCount = atoi((char*)szBuffer);
	if(m_BannerDataCount == 0)
	{
		fclose(fp);
		return FALSE;
	}

	m_BannerData      = new SBannerData[m_BannerDataCount];
	ZeroMemory(m_BannerData, sizeof(SBannerData) * m_BannerDataCount);
	for(BannerCount = 0; BannerCount < m_BannerDataCount; BannerCount++)
	{
		fgets(szBuffer, 1024, fp);
		strtok(szBuffer, "\n\t\r");
		sprintf((char*)szFileName, "Online\\Banner\\%s", szBuffer);
		clGrp.LoadXspr((char*)szFileName, m_BannerData[BannerCount].m_BannerImage, TRUE);

		fgets(szBuffer, 1024, fp);
		m_BannerData[BannerCount].m_FrameDelay = atoi((char*)szBuffer);		
		if(m_BannerData[BannerCount].m_FrameDelay <= 0) m_BannerData[BannerCount].m_FrameDelay = 1;

		fgets(szBuffer, 1024, fp);
		m_BannerData[BannerCount].m_Delay = atoi((char*)szBuffer);

		fgets(szBuffer, 1024, fp);
		strtok(szBuffer, "\n\t\r");
		strncpy((char*)m_BannerData[BannerCount].m_URL, (char*)szBuffer, 255);
	}
	fclose(fp);

	m_lpcltOnlineWorld = lpcltOnlineWorld;

	m_NowBanner          = 1;
	m_BannerShowTimeLast = GetTickCount() - (BANNERSHOWTIME / 2);

	m_bMouseDown = FALSE;

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineBanner::Free(void)
{
	if(m_BannerData)
	{
		for(int TempCount = 0; TempCount < m_BannerDataCount; TempCount++)
		{
			if(m_BannerData[TempCount].m_BannerImage.Image)
			{
				clGrp.FreeXspr(m_BannerData[TempCount].m_BannerImage);
			}
		}

		delete[] m_BannerData;
	}
	m_BannerDataCount = 0;
	m_BannerData      = NULL;

	m_NowBanner          = 0;
	m_BannerShowTimeLast = 0;

	m_lpcltOnlineWorld = NULL;

	m_bMouseDown = FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Poll
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineBanner::Poll(void)
{
	if(m_BannerData)
	{
		// 衛除檜 雖釭賊 棻擠寡傘煎 掖羹
		if((GetTickCount() - m_BannerShowTimeLast) >= BANNERSHOWTIME)
		{
			m_NowBanner++;
			if(m_NowBanner >= m_BannerDataCount) m_NowBanner = 1;

			m_BannerShowTimeLast = GetTickCount();
		}

		// 寡傘蒂 援蒂陽
		if(m_lpcltOnlineWorld->pOnlineNumPad->fDisable > 30)
		{
			if((m_bMouseDown == TRUE) && (IpD.LeftPressSwitch == FALSE))
			{
				if((IpD.Mouse_X >= 95) && (IpD.Mouse_X < (95 + 126)) && (IpD.Mouse_Y >= 562) && (IpD.Mouse_Y < (562 + 30)))
				{
					DWORD		TempTickCount;

					TempTickCount = (GetTickCount() - m_BannerShowTimeLast);
					if(TempTickCount < (DWORD)m_BannerData[m_NowBanner].m_Delay)
					{
						if(strcmp(m_BannerData[m_NowBanner].m_URL, "NULL") != 0)
						{
							ShellExecute(NULL, "open", m_BannerData[m_NowBanner].m_URL, NULL, NULL, SW_SHOWNORMAL);
						}
					}		
					else
					{
						if(strcmp(m_BannerData[0].m_URL, "NULL") != 0)
						{
							ShellExecute(NULL, "open", m_BannerData[0].m_URL, NULL, NULL, SW_SHOWNORMAL);
						}
					}
				}
			}
		}

		if(IpD.LeftPressSwitch) m_bMouseDown = TRUE;
		else					m_bMouseDown = FALSE;
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL OnlineBanner::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(m_BannerData)
	{
		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			static XSPR*	lpYetSpr = NULL;
			XSPR*			lpBaseSpr;
			XSPR*			lpSpr;
			SI32			Frame;
			DWORD			TempTickCount;
			SI32			StartPos;
			SI32			FrameDelay;

			TempTickCount = GetTickCount() - m_BannerShowTimeLast;
			FrameDelay    = 33;
			if(TempTickCount < (DWORD)m_BannerData[m_NowBanner].m_Delay)
			{
				lpSpr      = &m_BannerData[m_NowBanner].m_BannerImage;
				FrameDelay = m_BannerData[m_NowBanner].m_FrameDelay;
			}		
			else
			{
				TempTickCount -= m_BannerData[m_NowBanner].m_Delay;
				lpSpr      = &m_BannerData[0].m_BannerImage;
				FrameDelay = m_BannerData[0].m_FrameDelay;
			}
			lpBaseSpr = &m_BannerData[0].m_BannerImage;

			Frame = TempTickCount / FrameDelay;

			if(lpYetSpr == NULL) lpYetSpr = lpSpr;
			if(lpSpr)
			{
				StartPos = 0;
				if(lpYetSpr != lpSpr)
				{
					if(TempTickCount < 450)
					{
						StartPos = (int)((1.0f - (float)sin((float)(TempTickCount / 5) * 0.017444f)) * 115.0f);
						StartPos = 0;
					}
					else
					{
						lpYetSpr = lpSpr;
					}
				}

				if(Frame >= lpSpr->Header.font_number) Frame = lpSpr->Header.font_number - 1;
				clGrp.PutMSprite(95, 562, lpBaseSpr->Header.Xsize, lpBaseSpr->Header.Ysize, lpBaseSpr->Image);
				clGrp.PutMSprite(95, 562, lpYetSpr->Header.Xsize, lpYetSpr->Header.Ysize, &lpYetSpr->Image[lpYetSpr->Header.Start[lpYetSpr->Header.font_number - 1]]);
				clGrp.PutMSprite(95 + StartPos, 562, lpSpr->Header.Xsize, lpSpr->Header.Ysize, &lpSpr->Image[lpSpr->Header.Start[Frame]]);
			}
			clGrp.UnlockSurface(pSurface);
		}	
	}

	return TRUE;
}
*/