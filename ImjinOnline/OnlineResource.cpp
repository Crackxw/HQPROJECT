#include <GSL.h>
#include <Main.h>
#include <etc.h>


#include "Online.h"
#include <graphics16.h>
#include "OnlineResource.h"
#include "BindJXFile.h"

#include "OnlineCommonParser.h"		// 공용 파서기 추가

extern	char 				SavePath[];

CHAR	OnlineResource::szOnlineGameFile[MAX_ONLINE_GAME_FILE][128] = 
{
	"Online\\VillageBack\\DowntownBorder.Spr",		// 0
	"Online\\GameSpr\\balloon.spr",
	"Online\\Gamespr\\TargetOn.Spr",
	"Online\\Gamespr\\Minimap.Spr",
	"Online\\Gamespr\\BWait.Spr",
	"Online\\Gamespr\\CHARACTERSLOT.Spr",
	"Online\\Gamespr\\Mouse.spr",
	"Online\\Gamespr\\Inventory.spr",
	"Online\\Gamespr\\BookExit.Spr",
	"Online\\Gamespr\\NumPad.Spr",					

	"Online\\Gamespr\\MsgBoxBoard2.Spr",			// 10
	"Online\\Gamespr\\ScrollBar_H.Spr",
	"Online\\Gamespr\\ScrollBar_V.Spr",
	"Online\\Gamespr\\ScrollH_Up.Spr",
	"Online\\Gamespr\\ScrollV_Up.Spr",
	"Online\\Gamespr\\ScrollH_Down.Spr",
	"Online\\Gamespr\\ScrollV_Down.Spr",
	"Online\\VillageBack\\VillageBorder.Spr",
	"Online\\Gamespr\\Weather.Spr",
	"Online\\Gamespr\\LogIn\\LoginButton.Spr",

	"Online\\Gamespr\\NumButton.Spr",				// 20
	"Online\\Gamespr\\MsgBoxBoard1.Spr",	
	"Online\\Gamespr\\BlackTile.Spr",
	"Online\\Gamespr\\NumButton1.Spr",
	"Online\\Gamespr\\NumButton2.Spr",
	"Online\\Gamespr\\NumButton3.Spr",
	"Online\\Gamespr\\NUMBERGAGEBAR.Spr", 
	"Online\\Gamespr\\Minimap_Enlarge.Spr",			
	"Online\\Gamespr\\Minimap_Reduction.Spr",
	"Online\\Gamespr\\Minimap_Normal.Spr",			

	"Online\\Gamespr\\Minimap_Number.Spr",			// 30
	"Online\\Gamespr\\Minimap_Toggle.Spr",
	"Online\\Gamespr\\Minimap_Mini.Spr",	
	"Online\\Gamespr\\ALLIMAGE_BUTTONEM.Spr",
	"Online\\Gamespr\\Number14x14.Spr",	
	"Online\\Gamespr\\movemark.spr",
	"Online\\Gamespr\\ImoticonS.Spr",
	"Online\\Gamespr\\Count.Spr",					
	"Online\\Gamespr\\GuildHNSEffect.Spr",
	"Online\\Gamespr\\GuildDAIEffect.Spr",			

	"Online\\Gamespr\\NumberBigR.Spr",				// 40
	"Online\\Gamespr\\NumberBigG.Spr",
	"Online\\Gamespr\\VillageGage.Spr",
	"Online\\Gamespr\\UpBig.Spr",
	"Online\\Gamespr\\DownBig.Spr",
	"Online\\Gamespr\\panel\\MAIN_CHAT_BASE_1.Spr",
	"Online\\Gamespr\\buildexp.spr",
	"Online\\GameSpr\\Login\\scrollbar.Spr",		
	"Online\\GameSpr\\WarpEffect.Spr",				
	"Online\\GameSpr\\LV_Up.Spr",					

	"Online\\Gamespr\\ExitButton.Spr",				// 50
	"Online\\Gamespr\\exp.Spr",
	"Online\\Gamespr\\Money.Spr",
	"Online\\Gamespr\\sinyongdo.Spr",
	"Online\\Gamespr\\quest_1.Spr",
	"Online\\Gamespr\\MsgBoxButton.Spr",
	"Online\\Gamespr\\ChangeMap.Spr",
	"Online\\Gamespr\\ChangeWomen.Spr",				
	"Online\\Gamespr\\ChangeEffect.Spr",
	"Online\\Gamespr\\GuildDHSEffect.Spr",			

	"Online\\Gamespr\\GuildHNDEffect.Spr",			//60
	"Online\\Gamespr\\NewLogin\\login_agreement_scrbar.Spr",		
	"Online\\GameSpr\\NewLogin\\message_window_1.Spr",
	"Online\\GameSpr\\NewLogin\\message_window_2.Spr",	
	"Online\\GameSpr\\NewLogin\\message_window_button.Spr",	
	"Online\\GameSpr\\Panel\\MAIN_CHAT_SCRHANDLE.Spr",
	"Online\\Gamespr\\Book\\book_scr_handle.Spr",
	"Online\\Gamespr\\Effect\\suhoeffect.Spr",

	// robypark 2004/10/29 14:16
	// 공성전 관련 이펙트 추가
	"Online\\Gamespr\\Effect\\EffectTransform.Spr",	// 68, ON_GAMESPR_SIEGEWARFARE_TRANSFORM

	// robypark 2005/2/18 18:14
	// 공성전 적 상단 유닛 체력 게이지(%로 표시)
	"Online\\Gamespr\\Gauge\\SIEGE_CHAR_GAUGE_3.Spr",				// 69, ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY

	// robypark 2004/11/9 19:44
	// 공성전 대장 효과 영향 받는 캐릭터 표시 리소스 추가
	// 사용자의 아군일 경우 파란색 계열
	"Online\\Gamespr\\Effect\\EffectGeneralAuraBlue.Spr",	// 70, ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_BLUE

	// 사용자의 적군일 경우 붉은색 계열
	"Online\\Gamespr\\Effect\\EffectGeneralAuraRed.Spr",	// 71, ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_RED

	// 사용자의 적대상단도 같은 상단도 아닌 경우
	"Online\\Gamespr\\Effect\\EffectGeneralAura.Spr",		// 72, ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_UNRELATED

	// robypark 2004/11/10 15:58
	// 공성전 대장 캐릭터 표시 리소스 추가
	// 사용자의 아군일 경우 파란색 계열
	"Online\\Gamespr\\Effect\\EffectGeneralBlue.Spr",	// 73, ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_BLUE

	// 사용자의 적군일 경우 붉은색 계열
	"Online\\Gamespr\\Effect\\EffectGeneralRed.Spr",	// 74, ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_RED

	// 사용자의 적대상단도 같은 상단도 아닌 경우
	"Online\\Gamespr\\Effect\\EffectGeneral.Spr",	// 75, ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_UNRELATED

	// robypark 2004/11/24 18:42
	// 견고한 방어 이펙트
	"Online\\Gamespr\\Effect\\EffectEvasion.Spr",	// 76, ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION

	// 치명적 공격 이펙트
	"Online\\Gamespr\\Effect\\EffectDeathBlow.Spr",	// 77, ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW	

	// 마을 반격 기름 공격 이펙트
	"Online\\Gamespr\\Effect\\EffectVillageOil.Spr",	// 78, ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL

	// 마을 반격 돌 공격 이펙트
	"Online\\Gamespr\\Effect\\EffectVillageStone.Spr",	// 79, ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE

	// robypark 2004/12/12 18:7
	// 공성전 관련 효과 리소스 추가
	// 공성전 유닛인 상태에서 재차 소집권을 사용하여 변신할 경우 이펙트
	"Online\\Gamespr\\Effect\\EffectReTransform.Spr",			// 80, ON_GAMESPR_SIEGEWARFARE_EFFECT_RETRANSFORM
	// 공성전 유닛의 보급품이 바닥난 이펙트
	"Online\\Gamespr\\Effect\\EffectSupplyGoodsRanOut.Spr",		// 81, ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT
	// 공성전 유닛이 보급품을 받았다. 이펙트
	"Online\\Gamespr\\Effect\\EffectSupplyGoodsFilled.Spr",		// 82, ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED
	// 마을 방어력 향상 이펙트
	"Online\\Gamespr\\Effect\\EffectVillageDefenceUp.Spr",		// 83, ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_DEFENCE_UP
	// 마을 점령 효과
	"Online\\Gamespr\\Effect\\EffectOccupiedVillage.Spr",		// 84, ON_GAMESPR_SIEGEWARFARE_EFFECT_OCCUPIED_VILLAGE

	// robypark 2004/12/12 11:10
	// 검병으로 마을을 공격하여 점령하라는 이펙트
	"Online\\Gamespr\\Effect\\EffectEnabledOccupyVillage.Spr",		// 85, ON_GAMESPR_EFFECT_ENABLED_OCCUPY_VILLAGE
	// 캐릭터가 수성인터페이스 안에 있는 경우 캐릭터 대신에 그려진다.
	"Online\\Gamespr\\Effect\\EffectInVillageDefenceInterface.Spr",	// 86, ON_GAMESPR_EFFECT_IN_VILLAG_EDEFENCE_INTERFACE

	// robypark 2005/1/7 15:53
	// 게이지 관련 이미지
	// 공성전 유닛 체력 게이지(%로 표시)
	"Online\\Gamespr\\Gauge\\SIEGE_CHAR_GAUGE_2.Spr",			 	// 87, ON_GAMESPR_GAUGE_UNIT_HEALTH
	// 공성전 유닛 보급품 게이지(%로 표시)
	"Online\\Gamespr\\Gauge\\SIEGE_CHAR_GAUGE_1.Spr", 				// 88, ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT
	// 공성전 유닛 체력, 보급품 게이지 배경 이미지
	"Online\\Gamespr\\Gauge\\SIEGE_CHAR_GAUGE_BASE.Spr",			// 89, ON_GAMESPR_GAUGE_UNIT_BACKGROUND
	// 마을 방어력 량 게이지
	"Online\\Gamespr\\Gauge\\SIEGE_TOWN_GAUGE_1.Spr",				// 90, ON_GAMESPR_GAUGE_VILLAGE_DEFENCE
	// 마을 방어력 량 게이지 배경 이미지
	"Online\\Gamespr\\Gauge\\SIEGE_TOWN_GAUGE_1_BASE.Spr",			// 91, ON_GAMESPR_GAUGE_VILLAGE_DEFENCE_BACKGROUND
	// 마을 주둔군 수 게이지
	"Online\\Gamespr\\Gauge\\SIEGE_TOWN_GAUGE_2.Spr",				// 92, ON_GAMESPR_GAUGE_VILLAGE_GARRISON
	// 마을 주둔군 수 게이지 배경 이미지
	"Online\\Gamespr\\Gauge\\SIEGE_TOWN_GAUGE_2_BASE.Spr",			// 93, ON_GAMESPR_GAUGE_VILLAGE_GARRISON_BACKGROUND

	// dead penalty를 위한 덧그리기시 사용될 이미지
	"Online\\Item\\OverDeadPenalty.Spr",							// 94, ON_GAMESPR_ITEM_OVER_DEAD_PENALTY

	// robypark 2005/2/16 14:39
	// 녹색 숫자
	"Online\\Gamespr\\Effect\\FONT_EX_0.Spr",						// ON_GAMESPR_NUMBER_GREEN
	// 적색 숫자
	"Online\\Gamespr\\Effect\\FONT_EX_1.Spr",						// ON_GAMESPR_NUMBER_RED
	// 청색 숫자
	"Online\\Gamespr\\Effect\\FONT_EX_2.Spr",						// ON_GAMESPR_NUMBER_BLUE
	// 흰색 숫자
	"Online\\Gamespr\\Effect\\FONT_EX_3.Spr",						// ON_GAMESPR_NUMBER_WHITE

	// robypark 2004/11/4 15:55
	// 공성전 유닛 바닥 효과 리소스 추가
	// 중립 공성 유닛 졸병 바닥 효과
	"Online\\Gamespr\\Effect\\EffectSoldier.Spr",					// 99, ON_GAMESPR_SIEGEWARFARE_SOLDIER_EFFECT

	"Online\\Gamespr\\Effect\\EffectSoldierBlue.Spr",				// ON_GAMESPR_SIEGEWARFARE_SOLDIER_BLUE_EFFECT	// 아군 공성 유닛 사환 이펙트

	"Online\\Gamespr\\Effect\\EffectSoldierRed.Spr"					// ON_GAMESPR_SIEGEWARFARE_SOLDIER_RED_EFFECT	// 적군 공성 유닛 사환 이펙트
}; 

