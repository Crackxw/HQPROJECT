#ifndef _CALCATTACK_H
#define _CALCATTACK_H

#define ATTACKATB_ATB_TYPE1						0x1			
#define ATTACKATB_ATB_TYPE2						0x2			
#define ATTACKATB_ATB_TYPE3						0x4			
#define ATTACKATB_ATB_PHYSICS					0x8
#define ATTACKATB_ATB_MAGIC						0x10
#define ATTACKATB_ATB_LOWASSISTDPOWER			0x20		// 포복일때 방어력의 향상을 얻을 수 있다. 
#define ATTACKATB_ATB_AIR						0x40
#define ATTACKATB_ATB_MINRANGE					0x80
#define ATTACKATB_ATB_PULL						0x100		// 맞았을 떄 뒤로 밀림. 
#define ATTACKATB_ATB_PALSY						0x200		// 맞았을 떄 마비 효과. (털가면 사용)
#define ATTACKATB_ATB_VAMPIRE					0x400		// 체력 흡수. 

// robypark 2004/6/10 17:25
#define ATTACKATB_ATB_POISON					0x800		// 맞았을 때 중독효과

#define MAX_HIT_LIST_NUMBER    400


// private:
// 공격속성, 공격력, 공격대상을 고려하여 실제 피해를 계산한다. 
SHORT CalculateHitPoint(SHORT attackatb, SHORT power, _NewID attackid);


// public:

// 공격의 여러 속성을 고려하여 적의 체력을 저하시킨다. 
// 파라메타 설명:
// myid      : 공격을 시도하는 캐릭터의 아이디 
// myplayernumber: 공격을 시도하는 캐릭터의 플레이어 번호 
// attackatb : 공격속성(main.h에 정의 )
// range     : 피해 범위, 일반적으로 1( 1이면 -1 ~ +1)
// area      : 공격 대상 지역, 하늘이냐 땅이냐  (AREA_LAND, AREA_AIR)
// power     : 공격력 
// attackid  : 특별히 맞추고 싶은 캐릭터의 아이디 
// dx, dy    : 타격을 줄 목적지의 좌표 

// 리턴값   
//  0 -> 항상 0을 리턴 
SHORT CalculateHit(_NewID myid, SHORT myplayernumber, SHORT attackatb, SHORT range, SHORT area, SHORT power, _NewID attackid, SHORT dx, SHORT dy, SI32 hitrate);


// 적에대한 공격과 공격에 사후 조치를 취한다. 
BOOL HitNOperateAfterHit(_NewID myid, _NewID enemyid, SHORT myplayernumber, SHORT attackatb, SHORT power);


void InitAttackAtb();

// 각 공격 속성에 대한 데미지를 구한다. 
SI32 CalcDamage(SI32 attackatb, SI32 para1=0, SI32 para2=0, SI32 para3=0, SI32 para4=0);

// 공격속성과 캐릭터 속성에 따른 공격력 수정. 
// 공격자의 원공격력, 공격자의 공격속성, 방어자의 속성을 근거로 공격자의 원공격력을 수정한다. 
SI32 CalcApowerByAtb(SI32 apower, SI32 attackatb, SI32 atb);

#endif
