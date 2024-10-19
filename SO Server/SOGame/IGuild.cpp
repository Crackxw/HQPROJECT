#include "SOMain.h"
#include "SOWorld.h"
#include "IGuild.h"
#include "SODatabase.h"
#include "OnlineCommonStruct.h"

SOWorld*	cltIGuild::m_pMyWorld	=	NULL;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-02-25 ���� 9:14:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltIGuild::cltIGuild()
{
	m_Flags.Create(SO_TOTAL_GUILD_NUM);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-02-25 ���� 9:14:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltIGuild::~cltIGuild()
{
	Free();	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ѹ��� �ʱ�ȭ ���ٰ͵��� ���ش�.
//	���� ����	:	2002-04-13 ���� 6:13:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::OnceInit(SI32 siTotalVillageNum)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		// �ѹ��� �ʱ�ȭ ���ٰ͵��� ���ش�.
		m_Guilds[i].OnceInit(siTotalVillageNum);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�޸𸮸� �����Ѵ�.
//	���� ����	:	2002-02-26 ���� 1:19:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::Free()
{	
	m_Flags.Release();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �ʿ��� �ݾ��� ���´�.
//	���� ����	:	2003-01-27 ���� 2:24:10 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MONEY	cltIGuild::GetGuildCreateCost()
{
	// min(30000000, 2000000 + (��ܰ���ȸ�� *20000) )
	// ��ܰ���ȸ�� : ���尡 ���� ���ķ� ���ݱ��� ����� ������ ���� ȸ��.
	// ��� ���� ����� �ְ� 3õ������ �ѱ� �� ����.

	return min( (MONEY)30000000, (MONEY)(2000000 + (m_uiMaxGuildID * 20000)) );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-02-25 ���� 9:14:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltIGuild::CreateGuild(CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage)
{
	SI32					siEmptySlot;		
	DBIKGuildCreateInfo		GuildCreateInfo;		
	UI32					uiNewGuildID;

	SI32					i;

	

	if((siEmptySlot	= FindEmptySlot()) == -1)		return	0;
	
	// ����� �̸��� ������ �ȵȴ�.
	if(pszGuildName[0] == NULL)						return	0;
	if(pVillage == NULL)							return	0;		

	ZeroMemory(&GuildCreateInfo, sizeof(GuildCreateInfo));

	// ����� �̸��� �����Ѵ�.
	GuildCreateInfo.siAccount = pPlayer->stAccountInfo.siDBAccount;
	GuildCreateInfo.uiSlot = pPlayer->stAccountInfo.uiSlot;

	strncpy(GuildCreateInfo.szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);		
	
	GuildCreateInfo.uiVillageCode	=	pVillage->uiVillageCode;	

	//GuildCreateInfo.moMoney = ON_GUILD_COST_CREATE_GUILD;
	GuildCreateInfo.moMoney = GetGuildCreateCost();

	
	// �ִ� �����̵� �ϳ� �������� �� ����� ���̵�� ��� (�̷����ؼ� ������ �ѹ����������� ���� id�� ���� ���� 2���̻� �������ʴ´�.)
	m_uiMaxGuildID++;
	uiNewGuildID = m_uiMaxGuildID;
	GuildCreateInfo.uiGuildID = uiNewGuildID;

	// DB���� ��带 �����Ѵ�.
	// ����� �����Ѵ�.
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_CREATE, LPVOID(&GuildCreateInfo), sizeof(GuildCreateInfo));
	
	// ����� �����Ѵ�.
	pPlayer->SetGuild(uiNewGuildID);
	
	// ������ �����Ѵ�.
	pPlayer->SetClassInGuild(m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI));

	m_Guilds[siEmptySlot].CreateGuild(siEmptySlot, uiNewGuildID, pszGuildName, pPlayer, pVillage);	

	// ��尣�� ������ �Ѵ�
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{			
			// ó������ ���ΰ��� �߸����̴�.
			if(i != siEmptySlot)
			{
				SetRelationByIndex(siEmptySlot, i, ON_GUILD_RELATION_NEUTRAL, FALSE,0);			
				SetRelationByIndex(i, siEmptySlot, ON_GUILD_RELATION_NEUTRAL, FALSE,0);
			}			
		}	
	}

	// ����� ���� �Ǿ��ٴ� ���� ������ �����ش�.
	m_pMyWorld->OfficialAnnouncement(ON_RET_OA_GUILD_CREATED, LPARAM(pszGuildName));

	return uiNewGuildID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-02-25 ���� 9:14:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

		// ����� �ִٸ� ��� ����Ʈ�� �����Ѵ�.
		siFlagIDInStack			=	m_Flags.Add(stFlagInfo);
	}

	// ��带 �ʱ�ȭ �Ѵ�.
	m_Guilds[siEmptySlot].InitGuild(siEmptySlot, uiGuildID, pszGuildName, pszDAIID, uiGuildMemeberNum, siFlag, siFlagIDInStack);	

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.	
//	���� ����	:	2002-02-25 ���� 9:14:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::DeleteGuild(UI32 uiGuildID,SOVillage* pVillage)
{
	SI32			siFoundSlot;
	SI32			i = 0;	
	strFlagInfo		stFlagInfo;	
	CHAR			szGuildName[ON_GUILDNAME_LENGTH + 1];

	siFoundSlot	=	FindSlot(uiGuildID);

	if(siFoundSlot == -1 )	return FALSE;

	//�������� ����� ��ü�� �ȵȴ�.
//	if(m_Guilds[siFoundSlot].IsWar()) return FALSE;

	
	// �����Ϸ��� ������ ���踦 ��� �ʱ�ȭ�Ѵ�.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{	
		if(i == siFoundSlot)	continue;

		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(siFoundSlot != i)
				m_Guilds[i].InitRelation(siFoundSlot);
		}
	}
	//��ܿ� �ִ� �������� �����Ѵ�.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++ )
	{
		if(pVillage->m_stAttackGuild[i].uiGuildID != 0) continue;

		if(pVillage->m_stAttackGuild[i].uiGuildID == uiGuildID)
		{
			ZeroMemory(&pVillage->m_stAttackGuild[i],sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);
		}
	}
	//��ܿ� ������� ������.
	ZeroMemory(m_Guilds[siFoundSlot].m_stGuildResult,sizeof(cltGuildResult) * ON_MAX_GUILD_RESULT);
	//��ܿ� �Ҽӵ� ��� ������ ������.
	ZeroMemory(&m_Guilds[siFoundSlot].m_GuildUnit,sizeof(cltGuildUnit));

	m_Guilds[siFoundSlot].SendGuildItemDeleteMessage();

	stFlagInfo.uiGuildID	=	uiGuildID;
	stFlagInfo.siFlag		=	m_Guilds[siFoundSlot].GetFlag();

	// ��尡 ���� �Ǳ����� �̸��� ������ �д�
	ZeroMemory(szGuildName, sizeof(szGuildName));
	strncpy(szGuildName, m_Guilds[siFoundSlot].GetGuildName(), ON_GUILDNAME_LENGTH);

	if(m_Guilds[siFoundSlot].DeleteGuild() == TRUE)
	{		
		if(stFlagInfo.siFlag != -1)
			m_Flags.DeleteType(stFlagInfo);

		// ����� ��ü �Ǿ��ٴ� ���� ������ �����ش�.
		m_pMyWorld->OfficialAnnouncement(ON_RET_OA_GUILD_DELETED, LPARAM(szGuildName));				
			
		return	TRUE;
	}
	else
		return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �ִ� ������ ã�´�.
