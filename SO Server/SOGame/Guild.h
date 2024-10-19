// 수정 일자 : 2002-02-18 오전 11:15:33 - 양정모

#ifndef	_GUILD_HEADER
#define _GUILD_HEADER

#include "SOCommonHeader.h"
#include "GSCDefine.h"
#include "SOVillage.h"
#include "Stack.h"
#include "GuildMemberManager.h"


#define	ON_GUILD_RELATION_ALLY			1			// 동맹
#define	ON_GUILD_RELATION_ENEMY			2			// 적
#define	ON_GUILD_RELATION_NEUTRAL		3			// 중립			사용x
#define	ON_GUILD_RELATION_MINE			4			// 나의 상단.	사용x
#define ON_GUILD_RELATION_WAR			5			// 공성

#define ON_GUILD_UNIT_SWORD				0			// 검병
#define ON_GUILD_UNIT_SPEAR				1			// 창병
#define ON_GUILD_UNIT_HORSE				2			// 기마병
#define ON_GUILD_UNIT_ARROW				3			// 궁병
#define ON_GUILD_UNIT_DEFENSE			4			// 수비병
#define ON_GUILD_UNIT_RAMPART			5			// 충차.
#define ON_GUILD_UNIT_SIEGE				6			// 발석거.
#define ON_GUILD_UNIT_SUPPLY			7			// 보급수레.
#define ON_MAX_GUILD_UNIT				8

#define ON_MAX_GUILD_RESULT				20



enum {
	    ON_GUILD_GIVEUP_NORMAL = 0,
        ON_GUILD_GIVEUP_NOT_ANSWER,
//		ON_GUILD_GIVEUP_NOT_SEND,
		ON_GUILD_GIVEUP_OK,
		ON_GUILD_GIVEUP_NO,
};

//상단의 공성유닛을 관리하는 클래스.
class   cltGuildUnit				
{
private:
	SI16 siGuildUnit[ON_MAX_GUILD_UNIT];								// 현재 가지고 있는 공성 유닛.
public:
	void	InitGuildUnit();
	void	SetGuildUnit(SI16 siUnitKind,SI16 siCount);
	SI16	IncreaseGuildUnit(SI16 siUnitKind,SI16 siCount);	// 현재 가지고 있는 공성 유닛을 증가 시킨다.
	SI16	DecreaseGuildUnit(SI16 siUnitKind,SI16 siCount);
	SI16	GetGuildUnit(SI16 uiUnitKind);

};

class	cltGuildResult
{
private:
	SI16 siVillageCode;
	SI16 siPercent;
	
public:
	char szCharName[ON_ID_LENGTH + 1];

	void	SetCharName(char* szName);
	char*   GetCharName();
	void	SetVillageCode(SI16	siVillageCode);
	SI16	GetVillageCode();
	void	SetPercent(SI16	siPer);
	SI16	GetPercent();
	
};

/*class   cltVillageDamageByGuild
{
	SI16 siVillageCode;
	SI32 siSoldierDamage;
};*/

typedef struct tag_giveUpInfo
{
	SI32            uiVillageCode;  //함락된 마을
	SI16            siGiveUpFlag;   //
	MONEY           mnMoney;        //공성포기 비용

}GIVEUP_INFO, *LP_GIVEUP_INFO;

class	cltGuild
{
private:	
	UI32				m_uiArrayIndex;								// 배열 인덱스.
	UI32				m_uiGuildID;								// 상단의 ID
	CHAR				m_szGuildName[ON_GUILDNAME_LENGTH + 1];		// 상단의 이름.
	CHAR				m_szDAIID[ON_ID_LENGTH + 1];				// 대방의 이름.

	SI32				m_siFlag;									// 깃발.
	SI32				m_siFlagIDInStack;							// 스택에서의 깃발 ID.
	SStack<UI16>		m_Villages;									// 마을들.	

	GIVEUP_INFO         m_GiveUpInfo;
	BOOL                m_bIsWar;                                   // 공성전을 할수있나?
	
public:
	
