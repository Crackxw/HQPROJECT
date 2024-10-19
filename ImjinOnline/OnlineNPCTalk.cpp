#include <GSL.h>
		
#include "OnlineWorld.h"
#include "OnlineNPCTalk.h"
#include "OnlineMap.h"
#include "IOnlineChar.h"
#include "OnlineMyData.h"
#include "OnlineNPCIndex-Parser.h" 
#include "OnlineMegaText.h"


#define ON_X_RANGE		10
#define ON_Y_RANGE		10
#define ON_MEGATEXT_ID	MAX_PLAYER_NUM+1

OnlineNPCTalk::OnlineNPCTalk(cltOnlineWorld *pWorld) : m_pMyWorld(pWorld)
{
	m_dwDelayTime	=	200; 
	m_nNowNPCIndex	=	-1;
	m_nTextDraw		=	0;
	m_bDraw			=	FALSE;

	m_pMyWorld->pOnlineMegaText->CreateGDIPage(ON_MEGATEXT_ID);
}

OnlineNPCTalk::~OnlineNPCTalk()
{
	m_pMyWorld->pOnlineMegaText->Free(ON_MEGATEXT_ID);
}

void OnlineNPCTalk::Action()
{
	SI16 CharX, CharY;
	m_pMyWorld->pIOnlineChar->GetPos(m_pMyWorld->pMyData->GetMyCharUniqueID(), &CharX, &CharY);
	
	RECT			MapRect;
	POINT			NpcPos;

	for(int i=0; i<m_pMyWorld->pOnlineMap->m_NPCParser.GetNPCNum(); ++i)
	{
		m_pNpcHeader = m_pMyWorld->pOnlineMap->m_NPCParser.GetNPCByIndex(i);

		if(m_pNpcHeader->siMapIndex != m_pMyWorld->pOnlineMap->m_MapIndex)	continue;

		// robypark 2004/9/9 18:15
		// ȥ�㸻 ������ �Ǿ����� ���� ��쿡�� Skip
		if (FALSE == m_pNpcHeader->bTalkOneself)
			continue;

		SetRect(&MapRect, CharX-ON_X_RANGE, CharY-ON_Y_RANGE, CharX+ON_X_RANGE, CharY+ON_Y_RANGE);

		NpcPos.x = m_pNpcHeader->siMapXPos;
		NpcPos.y = m_pNpcHeader->siMapYPos;


		if(PtInRect(&MapRect, NpcPos))					// ���� �ȿ� NPC�� ���̵���
		{
			if(m_pNpcHeader->siTalk[m_nTextDraw] == 0)
			{
				m_bDraw		    = FALSE;
				m_dwDelayTime	= 0; 
				m_nNowNPCIndex	= -1;				
				return;
			}

			SI32	siStartX,  siStartY;
			SI32	siCenterX, siCenterY;
			SI32	siRelateX,	siRelateY;
			SI32	siCurrentMapX, siCurrentMapY;
			RECT	rcScreen;


			// ȭ�鿡 ����� ��ġ�� ����Ѵ�.
			m_pMyWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
			SI32	siDrawX = m_pNpcHeader->siMapXPos - siCurrentMapX;	
			SI32	siDrawY = m_pNpcHeader->siMapYPos - siCurrentMapY;
			

			siRelateX = m_pMyWorld->pOnlineMap->GetRX();
			siRelateY = m_pMyWorld->pOnlineMap->GetRY(); 
			
			rcScreen  =	m_pMyWorld->pOnlineMap->GetScreenRect();
			
			siCenterX = siDrawX * ONLINE_TILE_XSIZE_HALF - siDrawY * ONLINE_TILE_XSIZE_HALF	+ (rcScreen.right - rcScreen.left + 1) / 2	- siRelateX;			
			siCenterY = siDrawX * ONLINE_TILE_YSIZE_HALF + siDrawY * ONLINE_TILE_YSIZE_HALF + (rcScreen.bottom - rcScreen.top + 1) / 2	- siRelateY;

		
			// ������ Character ȭ�鿡 ������ ���� ��ġ�� �ٽ� ����Ѵ�.
			siStartX			=	siCenterX - 44 / 2;
			siStartY			=	siCenterY - 71 + 60;
			
			// ��縦 ���� ��ǥ�� ���Ѵ�.
			m_siDrawX	=	siCenterX+4;
			m_siDrawY	=	siCenterY  + 15 - 86;

			if(m_nNowNPCIndex != i)	
				m_dwDelayTime = 140;
			
			if(m_dwDelayTime++ > 130)   // 7��
			{
				m_nTextDraw++;

				if(m_nTextDraw >= 3)		m_nTextDraw = 0;

				// ��ǳ���� ����� �ش�.
				m_pMyWorld->pOnlineMegaText->DrawText(ON_MEGATEXT_ID, m_pMyWorld->pOnlineText->GetQuestText(m_pNpcHeader->siTalk[m_nTextDraw]), RGB(245, 245, 245), TRUE);

				m_nNowNPCIndex	= i;
				m_dwDelayTime	= 0;
				m_bDraw			= FALSE;
				return;
			}
			

			m_nNowNPCIndex	= i;							// ���� ���ϰ� �ִ� NPC Index��ȣ�� ������ �صд�.
			m_bDraw			= TRUE;
			return;
		}
	}

	m_bDraw		    = FALSE;
	m_dwDelayTime	= 0; 
	m_nNowNPCIndex	= -1;
}

void OnlineNPCTalk::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!m_bDraw)	return;

	m_pMyWorld->pOnlineMegaText->Show(ON_MEGATEXT_ID, pSurface, m_siDrawX, m_siDrawY);
}