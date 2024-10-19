#ifndef	_ONLINEMSGBOX_H_
#define _ONLINEMSGBOX_H_

// 파일명 : OnlineMsgBox.h
// 담당자 : 정진욱

#define ON_MSGBOX_TYPE_NONE					0		// 메시지 박스 타입 - OK, Cancel 버튼이 2다 없음
#define ON_MSGBOX_TYPE_OK					1		// 메시지 박스 타입 - OK
#define ON_MSGBOX_TYPE_CANCEL				2		// 메시지 박스 타입 - Cancel
#define ON_MSGBOX_TYPE_OKCANCEL				3		// 메시지 박스 타입 - OK, Cancel

// robypark 2004/12/20 17:32
// ON_MSGBOX_TYPE_OKCANCEL와 동일한 형태. 다만 버튼 텍스트만 다르다.
#define ON_MSGBOX_TYPE_ACCEPTDECLINE		4		// 메시지 박스 타입 - Accept, Decline



#define ON_MSGBOX_RETURN_OK					1		// 'OK' 눌렀음
#define ON_MSGBOX_RETURN_CANCEL				2		// 'Cancel' 또는 'X' 눌렀음

// robypark 2004/12/20 17:32
// ON_MSGBOX_RETURN_OK, ON_MSGBOX_RETURN_CANCEL 동일한 형태. 다만 버튼 텍스트만 다르다.
#define ON_MSGBOX_RETURN_ACCEPT				1		// 'Accept' 눌렀음
#define ON_MSGBOX_RETURN_DECLINE			2		// 'Decline' 또는 'X' 눌렀음

#define ON_MSGBOX_MAX_CAPTION_LENGTH		256
#define ON_MSGBOX_MAX_MSG_LENGTH			1024


class OnlineMsgBox
{
private:
	cltOnlineWorld					*m_pOnlineWorld;
	SI32							m_siType;													// 메시지 박스 타입(OK, OK/Cancel)

	XSPR							*m_pXsprBox[2];												// 채팅창 판넬
	XSPR							*m_pXsprButt[2];											// 버튼
	XSPR							*m_pXsprButtBack;											// 버튼 뒷 배경
	UI08							*m_pButton;													// 이미지 버튼용

	SI32							m_siBoardXNum, m_siBoardYNum;								// 보드 가운데 부분의 타일 수(가로, 세로)
	POINT							m_ptBoxStart;												// 메시지 박스의 시작 좌표
	POINT							m_ptButtStart[2];											// 버튼의 시작 좌표

	CHAR							m_strCaption[ON_MSGBOX_MAX_CAPTION_LENGTH];					// 제목
	CHAR							m_strMsg[ON_MSGBOX_MAX_MSG_LENGTH];							// 내용
	BOOL							m_bMouseDown;												// 마우스 상태
	BOOL							m_bActive;													// 메시지 박스를 띄울 것인가?
	RECT							m_rtMsgBox;													// 메시지 박스 렉트
	SI16							m_siNoField;												// 노 필드 지역
	BOOL							m_bLogin;

public:
	_Button							m_Butt[2];													// OK, Cancel 버튼

private:	
	VOID	SetData(CHAR *pCaption, CHAR *pMsg);												// 멤버 변수값들 설정
	VOID	DrawBox();																			// 메시지 박스를 그린다.
	VOID	DrawText(HDC hDC);																	// 텍스트를 찍는다.

public:
	OnlineMsgBox() {}
	~OnlineMsgBox() {}

	VOID	Init(cltOnlineWorld *pOnlineWorld,BOOL b  = FALSE);													// 초기화
	VOID	SetMsg(SI32 type, CHAR *pCaption, CHAR *pMsgFormat, ...);							// 메시지를 설정한다.
	SI32	Process();																			// 현재 상태를 설정하고 리턴한다.
    BOOL	IsOk();																				// '확인' 버튼을 눌렀는가?
    BOOL	IsCancel();																			// '취소' 버튼을 눌렀는가?
	VOID	Draw( LPDIRECTDRAWSURFACE7 pSurface );												// 출력
	BOOL	IsActive();																			// 현재 메시지 박스가 출력 중인가?
	VOID	Initialize();																		// 메시지 박스를 초기화 한다.
	VOID	SetLocation(SI32 x, SI32 y);
	VOID	SetAction(BOOL flag);
};

#endif



