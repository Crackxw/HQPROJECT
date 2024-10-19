#include "SOMain.h"
#include "SOWorld.h"
#include "IGuild.h"
#include "SODatabase.h"
#include "OnlineCommonStruct.h"

SOWorld*	cltIGuild::m_pMyWorld	=	NULL;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-02-25 오전 9:14:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltIGuild::cltIGuild()
{
	m_Flags.Create(SO_TOTAL_GUILD_NUM);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-02-25 오전 9:14:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltIGuild::~cltIGuild()
{
	Free();	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	한번만 초기화 해줄것들을 해준다.
//	수정 일자	:	2002-04-13 오후 6:13:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::OnceInit(SI32 siTotalVillageNum)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		// 한번만 초기화 해줄것들을 해준다.
		m_Guilds[i].OnceInit(siTotalVillageNum);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	메모리를 해제한다.
//	수정 일자	:	2002-02-26 오후 1:19:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::Free()
{	
	m_Flags.Release();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단 생성시 필요한 금액을 얻어온다.
//	수정 일자	:	2003-01-27 오후 2:24:10 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MONEY	cltIGuild::GetGuildCreateCost()
{
	// min(30000000, 2000000 + (상단개설회수 *20000) )
	// 상단개설회수 : 월드가 열린 이후로 지금까지 상단이 개설된 누적 회수.
	// 상단 개설 비용은 최고 3천만냥을 넘길 수 없다.

	return min( (MONEY)30000000, (MONEY)(2000000 + (m_uiMaxGuildID * 20000)) );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 생성한다.
//	수정 일자	:	2002-02-25 오전 9:14:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltIGuild::CreateGuild(CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage)
{
	SI32					siEmptySlot;		
	DBIKGuildCreateInfo		GuildCreateInfo;		
	UI32					uiNewGuildID;

	SI32					i;

	

	if((siEmptySlot	= FindEmptySlot()) == -1)		return	0;
	
	// 상단의 이름이 없으면 안된다.
	if(pszGuildName[0] == NULL)						return	0;
	if(pVillage == NULL)							return	0;		

	ZeroMemory(&GuildCreateInfo, sizeof(GuildCreateInfo));

	// 상단의 이름을 설정한다.
	GuildCreateInfo.siAccount = pPlayer->stAccountInfo.siDBAccount;
	GuildCreateInfo.uiSlot = pPlayer->stAccountInfo.uiSlot;

	strncpy(GuildCreateInfo.szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);		
	
	GuildCreateInfo.uiVillageCode	=	pVillage->uiVillageCode;	

	//GuildCreateInfo.moMoney = ON_GUILD_COST_CREATE_GUILD;
	GuildCreateInfo.moMoney = GetGuildCreateCost();

	
	// 최대 길드아이디를 하나 증가시켜 새 길드의 아이디로 사용 (이렇게해서 서버가 한번부팅했을때 같은 id를 갖는 길드는 2개이상 생기지않는다.)
	m_uiMaxGuildID++;
	uiNewGuildID = m_uiMaxGuildID;
	GuildCreateInfo.uiGuildID = uiNewGuildID;

	// DB에서 길드를 생성한다.
	// 상단을 생성한다.
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_CREATE, LPVOID(&GuildCreateInfo), sizeof(GuildCreateInfo));
	
	// 상단을 설정한다.
	pPlayer->SetGuild(uiNewGuildID);
	
	// 직급을 설정한다.
	pPlayer->SetClassInGuild(m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI));

	m_Guilds[siEmptySlot].CreateGuild(siEmptySlot, uiNewGuildID, pszGuildName, pPlayer, pVillage);	

	// 길드간의 설정을 한다
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{			
			// 처음에는 서로간에 중립적이다.
			if(i != siEmptySlot)
			{
				SetRelationByIndex(siEmptySlot, i, ON_GUILD_RELATION_NEUTRAL, FALSE,0);			
				SetRelationByIndex(i, siEmptySlot, ON_GUILD_RELATION_NEUTRAL, FALSE,0);
			}			
		}	
	}

	// 상단이 설립 되었다는 공지 사항을 보내준다.
	m_pMyWorld->OfficialAnnouncement(ON_RET_OA_GUILD_CREATED, LPARAM(pszGuildName));

	return uiNewGuildID;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 설정한다.
//	수정 일자	:	2002-02-25 오전 9:14:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::InitGuild(UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, SI32 uiGuildMemeberNum, SI32 siFlag)
{
	SI32			siEmptySlot;			
	strFlagInfo		stFlagInfo;
	SI32			siFlagIDInStack = -1;


	siEmptySlot	=	FindEmptySlot();
	if(siEmptySlot == -1)	return FALSE;
	
	if(siFlag != 0)
	{
		stFlagInfo.uiGuildID	=	uiGuildID;
		stFlagInfo.siFlag		=	siFlag;

		// 깃발이 있다면 깃발 리스트에 저장한다.
		siFlagIDInStack			=	m_Flags.Add(stFlagInfo);
	}

	// 길드를 초기화 한다.
	m_Guilds[siEmptySlot].InitGuild(siEmptySlot, uiGuildID, pszGuildName, pszDAIID, uiGuildMemeberNum, siFlag, siFlagIDInStack);	

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단을 삭제한다.	
//	수정 일자	:	2002-02-25 오전 9:14:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::DeleteGuild(UI32 uiGuildID,SOVillage* pVillage)
{
	SI32			siFoundSlot;
	SI32			i = 0;	
	strFlagInfo		stFlagInfo;	
	CHAR			szGuildName[ON_GUILDNAME_LENGTH + 1];

	siFoundSlot	=	FindSlot(uiGuildID);

	if(siFoundSlot == -1 )	return FALSE;

	//공성중인 상단은 해체가 안된다.
//	if(m_Guilds[siFoundSlot].IsWar()) return FALSE;

	
	// 삭제하려는 길드와의 관계를 모두 초기화한다.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{	
		if(i == siFoundSlot)	continue;

		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(siFoundSlot != i)
				m_Guilds[i].InitRelation(siFoundSlot);
		}
	}
	//상단에 주던 데미지를 제거한다.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++ )
	{
		if(pVillage->m_stAttackGuild[i].uiGuildID != 0) continue;

		if(pVillage->m_stAttackGuild[i].uiGuildID == uiGuildID)
		{
			ZeroMemory(&pVillage->m_stAttackGuild[i],sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);
		}
	}
	//상단에 결과값을 버린다.
	ZeroMemory(m_Guilds[siFoundSlot].m_stGuildResult,sizeof(cltGuildResult) * ON_MAX_GUILD_RESULT);
	//상단에 소속된 길드 유닛을 버린다.
	ZeroMemory(&m_Guilds[siFoundSlot].m_GuildUnit,sizeof(cltGuildUnit));

	m_Guilds[siFoundSlot].SendGuildItemDeleteMessage();

	stFlagInfo.uiGuildID	=	uiGuildID;
	stFlagInfo.siFlag		=	m_Guilds[siFoundSlot].GetFlag();

	// 길드가 삭제 되기전에 이름을 저장해 둔다
	ZeroMemory(szGuildName, sizeof(szGuildName));
	strncpy(szGuildName, m_Guilds[siFoundSlot].GetGuildName(), ON_GUILDNAME_LENGTH);

	if(m_Guilds[siFoundSlot].DeleteGuild() == TRUE)
	{		
		if(stFlagInfo.siFlag != -1)
			m_Flags.DeleteType(stFlagInfo);

		// 상단이 해체 되었다는 공지 사항을 보내준다.
		m_pMyWorld->OfficialAnnouncement(ON_RET_OA_GUILD_DELETED, LPARAM(szGuildName));				
			
		return	TRUE;
	}
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	비어 있는 슬롯을 찾는다.
//	수정 일자	:	2002-02-25 오전 9:19:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltIGuild::FindEmptySlot()
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == FALSE)
			return i;
	}

	return -1;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	슬롯을 찾는다.	
