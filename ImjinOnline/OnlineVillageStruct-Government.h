/************************************************************************************************
	���ϸ� : OnlineVillageStruct-Government.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREGOVERNMENT_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREGOVERNMENT_HEADER


#include <DDraw.h>
#include "OnlineCommonStruct.h"
#include "Online.h"
#include "OnlineVillageBaseBarrack.h"



#define		GUILDMAKE_MINLEVEL								30				// ����� ����� �ּ� �ʿ� Level
#define		GUILDMAKE_MINCREDIT								20				// ����� ����� �ּ� �ʿ� �ſ뵵

#define		MAX_INVESTMENT_TYPE								1				// ������ ���� ��
#define		MAX_INVESTMENT_COUNT							101				// �ִ� ������ ��

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
#define		ON_GOVERNMENT_GUILD_POLL				1			// ������
#define		ON_GOVERNMENT_GUILD_OPEN				2			// ��ܰ���
#define		ON_GOVERNMENT_GUILD_DELETE				3			// �����ü
#define		ON_GOVERNMENT_GUILD_JOIN_LIST			4			// ��ܿ�����
#define		ON_GOVERNMENT_GUILD_DISSMISSAL_LIST		5			// ��ܿ��ذ�
#define		ON_GOVERNMENT_GUILD_LIST				6			// ��ܸ���Ʈ
#define		ON_GOVERNMENT_GUILD_JOIN				7			// ��ܰ���
#define		ON_GOVERNMENT_GUILD_LEAVE				8			// ���Ż��
#define		ON_GOVERNMENT_GUILD_SELECT_FLAG			9			// ��߼���
#define		ON_GOVERNMENT_GUILD_VILLAGE_LIST		10			// ��ܸ�������Ʈ


#define		VILLAGE_DESCRIPT_NOTE		254


typedef struct
{
	BOOL						m_fData;									// Data Flag

	SI32						m_MinPercent;								// �ּ� ���ھ�
	SI32						m_MaxPercent;								// �ְ� ���ھ�
	SI32						m_MyInvestmentCanTime;						// �����Ҽ��ִ� ��¥
	MONEY						m_VillageGainingsMoney;						// ������ ����

	OnInvestmentInfo			m_InvestmentList[MAX_INVESTMENT_COUNT];		// ������ ������ ����
	MONEY						m_NowInvestmentMoney;						// ���� ���ڵ� �ݾ�
	SI16						m_MaxInvestment;							// ������ ������ ���� Max Count

	SI16						m_MyInvestment;								// ���� ������ Index
	SI16						m_BestInvestment;							// �ְ� ������ Index
	MONEY						m_MyGainingsMoney;							// ���� �������� ����

	UI08						m_GetOutFlag;								// ������ �������� Flag
	UI08						m_GetOutTex;								// ����� ����
} SVillageStructGovernment_InvestmentData;


// ��û Class
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

	// ���� ����
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
	_Button						m_Button_Guild_MakeGuild;							// ��� ����
	_Button						m_Button_Guild_DeleteGuild;							// ��� ��ü
	_Button						m_Button_Guild_DiffGuild;							// �ٸ� ��� �˻�
	_Button						m_Button_Guild_FlagGuild;							// ����� ��� ����
	_Button						m_Button_Guild_JoinGuild;							// ��� ����
	_Button						m_Button_Guild_DismissalGuild;						// ��� �ذ�
	_Button						m_Button_Guild_PlayerJoinDismissalGuild;			// ��� ����,Ż��

	_Button						m_Button_Guild_OK;									// Ȯ��
	_Button						m_Button_Guild_Cancel;								// ���ư���
	_Button						m_Button_Guild_JoinOK;								// �Դ����
	_Button						m_Button_Guild_JoinCancel;							// �Դ����
	_Button						m_Button_Guild_JoinExit;							// ���ư���

	_Button						m_Button_Guild_DHS_Appointment;						// ������Ӹ�
	_Button						m_Button_Guild_HND_Appointment;						// �ൿ�����Ӹ�
	_Button						m_Button_Guild_Class_Dissmissal;					// ��������
	_Button						m_Button_Guild_DismissalOK;							// ��ܿ��ذ�

	_Button						m_Button_Guild_VillageList;							// ��� ��������Ʈ
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
	OnlineEditControl*			m_Guild_GuildName;									// ��� Name
	HWND						m_YetEditHWND;
	BOOL						m_YetEditHWNDEnable;
	SI16						m_YetEditLockShortKey;

	// Investment Data
	SVillageStructGovernment_InvestmentData		m_InvestmentData[MAX_INVESTMENT_TYPE];			// ���� ����
	SI16										m_NowSelectInvestment;							// ���� ���õ� ���� Type
	MONEY										m_NowSettingInvestmentMoney;					// ���� ���ھ�
	stCapital*					m_stCapital;
	SI16						m_stCapitalCount;
	SI16						m_stCapitalSelect;
	SI16						m_stCapitalScroll;
	MONEY						m_InvestmentAfterMoney;											// ������ ��
	SI32						m_InvestmentTime;
	MONEY						m_InvestmentGetOutMoney;

	// ��������
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
	SI16						m_Guild_NowState;									// 0 : Data�� ��ٸ�����, 1 : None, 2 : ��� ����, 3 : �����ü, 4 : ��ܿ� ����, 5 : ��ܿ� �ذ�, 6 : �ٸ� ���, 7 : ��� ����, 8 : ��� Ż��, 9 : ��� ����
	SI16						m_Guild_NowMessageState;

	SI32						m_GuildScrollBar;								// Scroll ��ġ
	SI32						m_GuildScrollSelect;							// ������ Scroll ��ġ

	SI32						m_GuildJoinWaitPlayerTempFlag;
	SI32						m_GuildJoinWaitPlayerListCount;					// ��忡 �����ϰ��ϴ� Player List
	strCandidateInfo*			m_GuildJoinWaitPlayerList;
	SI32						m_GuildJoinWaitPlayerList_PageMaxCount;
	SI32						m_GuildJoinWaitPlayerList_PageNowCount;

	SI32						m_GuildGuildPlayerListCount;					// ��忡 ���Եž� �ִ� Player List
	strGuildMemberInfo*			m_GuildGuildPlayerList;
	SI32						m_GuildGuildPlayerList_PageMaxCount;
	SI32						m_GuildGuildPlayerList_PageNowCount;

	SI32						m_GuildDiffGuildListCount;						// �ٸ� ��� List
	strGuildInfo*				m_GuildDiffGuildList;
	SI32						m_GuildDiffGuild_Setting;

	OnGuildVillageInfo*			m_GuildVillageList;								// ������ ��� �Ҽ� ��������Ʈ
	SI32						m_GuildVillageListCount;
	SI32						m_GuildVillageList_PageMaxCount;
	SI32						m_GuildVillageList_PageNowCount;

	SI32						m_GuildFlag_MaxCount;
	SI32*						m_GuildFlag_Index;
	HRGN						m_RectHRGN;
	HRGN						m_RectBigHRGN;

	MONEY						m_GuildCreateCost;


public:
	OnlineVillageStructureGovernment();												// ������.
	virtual	~OnlineVillageStructureGovernment();									// �Ҹ���.
		
	virtual	VOID	Init(UI16 VillageCode, cltOnlineWorld* pOnlineWorld, VillageStructManager* pVillageStructManager);	// �ʱ�ȭ
	virtual	VOID	Free();																						// ����.
	virtual	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);									// �׼�.	
	virtual BOOL	Action();

	virtual BOOL	Draw_Back(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);				// Draw Back
	virtual BOOL	Draw_Investment(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);			// Draw Investment
	virtual BOOL	Draw_InvestmentGetOut(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);	// Draw Investment GetOut
	virtual BOOL	Draw_Descript(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);			// Draw Descript
	virtual BOOL	Draw_VillageInfo(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);			// Draw Village Info
	virtual BOOL	Draw_VillageInfo_SetDefenceInvestment(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);	// Draw Village Info Set Defence Investment
	virtual BOOL	Draw_Guild(LPDIRECTDRAWSURFACE7 pSurface, int TickCount);				// Draw Guild

	virtual SI16	FindInvestmentList(OnInvestmentInfo* lpOnInvestmentInfo, SI16 InvestmentCount, char* lpMyID);	// Find My ID
	virtual char*	GetGradeInGuild(SI32 Code);										// ���� �̸��� ����
	virtual BOOL	CheckGuildJWJ(SI32 Code);										// ���������� �˻�
	UI16			InvestmentList(BYTE *pMsg);										// ������ ����Ʈ�� ���´�.
};	


#endif
