#include <main.h>

#ifdef _IMJIN2
	#include "..\Gersang\reservetrain.h"
	#include "..\Gersang\rain.h"
	#include "..\Gersang\maindata.h"
	#include <etc.h>
	#include "..\Gersang\crop.h"
	#include "..\Gersang\music.h"
	#include "..\Gersang\char.h"
#else 
	#include "..\õ���� ��ȭ\char.h"
#endif

#include <map.h>
#include <CharInterface.h>
#include <findpath5.h>



extern _MainData GD;
extern _Map Map;
extern cltCharInterface clifChar;         // ĳ���� ���� �������̽� 
extern _KindInfo KI[];
extern _NewID ZeroID;
extern _Char CR[];

void cltCharOrg::InitHarvest()
{
	HarvestX=HarvestY=-1;

	BaseID.Init();
	ResType=0;
	ResAmount=0;
	RepeatNumber=0;
}

// �־��� ������ �Ÿ��� ���� ����� ���� ã�´�. 
_NewID cltCharOrg::FindShortestBase(SHORT x, SHORT y, SHORT p_number)
{

	SHORT i;
	SHORT millid=0, hqid=0;
    SHORT mill_short_length=1000;
	SHORT hq_short_length=1000;
	SHORT length;
	SHORT id;

	SHORT cnumber	=	GD.Player[PlayerNumber].CharNumber;
	SHORT sector	=	Map.GetSector(x, y);

	for(i=0;i<cnumber;i++)
	{
		id=GD.Player[PlayerNumber].CharID[i].ID;

		// �۾� �����ϰ� ���� ���Ϳ� �־�� �Ѵ�. 
	   if(clifChar.CanWork(id) && sector==Map.GetSector( clifChar.GetX(id) ,clifChar.GetY(id) ) )
	   {
			length=max(abs(clifChar.GetX(id) - x),abs(clifChar.GetY(id) - y));

			// ���� ���� ����� ���̸�, 
 			if(clifChar.IsAtb(id, ATB_MARKET)==TRUE )
			{
				// ��Ѱ��̸�, 
				if(KI[clifChar.GetKind(id)].IsKindAtb(KINDATB_MILL)==TRUE)
				{
				   if(length<mill_short_length)
				   {
			          mill_short_length=length;
			          millid=id;
				   }
				}
				// �����̸�, 
				else
				{
				   if(length<hq_short_length)
				   {
			          hq_short_length=length;
			          hqid=id;
				   }
				}
			}
	   }
	}	

	// ��Ѱ��� �ִٸ�, 
	if(millid)
	{
		// ��Ѱ��� �ָ��ְ�, ������ ������ �ִٸ�, 
		if(mill_short_length>15 && hq_short_length<15)
		{
		}
		else
		   return clifChar.GetID(millid);
	}

	// ������ �ִٸ�, 
	if(hqid)return clifChar.GetID(hqid);
	else return ZeroID;

}

BOOL cltCharOrg::SetHarvest(SHORT dx, SHORT dy, _NewID baseid)
{

	SHORT type, vary;
    
	// ���� ���õ� ���� �츮 �����̳� �����̸� �츮 ������ �� �������� �����Ѵ�. 
	if(IsSee(baseid.ID) 
	&& (clifChar.IsAtb(baseid.ID,ATB_MARKET))
	&&  clifChar.CanWork(baseid.ID)
	&&  clifChar.GetPlayerNumber(baseid.ID)== PlayerNumber)
	{
	   // ��ΰ� �ڽ��� ������ ��� �ǹ��� �����Ѵ�. 
	   BaseID=baseid;
	   BaseSelectedByPlayerSwitch=TRUE;

	   // ���� ���� ���Ŀ��� �����Ѵ�.
//	   HarvestX=HarvestY=-1;

       // ���� ������ ��Ѱ����� ���� ������ �ƹ��͵� ���� �ʴ´�. 
       if(ResAmount)
	   {
           WorkStep = WS_GOTO_BASE;
		   SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
		   return TRUE;
	   }
	   else return FALSE;
   }

	// ��� ������ Ȯ���Ѵ�. 
	type=Map.GetCropTypeMap(dx, dy);
	vary=Map.GetCropVaryMap(dx, dy);


	// Ǯ �̿��� ��� ��Ȯ�Ѵ�. 
	if(IsCropHarvest(NewID.ID, type)==TRUE)
	{
		TargetResType=type;

		HarvestX=dx;
		HarvestY=dy;

		// ���ο� ������ ã�´�. 
		BaseSelectedByPlayerSwitch=FALSE;
		BaseID=FindShortestBase(HarvestX, HarvestY, PlayerNumber);

		SetMove(HarvestX, HarvestY);

		WorkStep = WS_GOTO_TARGET;
				
		AttackGiveUpDelayor = 0;
		//CR[ID].AttackDelayor=0;

		return TRUE;
	}

	return FALSE;
}


