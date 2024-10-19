#ifndef	_LOGIN_DATA_HEADER
#define _LOGIN_DATA_HEADER


#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include "OnlineMsgBox.h"
#include "OnlineNotice.h"
#include "_FrontDefine.h"
#include "_LoginDefine.h"
#include "_FrontUserDefine.h"

// actdoll (2004/05/04 16:33) : [OECEX] 새 에디트 컨트롤러 세팅
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

enum
{   
	ON_LOGINMGR_LICENSE						= 0,
	ON_LOGINMGR_FRONTSERVER_CONNECT,
	ON_LOGINMGR_CLIENT_VERSION_CHECK,
	ON_LOGINMGR_CLIENT_VERSION_CHECK_WAIT,
	ON_LOGINMGR_NOTICE_REQUEST,
	ON_LOGINMGR_SERVER_INFO_REQUEST,
	ON_LOGINMGR_SERVER_SELECT,
	ON_LOGINMGR_GAMESERVER_LOGIN,
	ON_LOGINMGR_NOTICE,
	ON_LOGINMGR_ACCOUNT_LOGIN,
	ON_LOGINMGR_RECV_PWDATA,
	ON_LOGINMGR_CHAR_SELECT,
	ON_LOGINMGR_CHAR_CREATE,
	ON_LOGINMGR_IN_GAMEPOLL,
	ON_LOGINMGR_EXIT,
	ON_LOGINMGR_WAIT
};


enum
{   
	ON_RETURN_NONE		= 0,
	ON_RETURN_OK,
	ON_RETURN_EXIT,
	ON_RETURN_CREATE
};

// 조합형 캐릭터 관련 구조체
struct SMyChar 
{
	SI16							siImKind;
	CHAR							cDefaultBody[5];				// 캐릭터의 디폴트 몸(예: KMN)
	SI16							siHeroAniAction;				// Animation 그림을 찍는 Order상태
	SI16							siHeroBaseWClass;				// 무기에 따라 행동할 수 있는 동작 CLASS
	SI16							siHeroWeaponEnum;				// 캐릭터 애니메이션 속도
	SI16							siDir;							// 캐릭터의 현재 방향에 맞는 폰트번호를 계산할 때 필요한 변수
	SI16							siAniStep;						// 애니메이션 현재 단계
	SI32							siFont;							// 조합형 캐릭터는 캐릭터 애니메이션 현재 방향의 첫번째 폰트 번호(한 방향당 폰트 수 * DirectionNo)
	BOOL							bReverse;
	SI16							siCenterX, siCenterY;			// 캐릭터 중심 좌표
}; 

// 초기 로딩 화면
struct OnlineLoadingData
{
	UI08	uiFrame;
	XSPR	BackImage, BackOverImage, AniLogoImage, PlayGradeImage;
	XSPR	m_Spr_CompanyLogo;										// actdoll (2004/09/02 4:11) : 회사 로고
};

// 약관동의, 공지사항 화면
struct OnlineNoticeData
{
	_Button			OkButton, ExitButton;
	XSPR			NoticeImage;

	OnlineChatList	*m_pNoticeList;
};

// 서버 선택화면
struct OnlineSelectServerData
{
	FRONT::sWorldInfo*	pWorldInfo;								// 모든 월드들의 정보
	SI16				siWorldCount;							// 개설된 월드수

	SI16				siSelectStatus, siSelect;
	_Button				OkButton, ExitButton;
	XSPR				ServerBoxImage, ServerSelImage, PingImage, PingRingImage, ScrollAreaImage, *pScrollBarImage;

	SI32				m_siScroll;								// 맨위에 있는 서버의 인덱스
	RECT				m_rtScrollArea;
	SI32				m_siScrollBarY;
	float				m_fOldScrollY;

	OnlineSelectServerData()
	{
		pWorldInfo = NULL;
	}
};

// 로그인 화면에서 필요한 데이타들.
struct	OnlineLogInData
{	
//	OnlineEditControl		*pOnlineIDEditControl;								// ID 입력용 에디트 컨트롤.
//	OnlineEditControl		*pOnlinePassWordEditControl;						// 패스워드 입력용 에디트 컨트롤.	

	// actdoll (2004/05/04 16:33) : [OECEX] 새 에디트 컨트롤러 핸들 세팅
	HOEC					m_hOecID;											// ID입력용 에디트 컨트롤 핸들
	HOEC					m_hOecPW;											// 패스워드 입력용 에디트 컨트롤 핸들
	
	XSPR					LogInBoxImage;										// 로그인 이미지

	_Button					OkButton, ExitButton;								// OK 및 EXIT 버튼.(로그인시)
	_Button					RegistrationButton, JoyOnButton;

	SI32					siLogInStatus;										// 로그인 할때의 상태.
	SI16					siCount;											// 로그인 버튼를 누른 휫수
	CHAR					szID[LOGIN::ID_MAX_LENGTH], szPW[LOGIN::PW_MAX_LENGTH];
//	RECT					rtIDRect, rtPWRect;

