#ifndef	_IGUILD_HEADER
#define _IGUILD_HEADER

#include "SOVillage.h"
#include "SOCommonHeader.h"
#include "Guild.h"
#include "OnlineCommonStruct.h"

class	cltIGuild
{
private:	
	cltGuild				m_Guilds[SO_TOTAL_GUILD_NUM];
	SStack<strFlagInfo>		m_Flags;
	UI32                    m_uiMaxGuildID;					// ���尡 ���� ���ķ� ���ݱ��� ����� ������ ���� ȸ��

	static	SOWorld		*m_pMyWorld;
private:
	SI32		FindEmptySlot();																// ��� �ִ� ������ ã�´�.
	SI32		FindSlot(UI32 uiGuildID);														// ������ ã�´�.
	BOOL		FindSlot(UI32 uiGuildID1, UI32 uiGuildID2, SI32 *psiIndex1, SI32 *psiIndex2);	// ������ ã�´�.
	BOOL		FindSlot(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 *psiIndex1, SI32 *psiIndex2);	// ������ ã�´�.
	SI32		FindSlot(CHAR *pszName);														// ������ ã�´�.

	BOOL		SetRelationByIndex(SI32 siGuildIndex1, SI32 siGuildIndex2, SI32 siRelation, BOOL bInit,SI32 siTime);	// ����� ���踦 �����Ѵ�.

	BOOL		IsValidGuildIndex( SI32 siGuildIndex );											// ��ȿ�� Guild Index ���� �˻��Ѵ�.

public:
	cltIGuild();			// ������.
	~cltIGuild();			// �Ҹ���.
	
	VOID	Free();																			// �޸𸮸� �����Ѵ�.

	VOID	OnceInit(SI32 siTotalVillageNum);												// �ѹ��� �ʱ�ȭ ���ٰ͵��� ���ش�.
	MONEY	GetGuildCreateCost();															// ��� ������ �ʿ��� �ݾ��� ��´�.
	UI32	CreateGuild(CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage);		// ����� �����Ѵ�.
	BOOL	InitGuild(UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, 
							SI32 uiGuildMemeberNum, SI32 siFlag);							// ����� �����Ѵ�.
	BOOL	DeleteGuild(UI32 uiGuildID,SOVillage* pVillage);						
	
	BOOL	IsGuildFull();																	// ���� ����� �� ���ִ��� ����.	

	BOOL	AddGuildMember(UI32 uiGuildID, CHAR *pszName);										// ��ܿ� ��� ����� �߰��Ǿ���.
	
	BOOL	DelGuildMember(UI32 uiGuildID, CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick);		// ��ܿ��� ��� ����� �����Ǿ���.
	BOOL	AddCandidate(UI32 uiGuildID, SOPlayer *pPlayer);									// �����ڸ� �߰��Ѵ�.
	VOID	AddVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bInit);						// ��ܿ� ������ �߰��Ǿ���.
	VOID	DelVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bDeleteGuild);					// ��ܿ��� ������ �����Ǿ���.
	BOOL	ChangeClass(UI32 uiGuildID, CHAR *pszName, SI32 siNewClass);						// ������ �����Ų��.
	BOOL	Notice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice);								// �������� ���������� �����ش�.
