// ���� ���� : 2002-02-18 ���� 11:15:33 - ������

#ifndef	_GUILD_HEADER
#define _GUILD_HEADER

#include "SOCommonHeader.h"
#include "GSCDefine.h"
#include "SOVillage.h"
#include "Stack.h"
#include "GuildMemberManager.h"


#define	ON_GUILD_RELATION_ALLY			1			// ����
#define	ON_GUILD_RELATION_ENEMY			2			// ��
#define	ON_GUILD_RELATION_NEUTRAL		3			// �߸�			���x
#define	ON_GUILD_RELATION_MINE			4			// ���� ���.	���x
#define ON_GUILD_RELATION_WAR			5			// ����

#define ON_GUILD_UNIT_SWORD				0			// �˺�
#define ON_GUILD_UNIT_SPEAR				1			// â��
#define ON_GUILD_UNIT_HORSE				2			// �⸶��
#define ON_GUILD_UNIT_ARROW				3			// �ú�
#define ON_GUILD_UNIT_DEFENSE			4			// ����
#define ON_GUILD_UNIT_RAMPART			5			// ����.
#define ON_GUILD_UNIT_SIEGE				6			// �߼���.
#define ON_GUILD_UNIT_SUPPLY			7			// ���޼���.
#define ON_MAX_GUILD_UNIT				8

#define ON_MAX_GUILD_RESULT				20



enum {
	    ON_GUILD_GIVEUP_NORMAL = 0,
        ON_GUILD_GIVEUP_NOT_ANSWER,
//		ON_GUILD_GIVEUP_NOT_SEND,
		ON_GUILD_GIVEUP_OK,
		ON_GUILD_GIVEUP_NO,
};

//����� ���������� �����ϴ� Ŭ����.
class   cltGuildUnit				
{
private:
	SI16 siGuildUnit[ON_MAX_GUILD_UNIT];								// ���� ������ �ִ� ���� ����.
public:
	void	InitGuildUnit();
	void	SetGuildUnit(SI16 siUnitKind,SI16 siCount);
	SI16	IncreaseGuildUnit(SI16 siUnitKind,SI16 siCount);	// ���� ������ �ִ� ���� ������ ���� ��Ų��.
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
	SI32            uiVillageCode;  //�Զ��� ����
	SI16            siGiveUpFlag;   //
	MONEY           mnMoney;        //�������� ���

}GIVEUP_INFO, *LP_GIVEUP_INFO;

class	cltGuild
{
private:	
	UI32				m_uiArrayIndex;								// �迭 �ε���.
	UI32				m_uiGuildID;								// ����� ID
	CHAR				m_szGuildName[ON_GUILDNAME_LENGTH + 1];		// ����� �̸�.
	CHAR				m_szDAIID[ON_ID_LENGTH + 1];				// ����� �̸�.

	SI32				m_siFlag;									// ���.
	SI32				m_siFlagIDInStack;							// ���ÿ����� ��� ID.
	SStack<UI16>		m_Villages;									// ������.	

	GIVEUP_INFO         m_GiveUpInfo;
	BOOL                m_bIsWar;                                   // �������� �Ҽ��ֳ�?
	
public:
	
	GuildMemberManager	m_GuildMemberManager;						// �� ��ܿ� ��� ��ܿ����� �����Ѵ�.
	SI32				m_siLoginedGuildMemeberNum;					// ���� �α����� ��ܿ����� ��.

	//��������������������������������������������������������������������������������������������������������������������
	//���� �¸����� ����.
	//��������������������������������������������������������������������������������������������������������������������
	cltGuildResult		m_stGuildResult[ON_MAX_GUILD_RESULT];		// ����� ������ ������ VillageCode�� 
																	// ���� Percent�� ����.
	//��������������������������������������������������������������������������������������������������������������������
	//��� ��ǰ ����.
	//��������������������������������������������������������������������������������������������������������������������
	cltGuildUnit		m_GuildUnit;								// ����� ������ �ִ� ���� ����.
	SI32				m_siGuildSupply;							// ����� ������ �ִ� ����ǰ�� ����.
	//��������������������������������������������������������������������������������������������������������������������
	//���� ����
	//��������������������������������������������������������������������������������������������������������������������
	SI32				m_siGuildRelation[SO_TOTAL_GUILD_NUM];				// �� ��ܰ��� ����.
	SI32				m_siGuildRelationEnemyTime[SO_TOTAL_GUILD_NUM];		// �� ����� ��������� �� ���븦 ��û�� �ð�.
																			// ���� �ɰ� 3���� ������ ���۵ȴ�.
	char				m_szGuildfriendlyMessage[SO_TOTAL_GUILD_NUM][200];	// ȭģ�� ���� ��� �޼����� ������.
	//��������������������������������������������������������������������������������������������������������������������
private:

