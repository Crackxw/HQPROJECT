/************************************************************************************************
	だ橾貲 : OnlineObject.cpp
	氬渡濠 : 檜彌戮

	Map縑憮 餌辨ж朝 Object ж釭蒂 婦葬л
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
//	撲貲	:	儅撩濠.
//----------------------------------------------------------------------------------------------------------------
OnlineObject::OnlineObject()
{
	ZeroMemory(szName, sizeof(szName));

	m_pszMark						=	NULL;				
	siID							=	0;
	siKind							=	0;
	siObjectKindIndex				=	0;
	siAtb							=	0;			// 樓撩.

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
	
	siVillageKind					=	0;		// 葆擊檜塭賊 橫雯 葆擊檣陛?
	pMyOnlineWorld					=	NULL;
	pOnlineVillageDataOfObject		=	NULL;	
	pOnlinePortalDataOfObject		=	NULL;	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	模資濠.
//----------------------------------------------------------------------------------------------------------------
OnlineObject::~OnlineObject()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.
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
	
	// 裘縑朝 檜嘐 撲薑腎橫 氈戲嘎煎 裘縑 撲薑й в蹂陛 橈棻.	
	//pOnlineWorld->pOnlineMap->SetID(siX, siY, 1, 1, 0, siID, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.(葆擊檣 唳辦)
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
	
	// 葆擊橾 唳辦縑朝 葆擊虜檜 噙朝 等檜顫蒂 評煎 詭賅葬蒂 й渡嫡嬴 爾婦п菩棻.
	pOnlineVillageDataOfObject	=	new OnlineVillageDataOfObject;	
	pOnlineVillageDataOfObject->uiVillageCode               = pVillageHeader->uiVillageCode;
	pOnlineVillageDataOfObject->uiVillageStateCode          = pOnlineWorld->GetVillageStateCode(pVillageHeader->uiVillageCode);
	pOnlineVillageDataOfObject->pVillageHeader              = pVillageHeader;
	pOnlineVillageDataOfObject->siVillageExplosionFlag      = -1;
	pOnlineVillageDataOfObject->siVillageExplosionSoundWait = 0;
	
	// 葆擊曖 檜葷擊 橢橫螞棻.
	strcpy(szName, pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));	

	siX								=	pVillageHeader->siX;
	siY								=	pVillageHeader->siY;

	siXsize							=	pVillageHeader->siSize;
	siYsize							=	pVillageHeader->siSize;	
	
	// 措陛 Code縑 評艇 檜嘐雖 煎萄. default
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

	// 措陛諦 葆擊 觼晦縑 評艇 檜嘐雖 だ橾貲婁 葆擊 觼晦 橢晦
	pMyOnlineWorld->pOnlineResource->GetVillageImageFilename(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->pVillageHeader->szImageFile, pOnlineVillageDataOfObject->siVillageSize,
																	pOnlineVillageDataOfObject->szVillageFileName, pOnlineVillageDataOfObject->szVillageFileNameHalfE);

	// 措陛諦 葆擊 觼晦縑 評艇 檜嘐雖 謝ル 薑爾 ④渦 橢晦
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
	// 葆擊 斜葡曖 醞薄 嬪纂蒂 橾薑ж啪 ж晦 嬪ж罹 葆擊 措陛囀萄 塽 觼晦 撲薑縑憮 唸薑ж紫煙 熱薑
	siSprCenterX					=	pVillageXspr->Header.Xsize / 2;
	siSprCenterY					=	pVillageXspr->Header.Ysize / 2;
*/
	siAtb							=	ATB_OBJECT_VILLAGE;	


	// 裘縑 撲薑и棻.

	//pMyOnlineWorld->pOnlineMap->SetID(siX, siY, siXsize, siYsize, 0, siID, 0, 0);
