// 熱薑 橾濠 : 2002-02-18 螃瞪 11:28:08 - 曄薑賅

#include "SOMain.h"
#include "SOWorld.h"
#include "Guild.h"

SOWorld*	cltGuild::m_pMyWorld	=	NULL;
	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-04-13 螃�� 6:08:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltGuild::cltGuild()
{
	Init();			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-04-13 螃�� 6:08:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltGuild::~cltGuild()
{
	m_Villages.Release();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	и廓虜 蟾晦�� п還匙菟擊 п遽棻.
//	熱薑 橾濠	:	2002-04-13 螃�� 6:09:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::OnceInit(SI32 siTotalVillageNum)
{
	m_Villages.Create( siTotalVillageNum );	

	m_GuildMemberManager.OnceInit();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詹幗 滲熱菟擊 蟾晦�倆挬�.
//	熱薑 橾濠	:	2002-03-29 螃�� 1:51:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 鼻欽錳 婦葬濠蒂 蟾晦�� и棻.
	m_GuildMemberManager.Init();

	m_siLoginedGuildMemeberNum	=	0;

	SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 儅撩и棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 7:27:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::CreateGuild(UI32 uiArrayIndex, UI32 uiGuildID, CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage)
{
	// 鼻欽錳 婦葬濠蒂 蟾晦�� и棻.
	m_GuildMemberManager.Init();

	m_uiGuildID		=	uiGuildID;

	strncpy(m_szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);	
	strncpy(m_szDAIID, pPlayer->szID, ON_ID_LENGTH);
	ZeroMemory(m_siGuildRelation, sizeof(m_siGuildRelation));		

	m_uiArrayIndex						=	uiArrayIndex;		// 寡翮 檣策蝶蒂 撲薑и棻.
	m_siFlagIDInStack					=	-1;

	//奢撩 嶸棉 葬蝶お蒂 蟾晦�� п遽棻.
	m_GuildUnit.InitGuildUnit();

	// 葬蝶お縑 餌辨濠蒂 蹺陛и棻.
	m_GuildMemberManager.AddMember(pPlayer->szID, pPlayer->siClassInGuild, pPlayer->GetMoney() + pPlayer->GetSaving(), 
								pPlayer->IFollower.GetFollowerParameterLevel(0), pPlayer->uiTradeCredit,pPlayer->stAccountInfo.siDBAccount,pPlayer->stAccountInfo.uiSlot);		
	
	// 葆擊擊 蹺陛 衛鑑棻.
	// 蟾晦�� 睡碟擎 "FALSE"蒂 邀戲煎賦
	// 斜 葆擊縑 譆堅癱濠濠蒂 ч熱煎 撲薑ж啪 п遽棻.
	AddVillage(pVillage, FALSE);

	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 撲薑и棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 7:29:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::InitGuild(UI32 uiArrayIndex, UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, UI32 uiGuildMemeberNum, SI32 siFlag, SI32 siFlagIDInStack)
{
	m_uiGuildID		=	uiGuildID;

	strncpy(m_szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);	
	strncpy(m_szDAIID, pszDAIID, ON_ID_LENGTH);	
	m_uiArrayIndex				=	uiArrayIndex;		// 寡翮 檣策蝶蒂 撲薑и棻.		
	m_siFlag					=	siFlag;				// 梓嫦.
	m_siFlagIDInStack			=	siFlagIDInStack;		

	// 檜 鼻欽縑 菟橫螢 熱 氈朝 鼻欽錳曖 熱蒂 撲薑и棻.
	//UpdateMaxRealMemberNum();	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB煎 睡攪 賅萇 鼻欽錳曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃�� 5:03:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::SetMemberInfoFromDB()
{
	m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_RESULT, LPVOID(m_uiGuildID), LPVOID(this));
	// 鼻欽錳菟擊 DB縑憮 檗橫諦陛雖堅 m_GuildMemberManager縑 撮たи棻.
	return m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETGUILDMEMBER, LPVOID(m_uiGuildID), LPVOID(&m_GuildMemberManager));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 餉薯и棻.
//	熱薑 橾濠	:	2002-03-05 螃瞪 11:47:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::DeleteGuild()
{
	SI32		i;	
	BOOL		bDBSuccess;
	
	// DB縑憮 Guild蒂 餉薯и棻.		
	bDBSuccess = TRUE;
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_DELETE, &m_uiGuildID, sizeof(m_uiGuildID) ); //**

	m_GuildMemberManager.Init();

	// ⑷營 檜 鼻欽縑 樓п 氈朝 葆擊擊 賅舒 鼻欽縑 樓ж雖 彊擎 鼻鷓煎 п遽棻.
	for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
	{
		if(m_pMyWorld->pclVillage[i].uiGuildID == m_uiGuildID)
			DelVillage(&m_pMyWorld->pclVillage[i], TRUE);
	}

	// for debug
//	m_pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Clan was disorganized!!    Guild Name: %s    GuildID: %d", m_szGuildName, m_uiGuildID );
	writeInfoToFile( "DeletedGuild.txt", "Clan was disorganized!!    Guild Name: %s    GuildID: %d", m_szGuildName, m_uiGuildID );
	// ⑷營 檜 鼻欽縑 樓п 氈朝 Ы溯檜橫菟擊 賅舒 鼻欽縑 樓ж雖 彊擎 鼻鷓煎 п遽棻.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			if(m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID)
			{
				m_pMyWorld->clPlayer[i]->SetGuild(0);
				m_pMyWorld->clPlayer[i]->SetClassInGuild(0);

				// 詭衛雖蒂 爾頂遽棻.
				m_pMyWorld->clPlayer[i]->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_DELETEGLD));
			}
		}
	}

	// 滲熱菟擊 蟾晦�� и棻.
	Init();

	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltGuild::GetGuildID()
{
	return m_uiGuildID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼻欽曖 檜葷擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	cltGuild::GetGuildName()
{
	return m_szGuildName;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	渠寞曖 ID(僥濠翮)擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-04 螃�� 12:33:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	cltGuild::GetDAIID()
{
	return m_szDAIID;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模嶸ж堅 氈朝 葆擊曖 熱蒂 剩啖遽棻.
//	熱薑 橾濠	:	2002-03-04 螃�� 12:34:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	cltGuild::GetOwnerVillageNum()
{
	return m_Villages.m_NowUseDataCount;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詹幗 �蛾虃鷏� 剩啖遽棻.
//	熱薑 橾濠	:	2002-04-19 螃�� 3:11:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltGuild::GetGuildMemberNum()
{
	return m_GuildMemberManager.GetRealMemeberNum();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 薑衝 鼻欽錳檜 蹺陛腎歷棻.
//	熱薑 橾濠	:	2002-05-10 螃�� 9:44:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::AddGuildMember(CHAR *pszName)
{	
	
	SI32	i;
	CHAR	*pszMaxInvestmentorName;
	BOOL	IsSendChangeClassMsg = FALSE;	
	SI32	siClassInCode;

	strCandidateInfo *pInfo = NULL;

	// 鼻欽錳縑 蹺陛衛酈溥朝 Ы溯檜橫陛 雖錳濠檣雖 匐餌и棻.
	if( (pInfo = m_GuildMemberManager.GetCandidateMember( pszName )) == NULL )
	{
//		m_pMyWorld->clServer->WriteInfo( "..\\AddGuildMember Failed.txt", "GuildID : [ %u ], Name : [ %s ]", m_uiGuildID, pszName );
		writeInfoToFile( "AddGuildMember Failed.txt", "GuildID : [ %u ], Name : [ %s ]", m_uiGuildID, pszName );
		return	FALSE;
	}


	// 霜晝擎 晦獄瞳戲煎 餌�素抴�.		
	siClassInCode	=	m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);

	// 渦 檜鼻 陛殮й 熱 橈棻賊 FALSE蒂 葬欐и棻.
	if(m_GuildMemberManager.IsRealMemberFull() == TRUE)	return FALSE;	

	// ⑷營 陛殮ж溥朝 議葛攪陛 棻艇 葆擊曖 譆堅 癱濠濠檜塭賊 斜 葆擊擊 んл衛鑑棻.
	for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
	{
		pszMaxInvestmentorName	=	m_pMyWorld->pclVillage[i].m_Government->GetInvestmentMaxInvestmentorName();
			
		if(pszMaxInvestmentorName != NULL)
		{
			if(stricmp(pszMaxInvestmentorName, pszName) == 0)
			{
				// 譆堅 癱濠濠檜棻.
				// п渡 葆擊擊 蹺陛 衛鑑棻.
				AddVillage(&m_pMyWorld->pclVillage[i], FALSE);								
				//IsSendChangeClassMsg	=	TRUE;
		
				siClassInCode			=	m_pMyWorld->clIGuildClass.GetHNSCode();
			}
		}
	}
	
	// 霜晝 滲唳檜 腎歷棻朝勘 爾頂鍍棻賊 棻衛 爾頂 輿雖 彊嬴紫 脹棻.
	if(IsSendChangeClassMsg == FALSE)
	{
		if( ChangeClass( pInfo->szName, siClassInCode ) == FALSE )	
			return	FALSE;			
	}

	// 雖錳濠蒂 薑衝 鼻欽錳戲煎 夥脯遽棻.
	m_GuildMemberManager.ChangeClass(pszName, siClassInCode, TRUE);	

	// ⑷營 п渡 餌辨濠陛 煎斜檣醞檜塭賊 蕾樓и 鼻欽錳熱蒂 隸陛и棻.
	if( m_pMyWorld->GetPlayer( pszName ) != NULL )
	{
		GuildMemberLogin();		
	}

	// 賅萇 鼻欽錳縑啪 鼻欽 薑爾陛 滲唳腎歷棻朝 匙擊 憲溥遽棻.(MS)
	GuildInfoChanged();

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑憮 餉薯腎歷棻.
//	熱薑 橾濠	:	2002-04-13 螃瞪 10:03:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::DelGuildMember(CHAR *pszName, BOOL bCandidate, BOOL bSendMsg, BOOL bKick)
{
	DBIKGuildLeaveInfo			GuildLeaveInfo;	
	SOPlayer					*pPlayer;			

	strGuildCommonMemberInfo    MemberInfo;
	
	// п渡 Ы溯檜橫陛 檜 鼻欽錳檣雖 憲嬴褥婁 翕衛縑 薑爾蒂 橢橫螞棻.
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

	// 鼻欽錳 婦葬 贗楚蝶縑啪紫 憲溥遽棻.
	m_GuildMemberManager.DelMember(pszName, bCandidate);	


	// 葆擊縑憮 斜 議葛攪曖 霜嬪陛 氈棻賊 п薯и棻.
	// 雖錳濠陛 嬴棍 唳辦縑虜 熱чп遽棻.
	if(bCandidate == FALSE) m_pMyWorld->DeleteGuildClassInVillage(pszName);	

	

	// ⑷營 Ы溯檜橫陛 蕾樓腎橫 氈棻賊 Ы溯檜橫曖 鼻欽擊 橈紫煙 и棻.
	if((pPlayer = m_pMyWorld->GetPlayer(pszName)) != NULL)
	{
		// ⑷營 撲薑脹 鼻欽檜 橈紫煙 и棻.
		pPlayer->SetGuild(0);	

		if(bSendMsg == TRUE)
			pPlayer->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_DISMISSAL));		

		// 蕾樓脹 鼻欽錳熱縑憮 馬模и棻.
		GuildMemberLogout();
	}

	// 賅萇 鼻欽錳縑啪 鼻欽 薑爾陛 滲唳腎歷棻朝 匙擊 憲溥遽棻.(MS)
	GuildInfoChanged();

	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 葆擊檜 蹺陛腎歷棻.
//	熱薑 橾濠	:	2002-05-11 螃瞪 10:17:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::AddVillage(SOVillage *pVillage, BOOL bInit)
{	
	// 蟾晦�俯萺� 嬴棲罹撿 и棻.
	if(bInit == FALSE)
	{
		// 蟾晦�俯萺� 嬴棍陽縑朝 ⑷營 蹺陛ж溥朝 葆擊檜 檜嘐 鼻欽縑 陛殮п 氈朝啪 嬴棋雖 �挫恉挬�.

		// 鼻欽 ID陛 憮煎 偽棻賊 檜嘐 陛殮и 匙檜棻.
		// 檜 睡碟擎 奩萄衛 嬴楚 pVillage->SetOwnerGuild(m_uiGuildID) ж晦 瞪縑 п輿橫撿 и棻.
		if(pVillage->uiGuildID == m_uiGuildID)	return;

		// 葆擊擊 蹺陛й陽 ч熱紫 偽檜 雖薑п遽棻.	
		pVillage->SetOwnerGuild(m_uiGuildID);
		// だ惚脹 葆擊擊 50%曖 Я蒂 瓣錶 蹺陛и棻.		
		pVillage->AutoRestorationVillageDefence(TRUE);
	}

	// 葆擊擊 模樓衛鑑棻.
	m_Villages.Add(pVillage->uiVillageCode);

	// 譆渠 鼻欽錳曖 熱蒂 撲薑и棻.
	//UpdateMaxRealMemberNum();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑憮 葆擊檜 餉薯腎歷棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 8:00:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::DelVillage(SOVillage *pVillage, BOOL bDeleteGuild)
{
	pVillage->DeleteGuild();	

	// 模樓脹 葆擊縑憮 薯剪и棻.
	m_Villages.DeleteType(pVillage->uiVillageCode);

	// 譆渠 鼻欽錳曖 熱蒂 撲薑и棻.
	//UpdateMaxRealMemberNum();

	// ⑷營 望萄蒂 餉薯醞檜賊 
	// 鼻欽擊 橈擁晦 嬪ж罹 葆擊 熱蒂 羹觼ж雖 彊朝棻.
	if(bDeleteGuild == FALSE)
	{
		// 渦 檜鼻 模嶸ж堅 氈朝 葆擊檜 橈棻賊
		// 望萄蒂 餉薯и棻.
		if(m_Villages.m_NowUseDataCount <= 0)
		{
			// 鼻欽擊 橈撻棻.		
			return	TRUE;	
		}
	}	

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	薑瞳 滲熱蒂 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::InitStaticVariable(SOWorld *pWorld)
{
	m_pMyWorld	=	pWorld;
	
	GuildMemberManager::InitStaticVariable( &(m_pMyWorld->clIGuildClass) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 望萄檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:17:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::IsValidGuild()
{
	if(m_uiGuildID != 0)		return TRUE;
	else						return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢雖餌о擊 爾頂遽棻.
//	熱薑 橾濠	:	2002-03-26 螃�� 3:30:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::Notice(CHAR *pszName, CHAR *pszNotice)
{	
	CHAR	szNotice[1024];

	if(pszNotice == NULL)	return FALSE;

	// 鼻欽檜葷.
	sprintf(szNotice, "[%s] : %s", pszName, pszNotice);

	// ⑷營 檜 鼻欽縑 樓п 氈朝 Ы溯檜橫菟縑啪 詭衛雖蒂 爾頂遽棻.
	SendMsgToGuildMemebers(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_GUILD), LPVOID(szNotice));	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蕾樓п 氈朝 鼻欽錳縑啪 詭衛雖蒂 爾頂遽棻.
//	熱薑 橾濠	:	2002-03-16 螃瞪 10:32:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::SendMsgToGuildMemebers(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5)
{
	SI32		i;		

	// ⑷營 檜 鼻欽縑 樓п 氈朝 Ы溯檜橫菟縑啪 詭衛雖蒂 爾頂遽棻.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			// п渡 望萄錳檜堅 
			// 雖錳濠陛 嬴棲罹撿 и棻.
			if((m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID) && 
				(m_pMyWorld->clPlayer[i]->siClassInGuild != m_pMyWorld->clIGuildClass.GetJWJCode()))			
				m_pMyWorld->clPlayer[i]->SendFieldMsg(siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5);			
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寡翮 檣策蝶蒂 橢橫褥.
//	熱薑 橾濠	:	2002-03-16 螃�� 2:49:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltGuild::GetArrayIndex()
{
	return	m_uiArrayIndex;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ч熱蒂 雖薑и棻.
//	熱薑 橾濠	:	2002-03-21 螃�� 3:36:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::SetHNS(SOVillage *pVillage)
{	
	CHAR	*pszHNSID;
	
	// 葆擊曖 ん檣攪陛 嶸�褲捘� 憲嬴螞棻.
	if(pVillage == NULL)	return	FALSE;

	// 譆堅 癱濠濠曖 檜葷擊 橢橫螞棻.
	if((pszHNSID = pVillage->m_Government->GetInvestmentMaxInvestmentorName()) == NULL)	return	FALSE;


	
	// ч熱蒂 雖薑и棻.
	// ч熱曖 檜葷擊 釭醞擊 嬪ж罹 雖旎 雖薑п遽棻.
	pVillage->SetHNSID(pszHNSID);

	return	TRUE;
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	爾嶸ж堅 氈朝 葆擊曖 熱蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-03-29 螃�� 1:48:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::UpdateMaxRealMemberNum()
{		
	// 葆擊曖 譆渠 鼻欽錳熱蒂 薑и棻.
	m_GuildMemberManager.SetMaxRealMemberNum(min(ON_GUILD_MIN_MEMBER_NUM + ((m_Villages.m_NowUseDataCount - 1) * ON_GUILD_ADD_MEMBER_NUM_PER_ONEVILLAGE), ON_GUILD_MAX_MEMBER_NUM));	
}
*/

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 鼻欽檜 爾嶸й 熱 氈朝 譆渠 鼻欽錳 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-29 螃�� 1:53:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltGuild::GetMaxGuildMemebersNum()
{
	return m_GuildMemberManager.GetMaxRealMemberNum();
}
*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	梓嫦擊 撲薑и棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 3:26:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::ChangeFlag(SI32 siFlag)
{
	BOOL			bDBSuccess;
	SI32			i;
	

	// п渡 Ы楚斜蒂 棻艇 鼻欽縑憮 噙堅 氈朝雖 匐餌и棻
	if(m_pMyWorld->clIGuild.IsUsedFlag(siFlag) == TRUE)	return	FALSE;

	m_siFlag	=	siFlag;

	

	bDBSuccess = TRUE;
	

	if(bDBSuccess)
	{
		// 檜 鼻欽縑 模樓脹 賅萇 Ы溯檜橫曖 梓嫦擊 滲唳и棻.
		for(i = 0; i < ON_MAX_CONNECTION; i++)
		{
			if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
			{
				// п渡 望萄錳檜堅 
				// 雖錳濠陛 嬴棲罹撿 и棻.
				if((m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID) && (m_pMyWorld->clPlayer[i]->siClassInGuild != m_pMyWorld->clIGuildClass.GetJWJCode()))			
					m_pMyWorld->clPlayer[i]->ChangeGuildFlag(siFlag);					
			}
		}		

		// 檜 鼻欽縑 模樓脹 賅萇 葆擊曖 梓嫦擊 滲唳и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	梓嫦擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-04 螃瞪 11:48:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltGuild::GetFlag()
{	
	return	m_siFlag;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝶鷗縑憮曖 梓嫦 ID蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 2:59:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::SetFlagIDInStack(SI32 siFlagIDInStack)
{
	m_siFlagIDInStack	=	siFlagIDInStack;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝶鷗縑憮曖 梓嫦 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 2:36:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltGuild::GetFlagIDInStack()
{
	return m_siFlagIDInStack;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雖錳濠蒂 蹺陛и棻.
//	熱薑 橾濠	:	2002-04-13 螃瞪 9:37:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::AddCandidate(SOPlayer *pPlayer)
{	
	DBIKGuildJoinInfo			GuildJoinInfo;
	SI32						siLevel;		
	
	// DB縑 盪濰и棻.	
	ZeroMemory(&GuildJoinInfo, sizeof(GuildJoinInfo));
	GuildJoinInfo.uiGuildID		=	m_uiGuildID;	
	GuildJoinInfo.uiClass		=	m_pMyWorld->clIGuildClass.GetJWJCode();				

	GuildJoinInfo.siAccount     =   pPlayer->stAccountInfo.siDBAccount;
	GuildJoinInfo.uiSlot        =   pPlayer->stAccountInfo.uiSlot;

	
	siLevel		=	pPlayer->IFollower.GetFollowerParameterLevel(0);
	
	// 鼻欽錳 婦葬 贗楚蝶縑啪紫 憲溥遽棻.
	if(m_GuildMemberManager.AddMember(pPlayer->szID,GuildJoinInfo.uiClass, pPlayer->GetMoney() + pPlayer->GetSaving(),siLevel, pPlayer->uiTradeCredit,pPlayer->stAccountInfo.siDBAccount,pPlayer->stAccountInfo.uiSlot) == TRUE)
	{
		// Ы溯檜橫曖 鼻欽擊 撲薑и棻.
		pPlayer->SetGuild( m_uiGuildID );
		pPlayer->SetClassInGuild( GuildJoinInfo.uiClass );

		// 啪歜 憮幗縑憮 撩奢瞳戲煎 盪濰 ц擊 唳辦縑虜 DB縑 盪濰и棻.
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_JOIN, &GuildJoinInfo, sizeof(GuildJoinInfo));

		return	TRUE;
	}
	else
		return	FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝擊 滲唳衛鑑棻.
//	熱薑 橾濠	:	2002-04-13 螃瞪 9:58:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::ChangeClass( CHAR *pszName, SI32 siNewClass )
{
	DBIKGuildChangeClassInfo	GuildChangeClassInfo;	
	SOPlayer					*pPlayer = NULL;

	// 薑蜓 望萄 詹幗檣雖 憲嬴螞棻.
	strGuildCommonMemberInfo MemberInfo;

	if( m_GuildMemberManager.GetGuildMemberByCharName(pszName, &MemberInfo) == FALSE )
	{
		return FALSE;
	}

	// by semarine
	m_GuildMemberManager.ChangeClass( pszName, siNewClass, FALSE );
	
	// DB縑 盪濰и棻.
	GuildChangeClassInfo.siDBAccount	=	MemberInfo.siAccount;
	GuildChangeClassInfo.uiSlot			=	MemberInfo.uiSlot;

	GuildChangeClassInfo.uiGuildID		=	m_uiGuildID;		
	GuildChangeClassInfo.uiChangeClass	=	siNewClass;

	
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_CHANGE_CLASS, &GuildChangeClassInfo, sizeof(GuildChangeClassInfo) );

	// ⑷營 蕾樓腎橫 氈朝雖 憲嬴螞棻	
	// ⑷營 蕾樓腎橫 氈棻賊 憲溥遽棻.
	if( (pPlayer = m_pMyWorld->GetPlayerByDBAccount(MemberInfo.siAccount,MemberInfo.uiSlot)) != NULL)
	{
		// 億煎遴 霜晝戲煎 滲唳п遽棻.
		pPlayer->SetClassInGuild( siNewClass );
		// 詭衛雖蒂 爾頂遽棻.
		pPlayer->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_CHANGECLASS), LPVOID(siNewClass));	
	}

	// 賅萇 鼻欽錳縑啪 鼻欽 薑爾陛 滲唳腎歷棻朝 匙擊 憲溥遽棻.(MS)
	GuildInfoChanged();
	
	return	TRUE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 檜 葆擊縑 模樓脹 葆擊曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-13 螃�� 6:32:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::GetOwnerVillages(UI16 *puiOwnerVillageNum, UI16 *puiVillageCode)
{
	*puiOwnerVillageNum	=	m_Villages.m_NowUseDataCount;	
	memcpy(puiVillageCode, m_Villages.m_Data, sizeof(UI16) * m_Villages.m_NowUseDataCount);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 檜 葆擊縑 模樓脹 葆擊曖 晦獄 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-11-27 螃�� 2:45:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑衝 鼻欽錳曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 11:36:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltGuild::GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo)
{
	return	m_GuildMemberManager.GetRMList(dwPage, pGuildMemeberInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雖錳濠 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃�� 2:32:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltGuild::GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo)
{
	return	m_GuildMemberManager.GetCMList(dwPage, pCandidateInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪羹 薑衝 鼻欽錳擊 �飛橦� 爾罹輿晦 嬪п в蹂и む檜雖熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃�� 2:31:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	cltGuild::GetRMTotalPageNum()
{
	return m_GuildMemberManager.GetRMTotalPageNum();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪羹 雖錳濠蒂 �飛橦� 爾罹輿晦 嬪п в蹂и む檜雖熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃�� 2:31:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	cltGuild::GetCMTotalPageNum()
{
	return m_GuildMemberManager.GetCMTotalPageNum();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪 檜葷擊 餌辨ж罹 鼻欽曖 ID諦 望萄 霜晝擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-27 螃�� 2:40:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 陝 Player 檣蝶欐蝶縑 鼻欽 薑爾 滲唳擊 憲溥遽棻.
//	熱薑 橾濠	:	2004-01-05 PM __Lee_min_su__
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

			// 陝 鼻欽曖 Player 檣蝶欐蝶縑啪 鼻欽薑爾陛 滲唳腎歷擠擊 
			// 憲萼棻.
			pPlayer->SetGuildInfoChanged();
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽 薑爾蒂 橢朝棻.
//	熱薑 橾濠	:	2004-01-05 PM __Lee_min_su__
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
			// 餌�素� 薯諼и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-03-16 螃�� 2:49:32 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltGuild::InitRelation(UI32 uiArrayIndex)
{
	// 雖旎擎 欽雖 0戲煎 撲薑虜 ж賊 脹棻.
	m_siGuildRelation[uiArrayIndex]	=	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞳渠瞳檣 婦啗檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-19 螃�� 3:00:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::IsEnemyGuild(UI32 uiArrayIndex)
{
	if(m_siGuildRelation[uiArrayIndex] == ON_GUILD_RELATION_ENEMY)
		return	TRUE;
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢撩檜 陛棟и 鼻鷓(瞳渠 鼻欽擊 勒雖 3橾檜 雖陪 ж)檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-19 螃�� 3:00:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltGuild::IsWarGuild(UI32 uiArrayIndex)
{
	if(m_siGuildRelation[uiArrayIndex] == ON_GUILD_RELATION_WAR)
		return TRUE;
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-03-15 螃�� 5:17:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 翕裊措戲煎 撲薑ц棻.						
		GuildRelation.siRelation	=	ON_GUILD_RELATION_ALLY;	
		break;
	case ON_GUILD_RELATION_ENEMY:
		{
			// 瞳渠措戲煎 撲薑ц棻.	
			// 瞳渠蒂 勒 陳瞼蒂 盪濰п憮 棻擠輿縑 避檣棻.
			GuildRelation.siRelation	=	ON_GUILD_RELATION_ENEMY;

			if(siDbTime != 0)
			{
				//DB縑憮 陳瞼蒂 嫡嬴諮擊 唳辦.
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
		// 醞董檜 腎歷棻.
		GuildRelation.siRelation	=	ON_GUILD_RELATION_NEUTRAL;		
		break;
	}

	// 蟾晦�降� 嬴棍 陽縑虜 DB縑 撲薑ж堅
	// 詭衛雖蒂 爾頂遽棻.
	if(bInit == FALSE)
	{
		siNowYear					=	m_pMyWorld->RealWorldTime.uiYear;
		siNowMon					=	m_pMyWorld->RealWorldTime.uiMonth;
		siNowDay					=	m_pMyWorld->RealWorldTime.uiDay;

		sprintf(szTemp,"%04d%02d%02d",siNowYear,siNowMon,siNowDay);
		GuildRelation.siTime		=	atoi(szTemp);	

		// DB縑 撩奢瞳戲煎 盪濰ц朝雖 憲嬴螞棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-15 螃�� 5:21:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltGuild::GetRelation(UI32 uiArrayIndex)
{
	return	m_siGuildRelation[uiArrayIndex];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 營撲薑и棻.(衙橾 12衛葆棻)
//	熱薑 橾濠	:	2002-03-15 螃�� 5:17:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

			//瞳渠 勒雖 3橾檜 雖陬棻賊 奢撩賅萄煎 夥脯遽棻.
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

		TempGuildFrieldlyMessageOffer.uiGuildID1	=	pGuild->m_uiGuildID;	//頂陛 爾頂朝 鼻欽曖 嬴檜蛤.
		TempGuildFrieldlyMessageOffer.uiGuildID2	=	m_uiGuildID;			//頂 嬴檜蛤.
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

	//�俸�薯曖蒂 嫡嬴菟艘擊 唳辦 瞳渠 鼻欽縑 婦溼脹 薑爾蒂 賅舒 雖錶遽棻.
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

	//奢撩擊 啗樓 й匙檣雖 棻艇鼻欽婁 匐餌...
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
	//�俸�薯曖蒂 嫡嬴菟檜雖 彊懊擊 唳辦 詭撮雖虜 雖錶遽棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	渠寞縑啪 奢撩 唸婁縑 渠и 絲婁 爾晝ヶ擊 遽棻.
//	熱薑 橾濠	:	2003-01-03 螃瞪 11:01:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//VOID	cltGuild::RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode ,SI16 siRewardSupply)
VOID	cltGuild::RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode)
{
	SOPlayer	*pPlayer;
	MONEY		TempMoney;

	// ⑷營 渠寞檜 煎斜檣 醞檜賊 盪旎擋擊 滲唳п遽棻.
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
		
		// 瞪羹 鼻欽錳縑啪 詭衛雖蒂 爾頂遽棻.
		SendMsgToGuildMemebers( ON_OFFICIAL_ANNOUNCEMENT, SO_SFM_ONLYME, 
							LPVOID( &OADAIGetMoneyByDestroyVillageMsg  ), 
							LPVOID( sizeof( OADAIGetMoneyByDestroyVillageMsg ) ) );
	}
	else //渠寞檜 煎斜嬴醒 鼻鷓檜棻.
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
		

		// 瞪羹 鼻欽錳縑啪 詭衛雖蒂 爾頂遽棻.
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

	//奢撩擊 ж朝 蹂橾檜 嬴棲賊 false	
//	if(m_pMyWorld->pRealTime->tm_wday == 2 || m_pMyWorld->pRealTime->tm_wday == 4)	
	if(m_pMyWorld->m_WarTimeParser.IsWarDay(m_pMyWorld->pRealTime->tm_wday))
	{
		if(m_pMyWorld->pRealTime->tm_year + 1900 > siYear) return TRUE;
		if(m_pMyWorld->pRealTime->tm_mon + 1 > siMon ) return TRUE;

//		SI16 siCheck = m_pMyWorld->pRealTime->tm_yday - siDay;
		SI16 siCheck = m_pMyWorld->pRealTime->tm_mday - siDay;
		if(siCheck <= 1) return bChek;

		//瞳渠勒 蹂橾擊 掘и棻.
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

//�俸�薯曖 熱塊衛 棻艇鼻欽婁 奢撩檜 氈朝雖 匐餌蒂 п憮 橈戲賊 渦檜鼻 奢撩檜 橈棻堅 憲溥遽棻.
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

//奢撩 衛除檜 腎賊 奢撩檜 氈朝 鼻欽曖 鼻欽錳縑啪 憲溥遽棻.
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

//奢撩ん晦 薑爾蒂 撮たи棻.
VOID cltGuild::SetGiveUpInfo(UI16 uiVillageCode, MONEY mnMoney, SI16 siFlag)
{
	if(uiVillageCode < 0) return;
	if(mnMoney < 0) return;

	m_GiveUpInfo.uiVillageCode = uiVillageCode;
	m_GiveUpInfo.mnMoney       = mnMoney;
	m_GiveUpInfo.siGiveUpFlag  = siFlag;
}

//奢撩ん晦 薑爾蒂 陛螳螞棻.
VOID cltGuild::GetGiveUpInfo(UI16 *puiVillageCode, MONEY *pmnMoney)
{
	if(puiVillageCode == NULL || pmnMoney == NULL )return;

	*puiVillageCode = m_GiveUpInfo.uiVillageCode;
	*pmnMoney       = m_GiveUpInfo.mnMoney;
}

//奢撩ん晦 薑爾醞 Ы溯斜蒂 撮たи棻.
VOID cltGuild::SetGiveUpFlag(SI16 siFlag)
{
	if(siFlag < ON_GUILD_GIVEUP_NORMAL || siFlag > ON_GUILD_GIVEUP_NO) return;
	m_GiveUpInfo.siGiveUpFlag  = siFlag;
}

//奢撩ん晦 薑爾醞 Ы溯斜蒂 陛螳螞棻.
SI16 cltGuild::GetGiveUpFlag()
{
	return m_GiveUpInfo.siGiveUpFlag;
}

//棻艇鼻欽婁曖 婦啗蒂 瞳渠煎 滲唳衛麵遽棻.
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

//鼻欽錳菟縑啪 奢撩擊 ん晦ц棻堅 憲溥遽棻.
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
//			if(m_pMyWorld->m_WarTimeParser.IsWarTime(m_pMyWorld->pRealTime->tm_hour)) //奢撩衛除檣陛?
			{
				if(pPlayer->IsFieldAttackUnit()) //奢撩嶸棉檣陛
					pPlayer->ChangeNormalChar();
			}
		}
	}
}