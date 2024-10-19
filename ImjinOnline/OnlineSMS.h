//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - SMS 메시지 출력부
//	File Name		: OnlineSMS.cpp
//	Birth Date		: 2004. 06. 14.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 패널에서 SMS를 표시하는 부분입니다.
//
//===================================================================================================

#ifndef __ONLINE_SMS__
#define __ONLINE_SMS__

const	SI32	DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT	= 3000;		// SMS 데이터를 저장할 버퍼

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-06-14)
// Desc: SMS 클래스
//-----------------------------------------------------------------------------
class	OnlineSMS
{
private:
	cltOnlineWorld	*m_pMyOnlineWorld;						// 외부 포인터 - 게임 본체

	//-----------------------------------------------------------------------------
	//	스프라이트들
	//-----------------------------------------------------------------------------
	XSPR			m_SprImg_FrameNormal;					// 스프라이트 - 이미지 - 껍데기 보통
	XSPR			m_SprImg_FrameHide;						// 스프라이트 - 이미지 - 껍데기 최소
	XSPR			m_SprBtn_SetNormalSize;					// 스프라이트 - 버튼 - 보통 사이즈로 바꿈 버튼
	XSPR			m_SprBtn_SetHideSize;					// 스프라이트 - 버튼 - 최소 사이즈로 바꿈 버튼

	//-----------------------------------------------------------------------------
	//	구획들
	//-----------------------------------------------------------------------------
	POINT			m_pt_Frame;								// 시작점 - 껍데기
	RECT			m_rt_Message;							// 범위 - 메시지가 출력되는 범위

	//-----------------------------------------------------------------------------
	//	컨트롤들
	//-----------------------------------------------------------------------------
	_Button			m_Btn_SetNormalSize;					// 버튼 - 보통 사이즈로 바꿈
	_Button			m_Btn_SetHideSize;						// 버튼 - 최소 사이즈로 바꿈

	//-----------------------------------------------------------------------------
	//	플래그들
	//-----------------------------------------------------------------------------
	BOOL			m_bIsHideMode;							// 숨겨져 있는 형태의 모드인가?
	BOOL			m_bIsModeJustChanged;					// 방금전에 모드가 변경됐는가?
	BOOL			m_bIsFirstStart;						// 가장 먼저 보여지기 시작했는가?

	BOOL			m_bMouseDown;							// 마우스가 눌렀는가?
	SI32			m_siFieldBlock;							// 해당 컨트롤을 필드에서 블록시킬때의 할당 ID - 본체
	SI32			m_siFieldBlock_Button;					// 해당 컨트롤을 필드에서 블록시킬때의 할당 ID - 버튼


	//-----------------------------------------------------------------------------
	//	자료 데이터들
	//-----------------------------------------------------------------------------
	typedef struct 
	{
		char		m_pszRecvTime[16];						// 수신 시간
		char		m_pszTelNumber[32];						// 보낸 사람의 전화번호
		char		m_pszMessage[200];						// 보낸 사람의 메시지
	} ST_SMS_DATA, *PST_SMS_DATA;

	ST_SMS_DATA		m_SMSBuffer[DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT];		// SMS 버퍼
	SI32			m_siSMSBuffer_PopPos;									// 현재 추출 중인 위치
	SI32			m_siSMSBuffer_PushPos;									// 현재 삽입 중인 위치
	SI32			m_siSMSBuffer_Count;									// 들어온 SMS 메시지의 갯수

	char			m_pszShowMessage[256];					// 내부 플래그 - 하나의 메시지를 출력할 버퍼
	char			m_pszShowName[256];						// 내부 플래그 - 메시지를 보낸 장본인을 출력
	SI32			m_siShowPosition;						// 내부 플래그 - 메시지 출력 시 찍힌 위치
	BOOL			m_bWriteFinish;							// 내부 플래그 - 하나의 메시지가 다 찍힌 상태인가?
	DWORD			m_dwSMSStartTime;						// 내부 플래그 - 하나의 메시지가 완전히 보여지기 시작한 시간
	DWORD			m_dwTextStartTime;						// 내부 플래그 - 텍스트 한 글자가 출력된 시간
	DWORD			m_dwTextColor;							// 내부 플래그 - 텍스트 출력 색깔
	DWORD			m_dwNameColor;							// 내부 플래그 - 이름 출력 색깔
	



public:
	OnlineSMS();
	~OnlineSMS();

	void			Init			( cltOnlineWorld *pOnlineWorld );							// 초기화
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