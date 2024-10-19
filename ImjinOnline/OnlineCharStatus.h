//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 캐릭터 정보 창
//	File Name		: OnlineCharStatus.h
//	Birth Date		: 2001. 12. 28.
//	Creator			: 이 준 석, 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용자 콘솔에서 캐릭터 상태창을 띄워주는 부분입니다.
//
//===================================================================================================

#ifndef __ONLINE_CHARSTATUS_H__
#define __ONLINE_CHARSTATUS_H__

//-----------------------------------------------------------------------------
// Name: OnlineCharStatus
// Code: All (2004-02-16)
// Desc: 포함 헤더 선언 및 외부 클래스 정의
//-----------------------------------------------------------------------------
#include <Graphics16.h>
#include <Button.h>

#define ON_CHAR_STATUS_NUM		4

class   _ParametaBox;
class	IOnlineFollower;
class	OnlineEditControl;

//-----------------------------------------------------------------------------
// Name: OnlineCharStatus
// Code: All (2004-02-16)
// Desc: 클래스 선언
//-----------------------------------------------------------------------------
class	OnlineCharStatus
{
private:
	cltOnlineWorld				*m_pMyOnlineWorld;									// 게임 몸통 클래스. 외부에서 받아옴
	IOnlineFollower				*m_pMyFollower;										// 용병 정보 클래스. 외부에서 받아옴

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅(이 하단의 포인터는 나중에 지워줄 것)
	OnlineEditControl			*m_pEditBox_Rename;									// 에디트 박스 UI - 상태 창에서 용병 이름 바꿀 때
	HOEC						m_hOecRename;										// 에디트 박스 UI 핸들- 상태 창에서 용병 이름 바꿀 때

	_Button						m_Btn_ReName;										// 버튼 UI - 이름 바꾸기
	_Button						m_Btn_GrowUp[ ON_CHAR_STATUS_NUM ];					// 버튼 UI - 능력치 UP
	_Button						m_Btn_Close;										// 버튼 UI - 창닫기

	XSPR						m_Spr_Frm, m_Spr_Btn_Rename, m_Spr_Btn_GrowUp, m_Spr_Btn_Common;

	SI16						m_siX,		m_siY;									// 상태창의 시작점 x, y
	SI16						m_siWidth,	m_siHeight;								// 상태창의 폭과 너비
	SI32						m_siAccountNo;										// 이 클라이언트 사용자가 서버에서 부여받은 계정 번호
	SI32						m_siStatus;											// 현재 상태창의 작동 형태를 지정한다.(?)
//	SI16						siFont;												// 초상 폰트. 현재는 상태창에서 초상 출력을 안하므로 안쓴다.
	SI32						m_siSelectedDBSlotNo;								// 현재 선택한 용병의 DB 슬롯 번호(0일경우 내 본케릭을 선택한 상태다)
	UI08						m_uiGrownUpDBSlotNo;								// 현재 서버로 성장을 요청한 용병의 DB 슬롯 번호. 서버에서 응답 후 실제 적용시키는 값은 이 값을 쓴다.
	UI08						m_uiGrownUpAbility;									// 현재 서버로 성장을 요청한 용병의 능력 종류. 서버에서 응답 후 실제 적용시키는 값은 이 값을 쓴다.
	SI16						m_siNoFieldHandle;									// 필드 지정 불가를 선언할 때의 핸들
	CHAR						m_pszCharName[ ON_ID_LENGTH + 1 ];					// 용병 이름을 변경을 한다.( 임시 버퍼 )

	BOOL						m_bMousePushStatus;									// 왼쪽 마우스 누름 상태.	
	SI16						m_siRenameBtnStatus;								// 이름 바꾸기 버튼 상태.
	UI08						m_uiRenameDBSlotNo;									// 이름을 바꾸려는 용병의 DB 슬롯 번호
	RECT						m_rtBook;

public:
	OnlineCharStatus		( void );
	~OnlineCharStatus		( void );

	VOID	Init			( SI32 siAccountNo, cltOnlineWorld *pOnlineWorld, IOnlineFollower *pFollower );
	VOID	Free			( void );
	VOID	Action			( void );
	VOID	Draw			( LPDIRECTDRAWSURFACE7 pSurface );

	VOID	SetActive		( BOOL bActive );
	BOOL	IsActive		( void );
	VOID	SetSelect		( UI08 uiDBSlotNo );
//	VOID	SetX(SI16 six);
	OnlineEditControl	*GetRenameEditBox() { return m_pEditBox_Rename; };		// actdoll : 요것도 나중에 삭제
	HOEC	GetHOECRename( void )	{ return m_hOecRename; }

//----------------------------------------------------------------------------
//	서버에서 패킷 수신 시 적용하는 함수
//----------------------------------------------------------------------------
public:
	VOID	RecvMsg_GrownUp(BOOL bFlag);
	VOID	RecvMsg_Rename(BOOL bFlag);
};	

#endif//__ONLINE_CHARSTATUS_H__
