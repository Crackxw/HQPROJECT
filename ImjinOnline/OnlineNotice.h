/************************************************************************************************
	���ϸ� : OnlineNotice.h
	����� : �̻��

	��  �� : ��������
************************************************************************************************/
#ifndef __ON_NOTICE__H
#define __ON_NOTICE__H


#include "OnlineChatList.h"


class OnlineNotice
{
private:
	cltOnlineWorld					*m_pMyOnlineWorld;										// �¶��� ���� ������
	OnlineChatList					*pNoticeList;
	_Button							m_BClose;

	BOOL							m_bAction;												// ���� Ȱ�����ΰ� �˾� ���� ����
	XSPR							m_BackImageSpr, m_Button2;
	SI16							m_siX, m_siY;											// ���� ��ǥ
	BOOL							m_bMouseDown;											// ���콺 ����
	BOOL							m_bClose;

public:
	OnlineNotice() : m_pMyOnlineWorld(0), m_bAction(FALSE), m_bClose(FALSE) {};
	~OnlineNotice() { Free(); };


	VOID			Init( cltOnlineWorld *pOnlineWorld );									// �ʱ�ȭ
	VOID			Action();																// �׼�
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );									// ���
	BOOL			IsAction()	{	return m_bAction;	};									// ���� Ȱ�� ���ΰ�?
	VOID			SetAction( const char* pNotice, BOOL bFlag );
	VOID			Free();																	// ����
	BOOL			IsClose()	{	return m_bClose;	};	
};

#endif