//===================================================================================================================================================
//	FileName	:	CharFileLoadManager_Item.cpp
//	History		:	2001/10/23(박연숙) - Create
//	Discription	:	아이템(무기, 갑옷, 헬멧 등)의 정보를 관리한다.
//===================================================================================================================================================
#include <GSL.h>
#include <main.h>

#include "CharFileLoadManager.h"
#include "BindJXFile.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	SetDefaultItemInfo(CHAR *pDefaultBody)
// Description	:	??? 이 함수가 왜 필요했더라
BOOL CCharFileLoadManager::SetDefaultItemInfo(CHAR *pDefaultBody)
{
	strcpy(	m_HelmetInfo[0].cHelmet,		pDefaultBody);	
	strcpy(	m_ArmorInfo[0].cTorso,			pDefaultBody);	
	strcpy(	m_HumanInfo[0].cRightArm,		pDefaultBody);	
	strcpy(	m_HumanInfo[0].cLeftArm,		pDefaultBody);	
	strcpy(	m_HumanInfo[0].cLeg,			pDefaultBody);	
	strcpy(	m_ArmorInfo[0].cLeftShoulder,	"NONE");
	strcpy(	m_ArmorInfo[0].cRightShoulder,	"NONE");
	strcpy(	m_WeaponInfo[0].cLeftWeapon,	"NONE");	
	strcpy(	m_WeaponInfo[0].cRightWeapon,	"NONE");

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	LoadHumanInfo()
// Description	:	인간 정보를 파일로부터 읽어들인다.
BOOL CCharFileLoadManager::LoadHumanInfo(char *pFileName)
{
	FILE		*fp;
	char		cBuffer[MAX_LINESTR];
	char		cStrBuf[1024];
	SI32		siLineNo = 1;
	SI32		siCount;
	char		cSeps[16];
	char		*pToken;
	char		cTableHeaders[100][30];

//	fp = fopen(pFileName, "rt"); 
	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if( fp == NULL ) {	clGrp.Error("CharFileLoadManager_Item", "File Not Found : %s", pFileName);	return FALSE; }
	
	strcpy(cSeps, "\t\n\r");
	
	// 1줄은 TAG들을 읽어온다.
	memset(cBuffer, 0, sizeof(cBuffer));
	if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ fclose(fp); return FALSE; }
	{
		pToken = strtok(cBuffer, cSeps);
		if( pToken == NULL )		{ fclose(fp); return FALSE; }
		siCount = 0;
		while( pToken != NULL )
		{
			strcpy(cTableHeaders[siCount],pToken);
			pToken = strtok( NULL, cSeps );
			siCount++;
		}
	}

	// 2번째 줄로부터 내용을 얻어온다.
	while( fgets(cBuffer, MAX_LINESTR-1, fp) != NULL )
	{
		pToken = strtok(cBuffer, cSeps);
		if( pToken == NULL )	break;

		siCount = 0;
		while( pToken != NULL )
		{
			strcpy(cStrBuf, pToken);

			if(		 stricmp(cTableHeaders[siCount], "Name")		== NULL )	strcpy(m_HumanInfo[siLineNo].cName,			cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "ID")			== NULL )	m_HumanInfo[siLineNo].siID					= atoi(cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Left Arm")	== NULL )	strcpy(m_HumanInfo[siLineNo].cLeftArm,		cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Right Arm")	== NULL )	strcpy(m_HumanInfo[siLineNo].cRightArm,		cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Leg")			== NULL )	strcpy(m_HumanInfo[siLineNo].cLeg,			cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "SP1")			== NULL )	strcpy(m_HumanInfo[siLineNo].cSP1,			cStrBuf);

			pToken = strtok( NULL, cSeps );
			siCount++;
		}
		// 해당 INV, FLP CODE의 중복을 정리해서 설정한다.
		//??? SetItemsInvFlpInfoToShare(ITEM_TYPE_ARMORS, siLineNo);
		siLineNo++;
		if(siLineNo >= MAX_ITEMINFO)clGrp.Error("fd19474", "4324975");
		memset(cBuffer, 0, sizeof(cBuffer));
	}
	fclose(fp);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	LoadArmorInfo()
