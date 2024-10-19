/************************************************************************************************
	���ϸ� : OnlineObject.cpp
	����� : ������

	Map���� ����ϴ� Object �ϳ��� ������
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
//	����	:	������.
//----------------------------------------------------------------------------------------------------------------
OnlineObject::OnlineObject()
{
	ZeroMemory(szName, sizeof(szName));

	m_pszMark						=	NULL;				
	siID							=	0;
	siKind							=	0;
	siObjectKindIndex				=	0;
	siAtb							=	0;			// �Ӽ�.

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
	
	siVillageKind					=	0;		// �����̶�� � �����ΰ�?
	pMyOnlineWorld					=	NULL;
	pOnlineVillageDataOfObject		=	NULL;	
	pOnlinePortalDataOfObject		=	NULL;	
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���.
//----------------------------------------------------------------------------------------------------------------
OnlineObject::~OnlineObject()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.
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
	
	// �ʿ��� �̹� �����Ǿ� �����Ƿ� �ʿ� ������ �ʿ䰡 ����.	
	//pOnlineWorld->pOnlineMap->SetID(siX, siY, 1, 1, 0, siID, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.(������ ���)
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
	
	// ������ ��쿡�� �������� ���� ����Ÿ�� ���� �޸𸮸� �Ҵ�޾� �����صд�.
	pOnlineVillageDataOfObject	=	new OnlineVillageDataOfObject;	
	pOnlineVillageDataOfObject->uiVillageCode               = pVillageHeader->uiVillageCode;
	pOnlineVillageDataOfObject->uiVillageStateCode          = pOnlineWorld->GetVillageStateCode(pVillageHeader->uiVillageCode);
	pOnlineVillageDataOfObject->pVillageHeader              = pVillageHeader;
	pOnlineVillageDataOfObject->siVillageExplosionFlag      = -1;
	pOnlineVillageDataOfObject->siVillageExplosionSoundWait = 0;
	
	// ������ �̸��� ���´�.
	strcpy(szName, pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));	

	siX								=	pVillageHeader->siX;
	siY								=	pVillageHeader->siY;

	siXsize							=	pVillageHeader->siSize;
	siYsize							=	pVillageHeader->siSize;	
	
	// ���� Code�� ���� �̹��� �ε�. default
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

	// ������ ���� ũ�⿡ ���� �̹��� ���ϸ�� ���� ũ�� ���
	pMyOnlineWorld->pOnlineResource->GetVillageImageFilename(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->pVillageHeader->szImageFile, pOnlineVillageDataOfObject->siVillageSize,
																	pOnlineVillageDataOfObject->szVillageFileName, pOnlineVillageDataOfObject->szVillageFileNameHalfE);

	// ������ ���� ũ�⿡ ���� �̹��� ��ǥ ���� ��� ���
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
	// ���� �׸��� ���� ��ġ�� �����ϰ� �ϱ� ���Ͽ� ���� �����ڵ� �� ũ�� �������� �����ϵ��� ����
	siSprCenterX					=	pVillageXspr->Header.Xsize / 2;
	siSprCenterY					=	pVillageXspr->Header.Ysize / 2;
*/
	siAtb							=	ATB_OBJECT_VILLAGE;	


	// �ʿ� �����Ѵ�.

	//pMyOnlineWorld->pOnlineMap->SetID(siX, siY, siXsize, siYsize, 0, siID, 0, 0);
