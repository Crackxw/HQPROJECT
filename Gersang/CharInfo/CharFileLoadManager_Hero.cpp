//===================================================================================================================================================
//	FileName	:	CharFileLoadManager_Hero.cpp
//	History		:	2001/10/23(박연숙) - Create
//	Discription	:
//===================================================================================================================================================

#include <GSL.h>
#include "GSCDefine.h"
#include "CharFileLoadManager.h"
#include <debugtool.h>
#include <clGame.h>

#include "..\Gersang\Message.h"
#include "BIndJXFile.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	LoadHeroConfigInfo()
// Attribute	:	private func.
// Description	:	해당 폴더에서 COF파일을 열어서 Hero COF Structure를 설정한다.
//					(예) K MA A1 1HS U.cof

	
extern _clGame* pGame; 

BOOL CCharFileLoadManager::LoadHeroConfigInfo(SI32 siKindID)
{
	char					cFindCOFFileName[256];				// "......\COF\KMA\*.*"
	char					cCofFullPathName[256];				// "......\COF\KMA\KMAA11HSU.COF"
				
	SI32					siLineNo;
	SI32					siResult1, siResult2;
	SI32					siCount1, siCount2;

	HANDLE					hFind;
	WIN32_FIND_DATA			lpfd;
	FILE					*fp;
	char					cSeps[16];
	char					cNation[16], cSex[16], cClass[16], cAct[16];
	SI32					siClass, siAct;
	char					*pToken, *pTokenParam;
	SI32					siDir, siBodyPart;
	char					countrystr[10];			// 국가를 나타내는 문자. (조선:k, 일본:j, 명:c)

	char					cBuffer[1024];

	
	// 배열들을 초기화한다.
	ZeroMemory(&lpfd, sizeof(WIN32_FIND_DATA));
	memset(cBuffer, 0, sizeof(cBuffer));

 

	switch(siKindID)
	{
	case HERO_KM:	strcpy(countrystr, "KMA"); break;
	case HERO_KW:	strcpy(countrystr, "KWO"); break;

	case HERO_JM:	strcpy(countrystr, "JMA"); break;
	case HERO_JW:	strcpy(countrystr, "JWO"); break;

	case HERO_CM:	strcpy(countrystr, "CMA"); break;
	case HERO_CW:	strcpy(countrystr, "CWO"); break;

	case HERO_TM:	strcpy(countrystr, "TMA"); break;
	case HERO_TW:	strcpy(countrystr, "TWO"); break;
	default: clGrp.Error("fdsfsd973", "fsd9f27rjfsdf");break;

	}

	// 현재 경로의 모든 COF 파일들을 검사한다.
	wsprintf(cFindCOFFileName, ".\\Online\\JohapChar\\COF\\%s*.cof", countrystr);

	// 파일 검색
	hFind = FindFirstFile(cFindCOFFileName, &lpfd);

	if( hFind == INVALID_HANDLE_VALUE )		return FALSE;
	strcpy(cSeps, " \t\n\r");
	
	
	while( TRUE )
	{
		switch( lpfd.dwFileAttributes )
		{
		case FILE_ATTRIBUTE_ARCHIVE:

			// cof 파일명으로 히어로 코드를 분리 한다
			siResult1 = sscanf(lpfd.cFileName, "%1s%2s%2s%3s", cNation, cSex, cAct, cClass);

			// 에러 검출
			if( siResult1 != 4 ) { clGrp.Error("LoadHeroConfigInfo", "Invalid File Name [%s]", lpfd.cFileName); break; }

			// 히어로 코드를 얻어온다
			siAct	= GetHeroActCode(cAct);
			siClass = GetHeroClassCode(cClass);
			if( siAct	== -1 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid BodyAction Code on File [ %s ]", lpfd.cFileName); break; }
			if( siClass == -1 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid WeaponAction Code on File [ %s ]", lpfd.cFileName); break; }


			//-----------------------------------------------------------------
			// COF파일을 읽어 온다.
			//-----------------------------------------------------------------
			wsprintf(cCofFullPathName, "Online\\JohapChar\\COF\\%s", lpfd.cFileName);

			
			//fp = BindJXFile::GetInstance()->GetFile( cCofFullPathName );			
			fp = fopen( cCofFullPathName, "r" );
			if( fp == NULL )
			{
				clGrp.Error("No Cof file", cCofFullPathName);
				break;
			}


			// 이 캐릭터 그림의 FullPath를 설정한다.
			strcpy(m_HeroConfig[siKindID].cNation,		cNation);									// "K"
			strcpy(m_HeroConfig[siKindID].cSex,			cSex);										// "MA"


			//-------------------------
			// 먼저 Config의 헤더 부분을 읽어온다.
			siLineNo = 1;
			siResult1 = 0;
			for( siCount1=0; siCount1 < MAX_CFGHEADER; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				if( (pToken	= strtok(cBuffer, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				
				// 캐릭터 타입 검사
				if( stricmp(pToken, "TYPE") == 0 )
				{
					// COF화일명에서 알아낸 국적 코드와 실제 COF화일 내에 있는 국적 코드가 일치하는지 확인한다. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					if( stricmp(pTokenParam, cNation) != 0 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }

					// 캐릭터 성별의 숫자 코드를 기록한다. 
					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siSex = siResult1;
					
					// COF 화일명에서 알아낸 성별 코드와 실제 COF화일 내에 있는 성별 코드가 일치하는지 확인한다. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					if( stricmp(pTokenParam, cSex) != 0 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				}
				// 캐릭터 액션 검사
				else if( stricmp(pToken, "ACTION") == 0 )	
				{
					// COF 화일명에서 알아낸 액션 코드와 실제 COF 화일 내에 있는 액션 코드가 일치하는지 확인한다. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					siResult1 = GetHeroActCode(pTokenParam);
					if( siResult1 != siAct )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				}
				// 캐릭터 클래스 검사
				else if( stricmp(pToken, "CLASS") ==  0 )
				{
					//COF 화일명에서 알아낸 클래스 코드와 실제 COF화일 내에 있는 클래스 코드가 일치하는지 확인한다. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					siResult1 = GetHeroClassCode(pTokenParam);	
					// 용도는 특정 클래스이지만 실제로는 다른 클래스를 적용할 때가 있다.
					// 예를 들어 2HS클래스의 경우 죽을 때는 1HS 클래스의 데이터를 사용한다.
					// 이런 경우 화일 이름에는 2HS이 사용되지만 COF화일 내에서는 1HS이 사용된다. 
					// 따라서 COF화일의 데이터값과 화일명을 토대로 얻어낸 데이터가 다를 경우가 발생한다. 
					//if( siResult1 != siClass )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					if( siResult1 == -1 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				}
				// TOTALPIECE
				else if( stricmp(pToken, "TOTALPIECE") == 0 )
				{ 
					// 해당 액션의 해당 클래스가 전체 몇개의 조각으로 이루어진 것인지 저장한다. 
					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	
					{ 
						clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	
						fclose(fp); 
						return FALSE; 
					}
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siTotalPiece[siAct][siClass] = siResult1;				
				}
				// TOTALDIRECT
				else if( stricmp(pToken, "TOTALDIRECT") == 0 )
				{
					// 해당 액션의 해당 클래스가 몇 방향으로 이루어진 에니메이션인지 저장한다. 
					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siDirections[siAct][siClass] = siResult1;
				}
				// TOTALFRAME
				else if( stricmp(pToken, "TOTALFRAME") == 0 )
				{
					// 해당 액션의 해당 클래스 애니메이션이 몇개의 프레임으로 이루어진 것인지 저장한다. 
					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siTotalFrame[siAct][siClass][CHAR_SLOWSPEED]	= siResult1;

					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siTotalFrame[siAct][siClass][CHAR_NORMALSPEED]	= siResult1;

					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siTotalFrame[siAct][siClass][CHAR_FASTSPEED]	= siResult1;

					siResult1= 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siTotalFrame[siAct][siClass][CHAR_VERYFASTSPEED]= siResult1;
				}
				// FONTCOUNT
				else if( stricmp(pToken, "FONTCOUNT") == 0 )
				{
					siResult1= 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siFontCount[siAct][siClass] = siResult1;
				}
				// CENTERX
				else if( stricmp(pToken, "CENTERX") == 0 )
				{
					siResult1= 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siCenterX[siAct][siClass] = siResult1;
				}
				// CENTERY4
				else if( stricmp(pToken, "CENTERY") == 0 )
				{
					siResult1= 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siCenterY[siAct][siClass] = siResult1;
				}
				// ERROR
				else
				{
					clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	
					return FALSE;
				}
			} //for
			//-----------------------------------------------------------------



			//-----------------------------------------------------------------
			// 몸 부위 코드, 몸 부위 코드의 클래스 코드, OffsetX, OffsetY, 반투명인가  읽어오기
			for( siCount1=0; siCount1<m_HeroConfig[siKindID].siTotalPiece[siAct][siClass]; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ clGrp.Error("LoadHeroConfigInfo 1", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				// 몸의 부위 코드
				if( (pToken = strtok(cBuffer, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo 2", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				siBodyPart = GetHeroBodyCode(pToken);
				if( siBodyPart == -1 )							{ clGrp.Error("LoadHeroConfigInfo 3", "Invalid COF [%s]:%d Line %s", lpfd.cFileName,	siLineNo, pToken );	fclose(fp); return FALSE; }
				
				// 몸의 부위의 클래스 코드
				if( (pToken = strtok(NULL, cSeps))== NULL )		{ clGrp.Error("LoadHeroConfigInfo 4", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				siResult1 = GetHeroClassCode(pToken+2);

				if( siResult1 == -1 )							{ clGrp.Error("LoadHeroConfigInfo 5", "Invalid COF [%s]:%d Line %s", lpfd.cFileName,	siLineNo, pToken+2 );	fclose(fp); return FALSE; }
				strcpy(m_HeroConfig[siKindID].cPartCode[siAct][siClass][siBodyPart], pToken);

				// OFFSET X
				if( (pToken = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo 6", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				m_HeroConfig[siKindID].siOffsetX[siAct][siClass][siBodyPart] = atoi(pToken);
				
				// OFFSET Y
				if( (pToken = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo 7", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				m_HeroConfig[siKindID].siOffsetY[siAct][siClass][siBodyPart] = atoi(pToken);
				
				// BODY DRAWING TYPE
				if((pToken = strtok(NULL, cSeps)) == NULL)		{ clGrp.Error("LoadHeroConfigInfo 8", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				m_HeroConfig[siKindID].siRender[siAct][siClass][siBodyPart]  = atoi(pToken);
			} // for
			//-----------------------------------------------------------------



			//-----------------------------------------------------------------
			// 몸 부위 그리는 순서 읽기
			for( siCount1=0; siCount1 < m_HeroConfig[siKindID].siDirections[siAct][siClass]; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }

				// 방향 읽기
				if( (pToken = strtok(cBuffer, cSeps))== NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				sscanf(pToken, "%d", &siResult1);
				siDir = siResult1;

				for( siCount2=0; siCount2 < m_HeroConfig[siKindID].siTotalPiece[siAct][siClass]; siCount2++ )
				{
					if( (pToken = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
					siResult1 = GetHeroBodyCode(pToken);
					if( siResult1 == -1 )							{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }

					m_HeroConfig[siKindID].siPartDrawSort[siAct][siClass][siDir][siCount2] = siResult1;
				}
			}
			//
			//-----------------------------------------------------------------



			//-----------------------------------------------------------------
			// READPARTFRAME
			for( siCount1=0; siCount1 < 4; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )		{ clGrp.Error("LoadHeroConfigInfo", "1 Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				if( (pToken = strtok(cBuffer, cSeps)) == NULL )		{ clGrp.Error("LoadHeroConfigInfo", "2 Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }

				if( stricmp(pToken, "SLOWFRAME") == NULL )			siResult2 = CHAR_SLOWSPEED;	
				else if( stricmp(pToken, "NORMALFRAME") == NULL )	siResult2 = CHAR_NORMALSPEED;			
				else if( stricmp(pToken, "FASTFRAME") == NULL )		siResult2 =	CHAR_FASTSPEED;
				else if( stricmp(pToken, "FASTESTFRAME") == NULL )	siResult2 =	CHAR_VERYFASTSPEED;	
				else	{ clGrp.Error("3 Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				
				// 마지막 폰트 번호는 액션의 절정 시점이다.
				for( siCount2=0; siCount2<m_HeroConfig[siKindID].siTotalFrame[siAct][siClass][siResult2] + 1; siCount2++ )
				{
					if( (pToken = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pToken, "%d", &siResult1); 
					
					m_HeroConfig[siKindID].siPartFrame[siAct][siClass][siResult2][siCount2] = siResult1;
				}
			}
			//
			//-----------------------------------------------------------------


			fclose(fp);
			break;
		}
		if( FindNextFile(hFind, &lpfd) == 0 ) break;
	}

	FindClose(hFind);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	SetHeroConfig(SI32 siKindID, SCharHeroCurrEquip *pCurrEquip)
// Description	:	
BOOL CCharFileLoadManager::SetHeroConfig(SI32 siKindID, SCharHeroCurrEquip *pCurrEquip)
{

	SI32 siBodyAct, siWClass, siBodyPart;
	BOOL bPart;//, bAction;
	BOOL okswitch = FALSE;


	
	//======================================================================
	// 각 BodyPart를 읽어와야 하는지를 설정한다.
	//======================================================================
	ZeroMemory(pCurrEquip->bPart, sizeof(pCurrEquip->bPart));

	

	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		for( siWClass = 0; siWClass < MAX_HEROWEAPONCLASS; siWClass++ )
		{
			okswitch = FALSE;

			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				bPart = FALSE;
				if( strlen(m_HeroConfig[siKindID].cPartCode[siBodyAct][siWClass][siBodyPart]) != 0 )
				{
					
					switch( siBodyPart )
					{
					case HERO_BODY_HE:	
						if( stricmp(m_HelmetInfo[pCurrEquip->siHelmetInfoID].cHelmet,		"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_TR:	
						if( stricmp(m_ArmorInfo[pCurrEquip->siArmorInfoID].cTorso,			"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_LF:	
						if( stricmp(m_ArmorInfo[pCurrEquip->siArmorInfoID].cLeftShoulder,	"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_RF:	
						if( stricmp(m_ArmorInfo[pCurrEquip->siArmorInfoID].cRightShoulder,	"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_LA:	
						if( stricmp(m_HumanInfo[pCurrEquip->siHumanInfoID].cLeftArm,		"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_RA:	
						if( stricmp(m_HumanInfo[pCurrEquip->siHumanInfoID].cRightArm,		"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_LH:	
						if( stricmp(m_WeaponInfo[pCurrEquip->siWeaponInfoID].cLeftWeapon,	"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_RH:	
						if( stricmp(m_WeaponInfo[pCurrEquip->siWeaponInfoID].cRightWeapon,	"NONE") != 0 )	bPart = TRUE; break;
					case HERO_BODY_LG:	
						if( stricmp(m_HumanInfo[pCurrEquip->siHumanInfoID].cLeg,			"NONE") != 0 )	bPart = TRUE; break;
					default:			
						break;
					}

					if(bPart == TRUE)
					{
						okswitch = TRUE;
					}
					
				}
				pCurrEquip->bPart[siBodyAct][siWClass][siBodyPart] = bPart;

			}


			// 적절한 데이터가 준비되지 않았다. (하나의 이미지도 찍히지 않는다.)
			if(okswitch == FALSE)
			{
				//clGrp.Error("No Proper COF.", "kind:%d siWClass:%d siBodyAct:%d", siKindID, siWClass, siBodyAct);
			}

		}
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	MakeCurrHeroFileName(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip, CHAR *pDefaultBody)
// Description	:	pPrevEquip->bChanged, pCurrEquip->cFileName 설정
BOOL CCharFileLoadManager::MakeCurrHeroFileName(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip, CHAR *pDefaultBody,BOOL bFree)
{
	// 무기의 Class를 얻어온다.

		

	char cWeaponClass[5];
	SI32 siBodyAct, siBodyPart;
	BOOL bHeroPart[MAX_HEROACTIONS][MAX_HEROBODYPART];
	SI32 siWClass;	// Weaponclass Index	

	SI32 siHumanInfoID, siWeaponInfoID, siArmorInfoID, siHelmetInfoID;

	if(pPrevEquip != NULL) 
		memcpy(pPrevEquip, pCurrEquip, sizeof(SCharHeroCurrEquip));

	// 초기화를 한다.

	ZeroMemory(bHeroPart, sizeof(bHeroPart));

	siHumanInfoID		= pCurrEquip->siHumanInfoID;
	siWeaponInfoID		= pCurrEquip->siWeaponInfoID;
	siArmorInfoID		= pCurrEquip->siArmorInfoID;
	siHelmetInfoID		= pCurrEquip->siHelmetInfoID;




	//======================================================================
	// 설정무기의 CLASS를 얻어온다.
	//======================================================================
	//===============================================
	// 무기가 있으면 
	//===============================================
	if( siWeaponInfoID != 0 )	strcpy(cWeaponClass, m_WeaponInfo[siWeaponInfoID].cWClass);	
	else 						strcpy(cWeaponClass, "HTH");

	//===============================================
	// WeaponClass Index를 얻어온다.	ex) HTH
	//===============================================
	siWClass = GetHeroClassCode(cWeaponClass);

	//======================================================================
	// 각 Action별로 FILE의 이름을 설정한다.
	//======================================================================
	// NAME  Ex) "Man"
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			bHeroPart[siBodyAct][siBodyPart] = pCurrEquip->bPart[siBodyAct][siWClass][siBodyPart];

			if( bHeroPart[siBodyAct][siBodyPart] == TRUE )	
			{
				
				strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart], m_HeroConfig[siKindID].cSex);

			}
		}
	}
	// 여기까지 cFileName 예: "MA"
	

	// BODYPART 
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		if( bHeroPart[siBodyAct][HERO_BODY_HE] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_HE], "HE");
		if( bHeroPart[siBodyAct][HERO_BODY_TR] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_TR], "TR");
		if( bHeroPart[siBodyAct][HERO_BODY_LF] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LF], "LF");
		if( bHeroPart[siBodyAct][HERO_BODY_RF] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RF], "RF");
		if( bHeroPart[siBodyAct][HERO_BODY_LA] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LA], "LA");
		if( bHeroPart[siBodyAct][HERO_BODY_RA] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RA], "RA");
		if( bHeroPart[siBodyAct][HERO_BODY_LH] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LH], "LH");
		if( bHeroPart[siBodyAct][HERO_BODY_RH] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RH], "RH");
		if( bHeroPart[siBodyAct][HERO_BODY_LG] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LG], "LG");
		//사용하지 않는 S1,S2,S3는 초기화 하지 않는다.
		if( bHeroPart[siBodyAct][HERO_BODY_S1] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S1], "S1");
		if( bHeroPart[siBodyAct][HERO_BODY_S2] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S2], "S2");
		if( bHeroPart[siBodyAct][HERO_BODY_S3] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S3], "S3");
	}
	// 여기까지 cFileName 예: "MALG"

	// ITEM   : HERO CONFIG에 설정되어 있는 것만 설정해야 한다. (m_HeroInfo[siBodyAct][siWClass].cPartCode)
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		// 죽고 있는 동작이거나 죽어 있는 동작이라면
		if( siBodyAct == HERO_ACT_DT )
		{		
			if( bHeroPart[siBodyAct][HERO_BODY_HE] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_HE], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_TR] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_TR], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_LG] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LG], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_LF] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LF], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_RF] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RF], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_LA] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LA], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_RA] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RA], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_LH] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LH], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_RH] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RH], pDefaultBody);
			// S1 , S2, S3는 일단 NUDE //사용하지 않는 S1,S2,S3는 초기화 하지 않는다.
			if( bHeroPart[siBodyAct][HERO_BODY_S1] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S1], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_S2] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S2], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_S3] == TRUE ) 	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S3], pDefaultBody);
		}
		else
		{
			if( bHeroPart[siBodyAct][HERO_BODY_HE] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_HE], m_HelmetInfo[siHelmetInfoID].cHelmet);
			if( bHeroPart[siBodyAct][HERO_BODY_TR] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_TR], m_ArmorInfo[siArmorInfoID].cTorso);
			if( bHeroPart[siBodyAct][HERO_BODY_LF] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LF], m_ArmorInfo[siArmorInfoID].cLeftShoulder);
			if( bHeroPart[siBodyAct][HERO_BODY_RF] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RF], m_ArmorInfo[siArmorInfoID].cRightShoulder);
			if( bHeroPart[siBodyAct][HERO_BODY_LA] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LA], m_HumanInfo[siHumanInfoID].cLeftArm);
			if( bHeroPart[siBodyAct][HERO_BODY_RA] == TRUE ) 	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RA], m_HumanInfo[siHumanInfoID].cRightArm);
			
			
			if( bHeroPart[siBodyAct][HERO_BODY_LH] == TRUE )
			{
				char buffer[128];
				wsprintf(buffer, m_WeaponInfo[siWeaponInfoID].cLeftWeapon);

				// SP1무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP1") == 0)
				{
					// 조선 남자가 아니면, 
					if(siKindID != HERO_KM)			wsprintf(buffer, pDefaultBody);
				}
				// SP2무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP2") == 0)
				{
					// 일본 남자, 중국남자가 아니면, 
					if(siKindID != HERO_JM && siKindID != HERO_CM)
						wsprintf(buffer, pDefaultBody);
				}
				// SP4무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP4") == 0)
				{
					// 조선 여자가 아니면, 
					if(siKindID != HERO_KW)			wsprintf(buffer, pDefaultBody);
				}
				// SP5무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP5") == 0)
				{
					// 일본 여자, 명여자가 아니면, 
					if(siKindID != HERO_JW && siKindID != HERO_CW)
						wsprintf(buffer, pDefaultBody);
				}

				strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LH], buffer);


			}
			
			if( bHeroPart[siBodyAct][HERO_BODY_RH] == TRUE )
			{
				char buffer[128];
				wsprintf(buffer, m_WeaponInfo[siWeaponInfoID].cRightWeapon);

				// SP1무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP1") == 0)
				{
					// 조선 남자가 아니면, 
					if(siKindID != HERO_KM)			wsprintf(buffer, pDefaultBody);
				}
				// SP2무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP2") == 0)
				{
					// 일본 남자, 중국남자가 아니면, 
					if(siKindID != HERO_JM && siKindID != HERO_CM)
						wsprintf(buffer, pDefaultBody);
				}
				// SP4무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP4") == 0)
				{
					// 조선 여자가 아니면, 
					if(siKindID != HERO_KW)			wsprintf(buffer, pDefaultBody);
				}
				// SP5무기리를 사용하면, 
				if(strcmp(cWeaponClass, "SP5") == 0)
				{
					// 일본 여자, 명여자가 아니면, 
					if(siKindID != HERO_JW && siKindID != HERO_CW)
						wsprintf(buffer, pDefaultBody);
				}

				strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RH], buffer);
			}

			if( bHeroPart[siBodyAct][HERO_BODY_LG] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LG], m_HumanInfo[siHumanInfoID].cLeg);
			// S1 , S2, S3는 일단 NUDE //사용하지 않는 S1,S2,S3는 초기화 하지 않는다.
			if( bHeroPart[siBodyAct][HERO_BODY_S1] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S1], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_S2] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S2], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_S3] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S3], pDefaultBody);

		}
	}
	// 여기까지 cFileName 예: "MALGKD01"


	// ACTION, CLASS, EXT
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		// 죽고 있는 동작이거나 죽어 있는 동작이라면
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			if( bHeroPart[siBodyAct][siBodyPart] == TRUE )
			{
				strcat(pCurrEquip->cFileName[siBodyAct][siBodyPart], m_HeroConfig[siKindID].cPartCode[siBodyAct][siWClass][siBodyPart]);
				strcat(pCurrEquip->cFileName[siBodyAct][siBodyPart], "U.SPR");
				//캐릭터를 Free()시킬때라면.
				if(bFree == FALSE)
					strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart],"");
			}
		}
	}
			

	// pPrevEquip->bChanged 설정
	if( pPrevEquip != NULL )
		for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				pPrevEquip->bChanged[siBodyAct][siBodyPart] = FALSE;
				// 새로 불러야 할 이미지가 기존의 이미지 화일과 같은 것이 아니면,  
				if( strcmp(pPrevEquip->cFileName[siBodyAct][siBodyPart], pCurrEquip->cFileName[siBodyAct][siBodyPart]) != 0 )
				{
					// 달라졌음을 체크한다.(새롭게 로드해야 함을 알려준다.)
					pPrevEquip->bChanged[siBodyAct][siBodyPart] = TRUE;
				}
			}





	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			: LoadHeroFiles()
