/************************************************************************************************
	���ϸ� : OnlineFieldEffect.h
	����� : ������
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
	, EFFECTTYPE_VILLAGE_DEFENCE_UP	// ���� ���� ��� ȿ��
	, EFFECTTYPE_OCCUPIED_VILLAGE	// ���� ���� ȿ��

	, EFFECTTYPE_FIELD_CHANGENUMBER	// ���� ��ȭ

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
#define ON_FIELDEFFECT_ANIMATION_OCCUPIEDVILLAGE_REPEATTIMES	3	// ���� ���� ����Ʈ �ִϸ��̼� �ݺ�Ƚ��

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

// �ʵ忡 ǥ�õǴ� ���ں�ȭ
typedef struct
{
	SI16		m_Frame;

	SI16		m_MapXPos;
	SI16		m_MapYPos;
	SI08		m_Color;		// 0: Green, 1: Red, 2: Blue, 3: White
	bool		m_bVillageDefence;	// ���� ������ ��� ������ ��ġ ������ ���Ѵ�.
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
// ���� ���� ���
typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;
} SEffectData_VillageDefenceUp;

// robypark 2004/12/2 18:4
// ���� ����(�׺��� �� ���) ȿ��
typedef struct
{
	SI16		m_Frame;

	SI16		m_XPos;
	SI16		m_YPos;

	// robypark 2004/12/27 18:19
	SI08		m_scCurrentRepeatTimes;	// ���� Animation �ݺ� ��� Ƚ��
	SI08		m_scMaxRepeatTimes;		// �ִ� Animation �ݺ� ��� Ƚ��
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
		SEffectData_VillageDefenceUp		m_EffectData_VillageDefenceUp;	// ���� ���� ���
		SEffectData_OccupiedVillage			m_EffectData_OccupiedVillage;	// ���� ���� ȿ��
		SEffectData_Field_ChangeNumber		m_EffectData_FieldChangeNumber;	// �ʵ� ���� ��ȭ ȿ��
	};
} SEffectData;

#define RAIN_MAX					256
#define SNOW_MAX					256

typedef struct
{
	SI08				m_Type;					// 0 : None, 1 : �������� ��, 2 : �� ��������
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
	SI08				m_Type;					// 0 : None, 1 : �������� ��, 2 : �� ��������
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

// �ʵ忡�� Effect ����
class	OnlineFieldEffect
{
public:
	cltOnlineWorld*		m_lpWorld;

	SEffectData*		m_lpFieldEffect;
	SI32				m_FieldEffectCount;

	XSPR				m_CloudSpr;
	
	BOOL				m_fCloud;

	// robypark 2004/8/11 10:37
	// ��ġ ��������Ʈ �߰�
	XSPR				m_SprMagpie;

	BOOL				m_fWeather;
	SEffectRain			m_Rain;
	SEffectSnow			m_Snow;

public:
	OnlineFieldEffect();																	// ������.
	virtual	~OnlineFieldEffect();															// �Ҹ���.
		
	virtual	BOOL					Init(cltOnlineWorld* lpWorld, SI32 MaxEffectCount);		// �ʱ�ȭ
	virtual	VOID					Free(void);												// ����.

	virtual BOOL					Pool(void);												// ó��
	virtual BOOL					Render(LPDIRECTDRAWSURFACE7 pSurface);					// Render
	virtual BOOL					RenderInterfaceEffect(LPDIRECTDRAWSURFACE7 pSurface);	// Render Interface Effect

	virtual void					ClearEffect(void);										// Clear Effect
	virtual BOOL					AddEffect(SEffectData EffectData);						// Add Effect
	virtual void					SetCloud(BOOL fCloud);									// Set Cloud

	virtual void					SetWeather(BOOL fWeather);								// Set Weather
	virtual SWeatherType			GetNowPositionWeather(void);							// �� ��ġ������ ������
	virtual void					ChangeMap(SI16 Index);									// Map Change
};	


#endif
