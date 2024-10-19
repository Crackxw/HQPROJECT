#ifndef _CLGAME_H
#define _CLGAME_H


#include <Main.h>
#include <Config.h>
#include <gamebutton.h>
#include <OnlineWorld.h>

// actdoll (2004/05/04 16:33) : [OECEX] �� ����Ʈ ��Ʈ�ѷ� ����
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

class clNumberRender;
class clItemInfoWindow;
class OnlineChatList;
class EffectDataMgr;


#define MAX_UPDATRE_CHAR_INFO_NUMBER	100


// ���� Ŭ����. 
class _clBattle{
public:
	//------------------------------------
	// ������ �۲ð� Ŀ�� ������ 
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
	// actdoll (2004/05/04 16:33) : [OECEX] �� ����Ʈ ��Ʈ�ѷ� �ڵ� ����
	HOEC	m_hOecChat;	// ä�� ������ �Է¿� �ڵ�

private:
	SI16 m_siExp;



public:
	_clBattle();
	~_clBattle();

	BOOL Init();

	//  ���ӿ� ���� ��Ʈ�� �����.
	void CreateGameFont();
	// ���ӿ� ���� ��Ʈ�� �����Ѵ�. 
	void DeleteGameFont();

	// �� ��ư�� ã�´�. 
	SI32 FindEmptyGameButton();
	SI32 FindEmptyGameButton(SI32 ButtonType);
	// ���� ��ư�� ������ �ǰ��Ͽ� �����Ѵ�. 
	void DeleteGameButtonByType(SI32 ButtonType);
	void InitGameButton();
	// �����߿� ������ ��ư���� ��Ƽ� �׷��ش�. 
	void DrawGameButton(LPDIRECTDRAWSURFACE7 surface);
	BOOL OperateGameButton();
	


	SI32 FindUpdateCharInfoChar();
	// ĳ���� ������ ������Ʈ �� �� �ִ� ĳ�����ΰ�?
	BOOL IsProperCharForUpdate(SI32 playernumber);
	BOOL UpdateCharInfo(SI32 updateid, SI32 killerid, _OrderData *ptemporder);
	void PushUpdateCharInfo(SI32 id, SI32 killerid = 0);
	BOOL PopUpdateCharInfo(_OrderData* pdata);

	// ���� ��� ����ġ���� �ݿ��Ѵ�. 
	void ExpOperate(SI32 killerid, SI32 vitimid);

	void SetExp(const SI32 siExp) {m_siExp = ~(siExp + 256);};
	SI32 GetExp(){return ~m_siExp - 256;};

	


};


// ���� Ŭ����. 
class _clGame{
public:
	//------------------------------------
	// ������ ������ 
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
	// �ȷ�Ʈ �迭�� 
	//------------------------------------
	unsigned char OriginalGamePalette[768];
	unsigned char ThunderPalette[768];               
	COLORREF      OriginalSysColor[24];

	// ��� �ڵ�
	int				m_iNation;				// actdoll (2004/05/24 21:21) : ���� ���α׷��� ���ư��� ���� �ڵ�
	int				m_iServiceType;			// actdoll (2004/05/24 21:21) : ���� ���α׷��� ���ư��� ���� Ÿ��
	ELanguageCode	LanguageCode;			// actdoll (2004/02/23 15:21) : ���� ���α׷��� ���ư��� ������ ��� �ڵ�
	const	char*	m_pszHQNation;			// actdoll (2004/02/23 15:21) : ���� ���α׷��� ������ HQ�� ������ ������
	unsigned int	m_uiCodePage;			// actdoll (2004/02/23 15:21) : �ش� ������ �������� �� �ڵ�������

public:
	_Config		Config;										// ȯ�� ���� 


public:
	_clGame();
	~_clGame();

	// Desc: �ʱ�ȭ 
	BOOL Init(HINSTANCE hInstance, int nCmdShow);

	// ������ ����. 
	BOOL CreateGameWindow(HINSTANCE hInstance, int nCmdShow);

	// ���̷�Ʈ ��ο� ����. 
	BOOL CreateGameDirectDraw();

	void Poll(MSG *msg);

	// Desc: ���α׷��� �����Ҷ� ó�� �ϴ� ��ƾ 
	void finiObjects(void);

	// �¶��� �κ��� �ʱ�ȭ�Ѵ�. 
	BOOL	InitOnlineWorld();

	// ���� ���� ������ �ʱ�ȭ�Ѵ�. 
	BOOL InitBattle();

	// ��Ÿ �������� �ʱ�ȭ�Ѵ�. 
	BOOL InitEtc();

	void ChatInit();


	void InitSoundManager();	// ���� �ʱ�ȭ
	void FreeSound();			// ����(ȿ����, �������, ���� �Ŵ���) ����



	UI16	OnlineWorld();
	VOID	FreeOnlineWorld();
	BOOL	IsOnlineWorld();
	BOOL OnlineSendHeartBeat();
	void ChattingPacketProc(char *pPacket);
	CHAR *GetItemName( UI16 uiItemID );
	// �¶��� ��� �޽������� Ŭ������ �����͸� ���´�.
	OnlineNoticeList* GetOnlineNoticeListPointer();


	// ���ӿ� ���� �ȷ�Ʈ�� �ҷ��´�. 
	void LoadPalette(unsigned char *pal=NULL);

	VOID MonitorMacro();

	/////////////////////////////////////////
	// __Lee_min_su__
	//
	// Ư�� �޼��� ���̵� �����Ѵ�.
	UINT AlterationWndMsg(UINT uMsg);
	// Ư�� �޼��� ���̵��� ������� ������.
	UINT OriginallyWndMsg(UINT uMsg);
	/////////////////////////////////////////
};

#endif
