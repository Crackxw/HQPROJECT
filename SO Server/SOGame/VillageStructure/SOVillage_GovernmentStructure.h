#ifndef	_VILLAGE_GOVERNMENT_HEADER
#define	_VILLAGE_GOVERNMENT_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"


class	SODatabase;


typedef struct
{
	int							m_BestInvestorInvestmentIndex;
	int							m_BestInvestorInvestmenUpdateTime;

	int							m_Real_BestInvestorInvestmentIndex;
	int							m_Real_BestInvestorInvestmenUpdateTime;


	int                         m_OldBestInvestorIndex; //공성시 마을이 함락되었을때의 최투 인덱스

	SI32						m_InvestorDataCount;				// 투자자 Data Count
	stVillageInvestorData		m_InvestorData[128];				// 투자자 Data(처음은 무조건 국가 주식(사용자들에게는 보이지 않는 부분))

	MONEY						m_NowAllInvestmentMoney;			// 현재 투자된 총 금액
} stVillageInvestment;


class	SOVillage_Government : public SOVillage_BaseStructure
{
private:
	SOVillage*					m_lpVillage;

	stVillageInvestment*		m_lpstVillageInvestment;

public:
	SOVillage_Government();			// 생성자.
	~SOVillage_Government();		// 소멸자.
	
	BOOL			Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage);																	// 초기화.
	VOID			Free();																					// 메모리를 해제한다.	

	// 주식에 관한 함수들
	BOOL			VillageInvestment_DBGetData(void);														// 주식 Data를 DB에서 얻어온다.
	BOOL			VillageInvestment_Updata(void);															// 주식 Data를 Update한다. 주로 하는 일. 사람들의 %를 검사하고, 자격이 안되는(1% 또는 Money가 0이하)가 안되는 사람들을 삭제한다. 최고 투자자의 관한 정보도 다룬다.
	BOOL			VillageInvestment_ReLoad();
	BOOL			VillageInvestment_CheckBestInvestmentor(void);											// 주식 Data에서 최고 투자자를 검사
	BOOL			CheckInHourPitchDay(SOnlineTime YetTime, SOnlineTime NowTime, int nDay);
	BOOL			GovernmentInvestmentEnter(SOPlayer *pPlayer);											// 관청, 주식장에 들어간다.
	BOOL			GovernmentInvestmentLeave(SOPlayer *pPlayer);											// 관청, 주식장에 나간다.
	BOOL			GovernmentInvestmentGetList(SOPlayer *pPlayer);											// 관청, 주식장에서 리스트를 받는다.
	BOOL			GovernmentInvestmentSetInvestment(SOPlayer *pPlayer, __int64 InvestmentMoney,__int64 InvestmentSaveMoney);			// 관청, 주식장에서 주식을 산다.
	MONEY			GovernmentInvestmentClearInvestment(void);												// 관청, 주식장에서 주식을 모조리 삭제
	__int64			GetGainingsMoney(void);																	// 마을 수익 계산
	SI32			GetInvestmentMinPercent(void);															// 최소 투자 Percent 계산
	SI32			GetInvestmentMaxPercent(void);															// 최대 투자 Percent 계산
	__int64			GetBaseInvestment(void);																// 마을이 가지는 기본적인 국가 주식
	__int64			GetDeleteInvestment(void);																// 일정시간마다 사라지는 주식에 량
	MONEY			GetNowAllInvestment(void);																// 마을이 가지는 현재 총 주식
	MONEY			GetNowAllInvestmentP(SI32 P);															// 마을이 가지는 현재 총 주식
	char*			GetInvestmentMaxInvestmentorName(void);													// 현재 마을에 최대 투자자의 이름을 검색한다.
	UI32			GetInvestmentMaxInvestmentorGuild(void);												// 현재 마을에 최대 투자자의 길드를 검색한다.(만약 가입 대기중이면 0을 돌려준다.)
	__int64			GetInvestmentMaxInvestmentorMoney(void);												// 현재 마을에 최대 투자자의 돈을 검색한다.
	BOOL            GetInvestmentMaxInvestmentorAccountSlot(SI32 *pAccount,UI08 *pSlot);
	BOOL			CheckInvestmentMaxInvestmentorName(char* lpID, int Type);								// 현재 마을에 최대 투자자의 이름을 검색한다.
	stVillageInvestorData*	GetInvestmentDataByID(char* lpID);												// 현재 이 ID를 가진사람의 이 마을에 대한 투자 정보를 얻어온다.
	BOOL			WriteDescription(SOPlayer *pPlayer, char* Descript);
	stCapital*		GetVillageCapital();
	void			SendVillageReqMaxVillageDefenceInvestment(SOPlayer *pPlayer);							// 마을방어력에 투자할수 있는 정보를 얻는다
	void			SendVillageReqSetVillageDefenceInvestment(SOPlayer *pPlayer, MONEY InvestmentMoney);	// 마을 방어력에 투자하기

	BOOL			InvestmentGetOut(SOPlayer* lpPlayer, MONEY GetOutMoney);								// Get Out
	BYTE			InvestmentGetOutF(void);																// Get Out Flag
	BYTE			InvestmentGetOutTexP(void);																// Get Out Tex Percent
	
	BOOL            CheckBestInvestment(char *id, MONEY GetOutMoney);
	
	SI32            GetOldBestInvestIndex();

	//uiIndex에 해당하는 투자자의 돈을 uiPercent 만큼 깍는다.
	MONEY           DelInvestMoney(UI32 uiIndex, UI16 uiPercent, BOOL bFlag);
};


#endif
