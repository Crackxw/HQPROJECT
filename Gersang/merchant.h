#ifndef _MERCHANT_H
#define _MERCHANT_H

#define MERCHANT_INTERVAL    1000

// 3국의 상인을 정의한다. 
#define MAX_MERCHANT_TYPE	   3
#define MERCHANT_TYPE_KOREA    0
#define MERCHANT_TYPE_JAPAN	   1
#define MERCHANT_TYPE_CHINA    2

#define MAX_GOODS_KIND		50
#define GOODS_FOOD			0
#define GOODS_TREE			1
#define GOODS_MINE			2        // 여여문의 지뢰 (명)
#define GOODS_RAINMAKER		3        // 기우제용 주문 (조선, 일본)
#define GOODS_SAMARROW		4        // 대공 화전(일본 구입)
#define GOODS_ELEPHANT		5        // 코끼리 구입 (명)
#define GOODS_AIRMAGIC		6        // 공중탄 (조선) 
#define GOODS_BULL			7        // 황소   (조선)
#define GOODS_DRAGONBALL	8		 // 여의주   (명)
#define GOODS_GINSENG		9        // 인삼   (조선, 일본, 명)
#define GOODS_BOW1			10       // 대궁
#define GOODS_BOW2			11       // 철력궁 
#define GOODS_RIFLE1		12       // 용쇄총
#define GOODS_GUN1			13       // 드래군파이슨
#define GOODS_CANNON1		14		 // 철릉포
#define GOODS_SWORD1		15       // 청홍검
#define GOODS_SWORD2		16       // 폭마혈도
#define GOODS_SWORD3		17       // 쿠사나기의 검
#define GOODS_SWORD4		18       // 패왕도
#define GOODS_SWORD5		19       // 칠성검
#define GOODS_SWORD6		20       // 부동명왕검
#define GOODS_SWORD7		21       // 용비늘검
#define GOODS_SPEAR1		22       // 방천화극
#define GOODS_SPEAR2		23       // 흑철창
#define GOODS_SPEAR3		24       // 장팔사모
#define GOODS_AXE1			25       // 참마부도
#define GOODS_BOOK1			26       // 육도삼략
#define GOODS_BOOK2			27       // 손자병법
#define GOODS_BOOK3			28       // 화타의책
#define GOODS_NECKLACE1		29       // 달마의 염주
#define GOODS_ARMOUR1		30       // 등갑옷
#define GOODS_ARMOUR2		31       // 용기린갑옷
#define GOODS_ARMOUR3		32       // 현무갑옷
#define GOODS_ARMOUR4		33       // 수룡왕갑옷
#define GOODS_ARMOUR5		34       // 카케무샤의 투구
#define GOODS_BELT1			35       // 적토마의 갈기
#define GOODS_BELT2			36       // 화룡천마의 허리띠
#define GOODS_MED1			37       // 백설산삼
#define GOODS_MED2			38       // 백사탕
#define GOODS_MED3			39       // 용왕탕
#define GOODS_MED4			40       // 웅족탕
#define GOODS_MED5			41       // 진화탄
#define GOODS_STICK1		42       // 나찰신장봉
#define GOODS_STICK2		43		 // 백팔귀마봉
#define GOODS_MACE1			44       // 참철살퇴
#define GOODS_CLAW1			45       // 흑갈조도

class _GoodsEffect{
public:
	// 필요 조건 
	SI32 siNeedKind;			// 제품 구입의 조건 - 필요 유닛. 

	// % 기준. 
	SI32 siApower;              // (%)
	SI32 siAttackRange;			// (%)
	SI32 siDpower;				// (%)
	SI32 siStealHealth;         // (%) 적의 체력을 빼앗아 오는 정도. 
	SI32 siGeneralEffectApower;	// (%) 장수 효과(공격력 향상)
	SI32 siGeneralEffectDpower;	// (%) 장수 효과(방어력 향상)
	SI32 siMagicpower;			// (%) 마법력 향상
	SI32 siMoveSpeed;			
	SI32 siHealth;	

	
	// 절대 수치 기준. 
	SI32 siMagicPowerAbs;		
	SI32 siHealthRecoverAbs;	// 체력이 회복되는 주기(프레임 단위)


	// 스위치 
	BOOL bRecoverMagicPowerSwitch ;
	BOOL bStrongAttackSwitch ;			// 일격필살 존재 여부. 
	BOOL bKimBombSwitch;				// 폭뢰격 사용 가능 여부. 
	BOOL bMagic5jSwitch;				// 만월도끼춤 사용 가능 여부. 
	BOOL bCallDragonSwitch;				// 승천용 소환. 
	BOOL bMagic8kSwitch;				// 쐐기 지르기 .
	BOOL bGeneralEffectFireResistSwitch; // 장수효과 (불에대한 저항)
	BOOL bClonSwitch;						// 분신술을 사용하는지...
	BOOL bRecoverHealthSwitch;				// 체력 회복 . 
	BOOL bExtinguishSwitch;				// 주변의 불을 끈다. 

	// 

	void Init(){ZeroMemory(this, sizeof(_GoodsEffect)); }
};

class _Goods{
public:
	SHORT	Money, Tree;
	SI32	siBuyUnit;			// 판매 단위 

	SI32    siFont;
	SI32    siPercent;

	char	Text[32];
	DWORD	Property;

	_GoodsEffect GE;

	void Init(SHORT money, SHORT tree, SI32 sipercent, SI32 sifont, SI32 sibuyunit, char* text, SI32 property, _GoodsEffect *ge);

};




class _PlayerMerchant{
public:
	SI32 PlayerNumber;

	SHORT MerchantSwitch;        // 상인이 어딘가에 존재하는가?
	SI32  MerchantType ;         // 상인의 종류 (조선상인, 일본상인, 명상인등...)
	DWORD MerchantFrame;         // 상인이 존재하기 시작한 프레임 

	SHORT Goods[MAX_GOODS_KIND];             // 상품의 수량   

	// 물품을 준비한다. 
    void ReadyGoods();
	void Action();
	void MoveToPlayer();
	// 이 제품을 상인이 가져올 필요가 있는지 확인한다. 
	BOOL CanBuyGoods(SI32 sigood);

};
// 상인 클래스 
class _Merchant {
public:
	_PlayerMerchant PM[MAX_MULTI_PLAYER];

	void Init();

	void Action();
};

BOOL GetItemHelp(SI32 goods, char* helpstring);

void GoodsInit();

#endif