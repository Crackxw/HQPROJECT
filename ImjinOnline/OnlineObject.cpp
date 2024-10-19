/************************************************************************************************
	파일명 : OnlineObject.cpp
	담당자 : 이윤석

	Map에서 사용하는 Object 하나를 관리함
************************************************************************************************/
#include <GSL.h>
#include <main.h>
#include <OnlineWorld.h>
#include <DebugTool.h>
#include <ETC.h>

#include "..\Gersang\music.h"

#include "OnlineVillage-Parser.h"
//#include "OnlinePortal-Parser.h"
#include "OnlineObject.h"
#include "OnlineText.h"
#include "OnlineResource.h"

#include "OnlineFont.h"
#include "OnlineMegaText.h"


//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
OnlineObject::OnlineObject()
{
	ZeroMemory(szName, sizeof(szName));

	m_pszMark						=	NULL;				
	siID							=	0;
	siKind							=	0;
	siObjectKindIndex				=	0;
	siAtb							=	0;			// 속성.

	siFile							=	0;
	siFont							=	0;	
	siFile2							=	0;
	siFont2							=	0;	
	siSprCenterX					=	0;
	siSprCenterY					=	0;

	siX								=	0;
	siY								=	0;

	siXsize							=	0;
	siYsize							=	0;
	
	siVillageKind					=	0;		// 마을이라면 어떤 마을인가?
	pMyOnlineWorld					=	NULL;
	pOnlineVillageDataOfObject		=	NULL;	
	pOnlinePortalDataOfObject		=	NULL;	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
OnlineObject::~OnlineObject()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Init(SI16 siID, SI16 siKind, SI16 siX, SI16 siY, cltOnlineWorld *pOnlineWorld)
{
	OnlineObject::siID				=	siID;
	OnlineObject::siKind			=	siKind;
	OnlineObject::siX				=	siX;
	OnlineObject::siY				=	siY;
	pMyOnlineWorld					=	pOnlineWorld;			
	OnlineObject::siObjectKindIndex	=	pMyOnlineWorld->pIOnlineObjectKI->GetIndex(siKind);

	pMyOnlineWorld->pIOnlineObjectKI->GetNameFromIndex(siObjectKindIndex, szName);

	m_pszMark						=	pMyOnlineWorld->pIOnlineObjectKI->GetMarkFromIndex(siObjectKindIndex);
	siXsize							=	pMyOnlineWorld->pIOnlineObjectKI->GetXsizeFromIndex(siObjectKindIndex);		
	siYsize							=	pMyOnlineWorld->pIOnlineObjectKI->GetYsizeFromIndex(siObjectKindIndex);	

	siFile							=	pMyOnlineWorld->pIOnlineObjectKI->GetFileFromIndex(siObjectKindIndex);
	siFont							=	pMyOnlineWorld->pIOnlineObjectKI->GetFontFromIndex(siObjectKindIndex);	
	
	siSprCenterX					=	pMyOnlineWorld->pIOnlineObjectKI->GetSprCenterXFromIndex(siObjectKindIndex);
	siSprCenterY					=	pMyOnlineWorld->pIOnlineObjectKI->GetSprCenterYFromIndex(siObjectKindIndex);

	siAtb							=	pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex);	
	//siVillageKind					=	pMyOnlineWorld->pIOnlineObjectKI->GetVillageKind(siKind);	
	
	// 맵에는 이미 설정되어 있으므로 맵에 설정할 필요가 없다.	
	//pOnlineWorld->pOnlineMap->SetID(siX, siY, 1, 1, 0, siID, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.(마을인 경우)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Init(SI16 siID, VillageHeader *pVillageHeader, cltOnlineWorld *pOnlineWorld)
{	
//	char		TempBuffer[256];
	XSPR		*pVillageXspr;
//	SI32		siDotX1, siDotY1;	
//	SI32		siDotX2, siDotY2;


	OnlineObject::siID				=	siID;	
	OnlineObject::siKind			=	0;
	OnlineObject::siObjectKindIndex	=	0;
	pMyOnlineWorld					=	pOnlineWorld;	
	
	// 마을일 경우에는 마을만이 쓰는 데이타를 따로 메모리를 할당받아 보관해둔다.
	pOnlineVillageDataOfObject	=	new OnlineVillageDataOfObject;	
	pOnlineVillageDataOfObject->uiVillageCode               = pVillageHeader->uiVillageCode;
	pOnlineVillageDataOfObject->uiVillageStateCode          = pOnlineWorld->GetVillageStateCode(pVillageHeader->uiVillageCode);
	pOnlineVillageDataOfObject->pVillageHeader              = pVillageHeader;
	pOnlineVillageDataOfObject->siVillageExplosionFlag      = -1;
	pOnlineVillageDataOfObject->siVillageExplosionSoundWait = 0;
	
	// 마을의 이름을 얻어온다.
	strcpy(szName, pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));	

	siX								=	pVillageHeader->siX;
	siY								=	pVillageHeader->siY;

	siXsize							=	pVillageHeader->siSize;
	siYsize							=	pVillageHeader->siSize;	
	
	// 국가 Code에 따른 이미지 로드. default
	pOnlineVillageDataOfObject->siGuildFlagXPos					= 0;
	pOnlineVillageDataOfObject->siGuildFlagYPos					= -63;
	pVillageXspr												=	pMyOnlineWorld->pOnlineResource->GetxsprObject(pMyOnlineWorld->pOnlineResource->GetObjectFileIndex(pOnlineVillageDataOfObject->pVillageHeader->szImageFile));
	siSprCenterX												=	pVillageXspr->Header.Xsize / 2;
	siSprCenterY												=	pVillageXspr->Header.Ysize / 2;
	pOnlineVillageDataOfObject->m_ssSurrenderFlagPosX			= 0;
	pOnlineVillageDataOfObject->m_ssSurrenderFlagPosY			= -63;
	pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX	= 117;
	pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY	= 180;
	pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX		= 117;
	pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY		= 180;
	pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosX	= 69;
	pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosY	= 45;

	// 국가와 마을 크기에 따른 이미지 파일명과 마을 크기 얻기
	pMyOnlineWorld->pOnlineResource->GetVillageImageFilename(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->pVillageHeader->szImageFile, pOnlineVillageDataOfObject->siVillageSize,
																	pOnlineVillageDataOfObject->szVillageFileName, pOnlineVillageDataOfObject->szVillageFileNameHalfE);

	// 국가와 마을 크기에 따른 이미지 좌표 정보 헤더 얻기
	LPOnlineResource_VillageImagePositionInfo pVillageImagePositionInfo = pMyOnlineWorld->pOnlineResource->GetVillageImagePositionInfoHeader(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->siVillageSize);

	if (pVillageImagePositionInfo)
	{
		pOnlineVillageDataOfObject->siGuildFlagXPos					= pVillageImagePositionInfo->m_ssVillageGuildFlagPosX;
		pOnlineVillageDataOfObject->siGuildFlagYPos					= pVillageImagePositionInfo->m_ssVillageGuildFlagPosY;
		siSprCenterX												= pVillageImagePositionInfo->m_ssVillageDrawCenterX;
		siSprCenterY												= pVillageImagePositionInfo->m_ssVillageDrawCenterY;
		pOnlineVillageDataOfObject->m_ssSurrenderFlagPosX			= pVillageImagePositionInfo->m_ssSurrenderFlagPosX;
		pOnlineVillageDataOfObject->m_ssSurrenderFlagPosY			= pVillageImagePositionInfo->m_ssSurrenderFlagPosY;
		pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX	= pVillageImagePositionInfo->m_ssVillageGarrisionGaugePosX;
		pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY	= pVillageImagePositionInfo->m_ssVillageGarrisionGaugePosY;
		pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX		= pVillageImagePositionInfo->m_ssVillageDefenceGaugePosX;
		pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY		= pVillageImagePositionInfo->m_ssVillageDefenceGaugePosY;
		pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosX	= pVillageImagePositionInfo->m_ssEnabledOccupyVillagePosX;
		pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosY	= pVillageImagePositionInfo->m_ssEnabledOccupyVillagePosY;
	}

	siFile							=	pOnlineWorld->pOnlineResource->GetObjectFileIndex((char*)pOnlineVillageDataOfObject->szVillageFileName);
	siFont							=	0;
	siFile2							=	pOnlineWorld->pOnlineResource->GetObjectFileIndex((char*)pOnlineVillageDataOfObject->szVillageFileNameHalfE);
	siFont2							=	0;	
/*	
	pVillageXspr					=	pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);

	pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(siX, siY, siDotX1, siDotY1);
	pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(siX + (siXsize / 2), siY + (siYsize / 2), siDotX2, siDotY2);
	siDotY2							+=	ONLINE_TILE_YSIZE_HALF;

	// robypark 2004/12/9 14:45
	// 마을 그림의 중점 위치를 일정하게 하기 위하여 마을 국가코드 및 크기 설정에서 결정하도록 수정
	siSprCenterX					=	pVillageXspr->Header.Xsize / 2;
	siSprCenterY					=	pVillageXspr->Header.Ysize / 2;
*/
	siAtb							=	ATB_OBJECT_VILLAGE;	


	// 맵에 설정한다.

	//pMyOnlineWorld->pOnlineMap->SetID(siX, siY, siXsize, siYsize, 0, siID, 0, 0);
//	pMyOnlineWorld->pOnlineMap->SetColl(siX, siY, siXsize, siYsize, TRUE);
	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.(Portal인 경우)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Init(SI16 siID, PortalHeader *pPortalHeader, cltOnlineWorld *pOnlineWorld)
{	
	char*		lpText;

	pMyOnlineWorld					=	pOnlineWorld;	
	OnlineObject::siID				=	siID;	
	OnlineObject::siKind			=	pPortalHeader->siPortalImage;
	OnlineObject::siObjectKindIndex	=	pMyOnlineWorld->pIOnlineObjectKI->GetIndex(pPortalHeader->siPortalImage);
	
	// Portal일 경우에는 Portal만이 쓰는 데이타를 따로 메모리를 할당받아 보관해둔다.
	pOnlinePortalDataOfObject	=	new OnlinePortalDataOfObject;
	ZeroMemory(pOnlinePortalDataOfObject, sizeof(OnlinePortalDataOfObject));
	pOnlinePortalDataOfObject->siPortalCode = pPortalHeader->siPortalCode;
	
	// 마을의 이름을 얻어온다.
	lpText = pMyOnlineWorld->pOnlineText->Get(pPortalHeader->siPortalDescriptionCode);
	if(lpText)
	{
		ZeroMemory(szName, sizeof(szName));
		strncpy(szName, pMyOnlineWorld->pOnlineText->Get(pPortalHeader->siPortalDescriptionCode), 31);	
		strcpy((char*)pOnlinePortalDataOfObject->szPortalFileName, pMyOnlineWorld->pOnlineText->Get(pPortalHeader->siPortalDescriptionCode));
	}
	pOnlinePortalDataOfObject->uifTileDraw = (UI08)pPortalHeader->siTileFlag;

	siX								=	pPortalHeader->siPortalMapXPosition;
	siY								=	pPortalHeader->siPortalMapYPosition;

	siXsize							=	pMyOnlineWorld->pIOnlineObjectKI->GetXsizeFromIndex(siObjectKindIndex);		
	siYsize							=	pMyOnlineWorld->pIOnlineObjectKI->GetYsizeFromIndex(siObjectKindIndex);		
	
	siFile							=	pMyOnlineWorld->pIOnlineObjectKI->GetFileFromIndex(siObjectKindIndex);
	siFont							=	pMyOnlineWorld->pIOnlineObjectKI->GetFontFromIndex(siObjectKindIndex);	
	
	siSprCenterX					=	pMyOnlineWorld->pIOnlineObjectKI->GetSprCenterXFromIndex(siObjectKindIndex);
	siSprCenterY					=	pMyOnlineWorld->pIOnlineObjectKI->GetSprCenterYFromIndex(siObjectKindIndex);
	siAtb							=	ATB_OBJECT_VILLAGE;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	해제한다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Free()
{
	if(pOnlineVillageDataOfObject)
	{
		delete pOnlineVillageDataOfObject;
		pOnlineVillageDataOfObject	=	NULL;
	}
	if(pOnlinePortalDataOfObject)
	{
		delete pOnlinePortalDataOfObject;
		pOnlinePortalDataOfObject	=	NULL;
	}

	pMyOnlineWorld	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	화면에 그린다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Draw(SI16 SelectID)
{	
	if(pMyOnlineWorld == NULL) return;
	if(pOnlinePortalDataOfObject)
	{
		if(pOnlinePortalDataOfObject->uifTileDraw)
		{
			return;
		}
	}

	SI32	siDotDrawBaseX, siDotDrawBaseY;	
	SI32	siSrcDotDrawBaseX, siSrcDotDrawBaseY;	
	XSPR	*pObjectXspr;
	

	// 찍는 기준 도트 위치를 구한다.
	if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siX, siY, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == FALSE)
	{		
		return;
	}		

	pObjectXspr	=	pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);	
	if(pObjectXspr)
	{
		siDotDrawBaseX	=	siSrcDotDrawBaseX -  siSprCenterX - pMyOnlineWorld->pOnlineMap->GetRX();
		siDotDrawBaseY	=	siSrcDotDrawBaseY -  siSprCenterY - pMyOnlineWorld->pOnlineMap->GetRY();
			
		if(IsAtb(ATB_OBJECT_VILLAGE))	
		{
			SVillageData*	lpSVillageData;
			XSPR*			lpXSPR;
//			SI16			TempX;
//			SI16			TempY;
			float			TempPercent;
			VDEF			TempVillageDefence;
			SI16			TempFont;

			if(pOnlineVillageDataOfObject)
			{
				TempPercent = 1.0f;
				if(pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode))
				{
					TempVillageDefence = pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode);
					TempPercent        = (float)((double)TempVillageDefence / (double)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode));
				}

				// 마을 Object
				TempFont = siFont;
				if(TempPercent < 0.7f)
				{
					pObjectXspr	= pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile2);
					TempFont    = siFont2;
				}

				if(SelectID == siID)
				{
					// 선택시
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[TempFont + 1]]);
				}
				else
				{
					// 비 선택시
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[TempFont]]);
				}
			}
			else
			{
				if(SelectID == siID)
				{
					// 선택시
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[siFont + 1]]);
				}
				else
				{
					// 비 선택시
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
				}
			}

			if(pOnlineVillageDataOfObject)
			{
				// 현재 마을의 상태 표시
				int			TempCount;
				XSPR*		lpXSPR1;
				XSPR*		lpXSPR2;
				XSPR*		lpXSPR3;
				OnlineResource_VillageFireData*		TempOnlineResource_VillageFireData;

				if(pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode))
				{
					TempVillageDefence = pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode);
					TempPercent        = (float)((double)TempVillageDefence / (double)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode));
					lpXSPR1 = pMyOnlineWorld->pOnlineResource->GetxsprVillageEffect(0, 2);
					lpXSPR2 = pMyOnlineWorld->pOnlineResource->GetxsprVillageEffect(0, 1);
					lpXSPR3 = pMyOnlineWorld->pOnlineResource->GetxsprVillageEffect(0, 0);

					TempOnlineResource_VillageFireData = NULL;
					if(TempVillageDefence == 0)
					{
						TempOnlineResource_VillageFireData = pMyOnlineWorld->pOnlineResource->GetVillageFirePosition(pOnlineVillageDataOfObject->siVillageSize, pOnlineVillageDataOfObject->uiVillageStateCode, 0);
					}
					else if(TempPercent < 0.4f)
					{
						TempOnlineResource_VillageFireData = pMyOnlineWorld->pOnlineResource->GetVillageFirePosition(pOnlineVillageDataOfObject->siVillageSize, pOnlineVillageDataOfObject->uiVillageStateCode, 1);
					}
					else if(TempPercent < 0.7f)
					{
						TempOnlineResource_VillageFireData = pMyOnlineWorld->pOnlineResource->GetVillageFirePosition(pOnlineVillageDataOfObject->siVillageSize, pOnlineVillageDataOfObject->uiVillageStateCode, 2);
					}

					if(TempOnlineResource_VillageFireData)
					{
						for(TempCount = 0; TempCount < TempOnlineResource_VillageFireData->m_FireCount; TempCount++)
						{
							switch(TempOnlineResource_VillageFireData->m_FireSize[TempCount])
							{
								case 0:
									clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - (lpXSPR1->Header.Xsize >> 1) + TempOnlineResource_VillageFireData->m_FirePositionX[TempCount]
										, siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - (lpXSPR1->Header.Ysize >> 1) + TempOnlineResource_VillageFireData->m_FirePositionY[TempCount]
										, lpXSPR1->Header.Xsize, lpXSPR1->Header.Ysize
										, &lpXSPR1->Image[lpXSPR1->Header.Start[((GetTickCount() / 100) % lpXSPR1->Header.font_number)]]);
									break;

								case 1:
									clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - (lpXSPR2->Header.Xsize >> 1) + TempOnlineResource_VillageFireData->m_FirePositionX[TempCount]
										, siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - (lpXSPR2->Header.Ysize >> 1) + TempOnlineResource_VillageFireData->m_FirePositionY[TempCount]
										, lpXSPR2->Header.Xsize, lpXSPR2->Header.Ysize
										, &lpXSPR2->Image[lpXSPR2->Header.Start[((GetTickCount() / 100) % lpXSPR2->Header.font_number)]]);
									break;

								case 2:
									clGrp.PutSpriteLightImjinCT(siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - (lpXSPR3->Header.Xsize >> 1) + TempOnlineResource_VillageFireData->m_FirePositionX[TempCount]
										, siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - (lpXSPR3->Header.Ysize >> 1) + TempOnlineResource_VillageFireData->m_FirePositionY[TempCount]
										, lpXSPR3->Header.Xsize, lpXSPR3->Header.Ysize
										, &lpXSPR3->Image[lpXSPR3->Header.Start[((GetTickCount() / 100) % lpXSPR3->Header.font_number)]]);
									break;
							}
						}
					}
				}

				// 마을 깃발
				lpSVillageData = pMyOnlineWorld->pOnlineVillageDescriptList->GetVillageDataData(GetVillageCode());
				if(lpSVillageData)
				{
					if(lpSVillageData->m_fWait == FALSE)
					{
						if(lpSVillageData->m_VillageGuildName[0])
						{
							lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagV(lpSVillageData->m_VillageGuildFlag);
							if(lpXSPR)
							{
								if(lpXSPR->Image)
								{
									clGrp.PutSpriteT(siDotDrawBaseX + siSprCenterX + pOnlineVillageDataOfObject->siGuildFlagXPos, siDotDrawBaseY + siSprCenterY + pOnlineVillageDataOfObject->siGuildFlagYPos - lpXSPR->Header.Ysize, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[(GetTickCount() / 120) % lpXSPR->Header.font_number]]);
								}
							}
						}
					}
				}

				//=============================================================================================
				// robypark 2005/1/7 20:27
				// 마을 방어력 및 수비병 표시하기
				// 공성전 관련 정보보기 옵션이 활성화되었다면,
				if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
				{
					// 마을 방어력 그리기
					DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

					/*
					// 마을 데이터가 존재하여, 상단이 있는 경우
					if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
							&& (lpSVillageData->m_VillageGuildName[0]))
					{
						// 마을 주둔군 그리기
						DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
					}

					// 마을 점령하라는 이펙트 그리기
					DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
					*/
				}
				else	// 공성전 관련 정보보기 옵션이 활성화된 것이 아니라면,
				{
					// 마을 위에 커서로 가리키고 있는 경우,
					if (SelectID == siID)
					{
						// 공성전 유닛이라면,
						if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// 마을 방어력 그리기
							DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

							/*
							// 마을 데이터가 존재하여, 상단이 있는 경우
							if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
									&& (lpSVillageData->m_VillageGuildName[0]))
							{
								// 마을 주둔군 그리기
								DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							}

							// 마을 점령하라는 이펙트 그리기
							DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							*/
						}
						else if (FALSE == pMyOnlineWorld->bPeace)	// 전투모드라면
						{
							// 마을 방어력 그리기
							DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

							/*						
							// 마을 데이터가 존재하여, 상단이 있는 경우
							if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
									&& (lpSVillageData->m_VillageGuildName[0]))
							{
								// 마을 주둔군 그리기
								DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							}

							// 마을 점령하라는 이펙트 그리기
							DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							*/
							
						}
					}
					else	// 마우스 커서가 마을을 가리키지 않는 경우
					{
						/*
						// 마을 점령하라는 이펙트 그리기
						DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
						*/
					}
				}
				//=============================================================================================
			}	// if(pOnlineVillageDataOfObject)
		}
		else
		{
			// 일반 Object
			if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_ANI)
			{
				if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_TRANS)
				{
					clGrp.PutSpriteLightImjinCT(siDotDrawBaseX, siDotDrawBaseY, pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize,
						&pObjectXspr->Image[pObjectXspr->Header.Start[siFont / 4]]);
				}
				else
				{
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize,
						&pObjectXspr->Image[pObjectXspr->Header.Start[siFont / 4]]);
				}
			}
			else
			{
				if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_TRANS)
				{
					clGrp.PutSpriteLightImjinCT(siDotDrawBaseX, siDotDrawBaseY, pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
						&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
				}
				else
				{
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
						&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	화면에 그린다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::DrawTileType(SI16 SelectID)
{
	if(pMyOnlineWorld                         == NULL)  return;
	if(pOnlinePortalDataOfObject              == NULL)  return;
	if(pOnlinePortalDataOfObject->uifTileDraw == FALSE) return;

	SI32	siDotDrawBaseX, siDotDrawBaseY;	
	SI32	siSrcDotDrawBaseX, siSrcDotDrawBaseY;	
	XSPR	*pObjectXspr;
	

	// 찍는 기준 도트 위치를 구한다.
	if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siX, siY, siSrcDotDrawBaseX, siSrcDotDrawBaseY) == FALSE)
	{		
		return;
	}		


	pObjectXspr	=	pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);	
	if(pObjectXspr)
	{
		siDotDrawBaseX	=	siSrcDotDrawBaseX -  siSprCenterX - pMyOnlineWorld->pOnlineMap->GetRX();
		siDotDrawBaseY	=	siSrcDotDrawBaseY -  siSprCenterY - pMyOnlineWorld->pOnlineMap->GetRY();

			
		if(IsAtb(ATB_OBJECT_VILLAGE))	
		{
			if(pOnlinePortalDataOfObject)
			{
				if(pOnlinePortalDataOfObject->uifTileDraw)
				{
					// 마을 Object
					if(SelectID == siID)
					{
						// 선택시
						clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
										pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
										&pObjectXspr->Image[pObjectXspr->Header.Start[siFont + 1]]);
					}
					else
					{
						// 비 선택시
						clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
										pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
										&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
					}
				}
			}
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// robypark 2005/1/7 20:7
// 마을 방어력 게이지 출력
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineObject::DrawGaugeVillageDefence(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	POINT		TempStartPoint;

	// 출력 위치 계산
	TempStartPoint.x = siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX;
	TempStartPoint.y = siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY;

	// 배경 이미지 얻기
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_DEFENCE_BACKGROUND);

	// 배경 이미지 출력
	if (psprBackGround)
	{
		clGrp.PutSpriteJinC(TempStartPoint.x, TempStartPoint.y,
							psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
							psprBackGround->Image);
	}

	// 마을 방어력 게이지 이미지 얻기
	XSPR	*psprGaugeDefence = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_DEFENCE);

	if (psprGaugeDefence)
	{
		// 그려져야할 퍼센트 계산
		SI32	siPct = (pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode) * 100)
							/ (float)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode);

		// 그리기
		DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
							siPct, psprGaugeDefence);
	}

	/*
	DrawGauge(TempStartPoint.x, TempStartPoint.y, 200, 14, 
		(float)pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode)
		/ (float)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode),
		(SI08)COLOR_RED, 255);
	*/

	HDC hdc;
	// 마을의 방어력 값을 출력해준다.
	if(GETSCREENSURFACE(pMyOnlineWorld)->GetDC(&hdc) == DD_OK)
	{
		char szBuffer[64];
		sprintf(szBuffer, "%8ld / %8ld", (SI32)pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode), 
									(SI32)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode));
		::SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		::SetBkMode(hdc, TRANSPARENT);
		
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, TempStartPoint.x + 15 + 1, TempStartPoint.y + 6 + 1, psprGaugeDefence->Header.Xsize, psprGaugeDefence->Header.Ysize, szBuffer, RGB(40, 40, 40));
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, TempStartPoint.x + 15, TempStartPoint.y + 6, psprGaugeDefence->Header.Xsize, psprGaugeDefence->Header.Ysize, szBuffer, RGB(250, 254, 250));

		GETSCREENSURFACE(pMyOnlineWorld)->ReleaseDC(hdc);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// robypark 2005/1/7 20:7
