#ifndef _ONLINE_PANNEL_H
#define _ONLINE_PANNEL_H

// 파일명 : OnlinePannel.h
// 담당자 : 이준석

#include <iostream>
#include <vector>

#define ONLINEPANNEL_DISABLEFLAG_INVENTORY		0x00000001	// 인벤토리 토글 버튼
#define ONLINEPANNEL_DISABLEFLAG_CHAT			0x00000002	// 채팅창 크기 변경
#define ONLINEPANNEL_DISABLEFLAG_TRADEBOOK		0x00000004	// 장부
#define ONLINEPANNEL_DISABLEFLAG_MINIMAP		0x00000008	// 미니맵
#define ONLINEPANNEL_DISABLEFLAG_OPTION			0x00000010	// 옵션
#define ONLINEPANNEL_DISABLEFLAG_ATTACK			0x00000020	// 전투 및 평화모드
#define ONLINEPANNEL_DISABLEFLAG_IMOTICON		0x00000040	// 이모티콘
#define	ONLINEPANNEL_DISABLEFLAG_CHARINFO		0x00000080	// 용병창
#define	ONLINEPANNEL_DISABLEFLAG_TIP			0x00000100	// Quest, 풍선 도움말
#define	ONLINEPANNEL_DISABLEFLAG_PERMITTRADE	0x00000200	// 거래 차단, 허용
#define	ONLINEPANNEL_DISABLEFLAG_PARTY			0x00000400	// 친구목록 => 파티원
#define ONLINEPANNEL_DISABLEFLAG_STATUS			0x00000800	// 캐릭터 상태창
#define ONLINEPANNEL_DISABLEFLAG_FRIEND			0x00001000	// 친구목록


#define CHATTINGMODE_NORMAL					0
#define CHATTINGMODE_GUILD					1
#define CHATTINGMODE_PARTY					2

#define CHAT_HEIGHT_CLOSE					0
#define CHAT_HEIGHT_MEDIUM					1
#define CHAT_HEIGHT_HIGH					2


using namespace std;


#define WHISPER_LIST_COUNT		10

struct WhisperData
{
	CHAR	szName[ON_ID_LENGTH + 1];
	SI16	siX, siY;
	BOOL	bSelect;

	WhisperData()
	{
		ZeroMemory(szName, sizeof(szName));
		siX = 0;
		siY = 0;
		bSelect = FALSE;
	}
};

// robypark 2004/12/9 16:47
// 공성전 유닛인 상태에서 모든 공성전 유닛보기 버튼 인터페이스 관련 처리용 구조체
struct SShowInformationAboutSiegeWarfareUnit
{
	// robypark 2004/11/13 13:58
	// 공성전 유닛 상태 정보 보기 버튼 스프라이트
	XSPR				m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis;		// 버튼 테두리
	XSPR				m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton;	// 버튼 이미지

	/*
	// robypark 2004/11/13 13:58
	// 공성전 유닛 상태 정보 보기 버튼 (radio, checked or unchecked)
	_Button				m_Btn_ShowInformationAboutSiegeWarfareUnit;

	// robypark 2005/1/5 17:18
	// 공성전 유닛에서 => 일반 유닛으로 변신 요청 버튼
	_Button				m_Btn_RequestReturnNormalCharacter;
	*/
	// 공성전 변신을 위한 버튼
	_Button				m_btnRequestTrasnformUI;

	SI32				m_siNoFieldHandle;		// 공성전 유닛 변신하기 버튼 인터페이스 No Field Area Index
};

// robypark 2005/1/27 22:6
class CSiegeWarfareUnitUI;
class CSiegeWarfareTransformUIWnd;

class	OnlinePannel 
{
	private:
		cltOnlineWorld		*m_pMyOnlineWorld;										// 외부 포인터
		OnlineFieldChat		*m_pChatControl;										// 내부 포인터 - 채팅
		OnlineSMS			*m_pSMS;												// 내부 포인터 - SMS
		
		SI08				m_siPollStatus;
		BOOL				m_bIsInVillage;											// 현재 마을안에 있는지 Flag
		UI32				m_siDisableFlag;										// 마을안일때 Disable 목록

		// robypark 2004/12/9 16:47
		// 공성전 유닛인 상태에서 모든 공성전 유닛보기 버튼 인터페이스 관련 처리용 구조체
		SShowInformationAboutSiegeWarfareUnit	m_sShowInformationAboutSiegeWarfareUnit;