//	pMyOnlineWorld->pOnlineMap->SetColl(siX, siY, siXsize, siYsize, TRUE);
	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.(Portal檣 唳辦)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::Init(SI16 siID, PortalHeader *pPortalHeader, cltOnlineWorld *pOnlineWorld)
{	
	char*		lpText;

	pMyOnlineWorld					=	pOnlineWorld;	
	OnlineObject::siID				=	siID;	
	OnlineObject::siKind			=	pPortalHeader->siPortalImage;
	OnlineObject::siObjectKindIndex	=	pMyOnlineWorld->pIOnlineObjectKI->GetIndex(pPortalHeader->siPortalImage);
	
	// Portal橾 唳辦縑朝 Portal虜檜 噙朝 等檜顫蒂 評煎 詭賅葬蒂 й渡嫡嬴 爾婦п菩棻.
	pOnlinePortalDataOfObject	=	new OnlinePortalDataOfObject;
	ZeroMemory(pOnlinePortalDataOfObject, sizeof(OnlinePortalDataOfObject));
	pOnlinePortalDataOfObject->siPortalCode = pPortalHeader->siPortalCode;
	
	// 葆擊曖 檜葷擊 橢橫螞棻.
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
//	撲貲	:	п薯и棻.	
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
//	撲貲	:	�飛橦� 斜萼棻.
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
	

	// 鎰朝 晦遽 紫お 嬪纂蒂 掘и棻.
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

				// 葆擊 Object
				TempFont = siFont;
				if(TempPercent < 0.7f)
				{
					pObjectXspr	= pMyOnlineWorld->pOnlineResource->GetxsprObject(siFile2);
					TempFont    = siFont2;
				}

				if(SelectID == siID)
				{
					// 摹鷗衛
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[TempFont + 1]]);
				}
				else
				{
					// 綠 摹鷗衛
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[TempFont]]);
				}
			}
			else
			{
				if(SelectID == siID)
				{
					// 摹鷗衛
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[siFont + 1]]);
				}
				else
				{
					// 綠 摹鷗衛
					clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
									pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
									&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
				}
			}

			if(pOnlineVillageDataOfObject)
			{
				// ⑷營 葆擊曖 鼻鷓 ル衛
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

				// 葆擊 梓嫦
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
				// 葆擊 寞橫溘 塽 熱綠煽 ル衛ж晦
				// 奢撩瞪 婦溼 薑爾爾晦 褫暮檜 �側瘓音Ц�棻賊,
				if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
				{
					// 葆擊 寞橫溘 斜葬晦
					DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

					/*
					// 葆擊 等檜攪陛 襄營ж罹, 鼻欽檜 氈朝 唳辦
					if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
							&& (lpSVillageData->m_VillageGuildName[0]))
					{
						// 葆擊 輿菩捱 斜葬晦
						DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
					}

					// 葆擊 薄滄ж塭朝 檜めお 斜葬晦
					DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
					*/
				}
				else	// 奢撩瞪 婦溼 薑爾爾晦 褫暮檜 �側瘓音� 匙檜 嬴棲塭賊,
				{
					// 葆擊 嬪縑 醴憮煎 陛葬酈堅 氈朝 唳辦,
					if (SelectID == siID)
					{
						// 奢撩瞪 嶸棉檜塭賊,
						if (pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// 葆擊 寞橫溘 斜葬晦
							DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

							/*
							// 葆擊 等檜攪陛 襄營ж罹, 鼻欽檜 氈朝 唳辦
							if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
									&& (lpSVillageData->m_VillageGuildName[0]))
							{
								// 葆擊 輿菩捱 斜葬晦
								DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							}

							// 葆擊 薄滄ж塭朝 檜めお 斜葬晦
							DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							*/
						}
						else if (FALSE == pMyOnlineWorld->bPeace)	// 瞪癱賅萄塭賊
						{
							// 葆擊 寞橫溘 斜葬晦
							DrawGaugeVillageDefence(siSrcDotDrawBaseX, siSrcDotDrawBaseY);

							/*						
							// 葆擊 等檜攪陛 襄營ж罹, 鼻欽檜 氈朝 唳辦
							if ((lpSVillageData) && (lpSVillageData->m_fWait == FALSE)
									&& (lpSVillageData->m_VillageGuildName[0]))
							{
								// 葆擊 輿菩捱 斜葬晦
								DrawGaugeVillageGarrison(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							}

							// 葆擊 薄滄ж塭朝 檜めお 斜葬晦
							DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
							*/
							
						}
					}
					else	// 葆辦蝶 醴憮陛 葆擊擊 陛葬酈雖 彊朝 唳辦
					{
						/*
						// 葆擊 薄滄ж塭朝 檜めお 斜葬晦
						DrawEffectEnabledOccupyVillage(siSrcDotDrawBaseX, siSrcDotDrawBaseY);
						*/
					}
				}
				//=============================================================================================
			}	// if(pOnlineVillageDataOfObject)
		}
		else
		{
			// 橾奩 Object
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
//	撲貲	:	�飛橦� 斜萼棻.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::DrawTileType(SI16 SelectID)
{
	if(pMyOnlineWorld                         == NULL)  return;
	if(pOnlinePortalDataOfObject              == NULL)  return;
	if(pOnlinePortalDataOfObject->uifTileDraw == FALSE) return;

	SI32	siDotDrawBaseX, siDotDrawBaseY;	
	SI32	siSrcDotDrawBaseX, siSrcDotDrawBaseY;	
	XSPR	*pObjectXspr;
	

	// 鎰朝 晦遽 紫お 嬪纂蒂 掘и棻.
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
					// 葆擊 Object
					if(SelectID == siID)
					{
						// 摹鷗衛
						clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
										pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
										&pObjectXspr->Image[pObjectXspr->Header.Start[siFont + 1]]);
					}
					else
					{
						// 綠 摹鷗衛
						clGrp.PutSpriteT(siDotDrawBaseX, siDotDrawBaseY, 
										pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
										&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);
					}
				}
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// robypark 2005/1/7 20:7
// 葆擊 寞橫溘 啪檜雖 轎溘
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineObject::DrawGaugeVillageDefence(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	POINT		TempStartPoint;

	// 轎溘 嬪纂 啗骯
	TempStartPoint.x = siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosX;
	TempStartPoint.y = siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssVillageDefenceGaugePosY;

	// 寡唳 檜嘐雖 橢晦
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_DEFENCE_BACKGROUND);

	// 寡唳 檜嘐雖 轎溘
	if (psprBackGround)
	{
		clGrp.PutSpriteJinC(TempStartPoint.x, TempStartPoint.y,
							psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
							psprBackGround->Image);
	}

	// 葆擊 寞橫溘 啪檜雖 檜嘐雖 橢晦
	XSPR	*psprGaugeDefence = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_DEFENCE);

	if (psprGaugeDefence)
	{
		// 斜溥螳撿й ぷ撫お 啗骯
		SI32	siPct = (pMyOnlineWorld->GetVillageDefenceNowDefence(pOnlineVillageDataOfObject->uiVillageCode) * 100)
							/ (float)pMyOnlineWorld->GetVillageDefenceMaxDefence(pOnlineVillageDataOfObject->uiVillageCode);

		// 斜葬晦
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
	// 葆擊曖 寞橫溘 高擊 轎溘п遽棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// robypark 2005/1/7 20:7
// 葆擊 熱綠煽 啪檜雖 轎溘
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineObject::DrawGaugeVillageGarrison(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	POINT		TempStartPoint;

	// 轎溘 嬪纂 啗骯
	TempStartPoint.x = siSrcDotDrawBaseX - pMyOnlineWorld->pOnlineMap->GetRX() - pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosX;
	TempStartPoint.y = siSrcDotDrawBaseY - pMyOnlineWorld->pOnlineMap->GetRY() - pOnlineVillageDataOfObject->m_ssVillageGarrisionGaugePosY;

	// 寡唳 檜嘐雖 橢晦
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_GARRISON_BACKGROUND);

	// 寡唳 檜嘐雖 轎溘
	if (psprBackGround)
	{
		clGrp.PutSpriteJinC(TempStartPoint.x, TempStartPoint.y,
							psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
							psprBackGround->Image);
	}

	// 葆擊 寞橫溘 啪檜雖 檜嘐雖 橢晦
	XSPR	*psprGaugeGarrison = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_VILLAGE_GARRISON);

	// 0戲煎 釭散雖朝 匙擊 寞雖ж晦 嬪ж罹
	if (0 >= pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode)) // (譆渠 熱綠煽熱 <= 0) 檜塭賊
	{
		if (psprGaugeGarrison)
		{
			// 斜葬晦
			DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
								0, psprGaugeGarrison);
		}
	}
	else
	{
		if (psprGaugeGarrison)
		{
			// 斜溥螳撿й ぷ撫お 啗骯
			SI32	siPct = (pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode) * 100)
								/ (float)pMyOnlineWorld->GetVillageMaximumGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode);

			// 斜葬晦
			DrawGaugeVillage(TempStartPoint.x + 15, TempStartPoint.y + 6,
								siPct, psprGaugeGarrison);
		}
	}

	HDC hdc;
	// ⑷營 熱綠煽 睡渠熱蒂 轎溘п遽棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// robypark 2005/1/7 20:7
