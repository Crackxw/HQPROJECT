/************************************************************************************************
	파일명 : OnlineFieldEffect.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	___ONLINE_FIELD_EFFECT___
#define ___ONLINE_FIELD_EFFECT___


typedef enum
{
	EFFECTTYPE_VILLAGEDEMAGE
	, EFFECTTYPE_ATTACKVILLAGETOENEMY
	, EFFECTTYPE_CHANGENUMBER
	, EFFECTTYPE_VILLAGEEXPLOSION
	, EFFECTTYPE_WARPEFFECT

	// robypark 2004/12/2 18:4
	, EFFECTTYPE_VILLAGE_DEFENCE_UP	// 마을 방어력 향상 효과
	, EFFECTTYPE_OCCUPIED_VILLAGE	// 마을 점령 효과

	, EFFECTTYPE_FIELD_CHANGENUMBER	// 숫자 변화

} EEffectType;

typedef struct
{
	SI16		m_Frame;

	SI16		m_MapXPos;
	SI16		m_MapYPos;

	VDEF		m_Demage;
	SI16		m_UpPercent;
} SEffectData_VillageDemage;

typedef enum
{
	EEFFECTDATA_ATTACKVILLAGETOENEMY_ATTACKTYPE_FALLINGROCK
	, EEFFECTDATA_ATTACKVILLAGETOENEMY_ATTACKTYPE_SOWOIL
} EEffectData_AttackVillageToEnemy_AttackType;

// robypark 2004/12/27 18:19
#define ON_FIELDEFFECT_ANIMATION_OCCUPIEDVILLAGE_REPEATTIMES	3	// 마을 점령 이펙트 애니메이션 반복횟수

typedef struct
{
	EEffectData_AttackVillageToEnemy_AttackType		m_AttackType;
	SI16											m_Frame;

	SI16		m_AttackMapXPos;
	SI16		m_AttackMapYPos;
	SI16		m_DemageMapXPos;
	SI16		m_DemageMapYPos;

	SI32		m_Demage;
} SEffectData_AttackVillageToEnemy;

typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;
	__int64		m_ChangeNumber;
} SEffectData_ChangeNumber;

typedef enum
{
	EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN = 0
	, EFFECTDATA_FIELD_CHANGENUMBER_COLOR_RED
	, EFFECTDATA_FIELD_CHANGENUMBER_COLOR_BLUE
	, EFFECTDATA_FIELD_CHANGENUMBER_COLOR_WHITE

} EffectData_Field_ChangeNumber_Color;

// 필드에 표시되는 숫자변화
typedef struct
{
	SI16		m_Frame;

	SI16		m_MapXPos;
	SI16		m_MapYPos;
	SI08		m_Color;		// 0: Green, 1: Red, 2: Blue, 3: White
	bool		m_bVillageDefence;	// 마을 공격일 경우 데미지 위치 보정을 안한다.
	__int64		m_ChangeNumber;
} SEffectData_Field_ChangeNumber;

typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;
} SEffectData_VillageExplosion;

typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;
} SEffectData_WarpEffect;

// robypark 2004/12/2 18:4
// 마을 방어력 향상
typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;
} SEffectData_VillageDefenceUp;

// robypark 2004/12/2 18:4
// 마을 점령(항복의 흰 깃발) 효과
typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;

	// robypark 2004/12/27 18:19
	SI08		m_scCurrentRepeatTimes;	// 현재 Animation 반복 재생 횟수
	SI08		m_scMaxRepeatTimes;		// 최대 Animation 반복 재생 횟수
} SEffectData_OccupiedVillage;

typedef struct
{
	BOOL		m_fCreate;

	EEffectType	m_Type;
	union
	{
		SEffectData_VillageDemage			m_EffectData_VillageDemage;
		SEffectData_AttackVillageToEnemy	m_EffectData_AttackVillageToEnemy;
		SEffectData_ChangeNumber			m_EffectData_ChangeNum;
		SEffectData_VillageExplosion		m_EffectData_VillageExplosion;
		SEffectData_WarpEffect				m_EffectData_WarpEffect;
		SEffectData_VillageDefenceUp		m_EffectData_VillageDefenceUp;	// 마을 방어력 향상
		SEffectData_OccupiedVillage			m_EffectData_OccupiedVillage;	// 마을 점령 효과
		SEffectData_Field_ChangeNumber		m_EffectData_FieldChangeNumber;	// 필드 숫자 변화 효과
	};
} SEffectData;

#define RAIN_MAX					256
#define SNOW_MAX					256

typedef struct
{
	SI08				m_Type;					// 0 : None, 1 : 떨어지는 중, 2 : 다 떨어진후
	SI08				m_RainType;
	SI16				m_Frame;

	SI32				m_NowPositionX;
	SI32				m_NowPositionY;
	SI32				m_TargetPositionX;
	SI32				m_TargetPositionY;
	SI32				m_MovePositionX;
	SI32				m_MovePositionY;
} SEffectRainData;

typedef struct
{
	SI08				m_Type;					// 0 : None, 1 : 떨어지는 중, 2 : 다 떨어진후
	SI08				m_SnowType;
	SI16				m_Frame;
	float				m_FrameRotateFrame;

	SI32				m_TargetPositionX;

	SI32				m_NowPositionY;
	SI32				m_TargetPositionY;
	SI32				m_MovePositionY;
} SEffectSnowData;
 
typedef struct
{
	SI16				m_Percent;

	SI16				m_NowRainCount;
	SEffectRainData		m_RainData[RAIN_MAX];
} SEffectRain;

typedef struct
{
	SI16				m_Percent;

	SI16				m_NowSnowCount;
	SEffectSnowData		m_SnowData[SNOW_MAX];
} SEffectSnow;

// 필드에서 Effect 설정
class	OnlineFieldEffect
{
public:
	cltOnlineWorld*		m_lpWorld;

	SEffectData*		m_lpFieldEffect;
	SI32				m_FieldEffectCount;

	XSPR				m_CloudSpr;
	
	BOOL				m_fCloud;

	// robypark 2004/8/11 10:37
	// 까치 스프라이트 추가
	XSPR				m_SprMagpie;

	BOOL				m_fWeather;
	SEffectRain			m_Rain;
	SEffectSnow			m_Snow;

public:
	OnlineFieldEffect();																	// 생성자.
	virtual	~OnlineFieldEffect();															// 소멸자.
		
	virtual	BOOL					Init(cltOnlineWorld* lpWorld, SI32 MaxEffectCount);		// 초기화
	virtual	VOID					Free(void);												// 해제.

	virtual BOOL					Pool(void);												// 처리
	virtual BOOL					Render(LPDIRECTDRAWSURFACE7 pSurface);					// Render
	virtual BOOL					RenderInterfaceEffect(LPDIRECTDRAWSURFACE7 pSurface);	// Render Interface Effect

	virtual void					ClearEffect(void);										// Clear Effect
	virtual BOOL					AddEffect(SEffectData EffectData);						// Add Effect
	virtual void					SetCloud(BOOL fCloud);									// Set Cloud

	virtual void					SetWeather(BOOL fWeather);								// Set Weather
	virtual SWeatherType			GetNowPositionWeather(void);							// 현 위치에서의 날씨들
	virtual void					ChangeMap(SI16 Index);									// Map Change
};	


#endif
