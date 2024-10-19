#ifndef _ONLINE_PANNEL_H
#define _ONLINE_PANNEL_H

// ���ϸ� : OnlinePannel.h
// ����� : ���ؼ�

#include <iostream>
#include <vector>

#define ONLINEPANNEL_DISABLEFLAG_INVENTORY		0x00000001	// �κ��丮 ��� ��ư
#define ONLINEPANNEL_DISABLEFLAG_CHAT			0x00000002	// ä��â ũ�� ����
#define ONLINEPANNEL_DISABLEFLAG_TRADEBOOK		0x00000004	// ���
#define ONLINEPANNEL_DISABLEFLAG_MINIMAP		0x00000008	// �̴ϸ�
#define ONLINEPANNEL_DISABLEFLAG_OPTION			0x00000010	// �ɼ�
#define ONLINEPANNEL_DISABLEFLAG_ATTACK			0x00000020	// ���� �� ��ȭ���
#define ONLINEPANNEL_DISABLEFLAG_IMOTICON		0x00000040	// �̸�Ƽ��
#define	ONLINEPANNEL_DISABLEFLAG_CHARINFO		0x00000080	// �뺴â
#define	ONLINEPANNEL_DISABLEFLAG_TIP			0x00000100	// Quest, ǳ�� ����
#define	ONLINEPANNEL_DISABLEFLAG_PERMITTRADE	0x00000200	// �ŷ� ����, ���
#define	ONLINEPANNEL_DISABLEFLAG_PARTY			0x00000400	// ģ����� => ��Ƽ��
#define ONLINEPANNEL_DISABLEFLAG_STATUS			0x00000800	// ĳ���� ����â
#define ONLINEPANNEL_DISABLEFLAG_FRIEND			0x00001000	// ģ�����


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
// ������ ������ ���¿��� ��� ������ ���ֺ��� ��ư �������̽� ���� ó���� ����ü
struct SShowInformationAboutSiegeWarfareUnit
{
	// robypark 2004/11/13 13:58
	// ������ ���� ���� ���� ���� ��ư ��������Ʈ
	XSPR				m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis;		// ��ư �׵θ�
	XSPR				m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton;	// ��ư �̹���

	/*
	// robypark 2004/11/13 13:58
	// ������ ���� ���� ���� ���� ��ư (radio, checked or unchecked)
	_Button				m_Btn_ShowInformationAboutSiegeWarfareUnit;

	// robypark 2005/1/5 17:18
	// ������ ���ֿ��� => �Ϲ� �������� ���� ��û ��ư
	_Button				m_Btn_RequestReturnNormalCharacter;
	*/
	// ������ ������ ���� ��ư
	_Button				m_btnRequestTrasnformUI;

	SI32				m_siNoFieldHandle;		// ������ ���� �����ϱ� ��ư �������̽� No Field Area Index
};

// robypark 2005/1/27 22:6
class CSiegeWarfareUnitUI;
class CSiegeWarfareTransformUIWnd;

class	OnlinePannel 
{
	private:
		cltOnlineWorld		*m_pMyOnlineWorld;										// �ܺ� ������
		OnlineFieldChat		*m_pChatControl;										// ���� ������ - ä��
		OnlineSMS			*m_pSMS;												// ���� ������ - SMS
		
		SI08				m_siPollStatus;
		BOOL				m_bIsInVillage;											// ���� �����ȿ� �ִ��� Flag
		UI32				m_siDisableFlag;										// �������϶� Disable ���

		// robypark 2004/12/9 16:47
		// ������ ������ ���¿��� ��� ������ ���ֺ��� ��ư �������̽� ���� ó���� ����ü
		SShowInformationAboutSiegeWarfareUnit	m_sShowInformationAboutSiegeWarfareUnit;

		//-----------------------------------------------------------------------------
		//	��������Ʈ��
		//-----------------------------------------------------------------------------
		XSPR				m_SprImg_Pannel;										// ��������Ʈ - �̹��� - �г�
		XSPR				m_SprImg_PannelChatLine;								// ��������Ʈ - �̹��� - �г� �ϴ� �Է�â
		
		XSPR				m_SprIcon_Messenger;									// ��������Ʈ - ������ - ����
		XSPR				m_SprGauge_Hungry;										// ��������Ʈ - ���μ����� - �����

