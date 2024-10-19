/********************************************************************************************************
	���ϸ�	:	OnlineTip.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.04.**
	������	:	
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
	UI16	uiTipNum;			//	Tip ��ȣ
	UI32	uiSubjectIndex;		//	���� �ε���
	UI32	szContentIndex[10];		//	���� �ε���
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
	OnlineTotalCount    pOnlineTotalCount[200]; //list�� ��� �����ϱ� ���� �ӽ� �������.
	SI32			    m_MainTipScrollCount;   //��ũ��.
	SI32                m_TipSelect;			//Main == 1,Sub == 2,Con == 3  
	SI32				m_MainTipCount;         //��ũ�� ī���͸� �����ϱ� ���� ����.
	SI32				m_SubTipCount;
	UI16				m_ContentsTipCount;
	SI32                m_LineCount;            //����Ʈ�� ����.
	SI32				m_iSelectTarget;        //Main���� ���õ� ����Ʈ�� �ѹ�.

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
	SI16			m_siX, m_siY, m_siNoField;						//	���� ��ǥ
	RECT			m_rtTip;										//	TIP	ȭ�� �簢����
//	POINT			m_ptOldMousePos;
	BOOL			m_bFlag, m_bMouseDown;
	CHAR			m_szTipTextList[20][128];
//	SI32			m_siCurLine;									//	��µǾ�� �� ���� ����
	_Button			m_BBack,  m_BExit;						//	PREV, NEXT, EXIT

	SI32			m_siMaxStrLine;														// ȭ�鿡�� �� �ٱ��� ���� ���ΰ�
	SI32			m_siMaxStrLen;														// ���η� ���ڱ��� ���� ���ΰ�?

	SI32			m_siScrollBarStartX;												// ��ũ�ѹ��� ���� X��ǥ
	SI32			m_siScrollBarStartY;												// ��ũ�ѹ��� ���� Y��ǥ
	SI32			m_siScrollBarY;														// ��ũ�ѹ��� Y��ǥ
		
	RECT			m_rtScrollArea;														// ��ũ�ѹ� ������ ����
	SI32			m_siScrollAreaHeight;												// ��ũ�ѹ� ������ ����
	XSPR			*m_pXsprTipScrollBar;									 			// ��ũ�ѹ� �̹���

	VOID			DrawChatListScroll(LPDIRECTDRAWSURFACE7 pSurface);					// ä�ñ� ����� ��ũ���� �׸���.
	VOID			ScrollChatList();													// ä�ñ� ��� ��ũ���ϱ�
	UI32			DevideByLine(CHAR *pContent, SI32	siMaxStrLen);
//	VOID			DrawTipTextList(HDC hDC, SI32 siTextX, SI32 siTextY, UI32 line);


public:
	OnlineTip();
	~OnlineTip();

	BOOL			Init( cltOnlineWorld *pOnlineWorld );			// �ʱ�ȭ
	VOID			Action();										
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );			// ���

	VOID			Free();											// ����
	VOID			SetAction( BOOL bflag );						// Ȱ��ȭ �Ѵ�.
	BOOL			IsAction() { return m_bAction; };				// Ȱ��ȭ?

	///////////////////////////////////////////////////////////////////////////
	BOOL			SelectSubject(UI16 Variable);
	UI16			SelectContents(UI16 code);
	///////////////////////////////////////////////////////////////////////////
};

#endif