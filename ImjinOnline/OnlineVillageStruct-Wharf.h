/************************************************************************************************
	파일명 : OnlineVillageStruct-Wharf.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREWHARF_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREWHARF_HEADER

#include <DDraw.h>
#include "Online.h"
#include "OnlineVillageBaseStructure.h"

#define WHARF_NOWWORK_MAIN_START					1
#define WHARF_NOWWORK_MAIN_STARTPOOL				2
#define WHARF_NOWWORK_MAIN_POOL						3
#define WHARF_NOWWORK_CHECK_POOL					4	
#define WHARF_NOWWORK_CHECK_POOL_RETURNWAIT			5
#define WHARF_NOWWORK_MAIN_END						7
#define WHARF_NOWWORK_MAIN_ENDPOOL					8	
#define WHARF_NOWWORK_WAITROOM_START				9
#define WHARF_NOWWORK_WAITROOM_STARTPOOL			10
#define WHARF_NOWWORK_WAITROOM_POOL					11
#define WHARF_NOWWORK_WAITROOM_END					12
#define WHARF_NOWWORK_WAITROOM_ENDPOOL				13	
#define WHARF_NOWWORK_SHIP_POOL						14	
#define WHARF_NOWWORK_EXIT_POOL						15	
#define WHARF_NOWWORK_MAIN_LOWLEVEL					16

#define MAX_TICKET_LIST								128



/** --> 여기서 부터 추가 코드 (sagolboss)
	@author 김성준 (sagolboss)
	@date   2004-08-09 오후 3:17
	
	부두에서 티켓 리스트를 정렬해서 보여주는 기능에 관련된 것들이다.

#include <string>
#include <list>
using namespace std;

class CTicket
{
	struct TICKET_COLUMN_INFO
	{
		POINT  ptPos;
		DWORD  dwColor;
		string strContent;
	};

public:
	CTicket()  {};
	~CTicket() { m_ListTicketColumnInfo.clear(); }

	void Add(const POINT& ptPos, const char* lpszContent, const DWORD dwColor);
	void Draw(HDC hDC);

private:
	list<TICKET_COLUMN_INFO*> m_ListTicketColumnInfo;
};
*/
///<-- 추가 코드 끝(sagolboss)
	
// 부두 Class
class	OnlineVillageStructureWharf : public OnlineVillageBaseStructure
{
private:
	// Image
	XSPR						m_Image_MainInterface_Back;
	XSPR						m_Image_MainInterface_BackSelectTicket;
	XSPR						m_Image_MainInterface_Scroll;
	XSPR						m_Image_CheckInterface;

	XSPR						m_Image_Button_All_EM;
	XSPR						m_Image_Button_All_BM;
	XSPR						m_Image_Scroll_All_M;
	XSPR*						m_Image_Scroll_Up;
	XSPR*						m_Image_Scroll_Down;
	XSPR						m_Image_MainInterface_ScrollBar;
	XSPR						m_Image_CheckInterface_Line;

	XSPR						m_Image_Portrait;

	// Scroll
	SI16						m_NowScroll;

	// Button
	_Button						m_Button_Buy;
	_Button						m_Button_GoWaitRoom;
	_Button						m_Button_BuyOK;
	_Button						m_Button_BuyCancel;
	_Button						m_Button_ScrollUp;
	_Button						m_Button_ScrollDown;


public:
	// Base
	SI16						m_NextWork;
	SI16						m_NowWork;
	SI16						m_siX;
	SI16						m_siY;
	SI16						m_siCheckX;
	SI16						m_siCheckY;
	DWORD						m_StartTickCount;

	// Now My Ticket
	SI32						siTicketKind;

	// Ticket Data
	SI32						m_DiscountPercent;
	OnTicketInfo				m_TicketList[MAX_TICKET_LIST];
	SI16						m_MaxSelectTicket;
	SI16						m_NowSelectTicket;


public:
	OnlineVillageStructureWharf();												// 생성자.
	virtual	~OnlineVillageStructureWharf();										// 소멸자.
		
	virtual	VOID	Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);// 초기화
	virtual	VOID	Free();															// 해제.
	virtual	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);							// 액션.
	virtual BOOL	Action();

//	virtual BOOL	Draw_Main(LPDIRECTDRAWSURFACE7 pSurface, DWORD TickCount);		// Draw Back
	virtual BOOL	Draw_Select(LPDIRECTDRAWSURFACE7 pSurface);	// Draw Main
	virtual BOOL	Draw_Check(LPDIRECTDRAWSURFACE7 pSurface);		// Draw CheckBox
	virtual BOOL	Draw_WaitRoom(LPDIRECTDRAWSURFACE7 pSurface);	// Draw WaitRoom

	virtual BOOL	CheckMoveInShip(void);
	virtual BOOL	CheckTicketUse(void);
	virtual BOOL	GoShip(void);
};


#endif