		//-----------------------------------------------------------------------------
		//	스프라이트들
		//-----------------------------------------------------------------------------
		XSPR				m_SprImg_Pannel;										// 스프라이트 - 이미지 - 패널
		XSPR				m_SprImg_PannelChatLine;								// 스프라이트 - 이미지 - 패널 하단 입력창
		
		XSPR				m_SprIcon_Messenger;									// 스프라이트 - 아이콘 - 쪽지
		XSPR				m_SprGauge_Hungry;										// 스프라이트 - 프로세스바 - 배고픔

		XSPR				m_SprBtn_Inventory, m_SprBtn_CharStatus;				// 스프라이트 - 버튼 - 인벤토리, 캐릭터 상태
		XSPR				m_SprBtn_Account, m_SprBtn_Mercenary;					// 스프라이트 - 버튼 - 장부, 용병
		XSPR				m_SprBtn_Quest, m_SprBtn_Friends;						// 스프라이트 - 버튼 - 퀘스트, 친구목록
		XSPR				m_SprBtn_Option;										// 스프라이트 - 버튼 - 옵션창
		XSPR				m_SprBtn_Common;										// 스프라이트 - 버튼 - 일반버튼

		XSPR				m_SprBtn_WhisperDestID;									// 스프라이트 - 버튼 - 귓말 상대 목록
		XSPR				m_SprBtn_ShowWhisperList, m_SprBtn_ChangeChatWindow;	// 스프라이트 - 버튼 - 귓말 상대 목록 보이기, 채팅 표시창 바꾸기
		XSPR				m_SprBtn_ShowImoticon;									// 스프라이트 - 버튼 - 이모티콘 표시
		XSPR				m_SprBtn_MacroPlayPause, m_SprBtn_MacroSetting;			// 스프라이트 - 버튼 - 매크로 플레이, 세팅

		//-----------------------------------------------------------------------------
		//	구획들
		//-----------------------------------------------------------------------------
		POINT				m_pt_Pannel;											// Blt를 하기 위한 Pannel Rect
		POINT				m_pt_PannelChatLine;									// Blt를 하기 위한 Pannel Rect

		RECT				m_rt_Gauge_Hungry;										// 범위 - 포만감
		RECT				m_rt_MyMoney, m_rt_MyGrade;								// 범위 - 소지금, 신용등급
		RECT				m_rt_MyAttackPower, m_rt_Date;							// 범위 - 전투력, 현재시간
		RECT				m_rt_ChatEdit, m_rt_Messenger;							// 범위 - 채팅입력창, 쪽지 도착

		//-----------------------------------------------------------------------------
		//	버튼들
		//-----------------------------------------------------------------------------
		_Button				m_Btn_Inventory, m_Btn_CharStatus;					// 버튼 - 인벤토리, 캐릭터 상태
		_Button				m_Btn_Account, m_Btn_Mercenary;						// 버튼 - 장부, 용병
		_Button				m_Btn_Quest, m_Btn_Friends;							// 버튼 - 퀘스트, 친구목록
		_Button				m_Btn_Option;										// 버튼 - 옵션창
		_Button				m_Btn_AggressiveMode, m_Btn_TradePermission;		// 버튼 - 전투모드, 거래허용

		_Button				m_Btn_ShowWhisperList, m_Btn_ChangeChatWindow;		// 버튼 - 귓말 상대 목록 보이기, 채팅 표시창 바꾸기
		_Button				m_Btn_ShowImoticon;									// 버튼 - 이모티콘 표시
		_Button				m_Btn_MacroPlayPause, m_Btn_MacroSetting;			// 버튼 - 매크로 플레이, 세팅

		BOOL				m_bMouseDown;	 									// 마우스 버튼 체크
		BOOL				m_bShowMiniMap;										// 미니맵을 출력을 할것 인가

		SI32				VillagePositionX;
		SI32				VillagePositionY;
		SI16				m_siPannelNoField[3], m_siChatNoField; 
		SI32				m_siFieldBlock_ChatLine;							// 채팅 라인 필드 블록

		MONEY				TempYetMoney;
		SI32				TempYetBattlePower;
		SI32				TempYetCredit;

		BOOL				m_bWhisperMode;
		SI08				m_siWhisperSel;
		vector<WhisperData>	m_WhisperList;

	public:
	//	OnlineEditControl	*pOnlineChatEditControl;
		HOEC				m_hOecChat;											// 에디트 박스 UI 핸들- 
		BOOL				m_bMessageShow;										// 플래그 - 메세지 도착 알림 깜빡거림 표현
		DWORD				m_dwDistDelay;										// 플래그 - 메시지 도착 알림 타이밍 간격
		BOOL				m_bHomeKey;											// ???