//	수정 일자	:	2002-02-25 오전 9:25:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltIGuild::FindSlot(UI32 uiGuildID)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() == uiGuildID)
			return i;		
	}

	return -1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	슬롯을 찾는다.
//	수정 일자	:	2002-03-15 오후 5:23:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::FindSlot(UI32 uiGuildID1, UI32 uiGuildID2, SI32 *psiIndex1, SI32 *psiIndex2)
{
	SI32	i;
	
	*psiIndex1	=	*psiIndex2	=	-1;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() == uiGuildID1)
			*psiIndex1	=	i;
			
		if(m_Guilds[i].GetGuildID() == uiGuildID2)
			*psiIndex2	=	i;
	}

	if((*psiIndex1 != -1) && (*psiIndex2 != -1))
		return	TRUE;
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	슬롯을 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::FindSlot(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 *psiIndex1, SI32 *psiIndex2)
{
	SI32	i;
	
	*psiIndex1	=	*psiIndex2	=	-1;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() == uiGuildID1)
			*psiIndex1	=	i;
			
		if(strcmp(m_Guilds[i].GetGuildName(), pszGuildID2) == 0)
			*psiIndex2	=	i;
	}

	if((*psiIndex1 != -1) && (*psiIndex2 != -1))
		return	TRUE;
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	슬롯을 찾는다.	
//	수정 일자	:	2002-03-05 오전 11:01:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltIGuild::FindSlot(CHAR *pszName)
{
	SI32	i;
	CHAR	*pszGuildName;

	if(pszName == NULL)	return	-1;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		pszGuildName	=	m_Guilds[i].GetGuildName();

		if(pszGuildName != NULL)
		{
			if(strcmp(pszGuildName, pszName) == 0)	return i;		
		}		
	}

	return -1;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 상단 멤버가 추가되었다.
