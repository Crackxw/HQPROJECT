/************************************************************************************************
	파일명 : IOnlineChar.cpp
	담당자 : 이윤석

	현재 Game에서 존재하는 Player의 정보를 관리한다.
************************************************************************************************/
#include <GSL.h>
#include <graphics16.h>
#include "OnlineWorld.h"
#include "OnlineChar-Parser.h"
#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include <DebugTool.h>
#include "OnlineMyData.h"

#include "..\Gersang\maindata.h"
#include "OnlineMegaText.h"
#include ".\CharInfo\CharFileLoadManager.h"

#include	"FieldHeroInfoMgr.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자
//----------------------------------------------------------------------------------------------------------------
IOnlineChar::IOnlineChar()
{
	pclOnlineChar	=	NULL;
	pMyOnlineWorld	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------
IOnlineChar::~IOnlineChar()
{
	Free();	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	온라인 캐릭터의 인터페이스를 초기화 한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::Init(cltOnlineWorld *pWorld)
{
	pMyOnlineWorld	=	pWorld;

	// 캐릭터의 메모리를 할당한다.
	pclOnlineChar	=	new cltOnlineChar[MAX_ONLINE_CHAR];	

	if(pclOnlineChar)	
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	온라인 캐릭터의 인터페이스를 해제한다.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Free()
{	
	if(pclOnlineChar)
	{
		AllFree();

		delete [] pclOnlineChar;
		pclOnlineChar =	NULL;
	}

	pMyOnlineWorld	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 초기화 한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::Init(UI16 uiUniqueID, CHAR *pszID, SI32 siKind, SI16 siX, SI16 siY, DWORD ip, float Ranking, SI32 FightPower, cltOnlineWorld* pWorld,UI16* lpuiEquipWeapon)
{
	SI32	siID;

	// 이미 ID가 있는지 찾아본다.
	siID	=	GetID(uiUniqueID);
	if(siID != 0)	
	{
		// ID가 이미 있다.
		// 추가하지 않아도 된다.
		return FALSE;	
	}

	siID	=	FindEmptyID();	

	if(siID != 0)
	{			
		pclOnlineChar[siID].Init(uiUniqueID, siID, pszID, siKind, siX, siY, ip, Ranking, FightPower, pWorld,lpuiEquipWeapon);				
		return TRUE;
	}
	else
	{
		SetMessage("No more ID can be added.");
		return FALSE;		
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	장착 Item 지정 -제거-
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::SetEquipItem(UI16 uiUniqueID, UI16* lpuiEquipWeapon)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);		

	// 유효한 사용자여야 한다.
	if(siID != 0)
	{	
		return pclOnlineChar[siID].SetEquipItem(lpuiEquipWeapon);
	}	

	return FALSE;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 없앤다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Free(UI16 uiUniqueID)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);		

	// 유효한 사용자여야 한다.
	if(siID != 0)
	{	
		pclOnlineChar[siID].Free();							
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 화면에 그린다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Draw(SI16 siID, BOOL fWater)
{
	pclOnlineChar[siID].Draw(fWater);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 화면에 그린다.(따라다니는 용병용)	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::DrawByVMercenary(SI16 siID, BOOL fWater)
{
	pclOnlineChar[siID].DrawByVMercenary(fWater);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 반투명해서 찍어준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::DrawTransparent(SI16 siID, BOOL fWater)
{
	pclOnlineChar[siID].DrawTransparent(fWater);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Draw Name
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::DrawName(UI16 uiUniqueID, HDC hdc)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
	{	
		pclOnlineChar[siID].DrawName(hdc);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 이름 앞에 Guild 깃발을 찍는다.(만약 좌표가 -1이면 ID 찍는 부분 앞에 찍는다.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::DrawGuildFlag(UI16 uiUniqueID, SI16 ScreenX, SI16 ScreenY)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
	{	
		pclOnlineChar[siID].DrawGuildFlag(ScreenX, ScreenY);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	이름을 찍는다.(hdc를 사용하는 드로우)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::AllDrawName(HDC hdc)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 초기화가 된 캐릭터만을 출력한다.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 공성전 유닛만. 자기 캐릭터 제외
			if ((TRUE == pclOnlineChar[i].IsSiegeWarfareUnit())
				&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID()))
			{
				pclOnlineChar[i].DrawName(hdc);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	대사를 찍는다.(hdc를 사용하는 드로우)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::AllDrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 초기화가 된 캐릭터만을 출력한다.
		if(pclOnlineChar[i].IsInit() == TRUE)
			pclOnlineChar[i].DrawSpeech(lpSurface);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	액션
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Action()
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{	
		// 초기화가 된 캐릭터만을 출력한다.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{				
			pclOnlineChar[i].Action();
		}
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	따라다니는 용병들 액션
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::ActionByVMercenary()
{
	SI32	i;	

	// Map 초기화
	pMyOnlineWorld->pOnlineMap->VMercenaryBufferClear();

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{	
		// 초기화가 된 캐릭터만을 출력한다.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{				
			pclOnlineChar[i].ActionByVMercenary();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	타일 좌표를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::GetPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0)
	{	
		pclOnlineChar[siID].GetPos(psiX, psiY);
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	타일의 변위 좌표를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetRPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetRPos(psiX, psiY);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 목표 위치를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetDPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetDPos(psiX, psiY);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 다음 위치를 얻어온다.	
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetNPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetNPos(psiX, psiY);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 다음 위치를 얻어온다.(따라다니는 용병용)		
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetNPosByVMercenary(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetNPosByVMercenary(psiX, psiY);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 다음 위치의 비율을 얻어온다.	
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetMovePersent(UI16 uiUniqueID, SI16 *ps)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetMovePersent(ps);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 다음 위치의 비율을 얻어온다.(따라다니는 용병용)		
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetMovePersentByVMercenary(UI16 uiUniqueID, SI16 *ps)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetMovePersentByVMercenary(ps);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 명령을 삭제한다.
//----------------------------------------------------------------------------------------------------------------
void	IOnlineChar::ClearOrder(UI16 uiUniqueID)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].ClearOrder();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 명령을 버퍼에 넣는다.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::AddOrder(UI16 uiUniqueID, SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0)	
		return pclOnlineChar[siID].AddOrder(siOrder, siOrderParam1, siOrderParam2);
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 명령을 버퍼에 넣는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::AddOrder(UI16 uiUniqueID, OnOrder Order)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].AddOrder(Order);
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 UniqueID를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	IOnlineChar::GetUniqueID(SI16 siID)
{
	return pclOnlineChar[siID].GetUniqueID();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	UniqueID를 사용하여 ID(문자열)을 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetID(UI16 uiUniqueID, CHAR *pszID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].SetID(pszID);			
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	UniqueID를 사용하여 ID를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::GetID(UI16 uiUniqueID)
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return i;
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	UniqueID를 사용하여 Kind를 얻는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::GetKind(UI16 uiUniqueID)
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetKind();
	}

	return 0;
}

// Imjin Kind를 리턴함. Client에서 사용되는 캐릭터 종류 상수값
SI32	IOnlineChar::GetImjinKind(UI16 uiUniqueID)
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetImjinKind();
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	비어있는 아이디를 찾는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::FindEmptyID()
{
	SI32	i;

/*
// 현재의 캐릭터수를 파악하기 위한 테스트 루틴. 
	SI32 count = 0;
	char buffer[128];
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			count++;
		}
	}
	wsprintf(buffer, "캐릭터 수:%d", count);
	SetMessage(buffer);
*/	
	
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == FALSE)
			return i;
	}

	// 0을 리턴하는 거라면 ID를 찾지 못했다는 것이다.
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Get IP
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	IOnlineChar::GetIP(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetIP();
	}

	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 방향을 구한다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::GetMovingDirection(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetMovingDirection();
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	이 유니크 아이디를 가진 캐릭터가 존재하는지 알아온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::IsExistUniqueID(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(UI16(pclOnlineChar[i].GetUniqueID()) == uiUniqueID)
			return TRUE;			
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	유니크 ID 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	IOnlineChar::GetIPToUniqueID(DWORD IP)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			if(pclOnlineChar[i].GetIP() == IP)
			{
				return pclOnlineChar[i].GetUniqueID();
			}
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	이동중인지 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::IsMoving(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].IsMoving();			
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 이동 시킨다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetMove(UI16 uiUniqueID, SI32 siDX, SI32 siDY)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].SetMove(siDX, siDY);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 그 방향을 한 타일 이동 시킨다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetMoveByDirection(UI16 uiUniqueID, SI32 siDirection)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].SetMoveByDirection(siDirection);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 이동 Flag
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetWalkingFlag(UI16 uiUniqueID, UI16 Flag)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].SetWalkingFlag(Flag);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 대사를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetSpeech(UI16 uiUniqueID, CHAR *pString, BOOL fNotDel, CHAR* pID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].SetSpeech(pString, fNotDel, pID);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	대사가 떠있는지 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::CheckSpeech(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].CheckSpeech();
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	모든 캐릭터를 없앤다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::AllFree()
{
	SI32	i;
	
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			pclOnlineChar[i].Free();
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	사용자의 이름을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	IOnlineChar::GetCharName(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			return pclOnlineChar[i].GetCharName();
		}
	}	

	//EXIT();

	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 전투력을 얻는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineChar::GetFightPower(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			return pclOnlineChar[i].GetFightPower();
		}
	}	

	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 전투력을 지정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::SetFightPower(UI16 uiUniqueID, SI32 FightPower)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			return pclOnlineChar[i].SetFightPower(FightPower);
		}
	}	

	return FALSE;
}

BOOL	IOnlineChar::SetChangeDirection(UI16 uiUniqueID,SI16 siDirection)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].ChangeDirection(siDirection);
		}
	}	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프를 해야하는 상황인가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::IsWarp(UI16 uiUniqueID, SI16 siWarpX, SI16 siWarpY,SI16 siDistance)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].IsWarp(siWarpX, siWarpY,siDistance);
	}
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::Warp(UI16 uiUniqueID, SI16 siWarpX, SI16 siWarpY)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].Warp(siWarpX, siWarpY);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 마우스가 가리키는 캐릭터는지 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::IsPointToChar(SI16 siID, LPPOINT pptMouse, SI32 *psiDistance)
{
	return pclOnlineChar[siID].IsPointToChar(pptMouse, psiDistance);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 수행중인 명령을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OnOrder	IOnlineChar::GetCurrentOrder(UI16 uiUniqueID)
{
	SI32		i;
	OnOrder		OrderNull = {0, 0, 0};

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetCurrentOrder();
	}

	return OrderNull;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 클래스를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineChar::GetClass(UI16 uiUniqueID)
{
	SI32		i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetClass();
	}

	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	내 주위에 있는 캐릭터들의 대한 정보.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::RecvAroundChars(OnResAroundCharInfoMsg *pAroundCharsInfoMsg)
{
	SI32	i, j;
	UI16	uiMyChars[MAX_ONLINE_CHAR];
	UI16	*puiAroundCharsID;
	UI16	*psiCharsPos;

	// 우선 내가 가지고 있는 모든 캐릭터들의 ID를 모은다.
	ZeroMemory(uiMyChars, sizeof(uiMyChars));
	for(i = 0; i < MAX_ONLINE_CHAR; i++)
		uiMyChars[i]	=	pclOnlineChar[i].GetUniqueID();

	puiAroundCharsID	=	(UI16*)((CHAR*)pAroundCharsInfoMsg + sizeof(OnResAroundCharInfoMsg));
	psiCharsPos			=	(UI16*)((CHAR*)puiAroundCharsID + (sizeof(UI16) * pAroundCharsInfoMsg->uiCharNum));

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(uiMyChars[i] != 0)
		{
			for(j = 0; j < pAroundCharsInfoMsg->uiCharNum; j++)
			{
				// 내가 가지고 있는 캐릭터의 id가 있으면 그 부분은 가지고 있다고 체크한다.(0으로 만든다.)				
				if(uiMyChars[i] == puiAroundCharsID[j])
				{
					uiMyChars[i]			=	0;				// 나중에 삭제하기 위해서.
					puiAroundCharsID[j]		=	0;				// 나중에 추가하기 위해서.

					// 해당 캐릭터를 위치로 이동 시킨다.
					pclOnlineChar[i].AddOrder(ON_ORDER_MOVE, psiCharsPos[j * 2], psiCharsPos[j * 2 + 1]);					
					break;
				}
			}
		}
	}

	// 지워야 할 캐릭터를 삭제한다.
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(uiMyChars[i] != 0)
		{
			// 단 나는 삭제하지 않는다
			if(pclOnlineChar[i].GetUniqueID() != pMyOnlineWorld->pMyData->GetMyCharUniqueID())
				pclOnlineChar[i].Free();
		}
	}

	// 내가 모르는 캐릭터는 정보를 달라고 서버에게 요청한다.
	// 지금은 단순히 하나씩만 보낸다. 나중에 한번에 모와서 보낸다.
	for(i = 0; i < pAroundCharsInfoMsg->uiCharNum; i++)
	{
		if(puiAroundCharsID[i] != 0)
		{
			pMyOnlineWorld->RPI.Add(puiAroundCharsID[i]);			
			//DWriteStringToFile("IOnlineChar::RecvAroundChars 에서 플레이어의 정보를 달라고 요청한다.");
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	플레이어의 정보를 설정해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo)
{
	SI32		i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == lpBaseInfo->uiAccount)
			pclOnlineChar[i].SetPlayerInfo(lpBaseInfo, fType, lpPlayerInfo);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	플레이어의 정보를 설정해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetPlayerInfo(UI16 uiUniqueID, OnCharPlayerDetailInfo* lpPlayerInfo)
{
	SI32		i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].SetPlayerInfo(lpPlayerInfo);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터에게 아이템을 장착시킨다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, UI16 uiUniqueID, BOOL bEffectSound)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetEquip(bEquip, siEquipKind, siEquipID, bEffectSound);
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	(평화/전투 모드) 현재 장착하고 있는 무기 ID를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	IOnlineChar::GetWeaponDrawID(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetWeaponDrawID();
	}
	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	(평화/전투 모드) 현재 장착하고 있는 무기 ID를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetWeaponDrawID(UI16 uiUniqueID, SI16 siWeaponDrawID)
{
	SI32	siID;
	
	siID	= GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetWeaponDrawID(siWeaponDrawID);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Set Battle Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetBattleFlag(UI16 uiUniqueID, UI08 Flag)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetBattleFlag(Flag);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get Battle Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	IOnlineChar::GetBattleFlag(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetBattleFlag();
	}
	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Imoticon 출력
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetImoticon(UI16 uiUniqueID, UI16 uiImoticon)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetImoticon(uiImoticon);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Set Find Target Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetFindTargetFlag(UI16 uiUniqueID, UI08 Flag)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetFindTargetFlag(Flag);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 Data 지정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetGuildData(UI16 uiUniqueID, char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetGuildData(lpGuildName, siGuildInGrade, siGuildFlag);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 Name 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
char*	IOnlineChar::GetGuildName(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetGuildName();
	}

	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 직급 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineChar::GetGuildInGrade(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
	{
		return pclOnlineChar[siID].GetGuildInGrade();
	}
	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 깃발 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineChar::GetGuildFlag(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetGuildFlag();
	}

	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	적대 길드인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::SetIsHostilityGuild(UI16 uiUniqueID, UI08 uiIsHostilityGuild)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].SetIsHostilityGuild(uiIsHostilityGuild);
	}

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	적대 길드인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	IOnlineChar::GetIsHostilityGuild(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetIsHostilityGuild();
	}

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로 선택된 캐릭터여부를 설정 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void	IOnlineChar::SetNewSelectedFlag(UI16 uiUniqueID, BOOL flag)
{
	
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) pclOnlineChar[siID].SetNewSelectedFlag(flag);
	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로 선택된 캐릭터여부를 얻어옴.  
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::GetNewSelectedFlag(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetNewSelectedFlag();
	}

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	보여줄지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::Show(UI16 uiUniqueID, BOOL fShow)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
	{
		return pclOnlineChar[siID].Show(fShow);
	}

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	보여줄지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::GetShow(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetShow();
	}

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	개인상점 아이템 정보 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SellItemInfo	*IOnlineChar::GetBoothItem(UI16 uiUniqueID, UI08 uiSlot)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetBoothItem(uiSlot);
	}
	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	개인상점 아이템 정보 지정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetBoothItem(UI16 uiUniqueID, UI08 uiSlot, SellItemInfo ItemInfo)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetBoothItem(uiSlot, ItemInfo);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	개인상점 개설 여부 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineChar::IsBoothOpen(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].IsBoothOpen();
	}
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	개인상점 개설 여부 지정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineChar::SetBoothOpen(UI16 uiUniqueID, BOOL bOpenBooth)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetBoothOpen(bOpenBooth);
	}
}