// Description	: 이 함수 호출되기 전에 MakeCurrHeroFileName()가 먼저 호출되어야 한다.
BOOL CCharFileLoadManager::LoadHeroFiles(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip,BOOL bFree,SI32 siID,BOOL bBattleMode)
{

	SI32 siCount, siFileIndex, siEmptyShIndex;
	SI32 siBodyAct, siBodyPart;
	char cWeaponClass[5];
	char cPrevWeaponClass[5];
	char cFullPathName[256];
	BOOL bCompare, bFoundEmpty;

	////////// 새로운 무기와 이전 무기의 아이디를 구한다.  /////////////

	// 새로운 무기의 아이디. 
	SI32 new_weapon_id = 0;
	if(pCurrEquip != NULL) new_weapon_id = pCurrEquip->siWeaponInfoID;
	else new_weapon_id = 0;

	// 이전 무기의 아이디. +
	SI32 old_weapon_id = 0;

	if( pPrevEquip != NULL )
	{
		old_weapon_id		= pPrevEquip->siWeaponInfoID;
	}

	//======================================================================
	// 설정무기의 CLASS를 얻어온다.
	//======================================================================
	//===============================================
	// 무기가 있으면 
	//===============================================
	if( new_weapon_id != 0 )
	{
		strcpy(cWeaponClass, m_WeaponInfo[new_weapon_id].cWClass);	
	}
	else
	{
		strcpy(cWeaponClass, "HTH");	
	}

	if( old_weapon_id != 0 )
	{
		strcpy(cPrevWeaponClass, m_WeaponInfo[old_weapon_id].cWClass);	
	}
	else
	{
		strcpy(cPrevWeaponClass, "HTH");	
	}


	//===============================================
	// WeaponClass Index를 얻어온다.	ex) HTHU
	//===============================================
	SI32 siWClass;
	SI32 siPrevWClass;
	SI32 siFlag[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];

	siWClass = GetHeroClassCode(cWeaponClass);

	if( pPrevEquip != NULL )
		siPrevWClass = GetHeroClassCode(cPrevWeaponClass);

	// 새로운 무기에 어울리는 이미지를 불러오기 전에 기존 무기의 이미지를 제거한다. 
	if( pPrevEquip != NULL )
	{
		// 이전에 읽어들였던 해당 Spr 파일들을 제거한다.
		for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				siCount = pPrevEquip->siShareIndex[siBodyAct][siBodyPart];


				//캐릭터가 Free()될때 캐릭터의 모든 siShareFileUse를 수정하고.
				//캐릭터의 부분만을 바꿀땐 수정된 사항의 siShareFileUse만 수정한다.
				if((  (strcmp(pPrevEquip->cFileName[siBodyAct][siBodyPart],"") != 0) && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) ) 
					|| ( (bFree == FALSE) && (m_HeroShareSpr.siShareFileUse[siCount] > 0) ))
					{
						// 사용중인 횟수를 1회 줄인다. 
						m_HeroShareSpr.siShareFileUse[siCount]--;



						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") == 0)
							siFlag[siCount] = TRUE;

						//캐릭터를 Free()할때 siShareIndex와 cFileName을 초기화 시켜준다.
						if(bFree == FALSE)
						{
							pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = 0;
							strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart],"");
						}
						

						
						// 사용중인 횟수가 0회로 떨어지면 화일을 제거한다. 
						if( (m_HeroShareSpr.siShareFileUse[siCount] <= 0)  && (m_bSetXSpr[m_HeroShareSpr.siShareFileID[siCount]] ))
						{

							FreeHeroSprite(&m_XSpr[m_HeroShareSpr.siShareFileID[siCount]],m_HeroShareSpr.siShareFileID[siCount]);

							ZeroMemory(m_HeroShareSpr.cShareFileName[siCount], MAX_FULLPATHNAME);

							m_HeroShareSpr.siShareFileID[siCount]  = 0;
							
								// 사용중인 횟수를 0으로 재설정한다. 
							m_HeroShareSpr.siShareFileUse[siCount] = 0;

						}

					}


			}
		
	}



	// 현재 읽어들인 Spr 파일이름을 비교해서 설정한다.
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			siEmptyShIndex = 0;
			bFoundEmpty  = FALSE;

			// 각 몸의 부분이 있는 곳만 파일을 읽어들이기 위해 설정한다.
			if( (pCurrEquip->bPart[siBodyAct][siWClass][siBodyPart] == TRUE )
			 && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) )
			{
					
					pPrevEquip->bChanged[siBodyAct][siBodyPart] = FALSE;

					bCompare = FALSE;
					for( siCount = 1; siCount < MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM; siCount++ )
					{
						if(siFlag[siCount] == TRUE)
							continue;
						// 일단 파일 이름이 설정되어 있는 곳이 비어 있으면 그곳을 기억한다.
						if( strlen(m_HeroShareSpr.cShareFileName[siCount]) == 0 && bFoundEmpty == FALSE )
						{
							siEmptyShIndex = siCount;
							bFoundEmpty = TRUE;
							break;
						}
						// 파일이름을 비교해서 이미 읽어들였으면 그 FILE ID를 설정한다.
						if( stricmp(m_HeroShareSpr.cShareFileName[siCount], pCurrEquip->cFileName[siBodyAct][siBodyPart]) == 0 )
						{

								// pCurrEquip->siFileID 설정
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = m_HeroShareSpr.siShareFileID[siCount];

								// pCurrEquip->siShareIndex 설정
								pCurrEquip->siShareIndex[siBodyAct][siBodyPart]  = siCount;		// 나중에 파일을 메모리에서 해제시켜야 하는지 알아보기 위해 설정한다.
								m_HeroShareSpr.siShareFileUse[siCount]++;						// 이미 Load된 파일을 사용하므로 사용 카운드를 증가한다. 
		
								bCompare = TRUE;

								break;

						}
					}
					
					// 새로 파일을 읽어들여서 설정한다.
					if( bCompare == FALSE && bFoundEmpty == TRUE )
					{

						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") != 0)
						{
							// spr 파일을 불러오기
							if( FindEmptySprite(siFileIndex) == TRUE )
							{
								// 파일이름을 설정한다.
								strcpy(m_HeroShareSpr.cShareFileName[siEmptyShIndex], pCurrEquip->cFileName[siBodyAct][siBodyPart]);
							
								// FULL PATH 파일이름을 설정한다.
								sprintf(cFullPathName, "Online\\JohapChar\\Spr\\%s", m_HeroShareSpr.cShareFileName[siEmptyShIndex]);
								// 파일을 읽어들여서 여기서 설정한다.
								m_HeroShareSpr.siShareFileID[siEmptyShIndex] = siFileIndex;
								// 파일 Drawing을 위한 것에도 신경을 쓴다. 
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = siFileIndex;

								pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = siEmptyShIndex;	// 나중에 파일을 메모리에서 해제 시켜야하는지 알아보기 위해 설정한다.
								m_HeroShareSpr.siShareFileUse[siEmptyShIndex] = 1;		// 처음 이파일을 사용하므로 1이다.

								
								LoadHeroSprite(cFullPathName, siFileIndex,siID,bBattleMode);

							}
							else
							{
								m_HeroShareSpr.siShareFileID[siEmptyShIndex]  = 0;
								ZeroMemory(m_HeroShareSpr.cShareFileName[siEmptyShIndex], MAX_FULLPATHNAME);
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = 0;
								pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = 0;
								m_HeroShareSpr.siShareFileUse[siCount] = 0;
							}

						}
					}
			}
		}
		

	return TRUE;
}