// Description	:	
BOOL CCharFileLoadManager::LoadArmorInfo(char *pFileName)
{
	FILE		*fp;
	char		cBuffer[MAX_LINESTR];
	char		cStrBuf[1024];
	ZeroMemory(cStrBuf, sizeof(cStrBuf));
	SI32		siLineNo = 1;
	SI32		siCount = 0;
	char		cSeps[16];
	char		*pToken;
	char		cTableHeaders[100][30];

	strcpy(cSeps, "\t\n\r");

//	fp = fopen(pFileName, "rt");
	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if( fp == NULL ) {	clGrp.Error("CharFileLoadManager_Item", "File Not Found : %s", pFileName);	return FALSE; }
	
	// 1줄은 TAG들을 읽어온다.
	memset(cBuffer, 0, sizeof(cBuffer));
	if(fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ fclose(fp); return FALSE; }
	{
		pToken = strtok(cBuffer, cSeps);
		if( pToken == NULL )		{ fclose(fp); return FALSE; }
		siCount = 0;
		while( pToken != NULL )
		{
			strcpy(cTableHeaders[siCount],pToken);
			pToken = strtok( NULL, cSeps );
			siCount++;
		}
	}

	// 2번째 줄로부터 내용을 얻어온다.
	while( fgets(cBuffer, MAX_LINESTR-1, fp) != NULL )
	{
		pToken = strtok(cBuffer, cSeps);
		if( pToken == NULL )	break;

		siCount = 0;
		while( pToken != NULL )
		{
			strcpy(cStrBuf, pToken);

			if(		 stricmp(cTableHeaders[siCount], "Name")			== NULL )	strcpy(m_ArmorInfo[siLineNo].cName,				cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "ID")				== NULL )	m_ArmorInfo[siLineNo].siID						= atoi(cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Left Shoulder")	== NULL )	strcpy(m_ArmorInfo[m_ArmorInfo[siLineNo].siID].cLeftShoulder,		cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Right Shoulder")	== NULL )	strcpy(m_ArmorInfo[m_ArmorInfo[siLineNo].siID].cRightShoulder,	cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Torso")			== NULL )	strcpy(m_ArmorInfo[m_ArmorInfo[siLineNo].siID].cTorso,			cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Clothe flag")		== NULL )	m_ArmorInfo[m_ArmorInfo[siLineNo].siID].bArmor	= atoi(cStrBuf);

			pToken = strtok( NULL, cSeps );
			siCount++;
		}
		// 해당 INV, FLP CODE의 중복을 정리해서 설정한다.
		//??? SetItemsInvFlpInfoToShare(ITEM_TYPE_ARMORS, siLineNo);
		siLineNo++;
		if(siLineNo >= MAX_ITEMINFO)clGrp.Error("19474", "4324975");

		memset(cBuffer, 0, sizeof(cBuffer));
	}
	fclose(fp);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	LoadWeaponInfo()