SI08	IOnlineChar::GetDataType(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetDataType();
	}

	return 0;
}

void	IOnlineChar::SetDataType(UI16 uiUniqueID, SI08 fDataType)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetDataType(fDataType);
	}
}

BOOL	IOnlineChar::SetMercenary(UI16 uiUniqueID, SI32 siKind)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].SetMercenary(siKind);
	}

	return FALSE;
}

SI32	IOnlineChar::GetMercenary(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetMercenary();
	}

	return 0;
}

void	IOnlineChar::GetEquip(UI16 uiUniqueID, UI16* lpuiEquipWeapon)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].GetEquip(lpuiEquipWeapon);
	}
}

void	IOnlineChar::SetChangeEffect(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetChangeEffect();
	}
}

VOID IOnlineChar::SetAttackMode(UI16 uiUniqueID, BOOL b)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		pclOnlineChar[siID].SetAttackMode(b);
}

BOOL IOnlineChar::IsAttackMode(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].IsAttackMode();

	return FALSE;
}

int	IOnlineChar::GetScore(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].GetScore();

	return 0;
}

int IOnlineChar::GetRank(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].GetRank();

	return 0;
}

void IOnlineChar::SetRank(UI16 uiUniqueID, UI16 Rank)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		pclOnlineChar[siID].SetRank((int)Rank);
}

