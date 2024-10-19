//	���� ����	:	2002-02-20 ���� 2:22:07 - ������

#ifndef	_COMMAND_HEADER
#define	_COMMAND_HEADER

#include "SOCommonHeader.h"
#include "GuildClassParser.h"
#include "..\\ServerMonitor\\SMSProtocol.h"

#define		CONVERT_CHARCODE_TO_DBCHARNO( character, number ) ( number << 8 | character )

enum	{	SO_COMMAND_WARP, SO_COMMAND_CREATEMONSTER, SO_COMMAND_GIVEMONEY, SO_COMMAND_GIVEITEM, SO_COMMAND_LOGOUT, SO_COMMAND_NOTICE,
			SO_COMMAND_GETINFO, SO_COMMAND_BONUSUP, SO_COMMAND_GNOTICE, SO_COMMAND_WHISPER, SO_COMMAND_CREDITUP, SO_COMMAND_EXPUP, 
			SO_COMMAND_KILL_NEAR_MONSTER, SO_COMMAND_CLEAR_NEAR_ITEM, SO_COMMAND_COMPLETE_QUEST, SO_COMMAND_PARTYCHAT, 
			SO_TOTAL_COMMAND_NUM};

struct	strCommand
{
	CHAR	szCommand[32];										// ����� �ִ� ���̴� 32���̴�.
	SI32	siReqGrade;											// �� ����� �ϱ� ���Ͽ� �ʿ��� ���.(���, �Ϲ� ĳ����), -1 �̸� �ƹ��� �� �� �� �ִ�.
	CHAR	szReqClassInGuild[SO_MAX_GUILD_SC_LENGTH + 1];		// �� ����� �ϱ� ���Ͽ� ��峻���� �ʿ��� ���, 0 �̸� �ƹ��� �� �� �� �ִ�.
};

class	cltCommand
{	
private:
	SOWorld					*pMyWorld;	
	static	strCommand		m_stCommand[SO_TOTAL_COMMAND_NUM];

	CHAR			m_szGMString[1024];	
public:
	BOOL	Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY);										// ĳ���͸� ���� ��Ų��.

	BOOL	CreateMonster(UI16 uiGMPlayerID, CHAR *pszMonsterName, UI16 uiMonsterLevel, UI16 uiMonsterNum);	// ���͸� �����Ѵ�.
	BOOL    CreateMonsterID(UI16 uiGMPlayerID, UI16 uiMonId, UI16 uiMonsterLevel, UI16 uiMonsterGroupNum);  // ���͸� �����Ѵ�

	BOOL	GiveMoney(UI16 uiPlayerID, MONEY Money);														// ĳ������ ���� �����Ѵ�.
	
	BOOL	GiveItem(UI16 uiGMPlayerID, CHAR *pszItemName, UI16 uiItemQuantity);							// �������� ���� �����ش�.
	BOOL	GiveItemID(UI16 uiGMPlayerID, UI16 uiItemID, UI16 uiItemQuantity);     							// �������� ���� �����ش�.

	BOOL	LogOut(UI16 uiPlayerID);																		// ĳ���͸� �߹��Ѵ�.
	BOOL	Notice(CHAR *pszNotice);																		// ��������.
	BOOL	GuildNotice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice);									// ��� ��������.
	BOOL	GetPlayerInfo(UI16 uiGMPlayerID, UI16 uiPlayerID);												// ������� ������ ���´�.
	BOOL	IDToAccount(UI16 uiGMPlayerID, CHAR *pszID);													// ������� ���ڿ� ID�� ����Ͽ� ���� Account�� ���´�.
	BOOL	GetConnectUsersInfo(UI16 uiGMPlayerID);															// ���� ���ӵǾ� �ִ� ����ڵ��� ������ ���Ѵ�.
	BOOL	GetFPS(UI16 uiGMPlayerID);																		// ���� ������ �ʴ� ������ ���� ���Ѵ�.
	BOOL	BonusUp(UI16 uiPlayerID, UI08 uiFollowerID);													// ���ʽ��� ���� �����ش�.
	BOOL	CreditUp(UI16 uiPlayerID, SI32 siCredit);														// �ſ뵵�� ���� �����ش�.
	BOOL	ExpUp( UI16 uiPlayerID, UI08 uiFollowerID, UI32 uiExp );										// ����ġ�� ���� ��Ų��.
	BOOL	GetUserMailAddress(UI16 uiGMPlayerID, UI16 uiPlayerID);											// ������� E-Mail�� ���´�.
	BOOL	Whisper(UI16 uiPlayerID, CHAR *pszID, CHAR *pText);												// ����ڿ��� �ӼӸ��� ������.
	VOID	WriteStringToFile();																			// ȭ�Ͽ� �����Ѵ�.	
	BOOL	GetFieldMonsterInfo(UI16 uiGMPlayerID, SI32 siFieldMonsterID);									// FieldMonster�� ������ ���´�.
	BOOL	GetFollowerWeightInfo(UI16 uiGMPlayerID, UI16 uiPlayerID, UI08 uiFollowerID);					// Follower�� ���� ������ ���´�.
	BOOL	GetTileInfo(UI16 uiGMPlayerID, SI32 siX, SI32 siY);												// Ÿ���� ������ ���´�.
	BOOL	GetPlayerAccountByDBAccount(UI16 uiGMPlayerID, DWORD dwDBAccount);								// DB Account�� �̿��Ͽ� �÷��̾��� Account�� �̿��Ѵ�.
	BOOL	KillNearMonster( UI16 uiMapID, SI32 siX, SI32 siY );											// ������ ���͵��� ���δ�.
	BOOL	GetTotalMonsterNum( UI16 uiGMPlayerID );														// ���� ����ִ� ��� ������ ���� ���´�.
	BOOL	ClearNearItem( SOPlayer *pGMPlayer );															// �÷��̾ �ִ� ������ ������ �����۵��� ��� ���ش�.
	BOOL	CompleteQuest_KillMonster( UI16 uiGMPlayerID, CHAR *pszMonsterName, UI16 uiMonsterNum );		// ���͸� ���̴� ����Ʈ�� �����Ѵ�.
	BOOL	CompleteQuest_DoWork( UI16 uiGMPlayerID, SI32 siWorkAmount );									// �۾����� ����� ����Ʈ�� �����Ѵ�.
	BOOL	PartyChat( SOPlayer *pPlayer, CHAR *pszChatMsg );												// ��Ƽ���鿡�� ä�� �޽����� ������.
	BOOL	SendSMSMsg(SMS_REQ_MSG* pSMSMsg);																// SMS Messag ����

	int     FindChar(CHAR *pszStr, CHAR find);
	DWORD   ConvertCode(char *szStr, int nSize);
	
	strCommand*	GetCommand(CHAR *pszCommand, UserGrade USGrade, SI32 siClassInGuild);						// ����� ���´�.
public:
	cltCommand();												// ������.
	~cltCommand();											// �Ҹ���.

	VOID	Init(SOWorld *pmyworld);							// �ʱ�ȭ.
	BOOL	ProcessCMD(UI16 uiPlayerID, CHAR *pszCMD);			// ����� ó���Ѵ�.	
};

#endif
