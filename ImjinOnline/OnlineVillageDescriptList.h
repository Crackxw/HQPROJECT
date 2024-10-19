#ifndef	___ONLINE_VILLAGE_DESCRIPTLIST___
#define ___ONLINE_VILLAGE_DESCRIPTLIST___


typedef struct
{
	UI16			m_uiVillageCode;						// ���� ID
	BOOL			m_fWait;							// ���� �޾ƿ��� ������ Flag
	char			m_DescriptText[512];				// ���� ����

	DWORD			m_TickCount;						// �� Data�� ������ ������ �ð�
} SVillageDescriptData;

typedef struct
{
	UI16			m_uiVillageCode;								// ���� Code
	BOOL			m_fWait;										// ���� �޾ƿ��� ������ Flag

	SI16			m_VillageStateCode;								// ������ ���� �ڵ�
	char			m_VillageGuildName[ON_GUILDNAME_LENGTH + 1];	// ������尡 ������� ��� �̸�
	SI32			m_VillageGuildFlag;								// ������尡 ������� ���
	VDEF			m_vdChangedDefence;								// ������ ����
	VDEF			m_vdChangedMaxDefence;							// ������ �ִ� ����
	DWORD			m_dwGuildRelation;								// ���� ��ܰ��� ����

	// robypark 2004/11/10 18:35
	SI32			m_siCurrentGarrison;							// ������ ���� ��(�δ�� * 100��)
	SI32			m_siMaximumGarrison;							// ������ �ִ� ���� ��(�δ� * 100��)

	SI16			m_ssCounterAttackType;							// ���� �ݰ� ���� Ÿ��. �Ϲ�, ��, �⸧ ����

	DWORD			m_TickCount;									// �� Data�� ������ ������ �ð�
} SVillageData;


// ���� ���� List Class
class	OnlineVillageDescriptList
{
public:
	SVillageDescriptData*		m_lpVillageDescriptData;
	SI32						m_VillageDescriptDataCount;

	SVillageData*				m_lpVillageData;
	SI32						m_VillageDataCount;

public:
	OnlineVillageDescriptList();															// ������.
	virtual	~OnlineVillageDescriptList();													// �Ҹ���.

	virtual	VOID					Initialize();	
	virtual	BOOL					Init(SI32 MaxDescriptCount, SI32 MaxVillageDataCount);	// �ʱ�ȭ
	virtual	VOID					Free(void);												// ����.

	virtual BOOL					AddDescriptData(UI16 VillageCode);						// ���� ������ �߰��Ѵ�.
	virtual BOOL					SetDescriptData(UI16 VillageCode, char* lpText);			// ���� ������ �����Ѵ�.
	virtual SVillageDescriptData*	GetDescriptData(UI16 VillageCode);						// ���� ������ ��´�.

	virtual BOOL					AddVillageDataData(UI16 VillageCode);						// ���� Data�� �߰��Ѵ�.
	virtual BOOL					DelVillageDataData(UI16 VillageCode);						// ���� Data�� �����Ѵ�.
	virtual BOOL					SetVillageDataData(UI16 VillageCode, SI16 VillageStateCode, char* VillageGuildName, SI32 VillageGuildFlag, VDEF vdChangedDefence, VDEF vdChangedMaxDefence, DWORD dwGuildRelation, SI32 siCurrentGarrison, SI32 siMaximumGarrison, SI16 siCounterAttackType);		// ���� Data�� �����Ѵ�.
	virtual SVillageData*			GetVillageDataData(UI16 VillageCode);						// ���� Data�� ��´�.
	virtual BOOL					ClearVillageDataData(UI16 VillageCode);					// Ư�� ������ ������ ��� Data�� �ʱ�ȭ �Ѵ�.
};	


#endif