void IOnlineChar::SetScore(UI16 uiUniqueID, int Score)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		pclOnlineChar[siID].SetScore(Score);
}

BOOL IOnlineChar::IsInvalidBattle(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].IsInvalidBattle();

	return FALSE;
}

void IOnlineChar::SetSiegeWarfare_Hp(UI16 uiUniqueID,SI16 siFieldHp)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_Hp(siFieldHp);
}

void IOnlineChar::SetSiegeWarfare_Damage(UI16 uiUniqueID,SI16 siDamage)
{	
	SI32 siID;
	
	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_Damage(siDamage);
}

SI16 IOnlineChar::GetSiegeWarfare_Hp(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_Hp();

	return 0;
}

BOOL IOnlineChar::IsFieldAlive(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].IsFieldAlive();
	return 0;
}

// 시작 /////////////////////////////////////////
// robypark 2004/10/27 15:22
// 공성전 유닛 체력, 보급품, 공격지연 시간 함수
// 공성전 유닛 최대 체력치 설정
void IOnlineChar::SetSiegeWarfare_MaximumHP(UI16 uiUniqueID, SI16 siMaximumHP)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_MaximumHP(siMaximumHP);
}

// 공성전 유닛 최대 체력치 얻기
SI16 IOnlineChar::GetSiegeWarfare_MaximumHP(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_MaximumHP();

	return 0;
}