		XSPR				m_SprBtn_Inventory, m_SprBtn_CharStatus;				// ��������Ʈ - ��ư - �κ��丮, ĳ���� ����
		XSPR				m_SprBtn_Account, m_SprBtn_Mercenary;					// ��������Ʈ - ��ư - ���, �뺴
		XSPR				m_SprBtn_Quest, m_SprBtn_Friends;						// ��������Ʈ - ��ư - ����Ʈ, ģ�����
		XSPR				m_SprBtn_Option;										// ��������Ʈ - ��ư - �ɼ�â
		XSPR				m_SprBtn_Common;										// ��������Ʈ - ��ư - �Ϲݹ�ư

		XSPR				m_SprBtn_WhisperDestID;									// ��������Ʈ - ��ư - �Ӹ� ��� ���
		XSPR				m_SprBtn_ShowWhisperList, m_SprBtn_ChangeChatWindow;	// ��������Ʈ - ��ư - �Ӹ� ��� ��� ���̱�, ä�� ǥ��â �ٲٱ�
		XSPR				m_SprBtn_ShowImoticon;									// ��������Ʈ - ��ư - �̸�Ƽ�� ǥ��
		XSPR				m_SprBtn_MacroPlayPause, m_SprBtn_MacroSetting;			// ��������Ʈ - ��ư - ��ũ�� �÷���, ����

		//-----------------------------------------------------------------------------
		//	��ȹ��
		//-----------------------------------------------------------------------------
		POINT				m_pt_Pannel;											// Blt�� �ϱ� ���� Pannel Rect
		POINT				m_pt_PannelChatLine;									// Blt�� �ϱ� ���� Pannel Rect

		RECT				m_rt_Gauge_Hungry;										// ���� - ������
		RECT				m_rt_MyMoney, m_rt_MyGrade;								// ���� - ������, �ſ���
		RECT				m_rt_MyAttackPower, m_rt_Date;							// ���� - ������, ����ð�
		RECT				m_rt_ChatEdit, m_rt_Messenger;							// ���� - ä���Է�â, ���� ����

		//-----------------------------------------------------------------------------
		//	��ư��
		//-----------------------------------------------------------------------------
		_Button				m_Btn_Inventory, m_Btn_CharStatus;					// ��ư - �κ��丮, ĳ���� ����
		_Button				m_Btn_Account, m_Btn_Mercenary;						// ��ư - ���, �뺴
		_Button				m_Btn_Quest, m_Btn_Friends;							// ��ư - ����Ʈ, ģ�����
		_Button				m_Btn_Option;										// ��ư - �ɼ�â
		_Button				m_Btn_AggressiveMode, m_Btn_TradePermission;		// ��ư - �������, �ŷ����

		_Button				m_Btn_ShowWhisperList, m_Btn_ChangeChatWindow;		// ��ư - �Ӹ� ��� ��� ���̱�, ä�� ǥ��â �ٲٱ�
		_Button				m_Btn_ShowImoticon;									// ��ư - �̸�Ƽ�� ǥ��
		_Button				m_Btn_MacroPlayPause, m_Btn_MacroSetting;			// ��ư - ��ũ�� �÷���, ����

		BOOL				m_bMouseDown;	 									// ���콺 ��ư üũ
		BOOL				m_bShowMiniMap;										// �̴ϸ��� ����� �Ұ� �ΰ�

		SI32				VillagePositionX;
		SI32				VillagePositionY;
		SI16				m_siPannelNoField[3], m_siChatNoField; 
		SI32				m_siFieldBlock_ChatLine;							// ä�� ���� �ʵ� ���

		MONEY				TempYetMoney;
		SI32				TempYetBattlePower;
		SI32				TempYetCredit;

		BOOL				m_bWhisperMode;
		SI08				m_siWhisperSel;
		vector<WhisperData>	m_WhisperList;

	public:
	//	OnlineEditControl	*pOnlineChatEditControl;
		HOEC				m_hOecChat;											// ����Ʈ �ڽ� UI �ڵ�- 
		BOOL				m_bMessageShow;										// �÷��� - �޼��� ���� �˸� �����Ÿ� ǥ��
		DWORD				m_dwDistDelay;										// �÷��� - �޽��� ���� �˸� Ÿ�̹� ����
		BOOL				m_bHomeKey;											// ???