	protected:
		// robypark 2005/1/27 22:6
		CSiegeWarfareUnitUI				*m_pSiegeWarfareUnitUI;					// 공성전 유닛일 경우 인터페이스
		CSiegeWarfareTransformUIWnd		*m_pSiegeWarfareTransformUIWnd;			// 공성전 유닛으로 변신 윈도

	public:
		// robypark 2005/1/27 22:6
		// 공성전 유닛일 경우 인터페이스 얻기
		CSiegeWarfareUnitUI	*GetSiegeWarfareUnitUIHandle(void);

		// 공성전 유닛으로 변신 윈도
		CSiegeWarfareTransformUIWnd	*GetSiegeWarfareTransformUIWnd(void);

	public:
		OnlinePannel();															// 생성자.
		~OnlinePannel();														// 소멸자.

		VOID				Initialize();
		VOID				Init(cltOnlineWorld *pOnlineWorld);					// 초기화.
		VOID				Poll();												// 폴링 함수.
		VOID				Draw(LPDIRECTDRAWSURFACE7 pSurface);				// 드로우 함수.
		VOID				Free();												// 해제 함수.
		
		VOID				SetFlagVillage(BOOL bIsInVillage, UI32 Disable);	// 마을안에 있는지 Flag
		VOID				SetVillagePos(SI32 VilXPos, SI32 VilYPos);			// 마을의 위치
		BOOL				GetFlagOptionWindow(UI32 WindowType);				// 창을 열수 있는지 검사

		BOOL				GetfIsVillage();									// 현재 마을인지 Flag
		BOOL				GetVillagePosition(SI32& xPos, SI32& yPos);			// 마을 위치 검사

		BOOL				ChangeChattingMode(UI08 ChattinMode);				// 채팅모드 변경
		UI08				GetChattingMode();									// 채팅모드 얻기
		VOID				SetChatOpen( SI08 siHeight = 1 );	// 채팅창 열고 닫기
		BOOL				AddChat(UI32 uiID, CHAR *pStr, UI08 uiChatMode);
		BOOL				AddChat(CHAR *pStr, SI32 siContentColor, UI08 uiChatMode);
		BOOL				AddChatID(CHAR *pID, CHAR *pStr, UI08 uiChatMode);

		BOOL				Check_ManagerOrder(char* lpChatting);					// 관리자용 명령인지 검사
		BOOL				Process_ManagerOrder(char* lpChatting);					// 관리자용 명령 처리
		VOID				ModeChange(BOOL bFlag);									// 전투모드 인가 평화모드 인가.
		VOID				TradeChange(BOOL bFlag);								// 거래모드 인가 차단모드 인가.
		VOID				SetButtonText_PeaceMode(BOOL bPeace);					// robypark 2004/10/26 11:5 평화모드 버튼 텍스트만 설정하기
		VOID				SetButtonText_TradeMode(BOOL bTrade);					// robypark 2004/10/26 11:5 거래모드 버튼 텍스트만 설정하기

		VOID				AddWhisperList(char* lpname);
		VOID				SetWhisperList(BOOL bAction);

		VOID				ChangeChatBasePos( SI32 siAddBottomLen );

		HOEC				GetHOECChat( void )	{ return m_hOecChat; }

		BOOL				AddSMSMessage( char *pszRecvTime, char *pszTelNumber, char *pszMessage );

		// robypark 2005/1/28 14:3
		// 공성전 유닛 변신 버튼 활성화 설정
		void				SetShowTransformSiegeUnitBtn(BOOL bShow);

		// 공성전 유닛일 경우 인터페이스 활성화 설정
		void				ShowWindow_SiegeWarfareUnitUI(DWORD dwShowWindow);
		
		// 공성전 유닛일 경우 인터페이스 윈도 생성
		void				SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow);

		// 공성전 유닛일 경우 인터페이스 윈도 파괴
		void				SiegeWarfareUnitUI_DestroyWindow(void);

		// 공성전 유닛으로 변신 윈도 활성화 설정
		void				ShowWindow_SiegeWarfareTransformUIWnd(BOOL bShow);

		// robypark 2005/1/28 15:51
		// 공성전 상단 공성 유닛 리스트 요청 응답
		// ON_RESPONSE_LIST_GUILD_UNIT
		UI16				OnResponseSiegeWarfareUnitList(BYTE *pMsg);
};

#endif
 