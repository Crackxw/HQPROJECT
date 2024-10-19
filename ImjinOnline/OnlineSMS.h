//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - SMS �޽��� ��º�
//	File Name		: OnlineSMS.cpp
//	Birth Date		: 2004. 06. 14.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �гο��� SMS�� ǥ���ϴ� �κ��Դϴ�.
//
//===================================================================================================

#ifndef __ONLINE_SMS__
#define __ONLINE_SMS__

const	SI32	DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT	= 3000;		// SMS �����͸� ������ ����

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-06-14)
// Desc: SMS Ŭ����
//-----------------------------------------------------------------------------
class	OnlineSMS
{
private:
	cltOnlineWorld	*m_pMyOnlineWorld;						// �ܺ� ������ - ���� ��ü

	//-----------------------------------------------------------------------------
	//	��������Ʈ��
	//-----------------------------------------------------------------------------
	XSPR			m_SprImg_FrameNormal;					// ��������Ʈ - �̹��� - ������ ����
	XSPR			m_SprImg_FrameHide;						// ��������Ʈ - �̹��� - ������ �ּ�
	XSPR			m_SprBtn_SetNormalSize;					// ��������Ʈ - ��ư - ���� ������� �ٲ� ��ư
	XSPR			m_SprBtn_SetHideSize;					// ��������Ʈ - ��ư - �ּ� ������� �ٲ� ��ư

	//-----------------------------------------------------------------------------
	//	��ȹ��
	//-----------------------------------------------------------------------------
	POINT			m_pt_Frame;								// ������ - ������
	RECT			m_rt_Message;							// ���� - �޽����� ��µǴ� ����

	//-----------------------------------------------------------------------------
	//	��Ʈ�ѵ�
	//-----------------------------------------------------------------------------
	_Button			m_Btn_SetNormalSize;					// ��ư - ���� ������� �ٲ�
	_Button			m_Btn_SetHideSize;						// ��ư - �ּ� ������� �ٲ�

	//-----------------------------------------------------------------------------
	//	�÷��׵�
	//-----------------------------------------------------------------------------
	BOOL			m_bIsHideMode;							// ������ �ִ� ������ ����ΰ�?
	BOOL			m_bIsModeJustChanged;					// ������� ��尡 ����ƴ°�?
	BOOL			m_bIsFirstStart;						// ���� ���� �������� �����ߴ°�?

	BOOL			m_bMouseDown;							// ���콺�� �����°�?
	SI32			m_siFieldBlock;							// �ش� ��Ʈ���� �ʵ忡�� ��Ͻ�ų���� �Ҵ� ID - ��ü
	SI32			m_siFieldBlock_Button;					// �ش� ��Ʈ���� �ʵ忡�� ��Ͻ�ų���� �Ҵ� ID - ��ư


	//-----------------------------------------------------------------------------
	//	�ڷ� �����͵�
	//-----------------------------------------------------------------------------
	typedef struct 
	{
		char		m_pszRecvTime[16];						// ���� �ð�
		char		m_pszTelNumber[32];						// ���� ����� ��ȭ��ȣ
		char		m_pszMessage[200];						// ���� ����� �޽���
	} ST_SMS_DATA, *PST_SMS_DATA;

	ST_SMS_DATA		m_SMSBuffer[DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT];		// SMS ����
	SI32			m_siSMSBuffer_PopPos;									// ���� ���� ���� ��ġ
	SI32			m_siSMSBuffer_PushPos;									// ���� ���� ���� ��ġ
	SI32			m_siSMSBuffer_Count;									// ���� SMS �޽����� ����

	char			m_pszShowMessage[256];					// ���� �÷��� - �ϳ��� �޽����� ����� ����
	char			m_pszShowName[256];						// ���� �÷��� - �޽����� ���� �庻���� ���
	SI32			m_siShowPosition;						// ���� �÷��� - �޽��� ��� �� ���� ��ġ
	BOOL			m_bWriteFinish;							// ���� �÷��� - �ϳ��� �޽����� �� ���� �����ΰ�?
	DWORD			m_dwSMSStartTime;						// ���� �÷��� - �ϳ��� �޽����� ������ �������� ������ �ð�
	DWORD			m_dwTextStartTime;						// ���� �÷��� - �ؽ�Ʈ �� ���ڰ� ��µ� �ð�
	DWORD			m_dwTextColor;							// ���� �÷��� - �ؽ�Ʈ ��� ����
	DWORD			m_dwNameColor;							// ���� �÷��� - �̸� ��� ����
	



public:
	OnlineSMS();
	~OnlineSMS();

	void			Init			( cltOnlineWorld *pOnlineWorld );							// �ʱ�ȭ
	void			Free			( void );
	void			Initialize		( void );
	void			Action			( void );
	void			Draw			( LPDIRECTDRAWSURFACE7 pSurface );
	void			OnToggleShowMode( void );

	BOOL			IsHideMode			( void ) { return m_bIsHideMode; }
	BOOL			IsModeJustChanged	( void ) { return m_bIsModeJustChanged; }
	SI32			GetCurrentYSize		( void ) { return ( ( m_bIsHideMode ) ? m_SprImg_FrameHide.Header.Ysize : m_SprImg_FrameNormal.Header.Ysize ); }
	BOOL			PushSMSData			( char *pszRecvTime, char *pszTelNumber, char *pszMessage );

private:
	void			Action_MessageWork	( void );
	BOOL			PopSMSData			( void );

};

#endif//__ONLINE_SMS__