//	UI32	GetMaxGuildMemebersNum(UI32 uiGuildID);												// �� ����� ������ �� �ִ� �ִ� ��ܿ� ���� ���´�.
	UI32	GetGuildMemberNum(UI32 uiGuildID);													// �� ����� ���� ���� ȸ�� ���� ���´�.

	static	VOID InitStaticVariable(SOWorld *pWorld);										// ���� ������ �ʱ�ȭ �Ѵ�.	
	

	cltGuild	*GetGuildByIndex(SI32 siIndex);												// Guild�� ���´�.
	cltGuild	*GetGuildByID(UI32 uiGuildID);												// Guild�� ID�� ����Ͽ� Guild�� ���´�.
	cltGuild	*GetGuildByName(CHAR *pszName);												// Guild�� �̸��� ����Ͽ� Guild�� ���´�.
	CHAR		*GetGuildName(UI32 uiGuildID);												// Guild�� �̸��� ���´�.
	UI32		GetGuildID(CHAR *pszName);													// Guild�� ID�� ���´�.
	UI32		GetGuildIDByIndex(SI32 siIndex);											// Guild�� Index�� ID�� ���´�.
	SI32		GetGuildIndexByID( UI32 uiGuildID );										// Guild�� ID�� Index�� ���´�.
	
	VOID		SetGuildRelationFromDB();													// DB���� ����� ���踦 �����Ѵ�.	

	BOOL		GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass);	// Char�� �̸��� ����Ͽ� Guild ID�� ���´�. 	
	
	BOOL	SetRelation(UI32 uiGuildID1, UI32 uiGuildID2, SI32 siRelation, BOOL bInit, SI32 siTime);		// ����� ���踦 �����Ѵ�.
	BOOL	SetRelation(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 siRelatio, BOOL bInit, SI32 siTime);	// ����� ���踦 �����Ѵ�.
	BOOL	SetRelation(CHAR *pszGuildID1, UI32 uiGuildID2, SI32 siRelatio, BOOL bInit, SI32 siTime);	// ����� ���踦 �����Ѵ�.
	SI32	GetRelation(UI32 uiGuildID1, UI32 uiGuildID2);									// ����� ���踦 ���´�.
	void	SetRelationReLoad();
	BOOL	SetFriendlyMessage(UI32 uiGuild,CHAR* pszGuildid2,CHAR* pszMsg);
	BOOL	SetFriendlyMessageAccept(UI32 uiGuild,CHAR* pszGuildid2);
	BOOL	SetFriendlyMessageCancel(UI32 uiGuild,CHAR* pszGuildid2);
	BOOL	SetMessage(UI32 uiGuild1,UI32 uiGuild2,CHAR* pszMsg);

	BOOL	SetPowerInvestmentEnd();
	BOOL	GetGuildWarCheck(UI32 uiGuildID);

 

	BOOL	IsEnemyGuild(UI32 uiGuildID1, UI32 uiGuildID2);									// uiGuildID2�� uiGuildID1�� ������ �˾ƿ´�.
	BOOL	IsEnemyGuildBoth(UI32 uiGuildID1, UI32 uiGuildID2);								// uiGuildID2�� uiGuildID1�� ������ �˾ƿ´�. ���ΰ� ����������� �˾ƿ´�.
	BOOL	IsHostilityRelation(UI32 uiGuildID1, UI32 uiGuildID2);							// ������������˾ƿ´�.
	BOOL	IsEnemyRelationWar(UI32 uiGuildID1,UI32 uiGuildID2);									

	DWORD	RelationDefineToResponseDefine(DWORD dwDefine);									// ��� ���迡 ���õ� ���� Define�� ���ۿ� Define���� �����Ѵ�.
	DWORD	ResponseDefineToRelationDefine(DWORD dwDefine);									// ���ۿ� Define�� ��� ���迡 ���õ� ���� Define���� �����Ѵ�.	

	BOOL	SetHNS(SOVillage *pVillage);													// ����� �����Ѵ�.

	BOOL	SetFlag(UI32 uiGuildID, SI32 siFlag);											// ����� �����Ѵ�.
	SI32	GetFlag(UI32 uiGuildID);														// ����� ���´�.
	BOOL	IsUsedFlag(SI32 siFlag);														// ����ϴ� ������� �˾ƿ´�.
	BOOL    IsUsedName(CHAR *szName);                                                       // ����ϴ� �̸����� �˾ƿ´�.

	VOID	GetUsedFlagList(SI32 *psiFlagNum, strFlagInfo *pstFlagInfo);					// ���� ����� ����Ʈ�� ���´�.

	BOOL	SetMemberInfoFromDB();															// DB�� ���� ��� ��ܿ��� ������ ���´�.

	VOID	SendMsgToGuildMemebers( UI32 uiGuildID, SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL );	// ������ �ִ� ��ܿ����� �޽����� �����ش�.

//	BOOL	RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode, SI16	siRewardSupply);
	BOOL	RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode);
	BOOL    SetMaxGuildID();

	VOID	GuildMemberLogin( UI32 uiGuildID );
	VOID	GuildMemberLogout( UI32 uiGuildID );
	

	void   SendWarTime(); //���� �ð��� �Ȱ��� �˷��ش�.
	SI32   CheckVillage(UI16 uiVillageCode);
	
	BOOL   GiveUp(UI32 uiGuildID); //��������
	VOID   InitGiveUpInfo();
	VOID   DBGetGiveUpInfo();
};

#endif