//===================================================================================================================================================
//	FileName	:	CharFileLoadManager_Hero.cpp
//	History		:	2001/10/23(�ڿ���) - Create
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
// Description	:	�ش� �������� COF������ ��� Hero COF Structure�� �����Ѵ�.
//					(��) K MA A1 1HS U.cof

	
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
	char					countrystr[10];			// ������ ��Ÿ���� ����. (����:k, �Ϻ�:j, ��:c)

	char					cBuffer[1024];

	
	// �迭���� �ʱ�ȭ�Ѵ�.
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

	// ���� ����� ��� COF ���ϵ��� �˻��Ѵ�.
	wsprintf(cFindCOFFileName, ".\\Online\\JohapChar\\COF\\%s*.cof", countrystr);

	// ���� �˻�
	hFind = FindFirstFile(cFindCOFFileName, &lpfd);

	if( hFind == INVALID_HANDLE_VALUE )		return FALSE;
	strcpy(cSeps, " \t\n\r");
	
	
	while( TRUE )
	{
		switch( lpfd.dwFileAttributes )
		{
		case FILE_ATTRIBUTE_ARCHIVE:

			// cof ���ϸ����� ����� �ڵ带 �и� �Ѵ�
			siResult1 = sscanf(lpfd.cFileName, "%1s%2s%2s%3s", cNation, cSex, cAct, cClass);

			// ���� ����
			if( siResult1 != 4 ) { clGrp.Error("LoadHeroConfigInfo", "Invalid File Name [%s]", lpfd.cFileName); break; }

			// ����� �ڵ带 ���´�
			siAct	= GetHeroActCode(cAct);
			siClass = GetHeroClassCode(cClass);
			if( siAct	== -1 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid BodyAction Code on File [ %s ]", lpfd.cFileName); break; }
			if( siClass == -1 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid WeaponAction Code on File [ %s ]", lpfd.cFileName); break; }


			//-----------------------------------------------------------------
			// COF������ �о� �´�.
			//-----------------------------------------------------------------
			wsprintf(cCofFullPathName, "Online\\JohapChar\\COF\\%s", lpfd.cFileName);

			
			//fp = BindJXFile::GetInstance()->GetFile( cCofFullPathName );			
			fp = fopen( cCofFullPathName, "r" );
			if( fp == NULL )
			{
				clGrp.Error("No Cof file", cCofFullPathName);
				break;
			}


			// �� ĳ���� �׸��� FullPath�� �����Ѵ�.
			strcpy(m_HeroConfig[siKindID].cNation,		cNation);									// "K"
			strcpy(m_HeroConfig[siKindID].cSex,			cSex);										// "MA"


			//-------------------------
			// ���� Config�� ��� �κ��� �о�´�.
			siLineNo = 1;
			siResult1 = 0;
			for( siCount1=0; siCount1 < MAX_CFGHEADER; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				if( (pToken	= strtok(cBuffer, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				
				// ĳ���� Ÿ�� �˻�
				if( stricmp(pToken, "TYPE") == 0 )
				{
					// COFȭ�ϸ��� �˾Ƴ� ���� �ڵ�� ���� COFȭ�� ���� �ִ� ���� �ڵ尡 ��ġ�ϴ��� Ȯ���Ѵ�. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					if( stricmp(pTokenParam, cNation) != 0 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }

					// ĳ���� ������ ���� �ڵ带 ����Ѵ�. 
					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siSex = siResult1;
					
					// COF ȭ�ϸ��� �˾Ƴ� ���� �ڵ�� ���� COFȭ�� ���� �ִ� ���� �ڵ尡 ��ġ�ϴ��� Ȯ���Ѵ�. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					if( stricmp(pTokenParam, cSex) != 0 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				}
				// ĳ���� �׼� �˻�
				else if( stricmp(pToken, "ACTION") == 0 )	
				{
					// COF ȭ�ϸ��� �˾Ƴ� �׼� �ڵ�� ���� COF ȭ�� ���� �ִ� �׼� �ڵ尡 ��ġ�ϴ��� Ȯ���Ѵ�. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					siResult1 = GetHeroActCode(pTokenParam);
					if( siResult1 != siAct )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				}
				// ĳ���� Ŭ���� �˻�
				else if( stricmp(pToken, "CLASS") ==  0 )
				{
					//COF ȭ�ϸ��� �˾Ƴ� Ŭ���� �ڵ�� ���� COFȭ�� ���� �ִ� Ŭ���� �ڵ尡 ��ġ�ϴ��� Ȯ���Ѵ�. 
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					siResult1 = GetHeroClassCode(pTokenParam);	
					// �뵵�� Ư�� Ŭ���������� �����δ� �ٸ� Ŭ������ ������ ���� �ִ�.
					// ���� ��� 2HSŬ������ ��� ���� ���� 1HS Ŭ������ �����͸� ����Ѵ�.
					// �̷� ��� ȭ�� �̸����� 2HS�� �������� COFȭ�� �������� 1HS�� ���ȴ�. 
					// ���� COFȭ���� �����Ͱ��� ȭ�ϸ��� ���� �� �����Ͱ� �ٸ� ��찡 �߻��Ѵ�. 
					//if( siResult1 != siClass )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					if( siResult1 == -1 )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				}
				// TOTALPIECE
				else if( stricmp(pToken, "TOTALPIECE") == 0 )
				{ 
					// �ش� �׼��� �ش� Ŭ������ ��ü ��� �������� �̷���� ������ �����Ѵ�. 
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
					// �ش� �׼��� �ش� Ŭ������ �� �������� �̷���� ���ϸ��̼����� �����Ѵ�. 
					siResult1 = 0;
					if( (pTokenParam = strtok(NULL, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
					sscanf(pTokenParam, "%d", &siResult1);
					m_HeroConfig[siKindID].siDirections[siAct][siClass] = siResult1;
				}
				// TOTALFRAME
				else if( stricmp(pToken, "TOTALFRAME") == 0 )
				{
					// �ش� �׼��� �ش� Ŭ���� �ִϸ��̼��� ��� ���������� �̷���� ������ �����Ѵ�. 
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
			// �� ���� �ڵ�, �� ���� �ڵ��� Ŭ���� �ڵ�, OffsetX, OffsetY, �������ΰ�  �о����
			for( siCount1=0; siCount1<m_HeroConfig[siKindID].siTotalPiece[siAct][siClass]; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ clGrp.Error("LoadHeroConfigInfo 1", "Invalid COF [%s]:%d Line", cCofFullPathName,	siLineNo );	fclose(fp); return FALSE; }
				// ���� ���� �ڵ�
				if( (pToken = strtok(cBuffer, cSeps)) == NULL )	{ clGrp.Error("LoadHeroConfigInfo 2", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }
				siBodyPart = GetHeroBodyCode(pToken);
				if( siBodyPart == -1 )							{ clGrp.Error("LoadHeroConfigInfo 3", "Invalid COF [%s]:%d Line %s", lpfd.cFileName,	siLineNo, pToken );	fclose(fp); return FALSE; }
				
				// ���� ������ Ŭ���� �ڵ�
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
			// �� ���� �׸��� ���� �б�
			for( siCount1=0; siCount1 < m_HeroConfig[siKindID].siDirections[siAct][siClass]; siCount1++, siLineNo++ )
			{
				memset(cBuffer, 0, sizeof(cBuffer));
				if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ clGrp.Error("LoadHeroConfigInfo", "Invalid COF [%s]:%d Line", lpfd.cFileName,	siLineNo );	fclose(fp); return FALSE; }

				// ���� �б�
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
				
				// ������ ��Ʈ ��ȣ�� �׼��� ���� �����̴�.
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
	// �� BodyPart�� �о�;� �ϴ����� �����Ѵ�.
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


			// ������ �����Ͱ� �غ���� �ʾҴ�. (�ϳ��� �̹����� ������ �ʴ´�.)
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
// Description	:	pPrevEquip->bChanged, pCurrEquip->cFileName ����
BOOL CCharFileLoadManager::MakeCurrHeroFileName(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip, CHAR *pDefaultBody,BOOL bFree)
{
	// ������ Class�� ���´�.

		

	char cWeaponClass[5];
	SI32 siBodyAct, siBodyPart;
	BOOL bHeroPart[MAX_HEROACTIONS][MAX_HEROBODYPART];
	SI32 siWClass;	// Weaponclass Index	

	SI32 siHumanInfoID, siWeaponInfoID, siArmorInfoID, siHelmetInfoID;

	if(pPrevEquip != NULL) 
		memcpy(pPrevEquip, pCurrEquip, sizeof(SCharHeroCurrEquip));

	// �ʱ�ȭ�� �Ѵ�.

	ZeroMemory(bHeroPart, sizeof(bHeroPart));

	siHumanInfoID		= pCurrEquip->siHumanInfoID;
	siWeaponInfoID		= pCurrEquip->siWeaponInfoID;
	siArmorInfoID		= pCurrEquip->siArmorInfoID;
	siHelmetInfoID		= pCurrEquip->siHelmetInfoID;




	//======================================================================
	// ���������� CLASS�� ���´�.
	//======================================================================
	//===============================================
	// ���Ⱑ ������ 
	//===============================================
	if( siWeaponInfoID != 0 )	strcpy(cWeaponClass, m_WeaponInfo[siWeaponInfoID].cWClass);	
	else 						strcpy(cWeaponClass, "HTH");

	//===============================================
	// WeaponClass Index�� ���´�.	ex) HTH
	//===============================================
	siWClass = GetHeroClassCode(cWeaponClass);

	//======================================================================
	// �� Action���� FILE�� �̸��� �����Ѵ�.
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
	// ������� cFileName ��: "MA"
	

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
		//������� �ʴ� S1,S2,S3�� �ʱ�ȭ ���� �ʴ´�.
		if( bHeroPart[siBodyAct][HERO_BODY_S1] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S1], "S1");
		if( bHeroPart[siBodyAct][HERO_BODY_S2] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S2], "S2");
		if( bHeroPart[siBodyAct][HERO_BODY_S3] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S3], "S3");
	}
	// ������� cFileName ��: "MALG"

	// ITEM   : HERO CONFIG�� �����Ǿ� �ִ� �͸� �����ؾ� �Ѵ�. (m_HeroInfo[siBodyAct][siWClass].cPartCode)
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		// �װ� �ִ� �����̰ų� �׾� �ִ� �����̶��
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
			// S1 , S2, S3�� �ϴ� NUDE //������� �ʴ� S1,S2,S3�� �ʱ�ȭ ���� �ʴ´�.
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

				// SP1���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP1") == 0)
				{
					// ���� ���ڰ� �ƴϸ�, 
					if(siKindID != HERO_KM)			wsprintf(buffer, pDefaultBody);
				}
				// SP2���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP2") == 0)
				{
					// �Ϻ� ����, �߱����ڰ� �ƴϸ�, 
					if(siKindID != HERO_JM && siKindID != HERO_CM)
						wsprintf(buffer, pDefaultBody);
				}
				// SP4���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP4") == 0)
				{
					// ���� ���ڰ� �ƴϸ�, 
					if(siKindID != HERO_KW)			wsprintf(buffer, pDefaultBody);
				}
				// SP5���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP5") == 0)
				{
					// �Ϻ� ����, ���ڰ� �ƴϸ�, 
					if(siKindID != HERO_JW && siKindID != HERO_CW)
						wsprintf(buffer, pDefaultBody);
				}

				strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LH], buffer);


			}
			
			if( bHeroPart[siBodyAct][HERO_BODY_RH] == TRUE )
			{
				char buffer[128];
				wsprintf(buffer, m_WeaponInfo[siWeaponInfoID].cRightWeapon);

				// SP1���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP1") == 0)
				{
					// ���� ���ڰ� �ƴϸ�, 
					if(siKindID != HERO_KM)			wsprintf(buffer, pDefaultBody);
				}
				// SP2���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP2") == 0)
				{
					// �Ϻ� ����, �߱����ڰ� �ƴϸ�, 
					if(siKindID != HERO_JM && siKindID != HERO_CM)
						wsprintf(buffer, pDefaultBody);
				}
				// SP4���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP4") == 0)
				{
					// ���� ���ڰ� �ƴϸ�, 
					if(siKindID != HERO_KW)			wsprintf(buffer, pDefaultBody);
				}
				// SP5���⸮�� ����ϸ�, 
				if(strcmp(cWeaponClass, "SP5") == 0)
				{
					// �Ϻ� ����, ���ڰ� �ƴϸ�, 
					if(siKindID != HERO_JW && siKindID != HERO_CW)
						wsprintf(buffer, pDefaultBody);
				}

				strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_RH], buffer);
			}

			if( bHeroPart[siBodyAct][HERO_BODY_LG] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_LG], m_HumanInfo[siHumanInfoID].cLeg);
			// S1 , S2, S3�� �ϴ� NUDE //������� �ʴ� S1,S2,S3�� �ʱ�ȭ ���� �ʴ´�.
			if( bHeroPart[siBodyAct][HERO_BODY_S1] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S1], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_S2] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S2], pDefaultBody);
			if( bHeroPart[siBodyAct][HERO_BODY_S3] == TRUE )	strcat(pCurrEquip->cFileName[siBodyAct][HERO_BODY_S3], pDefaultBody);

		}
	}
	// ������� cFileName ��: "MALGKD01"


	// ACTION, CLASS, EXT
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
	{
		// �װ� �ִ� �����̰ų� �׾� �ִ� �����̶��
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			if( bHeroPart[siBodyAct][siBodyPart] == TRUE )
			{
				strcat(pCurrEquip->cFileName[siBodyAct][siBodyPart], m_HeroConfig[siKindID].cPartCode[siBodyAct][siWClass][siBodyPart]);
				strcat(pCurrEquip->cFileName[siBodyAct][siBodyPart], "U.SPR");
				//ĳ���͸� Free()��ų�����.
				if(bFree == FALSE)
					strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart],"");
			}
		}
	}
			

	// pPrevEquip->bChanged ����
	if( pPrevEquip != NULL )
		for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				pPrevEquip->bChanged[siBodyAct][siBodyPart] = FALSE;
				// ���� �ҷ��� �� �̹����� ������ �̹��� ȭ�ϰ� ���� ���� �ƴϸ�,  
				if( strcmp(pPrevEquip->cFileName[siBodyAct][siBodyPart], pCurrEquip->cFileName[siBodyAct][siBodyPart]) != 0 )
				{
					// �޶������� üũ�Ѵ�.(���Ӱ� �ε��ؾ� ���� �˷��ش�.)
					pPrevEquip->bChanged[siBodyAct][siBodyPart] = TRUE;
				}
			}





	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			: LoadHeroFiles()
