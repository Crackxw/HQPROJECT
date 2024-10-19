#ifndef _KINDINFO_H
#define _KINDINFO_H  

#include <CharAnimation.h>

// #define PRINT_CHARACTER_INF
class	cltOnlineWorld;

//-------------------------------------------------------------------------------------------------
// 속성 정의 
// 같은 종류의 유닛에게 영향을 미치는 속성. 
// 게임중 변경될 수 없는 속성이다. (초기화시 한번 설정하면 재수정 불가.!!!) 
//-------------------------------------------------------------------------------------------------
#define KINDATB_WATER							0x00000002			// 수상 캐릭터 
#define KINDATB_FLY								0x00000004			// 공중 캐릭터 
#define KINDATB_GENERAL							0x00000008			// 장수 캐릭터 
#define KINDATB_SWIM							0x00000100			// 수영 가능 
#define KINDATB_GEFFECTHIDE						0x00000200			// 장수 효과 - 부하 은신 
#define KINDATB_GEFFECTAPOWER					0x00000400			// 장수 효과 - 공격력 증대 
#define KINDATB_GEFFECTDPOWER					0x00000800			// 장수 효과 - 방어력 증대 
#define KINDATB_GEFFECTHEALTH					0x00001000			// 장수 효과 - 체력 회복  
#define KINDATB_GEFFECTGUN						0x00002000			// 장수 효과 - 총쏘는 유닛의 성능 향상 
#define KINDATB_AMBUSH							0x00004000			// 매복가능
#define KINDATB_AIRMAGIC						0x00010000			// 공중탄 사용 가능
#define KINDATB_SHOWORIGINALTEAMNUMBER			0x00020000			// 원래 플레이어 번호를 표시하는가 ?
#define KINDATB_DETECTMINE						0x00040000			// 지뢰 제거 가능
#define KINDATB_HERO							0x00080000			// 주인공 캐릭터(조합형)

#define KINDATB_SIEGE_WARFARE_UNIT				0x00100000			// robypark 2004/10/18 16:33: 공성전 캐릭터인지를 나타내는 속성 값

//-----------------------------------------------------------------------------
// Name: ST_KI_SUBDATA
// Code: actdoll (2004-03-06)
// Desc: KindInfo 초기화시 사용할 보조 데이터 구조체
//-----------------------------------------------------------------------------
struct ST_KI_SUBDATA 
{
	// KindInfo 서브 데이터 옵션들
	SI32	m_siOptionType_01;
	SI32	m_siOptionType_02;
	SI32	m_siOptionType_03;

	// KindInfo 서브 데이터 옵션들에 적용될 수치 첫번째
	SI32	m_siOptionValue1_01;
	SI32	m_siOptionValue1_02;
	SI32	m_siOptionValue1_03;

	// KindInfo 서브 데이터 옵션들에 적용될 수치 두번째
	SI32	m_siOptionValue2_01;
	SI32	m_siOptionValue2_02;
	SI32	m_siOptionValue2_03;
}; 

//-------------------------------------------------------------------------------------------------
// 캐릭터와 건물의 각종 정보를 저장한다.
//-------------------------------------------------------------------------------------------------
class _KindInfo 
{
protected:
	// robypark 2004-5-24, 14:59: 접근전이 불가능한 캐릭터의 최소 사격 가능지점에 관한 데이터 추가
	char					m_nMinAttackRange[2];		// 캐릭터가 공격 가능한 최소거리(기본 값으로 1)

public:
	// robbypark 2004-5-24, 14:59: 접근전이 불가능한 캐릭터의 최소 사격 가능지점에 관한 데이터 추가
	// 캐릭터가 공격 가능한 최소거리 얻기
	// SI32 siAttackAtb[in]: 몇 번째 공격 속성인가? (0 or 1)
	// return value: 해당 공격 속성의 공격 가능한 최소거리
	char GetMinAttackRange(SI32 siAttackAtb)
	{
		return m_nMinAttackRange[siAttackAtb];
	}

