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
	UI32                    m_uiMaxGuildID;					// 월드가 열린 이후로 지금까지 상단이 개설된 누적 회수

	static	SOWorld		*m_pMyWorld;
private:
	SI32		FindEmptySlot();																// 비어 있는 슬롯을 찾는다.
	SI32		FindSlot(UI32 uiGuildID);														// 슬롯을 찾는다.
	BOOL		FindSlot(UI32 uiGuildID1, UI32 uiGuildID2, SI32 *psiIndex1, SI32 *psiIndex2);	// 슬롯을 찾는다.
	BOOL		FindSlot(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 *psiIndex1, SI32 *psiIndex2);	// 슬롯을 찾는다.
	SI32		FindSlot(CHAR *pszName);														// 슬롯을 찾는다.

	BOOL		SetRelationByIndex(SI32 siGuildIndex1, SI32 siGuildIndex2, SI32 siRelation, BOOL bInit,SI32 siTime);	// 상단의 관계를 설정한다.

	BOOL		IsValidGuildIndex( SI32 siGuildIndex );											// 유효한 Guild Index 인지 검사한다.

public:
	cltIGuild();			// 생성자.
	~cltIGuild();			// 소멸자.
	
	VOID	Free();																			// 메모리를 해제한다.

	VOID	OnceInit(SI32 siTotalVillageNum);												// 한번만 초기화 해줄것들을 해준다.
	MONEY	GetGuildCreateCost();															// 상단 생성시 필요한 금액을 얻는다.
	UI32	CreateGuild(CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage);		// 상단을 생성한다.
	BOOL	InitGuild(UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, 
							SI32 uiGuildMemeberNum, SI32 siFlag);							// 상단을 설정한다.
	BOOL	DeleteGuild(UI32 uiGuildID,SOVillage* pVillage);						
	
	BOOL	IsGuildFull();																	// 현재 상단이 꽉 차있는지 본다.	

	BOOL	AddGuildMember(UI32 uiGuildID, CHAR *pszName);										// 상단에 상단 멤버가 추가되었다.
	
	BOOL	DelGuildMember(UI32 uiGuildID, CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick);		// 상단에서 상단 멤버가 삭제되었다.
	BOOL	AddCandidate(UI32 uiGuildID, SOPlayer *pPlayer);									// 지원자를 추가한다.
	VOID	AddVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bInit);						// 상단에 마을이 추가되었다.
	VOID	DelVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bDeleteGuild);					// 상단에서 마을이 삭제되었다.
	BOOL	ChangeClass(UI32 uiGuildID, CHAR *pszName, SI32 siNewClass);						// 직급을 변경시킨다.
	BOOL	Notice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice);								// 길드원에게 공지사항을 보내준다.