BOOL cltCharOrg::Harvest()
{
	int status;
	SHORT dx, dy;
	int kind=0;
	int factor=0;
	SHORT type;
	SHORT i, j;
	SHORT tx, ty;
	SHORT multifactor;
	BOOL bTempRestSwitch;
	SI32 tempStep = 0;
	
	switch(WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:
		// �������� ��ȿ�� ������ ���� �̵��Ѵ�.
		if(Map.IsInMapArea(HarvestX, HarvestY)==TRUE)
		{
			SetMove(HarvestX, HarvestY);
			WorkStep = WS_GOTO_TARGET;
		}
		else
		{
			OrderStop(NewID, BY_COMPUTER);
		}
		break;
		
	case WS_GOTO_TARGET:
			
		// ���� ������� ���� �ִ� ���ɷ����� ���ų� ������ 
		// ������ ���ư���.
		if(ResAmount>=KI[Kind].MaxResAmount)
		{
			WorkStep = WS_SET_GOTO_BASE;
			break;
		}
		// ������ ��Ȯ�ϴ� �߿��� ���� ���ڸ��� ����. 
		if(TargetResType==CROP_TYPE_TREE
		|| TargetResType==CROP_TYPE_BAMBOO)
		{
			status=Move(BASIC_MOVE_BLOCK_COUNT,1);
		}
		else
		{
			status=Move(BASIC_MOVE_BLOCK_COUNT);
		}

		if(MovePercent == 0)	
			BusySwitch=FALSE;
		else		
			BusySwitch=TRUE;
		
		// ��Ȯ�ϰ��� �ϴ� �������� ���� 
		if(status==1)
		{ 
			
			// ���̳� ���ڵ��� �ڱ� ��ġ���� ã�´�. 
			type=0;
			if((TargetResType==CROP_TYPE_RICE		 || TargetResType==CROP_TYPE_POTATO)
				&& IsAtb2(ATB2_HARVESTCROP)==TRUE)
			{
				// ���� ��� �ִٸ�, 
				if(Map.GetCropMountMap(X, Y))
				{
					type=Map.GetCropTypeMap(X, Y);
					if(type!=TargetResType)type=0;
				}
				
			}
			// ������ �ֺ����� ã�´�. 
			else if(TargetResType==CROP_TYPE_TREE
				||  TargetResType==CROP_TYPE_BAMBOO)
			{
				// �ֺ��� ��� ��� �ִ��� Ȯ���Ѵ�. 
				SHORT min_number=10;
				int cropnumber=0;
				
				for(i=-1;i<=1;i++)
					for(j=-1;j<=1;j++)
					{
						
						tx=X+j;
						ty=Y+i;
						if(Map.IsInMapArea(tx, ty)==TRUE)
						{
							// �ڿ��� ������ ������, 
							// ���� ��� ���� �־�� 
							// ���� �ٸ� ��ü���־�� �ȵȴ�. 
							if(Map.GetCropTypeMap(tx, ty)==TargetResType 
								&& Map.GetCropMountMap(tx, ty)
								&& Map.GetIDMap(tx, ty)==0)
							{
								// �α��� ����� �ľ��Ѵ�. 
								cropnumber=GetNearCropCount(tx, ty);
								
								
								// �ֺ��� ��� ���� ���� ���� ĵ��. 
								if(cropnumber<min_number)
								{
									min_number=cropnumber;
									
									type=TargetResType;
									HarvestX=tx;
									HarvestY=ty;
									
									// ������ �����Ѵ�. 
									Direction =  DecideDirection8(X, Y, HarvestX, HarvestY);
									
								}
							}
						}
					}
					
			}
			
			// ��� ��ġ�� �־�� �Ѵ�. 
			if(type)
			{
				ResType=type;
				AttackGiveUpDelayor=0;
				WorkStep=WS_HARVEST_CROP;
				Step=0;
				RepeatNumber=0;

				AttackGiveUpDelayor = 0;
			}
			else
			{
				//��� �̵��� �õ��ߴٰ� �����ϸ� �����. 
				AttackGiveUpDelayor++;
				
				if( AttackGiveUpDelayor > AttackGiveUpDelay )
				{
					OrderStop(NewID, BY_COMPUTER);
					break;
				}

				// ������ �������� ��� ã�´�. 
				if(FindNearCrop(HarvestX, HarvestY, dx, dy, TargetResType)==TRUE)
				{
					HarvestX=dx;
					HarvestY=dy;
					SetMove(dx, dy);
				}
				// ��� ã�� ���ϸ�,
				else
				{
					if(ResAmount>0)
					{
						WorkStep=WS_SET_GOTO_BASE;
						break;
					}
					else
					{
						OrderStop(NewID, BY_COMPUTER);
						break;
					}
				}
			}
		}
		
		
		break;
	 case WS_HARVEST_CROP:

		 bTempRestSwitch = FALSE;
#ifdef _IMJIN2
		 // ������ ���� ���̰� , 
		 // �޽��� ���ϴ� �ִϸ��̼��� �ְ�, 
		 // ���� �۾��� �������� �ʾ�����, 
		 if(( (ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
			 && KI[Kind].clCharAnimation[RestCharAnimationIndex].GetFrameNumber()
			 && Step==0 
			 && abs(RestFrame-GD.Frame)>300
			 && (Direction==SOUTH || Direction==WEST 
			 || Direction==NORTH || Direction==EAST)
			 && GD.Rand%10==0)
			 || (RestSwitch==TRUE) )
		 {
			 RestFrame=GD.Frame;
			 
			 // �޽��� ���ϴ� �ִϸ��̼��� �����ش�.
			 RestSwitch=TRUE;
			 
			 
			 WorkDelayor++;
			 if( WorkDelayor < KI[Kind].clCharAnimation[RestCharAnimationIndex].siDelay )
			 {
				 return TRUE;
			 }

			 WorkDelayor=0;
			 
			 Animation=ANI_RESTBEFORETREEHARVEST;		
			 
			 // ��Ȯ�ϴ� �������� �������̴�. 
			 tempStep = Step;
			 (++tempStep) %= KI[Kind].clCharAnimation[RestCharAnimationIndex].GetFrameNumber();
			 Step=tempStep;
		 
			 AniStep=Step;

			 UpdateAnimationSwitch=TRUE;
			 
			 // ���� ��ġ ����(�ٸ� ����� �޾Ƶ����� ����)
			 BusySwitch=TRUE;
			 
			 // ��Ȯ�� �� ����Ŭ�� ������.
			 if( Step == (KI[Kind].clCharAnimation[RestCharAnimationIndex].GetFrameNumber() - 1) )
			 {
				 RestSwitch=FALSE;
			 }			 
			// ������2�� ��쿡�� �޽��� ���ϴ� �ִϸ��̼��� �ֱ� ������.
			 bTempRestSwitch = TRUE;
		 }
#endif

		 if(bTempRestSwitch == FALSE && Work()==TRUE)
		 {
			 
			 // ���������� ��������� �������� ����Ѵ�.
			 LastHarvestMotionFrame=GD.Frame;
			 
			 
			 // ������ �ִ� ���� �ִ� ���۷� ���� ���ų� ������, 
			 if(ResAmount>=KI[Kind].MaxResAmount)
			 {
				 WorkStep=WS_SET_GOTO_BASE;
				 
			 }
			 else
			 {
				 
				 // ��Ȯ�ϴ� �ڸ��� ��� ���� �ִ��� Ȯ���Ѵ�. 	
				 if(IsCropHarvest(NewID.ID,  Map.GetCropTypeMap(HarvestX, HarvestY)  )==TRUE && Map.GetCropMountMap(HarvestX, HarvestY))
				 {
				 }
				 // ����� ������ �ִ� ��� ã�´�. 
				 else if(FindNearCrop(HarvestX, HarvestY, dx, dy, ResType))
				 {
					 HarvestX=dx;
					 HarvestY=dy;
					 SetMove(dx, dy);
					 WorkStep=WS_GOTO_TARGET;
				 }
				 else
				 {
					 if(ResAmount>0)
					 {
						 WorkStep=WS_SET_GOTO_BASE;
						 break;
					 }
					 else
					 {
						 OrderStop(NewID, BY_COMPUTER);
						 break;
					 }
				 }
			 }
		 }
		 break;
		 
	 case WS_SET_GOTO_BASE:
		 
		 // ��Ȯ�ϴ� ���� ����Ѵ�. 
		 //HarvestX=CR[ID.ID].GetX();
		 //HarvestY=CR[ID.ID].GetY();
		 
		 // ���� Base�� ������ ���̶�� �װ����� ����. 
		 if(BaseSelectedByPlayerSwitch==TRUE 
			 && IsSee(BaseID.ID) && clifChar.IsAtb(BaseID.ID, ATB_MARKET)
			 && clifChar.CanWork(BaseID.ID) && clifChar.GetPlayerNumber(BaseID.ID)==PlayerNumber)
		 {
			 SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
			 WorkStep=WS_GOTO_BASE;
		 }
		 // ������ �ִ� Base�� �������ϸ� ���� ã�´�. 
		 else
		 {
			 
			 // ������ ���ο� ������ ã�´�. 
			 BaseSelectedByPlayerSwitch=FALSE;
			 
			 BaseID=FindShortestBase(HarvestX, HarvestY, PlayerNumber);
			 
			 if(BaseID.IsEmpty()==FALSE)
			 {
				 SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
				 WorkStep=WS_GOTO_BASE;
			 }
			 else
			 {
				 OrderStop(NewID, BY_COMPUTER);
				 break;
			 }
		 }
		 break;
		 
	 case WS_GOTO_BASE:
		 status=Move(BASIC_MOVE_BLOCK_COUNT);
		 if( GetMovePercent() == 0 )	
			 BusySwitch= FALSE;
		 else BusySwitch=TRUE;
		 
		 // �̵��� �Ϸ�ǰų� 
		 if(status==MOVEINFO_MOVEDONE)
		 {
			 WorkStep = WS_DECIDE_LENGTH_TO_BASE;
			 
			 //��� �̵��� �õ��ߴٰ� �����ϸ� �����. 
			 AttackGiveUpDelayor++;
			 
			 if( AttackGiveUpDelayor > AttackGiveUpDelay )
			 {
				 OrderStop(NewID, BY_COMPUTER);
				 break;
			 }
		 }
		 // �߰� ������ ������ ���̶��, 
		 else if(status==MOVEINFO_HALFDONE)
		 {
			 WorkStep=WS_DECIDE_LENGTH_TO_BASE;
		 }
		 // ���� �̵��� �־��ٸ�, 
		 else if(status==MOVEINFO_REALMOVE)
		 {
			 AttackGiveUpDelayor= 0;
		 }
		 
		 break;
		 
	 case WS_DECIDE_LENGTH_TO_BASE:
		 
		 if( IsSee(BaseID.ID) && clifChar.IsAtb(BaseID.ID, ATB_MARKET)
			 && clifChar.GetPlayerNumber(BaseID.ID)==PlayerNumber && clifChar.CanWork(BaseID.ID))
		 {
			 
			 SHORT nearswitch=FALSE;
			 SHORT id;
			 for(i=-1;i<=1;i++)
				 for(j=-1;j<=1;j++)
				 {
					 id=Map.GetIDMap(X+j, Y+i);
					 if(IsSee(id) && id==BaseID.ID)
					 {
						 nearswitch=TRUE;
					 }
				 }
				 
				 
				 // �����̿� ������ �ִ�. 
				 if(nearswitch==TRUE)
				 {
					 WorkStep=WS_PUT_RESOURCE;
				 }
				 else
				 {
					 WorkStep=WS_GOTO_BASE;
					 SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
				 }
				 
		 }
		 else
		 {
			 BaseSelectedByPlayerSwitch=FALSE;
			 
			 BaseID=FindShortestBase(HarvestX, HarvestY, PlayerNumber);
			 if(BaseID.ID)
			 {
				 SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
				 WorkStep=WS_GOTO_BASE;
			 }
			 else
			 {
				 OrderStop(NewID, BY_COMPUTER);
				 break;
			 }
		 }
		 break;
		 
	 case WS_PUT_RESOURCE:
		 
		 if(IsSee(BaseID.ID) && clifChar.IsAtb(BaseID.ID, ATB_MARKET)
			 && clifChar.GetPlayerNumber(BaseID.ID)==PlayerNumber && clifChar.CanWork(BaseID.ID))
		 {
			 
			 if(ResAmount>0)
			 {
				 int PutMoneyUnit;
				 
				 if(ResAmount>=20)PutMoneyUnit=20;
				 else if(ResAmount>=15)PutMoneyUnit=15;
				 else if(ResAmount>=5)PutMoneyUnit=5;
				 else PutMoneyUnit=1;
				 
				 SHORT comadventage;
				 
				 // ��ǻ���� ��쿡 ��庥Ƽ���� �ش�. 
				 if(GD.Player[PlayerNumber].PI.Host==BY_COMPUTER)
				 {
					 comadventage=COMADVENTAGE_MOUNT;
				 }
				 else
				 {
					 comadventage=1;
				 }
#ifdef _IMJIN2
				 // ���� ���� ����� ���׷��̵� �Ǿ� �ִٸ�, 
				 if(GD.Player[PlayerNumber].UpgradeSwitch[UPGRADE_MARKET]==TRUE)
				 {
					 if(clifChar.IsAtb(BaseID.ID, ATB_ADVMARKET))
					 {
						 multifactor=AMOUNT_PER_UNIT_HARVEST_MILL;
					 }
					 else
					 {
						 multifactor=AMOUNT_PER_UNIT_HARVEST_HQ;
					 }
				 }
#endif
#ifdef _MYTH
				if(CR[BaseID.ID].IsAtb(ATB_ADVMARKET))
				{
					multifactor=AMOUNT_PER_UNIT_HARVEST_MILL;
				}
#endif
				 // ���� ���� ����� ���׷��̵� �Ǿ� ���� �ʴٸ�, 
				 else
				 {
					 multifactor=AMOUNT_PER_UNIT_HARVEST_HQ;
				 }
#ifdef _IMJIN2
				 // ��� ������ ���� ���̸�, 2�踦 �����ش�.
				 if(ResType==CROP_TYPE_RICE)
				 {
					 multifactor=multifactor*2;
				 }
#endif				 


				 if(ResAmount >= PutMoneyUnit)
				 {
					 ResAmount-=PutMoneyUnit;
					 
					 if(ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
					 {
#ifdef _IMJIN2
						 GD.Player[PlayerNumber].IncreaseTree(PutMoneyUnit*multifactor*comadventage);  
#endif

#ifdef _MYTH
						 GD.Player[PlayerNumber].IncreaseTree(PutMoneyUnit*multifactor*comadventage/2);  
#endif

						 CR[BaseID.ID].AccumulatedHarvestTree+=DWORD(PutMoneyUnit*multifactor);
						 
						 GD.RI[ PlayerNumber ].Tree+=(PutMoneyUnit*multifactor);
					 }
					 else
					 {
#ifdef _IMJIN2
						 GD.Player[PlayerNumber].IncreaseMoney(PutMoneyUnit*multifactor*comadventage);  
#endif

#ifdef _MYTH
						 GD.Player[PlayerNumber].IncreaseMoney(PutMoneyUnit*multifactor*comadventage/2);  
#endif

						 CR[BaseID.ID].AccumulatedHarvestCrop+=(PutMoneyUnit*multifactor);
						 
						 GD.RI[ PlayerNumber ].Crop+=(PutMoneyUnit*multifactor);
					 }
				 }
				 else
				 {
					 ResAmount--;
					 if(ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
					 {
#ifdef _IMJIN2
						 GD.Player[PlayerNumber].IncreaseTree(1*multifactor*comadventage);
#endif
#ifdef _MYTH
						GD.Player[PlayerNumber].IncreaseTree(1*multifactor*comadventage/2);
#endif
						 CR[BaseID.ID].AccumulatedHarvestTree+=(1*multifactor);
						 
						 GD.RI[ PlayerNumber ].Tree+=(1*multifactor);
					 }
					 else
					 {
#ifdef _IMJIN2
						 GD.Player[PlayerNumber].IncreaseMoney(1*multifactor*comadventage);
#endif
#ifdef _MYTH
						GD.Player[PlayerNumber].IncreaseMoney(1*multifactor*comadventage/2);
#endif
						 CR[BaseID.ID].AccumulatedHarvestCrop+=(1*multifactor);
						 
						 GD.RI[ PlayerNumber ].Crop+=(1*multifactor);
					 }
				 }
			 }
			 else
			 {
				 WorkStep=WS_DECIDE_LENGTH_TO_DEST;
			 }
		 }
		 else
		 {
			 OrderStop(NewID, BY_COMPUTER);
		 }	
		 break;
  }
  
  return TRUE;
}


// ��Ȯ�ϴ� �ִϸ��̼� �� ��Ÿ ó�� 
BOOL cltCharOrg::Work()
{
	int maxstep=0;
	int maxrepeatnumber;
	SI32 delay = 0;
	
	
#ifdef _IMJIN2
	// �۾��� �ϴ� Ƚ�� 
	maxrepeatnumber=12;
#endif
#ifdef _MYTH
	// ��� ������ �Ǿ� ������ ��Ȯ�ӵ��� ���ȴ�. 
	if(GD.Player[PlayerNumber].UpgradeSwitch[UPGRADE_UPFARMER] == TRUE)
	{
		maxrepeatnumber = 9;
	}
	else
	   maxrepeatnumber = 12;
#endif
	
	
    switch(ResType)
	{ 
		case CROP_TYPE_RICE:   // ���� ��� 
			Animation=ANI_HARVESTCROP;		
			maxstep = KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].GetFrameNumber();
			delay	= KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].siDelay;
			break;
#ifdef _IMJIN2
	   case	CROP_TYPE_POTATO:   // �������� ��� 
		   Animation=ANI_HARVESTCROP;		
		   maxstep  = KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].GetFrameNumber();	  
		   delay	= KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].siDelay;
		   break;
#endif
	   case CROP_TYPE_TREE:   // ������ ���  
	   case CROP_TYPE_BAMBOO:   // ������ ���  
		   Animation=ANI_HARVESTTREE;		
		   maxstep=KI[Kind].clCharAnimation[HarvestTreeCharAnimationIndex].GetFrameNumber();	  
		   delay	= KI[Kind].clCharAnimation[HarvestTreeCharAnimationIndex].siDelay;
		   break;

	   default:               // ��Ÿ�� ��� 
		   Animation=ANI_HARVESTCROP;
		   maxstep  =KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].GetFrameNumber();	  
		   delay	= KI[Kind].clCharAnimation[HarvestTreeCharAnimationIndex].siDelay;		   
		   break;
	}
	
	WorkDelayor++;
	if(WorkDelayor < delay){ return FALSE;}
	WorkDelayor=0;
	
	// ��Ȯ�ϴ� �������� �������̴�. 
	++Step%=maxstep;
	AniStep = Step;
	UpdateAnimationSwitch=TRUE;
	
	// ���� ��ġ ����(�ٸ� ����� �޾Ƶ����� ����)
	BusySwitch=TRUE;
	
	// ��Ȯ�� �� ����Ŭ�� ������.
	if(Step==(maxstep-1))
	{
		
		
		if(ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
		{
			// ������ �Ҹ��� ����. 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(X, Y)==FALSE)
				PushEffect(EFFECT_TREE, X, Y);
		}
		else
		{
#ifdef _IMJIN2			
			// �۾��� ���� ȿ������ �޸�����. 
			if(Kind==KIND_FARMERJ)
			{
				// ��� ������ �Ҹ��� ����. 
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				if(GD.IsInBlackFog(X, Y)==FALSE)
					PushEffect(EFFECT_FARMERJWORK, X, Y);
			}
			// ȣ���� �Ҹ��� ����. 
			else
			{
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				if(GD.IsInBlackFog(X, Y)==FALSE)
					PushEffect(EFFECT_CROP, X, Y);
			}
#endif
#ifdef _MYTH
			 // ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			 if(GD.IsInBlackFog(X, Y)==FALSE)
		         PushEffect(EFFECT_CROP, X, Y);
#endif
		}
		
		
		BusySwitch=FALSE;
		
		RepeatNumber++;
		
		if(RepeatNumber >= (maxrepeatnumber-1))
		{
#ifdef _IMJIN2
			// ��Ȯ�� ��� ������ ���� ������ �ִ� ������ ������ �����Ѵ�. 
			switch(ResType)
			{
			case CROP_TYPE_RICE:   // ���� ��� 
				ResAmount+=UNIT_HARVEST_MOUNT_CROP;
				break;
			case CROP_TYPE_POTATO:   // ������ ��� 
				ResAmount+=UNIT_HARVEST_MOUNT_CROP;
				break;
			case CROP_TYPE_TREE:   // ������ ��� 
			case CROP_TYPE_BAMBOO:
				ResAmount+=UNIT_HARVEST_MOUNT_TREE;
				break;
			default:               // ��Ÿ�� ��� 
				ResAmount+=UNIT_HARVEST_MOUNT_CROP;
				break;
			}
#endif
#ifdef _MYTH
			 ResAmount+=UNIT_HARVEST_MOUNT;
			 
			 // ������ ���� ���ش�. 
			 if(ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
			 {
				 ResAmount += 1;
			 }
#endif
			// ��� ���δ�.
			Map.DecreaseCrop(HarvestX, HarvestY, ResAmount);
		}
	}
	
	if(Step==0 && RepeatNumber>=(maxrepeatnumber-1))
	{
		
		RepeatNumber=0;
		//CR[ID].AttackDelayor=0;
		BusySwitch=FALSE;
		return TRUE;
	}
	else return FALSE;
	
}


// x, y�ֺ��� ��� �󸶳� �ִ��� Ȯ���Ѵ�. 
SHORT GetNearCropCount(SHORT x, SHORT y)
{
	
	int i, j;
	SHORT cropnumber=0;
	
	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			if(Map.IsInMapArea(x+j, y+i) && Map.GetCropMountMap(x+j, y+i))
			{
				cropnumber++;
			}
		}
		
	return cropnumber;
}
