//	수정 일자	:	2002-02-20 오후 2:22:07 - 양정모

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
	CHAR	szCommand[32];										// 명령의 최대 길이는 32자이다.
	SI32	siReqGrade;											// 이 명령을 하기 위하여 필요한 등급.(운영자, 일반 캐릭터), -1 이면 아무나 다 할 수 있다.
	CHAR	szReqClassInGuild[SO_MAX_GUILD_SC_LENGTH + 1];		// 이 명령을 하기 위하여 길드내에서 필요한 등급, 0 이면 아무나 다 할 수 있다.
};

class	cltCommand
{	
private:
	SOWorld					*pMyWorld;	
	static	strCommand		m_stCommand[SO_TOTAL_COMMAND_NUM];

	CHAR			m_szGMString[1024];	
public:
	BOOL	Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY);										// 캐릭터를 워프 시킨다.

	BOOL	CreateMonster(UI16 uiGMPlayerID, CHAR *pszMonsterName, UI16 uiMonsterLevel, UI16 uiMonsterNum);	// 몬스터를 생성한다.
	BOOL    CreateMonsterID(UI16 uiGMPlayerID, UI16 uiMonId, UI16 uiMonsterLevel, UI16 uiMonsterGroupNum);  // 몬스터를 생성한다

	BOOL	GiveMoney(UI16 uiPlayerID, MONEY Money);														// 캐릭터의 돈을 조정한다.
	
	BOOL	GiveItem(UI16 uiGMPlayerID, CHAR *pszItemName, UI16 uiItemQuantity);							// 아이템을 땅에 버려준다.
	BOOL	GiveItemID(UI16 uiGMPlayerID, UI16 uiItemID, UI16 uiItemQuantity);     							// 아이템을 땅에 버려준다.

	BOOL	LogOut(UI16 uiPlayerID);																		// 캐릭터를 추방한다.
	BOOL	Notice(CHAR *pszNotice);																		// 공지사항.
	BOOL	GuildNotice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice);									// 길드 공지사항.
	BOOL	GetPlayerInfo(UI16 uiGMPlayerID, UI16 uiPlayerID);												// 사용자의 정보를 얻어온다.
	BOOL	IDToAccount(UI16 uiGMPlayerID, CHAR *pszID);													// 사용자의 문자열 ID를 사용하여 숫자 Account를 얻어온다.
	BOOL	GetConnectUsersInfo(UI16 uiGMPlayerID);															// 현재 접속되어 있는 사용자들의 정보를 구한다.
	BOOL	GetFPS(UI16 uiGMPlayerID);																		// 현재 서버의 초당 프레임 수를 구한다.
	BOOL	BonusUp(UI16 uiPlayerID, UI08 uiFollowerID);													// 보너스를 증가 시켜준다.
	BOOL	CreditUp(UI16 uiPlayerID, SI32 siCredit);														// 신용도를 증가 시켜준다.
	BOOL	ExpUp( UI16 uiPlayerID, UI08 uiFollowerID, UI32 uiExp );										// 경험치를 증가 시킨다.
	BOOL	GetUserMailAddress(UI16 uiGMPlayerID, UI16 uiPlayerID);											// 사용자의 E-Mail을 얻어온다.
	BOOL	Whisper(UI16 uiPlayerID, CHAR *pszID, CHAR *pText);												// 사용자에게 귓속말을 보낸다.
	VOID	WriteStringToFile();																			// 화일에 저장한다.	
	BOOL	GetFieldMonsterInfo(UI16 uiGMPlayerID, SI32 siFieldMonsterID);									// FieldMonster의 정보를 얻어온다.
	BOOL	GetFollowerWeightInfo(UI16 uiGMPlayerID, UI16 uiPlayerID, UI08 uiFollowerID);					// Follower의 무게 정보를 얻어온다.
	BOOL	GetTileInfo(UI16 uiGMPlayerID, SI32 siX, SI32 siY);												// 타일의 정보를 얻어온다.
	BOOL	GetPlayerAccountByDBAccount(UI16 uiGMPlayerID, DWORD dwDBAccount);								// DB Account를 이용하여 플레이어의 Account를 이용한다.
	BOOL	KillNearMonster( UI16 uiMapID, SI32 siX, SI32 siY );											// 주위에 몬스터들을 죽인다.
	BOOL	GetTotalMonsterNum( UI16 uiGMPlayerID );														// 현재 살아있는 모든 몬스터의 수를 얻어온다.
	BOOL	ClearNearItem( SOPlayer *pGMPlayer );															// 플레이어가 있는 주위에 떨어진 아이템들을 모두 없앤다.
	BOOL	CompleteQuest_KillMonster( UI16 uiGMPlayerID, CHAR *pszMonsterName, UI16 uiMonsterNum );		// 몬스터를 죽이는 퀘스트를 수행한다.
	BOOL	CompleteQuest_DoWork( UI16 uiGMPlayerID, SI32 siWorkAmount );									// 작업량에 관계된 퀘스트를 수행한다.
	BOOL	PartyChat( SOPlayer *pPlayer, CHAR *pszChatMsg );												// 파티원들에게 채팅 메시지를 보낸다.
	BOOL	SendSMSMsg(SMS_REQ_MSG* pSMSMsg);																// SMS Messag 관련

	int     FindChar(CHAR *pszStr, CHAR find);
	DWORD   ConvertCode(char *szStr, int nSize);
	
	strCommand*	GetCommand(CHAR *pszCommand, UserGrade USGrade, SI32 siClassInGuild);						// 명령을 얻어온다.
public:
	cltCommand();												// 생성자.
	~cltCommand();											// 소멸자.

	VOID	Init(SOWorld *pmyworld);							// 초기화.
	BOOL	ProcessCMD(UI16 uiPlayerID, CHAR *pszCMD);			// 명령을 처리한다.	
};

#endif
