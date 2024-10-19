#ifndef		_SOPLANT_H_
#define		_SOPLANT_H_

#include	"OnlineProductStructure.h"
//#include	"Stack.h"
//#include	"OnlineCommonStruct.h"

struct	stFarmGroup;
struct	stFarmData;

struct	stFarmEquipment
{
	UI08			uiLevel;
	stEquipment		*pEquipment;
};


struct	stFarmStructure
{
	UI32			uiNum;				//	현재 생산한 개수
	UI32			uiFinalNum;			//	최종 목표 생산수.(0이면 아이템 생산중이지 않음)
	SI08			siRate;				//	생산 비율 (-30% ~ 30%)
	stFarmEquipment	Equipment[UPGRADE_CODE_NUM];

	VOID	Init()
	{
		uiNum		=	0;
		uiFinalNum	=	0;
		siRate		=	0;
		for(int i=0; i<UPGRADE_CODE_NUM; i++)
		{
			Equipment[i].uiLevel	=	0;
		}
	}

	SI32	GetRemainNum()
	{
		return (uiFinalNum - uiNum);
	}
};


struct stStorage						//	창고	-	★ 크기 44바이트
{
	STORAGE_ITEM		item[ON_MAX_STORAGEITEM];

	VOID	Init()
	{
		ZeroMemory(item, sizeof(STORAGE_ITEM)*ON_MAX_STORAGEITEM);
	}

	UI08	Search(UI16	ItemCode)
	{
		for(int i=0; i<ON_MAX_STORAGEITEM; i++)
		{
			if(item[i].uiItemCode==ItemCode)
			{
				if(item[i].uiQuantity > 0 )			return i;
			}
		}
		return UI08_ERROR;
	}

	UI08	GetEmptySlot()
	{		
		for(int i=0; i<ON_MAX_STORAGEITEM; i++)
		{
			if(item[i].uiQuantity == 0 )	return i;
		}
		return UI08_ERROR;
	}

	UI08	GetStorageItemIndex(UI16 ItemCode)
	{
		UI08	uiReturn;
		uiReturn	=	Search(ItemCode);
		if( uiReturn != UI08_ERROR )	return uiReturn;
		return GetEmptySlot();
	}

	UI08	CheckItemExist(UI16 uiItemCode, UI16 uiITemQnt=1)
	{
		for(int i=0; i<ON_MAX_STORAGEITEM; i++)
		{
			if(item[i].uiItemCode == uiItemCode)
			{
				if(item[i].uiQuantity >= uiITemQnt)
					return i;
				else continue;
			}
		}
		return UI08_ERROR;
	}
};


struct	stFarmSlot
{
	SI32				nSlotIndex;
	BOOL				bWorkable;					//	일 할 수 있는가?
	BOOL				bStore;						//	저장할 수 있는가?
	UI08				uiLevel;					//  0~3 까지 레벨이 있다.
	SI16				siPay;						//  수당.
	UI16				ProductItem;				//	생산중인 아이템 코드
	UI32				uiRemainLabor;
	UI32				uiLabor;					//	현재 노동량.
	stFacility			*pFacility;					//  NULL 이면 건물이 지어지지 않음
	stFarmData			*pFarm;
	union
	{
		stFarmStructure		Structure;				//	보통건물일 경우
		stStorage			Storage;				//	창고일 경우
	};

	// 함수
	VOID	Init()
	{
		bWorkable		=	FALSE;
		uiLevel			=	0;
		siPay			=	0;
		uiLabor			=	0;
		uiRemainLabor	=	0;
		ProductItem		=	0;
		pFacility		=	NULL;
		Structure.Init();
		Storage.Init();
	}

	VOID	EndProduct()
	{
		if( uiLevel == 0 )
		{
			Init();
		}
		else
		{
			bWorkable		=	FALSE;
			siPay			=	0;
			uiLabor			=	0;
			uiRemainLabor	=	0;
			ProductItem		=	0;
			if( bStore == FALSE )
			{
				Structure.uiFinalNum=	0;
				Structure.uiNum		=	0;
			}
		}
	}

