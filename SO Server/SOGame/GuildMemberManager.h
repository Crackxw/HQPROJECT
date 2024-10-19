#ifndef	_GUILD_MEMBER_HEADER
#define _GUILD_MEMBER_HEADER

#include "GSCDefine.h"
#include "SOCommonHeader.h"
#include "Stack.h"
#include "OnlineCommonStruct.h"
#include "iGuildClass.h"

//class	cltIGuildClass;

class	GuildMemberManager
{
private:			
	SStack<strGuildMemberInfo>				m_RealMembers;				// ��ܿ�(���� ��ܿ�)	
	SStack<strCandidateInfo>				m_Candidates;				// ��ܿ�(������)
	
	//SI32	m_siMaxRealMemberNum;										// �ִ� ��ܿ� ��
	
	DWORD	m_dwRMTotalPageNum;											// ���� ��ܿ� ������ ��
	DWORD	m_dwCMTotalPageNum;											// ������ ������ ��

	static	cltIGuildClass		*m_pclIGuildClass;						// ���� �ڵ带 ���� �� �ʿ��ϴ�.
private:
	SI32	FindEmptyGuildMemberSlot();									// ����ִ� ��ܿ� ������ ã�´�
	VOID	CalculateRMTotalPageNum();									// ��ü ���� ��ܿ��� ���������� �� ����Ѵ�.
	VOID	CalculateCMTotalPageNum();									// ��ü �������� ���������� �� ����Ѵ�.
public:
	GuildMemberManager();			// ������.
	~GuildMemberManager();			// �Ҹ���.

	
	VOID	Init();											// �ʱ�ȭ
	VOID	OnceInit();										// �ѹ��� �ʱ�ȭ�� ���� �͵��� ���ش�.
	BOOL	AddMember(CHAR *pszName, SI32 siClassInGuild, MONEY mnProperty, SI32 siLevel, UI32 uiTradeCredit, SI32 siAccount,UI08 uiSlot);	// ��ܿ� �� �߰���Ų��. siClassInGuild�� ���� ��ܿ����� ���������� �����Ѵ�.
	BOOL	DelMember(CHAR *pszName, BOOL bIsCandidate);	// m_stAllMembers���� �����Ѵ�.	
	UI32	GetRealMemeberNum();		// ���� ��ܿ��� ���� ���´�.
	UI32	GetCandidateMemberNum();	// ���� ������ ���� ���´�.
	BOOL	IsRealMemberFull();			// ���� ��ܿ��� �� á���� �˻��Ѵ�.	
	VOID	SetMaxRealMemberNum(SI32 siMaxRealMemberNum);	// �ִ� ��ܿ��� ���� �������ش�.
	SI32	GetMaxRealMemberNum();							// �ִ� ��ܿ��� ���� ���´�
	DWORD	GetRMTotalPageNum();							// ��ü ���� ��ܿ��� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
	DWORD	GetCMTotalPageNum();							// ��ü �����ڸ� ȭ�鿡 �����ֱ� ���� �ʿ��� ���������� ���´�.
	BOOL	ChangeClass(CHAR *pszName, SI32 siChangeClass, BOOL bCandidate);		// ������ �������ش�.
	UI32	GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo);			// ���� ��ܿ��� ����Ʈ�� ���´�.
	UI32	GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo);				// ������ ����Ʈ�� ���´�.
	BOOL	IsCandidate(SI32 siClassInGuild);				// ������ ���Ƽ� ���������� Ȯ���Ѵ�.
	BOOL	GetGuildIDByCharName(CHAR *pszName, SI32 *psiGuildClass);		// ĳ���� �̸��� ����Ͽ� ��� ������ ���´�.
	BOOL    GetGuildMemberByCharName(CHAR *pszName, strGuildCommonMemberInfo *pMemberInfo);
	BOOL    GetGuildMemberByCharAccount(SI32 siAccount,UI08 uiSlot, strGuildCommonMemberInfo *pMemberInfo);
	BOOL	IsMember(CHAR *pszName);										// ��ܿ����� �˾ƿ´�.(������ ����)	

	BOOL	IsCandidateMember( CHAR *pszName );								// ���������� �˾ƿ´�.
	strCandidateInfo *GetCandidateMember(CHAR *pszName);

	CHAR*	GetDHSID();										// ������� ID�� ���´�. (������ NULL ����)
	UI32	GetTotalHNDNum();								// ��峻 �ൿ������ ���� ���´�.

	static	VOID InitStaticVariable( cltIGuildClass *pclIGuildClass );	// ���� ������ �ʱ�ȭ �Ѵ�.
};

#endif