	static	SOWorld	*m_pMyWorld;
private:
	UI32	FindEmptySlot();						// ����� �߰��� ����ִ� ������ ã�´�.	
	UI32	FindSlot(UI16 uiAccount);				// ����� �����Ǿ� �ִ� ������ ã�´�.		

	//VOID	UpdateMaxRealMemberNum();				// �����ϰ� �ִ� ������ ���� �����Ѵ�.
public:
	cltGuild();										// ������.
	~cltGuild();									// �Ҹ���.

	VOID	OnceInit(SI32 siTotalVillageNum);		// �ѹ��� �ʱ�ȭ ���ٰ͵��� ���ش�.
	VOID	Init();									// ��� �������� �ʱ�ȭ�Ѵ�.	
	VOID	CreateGuild(UI32 uiArrayIndex, 
						UI32 uiGuildID, 
						CHAR *pszGuildName,
						SOPlayer *pPlayer,
						SOVillage *pVillage);			// ����� �����Ѵ�.
	VOID	InitGuild(	UI32 uiArrayIndex, 
						UI32 uiGuildID, 
						CHAR *pszGuildName,
						CHAR *pszDAIID,						
						UI32 uiGuildMemeberNum, SI32 siFlag, 
						SI32 siFlagIDInStack);		// DB���� �о�� ����� �о�´�.
	BOOL	DeleteGuild();							// ����� �����Ѵ�.	