	VOID	LevelUp()
	{
		uiLevel++;
		bWorkable		=	FALSE;
		siPay			=	0;
		uiLabor			=	0;
		uiRemainLabor	=	0;
		ProductItem		=	0;
	}

	BOOL	bOnSchedule()
	{
		if( siPay > 0 )
		{
			if( uiRemainLabor > 0 )		return TRUE;
			else	siPay = 0;
		}
		return FALSE;
	}

	BOOL	bStorage()
	{
		if(pFacility)
		{
			if(bStore)	return TRUE;
		}
		return FALSE;
	}
/*
	stFarmEquipment*	GetEquip(UI32 EquipID)
	{
		if( bStore == FALSE )
		{
			if( pFacility )
			{
				if( uiEquipID	< UPGRADE_CODE_NUM)
				{
					pFacility->UpCode[uiEquipID]
				}
			}
		}
		return NULL;
	}*/
};

/*
struct	stFarmData					// 6개의 슬롯으로 된 농장, 한마을에 여러개 있다. (일단 10개로 배열)
{
	SI32			nIndex;							//	몇번째 농장인가?
	SI32			siOwnerDBAcnt;					//	농장주인 : 0 이면 아직 안 팔린 농장
	UI08			uiOwnerSlot;
	SStack<UI16>	UserManager;					//  일꾼들을 관리하는 객체
	SI32			siWorkDay;						//	최근 일한 날로부터 몇 일 지났는가? , 일정시간동안 가동안한 농장 처분위함
	OnFarmList		FarmInfo;						//  전송을 위한 구조체.(uiAvailNum,szFarmName,szOwner,Labor)
	stFarmGroup		*pGroup;
	stPlants		*pPlant;
	stFarmSlot		FarmSlot[ON_FARM_SLOT_NUM];		// 슬롯 6개


	stFarmData()
	{
		UserManager.Create(ON_MAX_PLANT_PLAYER_NUM);
	}

	VOID Init()
	{
		siOwnerDBAcnt		=	0;
		uiOwnerSlot			=	0;
		siWorkDay			=	0;
		FarmInfo.Init();
		FarmInfo.uiAvailSlot=	pPlant->uiInitSlot;
		UserManager.DeleteAll();
		for(int i=0; i<ON_FARM_SLOT_NUM; i++)
		{
			FarmSlot[i].Init();
			FarmSlot[i].nSlotIndex	=	i;
			FarmSlot[i].pFarm		=	this;
		}
	}

	~stFarmData()
	{
		UserManager.Release();
	}

	stFarmSlot	*GetFactory( SI32 siFactoryIndex )
	{
		if( siFactoryIndex < FarmInfo.uiAvailSlot && siFactoryIndex >= 0)
			return &FarmSlot[ siFactoryIndex ];
		
		return NULL;
	}

};

*/

/*
struct stFarmGroup
{
	stFarmData		Farm[ON_VILLAGE_FARM_NUM];		// 한 마을에 몇개의 건물이 있는가?
	stPlants		*pPlant;
	SOVillage		*pVillage;

	stFarmGroup(SOVillage *pV, stPlants *pP)
	{
		pPlant		=	pP;
		pVillage	=	pV;
		Init();
	}

	~stFarmGroup()
	{
	}

	VOID	Init()
	{
		for(int i=0; i<ON_VILLAGE_FARM_NUM; i++)
		{
			Farm[i].nIndex	=	i;
			Farm[i].pGroup	=	this;
			Farm[i].pPlant	=	pPlant;
			Farm[i].Init();
		}
	}

	stFarmData *GetFarm(SI32 Index)
	{
		if( Index < ON_VILLAGE_FARM_NUM && Index >= 0)
			return &Farm[Index];
		return NULL;
	}
};
*/

#endif	// _SOPLANT_H_