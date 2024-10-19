#ifndef _MAGIC_H
#define _MAGIC_H


// 마법에 필요한 에너지
#define MAGICPOWER_BETRAY     				60		// 현혹에 필요한 마법력 
#define MAGICPOWER_ABSORB     				40		// 흡수에 필요한 마법력 
#define MAGICPOWER_MAGIC8J     				50		// 환영술에 필요한 마법력 
#define MAGICPOWER_EARTHQUAKE 				40		// 지진에 필요한 마법력 
#define MAGICPOWER_TORNADO    				100		// 풍백술에 필요한 마법력 
#define MAGICPOWER_LIGHTNING  				40		// 번개에 필요한 마법력 
#define MAGICPOWER_PROTECTOR  				50		// 보호막에 필요한 마법력 
#define MAGICPOWER_STEALTECH  				90		// 기술 훔치기에 필요한 마법력 
#define MAGICPOWER_RAINMAKE   				140		// 기우제 
#define MAGICPOWER_WARP       				30		// 공간이동 
#define MAGICPOWER_HEAL       				20		// 치료 
#define MAGICPOWER_MANA						20		// 마나 치료.
#define MAGICPOWER_STEALEXP   				40
#define MAGICPOWER_CLON       				80		// 자신을 복제하는 마법력
#define MAGICPOWER_ENEMYCLON       			40 		// 적을 복제하는 마법력
#define MAGICPOWER_SHIELD					80		// 방어막을 쳐주는데 소모되는 마법력
#define MAGICPOWER_GROUNDEXP				100		// 연옥술 만드는데 필요한 마법력 
#define MAGICPOWER_MAGIC4C					50      // 기공신포. 
#define MAGICPOWER_MAGIC6C					100		// 포박술 만드는데 필요한 마법력 
#define MAGICPOWER_KIMBOMB					100		// 김덕령 폭뢰격 만드는데 필요한 마법력 
#define MAGICPOWER_MAGIC5J					50		// 만월도끼춤 만드는데 필요한 마법력 
#define MAGICPOWER_DETECTMINE				40  
#define MAGICPOWER_DRAGON					200     // 승천용 소환에 소모되는 마법.   
#define MAGICPOWER_TOADPOISON				20      /// 독두꺼비가 중독 시키는데 필요한 마법. 
#define MAGICPOWER_JUSTICEK					2       /// 유생이 적을 마비시키는데 필요한 마법. 
#define MAGICPOWER_WIDOW					10      /// 거미요괴 거미줄 마법. 
#define MAGICPOWER_WAVE						20		// 바다거북이의 파동 마법 . 
#define MAGICPOWER_FIRE						10		// 화염 공격에 소모되는 마법. 
#define MAGICPOWER_MAGIC8K					2		// 쐐기지르기 
#define MAGICPOWER_FIREARROW2				2		// 마법불화살
#define MAGICPOWER_EXTINGUISH				10		// 소화에 필요한 마법. 
#define MAGICPOWER_MAGIC4T    				100		// 원심분쇄격(흑룡대차륜)에 필요한 마법력 
#define MAGICPOWER_MAGIC2T    				100		//독무술에 필요한 마법력 
#define MAGICPOWER_RECALL_PANDA				300     //팬더 소환에 필요한 마법력
#define MAGICPOWER_RECALL_MONKEY			100     //원숭이 소환에 필요한 마법력 
#define MAGICPOWER_RECALL_AMAKUSA			10		//요술사 소환에 필요한 마법력.
#define MAGICPOWER_MINE						2		//지뢰 매설.
#define MAGICPOWER_YELLOWSAND				100		//모래의신의 황사 마법.
#define MAGICPOWER_ICE						100	    // 이사의 사념 특수기술.
#define MAGICPOWER_BIGTREE					50		// 거대수 특수공격. 
#define MAGICPOWER_SOUL						80		// 왕거한 야수의 혼.
#define MAGICPOWER_GHOST					30		// 페르난데스 귀신탄.
#define MAGICPOWER_DARKFIRE					100		// 임평후 암흑화염 폭풍.
#define MAGICPOWER_IVORY					30		// 상아폭격.
#define MAGICPOWER_BLUEDRAGON				30		// 화공신포.
#define MAGICPOWER_RUNNINGFIRE				150		// 연발포탄.
#define MAGICPOWER_ENERGYPA					20		// 장풍.
#define MAGICPOWER_SNOWATTACK				30		//
#define MAGICPOWER_FIREATTACK				50		// 화공신포.
#define MAGICPOWER_FIREENERGYPA				100		// 열화장풍(비격황룡권).
#define MAGICPOWER_RECALL_TIGER				50		// 비호의 졸개 호랑이 소환.