// 공성전 유닛 보급품 현재량 설정
void IOnlineChar::SetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID, SI32 siCurrentSupplyGoodsAmount)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_CurrentSupplyGoodsAmount(siCurrentSupplyGoodsAmount);
}

// 공성전 유닛 보급품 현재량 얻기
SI32 IOnlineChar::GetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_CurrentSupplyGoodsAmount();

	return 0;
}

// 공성전 유닛 보급품 최대량 설정
void IOnlineChar::SetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID, SI32 siSupplyGoodsAmount)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_SupplyGoodsAmount(siSupplyGoodsAmount);
}

// 공성전 유닛 보급품 최대량 얻기
SI32 IOnlineChar::GetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_SupplyGoodsAmount();

	return 0;
}

// 공성전 유닛의 공격 지연 시간 설정
void IOnlineChar::SetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID, DWORD dwLastAttackDelay)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfareUnit_LastAttackDelay(dwLastAttackDelay);
}

// 공성전 유닛의 경과된 공격 지연 시간 설정
void IOnlineChar::SetSiegeWarfareUnit_PastAttackDelay(UI16 uiUniqueID, DWORD dwPastAttackDelay)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfareUnit_PastAttackDelay(dwPastAttackDelay);
}

// 공성전 유닛의 공격 지연 시간 얻기
DWORD IOnlineChar::GetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfareUnit_LastAttackDelay();

	return 0;
}
// 끝 /////////////////////////////////////////	