	UI32	GetGuildID();							// ����� ID�� ���´�.
	CHAR*	GetGuildName();							// ����� �̸��� ���´�.	
	CHAR*	GetDAIID();								// ����� ID(���ڿ�)�� ���´�.
	UI16	GetOwnerVillageNum();					// �����ϰ� �ִ� ������ ���� �Ѱ��ش�.
	UI32	GetGuildMemberNum();					// ��� ȸ������ �Ѱ��ش�.
	BOOL	GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass);		// ĳ���� �̸��� ����Ͽ� ����� ID�� ��� ������ ���´�.


	BOOL	AddGuildMember(CHAR *pszName);										// ��ܿ� ��� ����� �߰��Ǿ���.
	BOOL	DelGuildMember(CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick);		// ��ܿ��� ��� ����� �����Ǿ���. bKick�� �µǾ������� �ѾƳ�����
	BOOL	AddCandidate(SOPlayer *pPlayer);									// �����ڸ� �߰��Ѵ�.	
	VOID	AddVillage(SOVillage *pVillage, BOOL bInit);						// ��ܿ� ������ �߰��Ǿ���.
	BOOL	DelVillage(SOVillage *pVillage, BOOL bDeleteGuild);					// ��ܿ��� ������ �����Ǿ���.

	BOOL	IsValidGuild();													// ��ȿ�� ������� �˾ƿ´�.
	BOOL	Notice(CHAR *pszName, CHAR *pszNotice);							// ���������� �����ش�.	

	UI32	GetArrayIndex();						// �迭 �ε����� ����.

	BOOL	ChangeFlag(SI32 siFlag);				// ����� �����Ѵ�.
	SI32	GetFlag();								// ����� ���´�.

	BOOL	SetHNS(SOVillage *pVillage);									// ����� �����Ѵ�.

	static	VOID InitStaticVariable(SOWorld *pWorld);	// ���� ������ �ʱ�ȭ �Ѵ�.

	VOID	SetFlagIDInStack(SI32 siFlagIDInStack);		// ���ÿ����� ��� ID�� �����Ѵ�.
	SI32	GetFlagIDInStack();							// ���ÿ����� ��� ID�� ���´�.

	BOOL	ChangeClass( CHAR *pszName, SI32 siNewClass );	// ������ �����Ų��.
	VOID	GetOwnerVillages(UI16 *puiOwnerVillageNum, UI16 *puiVillageCode);								// ���� �� ������ �Ҽӵ� ������ �ڵ带 ���´�.
	VOID	GetOwnerGuildVillageInfo( UI16 *puiOwnerVillageNum, OnGuildVillageInfo *pGuildVillageInfo );	// ���� �� ������ �Ҽӵ� ������ �⺻ ������ ���´�.

	DWORD	GetRMTotalPageNum();															// ��ü ���� ��ܿ��� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
	DWORD	GetCMTotalPageNum();															// ��ü �����ڸ� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
	UI32	GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo);					// ���� ��ܿ��� ����Ʈ�� ���´�.
	UI32	GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo);						// ������ ����Ʈ�� ���´�.

	BOOL	SetMemberInfoFromDB();															// DB�� ���� ��� ��ܿ��� ������ ���´�.

	VOID	SendMsgToGuildMemebers(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL);	// ������ �ִ� ��ܿ����� �޽����� �����ش�.		

	VOID	GuildMemberLogin();
	VOID	GuildMemberLogout();

	void	GuildInfoChanged();
	BOOL	GetAuditGuildInfo(SOPlayer* pPlayer);

	//��������������������������������������������������������������������������������������������������������������������
	// ���� ���� �߰�,���� �Լ���
	//��������������������������������������������������������������������������������������������������������������������
	VOID	InitRelation(UI32 uiArrayIndex);												// ����� ���踦 �ʱ�ȭ �Ѵ�.
	VOID	SetRelation(cltGuild *pGuild, SI32 siRelation, BOOL bInit,SI32 siDbTime);		// ����� ���踦 �����Ѵ�.
	SI32	GetRelation(UI32 uiArrayIndex);													// ����� ���踦 ���´�.
	BOOL	IsEnemyGuild(UI32 uiArrayIndex);												// �������� �������� �˾ƿ´�.
	BOOL	IsWarGuild(UI32 uiArrayIndex);													// ���� ������ �˾ƿ´�.
	void	SetRelationReLoad();															// �Ϸ翡 �ѹ��� ��ܰ��� ���踦 �缳�� ���ش�.

	SI32    IncreaseGuildSupply(SI32 siSupply);							// ����� ����ǰ ����.
	SI32	DecreaseGuildSupply(SI32 siSupply);							// ����� ����ǰ ����.
	SI32	GetGuildSupply();
	void	SetGuildSupply(SI32	siSupply);

	
	char*   GetFrieldlyMessage(UI32 uiArrayIndex);
	BOOL	SetFriendlyMessage(cltGuild *pGuild,CHAR* pszFriendlyMsg);	// ȭģ���Ǹ� �Ѵ�.
	BOOL	SetFriendlyMessageAccept(cltGuild*	pGuild);				// ȭģ���� ����.
	BOOL	SetFriendlyMessageCancel(cltGuild*	pGuild);				// ȭģ���� ����.
	BOOL	SetMessage(cltGuild* pGuild,CHAR* pszMsg);					

	BOOL	SetPowerInvestmentEnd();									// �������� ����.
	BOOL	SendGuildItemDeleteMessage();								// ����� �������� ����� ���� ���� �����͸� ������.

//	VOID	RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode ,SI16 siRewardSupply);	//��濡�� ���� ���ڱ����� �ش�.
	VOID	RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode );	//��濡�� ���� ���ڱ����� �ش�.
	
	BOOL    IsWar() { return m_bIsWar; }                                //����� �������ΰ�....                                                                      
	VOID    SetWarFlag(bool bFlag) { m_bIsWar = bFlag; }

	BOOL    CheckWar();
	BOOL    CheckDay(SI16 siYear, SI16 siMon, SI16 siDay); //������ �Ҽ��ִ��� �˻� �Ѵ�.
	BOOL    CheckDay2(SI16 siYear, SI16 siMon, SI16 siDay);
	
	void    SendWarTime();
	BOOL    CheckVillage(UI16 uiVillageCode);
	
//���� ���� ����	
	VOID    SetGiveUpInfo(UI16 uiVillageCode, MONEY mnMoney, SI16 siFlag);
	VOID    GetGiveUpInfo(UI16 *puiVillageCode, MONEY *pmnMoney);
	
	VOID    SetGiveUpFlag(SI16 siFlag);
	SI16    GetGiveUpFlag();

	int     GetgiveUpVillageCode() { return m_GiveUpInfo.uiVillageCode;	}

	VOID    GiveUp();
	void    SendGiveUpMsg();

	//��������������������������������������������������������������������������������������������������������������������
};

#endif
