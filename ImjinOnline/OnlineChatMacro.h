//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> �ݺ� ���ϱ� ����â
//	File Name		: OnlineChatMacro.h
//	Birth Date		: 2003. 01. 13.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: ������� �ֿܼ��� �ݺ� ���ϱ� ����� �����ϴ� â�� ����ϴ� Ŭ�����Դϴ�.
//
//===================================================================================================

#ifndef __ONLINE_CHATMACRO_H__
#define __ONLINE_CHATMACRO_H__

#pragma once

//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: ���� ��� ���� �� �ܺ� Ŭ���� ����
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: �ݺ� ���ϱ�â Ŭ����
//-----------------------------------------------------------------------------
class OnlineChatMacro
{
private:
	enum { OK, CANCEL, MAXCOUNT };
	enum { MAX_EDIT_BOX = 5 };

private:
	cltOnlineWorld				*m_pMyOnlineWorld;					// ���� ���� Ŭ����. �ܺο��� �޾ƿ�

	// actdoll (2004/05/07 12:28) : [OECEX]
//	vector<OnlineEditControl*>	m_EditBox_Slot;						// ����Ʈ �ڽ� UI - ��ũ�ο��� ����� �ؽ�Ʈ�� �Է��ϴ� ������ ���� �ڽ�
//	vector<RECT>				m_Size_EditBox;						// ����Ʈ �ڽ� UI - �� ����Ʈ �ڽ��� ��ȹ ����
	vector<HOEC>				m_hOecSlot;							// ����Ʈ �ڽ� UI - ��ũ�ο��� ����� �ؽ�Ʈ�� �Է��ϴ� ������ ���� �ڽ�
	_Button						m_Btn_Common[MAXCOUNT];				// ��ư UI - Ȯ��, ���

	XSPR						m_Spr_Frm;							// ��������Ʈ�� - ������
	XSPR						m_Spr_Btn_ScrollBar;				// ��������Ʈ�� - �ӵ� ���� ��ũ�� ���� �ڵ�

	RECT						m_Size_ScrollBar;					// �ӵ� ���� ��ũ�� ���� ����

	char						m_pszString[MAX_EDIT_BOX][512];		// ����Ʈ �ڽ����� ���� ��ũ�� ��

	SI32						m_siNoFieldHandle;					// �ʵ� ���� �Ұ��� ������ ���� �ڵ�
	SI32						m_siDrawIndex;						// ��ũ�� ���� �� ����� ä�� �޽����� �ε���
	SI32						m_siDrawTimeGap;					// ä�� �޽��� ��� ����(ms)

	SI32						m_siScrollBarXPos;					// ��ũ�� ���� ���� X ��ġ��

	DWORD						m_dwLastUpdateTime;					// ���������� ���� ��� �ð�

	BOOL						m_bIsRunning;						// ��ũ�θ� ���� ���״��� �ƴϳ�

	BOOL						m_bMousePush;
	BOOL						m_bIsActive;

public:
	OnlineChatMacro			( cltOnlineWorld *p ) : m_pMyOnlineWorld(p)	{ Init(); }
	~OnlineChatMacro		( void )									{ Free(); }

	void	Initialize		( void );
	void	Draw			( LPDIRECTDRAWSURFACE7 lpSurface );
	void	Action			( void );

	void	OnButtonPush	( void );	
	void	OnRunningToggle	( void );
	void	End				( void )									{ m_bIsRunning = FALSE; }

	BOOL	IsRunning		( void )									{ return m_bIsRunning; }
	BOOL	IsActive		( void )									{ return m_bIsActive; }

private:	
	void	Init			( void );
	void	Free			( void );
	
	void	SetData_Settings( void );
	void	SaveString		( void );


	void	Draw_Settings	( LPDIRECTDRAWSURFACE7 lpSurface );
	void	Action_Settings	( void );

	void	GetSpeedValue	( SI32 &siTimeGap, SI32 &siScrollBarXPos );

};
#endif//__ONLINE_CHATMACRO_H__