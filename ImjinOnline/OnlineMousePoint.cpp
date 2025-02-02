#include <GSL.h>
//#include "Main.h"		// actdoll (2004/03/18 13:31) : 橈橫紫 闡だ橾檜 腎朝 ④渦. 霞瞼 в蹂橈湍陛 鼻嬪 ④渦 頂縑 摹樹腎橫 氈朝 唳辦歜. 檜匙 陽僥縑 僥薯 嫦儅衛 棻衛 麵晦 夥塋
#include "Online.h"
#include "OnlineMegaText.h"
#include "OnlineWorld.h"
#include "DebugTool.h"
#include "OnlineMousePoint.h"
#include "OnlineResource.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 儅撩濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineMousePoint::OnlineMousePoint()
{
	m_fCreate = FALSE;

	m_pOnlineResource = NULL;
	m_pOnlineMap = NULL;

	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineMousePoint::~OnlineMousePoint()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMousePoint::Init(OnlineResource *pOnlineResource, OnlineMap *pOnlineMap)
{
	m_fCreate = TRUE;
	
	m_pOnlineResource = pOnlineResource;
	m_pOnlineMap = pOnlineMap;

	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMousePoint::Free()
{
	m_fCreate = FALSE;
	
	m_pOnlineResource = NULL;
	m_pOnlineMap = NULL;
	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Mouse Point 蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMousePoint::SetMouse(SI32 siTileX, SI32 siTileY, SI32 siMapIndex)
{
	SI32		TempCount;
	SI32		TempEmpty;
	SI32		TempEmptyTime;

	// 綴 奢除擊 瓊嬴 蹺陛и棻.
	TempEmpty = -1;
	for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
	{
		if(m_MousePointData[TempCount].sifCreate == FALSE)
		{
			TempEmpty = TempCount;
			break;
		}
	}

	// 虜擒 綴奢除檜 橈戲賊 陛濰 釭醞匙擊 餉薯
	if(TempEmpty == -1)
	{
		TempEmptyTime = -1;
		for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
		{
			if(m_MousePointData[TempCount].sifCreate)
			{
				if(TempEmptyTime <= m_MousePointData[TempCount].siCount)
				{
					TempEmpty     = TempCount;
					TempEmptyTime = m_MousePointData[TempCount].siCount;
				}
			}
		}
	}

	// 瓊擎 蘊縑 盪濰
	m_MousePointData[TempEmpty].sifCreate  = TRUE;
	m_MousePointData[TempEmpty].siTileX    = siTileX;
	m_MousePointData[TempEmpty].siTileY    = siTileY;
	m_MousePointData[TempEmpty].siMapIndex = siMapIndex;
	m_MousePointData[TempEmpty].siCount    = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Mouse Point 賅舒 蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMousePoint::Clear(void)
{
	ZeroMemory(m_MousePointData, sizeof(SMousePointInfo) * MAX_MOUSEPOINT);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Mouse Point 曖 擋暮.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMousePoint::Action(void)
{
	SI32		TempCount;
	XSPR*		TemplpXSpr;

	TemplpXSpr = m_pOnlineResource->GetxsprGame(ON_GAMESPR_MOVEPOINT);

	for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
	{
		if(m_MousePointData[TempCount].sifCreate)
		{
			// Frame 隸陛
			m_MousePointData[TempCount].siCount++;

			// 橾薑 Frame 檜鼻檜賊 餉薯
			if(m_MousePointData[TempCount].siCount >= (TemplpXSpr->Header.font_number * 3))
			{
				m_MousePointData[TempCount].sifCreate = FALSE;
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 Mouse Point 蒂 �飛橦� 斜萼棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID OnlineMousePoint::Draw(void)
{
	SI32		TempCount;
	SI32		siDotDrawBaseX;
	SI32		siDotDrawBaseY;
	XSPR*		TemplpXSpr;

	TemplpXSpr = m_pOnlineResource->GetxsprGame(ON_GAMESPR_MOVEPOINT);

	// 轎溘
	for(TempCount = 0; TempCount < MAX_MOUSEPOINT; TempCount++)
	{
		if(m_MousePointData[TempCount].sifCreate)
		{
			if(m_MousePointData[TempCount].siMapIndex == m_pOnlineMap->m_MapIndex)
			{
				if(m_pOnlineMap->TransformMapXYToScreenDotCenterXY(m_MousePointData[TempCount].siTileX, m_MousePointData[TempCount].siTileY, siDotDrawBaseX, siDotDrawBaseY))
				{		
					siDotDrawBaseX = siDotDrawBaseX - (TemplpXSpr->Header.Xsize / 2) - m_pOnlineMap->GetRX();
					siDotDrawBaseY = siDotDrawBaseY - (TemplpXSpr->Header.Ysize / 2) - m_pOnlineMap->GetRY();

					clGrp.PutSpriteLightImjinCT(siDotDrawBaseX, siDotDrawBaseY, TemplpXSpr->Header.Xsize, TemplpXSpr->Header.Ysize, &TemplpXSpr->Image[TemplpXSpr->Header.Start[m_MousePointData[TempCount].siCount / 3]]);
				}		
			}
		}
	}
}