//	UI32	GetMaxGuildMemebersNum(UI32 uiGuildID);												// 이 상단이 보유할 수 있는 최대 상단원 수를 얻어온다.
	UI32	GetGuildMemberNum(UI32 uiGuildID);													// 이 상단의 현재 정식 회원 수를 얻어온다.

	static	VOID InitStaticVariable(SOWorld *pWorld);										// 정적 변수를 초기화 한다.	
	

	cltGuild	*GetGuildByIndex(SI32 siIndex);												// Guild를 얻어온다.
	cltGuild	*GetGuildByID(UI32 uiGuildID);												// Guild의 ID를 사용하여 Guild를 얻어온다.
	cltGuild	*GetGuildByName(CHAR *pszName);												// Guild의 이름을 사용하여 Guild를 얻어온다.
	CHAR		*GetGuildName(UI32 uiGuildID);												// Guild의 이름을 얻어온다.
	UI32		GetGuildID(CHAR *pszName);													// Guild의 ID를 얻어온다.
	UI32		GetGuildIDByIndex(SI32 siIndex);											// Guild의 Index로 ID를 얻어온다.
	SI32		GetGuildIndexByID( UI32 uiGuildID );										// Guild의 ID로 Index를 얻어온다.
	
	VOID		SetGuildRelationFromDB();													// DB에서 길드의 관계를 설정한다.	

	BOOL		GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass);	// Char의 이름을 사용하여 Guild ID를 얻어온다. 	
	
	BOOL	SetRelation(UI32 uiGuildID1, UI32 uiGuildID2, SI32 siRelation, BOOL bInit, SI32 siTime);		// 상단의 관계를 설정한다.
	BOOL	SetRelation(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 siRelatio, BOOL bInit, SI32 siTime);	// 상단의 관계를 설정한다.
	BOOL	SetRelation(CHAR *pszGuildID1, UI32 uiGuildID2, SI32 siRelatio, BOOL bInit, SI32 siTime);	// 상단의 관계를 설정한다.
	SI32	GetRelation(UI32 uiGuildID1, UI32 uiGuildID2);									// 상단의 관계를 얻어온다.
	void	SetRelationReLoad();
	BOOL	SetFriendlyMessage(UI32 uiGuild,CHAR* pszGuildid2,CHAR* pszMsg);
	BOOL	SetFriendlyMessageAccept(UI32 uiGuild,CHAR* pszGuildid2);
	BOOL	SetFriendlyMessageCancel(UI32 uiGuild,CHAR* pszGuildid2);
	BOOL	SetMessage(UI32 uiGuild1,UI32 uiGuild2,CHAR* pszMsg);

	BOOL	SetPowerInvestmentEnd();
	BOOL	GetGuildWarCheck(UI32 uiGuildID);

 

	BOOL	IsEnemyGuild(UI32 uiGuildID1, UI32 uiGuildID2);									// uiGuildID2가 uiGuildID1의 적인지 알아온다.
	BOOL	IsEnemyGuildBoth(UI32 uiGuildID1, UI32 uiGuildID2);								// uiGuildID2가 uiGuildID1의 적인지 알아온다. 서로가 적대관계인지 알아온다.
	BOOL	IsHostilityRelation(UI32 uiGuildID1, UI32 uiGuildID2);							// 적대관계인지알아온다.
	BOOL	IsEnemyRelationWar(UI32 uiGuildID1,UI32 uiGuildID2);									

	DWORD	RelationDefineToResponseDefine(DWORD dwDefine);									// 상관 관계에 관련된 게임 Define을 전송용 Define으로 변경한다.
	DWORD	ResponseDefineToRelationDefine(DWORD dwDefine);									// 전송용 Define을 상관 관계에 관련된 게임 Define으로 변경한다.	

	BOOL	SetHNS(SOVillage *pVillage);													// 행수를 지정한다.

	BOOL	SetFlag(UI32 uiGuildID, SI32 siFlag);											// 깃발을 설정한다.
	SI32	GetFlag(UI32 uiGuildID);														// 깃발을 얻어온다.
	BOOL	IsUsedFlag(SI32 siFlag);														// 사용하는 깃발인지 알아온다.
	BOOL    IsUsedName(CHAR *szName);                                                       // 사용하는 이름인지 알아온다.

	VOID	GetUsedFlagList(SI32 *psiFlagNum, strFlagInfo *pstFlagInfo);					// 사용된 깃발의 리스트를 얻어온다.

	BOOL	SetMemberInfoFromDB();															// DB로 부터 모든 상단원의 정보를 얻어온다.

	VOID	SendMsgToGuildMemebers( UI32 uiGuildID, SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL );	// 접속해 있는 상단원에게 메시지를 보내준다.

//	BOOL	RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode, SI16	siRewardSupply);
	BOOL	RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode);
	BOOL    SetMaxGuildID();

	VOID	GuildMemberLogin( UI32 uiGuildID );
	VOID	GuildMemberLogout( UI32 uiGuildID );
	

	void   SendWarTime(); //공성 시간이 된것을 알려준다.
	SI32   CheckVillage(UI16 uiVillageCode);
	
	BOOL   GiveUp(UI32 uiGuildID); //전투포기
	VOID   InitGiveUpInfo();
	VOID   DBGetGiveUpInfo();
};

#endif