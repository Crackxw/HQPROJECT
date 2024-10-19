#ifndef _CHARANIMATION_H
#define _CHARANIMATION_H

#define ANI_ATB_2DIR							1		// 2방향 애니메이션. (실제로는 1개의 그림만이 존재하는 것임.)
#define ANI_ATB_16DIR							2		// 16방향 애니메이션. (실제로는 9개의 그림만이 존재하는 것임.)
#define ANI_ATB_SINGLEFRAME						4		// 한프레임 애니메이션.(여러 프레임으로 되어 있지만 한 프레임만으로만 표현하고자 함. )
#define ANI_ATB_REVERSE							8		// 정렬된 순서를 역으로 애니메이션 시킨다. (건물의 해체등..)
#define ANI_ATB_ONCE							16		// 한번만 애니메이션 되는 속성을 갖고 있다. 

//----------------------------------------------------
//  애니메이션의 종류 정의 
//----------------------------------------------------
#define ANI_WAIT0								1		// 정지 애니메이션 
#define ANI_WAIT1								2		// 정지 애니메이션
#define ANI_WAITUPGRADE							5
#define ANI_MOVE								6		// 이동 애니메이션 
#define ANI_MOVE2								7		// 이동 애니메이션 
#define ANI_LOWMOVE								8		// 기어서 이동 애니메이션 
#define ANI_STRONGFIRE							11
#define ANI_ATTACK								12		// 캐릭터 공격1 애니메이션
#define ANI_ATTACK2								13		// 캐릭터 공격2 애니메이션
#define ANI_LOWATTACK							14		// 포복 상태 공격 애니메이션
#define ANI_EXP									15
#define ANI_DYING								16		// 캐릭터 사망 애니메이션
#define ANI_ABSWAIT								17
#define ANI_HARVESTCROP							18
#define ANI_HARVESTTREE							19
#define ANI_BUILD								20
#define ANI_SELL								21
#define ANI_HEAL								22		// 치료마법 사용 애니메이션
#define ANI_READYTOFIRE							23
#define ANI_RESTBEFORETREEHARVEST				24
#define ANI_MAGIC								25		// 마법 사용 애니메이션
#define ANI_DEAD								26
#define ANI_MAKEPORTAL							27
#define ANI_COVER								28		// 위장 가능 
#define ANI_CARTOBUILD							30		//  건설 수레 -> 건물
#define ANI_BUILDTOCAR							31		//  건물건설  -> 수레 
#define ANI_SWIM								32		//  수영 
#define ANI_SWIM_WAIT							33		  	 	  //  수영 중 정지
#define ANI_AMBUSH								34		  	 	  //  매복 
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
#define ANI_MANA								53		// 마나 회복술 시전 애니메이션

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

	// 특정 동작(공격) 시작되는 폰드번호로 
	SI32 GetAnimationInstant(void)
	{
		return siInstant;
	}
};

#endif