// 葆擊 匐煽戲煎 薄滄ж朝 檜めお 轎溘
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineObject::DrawEffectEnabledOccupyVillage(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY)
{
	// 葆擊縑 熱綠煽檜 橈擊 唳辦 匐煽戲煎 奢問ж罹 葆擊擊 薄滄ж塭朝 檜めお 斜葬晦
	// 奢撩衛除檜貊, 餌辨濠諦 奢撩ж朝 陳曖 葆擊檜塭賊
	if ((pMyOnlineWorld->IsInTimeSiegeWarfare())
		&& (pMyOnlineWorld->GetVillageWarDayGuild(pOnlineVillageDataOfObject->uiVillageCode))
		&& (0 == pMyOnlineWorld->GetVillageCurrentGarrisonCount(pOnlineVillageDataOfObject->uiVillageCode)))
	{
		// 薄滄ж塭朝 檜めお 斜葬晦
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	// 擋暮.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineObject::Action()
{
	XSPR	*pObjectXspr;
	BOOL	TempfSound;

	if(pOnlineVillageDataOfObject)
	{
		// ⑷營 葆擊橾唳辦 措陛 囀萄陛 ぎ溥雖賊 棻衛 煎註
		if(pOnlineVillageDataOfObject->uiVillageStateCode != pMyOnlineWorld->GetVillageStateCode(pOnlineVillageDataOfObject->uiVillageCode))
		{
			XSPR		*pVillageXspr;
//			SI32		siDotX1, siDotY1;	
//			SI32		siDotX2, siDotY2;
//			char		TempBuffer[256];
	
			pOnlineVillageDataOfObject->uiVillageStateCode = pMyOnlineWorld->GetVillageStateCode(pOnlineVillageDataOfObject->uiVillageCode);

			// 措陛 Code縑 評艇 檜嘐雖 煎萄
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

			// 措陛諦 葆擊 觼晦縑 評艇 檜嘐雖 だ橾貲婁 葆擊 觼晦 橢晦
			pMyOnlineWorld->pOnlineResource->GetVillageImageFilename(pOnlineVillageDataOfObject->uiVillageStateCode, pOnlineVillageDataOfObject->pVillageHeader->szImageFile, pOnlineVillageDataOfObject->siVillageSize,
																			pOnlineVillageDataOfObject->szVillageFileName, pOnlineVillageDataOfObject->szVillageFileNameHalfE);

			// 措陛諦 葆擊 觼晦縑 評艇 檜嘐雖 謝ル 薑爾 ④渦 橢晦
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
			// 葆擊 斜葡曖 醞薄 嬪纂蒂 橾薑ж啪 ж晦 嬪ж罹 葆擊 措陛囀萄 塽 觼晦 撲薑縑憮 唸薑ж紫煙 熱薑
			siSprCenterX					=	pVillageXspr->Header.Xsize / 2;
			siSprCenterY					=	pVillageXspr->Header.Xsize / 2;
			*/
			siAtb							=	ATB_OBJECT_VILLAGE;	
		}

		// 餌遴萄陛 釭螢熱 氈朝雖 匐餌
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
		// 鼠衛
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
					// 棻艇 擁棲詭檜暮戲煎 瞪��
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
//	撲貲	:	蟾晦�降� 腎歷朝陛?
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
//	撲貲	:	樓撩擊 陛雖堅 氈朝陛?
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineObject::IsAtb(SI32 siAtb)
{
	if((OnlineObject::siAtb & siAtb) == siAtb)
		return TRUE;
	else
		return FALSE;
}  

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊檜塭賊 謙盟蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineObject::GetVillageKind()
{	
	return siVillageKind;	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	螃粽薛お曖 檜葷擊 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetName(CHAR pszName[32])
{
	strcpy(pszName, szName);
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	撲貲擊 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
CHAR	*OnlineObject::GetMark(VOID)
{
	return(m_pszMark);
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊檜 氈朝 嬪纂蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	siX;
	*psiY	=	siY;
}


//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	螃粽薛お檜 離雖ж朝 艙羲擊 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetSize(SI16 *psiXsize, SI16 *psiYsize)
{
	*psiXsize	=	siXsize;
	*psiYsize	=	siYsize;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊橾 唳辦 葆擊婁 翱唸腎朝 �倣狨� 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineObject::GetVillageFileName(CHAR *pszVillageFileName)
{
	if(pOnlineVillageDataOfObject)
	{
		strcpy(pszVillageFileName, pOnlineVillageDataOfObject->szVillageFileName);
		return;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	幗斜蒂 濩晦 嬪и 薑爾蒂 �倣狤� 噹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineObject::WriteFileForBug(FILE *fp)
{
	fprintf(fp, "Object:WriteFile  ID  : [%d] pMyOnlineWorld : [%d]\n", siID, pMyOnlineWorld);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	螃粽薛お曖 謙盟蒂 橢橫螞棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	OnlineObject::GetKind()
{
	return siKind;
}
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	螃粽薛お曖 謙盟蒂 橢橫螞棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnlineObject::GetKindIndex()
{
	return siObjectKindIndex;
}
*/
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	螃粽薛お陛 ん驍檣雖 匐餌и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineObject::IsPortal()
{
	if(pOnlinePortalDataOfObject) return TRUE;

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Portal ID蒂 給溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	OnlineObject::GetPortalID()
{
	if(pOnlinePortalDataOfObject)
	{
		return pOnlinePortalDataOfObject->siPortalCode;
	}

	return -1;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Portal Name蒂 給溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
char*	OnlineObject::GetPortalName()
{
	if(pOnlinePortalDataOfObject)
	{
		return (char*)pOnlinePortalDataOfObject->szPortalFileName;
	}

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Village ID蒂 給溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	OnlineObject::GetVillageCode(void)
{
	if(pOnlineVillageDataOfObject != NULL) return pOnlineVillageDataOfObject->uiVillageCode;

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	x, y 嬪纂縑 Image陛 氈朝雖 匐餌и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		// 鎰朝 晦遽 紫お 嬪纂蒂 掘и棻.
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

		// Surface縑 棻陛 Copyп憮 斜 嬪纂蒂 匐餌и棻.
		TempSurface = pMyOnlineWorld->pIOnlineObject->GetTempSurface();
		if(TempSurface == NULL) return FALSE;

		// 檜瞪 Data 盪濰
		if(pSurface) clGrp.UnlockSurface(pSurface);
		clGrp.GetClipArea(TempX1, TempX2, TempY1, TempY2);

		fCheck = FALSE;
		if(clGrp.LockSurface(TempSurface) == TRUE)
		{
			clGrp.SetClipArea(0, 0, 2, 2);

			clGrp.PutPixel(0, 0, (BYTE)254);

			// 辦摹 Surface縑 斜萼棻.
			clGrp.PutSpriteT(siDotDrawBaseX - x, siDotDrawBaseY - y, 
							pObjectXspr->Header.Xsize, pObjectXspr->Header.Ysize, 
							&pObjectXspr->Image[pObjectXspr->Header.Start[siFont]]);

			// 斜 嬪纂縑 Image陛 氈朝雖 匐餌и棻.
			if(clGrp.GetPixel(0, 0) != 254) fCheck = TRUE;

			clGrp.UnlockSurface(TempSurface);
		}

		// 檜瞪 Data 犒掘
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

		// 鎰朝 晦遽 紫お 嬪纂蒂 掘и棻.
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

		// Surface縑 棻陛 Copyп憮 斜 嬪纂蒂 匐餌и棻.
		TempSurface = pMyOnlineWorld->pIOnlineObject->GetTempSurface();
		if(TempSurface == NULL) return FALSE;

		// 檜瞪 Data 盪濰
		if(pSurface) clGrp.UnlockSurface(pSurface);
		clGrp.GetClipArea(TempX1, TempX2, TempY1, TempY2);

		fCheck = FALSE;
		if(clGrp.LockSurface(TempSurface) == TRUE)
		{
			clGrp.SetClipArea(0, 0, 2, 2);

			clGrp.PutPixel(0, 0, (BYTE)254);

			// 辦摹 Surface縑 斜萼棻.
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

			// 斜 嬪纂縑 Image陛 氈朝雖 匐餌и棻.
			if(clGrp.GetPixel(0, 0) != 254) fCheck = TRUE;

			clGrp.UnlockSurface(TempSurface);
		}

		// 檜瞪 Data 犒掘
		clGrp.SetClipArea(TempX1, TempX2, TempY1, TempY2);
		if(pSurface) clGrp.LockSurface(pSurface);

		return fCheck;
	}

	return FALSE;
}
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Tile Draw 檣雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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