	GuildMemberManager	m_GuildMemberManager;						// 현 상단에 모든 상단원들을 관리한다.
	SI32				m_siLoginedGuildMemeberNum;					// 현재 로그인한 상단원들의 수.

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//공성 승리시의 권한.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	cltGuildResult		m_stGuildResult[ON_MAX_GUILD_RESULT];		// 상단이 공격한 마을의 VillageCode와 
																	// 폭투 Percent가 들어간다.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//상단 물품 관련.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	cltGuildUnit		m_GuildUnit;								// 상단이 가지고 있는 공성 유닛.
	SI32				m_siGuildSupply;							// 상단이 가지고 있는 보급품의 갯수.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//적대 관련
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	SI32				m_siGuildRelation[SO_TOTAL_GUILD_NUM];				// 각 상단과의 관계.
	SI32				m_siGuildRelationEnemyTime[SO_TOTAL_GUILD_NUM];		// 각 상단이 적대관계일 때 적대를 신청한 시간.
																			// 적대 걸고 3일후 공성이 시작된다.
	char				m_szGuildfriendlyMessage[SO_TOTAL_GUILD_NUM][200];	// 화친을 원할 경우 메세지를 보낸다.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
private:

	static	SOWorld	*m_pMyWorld;
private:
	UI32	FindEmptySlot();						// 멤버를 추가할 비어있는 슬롯을 찾는다.	
	UI32	FindSlot(UI16 uiAccount);				// 멤버가 설정되어 있는 슬롯을 찾는다.		

	//VOID	UpdateMaxRealMemberNum();				// 보유하고 있는 마을의 수를 설정한다.
public:
	cltGuild();										// 생성자.
	~cltGuild();									// 소멸자.

	VOID	OnceInit(SI32 siTotalVillageNum);		// 한번만 초기화 해줄것들을 해준다.
	VOID	Init();									// 멤버 변수들을 초기화한다.	
	VOID	CreateGuild(UI32 uiArrayIndex, 
						UI32 uiGuildID, 
						CHAR *pszGuildName,
						SOPlayer *pPlayer,
						SOVillage *pVillage);			// 상단을 생성한다.
	VOID	InitGuild(	UI32 uiArrayIndex, 
						UI32 uiGuildID, 
						CHAR *pszGuildName,
						CHAR *pszDAIID,						
						UI32 uiGuildMemeberNum, SI32 siFlag, 
						SI32 siFlagIDInStack);		// DB에서 읽어와 상단을 읽어온다.
	BOOL	DeleteGuild();							// 상단을 삭제한다.	