	// robbypark 2004-5-24, 14:59: 접근전이 불가능한 캐릭터의 최소 사격 가능지점에 관한 데이터 추가
	// 캐릭터가 공격 가능한 최소거리 설정
	// SI32 siAttackAtb[in]: 몇 번째 공격 속성인가? (0 or 1)
	// char nMinAttackRange[in]: 공격 가능한 최소거리 값
	// return value: 해당 공격 속성의 공격 가능한 최소거리
	void SetMinAttackRange(SI32 siAttackAtb, char nMinAttackRange)
	{
		m_nMinAttackRange[siAttackAtb] = nMinAttackRange;
	}

private:
	SHORT 					m_ssOnlinePortraitFont;							// 거상에서 일반용 초상화의 이미지 폰트, 전투 중에도 사용됨. 
	SHORT 					m_ssOnlineMercenaryFont;						// 거상에서 용병용 초상화의 이미지 폰트 
//	SHORT 					MaxApower;
	SI32					m_siExpVaryPercent;								// 경험치 가중치. 
	SI32 					m_siAttackRange1, m_siAttackRange2;				// 공격1 사정거리, 공격2 사정거리
	DWORD 					m_uiAttackAtb1;									// 공격 속성 1
	DWORD 					m_uiAttackAtb2;									// 공격 속성 2
	SI32 					m_siAttackDelay1;								// 공격 1 지연시간
	SI32 					m_siAttackDelay2;								// 공격 2 지연시간
	SI08					m_scMagicApplyRate;								// 마법 적용률(최종 Damage 계산에 적용된다)
	
public:
	DWORD 					m_uiUnique;										// 정보들이 변조되었는지를 알수 있는 유니크 값.
	SI32					m_siServerID;									// 서버에서 사용되는 이 캐릭터의 대응 ID

	SI32					m_siAttackEffect1;								// 공격1의 효과음
	SI32					m_siAttackEffect2;								// 공격2의 효과음
	
	SHORT 					m_ssBottomIndent;        						// 그림 폰트의 바닥에서 발이 얼마나 떨어져 있는가?
	SHORT 					m_ssIDXsize, m_ssIDYsize;    					// 캐릭터가 전투맵의 타일에서 차지하는 타일의 개수. 가로, 세로.

	SHORT					m_ssXsizePercent, m_ssYsizePercent;				//  전투 중에 캐릭터 선택 박스 크기. 
	int						m_siKindAtb;									// 주인공(조합형), 장수, 등의 캐릭터인지를 나타내는 속성
	SHORT 					m_ssNation;             						// 어느나라의 캐릭터인가?
	SHORT 					m_ssMoveSpeed;									// 캐릭터 이동속도(전투)
						
	SI32					m_siSearchRange;								// 캐릭터 시야(전투)
	SHORT 					m_ssMaxFireRepeatNumber, m_ssFireRepeatDelay;
	SHORT 					m_ssWorkDelay, m_ssDeadDelay, m_ssTowerDelay, m_ssChangeDirectionDelay, m_ssAttackGiveUpDelay;
	DWORD 					m_uiAtb;										// 공격 속성 타입 1
	DWORD 					m_uiAtb2;										// 공격 속성 타입 2
	DWORD 					m_uiAtb3;										// 공격 속성 타입 3
	DWORD					m_uiAtb4;										// 공격 속성 타입 4
	DWORD					m_uiAtb5;										// 공격 속성 타입 5 robypark 2004/10/1 17:6
	DWORD 					m_uiDyingAtb;									// 사망시 속성(시체 부패, 등)
	DWORD 					m_uiDefenceAtb;									// 방어구 타입(2004/10/5 사용안됨)
	SHORT 					m_ssVibrateSwitch;								// 공중 유닛 효과 스위치
	SHORT					m_ssPriority_Attack_Type;						// robypark 2004/10/18 12:10 - 두 가지 공격 속성을 가진 경우 인공지능 처리관련 값

	ST_KI_SUBDATA			m_SubData;			// actdoll (2004/03/06 3:19) : 보조 데이터가 필요하므로 추가한다.
		
	SI32					m_siTryAttackEffect;	// 필드에서 공격할 때 효과음. 
	SI32 					m_siSelectEffect;		// 효과음. 캐릭터 선택
	SI32 					m_siDyingEffect;		// 효과음. 캐릭터 사망
	SI32 					m_siMoveEffect;			// 효과음. 캐릭터 이동. 이동명령
	SI32 					m_siAttackEffect;		// 효과음. 캐릭터 공격

	char 				   *m_pszName;				// 캐릭터 명
	char 				   *m_pszIDName; 

	SHORT					m_ssHeroID;				// 주인공(조합형) 캐릭터 ID
	SHORT 					m_ssDefaultArmor;		// 주인공(조합형) 캐릭터 기본 방어구
	SHORT 					m_ssDefaultHelmet;		// 주인공(조합형) 캐릭터 기본 헬멧
	SHORT 					m_ssDefaultWeapon;		// 주인공(조합형) 캐릭터 기본 무기