// 마을 수비병 게이지 출력
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineObject::DrawGaugeVillageGarrison(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	POINT		TempStartPoint;

	// 출력 위치 계산
	TempStartPoint.x = siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX;
	TempStartPoint.y = siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY;

	// 배경 이미지 얻기
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_GARRISON_BACKGROUND);

	// 배경 이미지 출력
	if (psprBackGround)
	{
		clGrp.PutSpriteJinC(TempStartPoint.x, TempStartPoint.y,
							psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
							psprBackGround->Image);
	}

	// 마을 방어력 게이지 이미지 얻기
	XSPR	*psprGaugeGarrison = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_GARRISON);

	// 0으로 나눠지는 것을 방지하기 위하여
	if (0 >= pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode)) // (최대 수비병수 <= 0) 이라면
	{
		if (psprGaugeGarrison)
		{
			// 그리기
			DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
								0, psprGaugeGarrison);
		}
	}
	else
	{
		if (psprGaugeGarrison)
		{
			// 그려져야할 퍼센트 계산
			SI32	siPct = (pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode) * 100)
								/ (float)pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode);

			// 그리기
			DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
								siPct, psprGaugeGarrison);
		}
	}

	HDC hdc;
	// 현재 수비병 부대수를 출력해준다.
	if(GETSCREENSURFACE(pMyOnlineWorld)->GetDC(&hdc) == DD_OK)
	{
		char szBuffer[64];
		sprintf(szBuffer, "%8ld / %8ld", (SI32)pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode), 
									(SI32)pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode));

		::SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		::SetBkMode(hdc, TRANSPARENT);

		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, TempStartPoint.x + 15 + 1, TempStartPoint.y + 6 + 1, psprGaugeGarrison->Header.Xsize, psprGaugeGarrison->Header.Ysize, szBuffer, RGB(40, 40, 40));
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, TempStartPoint.x + 15, TempStartPoint.y + 6, psprGaugeGarrison->Header.Xsize, psprGaugeGarrison->Header.Ysize, szBuffer, RGB(250, 254, 250));

		GETSCREENSURFACE(pMyOnlineWorld)->ReleaseDC(hdc);
	}

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// robypark 2005/1/7 20:7
// 마을 검병으로 점령하는 이펙트 출력
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineObject::DrawEffectEnabledOccupyVillage(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	// 마을에 수비병이 없을 경우 검병으로 공격하여 마을을 점령하라는 이펙트 그리기
	// 공성시간이며, 사용자와 공성하는 날의 마을이라면
	if ((pMyOnlineWorld->IsInTimeSiegeWarfare())
		&& (pMyOnlineWorld->GetVillageWarDayGuild(pOnlineVillageDataOfObject->uiVillageCode))
		&& (0 == pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode)))
	{
		// 점령하라는 이펙트 그리기
		XSPR *psprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_EFFECT_ENABLED_OCCUPY_VILLAGE);

		if (psprEffect)
		{
			clGrp.PutSpriteCT(siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosX, 
								siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosY,
								psprEffect->Header.Xsize, psprEffect->Header.Ysize,
								&psprEffect->Image[psprEffect->Header.Start
								[(GetTickCount() / 100) % psprEffect->Header.font_number]]);
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	// 액션.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineObject::Action()
{
	XSPR	*pObjectXspr;
	BOOL	TempfSound;

	if(pOnlineVillageDataOfObject)
	{
		// 현재 마을일경우 국가 코드가 틀려지면 다시 로딩
		if(pOnlineVillageDataOfObject->uiVillageStateCode != pMyOnlineWorld->GetVillageStateCode(pOnlineVillageDataOfObject->uiVillageCode))
		{
			XSPR		*pVillageXspr;
//			SI32		siDotX1, siDotY1;	
//			SI32		siDotX2, siDotY2;
//			char		TempBuffer[256];
	
			pOnlineVillageDataOfObject->uiVillageStateCode = pMyOnlineWorld->GetVillageStateCode(pOnlineVillageDataOfObject->uiVillageCode);

			// 국가 Code에 따른 이미지 로드
			pOnlineVillageDataOfObject->siGuildFlagXPos					= 0;
			pOnlineVillageDataOfObject->siGuildFlagYPos					= -63;
			pVillageXspr												=	pMyOnlineWorld->pOnlineResource->GetxsprObject(pMyOnlineWorld->pOnlineResource->GetObjectFileIndex(pOnlineVillageDataOfObject->pVillageHeader->szImageFile));
			siSprCenterX												=	pVillageXspr->Header.Xsize / 2;
			siSprCenterY												=	pVillageXspr->Header.Ysize / 2;
			pOnlineVillageDataOfObject->m_ssSurrenderFlagPosX			= 0;
			pOnlineVillageDataOfObject->m_ssSurrenderFlagPosY			= -63;
			pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX	= 117;
			pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY	= 180;
			pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX		= 117;
			pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY		= 180;
			pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosX	= 69;
			pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosY	= 45;

			// 국가와 마을 크기에 따른 이미지 파일명과 마을 크기 얻기
			pMyOnlineWorld->pOnlineResource->GetVillageImageFilename(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->pVillageHeader->szImageFile, pOnlineVillageDataOfObject->siVillageSize,
																			pOnlineVillageDataOfObject->szVillageFileName, pOnlineVillageDataOfObject->szVillageFileNameHalfE);

			// 국가와 마을 크기에 따른 이미지 좌표 정보 헤더 얻기
			LPOnlineResource_VillageImagePositionInfo pVillageImagePositionInfo = pMyOnlineWorld->pOnlineResource->GetVillageImagePositionInfoHeader(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->siVillageSize);

			if (pVillageImagePositionInfo)
			{
				pOnlineVillageDataOfObject->siGuildFlagXPos					= pVillageImagePositionInfo->m_ssVillageGuildFlagPosX;
				pOnlineVillageDataOfObject->siGuildFlagYPos					= pVillageImagePositionInfo->m_ssVillageGuildFlagPosY;
				siSprCenterX												= pVillageImagePositionInfo->m_ssVillageDrawCenterX;
				siSprCenterY												= pVillageImagePositionInfo->m_ssVillageDrawCenterY;
				pOnlineVillageDataOfObject->m_ssSurrenderFlagPosX			= pVillageImagePositionInfo->m_ssSurrenderFlagPosX;
				pOnlineVillageDataOfObject->m_ssSurrenderFlagPosY			= pVillageImagePositionInfo->m_ssSurrenderFlagPosY;
				pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX	= pVillageImagePositionInfo->m_ssVillageGarrisionGaugePosX;
				pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY	= pVillageImagePositionInfo->m_ssVillageGarrisionGaugePosY;
				pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX		= pVillageImagePositionInfo->m_ssVillageDefenceGaugePosX;
				pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY		= pVillageImagePositionInfo->m_ssVillageDefenceGaugePosY;
				pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosX	= pVillageImagePositionInfo->m_ssEnabledOccupyVillagePosX;
				pOnlineVillageDataOfObject->m_ssEnabledOccupyVillagePosY	= pVillageImagePositionInfo->m_ssEnabledOccupyVillagePosY;
			}

			siFile							=	pMyOnlineWorld->pOnlineResource->GetObjectFileIndex((char*)pOnlineVillageDataOfObject->szVillageFileName);
			siFont							=	0;
			/*
			pVillageXspr					=	pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);

			pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(siX, siY, siDotX1, siDotY1);
			pMyOnlineWorld->pOnlineMap->TransformMapXYToRealDotCenterXY(siX + (siXsize / 2), siY + (siYsize / 2), siDotX2, siDotY2);
			siDotY2							+=	ONLINE_TILE_YSIZE_HALF;

			// robypark 2004/12/9 14:45
			// 마을 그림의 중점 위치를 일정하게 하기 위하여 마을 국가코드 및 크기 설정에서 결정하도록 수정
			siSprCenterX					=	pVillageXspr->Header.Xsize / 2;
			siSprCenterY					=	pVillageXspr->Header.Xsize / 2;
			*/
			siAtb							=	ATB_OBJECT_VILLAGE;	
		}

		// 사운드가 나올수 있는지 검사
		TempfSound = FALSE;
		if(abs(pMyOnlineWorld->pOnlineMap->m_siX - siX) + abs(pMyOnlineWorld->pOnlineMap->m_siY - siY) < 24)
		{
			TempfSound = TRUE;
		}

		if(pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode))
		{
			float		TempPercent;
			VDEF		TempVillageDefence;
			
			TempVillageDefence = pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode);
			TempPercent        = (float)((double)TempVillageDefence / (double)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode));

			if(TempVillageDefence == 0)
			{
				if(pOnlineVillageDataOfObject->siVillageExplosionFlag != 1)
				{
					if(pOnlineVillageDataOfObject->siVillageExplosionFlag != -1)
					{
						if(TempfSound)
						{
							PushEffect(EFFECT_HALFEXP, 0, 0);
						}
					}
					pOnlineVillageDataOfObject->siVillageExplosionFlag = 1;
				}
			}
			else if(TempPercent < 0.4f)
			{
				if(pOnlineVillageDataOfObject->siVillageExplosionFlag != 2)
				{
					if(pOnlineVillageDataOfObject->siVillageExplosionFlag != -1)
					{
						if(TempfSound)
						{
							PushEffect(EFFECT_HALFEXP, 0, 0);
						}
					}
					pOnlineVillageDataOfObject->siVillageExplosionFlag = 2;
				}
			}
			else if(TempPercent < 0.7f)
			{
				if(pOnlineVillageDataOfObject->siVillageExplosionFlag != 3)
				{
					if(pOnlineVillageDataOfObject->siVillageExplosionFlag != -1)
					{
						if(TempfSound)
						{
							PushEffect(EFFECT_HALFEXP, 0, 0);
						}
					}
					pOnlineVillageDataOfObject->siVillageExplosionFlag = 3;
				}
			}

			if(pOnlineVillageDataOfObject->siVillageExplosionFlag > 0)
			{
				if(pOnlineVillageDataOfObject->siVillageExplosionSoundWait == 0) pOnlineVillageDataOfObject->siVillageExplosionSoundWait = GetTickCount();

				if((GetTickCount() - pOnlineVillageDataOfObject->siVillageExplosionSoundWait) > 5000)
				{
					pOnlineVillageDataOfObject->siVillageExplosionSoundWait = GetTickCount();
					if(TempfSound)
					{
						PushEffect(EFFECT_FIRE, 0, 0);
					}					
				}
			}
		}
	}
	else if(pOnlinePortalDataOfObject)
	{
		// 무시
	}
	else
	{
		if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_ANI)
		{
			pObjectXspr = pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);	

			siFont++;
			if(siFont >= (pMyOnlineWorld->pIOnlineObjectKI->GetTotalFrameFromIndex(siObjectKindIndex) * 4)) siFont = 0;
		}
		else
		{
			/*
			SI16	TempNPCFlag;
			SI16	TempNPCAnimation;

			TempNPCFlag      = FALSE;
			TempNPCAnimation = 0;
			if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC01)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 1;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC02)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 2;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC03)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 3;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC04)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 4;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC05)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 5;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC06)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 6;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC07)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 7;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC08)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 8;
			}
			else if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_NPC09)
			{
				TempNPCFlag      = TRUE;
				TempNPCAnimation = 9;
			}

			if(TempNPCFlag)
			{
				SI16		TotalFrame;
				SI16		NowFrame;
				SI16		NowAnimation;
				SI16		TempCount;

				siFont++;

				TotalFrame = (pMyOnlineWorld->pIOnlineObjectKI->GetTotalFrameFromIndex(siObjectKindIndex) * 4) / TempNPCAnimation;
				NowFrame   = siFont % TotalFrame;
				if(NowFrame >= TotalFrame)
				{
					// 다른 애니메이션으로 전환
					NowFrame = 0;
					for(TempCount = 0; TempCount < TempNPCAnimation; TempCount++)
					{
						NowFrame += TempCount + 1;
					}
					NowAnimation = rand() % NowFrame;

					NowFrame = 0;
					for(TempCount = 0; TempCount < TempNPCAnimation; TempCount++)
					{
						NowFrame += TempCount + 1;
						if(NowAnimation < NowFrame)
						{
							NowAnimation = TempNPCAnimation - (TempCount + 1);
							break;
						}
					}

					siFont = NowAnimation * TotalFrame;
				}
			}  */
		}
	}
}

