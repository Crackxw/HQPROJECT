// 수정 일자 : 2002-02-18 오전 11:28:08 - 양정모

#include "SOMain.h"
#include "SOWorld.h"
#include "Guild.h"

SOWorld*	cltGuild::m_pMyWorld	=	NULL;
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-04-13 오후 6:08:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuild::cltGuild()
{
	Init();			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-04-13 오후 6:08:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuild::~cltGuild()
{
	m_Villages.Release();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	한번만 초기화 해줄것들을 해준다.
//	수정 일자	:	2002-04-13 오후 6:09:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::OnceInit(SI32 siTotalVillageNum)
{
	m_Villages.Create( siTotalVillageNum );	

	m_GuildMemberManager.OnceInit();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	멤버 변수들을 초기화한다.
//	수정 일자	:	2002-03-29 오후 1:51:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::Init()
{
	m_uiGuildID			=	0;
	ZeroMemory(m_szGuildName, sizeof(m_szGuildName));	
	ZeroMemory(m_szDAIID, sizeof(m_szDAIID));
	ZeroMemory(m_siGuildRelation, sizeof(m_siGuildRelation));			
	m_siFlag				=	0;	
	m_siFlagIDInStack		=	-1;
	m_siGuildSupply			=	0;
	m_bIsWar                = FALSE; 

	m_Villages.DeleteAll();	

	ZeroMemory(m_szGuildfriendlyMessage,sizeof(char)*SO_TOTAL_GUILD_NUM*200);

	m_GuildUnit.InitGuildUnit();

	// 상단원 관리자를 초기화 한다.
	m_GuildMemberManager.Init();

	m_siLoginedGuildMemeberNum	=	0;

	SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 생성한다.
//	수정 일자	:	2002-05-09 오후 7:27:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::CreateGuild(UI32 uiArrayIndex, UI32 uiGuildID, CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage)
{
	// 상단원 관리자를 초기화 한다.
	m_GuildMemberManager.Init();

	m_uiGuildID		=	uiGuildID;

	strncpy(m_szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);	
	strncpy(m_szDAIID, pPlayer->szID, ON_ID_LENGTH);
	ZeroMemory(m_siGuildRelation, sizeof(m_siGuildRelation));		

	m_uiArrayIndex						=	uiArrayIndex;		// 배열 인덱스를 설정한다.
	m_siFlagIDInStack					=	-1;

	//공성 유닛 리스트를 초기화 해준다.
	m_GuildUnit.InitGuildUnit();

	// 리스트에 사용자를 추가한다.
	m_GuildMemberManager.AddMember(pPlayer->szID, pPlayer->siClassInGuild, pPlayer->GetMoney() + pPlayer->GetSaving(), 
								pPlayer->IFollower.GetFollowerParameterLevel(0), pPlayer->uiTradeCredit,pPlayer->stAccountInfo.siDBAccount,pPlayer->stAccountInfo.uiSlot);		
	
	// 마을을 추가 시킨다.
	// 초기화 부분은 "FALSE"를 줌으로써
	// 그 마을에 최고투자자를 행수로 설정하게 해준다.
	AddVillage(pVillage, FALSE);

	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 설정한다.
//	수정 일자	:	2002-05-09 오후 7:29:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::InitGuild(UI32 uiArrayIndex, UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, UI32 uiGuildMemeberNum, SI32 siFlag, SI32 siFlagIDInStack)
{
	m_uiGuildID		=	uiGuildID;

	strncpy(m_szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);	
	strncpy(m_szDAIID, pszDAIID, ON_ID_LENGTH);	
	m_uiArrayIndex				=	uiArrayIndex;		// 배열 인덱스를 설정한다.		
	m_siFlag					=	siFlag;				// 깃발.
	m_siFlagIDInStack			=	siFlagIDInStack;		

	// 이 상단에 들어올 수 있는 상단원의 수를 설정한다.
	//UpdateMaxRealMemberNum();	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB로 부터 모든 상단원의 정보를 얻어온다.
//	수정 일자	:	2002-04-22 오후 5:03:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::SetMemberInfoFromDB()
{
	m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_RESULT, LPVOID(m_uiGuildID), LPVOID(this));
	// 상단원들을 DB에서 읽어와가지고 m_GuildMemberManager에 세팅한다.
	return m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETGUILDMEMBER, LPVOID(m_uiGuildID), LPVOID(&m_GuildMemberManager));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 삭제한다.
//	수정 일자	:	2002-03-05 오전 11:47:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::DeleteGuild()
{
	SI32		i;	
	BOOL		bDBSuccess;
	
	// DB에서 Guild를 삭제한다.		
	bDBSuccess = TRUE;
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_DELETE, &m_uiGuildID, sizeof(m_uiGuildID) ); //**

	m_GuildMemberManager.Init();

	// 현재 이 상단에 속해 있는 마을을 모두 상단에 속하지 않은 상태로 해준다.
	for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
	{
		if(m_pMyWorld->pclVillage[i].uiGuildID == m_uiGuildID)
			DelVillage(&m_pMyWorld->pclVillage[i], TRUE);
	}

	// for debug
//	m_pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Clan was disorganized!!    Guild Name: %s    GuildID: %d", m_szGuildName, m_uiGuildID );
	writeInfoToFile( "DeletedGuild.txt", "Clan was disorganized!!    Guild Name: %s    GuildID: %d", m_szGuildName, m_uiGuildID );
	// 현재 이 상단에 속해 있는 플레이어들을 모두 상단에 속하지 않은 상태로 해준다.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			if(m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID)
			{
				m_pMyWorld->clPlayer[i]->SetGuild(0);
				m_pMyWorld->clPlayer[i]->SetClassInGuild(0);

				// 메시지를 보내준다.
				m_pMyWorld->clPlayer[i]->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_DELETEGLD));
			}
		}
	}

	// 변수들을 초기화 한다.
	Init();

	return TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단의 ID를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltGuild::GetGuildID()
{
	return m_uiGuildID;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	상단의 이름을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	cltGuild::GetGuildName()
{
	return m_szGuildName;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	대방의 ID(문자열)을 얻어온다.
//	수정 일자	:	2002-03-04 오후 12:33:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	cltGuild::GetDAIID()
{
	return m_szDAIID;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소유하고 있는 마을의 수를 넘겨준다.
//	수정 일자	:	2002-03-04 오후 12:34:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	cltGuild::GetOwnerVillageNum()
{
	return m_Villages.m_NowUseDataCount;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	멤버 회원수를 넘겨준다.
//	수정 일자	:	2002-04-19 오후 3:11:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltGuild::GetGuildMemberNum()
{
	return m_GuildMemberManager.GetRealMemeberNum();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 정식 상단원이 추가되었다.
//	수정 일자	:	2002-05-10 오후 9:44:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::AddGuildMember(CHAR *pszName)
{	
	
	SI32	i;
	CHAR	*pszMaxInvestmentorName;
	BOOL	IsSendChangeClassMsg = FALSE;	
	SI32	siClassInCode;

	strCandidateInfo *pInfo = NULL;

	// 상단원에 추가시키려는 플레이어가 지원자인지 검사한다.
	if( (pInfo = m_GuildMemberManager.GetCandidateMember( pszName )) == NULL )
	{
//		m_pMyWorld->clServer->WriteInfo( "..\\AddGuildMember Failed.txt", "GuildID : [ %u ], Name : [ %s ]", m_uiGuildID, pszName );
		writeInfoToFile( "AddGuildMember Failed.txt", "GuildID : [ %u ], Name : [ %s ]", m_uiGuildID, pszName );
		return	FALSE;
	}


	// 직급은 기본적으로 사환이다.		
	siClassInCode	=	m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);

	// 더 이상 가입할 수 없다면 FALSE를 리턴한다.
	if(m_GuildMemberManager.IsRealMemberFull() == TRUE)	return FALSE;	

	// 현재 가입하려는 캐릭터가 다른 마을의 최고 투자자이라면 그 마을을 포함시킨다.
	for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
	{
		pszMaxInvestmentorName	=	m_pMyWorld->pclVillage[i].m_Government->GetInvestmentMaxInvestmentorName();
			
		if(pszMaxInvestmentorName != NULL)
		{
			if(stricmp(pszMaxInvestmentorName, pszName) == 0)
			{
				// 최고 투자자이다.
				// 해당 마을을 추가 시킨다.
				AddVillage(&m_pMyWorld->pclVillage[i], FALSE);								
				//IsSendChangeClassMsg	=	TRUE;
		
				siClassInCode			=	m_pMyWorld->clIGuildClass.GetHNSCode();
			}
		}
	}
	
	// 직급 변경이 되었다는걸 보내줬다면 다시 보내 주지 않아도 된다.
	if(IsSendChangeClassMsg == FALSE)
	{
		if( ChangeClass( pInfo->szName, siClassInCode ) == FALSE )	
			return	FALSE;			
	}

	// 지원자를 정식 상단원으로 바꿔준다.
	m_GuildMemberManager.ChangeClass(pszName, siClassInCode, TRUE);	

	// 현재 해당 사용자가 로그인중이라면 접속한 상단원수를 증가한다.
	if( m_pMyWorld->GetPlayer( pszName ) != NULL )
	{
		GuildMemberLogin();		
	}

	// 모든 상단원에게 상단 정보가 변경되었다는 것을 알려준다.(MS)
	GuildInfoChanged();

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에서 삭제되었다.
//	수정 일자	:	2002-04-13 오전 10:03:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::DelGuildMember(CHAR *pszName, BOOL bCandidate, BOOL bSendMsg, BOOL bKick)
{
	DBIKGuildLeaveInfo			GuildLeaveInfo;	
	SOPlayer					*pPlayer;			

	strGuildCommonMemberInfo    MemberInfo;
	
	// 해당 플레이어가 이 상단원인지 알아옴과 동시에 정보를 얻어온다.
	if (m_GuildMemberManager.GetGuildMemberByCharName(pszName,&MemberInfo) == FALSE)
	{
		return FALSE;
	}

	ZeroMemory(&GuildLeaveInfo, sizeof(GuildLeaveInfo));
	GuildLeaveInfo.uiGuildID	= m_uiGuildID;					
	GuildLeaveInfo.uiClass      = MemberInfo.siClassInGuild;
	GuildLeaveInfo.siAccount    = MemberInfo.siAccount;
	GuildLeaveInfo.uiSlot       = MemberInfo.uiSlot;

	if (bKick)
	{
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_KICK, &GuildLeaveInfo, sizeof(GuildLeaveInfo));
	}
	else 
	{
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_LEAVE, &GuildLeaveInfo, sizeof(GuildLeaveInfo));
	}

	// 상단원 관리 클래스에게도 알려준다.
	m_GuildMemberManager.DelMember(pszName, bCandidate);	


	// 마을에서 그 캐릭터의 직위가 있다면 해제한다.
	// 지원자가 아닐 경우에만 수행해준다.
	if(bCandidate == FALSE) m_pMyWorld->DeleteGuildClassInVillage(pszName);	

	

	// 현재 플레이어가 접속되어 있다면 플레이어의 상단을 없도록 한다.
	if((pPlayer = m_pMyWorld->GetPlayer(pszName)) != NULL)
	{
		// 현재 설정된 상단이 없도록 한다.
		pPlayer->SetGuild(0);	

		if(bSendMsg == TRUE)
			pPlayer->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_DISMISSAL));		

		// 접속된 상단원수에서 감소한다.
		GuildMemberLogout();
	}

	// 모든 상단원에게 상단 정보가 변경되었다는 것을 알려준다.(MS)
	GuildInfoChanged();

	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 마을이 추가되었다.
//	수정 일자	:	2002-05-11 오전 10:17:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::AddVillage(SOVillage *pVillage, BOOL bInit)
{	
	// 초기화중이 아니여야 한다.
	if(bInit == FALSE)
	{
		// 초기화중이 아닐때에는 현재 추가하려는 마을이 이미 상단에 가입해 있는게 아닌지 확인한다.

		// 상단 ID가 서로 같다면 이미 가입한 것이다.
		// 이 부분은 반드시 아래 pVillage->SetOwnerGuild(m_uiGuildID) 하기 전에 해주어야 한다.
		if(pVillage->uiGuildID == m_uiGuildID)	return;

		// 마을을 추가할때 행수도 같이 지정해준다.	
		pVillage->SetOwnerGuild(m_uiGuildID);
		// 파괴된 마을을 50%의 피를 채워 추가한다.		
		pVillage->AutoRestorationVillageDefence(TRUE);
	}

	// 마을을 소속시킨다.
	m_Villages.Add(pVillage->uiVillageCode);

	// 최대 상단원의 수를 설정한다.
	//UpdateMaxRealMemberNum();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에서 마을이 삭제되었다.
//	수정 일자	:	2002-05-09 오후 8:00:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::DelVillage(SOVillage *pVillage, BOOL bDeleteGuild)
{
	pVillage->DeleteGuild();	

	// 소속된 마을에서 제거한다.
	m_Villages.DeleteType(pVillage->uiVillageCode);

	// 최대 상단원의 수를 설정한다.
	//UpdateMaxRealMemberNum();

	// 현재 길드를 삭제중이면 
	// 상단을 없애기 위하여 마을 수를 체크하지 않는다.
	if(bDeleteGuild == FALSE)
	{
		// 더 이상 소유하고 있는 마을이 없다면
		// 길드를 삭제한다.
		if(m_Villages.m_NowUseDataCount <= 0)
		{
			// 상단을 없앤다.		
			return	TRUE;	
		}
	}	

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	정적 변수를 초기화 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::InitStaticVariable(SOWorld *pWorld)
{
	m_pMyWorld	=	pWorld;
	
	GuildMemberManager::InitStaticVariable( &(m_pMyWorld->clIGuildClass) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 길드인지 알아온다.
//	수정 일자	:	2002-02-25 오전 9:17:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::IsValidGuild()
{
	if(m_uiGuildID != 0)		return TRUE;
	else						return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공지사항을 보내준다.
//	수정 일자	:	2002-03-26 오후 3:30:39 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::Notice(CHAR *pszName, CHAR *pszNotice)
{	
	CHAR	szNotice[1024];

	if(pszNotice == NULL)	return FALSE;

	// 상단이름.
	sprintf(szNotice, "[%s] : %s", pszName, pszNotice);

	// 현재 이 상단에 속해 있는 플레이어들에게 메시지를 보내준다.
	SendMsgToGuildMemebers(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_GUILD), LPVOID(szNotice));	

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	접속해 있는 상단원에게 메시지를 보내준다.
//	수정 일자	:	2002-03-16 오전 10:32:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::SendMsgToGuildMemebers(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5)
{
	SI32		i;		

	// 현재 이 상단에 속해 있는 플레이어들에게 메시지를 보내준다.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			// 해당 길드원이고 
			// 지원자가 아니여야 한다.
			if((m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID) && 
				(m_pMyWorld->clPlayer[i]->siClassInGuild != m_pMyWorld->clIGuildClass.GetJWJCode()))			
				m_pMyWorld->clPlayer[i]->SendFieldMsg(siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5);			
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	배열 인덱스를 얻어옴.
//	수정 일자	:	2002-03-16 오후 2:49:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltGuild::GetArrayIndex()
{
	return	m_uiArrayIndex;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	행수를 지정한다.
//	수정 일자	:	2002-03-21 오후 3:36:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::SetHNS(SOVillage *pVillage)
{	
	CHAR	*pszHNSID;
	
	// 마을의 포인터가 유효한지 알아온다.
	if(pVillage == NULL)	return	FALSE;

	// 최고 투자자의 이름을 얻어온다.
	if((pszHNSID = pVillage->m_Government->GetInvestmentMaxInvestmentorName()) == NULL)	return	FALSE;


	
	// 행수를 지정한다.
	// 행수의 이름을 나중을 위하여 지금 지정해준다.
	pVillage->SetHNSID(pszHNSID);

	return	TRUE;
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	보유하고 있는 마을의 수를 설정한다.
//	수정 일자	:	2002-03-29 오후 1:48:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::UpdateMaxRealMemberNum()
{		
	// 마을의 최대 상단원수를 정한다.
	m_GuildMemberManager.SetMaxRealMemberNum(min(ON_GUILD_MIN_MEMBER_NUM + ((m_Villages.m_NowUseDataCount - 1) * ON_GUILD_ADD_MEMBER_NUM_PER_ONEVILLAGE), ON_GUILD_MAX_MEMBER_NUM));	
}
*/

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 상단이 보유할 수 있는 최대 상단원 수를 얻어온다.
//	수정 일자	:	2002-03-29 오후 1:53:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltGuild::GetMaxGuildMemebersNum()
{
	return m_GuildMemberManager.GetMaxRealMemberNum();
}
*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	깃발을 설정한다.
//	수정 일자	:	2002-04-04 오후 3:26:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::ChangeFlag(SI32 siFlag)
{
	BOOL			bDBSuccess;
	SI32			i;
	

	// 해당 플래그를 다른 상단에서 쓰고 있는지 검사한다
	if(m_pMyWorld->clIGuild.IsUsedFlag(siFlag) == TRUE)	return	FALSE;

	m_siFlag	=	siFlag;

	

	bDBSuccess = TRUE;
	

	if(bDBSuccess)
	{
		// 이 상단에 소속된 모든 플레이어의 깃발을 변경한다.
		for(i = 0; i < ON_MAX_CONNECTION; i++)
		{
			if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
			{
				// 해당 길드원이고 
				// 지원자가 아니여야 한다.
				if((m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID) && (m_pMyWorld->clPlayer[i]->siClassInGuild != m_pMyWorld->clIGuildClass.GetJWJCode()))			
					m_pMyWorld->clPlayer[i]->ChangeGuildFlag(siFlag);					
			}
		}		

		// 이 상단에 소속된 모든 마을의 깃발을 변경한다.
		for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
		{
			if(m_pMyWorld->pclVillage[i].uiGuildID == m_uiGuildID)
				m_pMyWorld->pclVillage[i].ChangeGuildFlag(siFlag);				
		}

		return	TRUE;	
	}
	else
	{
		return	FALSE;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	깃발을 얻어온다.
//	수정 일자	:	2002-04-04 오전 11:48:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltGuild::GetFlag()
{	
	return	m_siFlag;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	스택에서의 깃발 ID를 설정한다.
//	수정 일자	:	2002-04-04 오후 2:59:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::SetFlagIDInStack(SI32 siFlagIDInStack)
{
	m_siFlagIDInStack	=	siFlagIDInStack;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	스택에서의 깃발 ID를 얻어온다.
//	수정 일자	:	2002-04-04 오후 2:36:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltGuild::GetFlagIDInStack()
{
	return m_siFlagIDInStack;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	지원자를 추가한다.
//	수정 일자	:	2002-04-13 오전 9:37:39 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::AddCandidate(SOPlayer *pPlayer)
{	
	DBIKGuildJoinInfo			GuildJoinInfo;
	SI32						siLevel;		
	
	// DB에 저장한다.	
	ZeroMemory(&GuildJoinInfo, sizeof(GuildJoinInfo));
	GuildJoinInfo.uiGuildID		=	m_uiGuildID;	
	GuildJoinInfo.uiClass		=	m_pMyWorld->clIGuildClass.GetJWJCode();				

	GuildJoinInfo.siAccount     =   pPlayer->stAccountInfo.siDBAccount;
	GuildJoinInfo.uiSlot        =   pPlayer->stAccountInfo.uiSlot;

	
	siLevel		=	pPlayer->IFollower.GetFollowerParameterLevel(0);
	
	// 상단원 관리 클래스에게도 알려준다.
	if(m_GuildMemberManager.AddMember(pPlayer->szID,GuildJoinInfo.uiClass, pPlayer->GetMoney() + pPlayer->GetSaving(),siLevel, pPlayer->uiTradeCredit,pPlayer->stAccountInfo.siDBAccount,pPlayer->stAccountInfo.uiSlot) == TRUE)
	{
		// 플레이어의 상단을 설정한다.
		pPlayer->SetGuild( m_uiGuildID );
		pPlayer->SetClassInGuild( GuildJoinInfo.uiClass );

		// 게임 서버에서 성공적으로 저장 했을 경우에만 DB에 저장한다.
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_JOIN, &GuildJoinInfo, sizeof(GuildJoinInfo));

		return	TRUE;
	}
	else
		return	FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	직급을 변경시킨다.
//	수정 일자	:	2002-04-13 오전 9:58:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::ChangeClass( CHAR *pszName, SI32 siNewClass )
{
	DBIKGuildChangeClassInfo	GuildChangeClassInfo;	
	SOPlayer					*pPlayer = NULL;

	// 정말 길드 멤버인지 알아온다.
	strGuildCommonMemberInfo MemberInfo;

	if( m_GuildMemberManager.GetGuildMemberByCharName(pszName, &MemberInfo) == FALSE )
	{
		return FALSE;
	}

	// by semarine
	m_GuildMemberManager.ChangeClass( pszName, siNewClass, FALSE );
	
	// DB에 저장한다.
	GuildChangeClassInfo.siDBAccount	=	MemberInfo.siAccount;
	GuildChangeClassInfo.uiSlot			=	MemberInfo.uiSlot;

	GuildChangeClassInfo.uiGuildID		=	m_uiGuildID;		
	GuildChangeClassInfo.uiChangeClass	=	siNewClass;

	
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_CHANGE_CLASS, &GuildChangeClassInfo, sizeof(GuildChangeClassInfo) );

	// 현재 접속되어 있는지 알아온다	
	// 현재 접속되어 있다면 알려준다.
	if( (pPlayer = m_pMyWorld->GetPlayerByDBAccount(MemberInfo.siAccount,MemberInfo.uiSlot)) != NULL)
	{
		// 새로운 직급으로 변경해준다.
		pPlayer->SetClassInGuild( siNewClass );
		// 메시지를 보내준다.
		pPlayer->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_CHANGECLASS), LPVOID(siNewClass));	
	}

	// 모든 상단원에게 상단 정보가 변경되었다는 것을 알려준다.(MS)
	GuildInfoChanged();
	
	return	TRUE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 이 마을에 소속된 마을의 코드를 얻어온다.
//	수정 일자	:	2002-04-13 오후 6:32:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::GetOwnerVillages(UI16 *puiOwnerVillageNum, UI16 *puiVillageCode)
{
	*puiOwnerVillageNum	=	m_Villages.m_NowUseDataCount;	
	memcpy(puiVillageCode, m_Villages.m_Data, sizeof(UI16) * m_Villages.m_NowUseDataCount);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 이 마을에 소속된 마을의 기본 정보를 얻어온다.
//	수정 일자	:	2002-11-27 오후 2:45:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::GetOwnerGuildVillageInfo( UI16 *puiOwnerVillageNum, OnGuildVillageInfo *pGuildVillageInfo )
{
	SI32			i;	
	SOVillage		*pVillage;	
	
	for( i = 0; i < m_Villages.m_NowUseDataCount; i++ )
	{
		pVillage = m_pMyWorld->GetVillageByCode( m_Villages.m_Data[ i ] );

		pVillage->GetInfo( &pGuildVillageInfo[ i ] );	
	}

	*puiOwnerVillageNum		=	m_Villages.m_NowUseDataCount;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정식 상단원의 리스트를 얻어온다.
//	수정 일자	:	2002-04-22 오전 11:36:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltGuild::GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo)
{
	return	m_GuildMemberManager.GetRMList(dwPage, pGuildMemeberInfo);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	지원자 리스트를 얻어온다.
//	수정 일자	:	2002-04-22 오후 2:32:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltGuild::GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo)
{
	return	m_GuildMemberManager.GetCMList(dwPage, pCandidateInfo);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전체 정식 상단원을 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
//	수정 일자	:	2002-04-22 오후 2:31:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltGuild::GetRMTotalPageNum()
{
	return m_GuildMemberManager.GetRMTotalPageNum();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전체 지원자를 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
//	수정 일자	:	2002-04-22 오후 2:31:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltGuild::GetCMTotalPageNum()
{
	return m_GuildMemberManager.GetCMTotalPageNum();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터 이름을 사용하여 상단의 ID와 길드 직급을 얻어온다.
//	수정 일자	:	2002-05-27 오후 2:40:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass)
{
	if(m_GuildMemberManager.GetGuildIDByCharName(pszName, psiGuildClass) == TRUE)
	{
		*puiGuildID	=	m_uiGuildID;

		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
}

VOID	cltGuild::GuildMemberLogin()
{
	m_siLoginedGuildMemeberNum++;
}

VOID	cltGuild::GuildMemberLogout()
{
	m_siLoginedGuildMemeberNum--;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 각 Player 인스턴스에 상단 정보 변경을 알려준다.
//	수정 일자	:	2004-01-05 PM __Lee_min_su__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void	cltGuild::GuildInfoChanged()
{
	SOPlayer*	pPlayer = NULL;
	UI32		nMemberCount = 0;
	DWORD		dwRMTotalPageNum = m_GuildMemberManager.GetRMTotalPageNum();
	strGuildMemberInfo stInfo[ON_GUILD_MEMBERNUM_PER_PAGE];

	for(DWORD dwPageNum = 0 ; dwPageNum < dwRMTotalPageNum ; dwPageNum++)
	{
		memset(stInfo, 0, sizeof(strGuildMemberInfo)*ON_GUILD_MEMBERNUM_PER_PAGE);
		nMemberCount = GetRMList(dwPageNum+1, stInfo);

		for(UI32 n = 0 ; n < nMemberCount && 
			n < ON_GUILD_MEMBERNUM_PER_PAGE ; n++)
		{
			if((pPlayer = m_pMyWorld->GetPlayer(stInfo[n].szName)) == NULL)
				continue;

			// 각 상단의 Player 인스턴스에게 상단정보가 변경되었음을 
			// 알린다.
			pPlayer->SetGuildInfoChanged();
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단 정보를 얻는다.
//	수정 일자	:	2004-01-05 PM __Lee_min_su__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL cltGuild::GetAuditGuildInfo(SOPlayer* pPlayer)
{
	if(pPlayer == NULL) 
		return FALSE;

	strGuildMemberInfo		stInfo[ON_GUILD_MEMBERNUM_PER_PAGE];
	strAuditGuildMemberInfo stGuildMemberInfo[SO_TOTAL_GUILD_NUM];	
	UI32					nMemberCount = 0;
	int						nCount = 0;
	SI32					siSHNClass = \
		m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);

	UI32					nRealMemberNum = GetGuildMemberNum();
	DWORD					dwRMTotalPageNum = m_GuildMemberManager.GetRMTotalPageNum();	

	memset(stGuildMemberInfo, 0, \
		sizeof(strAuditGuildMemberInfo)*SO_TOTAL_GUILD_NUM);

	for(DWORD dwPageNum = 0 ; dwPageNum < dwRMTotalPageNum ; dwPageNum++)
	{
		memset(stInfo, 0, sizeof(strGuildMemberInfo)*ON_GUILD_MEMBERNUM_PER_PAGE);
		nMemberCount = GetRMList(dwPageNum+1, stInfo);
		assert(nMemberCount != 0);

		for(UI32 n = 0 ; n < nMemberCount &&
			n < ON_GUILD_MEMBERNUM_PER_PAGE ; n++)
		{
			if( nCount >= SO_TOTAL_GUILD_NUM)
				break;
			// 사환은 제외한다.
			if(stInfo[n].siClassInGuild == siSHNClass)
				continue;

			strncpy(stGuildMemberInfo[nCount].szName, stInfo[n].szName, ON_ID_LENGTH+1);
			stGuildMemberInfo[nCount].siClassInGuild = stInfo[n].siClassInGuild;			

			nCount++;
		}
	}

	pPlayer->SendFieldMsg(ON_RESPONSE_AUDIT_GUILD_INFO, \
		SO_SFM_ONLYME, LPVOID(nRealMemberNum), \
		LPVOID(m_siLoginedGuildMemeberNum), LPVOID(nCount), LPVOID(stGuildMemberInfo));
	
	return TRUE;
}


SI32 cltGuild::IncreaseGuildSupply(SI32 siSupply)
{
	m_siGuildSupply += siSupply;
	return m_siGuildSupply;
}

SI32 cltGuild::DecreaseGuildSupply(SI32 siSupply)
{
	m_siGuildSupply -= siSupply;
	return m_siGuildSupply;
}

SI32 cltGuild::GetGuildSupply()
{
	return m_siGuildSupply;
}

void cltGuild::SetGuildSupply(SI32	siSupply)
{
	m_siGuildSupply = siSupply;
}


void cltGuildUnit::InitGuildUnit()
{
	for(int i = 0; i < ON_MAX_GUILD_UNIT; i++)
	{
		siGuildUnit[i]			=	0;
	}
}	

SI16 cltGuildUnit::IncreaseGuildUnit(SI16 siUnitKind,SI16 siCount)
{
	siGuildUnit[siUnitKind] += siCount;
	if(siGuildUnit[siUnitKind] >= ON_MAX_GUILD_UNIT_COUNT) 
		siGuildUnit[siUnitKind] = ON_MAX_GUILD_UNIT_COUNT;

	return siGuildUnit[siUnitKind];
}

SI16 cltGuildUnit::DecreaseGuildUnit(SI16 siUnitKind,SI16 siCount)
{
	siGuildUnit[siUnitKind] -= siCount;
	if(siGuildUnit[siUnitKind] <= 0) 
		siGuildUnit[siUnitKind] = 0;

	return siGuildUnit[siUnitKind];
}

SI16 cltGuildUnit::GetGuildUnit(SI16 uiUnitKind)
{
	if(siGuildUnit[uiUnitKind] <= 0)
		siGuildUnit[uiUnitKind] = 0;
	else if(siGuildUnit[uiUnitKind] >= ON_MAX_GUILD_UNIT_COUNT)
		siGuildUnit[uiUnitKind] = ON_MAX_GUILD_UNIT_COUNT;
		
	return siGuildUnit[uiUnitKind];
}

void cltGuildUnit::SetGuildUnit(SI16 siUnitKind,SI16 siCount)
{
	if(siUnitKind < ON_GUILD_UNIT_SWORD || siUnitKind >= ON_MAX_GUILD_UNIT) return;
	
	if(siCount >= ON_MAX_GUILD_UNIT_COUNT) 
		siGuildUnit[siUnitKind] = ON_MAX_GUILD_UNIT_COUNT;
	else if(siCount <= 0)
		siGuildUnit[siUnitKind] = 0;
	else
		siGuildUnit[siUnitKind] = siCount;
}

void cltGuildResult::SetCharName(char* szName)
{
	strcpy(szCharName,szName);
}

char* cltGuildResult::GetCharName()
{
	return szCharName;
}

void cltGuildResult::SetVillageCode(SI16 siCode)
{
	siVillageCode = siCode;
}

SI16 cltGuildResult::GetVillageCode()
{
	return siVillageCode;
}

void cltGuildResult::SetPercent(SI16 siPer)
{
	siPercent	  = siPer;	
}

SI16 cltGuildResult::GetPercent()
{
	return siPercent;
}

BOOL cltGuild::SetPowerInvestmentEnd()
{
	for(SI16 i = 0 ; i < ON_MAX_GUILD_RESULT ; i++)
	{
		m_stGuildResult[i].SetCharName("");
		m_stGuildResult[i].SetPercent(0);
		m_stGuildResult[i].SetVillageCode(0);
	}
	return TRUE;
}

BOOL cltGuild::SendGuildItemDeleteMessage()
{
	SOPlayer*	pPlayer = NULL;
	UI32		nMemberCount = 0;
	DWORD		dwRMTotalPageNum = m_GuildMemberManager.GetRMTotalPageNum();
	strGuildMemberInfo stInfo[ON_GUILD_MEMBERNUM_PER_PAGE];

	for(DWORD dwPageNum = 0 ; dwPageNum < dwRMTotalPageNum ; dwPageNum++)
	{
		memset(stInfo, 0, sizeof(strGuildMemberInfo)*ON_GUILD_MEMBERNUM_PER_PAGE);
		nMemberCount = GetRMList(dwPageNum+1, stInfo);

		for(UI32 n = 0 ; n < nMemberCount && 
			n < ON_GUILD_MEMBERNUM_PER_PAGE ; n++)
		{
			if((pPlayer = m_pMyWorld->GetPlayer(stInfo[n].szName)) == NULL)
				continue;

			pPlayer->SendGuildItemDeleteMessage();
			
		}
	}	
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 초기화 한다.
//	수정 일자	:	2002-03-16 오후 2:49:32 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::InitRelation(UI32 uiArrayIndex)
{
	// 지금은 단지 0으로 설정만 하면 된다.
	m_siGuildRelation[uiArrayIndex]	=	0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	적대적인 관계인지 알아온다.
//	수정 일자	:	2002-03-19 오후 3:00:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::IsEnemyGuild(UI32 uiArrayIndex)
{
	if(m_siGuildRelation[uiArrayIndex] == ON_GUILD_RELATION_ENEMY)
		return	TRUE;
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공성이 가능한 상태(적대 상단을 건지 3일이 지난 하)인지 알아온다.
//	수정 일자	:	2002-03-19 오후 3:00:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuild::IsWarGuild(UI32 uiArrayIndex)
{
	if(m_siGuildRelation[uiArrayIndex] == ON_GUILD_RELATION_WAR)
		return TRUE;
	else
		return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 설정한다.
//	수정 일자	:	2002-03-15 오후 5:17:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuild::SetRelation(cltGuild *pGuild, SI32 siRelation, BOOL bInit,SI32 siDbTime)
{
	DBIKGuildRelation	GuildRelation;	
	BOOL				bDBSuccess = TRUE;
	SI16				siNowYear,siNowMon,siNowDay;
	SI16				siStartYear,siStartMon,siStartDay;
	char				szTemp[10],szTemp2[10],szTemp3[10],szTemp4[10];
	SI32				siTime;


	GuildRelation.uiGuildID1	=	m_uiGuildID;
	GuildRelation.uiGuildID2	=	pGuild->GetGuildID();	
	GuildRelation.siTime		=	0;
	siTime						=	0;

	siTime = siDbTime;

	switch(siRelation)
	{
	case ON_GUILD_RELATION_ALLY:
		// 동맹국으로 설정했다.						
		GuildRelation.siRelation	=	ON_GUILD_RELATION_ALLY;	
		break;
	case ON_GUILD_RELATION_ENEMY:
		{
			// 적대국으로 설정했다.	
			// 적대를 건 날짜를 저장해서 다음주에 죽인다.
			GuildRelation.siRelation	=	ON_GUILD_RELATION_ENEMY;

			if(siDbTime != 0)
			{
				//DB에서 날짜를 받아왔을 경우.
				sprintf(szTemp,"%d",siDbTime);
				
				siStartYear = atoi(strncpy(szTemp2,szTemp,4));
				siStartMon  = atoi(strncpy(szTemp3,&szTemp[4],2));
				siStartDay  = atoi(strncpy(szTemp4,&szTemp[6],2));

//				if(CheckDay(siStartYear, siStartMon, siStartDay))
				if(CheckDay2(siStartYear, siStartMon, siStartDay))				
				{
					GuildRelation.siRelation = ON_GUILD_RELATION_WAR;
					m_bIsWar = TRUE;
				}
/*
				SI16 siCheckDay = m_pMyWorld->clQuestParser.GetComPareDate(siStartYear,siStartMon,siStartDay,
					m_pMyWorld->pRealTime->tm_year + 1900,m_pMyWorld->pRealTime->tm_mon + 1,m_pMyWorld->pRealTime->tm_mday);

				if(siCheckDay >= 1)
				{
					GuildRelation.siRelation	=	ON_GUILD_RELATION_WAR;
					m_bIsWar = TRUE;
				}
				*/
			}
		}
		break;
	case ON_GUILD_RELATION_NEUTRAL:
		// 중립이 되었다.
		GuildRelation.siRelation	=	ON_GUILD_RELATION_NEUTRAL;		
		break;
	}

	// 초기화가 아닐 때에만 DB에 설정하고
	// 메시지를 보내준다.
	if(bInit == FALSE)
	{
		siNowYear					=	m_pMyWorld->RealWorldTime.uiYear;
		siNowMon					=	m_pMyWorld->RealWorldTime.uiMonth;
		siNowDay					=	m_pMyWorld->RealWorldTime.uiDay;

		sprintf(szTemp,"%04d%02d%02d",siNowYear,siNowMon,siNowDay);
		GuildRelation.siTime		=	atoi(szTemp);	

		// DB에 성공적으로 저장했는지 알아온다.
		//bDBSuccess	=	m_pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_GUILD_SETRELATION, LPVOID(&GuildRelation), NULL);	
		bDBSuccess = TRUE;
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_SETRELATION, &GuildRelation, sizeof(GuildRelation) );

		//SendMsgToGuildMemebers(ON_RESPONSE_GUILD_SET_RELATION, SO_SFM_ONLYME, LPVOID(siRelation), LPVOID(pGuild->GetGuildName()));	

		siTime = GuildRelation.siTime;
	}



	if(bDBSuccess == TRUE)
	{
		m_siGuildRelation[pGuild->GetArrayIndex()]				=	GuildRelation.siRelation;
		m_siGuildRelationEnemyTime[pGuild->GetArrayIndex()]		=   siTime;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 얻어온다.
//	수정 일자	:	2002-03-15 오후 5:21:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltGuild::GetRelation(UI32 uiArrayIndex)
{
	return	m_siGuildRelation[uiArrayIndex];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 재설정한다.(매일 12시마다)
//	수정 일자	:	2002-03-15 오후 5:17:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void cltGuild::SetRelationReLoad()
{
	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM ; i++)
	{
		if(m_siGuildRelation[i] == ON_GUILD_RELATION_ENEMY)
		{
			SI16	siStartYear,siStartMon,siStartDay;
			char	szTemp[10],szTemp2[10],szTemp3[10],szTemp4[10];

			sprintf(szTemp,"%d",m_siGuildRelationEnemyTime[i]);
			
			siStartYear = atoi(strncpy(szTemp2,szTemp,4));
			siStartMon  = atoi(strncpy(szTemp3,&szTemp[4],2));
			siStartDay  = atoi(strncpy(szTemp4,&szTemp[6],2));

//			if(CheckDay(siStartYear, siStartMon, siStartDay))
			if(CheckDay2(siStartYear, siStartMon, siStartDay))
			{
				m_siGuildRelation[i] = ON_GUILD_RELATION_WAR;
				m_bIsWar = TRUE;
			}
			/*
			SI16 siCheckDay = m_pMyWorld->clQuestParser.GetComPareDate(siStartYear,siStartMon,siStartDay,
				m_pMyWorld->RealWorldTime.uiYear,m_pMyWorld->RealWorldTime.uiMonth,m_pMyWorld->RealWorldTime.uiDay);

			//적대 건지 3일이 지났다면 공성모드로 바꿔준다.
			if(siCheckDay >= 1)
			{
				m_siGuildRelation[i]				=	ON_GUILD_RELATION_WAR;
				m_bIsWar = TRUE;
			}
			*/
		}
	}
}

BOOL	cltGuild::SetFriendlyMessage(cltGuild *pGuild,CHAR* pszFriendlyMsg)
{
	if(pGuild->m_szGuildfriendlyMessage[GetArrayIndex()] != NULL)
	{
		strncpy(pGuild->m_szGuildfriendlyMessage[GetArrayIndex()],(char*)pszFriendlyMsg,81);

		DBIKGuildFrieldlyMessageOffer	TempGuildFrieldlyMessageOffer;

		TempGuildFrieldlyMessageOffer.uiGuildID1	=	pGuild->m_uiGuildID;	//내가 보내는 상단의 아이디.
		TempGuildFrieldlyMessageOffer.uiGuildID2	=	m_uiGuildID;			//내 아이디.
		strncpy(TempGuildFrieldlyMessageOffer.szMessage,(char*)pszFriendlyMsg,81);

		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_OFFER,&TempGuildFrieldlyMessageOffer,sizeof(DBIKGuildFrieldlyMessageOffer));

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL	cltGuild::SetMessage(cltGuild* pGuild,CHAR* pszMsg)
{
	strncpy(pGuild->m_szGuildfriendlyMessage[GetArrayIndex()],(char*)pszMsg,81);
	return TRUE;
}

BOOL	cltGuild::SetFriendlyMessageAccept(cltGuild* pGuild)
{
	BOOL bTemp = FALSE;
//	if(m_pMyWorld->pRealTime->tm_wday == 2 || m_pMyWorld->pRealTime->tm_wday == 4)
//	{
//		if(m_pMyWorld->pRealTime->tm_hour >= 0 && m_pMyWorld->pRealTime->tm_hour <= 20)
//		{			
			if( (pGuild->m_siGuildRelation[GetArrayIndex()] == ON_GUILD_RELATION_WAR) && 
			    (m_siGuildRelation[pGuild->GetArrayIndex()] == ON_GUILD_RELATION_WAR) ) 
			{
				bTemp = TRUE;
			}
//		}
//	}

	//화친제의를 받아들였을 경우 적대 상단에 관련된 정보를 모두 지워준다.
	pGuild->m_siGuildRelation[GetArrayIndex()] = 0;
	strcpy(pGuild->m_szGuildfriendlyMessage[GetArrayIndex()],"");
	pGuild->m_siGuildRelationEnemyTime[GetArrayIndex()] = 0;

	m_siGuildRelation[pGuild->GetArrayIndex()] = 0;
	strcpy(m_szGuildfriendlyMessage[pGuild->GetArrayIndex()],"");
	m_siGuildRelationEnemyTime[pGuild->GetArrayIndex()] = 0;
	
	DBIKGuildFriendlyMessageAccept	TempDBIKGuildFriendlyMessageAccept;
	TempDBIKGuildFriendlyMessageAccept.uiGuildID1	=	m_uiGuildID;
	TempDBIKGuildFriendlyMessageAccept.uiGuildID2	=	pGuild->m_uiGuildID;
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_ACCEPT,&TempDBIKGuildFriendlyMessageAccept,sizeof(DBIKGuildFriendlyMessageAccept));

	//공성을 계속 할것인지 다른상단과 검사...
	if(bTemp)
	{
		CheckWar();
		pGuild->CheckWar();
	}


/*	DBIKGuildFriendlyMessageAccept	TempDBIKGuildFriendlyMessageAccept2;
	TempDBIKGuildFriendlyMessageAccept2.uiGuildID1	=	pGuild->m_uiGuildID;
	TempDBIKGuildFriendlyMessageAccept2.uiGuildID2	=	m_uiGuildID;
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_ACCEPT,&TempDBIKGuildFriendlyMessageAccept2,sizeof(DBIKGuildFriendlyMessageAccept));*/

	return TRUE;

}

BOOL	cltGuild::SetFriendlyMessageCancel(cltGuild* pGuild)
{
	//화친제의를 받아들이지 않았을 경우 메세지만 지워준다.
	strcpy(m_szGuildfriendlyMessage[pGuild->GetArrayIndex()],"");

	DBIKGuildFriendlyMessageCancel	TempDBIKGuildFriendlyMessageCancel;
	TempDBIKGuildFriendlyMessageCancel.uiGuildID1	=	m_uiGuildID;
	TempDBIKGuildFriendlyMessageCancel.uiGuildID2	=	pGuild->m_uiGuildID;
	
	strcpy(TempDBIKGuildFriendlyMessageCancel.szMessage,"");

	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_CANCEL,&TempDBIKGuildFriendlyMessageCancel,sizeof(DBIKGuildFriendlyMessageCancel));

	return TRUE;
}

char* cltGuild::GetFrieldlyMessage(UI32 uiArrayIndex)
{
	return m_szGuildfriendlyMessage[uiArrayIndex];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	대방에게 공성 결과에 대한 돈과 보급품을 준다.
//	수정 일자	:	2003-01-03 오전 11:01:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//VOID	cltGuild::RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode ,SI16 siRewardSupply)
VOID	cltGuild::RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode)
{
	SOPlayer	*pPlayer;
	MONEY		TempMoney;

	// 현재 대방이 로그인 중이면 저금액을 변경해준다.
	if( ( pPlayer = m_pMyWorld->GetPlayer( m_szDAIID ) ) != NULL )
	{
		TempMoney = pPlayer->ModifySaving( mnReward );

		DBIKGuildReward		TempDBIKGuildReward;
		memset(&TempDBIKGuildReward, 0, sizeof(DBIKGuildReward));
		
		TempDBIKGuildReward.siAccount			=	pPlayer->stAccountInfo.siDBAccount;
		TempDBIKGuildReward.uiSlot				=	pPlayer->stAccountInfo.uiSlot;
		TempDBIKGuildReward.mnMoney				=	TempMoney;
		TempDBIKGuildReward.uiGuildID			=	pPlayer->uiGuildID;
		TempDBIKGuildReward.siIncreaseSupply	=	0;
		
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_WAR_PROFIT, &TempDBIKGuildReward, sizeof(DBIKGuildReward));


		OnOADAIGetMoneyByDestroyVillageMsg			OADAIGetMoneyByDestroyVillageMsg;
		memset(&OADAIGetMoneyByDestroyVillageMsg, 0, sizeof(OnOADAIGetMoneyByDestroyVillageMsg));
		
		OADAIGetMoneyByDestroyVillageMsg.dwMsgType		=	ON_OFFICIAL_ANNOUNCEMENT;
		OADAIGetMoneyByDestroyVillageMsg.dwResponse		=	ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE;
		OADAIGetMoneyByDestroyVillageMsg.mnMoney		=	TempMoney;
		OADAIGetMoneyByDestroyVillageMsg.uiVillageCode	=	uiVillageCode;
		
		// 전체 상단원에게 메시지를 보내준다.
		SendMsgToGuildMemebers( ON_OFFICIAL_ANNOUNCEMENT, SO_SFM_ONLYME, 
							LPVOID( &OADAIGetMoneyByDestroyVillageMsg  ), 
							LPVOID( sizeof( OADAIGetMoneyByDestroyVillageMsg ) ) );
	}
	else //대방이 로그아웃 상태이다.
	{
		DBIKGuildFallVillage              TempDBIKGuildFallVillage;
		memset(&TempDBIKGuildFallVillage, 0, sizeof(DBIKGuildFallVillage));

		TempDBIKGuildFallVillage.uiVillageCode = uiVillageCode;
		TempDBIKGuildFallVillage.uiGuildID     = m_uiGuildID;
		TempDBIKGuildFallVillage.mnMoney       = mnReward;

		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_REWARDTODAIFORDESTROYVILLAGE,
			                                 &TempDBIKGuildFallVillage, sizeof(DBIKGuildFallVillage));
		

		OnOADAIGetMoneyByDestroyVillageMsg			OADAIGetMoneyByDestroyVillageMsg;
		memset(&OADAIGetMoneyByDestroyVillageMsg, 0, sizeof(OnOADAIGetMoneyByDestroyVillageMsg));

		OADAIGetMoneyByDestroyVillageMsg.dwMsgType		=	ON_OFFICIAL_ANNOUNCEMENT;
		OADAIGetMoneyByDestroyVillageMsg.dwResponse		=	ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE;
		OADAIGetMoneyByDestroyVillageMsg.mnMoney		=	mnReward;
		OADAIGetMoneyByDestroyVillageMsg.uiVillageCode	=	uiVillageCode;
		

		// 전체 상단원에게 메시지를 보내준다.
		SendMsgToGuildMemebers( ON_OFFICIAL_ANNOUNCEMENT, SO_SFM_ONLYME, 
							    LPVOID( &OADAIGetMoneyByDestroyVillageMsg  ), 
							    LPVOID( sizeof( OADAIGetMoneyByDestroyVillageMsg ) ) );
	}

	m_pMyWorld->OfficialAnnouncement(ON_RET_OA_VILL_SURRENDER, LPARAM(m_szGuildName), LPARAM(uiVillageCode));
}

BOOL cltGuild::CheckDay(SI16 siYear, SI16 siMon, SI16 siDay)
{
	if(siYear < 0 || siMon < 0 || siDay < 0) return FALSE;

	BOOL bChek = FALSE;

	//공성을 하는 요일이 아니면 false	
//	if(m_pMyWorld->pRealTime->tm_wday == 2 || m_pMyWorld->pRealTime->tm_wday == 4)	
	if(m_pMyWorld->m_WarTimeParser.IsWarDay(m_pMyWorld->pRealTime->tm_wday))
	{
		if(m_pMyWorld->pRealTime->tm_year + 1900 > siYear) return TRUE;
		if(m_pMyWorld->pRealTime->tm_mon + 1 > siMon ) return TRUE;

//		SI16 siCheck = m_pMyWorld->pRealTime->tm_yday - siDay;
		SI16 siCheck = m_pMyWorld->pRealTime->tm_mday - siDay;
		if(siCheck <= 1) return bChek;

		//적대건 요일을 구한다.
		int siWeekly = m_pMyWorld->pRealTime->tm_wday;
		for(int i = 0; i < siCheck; i++)
		{
			siWeekly--;
			if(siWeekly < 0) siWeekly = 6;
		}

		switch(siWeekly)
		{
		case 0: //sun
			{
				if(siCheck >= 2) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;

		case 1: //mon
			{
				if(siCheck >= 8) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;

		case 2: //tue
			{
				if(siCheck >= 7) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;

		case 3: //wed
			{
				if(siCheck >= 6) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;

		case 4: //thu
			{
				if(siCheck >= 5) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;

		case 5: //fri
			{
				if(siCheck >= 4) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;

		case 6: //sat
			{
				if(siCheck >= 3) bChek = TRUE;
				else             bChek = FALSE;
			}
			break;
		}
    }

	return bChek;
}

BOOL  cltGuild::CheckDay2(SI16 siYear, SI16 siMon, SI16 siDay)
{
	if(m_pMyWorld->pRealTime->tm_year + 1900 > siYear) return TRUE;
	if(m_pMyWorld->pRealTime->tm_mon + 1 > siMon ) return TRUE;
	if(m_pMyWorld->pRealTime->tm_mday - siDay >= 1) return TRUE;

	return FALSE;
}

//화친제의 수락시 다른상단과 공성이 있는지 검사를 해서 없으면 더이상 공성이 없다고 알려준다.
BOOL cltGuild::CheckWar()
{
	int i = 0;
	for(; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_siGuildRelation[i] == ON_GUILD_RELATION_WAR) 
			return TRUE;
	}

	SOPlayer*	pPlayer = NULL;
	UI32		nMemberCount = 0;
	DWORD		dwRMTotalPageNum = m_GuildMemberManager.GetRMTotalPageNum();
	strGuildMemberInfo stInfo[ON_GUILD_MEMBERNUM_PER_PAGE];

	for(DWORD dwPageNum = 0 ; dwPageNum < dwRMTotalPageNum ; dwPageNum++)
	{
		memset(stInfo, 0, sizeof(strGuildMemberInfo)*ON_GUILD_MEMBERNUM_PER_PAGE);
		nMemberCount = GetRMList(dwPageNum+1, stInfo);

		for(UI32 n = 0 ; n < nMemberCount && n < ON_GUILD_MEMBERNUM_PER_PAGE ; n++)
		{
			if((pPlayer = m_pMyWorld->GetPlayer(stInfo[n].szName)) == NULL)
				continue;

			pPlayer->SendFieldMsg(ON_RESPONSE_NOT_WAR, SO_SFM_ONLYME);
		}
	}		
	

	return FALSE;
}

//공성 시간이 되면 공성이 있는 상단의 상단원에게 알려준다.
void cltGuild::SendWarTime()
{
	SOPlayer*	pPlayer = NULL;
	UI32		nMemberCount = 0;
	DWORD		dwRMTotalPageNum = m_GuildMemberManager.GetRMTotalPageNum();
	strGuildMemberInfo stInfo[ON_GUILD_MEMBERNUM_PER_PAGE];

	for(DWORD dwPageNum = 0 ; dwPageNum < dwRMTotalPageNum ; dwPageNum++)
	{
		memset(stInfo, 0, sizeof(strGuildMemberInfo)*ON_GUILD_MEMBERNUM_PER_PAGE);
		nMemberCount = GetRMList(dwPageNum+1, stInfo);

		for(UI32 n = 0 ; n < nMemberCount && n < ON_GUILD_MEMBERNUM_PER_PAGE ; n++)
		{
			if((pPlayer = m_pMyWorld->GetPlayer(stInfo[n].szName)) == NULL)
				continue;

			pPlayer->SendFieldMsg(ON_RESPONE_WAR_TIME, SO_SFM_ONLYME);
		}
	}
}

BOOL cltGuild::CheckVillage(UI16 uiVillageCode)
{
	return m_Villages.FindType(uiVillageCode);
}

//공성포기 정보를 세팅한다.
VOID cltGuild::SetGiveUpInfo(UI16 uiVillageCode, MONEY mnMoney, SI16 siFlag)
{
	if(uiVillageCode < 0) return;
	if(mnMoney < 0) return;

	m_GiveUpInfo.uiVillageCode = uiVillageCode;
	m_GiveUpInfo.mnMoney       = mnMoney;
	m_GiveUpInfo.siGiveUpFlag  = siFlag;
}

//공성포기 정보를 가져온다.
VOID cltGuild::GetGiveUpInfo(UI16 *puiVillageCode, MONEY *pmnMoney)
{
	if(puiVillageCode == NULL || pmnMoney == NULL )return;

	*puiVillageCode = m_GiveUpInfo.uiVillageCode;
	*pmnMoney       = m_GiveUpInfo.mnMoney;
}

//공성포기 정보중 플레그를 세팅한다.
VOID cltGuild::SetGiveUpFlag(SI16 siFlag)
{
	if(siFlag < ON_GUILD_GIVEUP_NORMAL || siFlag > ON_GUILD_GIVEUP_NO) return;
	m_GiveUpInfo.siGiveUpFlag  = siFlag;
}

//공성포기 정보중 플레그를 가져온다.
SI16 cltGuild::GetGiveUpFlag()
{
	return m_GiveUpInfo.siGiveUpFlag;
}

//다른상단과의 관계를 적대로 변경시켜준다.
VOID cltGuild::GiveUp()
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_siGuildRelation[i] == ON_GUILD_RELATION_WAR)
		{
			m_siGuildRelation[i] = ON_GUILD_RELATION_ENEMY;
			if(m_bIsWar) m_bIsWar = FALSE;
		}
	}
}

//상단원들에게 공성을 포기했다고 알려준다.
void cltGuild::SendGiveUpMsg()
{
	SOPlayer*	pPlayer = NULL;
	UI32		nMemberCount = 0;
	DWORD		dwRMTotalPageNum = m_GuildMemberManager.GetRMTotalPageNum();
	strGuildMemberInfo stInfo[ON_GUILD_MEMBERNUM_PER_PAGE];

	for(DWORD dwPageNum = 0 ; dwPageNum < dwRMTotalPageNum ; dwPageNum++)
	{
		memset(stInfo, 0, sizeof(strGuildMemberInfo)*ON_GUILD_MEMBERNUM_PER_PAGE);
		nMemberCount = GetRMList(dwPageNum+1, stInfo);

		for(UI32 n = 0 ; n < nMemberCount && n < ON_GUILD_MEMBERNUM_PER_PAGE ; n++)
		{
			if((pPlayer = m_pMyWorld->GetPlayer(stInfo[n].szName)) == NULL)
				continue;

			pPlayer->SendFieldMsg(ON_RESPONE_GIVEUP, SO_SFM_ONLYME);

			if(m_pMyWorld->m_WarTimeParser.IsWarCheck(m_pMyWorld->pRealTime->tm_wday, m_pMyWorld->pRealTime->tm_hour))
//			if(m_pMyWorld->m_WarTimeParser.IsWarTime(m_pMyWorld->pRealTime->tm_hour)) //공성시간인가?
			{
				if(pPlayer->IsFieldAttackUnit()) //공성유닛인가
					pPlayer->ChangeNormalChar();
			}
		}
	}
}