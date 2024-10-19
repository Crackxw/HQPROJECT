

#ifndef	__ONLINEPRODUCTSTRUCTURE_H_
#define	__ONLINEPRODUCTSTRUCTURE_H_

#include "GSCDefine.h"

struct	stEquipment
{
	union
	{
		CHAR			Code[4];						//	건물코드 (FNC:울타리)
		UI32			uiNumCode;						//	코드를 숫자로 한것.
	};
	SI32			siTextCode;						//  텍스트
	UI32			uiCost;							//	가격
	UI08			Repeat;							//	업그레이드 반복 수
	UI08			uiResist[RESIST_KIND_NUM];		//	재해에 대한 보호정도
};

struct stFacility
{
	union
	{
		CHAR			Code[4];						//	코드 (STG:창고, FRU:과수원)
		UI32			uiNumCode;						//	코드를 숫자로 변환한 값
	};
	CHAR			Name[8];
	SI32			siTextCode;						//  텍스트
	SI16			siSpriteFont;					//  스프라이트 폰트
	// 생산하는 제품 아이템 번호
	UI16			uiPr[4][PRODUCT_CODE_NUM];			//	생산할 수 있는 아이템 코드 (00011)
	//	업그레이드 하는 도구,시설물 의 코드
	SI16			siUpCodeNum;					//  설치할 수 있는 시설 갯수
	union
	{
		CHAR			UpCode[UPGRADE_CODE_NUM][4];	//	설치할 수 있는 시설 코드 (FNC:울타리)
		UI32			UpNumCode[UPGRADE_CODE_NUM];	//	숫자로.
	};
	UI32			UpLabor[MAX_UPGRADESLOT_NUM];
};

struct stPlants
{
	SI32				DBIndex;
	union
	{
		CHAR			Code[4];						//	건물코드 (FAM:농장)
		UI32			uiNumCode;						//	코드를 숫자로 변환한 값
	};
	UI32			uiCost;							//	가격
	SI32			siTextCode;						//	Text
	UI08			uiMaxSlot;						//	최대슬롯수 (6)
	UI08			uiInitSlot;						//	초기슬롯수 (4)
	SI16			siBuildNum;						//  지을수 있는 건물 수
	union
	{
		CHAR			FCode[FACILITY_CODE_NUM][4];	//	지을수 있는 건물 (STG:창고)(FRU:과수원)
		UI32			FNumCode[FACILITY_CODE_NUM];	//	위값을 숫자로 변환.
	};
	UI32			Up2Labor;
	UI32			Up3Labor;
	SI32			siReqCredit;					//	필요 신용도
};

 
class	OnlineBuilding
{
private:
	stEquipment			*pEquipment;
	stFacility			*pFacility;
	stPlants			*pPlants;
	SI32				EquipNum;		//	개수
	SI32				FacilNum;
	SI32				PlantsNum;

public:
	OnlineBuilding();
	~OnlineBuilding();

	stPlants		*GetPlantsFromID( UI08 uiPlantID );	//0:농장, ...

	stEquipment		*GetEquipment( SI32 siIndex );
	stFacility		*GetFacility( SI32 siIndex );
	stPlants		*GetPlants( SI32 siIndex );

	stEquipment		*GetEquipment( CHAR	*pCode );
	stFacility		*GetFacility( CHAR *pCode);
	stPlants		*GetPlants( CHAR *pCode);

	SI32			GetIndex(CHAR *FaCode, UI16	uiItem, UI08 Level=3, UI08 uiNation = ON_COUNTRY_KOREA);
	UI32			CodeToNum(CHAR	*pCode);
	BOOL			Init();

	SI32			GetEquipNum()			{	return EquipNum;	};
	SI32			GetFacilNum()			{	return FacilNum;	};
	SI32			GetPlantsNum()			{	return PlantsNum;	};
};

#endif

