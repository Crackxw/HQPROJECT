

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoExecutor.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "stdafx.h"
#include "AdoExecutor.h"
#include "JOCharListUpdateConfig.h"
#include "..\\JOCommon\\AdoRecordset.h"
#include "..\\JOCommon\\AdoCommand.h"
#include "Structure.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////

CAdoExecutor::CAdoExecutor() : 
m_pAdoConnection(NULL), 
m_bInit(FALSE)
{
	Initialize();
}

//////////////////////////////////////////////////////////////////////

CAdoExecutor::~CAdoExecutor()
{
	Finalize();
}

//////////////////////////////////////////////////////////////////////

void CAdoExecutor::Initialize()
{
	if(m_bInit == TRUE)
		return;

//	CoInitialize(NULL);

	m_pAdoConnection = new CAdoConnection;

//	CJOCharListUpdateConfig Config;

//	Config.SetConfigFileName(GERSANG_CHAR_LIST_UPDATE_INFO_FILE);

//	strcpy(m_szAdoConnectionString, Config.DBConnectionString(1));
}

//////////////////////////////////////////////////////////////////////

void CAdoExecutor::Finalize()
{

	if(m_bInit == FALSE)
		return;

	delete m_pAdoConnection;
	m_pAdoConnection = NULL;

//	CoUninitialize();
}

//////////////////////////////////////////////////////////////////////

