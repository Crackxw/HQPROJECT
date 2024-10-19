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


	int                         m_OldBestInvestorIndex; //������ ������ �Զ��Ǿ������� ���� �ε���

	SI32						m_InvestorDataCount;				// ������ Data Count
	stVillageInvestorData		m_InvestorData[128];				// ������ Data(ó���� ������ ���� �ֽ�(����ڵ鿡�Դ� ������ �ʴ� �κ�))

	MONEY						m_NowAllInvestmentMoney;			// ���� ���ڵ� �� �ݾ�
} stVillageInvestment;


class	SOVillage_Government : public SOVillage_BaseStructure
{
private:
	SOVillage*					m_lpVillage;

	stVillageInvestment*		m_lpstVillageInvestment;

public:
	SOVillage_Government();			// ������.
	~SOVillage_Government();		// �Ҹ���.
	
	BOOL			Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage);																	// �ʱ�ȭ.
	VOID			Free();																					// �޸𸮸� �����Ѵ�.	

	// �ֽĿ� ���� �Լ���
	BOOL			VillageInvestment_DBGetData(void);														// �ֽ� Data�� DB���� ���´�.
	BOOL			VillageInvestment_Updata(void);															// �ֽ� Data�� Update�Ѵ�. �ַ� �ϴ� ��. ������� %�� �˻��ϰ�, �ڰ��� �ȵǴ�(1% �Ǵ� Money�� 0����)�� �ȵǴ� ������� �����Ѵ�. �ְ� �������� ���� ������ �ٷ��.
	BOOL			VillageInvestment_ReLoad();
	BOOL			VillageInvestment_CheckBestInvestmentor(void);											// �ֽ� Data���� �ְ� �����ڸ� �˻�
	BOOL			CheckInHourPitchDay(SOnlineTime YetTime, SOnlineTime NowTime, int nDay);
	BOOL			GovernmentInvestmentEnter(SOPlayer *pPlayer);											// ��û, �ֽ��忡 ����.
	BOOL			GovernmentInvestmentLeave(SOPlayer *pPlayer);											// ��û, �ֽ��忡 ������.
	BOOL			GovernmentInvestmentGetList(SOPlayer *pPlayer);											// ��û, �ֽ��忡�� ����Ʈ�� �޴´�.
	BOOL			GovernmentInvestmentSetInvestment(SOPlayer *pPlayer, __int64 InvestmentMoney,__int64 InvestmentSaveMoney);			// ��û, �ֽ��忡�� �ֽ��� ���.
	MONEY			GovernmentInvestmentClearInvestment(void);												// ��û, �ֽ��忡�� �ֽ��� ������ ����
	__int64			GetGainingsMoney(void);																	// ���� ���� ���
	SI32			GetInvestmentMinPercent(void);															// �ּ� ���� Percent ���
	SI32			GetInvestmentMaxPercent(void);															// �ִ� ���� Percent ���
	__int64			GetBaseInvestment(void);																// ������ ������ �⺻���� ���� �ֽ�
	__int64			GetDeleteInvestment(void);																// �����ð����� ������� �ֽĿ� ��
	MONEY			GetNowAllInvestment(void);																// ������ ������ ���� �� �ֽ�
	MONEY			GetNowAllInvestmentP(SI32 P);															// ������ ������ ���� �� �ֽ�
	char*			GetInvestmentMaxInvestmentorName(void);													// ���� ������ �ִ� �������� �̸��� �˻��Ѵ�.
	UI32			GetInvestmentMaxInvestmentorGuild(void);												// ���� ������ �ִ� �������� ��带 �˻��Ѵ�.(���� ���� ������̸� 0�� �����ش�.)
	__int64			GetInvestmentMaxInvestmentorMoney(void);												// ���� ������ �ִ� �������� ���� �˻��Ѵ�.
	BOOL            GetInvestmentMaxInvestmentorAccountSlot(SI32 *pAccount,UI08 *pSlot);
	BOOL			CheckInvestmentMaxInvestmentorName(char* lpID, int Type);								// ���� ������ �ִ� �������� �̸��� �˻��Ѵ�.
	stVillageInvestorData*	GetInvestmentDataByID(char* lpID);												// ���� �� ID�� ��������� �� ������ ���� ���� ������ ���´�.
	BOOL			WriteDescription(SOPlayer *pPlayer, char* Descript);
	stCapital*		GetVillageCapital();
	void			SendVillageReqMaxVillageDefenceInvestment(SOPlayer *pPlayer);							// �������¿� �����Ҽ� �ִ� ������ ��´�
	void			SendVillageReqSetVillageDefenceInvestment(SOPlayer *pPlayer, MONEY InvestmentMoney);	// ���� ���¿� �����ϱ�

	BOOL			InvestmentGetOut(SOPlayer* lpPlayer, MONEY GetOutMoney);								// Get Out
	BYTE			InvestmentGetOutF(void);																// Get Out Flag
	BYTE			InvestmentGetOutTexP(void);																// Get Out Tex Percent
	
	BOOL            CheckBestInvestment(char *id, MONEY GetOutMoney);
	
	SI32            GetOldBestInvestIndex();

	//uiIndex�� �ش��ϴ� �������� ���� uiPercent ��ŭ ��´�.
	MONEY           DelInvestMoney(UI32 uiIndex, UI16 uiPercent, BOOL bFlag);
};


#endif