//	pMyOnlineWorld->pOnlineMap->SetColl(siX, siY, siXsize, siYsize, TRUE);
	
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.(Portal�� ���)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Init(SI16 siID, PortalHeader *pPortalHeader, cltOnlineWorld *pOnlineWorld)
{	
	char*		lpText;

	pMyOnlineWorld					=	pOnlineWorld;	
	OnlineObject::siID				=	siID;	
	OnlineObject::siKind			=	pPortalHeader->siPortalImage;
	OnlineObject::siObjectKindIndex	=	pMyOnlineWorld->pIOnlineObjectKI->GetIndex(pPortalHeader->siPortalImage);
	
	// Portal�� ��쿡�� Portal���� ���� ����Ÿ�� ���� �޸𸮸� �Ҵ�޾� �����صд�.
	pOnlinePortalDataOfObject	=	new OnlinePortalDataOfObject;
	ZeroMemory(pOnlinePortalDataOfObject, sizeof(OnlinePortalDataOfObject));
	pOnlinePortalDataOfObject->siPortalCode = pPortalHeader->siPortalCode;
	
	// ������ �̸��� ���´�.
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
//	����	:	�����Ѵ�.	
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
//	����	:	ȭ�鿡 �׸���.
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
	

	// ��� ���� ��Ʈ ��ġ�� ���Ѵ�.
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

				// ���� Object
				TempFont = siFont;
				if(TempPercent < 0.7f)
				{
					pObjectXspr	= pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile2);
					TempFont    = siFont2;
				}

				if(SelectID == siID)
				{
					// ���ý�
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[TempFont + 1]]);
				}
				else
				{
					// �� ���ý�
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[TempFont]]);
				}
			}
			else
			{
				if(SelectID == siID)
				{
					// ���ý�
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[siFont + 1]]);
				}
				else
				{
					// �� ���ý�
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
				}
			}

			if(pOnlineVillageDataOfObject)
			{
				// ���� ������ ���� ǥ��
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

				// ���� ���
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
				// ���� ���� �� ���� ǥ���ϱ�
				// ������ ���� �������� �ɼ��� Ȱ��ȭ�Ǿ��ٸ�,
				if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
				{
					// ���� ���� �׸���
					DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

					/*
					// ���� �����Ͱ� �����Ͽ�, ����� �ִ� ���
					if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
							&& (lpSVillageData->m_VillageGuildName[0]))
					{
						// ���� �ֵб� �׸���
						DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
					}

					// ���� �����϶�� ����Ʈ �׸���
					DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
					*/
				}
				else	// ������ ���� �������� �ɼ��� Ȱ��ȭ�� ���� �ƴ϶��,
				{
					// ���� ���� Ŀ���� ����Ű�� �ִ� ���,
					if (SelectID == siID)
					{
						// ������ �����̶��,
						if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// ���� ���� �׸���
							DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

							/*
							// ���� �����Ͱ� �����Ͽ�, ����� �ִ� ���
							if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
									&& (lpSVillageData->m_VillageGuildName[0]))
							{
								// ���� �ֵб� �׸���
								DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							}

							// ���� �����϶�� ����Ʈ �׸���
							DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							*/
						}
						else if (FALSE == pMyOnlineWorld->bPeace)	// ���������
						{
							// ���� ���� �׸���
							DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

							/*						
							// ���� �����Ͱ� �����Ͽ�, ����� �ִ� ���
							if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
									&& (lpSVillageData->m_VillageGuildName[0]))
							{
								// ���� �ֵб� �׸���
								DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							}

							// ���� �����϶�� ����Ʈ �׸���
							DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							*/
							
						}
					}
					else	// ���콺 Ŀ���� ������ ����Ű�� �ʴ� ���
					{
						/*
						// ���� �����϶�� ����Ʈ �׸���
						DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
						*/
					}
				}
				//=============================================================================================
			}	// if(pOnlineVillageDataOfObject)
		}
		else
		{
			// �Ϲ� Object
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
//	����	:	ȭ�鿡 �׸���.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::DrawTileType(SI16 SelectID)
{
	if(pMyOnlineWorld                         == NULL)  return;
	if(pOnlinePortalDataOfObject              == NULL)  return;
	if(pOnlinePortalDataOfObject->uifTileDraw == FALSE) return;

	SI32	siDotDrawBaseX, siDotDrawBaseY;	
	SI32	siSrcDotDrawBaseX, siSrcDotDrawBaseY;	
	XSPR	*pObjectXspr;
	

	// ��� ���� ��Ʈ ��ġ�� ���Ѵ�.
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
					// ���� Object
					if(SelectID == siID)
					{
						// ���ý�
						clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
										pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
										&pObjectXspr->Image[pObjectXspr->Header.Start[siFont + 1]]);
					}
					else
					{
						// �� ���ý�
						clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
										pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
										&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
					}
				}
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// robypark 2005/1/7 20:7
// ���� ���� ������ ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineObject::DrawGaugeVillageDefence(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	POINT		TempStartPoint;

	// ��� ��ġ ���
	TempStartPoint.x = siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX;
	TempStartPoint.y = siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY;

	// ��� �̹��� ���
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_DEFENCE_BACKGROUND);

	// ��� �̹��� ���
	if (psprBackGround)
	{
		clGrp.PutSpriteJinC(TempStartPoint.x, TempStartPoint.y,
							psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
							psprBackGround->Image);
	}

	// ���� ���� ������ �̹��� ���
	XSPR	*psprGaugeDefence = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_DEFENCE);

	if (psprGaugeDefence)
	{
		// �׷������� �ۼ�Ʈ ���
		SI32	siPct = (pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode) * 100)
							/ (float)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode);

		// �׸���
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
	// ������ ���� ���� ������ش�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// robypark 2005/1/7 20:7
// ���� ���� ������ ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineObject::DrawGaugeVillageGarrison(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	POINT		TempStartPoint;

	// ��� ��ġ ���
	TempStartPoint.x = siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX;
	TempStartPoint.y = siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY;

	// ��� �̹��� ���
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_GARRISON_BACKGROUND);

	// ��� �̹��� ���
	if (psprBackGround)
	{
		clGrp.PutSpriteJinC(TempStartPoint.x, TempStartPoint.y,
							psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
							psprBackGround->Image);
	}

	// ���� ���� ������ �̹��� ���
	XSPR	*psprGaugeGarrison = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_GARRISON);

	// 0���� �������� ���� �����ϱ� ���Ͽ�
	if (0 >= pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode)) // (�ִ� ���񺴼� <= 0) �̶��
	{
		if (psprGaugeGarrison)
		{
			// �׸���
			DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
								0, psprGaugeGarrison);
		}
	}
	else
	{
		if (psprGaugeGarrison)
		{
			// �׷������� �ۼ�Ʈ ���
			SI32	siPct = (pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode) * 100)
								/ (float)pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode);

			// �׸���
			DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
								siPct, psprGaugeGarrison);
		}
	}

	HDC hdc;
	// ���� ���� �δ���� ������ش�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// robypark 2005/1/7 20:7