// Description	: �� �Լ� ȣ��Ǳ� ���� MakeCurrHeroFileName()�� ���� ȣ��Ǿ�� �Ѵ�.
BOOL CCharFileLoadManager::LoadHeroFiles(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip,BOOL bFree,SI32 siID,BOOL bBattleMode)
{

	SI32 siCount, siFileIndex, siEmptyShIndex;
	SI32 siBodyAct, siBodyPart;
	char cWeaponClass[5];
	char cPrevWeaponClass[5];
	char cFullPathName[256];
	BOOL bCompare, bFoundEmpty;

	////////// ���ο� ����� ���� ������ ���̵� ���Ѵ�.  /////////////

	// ���ο� ������ ���̵�. 
	SI32 new_weapon_id = 0;
	if(pCurrEquip != NULL) new_weapon_id = pCurrEquip->siWeaponInfoID;
	else new_weapon_id = 0;

	// ���� ������ ���̵�. +
	SI32 old_weapon_id = 0;

	if( pPrevEquip != NULL )
	{
		old_weapon_id		= pPrevEquip->siWeaponInfoID;
	}

	//======================================================================
	// ���������� CLASS�� ���´�.
	//======================================================================
	//===============================================
	// ���Ⱑ ������ 
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
	// WeaponClass Index�� ���´�.	ex) HTHU
	//===============================================
	SI32 siWClass;
	SI32 siPrevWClass;
	SI32 siFlag[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];

	siWClass = GetHeroClassCode(cWeaponClass);

	if( pPrevEquip != NULL )
		siPrevWClass = GetHeroClassCode(cPrevWeaponClass);

	// ���ο� ���⿡ ��︮�� �̹����� �ҷ����� ���� ���� ������ �̹����� �����Ѵ�. 
	if( pPrevEquip != NULL )
	{
		// ������ �о�鿴�� �ش� Spr ���ϵ��� �����Ѵ�.
		for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				siCount = pPrevEquip->siShareIndex[siBodyAct][siBodyPart];


				//ĳ���Ͱ� Free()�ɶ� ĳ������ ��� siShareFileUse�� �����ϰ�.
				//ĳ������ �κи��� �ٲܶ� ������ ������ siShareFileUse�� �����Ѵ�.
				if((  (strcmp(pPrevEquip->cFileName[siBodyAct][siBodyPart],"") != 0) && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) ) 
					|| ( (bFree == FALSE) && (m_HeroShareSpr.siShareFileUse[siCount] > 0) ))
					{
						// ������� Ƚ���� 1ȸ ���δ�. 
						m_HeroShareSpr.siShareFileUse[siCount]--;



						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") == 0)
							siFlag[siCount] = TRUE;

						//ĳ���͸� Free()�Ҷ� siShareIndex�� cFileName�� �ʱ�ȭ �����ش�.
						if(bFree == FALSE)
						{
							pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = 0;
							strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart],"");
						}
						

						
						// ������� Ƚ���� 0ȸ�� �������� ȭ���� �����Ѵ�. 
						if( (m_HeroShareSpr.siShareFileUse[siCount] <= 0)  && (m_bSetXSpr[m_HeroShareSpr.siShareFileID[siCount]] ))
						{

							FreeHeroSprite(&m_XSpr[m_HeroShareSpr.siShareFileID[siCount]],m_HeroShareSpr.siShareFileID[siCount]);

							ZeroMemory(m_HeroShareSpr.cShareFileName[siCount], MAX_FULLPATHNAME);

							m_HeroShareSpr.siShareFileID[siCount]  = 0;
							
								// ������� Ƚ���� 0���� �缳���Ѵ�. 
							m_HeroShareSpr.siShareFileUse[siCount] = 0;

						}

					}


			}
		
	}



	// ���� �о���� Spr �����̸��� ���ؼ� �����Ѵ�.
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			siEmptyShIndex = 0;
			bFoundEmpty  = FALSE;

			// �� ���� �κ��� �ִ� ���� ������ �о���̱� ���� �����Ѵ�.
			if( (pCurrEquip->bPart[siBodyAct][siWClass][siBodyPart] == TRUE )
			 && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) )
			{
					
					pPrevEquip->bChanged[siBodyAct][siBodyPart] = FALSE;

					bCompare = FALSE;
					for( siCount = 1; siCount < MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM; siCount++ )
					{
						if(siFlag[siCount] == TRUE)
							continue;
						// �ϴ� ���� �̸��� �����Ǿ� �ִ� ���� ��� ������ �װ��� ����Ѵ�.
						if( strlen(m_HeroShareSpr.cShareFileName[siCount]) == 0 && bFoundEmpty == FALSE )
						{
							siEmptyShIndex = siCount;
							bFoundEmpty = TRUE;
							break;
						}
						// �����̸��� ���ؼ� �̹� �о�鿴���� �� FILE ID�� �����Ѵ�.
						if( stricmp(m_HeroShareSpr.cShareFileName[siCount], pCurrEquip->cFileName[siBodyAct][siBodyPart]) == 0 )
						{

								// pCurrEquip->siFileID ����
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = m_HeroShareSpr.siShareFileID[siCount];

								// pCurrEquip->siShareIndex ����
								pCurrEquip->siShareIndex[siBodyAct][siBodyPart]  = siCount;		// ���߿� ������ �޸𸮿��� �������Ѿ� �ϴ��� �˾ƺ��� ���� �����Ѵ�.
								m_HeroShareSpr.siShareFileUse[siCount]++;						// �̹� Load�� ������ ����ϹǷ� ��� ī��带 �����Ѵ�. 
		
								bCompare = TRUE;

								break;

						}
					}
					
					// ���� ������ �о�鿩�� �����Ѵ�.
					if( bCompare == FALSE && bFoundEmpty == TRUE )
					{

						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") != 0)
						{
							// spr ������ �ҷ�����
							if( FindEmptySprite(siFileIndex) == TRUE )
							{
								// �����̸��� �����Ѵ�.
								strcpy(m_HeroShareSpr.cShareFileName[siEmptyShIndex], pCurrEquip->cFileName[siBodyAct][siBodyPart]);
							
								// FULL PATH �����̸��� �����Ѵ�.
								sprintf(cFullPathName, "Online\\JohapChar\\Spr\\%s", m_HeroShareSpr.cShareFileName[siEmptyShIndex]);
								// ������ �о�鿩�� ���⼭ �����Ѵ�.
								m_HeroShareSpr.siShareFileID[siEmptyShIndex] = siFileIndex;
								// ���� Drawing�� ���� �Ϳ��� �Ű��� ����. 
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = siFileIndex;

								pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = siEmptyShIndex;	// ���߿� ������ �޸𸮿��� ���� ���Ѿ��ϴ��� �˾ƺ��� ���� �����Ѵ�.
								m_HeroShareSpr.siShareFileUse[siEmptyShIndex] = 1;		// ó�� �������� ����ϹǷ� 1�̴�.

								
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

	////////// ���ο� ����� ���� ������ ���̵� ���Ѵ�.  /////////////

	// ���ο� ������ ���̵�. 
	SI32 new_weapon_id = 0;
	if(pCurrEquip != NULL) new_weapon_id = pCurrEquip->siWeaponInfoID;
	else new_weapon_id = 0;

	// ���� ������ ���̵�. +
	SI32 old_weapon_id = 0;

	if( pPrevEquip != NULL )
	{
		old_weapon_id		= pPrevEquip->siWeaponInfoID;
	}

	//======================================================================
	// ���������� CLASS�� ���´�.
	//======================================================================
	//===============================================
	// ���Ⱑ ������ 
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
	// WeaponClass Index�� ���´�.	ex) HTHU
	//===============================================
	SI32 siWClass;
	SI32 siPrevWClass;
	SI32 siFlag[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];

	siWClass = GetHeroClassCode(cWeaponClass);

	if( pPrevEquip != NULL )
		siPrevWClass = GetHeroClassCode(cPrevWeaponClass);

	// ���ο� ���⿡ ��︮�� �̹����� �ҷ����� ���� ���� ������ �̹����� �����Ѵ�. 
	if( pPrevEquip != NULL )
	{
		// ������ �о�鿴�� �ش� Spr ���ϵ��� �����Ѵ�.
		for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
			for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
			{
				siCount = pPrevEquip->siShareIndex[siBodyAct][siBodyPart];


				//ĳ���Ͱ� Free()�ɶ� ĳ������ ��� siShareFileUse�� �����ϰ�.
				//ĳ������ �κи��� �ٲܶ� ������ ������ siShareFileUse�� �����Ѵ�.
				if((  (strcmp(pPrevEquip->cFileName[siBodyAct][siBodyPart],"") != 0) && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) ) 
					|| ( (bFree == FALSE) && (m_HeroDefaultSpr.siShareFileUse[siCount] > 0) ))
					{
						// ������� Ƚ���� 1ȸ ���δ�. 
						m_HeroDefaultSpr.siShareFileUse[siCount]--;



						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") == 0)
							siFlag[siCount] = TRUE;

						//ĳ���͸� Free()�Ҷ� siShareIndex�� cFileName�� �ʱ�ȭ �����ش�.
						if(bFree == FALSE)
						{
							pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = 0;
							strcpy(pCurrEquip->cFileName[siBodyAct][siBodyPart],"");
						}
						

						
						// ������� Ƚ���� 0ȸ�� �������� ȭ���� �����Ѵ�. 
						if( (m_HeroDefaultSpr.siShareFileUse[siCount] <= 0) )
						{

							FreeDefaultHeroSprite(m_HeroDefaultSpr.siShareFileID[siCount]);

							m_HeroDefaultSpr.siShareFileID[siCount]  = 0;

							ZeroMemory(m_HeroDefaultSpr.cShareFileName[siCount], MAX_FULLPATHNAME);

								// ������� Ƚ���� 0���� �缳���Ѵ�. 
							m_HeroDefaultSpr.siShareFileUse[siCount] = 0;

						}

					}


			}
		
	}



	// ���� �о���� Spr �����̸��� ���ؼ� �����Ѵ�.
	for( siBodyAct = 0; siBodyAct < MAX_HEROACTIONS; siBodyAct++ )
		for( siBodyPart = 0; siBodyPart < MAX_HEROBODYPART; siBodyPart++ )
		{
			siEmptyShIndex = 0;
			bFoundEmpty  = FALSE;

			// �� ���� �κ��� �ִ� ���� ������ �о���̱� ���� �����Ѵ�.
			if( (pCurrEquip->bPart[siBodyAct][siWClass][siBodyPart] == TRUE )
			 && (pPrevEquip->bChanged[siBodyAct][siBodyPart] == TRUE) )
			{
					
					pPrevEquip->bChanged[siBodyAct][siBodyPart] = FALSE;

					bCompare = FALSE;
					for( siCount = 1; siCount < MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM; siCount++ )
					{
						if(siFlag[siCount] == TRUE)
							continue;
						// �ϴ� ���� �̸��� �����Ǿ� �ִ� ���� ��� ������ �װ��� ����Ѵ�.
						if( strlen(m_HeroDefaultSpr.cShareFileName[siCount]) == 0 && bFoundEmpty == FALSE )
						{
							siEmptyShIndex = siCount;
							bFoundEmpty = TRUE;
							break;
						}
						// �����̸��� ���ؼ� �̹� �о�鿴���� �� FILE ID�� �����Ѵ�.
						if( stricmp(m_HeroDefaultSpr.cShareFileName[siCount], pCurrEquip->cFileName[siBodyAct][siBodyPart]) == 0 )
						{

								// pCurrEquip->siFileID ����
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = m_HeroDefaultSpr.siShareFileID[siCount];

								// pCurrEquip->siShareIndex ����
								pCurrEquip->siShareIndex[siBodyAct][siBodyPart]  = siCount;		// ���߿� ������ �޸𸮿��� �������Ѿ� �ϴ��� �˾ƺ��� ���� �����Ѵ�.
								m_HeroDefaultSpr.siShareFileUse[siCount]++;						// �̹� Load�� ������ ����ϹǷ� ��� ī��带 �����Ѵ�. 
		
								bCompare = TRUE;

								break;

						}
					}
					
					// ���� ������ �о�鿩�� �����Ѵ�.
					if( bCompare == FALSE && bFoundEmpty == TRUE )
					{

						if(strcmp(pCurrEquip->cFileName[siBodyAct][siBodyPart],"") != 0)
						{
							// spr ������ �ҷ�����
							if( FindDefaultHeroFile(siFileIndex) == TRUE )
							{
								// �����̸��� �����Ѵ�.
								strcpy(m_HeroDefaultSpr.cShareFileName[siEmptyShIndex], pCurrEquip->cFileName[siBodyAct][siBodyPart]);
							
								// FULL PATH �����̸��� �����Ѵ�.
								sprintf(cFullPathName, "Online\\JohapChar\\Spr\\%s", m_HeroDefaultSpr.cShareFileName[siEmptyShIndex]);
								// ������ �о�鿩�� ���⼭ �����Ѵ�.
								m_HeroDefaultSpr.siShareFileID[siEmptyShIndex] = siFileIndex;
								// ���� Drawing�� ���� �Ϳ��� �Ű��� ����. 
								pCurrEquip->siFileID[siBodyAct][siBodyPart] = siFileIndex;

								pCurrEquip->siShareIndex[siBodyAct][siBodyPart] = siEmptyShIndex;	// ���߿� ������ �޸𸮿��� ���� ���Ѿ��ϴ��� �˾ƺ��� ���� �����Ѵ�.
								m_HeroDefaultSpr.siShareFileUse[siEmptyShIndex] = 1;		// ó�� �������� ����ϹǷ� 1�̴�.

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
// Description	:	���ΰ� Action ���ڿ� �ڵ带 �޾� �ش� ���������� �ǵ�����.
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
// Description	:	���ΰ� Class ���ڿ� �ڵ带 �޾� �ش� ���������� �ǵ�����.
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
// Description	:	���ΰ� Class ���ڿ� �ڵ带 �޾� �ش� ���������� �ǵ�����.
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
// Description	:	���ΰ� Body ���ڿ� �ڵ带 �޾� �ش� ���������� �ǵ�����.
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
// siEquip		:  ����? ��� ���� ����
//siEquipInfoID	: ����� ���̵�. 
//siKindID		: ������? ������?���� ����. 
// siID			: ĳ������ ���̵�. 
// pDefaultBody	: ����Ʈ�� ������ ���̵�. 
BOOL CCharFileLoadManager::SetChange(SI32 siEquip, SI32 siEquipInfoID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody,BOOL bBattleMode)
{
	if( siEquipInfoID == -1 ) return FALSE;

	// ����Ʈ ������ �ڵ带 �����Ѵ�.
	SetDefaultItemInfo(pDefaultBody);

	// siHumanInfoID �� �����Ǿ� ���� �ʾ����� �ϴ� siKindID + MAX_HERO_KIND_NUM �� ����.
	if( m_HeroCurrEquip[siID].siHumanInfoID == 0 )		m_HeroCurrEquip[siID].siHumanInfoID		= siKindID + MAX_HERO_KIND_NUM;

	switch( siEquip )
	{
	case ON_WEAR_ARMOR_ITEM:	
		m_HeroCurrEquip[siID].siArmorInfoID		= siEquipInfoID;
		// �⺻���̳Ŀ� ���� siHumanInfoID ���� �ٸ���.
		m_HeroCurrEquip[siID].siHumanInfoID		= siKindID + MAX_HERO_KIND_NUM * m_ArmorInfo[siEquipInfoID].bArmor;
		break;
	case ON_WEAR_HELMET_ITEM:	m_HeroCurrEquip[siID].siHelmetInfoID	= siEquipInfoID;	break;
	case ON_WEAR_WEAPON_ITEM:	m_HeroCurrEquip[siID].siWeaponInfoID	= siEquipInfoID;	break;
	default:					return FALSE;
	}

	SetHeroConfig(siKindID, &m_HeroPrevEquip[siID]);
	SetHeroConfig(siKindID, &m_HeroCurrEquip[siID]);

	// ���� Hero�� �����̸��� ������ �� Load�Ѵ�.
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

	// ����Ʈ ������ �ڵ带 �����Ѵ�.
	SetDefaultItemInfo(pDefaultBody);

	m_DefaultCurrEquip[siID].siArmorInfoID	= siArmorID;
	m_DefaultCurrEquip[siID].siHelmetInfoID	= siHelmetID;
	m_DefaultCurrEquip[siID].siWeaponInfoID	= siWeaponID;
	// �����̳� �⺻���̳Ŀ� ���� siHumanInfoID ���� �ٸ���.
	m_DefaultCurrEquip[siID].siHumanInfoID	= siKindID + MAX_HERO_KIND_NUM * m_ArmorInfo[siArmorID].bArmor;

	SetHeroConfig(siKindID, &m_DefaultPrevEquip[siID]);
	SetHeroConfig(siKindID, &m_DefaultCurrEquip[siID]);

	// ���� Hero�� �����̸��� ������ �� Load�Ѵ�.
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

	// ����Ʈ ������ �ڵ带 �����Ѵ�.
	SetDefaultItemInfo(pDefaultBody);

	m_HeroCurrEquip[siID].siArmorInfoID		= siArmorID;
	m_HeroCurrEquip[siID].siHelmetInfoID	= siHelmetID;
	m_HeroCurrEquip[siID].siWeaponInfoID	= siWeaponID;
	// �����̳� �⺻���̳Ŀ� ���� siHumanInfoID ���� �ٸ���.
	m_HeroCurrEquip[siID].siHumanInfoID		= siKindID + MAX_HERO_KIND_NUM * m_ArmorInfo[siArmorID].bArmor;

	SetHeroConfig(siKindID, &m_HeroPrevEquip[siID]);
	SetHeroConfig(siKindID, &m_HeroCurrEquip[siID]);

	// ���� Hero�� �����̸��� ������ �� Load�Ѵ�.
	MakeCurrHeroFileName(siKindID, &m_HeroPrevEquip[siID], &m_HeroCurrEquip[siID], pDefaultBody,bFree);
	LoadHeroFiles(siKindID, &m_HeroPrevEquip[siID], &m_HeroCurrEquip[siID],bFree,siID,bBattleMode);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: LoadHeroSprite()
// Desc: ������ ������ �о�´�.
BOOL CCharFileLoadManager::LoadHeroSprite(char *pFileName, SI32 &siFileIndex,SI32 siID,BOOL bBattleMode)
{
	strXsprQueueData st;

	if((pGame->pOnlineWorld->pMyData->GetMyCharUniqueID() == pGame->pOnlineWorld->pIOnlineChar->GetUniqueID((UI16)siID)) || bBattleMode == TRUE)
	{
		clGrp.LoadXspr( pFileName, m_XSpr[siFileIndex] );

		m_bSetXSpr[siFileIndex] = TRUE;			// ��������̶�� ǥ��
	}
	else
	{
		// ����ü�� ä���.
		st.pXspr		=	&m_XSpr[siFileIndex];
		st.siFileIndex	=	siFileIndex;
		strcpy( st.szFileName,pFileName );

		m_bSetXSpr[siFileIndex] = TRUE;			// ��������̶�� ǥ��

		while( !pOnlineLoadXSprProc->EnqueueWorkQueue(st) )
		{
			// ť�� max�� �� ������
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
// Desc: ������ ������ �����Ѵ�.
BOOL CCharFileLoadManager::FreeHeroSprite(XSPR* spr ,SI32 siFileIndex)
{	
	// ���� �۾��� pOnlineLoadXSprProc ������� �ѱ��.
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


// GamePoll���� �������Ӹ��� ȣ�����ִ� �Լ��̴�.
BOOL CCharFileLoadManager::CheckXSprCompleteQueue()
{
	while( pOnlineLoadXSprProc->PeekCompleteQueue() )
	{
		strXsprQueueData	stCompleteData;

		pOnlineLoadXSprProc->DequeueCompleteQueue( stCompleteData );

		// ť���� ����... ������ �ϴ� �ϴ� ���� ����...
	}

	return TRUE;
}


// GamePoll���� �������Ӹ��� ȣ�����ִ� �Լ��̴�.
BOOL CCharFileLoadManager::CheckXSprFreeCompleteQueue()
{
	// �������ʿ��� Xspr�� �����ƴٰ� ť�� �־�� ���� ���� ó���Ѵ�.
	while( pOnlineLoadXSprProc->PeekFreeQueue() )
	{
		strXsprQueueData	stCompleteData;

		pOnlineLoadXSprProc->DequeueFreeQueue(stCompleteData);
		// ����� �� �ִ� �����̶�� ǥ�÷� FALSE
		m_bSetXSpr[stCompleteData.siFileIndex] = FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: FreeHeroSpriteAll()
// Desc: ��� spr�� �����Ѵ�.
void CCharFileLoadManager::FreeHeroSpriteAll()
{
/*	while( pOnlineLoadXSprProc->GetWorkQueueLength() > 0 )
	{
		CheckXSprCompleteQueue();
		CheckXSprFreeCompleteQueue();
	}
*/
	// actdoll (2004/06/29 16:05) : �����߰�! ����Ʈ�� �ε��� ĳ���� ��������Ʈ�� ����
	for( int i=0; i<MAX_DEFAULT_XSPR; i++ )
	{
		if( m_bSetDefaultSpr[i] )
		{
			FreeDefaultHeroSprite(m_HeroDefaultSpr.siShareFileID[i]);
			m_bSetDefaultSpr[i] = false;
		}
	}
		

	// Free list�� �ƹ��͵� ���� free complete queue�� ���� ���� ������ ó���� �Ŀ� �Ʒ����� ���� �۾��� �����Ѵ�.
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
		// ���� �۾��� thread�� �ѱ��.
		if( m_bSetXSpr[siCount] )
		{
			FreeHeroSprite( &m_XSpr[siCount], siCount );
		}
	}

	// Work queue, free list�� ������ ���� ������ ��ٸ���.
	while( pOnlineLoadXSprProc->GetWorkQueueLength() > 0 || pOnlineLoadXSprProc->GetFreeListLength() > 0 ) 
		CheckXSprFreeCompleteQueue();
}


