#ifndef	___ONLINE_VILLAGE_DESCRIPTLIST___
#define ___ONLINE_VILLAGE_DESCRIPTLIST___


typedef struct
{
	UI16			m_uiVillageCode;						// 마을 ID
	BOOL			m_fWait;							// 현재 받아오는 중인지 Flag
	char			m_DescriptText[512];				// 마을 설명

	DWORD			m_TickCount;						// 이 Data를 마지막 갱신한 시간
} SVillageDescriptData;

typedef struct
{
	UI16			m_uiVillageCode;								// 마을 Code
	BOOL			m_fWait;										// 현재 받아오는 중인지 Flag

	SI16			m_VillageStateCode;								// 마을의 국가 코드
	char			m_VillageGuildName[ON_GUILDNAME_LENGTH + 1];	// 마을길드가 있을경우 길드 이름
	SI32			m_VillageGuildFlag;								// 마을길드가 있을경우 깃발
	VDEF			m_vdChangedDefence;								// 마을의 방어력
	VDEF			m_vdChangedMaxDefence;							// 마을의 최대 방어력
	DWORD			m_dwGuildRelation;								// 마을 상단과의 관계

	// robypark 2004/11/10 18:35
	SI32			m_siCurrentGarrison;							// 마을의 수비병 수(부대수 * 100명)
	SI32			m_siMaximumGarrison;							// 마을의 최대 수비병 수(부대 * 100명)

	SI16			m_ssCounterAttackType;							// 마을 반격 공격 타입. 일반, 돌, 기름 공격

	DWORD			m_TickCount;									// 이 Data를 마지막 갱신한 시간
} SVillageData;


// 마을 설명 List Class
class	OnlineVillageDescriptList
{
public:
	SVillageDescriptData*		m_lpVillageDescriptData;
	SI32						m_VillageDescriptDataCount;

	SVillageData*				m_lpVillageData;
	SI32						m_VillageDataCount;

public:
	OnlineVillageDescriptList();															// 생성자.
	virtual	~OnlineVillageDescriptList();													// 소멸자.

	virtual	VOID					Initialize();	
	virtual	BOOL					Init(SI32 MaxDescriptCount, SI32 MaxVillageDataCount);	// 초기화
	virtual	VOID					Free(void);												// 해제.

	virtual BOOL					AddDescriptData(UI16 VillageCode);						// 마을 설명을 추가한다.
	virtual BOOL					SetDescriptData(UI16 VillageCode, char* lpText);			// 마을 설명을 지정한다.
	virtual SVillageDescriptData*	GetDescriptData(UI16 VillageCode);						// 마을 설명을 얻는다.

	virtual BOOL					AddVillageDataData(UI16 VillageCode);						// 마을 Data을 추가한다.
	virtual BOOL					DelVillageDataData(UI16 VillageCode);						// 마을 Data을 삭제한다.
	virtual BOOL					SetVillageDataData(UI16 VillageCode, SI16 VillageStateCode, char* VillageGuildName, SI32 VillageGuildFlag, VDEF vdChangedDefence, VDEF vdChangedMaxDefence, DWORD dwGuildRelation, SI32 siCurrentGarrison, SI32 siMaximumGarrison, SI16 siCounterAttackType);		// 마을 Data을 지정한다.
	virtual SVillageData*			GetVillageDataData(UI16 VillageCode);						// 마을 Data을 얻는다.
	virtual BOOL					ClearVillageDataData(UI16 VillageCode);					// 특정 마을을 제외한 모든 Data를 초기화 한다.
};	


#endif