// robypark 2004/11/2 17:58
// 공성전 유닛으로 변신 효과 사용
void	IOnlineChar::SetSiegeWarfare_TransformEffect(UI16 uiUniqueID, SI32 siSiegeWarfareTransformType)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_TransformEffect(siSiegeWarfareTransformType);
}

// robypark 2004/12/2 19:2
// 공성전 유닛이 보급품 관련 효과 설정. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
void	IOnlineChar::SetSiegeWarfare_EffectSupplyGoods(UI16 uiUniqueID, SI32 siEffectSupplyGoodsType)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_EffectSupplyGoods(siEffectSupplyGoodsType);
}

// robypark 2004/11/2 18:6
// 공성전 유닛 보급품 자동 소모(1%)
void	IOnlineChar::WasteSiegeWarfareSupplyGoods(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].WasteSiegeWarfareSupplyGoods();
}

// robypark 2004/11/3 15:24
// 공성전 유닛이 마을로 부터 공격받는 효과 설정
void	IOnlineChar::SetSiegeWarfare_VillageCountAttack(UI16 uiUniqueID, SI32 siEffectVillageConterAttackType)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_VillageCountAttack(siEffectVillageConterAttackType);
}

// robypark 2004/11/24 18:31
// 공성전 견고한 방어 효과 사용
void	IOnlineChar::SetSiegeWarfare_Evasion(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_Evasion();
}

