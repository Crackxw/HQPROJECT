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
	SStack<strGuildMemberInfo>				m_RealMembers;				// 상단원(정식 상단원)	
	SStack<strCandidateInfo>				m_Candidates;				// 상단원(지원자)
	
	//SI32	m_siMaxRealMemberNum;										// 최대 상단원 수
	
	DWORD	m_dwRMTotalPageNum;											// 정식 상단원 페이지 수
	DWORD	m_dwCMTotalPageNum;											// 지원자 페이지 수

	static	cltIGuildClass		*m_pclIGuildClass;						// 직급 코드를 얻을 때 필요하다.
private:
	SI32	FindEmptyGuildMemberSlot();									// 비어있는 상단원 슬롯을 찾는다
	VOID	CalculateRMTotalPageNum();									// 전체 정식 상단원의 페이지수를 재 계산한다.
	VOID	CalculateCMTotalPageNum();									// 전체 지원자의 페이지수를 재 계산한다.
public:
	GuildMemberManager();			// 생성자.
	~GuildMemberManager();			// 소멸자.

	
	VOID	Init();											// 초기화
	VOID	OnceInit();										// 한번만 초기화를 해줄 것들을 해준다.
	BOOL	AddMember(CHAR *pszName, SI32 siClassInGuild, MONEY mnProperty, SI32 siLevel, UI32 uiTradeCredit, SI32 siAccount,UI08 uiSlot);	// 상단원 에 추가시킨다. siClassInGuild에 따라서 상단원인지 지원자인지 구별한다.
	BOOL	DelMember(CHAR *pszName, BOOL bIsCandidate);	// m_stAllMembers에서 제거한다.	
	UI32	GetRealMemeberNum();		// 실제 상단원의 수를 얻어온다.
	UI32	GetCandidateMemberNum();	// 실제 지원자 수를 얻어온다.
	BOOL	IsRealMemberFull();			// 실제 상단원이 꽉 찼는지 검사한다.	
	VOID	SetMaxRealMemberNum(SI32 siMaxRealMemberNum);	// 최대 상단원의 수를 지정해준다.
	SI32	GetMaxRealMemberNum();							// 최대 상단원의 수를 얻어온다
	DWORD	GetRMTotalPageNum();							// 전체 정식 상단원을 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
	DWORD	GetCMTotalPageNum();							// 전체 지원자를 화면에 보여주기 위해 필요한 페이지수를 얻어온다.
	BOOL	ChangeClass(CHAR *pszName, SI32 siChangeClass, BOOL bCandidate);		// 직급을 변경해준다.
	UI32	GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo);			// 정식 상단원의 리스트를 얻어온다.
	UI32	GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo);				// 지원자 리스트를 얻어온다.
	BOOL	IsCandidate(SI32 siClassInGuild);				// 직급을 보아서 지원자인지 확인한다.
	BOOL	GetGuildIDByCharName(CHAR *pszName, SI32 *psiGuildClass);		// 캐릭터 이름을 사용하여 길드 직급을 얻어온다.
	BOOL    GetGuildMemberByCharName(CHAR *pszName, strGuildCommonMemberInfo *pMemberInfo);
	BOOL    GetGuildMemberByCharAccount(SI32 siAccount,UI08 uiSlot, strGuildCommonMemberInfo *pMemberInfo);
	BOOL	IsMember(CHAR *pszName);										// 상단원인지 알아온다.(지원자 포함)	

	BOOL	IsCandidateMember( CHAR *pszName );								// 지원자인지 알아온다.
	strCandidateInfo *GetCandidateMember(CHAR *pszName);

	CHAR*	GetDHSID();										// 대행수의 ID를 얻어온다. (없으면 NULL 리턴)
	UI32	GetTotalHNDNum();								// 길드내 행동대장의 수를 얻어온다.

	static	VOID InitStaticVariable( cltIGuildClass *pclIGuildClass );	// 정적 변수를 초기화 한다.
};

#endif
