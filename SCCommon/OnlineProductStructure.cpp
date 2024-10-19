#include <GSL.h>

#include	"OnlineProductStructure.h"
#include	"OnlineVillage-Parser.h"
#include	"BindJxFile.h"

// 생성자
OnlineBuilding::OnlineBuilding()
{
}

// 소멸자
OnlineBuilding::~OnlineBuilding()
{
	if( pPlants )
	{
		delete [] pPlants;
		pPlants		=	NULL;
	}
	if( pFacility )
	{
		delete [] pFacility;
		pFacility	=	NULL;
	}
	if( pEquipment )
	{
		delete [] pEquipment;
		pEquipment	=	NULL;
	}
}

BOOL	OnlineBuilding::Init()
{
	
	FILE	*fp;
	CHAR	Buf[2048];
	CHAR	*pToken = NULL;
	CHAR	seps[] = "\" '\t\n\r";
	SI32	nFileCount=0, i,j, ix;
	SI32	Temp=0,Num=0;
	const char	*szFileName[]	=	{"Online\\Village\\Factory.txt",
									"Online\\Village\\Facility.txt",
									"Online\\Village\\FactoryUpgrade.txt", NULL};

	while(szFileName[nFileCount])
	{
		fp = BindJXFile::GetInstance()->GetFile( szFileName[nFileCount] );
		if(fp == NULL)		return	FALSE;
		
		i	=	0;
		Temp=	0;
		Num	=	0;		
		
		// 앞부분 제외하고 Code만 읽는다.
		while( fgets(Buf, sizeof(Buf), fp) )
		{
			pToken	=	strtok(Buf, seps);
			if(pToken == NULL)
				continue;
			if( stricmp(pToken, "Code") == 0 )
			{
				Temp = ftell(fp);
				while( fgets(Buf, sizeof(Buf), fp) )
				{
					pToken = strtok(Buf, seps);
					if(pToken)
						Num++;
				}
				break;
			}
		}
		if( Temp == 0 )
		{
			//printf("Temp가 0이다., Code를 못 찾았다.[%s]\n",Buf);
			return FALSE;
		}

		fseek(fp, Temp, SEEK_SET );
		switch(nFileCount)
		{
		case 0:
			pPlants		=	new stPlants[Num];
			memset(pPlants, 0, sizeof(stPlants)*Num );
			PlantsNum	=	Num;
			//printf("pPlants		=	new stPlants[%d]\n",Num);
			break;
		case 1:
			pFacility	=	new stFacility[Num];
			memset(pFacility, 0, sizeof(stFacility)*Num );
			FacilNum	=	Num;
			//printf("pFacility	=	new stFacility[%d]\n",Num);
			break;
		case 2:
			pEquipment	=	new stEquipment[Num];
			memset(pEquipment, 0, sizeof(stEquipment)*Num);
			EquipNum	=	Num;
			//printf("pEquipment	=	new stEquipment[%d]\n",Num);
			break;
		default:
			//printf("nFileCount가 이상하다.[%d]\n");
			return FALSE;
		}

		//	이제부터 내용이다.
		for( int i=0; i<Num; i++ )
		{
			fgets(Buf, sizeof(Buf), fp);

			//printf("%s\n",Buf);
			pToken = strtok(Buf, seps);
			if(pToken == NULL)
				break;
			switch(nFileCount)
			{
			case	0:	//	Factory.txt 일 경우.
				strcpy(pPlants[i].Code, pToken );									//	Code : FAM
				//printf("[%s][%s][%d]\n",pToken,pPlants[i].Code,pPlants[i].uiNumCode);
				//getchar();

				if( strcmp(pToken, ON_VIL_ST_KIND_CODE_FARM) == 0 )				pPlants[i].DBIndex	= ON_PLANT_FARM;
				else if( strcmp(pToken, ON_VIL_ST_KIND_CODE_WEAPON) == 0 )		pPlants[i].DBIndex	= ON_PLANT_WEAPON;
				else if( strcmp(pToken, ON_VIL_ST_KIND_CODE_ARMOR) == 0 )		pPlants[i].DBIndex	= ON_PLANT_ARMOR;
				else if( strcmp(pToken, ON_VIL_ST_KIND_CODE_PASTURE) == 0 )		pPlants[i].DBIndex	= ON_PLANT_RANCH;
				else if( strcmp(pToken, ON_VIL_ST_KIND_CODE_FACTORY) == 0 )		pPlants[i].DBIndex	= ON_PLANT_FACTORY;
				else if( strcmp(pToken, ON_VIL_ST_KIND_CODE_LUMBERING) == 0 )	pPlants[i].DBIndex	= ON_PLANT_LUMBERING;
				else if( strcmp(pToken, ON_VIL_ST_KIND_CODE_MINE) == 0 )		pPlants[i].DBIndex	= ON_PLANT_MINE;

				pToken = strtok( NULL, seps );										//	Desc : 농장
				//printf("[%s]\n",pToken);
				pToken = strtok( NULL, seps );										//	Text
				pPlants[i].siTextCode = atoi( pToken );
				pToken = strtok( NULL, seps );										//	Cost : 100000
				pPlants[i].uiCost		=	atoi(pToken);
				//printf("앗?? %s\n",pToken);
				pToken = strtok( NULL, seps );										//	Slot Num : 6
				pPlants[i].uiMaxSlot	=	atoi(pToken);
				//printf("[%s][%d]\n",pToken,pPlants[i].uiMaxSlot);

				pToken				=	strtok( NULL, seps );						//	Available Slot Num : 4
				pPlants[i].uiInitSlot	=	atoi(pToken);
				pPlants[i].siBuildNum	=	0;				

				for( ix=0; ix<FACILITY_CODE_NUM; ix++ )
				{
					pToken = strtok( NULL, seps );
					if(pToken[0] != '0' )
					{
						strcpy(pPlants[i].FCode[pPlants[i].siBuildNum], pToken );
						pPlants[i].siBuildNum++;
						//printf("%s",pToken);
					}
				}

				pToken				=	strtok(NULL, seps);							//	Req. Credit
				pPlants[i].siReqCredit=	atoi(pToken);
				
				//printf("Plants[%d] : Code=%s, FCode[0]=%s, FCode[5]=%s\n",
				//	i,pPlants[i].Code,pPlants[i].FCode[0],pPlants[i].FCode[5]);

				//i++;
				//getchar();
				break;

			case	1:	//	Facility.txt 일 경우.				
				strcpy(pFacility[i].Code, pToken );						//	Code : FAM
				//printf("0. %s %d\n",pToken,pFacility[i].uiNumCode);
				//getchar();
				pToken = strtok( NULL, seps );							//	Name Code : 00100020
				pFacility[i].siTextCode	=	atoi( pToken );
				pToken = strtok( NULL, seps );							//	Desc : 과수원
				memcpy(pFacility[i].Name, pToken, sizeof(pFacility[i].Name));
				pToken = strtok( NULL, seps );							//	Sprite Font
				pFacility[i].siSpriteFont = atoi( pToken );

				// 조선
				j=0;
				do{
					pToken = strtok( NULL, seps );						//	Pr01 : 00011
					pFacility[i].uiPr[ON_COUNTRY_KOREA-1][j]	=	atoi(pToken);
				//	printf("%d - %s\n",j,pToken);
					j++;
				}while(j<PRODUCT_CODE_NUM);
				//printf("pFacility[%d] : uiPr[0] =%u, uiPr[9]=%u, %s\n",i,pFacility[i].uiPr[0],pFacility[i].uiPr[9],pToken);

				// 일본
				j=0;
				do{
					pToken = strtok( NULL, seps );						//	Pr01 : 00011
					pFacility[i].uiPr[ON_COUNTRY_JAPAN-1][j]	=	atoi(pToken);
					j++;
				}while(j<PRODUCT_CODE_NUM);

				// 명
				j=0;
				do{
					pToken = strtok( NULL, seps );						//	Pr01 : 00011
					pFacility[i].uiPr[ON_COUNTRY_CHINA-1][j]	=	atoi(pToken);
					pFacility[i].uiPr[ON_COUNTRY_TAIWAN-1][j]	=	atoi(pToken);
					j++;
				}while(j<PRODUCT_CODE_NUM);


				pFacility[i].siUpCodeNum = 0;
				for( ix=0 ; ix<UPGRADE_CODE_NUM; ix++ )
				{
					pToken = strtok( NULL, seps );
					if( pToken[0] != '0' )
					{
						strcpy(pFacility[i].UpCode[pFacility[i].siUpCodeNum], pToken );				
						pFacility[i].siUpCodeNum++;
					}
				}
				j=0;
				do{
					pToken = strtok( NULL, seps );
					pFacility[i].UpLabor[j]  = atoi(pToken);
					//printf("%d - %s\n",j,pToken);
					j++;
				}while(j<MAX_UPGRADESLOT_NUM);
				//printf("[pFacility[%d].Code = %s]\n",i,pFacility[i].Code);
				//printf("pFacility[%d] : Code = %s, uiPr[0] = %d, UpCode[0] = %s, UpCode[4] = %s\n",
					//i,pFacility[i].Code, pFacility[i].uiPr[0], pFacility[i].UpCode[0],pFacility[i].UpCode[4]);				
				//i++;
				//getchar();
				break;

			case	2:	//	FactoryUpgrade 일 경우.
				strcpy( pEquipment[i].Code, pToken );					//	Code : FNC
				pToken = strtok( NULL, seps );							//	Name Code : 00100020
				pEquipment[i].siTextCode = atoi( pToken );
				pToken = strtok( NULL, seps );							//	Desc : 울타리 설치
				pToken = strtok( NULL, seps );							//	Work : 300
				pEquipment[i].uiCost	=	atoi(pToken);
				pToken = strtok( NULL, seps );							//	Repeat Num : 5
				pEquipment[i].Repeat	=	atoi(pToken);
				j=0;
				do{
					pToken = strtok( NULL, seps );						//	Pr01 : 00011
					pEquipment[i].uiResist[j]	=	atoi(pToken);
					pToken = strtok( NULL, seps );
					pToken = strtok( NULL, seps );
					//printf("%d - %s\n",j,pToken);
					j++;
				}while(j<RESIST_KIND_NUM);

				//pToken = strtok( NULL, seps );							//	BeastResist: 5
				//pEquipment[i].uiBeastResist=	atoi(pToken);				
				//printf("pEquipment[%d] : Code = %s, uiCost = %u, Repeat =%u, uiBirdResist = %u);\n",
				//	i,pEquipment[i].Code,pEquipment[i].uiCost,pEquipment[i].Repeat, pEquipment[i].uiResist[1]);
				//i++;
				//getchar();
				break;
			default:
			//printf("nFileCount가 이상하다.[%d]\n");
				return FALSE;
			}
		}
		fclose(fp);		
		nFileCount++;
	}
	return TRUE;
}

