// ���� ���� : 2002-02-18 ���� 11:28:08 - ������

#include "SOMain.h"
#include "SOWorld.h"
#include "Guild.h"

SOWorld*	cltGuild::m_pMyWorld	=	NULL;
	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-04-13 ���� 6:08:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltGuild::cltGuild()
{
	Init();			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-04-13 ���� 6:08:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltGuild::~cltGuild()
{
	m_Villages.Release();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ѹ��� �ʱ�ȭ ���ٰ͵��� ���ش�.
//	���� ����	:	2002-04-13 ���� 6:09:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::OnceInit(SI32 siTotalVillageNum)
{
	m_Villages.Create( siTotalVillageNum );	

	m_GuildMemberManager.OnceInit();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �������� �ʱ�ȭ�Ѵ�.
//	���� ����	:	2002-03-29 ���� 1:51:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ��ܿ� �����ڸ� �ʱ�ȭ �Ѵ�.
	m_GuildMemberManager.Init();

	m_siLoginedGuildMemeberNum	=	0;

	SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-05-09 ���� 7:27:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::CreateGuild(UI32 uiArrayIndex, UI32 uiGuildID, CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage)
{
	// ��ܿ� �����ڸ� �ʱ�ȭ �Ѵ�.
	m_GuildMemberManager.Init();

	m_uiGuildID		=	uiGuildID;

	strncpy(m_szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);	
	strncpy(m_szDAIID, pPlayer->szID, ON_ID_LENGTH);
	ZeroMemory(m_siGuildRelation, sizeof(m_siGuildRelation));		

	m_uiArrayIndex						=	uiArrayIndex;		// �迭 �ε����� �����Ѵ�.
	m_siFlagIDInStack					=	-1;

	//���� ���� ����Ʈ�� �ʱ�ȭ ���ش�.
	m_GuildUnit.InitGuildUnit();

	// ����Ʈ�� ����ڸ� �߰��Ѵ�.
	m_GuildMemberManager.AddMember(pPlayer->szID, pPlayer->siClassInGuild, pPlayer->GetMoney() + pPlayer->GetSaving(), 
								pPlayer->IFollower.GetFollowerParameterLevel(0), pPlayer->uiTradeCredit,pPlayer->stAccountInfo.siDBAccount,pPlayer->stAccountInfo.uiSlot);		
	
	// ������ �߰� ��Ų��.
	// �ʱ�ȭ �κ��� "FALSE"�� �����ν�
	// �� ������ �ְ������ڸ� ����� �����ϰ� ���ش�.
	AddVillage(pVillage, FALSE);

	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-05-09 ���� 7:29:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::InitGuild(UI32 uiArrayIndex, UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, UI32 uiGuildMemeberNum, SI32 siFlag, SI32 siFlagIDInStack)
{
	m_uiGuildID		=	uiGuildID;

	strncpy(m_szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);	
	strncpy(m_szDAIID, pszDAIID, ON_ID_LENGTH);	
	m_uiArrayIndex				=	uiArrayIndex;		// �迭 �ε����� �����Ѵ�.		
	m_siFlag					=	siFlag;				// ���.
	m_siFlagIDInStack			=	siFlagIDInStack;		

	// �� ��ܿ� ���� �� �ִ� ��ܿ��� ���� �����Ѵ�.
	//UpdateMaxRealMemberNum();	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB�� ���� ��� ��ܿ��� ������ ���´�.
//	���� ����	:	2002-04-22 ���� 5:03:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::SetMemberInfoFromDB()
{
	m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_GUILD_RESULT, LPVOID(m_uiGuildID), LPVOID(this));
	// ��ܿ����� DB���� �о�Ͱ����� m_GuildMemberManager�� �����Ѵ�.
	return m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETGUILDMEMBER, LPVOID(m_uiGuildID), LPVOID(&m_GuildMemberManager));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-03-05 ���� 11:47:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::DeleteGuild()
{
	SI32		i;	
	BOOL		bDBSuccess;
	
	// DB���� Guild�� �����Ѵ�.		
	bDBSuccess = TRUE;
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_DELETE, &m_uiGuildID, sizeof(m_uiGuildID) ); //**

	m_GuildMemberManager.Init();

	// ���� �� ��ܿ� ���� �ִ� ������ ��� ��ܿ� ������ ���� ���·� ���ش�.
	for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
	{
		if(m_pMyWorld->pclVillage[i].uiGuildID == m_uiGuildID)
			DelVillage(&m_pMyWorld->pclVillage[i], TRUE);
	}

	// for debug
//	m_pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Clan was disorganized!!    Guild Name: %s    GuildID: %d", m_szGuildName, m_uiGuildID );
	writeInfoToFile( "DeletedGuild.txt", "Clan was disorganized!!    Guild Name: %s    GuildID: %d", m_szGuildName, m_uiGuildID );
	// ���� �� ��ܿ� ���� �ִ� �÷��̾���� ��� ��ܿ� ������ ���� ���·� ���ش�.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			if(m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID)
			{
				m_pMyWorld->clPlayer[i]->SetGuild(0);
				m_pMyWorld->clPlayer[i]->SetClassInGuild(0);

				// �޽����� �����ش�.
				m_pMyWorld->clPlayer[i]->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_DELETEGLD));
			}
		}
	}

	// �������� �ʱ�ȭ �Ѵ�.
	Init();

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����� ID�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltGuild::GetGuildID()
{
	return m_uiGuildID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����� �̸��� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
CHAR*	cltGuild::GetGuildName()
{
	return m_szGuildName;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ID(���ڿ�)�� ���´�.
//	���� ����	:	2002-03-04 ���� 12:33:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
CHAR*	cltGuild::GetDAIID()
{
	return m_szDAIID;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ϰ� �ִ� ������ ���� �Ѱ��ش�.
//	���� ����	:	2002-03-04 ���� 12:34:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	cltGuild::GetOwnerVillageNum()
{
	return m_Villages.m_NowUseDataCount;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ȸ������ �Ѱ��ش�.
//	���� ����	:	2002-04-19 ���� 3:11:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltGuild::GetGuildMemberNum()
{
	return m_GuildMemberManager.GetRealMemeberNum();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� ���� ��ܿ��� �߰��Ǿ���.
//	���� ����	:	2002-05-10 ���� 9:44:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::AddGuildMember(CHAR *pszName)
{	
	
	SI32	i;
	CHAR	*pszMaxInvestmentorName;
	BOOL	IsSendChangeClassMsg = FALSE;	
	SI32	siClassInCode;

	strCandidateInfo *pInfo = NULL;

	// ��ܿ��� �߰���Ű���� �÷��̾ ���������� �˻��Ѵ�.
	if( (pInfo = m_GuildMemberManager.GetCandidateMember( pszName )) == NULL )
	{
//		m_pMyWorld->clServer->WriteInfo( "..\\AddGuildMember Failed.txt", "GuildID : [ %u ], Name : [ %s ]", m_uiGuildID, pszName );
		writeInfoToFile( "AddGuildMember Failed.txt", "GuildID : [ %u ], Name : [ %s ]", m_uiGuildID, pszName );
		return	FALSE;
	}


	// ������ �⺻������ ��ȯ�̴�.		
	siClassInCode	=	m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);

	// �� �̻� ������ �� ���ٸ� FALSE�� �����Ѵ�.
	if(m_GuildMemberManager.IsRealMemberFull() == TRUE)	return FALSE;	

	// ���� �����Ϸ��� ĳ���Ͱ� �ٸ� ������ �ְ� �������̶�� �� ������ ���Խ�Ų��.
	for(i = 0; i < m_pMyWorld->siTotalVillageNum; i++)
	{
		pszMaxInvestmentorName	=	m_pMyWorld->pclVillage[i].m_Government->GetInvestmentMaxInvestmentorName();
			
		if(pszMaxInvestmentorName != NULL)
		{
			if(stricmp(pszMaxInvestmentorName, pszName) == 0)
			{
				// �ְ� �������̴�.
				// �ش� ������ �߰� ��Ų��.
				AddVillage(&m_pMyWorld->pclVillage[i], FALSE);								
				//IsSendChangeClassMsg	=	TRUE;
		
				siClassInCode			=	m_pMyWorld->clIGuildClass.GetHNSCode();
			}
		}
	}
	
	// ���� ������ �Ǿ��ٴ°� ������ٸ� �ٽ� ���� ���� �ʾƵ� �ȴ�.
	if(IsSendChangeClassMsg == FALSE)
	{
		if( ChangeClass( pInfo->szName, siClassInCode ) == FALSE )	
			return	FALSE;			
	}

	// �����ڸ� ���� ��ܿ����� �ٲ��ش�.
	m_GuildMemberManager.ChangeClass(pszName, siClassInCode, TRUE);	

	// ���� �ش� ����ڰ� �α������̶�� ������ ��ܿ����� �����Ѵ�.
	if( m_pMyWorld->GetPlayer( pszName ) != NULL )
	{
		GuildMemberLogin();		
	}

	// ��� ��ܿ����� ��� ������ ����Ǿ��ٴ� ���� �˷��ش�.(MS)
	GuildInfoChanged();

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� �����Ǿ���.
//	���� ����	:	2002-04-13 ���� 10:03:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::DelGuildMember(CHAR *pszName, BOOL bCandidate, BOOL bSendMsg, BOOL bKick)
{
	DBIKGuildLeaveInfo			GuildLeaveInfo;	
	SOPlayer					*pPlayer;			

	strGuildCommonMemberInfo    MemberInfo;
	
	// �ش� �÷��̾ �� ��ܿ����� �˾ƿȰ� ���ÿ� ������ ���´�.
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

	// ��ܿ� ���� Ŭ�������Ե� �˷��ش�.
	m_GuildMemberManager.DelMember(pszName, bCandidate);	


	// �������� �� ĳ������ ������ �ִٸ� �����Ѵ�.
	// �����ڰ� �ƴ� ��쿡�� �������ش�.
	if(bCandidate == FALSE) m_pMyWorld->DeleteGuildClassInVillage(pszName);	

	

	// ���� �÷��̾ ���ӵǾ� �ִٸ� �÷��̾��� ����� ������ �Ѵ�.
	if((pPlayer = m_pMyWorld->GetPlayer(pszName)) != NULL)
	{
		// ���� ������ ����� ������ �Ѵ�.
		pPlayer->SetGuild(0);	

		if(bSendMsg == TRUE)
			pPlayer->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_DISMISSAL));		

		// ���ӵ� ��ܿ������� �����Ѵ�.
		GuildMemberLogout();
	}

	// ��� ��ܿ����� ��� ������ ����Ǿ��ٴ� ���� �˷��ش�.(MS)
	GuildInfoChanged();

	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� ������ �߰��Ǿ���.
//	���� ����	:	2002-05-11 ���� 10:17:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::AddVillage(SOVillage *pVillage, BOOL bInit)
{	
	// �ʱ�ȭ���� �ƴϿ��� �Ѵ�.
	if(bInit == FALSE)
	{
		// �ʱ�ȭ���� �ƴҶ����� ���� �߰��Ϸ��� ������ �̹� ��ܿ� ������ �ִ°� �ƴ��� Ȯ���Ѵ�.

		// ��� ID�� ���� ���ٸ� �̹� ������ ���̴�.
		// �� �κ��� �ݵ�� �Ʒ� pVillage->SetOwnerGuild(m_uiGuildID) �ϱ� ���� ���־�� �Ѵ�.
		if(pVillage->uiGuildID == m_uiGuildID)	return;

		// ������ �߰��Ҷ� ����� ���� �������ش�.	
		pVillage->SetOwnerGuild(m_uiGuildID);
		// �ı��� ������ 50%�� �Ǹ� ä�� �߰��Ѵ�.		
		pVillage->AutoRestorationVillageDefence(TRUE);
	}

	// ������ �Ҽӽ�Ų��.
	m_Villages.Add(pVillage->uiVillageCode);

	// �ִ� ��ܿ��� ���� �����Ѵ�.
	//UpdateMaxRealMemberNum();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� ������ �����Ǿ���.
//	���� ����	:	2002-05-09 ���� 8:00:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::DelVillage(SOVillage *pVillage, BOOL bDeleteGuild)
{
	pVillage->DeleteGuild();	

	// �Ҽӵ� �������� �����Ѵ�.
	m_Villages.DeleteType(pVillage->uiVillageCode);

	// �ִ� ��ܿ��� ���� �����Ѵ�.
	//UpdateMaxRealMemberNum();

	// ���� ��带 �������̸� 
	// ����� ���ֱ� ���Ͽ� ���� ���� üũ���� �ʴ´�.
	if(bDeleteGuild == FALSE)
	{
		// �� �̻� �����ϰ� �ִ� ������ ���ٸ�
		// ��带 �����Ѵ�.
		if(m_Villages.m_NowUseDataCount <= 0)
		{
			// ����� ���ش�.		
			return	TRUE;	
		}
	}	

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ �ʱ�ȭ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::InitStaticVariable(SOWorld *pWorld)
{
	m_pMyWorld	=	pWorld;
	
	GuildMemberManager::InitStaticVariable( &(m_pMyWorld->clIGuildClass) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� ������� �˾ƿ´�.
//	���� ����	:	2002-02-25 ���� 9:17:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::IsValidGuild()
{
	if(m_uiGuildID != 0)		return TRUE;
	else						return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���������� �����ش�.
//	���� ����	:	2002-03-26 ���� 3:30:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::Notice(CHAR *pszName, CHAR *pszNotice)
{	
	CHAR	szNotice[1024];

	if(pszNotice == NULL)	return FALSE;

	// ����̸�.
	sprintf(szNotice, "[%s] : %s", pszName, pszNotice);

	// ���� �� ��ܿ� ���� �ִ� �÷��̾�鿡�� �޽����� �����ش�.
	SendMsgToGuildMemebers(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_GUILD), LPVOID(szNotice));	

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ִ� ��ܿ����� �޽����� �����ش�.
//	���� ����	:	2002-03-16 ���� 10:32:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::SendMsgToGuildMemebers(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5)
{
	SI32		i;		

	// ���� �� ��ܿ� ���� �ִ� �÷��̾�鿡�� �޽����� �����ش�.
	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
		{
			// �ش� �����̰� 
			// �����ڰ� �ƴϿ��� �Ѵ�.
			if((m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID) && 
				(m_pMyWorld->clPlayer[i]->siClassInGuild != m_pMyWorld->clIGuildClass.GetJWJCode()))			
				m_pMyWorld->clPlayer[i]->SendFieldMsg(siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5);			
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�迭 �ε����� ����.
//	���� ����	:	2002-03-16 ���� 2:49:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltGuild::GetArrayIndex()
{
	return	m_uiArrayIndex;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-03-21 ���� 3:36:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::SetHNS(SOVillage *pVillage)
{	
	CHAR	*pszHNSID;
	
	// ������ �����Ͱ� ��ȿ���� �˾ƿ´�.
	if(pVillage == NULL)	return	FALSE;

	// �ְ� �������� �̸��� ���´�.
	if((pszHNSID = pVillage->m_Government->GetInvestmentMaxInvestmentorName()) == NULL)	return	FALSE;


	
	// ����� �����Ѵ�.
	// ����� �̸��� ������ ���Ͽ� ���� �������ش�.
	pVillage->SetHNSID(pszHNSID);

	return	TRUE;
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ϰ� �ִ� ������ ���� �����Ѵ�.
//	���� ����	:	2002-03-29 ���� 1:48:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::UpdateMaxRealMemberNum()
{		
	// ������ �ִ� ��ܿ����� ���Ѵ�.
	m_GuildMemberManager.SetMaxRealMemberNum(min(ON_GUILD_MIN_MEMBER_NUM + ((m_Villages.m_NowUseDataCount - 1) * ON_GUILD_ADD_MEMBER_NUM_PER_ONEVILLAGE), ON_GUILD_MAX_MEMBER_NUM));	
}
*/

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ����� ������ �� �ִ� �ִ� ��ܿ� ���� ���´�.
//	���� ����	:	2002-03-29 ���� 1:53:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltGuild::GetMaxGuildMemebersNum()
{
	return m_GuildMemberManager.GetMaxRealMemberNum();
}
*/

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-04-04 ���� 3:26:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::ChangeFlag(SI32 siFlag)
{
	BOOL			bDBSuccess;
	SI32			i;
	

	// �ش� �÷��׸� �ٸ� ��ܿ��� ���� �ִ��� �˻��Ѵ�
	if(m_pMyWorld->clIGuild.IsUsedFlag(siFlag) == TRUE)	return	FALSE;

	m_siFlag	=	siFlag;

	

	bDBSuccess = TRUE;
	

	if(bDBSuccess)
	{
		// �� ��ܿ� �Ҽӵ� ��� �÷��̾��� ����� �����Ѵ�.
		for(i = 0; i < ON_MAX_CONNECTION; i++)
		{
			if(m_pMyWorld->clPlayer[i]->uiState != ON_STATE_LOGOUT)
			{
				// �ش� �����̰� 
				// �����ڰ� �ƴϿ��� �Ѵ�.
				if((m_pMyWorld->clPlayer[i]->uiGuildID == m_uiGuildID) && (m_pMyWorld->clPlayer[i]->siClassInGuild != m_pMyWorld->clIGuildClass.GetJWJCode()))			
					m_pMyWorld->clPlayer[i]->ChangeGuildFlag(siFlag);					
			}
		}		

		// �� ��ܿ� �Ҽӵ� ��� ������ ����� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���´�.
//	���� ����	:	2002-04-04 ���� 11:48:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltGuild::GetFlag()
{	
	return	m_siFlag;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ÿ����� ��� ID�� �����Ѵ�.
//	���� ����	:	2002-04-04 ���� 2:59:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::SetFlagIDInStack(SI32 siFlagIDInStack)
{
	m_siFlagIDInStack	=	siFlagIDInStack;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ÿ����� ��� ID�� ���´�.
//	���� ����	:	2002-04-04 ���� 2:36:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltGuild::GetFlagIDInStack()
{
	return m_siFlagIDInStack;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ڸ� �߰��Ѵ�.
//	���� ����	:	2002-04-13 ���� 9:37:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::AddCandidate(SOPlayer *pPlayer)
{	
	DBIKGuildJoinInfo			GuildJoinInfo;
	SI32						siLevel;		
	
	// DB�� �����Ѵ�.	
	ZeroMemory(&GuildJoinInfo, sizeof(GuildJoinInfo));
	GuildJoinInfo.uiGuildID		=	m_uiGuildID;	
	GuildJoinInfo.uiClass		=	m_pMyWorld->clIGuildClass.GetJWJCode();				

	GuildJoinInfo.siAccount     =   pPlayer->stAccountInfo.siDBAccount;
	GuildJoinInfo.uiSlot        =   pPlayer->stAccountInfo.uiSlot;

	
	siLevel		=	pPlayer->IFollower.GetFollowerParameterLevel(0);
	
	// ��ܿ� ���� Ŭ�������Ե� �˷��ش�.
	if(m_GuildMemberManager.AddMember(pPlayer->szID,GuildJoinInfo.uiClass, pPlayer->GetMoney() + pPlayer->GetSaving(),siLevel, pPlayer->uiTradeCredit,pPlayer->stAccountInfo.siDBAccount,pPlayer->stAccountInfo.uiSlot) == TRUE)
	{
		// �÷��̾��� ����� �����Ѵ�.
		pPlayer->SetGuild( m_uiGuildID );
		pPlayer->SetClassInGuild( GuildJoinInfo.uiClass );

		// ���� �������� ���������� ���� ���� ��쿡�� DB�� �����Ѵ�.
		m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_JOIN, &GuildJoinInfo, sizeof(GuildJoinInfo));

		return	TRUE;
	}
	else
		return	FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ų��.
//	���� ����	:	2002-04-13 ���� 9:58:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::ChangeClass( CHAR *pszName, SI32 siNewClass )
{
	DBIKGuildChangeClassInfo	GuildChangeClassInfo;	
	SOPlayer					*pPlayer = NULL;

	// ���� ��� ������� �˾ƿ´�.
	strGuildCommonMemberInfo MemberInfo;

	if( m_GuildMemberManager.GetGuildMemberByCharName(pszName, &MemberInfo) == FALSE )
	{
		return FALSE;
	}

	// by semarine
	m_GuildMemberManager.ChangeClass( pszName, siNewClass, FALSE );
	
	// DB�� �����Ѵ�.
	GuildChangeClassInfo.siDBAccount	=	MemberInfo.siAccount;
	GuildChangeClassInfo.uiSlot			=	MemberInfo.uiSlot;

	GuildChangeClassInfo.uiGuildID		=	m_uiGuildID;		
	GuildChangeClassInfo.uiChangeClass	=	siNewClass;

	
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_CHANGE_CLASS, &GuildChangeClassInfo, sizeof(GuildChangeClassInfo) );

	// ���� ���ӵǾ� �ִ��� �˾ƿ´�	
	// ���� ���ӵǾ� �ִٸ� �˷��ش�.
	if( (pPlayer = m_pMyWorld->GetPlayerByDBAccount(MemberInfo.siAccount,MemberInfo.uiSlot)) != NULL)
	{
		// ���ο� �������� �������ش�.
		pPlayer->SetClassInGuild( siNewClass );
		// �޽����� �����ش�.
		pPlayer->SendFieldMsg(ON_GUILD_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_GLD_CHANGECLASS), LPVOID(siNewClass));	
	}

	// ��� ��ܿ����� ��� ������ ����Ǿ��ٴ� ���� �˷��ش�.(MS)
	GuildInfoChanged();
	
	return	TRUE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� ������ �Ҽӵ� ������ �ڵ带 ���´�.
//	���� ����	:	2002-04-13 ���� 6:32:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::GetOwnerVillages(UI16 *puiOwnerVillageNum, UI16 *puiVillageCode)
{
	*puiOwnerVillageNum	=	m_Villages.m_NowUseDataCount;	
	memcpy(puiVillageCode, m_Villages.m_Data, sizeof(UI16) * m_Villages.m_NowUseDataCount);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� ������ �Ҽӵ� ������ �⺻ ������ ���´�.
//	���� ����	:	2002-11-27 ���� 2:45:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ��ܿ��� ����Ʈ�� ���´�.
//	���� ����	:	2002-04-22 ���� 11:36:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltGuild::GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo)
{
	return	m_GuildMemberManager.GetRMList(dwPage, pGuildMemeberInfo);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ����Ʈ�� ���´�.
//	���� ����	:	2002-04-22 ���� 2:32:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltGuild::GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo)
{
	return	m_GuildMemberManager.GetCMList(dwPage, pCandidateInfo);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ü ���� ��ܿ��� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
//	���� ����	:	2002-04-22 ���� 2:31:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	cltGuild::GetRMTotalPageNum()
{
	return m_GuildMemberManager.GetRMTotalPageNum();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ü �����ڸ� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
//	���� ����	:	2002-04-22 ���� 2:31:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	cltGuild::GetCMTotalPageNum()
{
	return m_GuildMemberManager.GetCMTotalPageNum();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���� �̸��� ����Ͽ� ����� ID�� ��� ������ ���´�.
//	���� ����	:	2002-05-27 ���� 2:40:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �� Player �ν��Ͻ��� ��� ���� ������ �˷��ش�.
//	���� ����	:	2004-01-05 PM __Lee_min_su__
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

			// �� ����� Player �ν��Ͻ����� ��������� ����Ǿ����� 
			// �˸���.
			pPlayer->SetGuildInfoChanged();
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ��´�.
//	���� ����	:	2004-01-05 PM __Lee_min_su__
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
			// ��ȯ�� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-03-16 ���� 2:49:32 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuild::InitRelation(UI32 uiArrayIndex)
{
	// ������ ���� 0���� ������ �ϸ� �ȴ�.
	m_siGuildRelation[uiArrayIndex]	=	0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �������� �˾ƿ´�.
//	���� ����	:	2002-03-19 ���� 3:00:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::IsEnemyGuild(UI32 uiArrayIndex)
{
	if(m_siGuildRelation[uiArrayIndex] == ON_GUILD_RELATION_ENEMY)
		return	TRUE;
	else
		return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ����(���� ����� ���� 3���� ���� ��)���� �˾ƿ´�.
//	���� ����	:	2002-03-19 ���� 3:00:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltGuild::IsWarGuild(UI32 uiArrayIndex)
{
	if(m_siGuildRelation[uiArrayIndex] == ON_GUILD_RELATION_WAR)
		return TRUE;
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 �����Ѵ�.
//	���� ����	:	2002-03-15 ���� 5:17:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// ���ͱ����� �����ߴ�.						
		GuildRelation.siRelation	=	ON_GUILD_RELATION_ALLY;	
		break;
	case ON_GUILD_RELATION_ENEMY:
		{
			// ���뱹���� �����ߴ�.	
			// ���븦 �� ��¥�� �����ؼ� �����ֿ� ���δ�.
			GuildRelation.siRelation	=	ON_GUILD_RELATION_ENEMY;

			if(siDbTime != 0)
			{
				//DB���� ��¥�� �޾ƿ��� ���.
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
		// �߸��� �Ǿ���.
		GuildRelation.siRelation	=	ON_GUILD_RELATION_NEUTRAL;		
		break;
	}

	// �ʱ�ȭ�� �ƴ� ������ DB�� �����ϰ�
	// �޽����� �����ش�.
	if(bInit == FALSE)
	{
		siNowYear					=	m_pMyWorld->RealWorldTime.uiYear;
		siNowMon					=	m_pMyWorld->RealWorldTime.uiMonth;
		siNowDay					=	m_pMyWorld->RealWorldTime.uiDay;

		sprintf(szTemp,"%04d%02d%02d",siNowYear,siNowMon,siNowDay);
		GuildRelation.siTime		=	atoi(szTemp);	

		// DB�� ���������� �����ߴ��� �˾ƿ´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 ���´�.
//	���� ����	:	2002-03-15 ���� 5:21:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltGuild::GetRelation(UI32 uiArrayIndex)
{
	return	m_siGuildRelation[uiArrayIndex];
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 �缳���Ѵ�.(���� 12�ø���)
//	���� ����	:	2002-03-15 ���� 5:17:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

			//���� ���� 3���� �����ٸ� �������� �ٲ��ش�.
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

		TempGuildFrieldlyMessageOffer.uiGuildID1	=	pGuild->m_uiGuildID;	//���� ������ ����� ���̵�.
		TempGuildFrieldlyMessageOffer.uiGuildID2	=	m_uiGuildID;			//�� ���̵�.
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

	//ȭģ���Ǹ� �޾Ƶ鿴�� ��� ���� ��ܿ� ���õ� ������ ��� �����ش�.
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

	//������ ��� �Ұ����� �ٸ���ܰ� �˻�...
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
	//ȭģ���Ǹ� �޾Ƶ����� �ʾ��� ��� �޼����� �����ش�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��濡�� ���� ����� ���� ���� ����ǰ�� �ش�.
//	���� ����	:	2003-01-03 ���� 11:01:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//VOID	cltGuild::RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode ,SI16 siRewardSupply)
VOID	cltGuild::RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode)
{
	SOPlayer	*pPlayer;
	MONEY		TempMoney;

	// ���� ����� �α��� ���̸� ���ݾ��� �������ش�.
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
		
		// ��ü ��ܿ����� �޽����� �����ش�.
		SendMsgToGuildMemebers( ON_OFFICIAL_ANNOUNCEMENT, SO_SFM_ONLYME, 
							LPVOID( &OADAIGetMoneyByDestroyVillageMsg  ), 
							LPVOID( sizeof( OADAIGetMoneyByDestroyVillageMsg ) ) );
	}
	else //����� �α׾ƿ� �����̴�.
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
		

		// ��ü ��ܿ����� �޽����� �����ش�.
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

	//������ �ϴ� ������ �ƴϸ� false	
//	if(m_pMyWorld->pRealTime->tm_wday == 2 || m_pMyWorld->pRealTime->tm_wday == 4)	
	if(m_pMyWorld->m_WarTimeParser.IsWarDay(m_pMyWorld->pRealTime->tm_wday))
	{
		if(m_pMyWorld->pRealTime->tm_year + 1900 > siYear) return TRUE;
		if(m_pMyWorld->pRealTime->tm_mon + 1 > siMon ) return TRUE;

//		SI16 siCheck = m_pMyWorld->pRealTime->tm_yday - siDay;
		SI16 siCheck = m_pMyWorld->pRealTime->tm_mday - siDay;
		if(siCheck <= 1) return bChek;

		//����� ������ ���Ѵ�.
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

//ȭģ���� ������ �ٸ���ܰ� ������ �ִ��� �˻縦 �ؼ� ������ ���̻� ������ ���ٰ� �˷��ش�.
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

//���� �ð��� �Ǹ� ������ �ִ� ����� ��ܿ����� �˷��ش�.
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

//�������� ������ �����Ѵ�.
VOID cltGuild::SetGiveUpInfo(UI16 uiVillageCode, MONEY mnMoney, SI16 siFlag)
{
	if(uiVillageCode < 0) return;
	if(mnMoney < 0) return;

	m_GiveUpInfo.uiVillageCode = uiVillageCode;
	m_GiveUpInfo.mnMoney       = mnMoney;
	m_GiveUpInfo.siGiveUpFlag  = siFlag;
}

//�������� ������ �����´�.
VOID cltGuild::GetGiveUpInfo(UI16 *puiVillageCode, MONEY *pmnMoney)
{
	if(puiVillageCode == NULL || pmnMoney == NULL )return;

	*puiVillageCode = m_GiveUpInfo.uiVillageCode;
	*pmnMoney       = m_GiveUpInfo.mnMoney;
}

//�������� ������ �÷��׸� �����Ѵ�.
VOID cltGuild::SetGiveUpFlag(SI16 siFlag)
{
	if(siFlag < ON_GUILD_GIVEUP_NORMAL || siFlag > ON_GUILD_GIVEUP_NO) return;
	m_GiveUpInfo.siGiveUpFlag  = siFlag;
}

//�������� ������ �÷��׸� �����´�.
SI16 cltGuild::GetGiveUpFlag()
{
	return m_GiveUpInfo.siGiveUpFlag;
}

//�ٸ���ܰ��� ���踦 ����� ��������ش�.
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

//��ܿ��鿡�� ������ �����ߴٰ� �˷��ش�.
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
//			if(m_pMyWorld->m_WarTimeParser.IsWarTime(m_pMyWorld->pRealTime->tm_hour)) //�����ð��ΰ�?
			{
				if(pPlayer->IsFieldAttackUnit()) //���������ΰ�
					pPlayer->ChangeNormalChar();
			}
		}
	}
}