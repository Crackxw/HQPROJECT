/************************************************************************************************
	파일명 : OnlineVillageStruct-Government.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREGOVERNMENT_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREGOVERNMENT_HEADER


#include <DDraw.h>
#include "OnlineCommonStruct.h"
#include "Online.h"
#include "OnlineVillageBaseBarrack.h"



#define		GUILDMAKE_MINLEVEL								30				// 상단을 만들시 최소 필요 Level
#define		GUILDMAKE_MINCREDIT								20				// 상단을 만들시 최소 필요 신용도

#define		MAX_INVESTMENT_TYPE								1				// 투자할 종류 수
#define		MAX_INVESTMENT_COUNT							101				// 최대 투자자 수

#define		GOVERNMENT_NOWWORK_INVEST_START							1	
#define		GOVERNMENT_NOWWORK_INVEST_STARTPOOL						2	
#define		GOVERNMENT_NOWWORK_INVEST_POOL							3	
#define		GOVERNMENT_NOWWORK_INVEST_SETINVESTMENT					4	
#define		GOVERNMENT_NOWWORK_INVEST_INVESTMENTGETOUT				5	
#define		GOVERNMENT_NOWWORK_INVEST_INVESTMENTGETOUT_RESULTWAIT	6
#define		GOVERNMENT_NOWWORK_INVEST_END							7
#define		GOVERNMENT_NOWWORK_INVEST_ENDPOOL						8	
#define		GOVERNMENT_NOWWORK_DESCRIPT_POOL						9	
#define		GOVERNMENT_NOWWORK_DESCRIPT_RESULTWAIT					10
#define		GOVERNMENT_NOWWORK_DESCRIPT_END							11
#define		GOVERNMENT_NOWWORK_DESCRIPT_ENDPOOL						12	
#define		GOVERNMENT_NOWWORK_VILLLAGEINFO_POOL					13
#define		GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_WAIT			14
#define		GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_POOL			15
#define		GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_RESULTWAIT	16
#define		GOVERNMENT_NOWWORK_VILLLAGEINFO_END						17
#define		GOVERNMENT_NOWWORK_VILLLAGEINFO_ENDPOOL					18
#define		GOVERNMENT_NOWWORK_GUILD_POOL							19
#define		GOVERNMENT_NOWWORK_GUILD_END							20
#define		GOVERNMENT_NOWWORK_GUILD_ENDPOOL						21	

#define		GOVERNMENT_WAITFLAG_GETLIST					1
#define		GOVERNMENT_WAITFLAG_INVESTMENTRESULT		2
#define		GOVERNMENT_WAITFLAG_INVESTMENTRESULTCHECK	3

#define		ON_GOVERNMENT_GUILD_INIT				0
#define		ON_GOVERNMENT_GUILD_POLL				1			// 메인폴
#define		ON_GOVERNMENT_GUILD_OPEN				2			// 상단개설
#define		ON_GOVERNMENT_GUILD_DELETE				3			// 상단해체
#define		ON_GOVERNMENT_GUILD_JOIN_LIST			4			// 상단원가입
#define		ON_GOVERNMENT_GUILD_DISSMISSAL_LIST		5			// 상단원해고
#define		ON_GOVERNMENT_GUILD_LIST				6			// 상단리스트
#define		ON_GOVERNMENT_GUILD_JOIN				7			// 상단가입
#define		ON_GOVERNMENT_GUILD_LEAVE				8			// 상단탈퇴
#define		ON_GOVERNMENT_GUILD_SELECT_FLAG			9			// 깃발선택
#define		ON_GOVERNMENT_GUILD_VILLAGE_LIST		10			// 상단마을리스트


#define		VILLAGE_DESCRIPT_NOTE		254


typedef struct
{
	BOOL						m_fData;									// Data Flag

	SI32						m_MinPercent;								// 최소 투자액
	SI32						m_MaxPercent;								// 최고 투자액
	SI32						m_MyInvestmentCanTime;						// 투자할수있는 날짜
	MONEY						m_VillageGainingsMoney;						// 마을의 수입

	OnInvestmentInfo			m_InvestmentList[MAX_INVESTMENT_COUNT];		// 마을의 투자자 정보
	MONEY						m_NowInvestmentMoney;						// 현재 투자된 금액
	SI16						m_MaxInvestment;							// 마을의 투자자 정보 Max Count

	SI16						m_MyInvestment;								// 나의 투자자 Index
	SI16						m_BestInvestment;							// 최고 투자자 Index
	MONEY						m_MyGainingsMoney;							// 나의 마을수입 배당분

	UI08						m_GetOutFlag;								// 인출이 가능한지 Flag
	UI08						m_GetOutTex;								// 인출시 세금
} SVillageStructGovernment_InvestmentData;


// 관청 Class
class	OnlineVillageStructureGovernment : public OnlineVillageBaseStructure
{
private:
	// Image
	XSPR						m_Image_MainInterface_Face;
	XSPR						m_Image_InvestmentInterface_Back;
	XSPR						m_Image_DescriptInterface_Back;
	XSPR						m_Image_GuildInterface_Back;
	XSPR						m_Image_VillageInfoInterface_Back;
	XSPR						m_Image_VillageInfoDefenceInvestmentInterface_Back;

	XSPR						m_Image_Button_Guild_DB;
	XSPR						m_Image_Button_All_MM;
	XSPR						m_Image_Button_All_EM;
	XSPR						m_Image_Button_All_B;
	XSPR						m_Image_Scroll_All_M;
	XSPR*						m_Image_Scroll_All_SBU;
	XSPR*						m_Image_Scroll_All_SBD;
	XSPR*						m_Image_Scroll_All_SBBU;
	XSPR*						m_Image_Scroll_All_SBBD;

	OnRealWorldTime				m_NowTime;
	RECT						m_rtDescriptRect;

	// 마을 정보
	char						m_VillageDescript_Descript[VILLAGE_DESCRIPT_NOTE + 1];
	char						m_VillageInfo_StructList[256];
	HRGN						m_VillageInfo_RectHRGN;
	HRGN						m_VillageInfo_RectBigHRGN;
	
	// Scroll
	SI16						m_NowScrollInvestmentType;
	SI16						m_NowScroll;

	// Button
	_Button						m_Button_Main_Investment;
	_Button						m_Button_Main_VillageDescript;
	_Button						m_Button_Main_Guild;
	_Button						m_Button_Main_VillageInfo;
	_Button						m_Button_Main_Exit;

	// Investment Button
	_Button						m_Button_Investment_SetInvestment;
	_Button						m_Button_Investment_InvestmentGetOut;
	_Button						m_Button_Investment_InvestmentTypeScrollUp;
	_Button						m_Button_Investment_InvestmentTypeScrollDown;
	_Button						m_Button_Investment_InvestmentDataScrollUp;
	_Button						m_Button_Investment_InvestmentDataScrollDown;

	_Button						m_Button_Investment_VillageProfitYearScrollUp;
	_Button						m_Button_Investment_VillageProfitYearScrollDown;
	_Button						m_Button_Investment_VillageProfitVYearScrollUp;
	_Button						m_Button_Investment_VillageProfitVYearScrollDown;

	_Button						m_Button_Investment_InvestmentGetOut_OK;
	_Button						m_Button_Investment_InvestmentGetOut_Cancel;

	// Descropt Button
	_Button						m_Button_Descropt_SetDescropt;

	// Village Info Button
	_Button						m_Button_VillageInfo_InvestmentDefence;
	_Button						m_Button_VillageInfo_InvestmentDefenceOK;
	_Button						m_Button_VillageInfo_InvestmentDefenceCancel;
	_Button						m_Button_VillageInfo_DescriptScrollUp;
	_Button						m_Button_VillageInfo_DescriptScrollDown;

	// Guild Button
	_Button						m_Button_Guild_MakeGuild;							// 상단 설립
	_Button						m_Button_Guild_DeleteGuild;							// 상단 해체
	_Button						m_Button_Guild_DiffGuild;							// 다른 상단 검색
	_Button						m_Button_Guild_FlagGuild;							// 상단의 깃발 선택
	_Button						m_Button_Guild_JoinGuild;							// 상단 가입
	_Button						m_Button_Guild_DismissalGuild;						// 상단 해고
	_Button						m_Button_Guild_PlayerJoinDismissalGuild;			// 상단 가입,탈퇴

	_Button						m_Button_Guild_OK;									// 확인
	_Button						m_Button_Guild_Cancel;								// 돌아가기
	_Button						m_Button_Guild_JoinOK;								// 입단허용
	_Button						m_Button_Guild_JoinCancel;							// 입단취소
	_Button						m_Button_Guild_JoinExit;							// 돌아가기

	_Button						m_Button_Guild_DHS_Appointment;						// 대행수임명
	_Button						m_Button_Guild_HND_Appointment;						// 행동대장임명
	_Button						m_Button_Guild_Class_Dissmissal;					// 직급해임
	_Button						m_Button_Guild_DismissalOK;							// 상단원해고

	_Button						m_Button_Guild_VillageList;							// 상단 마을리스트
	_Button						m_Button_Guild_InvestmentTypeScrollUp;
	_Button						m_Button_Guild_InvestmentTypeScrollDown;



public:
	// Base
	SI16						m_NextWork;
	SI32						m_fWaitFlag;
	SI16						m_NowWork;
	SI16						m_TempPos;
	DWORD						m_StartTickCount;

	OnlineEditControl*			m_pDescriptEditBox;
	OnlineEditControl*			m_Guild_GuildName;									// 길드 Name
	HWND						m_YetEditHWND;
	BOOL						m_YetEditHWNDEnable;
	SI16						m_YetEditLockShortKey;

	// Investment Data
	SVillageStructGovernment_InvestmentData		m_InvestmentData[MAX_INVESTMENT_TYPE];			// 투자 정보
	SI16										m_NowSelectInvestment;							// 현재 선택된 투자 Type
	MONEY										m_NowSettingInvestmentMoney;					// 현재 투자액
	stCapital*					m_stCapital;
	SI16						m_stCapitalCount;
	SI16						m_stCapitalSelect;
	SI16						m_stCapitalScroll;
	MONEY						m_InvestmentAfterMoney;											// 투자후 돈
	SI32						m_InvestmentTime;
	MONEY						m_InvestmentGetOutMoney;

	// 마을정보
	char						m_VillageInfo_Descript[VILLAGE_DESCRIPT_NOTE + 1];
	int							m_VillageInfo_DescriptScroll;
	int							m_VillageInfo_DescriptLine;
	SI32						m_VillageInfo_InOutPlayerCount;
	VDEF						m_VillageInfo_NowDefence;
	VDEF						m_VillageInfo_MaxDefence;
	MONEY						m_VillageInfo_DefencePerMoney;
	MONEY						m_VillageInfo_MaxMoney;
	VDEF						m_VillageInfo_UpDefence;

	// Guild Data
	SI16						m_Guild_NowState;									// 0 : Data를 기다리는중, 1 : None, 2 : 상단 개설, 3 : 상단해체, 4 : 상단원 가입, 5 : 상단원 해고, 6 : 다른 상단, 7 : 상단 가입, 8 : 상단 탈퇴, 9 : 깃발 선택
	SI16						m_Guild_NowMessageState;

	SI32						m_GuildScrollBar;								// Scroll 위치
	SI32						m_GuildScrollSelect;							// 선택한 Scroll 위치

	SI32						m_GuildJoinWaitPlayerTempFlag;
	SI32						m_GuildJoinWaitPlayerListCount;					// 길드에 가입하고하는 Player List
	strCandidateInfo*			m_GuildJoinWaitPlayerList;
	SI32						m_GuildJoinWaitPlayerList_PageMaxCount;
	SI32						m_GuildJoinWaitPlayerList_PageNowCount;

	SI32						m_GuildGuildPlayerListCount;					// 길드에 가입돼어 있는 Player List
	strGuildMemberInfo*			m_GuildGuildPlayerList;
	SI32						m_GuildGuildPlayerList_PageMaxCount;
	SI32						m_GuildGuildPlayerList_PageNowCount;

	SI32						m_GuildDiffGuildListCount;						// 다른 길드 List
	strGuildInfo*				m_GuildDiffGuildList;
	SI32						m_GuildDiffGuild_Setting;

	OnGuildVillageInfo*			m_GuildVillageList;								// 선택한 길드 소속 마을리스트
	SI32						m_GuildVillageListCount;
	SI32						m_GuildVillageList_PageMaxCount;
	SI32						m_GuildVillageList_PageNowCount;

	SI32						m_GuildFlag_MaxCount;
	SI32*						m_GuildFlag_Index;
	HRGN						m_RectHRGN;
	HRGN						m_RectBigHRGN;

	MONEY						m_GuildCreateCost;


public:
	OnlineVillageStructureGovernment();												// 생성자.
	virtual	~OnlineVillageStructureGovernment();									// 소멸자.
		
	virtual	VOID	Init(UI16 VillageCode, cltOnlineWorld* pOnlineWorld, VillageStructManager* pVillageStructManager);	// 초기화
	virtual	VOID	Free();																						// 해제.
	virtual	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);									// 액션.	
	virtual BOOL	Action();

	virtual BOOL	Draw_Back(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);				// Draw Back
	virtual BOOL	Draw_Investment(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);			// Draw Investment
	virtual BOOL	Draw_InvestmentGetOut(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);	// Draw Investment GetOut
	virtual BOOL	Draw_Descript(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);			// Draw Descript
	virtual BOOL	Draw_VillageInfo(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);			// Draw Village Info
	virtual BOOL	Draw_VillageInfo_SetDefenceInvestment(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);	// Draw Village Info Set Defence Investment
	virtual BOOL	Draw_Guild(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);				// Draw Guild

	virtual SI16	FindInvestmentList(OnInvestmentInfo* lpOnInvestmentInfo, SI16 InvestmentCount, char* lpMyID);	// Find My ID
	virtual char*	GetGradeInGuild(SI32 Code);										// 직급 이름을 얻어옴
	virtual BOOL	CheckGuildJWJ(SI32 Code);										// 지원자인지 검사
	UI16			InvestmentList(BYTE *pMsg);										// 투자자 리스트를 얻어온다.
};	


#endif
