#ifndef _CLGAME_H
#define _CLGAME_H


#include <Main.h>
#include <Config.h>
#include <gamebutton.h>
#include <OnlineWorld.h>

// actdoll (2004/05/04 16:33) : [OECEX] 새 에디트 컨트롤러 세팅
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

class clNumberRender;
class clItemInfoWindow;
class OnlineChatList;
class EffectDataMgr;


#define MAX_UPDATRE_CHAR_INFO_NUMBER	100


// 전투 클래스. 
class _clBattle{
public:
	//------------------------------------
	// 윈도우 글꼴과 커서 변수들 
	//------------------------------------
	HFONT HelpFont;
	HFONT GetExpFont;
	HFONT TitleFont;
	HFONT StaffFont;
	HFONT SetGameFont;
	HFONT CharInfoFont;
	HFONT PlayerNameFont;
	HFONT BriefingFont;
	HFONT SaveInfoFont;
	HFONT ChatFont;
	HFONT VersionInfoFont;
	HFONT VersionInfoPlusFont;
	HFONT NormalEditFont;
	HFONT NormalEditPlusFont;
	HFONT ChatEditFont;
	HFONT WinLoseFont;
	HFONT IDFont;
	HFONT GameItemFont;

	HFONT HelpFont_Underline;
	HFONT GetExpFont_Underline;
	HFONT TitleFont_Underline;
	HFONT StaffFont_Underline;
	HFONT SetGameFont_Underline;
	HFONT CharInfoFont_Underline;
	HFONT PlayerNameFont_Underline;
	HFONT BriefingFont_Underline;
	HFONT SaveInfoFont_Underline;
	HFONT ChatFont_Underline;
	HFONT VersionInfoFont_Underline;
	HFONT VersionInfoPlusFont_Underline;
	HFONT NormalEditFont_Underline;
	HFONT NormalEditPlusFont_Underline;
	HFONT ChatEditFont_Underline;
	HFONT WinLoseFont_Underline;
	HFONT IDFont_Underline;
	HFONT GameItemFont_Underline;


	_GameButton GameButton[MAX_GAME_BUTTON];

	SI32 LastUpdateCharInfo;
	SI32 UpdateCharInfoDataIndex;
	char UpdateCharInfoData[MAX_UPDATRE_CHAR_INFO_NUMBER][200];

//	OnlineEditControl		m_EditControl;
	// actdoll (2004/05/04 16:33) : [OECEX] 새 에디트 컨트롤러 핸들 세팅
	HOEC	m_hOecChat;	// 채팅 데이터 입력용 핸들

private:
	SI16 m_siExp;



public:
	_clBattle();
	~_clBattle();

	BOOL Init();

	//  게임에 사용될 폰트를 만든다.
	void CreateGameFont();
	// 게임에 사용된 폰트를 해제한다. 
	void DeleteGameFont();

	// 빈 버튼을 찾는다. 
	SI32 FindEmptyGameButton();
	SI32 FindEmptyGameButton(SI32 ButtonType);
	// 게임 버튼을 종류에 의거하여 삭제한다. 
	void DeleteGameButtonByType(SI32 ButtonType);
	void InitGameButton();
	// 게임중에 나오는 버튼들을 모아서 그려준다. 
	void DrawGameButton(LPDIRECTDRAWSURFACE7 surface);
	BOOL OperateGameButton();
	


	SI32 FindUpdateCharInfoChar();
	// 캐릭터 정보를 업데이트 할 수 있는 캐릭터인가?
	BOOL IsProperCharForUpdate(SI32 playernumber);
	BOOL UpdateCharInfo(SI32 updateid, SI32 killerid, _OrderData *ptemporder);
	void PushUpdateCharInfo(SI32 id, SI32 killerid = 0);
	BOOL PopUpdateCharInfo(_OrderData* pdata);

	// 전투 결과 경험치등을 반영한다. 
	void ExpOperate(SI32 killerid, SI32 vitimid);

	void SetExp(const SI32 siExp) {m_siExp = ~(siExp + 256);};
	SI32 GetExp(){return ~m_siExp - 256;};

	


};


// 메인 클래스. 
class _clGame{
public:
	//------------------------------------
	// 윈도우 변수들 
	//------------------------------------
	HWND        Hwnd;
	HINSTANCE   hInst;
	BOOL		bPause;

	_clBattle		*pBattle;
	cltOnlineWorld	*pOnlineWorld;

	clNumberRender*		pNumberRender;
	clItemInfoWindow*	pItemInfoWindow;
	EffectDataMgr*		m_pEffectDataMgr;

	OnlineChatList		m_ChatList;

	//------------------------------------
	// 팔래트 배열들 
	//------------------------------------
	unsigned char OriginalGamePalette[768];
	unsigned char ThunderPalette[768];               
	COLORREF      OriginalSysColor[24];

	// 언어 코드
	int				m_iNation;				// actdoll (2004/05/24 21:21) : 현재 프로그램이 돌아가는 국가 코드
	int				m_iServiceType;			// actdoll (2004/05/24 21:21) : 현재 프로그램이 돌아가는 서비스 타입
	ELanguageCode	LanguageCode;			// actdoll (2004/02/23 15:21) : 현재 프로그램이 돌아가는 윈도의 언어 코드
	const	char*	m_pszHQNation;			// actdoll (2004/02/23 15:21) : 현재 프로그램이 참조할 HQ내 국가별 폴더명
	unsigned int	m_uiCodePage;			// actdoll (2004/02/23 15:21) : 해당 국가를 기준으로 한 코드페이지

public:
	_Config		Config;										// 환경 변수 


public:
	_clGame();
	~_clGame();

	// Desc: 초기화 
	BOOL Init(HINSTANCE hInstance, int nCmdShow);

	// 윈도우 생성. 
	BOOL CreateGameWindow(HINSTANCE hInstance, int nCmdShow);

	// 다이렉트 드로우 생성. 
	BOOL CreateGameDirectDraw();

	void Poll(MSG *msg);

	// Desc: 프로그램을 종료할때 처리 하는 루틴 
	void finiObjects(void);

	// 온라인 부분을 초기화한다. 
	BOOL	InitOnlineWorld();

	// 전투 관련 변수를 초기화한다. 
	BOOL InitBattle();

	// 기타 변수들을 초기화한다. 
	BOOL InitEtc();

	void ChatInit();


	void InitSoundManager();	// 사운드 초기화
	void FreeSound();			// 사운드(효과음, 배경음악, 사운드 매니저) 해제



	UI16	OnlineWorld();
	VOID	FreeOnlineWorld();
	BOOL	IsOnlineWorld();
	BOOL OnlineSendHeartBeat();
	void ChattingPacketProc(char *pPacket);
	CHAR *GetItemName( UI16 uiItemID );
	// 온라인 상단 메시지관련 클래스의 포인터를 얻어온다.
	OnlineNoticeList* GetOnlineNoticeListPointer();


	// 게임에 사용될 팔레트를 불러온다. 
	void LoadPalette(unsigned char *pal=NULL);

	VOID MonitorMacro();

	/////////////////////////////////////////
	// __Lee_min_su__
	//
	// 특정 메세지 아이디를 변경한다.
	UINT AlterationWndMsg(UINT uMsg);
	// 특정 메세지 아이디을 원래대로 돌린다.
	UINT OriginallyWndMsg(UINT uMsg);
	/////////////////////////////////////////
};

#endif