	OnlineLogInData()
	{
//		pOnlineIDEditControl = NULL;
//		pOnlinePassWordEditControl = NULL;
		m_hOecID	= NULL;
		m_hOecPW	= NULL;
	}
};

// 케릭터를 선택 & 생성한다.
struct	OnlineSelectCharacterData
{
	XSPR					m_ImgButtonBaseSpr,m_ImgCharSelectBase;
	XSPR					m_ImgCharSelect[8];
	XSPR					m_ImgCharSelectBox;

	OnlineMsgBox			*pOnlineMsgBoxDel;									// 캐릭터 삭제 메시지 박스
	_Button					BExit, OkButton,BNewChar, BDelChar;					// 버튼들
	_Button					BSelect[3];
	OnlineNotice			*pOnlineNotice;
	SMyChar					*pMyChar; 


	SI16					siSelectKind[3];
	SI16					siStatus; 
	SI16					siMaxCharNum;										// 내가 가지고 있는 케릭터 수
	SI16					siKind;												// 주인공 케릭터 선택
	SI16					siNowChar;											// 현재 보여지고 있는 케릭터
	UI08					uiSlot;												// 현재 선택한 케릭터 슬롯
	UI08					uiDelSlot;											// 삭제될 슬롯
	DWORD					dwAnyTime;
	BOOL					bButtonAction;										// 캐릭삭제 메시지박스가 떠 있을때 버튼동작을 막음

	OnlineSelectCharacterData()
	{
		pOnlineMsgBoxDel = NULL;
		pOnlineNotice = NULL;
		pMyChar = NULL;
	}
};


// 서버로 부터 나의 데이타를 받는 동안의 화면에서 쓰이는 데이타들.
struct	OnlineGameServerLogIn
{
	SI16					siStatus;
	SI32					bMessageBoxFloating;								// 성공적으로 서버에 접속하였는가.
	DWORD					siLogInTime;										// 로그인 한 시간.	
	BOOL					bLogInOk;
};

// 새로운 캐릭터 생성 
struct	OnlineCreateCharacter
{
	SI16					siStatus;
	UI08					uiSlot;												// 비어 있는 슬롯 번호
	SI16					siKind;												// 선택한 케릭터
	CHAR					szID[LOGIN::NAME_MAX_LENGTH];						// 에디트 박스에서 받는 ID	
	UI08					Totalb, siStra, siDext, siHp2, siInteli;			// 보너스, 힘, 민첩성, 생명, 지력

//	OnlineEditControl		*pOnlinIDEditBox;									// ID 둥록용 에디트 컨트롤.	
	// actdoll (2004/05/04 16:33) : [OECEX] 새 에디트 컨트롤러 핸들 세팅
	HOEC					m_hOecCharName;										// 캐릭터 이름 에디트 컨트롤

	XSPR					m_ImgCharList, m_ImgSelectChar[8],m_ImgLargeChar[8];
	XSPR					m_ImgWhiteBox,m_ImgInfoBoxBase;
	XSPR					m_ImgInfoBoxUp,m_ImgInfoBoxDn;
	XSPR					m_ImgButtonSpr,m_ImgButtonBaseSpr;
	XSPR					m_ImgNewCharSpr;

	UI16					uiCharCode[100];									// 케릭터 종류
	SI32					siCharTotalNum;										// 총 케릭터 종류 수

	_Button					m_BttnSelectChar[8];
	_Button					m_BttnInfoBoxUp[5],m_BttnInfoBoxDn[5];
	_Button					m_BttnOk,m_BttnBack;

	OnlineCreateCharacter()
	{
//		pOnlinIDEditBox = NULL;
		m_hOecCharName	= NULL;
	}
};

// 로그인 전반에 걸쳐 필요한 데이타들.
struct	OnlineLogInMgr
{
	// 전반적인부분
	SI16						siStatus;
	BOOL						bMouseDown;
	POINT						ptMouse;
	DWORD						m_StartTickCount;
	BOOL						bStatusBack;							// 접속,연결 실패 메시지 확인시 상태되돌림
	DWORD						LogInTime;								// 5분후 종료를위해 로그인한 시간체크

	XSPR						BackImage, BackOverImage, AniLogoImage, LogoImage, PlayGradeImage;
	XSPR						SButtonImage, LButtonImage;
	XSPR						m_Spr_CompanyLogo;

	CHAR						szNotice[FRONT::NOTICE_MAX_LENGTH];		// 공지사항

	OnlineNoticeData			m_NoticeData;							// 약관동의
	OnlineSelectServerData		m_SelectServerData;						// 서버선택
	OnlineLogInData				m_LogInData;							// 계정 로그인
	OnlineSelectCharacterData	m_SelectCharData;						// 케릭터를 선택
	OnlineCreateCharacter		m_CreateCharacter;						// 케릭터를 생성
	OnlineGameServerLogIn		m_GameServerLogIn;						// 게임서버 로그인	
};

#endif