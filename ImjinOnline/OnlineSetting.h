/************************************************************************************************
	���ϸ� : OnlineSetting.h
	����� : �� �� ��
	��¥   : 2002.07.25
	��  �� : ȯ�漳��
************************************************************************************************/
#ifndef __ON_SETTING__H
#define __ON_SETTING__H


class OnlineSetting
{
private:
	cltOnlineWorld					*m_pMyOnlineWorld;										// �¶��� ���� ������
	_Button							m_BLogIn, m_BExit, m_BCompany, m_BShopping, m_BBack, m_BOption;

	BOOL							m_bAction;												// ���� Ȱ�����ΰ� �˾� ���� ����
	XSPR							m_BackImageSpr, m_ButtonSpr;
	SI16							m_siX, m_siY, m_siNoField;								// ���� ��ǥ
	BOOL							m_bMouseDown;											// ���콺 ����

	BOOL							m_OptionMode;
	SI16							m_OptionNoField;

	BOOL							m_bExistShop;	// actdoll (2004/05/26 17:58) : ���θ��� ���� ��� �̰��� true�� �ȴ�..

private:
	void			LoadImage();
	void			FreeImage();

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