//	수정 일자	:	2002-03-04 오후 12:49:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::AddGuildMember(UI32 uiGuildID, CHAR *pszName)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
		return m_Guilds[siFoundSlot].AddGuildMember(pszName);			
	else
		return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에서 상단 멤버가 삭제되었다.	
//	수정 일자	:	2002-03-04 오후 12:49:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*BOOL	cltIGuild::DelGuildMember(UI32 uiGuildID, SOPlayer *pPlayer, BOOL bCandidate)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].DelGuildMember(pPlayer->szID, bCandidate, bSendMsg);	
	}	

	return FALSE;
}*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에서 상단 멤버가 삭제되었다.
//	수정 일자	:	2002-03-06 오후 1:22:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::DelGuildMember(UI32 uiGuildID, CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].DelGuildMember(pszName, bCandidate, bSendMsg,bKick);	
	}	

	return FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	지원자를 추가한다.
//	수정 일자	:	2002-04-13 오전 9:45:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::AddCandidate(UI32 uiGuildID, SOPlayer *pPlayer)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].AddCandidate(pPlayer);	
	}	

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 마을이 추가되었다.
//	수정 일자	:	2002-03-04 오후 1:45:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::AddVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bInit)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
	{
		m_Guilds[siFoundSlot].AddVillage(pVillage, bInit);			

		if(bInit == FALSE)
		{
			// 초기화중이 아닐때에만 공지 사항을 보내준다.
			
			// 마을이 해당 상단에 소속되었다는 메시지를 보내준다.
			m_pMyWorld->OfficialAnnouncement(ON_RET_OA_VILL_OCCUPIED, LPARAM(m_Guilds[siFoundSlot].GetGuildName()), LPARAM(pVillage->uiVillageCode));
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에서 마을이 삭제되었다.
//	수정 일자	:	2002-03-04 오후 12:55:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::DelVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bDeleteGuild)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		if(m_Guilds[siFoundSlot].DelVillage(pVillage, bDeleteGuild) == TRUE)
		{
//			m_pMyWorld->clServer->WriteInfo("Delete Guild List.txt", "Deleting clan because of no possessed town [%s]", m_Guilds[siFoundSlot].GetGuildName());
			writeInfoToFile("Delete Guild List.txt", "Deleting clan because of no possessed town [%s]", m_Guilds[siFoundSlot].GetGuildName());

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// for debug
			if( uiGuildID == 0 )
			{
//				m_pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Deleting clan because of no possessed town. But the Clan ID is 0" );
				writeInfoToFile( "DeletedGuild.txt", "Deleting clan because of no possessed town. But the Clan ID is 0" );			
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			m_Guilds[siFoundSlot].SetWarFlag(FALSE);
			// 더 이상 이 상단에는 마을이 없다. 상단을 제거한다.
			DeleteGuild(uiGuildID, pVillage);

			pVillage->m_bIsFall        = FALSE;
			pVillage->m_bVillageAttack = FALSE;
			
		}
		else //전투포기 할것인지 물어본다.
		{
			if(m_pMyWorld->m_WarTimeParser.IsWarCheck(m_pMyWorld->pRealTime->tm_wday, m_pMyWorld->pRealTime->tm_hour)) return;
			if(m_pMyWorld->m_WarTimeParser.IsWarTime(m_pMyWorld->pRealTime->tm_hour)) return;
			if(m_Guilds[siFoundSlot].GetGiveUpFlag() == ON_GUILD_GIVEUP_OK) return;

			SOPlayer *pPlayer = m_pMyWorld->GetPlayer( m_Guilds[siFoundSlot].GetDAIID());			
			UI16 uiVillageCode = pVillage->uiVillageCode; //마을 코드
			SI32 siBestIndex = pVillage->m_Government->GetOldBestInvestIndex(); //최투의 인덱스 
			MONEY mnMoney = pVillage->m_Government->DelInvestMoney(siBestIndex, 1, FALSE); //전투포기 비용

			if(pPlayer != NULL)//대방이 로그인을 했는가?
				pPlayer->SendFieldMsg(ON_ASK_GIVEUP, SO_SFM_ONLYME, LPVOID(uiVillageCode), LPVOID(mnMoney));
			
			m_Guilds[siFoundSlot].SetGiveUpInfo(uiVillageCode, mnMoney, ON_GUILD_GIVEUP_NOT_ANSWER);

			DBIKWarGiveUp           warGiveUp;
			memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

			warGiveUp.uiGuildID     = uiGuildID;
			warGiveUp.uiVillageCode = uiVillageCode;
			warGiveUp.mnMoney       = mnMoney;
			warGiveUp.siFlag        = ON_GUILD_GIVEUP_NOT_ANSWER;

			m_pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP,
				                                  &warGiveUp, sizeof(DBIKWarGiveUp));
			
		}
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.
//	수정 일자	:	2002-02-26 오전 9:27:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::InitStaticVariable(SOWorld *pWorld)
{
	m_pMyWorld	=	pWorld;
	cltGuild::InitStaticVariable(pWorld);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 상단이 꽉 차있는지 본다.
//	수정 일자	:	2002-02-26 오후 5:09:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::IsGuildFull()
{
	if(FindEmptySlot() == -1)	return TRUE;
	else						return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild의 ID를 사용하여 Guild를 얻어온다.
//	수정 일자	:	2002-03-16 오후 3:12:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuild*	cltIGuild::GetGuildByID(UI32 uiGuildID)
{
	SI32	siFoundSlot;		

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return &m_Guilds[siFoundSlot];
	}
	else
		return NULL;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild를 얻어온다.
//	수정 일자	:	2002-03-04 오전 11:45:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuild*	cltIGuild::GetGuildByIndex(SI32 siIndex)
{
	// siIndex의 길드가 유효한지 본다.
	if(m_Guilds[siIndex].IsValidGuild() == FALSE)	return	NULL;
	else	
		return	&m_Guilds[siIndex];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild의 이름을 사용하여 Guild를 얻어온다.
//	수정 일자	:	2002-04-22 오전 10:22:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuild*	cltIGuild::GetGuildByName(CHAR *pszName)
{
	SI32	siFoundSlot;		

	if((siFoundSlot = FindSlot(pszName)) != -1)	
	{
		return &m_Guilds[siFoundSlot];
	}
	else
		return NULL;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild의 이름을 얻어온다.
//	수정 일자	:	2002-02-26 오후 5:09:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	cltIGuild::GetGuildName(UI32 uiGuildID)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].GetGuildName();
	}
	else
		return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild의 ID를 얻어온다.
//	수정 일자	:	2002-03-05 오전 10:59:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltIGuild::GetGuildID(CHAR *pszName)
{
	SI32	siFoundSlot;	

	if(pszName == NULL)	return 0;
		
	if((siFoundSlot = FindSlot(pszName)) != -1)	
	{
		return m_Guilds[siFoundSlot].GetGuildID();
	}
	else
		return NULL;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Char의 이름을 사용하여 Guild ID를 얻어온다. 
//					찾는데 성공했다면 TRUE. 실패했다면 FALSE.
//	수정 일자	:	2002-05-27 오후 2:37:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass)
{	
	SI32			i;

	if(pszName == NULL)	return 0;

	// 메모리에서 얻어온다.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{	
			if(m_Guilds[i].GetGuildIDByCharName(pszName, puiGuildID, psiGuildClass) == TRUE)
				return	TRUE;	
		}
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	직급을 변경시킨다.
//	수정 일자	:	2002-03-06 오후 1:43:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::ChangeClass( UI32 uiGuildID, CHAR *pszName, SI32 siNewClass )
{	
	
	SI32						siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) != -1 )	
	{
		
		return m_Guilds[ siFoundSlot ].ChangeClass( pszName, siNewClass );	
	}	

	return FALSE;


}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	길드원에게 공지사항을 보내준다.
//	수정 일자	:	2002-03-26 오후 3:30:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::Notice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
		return m_Guilds[siFoundSlot].Notice(pszName, pszNotice);	
	else
		return FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::SetRelationByIndex(SI32 siGuildIndex1, SI32 siGuildIndex2, SI32 siRelation, BOOL bInit,SI32 siTime)
{	
	m_Guilds[siGuildIndex1].SetRelation(&m_Guilds[siGuildIndex2], siRelation, bInit, siTime);	
	m_Guilds[siGuildIndex2].SetRelation(&m_Guilds[siGuildIndex1], siRelation, TRUE, siTime);
/*
	switch(siRelation)
	{
	case ON_GUILD_RELATION_ENEMY:		
		// 적으로 적대적인 관계를 설정할 경우 적도 자동으로 나에게 적대적인 관계가 된다.
		m_Guilds[siGuildIndex2].SetRelation(&m_Guilds[siGuildIndex1], siRelation, bInit);	
		break;
	}
*/
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 설정한다.
//	수정 일자	:	2002-03-15 오후 5:16:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::SetRelation(UI32 uiGuildID1, UI32 uiGuildID2, SI32 siRelation, BOOL bInit,SI32 siTime)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		SetRelationByIndex(siFoundSlot1, siFoundSlot2, siRelation, bInit,siTime);
		return	TRUE;
	}
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::SetRelation(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 siRelation, BOOL bInit,SI32 siTime)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, pszGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		SetRelationByIndex(siFoundSlot1, siFoundSlot2, siRelation, bInit,siTime);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetRelation(CHAR *pszGuildID1, UI32 uiGuildID2, SI32 siRelation, BOOL bInit, SI32 siTime)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID2, pszGuildID1, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		SetRelationByIndex(siFoundSlot2, siFoundSlot1, siRelation, bInit,siTime);
		return	TRUE;
	}
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단의 관계를 얻어온다.
//	수정 일자	:	2002-03-15 오후 5:19:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltIGuild::GetRelation(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		return	m_Guilds[siFoundSlot1].GetRelation(UI32(siFoundSlot2));	
	}
	else
		return	0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild의 Index로 ID를 얻어온다.
//	수정 일자	:	2002-03-16 오후 2:10:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltIGuild::GetGuildIDByIndex(SI32 siIndex)
{
	if(m_Guilds[siIndex].IsValidGuild() == TRUE)
		return m_Guilds[siIndex].GetGuildID();
	else
		return	0;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB에서 길드의 관계를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::SetGuildRelationFromDB()
{
	SI32	i, j;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			// 우선 모든 상단간의 관계를 '중립'으로 설정해준다.
			for(j = 0; j < SO_TOTAL_GUILD_NUM; j++)
			{
				if(i != j)
				{
					if(m_Guilds[j].IsValidGuild() == TRUE)
						SetRelationByIndex(i, j, ON_GUILD_RELATION_NEUTRAL, TRUE,0);					
				}
			}			
	
			m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETRELATION, LPVOID(m_Guilds[i].GetGuildID()), LPVOID(this));
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	uiGuildID2가 uiGuildID1의 적인지 알아온다.
//	수정 일자	:	2002-03-19 오후 2:56:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::IsEnemyGuild(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		return	m_Guilds[siFoundSlot1].IsEnemyGuild(siFoundSlot2);
	}
	else
		return	FALSE;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	uiGuildID2가 uiGuildID1의 적인지 알아온다. 서로가 적대관계인지 알아온다.
//	수정 일자	:	2002-03-19 오후 2:56:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::IsEnemyGuildBoth(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		if((m_Guilds[siFoundSlot1].IsEnemyGuild(siFoundSlot2) == TRUE) && (m_Guilds[siFoundSlot2].IsEnemyGuild(siFoundSlot1) == TRUE))
			return	TRUE;
	}
	
	return	FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	적대관계인지알아온다.
//	수정 일자	:	2002-03-21 오후 2:15:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::IsHostilityRelation(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;	

	if(uiGuildID1 == uiGuildID2)	return	FALSE;

	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		if(m_Guilds[siFoundSlot1].IsEnemyGuild(siFoundSlot2) == TRUE ||
			m_Guilds[siFoundSlot2].IsEnemyGuild(siFoundSlot1) == TRUE)
			return	TRUE;	
	}
	
	return	FALSE;	
}




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상관 관계에 관련된 게임 Define을 전송용 Define으로 변경한다.
//	수정 일자	:	2002-03-21 오후 1:55:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltIGuild::RelationDefineToResponseDefine(DWORD dwDefine)
{
	switch(dwDefine)
	{
	case ON_GUILD_RELATION_ALLY:	return ON_RET_GLD_ALLY;
	case ON_GUILD_RELATION_ENEMY:	return ON_RET_GLD_ENEMY;
	case ON_GUILD_RELATION_NEUTRAL:	return ON_RET_GLD_NEUTRAL;
	case ON_GUILD_RELATION_MINE:	return ON_RET_GLD_MINE;
	case ON_GUILD_RELATION_WAR:		return ON_RET_GLD_WAR;
	}

	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전송용 Define을 상관 관계에 관련된 게임 Define으로 변경한다.
//	수정 일자	:	2002-03-21 오후 1:55:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltIGuild::ResponseDefineToRelationDefine(DWORD dwDefine)
{
	switch(dwDefine)
	{
	case ON_RET_GLD_ALLY:		return ON_GUILD_RELATION_ALLY;
	case ON_RET_GLD_ENEMY:		return ON_GUILD_RELATION_ENEMY;
	case ON_RET_GLD_NEUTRAL:	return ON_GUILD_RELATION_NEUTRAL;
	case ON_RET_GLD_MINE:		return ON_GUILD_RELATION_MINE;
	case ON_RET_GLD_WAR:		return ON_GUILD_RELATION_WAR;
	}

	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	행수를 지정한다.
//	수정 일자	:	2002-03-21 오후 3:39:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::SetHNS(SOVillage *pVillage)
{
	SI32	siFoundSlot;

	// GuildID가 유효한지 본다.
	if(pVillage->uiGuildID == 0)							return	FALSE;
	if((siFoundSlot = FindSlot(pVillage->uiGuildID)) == -1)	return	FALSE;

	// 상단 Class에 행수를 설정한다.
	m_Guilds[siFoundSlot].SetHNS(pVillage);
	return	TRUE;
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 상단이 보유할 수 있는 최대 상단원 수를 얻어온다.
//	수정 일자	:	2002-03-29 오후 1:56:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltIGuild::GetMaxGuildMemebersNum(UI32 uiGuildID)
{	
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetMaxGuildMemebersNum();
}
*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	깃발을 얻어온다.
//	수정 일자	:	2002-04-04 오후 1:38:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltIGuild::GetFlag(UI32 uiGuildID)
{
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetFlag();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	깃발을 설정한다.
//	수정 일자	:	2002-04-04 오후 1:38:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::SetFlag(UI32 uiGuildID, SI32 siFlag)
{
	SI32			siFoundSlot;	
	//SI32			siFlagIDInStack;
	strFlagInfo		stFlagInfo;	

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;	

	// 지워야 하는 플래그의 정보를 설정한다.
	stFlagInfo.uiGuildID		=	uiGuildID;
	stFlagInfo.siFlag			=	m_Guilds[siFoundSlot].GetFlag();

	//siFlagIDInStack	=	m_Guilds[siFoundSlot].GetFlagIDInStack();
	
	if(m_Guilds[siFoundSlot].ChangeFlag(siFlag) == TRUE)
	{		
		if(stFlagInfo.siFlag != -1)
			m_Flags.DeleteType(stFlagInfo);		

		stFlagInfo.uiGuildID	=	uiGuildID;
		stFlagInfo.siFlag		=	siFlag;
		m_Guilds[siFoundSlot].SetFlagIDInStack(m_Flags.Add(stFlagInfo));

		return	TRUE;
	}
	else
	{
		return	FALSE;
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	사용된 깃발의 리스트를 얻어온다.
//	수정 일자	:	2002-04-04 오후 2:41:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::GetUsedFlagList(SI32 *psiFlagNum, strFlagInfo *pstFlagInfo)
{
	*psiFlagNum	=	m_Flags.m_NowUseDataCount;		

	memcpy(pstFlagInfo, m_Flags.m_Data, m_Flags.m_NowUseDataCount * sizeof(strFlagInfo));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	사용하는 깃발인지 알아온다.
//	수정 일자	:	2002-04-11 오후 5:00:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::IsUsedFlag(SI32 siFlag)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(m_Guilds[i].GetFlag() == siFlag)
				return	TRUE;
		}
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	사용하는 이름인지 알아온다.
//	수정 일자	:	2003-01-7 오후 5:00:43 - 박준태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

BOOL	cltIGuild::IsUsedName(CHAR *szName)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(!strcmp(m_Guilds[i].GetGuildName(),szName))
				return	TRUE;
		}
	}

	return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB로 부터 모든 상단원의 정보를 얻어온다.
//	수정 일자	:	2002-04-22 오후 5:07:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::SetMemberInfoFromDB()
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{	
			if (m_Guilds[i].SetMemberInfoFromDB() == FALSE)
			{
				
				return FALSE;			
			}

		}
	}
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 상단의 현재 정식 회원 수를 얻어온다.
//	수정 일자	:	2002-05-09 오후 2:58:20 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltIGuild::GetGuildMemberNum(UI32 uiGuildID)
{
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetGuildMemberNum();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	접속해 있는 상단원에게 메시지를 보내준다.
//	수정 일자	:	2002-12-29 오후 7:14:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::SendMsgToGuildMemebers( UI32 uiGuildID, SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5 )
{
	SI32	siFoundSlot;

	if( (siFoundSlot = FindSlot( uiGuildID ) ) == -1 )	return;

	m_Guilds[ siFoundSlot ].SendMsgToGuildMemebers( siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5 );
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단에 소속되는 마을을 알려준다.
//	수정 일자	:	2002-02-27 오후 3:53:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltIGuild::AddVillage(UI32 uiGuildID, UI16 uiVillageCode)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].AddVillage(uiVillageCode);
	}
}*/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Guild의 ID로 Index를 얻어온다.
//	수정 일자	:	2003-01-01 오전 7:07:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltIGuild::GetGuildIndexByID( UI32 uiGuildID )
{
	return	FindSlot( uiGuildID );	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	
//	수정 일자	:	2003-01-01 오전 7:10:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//BOOL	cltIGuild::RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode,SI16 siRewardSupply)
BOOL	cltIGuild::RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode)
{
	cltGuild	*pGuild;

	if( ( pGuild = GetGuildByID( uiGuildID ) ) == NULL )			return FALSE;
	
//	pGuild->RewardToGuildDAIForDestroyVillage( mnReward, uiVillageCode ,siRewardSupply);
	pGuild->RewardToGuildDAIForDestroyVillage( mnReward, uiVillageCode );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 Guild Index 인지 검사한다.
//	수정 일자	:	2003-01-01 오전 7:11:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltIGuild::IsValidGuildIndex( SI32 siGuildIndex )
{
	if( ( siGuildIndex >= 0 ) && ( siGuildIndex < SO_TOTAL_GUILD_NUM ) )		return	TRUE;

	return	FALSE;
}

BOOL	cltIGuild::SetMaxGuildID()
{
	m_uiMaxGuildID = 0;

	for(UI32 i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if (m_uiMaxGuildID < m_Guilds[i].GetGuildID())
			{
				m_uiMaxGuildID = m_Guilds[i].GetGuildID();
			}
		}
	}

	return TRUE;
	
}

VOID	cltIGuild::GuildMemberLogin( UI32 uiGuildID )
{
	SI32	siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) == -1 )			return;
	
	m_Guilds[ siFoundSlot ].GuildMemberLogin();
}
	


VOID	cltIGuild::GuildMemberLogout( UI32 uiGuildID )
{
	SI32	siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) == -1 )			return;
	
	m_Guilds[ siFoundSlot ].GuildMemberLogout();
}