	SI32					m_siCharAnimationInfo[MAX_ANIMATION_PER_UNIT];	// 캐릭터 애니메이션 정보
	cltCharAnimation		m_clCharAnimation[MAX_ANIMATION_PER_UNIT];		// 캐릭터 애니메이션 관리자

#ifdef PRINT_CHARACTER_INF
	char					m_nameCode[200];
	char					m_aniFileName[MAX_ANIMATION_PER_UNIT][200];

	void					PrintInf(void);
#endif

#ifndef _USE_HASHCODE_EFFECTSOUND
	void					Init(char*namecode,
								int kindatb, 
								SHORT movespeed, 
								SI32 expvarypercent, 
								SI32 attackrange1, SI32 attackrange2, SHORT searchrange, 
								SI32 attackdelay1, SI32 attackdelay2,
								SHORT maxfirerepeatnumber, SHORT firerepeatdelay,
								SHORT workdelay, SHORT deaddelay, SHORT towerdelay, SHORT changedirectiondelay, SHORT attackgiveupdelay, 
								DWORD atb, 
								DWORD atb2,
								DWORD atb3,
								DWORD atb4,
								DWORD dyingatb, 
								SI32 attackatb, SI32 attackatb2,
								SI32 attackeffect1, SI32 attackeffect2,
								DWORD defenceatb,
								SHORT vibrateswitch, 
								SI32 tryattackeffect, SI32 selecteffect, SI32 dyingeffect, SI32 moveeffect, SI32 attackeffect, 
								char* name, char *idname, 
								SHORT heroid, SHORT defaultArmor, SHORT defaultHelmet, SHORT defaultWeapon, 
								ST_KI_SUBDATA	*pSubData	= NULL,
								SI08 magicapplyrate			= 70);
#else
	void					Init(char*namecode,
								int kindatb, 
								SHORT movespeed, 
								SI32 expvarypercent, 
								SI32 attackrange1, SI32 attackrange2, SHORT searchrange, 
								SI32 attackdelay1, SI32 attackdelay2,
								SHORT maxfirerepeatnumber, SHORT firerepeatdelay,
								SHORT workdelay, SHORT deaddelay, SHORT towerdelay, SHORT changedirectiondelay, SHORT attackgiveupdelay, 
								DWORD atb, 
								DWORD atb2,
								DWORD atb3,
								DWORD atb4,
								DWORD dyingatb, 
								SI32 attackatb, SI32 attackatb2,
								const char* attackeffect1, 
								const char* attackeffect2,
								DWORD defenceatb,
								SHORT vibrateswitch, 
								const char* tryattackeffect, 
								const char* selecteffect, 
								const char* dyingeffect, 
								const char* moveeffect, 
								const char* attackeffect, 
								char* name, char *idname, 
								SHORT heroid, SHORT defaultArmor, SHORT defaultHelmet, SHORT defaultWeapon, 
								ST_KI_SUBDATA	*pSubData	= NULL,
								SI08 magicapplyrate			= 70);
#endif
	// robypark 2004/9/23 14:50
	// 캐릭터의 몇 몇 정보를 HQ의 CharInfo.txt파일에서 읽어오도록 수정한다.
	void					Init(char*namecode,
								int kindatb, 
								DWORD atb, 
								DWORD atb2,
								DWORD atb3,
								DWORD atb4,
								DWORD atb5,
								DWORD dyingatb, 
								SI32 attackatb1,
								SI32 attackatb2,
								DWORD defenceatb,
								char* name,
								char *idname, 
								SHORT defaultarmor = -1,
								SHORT defaulthelmet = -1,
								SHORT defaultweapon = -1,
								SHORT heroid				= 0,
								ST_KI_SUBDATA	*pSubData	= NULL,
								SI08 magicapplyrate			= 70);

	void					SetFile(SI32 Ani, BOOL DirSwitch, char* File,SI32 Font,SI32 Frame, SI32 Instant, SI32 Delay, SI32 atb=0);

	DWORD					GetUnique( void );


	BOOL					HaveUpgradeType(SHORT upgrade);

	DWORD					Add( void );

	SI32					GetAttackRange(SI32 currentattack);
	SI32					GetAttackAtb(SI32 currentattack);
	SI32					GetAttackDelay(SI32 currentattack);

	int						IsKindAtb(int atb)
	{
	   if( m_siKindAtb & atb )	return TRUE;
	   return FALSE;
	}

