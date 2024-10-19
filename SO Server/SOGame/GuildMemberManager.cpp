#include "GuildMemberManager.h"

cltIGuildClass*		GuildMemberManager::m_pclIGuildClass;						// 직급 코드를 얻을 때 필요하다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-04-19 오후 2:14:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
GuildMemberManager::GuildMemberManager()
{
	Init();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-04-19 오후 2:14:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
GuildMemberManager::~GuildMemberManager()
{	
	m_RealMembers.Release();
	m_Candidates.Release();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.	
//	수정 일자	:	2002-04-19 오후 2:46:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	GuildMemberManager::InitStaticVariable( cltIGuildClass *pclIGuildClass )
{
	m_pclIGuildClass	=	pclIGuildClass;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	한번만 초기화를 해줄 것들을 해준다.
//	수정 일자	:	2002-04-19 오후 3:41:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	GuildMemberManager::OnceInit()
{
	m_RealMembers.Create(ON_GUILD_MIN_MEMBER_NUM);
	m_Candidates.Create(ON_GUILD_MAX_CANDIDATE_NUM);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화
//	수정 일자	:	2002-04-19 오후 2:14:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	GuildMemberManager::Init()
{
	m_RealMembers.DeleteAll();
	m_Candidates.DeleteAll();

	CalculateRMTotalPageNum();		
	CalculateCMTotalPageNum();

	//m_siMaxRealMemberNum	=	ON_GUILD_MIN_MEMBER_NUM;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단원 에 추가시킨다. siClassInGuild에 따라서 상단원인지 지원자인지 구별한다.
//	수정 일자	:	2002-04-19 오후 2:16:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 지원자이다,		
		ZeroMemory(&stCandidateInfo, sizeof(stCandidateInfo));
		
		strncpy(stCandidateInfo.szName, pszName, ON_ID_LENGTH);					// 이름 복사.	
		stCandidateInfo.mnPropery				=	mnProperty;						// 총 자산
		stCandidateInfo.siLevel					=	siLevel;						// 레벨
		stCandidateInfo.uiTradeCredit			=	uiTradeCredit;					// 무역 신용도				

		stCandidateInfo.siAccount               =   siAccount;
		stCandidateInfo.uiSlot                  =   uiSlot;
		
		m_Candidates.Add(stCandidateInfo);
		CalculateCMTotalPageNum();
	}
	else
	{
		// 지원자가 아니다.
		
		// 리스트에 사용자를 추가한다.
		ZeroMemory(&stGuildMemberInfo, sizeof(stGuildMemberInfo));
		
		strncpy(stGuildMemberInfo.szName, pszName, ON_ID_LENGTH);					// 이름 복사.
		stGuildMemberInfo.siClassInGuild		=	siClassInGuild;					// 클래스 복사.
		stGuildMemberInfo.mnPropery				=	mnProperty;						// 총 자산
		stGuildMemberInfo.siLevel				=	siLevel;						// 레벨
		stGuildMemberInfo.uiTradeCredit			=	uiTradeCredit;					// 무역 신용도				

		stGuildMemberInfo.siAccount               =   siAccount;
		stGuildMemberInfo.uiSlot                  =   uiSlot;
		
		
		m_RealMembers.Add(stGuildMemberInfo);
		CalculateRMTotalPageNum();
	}
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	m_stAllMembers에서 제거한다.	
//	수정 일자	:	2002-04-19 오후 2:35:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::DelMember(CHAR *pszName, BOOL bIsCandidate)
{
	SI32			i;

	if(bIsCandidate == TRUE)
	{
		// 지원자이다.
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
		// 상단원이다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	실제 상단원의 수를 얻어온다.
//	수정 일자	:	2002-04-19 오후 3:49:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	GuildMemberManager::GetRealMemeberNum()
{
	return	m_RealMembers.m_NowUseDataCount;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	실제 지원자 수를 얻어온다.
//	수정 일자	:	2002-05-09 오후 2:11:23 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	GuildMemberManager::GetCandidateMemberNum()
{
	return m_Candidates.m_NowUseDataCount;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	실제 상단원이 꽉 찼는지 검사한다.	
//	수정 일자	:	2002-04-19 오후 3:50:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::IsRealMemberFull()
{
	if( m_RealMembers.m_NowUseDataCount >= ON_GUILD_MIN_MEMBER_NUM )		return	TRUE;
	else																	return	FALSE;	
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	최대 상단원의 수를 지정해준다.
//	수정 일자	:	2002-04-19 오후 3:53:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	GuildMemberManager::SetMaxRealMemberNum(SI32 siMaxRealMemberNum)
{	
	//m_siMaxRealMemberNum	=	siMaxRealMemberNum;

	// 지금은 무조건 최대 100명이다.
	m_siMaxRealMemberNum	=	ON_GUILD_MIN_MEMBER_NUM;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	최대 상단원의 수를 얻어온다
//	수정 일자	:	2002-04-19 오후 4:24:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	GuildMemberManager::GetMaxRealMemberNum()
{
	return m_siMaxRealMemberNum;
}
*/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	직급을 변경해준다.
//	수정 일자	:	2002-04-19 오후 3:56:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::ChangeClass(CHAR *pszName, SI32 siChangeClass, BOOL bCandidate)
{	
	SI32					i;
	strGuildMemberInfo		stGuildMemberInfo;

	if(bCandidate == TRUE)
	{
		// 지원자이다.		
		for(i = 0; i < m_Candidates.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH) == 0)			
			{
				if(IsCandidate(siChangeClass) == FALSE)
				{	
					// 더 이상 지원자가 아니다.

					ZeroMemory(&stGuildMemberInfo, sizeof(stGuildMemberInfo));
				
					strncpy(stGuildMemberInfo.szName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH);
					stGuildMemberInfo.siClassInGuild	=	siChangeClass;
					stGuildMemberInfo.siAccount         =   m_Candidates.m_Data[i].siAccount;
					stGuildMemberInfo.uiSlot            =   m_Candidates.m_Data[i].uiSlot;
					stGuildMemberInfo.mnPropery			=	m_Candidates.m_Data[i].mnPropery;
					stGuildMemberInfo.siLevel			=	m_Candidates.m_Data[i].siLevel;
					stGuildMemberInfo.uiTradeCredit		=	m_Candidates.m_Data[i].uiTradeCredit;			

					// 정식 상단원 리스트에 추가한다.
					m_RealMembers.Add(stGuildMemberInfo);

					// 지원자 리스트에서 제거한다.
					m_Candidates.Delete(i);					
					
					// 정식 상단원과 지원자 리스트의 전체 페이지 수를 재 계산한다.
					CalculateRMTotalPageNum();
					CalculateCMTotalPageNum();
				}									
			}
		}
	}
	else		// 일반 상단원이다.
	{
		for(i = 0; i < m_RealMembers.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_RealMembers.m_Data[i].szName, ON_ID_LENGTH) == 0)						
				m_RealMembers.m_Data[i].siClassInGuild	=	siChangeClass;					
		}
	}

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전체 정식 상단원을 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
//	수정 일자	:	2002-04-22 오전 9:54:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	GuildMemberManager::GetRMTotalPageNum()
{	
	return	m_dwRMTotalPageNum;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전체 지원자를 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
//	수정 일자	:	2002-04-22 오전 9:54:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	GuildMemberManager::GetCMTotalPageNum()
{	
	return	m_dwCMTotalPageNum;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전체 정식 상단원의 페이지수를 재 계산한다.
//	수정 일자	:	2002-04-22 오전 9:54:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	GuildMemberManager::CalculateRMTotalPageNum()
{
	if(m_RealMembers.m_NowUseDataCount % ON_GUILD_MEMBERNUM_PER_PAGE == 0)
		m_dwRMTotalPageNum	=	m_RealMembers.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE;
	else	
		m_dwRMTotalPageNum	=	m_RealMembers.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE + 1;			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전체 지원자의 페이지수를 재 계산한다.
//	수정 일자	:	2002-04-22 오전 9:59:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	GuildMemberManager::CalculateCMTotalPageNum()
{
	if(m_Candidates.m_NowUseDataCount % ON_GUILD_MEMBERNUM_PER_PAGE == 0)
		m_dwCMTotalPageNum	=	m_Candidates.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE;
	else	
		m_dwCMTotalPageNum	=	m_Candidates.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE + 1;			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정식 상단원의 리스트를 얻어온다.
//	수정 일자	:	2002-04-22 오전 11:36:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	GuildMemberManager::GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo)
{
	UI32	uiStartIndex;	
	UI32	uiSendMembersNum;
	
	uiStartIndex	=	(dwPage - 1) * ON_GUILD_MEMBERNUM_PER_PAGE;

	// 시작 인덱스 최소한 0 이상이여야 한다.
	if(uiStartIndex < 0)	return	0;
	
	uiSendMembersNum	=	min(ON_GUILD_MEMBERNUM_PER_PAGE, m_RealMembers.m_NowUseDataCount - uiStartIndex);
	memcpy(pGuildMemeberInfo, &m_RealMembers.m_Data[uiStartIndex], sizeof(strGuildMemberInfo) * uiSendMembersNum);		

	return	uiSendMembersNum;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	지원자의 리스트를 얻어온다.
//	수정 일자	:	2002-04-22 오전 11:36:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	GuildMemberManager::GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo)
{
	UI32	uiStartIndex;
	UI32	uiSendMembersNum;
	
	uiStartIndex	=	(dwPage - 1) * ON_GUILD_MEMBERNUM_PER_PAGE;

	// 인덱스가 유효한 범위 안에 있는지 검사한다.
	if(uiStartIndex < 0 || uiStartIndex >= (UI32)m_Candidates.m_NowUseDataCount)	return	0;
	
	uiSendMembersNum	=	min(ON_GUILD_MEMBERNUM_PER_PAGE, m_Candidates.m_NowUseDataCount - uiStartIndex);
	memcpy(pCandidateInfo, &m_Candidates.m_Data[uiStartIndex], sizeof(strCandidateInfo) * uiSendMembersNum);		

	return	uiSendMembersNum;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	직급을 보아서 지원자인지 확인한다.
//	수정 일자	:	2002-04-30 오후 1:05:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::IsCandidate( SI32 siClassInGuild )
{
	if( siClassInGuild == m_pclIGuildClass->GetJWJCode() )	return	TRUE;
	else													return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터 이름을 사용하여 길드 직급을 얻어온다.
//	수정 일자	:	2002-05-27 오후 2:41:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::GetGuildIDByCharName(CHAR *pszName, SI32 *psiGuildClass)
{
	SI32	i;	

	// 정식 멤버원 중에 있는지 찾아본다.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)
	{
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)
		{
			*psiGuildClass	=	m_RealMembers.m_Data[i].siClassInGuild;
			return	TRUE;
		}
	}

	// 지원자 중에 있는지 찾아본다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터 이름을 사용하여 모든정보를 얻어온다.
//	수정 일자	:	2002-05-27 오후 2:41:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::GetGuildMemberByCharName(CHAR *pszName,strGuildCommonMemberInfo *pInfo)
{
	SI32	i;	

	// 정식 멤버원 중에 있는지 찾아본다.
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

	// 지원자 중에 있는지 찾아본다.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터 어커운트를 사용하여 모든정보를 얻어온다.
//	수정 일자	:	2002-05-27 오후 2:41:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::GetGuildMemberByCharAccount(SI32 siAccount,UI08 uiSlot,strGuildCommonMemberInfo *pInfo)
{
	SI32	i;	

	// 정식 멤버원 중에 있는지 찾아본다.
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

	// 지원자 중에 있는지 찾아본다.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상단원인지 알아온다.(지원자 포함)
//	수정 일자	:	2002-08-12 오후 7:31:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::IsMember(CHAR *pszName)
{
	SI32	i;	

	// 정식 멤버원 중에 있는지 찾아본다.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)	
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)			return	TRUE;	

	// 지원자 중에 있는지 찾아본다.
	for(i = 0; i  < m_Candidates.m_NowUseDataCount; i++)	
		if(stricmp(m_Candidates.m_Data[i].szName, pszName) == 0)			return	TRUE;
		
	return	FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	지원자인지 알아온다.
//	수정 일자	:	2002-12-31 오후 3:32:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	GuildMemberManager::IsCandidateMember( CHAR *pszName )
{
	SI32	i;
	
	for( i = 0; i < m_Candidates.m_NowUseDataCount; i++ )	
	{
		if( stricmp( m_Candidates.m_Data[ i ].szName, pszName ) == 0 )			return	TRUE;
	}
		
	return	FALSE;		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	지원자인지 알아온다.
//	수정 일자	:	2002-12-31 오후 3:32:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
strCandidateInfo	*GuildMemberManager::GetCandidateMember( CHAR *pszName )
{
	SI32	i;
	
	for( i = 0; i < m_Candidates.m_NowUseDataCount; i++ )	
	{
		if( stricmp( m_Candidates.m_Data[ i ].szName, pszName ) == 0 )			return	&m_Candidates.m_Data[ i ];
	}
		
	return	NULL;		
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	대행수의 ID를 얻어온다. 상단내에 대행수가 없으면 NULL을 리턴한다.
//	수정 일자	:	2002-12-31 오후 3:32:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	GuildMemberManager::GetDHSID()
{
	SI32	i;
	
	// 정식 멤버원 중에 대행수가 있는지 찾아본다.
	for( i = 0; i  < m_RealMembers.m_NowUseDataCount; i++ )
	{
		if( m_RealMembers.m_Data[i].siClassInGuild == m_pclIGuildClass->GetDHSCode() )
			return	m_RealMembers.m_Data[i].szName;
	}
	
	return	NULL;		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	길드내 행동대장의 수를 얻어온다.
//	수정 일자	:	2002-12-31 오후 3:32:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	GuildMemberManager::GetTotalHNDNum()
{
	SI32	i;
	UI32	uiCount	=	0;
	
	// 정식 멤버원 중에 대행수가 있는지 찾아본다.
	for( i = 0; i  < m_RealMembers.m_NowUseDataCount; i++ )
	{
		if( m_RealMembers.m_Data[i].siClassInGuild == m_pclIGuildClass->GetHNDCode() )
			uiCount++;
	}
	
	return	uiCount;
}