stEquipment	*OnlineBuilding::GetEquipment( SI32 siIndex )
{
	return &pEquipment[ siIndex ];
}


stFacility	*OnlineBuilding::GetFacility( SI32 siIndex )
{
	return &pFacility[ siIndex ];
}

stPlants	*OnlineBuilding::GetPlants( SI32 siIndex )
{
	return &pPlants[ siIndex ];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	문자형 코드를 숫자형 코드로 변환한다.
//	수정 일자	:	2002-08-19 오후 1:00:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	OnlineBuilding::CodeToNum(CHAR *pCode)
{
	UI32	uiCode;

	uiCode		=	*(SI32*)pCode;
	
	return uiCode;
}

stEquipment	*OnlineBuilding::GetEquipment( CHAR	*pCode )
{
	UI32		uiCode	=	CodeToNum(pCode);
	for(int i=0; i<EquipNum; i++)
	{
		if(pEquipment[i].uiNumCode == uiCode)
			return &pEquipment[i];
	}
	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	문자열 코드를 이용하여 생산 시설물의 포인터를 얻어온다.
//	수정 일자	:	2002-08-19 오후 12:58:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stFacility	*OnlineBuilding::GetFacility( CHAR *pCode)
{
	SI32		i;
	UI32		uiCode	=	CodeToNum(pCode);

	for(i = 0; i < FacilNum; i++)
	{
		if(pFacility[i].uiNumCode == uiCode)
			return &pFacility[i];
	}

	return NULL;
}

stPlants	*OnlineBuilding::GetPlants( CHAR *pCode)
{
	UI32		uiCode	=	CodeToNum(pCode);
	for(int i=0; i<PlantsNum; i++)
	{
		if(pPlants[i].uiNumCode == uiCode)
			return &pPlants[i];
	}
	return NULL;
}


SI32	OnlineBuilding::GetIndex(CHAR *FaCode, UI16	uiItem, UI08 Level, UI08 uiNation)
{
	stFacility		*pstFacil;
	SI32			i, MaxNum;
	UI16			*pTemp;

	switch(Level)
	{
	case 1:
		MaxNum =	ON_FARM_LEV1_PRODUCT;
		break;
	case 2:
		MaxNum	=	ON_FARM_LEV2_PRODUCT;
		break;
	case 3:
		MaxNum	=	ON_FARM_LEV3_PRODUCT;
		break;
	default:
		return -1;
	}
	
	pstFacil	=	GetFacility(FaCode);
	if(pstFacil)
	{
		switch(uiNation)
		{
		case NATION_CODE_KOREA:
		case ON_COUNTRY_KOREA:
			pTemp = pstFacil->uiPr[0];
			break;
		case NATION_CODE_JAPAN:
		case ON_COUNTRY_JAPAN:
			pTemp = pstFacil->uiPr[1];
			break;
		case NATION_CODE_CHINA:
		case ON_COUNTRY_CHINA:
		case NATION_CODE_TAIWAN:
		case ON_COUNTRY_TAIWAN:
			pTemp = pstFacil->uiPr[2];
			break;
		default:
			return -1;
		}
		
		for(i=0; i< MaxNum; i++)
		{
			if(pTemp[i]== uiItem)
				return i;
		}
	}
	return -1;
}

stPlants	*OnlineBuilding::GetPlantsFromID( UI08 uiPlantID )	//0:농장, ...
{
	if( uiPlantID < PlantsNum)
		return &pPlants[uiPlantID];
	return NULL;
}