	protected:
		// robypark 2005/1/27 22:6
		CSiegeWarfareUnitUI				*m_pSiegeWarfareUnitUI;					// ������ ������ ��� �������̽�
		CSiegeWarfareTransformUIWnd		*m_pSiegeWarfareTransformUIWnd;			// ������ �������� ���� ����

	public:
		// robypark 2005/1/27 22:6
		// ������ ������ ��� �������̽� ���
		CSiegeWarfareUnitUI	*GetSiegeWarfareUnitUIHandle(void);

		// ������ �������� ���� ����
		CSiegeWarfareTransformUIWnd	*GetSiegeWarfareTransformUIWnd(void);

	public:
		OnlinePannel();															// ������.
		~OnlinePannel();														// �Ҹ���.

		VOID				Initialize();
		VOID				Init(cltOnlineWorld *pOnlineWorld);					// �ʱ�ȭ.
		VOID				Poll();												// ���� �Լ�.
		VOID				Draw(LPDIRECTDRAWSURFACE7 pSurface);				// ��ο� �Լ�.
		VOID				Free();												// ���� �Լ�.
		
		VOID				SetFlagVillage(BOOL bIsInVillage, UI32 Disable);	// �����ȿ� �ִ��� Flag
		VOID				SetVillagePos(SI32 VilXPos, SI32 VilYPos);			// ������ ��ġ
		BOOL				GetFlagOptionWindow(UI32 WindowType);				// â�� ���� �ִ��� �˻�

		BOOL				GetfIsVillage();									// ���� �������� Flag
		BOOL				GetVillagePosition(SI32& xPos, SI32& yPos);			// ���� ��ġ �˻�

		BOOL				ChangeChattingMode(UI08 ChattinMode);				// ä�ø�� ����
		UI08				GetChattingMode();									// ä�ø�� ���
		VOID				SetChatOpen( SI08 siHeight = 1 );	// ä��â ���� �ݱ�
		BOOL				AddChat(UI32 uiID, CHAR *pStr, UI08 uiChatMode);
		BOOL				AddChat(CHAR *pStr, SI32 siContentColor, UI08 uiChatMode);
		BOOL				AddChatID(CHAR *pID, CHAR *pStr, UI08 uiChatMode);

		BOOL				Check_ManagerOrder(char* lpChatting);					// �����ڿ� ������� �˻�
		BOOL				Process_ManagerOrder(char* lpChatting);					// �����ڿ� ��� ó��
		VOID				ModeChange(BOOL bFlag);									// ������� �ΰ� ��ȭ��� �ΰ�.
		VOID				TradeChange(BOOL bFlag);								// �ŷ���� �ΰ� ���ܸ�� �ΰ�.
		VOID				SetButtonText_PeaceMode(BOOL bPeace);					// robypark 2004/10/26 11:5 ��ȭ��� ��ư �ؽ�Ʈ�� �����ϱ�
		VOID				SetButtonText_TradeMode(BOOL bTrade);					// robypark 2004/10/26 11:5 �ŷ���� ��ư �ؽ�Ʈ�� �����ϱ�

		VOID				AddWhisperList(char* lpname);
		VOID				SetWhisperList(BOOL bAction);

		VOID				ChangeChatBasePos( SI32 siAddBottomLen );

		HOEC				GetHOECChat( void )	{ return m_hOecChat; }

		BOOL				AddSMSMessage( char *pszRecvTime, char *pszTelNumber, char *pszMessage );

		// robypark 2005/1/28 14:3
		// ������ ���� ���� ��ư Ȱ��ȭ ����
		void				SetShowTransformSiegeUnitBtn(BOOL bShow);

		// ������ ������ ��� �������̽� Ȱ��ȭ ����
		void				ShowWindow_SiegeWarfareUnitUI(DWORD dwShowWindow);
		
		// ������ ������ ��� �������̽� ���� ����
		void				SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow);

		// ������ ������ ��� �������̽� ���� �ı�
		void				SiegeWarfareUnitUI_DestroyWindow(void);

		// ������ �������� ���� ���� Ȱ��ȭ ����
		void				ShowWindow_SiegeWarfareTransformUIWnd(BOOL bShow);

		// robypark 2005/1/28 15:51
		// ������ ��� ���� ���� ����Ʈ ��û ����
		// ON_RESPONSE_LIST_GUILD_UNIT
		UI16				OnResponseSiegeWarfareUnitList(BYTE *pMsg);
};

#endif
 