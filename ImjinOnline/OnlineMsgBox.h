#ifndef	_ONLINEMSGBOX_H_
#define _ONLINEMSGBOX_H_

// ���ϸ� : OnlineMsgBox.h
// ����� : ������

#define ON_MSGBOX_TYPE_NONE					0		// �޽��� �ڽ� Ÿ�� - OK, Cancel ��ư�� 2�� ����
#define ON_MSGBOX_TYPE_OK					1		// �޽��� �ڽ� Ÿ�� - OK
#define ON_MSGBOX_TYPE_CANCEL				2		// �޽��� �ڽ� Ÿ�� - Cancel
#define ON_MSGBOX_TYPE_OKCANCEL				3		// �޽��� �ڽ� Ÿ�� - OK, Cancel

// robypark 2004/12/20 17:32
// ON_MSGBOX_TYPE_OKCANCEL�� ������ ����. �ٸ� ��ư �ؽ�Ʈ�� �ٸ���.
#define ON_MSGBOX_TYPE_ACCEPTDECLINE		4		// �޽��� �ڽ� Ÿ�� - Accept, Decline



#define ON_MSGBOX_RETURN_OK					1		// 'OK' ������
#define ON_MSGBOX_RETURN_CANCEL				2		// 'Cancel' �Ǵ� 'X' ������

// robypark 2004/12/20 17:32
// ON_MSGBOX_RETURN_OK, ON_MSGBOX_RETURN_CANCEL ������ ����. �ٸ� ��ư �ؽ�Ʈ�� �ٸ���.
#define ON_MSGBOX_RETURN_ACCEPT				1		// 'Accept' ������
#define ON_MSGBOX_RETURN_DECLINE			2		// 'Decline' �Ǵ� 'X' ������

#define ON_MSGBOX_MAX_CAPTION_LENGTH		256
#define ON_MSGBOX_MAX_MSG_LENGTH			1024


class OnlineMsgBox
{
private:
	cltOnlineWorld					*m_pOnlineWorld;
	SI32							m_siType;													// �޽��� �ڽ� Ÿ��(OK, OK/Cancel)

	XSPR							*m_pXsprBox[2];												// ä��â �ǳ�
	XSPR							*m_pXsprButt[2];											// ��ư
	XSPR							*m_pXsprButtBack;											// ��ư �� ���
	UI08							*m_pButton;													// �̹��� ��ư��

	SI32							m_siBoardXNum, m_siBoardYNum;								// ���� ��� �κ��� Ÿ�� ��(����, ����)
	POINT							m_ptBoxStart;												// �޽��� �ڽ��� ���� ��ǥ
	POINT							m_ptButtStart[2];											// ��ư�� ���� ��ǥ

	CHAR							m_strCaption[ON_MSGBOX_MAX_CAPTION_LENGTH];					// ����
	CHAR							m_strMsg[ON_MSGBOX_MAX_MSG_LENGTH];							// ����
	BOOL							m_bMouseDown;												// ���콺 ����
	BOOL							m_bActive;													// �޽��� �ڽ��� ��� ���ΰ�?
	RECT							m_rtMsgBox;													// �޽��� �ڽ� ��Ʈ
	SI16							m_siNoField;												// �� �ʵ� ����
	BOOL							m_bLogin;

public:
	_Button							m_Butt[2];													// OK, Cancel ��ư

private:	
	VOID	SetData(CHAR *pCaption, CHAR *pMsg);												// ��� �������� ����
	VOID	DrawBox();																			// �޽��� �ڽ��� �׸���.
	VOID	DrawText(HDC hDC);																	// �ؽ�Ʈ�� ��´�.

public:
	OnlineMsgBox() {}
	~OnlineMsgBox() {}

	VOID	Init(cltOnlineWorld *pOnlineWorld,BOOL b  = FALSE);													// �ʱ�ȭ
	VOID	SetMsg(SI32 type, CHAR *pCaption, CHAR *pMsgFormat, ...);							// �޽����� �����Ѵ�.
	SI32	Process();																			// ���� ���¸� �����ϰ� �����Ѵ�.
    BOOL	IsOk();																				// 'Ȯ��' ��ư�� �����°�?
    BOOL	IsCancel();																			// '���' ��ư�� �����°�?
	VOID	Draw( LPDIRECTDRAWSURFACE7 pSurface );												// ���
	BOOL	IsActive();																			// ���� �޽��� �ڽ��� ��� ���ΰ�?
	VOID	Initialize();																		// �޽��� �ڽ��� �ʱ�ȭ �Ѵ�.
	VOID	SetLocation(SI32 x, SI32 y);
	VOID	SetAction(BOOL flag);
};

#endif