// ���� �˺����� �����ϴ� ����Ʈ ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineObject::DrawEffectEnabledOccupyVillage(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	// ������ ������ ���� ��� �˺����� �����Ͽ� ������ �����϶�� ����Ʈ �׸���
	// �����ð��̸�, ����ڿ� �����ϴ� ���� �����̶��
	if ((pMyOnlineWorld->IsInTimeSiegeWarfare())
		&& (pMyOnlineWorld->GetVillageWarDayGuild(pOnlineVillageDataOfObject->uiVillageCode))
		&& (0 == pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode)))
	{
		// �����϶�� ����Ʈ �׸���
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	// �׼�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineObject::Action()
{
	XSPR	*pObjectXspr;
	BOOL	TempfSound;

	if(pOnlineVillageDataOfObject)
	{
		// ���� �����ϰ�� ���� �ڵ尡 Ʋ������ �ٽ� �ε�
		if(pOnlineVillageDataOfObject->uiVillageStateCode != pMyOnlineWorld->GetVillageStateCode(pOnlineVillageDataOfObject->uiVillageCode))
		{
			XSPR		*pVillageXspr;
//			SI32		siDotX1, siDotY1;	
//			SI32		siDotX2, siDotY2;
//			char		TempBuffer[256];
	
			pOnlineVillageDataOfObject->uiVillageStateCode = pMyOnlineWorld->GetVillageStateCode(pOnlineVillageDataOfObject->uiVillageCode);

			// ���� Code�� ���� �̹��� �ε�
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

			// ������ ���� ũ�⿡ ���� �̹��� ���ϸ�� ���� ũ�� ���
			pMyOnlineWorld->pOnlineResource->GetVillageImageFilename(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->pVillageHeader->szImageFile, pOnlineVillageDataOfObject->siVillageSize,
																			pOnlineVillageDataOfObject->szVillageFileName, pOnlineVillageDataOfObject->szVillageFileNameHalfE);

			// ������ ���� ũ�⿡ ���� �̹��� ��ǥ ���� ��� ���
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
			// ���� �׸��� ���� ��ġ�� �����ϰ� �ϱ� ���Ͽ� ���� �����ڵ� �� ũ�� �������� �����ϵ��� ����
			siSprCenterX					=	pVillageXspr->Header.Xsize / 2;
			siSprCenterY					=	pVillageXspr->Header.Xsize / 2;
			*/
			siAtb							=	ATB_OBJECT_VILLAGE;	
		}

		// ���尡 ���ü� �ִ��� �˻�
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
		// ����
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
					// �ٸ� �ִϸ��̼����� ��ȯ
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
//	����	:	�ʱ�ȭ�� �Ǿ��°�?
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
//	����	:	�Ӽ��� ������ �ִ°�?
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineObject::IsAtb(SI32 siAtb)
{
	if((OnlineObject::siAtb & siAtb) == siAtb)
		return TRUE;
	else
		return FALSE;
}  

//----------------------------------------------------------------------------------------------------------------
//	����	:	�����̶�� ������ ���´�.
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineObject::GetVillageKind()
{	
	return siVillageKind;	
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������Ʈ�� �̸��� ���´�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetName(CHAR pszName[32])
{
	strcpy(pszName, szName);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ ���´�.
//----------------------------------------------------------------------------------------------------------------
CHAR	*OnlineObject::GetMark(VOID)
{
	return(m_pszMark);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ �ִ� ��ġ�� ���´�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	siX;
	*psiY	=	siY;
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	������Ʈ�� �����ϴ� ������ ���´�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetSize(SI16 *psiXsize, SI16 *psiYsize)
{
	*psiXsize	=	siXsize;
	*psiYsize	=	siYsize;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ��� ������ ����Ǵ� ȭ���� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetVillageFileName(CHAR *pszVillageFileName)
{
	if(pOnlineVillageDataOfObject)
	{
		strcpy(pszVillageFileName, pOnlineVillageDataOfObject->szVillageFileName);
		return;
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���׸� ��� ���� ������ ȭ�Ͽ� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineObject::WriteFileForBug(FILE *fp)
{
	fprintf(fp, "Object:WriteFile  ID  : [%d] pMyOnlineWorld : [%d]\n", siID, pMyOnlineWorld);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������Ʈ�� ������ ���´�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineObject::GetKind()
{
	return siKind;
}
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������Ʈ�� ������ ���´�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	OnlineObject::GetKindIndex()
{
	return siObjectKindIndex;
}
*/
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������Ʈ�� ��Ż���� �˻��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineObject::IsPortal()
{
	if(pOnlinePortalDataOfObject) return TRUE;

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Portal ID�� �����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineObject::GetPortalID()
{
	if(pOnlinePortalDataOfObject)
	{
		return pOnlinePortalDataOfObject->siPortalCode;
	}

	return -1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Portal Name�� �����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
char*	OnlineObject::GetPortalName()
{
	if(pOnlinePortalDataOfObject)
	{
		return (char*)pOnlinePortalDataOfObject->szPortalFileName;
	}

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Village ID�� �����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	OnlineObject::GetVillageCode(void)
{
	if(pOnlineVillageDataOfObject != NULL) return pOnlineVillageDataOfObject->uiVillageCode;

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	x, y ��ġ�� Image�� �ִ��� �˻��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

		// ��� ���� ��Ʈ ��ġ�� ���Ѵ�.
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

		// Surface�� �ٰ� Copy�ؼ� �� ��ġ�� �˻��Ѵ�.
		TempSurface = pMyOnlineWorld->pIOnlineObject->GetTempSurface();
		if(TempSurface == NULL) return FALSE;

		// ���� Data ����
		if(pSurface) clGrp.UnlockSurface(pSurface);
		clGrp.GetClipArea(TempX1, TempX2, TempY1, TempY2);

		fCheck = FALSE;
		if(clGrp.LockSurface(TempSurface) == TRUE)
		{
			clGrp.SetClipArea(0, 0, 2, 2);

			clGrp.PutPixel(0, 0, (BYTE)254);

			// �켱 Surface�� �׸���.
			clGrp.PutSpriteT(siDotDrawBaseX - x, siDotDrawBaseY - y, 
							pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
							&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);

			// �� ��ġ�� Image�� �ִ��� �˻��Ѵ�.
			if(clGrp.GetPixel(0, 0) != 254) fCheck = TRUE;

			clGrp.UnlockSurface(TempSurface);
		}

		// ���� Data ����
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

		// ��� ���� ��Ʈ ��ġ�� ���Ѵ�.
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

		// Surface�� �ٰ� Copy�ؼ� �� ��ġ�� �˻��Ѵ�.
		TempSurface = pMyOnlineWorld->pIOnlineObject->GetTempSurface();
		if(TempSurface == NULL) return FALSE;

		// ���� Data ����
		if(pSurface) clGrp.UnlockSurface(pSurface);
		clGrp.GetClipArea(TempX1, TempX2, TempY1, TempY2);

		fCheck = FALSE;
		if(clGrp.LockSurface(TempSurface) == TRUE)
		{
			clGrp.SetClipArea(0, 0, 2, 2);

			clGrp.PutPixel(0, 0, (BYTE)254);

			// �켱 Surface�� �׸���.
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

			// �� ��ġ�� Image�� �ִ��� �˻��Ѵ�.
			if(clGrp.GetPixel(0, 0) != 254) fCheck = TRUE;

			clGrp.UnlockSurface(TempSurface);
		}

		// ���� Data ����
		clGrp.SetClipArea(TempX1, TempX2, TempY1, TempY2);
		if(pSurface) clGrp.LockSurface(pSurface);

		return fCheck;
	}

	return FALSE;
}
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Tile Draw ���� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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