// Description	:	파일을 열어서 무기 정보를 읽어들인다.
BOOL CCharFileLoadManager::LoadWeaponInfo(char *pFileName)
{
	FILE *fp;
	char cBuffer[MAX_LINESTR];
	char cStrBuf[1024];
	SI32 siLineNo = 1;
	SI32 siCount = 0;
	char cSeps[16];
	char *pToken;
	char cTableHeaders[100][30];
	strcpy(cSeps, "\t\n\r");

//	fp = fopen(pFileName, "rt");
	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if( fp == NULL ) {	clGrp.Error("CharFileLoadManager_Item", "File Not Found : %s", pFileName);	return FALSE; }

	// 1줄은 TAG들을 읽어온다.
	memset(cBuffer, 0, sizeof(cBuffer));
	if( fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ fclose(fp); return FALSE; }
	{
		pToken = strtok(cBuffer, cSeps);
		if( pToken == NULL )	{ fclose(fp); return FALSE; }
		siCount = 0;
		while( pToken != NULL )
		{
			strcpy(cTableHeaders[siCount], pToken);
			pToken = strtok(NULL, cSeps);
			siCount++;
		}
	}

	// 2번째 줄로부터 내용을 얻어온다.
	while( fgets(cBuffer, MAX_LINESTR-1, fp) != NULL )
	{
		pToken = strtok(cBuffer, cSeps);
		if( pToken == NULL )	break;
		siCount = 0;
		while( pToken != NULL )
		{
			strcpy(cStrBuf, pToken);

			if(		 stricmp(cTableHeaders[siCount], "Name")		== NULL )	strcpy(m_WeaponInfo[siLineNo].cName,		cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "ID")			== NULL )	m_WeaponInfo[siLineNo].siID				= atoi(cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Class")		== NULL )	strcpy(m_WeaponInfo[m_WeaponInfo[siLineNo].siID].cWClass,		cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Left Weapon")	== NULL )	strcpy(m_WeaponInfo[m_WeaponInfo[siLineNo].siID].cLeftWeapon,	cStrBuf);
			else if( stricmp(cTableHeaders[siCount], "Right Weapon")== NULL )	strcpy(m_WeaponInfo[m_WeaponInfo[siLineNo].siID].cRightWeapon,	cStrBuf);

			pToken = strtok( NULL, cSeps );
			siCount++;
		}

		siLineNo++;
		if(siLineNo >= MAX_ITEMINFO)clGrp.Error("19474", "4324975");
		memset(cBuffer, 0, sizeof(cBuffer));
	}
	fclose(fp);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	LoadHelmetInfo()
// Description	:	
BOOL CCharFileLoadManager::LoadHelmetInfo(char *pFileName)
{
	FILE *fp;
	char cBuffer[MAX_LINESTR];
	char cStrBuf[1024];
	SI32 siLineNo = 1;
	SI32 siCount = 0;
	char cSeps[16];
	char *pToken;
	char cTableHeaders[100][30];
	strcpy(cSeps, "\t\n\r");

//	fp = fopen(pFileName, "rt");
	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if(fp == NULL) {	clGrp.Error("CharFileLoadManager_Item", "File Not Found : %s", pFileName);	return FALSE; }
	
	if( fp != NULL )
	{
		// 1줄은 TAG들을 읽어온다.
		memset(cBuffer, 0, sizeof(cBuffer));
		if(fgets(cBuffer, MAX_LINESTR-1, fp) == NULL )	{ fclose(fp); return FALSE; }
		{
			pToken = strtok(cBuffer, cSeps);
			if( pToken == NULL )		{ fclose(fp); return FALSE; }
			siCount = 0;
			while( pToken != NULL )
			{
				strcpy(cTableHeaders[siCount],pToken);
				pToken = strtok( NULL, cSeps );
				siCount++;
			}
		}

		// 2번째 줄로부터 내용을 얻어온다.
		while( fgets(cBuffer, MAX_LINESTR-1, fp) != NULL )
		{
			pToken = strtok(cBuffer, cSeps);
			if( pToken == NULL )	break;
			siCount = 0;
			while( pToken != NULL )
			{
				strcpy(cStrBuf, pToken);

				if(		 stricmp(cTableHeaders[siCount], "Name")		== NULL )	strcpy(m_HelmetInfo[siLineNo].cName,		cStrBuf);
				else if( stricmp(cTableHeaders[siCount], "ID")			== NULL )	m_HelmetInfo[siLineNo].siID					= atoi(cStrBuf);
				else if( stricmp(cTableHeaders[siCount], "Helmet")		== NULL )
				{
					strcpy(m_HelmetInfo[m_HelmetInfo[siLineNo].siID].cHelmet,		cStrBuf);
				}

				pToken = strtok( NULL, cSeps );
				siCount++;
			}
			// 해당 INV, FLP CODE의 중복을 정리해서 설정한다.
			//??? SetItemsInvFlpInfoToShare(ITEM_TYPE_HELMETS, siLineNo);
			siLineNo++;
			if(siLineNo >= MAX_ITEMINFO)clGrp.Error("19474", "4324975re");
			
			memset(cBuffer, 0, sizeof(cBuffer));
		}
		fclose(fp);
	}

	return TRUE;
}