CHAR	OnlineResource::szOnlineTileFile[MAX_ONLINE_TILE_FILE][128];
CHAR	OnlineResource::szOnlineItemFile[MAX_ONLINE_ITEM_FILE][128];
CHAR	OnlineResource::szOnlineObjectFile[MAX_ONLINE_OBJECT_FILE][128];


//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자
//	리턴	:	
//----------------------------------------------------------------------------------------------------------------
OnlineResource::OnlineResource()
{
	siCount = 0;

	ZeroMemory(ImageBaseTileData, sizeof(OnlineResource_BaseData) * MAX_ONLINE_TILE_BASEDATAFILE);
	ZeroMemory(ImageExtTileData, sizeof(OnlineResource_ExtData) * MAX_ONLINE_TILE_EXTDATAFILE);

	ZeroMemory(ImageBaseObjectData, sizeof(OnlineResource_BaseData) * MAX_ONLINE_OBJECT_BASEDATAFILE);
	ZeroMemory(ImageExtObjectData, sizeof(OnlineResource_ExtData) * MAX_ONLINE_OBJECT_EXTDATAFILE);

	ZeroMemory(xsprOnlineItem, sizeof(XSPR) * MAX_ONLINE_ITEM_FILE);
	ZeroMemory(xsprOnlineGame, sizeof(xsprOnlineGame) );

	siVillageFirePositionCount = 0;
	sVillageFirePosition       = NULL;

	m_lpsVillageImagePositionInfo = NULL;	// 마을 그릴 때 사용되는 이미지들 포지션 정보 개수
	m_ssCntVillageImagePositionInfo = 0;	// 마을 그릴 때 사용되는 이미지들 포지션 정보 변수
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자
//	리턴	:	
//----------------------------------------------------------------------------------------------------------------
OnlineResource::~OnlineResource()
{

}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	온라인에서 사용되는 리소스(화일등)을 로드한다.
//	리턴	:	성공하면 TRUE. 실패하면 FALSE.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineResource::Init()
{
	SI32	i, j;
	CHAR	szFileName[1024];	
	CHAR	szBuffer[1024];	
	FILE*	fp;
	char	*pToken = NULL;


	ZeroMemory( szOnlineObjectFile, sizeof( szOnlineObjectFile ) );

	ZeroMemory(xsprOnlineItem, sizeof(XSPR) * MAX_ONLINE_ITEM_FILE);
	
	// 게임용 스프라이트를 로드한다.
	i	=	0;
	
	while(1)
	{
		GetFileNamePath(szOnlineGameFile[i], SavePath, szFileName);
		if(strcmp(szOnlineGameFile[i], "") == 0)
			break;
		else
		{ 
			switch(i)
			{
				case ON_GAMESPR_INVENTORY:
//				case ON_GAMESPR_PANNEL_MAIN:
//				case ON_GAMESPR_BOOK:
//				case ON_GAMESPR_PANNEL_GAUGE:
					clGrp.LoadXspr(szFileName, xsprOnlineGame[i], TRUE);
					break;

				default:
					clGrp.LoadXspr(szFileName, xsprOnlineGame[i]);
					break;
			}
 
			i++;
		}
	}
	
	// 다쉬 카운트를 초기화한다.
	siCount	=	0;

	// 오브젝트 스프라이트를 로드함	
	ZeroMemory(ImageBaseObjectData, sizeof(OnlineResource_BaseData) * MAX_ONLINE_OBJECT_BASEDATAFILE);
	for(i = 0; i < MAX_ONLINE_OBJECT_BASEDATAFILE; i++)
	{
		ImageBaseObjectData[i].m_TileIndex   = -1;
		ImageBaseObjectData[i].m_ImageBuffer = new UI08[MAX_ONLINE_OBJECTDATA_IMAGESIZE];
		ImageBaseObjectData[i].m_XSPR.Image  = ImageBaseObjectData[i].m_ImageBuffer;
	}
	
	ZeroMemory(ImageExtObjectData, sizeof(OnlineResource_ExtData) * MAX_ONLINE_OBJECT_EXTDATAFILE);
	for(i = 0; i < MAX_ONLINE_OBJECT_EXTDATAFILE; i++)
	{
		ImageExtObjectData[i].m_TileIndex   = -1;
	}
	LoadCharSpr( "Object", "Object" );	

	// 이모티콘 로드
	ZeroMemory(xsprOnlineImoticon, sizeof(XSPR) * MAX_ONLINE_IMOTICON_FILE);
	for(i = 0; i < MAX_ONLINE_IMOTICON_FILE; i++)
	{
		// File Name
		sprintf((char*)szFileName, "Online\\GameSpr\\Imoticon\\Imo%03d.spr", i);

		// File이 있는지 검사
		fp = fopen((char*)szFileName, "rb");
		if(fp == NULL) break;
		fclose(fp);

		// Load
		clGrp.LoadXspr(szFileName, xsprOnlineImoticon[i]);
	}

	// 깃발 로드
	ZeroMemory(xsprOnlineFlagM, sizeof(XSPR) * MAX_ONLINE_FLAG_FILE);
	ZeroMemory(xsprOnlineFlagV, sizeof(XSPR) * MAX_ONLINE_FLAG_FILE);
	for(i = 0; i < 1; i++)
	{
		// File Name
		sprintf((char*)szFileName, "Online\\GameSpr\\Flag\\Flag_M_%04d.spr", i);

		// File이 있는지 검사
		fp = fopen((char*)szFileName, "rb");
		if(fp == NULL) break;
		fclose(fp);

		// Load
		clGrp.LoadXspr(szFileName, xsprOnlineFlagM[i]);

	
		// File Name
		sprintf((char*)szFileName, "Online\\GameSpr\\Flag\\Flag_V_%04d.spr", i);

		// File이 있는지 검사
		fp = fopen((char*)szFileName, "rb");
		if(fp == NULL) break;
		fclose(fp);

		// Load
		clGrp.LoadXspr(szFileName, xsprOnlineFlagV[i]);
	}
	for(i = 1; i < MAX_ONLINE_FLAG_FILE; i++)
	{
		// File Name
		sprintf((char*)szFileName, "Online\\GameSpr\\Flag\\Flag_M_%04d.spr", i);

		// File이 있는지 검사
		fp = fopen((char*)szFileName, "rb");
		if(fp == NULL) break;
		fclose(fp);

		// Load
		clGrp.LoadXspr(szFileName, xsprOnlineFlagM[i]);

	
		// File Name
		sprintf((char*)szFileName, "Online\\GameSpr\\Flag\\Flag_V_%04d.spr", i);

		// File이 있는지 검사
		fp = fopen((char*)szFileName, "rb");
		if(fp == NULL) break;
		fclose(fp);

		// Load
		clGrp.LoadXspr(szFileName, xsprOnlineFlagV[i]);
	}

	// 타일 스프라이트를 로드한다.
	ZeroMemory(ImageBaseTileData, sizeof(OnlineResource_BaseData) * MAX_ONLINE_TILE_BASEDATAFILE);
	for(i = 0; i < MAX_ONLINE_TILE_BASEDATAFILE; i++)
	{
		ImageBaseTileData[i].m_TileIndex   = -1;
		ImageBaseTileData[i].m_ImageBuffer = new UI08[MAX_ONLINE_TILEDATA_IMAGESIZE];
		ImageBaseTileData[i].m_XSPR.Image  = ImageBaseTileData[i].m_ImageBuffer;
	}
	
	ZeroMemory(ImageExtTileData, sizeof(OnlineResource_ExtData) * MAX_ONLINE_TILE_EXTDATAFILE);
	for(i = 0; i < MAX_ONLINE_TILE_EXTDATAFILE; i++)
	{
		ImageExtTileData[i].m_TileIndex   = -1;
	}

	LoadInformationDirectoryRecurs("online\\tile",		ON_LOAD_KIND_TI);

	// Village Effect
	clGrp.LoadXspr("Online\\GameSpr\\VFire1.Spr", xsprOnlineVillageEffect[0][0][0]);
	clGrp.LoadXspr("Online\\GameSpr\\VFire2.Spr", xsprOnlineVillageEffect[0][0][1]);
	clGrp.LoadXspr("Online\\GameSpr\\VFire3.Spr", xsprOnlineVillageEffect[0][0][2]);

	// Village Attack
	clGrp.LoadXspr("Online\\GameSpr\\VillageAttackStone.Spr", xsprOnlineVillageAttackEffect[0]);
	clGrp.LoadXspr("Online\\GameSpr\\VillageAttackWater.Spr", xsprOnlineVillageAttackEffect[1]);
	clGrp.LoadXspr("Online\\GameSpr\\VillageAttackWater1.Spr", xsprOnlineVillageAttackEffect[2]);

	for(i = 0; i < MAX_ONLINE_TILE_FILE; i++)
	{
		szOnlineTileFilePos[i] = -1;
	}
	for(i = 0; i < MAX_ONLINE_OBJECT_FILE; i++)
	{
		szOnlineObjectFilePos[i] = -1;
	}
	
	fp = BindJXFile::GetInstance()->GetFile( "Online\\Village\\FirePosition.txt" );
	
	if(fp)
	{
		fgets(szBuffer, 1024, fp);
		fgets(szBuffer, 1024, fp);
		fgets(szBuffer, 1024, fp);
		fgets(szBuffer, 1024, fp);

		pToken                     = strtok(szBuffer, "\n\t");
		pToken                     = strtok(NULL, "\n\t");
		siVillageFirePositionCount = atoi(pToken);
		sVillageFirePosition       = new OnlineResource_VillageFireData[siVillageFirePositionCount];
		ZeroMemory(sVillageFirePosition, sizeof(OnlineResource_VillageFireData) * siVillageFirePositionCount);

		fgets(szBuffer, 1024, fp);
		for(i = 0; i < siVillageFirePositionCount; i++)
		{
			fgets(szBuffer, 1024, fp);
			
			// 마을의 Size(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)
			pToken = strtok(szBuffer, "\n\t");
			sVillageFirePosition[i].m_VillageSize = atoi(pToken);

			// 마을의 국가코드
			pToken = strtok(NULL, "\n\t");
			switch(pToken[0])
			{
				case 'K':	case 'k':	
					sVillageFirePosition[i].m_VillageState = ON_COUNTRY_KOREA;
					break;

				case 'J':	case 'j':	
					sVillageFirePosition[i].m_VillageState = ON_COUNTRY_JAPAN;
					break;

				case 'C':	case 'c':	
					sVillageFirePosition[i].m_VillageState = ON_COUNTRY_CHINA;
					break;

				case 'T':	case 't':	
					sVillageFirePosition[i].m_VillageState = ON_COUNTRY_TAIWAN;
					break;
			}

			// 불이난정도
			pToken = strtok(NULL, "\n\t");
			sVillageFirePosition[i].m_VillageFirePer = atoi(pToken);

			// 불수
			pToken = strtok(NULL, "\n\t");
			sVillageFirePosition[i].m_FireCount = atoi(pToken);

			for(j = 0; j < sVillageFirePosition[i].m_FireCount; j++)
			{
				// 불위치
				pToken = strtok(NULL, "\n\t");
				sVillageFirePosition[i].m_FirePositionX[j] = atoi(pToken);

				// 불위치
				pToken = strtok(NULL, "\n\t");
				sVillageFirePosition[i].m_FirePositionY[j] = atoi(pToken);

				// 불 크기
				pToken = strtok(NULL, "\n\t");
				sVillageFirePosition[i].m_FireSize[j] = atoi(pToken);
			}
		}

		fclose(fp);
	}

	// 마을 그릴 때 사용되는 이미지들 포지션 정보 파일 HQ에서 읽기
	LoadVillageImagePositionInfoFile();

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineResource::Free()
{
	SI32	i;

	// 게임용 스프라이트를 해제한다.	
	i	=	0;
	while(1)
	{
		if(strcmp(szOnlineGameFile[i], "") == 0)
			break;
		else
		{
			if(xsprOnlineGame[i].Image)
				clGrp.FreeXspr(xsprOnlineGame[i]);
			i++;
		}		
	}

	// 타일 스프라이트를 해제한다.
	for(i = 0; i < MAX_ONLINE_TILE_BASEDATAFILE; i++)
	{
		if(ImageBaseTileData[i].m_ImageBuffer) delete[] ImageBaseTileData[i].m_ImageBuffer;
	}
	for(i = 0; i < MAX_ONLINE_TILE_EXTDATAFILE; i++)
	{
		if(ImageExtTileData[i].m_XSPR.Image) clGrp.FreeXspr(ImageExtTileData[i].m_XSPR);
	}
	ZeroMemory(ImageBaseTileData, sizeof(OnlineResource_BaseData) * MAX_ONLINE_TILE_BASEDATAFILE);
	ZeroMemory(ImageExtTileData, sizeof(OnlineResource_ExtData) * MAX_ONLINE_TILE_EXTDATAFILE);

	// 오브젝트 스프라이트를 해제한다.
	for(i = 0; i < MAX_ONLINE_OBJECT_BASEDATAFILE; i++)
	{
		if(ImageBaseObjectData[i].m_ImageBuffer) delete[] ImageBaseObjectData[i].m_ImageBuffer;
	}
	for(i = 0; i < MAX_ONLINE_OBJECT_EXTDATAFILE; i++)
	{
		if(ImageExtObjectData[i].m_XSPR.Image) clGrp.FreeXspr(ImageExtObjectData[i].m_XSPR);
	}
	ZeroMemory(ImageBaseObjectData, sizeof(OnlineResource_BaseData) * MAX_ONLINE_OBJECT_BASEDATAFILE);
	ZeroMemory(ImageExtObjectData,  sizeof(OnlineResource_ExtData) * MAX_ONLINE_OBJECT_EXTDATAFILE);

	// 이모티콘 스프라이트를 해제한다.
	for(i = 0; i < MAX_ONLINE_IMOTICON_FILE; i++)
	{
		if(xsprOnlineImoticon[i].Image)
		{
			clGrp.FreeXspr(xsprOnlineImoticon[i]);
			xsprOnlineImoticon[i].Image = NULL;
		}
	}

	// Item 로드
	for(i = 0; i < MAX_ONLINE_ITEM_FILE; i++)
	{
		if(xsprOnlineItem[i].Image)
		{
			clGrp.FreeXspr(xsprOnlineItem[i]);
			xsprOnlineItem[i].Image = NULL;
		}
	}

	// 깃발 스프라이트를 해제한다.
	for(i = 0; i < MAX_ONLINE_FLAG_FILE; i++)
	{
		if(xsprOnlineFlagM[i].Image)
		{
			clGrp.FreeXspr(xsprOnlineFlagM[i]);
			xsprOnlineFlagM[i].Image = NULL;
		}
		if(xsprOnlineFlagV[i].Image)
		{
			clGrp.FreeXspr(xsprOnlineFlagV[i]);
			xsprOnlineFlagV[i].Image = NULL;
		}
	}

	// Village Effect
	for(SI16 TempCount = 0; TempCount < 1; TempCount++)
	{
		for(SI16 TempCount1 = 0; TempCount1 < 1; TempCount1++)
		{
			for(SI16 TempCount2 = 0; TempCount2 < 3; TempCount2++)
			{
				clGrp.FreeXspr(xsprOnlineVillageEffect[TempCount][TempCount1][TempCount2]);
				xsprOnlineVillageEffect[TempCount][TempCount1][TempCount2].Image = NULL;
			}
		}
	}

	// Village Attack Effect
	for(TempCount = 0; TempCount < 3; TempCount++)
	{
		clGrp.FreeXspr(xsprOnlineVillageAttackEffect[TempCount]);
		xsprOnlineVillageAttackEffect[TempCount].Image = NULL;
	}

	for(i = 0; i < MAX_ONLINE_TILE_FILE; i++)
	{
		szOnlineTileFilePos[i] = -1;
	}
	for(i = 0; i < MAX_ONLINE_OBJECT_FILE; i++)
	{
		szOnlineObjectFilePos[i] = -1;
	}

	// 마을 이미지 위치 정보 구조체 변수에 메모리 해제
	if (m_lpsVillageImagePositionInfo)
	{
		delete []m_lpsVillageImagePositionInfo;
		m_lpsVillageImagePositionInfo = NULL;

		m_ssCntVillageImagePositionInfo = 0;
	}

	siVillageFirePositionCount = 0;
	if(sVillageFirePosition)
	{
		delete[] sVillageFirePosition;
		sVillageFirePosition = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	게임 스프라이트 배열의 인덱스를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	OnlineResource::GetGameFileIndex(CHAR *pszFileName)
{
	SI32	i;	
	CHAR	drive[_MAX_DRIVE];
	CHAR	dir[_MAX_DIR];
	CHAR	fname[_MAX_FNAME];
	CHAR	ext[_MAX_EXT];
	CHAR	szFileName[1024];	

	if(pszFileName == NULL)
		return -1;

	for(i = 0; i < 	MAX_ONLINE_GAME_FILE; i++)
	{
		_splitpath(szOnlineGameFile[i], drive, dir, fname, ext);
		wsprintf(szFileName, "%s%s", fname, ext);

		if(stricmp(pszFileName, szFileName)  == 0)
			return i;
	}

	return -1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Item 스프라이트 배열의 인덱스를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	OnlineResource::GetItemFileIndex(CHAR *pszFileName)
{
	SI32	i;	
	CHAR	drive[_MAX_DRIVE];
	CHAR	dir[_MAX_DIR];
	CHAR	fname[_MAX_FNAME];
	CHAR	ext[_MAX_EXT];
	CHAR	szFileName[1024];	

	if(pszFileName == NULL)
		return -1;

	for(i = 0; i < 	MAX_ONLINE_ITEM_FILE; i++)
	{
		_splitpath(szOnlineItemFile[i], drive, dir, fname, ext);
		wsprintf(szFileName, "%s%s", fname, ext);

		if(stricmp(pszFileName, szFileName)  == 0)
			return i;
	}

	return -1;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 스프라이트 배열의 인덱스를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	OnlineResource::GetCharFileIndex(CHAR *pszFileName)
{
	return -1;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	오브젝트 스프라이트 배열의 인덱스를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	OnlineResource::GetObjectFileIndex(CHAR *pszFileName)
{
	SI32	i;	
	CHAR	drive[_MAX_DRIVE];
	CHAR	dir[_MAX_DIR];
	CHAR	fname[_MAX_FNAME];
	CHAR	ext[_MAX_EXT];
	CHAR	szFileName[1024];	

	if(pszFileName == NULL)
		return -1;

	ZeroMemory( szFileName, sizeof( szFileName ) );

	for(i = 0; i < 	MAX_ONLINE_OBJECT_FILE; i++)
	{
		if(szOnlineObjectFile[i][0] == NULL)
			break;
		
		_splitpath(szOnlineObjectFile[i], drive, dir, fname, ext);
		sprintf(szFileName, "%s%s", fname, ext);

		if(stricmp(pszFileName, szFileName)  == 0)
			return i;
	}

	return -1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	private(게임용 스프라이트)에 접근할 수 있는 함수를 만든다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprGame(SI16 siIndex)
{
	return	&xsprOnlineGame[siIndex];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(타일용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprTile(SI16 siIndex)
{	
	if(siIndex == -1) return NULL;

	SI32		TempCount;
	SI32		TempCount1;

	// 현재 로딩된 Data가 있는지 검사
	if(szOnlineTileFilePos[siIndex] != -1)
	{
		if(szOnlineTileFilePos[siIndex] < 256)
		{
			TempCount1 = szOnlineTileFilePos[siIndex];
			ImageBaseTileData[TempCount1].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;

			return &ImageBaseTileData[TempCount1].m_XSPR;
		}
		else
		{
			TempCount1 = szOnlineTileFilePos[siIndex] - 256;
			ImageExtTileData[TempCount1].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;

			return &ImageExtTileData[TempCount1].m_XSPR;
		}
	}

	// 만약 존재하지 않으면, 화일을 로딩한다.
	BOOL		fLargeTile;

	fLargeTile = FALSE;
	for(TempCount = 0; TempCount < MAX_ONLINE_TILE_BASEDATAFILE; TempCount++)
	{
		if(ImageBaseTileData[TempCount].m_TileIndex == -1)
		{
			ImageBaseTileData[TempCount].m_TileIndex    = siIndex;
			ImageBaseTileData[TempCount].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;
			ImageBaseTileData[TempCount].m_XSPR.Image   = ImageBaseTileData[TempCount].m_ImageBuffer;

			sprintf((char*)m_TempBuffer, "%sOnline\\Tile\\%s", SavePath, szOnlineTileFile[siIndex]);
			if(clGrp.LoadXsprInAllocMem((char*)m_TempBuffer, ImageBaseTileData[TempCount].m_XSPR, MAX_ONLINE_TILEDATA_IMAGESIZE) == FALSE)
			{
				ImageBaseTileData[TempCount].m_TileIndex    = -1;
				ImageBaseTileData[TempCount].m_LastUseCount = 0;

				fLargeTile = TRUE;
				break;
			}

			szOnlineTileFilePos[siIndex] = TempCount;

			return &ImageBaseTileData[TempCount].m_XSPR;
		}
	}
	for(TempCount = 0; TempCount < MAX_ONLINE_TILE_EXTDATAFILE; TempCount++)
	{
		if(ImageExtTileData[TempCount].m_TileIndex == -1)
		{
			ImageExtTileData[TempCount].m_TileIndex    = siIndex;
			ImageExtTileData[TempCount].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;
			sprintf((char*)m_TempBuffer, "%sOnline\\Tile\\%s", SavePath, szOnlineTileFile[siIndex]);
			if(clGrp.LoadXspr((char*)m_TempBuffer, ImageExtTileData[TempCount].m_XSPR) == FALSE)
			{
				if(fLargeTile) clGrp.Error("Not Load Large Tile", "siIndex = %d, siFileName = %s", siIndex, (char*)szOnlineTileFile[siIndex]);
				else           clGrp.Error("Not Load Tile",       "siIndex = %d, siFileName = %s", siIndex, (char*)szOnlineTileFile[siIndex]);

				return NULL;
			}

			szOnlineTileFilePos[siIndex] = TempCount + 256;

			return &ImageExtTileData[TempCount].m_XSPR;
		}
	}

	clGrp.Error("Not Find Empty Tile Buffer", "siIndex = %d", siIndex);

	return NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(오브젝트용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprObject(SI16 siIndex)
{	
	if(siIndex == -1) return NULL;

	SI32		TempCount;
	SI32		TempCount1;

	// 현재 로딩된 Data가 있는지 검사
	if(szOnlineObjectFilePos[siIndex] != -1)
	{
		if(szOnlineObjectFilePos[siIndex] < 256)
		{
			TempCount1 = szOnlineObjectFilePos[siIndex];
			ImageBaseObjectData[TempCount1].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;

			return &ImageBaseObjectData[TempCount1].m_XSPR;
		}
		else
		{
			TempCount1 = szOnlineObjectFilePos[siIndex] - 256;
			ImageExtObjectData[TempCount1].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;

			return &ImageExtObjectData[TempCount1].m_XSPR;
		}
	}

	// 만약 존재하지 않으면, 화일을 로딩한다.
	BOOL		fLargeObject;

	fLargeObject = FALSE;
	for(TempCount = 0; TempCount < MAX_ONLINE_OBJECT_BASEDATAFILE; TempCount++)
	{
		if(ImageBaseObjectData[TempCount].m_TileIndex == -1)
		{
			ImageBaseObjectData[TempCount].m_TileIndex    = siIndex;
			ImageBaseObjectData[TempCount].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;
			ImageBaseObjectData[TempCount].m_XSPR.Image   = ImageBaseObjectData[TempCount].m_ImageBuffer;

			sprintf((char*)m_TempBuffer, "%s%s", SavePath, szOnlineObjectFile[siIndex]);
			if(clGrp.LoadXsprInAllocMem((char*)m_TempBuffer, ImageBaseObjectData[TempCount].m_XSPR, MAX_ONLINE_OBJECTDATA_IMAGESIZE) == FALSE)
			{
				ImageBaseObjectData[TempCount].m_TileIndex    = -1;
				ImageBaseObjectData[TempCount].m_LastUseCount = 0;

				fLargeObject = TRUE;
				break;
			}

			szOnlineObjectFilePos[siIndex] = TempCount;

			return &ImageBaseObjectData[TempCount].m_XSPR;
		}
	}
	for(TempCount = 0; TempCount < MAX_ONLINE_OBJECT_EXTDATAFILE; TempCount++)
	{
		if(ImageExtObjectData[TempCount].m_TileIndex == -1)
		{
			ImageExtObjectData[TempCount].m_TileIndex    = siIndex;
			ImageExtObjectData[TempCount].m_LastUseCount = MAX_ONLINE_DATA_DELETCOUNT;
			sprintf((char*)m_TempBuffer, "%s%s", SavePath, szOnlineObjectFile[siIndex]);
			if(clGrp.LoadXspr((char*)m_TempBuffer, ImageExtObjectData[TempCount].m_XSPR) == FALSE)
			{
				if(fLargeObject) clGrp.Error("Not Load Large Object", "siIndex = %d, siFileName = %s", siIndex, (char*)szOnlineObjectFile[siIndex]);
				else             clGrp.Error("Not Load Object",       "siIndex = %d, siFileName = %s", siIndex, (char*)szOnlineObjectFile[siIndex]);

				return NULL;
			}

			szOnlineObjectFilePos[siIndex] = TempCount + 256;

			return &ImageExtObjectData[TempCount].m_XSPR;
		}
	}

	clGrp.Error("Not Find Empty Object Buffer", "siIndex = %d", siIndex);

	return NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(이모티콘용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprImoticon(SI16 siIndex)
{			 
	return	&xsprOnlineImoticon[siIndex];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(Item용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprItem(SI16 siIndex)
{			 
	return	&xsprOnlineItem[siIndex];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(깃발(아이디 앞에 붙는것)용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprFlagM(SI16 siIndex)
{			 
	return	&xsprOnlineFlagM[siIndex];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(깃발(마을밖에서)용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprFlagV(SI16 siIndex)
{			 
	return	&xsprOnlineFlagV[siIndex];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(마을 효과용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprVillageEffect(SI16 siSize, SI16 siPercent)
{
	return	&xsprOnlineVillageEffect[0][siSize][siPercent];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을이 불탈때 불타는 위치
//----------------------------------------------------------------------------------------------------------------
OnlineResource_VillageFireData*	OnlineResource::GetVillageFirePosition(SI16 VillageSize, SI16 State, SI16 Per)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < siVillageFirePositionCount; TempCount++)
	{
		if(sVillageFirePosition[TempCount].m_VillageSize == VillageSize)
		{
			if(sVillageFirePosition[TempCount].m_VillageState == State)
			{
				if(sVillageFirePosition[TempCount].m_VillageFirePer == Per)
				{
					return &sVillageFirePosition[TempCount];
				}
			}
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	private(마을 공격 효과용 스프라이트)에 접근할 수 있는 함수를 만든다.
//----------------------------------------------------------------------------------------------------------------
XSPR	*OnlineResource::GetxsprVillageAttackEffect(SI16 Type)
{
	return	&xsprOnlineVillageAttackEffect[Type];
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	케릭터 스프라이트 파일을 찾아서 로드함
//----------------------------------------------------------------------------------------------------------------
VOID OnlineResource::LoadCharSpr( char *pDirectory, char *pPolder )
{
	WIN32_FIND_DATA		fd;
	HANDLE				hFilesHandle;
	CHAR				szDirectory[1024];	
	CHAR				szMainDirectory[100];

	BYTE				*pMemory = NULL;
	UINT				Byte = 0;	
	UINT				ix   = 0;

	
	wsprintf( szDirectory, "Online\\%s\\*.*", pDirectory);
	wsprintf( szMainDirectory, "Online\\%s\\", pPolder );

	hFilesHandle =	FindFirstFile(szDirectory, &fd);

	if(hFilesHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 디렉토리이다.

				if(strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
				{
					wsprintf(szDirectory, "%s%s\\", pDirectory, fd.cFileName);
					LoadCharSpr(szDirectory, pPolder);
				}

			}
			else
			{
				//if( ( ( strstr( fd.cFileName , "Spr" ) ) != NULL ) && ( strstr( szDirectory, pPolder ) != NULL ) )
				if( ( ( stricmp( &fd.cFileName[strlen(fd.cFileName) - 4] , ".Spr" ) ) == 0 ) && ( strstr( szDirectory, pPolder ) != NULL ) )
				{								
					if( ( stricmp( pPolder, "Object" ) ) == 0 )
					{	
						sprintf( szOnlineObjectFile[ siCount ], "%s%s", szMainDirectory, fd.cFileName );
					}
					siCount++;
				}
			}

		}while(FindNextFile(hFilesHandle, &fd));

		FindClose(hFilesHandle);
	}
}
//--------------------------------------------------------------------------------
// Name: AddItemSpr
// Desc: 아이템을 로드함(초기에 로드하지 않고 추가로 로드 할수 있음)
//--------------------------------------------------------------------------------
SI32	OnlineResource::AddItemSpr( char *pFileName )
{
	SI16	i;

	for( i=0 ; i<MAX_ONLINE_OBJECT_FILE ; i++)
	{
		if( szOnlineItemFile[i][0] == NULL )											// 파일 이름중 비어 있는것을 얻어 온다.
		{				
			wsprintf( szOnlineItemFile[i], "Online\\Item\\%s", pFileName );
			clGrp.LoadXspr( szOnlineItemFile[i], xsprOnlineItem[i] );
			return i;
		}

	}
	return -1;
}
//----------------------------------------------------------------------------------------------------------------
// 정환이꺼
//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Name: RecursScanDirectory()
// Desc: 디렉토리를 스캔해서 데이터를 읽어온다.; 재귀 함수
//--------------------------------------------------------------------------------
BOOL	OnlineResource::LoadInformationDirectoryRecurs(char * pCurPath, SI32 siKind)
{
	//---------------------------------------------------------------
	// 폴더 이름 추출&처리
	//---------------------------------------------------------------
	SI16 bCorrectname;
	char cString[256];
	char cFoldername[256];
	char cSeps[] = "\\";
	char * pToken;

	strcpy(cString, (LPSTR)(LPCSTR)pCurPath);
	ZeroMemory(cFoldername, sizeof(cFoldername));

	pToken = strtok( cString, cSeps );
	bCorrectname = 0;
	while( pToken != NULL )
	{
		pToken = strtok( NULL, cSeps );

		if( pToken == NULL )
		{
			// 만약 해당되는 코드 길이의 폴더 라면
			if( strlen(cFoldername) == 2 )
			{
				// 옳바른 이름이라는 프래그를 TRUE
				bCorrectname = 1;
			}
			// 만약 해당되는 코드 길이의 확장형 폴더 라면
			if( strlen(cFoldername) == 3 )
			{
				// 옳바른 이름이라는 프래그를 TRUE
				bCorrectname = 2;
			}
			break;
		}

		// 전체 경로명이 아닌 현재 폴더명만 축출한다
		strcpy(cFoldername, pToken);
	}

	//---------------------------------------------------------------
	// 폴더 검색 재귀
	//---------------------------------------------------------------
	char cFindfilename[256];
	char cNewpathname[256];
	
	HANDLE	hFind;
	SI32 siCount = 0;
	WIN32_FIND_DATA lpfd;
	ZeroMemory(&lpfd, sizeof(LPWIN32_FIND_DATA));

	// 현재 경로의 모든 화일들을 검사한다.

	strcpy(cFindfilename, pCurPath);
	strcat(cFindfilename, "\\*.*");

	// 화일 검색
	hFind = FindFirstFile(cFindfilename, &lpfd);

	if( hFind == INVALID_HANDLE_VALUE ) return FALSE;

	while( 1 )
	{
		if(lpfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// . 디렉토리는 넘긴다
			if( strstr(lpfd.cFileName, ".") == 0 )
			{
				strcpy(cNewpathname, pCurPath);
				strcat(cNewpathname, "\\");
				strcat(cNewpathname, lpfd.cFileName);

				// 하위 디렉토리 재귀
				LoadInformationDirectoryRecurs(cNewpathname, siKind);
			}
		}
		else if(lpfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{                
			if( bCorrectname != 0 )
			{
				//----------------------------------------------------------------------------
				// 코드 변환
				//----------------------------------------------------------------------------
				SI32 siHUpper, siUpper, siLower, siSpriteCode;

				siHUpper = siUpper = siLower = 0;
				if(bCorrectname == 1)
				{
					if((cFoldername[0]) >= 0x30 && (cFoldername[0] <= 0x39)) siUpper = cFoldername[0] - 0x30;
					if((cFoldername[0]) >= 0x41 && (cFoldername[0] <= 0x5A)) siUpper = cFoldername[0] - 0x30 - 7;
					if((cFoldername[0]) >= 0x61 && (cFoldername[0] <= 0x7A)) siUpper = cFoldername[0] - 0x50 - 7;
					if((cFoldername[1]) >= 0x30 && (cFoldername[1] <= 0x39)) siLower = cFoldername[1] - 0x30;
					if((cFoldername[1]) >= 0x41 && (cFoldername[1] <= 0x5A)) siLower = cFoldername[1] - 0x30 - 7;
					if((cFoldername[1]) >= 0x61 && (cFoldername[1] <= 0x7A)) siLower = cFoldername[1] - 0x50 - 7;
				}
				else
				{
					if((cFoldername[0]) >= 0x30 && (cFoldername[0] <= 0x39)) siHUpper = cFoldername[0] - 0x30;
					if((cFoldername[0]) >= 0x41 && (cFoldername[0] <= 0x5A)) siHUpper = cFoldername[0] - 0x30 - 7;
					if((cFoldername[0]) >= 0x61 && (cFoldername[0] <= 0x7A)) siHUpper = cFoldername[0] - 0x50 - 7;
					if((cFoldername[1]) >= 0x30 && (cFoldername[1] <= 0x39)) siUpper  = cFoldername[1] - 0x30;
					if((cFoldername[1]) >= 0x41 && (cFoldername[1] <= 0x5A)) siUpper  = cFoldername[1] - 0x30 - 7;
					if((cFoldername[1]) >= 0x61 && (cFoldername[1] <= 0x7A)) siUpper  = cFoldername[1] - 0x50 - 7;
					if((cFoldername[2]) >= 0x30 && (cFoldername[2] <= 0x39)) siLower  = cFoldername[2] - 0x30;
					if((cFoldername[2]) >= 0x41 && (cFoldername[2] <= 0x5A)) siLower  = cFoldername[2] - 0x30 - 7;
					if((cFoldername[2]) >= 0x61 && (cFoldername[2] <= 0x7A)) siLower  = cFoldername[2] - 0x50 - 7;
				}

				siSpriteCode = (siHUpper * 36 * 36) + (siUpper * 36) + siLower;

				//----------------------------------------------------------------------------
				// 확장자가 SPR인지 검사한다
				//----------------------------------------------------------------------------
				char cCoffilename[256];
				char cXSprFileName[256];
				char cSeps[] = ".";
				char * pToken;

				strcpy(cXSprFileName, lpfd.cFileName);

				pToken = strtok( cXSprFileName, cSeps );
				strcpy(cCoffilename, pToken);
				strcat(cCoffilename, ".COF");

				pToken = strtok( NULL, cSeps );
				if( stricmp(pToken, "SPR") == 0 )
				{
					char cSPRfullpathname[256];

					switch( siKind )
					{
					case ON_LOAD_KIND_TI:
						// SPR과 COF의 풀패스명을 만든다
						strcpy(cSPRfullpathname, cFoldername);
						strcat(cSPRfullpathname, "\\");
						strcat(cSPRfullpathname, lpfd.cFileName);

						// 이름 저장
						ZeroMemory(szOnlineTileFile[siSpriteCode], 128);
						strcpy(szOnlineTileFile[siSpriteCode], cSPRfullpathname);
						break;
					}
				}
			}
		}

		if( FindNextFile(hFind, &lpfd) == 0 ) break;
	}

	FindClose(hFind);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Prepare
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineResource::Prepare(void)
{
	SI32		TempCount;

	// 현재 로딩된 Tile중 삭제 돼어야 할 게 있는지 검사
	for(TempCount = 0; TempCount < MAX_ONLINE_TILE_BASEDATAFILE; TempCount++)
	{
		if(ImageBaseTileData[TempCount].m_TileIndex != -1)
		{
			ImageBaseTileData[TempCount].m_LastUseCount--;
			if(ImageBaseTileData[TempCount].m_LastUseCount <= 0)
			{
				szOnlineTileFilePos[ImageBaseTileData[TempCount].m_TileIndex] = -1;
				ImageBaseTileData[TempCount].m_YetTileIndex = ImageBaseTileData[TempCount].m_TileIndex;
				ImageBaseTileData[TempCount].m_TileIndex = -1;
			}
		}
	}
	for(TempCount = 0; TempCount < MAX_ONLINE_TILE_EXTDATAFILE; TempCount++)
	{
		if(ImageExtTileData[TempCount].m_TileIndex != -1)
		{
			ImageExtTileData[TempCount].m_LastUseCount--;
			if(ImageExtTileData[TempCount].m_LastUseCount <= 0)
			{
				szOnlineTileFilePos[ImageExtTileData[TempCount].m_TileIndex] = -1;
				ImageExtTileData[TempCount].m_YetTileIndex = ImageExtTileData[TempCount].m_TileIndex;
				ImageExtTileData[TempCount].m_TileIndex = -1;
				clGrp.FreeXspr(ImageExtTileData[TempCount].m_XSPR);
			}
		}
	}

	// 현재 로딩된 Object중 삭제 돼어야 할 게 있는지 검사
	for(TempCount = 0; TempCount < MAX_ONLINE_OBJECT_BASEDATAFILE; TempCount++)
	{
		if(ImageBaseObjectData[TempCount].m_TileIndex != -1)
		{
			ImageBaseObjectData[TempCount].m_LastUseCount--;
			if(ImageBaseObjectData[TempCount].m_LastUseCount <= 0)
			{
				szOnlineObjectFilePos[ImageBaseObjectData[TempCount].m_TileIndex] = -1;
				ImageBaseObjectData[TempCount].m_YetTileIndex = ImageBaseObjectData[TempCount].m_TileIndex;
				ImageBaseObjectData[TempCount].m_TileIndex = -1;
			}
		}
	}
	for(TempCount = 0; TempCount < MAX_ONLINE_OBJECT_EXTDATAFILE; TempCount++)
	{
		if(ImageExtObjectData[TempCount].m_TileIndex != -1)
		{
			ImageExtObjectData[TempCount].m_LastUseCount--;
			if(ImageExtObjectData[TempCount].m_LastUseCount <= 0)
			{
				szOnlineObjectFilePos[ImageExtObjectData[TempCount].m_TileIndex] = -1;
				ImageExtObjectData[TempCount].m_YetTileIndex = ImageExtObjectData[TempCount].m_TileIndex;
				ImageExtObjectData[TempCount].m_TileIndex = -1;
				clGrp.FreeXspr(ImageExtObjectData[TempCount].m_XSPR);
			}
		}
	}

	return TRUE;
}

// robypark 2004/12/28 17:13
// desc: 마을 그릴 때 사용되는 이미지들 포지션 정보 파일 HQ에서 읽기
BOOL	OnlineResource::LoadVillageImagePositionInfoFile(void)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char	pszFileName[] = "Online\\Village\\VillageImagePositionInfo.txt";

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "LoadVillageImagePositionInfoFile Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 마을 이미지 위치 정보 구조체 변수에 메모리 해제
	if (m_lpsVillageImagePositionInfo)
	{
		delete []m_lpsVillageImagePositionInfo;
		m_lpsVillageImagePositionInfo = NULL;

		m_ssCntVillageImagePositionInfo = 0;
	}

	// 마을 이미지 위치 정보 구조체 변수에 메모리 할당
	m_ssCntVillageImagePositionInfo = ocp.GetMaxDataLineCount();	// 데이터 개수
	m_lpsVillageImagePositionInfo = new OnlineResource_VillageImagePositionInfo[m_ssCntVillageImagePositionInfo];

	SI32	siArrayIdx = 0;		// 배열 인덱스

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		char	szVillageState[32];
		SI32	ssVillageSize, ssVillageGuildFlagPosX, ssVillageGuildFlagPosY, ssSurrenderFlagPosX, ssSurrenderFlagPosY, 
					ssVillageDrawCenterX, ssVillageDrawCenterY, ssVillageGarrisionGaugePosX, ssVillageGarrisionGaugePosY, 
					ssVillageDefenceGaugePosX, ssVillageDefenceGaugePosY, ssEnabledOccupyVillagePosX, ssEnabledOccupyVillagePosY;

		// 자료를 순차적으로 받는다.
		ocp.GetValue(szVillageState,	31,				iLineNo, iColNo);	// 마을의 국가코드
		ocp.GetValue(ssVillageSize,						iLineNo, iColNo);	// 마을의 Size(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)

		// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
		ocp.GetValue(ssVillageGuildFlagPosX,			iLineNo, iColNo);	// 상단 깃발 위치
		ocp.GetValue(ssVillageGuildFlagPosY,			iLineNo, iColNo);	// 상단 깃발 위치

		// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
		ocp.GetValue(ssSurrenderFlagPosX,				iLineNo, iColNo);	// 마을 점령 깃발 위치
		ocp.GetValue(ssSurrenderFlagPosY,				iLineNo, iColNo);	// 마을 점령 깃발 위치

		// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
		ocp.GetValue(ssVillageDrawCenterX,				iLineNo, iColNo);	// 마을이 그려지는 중점좌표
		ocp.GetValue(ssVillageDrawCenterY,				iLineNo, iColNo);	// 마을이 그려지는 중점좌표

		// 마을 중심(변환된 dot)에서 위로 떨어진 거리에 위치하는 값
		ocp.GetValue(ssVillageGarrisionGaugePosX,		iLineNo, iColNo);	// 마을 주둔군(수비병) 표시 게이지 위치
		ocp.GetValue(ssVillageGarrisionGaugePosY,		iLineNo, iColNo);	// 마을 주둔군(수비병) 표시 게이지 위치
		ocp.GetValue(ssVillageDefenceGaugePosX,			iLineNo, iColNo);	// 마을 방어력 표시 게이지 위치
		ocp.GetValue(ssVillageDefenceGaugePosY,			iLineNo, iColNo);	// 마을 방어력 표시 게이지 위치
		ocp.GetValue(ssEnabledOccupyVillagePosX,		iLineNo, iColNo);	// 수비병으로 공격하여 마을 점령을 하라는 이미지 위치
		ocp.GetValue(ssEnabledOccupyVillagePosY,		iLineNo, iColNo);	// 수비병으로 공격하여 마을 점령을 하라는 이미지 위치

		// 국가 코드 설정
		if (strcmp(szVillageState, "Korea") == 0)		// 조선
			m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageState = ON_COUNTRY_KOREA;
		else if (strcmp(szVillageState, "Japan") == 0)		// 일본
			m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageState = ON_COUNTRY_JAPAN;
		else if (strcmp(szVillageState, "China") == 0)		// 중국
			m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageState = ON_COUNTRY_CHINA;
		else if (strcmp(szVillageState, "Taiwan") == 0)		// 대만
			m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageState = ON_COUNTRY_TAIWAN;
		else
			m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageState = ON_COUNTRY_NONE;

		// 데이터 셑팅
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageSize				= ssVillageSize;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageGuildFlagPosX		= ssVillageGuildFlagPosX;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageGuildFlagPosY		= ssVillageGuildFlagPosY;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssSurrenderFlagPosX			= ssSurrenderFlagPosX;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssSurrenderFlagPosY			= ssSurrenderFlagPosY;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageDrawCenterX		= ssVillageDrawCenterX;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageDrawCenterY		= ssVillageDrawCenterY;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageGarrisionGaugePosX	= ssVillageGarrisionGaugePosX;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageGarrisionGaugePosY	= ssVillageGarrisionGaugePosY;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageDefenceGaugePosX	= ssVillageDefenceGaugePosX;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssVillageDefenceGaugePosY	= ssVillageDefenceGaugePosY;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssEnabledOccupyVillagePosX	= ssEnabledOccupyVillagePosX;
		m_lpsVillageImagePositionInfo[siArrayIdx].m_ssEnabledOccupyVillagePosY	= ssEnabledOccupyVillagePosY;

		// 배열 인덱스 증가
		siArrayIdx++;

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

//----------------------------------------------------------------------------------------------
// robypark 2004/12/28 17:13
// desc: 마을 그릴 때 사용되는 이미지들 파일명 및 마을 크기 얻기
// parameter:
// SI16	ssVillageState[IN]: 마을의 국가코드
// char	*pszVillageSizeImageFilename[IN]: 마을의 크기를 나타내는 이미지 파일 명(town49.spr, town25.spr, town9.spr)
// SI16	&ssVillageSize[OUT]: 마을의 크기값(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)
// char *pszVillageImageFilename[OUT]: 마을 이미지 파일명. 마을 방어력이 70% 이상일 경우
// char *pszBrokenVillageImageFilename[OUT]: 부서진 마을 이미지 파일명. 마을 방어력이 70%미만일 경우
//----------------------------------------------------------------------------------------------
void	OnlineResource::GetVillageImageFilename(SI16 ssVillageState, char *pszVillageSizeImageFilename, SI16 &ssVillageSize,
													char *pszVillageImageFilename, char *pszBrokenVillageImageFilename)
{
	// 마을 크기 결정
	ssVillageSize = GetVillageSize(pszVillageSizeImageFilename);

	// 마을 이미지 파일 설정
	switch (ssVillageState)
	{
		case ON_COUNTRY_KOREA:
			sprintf((char*)pszVillageImageFilename,       "K%s",  pszVillageSizeImageFilename);
			sprintf((char*)pszBrokenVillageImageFilename, "KH%s", pszVillageSizeImageFilename);
			break;

		case ON_COUNTRY_JAPAN:
			sprintf((char*)pszVillageImageFilename,       "J%s",  pszVillageSizeImageFilename);
			sprintf((char*)pszBrokenVillageImageFilename, "JH%s", pszVillageSizeImageFilename);
			break;

		case ON_COUNTRY_CHINA:
			sprintf((char*)pszVillageImageFilename,       "C%s",  pszVillageSizeImageFilename);
			sprintf((char*)pszBrokenVillageImageFilename, "CH%s", pszVillageSizeImageFilename);
			break;

		case ON_COUNTRY_TAIWAN:
			sprintf((char*)pszVillageImageFilename,       "T%s",  pszVillageSizeImageFilename);
			sprintf((char*)pszBrokenVillageImageFilename, "TH%s", pszVillageSizeImageFilename);
			break;
		default:
			sprintf((char*)pszVillageImageFilename,       "%s",  pszVillageSizeImageFilename);
			sprintf((char*)pszBrokenVillageImageFilename, "%s", pszVillageSizeImageFilename);
			break;
	}
}

//----------------------------------------------------------------------------------------------
// robypark 2004/12/28 17:13
// desc: 마을 그릴 때 사용되는 이미지들 포지션 정보 헤더 얻기
// parameter:
// SI16	ssVillageState[IN]: 마을의 국가코드
// SI16	ssVillageSize[IN]: 마을의 크기값(0 : 큰마을, 1 : 중간마을, 2 : 작은마을)
//----------------------------------------------------------------------------------------------
LPOnlineResource_VillageImagePositionInfo	OnlineResource::GetVillageImagePositionInfoHeader(SI16 ssVillageState, SI16 ssVillageSize)
{
	for (SI16 i = 0; i < m_ssCntVillageImagePositionInfo; i++)
	{
		if (m_lpsVillageImagePositionInfo[i].m_ssVillageState == ssVillageState)	// 국가 코드
		{
			if (m_lpsVillageImagePositionInfo[i].m_ssVillageSize == ssVillageSize)	// 마을 크기
			{
				return &m_lpsVillageImagePositionInfo[i];
			}
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------------------------
// robypark 2004/12/29 17:26
// desc: 마을 크기 얻기
// parameter:
// char	*pszVillageSizeImageFilename[IN]: 마을의 크기를 나타내는 이미지 파일 명(town49.spr, town25.spr, town9.spr)
//----------------------------------------------------------------------------------------------
SI16	OnlineResource::GetVillageSize(char *pszVillageSizeImageFilename)
{
	SI16 ssVillageSize;

	// 마을 크기 결정
	if(strcmp(pszVillageSizeImageFilename, "town49.Spr") == 0)		// 큰
		ssVillageSize = 0;
	else if(strcmp(pszVillageSizeImageFilename, "town25.Spr") == 0)	// 중간
		ssVillageSize = 1;
	else if(strcmp(pszVillageSizeImageFilename, "town9.Spr") == 0)	// 작은
		ssVillageSize = 2;
	else
		ssVillageSize = 0;

	return ssVillageSize;
}