//12시에 한번씩 날려서 상단 체크 하게끔 만든다.
void	cltIGuild::SetRelationReLoad()
{
	printf("상단관계 재설정 시작 ");
	
	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM ; i++)
	{
		m_Guilds[i].SetRelationReLoad();
	}

	printf("상단관계 재설정 끝\n");
}

// 공성하는 날인가를 체크 해주면 된다.
// 캐릭터 방어나 공격에 쓴다.
BOOL	cltIGuild::IsEnemyRelationWar(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;	

	if(uiGuildID1 == uiGuildID2)	return	FALSE;

	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		if(m_Guilds[siFoundSlot1].IsWarGuild(siFoundSlot2) == TRUE ||
			m_Guilds[siFoundSlot2].IsWarGuild(siFoundSlot1) == TRUE)
			return	TRUE;	
	}
	
	return	FALSE;
}

BOOL	cltIGuild::SetMessage(UI32 uiGuild1,UI32 uiGuild2,CHAR* pszMsg)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuild1, uiGuild2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetMessage(&m_Guilds[siFoundSlot2],pszMsg);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetFriendlyMessage(UI32 uiGuild,CHAR* pszGuildid2,CHAR* pszMsg)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuild, pszGuildid2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetFriendlyMessage(&m_Guilds[siFoundSlot2],pszMsg);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetFriendlyMessageAccept(UI32 uiGuild,CHAR* pszGuildid2)
{
	SI32	siFoundSlot1, siFoundSlot2;

	if(FindSlot(uiGuild, pszGuildid2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetFriendlyMessageAccept(&m_Guilds[siFoundSlot2]);
		m_Guilds[siFoundSlot1].SetWarFlag(FALSE);
		m_Guilds[siFoundSlot2].SetWarFlag(FALSE);

		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetFriendlyMessageCancel(UI32 uiGuild,CHAR* pszGuildid2)
{
	SI32	siFoundSlot1, siFoundSlot2;

	if(FindSlot(uiGuild, pszGuildid2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetFriendlyMessageCancel(&m_Guilds[siFoundSlot2]);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetPowerInvestmentEnd()
{
	for(SI16 i = 0; i< SO_TOTAL_GUILD_NUM ; i++)
	{
		if(m_Guilds[i].GetGuildID() != 0)
		{
			m_Guilds[i].SetPowerInvestmentEnd();
		}
	}
	return TRUE;
}

BOOL	cltIGuild::GetGuildWarCheck(UI32 uiGuildID)
{
	SI32	siFoundSlot1;

	siFoundSlot1 = FindSlot(uiGuildID);

	for(SI16 i = 0 ; i < SO_TOTAL_GUILD_NUM ; i++)
	{
		if(m_Guilds[siFoundSlot1].m_siGuildRelation[i] == ON_GUILD_RELATION_WAR || 
			m_Guilds[siFoundSlot1].m_siGuildRelation[i] == ON_GUILD_RELATION_ENEMY)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//공성 시간이 되면 상단원에게 공성 시간이 되었다고 알려준다.
void   cltIGuild::SendWarTime()
{
	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{ 
		if(m_Guilds[i].GetGuildID() != 0)
		{
			if(m_Guilds[i].IsWar() == TRUE)
				m_Guilds[i].SendWarTime();
		}
	}
}

//상단에 가지고있는 마을을 검사한다.
SI32 cltIGuild::CheckVillage(UI16 uiVillageCode)
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() != 0)
		{
			if(m_Guilds[i].CheckVillage(uiVillageCode))
				return i;
		}
	}

	return -1;
}

//공성시간이 끝나면 공성포기 정보를 초기화 한다.
VOID cltIGuild::InitGiveUpInfo()
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGiveUpFlag() != ON_GUILD_GIVEUP_NORMAL)
		{
			DBIKWarGiveUp           warGiveUp;
			memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

			warGiveUp.uiGuildID     = m_Guilds[i].GetGuildID();
			warGiveUp.siFlag        = ON_GUILD_GIVEUP_NORMAL;

			m_pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP, &warGiveUp, sizeof(DBIKWarGiveUp));			

			m_Guilds[i].SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
		}
	}
}

BOOL cltIGuild::GiveUp(UI32 uiMyGuildID)
{
	if(uiMyGuildID < 0) return FALSE;

	int nSlot = FindSlot(uiMyGuildID);
	if(nSlot < 0) return FALSE;
	
	m_Guilds[nSlot].GiveUp();

	//다른상단의 정보도 수정해준다.
	UI32 uiOterGuildID;
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		uiOterGuildID = m_Guilds[i].GetGuildID();
		if(uiOterGuildID < 0) continue;
		if(uiMyGuildID == uiOterGuildID) continue;

		if(GetRelation(uiOterGuildID, uiMyGuildID) == ON_GUILD_RELATION_WAR)
		{
			SetRelationByIndex(nSlot, i, ON_GUILD_RELATION_ENEMY, FALSE, 0);
		}
	}

	return TRUE;
}

//DB에서 공성 포기관련 정보를 가져온다.
VOID cltIGuild::DBGetGiveUpInfo()
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		UI32 uiGuilD = m_Guilds[i].GetGuildID();
		DBIKWarGiveUp        warGiveUp;
		memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));
		m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_WARGIVEUP, &uiGuilD, &warGiveUp, NULL);

		if(warGiveUp.mnMoney > 0)  
			m_Guilds[i].SetGiveUpInfo(warGiveUp.uiVillageCode, warGiveUp.mnMoney, warGiveUp.siFlag);
		else //공성 포기 비용이 없는경우 초기화를 한다.
			m_Guilds[i].SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
	}
}