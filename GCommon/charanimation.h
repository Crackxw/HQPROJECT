#ifndef _CHARANIMATION_H
#define _CHARANIMATION_H

#define ANI_ATB_2DIR							1		// 2���� �ִϸ��̼�. (�����δ� 1���� �׸����� �����ϴ� ����.)
#define ANI_ATB_16DIR							2		// 16���� �ִϸ��̼�. (�����δ� 9���� �׸����� �����ϴ� ����.)
#define ANI_ATB_SINGLEFRAME						4		// �������� �ִϸ��̼�.(���� ���������� �Ǿ� ������ �� �����Ӹ����θ� ǥ���ϰ��� ��. )
#define ANI_ATB_REVERSE							8		// ���ĵ� ������ ������ �ִϸ��̼� ��Ų��. (�ǹ��� ��ü��..)
#define ANI_ATB_ONCE							16		// �ѹ��� �ִϸ��̼� �Ǵ� �Ӽ��� ���� �ִ�. 

//----------------------------------------------------
//  �ִϸ��̼��� ���� ���� 
//----------------------------------------------------
#define ANI_WAIT0								1		// ���� �ִϸ��̼� 
#define ANI_WAIT1								2		// ���� �ִϸ��̼�
#define ANI_WAITUPGRADE							5
#define ANI_MOVE								6		// �̵� �ִϸ��̼� 
#define ANI_MOVE2								7		// �̵� �ִϸ��̼� 
#define ANI_LOWMOVE								8		// �� �̵� �ִϸ��̼� 
#define ANI_STRONGFIRE							11
#define ANI_ATTACK								12		// ĳ���� ����1 �ִϸ��̼�
#define ANI_ATTACK2								13		// ĳ���� ����2 �ִϸ��̼�
#define ANI_LOWATTACK							14		// ���� ���� ���� �ִϸ��̼�
#define ANI_EXP									15
#define ANI_DYING								16		// ĳ���� ��� �ִϸ��̼�
#define ANI_ABSWAIT								17
#define ANI_HARVESTCROP							18
#define ANI_HARVESTTREE							19
#define ANI_BUILD								20
#define ANI_SELL								21
#define ANI_HEAL								22		// ġ�Ḷ�� ��� �ִϸ��̼�
#define ANI_READYTOFIRE							23
#define ANI_RESTBEFORETREEHARVEST				24
#define ANI_MAGIC								25		// ���� ��� �ִϸ��̼�
#define ANI_DEAD								26
#define ANI_MAKEPORTAL							27
#define ANI_COVER								28		// ���� ���� 
#define ANI_CARTOBUILD							30		//  �Ǽ� ���� -> �ǹ�
#define ANI_BUILDTOCAR							31		//  �ǹ��Ǽ�  -> ���� 
#define ANI_SWIM								32		//  ���� 
#define ANI_SWIM_WAIT							33		  	 	  //  ���� �� ����
#define ANI_AMBUSH								34		  	 	  //  �ź� 
#define ANI_HALFDESTROYEDUPGRADE				35
#define ANI_HALFDESTROYED						36
#define ANI_ATTACKTOWER							37
#define ANI_TOWER								38
#define ANI_SATJETC								39
#define ANI_MAGICETC							40
#define ANI_FIREHOUSEKETC						41
#define ANI_ETC									42
#define ANI_SHIPYARDKETC						43
#define ANI_TOWERCETC							44
#define ANI_ATTACKETC							45
#define ANI_DYINGETC							46
#define ANI_GENERAL4K_MAGICETC					47
#define ANI_GENERAL4J_MAGICETC					48
#define ANI_INVALID								49
#define ANI_INGROUND							50
#define ANI_OUTGROUND							51
#define ANI_MAGIC2								52
#define ANI_MANA								53		// ���� ȸ���� ���� �ִϸ��̼�

class cltCharAnimation {
public:
	BOOL  bDirSwitch;
private:
    SI32  siFrameNumber;

public:
	SI32  siFile;
	SI32  siFont;
	SI32  siInstant;
	SI32  siDelay;
	SI32 siAtb;

	void Set(BOOL dirswitch, SI32 framenumber, char* file, SI32 font, SI32 instant, SI32 delay, SI32 atb = 0);
	void DecideFontAni(SI32 dir, SI32 step, int* return_file, int* return_font, BOOL * return_reverseswitch);
	BOOL IsReady(SI32 step);

	SI32 GetFrameNumber();

	// Ư�� ����(����) ���۵Ǵ� �����ȣ�� 
	SI32 GetAnimationInstant(void)
	{
		return siInstant;
	}
};

#endif