BOOL CCharFileLoadManager::LoadDefaultHeroFiles(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip,BOOL bFree)
{

	SI32 siCount, siFileIndex, siEmptyShIndex;
	SI32 siBodyAct, siBodyPart;
	char cWeaponClass[5];
	char cPrevWeaponClass[5];
	char cFullPathName[256];
	BOOL bCompare, bFoundEmpty;

	////////// 새로운 무기와 이전 무기의 아이디를 구한다.  /////////////

	// 새로운 무기의 아이디. 
	SI32 new_weapon_id = 0;
	if(pCurrEquip != NULL) new_weapon_id = pCurrEquip->siWeaponInfoID;
	else new_weapon_id = 0;

	// 이전 무기의 아이디. +
	SI32 old_weapon_id = 0;

	if( pPrevEquip != NULL )
	{
		old_weapon_id		= pPrevEquip->siWeaponInfoID;
	}

	//======================================================================
	// 설정무기의 CLASS를 얻어온다.
	//======================================================================
	//===============================================
	// 무기가 있으면 
	//===============================================
	if( new_weapon_id != 0 )
	{
		strcpy(cWeaponClass, m_WeaponInfo[new_weapon_id].cWClass);	
	}
	else
	{
		strcpy(cWeaponClass, "HTH");	
	}

	if( old_weapon_id != 0 )
	{
		strcpy(cPrevWeaponClass, m_WeaponInfo[old_weapon_id].cWClass);	
	}
	else
	{
		strcpy(cPrevWeaponClass, "HTH");	
	}


	//===============================================
	// WeaponClass Index를 얻어온다.	ex) HTHU
	//===============================================
	SI32 siWClass;
	SI32 siPrevWClass;
	SI32 siFlag[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];

	siWClass = GetHeroClassCode(cWeaponClass);

	if( pPrevEquip != NULL )
		siPrevWClass = GetHeroClassCode(cPrevWeaponClass);

	// 새로운 무기에 어울리는 이미지를 불러오기 전에 기존 무기의 이미지를 제거한다. 
	if( pPrevEquip != NULL )
	{
		// 이전에 읽어들였던 해당 Spr 파일들을 제거한다.
		for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				siCount = pPrevEquip->siShareIndex[siBodyAct][siBodyPart];


				//캐릭터가 Free()될때 캐릭터의 모든 siShareFileUse를 수정하고.
				//캐릭터의 부분만을 바꿀땐 수정된 사항의 siShareFileUse만 수정한다.
				if((  (strcmp(pPrevEquip->cFileName[siBodyAct][siBodyPart],"") != 0) && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) ) 
					|| ( (bFree == FALSE) && (m_HeroDefaultSpr.siShareFileUse[siCount] > 0) ))
					{
						// 사용중인 횟수를 1회 줄인다. 
						m_HeroDefaultSpr.siShareFileUse[siCount]--;



						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") == 0)
							siFlag[siCount] = TRUE;

						//캐릭터를 Free()할때 siShareIndex와 cFileName을 초기화 시켜준다.
						if(bFree == FALSE)
						{
							pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = 0;
							strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart],"");
						}
						

						
						// 사용중인 횟수가 0회로 떨어지면 화일을 제거한다. 
						if( (m_HeroDefaultSpr.siShareFileUse[siCount] <= 0) )
						{

							FreeDefaultHeroSprite(m_HeroDefaultSpr.siShareFileID[siCount]);

							m_HeroDefaultSpr.siShareFileID[siCount]  = 0;

							ZeroMemory(m_HeroDefaultSpr.cShareFileName[siCount], MAX_FULLPATHNAME);

								// 사용중인 횟수를 0으로 재설정한다. 
							m_HeroDefaultSpr.siShareFileUse[siCount] = 0;

						}

					}


			}
		
	}



	// 현재 읽어들인 Spr 파일이름을 비교해서 설정한다.
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			siEmptyShIndex = 0;
			bFoundEmpty  = FALSE;

			// 각 몸의 부분이 있는 곳만 파일을 읽어들이기 위해 설정한다.
			if( (pCurrEquip->bPart[siBodyAct][siWClass][siBodyPart] == TRUE )
			 && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) )
			{
					
					pPrevEquip->bChanged[siBodyAct][siBodyPart] = FALSE;

					bCompare = FALSE;
					for( siCount = 1; siCount < MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM; siCount++ )
					{
						if(siFlag[siCount] == TRUE)
							continue;
						// 일단 파일 이름이 설정되어 있는 곳이 비어 있으면 그곳을 기억한다.
						if( strlen(m_HeroDefaultSpr.cShareFileName[siCount]) == 0 && bFoundEmpty == FALSE )
						{
							siEmptyShIndex = siCount;
							bFoundEmpty = TRUE;
							break;
						}
						// 파일이름을 비교해서 이미 읽어들였으면 그 FILE ID를 설정한다.
						if( stricmp(m_HeroDefaultSpr.cShareFileName[siCount], pCurrEquip->cFileName[siBodyAct][siBodyPart]) == 0 )
						{

								// pCurrEquip->siFileID 설정
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = m_HeroDefaultSpr.siShareFileID[siCount];

								// pCurrEquip->siShareIndex 설정
								pCurrEquip->siShareIndex[siBodyAct][siBodyPart]  = siCount;		// 나중에 파일을 메모리에서 해제시켜야 하는지 알아보기 위해 설정한다.
								m_HeroDefaultSpr.siShareFileUse[siCount]++;						// 이미 Load된 파일을 사용하므로 사용 카운드를 증가한다. 
		
								bCompare = TRUE;

								break;

						}
					}
					
					// 새로 파일을 읽어들여서 설정한다.
					if( bCompare == FALSE && bFoundEmpty == TRUE )
					{

						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") != 0)
						{
							// spr 파일을 불러오기
							if( FindDefaultHeroFile(siFileIndex) == TRUE )
							{
								// 파일이름을 설정한다.
								strcpy(m_HeroDefaultSpr.cShareFileName[siEmptyShIndex], pCurrEquip->cFileName[siBodyAct][siBodyPart]);
							
								// FULL PATH 파일이름을 설정한다.
								sprintf(cFullPathName, "Online\\JohapChar\\Spr\\%s", m_HeroDefaultSpr.cShareFileName[siEmptyShIndex]);
								// 파일을 읽어들여서 여기서 설정한다.
								m_HeroDefaultSpr.siShareFileID[siEmptyShIndex] = siFileIndex;
								// 파일 Drawing을 위한 것에도 신경을 쓴다. 
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = siFileIndex;

								pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = siEmptyShIndex;	// 나중에 파일을 메모리에서 해제 시켜야하는지 알아보기 위해 설정한다.
								m_HeroDefaultSpr.siShareFileUse[siEmptyShIndex] = 1;		// 처음 이파일을 사용하므로 1이다.

								LoadDefaultHeroSprite(cFullPathName, siFileIndex);

								m_bSetDefaultSpr[siFileIndex] = TRUE;

							}
							else
							{
								m_HeroDefaultSpr.siShareFileID[siEmptyShIndex]  = 0;
								ZeroMemory(m_HeroDefaultSpr.cShareFileName[siEmptyShIndex], MAX_FULLPATHNAME);
//								pCurrEquip->siFileID[siBodyAct][siBodyPart] = 0;
//								pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = 0;
								m_HeroDefaultSpr.siShareFileUse[siCount] = 0;
							}

						}
					}
			}
		}
		

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	GetHeroActCode()
// Description	:	주인공 Action 문자열 코드를 받아 해당 정수값으로 되돌린다.
SI32 CCharFileLoadManager::GetHeroActCode(char * pAct)
{
	if( strstr(strupr(pAct), "A1") != 0 )	return HERO_ACT_A1;
	if( strstr(strupr(pAct), "N1") != 0 )	return HERO_ACT_N1;
	if( strstr(strupr(pAct), "WL") != 0 )	return HERO_ACT_WL;
	if( strstr(strupr(pAct), "DT") != 0 )	return HERO_ACT_DT;

	return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	GetHeroClassCode(char * pClass)
// Description	:	주인공 Class 문자열 코드를 받아 해당 정수값으로 되돌린다.
SI32 CCharFileLoadManager::GetHeroClassCode(char * pClass)
{
	if( stricmp(pClass, "1HS") == 0 )		return HERO_CLASS_1HS;
	if( stricmp(pClass, "2HS") == 0 )		return HERO_CLASS_2HS;
	if( stricmp(pClass, "2HT") == 0 )		return HERO_CLASS_2HT;
	if( stricmp(pClass, "THW") == 0 ) 		return HERO_CLASS_THW;
	if( stricmp(pClass, "BOW") == 0 ) 		return HERO_CLASS_BOW;
	if( stricmp(pClass, "HTH") == 0 ) 		return HERO_CLASS_HTH;
	if( stricmp(pClass, "RIF") == 0 ) 		return HERO_CLASS_RIF;
	if( stricmp(pClass, "SP1") == 0 ) 		return HERO_CLASS_SP1;
	if( stricmp(pClass, "SP2") == 0 ) 		return HERO_CLASS_SP2;
	if( stricmp(pClass, "SP3") == 0 ) 		return HERO_CLASS_SP3;
	if( stricmp(pClass, "SP4") == 0 ) 		return HERO_CLASS_SP4;
	if( stricmp(pClass, "SP5") == 0 ) 		return HERO_CLASS_SP5;

	return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	GetHeroClassCode(SCharHeroCurrEquip *pEquip)
// Description	:	주인공 Class 문자열 코드를 받아 해당 정수값으로 되돌린다.
SI32 CCharFileLoadManager::GetHeroClassCode(SCharHeroCurrEquip *pEquip)
{
	char cClassCode[20];
	SI32 siWeaponID =	pEquip->siWeaponInfoID;
	//SI32 siHorseID	=	pEquip->siHorseInfoID;

	if( siWeaponID == 0 )	strcpy(cClassCode, "HTH");	
	else					strcpy(cClassCode, m_WeaponInfo[siWeaponID].cWClass);

	/*
	if( siHorseID == 0 )	strcat(cClassCode, "U");
	else					strcat(cClassCode, "R");
	*/

	if( stricmp(cClassCode, "1HS") == 0 ) return HERO_CLASS_1HS;
	if( stricmp(cClassCode, "2HS") == 0 ) return HERO_CLASS_2HS;
	if( stricmp(cClassCode, "2HT") == 0 ) return HERO_CLASS_2HT;
	if( stricmp(cClassCode, "BOW") == 0 ) return HERO_CLASS_BOW;
	if( stricmp(cClassCode, "HTH") == 0 ) return HERO_CLASS_HTH;
	if( stricmp(cClassCode, "RIF") == 0 ) return HERO_CLASS_RIF;
	if( stricmp(cClassCode, "THW") == 0 ) return HERO_CLASS_THW;
	if( stricmp(cClassCode, "SP1") == 0 ) return HERO_CLASS_SP1;
	if( stricmp(cClassCode, "SP2") == 0 ) return HERO_CLASS_SP2;
	if( stricmp(cClassCode, "SP3") == 0 ) return HERO_CLASS_SP3;
	if( stricmp(cClassCode, "SP4") == 0 ) return HERO_CLASS_SP4;
	if( stricmp(cClassCode, "SP5") == 0 ) return HERO_CLASS_SP5;
	
	return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	GetHeroBodyCode()
// Description	:	주인공 Body 문자열 코드를 받아 해당 정수값으로 되돌린다.
SI32 CCharFileLoadManager::GetHeroBodyCode(char * pBody)
{
	if( stricmp(pBody, "HE") == 0 )		return HERO_BODY_HE;
	if( stricmp(pBody, "TR") == 0 )		return HERO_BODY_TR;
	if( stricmp(pBody, "LF") == 0 )		return HERO_BODY_LF;
	if( stricmp(pBody, "RF") == 0 )		return HERO_BODY_RF;
	if( stricmp(pBody, "LA") == 0 )		return HERO_BODY_LA;
	if( stricmp(pBody, "RA") == 0 )		return HERO_BODY_RA;
	if( stricmp(pBody, "LH") == 0 )		return HERO_BODY_LH;
	if( stricmp(pBody, "RH") == 0 )		return HERO_BODY_RH;
	if( stricmp(pBody, "LG") == 0 )		return HERO_BODY_LG;
	if( stricmp(pBody, "S1") == 0 )		return HERO_BODY_S1;
	if( stricmp(pBody, "S2") == 0 )		return HERO_BODY_S2;
	if( stricmp(pBody, "S3") == 0 )		return HERO_BODY_S3;

	return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: SetChange(SI32 siEquip, SI32 siEquipInfoID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody)
// Desc: 
// siEquip		:  갑옷? 헬멧 등의 구분
//siEquipInfoID	: 장비의 아이디. 
//siKindID		: 조선남? 조선녀?등의 정보. 
// siID			: 캐릭터의 아이디. 
// pDefaultBody	: 디폴트로 장착한 아이디. 
BOOL CCharFileLoadManager::SetChange(SI32 siEquip, SI32 siEquipInfoID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody,BOOL bBattleMode)
{
	if( siEquipInfoID == -1 ) return FALSE;

	// 디폴트 아이템 코드를 결정한다.
	SetDefaultItemInfo(pDefaultBody);

	// siHumanInfoID 가 설정되어 있지 않았으면 일단 siKindID + MAX_HERO_KIND_NUM 로 설정.
	if( m_HeroCurrEquip[siID].siHumanInfoID == 0 )		m_HeroCurrEquip[siID].siHumanInfoID		= siKindID + MAX_HERO_KIND_NUM;

	switch( siEquip )
	{
	case ON_WEAR_ARMOR_ITEM:	
		m_HeroCurrEquip[siID].siArmorInfoID		= siEquipInfoID;
		// 기본옷이냐에 따라 siHumanInfoID 값이 다르다.
		m_HeroCurrEquip[siID].siHumanInfoID		= siKindID + MAX_HERO_KIND_NUM * m_ArmorInfo[siEquipInfoID].bArmor;
		break;
	case ON_WEAR_HELMET_ITEM:	m_HeroCurrEquip[siID].siHelmetInfoID	= siEquipInfoID;	break;
	case ON_WEAR_WEAPON_ITEM:	m_HeroCurrEquip[siID].siWeaponInfoID	= siEquipInfoID;	break;
	default:					return FALSE;
	}

	SetHeroConfig(siKindID, &m_HeroPrevEquip[siID]);
	SetHeroConfig(siKindID, &m_HeroCurrEquip[siID]);

	// 현재 Hero의 파일이름을 설정한 후 Load한다.
	MakeCurrHeroFileName(siKindID, &m_HeroPrevEquip[siID], &m_HeroCurrEquip[siID], pDefaultBody,TRUE);
	LoadHeroFiles(siKindID, &m_HeroPrevEquip[siID], &m_HeroCurrEquip[siID],TRUE,siID,bBattleMode);

	return TRUE;
}


BOOL CCharFileLoadManager::SetDefaultChange(SI32 siArmorID, SI32 siHelmetID, SI32 siWeaponID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody)
{
	if( (siArmorID == -1) || (siHelmetID == -1) || (siWeaponID == -1) ) return FALSE;
	
	if(siID <0 || siID >= MAX_ONLINE_CHAR)
	{
		char buffer[128];
		wsprintf(buffer, "Exceed Char ID:%d", siID);
		SetMessage(buffer);
		return FALSE;
	}

	// 디폴트 아이템 코드를 결정한다.
	SetDefaultItemInfo(pDefaultBody);

	m_DefaultCurrEquip[siID].siArmorInfoID	= siArmorID;
	m_DefaultCurrEquip[siID].siHelmetInfoID	= siHelmetID;
	m_DefaultCurrEquip[siID].siWeaponInfoID	= siWeaponID;
	// 갑옷이냐 기본옷이냐에 따라 siHumanInfoID 값이 다르다.
	m_DefaultCurrEquip[siID].siHumanInfoID	= siKindID + MAX_HERO_KIND_NUM * m_ArmorInfo[siArmorID].bArmor;

	SetHeroConfig(siKindID, &m_DefaultPrevEquip[siID]);
	SetHeroConfig(siKindID, &m_DefaultCurrEquip[siID]);

	// 현재 Hero의 파일이름을 설정한 후 Load한다.
	MakeCurrHeroFileName(siKindID, &m_DefaultPrevEquip[siID], &m_DefaultCurrEquip[siID], pDefaultBody,TRUE);
	LoadDefaultHeroFiles(siKindID, &m_DefaultPrevEquip[siID], &m_DefaultCurrEquip[siID],TRUE);

	return TRUE;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: SetAllChange(SI32 siArmorID, SI32 siHelmetID, SI32 siWeaponID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody)
// Desc: 
BOOL CCharFileLoadManager::SetAllChange(SI32 siArmorID, SI32 siHelmetID, SI32 siWeaponID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody, BOOL bFree,BOOL bBattleMode)
{
	if( (siArmorID == -1) || (siHelmetID == -1) || (siWeaponID == -1) ) return FALSE;
	
	if(siID <0 || siID >= MAX_ONLINE_CHAR)
	{
		char buffer[128];
		wsprintf(buffer, "Exceed Char ID:%d", siID);
		SetMessage(buffer);
		return FALSE;
	}

	// 디폴트 아이템 코드를 결정한다.
	SetDefaultItemInfo(pDefaultBody);

	m_HeroCurrEquip[siID].siArmorInfoID		= siArmorID;
	m_HeroCurrEquip[siID].siHelmetInfoID	= siHelmetID;
	m_HeroCurrEquip[siID].siWeaponInfoID	= siWeaponID;
	// 갑옷이냐 기본옷이냐에 따라 siHumanInfoID 값이 다르다.
	m_HeroCurrEquip[siID].siHumanInfoID		= siKindID + MAX_HERO_KIND_NUM * m_ArmorInfo[siArmorID].bArmor;

	SetHeroConfig(siKindID, &m_HeroPrevEquip[siID]);
	SetHeroConfig(siKindID, &m_HeroCurrEquip[siID]);

	// 현재 Hero의 파일이름을 설정한 후 Load한다.
	MakeCurrHeroFileName(siKindID, &m_HeroPrevEquip[siID], &m_HeroCurrEquip[siID], pDefaultBody,bFree);
	LoadHeroFiles(siKindID, &m_HeroPrevEquip[siID], &m_HeroCurrEquip[siID],bFree,siID,bBattleMode);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: LoadHeroSprite()
// Desc: 낱개의 파일을 읽어온다.
BOOL CCharFileLoadManager::LoadHeroSprite(char *pFileName, SI32 &siFileIndex,SI32 siID,BOOL bBattleMode)
{
	strXsprQueueData st;

	if((pGame->pOnlineWorld->pMyData->GetMyCharUniqueID() == pGame->pOnlineWorld->pIOnlineChar->GetUniqueID((UI16)siID)) || bBattleMode == TRUE)
	{
		clGrp.LoadXspr( pFileName, m_XSpr[siFileIndex] );

		m_bSetXSpr[siFileIndex] = TRUE;			// 사용중임이라고 표시
	}
	else
	{
		// 구조체를 채운다.
		st.pXspr		=	&m_XSpr[siFileIndex];
		st.siFileIndex	=	siFileIndex;
		strcpy( st.szFileName,pFileName );

		m_bSetXSpr[siFileIndex] = TRUE;			// 사용중임이라고 표시

		while( !pOnlineLoadXSprProc->EnqueueWorkQueue(st) )
		{
			// 큐가 max라서 블럭 상태임
	//		SetMessage( "Queue buffer full.. Blocking..." );
			Sleep(10);
		}
	}
	//SetMessage( "Work(%d)  Complete(%d)", pOnlineLoadXSprProc->GetWorkQueueLength(), pOnlineLoadXSprProc->GetCompleteQueueLength() );
	//clGrp.LoadXspr( pFileName, m_XSpr[siFileIndex] );
		
	return TRUE;
}

BOOL CCharFileLoadManager::LoadDefaultHeroSprite(char *pFileName, SI32 &siFileIndex)
{
	if(clGrp.LoadXspr(pFileName,  m_DefaultSpr[siFileIndex]) == TRUE)
	{
		return TRUE;
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: FreeHeroSprite()
// Desc: 낱개의 파일을 해제한다.
BOOL CCharFileLoadManager::FreeHeroSprite(XSPR* spr ,SI32 siFileIndex)
{	
	// 해제 작업은 pOnlineLoadXSprProc 쓰레드로 넘긴다.
	strXsprQueueData stData;
	stData.pXspr = spr;
	stData.siFileIndex = siFileIndex;
	strcpy(stData.szFileName,"");

	while( !pOnlineLoadXSprProc->AddFreeList(&stData) )
	{
		Sleep(10);
	}

	return TRUE;
}

BOOL CCharFileLoadManager::FreeDefaultHeroSprite(SI32 siFileIndex)
{
	if(siFileIndex > 0)
	{
		clGrp.FreeXspr(m_DefaultSpr[siFileIndex]);
	}

	return TRUE;
}


// GamePoll에서 매프레임마다 호출해주는 함수이다.
BOOL CCharFileLoadManager::CheckXSprCompleteQueue()
{
	while( pOnlineLoadXSprProc->PeekCompleteQueue() )
	{
		strXsprQueueData	stCompleteData;

		pOnlineLoadXSprProc->DequeueCompleteQueue( stCompleteData );

		// 큐에서 빼서... 지금은 일단 하는 일이 없당...
	}

	return TRUE;
}


// GamePoll에서 매프레임마다 호출해주는 함수이다.
BOOL CCharFileLoadManager::CheckXSprFreeCompleteQueue()
{
	// 쓰레드쪽에서 Xspr이 해제됐다고 큐에 넣어둔 것을 빼서 처리한다.
	while( pOnlineLoadXSprProc->PeekFreeQueue() )
	{
		strXsprQueueData	stCompleteData;

		pOnlineLoadXSprProc->DequeueFreeQueue(stCompleteData);
		// 사용할 수 있는 슬롯이라는 표시로 FALSE
		m_bSetXSpr[stCompleteData.siFileIndex] = FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: FreeHeroSpriteAll()
// Desc: 모든 spr을 해제한다.
void CCharFileLoadManager::FreeHeroSpriteAll()
{
/*	while( pOnlineLoadXSprProc->GetWorkQueueLength() > 0 )
	{
		CheckXSprCompleteQueue();
		CheckXSprFreeCompleteQueue();
	}
*/
	// actdoll (2004/06/29 16:05) : 누수발견! 디폴트로 로딩된 캐릭터 스프라이트를 해제
	for( int i=0; i<MAX_DEFAULT_XSPR; i++ )
	{
		if( m_bSetDefaultSpr[i] )
		{
			FreeDefaultHeroSprite(m_HeroDefaultSpr.siShareFileID[i]);
			m_bSetDefaultSpr[i] = false;
		}
	}
		

	// Free list에 아무것도 없고 free complete queue에 들어온 것을 완전히 처리한 후에 아래에서 해제 작업을 시작한다.
	while( pOnlineLoadXSprProc->GetFreeListLength() > 0 || pOnlineLoadXSprProc->GetFreeQueueLength() > 0 ) 
	{
		CheckXSprFreeCompleteQueue();
	}

//	SI32 siFileID;
	SI32 siCount;

	for( siCount = 0; siCount < MAX_XSPR; siCount++ )
	{		
/*
		siFileID  = m_HeroShareSpr.siShareFileID[siCount];

		if( siFileID > 0 && m_XSpr[siFileID].bLoadComplete && m_XSpr[siFileID].Image )
			FreeHeroSprite(&m_XSpr[siFileID],siFileID);
*/
		//clGrp.FreeXspr( m_XSpr[siCount] );
		// 해제 작업을 thread로 넘긴다.
		if( m_bSetXSpr[siCount] )
		{
			FreeHeroSprite( &m_XSpr[siCount], siCount );
		}
	}

	// Work queue, free list가 완전히 빠질 때까지 기다린다.
	while( pOnlineLoadXSprProc->GetWorkQueueLength() > 0 || pOnlineLoadXSprProc->GetFreeListLength() > 0 ) 
		CheckXSprFreeCompleteQueue();
}


