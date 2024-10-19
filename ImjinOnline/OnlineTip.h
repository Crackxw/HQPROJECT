/********************************************************************************************************
	파일명	:	OnlineTip.h
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2002.04.**
	수정일	:	
********************************************************************************************************/

#ifndef __ONLINE_TIP_H
#define __ONLINE_TIP_H
 

#include "Online.h"
#include "OnlineChatList.h"
#include "OnlineTip-Parser.h"

struct OnlineTotalCount
{
	char szLine[128];
	UI16 uiVariable;
	UI16 uicode;
	UI16 uiSubject;
};


struct TipHeader
{
	UI16	uiTipNum;			//	Tip 번호
	UI32	uiSubjectIndex;		//	제목 인덱스
	UI32	szContentIndex[10];		//	내용 인덱스
};

class OnlineTip
{
private:	
	cltOnlineWorld		*m_pOnlineWorld;	
	

	OnlineTipParser			 pOnlineTipParser;            
	OnlineTipContentsHeader* pOnlineTipContentsHeader;
	OnlineTipHeader*         pOnlineTipHeader;

	////////////////////////////Test insert///////////////////////////////////

	

	//sub tip 
	OnlineTotalCount    pOnlineTotalCount[200]; //list를 잠시 저장하기 위한 임시 저장공간.
	SI32			    m_MainTipScrollCount;   //스크롤.
	SI32                m_TipSelect;			//Main == 1,Sub == 2,Con == 3  
	SI32				m_MainTipCount;         //스크롤 카운터를 저장하기 위한 공간.
	SI32				m_SubTipCount;
	UI16				m_ContentsTipCount;
	SI32                m_LineCount;            //리스트의 갯수.
	SI32				m_iSelectTarget;        //Main에서 선택된 리스트의 넘버.

	XSPR				pScroll_Up;           //scroll up
	XSPR				pScroll_Down;		  //scroll down
	XSPR				pScroll_Bar;          //scroll bar

	_Button				m_Button_Scroll_Up;
	_Button				m_Button_Scroll_Down;
	_Button				m_Button_Scroll_Bar;
	INT					siScroll_Bar;
	char				m_Contents[20][1024];	
	

	BOOL			m_bAction;
	XSPR			m_XsprTipBoard, m_XsprButton;
	SI16			m_siX, m_siY, m_siNoField;						//	기준 좌표
	RECT			m_rtTip;										//	TIP	화면 사각영역
//	POINT			m_ptOldMousePos;
	BOOL			m_bFlag, m_bMouseDown;
	CHAR			m_szTipTextList[20][128];
//	SI32			m_siCurLine;									//	출력되어야 할 현재 라인
	_Button			m_BBack,  m_BExit;						//	PREV, NEXT, EXIT

	SI32			m_siMaxStrLine;														// 화면에서 몇 줄까지 보일 것인가
	SI32			m_siMaxStrLen;														// 가로로 몇자까지 보일 것인가?

	SI32			m_siScrollBarStartX;												// 스크롤바의 시작 X좌표
	SI32			m_siScrollBarStartY;												// 스크롤바의 시작 Y좌표
	SI32			m_siScrollBarY;														// 스크롤바의 Y좌표
		
	RECT			m_rtScrollArea;														// 스크롤바 영역의 범위
	SI32			m_siScrollAreaHeight;												// 스크롤바 영역의 높이
	XSPR			*m_pXsprTipScrollBar;									 			// 스크롤바 이미지

	VOID			DrawChatListScroll(LPDIRECTDRAWSURFACE7 pSurface);					// 채팅글 목록의 스크롤을 그린다.
	VOID			ScrollChatList();													// 채팅글 목록 스크롤하기
	UI32			DevideByLine(CHAR *pContent, SI32	siMaxStrLen);
//	VOID			DrawTipTextList(HDC hDC, SI32 siTextX, SI32 siTextY, UI32 line);


public:
	OnlineTip();
	~OnlineTip();

	BOOL			Init( cltOnlineWorld *pOnlineWorld );			// 초기화
	VOID			Action();										
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );			// 찍어

	VOID			Free();											// 날려
	VOID			SetAction( BOOL bflag );						// 활성화 한다.
	BOOL			IsAction() { return m_bAction; };				// 활성화?

	///////////////////////////////////////////////////////////////////////////
	BOOL			SelectSubject(UI16 Variable);
	UI16			SelectContents(UI16 code);
	///////////////////////////////////////////////////////////////////////////
};

#endif