// robypark 2004/11/24 18:31
// 공성전 치명적 공격 효과 사용
void	IOnlineChar::SetSiegeWarfare_DeathBlow(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_DeathBlow();
}

void	IOnlineChar::SetCaptain(UI16 uiUniqueID,BOOL bCaptain)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetCaptain(bCaptain);
}

BOOL	IOnlineChar::GetCaptain(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetCaptain();

	return 0;
}
/*
void	IOnlineChar::SetSupplyCharge(UI16 uiUniqueID, SI16 siSupply)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSupplyCharge(siSupply);
}

SI16	IOnlineChar::GetSupplyCharge(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSupplyCharge();

	return 0;
}
*/
// robypark 2004/11/12 21:27
// 공성전 유닛의  체력 및 보급품 수치(%) 그리기
void	IOnlineChar::DrawSiegeWarfareUnit_StatusBar(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].DrawSiegeWarfareUnit_StatusBar();
}

// robypark 2004/11/13 16:41
// 모든 공성전 유닛의  체력 및 보급품 수치(%) 그리기
void	IOnlineChar::AllDrawSiegeWarfareUnit_StatusBar(void)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 초기화가 된 캐릭터만을 출력한다.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 공성전 유닛만. 자기 자신은 제외
			if ((TRUE == pclOnlineChar[i].IsSiegeWarfareUnit())
				&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID()))
			{
				pclOnlineChar[i].DrawSiegeWarfareUnit_StatusBar();
			}
		}
	}
}

// robypark 2004/11/22 10:56
// 모든 공성전 유닛의 상단 깃발 그리기
void	IOnlineChar::AllDrawSiegeWarfareUnit_GuildFlag(void)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 초기화가 된 캐릭터만을 출력한다.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 공성전 유닛만. 자기 자신은 제외
			if ((TRUE == pclOnlineChar[i].IsSiegeWarfareUnit())
				&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID()))
			{
				pclOnlineChar[i].DrawGuildFlag();
			}
		}
	}
}

// robypark 2004/12/16 14:33
// 공성전 유닛인가?
BOOL	IOnlineChar::IsSiegeWarfareUnit(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		return pclOnlineChar[siID].IsSiegeWarfareUnit();

	return FALSE;
}