/*
//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화가 되었는가?
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineObject::IsInit()
{
	if(pMyOnlineWorld)
		return TRUE;
	else
		return FALSE;		
}
*/
//----------------------------------------------------------------------------------------------------------------
//	설명	:	속성을 가지고 있는가?
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineObject::IsAtb(SI32 siAtb)
{
	if((OnlineObject::siAtb & siAtb) == siAtb)
		return TRUE;
	else
		return FALSE;
}  

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을이라면 종류를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineObject::GetVillageKind()
{	
	return siVillageKind;	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	오브젝트의 이름을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetName(CHAR pszName[32])
{
	strcpy(pszName, szName);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	설명을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
CHAR	*OnlineObject::GetMark(VOID)
{
	return(m_pszMark);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을이 있는 위치를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	siX;
	*psiY	=	siY;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	오브젝트이 차지하는 영역을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetSize(SI16 *psiXsize, SI16 *psiYsize)
{
	*psiXsize	=	siXsize;
	*psiYsize	=	siYsize;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	마을일 경우 마을과 연결되는 화일을 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetVillageFileName(CHAR *pszVillageFileName)
{
	if(pOnlineVillageDataOfObject)
	{
		strcpy(pszVillageFileName, pOnlineVillageDataOfObject->szVillageFileName);
		return;
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	버그를 잡기 위한 정보를 화일에 쓴다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineObject::WriteFileForBug(FILE *fp)
{
	fprintf(fp, "Object:WriteFile  ID  : [%d] pMyOnlineWorld : [%d]\n", siID, pMyOnlineWorld);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	오브젝트의 종류를 얻어온다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	OnlineObject::GetKind()
{
	return siKind;
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	오브젝트의 종류를 얻어온다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	OnlineObject::GetKindIndex()
{
	return siObjectKindIndex;
}
*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	오브젝트가 포탈인지 검사한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineObject::IsPortal()
{
	if(pOnlinePortalDataOfObject) return TRUE;

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Portal ID를 돌려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	OnlineObject::GetPortalID()
{
	if(pOnlinePortalDataOfObject)
	{
		return pOnlinePortalDataOfObject->siPortalCode;
	}

	return -1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Portal Name를 돌려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
char*	OnlineObject::GetPortalName()
{
	if(pOnlinePortalDataOfObject)
	{
		return (char*)pOnlinePortalDataOfObject->szPortalFileName;
	}

	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Village ID를 돌려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	OnlineObject::GetVillageCode(void)
{
	if(pOnlineVillageDataOfObject != NULL) return pOnlineVillageDataOfObject->uiVillageCode;

	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	x, y 위치에 Image가 있는지 검사한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineObject::CheckImage(SI16 x, SI16 y, LPDIRECTDRAWSURFACE7 pSurface)
{
	if(pMyOnlineWorld == NULL) return FALSE;

	if((pOnlineVillageDataOfObject) || (pOnlinePortalDataOfObject))
	{
		SI32						siDotDrawBaseX, siDotDrawBaseY;	
		XSPR						*pObjectXspr;
		LPDIRECTDRAWSURFACE7		TempSurface;
		BOOL						fCheck;
		SI32						TempX1, TempX2, TempY1, TempY2;

		pObjectXspr	=	pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);	
		if(pObjectXspr == NULL) return FALSE;

		// 찍는 기준 도트 위치를 구한다.
		if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siX, siY, siDotDrawBaseX, siDotDrawBaseY) == FALSE)
		{		
			return FALSE;
		}		

		siDotDrawBaseX = siDotDrawBaseX - siSprCenterX - pMyOnlineWorld->pOnlineMap->GetRX();
		siDotDrawBaseY = siDotDrawBaseY - siSprCenterY - pMyOnlineWorld->pOnlineMap->GetRY();

		if(x < siDotDrawBaseX)                               return FALSE;
		if(x > (siDotDrawBaseX + pObjectXspr->Header.Xsize)) return FALSE;
		if(y < siDotDrawBaseY)                               return FALSE;
		if(y > (siDotDrawBaseY + pObjectXspr->Header.Ysize)) return FALSE;

		// Surface에 다가 Copy해서 그 위치를 검사한다.
		TempSurface = pMyOnlineWorld->pIOnlineObject->GetTempSurface();
		if(TempSurface == NULL) return FALSE;

		// 이전 Data 저장
		if(pSurface) clGrp.UnlockSurface(pSurface);
		clGrp.GetClipArea(TempX1, TempX2, TempY1, TempY2);

		fCheck = FALSE;
		if(clGrp.LockSurface(TempSurface) == TRUE)
		{
			clGrp.SetClipArea(0, 0, 2, 2);

			clGrp.PutPixel(0, 0, (BYTE)254);

			// 우선 Surface에 그린다.
			clGrp.PutSpriteT(siDotDrawBaseX - x, siDotDrawBaseY - y, 
							pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
							&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);

			// 그 위치에 Image가 있는지 검사한다.
			if(clGrp.GetPixel(0, 0) != 254) fCheck = TRUE;

			clGrp.UnlockSurface(TempSurface);
		}

		// 이전 Data 복구
		clGrp.SetClipArea(TempX1, TempX2, TempY1, TempY2);
		if(pSurface) clGrp.LockSurface(pSurface);

		return fCheck;
	}
	else
	{
		SI32						siDotDrawBaseX, siDotDrawBaseY;	
		XSPR						*pObjectXspr;
		LPDIRECTDRAWSURFACE7		TempSurface;
		BOOL						fCheck;
		SI32						TempX1, TempX2, TempY1, TempY2;

		pObjectXspr	=	pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile);	
		if(pObjectXspr == NULL) return FALSE;

		// 찍는 기준 도트 위치를 구한다.
		if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siX, siY, siDotDrawBaseX, siDotDrawBaseY) == FALSE)
		{		
			return FALSE;
		}		

		siDotDrawBaseX = siDotDrawBaseX - siSprCenterX - pMyOnlineWorld->pOnlineMap->GetRX();
		siDotDrawBaseY = siDotDrawBaseY - siSprCenterY - pMyOnlineWorld->pOnlineMap->GetRY();

		if(x < siDotDrawBaseX)                               return FALSE;
		if(x > (siDotDrawBaseX + pObjectXspr->Header.Xsize)) return FALSE;
		if(y < siDotDrawBaseY)                               return FALSE;
		if(y > (siDotDrawBaseY + pObjectXspr->Header.Ysize)) return FALSE;

		// Surface에 다가 Copy해서 그 위치를 검사한다.
		TempSurface = pMyOnlineWorld->pIOnlineObject->GetTempSurface();
		if(TempSurface == NULL) return FALSE;

		// 이전 Data 저장
		if(pSurface) clGrp.UnlockSurface(pSurface);
		clGrp.GetClipArea(TempX1, TempX2, TempY1, TempY2);

		fCheck = FALSE;
		if(clGrp.LockSurface(TempSurface) == TRUE)
		{
			clGrp.SetClipArea(0, 0, 2, 2);

			clGrp.PutPixel(0, 0, (BYTE)254);

			// 우선 Surface에 그린다.
			if(pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(siObjectKindIndex) & ATB_OBJECT_ANI)
			{
				clGrp.PutSpriteT(siDotDrawBaseX - x, siDotDrawBaseY - y, 
								pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
								&pObjectXspr->Image[pObjectXspr->Header.Start[siFont / 4]]);
			}
			else
			{
				clGrp.PutSpriteT(siDotDrawBaseX - x, siDotDrawBaseY - y, 
								pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
								&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
			}

			// 그 위치에 Image가 있는지 검사한다.
			if(clGrp.GetPixel(0, 0) != 254) fCheck = TRUE;

			clGrp.UnlockSurface(TempSurface);
		}

		// 이전 Data 복구
		clGrp.SetClipArea(TempX1, TempX2, TempY1, TempY2);
		if(pSurface) clGrp.LockSurface(pSurface);

		return fCheck;
	}

	return FALSE;
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Tile Draw 인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineObject::CheckTileDraw(void)
{
	if(pOnlinePortalDataOfObject)
	{
		if(pOnlinePortalDataOfObject->uifTileDraw)
		{
			return TRUE;
		}
	}

	return FALSE;
}
*/