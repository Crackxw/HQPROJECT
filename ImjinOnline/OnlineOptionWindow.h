/*
#ifndef __ONLINE_OPTIONWINDOW__
#define __ONLINE_OPTIONWINDOW__


class OnlineOptionWindow
{
private:
	cltOnlineWorld					*m_pMyOnlineWorld;										// �¶��� ���� ������
	_Button							m_BLogIn, m_BExit, m_BJoyon, m_BBack;


public:
	OnlineSetting() : m_pMyOnlineWorld(0), m_bAction(FALSE) {};
	~OnlineSetting() { Free(); };


	VOID			Init( cltOnlineWorld *pOnlineWorld );									// �ʱ�ȭ
	VOID			Action();																// �׼�
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );									// ���
	BOOL			IsAction()	{	return m_bAction;	};									// ���� Ȱ�� ���ΰ�?
	VOID			SetAction( BOOL bFlag );
	VOID			Free();																	// ����
	VOID			GameInit();																// �α��� ���ư��� ���� ���� �ʱ�ȭ
};

#endif
*/