// robypark 2004/12/17 17:20
// 사용자가 현재 수성 인터페이스 상에 있는가?
BOOL	IOnlineChar::IsInVillageDefenceInterface(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		return pclOnlineChar[siID].IsInVillageDefenceInterface();

	return FALSE;
}

// robypark 2004/12/17 17:20
// 캐릭터가 현재 수성 인터페이스 상에 있는가? 설정
void	IOnlineChar::SetInVillageDefenceInterface(UI16 uiUniqueID, BOOL bIsInVillageDefenceInterface)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		pclOnlineChar[siID].SetInVillageDefenceInterface(bIsInVillageDefenceInterface);
}

// robypark 2005/1/14 18:39
// 캐릭터를 그릴 때 그려지기 시작하는 위치 얻기
void	IOnlineChar::GetCharacterDrawCenterPos(UI16 uiUniqueID, SI16 &siCenterX, SI16 &siCenterY)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		pclOnlineChar[siID].GetCharacterDrawCenterPos(siCenterX, siCenterY);
}

// robypark 2005/1/14 19:8
// 모든 주인공 캐릭터를 피난민 혹은 일반 주인공 캐릭터로 변신시킨다.
void	IOnlineChar::AllHeroCharacterTransformTo(void)
{
	SI32	i;

	// 본 맵이 아니라면 피난민 관련 캐릭터 변신 처리를 하지 않는다.
	if (0 != pMyOnlineWorld->pOnlineMap->m_MapIndex)	// 던전, 저잣거리 등의 맵이라면,
		return;

	// 사용자의 공성이 없다면,
	if (FALSE == pMyOnlineWorld->pMyData->IsSiegeWarfareDay())
		return;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 초기화가 된 캐릭터만
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 자기 캐릭터 제외
			if (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID())
			{
				DWORD		IP;
				CHAR		TempBuffer[128];
				
				// 캐릭 정보 얻기
				UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
				SI16 siX, siY;
				SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
				CHAR TempBuffer1[128];
				ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

				// 캐릭터 기본 정보 얻기
				uiUniqueID		= pclOnlineChar[i].GetUniqueID();
				uiKind			= pclOnlineChar[i].GetKind();

				// 주인공 캐릭터인가?
				// 주인공 관련 데이터 관리자 얻기
				CFieldHeroInfo *pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(uiKind);
				// 주인공 캐릭터가 아니라면, 변신할 필요가 없다.
				if (NULL == pFieldHeroInfo)
					continue;

				// 공성전 유닛인가?
				LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(uiKind);
				// 공성전 유닛일 경우, 변신할 필요가 없다.
				if (lpsSiegeUnitTable)
					continue;

				IP				= GetID(uiUniqueID);
				siFightPower	= pclOnlineChar[i].GetFightPower();
				siGuildInGrade	= pclOnlineChar[i].GetGuildInGrade();
				siGuildFlag		= pclOnlineChar[i].GetGuildFlag();
				siMercenaryKind = pclOnlineChar[i].GetMercenary();

				// 이름
				CopyMemory(TempBuffer, pclOnlineChar[i].GetCharName(), ON_ID_LENGTH);
				// 길드명
				if(pclOnlineChar[i].GetGuildName())
					CopyMemory(TempBuffer1, pclOnlineChar[i].GetGuildName(), ON_ID_LENGTH);

				// 캐릭터 위치 얻기
				pclOnlineChar[i].GetPos(&siX, &siY);

				// 캐릭터 장비 얻기
				pclOnlineChar[i].GetEquip(uiEquipWeapon);
				
				// 캐릭터 삭제
				pclOnlineChar[i].Free();

				// 캐릭터 생성(초기화)
				Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, pMyOnlineWorld, (UI16*)uiEquipWeapon);

				// 장비 설정
				SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);
				
				// 상단 및 용병 데이터 설정
				SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
				SetMercenary(uiUniqueID, siMercenaryKind);
			}
		}
	}
}

// robypark 2005/2/17 21:44
// 마우스 커서 아래 캐릭터인지를 설정한다.
void	IOnlineChar::SetCursorHoverCharacter(UI16 uiUniqueID, BOOL bHover/* = FALSE*/)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		pclOnlineChar[siID].SetCursorHoverCharacter(bHover);
}
