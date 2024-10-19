//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ��������� ��ȭâ
//	File Name		: OnlineQuestPlay.h
//	Birth Date		: 2002. 07. 08.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�������̽����� �����ο��� ����Ʈ ���� ���� �� ����/��Ҹ� �ϴ� â�� ���� �κ��Դϴ�.
//
//===================================================================================================

#ifndef __ONLINE_QUESTPLAY_H__
#define __ONLINE_QUESTPLAY_H__


//-----------------------------------------------------------------------------
// Name: OnlineQuestPlay
// Code: All (2004-02-16)
// Desc: Ŭ���� ����
//-----------------------------------------------------------------------------
class OnlineQuestPlay
{
private:	
	cltOnlineWorld*		m_pOnlineWorld;								// ���� ���Ӻ� Ŭ������ ������, �ܺο��� �޾ƿ´�.

	XSPR*				m_pSpr_Frm;									// ��������Ʈ�� - ������(�ܺ��� ���ҽ��� ����)
	XSPR				m_Spr_Btn;									// ��������Ʈ�� - ��ư

	_Button				m_Accept;									// ��ư UI - Quest ���� ��ư
	_Button				m_QuestCancel;								// ��ư UI - Quest ��� ��ư 
	_Button				m_Cancel;									// ��ư UI - ��ȭ ��� ��ư
	_Button				m_Next;										// ��ư UI - ���� ��ȭ��
	_Button				m_End;										// ��ư UI - ��ȭ ��

	BOOL				m_bAction;
	BOOL				m_bMouseDown;
	SI16				m_siNoFieldHandle;

	SI16				m_QuestCode;								// ����Ʈ �ε���
	SI16				m_ChattingType;								// Chatting Type
	SI16				m_ChattingNowPage;							// Chatting Now Page
	SI16				m_ChattingMaxPage;							// Chatting Max Page
	SI16				m_ChattingMaxSize;
	SI16				m_ChattingScroll;

	HRGN				m_TextClipper;
	HRGN				m_TextClipperOld;

	SI32				m_QuestStartBeforePrivQuestNPC;				// ����� Start Before�϶� ������ ���� ���� Quest�� �������

public:
	OnlineQuestPlay();
	~OnlineQuestPlay();

	BOOL			Init(cltOnlineWorld *pOnlineWorld);				// �ʱ�ȭ
	VOID			Free(void);										// ����

	VOID			Poll(void);										// ó��
	VOID			Draw(LPDIRECTDRAWSURFACE7 pSurface);			// ���

	VOID			SetAction(BOOL fAction, SI16 QuestCode = 0, SI16 ChattingType = 0);	// Ȱ��ȭ �Ѵ�.
	BOOL			IsAction(void) {return m_bAction;};							// Ȱ��ȭ �˻�

protected:
	UI32			GetTextCode(void);								// ���� ���Ŵ� Text�� ����
	SI32			GetTextMaxLine(HDC hDC);						// ���� ���Ŵ� Text�� �ִ� Line�� ����
	SI32			GetPrevQuestCompleted(SQuestHeader* lpQuest);	// ������ �� Quest�� ������ �ִ��� �˻�. ������ ������ ������ �� Quest�� NPC Code�� �Ѱ��ش�.
};


#endif//__ONLINE_QUESTPLAY_H__