	UI32	GetGuildID();							// 상단의 ID를 얻어온다.
	CHAR*	GetGuildName();							// 상단의 이름을 얻어온다.	
	CHAR*	GetDAIID();								// 대방의 ID(문자열)을 얻어온다.
	UI16	GetOwnerVillageNum();					// 소유하고 있는 마을의 수를 넘겨준다.
	UI32	GetGuildMemberNum();					// 멤버 회원수를 넘겨준다.
	BOOL	GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass);		// 캐릭터 이름을 사용하여 상단의 ID와 길드 직급을 얻어온다.


	BOOL	AddGuildMember(CHAR *pszName);										// 상단에 상단 멤버가 추가되었다.
	BOOL	DelGuildMember(CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick);		// 상단에서 상단 멤버가 삭제되었다. bKick이 셋되어있으면 쫓아낸것임
	BOOL	AddCandidate(SOPlayer *pPlayer);									// 지원자를 추가한다.	
	VOID	AddVillage(SOVillage *pVillage, BOOL bInit);						// 상단에 마을이 추가되었다.
	BOOL	DelVillage(SOVillage *pVillage, BOOL bDeleteGuild);					// 상단에서 마을이 삭제되었다.

	BOOL	IsValidGuild();													// 유효한 길드인지 알아온다.
	BOOL	Notice(CHAR *pszName, CHAR *pszNotice);							// 공지사항을 보내준다.	

	UI32	GetArrayIndex();						// 배열 인덱스를 얻어옴.

	BOOL	ChangeFlag(SI32 siFlag);				// 깃발을 설정한다.
	SI32	GetFlag();								// 깃발을 얻어온다.

	BOOL	SetHNS(SOVillage *pVillage);									// 행수를 지정한다.

	static	VOID InitStaticVariable(SOWorld *pWorld);	// 정적 변수를 초기화 한다.

	VOID	SetFlagIDInStack(SI32 siFlagIDInStack);		// 스택에서의 깃발 ID를 설정한다.
	SI32	GetFlagIDInStack();							// 스택에서의 깃발 ID를 얻어온다.

	BOOL	ChangeClass( CHAR *pszName, SI32 siNewClass );	// 직급을 변경시킨다.
	VOID	GetOwnerVillages(UI16 *puiOwnerVillageNum, UI16 *puiVillageCode);								// 현재 이 마을에 소속된 마을의 코드를 얻어온다.
	VOID	GetOwnerGuildVillageInfo( UI16 *puiOwnerVillageNum, OnGuildVillageInfo *pGuildVillageInfo );	// 현재 이 마을에 소속된 마을의 기본 정보를 얻어온다.

	DWORD	GetRMTotalPageNum();															// 전체 정식 상단원을 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
	DWORD	GetCMTotalPageNum();															// 전체 지원자를 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
	UI32	GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo);					// 정식 상단원의 리스트를 얻어온다.
	UI32	GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo);						// 지원자 리스트를 얻어온다.

	BOOL	SetMemberInfoFromDB();															// DB로 부터 모든 상단원의 정보를 얻어온다.

	VOID	SendMsgToGuildMemebers(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL);	// 접속해 있는 상단원에게 메시지를 보내준다.		

	VOID	GuildMemberLogin();
	VOID	GuildMemberLogout();

	void	GuildInfoChanged();
	BOOL	GetAuditGuildInfo(SOPlayer* pPlayer);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 공성 관련 추가,수정 함수들
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID	InitRelation(UI32 uiArrayIndex);												// 상단의 관계를 초기화 한다.
	VOID	SetRelation(cltGuild *pGuild, SI32 siRelation, BOOL bInit,SI32 siDbTime);		// 상단의 관계를 설정한다.
	SI32	GetRelation(UI32 uiArrayIndex);													// 상단의 관계를 얻어온다.
	BOOL	IsEnemyGuild(UI32 uiArrayIndex);												// 적대적인 관계인지 알아온다.
	BOOL	IsWarGuild(UI32 uiArrayIndex);													// 공성 중인지 알아온다.
	void	SetRelationReLoad();															// 하루에 한번씩 상단간의 관계를 재설정 해준다.

	SI32    IncreaseGuildSupply(SI32 siSupply);							// 상단의 보급품 증가.
	SI32	DecreaseGuildSupply(SI32 siSupply);							// 상단의 보급품 감소.
	SI32	GetGuildSupply();
	void	SetGuildSupply(SI32	siSupply);

	
	char*   GetFrieldlyMessage(UI32 uiArrayIndex);
	BOOL	SetFriendlyMessage(cltGuild *pGuild,CHAR* pszFriendlyMsg);	// 화친제의를 한다.
	BOOL	SetFriendlyMessageAccept(cltGuild*	pGuild);				// 화친제의 수락.
	BOOL	SetFriendlyMessageCancel(cltGuild*	pGuild);				// 화친제의 거절.
	BOOL	SetMessage(cltGuild* pGuild,CHAR* pszMsg);					

	BOOL	SetPowerInvestmentEnd();									// 폭투권한 삭제.
	BOOL	SendGuildItemDeleteMessage();								// 상단이 없어지면 상단의 공성 관련 데이터를 날린다.

//	VOID	RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode ,SI16 siRewardSupply);	//대방에게 돈과 투자권한을 준다.
	VOID	RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode );	//대방에게 돈과 투자권한을 준다.
	
	BOOL    IsWar() { return m_bIsWar; }                                //상단이 공성중인가....                                                                      
	VOID    SetWarFlag(bool bFlag) { m_bIsWar = bFlag; }

	BOOL    CheckWar();
	BOOL    CheckDay(SI16 siYear, SI16 siMon, SI16 siDay); //공성을 할수있는지 검사 한다.
	BOOL    CheckDay2(SI16 siYear, SI16 siMon, SI16 siDay);
	
	void    SendWarTime();
	BOOL    CheckVillage(UI16 uiVillageCode);
	
//공성 포기 관련	
	VOID    SetGiveUpInfo(UI16 uiVillageCode, MONEY mnMoney, SI16 siFlag);
	VOID    GetGiveUpInfo(UI16 *puiVillageCode, MONEY *pmnMoney);
	
	VOID    SetGiveUpFlag(SI16 siFlag);
	SI16    GetGiveUpFlag();

	int     GetgiveUpVillageCode() { return m_GiveUpInfo.uiVillageCode;	}

	VOID    GiveUp();
	void    SendGiveUpMsg();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

#endif
