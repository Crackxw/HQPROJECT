#ifndef _CALCATTACK_H
#define _CALCATTACK_H

#define ATTACKATB_ATB_TYPE1						0x1			
#define ATTACKATB_ATB_TYPE2						0x2			
#define ATTACKATB_ATB_TYPE3						0x4			
#define ATTACKATB_ATB_PHYSICS					0x8
#define ATTACKATB_ATB_MAGIC						0x10
#define ATTACKATB_ATB_LOWASSISTDPOWER			0x20		// �����϶� ������ ����� ���� �� �ִ�. 
#define ATTACKATB_ATB_AIR						0x40
#define ATTACKATB_ATB_MINRANGE					0x80
#define ATTACKATB_ATB_PULL						0x100		// �¾��� �� �ڷ� �и�. 
#define ATTACKATB_ATB_PALSY						0x200		// �¾��� �� ���� ȿ��. (�а��� ���)
#define ATTACKATB_ATB_VAMPIRE					0x400		// ü�� ���. 

// robypark 2004/6/10 17:25
#define ATTACKATB_ATB_POISON					0x800		// �¾��� �� �ߵ�ȿ��

#define MAX_HIT_LIST_NUMBER    400


// private:
// ���ݼӼ�, ���ݷ�, ���ݴ���� ����Ͽ� ���� ���ظ� ����Ѵ�. 
SHORT CalculateHitPoint(SHORT attackatb, SHORT power, _NewID attackid);


// public:

// ������ ���� �Ӽ��� ����Ͽ� ���� ü���� ���Ͻ�Ų��. 
// �Ķ��Ÿ ����:
// myid      : ������ �õ��ϴ� ĳ������ ���̵� 
// myplayernumber: ������ �õ��ϴ� ĳ������ �÷��̾� ��ȣ 
// attackatb : ���ݼӼ�(main.h�� ���� )
// range     : ���� ����, �Ϲ������� 1( 1�̸� -1 ~ +1)
// area      : ���� ��� ����, �ϴ��̳� ���̳�  (AREA_LAND, AREA_AIR)
// power     : ���ݷ� 
// attackid  : Ư���� ���߰� ���� ĳ������ ���̵� 
// dx, dy    : Ÿ���� �� �������� ��ǥ 

// ���ϰ�   
//  0 -> �׻� 0�� ���� 
SHORT CalculateHit(_NewID myid, SHORT myplayernumber, SHORT attackatb, SHORT range, SHORT area, SHORT power, _NewID attackid, SHORT dx, SHORT dy, SI32 hitrate);


// �������� ���ݰ� ���ݿ� ���� ��ġ�� ���Ѵ�. 
BOOL HitNOperateAfterHit(_NewID myid, _NewID enemyid, SHORT myplayernumber, SHORT attackatb, SHORT power);


void InitAttackAtb();

// �� ���� �Ӽ��� ���� �������� ���Ѵ�. 
SI32 CalcDamage(SI32 attackatb, SI32 para1=0, SI32 para2=0, SI32 para3=0, SI32 para4=0);

// ���ݼӼ��� ĳ���� �Ӽ��� ���� ���ݷ� ����. 
// �������� �����ݷ�, �������� ���ݼӼ�, ������� �Ӽ��� �ٰŷ� �������� �����ݷ��� �����Ѵ�. 
SI32 CalcApowerByAtb(SI32 apower, SI32 attackatb, SI32 atb);

#endif