	// 시작 /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 18:19
	// 캐릭터 속성 검사 함수 추가
	// 캐릭터 속성1에 해당 플래그 속성 존재하는지 검사
	int						IsAttribute1(DWORD dwAttribute)
	{
		if (m_uiAtb & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// 캐릭터 속성2에 해당 플래그 속성 존재하는지 검사
	int						IsAttribute2(DWORD dwAttribute)
	{
		if (m_uiAtb2 & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// 캐릭터 속성3에 해당 플래그 속성 존재하는지 검사
	int						IsAttribute3(DWORD dwAttribute)
	{
		if (m_uiAtb3 & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// 캐릭터 속성4에 해당 플래그 속성 존재하는지 검사
	int						IsAttribute4(DWORD dwAttribute)
	{
		if (m_uiAtb4 & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// 캐릭터 속성5에 해당 플래그 속성 존재하는지 검사
	int						IsAttribute5(DWORD dwAttribute)
	{
		if (m_uiAtb5 & dwAttribute)
			return TRUE;

		return FALSE;
	}
	// 끝 /////////////////////////////////////////////////////////////////

	// robypark 2004/11/3 10:35 캐릭터 공격 속성(무기 사용) 효과음 얻기
	SI32	GetWeaponAttackEffect	( SI32 siAttackIdx )	
	{
		switch (siAttackIdx)
		{
			case 0:
				return m_siAttackEffect1;
			case 1:
				return m_siAttackEffect2;
			default:
				break;
		}

		return 0;
	}

	SHORT					GetMakeDelay			( void );

	SHORT					GetOnlinePortraitFont	( void )			{ return m_ssOnlinePortraitFont; }
	SHORT					GetOnlineMercenaryFont	( void )			{ return m_ssOnlineMercenaryFont; }

	SI32					GetExpVaryPercent		( void )			{ return m_siExpVaryPercent; }
	SI08					GetMagicApplyRate		( void )			{ return m_scMagicApplyRate; }
	void					SetMagicApplyRate		( SI08 magicrate )	{ m_scMagicApplyRate = magicrate; }

	// 종류별로 중심 좌표를 기준으로 캐릭터를 찍어준다. 
	void Draw(UI16 kind, SI32 animation, UI16 Frame, SI16 X, SI16 Y, cltOnlineWorld *pWorld, SI32 armourcode=0, SI32 helmetcode=0, SI32 weaponcode=0, SI08 siSlot=0);

	// robypark 2004/10/18 12:10
	SHORT					GetPriority_Attack_Type	( void )			{ return m_ssPriority_Attack_Type; }
	void					SetPriority_Attack_Type ( SHORT ssPriority_Attack_Type )	{ m_ssPriority_Attack_Type = ssPriority_Attack_Type; }

	// robypark 2004/11/3 18:5
	// 캐릭터 애니메이션 정보 인덱스 얻기
	SI32					GetCharAnimationInfoIdx(SI32 siAnimationIdx)
	{
		SI32 siAnimatonInfoIdx = 1;

		for (siAnimatonInfoIdx = 1; siAnimatonInfoIdx < MAX_ANIMATION_PER_UNIT; siAnimatonInfoIdx++ )
		{
			if( m_siCharAnimationInfo[siAnimatonInfoIdx] == siAnimationIdx)
			{
				return siAnimatonInfoIdx;
			}
		}

		return 0;
	}

	// robypark 2004/11/3 18:9
	// 캐릭터 애니메이션 중에 특정 동작(함수 호출) 발생 시점 폰트 번호 얻기
	SI32					GetCharAniamtionInstant(SI32 siAnimationIdx)
	{
		return m_clCharAnimation[GetCharAnimationInfoIdx(siAnimationIdx)].GetAnimationInstant();
	}
};

BOOL	InitKindInfo( void );
SI32	TransKindOnlineToImjin( SI32 kind, BOOL bShowErrorMsg = true );
SI32	TransKindImjinToOnline( SI32 kind, BOOL bShowErrorMsg = true );
char	*GetKindName(SHORT kind, SHORT pnumber );

//특정 캐릭터에 대해서 공격시도할 때 효과음을 연주한다. 
BOOL PlayTryAttackEffect(SI32 onlinekind);
//특정 캐릭터를 필드상에 보이게 할 때 효과음을 연주한다. 
BOOL PlayShowPetEffect(SI32 onlinekind);

// 공격 속성에 따라 공격 지연 시간을 얻어온다. 
SI32 DecideAttackDelayByAttackAtb(SI32 attackatb);

// kindinfo등의 해킹 여부를 검사한다. 
BOOL CheckKindInfo( void );

#endif