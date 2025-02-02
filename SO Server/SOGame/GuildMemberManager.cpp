#include "GuildMemberManager.h"

cltIGuildClass*		GuildMemberManager::m_pclIGuildClass;						// 霜晝 囀萄蒂 橢擊 陽 в蹂ж棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-04-19 螃�� 2:14:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
GuildMemberManager::GuildMemberManager()
{
	Init();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-04-19 螃�� 2:14:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
GuildMemberManager::~GuildMemberManager()
{	
	m_RealMembers.Release();
	m_Candidates.Release();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.	
//	熱薑 橾濠	:	2002-04-19 螃�� 2:46:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	GuildMemberManager::InitStaticVariable( cltIGuildClass *pclIGuildClass )
{
	m_pclIGuildClass	=	pclIGuildClass;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	и廓虜 蟾晦�飛� п還 匙菟擊 п遽棻.
//	熱薑 橾濠	:	2002-04-19 螃�� 3:41:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	GuildMemberManager::OnceInit()
{
	m_RealMembers.Create(ON_GUILD_MIN_MEMBER_NUM);
	m_Candidates.Create(ON_GUILD_MAX_CANDIDATE_NUM);
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��
//	熱薑 橾濠	:	2002-04-19 螃�� 2:14:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	GuildMemberManager::Init()
{
	m_RealMembers.DeleteAll();
	m_Candidates.DeleteAll();

	CalculateRMTotalPageNum();		
	CalculateCMTotalPageNum();

	//m_siMaxRealMemberNum	=	ON_GUILD_MIN_MEMBER_NUM;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽錳 縑 蹺陛衛鑑棻. siClassInGuild縑 評塭憮 鼻欽錳檣雖 雖錳濠檣雖 掘滌и棻.
//	熱薑 橾濠	:	2002-04-19 螃�� 2:16:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::AddMember(CHAR *pszName, SI32 siClassInGuild, MONEY mnProperty, SI32 siLevel, UI32 uiTradeCredit,SI32 siAccount,UI08 uiSlot)
{	
	strGuildMemberInfo				stGuildMemberInfo;
	strCandidateInfo				stCandidateInfo;
	
	if(IsCandidate(siClassInGuild) == FALSE)		
	{
		//if( m_RealMembers.m_NowUseDataCount >= min( ON_GUILD_MAX_MEMBER_NUM, m_siMaxRealMemberNum ) )
		if( m_RealMembers.m_NowUseDataCount >= ON_GUILD_MIN_MEMBER_NUM )
		{
			return	FALSE;	
		}
	}
	else											
	{
		if( m_Candidates.m_NowUseDataCount >= ON_GUILD_MAX_CANDIDATE_NUM )
		{
			return	FALSE;
		}
	}
	
	
	if(IsCandidate(siClassInGuild) == TRUE)
	{
		// 雖錳濠檜棻,		
		ZeroMemory(&stCandidateInfo, sizeof(stCandidateInfo));
		
		strncpy(stCandidateInfo.szName, pszName, ON_ID_LENGTH);					// 檜葷 犒餌.	
		stCandidateInfo.mnPropery				=	mnProperty;						// 識 濠骯
		stCandidateInfo.siLevel					=	siLevel;						// 溯漣
		stCandidateInfo.uiTradeCredit			=	uiTradeCredit;					// 鼠羲 褐辨紫				

		stCandidateInfo.siAccount               =   siAccount;
		stCandidateInfo.uiSlot                  =   uiSlot;
		
		m_Candidates.Add(stCandidateInfo);
		CalculateCMTotalPageNum();
	}
	else
	{
		// 雖錳濠陛 嬴棲棻.
		
		// 葬蝶お縑 餌辨濠蒂 蹺陛и棻.
		ZeroMemory(&stGuildMemberInfo, sizeof(stGuildMemberInfo));
		
		strncpy(stGuildMemberInfo.szName, pszName, ON_ID_LENGTH);					// 檜葷 犒餌.
		stGuildMemberInfo.siClassInGuild		=	siClassInGuild;					// 贗楚蝶 犒餌.
		stGuildMemberInfo.mnPropery				=	mnProperty;						// 識 濠骯
		stGuildMemberInfo.siLevel				=	siLevel;						// 溯漣
		stGuildMemberInfo.uiTradeCredit			=	uiTradeCredit;					// 鼠羲 褐辨紫				

		stGuildMemberInfo.siAccount               =   siAccount;
		stGuildMemberInfo.uiSlot                  =   uiSlot;
		
		
		m_RealMembers.Add(stGuildMemberInfo);
		CalculateRMTotalPageNum();
	}
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	m_stAllMembers縑憮 薯剪и棻.	
//	熱薑 橾濠	:	2002-04-19 螃�� 2:35:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::DelMember(CHAR *pszName, BOOL bIsCandidate)
{
	SI32			i;

	if(bIsCandidate == TRUE)
	{
		// 雖錳濠檜棻.
		for(i = 0; i < m_Candidates.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH) == 0)			
			{				
				m_Candidates.Delete(i);
				CalculateCMTotalPageNum();
				break;
			}
		}
	}
	else
	{
		// 鼻欽錳檜棻.
		for(i = 0; i < m_RealMembers.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_RealMembers.m_Data[i].szName, ON_ID_LENGTH) == 0)
			{			
				m_RealMembers.Delete(i);
				CalculateRMTotalPageNum();
				break;
			}
		}
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褒薯 鼻欽錳曖 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-19 螃�� 3:49:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	GuildMemberManager::GetRealMemeberNum()
{
	return	m_RealMembers.m_NowUseDataCount;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褒薯 雖錳濠 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 2:11:23 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	GuildMemberManager::GetCandidateMemberNum()
{
	return m_Candidates.m_NowUseDataCount;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褒薯 鼻欽錳檜 粕 獺朝雖 匐餌и棻.	
//	熱薑 橾濠	:	2002-04-19 螃�� 3:50:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::IsRealMemberFull()
{
	if( m_RealMembers.m_NowUseDataCount >= ON_GUILD_MIN_MEMBER_NUM )		return	TRUE;
	else																	return	FALSE;	
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	譆渠 鼻欽錳曖 熱蒂 雖薑п遽棻.
//	熱薑 橾濠	:	2002-04-19 螃�� 3:53:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	GuildMemberManager::SetMaxRealMemberNum(SI32 siMaxRealMemberNum)
{	
	//m_siMaxRealMemberNum	=	siMaxRealMemberNum;

	// 雖旎擎 鼠褻勒 譆渠 100貲檜棻.
	m_siMaxRealMemberNum	=	ON_GUILD_MIN_MEMBER_NUM;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	譆渠 鼻欽錳曖 熱蒂 橢橫螞棻
//	熱薑 橾濠	:	2002-04-19 螃�� 4:24:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	GuildMemberManager::GetMaxRealMemberNum()
{
	return m_siMaxRealMemberNum;
}
*/


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝擊 滲唳п遽棻.
//	熱薑 橾濠	:	2002-04-19 螃�� 3:56:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::ChangeClass(CHAR *pszName, SI32 siChangeClass, BOOL bCandidate)
{	
	SI32					i;
	strGuildMemberInfo		stGuildMemberInfo;

	if(bCandidate == TRUE)
	{
		// 雖錳濠檜棻.		
		for(i = 0; i < m_Candidates.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH) == 0)			
			{
				if(IsCandidate(siChangeClass) == FALSE)
				{	
					// 渦 檜鼻 雖錳濠陛 嬴棲棻.

					ZeroMemory(&stGuildMemberInfo, sizeof(stGuildMemberInfo));
				
					strncpy(stGuildMemberInfo.szName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH);
					stGuildMemberInfo.siClassInGuild	=	siChangeClass;
					stGuildMemberInfo.siAccount         =   m_Candidates.m_Data[i].siAccount;
					stGuildMemberInfo.uiSlot            =   m_Candidates.m_Data[i].uiSlot;
					stGuildMemberInfo.mnPropery			=	m_Candidates.m_Data[i].mnPropery;
					stGuildMemberInfo.siLevel			=	m_Candidates.m_Data[i].siLevel;
					stGuildMemberInfo.uiTradeCredit		=	m_Candidates.m_Data[i].uiTradeCredit;			

					// 薑衝 鼻欽錳 葬蝶お縑 蹺陛и棻.
					m_RealMembers.Add(stGuildMemberInfo);

					// 雖錳濠 葬蝶お縑憮 薯剪и棻.
					m_Candidates.Delete(i);					
					
					// 薑衝 鼻欽錳婁 雖錳濠 葬蝶お曖 瞪羹 む檜雖 熱蒂 營 啗骯и棻.
					CalculateRMTotalPageNum();
					CalculateCMTotalPageNum();
				}									
			}
		}
	}
	else		// 橾奩 鼻欽錳檜棻.
	{
		for(i = 0; i < m_RealMembers.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_RealMembers.m_Data[i].szName, ON_ID_LENGTH) == 0)						
				m_RealMembers.m_Data[i].siClassInGuild	=	siChangeClass;					
		}
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪羹 薑衝 鼻欽錳擊 �飛橦� 爾罹輿晦 嬪п в蹂и む檜雖熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 9:54:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	GuildMemberManager::GetRMTotalPageNum()
{	
	return	m_dwRMTotalPageNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪羹 雖錳濠蒂 �飛橦� 爾罹輿晦 嬪п в蹂и む檜雖熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 9:54:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	GuildMemberManager::GetCMTotalPageNum()
{	
	return	m_dwCMTotalPageNum;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪羹 薑衝 鼻欽錳曖 む檜雖熱蒂 營 啗骯и棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 9:54:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	GuildMemberManager::CalculateRMTotalPageNum()
{
	if(m_RealMembers.m_NowUseDataCount % ON_GUILD_MEMBERNUM_PER_PAGE == 0)
		m_dwRMTotalPageNum	=	m_RealMembers.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE;
	else	
		m_dwRMTotalPageNum	=	m_RealMembers.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE + 1;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪羹 雖錳濠曖 む檜雖熱蒂 營 啗骯и棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 9:59:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	GuildMemberManager::CalculateCMTotalPageNum()
{
	if(m_Candidates.m_NowUseDataCount % ON_GUILD_MEMBERNUM_PER_PAGE == 0)
		m_dwCMTotalPageNum	=	m_Candidates.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE;
	else	
		m_dwCMTotalPageNum	=	m_Candidates.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE + 1;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑衝 鼻欽錳曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 11:36:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	GuildMemberManager::GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo)
{
	UI32	uiStartIndex;	
	UI32	uiSendMembersNum;
	
	uiStartIndex	=	(dwPage - 1) * ON_GUILD_MEMBERNUM_PER_PAGE;

	// 衛濛 檣策蝶 譆模и 0 檜鼻檜罹撿 и棻.
	if(uiStartIndex < 0)	return	0;
	
	uiSendMembersNum	=	min(ON_GUILD_MEMBERNUM_PER_PAGE, m_RealMembers.m_NowUseDataCount - uiStartIndex);
	memcpy(pGuildMemeberInfo, &m_RealMembers.m_Data[uiStartIndex], sizeof(strGuildMemberInfo) * uiSendMembersNum);		

	return	uiSendMembersNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雖錳濠曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 11:36:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	GuildMemberManager::GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo)
{
	UI32	uiStartIndex;
	UI32	uiSendMembersNum;
	
	uiStartIndex	=	(dwPage - 1) * ON_GUILD_MEMBERNUM_PER_PAGE;

	// 檣策蝶陛 嶸�褲� 彰嬪 寰縑 氈朝雖 匐餌и棻.
	if(uiStartIndex < 0 || uiStartIndex >= (UI32)m_Candidates.m_NowUseDataCount)	return	0;
	
	uiSendMembersNum	=	min(ON_GUILD_MEMBERNUM_PER_PAGE, m_Candidates.m_NowUseDataCount - uiStartIndex);
	memcpy(pCandidateInfo, &m_Candidates.m_Data[uiStartIndex], sizeof(strCandidateInfo) * uiSendMembersNum);		

	return	uiSendMembersNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝擊 爾嬴憮 雖錳濠檣雖 �挫恉挬�.
//	熱薑 橾濠	:	2002-04-30 螃�� 1:05:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::IsCandidate( SI32 siClassInGuild )
{
	if( siClassInGuild == m_pclIGuildClass->GetJWJCode() )	return	TRUE;
	else													return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪 檜葷擊 餌辨ж罹 望萄 霜晝擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-27 螃�� 2:41:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::GetGuildIDByCharName(CHAR *pszName, SI32 *psiGuildClass)
{
	SI32	i;	

	// 薑衝 詹幗錳 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)
	{
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)
		{
			*psiGuildClass	=	m_RealMembers.m_Data[i].siClassInGuild;
			return	TRUE;
		}
	}

	// 雖錳濠 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_Candidates.m_NowUseDataCount; i++)
	{
		if(stricmp(m_Candidates.m_Data[i].szName, pszName) == 0)
		{
			*psiGuildClass	=	m_pclIGuildClass->GetJWJCode();
			return	TRUE;
		}
	}

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪 檜葷擊 餌辨ж罹 賅萇薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-27 螃�� 2:41:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::GetGuildMemberByCharName(CHAR *pszName,strGuildCommonMemberInfo *pInfo)
{
	SI32	i;	

	// 薑衝 詹幗錳 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)
	{
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)
		{
			pInfo->siAccount = m_RealMembers.m_Data[i].siAccount;
			pInfo->uiSlot = m_RealMembers.m_Data[i].uiSlot;
			pInfo->siClassInGuild = m_RealMembers.m_Data[i].siClassInGuild;
			
			return	TRUE;
		}
	}

	// 雖錳濠 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_Candidates.m_NowUseDataCount; i++)
	{
		if(stricmp(m_Candidates.m_Data[i].szName, pszName) == 0)
		{
			pInfo->siAccount = m_Candidates.m_Data[i].siAccount;
			pInfo->uiSlot = m_Candidates.m_Data[i].uiSlot;			
			pInfo->siClassInGuild	=	m_pclIGuildClass->GetJWJCode();

			return	TRUE;
		}
	}

	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪 橫醴遴お蒂 餌辨ж罹 賅萇薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-27 螃�� 2:41:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::GetGuildMemberByCharAccount(SI32 siAccount,UI08 uiSlot,strGuildCommonMemberInfo *pInfo)
{
	SI32	i;	

	// 薑衝 詹幗錳 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)
	{
		if (siAccount == m_RealMembers.m_Data[i].siAccount)
		{
			if (uiSlot == m_RealMembers.m_Data[i].uiSlot)
			{
				pInfo->siAccount = m_RealMembers.m_Data[i].siAccount;
				pInfo->uiSlot = m_RealMembers.m_Data[i].uiSlot;
				pInfo->siClassInGuild = m_RealMembers.m_Data[i].siClassInGuild;				
				return	TRUE;				
			}
		}
		
	}

	// 雖錳濠 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_Candidates.m_NowUseDataCount; i++)
	{
		if (siAccount == m_Candidates.m_Data[i].siAccount)
		{
			if (uiSlot == m_Candidates.m_Data[i].uiSlot)
			{
				pInfo->siAccount = m_Candidates.m_Data[i].siAccount;
				pInfo->uiSlot = m_Candidates.m_Data[i].uiSlot;			
				pInfo->siClassInGuild	=	m_pclIGuildClass->GetJWJCode();
				
				return	TRUE;				
			}
		}
		
	}

	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽錳檣雖 憲嬴螞棻.(雖錳濠 んл)
//	熱薑 橾濠	:	2002-08-12 螃�� 7:31:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::IsMember(CHAR *pszName)
{
	SI32	i;	

	// 薑衝 詹幗錳 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)	
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)			return	TRUE;	

	// 雖錳濠 醞縑 氈朝雖 瓊嬴獄棻.
	for(i = 0; i  < m_Candidates.m_NowUseDataCount; i++)	
		if(stricmp(m_Candidates.m_Data[i].szName, pszName) == 0)			return	TRUE;
		
	return	FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雖錳濠檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-12-31 螃�� 3:32:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	GuildMemberManager::IsCandidateMember( CHAR *pszName )
{
	SI32	i;
	
	for( i = 0; i < m_Candidates.m_NowUseDataCount; i++ )	
	{
		if( stricmp( m_Candidates.m_Data[ i ].szName, pszName ) == 0 )			return	TRUE;
	}
		
	return	FALSE;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雖錳濠檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-12-31 螃�� 3:32:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strCandidateInfo	*GuildMemberManager::GetCandidateMember( CHAR *pszName )
{
	SI32	i;
	
	for( i = 0; i < m_Candidates.m_NowUseDataCount; i++ )	
	{
		if( stricmp( m_Candidates.m_Data[ i ].szName, pszName ) == 0 )			return	&m_Candidates.m_Data[ i ];
	}
		
	return	NULL;		
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	渠ч熱曖 ID蒂 橢橫螞棻. 鼻欽頂縑 渠ч熱陛 橈戲賊 NULL擊 葬欐и棻.
//	熱薑 橾濠	:	2002-12-31 螃�� 3:32:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	GuildMemberManager::GetDHSID()
{
	SI32	i;
	
	// 薑衝 詹幗錳 醞縑 渠ч熱陛 氈朝雖 瓊嬴獄棻.
	for( i = 0; i  < m_RealMembers.m_NowUseDataCount; i++ )
	{
		if( m_RealMembers.m_Data[i].siClassInGuild == m_pclIGuildClass->GetDHSCode() )
			return	m_RealMembers.m_Data[i].szName;
	}
	
	return	NULL;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	望萄頂 ч翕渠濰曖 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-12-31 螃�� 3:32:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	GuildMemberManager::GetTotalHNDNum()
{
	SI32	i;
	UI32	uiCount	=	0;
	
	// 薑衝 詹幗錳 醞縑 渠ч熱陛 氈朝雖 瓊嬴獄棻.
	for( i = 0; i  < m_RealMembers.m_NowUseDataCount; i++ )
	{
		if( m_RealMembers.m_Data[i].siClassInGuild == m_pclIGuildClass->GetHNDCode() )
			uiCount++;
	}
	
	return	uiCount;
}
