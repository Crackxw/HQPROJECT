#include "GuildMemberManager.h"

cltIGuildClass*		GuildMemberManager::m_pclIGuildClass;						// ���� �ڵ带 ���� �� �ʿ��ϴ�.

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-04-19 ���� 2:14:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
GuildMemberManager::GuildMemberManager()
{
	Init();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-04-19 ���� 2:14:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
GuildMemberManager::~GuildMemberManager()
{	
	m_RealMembers.Release();
	m_Candidates.Release();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.	
//	���� ����	:	2002-04-19 ���� 2:46:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GuildMemberManager::InitStaticVariable( cltIGuildClass *pclIGuildClass )
{
	m_pclIGuildClass	=	pclIGuildClass;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ѹ��� �ʱ�ȭ�� ���� �͵��� ���ش�.
//	���� ����	:	2002-04-19 ���� 3:41:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GuildMemberManager::OnceInit()
{
	m_RealMembers.Create(ON_GUILD_MIN_MEMBER_NUM);
	m_Candidates.Create(ON_GUILD_MAX_CANDIDATE_NUM);
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ
//	���� ����	:	2002-04-19 ���� 2:14:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GuildMemberManager::Init()
{
	m_RealMembers.DeleteAll();
	m_Candidates.DeleteAll();

	CalculateRMTotalPageNum();		
	CalculateCMTotalPageNum();

	//m_siMaxRealMemberNum	=	ON_GUILD_MIN_MEMBER_NUM;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� �� �߰���Ų��. siClassInGuild�� ���� ��ܿ����� ���������� �����Ѵ�.
//	���� ����	:	2002-04-19 ���� 2:16:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// �������̴�,		
		ZeroMemory(&stCandidateInfo, sizeof(stCandidateInfo));
		
		strncpy(stCandidateInfo.szName, pszName, ON_ID_LENGTH);					// �̸� ����.	
		stCandidateInfo.mnPropery				=	mnProperty;						// �� �ڻ�
		stCandidateInfo.siLevel					=	siLevel;						// ����
		stCandidateInfo.uiTradeCredit			=	uiTradeCredit;					// ���� �ſ뵵				

		stCandidateInfo.siAccount               =   siAccount;
		stCandidateInfo.uiSlot                  =   uiSlot;
		
		m_Candidates.Add(stCandidateInfo);
		CalculateCMTotalPageNum();
	}
	else
	{
		// �����ڰ� �ƴϴ�.
		
		// ����Ʈ�� ����ڸ� �߰��Ѵ�.
		ZeroMemory(&stGuildMemberInfo, sizeof(stGuildMemberInfo));
		
		strncpy(stGuildMemberInfo.szName, pszName, ON_ID_LENGTH);					// �̸� ����.
		stGuildMemberInfo.siClassInGuild		=	siClassInGuild;					// Ŭ���� ����.
		stGuildMemberInfo.mnPropery				=	mnProperty;						// �� �ڻ�
		stGuildMemberInfo.siLevel				=	siLevel;						// ����
		stGuildMemberInfo.uiTradeCredit			=	uiTradeCredit;					// ���� �ſ뵵				

		stGuildMemberInfo.siAccount               =   siAccount;
		stGuildMemberInfo.uiSlot                  =   uiSlot;
		
		
		m_RealMembers.Add(stGuildMemberInfo);
		CalculateRMTotalPageNum();
	}
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	m_stAllMembers���� �����Ѵ�.	
//	���� ����	:	2002-04-19 ���� 2:35:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::DelMember(CHAR *pszName, BOOL bIsCandidate)
{
	SI32			i;

	if(bIsCandidate == TRUE)
	{
		// �������̴�.
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
		// ��ܿ��̴�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��ܿ��� ���� ���´�.
//	���� ����	:	2002-04-19 ���� 3:49:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	GuildMemberManager::GetRealMemeberNum()
{
	return	m_RealMembers.m_NowUseDataCount;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ���� ���´�.
//	���� ����	:	2002-05-09 ���� 2:11:23 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	GuildMemberManager::GetCandidateMemberNum()
{
	return m_Candidates.m_NowUseDataCount;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��ܿ��� �� á���� �˻��Ѵ�.	
//	���� ����	:	2002-04-19 ���� 3:50:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::IsRealMemberFull()
{
	if( m_RealMembers.m_NowUseDataCount >= ON_GUILD_MIN_MEMBER_NUM )		return	TRUE;
	else																	return	FALSE;	
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ִ� ��ܿ��� ���� �������ش�.
//	���� ����	:	2002-04-19 ���� 3:53:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GuildMemberManager::SetMaxRealMemberNum(SI32 siMaxRealMemberNum)
{	
	//m_siMaxRealMemberNum	=	siMaxRealMemberNum;

	// ������ ������ �ִ� 100���̴�.
	m_siMaxRealMemberNum	=	ON_GUILD_MIN_MEMBER_NUM;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ִ� ��ܿ��� ���� ���´�
//	���� ����	:	2002-04-19 ���� 4:24:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	GuildMemberManager::GetMaxRealMemberNum()
{
	return m_siMaxRealMemberNum;
}
*/


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �������ش�.
//	���� ����	:	2002-04-19 ���� 3:56:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::ChangeClass(CHAR *pszName, SI32 siChangeClass, BOOL bCandidate)
{	
	SI32					i;
	strGuildMemberInfo		stGuildMemberInfo;

	if(bCandidate == TRUE)
	{
		// �������̴�.		
		for(i = 0; i < m_Candidates.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH) == 0)			
			{
				if(IsCandidate(siChangeClass) == FALSE)
				{	
					// �� �̻� �����ڰ� �ƴϴ�.

					ZeroMemory(&stGuildMemberInfo, sizeof(stGuildMemberInfo));
				
					strncpy(stGuildMemberInfo.szName, m_Candidates.m_Data[i].szName, ON_ID_LENGTH);
					stGuildMemberInfo.siClassInGuild	=	siChangeClass;
					stGuildMemberInfo.siAccount         =   m_Candidates.m_Data[i].siAccount;
					stGuildMemberInfo.uiSlot            =   m_Candidates.m_Data[i].uiSlot;
					stGuildMemberInfo.mnPropery			=	m_Candidates.m_Data[i].mnPropery;
					stGuildMemberInfo.siLevel			=	m_Candidates.m_Data[i].siLevel;
					stGuildMemberInfo.uiTradeCredit		=	m_Candidates.m_Data[i].uiTradeCredit;			

					// ���� ��ܿ� ����Ʈ�� �߰��Ѵ�.
					m_RealMembers.Add(stGuildMemberInfo);

					// ������ ����Ʈ���� �����Ѵ�.
					m_Candidates.Delete(i);					
					
					// ���� ��ܿ��� ������ ����Ʈ�� ��ü ������ ���� �� ����Ѵ�.
					CalculateRMTotalPageNum();
					CalculateCMTotalPageNum();
				}									
			}
		}
	}
	else		// �Ϲ� ��ܿ��̴�.
	{
		for(i = 0; i < m_RealMembers.m_NowUseDataCount; i++)
		{			
			if(strncmp(pszName, m_RealMembers.m_Data[i].szName, ON_ID_LENGTH) == 0)						
				m_RealMembers.m_Data[i].siClassInGuild	=	siChangeClass;					
		}
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ü ���� ��ܿ��� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
//	���� ����	:	2002-04-22 ���� 9:54:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	GuildMemberManager::GetRMTotalPageNum()
{	
	return	m_dwRMTotalPageNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ü �����ڸ� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
//	���� ����	:	2002-04-22 ���� 9:54:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	GuildMemberManager::GetCMTotalPageNum()
{	
	return	m_dwCMTotalPageNum;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ü ���� ��ܿ��� ���������� �� ����Ѵ�.
//	���� ����	:	2002-04-22 ���� 9:54:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GuildMemberManager::CalculateRMTotalPageNum()
{
	if(m_RealMembers.m_NowUseDataCount % ON_GUILD_MEMBERNUM_PER_PAGE == 0)
		m_dwRMTotalPageNum	=	m_RealMembers.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE;
	else	
		m_dwRMTotalPageNum	=	m_RealMembers.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE + 1;			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ü �������� ���������� �� ����Ѵ�.
//	���� ����	:	2002-04-22 ���� 9:59:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	GuildMemberManager::CalculateCMTotalPageNum()
{
	if(m_Candidates.m_NowUseDataCount % ON_GUILD_MEMBERNUM_PER_PAGE == 0)
		m_dwCMTotalPageNum	=	m_Candidates.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE;
	else	
		m_dwCMTotalPageNum	=	m_Candidates.m_NowUseDataCount / ON_GUILD_MEMBERNUM_PER_PAGE + 1;			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��ܿ��� ����Ʈ�� ���´�.
//	���� ����	:	2002-04-22 ���� 11:36:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	GuildMemberManager::GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo)
{
	UI32	uiStartIndex;	
	UI32	uiSendMembersNum;
	
	uiStartIndex	=	(dwPage - 1) * ON_GUILD_MEMBERNUM_PER_PAGE;

	// ���� �ε��� �ּ��� 0 �̻��̿��� �Ѵ�.
	if(uiStartIndex < 0)	return	0;
	
	uiSendMembersNum	=	min(ON_GUILD_MEMBERNUM_PER_PAGE, m_RealMembers.m_NowUseDataCount - uiStartIndex);
	memcpy(pGuildMemeberInfo, &m_RealMembers.m_Data[uiStartIndex], sizeof(strGuildMemberInfo) * uiSendMembersNum);		

	return	uiSendMembersNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ����Ʈ�� ���´�.
//	���� ����	:	2002-04-22 ���� 11:36:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	GuildMemberManager::GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo)
{
	UI32	uiStartIndex;
	UI32	uiSendMembersNum;
	
	uiStartIndex	=	(dwPage - 1) * ON_GUILD_MEMBERNUM_PER_PAGE;

	// �ε����� ��ȿ�� ���� �ȿ� �ִ��� �˻��Ѵ�.
	if(uiStartIndex < 0 || uiStartIndex >= (UI32)m_Candidates.m_NowUseDataCount)	return	0;
	
	uiSendMembersNum	=	min(ON_GUILD_MEMBERNUM_PER_PAGE, m_Candidates.m_NowUseDataCount - uiStartIndex);
	memcpy(pCandidateInfo, &m_Candidates.m_Data[uiStartIndex], sizeof(strCandidateInfo) * uiSendMembersNum);		

	return	uiSendMembersNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���Ƽ� ���������� Ȯ���Ѵ�.
//	���� ����	:	2002-04-30 ���� 1:05:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::IsCandidate( SI32 siClassInGuild )
{
	if( siClassInGuild == m_pclIGuildClass->GetJWJCode() )	return	TRUE;
	else													return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���� �̸��� ����Ͽ� ��� ������ ���´�.
//	���� ����	:	2002-05-27 ���� 2:41:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::GetGuildIDByCharName(CHAR *pszName, SI32 *psiGuildClass)
{
	SI32	i;	

	// ���� ����� �߿� �ִ��� ã�ƺ���.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)
	{
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)
		{
			*psiGuildClass	=	m_RealMembers.m_Data[i].siClassInGuild;
			return	TRUE;
		}
	}

	// ������ �߿� �ִ��� ã�ƺ���.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���� �̸��� ����Ͽ� ��������� ���´�.
//	���� ����	:	2002-05-27 ���� 2:41:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::GetGuildMemberByCharName(CHAR *pszName,strGuildCommonMemberInfo *pInfo)
{
	SI32	i;	

	// ���� ����� �߿� �ִ��� ã�ƺ���.
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

	// ������ �߿� �ִ��� ã�ƺ���.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���� ��Ŀ��Ʈ�� ����Ͽ� ��������� ���´�.
//	���� ����	:	2002-05-27 ���� 2:41:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::GetGuildMemberByCharAccount(SI32 siAccount,UI08 uiSlot,strGuildCommonMemberInfo *pInfo)
{
	SI32	i;	

	// ���� ����� �߿� �ִ��� ã�ƺ���.
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

	// ������ �߿� �ִ��� ã�ƺ���.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ����� �˾ƿ´�.(������ ����)
//	���� ����	:	2002-08-12 ���� 7:31:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::IsMember(CHAR *pszName)
{
	SI32	i;	

	// ���� ����� �߿� �ִ��� ã�ƺ���.
	for(i = 0; i  < m_RealMembers.m_NowUseDataCount; i++)	
		if(stricmp(m_RealMembers.m_Data[i].szName, pszName) == 0)			return	TRUE;	

	// ������ �߿� �ִ��� ã�ƺ���.
	for(i = 0; i  < m_Candidates.m_NowUseDataCount; i++)	
		if(stricmp(m_Candidates.m_Data[i].szName, pszName) == 0)			return	TRUE;
		
	return	FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���������� �˾ƿ´�.
//	���� ����	:	2002-12-31 ���� 3:32:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	GuildMemberManager::IsCandidateMember( CHAR *pszName )
{
	SI32	i;
	
	for( i = 0; i < m_Candidates.m_NowUseDataCount; i++ )	
	{
		if( stricmp( m_Candidates.m_Data[ i ].szName, pszName ) == 0 )			return	TRUE;
	}
		
	return	FALSE;		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���������� �˾ƿ´�.
//	���� ����	:	2002-12-31 ���� 3:32:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
strCandidateInfo	*GuildMemberManager::GetCandidateMember( CHAR *pszName )
{
	SI32	i;
	
	for( i = 0; i < m_Candidates.m_NowUseDataCount; i++ )	
	{
		if( stricmp( m_Candidates.m_Data[ i ].szName, pszName ) == 0 )			return	&m_Candidates.m_Data[ i ];
	}
		
	return	NULL;		
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������� ID�� ���´�. ��ܳ��� ������� ������ NULL�� �����Ѵ�.
//	���� ����	:	2002-12-31 ���� 3:32:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
CHAR*	GuildMemberManager::GetDHSID()
{
	SI32	i;
	
	// ���� ����� �߿� ������� �ִ��� ã�ƺ���.
	for( i = 0; i  < m_RealMembers.m_NowUseDataCount; i++ )
	{
		if( m_RealMembers.m_Data[i].siClassInGuild == m_pclIGuildClass->GetDHSCode() )
			return	m_RealMembers.m_Data[i].szName;
	}
	
	return	NULL;		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��峻 �ൿ������ ���� ���´�.
//	���� ����	:	2002-12-31 ���� 3:32:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	GuildMemberManager::GetTotalHNDNum()
{
	SI32	i;
	UI32	uiCount	=	0;
	
	// ���� ����� �߿� ������� �ִ��� ã�ƺ���.
	for( i = 0; i  < m_RealMembers.m_NowUseDataCount; i++ )
	{
		if( m_RealMembers.m_Data[i].siClassInGuild == m_pclIGuildClass->GetHNDCode() )
			uiCount++;
	}
	
	return	uiCount;
}