BOOL CAdoExecutor::IsOpen()
{
	return m_pAdoConnection->IsOpen()  == true ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////

BOOL CAdoExecutor::Connect(char* pAdoConnString)
{
	BOOL bRetVal = FALSE;
	
	if(m_pAdoConnection->Open(pAdoConnString) == ADO_CONNECT_SUCCEED)
	{
		bRetVal = TRUE;
	}
	else
	{
		bRetVal = FALSE;
		m_LogWriter.PrintF("Error_ADO DB Connect.Log", 
			"DB Connect failed\n Connection String = %s", pAdoConnString);
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

BYTE CAdoExecutor::Execution(DWORD dwType, void* pMsg)
{
	BYTE byRetVal = FALSE;

	switch(dwType)
	{
		case 1000 :
			{		
				byRetVal = CharListUpdate(pMsg);
			}
			break;

		case 1001 : 
			{
				byRetVal = Set84YearBirthdayUser(pMsg);
			}
			break;
		default : 
			{
				byRetVal = 0;
			}
			break;
	}

	return byRetVal;
}

//////////////////////////////////////////////////////////////////////

BYTE CAdoExecutor::CharListUpdate(void* pMsg)
{
	BYTE byResult = 0;
	CharHead*	pInfo = (CharHead*)pMsg;

	SHORT		snResult = 0;
	bool		bIsEof = false;
	char		szQuery[4096];
	char		szQuery1[2048];
	char		szQuery2[2048];
	memset(szQuery, 0, sizeof(szQuery));
	memset(szQuery1, 0, sizeof(szQuery1));
	memset(szQuery2, 0, sizeof(szQuery2));

	CAdoRecordset   cAdoRecordset(m_pAdoConnection);

	sprintf(szQuery, 
		"select top 1 CharacterID from CharacterList where CharacterID = %d", pInfo->uiID);

	bool bOpen = cAdoRecordset.Open(szQuery, CAdoRecordset::openQuery);

	if(bOpen == FALSE)
	{
		return 0;
	}

	memset(szQuery, 0, sizeof(szQuery));

	// update
	if(cAdoRecordset.GetRecordCount() > 0)
	{
		sprintf(szQuery1, "Update CharacterList set Name = '%s', [Level] = %d, CurExp = %d, MyExp = %d, Nation = %d, Str = %d, Dex = %d, Vit = %d, [Int] = %d, AC = %d, DamageRegist = %d, MagicRegist = %d,", 
			pInfo->szCharName, pInfo->siLv, pInfo->Parameta.siCurExp, 
			pInfo->uiMyExp, pInfo->siNation, pInfo->Parameta.siStr, 
			pInfo->Parameta.siDex, pInfo->Parameta.siVit, pInfo->Parameta.siInt, 
			pInfo->Parameta.siAC, pInfo->Parameta.DamageResist, 
			pInfo->Parameta.MagicResist	);

		sprintf(szQuery2, " EquipMinDamage = %d, EquipMaxDamage = %d, MoveSpeed = %d, Life = %d, Mana = %d, Cost = %d, Bonus = %d where CharacterID = %d", 
			pInfo->Parameta.EquipMinDamage, pInfo->Parameta.EquipMaxDamage, 
			pInfo->siMoveSpeed, pInfo->Parameta.siLife, pInfo->Parameta.siMana, pInfo->siCost, 
			pInfo->Parameta.siBonus, pInfo->uiID);

		_stprintf(szQuery, "%s%s", szQuery1, szQuery2);
		TRACE("%s\n", szQuery);

		byResult = 1;
	}
	// insert
	else
	{
		sprintf(szQuery, "Insert into CharacterList(CharacterID, Name, [Level], CurExp, MyExp, Nation, Str, Dex, Vit, [Int], AC, DamageRegist, MagicRegist, EquipMinDamage, EquipMaxDamage, MoveSpeed, Life, Mana, Cost, Bonus) values(%d, '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", 
			pInfo->uiID,
			pInfo->szCharName, pInfo->siLv, pInfo->Parameta.siCurExp, 
			pInfo->uiMyExp, pInfo->siNation, pInfo->Parameta.siStr, 
			pInfo->Parameta.siDex, pInfo->Parameta.siVit, pInfo->Parameta.siInt, 
			pInfo->Parameta.siAC, pInfo->Parameta.DamageResist, 
			pInfo->Parameta.MagicResist, 
			pInfo->Parameta.EquipMinDamage, pInfo->Parameta.EquipMaxDamage, 
			pInfo->siMoveSpeed, pInfo->Parameta.siLife, pInfo->Parameta.siMana, pInfo->siCost, 
			pInfo->Parameta.siBonus);

		byResult = 2;


		m_LogWriter.PrintF("Character Data.Log", "%d, '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			pInfo->uiID,
			pInfo->szCharName, pInfo->siLv, pInfo->Parameta.siCurExp, 
			pInfo->uiMyExp, pInfo->siNation, pInfo->Parameta.siStr, 
			pInfo->Parameta.siDex, pInfo->Parameta.siVit, pInfo->Parameta.siInt, 
			pInfo->Parameta.siAC, pInfo->Parameta.DamageResist, 
			pInfo->Parameta.MagicResist, 
			pInfo->Parameta.EquipMinDamage, pInfo->Parameta.EquipMaxDamage, 
			pInfo->siMoveSpeed, pInfo->Parameta.siLife, pInfo->Parameta.siMana, pInfo->siCost, 
			pInfo->Parameta.siBonus);

		

		TRACE("%s\n", szQuery);
	}

	bOpen = cAdoRecordset.Open(szQuery, CAdoRecordset::openQuery);

	if(bOpen == FALSE)
	{
		return 0;
	}
	
	return byResult;
}

//////////////////////////////////////////////////////////////////////
// 84년생 유저 데이타

BYTE CAdoExecutor::Set84YearBirthdayUser(void* pMsg)
{
	char* pUserID = (char*)(pMsg);

	CAdoCommand cAdoCommand;
	BYTE byRetVal = 1;

	ADO_BEGIN()
		cAdoCommand.Init(m_pAdoConnection->GetActiveConnection(), "SP_Set84YearBirthdayUser");
		cAdoCommand.SetParameterValue(1, (_bstr_t)	pUserID);		
		cAdoCommand.Execute(NULL, NULL, adCmdStoredProc);

		byRetVal = cAdoCommand.GetParameterValue(0);
	ADO_ON_ERROR(e)
		byRetVal = 1;
		OnADOError(e, "SP_Set84YearBirthdayUser");
	ADO_END()

	return byRetVal;
}

//////////////////////////////////////////////////////////////////////

void CAdoExecutor::OnADOError(_com_error& e, char* szText)
{
	_bstr_t bstrSource		(e.Source());
	_bstr_t bstrDescription	(e.Description());
	char	szSource[256] = {0,};
	char	szDescription[512] = {0,};
	char	szBuf[1024] = {0,};

	strncpy(szSource, bstrSource, 256);
	strncpy(szDescription, bstrDescription, 512);
	
	sprintf(szBuf, "meaning = %lx\nSource = %s\nDescription = %s", 
		szSource, szDescription);

	m_LogWriter.PrintF("AdoError.Log", "DB Error: %s\nCode = %lx\n%s", 
		szText, e.Error(), szBuf );
}

//////////////////////////////////////////////////////////////////////