#define MAGICPOWER_DOUBLE_SWORD				100		// 쌍검난무
#define MAGICPOWER_LONG_SPEAR				300		// 장창소환
#define MAGICPOWER_ICE_BARRIER				50		// 빙석술
#define MAGICPOWER_FLOWER_NEEDLE_RAIN		100		// 만천화우
#define MAGICPOWER_SOUL_CHANGE				100		// 빙의술
#define MAGICPOWER_STONE_MAGIC				60		// 석괴술
#define MAGICPOWER_GOD_SPEAR				40		// 신창노도
#define MAGICPOWER_CLOUD_WIND				20		// 풍모술
#define MAGICPOWER_MANABURN					0		// 마나번


//----------------------------------------------------------------------------
// robypark (2004/05/17 13:57) : 추가 기술 소모 마법력- 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
#define MAGICPOWER_FIRE_ROUND_DANCE			200		// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
#define MAGICPOWER_NOVA						250		// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
#define MAGICPOWER_FURY_OF_VOLCANO			200		// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
#define MAGICPOWER_POISONING_HEAVENS		250		// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
#define MAGICPOWER_BOMB_OF_HEAVEN			200		// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
#define MAGICPOWER_SIEGE_MODE				200		// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
#define MAGICPOWER_ONSLAUGHT_FIREBALL		200		// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
#define MAGICPOWER_FIRERAIN_OF_GOD			225		// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) => 마나소비량 250=>225 robypark 2004/11/2 11:34

// robypark 2004/6/4 14:12
#define MAGICPOWER_SIEGE_MODE_UNDO			0		// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령<=순비연 2차 전직 기술)

// robypark 2004/11/11 11:17
#define MAGICPOWER_SIEGE_MODE_FIRE_MISSILE	25		// 중국 불랑기포차 앙천대소일 때 공격 미사일 발사 소모 마나량

////////////////////////////////////////////////////////////////////////////
#define MAGICPOWER_MAGICTEST				1       //테스트 마법에 필요한 마법력.
////////////////////////////////////////////////////////////////////////////


class _Magic {
public:
	SI32 m_siMagicKind;
	SI32 m_siX, m_siY;
	_NewID m_MagicID;
	SI32 m_siMagicPlayerNumber;
	SI32 m_siArea;
	SI32 m_siApower;
	_NewID m_AttackID;

	SI32 m_siStep;
	SI32 m_siHitRate;

	SI32 m_siSmokeID;

	_Magic(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	virtual SI32 Action();
};


class _KimBomb : public _Magic{
public:

	_KimBomb(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _Magic5j : public _Magic{
public:

	_Magic5j(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32	Action();
	_NewID	FindNearEnemy(SI32 six, SI32 siy);
};


class _MagicExtinguish: public _Magic{
public:

	_MagicExtinguish(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, _NewID AttackID);
	SI32 Action();
};

class _MagicDetectMine: public _Magic{
public:

	_MagicDetectMine(SI32 simagickind, SI32 siX, SI32 siY, SI32 siplayernumber);
	SI32 Action();
};

class _Magic6c : public _Magic{
public:

	_Magic6c(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

// 기공신포. 
class _Magic4c : public _Magic{
public:

	_Magic4c(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//상아폭격.
class _MagicIvory : public _Magic{
public:
	_MagicIvory(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _MagicBlueDragon : public _Magic{

public:
	_MagicBlueDragon(SI32 siMagicKind,SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _MagicFireAttack : public _Magic{

public: 
	_MagicFireAttack(SI32 siMagicKind,SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _MagicRunningFire : public _Magic{

public:
	_MagicRunningFire(SI32 siMagicKind,SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();

	SI16	m_siCount;
};

class _MagicEnergyPa : public _Magic{

public:
	_MagicEnergyPa(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();

};

class _MagicFireEnergyPa : public _Magic{

public:
	_MagicFireEnergyPa(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action(); 


};

class _MagicIce : public _Magic{
public:
	_MagicIce(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//마법 테스트.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _MagicTest : public _Magic{
public:
	_MagicTest(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 설녀 눈폭풍. 
class _SnowMagic : public _Magic{
public:

	_SnowMagic(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

// 독무술. 
class _Magic2T : public _Magic{
public:

	_Magic2T(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

// 거대수 특수공격. 
class _BigTree : public _Magic{
public:

	_BigTree(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//쌍검난무.
class _MagicDoubleSword : public _Magic{
public:
	_MagicDoubleSword(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//마나번.
class _MagicManaBurn : public _Magic{
public:
	_MagicManaBurn(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//장창소환.
class _MagicLongSpear : public _Magic{
public:
	_MagicLongSpear(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//빙석술.
class _MagicIceBarrier : public _Magic{
public:
	SI32 m_siBlockX[9], m_siBlockY[9];

	_MagicIceBarrier(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//만천화우
class _MagicFlowerNeedleRain : public _Magic{
public:
	SI32 m_siSmokesID[4];

	_MagicFlowerNeedleRain(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//빙의술
class _MagicSoulChange : public _Magic{
public:
	_MagicSoulChange(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//석괴술
class _MagicStoneMagic : public _Magic{
public:
	_MagicStoneMagic(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//신창노도
class _MagicGodSpear : public _Magic{
public:
	_MagicGodSpear(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//풍모술
class _MagicCloudWind : public _Magic{
public:
	_MagicCloudWind(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:17) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
class _Magic_FIRE_ROUND_DANCE : public _Magic
{
	public:
		// 생성자
		_Magic_FIRE_ROUND_DANCE(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// 마법(기술) 동작 수행
		SI32 Action();

	protected:
		UI32				m_uiBeginTime;		// 강강수월래 기술 시전 시작 시간
		SI32				m_siFloorSmokeID;	// 강강수월래 바닥 Smoke ID
};

// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
class _Magic_FURY_OF_VOLCANO : public _Magic
{
	public:
		// 생성자
		_Magic_FURY_OF_VOLCANO(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// 마법(기술) 동작 수행
		SI32 Action();

	protected:
		UI32				m_uiBeginTime;			// 격노염폭 기술 시전 시작 시간
};

// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
class _Magic_POISONING_HEAVENS : public _Magic
{
	public:
		// 생성자
		_Magic_POISONING_HEAVENS(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// 마법(기술) 동작 수행
		SI32 Action();
};

// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
class _Magic_BOMB_OF_HEAVEN : public _Magic
{
	public:
		// 생성자
		_Magic_BOMB_OF_HEAVEN(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// 마법(기술) 동작 수행
		SI32 Action();
};

// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
class _Magic_FIRERAIN_OF_GOD : public _Magic
{
	public:
		// 생성자
		_Magic_FIRERAIN_OF_GOD(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// 마법(기술) 동작 수행
		SI32 Action();

	protected:
		UI32				m_uiBeginTime;		// 천벌화시 기술 시전 시작 시간
};



void MagicAction();
void DeleteMagic();
SI32 FindEmptyMagic();


void SetKimBomb(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagic5j(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicExtinguish(SI32 siX, SI32 siY);
void SetMagic6c(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagic4c(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicIvory(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicBlueDragon(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicFireAttack(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicRunningFire(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicIce(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetSnowMagic(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicMine(SI32 siX, SI32 siY);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMagicTest(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMagic2T(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicDetectMine(SI32 siX, SI32 siY, SI32 playernumber);
void SetBigTree(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicEnergyPa(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicFireEnergyPa(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicDoubleSword(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicManaBurn(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicLongSpear(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicIceBarrier(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicFlowerNeedleRain(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicSoulChange(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicStoneMagic(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicGodSpear(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicCloudWind(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:17) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
void SetMagic_FIRE_ROUND_DANCE(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
void SetMagic_FURY_OF_VOLCANO(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
void SetMagic_POISONING_HEAVENS(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
void SetMagic_BOMB_OF_HEAVEN(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
void SetMagic_FIRERAIN_OF_GOD(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

#endif