//	���� ����	:	2002-02-25 ���� 9:19:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ã�´�.	
//	���� ����	:	2002-02-25 ���� 9:25:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ã�´�.
//	���� ����	:	2002-03-15 ���� 5:23:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ã�´�.	
//	���� ����	:	2002-03-05 ���� 11:01:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� ��� ����� �߰��Ǿ���.
//	���� ����	:	2002-03-04 ���� 12:49:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::AddGuildMember(UI32 uiGuildID, CHAR *pszName)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
		return m_Guilds[siFoundSlot].AddGuildMember(pszName);			
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� ��� ����� �����Ǿ���.	
//	���� ����	:	2002-03-04 ���� 12:49:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
/*BOOL	cltIGuild::DelGuildMember(UI32 uiGuildID, SOPlayer *pPlayer, BOOL bCandidate)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].DelGuildMember(pPlayer->szID, bCandidate, bSendMsg);	
	}	

	return FALSE;
}*/

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� ��� ����� �����Ǿ���.
//	���� ����	:	2002-03-06 ���� 1:22:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::DelGuildMember(UI32 uiGuildID, CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].DelGuildMember(pszName, bCandidate, bSendMsg,bKick);	
	}	

	return FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ڸ� �߰��Ѵ�.
//	���� ����	:	2002-04-13 ���� 9:45:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::AddCandidate(UI32 uiGuildID, SOPlayer *pPlayer)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].AddCandidate(pPlayer);	
	}	

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� ������ �߰��Ǿ���.
//	���� ����	:	2002-03-04 ���� 1:45:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::AddVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bInit)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
	{
		m_Guilds[siFoundSlot].AddVillage(pVillage, bInit);			

		if(bInit == FALSE)
		{
			// �ʱ�ȭ���� �ƴҶ����� ���� ������ �����ش�.
			
			// ������ �ش� ��ܿ� �ҼӵǾ��ٴ� �޽����� �����ش�.
			m_pMyWorld->OfficialAnnouncement(ON_RET_OA_VILL_OCCUPIED, LPARAM(m_Guilds[siFoundSlot].GetGuildName()), LPARAM(pVillage->uiVillageCode));
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ��� ������ �����Ǿ���.
//	���� ����	:	2002-03-04 ���� 12:55:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
			// �� �̻� �� ��ܿ��� ������ ����. ����� �����Ѵ�.
			DeleteGuild(uiGuildID, pVillage);

			pVillage->m_bIsFall        = FALSE;
			pVillage->m_bVillageAttack = FALSE;
			
		}
		else //�������� �Ұ����� �����.
		{
			if(m_pMyWorld->m_WarTimeParser.IsWarCheck(m_pMyWorld->pRealTime->tm_wday, m_pMyWorld->pRealTime->tm_hour)) return;
			if(m_pMyWorld->m_WarTimeParser.IsWarTime(m_pMyWorld->pRealTime->tm_hour)) return;
			if(m_Guilds[siFoundSlot].GetGiveUpFlag() == ON_GUILD_GIVEUP_OK) return;

			SOPlayer *pPlayer = m_pMyWorld->GetPlayer( m_Guilds[siFoundSlot].GetDAIID());			
			UI16 uiVillageCode = pVillage->uiVillageCode; //���� �ڵ�
			SI32 siBestIndex = pVillage->m_Government->GetOldBestInvestIndex(); //������ �ε��� 
			MONEY mnMoney = pVillage->m_Government->DelInvestMoney(siBestIndex, 1, FALSE); //�������� ���

			if(pPlayer != NULL)//����� �α����� �ߴ°�?
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-02-26 ���� 9:27:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::InitStaticVariable(SOWorld *pWorld)
{
	m_pMyWorld	=	pWorld;
	cltGuild::InitStaticVariable(pWorld);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ����� �� ���ִ��� ����.
//	���� ����	:	2002-02-26 ���� 5:09:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::IsGuildFull()
{
	if(FindEmptySlot() == -1)	return TRUE;
	else						return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� ID�� ����Ͽ� Guild�� ���´�.
//	���� ����	:	2002-03-16 ���� 3:12:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� ���´�.
//	���� ����	:	2002-03-04 ���� 11:45:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltGuild*	cltIGuild::GetGuildByIndex(SI32 siIndex)
{
	// siIndex�� ��尡 ��ȿ���� ����.
	if(m_Guilds[siIndex].IsValidGuild() == FALSE)	return	NULL;
	else	
		return	&m_Guilds[siIndex];
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� �̸��� ����Ͽ� Guild�� ���´�.
//	���� ����	:	2002-04-22 ���� 10:22:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� �̸��� ���´�.
//	���� ����	:	2002-02-26 ���� 5:09:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� ID�� ���´�.
//	���� ����	:	2002-03-05 ���� 10:59:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Char�� �̸��� ����Ͽ� Guild ID�� ���´�. 
//					ã�µ� �����ߴٸ� TRUE. �����ߴٸ� FALSE.
//	���� ����	:	2002-05-27 ���� 2:37:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass)
{	
	SI32			i;

	if(pszName == NULL)	return 0;

	// �޸𸮿��� ���´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ų��.
//	���� ����	:	2002-03-06 ���� 1:43:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::ChangeClass( UI32 uiGuildID, CHAR *pszName, SI32 siNewClass )
{	
	
	SI32						siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) != -1 )	
	{
		
		return m_Guilds[ siFoundSlot ].ChangeClass( pszName, siNewClass );	
	}	

	return FALSE;


}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���������� �����ش�.
//	���� ����	:	2002-03-26 ���� 3:30:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::Notice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
		return m_Guilds[siFoundSlot].Notice(pszName, pszNotice);	
	else
		return FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::SetRelationByIndex(SI32 siGuildIndex1, SI32 siGuildIndex2, SI32 siRelation, BOOL bInit,SI32 siTime)
{	
	m_Guilds[siGuildIndex1].SetRelation(&m_Guilds[siGuildIndex2], siRelation, bInit, siTime);	
	m_Guilds[siGuildIndex2].SetRelation(&m_Guilds[siGuildIndex1], siRelation, TRUE, siTime);
/*
	switch(siRelation)
	{
	case ON_GUILD_RELATION_ENEMY:		
		// ������ �������� ���踦 ������ ��� ���� �ڵ����� ������ �������� ���谡 �ȴ�.
		m_Guilds[siGuildIndex2].SetRelation(&m_Guilds[siGuildIndex1], siRelation, bInit);	
		break;
	}
*/
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 �����Ѵ�.
//	���� ����	:	2002-03-15 ���� 5:16:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���踦 ���´�.
//	���� ����	:	2002-03-15 ���� 5:19:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� Index�� ID�� ���´�.
//	���� ����	:	2002-03-16 ���� 2:10:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltIGuild::GetGuildIDByIndex(SI32 siIndex)
{
	if(m_Guilds[siIndex].IsValidGuild() == TRUE)
		return m_Guilds[siIndex].GetGuildID();
	else
		return	0;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB���� ����� ���踦 �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::SetGuildRelationFromDB()
{
	SI32	i, j;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			// �켱 ��� ��ܰ��� ���踦 '�߸�'���� �������ش�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	uiGuildID2�� uiGuildID1�� ������ �˾ƿ´�.
//	���� ����	:	2002-03-19 ���� 2:56:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	uiGuildID2�� uiGuildID1�� ������ �˾ƿ´�. ���ΰ� ����������� �˾ƿ´�.
//	���� ����	:	2002-03-19 ���� 2:56:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������������˾ƿ´�.
//	���� ����	:	2002-03-21 ���� 2:15:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ���迡 ���õ� ���� Define�� ���ۿ� Define���� �����Ѵ�.
//	���� ����	:	2002-03-21 ���� 1:55:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ۿ� Define�� ��� ���迡 ���õ� ���� Define���� �����Ѵ�.
//	���� ����	:	2002-03-21 ���� 1:55:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-03-21 ���� 3:39:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::SetHNS(SOVillage *pVillage)
{
	SI32	siFoundSlot;

	// GuildID�� ��ȿ���� ����.
	if(pVillage->uiGuildID == 0)							return	FALSE;
	if((siFoundSlot = FindSlot(pVillage->uiGuildID)) == -1)	return	FALSE;

	// ��� Class�� ����� �����Ѵ�.
	m_Guilds[siFoundSlot].SetHNS(pVillage);
	return	TRUE;
}
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ����� ������ �� �ִ� �ִ� ��ܿ� ���� ���´�.
//	���� ����	:	2002-03-29 ���� 1:56:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltIGuild::GetMaxGuildMemebersNum(UI32 uiGuildID)
{	
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetMaxGuildMemebersNum();
}
*/
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���´�.
//	���� ����	:	2002-04-04 ���� 1:38:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltIGuild::GetFlag(UI32 uiGuildID)
{
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetFlag();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ�.
//	���� ����	:	2002-04-04 ���� 1:38:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIGuild::SetFlag(UI32 uiGuildID, SI32 siFlag)
{
	SI32			siFoundSlot;	
	//SI32			siFlagIDInStack;
	strFlagInfo		stFlagInfo;	

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;	

	// ������ �ϴ� �÷����� ������ �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ����� ����Ʈ�� ���´�.
//	���� ����	:	2002-04-04 ���� 2:41:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::GetUsedFlagList(SI32 *psiFlagNum, strFlagInfo *pstFlagInfo)
{
	*psiFlagNum	=	m_Flags.m_NowUseDataCount;		

	memcpy(pstFlagInfo, m_Flags.m_Data, m_Flags.m_NowUseDataCount * sizeof(strFlagInfo));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ϴ� ������� �˾ƿ´�.
//	���� ����	:	2002-04-11 ���� 5:00:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ϴ� �̸����� �˾ƿ´�.
//	���� ����	:	2003-01-7 ���� 5:00:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB�� ���� ��� ��ܿ��� ������ ���´�.
//	���� ����	:	2002-04-22 ���� 5:07:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ����� ���� ���� ȸ�� ���� ���´�.
//	���� ����	:	2002-05-09 ���� 2:58:20 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltIGuild::GetGuildMemberNum(UI32 uiGuildID)
{
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetGuildMemberNum();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ִ� ��ܿ����� �޽����� �����ش�.
//	���� ����	:	2002-12-29 ���� 7:14:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::SendMsgToGuildMemebers( UI32 uiGuildID, SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5 )
{
	SI32	siFoundSlot;

	if( (siFoundSlot = FindSlot( uiGuildID ) ) == -1 )	return;

	m_Guilds[ siFoundSlot ].SendMsgToGuildMemebers( siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5 );
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ܿ� �ҼӵǴ� ������ �˷��ش�.
//	���� ����	:	2002-02-27 ���� 3:53:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIGuild::AddVillage(UI32 uiGuildID, UI16 uiVillageCode)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].AddVillage(uiVillageCode);
	}
}*/


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Guild�� ID�� Index�� ���´�.
//	���� ����	:	2003-01-01 ���� 7:07:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltIGuild::GetGuildIndexByID( UI32 uiGuildID )
{
	return	FindSlot( uiGuildID );	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	
//	���� ����	:	2003-01-01 ���� 7:10:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//BOOL	cltIGuild::RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode,SI16 siRewardSupply)
BOOL	cltIGuild::RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode)
{
	cltGuild	*pGuild;

	if( ( pGuild = GetGuildByID( uiGuildID ) ) == NULL )			return FALSE;
	
//	pGuild->RewardToGuildDAIForDestroyVillage( mnReward, uiVillageCode ,siRewardSupply);
	pGuild->RewardToGuildDAIForDestroyVillage( mnReward, uiVillageCode );

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� Guild Index ���� �˻��Ѵ�.
//	���� ����	:	2003-01-01 ���� 7:11:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//12�ÿ� �ѹ��� ������ ��� üũ �ϰԲ� �����.
void	cltIGuild::SetRelationReLoad()
{
	printf("��ܰ��� �缳�� ���� ");
	
	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM ; i++)
	{
		m_Guilds[i].SetRelationReLoad();
	}

	printf("��ܰ��� �缳�� ��\n");
}

// �����ϴ� ���ΰ��� üũ ���ָ� �ȴ�.
// ĳ���� �� ���ݿ� ����.
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

//���� �ð��� �Ǹ� ��ܿ����� ���� �ð��� �Ǿ��ٰ� �˷��ش�.
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

//��ܿ� �������ִ� ������ �˻��Ѵ�.
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

//�����ð��� ������ �������� ������ �ʱ�ȭ �Ѵ�.
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

	//�ٸ������ ������ �������ش�.
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

//DB���� ���� ������� ������ �����´�.
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
		else //���� ���� ����� ���°�� �ʱ�ȭ�� �Ѵ�.
			m_Guilds[i].SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
	}
}