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
	#include "..\천년의 신화\char.h"
#endif

#include <map.h>
#include <CharInterface.h>
#include <findpath5.h>



extern _MainData GD;
extern _Map Map;
extern cltCharInterface clifChar;         // 캐릭터 관련 인터페이스 
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

// 주어진 곳에서 거리가 가장 가까운 곳을 찾는다. 
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

		// 작업 가능하고 같은 섹터에 있어야 한다. 
	   if(clifChar.CanWork(id) && sector==Map.GetSector( clifChar.GetX(id) ,clifChar.GetY(id) ) )
	   {
			length=max(abs(clifChar.GetX(id) - x),abs(clifChar.GetY(id) - y));

			// 시장 가장 가까운 것이면, 
 			if(clifChar.IsAtb(id, ATB_MARKET)==TRUE )
			{
				// 방앗간이면, 
				if(KI[clifChar.GetKind(id)].IsKindAtb(KINDATB_MILL)==TRUE)
				{
				   if(length<mill_short_length)
				   {
			          mill_short_length=length;
			          millid=id;
				   }
				}
				// 본영이면, 
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

	// 방앗간이 있다면, 
	if(millid)
	{
		// 방앗간이 멀리있고, 본영은 가까이 있다면, 
		if(mill_short_length>15 && hq_short_length<15)
		{
		}
		else
		   return clifChar.GetID(millid);
	}

	// 본영이 있다면, 
	if(hqid)return clifChar.GetID(hqid);
	else return ZeroID;

}

BOOL cltCharOrg::SetHarvest(SHORT dx, SHORT dy, _NewID baseid)
{

	SHORT type, vary;
    
	// 만약 선택된 것이 우리 본영이나 시장이면 우리 기지를 그 본영으로 설정한다. 
	if(IsSee(baseid.ID) 
	&& (clifChar.IsAtb(baseid.ID,ATB_MARKET))
	&&  clifChar.CanWork(baseid.ID)
	&&  clifChar.GetPlayerNumber(baseid.ID)== PlayerNumber)
	{
	   // 농부가 자신의 기지로 삼는 건물을 설정한다. 
	   BaseID=baseid;
	   BaseSelectedByPlayerSwitch=TRUE;

	   // 갖다 놓은 이후에는 정지한다.
//	   HarvestX=HarvestY=-1;

       // 돈이 있으면 방앗간으로 가고 없으면 아무것도 하지 않는다. 
       if(ResAmount)
	   {
           WorkStep = WS_GOTO_BASE;
		   SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
		   return TRUE;
	   }
	   else return FALSE;
   }

	// 곡물의 종류를 확인한다. 
	type=Map.GetCropTypeMap(dx, dy);
	vary=Map.GetCropVaryMap(dx, dy);


	// 풀 이외의 곡물을 수확한다. 
	if(IsCropHarvest(NewID.ID, type)==TRUE)
	{
		TargetResType=type;

		HarvestX=dx;
		HarvestY=dy;

		// 새로운 기지를 찾는다. 
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
		// 지도상의 유효한 지역에 한해 이동한다.
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
			
		// 현재 운반중인 돈이 최대 가능량보다 많거나 같으면 
		// 기지로 돌아간다.
		if(ResAmount>=KI[Kind].MaxResAmount)
		{
			WorkStep = WS_SET_GOTO_BASE;
			break;
		}
		// 나무를 수확하는 중에는 나무 옆자리로 간다. 
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
		
		// 수확하고자 하는 목적지에 도착 
		if(status==1)
		{ 
			
			// 쌀이나 감자등은 자기 위치에서 찾는다. 
			type=0;
			if((TargetResType==CROP_TYPE_RICE		 || TargetResType==CROP_TYPE_POTATO)
				&& IsAtb2(ATB2_HARVESTCROP)==TRUE)
			{
				// 남은 곡물이 있다면, 
				if(Map.GetCropMountMap(X, Y))
				{
					type=Map.GetCropTypeMap(X, Y);
					if(type!=TargetResType)type=0;
				}
				
			}
			// 나무는 주변에서 찾는다. 
			else if(TargetResType==CROP_TYPE_TREE
				||  TargetResType==CROP_TYPE_BAMBOO)
			{
				// 주변에 몇개의 곡물이 있는지 확인한다. 
				SHORT min_number=10;
				int cropnumber=0;
				
				for(i=-1;i<=1;i++)
					for(j=-1;j<=1;j++)
					{
						
						tx=X+j;
						ty=Y+i;
						if(Map.IsInMapArea(tx, ty)==TRUE)
						{
							// 자원의 종류가 목재라면, 
							// 남은 곡물의 양이 있어야 
							// 뭔가 다른 물체가있어서는 안된다. 
							if(Map.GetCropTypeMap(tx, ty)==TargetResType 
								&& Map.GetCropMountMap(tx, ty)
								&& Map.GetIDMap(tx, ty)==0)
							{
								// 인근의 곡물양을 파악한다. 
								cropnumber=GetNearCropCount(tx, ty);
								
								
								// 주변에 곡물이 가장 적은 것을 캔다. 
								if(cropnumber<min_number)
								{
									min_number=cropnumber;
									
									type=TargetResType;
									HarvestX=tx;
									HarvestY=ty;
									
									// 방향을 결정한다. 
									Direction =  DecideDirection8(X, Y, HarvestX, HarvestY);
									
								}
							}
						}
					}
					
			}
			
			// 곡물이 위치에 있어야 한다. 
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
				//몇번 이동을 시도했다가 실패하면 멈춘다. 
				AttackGiveUpDelayor++;
				
				if( AttackGiveUpDelayor > AttackGiveUpDelay )
				{
					OrderStop(NewID, BY_COMPUTER);
					break;
				}

				// 인접한 지역에서 곡물을 찾는다. 
				if(FindNearCrop(HarvestX, HarvestY, dx, dy, TargetResType)==TRUE)
				{
					HarvestX=dx;
					HarvestY=dy;
					SetMove(dx, dy);
				}
				// 곡물을 찾지 못하면,
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
		 // 나무를 베는 중이고 , 
		 // 휴식을 취하는 애니메이션이 있고, 
		 // 아직 작업을 시작하지 않았으며, 
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
			 
			 // 휴식을 취하는 애니메이션을 보여준다.
			 RestSwitch=TRUE;
			 
			 
			 WorkDelayor++;
			 if( WorkDelayor < KI[Kind].clCharAnimation[RestCharAnimationIndex].siDelay )
			 {
				 return TRUE;
			 }

			 WorkDelayor=0;
			 
			 Animation=ANI_RESTBEFORETREEHARVEST;		
			 
			 // 수확하는 프레임은 프레임이다. 
			 tempStep = Step;
			 (++tempStep) %= KI[Kind].clCharAnimation[RestCharAnimationIndex].GetFrameNumber();
			 Step=tempStep;
		 
			 AniStep=Step;

			 UpdateAnimationSwitch=TRUE;
			 
			 // 현재 조치 중임(다른 명령을 받아들이지 않음)
			 BusySwitch=TRUE;
			 
			 // 수확의 한 싸이클이 끝났다.
			 if( Step == (KI[Kind].clCharAnimation[RestCharAnimationIndex].GetFrameNumber() - 1) )
			 {
				 RestSwitch=FALSE;
			 }			 
			// 임진록2인 경우에만 휴식을 취하는 애니메이션이 있기 때문에.
			 bTempRestSwitch = TRUE;
		 }
#endif

		 if(bTempRestSwitch == FALSE && Work()==TRUE)
		 {
			 
			 // 마지막으로 모션을취한 프레임을 기억한다.
			 LastHarvestMotionFrame=GD.Frame;
			 
			 
			 // 가지고 있는 돈이 최대 수송량 보다 많거나 같으면, 
			 if(ResAmount>=KI[Kind].MaxResAmount)
			 {
				 WorkStep=WS_SET_GOTO_BASE;
				 
			 }
			 else
			 {
				 
				 // 수확하던 자리에 곡물이 남아 있는지 확인한다. 	
				 if(IsCropHarvest(NewID.ID,  Map.GetCropTypeMap(HarvestX, HarvestY)  )==TRUE && Map.GetCropMountMap(HarvestX, HarvestY))
				 {
				 }
				 // 가까운 지역에 있는 곡물을 찾는다. 
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
		 
		 // 수확하던 곳을 기억한다. 
		 //HarvestX=CR[ID.ID].GetX();
		 //HarvestY=CR[ID.ID].GetY();
		 
		 // 현재 Base가 적절한 것이라면 그곳으로 간다. 
		 if(BaseSelectedByPlayerSwitch==TRUE 
			 && IsSee(BaseID.ID) && clifChar.IsAtb(BaseID.ID, ATB_MARKET)
			 && clifChar.CanWork(BaseID.ID) && clifChar.GetPlayerNumber(BaseID.ID)==PlayerNumber)
		 {
			 SetMove(clifChar.GetX(BaseID.ID), clifChar.GetY(BaseID.ID));
			 WorkStep=WS_GOTO_BASE;
		 }
		 // 가지고 있던 Base가 부적절하면 새로 찾는다. 
		 else
		 {
			 
			 // 적합한 새로운 기지를 찾는다. 
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
		 
		 // 이동이 완료되거나 
		 if(status==MOVEINFO_MOVEDONE)
		 {
			 WorkStep = WS_DECIDE_LENGTH_TO_BASE;
			 
			 //몇번 이동을 시도했다가 실패하면 멈춘다. 
			 AttackGiveUpDelayor++;
			 
			 if( AttackGiveUpDelayor > AttackGiveUpDelay )
			 {
				 OrderStop(NewID, BY_COMPUTER);
				 break;
			 }
		 }
		 // 중간 지점에 도착한 것이라면, 
		 else if(status==MOVEINFO_HALFDONE)
		 {
			 WorkStep=WS_DECIDE_LENGTH_TO_BASE;
		 }
		 // 실제 이동이 있었다면, 
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
				 
				 
				 // 가까이에 기지가 있다. 
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
				 
				 // 컴퓨터인 경우에 어드벤티지를 준다. 
				 if(GD.Player[PlayerNumber].PI.Host==BY_COMPUTER)
				 {
					 comadventage=COMADVENTAGE_MOUNT;
				 }
				 else
				 {
					 comadventage=1;
				 }
#ifdef _IMJIN2
				 // 시장 성능 향상이 업그레이드 되어 있다면, 
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
				 // 시장 성능 향상이 업그레이드 되어 있지 않다면, 
				 else
				 {
					 multifactor=AMOUNT_PER_UNIT_HARVEST_HQ;
				 }
#ifdef _IMJIN2
				 // 곡물의 종류에 따라 쌀이면, 2배를 곱해준다.
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


// 수확하는 애니메이션 및 기타 처리 
BOOL cltCharOrg::Work()
{
	int maxstep=0;
	int maxrepeatnumber;
	SI32 delay = 0;
	
	
#ifdef _IMJIN2
	// 작업을 하는 횟수 
	maxrepeatnumber=12;
#endif
#ifdef _MYTH
	// 쟁기 개량이 되어 있으면 수확속도가 향상된다. 
	if(GD.Player[PlayerNumber].UpgradeSwitch[UPGRADE_UPFARMER] == TRUE)
	{
		maxrepeatnumber = 9;
	}
	else
	   maxrepeatnumber = 12;
#endif
	
	
    switch(ResType)
	{ 
		case CROP_TYPE_RICE:   // 쌀인 경우 
			Animation=ANI_HARVESTCROP;		
			maxstep = KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].GetFrameNumber();
			delay	= KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].siDelay;
			break;
#ifdef _IMJIN2
	   case	CROP_TYPE_POTATO:   // 옥수수인 경우 
		   Animation=ANI_HARVESTCROP;		
		   maxstep  = KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].GetFrameNumber();	  
		   delay	= KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].siDelay;
		   break;
#endif
	   case CROP_TYPE_TREE:   // 나무인 경우  
	   case CROP_TYPE_BAMBOO:   // 나무인 경우  
		   Animation=ANI_HARVESTTREE;		
		   maxstep=KI[Kind].clCharAnimation[HarvestTreeCharAnimationIndex].GetFrameNumber();	  
		   delay	= KI[Kind].clCharAnimation[HarvestTreeCharAnimationIndex].siDelay;
		   break;

	   default:               // 기타의 경우 
		   Animation=ANI_HARVESTCROP;
		   maxstep  =KI[Kind].clCharAnimation[HarvestCropCharAnimationIndex].GetFrameNumber();	  
		   delay	= KI[Kind].clCharAnimation[HarvestTreeCharAnimationIndex].siDelay;		   
		   break;
	}
	
	WorkDelayor++;
	if(WorkDelayor < delay){ return FALSE;}
	WorkDelayor=0;
	
	// 수확하는 프레임은 프레임이다. 
	++Step%=maxstep;
	AniStep = Step;
	UpdateAnimationSwitch=TRUE;
	
	// 현재 조치 중임(다른 명령을 받아들이지 않음)
	BusySwitch=TRUE;
	
	// 수확의 한 싸이클이 끝났다.
	if(Step==(maxstep-1))
	{
		
		
		if(ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
		{
			// 도끼질 소리를 낸다. 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(X, Y)==FALSE)
				PushEffect(EFFECT_TREE, X, Y);
		}
		else
		{
#ifdef _IMJIN2			
			// 작업에 따라 효과음을 달리낸다. 
			if(Kind==KIND_FARMERJ)
			{
				// 기계 돌리는 소리를 낸다. 
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(X, Y)==FALSE)
					PushEffect(EFFECT_FARMERJWORK, X, Y);
			}
			// 호미질 소리를 낸다. 
			else
			{
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(X, Y)==FALSE)
					PushEffect(EFFECT_CROP, X, Y);
			}
#endif
#ifdef _MYTH
			 // 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			 if(GD.IsInBlackFog(X, Y)==FALSE)
		         PushEffect(EFFECT_CROP, X, Y);
#endif
		}
		
		
		BusySwitch=FALSE;
		
		RepeatNumber++;
		
		if(RepeatNumber >= (maxrepeatnumber-1))
		{
#ifdef _IMJIN2
			// 수확한 곡물의 종류에 따라 가지고 있는 물건의 종류를 설정한다. 
			switch(ResType)
			{
			case CROP_TYPE_RICE:   // 쌀인 경우 
				ResAmount+=UNIT_HARVEST_MOUNT_CROP;
				break;
			case CROP_TYPE_POTATO:   // 감자인 경우 
				ResAmount+=UNIT_HARVEST_MOUNT_CROP;
				break;
			case CROP_TYPE_TREE:   // 나무인 경우 
			case CROP_TYPE_BAMBOO:
				ResAmount+=UNIT_HARVEST_MOUNT_TREE;
				break;
			default:               // 기타의 경우 
				ResAmount+=UNIT_HARVEST_MOUNT_CROP;
				break;
			}
#endif
#ifdef _MYTH
			 ResAmount+=UNIT_HARVEST_MOUNT;
			 
			 // 나무는 조금 더준다. 
			 if(ResType==CROP_TYPE_TREE || ResType==CROP_TYPE_BAMBOO)
			 {
				 ResAmount += 1;
			 }
#endif
			// 곡물을 줄인다.
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


// x, y주변에 곡물이 얼마나 있는지 확인한다. 
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
