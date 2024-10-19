#include "SOMain.h"
#include "SOWorld.h"
#include "SOMap.h"
#include "OnlineObjectParser.h"
#include "OnlineFieldParser.h"
#include "GSCDefine.h"

SOWorld	*	SOMap::pMyWorld;					// World

//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
SOMap::SOMap() : OnlineBaseMap()
{		
	pstMap				=	NULL;
	puiMultiplyMapYOff	=	NULL;
	bUpdateDBSwitch		=	FALSE;
	m_pSectorNode		=	NULL;
	m_pstSector			=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
SOMap::~SOMap()
{
	delete [] m_pSectorNode;
	Free(); 
}  

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-07-26 오후 5:13:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::Init(MapIndexHeader *pMapIndexHeader)
{	

	LoadSaveMapHeader		Header;
	BOOL					bRet	=	FALSE;
	FILE					*fp		=	NULL;
	SI32					i, j, k, l;
	LoadSaveMapTI			*pReadMap  = NULL;
	SI32					TempCount;	
	SI32					siSectorIndex, siTempSectorIndex;
	SI32					siNearSectorCount;
	
	// 맵을 로드한다.
	if((fp = fopen(pMapIndexHeader->szMapFileName, "rb")) != NULL)
	{	
		if(fread(&Header, sizeof(LoadSaveMapHeader), 1, fp) != 1)
			goto END;		

		puiMultiplyMapYOff	=	new UI32[Header.siYsize];		
		if(puiMultiplyMapYOff == NULL)			goto END;		
		for(i = 0; i < Header.siYsize; i++)
			puiMultiplyMapYOff[i]	=	i * Header.siXsize;		

		// 맵을 저장할 메모리를 할당 받는다.
		pReadMap	=	new	LoadSaveMapTI[Header.siXsize * Header.siYsize];
		if(pReadMap == NULL)	goto END;						
		
		for(TempCount = 0; TempCount < Header.siXsize * Header.siYsize; TempCount++)
		{
			if(fread(&pReadMap[TempCount].stLoadSaveTile, sizeof(LoadSaveTile), 1, fp) != 1)
				goto END;
			if(fread(&pReadMap[TempCount].stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fp) != 1)
				goto END;
			if(fread(&pReadMap[TempCount].uiObjectInfo, sizeof(UI16), 1, fp) != 1)
				goto END;
		}

		m_siXsize		=	Header.siXsize;
		m_siYsize		=	Header.siYsize;				
		

		// 맵에 속성을 설정해준다.
		if(SetMap(pReadMap) == FALSE)
			goto END;		

		uiMapID			=	pMapIndexHeader->siMapIndex;
		siMapType		=	pMapIndexHeader->siMapType;
		uiVillageCode	=	pMapIndexHeader->uiVillageCode;
		bIsFieldMap		=	(pMapIndexHeader->fWarFlag == 2);
		siWeather		=	pMapIndexHeader->siWeather;

		// 가로 섹터 수를 얻어온다.
		if(m_siXsize % ON_TILENUM_PER_SECTOR == 0)			siHorizonSectorNum	=	m_siXsize / ON_TILENUM_PER_SECTOR;	
		else												siHorizonSectorNum	=	m_siXsize / ON_TILENUM_PER_SECTOR + 1;	

		// 세로 섹터 수를 얻어온다.
		if(m_siYsize % ON_TILENUM_PER_SECTOR == 0)			siVerticalSectorNum	=	m_siYsize / ON_TILENUM_PER_SECTOR;	
		else												siVerticalSectorNum	=	m_siYsize / ON_TILENUM_PER_SECTOR + 1;	

		/*
		siHorizonSectorNum *= 2;
		siVerticalSectorNum *= 2;
		*/

		m_pstSector	=	new	strSector[ siHorizonSectorNum * siVerticalSectorNum ];
	
		if(m_pstSector == NULL)	goto	END;

		siSectorIndex = -1;

		for(i = 0; i < siVerticalSectorNum; i++) {
			for(j = 0; j < siHorizonSectorNum; j++) {

				siSectorIndex++;

				// 각 섹터를 초기화 한다.				
				InitSector( siSectorIndex );		
				
				m_pstSector[ siSectorIndex ].uiY = i;
				m_pstSector[ siSectorIndex ].uiX = j;

				// 주위 섹터의 포인터를 지정한다.
				siNearSectorCount	=	0;
				
				for(k = -1; k <= 1; k++) {
					for(l = -1; l <= 1; l++) {	
						siTempSectorIndex	=	(i + k) * siHorizonSectorNum + (j + l);

						if( (i + k) >= 0 && (i + k) < siVerticalSectorNum &&
							(j + l) >= 0 && (j + l) < siHorizonSectorNum ) {

								m_pstSector[siSectorIndex].pNearSector[siNearSectorCount]	=	&m_pstSector[siTempSectorIndex];
								siNearSectorCount++;							
						}
					}
				}	
			}
		}		
	
		bRet		=	TRUE;
		}

		m_pSectorNode = new cSectorNode[ siHorizonSectorNum * siVerticalSectorNum ];

		for( i = 0; i < siHorizonSectorNum * siVerticalSectorNum; ++i ) {
			m_sectorlist.AddToFree( &m_pSectorNode[ i ] );
		}

END:
	if(fp)
	{
		fclose(fp);
		fp	=	NULL;
	}
	
	if(bRet == FALSE)
	{
		// 할당 받았던 모든 것을 해제한다.
		Free();
	}

	if(pReadMap != NULL)
	{
		delete [] pReadMap;
		pReadMap	=	NULL;
	}

	return bRet;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제.
//----------------------------------------------------------------------------------------------------------------
VOID	SOMap::Free()
{
	SI32	i;

	if(pstMap)
	{
		delete [] pstMap;
		pstMap	=	NULL;
	}

	if(puiMultiplyMapYOff)
	{
		delete [] puiMultiplyMapYOff;
		puiMultiplyMapYOff	=	NULL;
	}

	m_siXsize	=	0;
	m_siYsize	=	0;

	if(m_pstSector != NULL)
	{
		for(i = 0; i < siHorizonSectorNum * siVerticalSectorNum; i++)
			FreeSector(i);		

		delete [] m_pstSector;
		m_pstSector	=	NULL;
	}


}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터를 설정한다.
//	수정 일자	:	2002-06-03 오후 3:30:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo)
{
	SI32		siNearCharNum;

	if(pBaseChar->uiAccount == 0)																				return	FALSE;	// 유효하지 않은 ID이다.
	if(IsInMapArea(pBaseChar->siX, pBaseChar->siY) == FALSE)													return	FALSE;	// 맵 영역 밖에 있다.
	if(pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiTileAtb & ON_TILE_ATB_COLL)				return	FALSE;	// 충돌 지역이다.		
	if((pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiTileKind & uiCharMoveableTileInfo) == 0)	return	FALSE;	// 해당 타일에 캐릭터가 설수 있는 타일인지 판단한다.
	if(pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID != 0)							return	FALSE;	// 이미 다른 플레이어가 심겨져 있다.		
	
	// 섹터에 추가 시킨다.
	if( AddCharToSector(pBaseChar->siX, pBaseChar->siY, pBaseChar) == FALSE )									return	FALSE;	// 섹터에 플레이어를 추가 시킨다.	

	SetCharID(pBaseChar->siX, pBaseChar->siY, pBaseChar);	

	// 주위에 플레이어들을 구한다.
	siNearCharNum =	GetNearSectorPlayerList(pBaseChar->siX, pBaseChar->siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
	pBaseChar->SendFieldMsg( ON_ADDCHAR, SO_SFM_CUSTOM, LPVOID( siNearCharNum ), LPVOID( uiCharUniIDs ) );	
	
	return	TRUE;	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 이동 했을 때 새로 추가되는 섹터 와 삭제되는 섹터를 얻는다. 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void    SOMap::GetNewSectorList( SI32 siOldIndex, SI32 siNewIndex, strSector **pAddSector, strSector **pDelSector )
{
	
	UI32 i, j;
	UI32 uiAddCount, uiDelCount;
	BOOL bAdd, bDel;

	uiAddCount = uiDelCount = 0; 

	memset( pAddSector, 0, 4 * 9 );
	memset( pDelSector, 0, 4 * 9 );

	for( i = 0; i < 9; ++i ) { 
		bAdd = bDel = TRUE;
		for( j = 0; j < 9; ++j ) { 
			if( m_pstSector[ siNewIndex ].pNearSector[ i ] == 
				m_pstSector[ siOldIndex ].pNearSector[ j ] ) { bAdd = FALSE; break; }
		}
	
		if( bAdd && m_pstSector[ siNewIndex ].pNearSector[ i ] ) 
			pAddSector[ uiAddCount++ ] = m_pstSector[ siNewIndex ].pNearSector[ i ];

		for( j = 0; j < 9; ++j ) { 
			if( m_pstSector[ siOldIndex ].pNearSector[ i ] == 
				m_pstSector[ siNewIndex ].pNearSector[ j ] ) { bDel = FALSE; break; }
		} 

		if( bDel && m_pstSector[ siOldIndex ].pNearSector[ i ] ) 
			pDelSector[ uiDelCount++ ] = m_pstSector[ siOldIndex ].pNearSector[ i ]; 
	} 

}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터를 이동 시킨다.
//	수정 일자	:	2002-06-03 오전 11:38:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::MoveChar( BaseChar *pBaseChar, SI32 siX, SI32 siY, BOOL bWarUnit,UI32 uiCharMoveableTileInfo)
{
	SI32						siCurSectorIndex, siDestSectorIndex;
	SI32						siAddCharsNum, siDelCharsNum, siAddItemsNum;
	SI32						siNewInfoOnMove[3];			// 0번은 삭제해야할 캐릭터이고 1번은 삭제해야할 캐릭터이다.	


	SI32						i;
	SOPlayer					*pPlayer;	
	cltSOShip					*pSOShip;
	UI16						uiCustomerNum;
	static UI16					uiCustoms[SO_SHIP_MAX_CUSTOMER];	


	static strSector			*pAddSector[ 9 ];				// 새로 추가되는 섹터의 포인터를 저장할 곳
	static strSector			*pDelSector[ 9 ];				// 삭제할 섹터의 포인터를 저장할 곳

	if(pBaseChar->uiAccount == 0)															return	FALSE;	// 유효하지 않은 ID이다.
	if(IsInMapArea(siX, siY) == FALSE)														return	FALSE;	// 맵 영역 밖에 있다.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)					return	FALSE;	// 충돌 지역이다.		
	if((pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind & uiCharMoveableTileInfo) == 0)	return	FALSE;	// 해당 타일에 캐릭터가 설수 있는 타일인지 판단한다.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)								return	FALSE;	// 이미 다른 플레이어가 심겨져 있다.	
				
	// 이동하려는 섹터가 현재 있는 섹터와 같은지 검사한다.
	siCurSectorIndex	=	GetSectorIndex(pBaseChar->siX, pBaseChar->siY);		// 현재 캐릭터가 위치하고 있는 섹터를 구한다.
	siDestSectorIndex	=	GetSectorIndex(siX, siY);							// 이동할 위치의 섹터를 구한다.

	// 이동할 섹터가 현재 섹터의 주위 섹터인지 검사한다.
	if( IsNearSector( siCurSectorIndex, siDestSectorIndex ) == FALSE )
	{
//		pMyWorld->clServer->WriteInfo( "..\\MoveCharError.txt", "[ ID : %s ], [ %u ] [ %d, %d ] --> [ %d, %d ]",
		writeInfoToFile( "MoveCharError.txt", "[ ID : %s ], [ %u ] [ %d, %d ] --> [ %d, %d ]",
										pBaseChar->szID, pBaseChar->uiMapID, pBaseChar->siX, pBaseChar->siY,
										siX, siY );
										
		

		return	FALSE;
	}


	// 다른 섹터로 이동할 경우의 처리를 해준다..
	if(siCurSectorIndex != siDestSectorIndex)
	{
		// 추가되는 섹터와 삭제되는 섹터를 구한다		
		GetNewSectorList( siCurSectorIndex, siDestSectorIndex, pAddSector, pDelSector );		

		// 새로운 섹터에 캐릭터를 추가 시킨다.
		if( AddCharToSector(siDestSectorIndex, pBaseChar) == TRUE )
		{
			// 기존의 섹터에서 캐릭터를 제거하고 --------------------------------------------------------*
			DelCharToSector(siCurSectorIndex, pBaseChar);		

			// 섹터가 변경되었다.
			siAddItemsNum = GetSectorItemList( pAddSector, FieldItemInfo );
			siDelCharsNum = GetSectorPlayerList( pDelSector, &uiAddDelCharsID[ 0 ], ON_MAP_CHARID_FIELD );							
			siAddCharsNum = GetSectorPlayerList( pAddSector, &uiAddDelCharsID[ siDelCharsNum ], ON_MAP_CHARID_FIELD );				

			siNewInfoOnMove[0]	=	siDelCharsNum;			// 삭제해야 하는 캐릭터의 수.		
			siNewInfoOnMove[1]	=	siAddCharsNum;			// 추가해야 하는 캐릭터의 수.		
			siNewInfoOnMove[2]	=	siAddItemsNum;			// 추가해야 하는 아이템의 수.		

			if(ISPLAYER(pBaseChar->uiAccount) == TRUE)
			{
				pPlayer	=	(SOPlayer*)pBaseChar;
					
				// 나에게 새로운 정보를 보내준다.
				pPlayer->SendFieldMsg(ON_NEWINFOONMOVE, SO_SFM_ONLYME, LPVOID(siNewInfoOnMove), uiAddDelCharsID, FieldItemInfo);
			}		
			else if(ISSHIP(pBaseChar->uiAccount) == TRUE)
			{
				pSOShip			=	(cltSOShip*)pBaseChar;

				// 배에탄 플레이어들에게 새로운 정보를 보내준다.
				uiCustomerNum	=	pSOShip->GetCustomers(uiCustoms);

				for(i = 0; i < uiCustomerNum; i++)
				{
					if((pPlayer = pMyWorld->pPlayerOnAccount[uiCustoms[i]]) != NULL)
						pPlayer->SendFieldMsg(ON_NEWINFOONMOVE, SO_SFM_ONLYME, LPVOID(siNewInfoOnMove), uiAddDelCharsID, FieldItemInfo);
				}		
			}
			

			// 내 주위의 플레이어들에게 나를 삭제 및 추가하라고 알려줘야 한다.
			siDelCharsNum = GetSectorPlayerList( pDelSector, &uiAddDelCharsID[ 0 ], ON_MAP_CHARID_ALLPLAYER );							
			siAddCharsNum = GetSectorPlayerList( pAddSector, &uiAddDelCharsID[ siDelCharsNum ], ON_MAP_CHARID_ALLPLAYER );						

			// 내 캐릭터 삭제 정보를 보내준다. 벗어나는 섹터영역에대해서..			
			pBaseChar->SendFieldMsg(ON_DELCHAR, SO_SFM_CUSTOM, LPVOID( siDelCharsNum ), LPVOID( uiAddDelCharsID ) );

			// 내 캐릭터 추가 정보를 보내준다. 추가되는 섹터영역에대해서..			
			pBaseChar->SendFieldMsg(ON_ADDCHAR, SO_SFM_CUSTOM, LPVOID( siAddCharsNum ), LPVOID( &uiAddDelCharsID[ siDelCharsNum ]) );		
		}	
		else
		{
			// 섹터를 이동 시키는데 실패했다.
			return	FALSE;

		}
	}

	DelCharID( pBaseChar->siX, pBaseChar->siY );									// 맵에서 캐릭터의 ID를 삭제해준다.
	SetCharID( siX, siY, pBaseChar );												// 맵에 캐릭터의ID를 설정해준다.
		
	// 캐릭터의 새로운 위치를 지정해준다.
	pBaseChar->siX	=	siX;
	pBaseChar->siY	=	siY;	

	if(bWarUnit == TRUE)
		pBaseChar->SendFieldMsg( ON_RESPONSE_POSITION, SO_SFM_MEANDNEARCHAR, LPVOID( &pBaseChar->uiAccount ), LPVOID( 1 ) );
	else // 캐릭터가 이동했다고 설정한다.
		pBaseChar->SetMoved();

	return	TRUE;
} 

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터를 삭제한다.
//	수정 일자	:	2002-06-03 오후 3:32:04 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::DelChar(BaseChar *pBaseChar)
{

	// 맵에서 지우려고 하는 캐릭터의 ID가 내 캐릭터의 ID 여야 한다.
	if( pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID == pBaseChar->uiAccount )		
	{
		DelCharID(pBaseChar->siX, pBaseChar->siY);
	
		// 섹터에서 삭제한다.
		DelCharToSector(pBaseChar->siX, pBaseChar->siY, pBaseChar);		
		
		pBaseChar->SendFieldMsg( ON_DELCHAR, SO_SFM_ONLYNEARCHAR ); 
		
		return	TRUE;
	}
	else																													
	{
		// 맵에서 지우려고 하는 캐릭터의 ID가 내 캐릭터의 ID가 아니다.
//		pMyWorld->clServer->WriteInfo("..\\log_for_debug.txt", "2 [Map ID : %d]  [uiAccount willing to be deleted : %d]  [ID from the x, y on the map ID : %d]",
		writeInfoToFile("log_for_debug.txt", "2 [Map ID : %d]  [uiAccount willing to be deleted : %d]  [ID from the x, y on the map ID : %d]",
										pBaseChar->uiMapID, pBaseChar->uiAccount, pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID );
		return	FALSE;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	단순히 캐릭터의 ID를 맵에 설정만 해준다.
//	수정 일자	:	2002-06-03 오전 11:47:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::SetCharID(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	pBaseChar->uiAccount;			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	단순히 캐릭터의 ID를 맵에서 삭제해 준다.
//	수정 일자	:	2002-06-03 오전 11:57:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::DelCharID(SI32 siX, SI32 siY)
{
	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	0;			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	단순히 맵에 아이템을 설정만 해준다.
//	수정 일자	:	2002-06-03 오후 12:17:04 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::SetItemID( SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	DBIKFieldItemInfo	FieldItemInfo;
	BOOL				bExistItem;

	if( pstMap[ puiMultiplyMapYOff[ siY ] + siX ].uiItemID == uiItemID )
	{
		bExistItem	=	TRUE;
	}
	else
	{
		bExistItem	=	FALSE;

		pstMap[ puiMultiplyMapYOff[ siY ] + siX ].uiItemID				=	uiItemID;
	}
	
	pstMap[ puiMultiplyMapYOff[ siY ] + siX ].uiItemQuantity		=	uiItemQuantity;

	// DB에 정보를 업데이트한다.
	if( bUpdateDBSwitch == TRUE )
	{
		if( bSendToDB == TRUE )
		{
			// DB에 아이템을 저장한다.
			FieldItemInfo.uiMapID		=	uiMapID;
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;
			FieldItemInfo.uiID			=	uiItemID;
			FieldItemInfo.uiQuantity	=	uiItemQuantity;
		
			pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_APPENDFIELDITEM, &FieldItemInfo, sizeof( FieldItemInfo ) );
		}

	}

	// 기존의 이 타일이 섹터 정보에 이미 추가되어 있으면 
	// 더 이상 추가하지 않는다.
	if( bExistItem == FALSE )
	{
		// 해당 섹터에도 포함 시킨다.			
		AddItemToSector( siX, siY, &pstMap[ puiMultiplyMapYOff[ siY ] + siX ] );
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	단순히 맵에 아이템을 제거만 해준다.
//	수정 일자	:	2002-06-03 오후 12:17:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::DelItemID( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	DBIKFieldItemInfo	FieldItemInfo;

	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	0;
	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	0;	

	// DB에 정보를 업데이트한다.
	if(bUpdateDBSwitch == TRUE)
	{
		if( bSendToDB == TRUE )
		{
			// DB에서도 아이템을 삭제한다.
			FieldItemInfo.siAccount		=	pPlayer->stAccountInfo.siDBAccount;
			FieldItemInfo.uiSlot		=	pPlayer->stAccountInfo.uiSlot;
			FieldItemInfo.uiMapID		=	uiMapID;
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;			
		
			pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEFIELDITEM, &FieldItemInfo, sizeof(FieldItemInfo));
		}
	}

	DelItemToSector( siX, siY, &pstMap[ puiMultiplyMapYOff[ siY ] + siX ] );
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	맵에 ID를 설정한다.
//	수정 일자	:	2002-05-21 오후 3:17:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::SetID(SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI32 uiCharMoveableTileInfo)
{	
	if(IsInMapArea(siX, siY) == FALSE)
	{
		return FALSE;	// 맵 영역 밖에 있다.
	}

	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)
	{
		return FALSE;	// 충돌 지역이다.
	}

	if(pBaseChar->uiAccount != 0)
	{
		// 맵에 캐릭터를 설정한다.

		// 우선 해당 타일에 캐릭터가 설수 있는 타일인지 판단한다.
		if((pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind & uiCharMoveableTileInfo) == 0)
		{
			return FALSE;
		}

		if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)
		{
			// 이미 다른 플레이어가 심겨져 있다.
			return FALSE;
		}
		else
		{
			SetCharID(siX, siY, pBaseChar);
			return TRUE;
		}	
	}

	if(uiItemID != 0 && uiItemQuantity != 0)
	{
		// 맵에 아이템을 설정한다.

		if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0)
		{
			// 이미 다른 아이템이 심겨져있다.
			return FALSE;
		}
		else
		{
			pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	uiItemID;
			pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	uiItemQuantity;

			// DB에 정보를 업데이트한다.
			if(bUpdateDBSwitch == TRUE)
			{
				DBIKFieldItemInfo	FieldItemInfo;

				// DB에 아이템을 저장한다.
				FieldItemInfo.siX			=	siX;
				FieldItemInfo.siY			=	siY;
				FieldItemInfo.uiID			=	uiItemID;
				FieldItemInfo.uiQuantity	=	uiItemQuantity;
				pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDFIELDITEM, LPVOID(&FieldItemInfo), NULL);
			}

			// 해당 섹터에도 포함 시킨다.			
			AddItemToSector(siX, siY, &pstMap[puiMultiplyMapYOff[siY] + siX]);
			return TRUE;
		}	
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	맵에서 ID를 삭제한다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOMap::DelID(SI32 siX, SI32 siY, SI32 siDelInfo)
{
	if(IsInMapArea(siX, siY) == FALSE)	return;

	if(siDelInfo & ON_MAP_DELID_CHAR)
	{
		pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	0;		
	}

	if(siDelInfo & ON_MAP_DELID_ITEM)
	{
		pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	0;
		pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	0;

		// DB에 정보를 업데이트한다.
		if(bUpdateDBSwitch == TRUE)
		{
			DBIKFieldItemInfo	FieldItemInfo;

			// DB에서도 아이템을 삭제한다.
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;			
			pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_DELETEFIELDITEM, LPVOID(&FieldItemInfo), NULL);
		}

		// 아이템이 있는 타일을 삭제 시켜준다.
		DelItemToSector(siX, siY, &pstMap[puiMultiplyMapYOff[siY] + siX]);		
	}	
}
*/
//----------------------------------------------------------------------------------------------------------------
//	설명	:	맵에 속성을 설정해준다.
//----------------------------------------------------------------------------------------------------------------
BOOL	SOMap::SetMap(LoadSaveMapTI* pMap)
{
	SI32				i, j, k;
	ObjectInfoBfs		*pObjectInfoBfs;
//	UI16				uiFontIndex;	
	OnlineObjectParser	ObjectParser;
	OnlineFieldParser	FieldParser;
	ObjectHeader		*pObjectHeader;
	MilepostHeader		*pFieldObject;
	SI32				TempX, TempY;
	SI32				TempX1, TempY1;

	if(ObjectParser.Init("Online\\Object\\Object.txt") == FALSE)
		return FALSE;

	if(FieldParser.Init("Online\\Object\\Field Object.txt") == FALSE)
		return FALSE;

	pstMap				=	new	strTI[ m_siXsize * m_siYsize ];


	if(pstMap == NULL)
	{
		printf("allocate map memory failed.");
		return FALSE;
	}
	
	ZeroMemory(pstMap, sizeof(strTI) * m_siXsize * m_siYsize);

	// 오브젝트인 경우 캐릭터와 충돌하는 오브젝트가 있고 그냥 통과하는 오브젝트가 있다.
	// 그 여부를 미리 계산한다.
	for(i = 0; i < m_siYsize; i++)
	{
		for(j = 0; j < m_siXsize; j++)
		{			
			pObjectInfoBfs	=	(ObjectInfoBfs*)&pMap[puiMultiplyMapYOff[i] + j].uiObjectInfo;

			switch(pObjectInfoBfs->BaseBf.uiCode)
			{
			case ON_MAP_OI_CODE_KIND:				
				pObjectHeader	=	ObjectParser.GetObject(pObjectInfoBfs->BaseBf.siParameter);

				/*
				if(pObjectHeader->siAttribute & ATB_OBJECT_ISOBJECT)
					pstMap[puiMultiplyMapYOff[i] + j].uiTileAtb		|=	ON_TILE_ATB_COLL;
					*/
				break;
			case ON_MAP_OI_CODE_VILLAGE:
				//pstMap[puiMultiplyMapYOff[i] + j].uiTileAtb		|=	ON_TILE_ATB_COLL;
				pstMap[puiMultiplyMapYOff[i] + j].uiTileAtb		|=	ON_TILE_ATB_VILLAGE;				
				break;
			}
		}
	}
	
	for(i = 0; i < m_siYsize; i++)
	{
		for(j = 0; j < m_siXsize; j++)
		{	
			pstMap[puiMultiplyMapYOff[i] + j].uiX				=	j;
			pstMap[puiMultiplyMapYOff[i] + j].uiY				=	i;
			pstMap[puiMultiplyMapYOff[i] + j].uiCharUniID		=	0;
			pstMap[puiMultiplyMapYOff[i] + j].uiItemID			=	0;
			pstMap[puiMultiplyMapYOff[i] + j].uiItemQuantity	=	0;

			pObjectInfoBfs	=	(ObjectInfoBfs*)&pMap[puiMultiplyMapYOff[i] + j].uiObjectInfo;			

			// 타일 종류도 얻어온다.
			if(pMap[puiMultiplyMapYOff[i] + j].stLoadSaveTile.uiAtb & 0x8)
			{
				pstMap[puiMultiplyMapYOff[i] + j].uiTileAtb	 |= ON_TILE_ATB_COLL;
			}
			if(pMap[puiMultiplyMapYOff[i] + j].stLoadSaveTile.uiAtb & 0x1)
			{
				pstMap[puiMultiplyMapYOff[i] + j].uiTileKind = ON_TILE_KIND_GROUND;			
			}
			if(pMap[puiMultiplyMapYOff[i] + j].stLoadSaveTile.uiAtb & 0x2)
			{
				pstMap[puiMultiplyMapYOff[i] + j].uiTileKind = ON_TILE_KIND_SEA;
			}

			/*
			// 충돌 오브젝트가 심겨져 있는지 알아온다.
			switch(pObjectInfoBfs->BaseBf.uiCode)
			{			
			case ON_MAP_OI_CODE_RELATIVEPOS:
				// 원 위치의 오브젝트가 충돌 속성을 가지고 있으면 이것도 충돌 속성을
				// 가지고 있다.
				siOX	=	j + pObjectInfoBfs->RelativePosBf.siX;
				siOY	=	i + pObjectInfoBfs->RelativePosBf.siY;

				if(pstMap[puiMultiplyMapYOff[siOY] + siOX].uiTileAtb & ON_TILE_ATB_COLL)
					pstMap[puiMultiplyMapYOff[i] + j].uiTileAtb		|=	ON_TILE_ATB_COLL;
				break;			
			}
			*/
		}
	}

	for(i = 0; i < FieldParser.GetTotalNum(); i++)
	{
		pFieldObject = FieldParser.GetMilepost(i);

		TempX = ((pFieldObject->uiID & 0xffff0000) >> 16);
		TempY = (pFieldObject->uiID & 0xffff);

		for(j = -1; j < 1; j++)
		{
			for(k = -1; k < 1; k++)
			{
				TempX1 = TempX + j;
				TempY1 = TempY + k;
				if((TempX1 >= 0) && (TempX1 < m_siXsize) && (TempY1 >= 0) && (TempY1 < m_siYsize))
				{
					pstMap[puiMultiplyMapYOff[TempY1] + TempX1].uiTileAtb |= ON_TILE_ATB_COLL;
				}
			}
		}
	}

	FieldParser.Free();
	ObjectParser.Free();

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	일정 영역에 있는 캐릭터들의 ID를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	SOMap::GetCharUniID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, UI16 uiMaxGetCharIDNum, SI32 siGetCharInfo)
{
	SI32	i, j;
	SI32	siCharCounter;
	UI16	uiID;

	CorrectionPos(&siSX, &siSY, &siEX, &siEY);

	siCharCounter	=	0;
	for(i = siSY; i <= siEY; i++)
	{
		for(j = siSX; j <= siEX; j++)
		{
			uiID	=	pstMap[puiMultiplyMapYOff[i] + j].uiCharUniID;

			if(uiID != 0)
			{
				if((siGetCharInfo & ON_MAP_CHARID_PLAYER) && ISPLAYER(uiID) == TRUE)
				{
					pCharUniIDs[siCharCounter]		=	uiID;
					siCharCounter++;	
				}
				
				if((siGetCharInfo & ON_MAP_CHARID_MONSTER) && ISMONSTER(uiID) == TRUE)
				{
					pCharUniIDs[siCharCounter]		=	uiID;
					siCharCounter++;	
				}				

				if((siGetCharInfo & ON_MAP_CHARID_SHIP) && ISSHIP(uiID) == TRUE)
				{
					pCharUniIDs[siCharCounter]		=	uiID;
					siCharCounter++;	
				}				

				if(uiMaxGetCharIDNum == siCharCounter)
					return siCharCounter;
			}
		}
	}

	return siCharCounter;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	맵에서 캐릭터의 ID를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
UI32	SOMap::GetCharUniID(SI32 siX, SI32 siY, SI32 siGetCharInfo)
{
	UI16	uiID;

	if( IsInMapArea(siX, siY) == FALSE ) return 0;	

	uiID	=	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID;

	if(uiID == 0)	
		return 0;	

	if((siGetCharInfo & ON_MAP_CHARID_PLAYER) && ISPLAYER(uiID) == TRUE)	
		return uiID;
		
	if((siGetCharInfo & ON_MAP_CHARID_MONSTER) && ISMONSTER(uiID) == TRUE)	
		return uiID;

	if((siGetCharInfo & ON_MAP_CHARID_SHIP) && ISSHIP(uiID) == TRUE)	
		return uiID;

	return 0;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	일정 영역에 있는 아이템의 ID를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	SOMap::GetItemID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pItemIDs, UI16 *pItemQuantity)
{
	SI32	i, j;
	SI32	siItemCounter;

	CorrectionPos(&siSX, &siSY, &siEX, &siEY);
		
	siItemCounter	=	0;
	for(i = siSY; i <= siEY; i++)
	{
		for(j = siSX; j <= siEX; j++)
		{
			if(pstMap[puiMultiplyMapYOff[i] + j].uiItemID != 0)
			{
				pItemIDs[siItemCounter]			=	pstMap[puiMultiplyMapYOff[i] + j].uiItemID;
				pItemQuantity[siItemCounter]	=	pstMap[puiMultiplyMapYOff[i] + j].uiItemQuantity;
				siItemCounter++;
			}
		}
	}

	return siItemCounter;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	한 타일에 설정되어 있는 아이템의 정보를 얻어온다.
//	리턴	:	그 타일에 아이템이 있으면 TRUE. 아이템이 없으면 FALSE.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::GetItemID(SI32 siX, SI32 siY, UI16 *pItemIDs, UI16 *pItemQuantity)
{
	// 유효한 지역인지 체크한다.
	if(IsInMapArea(siX, siY) == FALSE)	return FALSE;	

	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0)
	{	
		*pItemIDs		=	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID;
		*pItemQuantity	=	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity;

		return TRUE;
	}
	else
		return FALSE;

}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	좌표를 교정해준다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOMap::CorrectionPos(SI32 *psiSX, SI32 *psiSY, SI32 *psiEX, SI32 *psiEY)
{
	*psiSX	=	max(0, *psiSX);
	*psiSY	=	max(0, *psiSY);
	*psiEX	=	min(m_siXsize - 1, *psiEX);
	*psiEY	=	min(m_siYsize - 1, *psiEY);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	맵에 설정되어 있는 캐릭터의 id나 item의 id를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOMap::GetID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, 
					 OnlineReceiveFieldItemInfo *pFieldItemInfo, UI16 *psiCharNum, UI16 *psiItemNum, SI32 siGetCharInfo, 
					UI16 uiExceptionCharID)
{
	SI32	i, j;
	SI32	siItemCounter;
	SI32	siCharCounter;
	UI16	uiID;

	CorrectionPos(&siSX, &siSY, &siEX, &siEY);

	if(pCharUniIDs != NULL)
	{
		siCharCounter	=	0;
		for(i = siSY; i <= siEY; i++)	
			for(j = siSX; j <= siEX; j++)
			{				
				uiID	=	pstMap[puiMultiplyMapYOff[i] + j].uiCharUniID;
	

				if(uiID != 0 && uiExceptionCharID != uiID)
				{
					if((siGetCharInfo & ON_MAP_CHARID_PLAYER) && ISPLAYER(uiID) == TRUE)
					{					
						// 실제 포인터를 가지고 있는지 확인한다.
						if(pMyWorld->pPlayerOnAccount[uiID] != NULL)
						{
							pCharUniIDs[siCharCounter]		=	uiID;
							siCharCounter++;						
						}
					}					

					if((siGetCharInfo & ON_MAP_CHARID_MONSTER) && ISMONSTER(uiID) == TRUE)
					{
						if(pMyWorld->clMonster.IsValidID(uiID) == TRUE)
						{
							pCharUniIDs[siCharCounter]		=	uiID;
							siCharCounter++;	
						}
					}				


					if((siGetCharInfo & ON_MAP_CHARID_SHIP) && ISSHIP(uiID) == TRUE)
					{
						pCharUniIDs[siCharCounter]		=	uiID;
						siCharCounter++;	
					}				
				}
			}

		*psiCharNum	=	siCharCounter;
	}

	if(pFieldItemInfo != NULL)
	{
		siItemCounter	=	0;
		for(i = siSY; i <= siEY; i++)	
			for(j = siSX; j <= siEX; j++)
			{
				if(pstMap[puiMultiplyMapYOff[i] + j].uiItemID != 0)
				{
					pFieldItemInfo[siItemCounter].uiItemID		=	pstMap[puiMultiplyMapYOff[i] + j].uiItemID;
					pFieldItemInfo[siItemCounter].uiQuantity	=	pstMap[puiMultiplyMapYOff[i] + j].uiItemQuantity;
					pFieldItemInfo[siItemCounter].siX			=	j;
					pFieldItemInfo[siItemCounter].siY			=	i;
					siItemCounter++;
				}
			}

		*psiItemNum		=	siItemCounter;
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	맵에서 특정 좌표를 찾는다.
//	수정 일자	:	2002-08-05 오전 11:04:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
POINT	SOMap::SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 )
{

	POINT	TempPoint;
	SI32	TempDistance;
	SI32	TempCount;
	SI32	TempX;
	SI32	TempY;
	SI32	siDirectionCounter;
	strTI	*pTileInfo;
	SI32	siSameItemSearchArea	=	3;			// 같은 아이템이 주변에 있는지 검색할 때 그 영역의 크기(반지름)

	// 캐릭터를 심을 위치를 찾을때.
	if(siSearchInfo == ON_MAP_SI_CHAR)
	{
		if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
		{
			pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

			if((pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && pTileInfo->uiCharUniID == 0 &&
				(pTileInfo->uiTileKind & siParam1))
			{
				TempPoint.x = siCX;
				TempPoint.y = siCY;
				return TempPoint;
			}
		}

		for(TempDistance = 1; TempDistance < 200; TempDistance++)
		{
			for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
			{
				for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
				{
					switch(siDirectionCounter)
					{
					case 0:			// 위
						TempX = siCX + TempCount;
						TempY = siCY - TempDistance;
						break;
					case 1:			// 아래
						TempX = siCX - TempCount;
						TempY = siCY + TempDistance;
						break;
					case 2:			// 좌					
						TempX = siCX - TempDistance;
						TempY = siCY - TempCount;
						break;
					case 3:			// 우
						TempX = siCX + TempDistance;
						TempY = siCY + TempCount;
						break;
					}
					
					if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
					{
						pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];
						
						if((pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && pTileInfo->uiCharUniID == 0 && (pTileInfo->uiTileKind & siParam1))
						{
							TempPoint.x = TempX;
							TempPoint.y = TempY;
							return TempPoint;
						}
					}			
				}
			}
		}
	}
	else if(siSearchInfo == ON_MAP_SI_ITEM)
	{
		// siParam1: Item ID

		// 필드에 버리는 아이템은 ON_MAX_ITEMNUM_ON_TILE을 초과할 수 없다.
		if( siParam2 <= ON_MAX_ITEMNUM_ON_TILE )
		{		

			// 서있는 자리에 같은 종류의 아이템이 있는지 검사해서 있으면 그 위치를 리턴한다.
			if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
			{
				pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

				// 일단 충돌 타일이 아니고 타일 종류는 땅이어야 한다.
				if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == siParam1) )
				{
					// ---- 20050216 - 윤영대 수정
					// 한 타일에 놓일 수 있는 최대 아이템의 수를 초과하지 않는지 검사한다.
					if( SI32( pTileInfo->uiItemQuantity + siParam2 ) <= ON_MAX_ITEMNUM_ON_TILE )
					{
						TempPoint.x = siCX;
						TempPoint.y = siCY;
						return TempPoint;
					}
					// ----
//					TempPoint.x = siCX;
//					TempPoint.y = siCY;
//					return TempPoint;
				}
			}

			// 주변을 차례로 돌면서 같은 종류의 아이템이 있는지 검사해서 있으면 그 위치를 리턴한다. (siSameItemSearchArea만큼)
			for(TempDistance = 1; TempDistance <= siSameItemSearchArea; TempDistance++)
			{
				for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
				{
					for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
					{
						switch(siDirectionCounter)
						{
						case 0:			// 위
							TempX = siCX + TempCount;		TempY = siCY - TempDistance;		break;
						case 1:			// 아래
							TempX = siCX - TempCount;		TempY = siCY + TempDistance;		break;
						case 2:			// 좌					
							TempX = siCX - TempDistance;	TempY = siCY - TempCount;			break;
						case 3:			// 우
							TempX = siCX + TempDistance;	TempY = siCY + TempCount;			break;
						}

						if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
						{
							pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];

							// 충돌 타일이 아니고 타일 종류는 땅이고 그 타일에 같은 종류의 아이템이 있으면 리턴한다.
							if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == siParam1) )
							{		
								// 한 타일에 놓일 수 있는 최대 아이템의 수를 초과하지 않는지 검사한다.
								if( SI32( pTileInfo->uiItemQuantity + siParam2 ) <= ON_MAX_ITEMNUM_ON_TILE )
								{
									TempPoint.x = TempX;
									TempPoint.y = TempY;
									
									return TempPoint;
								}
							}
						}			
					}			
				}
			}

			// 같은 종류의 아이템이 발견되지 않았으므로 최대 200개까지 빈 땅을 찾는다.

			// 먼저 서있는 자리가 아이템이 없는 빈 땅인지 체크한다.
			if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
			{
				pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

				// 일단 충돌 타일이 아니고 타일 종류는 땅이고 아이템이 없는 빈 땅이면 리턴한다.
				if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == 0) )
				{
					TempPoint.x = siCX;
					TempPoint.y = siCY;
					return TempPoint;
				}
			}

			// 주변 최대 200개까지 아이템이 없는 빈 땅을 찾아본다.
			for(TempDistance = 1; TempDistance < 200; TempDistance++)
			{
				for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
				{
					for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
					{
						switch(siDirectionCounter)
						{
						case 0:			// 위
							TempX = siCX + TempCount;		TempY = siCY - TempDistance;		break;
						case 1:			// 아래
							TempX = siCX - TempCount;		TempY = siCY + TempDistance;		break;
						case 2:			// 좌					
							TempX = siCX - TempDistance;	TempY = siCY - TempCount;			break;
						case 3:			// 우
							TempX = siCX + TempDistance;	TempY = siCY + TempCount;			break;
						}

						if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
						{
							pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];

							// 충돌 타일이 아니고 타일 종류는 땅이고 아이템이 없는 빈 땅이면 리턴한다.
							if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && pTileInfo->uiItemID == 0 )
							{
								TempPoint.x = TempX;
								TempPoint.y = TempY;
								return TempPoint;
							}
						}			
					}			
				}
			}
		}
	}

	TempPoint.x = -1;
	TempPoint.y = -1;
	return TempPoint;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	DB로부터 필드에 떨어져 있는 아이템의 정보를 얻어와서 맵에 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::SetItemFromDB()
{
	// 아이템을 설정하기 위한 함수 SetID를 호출 시 그 아이템을 DB에도 저장한다.
	// 하지만 DB에서 얻어온걸 다시 설정할 필요가 없기 때문에 UpdateDBSwitch를 FALSE 시킨다.
	bUpdateDBSwitch	=	FALSE;

	// 아이템을 필드에 설정한다.
	// 속도 및 효율성을 고려하여 GetDBInfo 내에서 SetID를 호출하여 아이템을 설정한다.
	pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_ALLFIELDITEM, LPVOID(this), NULL);

	bUpdateDBSwitch	=	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	맵의 사이즈를 구한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SIZE	SOMap::GetMapSize()
{
	SIZE	szMapSize;

	szMapSize.cx	=	m_siXsize;
	szMapSize.cy	=	m_siYsize;

	return szMapSize;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소속되어 있는 마을의 코드를 얻어온다.
//	수정 일자	:	2002-04-22 오전 11:07:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	SOMap::GetVillageCode()
{
	return uiVillageCode;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	충돌 타일인지 알아온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::IsColl(SI32 siX, SI32 siY)
{
	return	OnlineBaseMap::IsColl(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	충돌 타일인지 알아온다.(특정 캐릭터 입장에서)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb)
{
	// 다른 캐릭터가 서 있다면 충돌 타일이다.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)	return	TRUE;

	return	OnlineBaseMap::IsColl(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb, 
								  pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind,
								siCharMoveableTileAtb);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	던전인가?
//	수정 일자	:	2002-04-22 오전 11:08:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::IsDungeon()
{
	if(siMapType == MAPINDEXPARSERMAPTYPE_DUNGEON)		return	TRUE;
	if(uiVillageCode != 0)								return	TRUE;
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	비가 올 수 있는 지역인가?
//	수정 일자	:	2003-04-09 오후 13:25 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::CanBeRainy()
{
	if( (siWeather & MAPINDEXPARSERWEATHER_RAIN) )		return	TRUE;
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	눈이 올 수 있는 지역인가?
//	수정 일자	:	2003-04-09 오후 13:27 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::CanBeSnowy()
{
	if( (siWeather & MAPINDEXPARSERWEATHER_SNOW) )		return	TRUE;
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터를 초기화 한다.
//	수정 일자	:	2002-05-29 오후 2:04:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::InitSector(SI32 siArrayIndex)
{
	SI32	siGroundTileNum, siSeaTileNum;	


	siGroundTileNum	= siSeaTileNum = 400;

	// Sector를 초기화 한다.
	if(siGroundTileNum != 0)
	{
		m_pstSector[siArrayIndex].PlayerCharList.Create(siGroundTileNum);
		m_pstSector[siArrayIndex].MonsterCharList.Create(siGroundTileNum);			

		m_pstSector[siArrayIndex].ItemList.Create(siGroundTileNum);	
	}

	if(siSeaTileNum != 0)
	{
		m_pstSector[siArrayIndex].ShipCharList.Create(siSeaTileNum);	
	}	

	for( int k = 0; k < 9; k++ )
		m_pstSector[ siArrayIndex ].pNearSector[ k ] = NULL;

	m_pstSector[ siArrayIndex ].pSectorNode = NULL;
	m_pstSector[ siArrayIndex ].temp = 0;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터를 삭제한다.
//	수정 일자	:	2002-05-27 오후 1:02:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::FreeSector(SI32 siArrayIndex)
{
	m_pstSector[siArrayIndex].PlayerCharList.Release();
	m_pstSector[siArrayIndex].MonsterCharList.Release();	
	m_pstSector[siArrayIndex].ShipCharList.Release();	
	m_pstSector[siArrayIndex].ItemList.Release();
	
	ZeroMemory(m_pstSector[siArrayIndex].pNearSector, sizeof(m_pstSector[siArrayIndex].pNearSector));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터의 Index를 구한다.
//	수정 일자	:	2002-05-27 오후 1:15:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOMap::GetSectorIndex(SI32 siX, SI32 siY)
{
/*
	int x = int( ( siX - siY + m_siXsize - 1) / ON_TILENUM_PER_SECTOR  );
	int y = int( ( siX + siY ) / ON_TILENUM_PER_SECTOR );
	
	return ( y * siHorizonSectorNum ) + x;
*/
	 return	( ( siY / ON_TILENUM_PER_SECTOR ) * siHorizonSectorNum + ( siX / ON_TILENUM_PER_SECTOR ) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터에 플레이어를 추가 시킨다.
//	수정 일자	:	2002-05-30 오전 10:05:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::AddCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	SI32	siArrayIndex;	

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	// 섹터에 추가한다.
	return	AddCharToSector(siArrayIndex, pBaseChar);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터에 플레이어를 추가 시킨다.
//	수정 일자	:	2002-06-03 오전 11:53:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::AddCharToSector(SI32	siArrayIndex, BaseChar *pBaseChar)
{
	SI32	siRet = -1;

	strSector *pSector = &m_pstSector[siArrayIndex];

	// 현재의 플레이어가 다른 섹터에 있다가 이 섹터로 이동해 왔다면 설정해준다.	
	if(ISPLAYER(pBaseChar->uiAccount) == TRUE) {
		siRet	=	pSector->PlayerCharList.Add(pBaseChar->uiAccount);

		if( pSector->pSectorNode == NULL ) {
			pSector->pSectorNode = (cSectorNode *)m_sectorlist.PushFront();
			pSector->pSectorNode->m_pSector = pSector;
		}

	}
	else if(ISMONSTER(pBaseChar->uiAccount) == TRUE)	siRet	=	pSector->MonsterCharList.Add(pBaseChar->uiAccount);		
	else if(ISSHIP(pBaseChar->uiAccount) == TRUE) {
		siRet	=	pSector->ShipCharList.Add(pBaseChar->uiAccount);		

		if( pSector->pSectorNode == NULL ) {
			pSector->pSectorNode = (cSectorNode *)m_sectorlist.PushFront();
			pSector->pSectorNode->m_pSector = pSector;
		}
	}

	if( siRet >= 0 ) return	TRUE;
	else	return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터에서 플레이어를 삭제한다.
//	수정 일자	:	2002-05-30 오전 10:09:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::DelCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	SI32	siArrayIndex;

	siArrayIndex	=	GetSectorIndex(siX, siY);

	// 섹터에서 삭제한다.
	DelCharToSector(siArrayIndex, pBaseChar);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터에서 플레이어를 삭제한다.
//	수정 일자	:	2002-05-30 오전 10:09:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::DelCharToSector(SI32	siArrayIndex, BaseChar *pBaseChar)
{	
	SI32	siDeleteTypeRet = -2;
	strSector *pSector = &m_pstSector[siArrayIndex];
	
	// 섹터에서 삭제한다.
	if(ISPLAYER(pBaseChar->uiAccount) == TRUE)	{
		siDeleteTypeRet = pSector->PlayerCharList.DeleteType( pBaseChar->uiAccount );	

		if( pSector->PlayerCharList.m_NowUseDataCount == 0 && 
			pSector->ShipCharList.m_NowUseDataCount == 0 ) {
			m_sectorlist.Remove( pSector->pSectorNode );
			pSector->pSectorNode = NULL;
		}
	}
	else if(ISMONSTER(pBaseChar->uiAccount) == TRUE)	siDeleteTypeRet = pSector->MonsterCharList.DeleteType(pBaseChar->uiAccount);		
	else if(ISSHIP(pBaseChar->uiAccount) == TRUE)  {
		siDeleteTypeRet = pSector->ShipCharList.DeleteType(pBaseChar->uiAccount);		

		if( pSector->PlayerCharList.m_NowUseDataCount == 0 && 
			pSector->ShipCharList.m_NowUseDataCount == 0 ) {

			m_sectorlist.Remove( pSector->pSectorNode );
			pSector->pSectorNode = NULL;
		}

	}

	if(siDeleteTypeRet < 0)
	{
//		pMyWorld->clServer->WriteInfo("..\\DelCharFromSector.txt", "RETURN : [%d], Account : [%d], uiMapID [%d], siX [%d], siY [%d]", 
		writeInfoToFile("DelCharFromSector.txt", "RETURN : [%d], Account : [%d], uiMapID [%d], siX [%d], siY [%d]", 
			siDeleteTypeRet, pBaseChar->uiAccount, pBaseChar->uiMapID, pBaseChar->siX, pBaseChar->siY);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터에서 아이템이 있는 타일을 추가한다.
//	수정 일자	:	2002-06-03 오전 9:42:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::AddItemToSector(SI32 siX, SI32 siY, strTI *pstTile)
{
	SI32	siArrayIndex;
		
	if(IsInMapArea(siX, siY) == FALSE)	return;

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	m_pstSector[siArrayIndex].ItemList.Add(pstTile);

}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	섹터에서 아이템이 있는 타일을 삭제한다.
//	수정 일자	:	2002-06-03 오전 9:47:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::DelItemToSector(SI32 siX, SI32 siY, strTI *pstTile)
{
	SI32	siArrayIndex;
		
	if(IsInMapArea(siX, siY) == FALSE)	return;

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	m_pstSector[siArrayIndex].ItemList.DeleteType(pstTile);

}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 맵에 설정한다.
//	수정 일자	:	2002-06-03 오후 1:08:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::SetItem(SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	if(IsInMapArea(siX, siY) == FALSE)												return	FALSE;	// 맵 영역 밖에 있다.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)			return	FALSE;	// 충돌 지역이다.
	if(uiItemID == 0 || uiItemQuantity == 0)										return	FALSE;	// 유효하지 않은 값이다.

	// 셋팅하려는 곳에 이미 다른 아이템이 있는 경우 같은 종류이면 수량을 더해서 셋팅하고 다른 종류이면 FALSE를 리턴한다.
	if( pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0 )
	{
		if( pstMap[puiMultiplyMapYOff[siY]+siX].uiItemID == uiItemID )
		{
//			return TRUE;
			// ---- 윤영대 - 20050304 추가 : 아이템 갯수를 다시 체크한다.
			if( (pstMap[puiMultiplyMapYOff[siY]+siX].uiItemQuantity + uiItemQuantity) <= ON_MAX_ITEMNUM_ON_TILE )
			{
				SetItemID(siX, siY, uiItemID, pstMap[puiMultiplyMapYOff[siY]+siX].uiItemQuantity + uiItemQuantity, bSendToDB );
				return TRUE;
			}
			// ----
			return FALSE;
		}
		else
		{
			return FALSE;

		}
	}

	SetItemID(siX, siY, uiItemID, uiItemQuantity, bSendToDB );										// 아이템을 설정한다.

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 맵에서 제거한다.
//	수정 일자	:	2002-06-03 오후 1:08:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::DelItem( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	if(IsInMapArea(siX, siY) == FALSE)												return	FALSE;	// 맵 영역 밖에 있다.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)			return	FALSE;	// 충돌 지역이다.	
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID == 0)							return	FALSE;	// 현재 심겨져 있는 아이템이 없다.
	
	DelItemID( pPlayer, siX, siY, bSendToDB );																// 아이템을 설정한다.

	return	TRUE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Polling
//	수정 일자	:	2002-06-03 오후 5:02:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::Poll(BOOL bSendMoveMsg)
{
	SI32			i, j, ii;	
	UI16			uiMovedCharsCounter;
	SOPlayer		*pPlayer;	
	BaseChar		*pPrimaryChar  = NULL;
	strSector		*pSector, *pstSector;
	
	/*
	for(i = 0; i < siHorizonSectorNum * siVerticalSectorNum; i++)
	{	
		// 현 섹터에 8 방향 주위에 플레이어가 있다면 몬스터 처리를 해준다.		
		if(IsPlayerInNearSector(i) == FALSE)	continue;
		
		pstSector	=	&m_pstSector[i];	

		for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
		{			
			// 몬스터가 활동을 하도록 한다.
			pMyWorld->clMonster.Action(pstSector->MonsterCharList.m_Data[j]);						
		}				

		// 메시지를 보내주어야 하는 프레임이라면
		// 이 섹터에서 이동이 있었던 캐릭터들의 메시지를 보내준다.
		if(bSendMoveMsg == TRUE)
		{
			// 이동 메시지를 보내줄 순간이 되었다면 이동 메시지를 보내준다.	
			uiMovedCharsCounter	=	0;	
			
			// 이동한 몬스터들을 추출한다.
			for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clMonster.DidSendMoveInfo(pstSector->MonsterCharList.m_Data[j]) == TRUE)
				{	
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->MonsterCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}
			
			// 이동한 플레이어들을 추출한다.
			for(j = 0; j < pstSector->PlayerCharList.m_NowUseDataCount; j++)
			{
				if((pPlayer = pMyWorld->pPlayerOnAccount[pstSector->PlayerCharList.m_Data[j]]) == NULL) continue;
				
				if(pPlayer->DidSendMoveInfo() == TRUE)						
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->PlayerCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}							
			}
			
			// 이동한 배를 추출한다.
			for(j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clSOShip.DidSendMoveInfo(pstSector->ShipCharList.m_Data[j]) == TRUE)
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->ShipCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}									
			 
			// 이동 메시지를 보낼 주체를 고른 후 메시지를 보내준다.
			if(uiMovedCharsCounter >= 1)
			{
				pPrimaryChar			=	(BaseChar*)pMyWorld->GetBaseChar(uiMovedChars[0]);
				
				if(pPrimaryChar != NULL)						
				{
					pPrimaryChar->SendFieldMsg(ON_RESPONSE_POSITION, SO_SFM_MEANDNEARCHAR, LPVOID(uiMovedChars), LPVOID(uiMovedCharsCounter));																					
				}
			}
		}
	}
	*/

	int n = m_sectorlist.GetNumberNodes();
	int r = rand();

	cSectorNode *pSectorNode = (cSectorNode *)m_sectorlist.GetFirstNode();
	if(pSectorNode == NULL) return;

	for( i = 0; i < n; ++i ) {	

	pSectorNode = (cSectorNode *)pSectorNode->GetNext();
	if(pSectorNode == NULL) return;

	pSector	=	pSectorNode->m_pSector;
	if(pSector == NULL) return;

	for( ii = 0; ii < 9; ++ii ) {
		
		pstSector = pSector->pNearSector[ ii ];

		if( pstSector == NULL || pstSector->temp == r ) continue;

		pstSector->temp = r;
		

		for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
		{			
			// 몬스터가 활동을 하도록 한다.
			pMyWorld->clMonster.Action(pstSector->MonsterCharList.m_Data[j]);						
		}				

		// 메시지를 보내주어야 하는 프레임이라면
		// 이 섹터에서 이동이 있었던 캐릭터들의 메시지를 보내준다.
		if(bSendMoveMsg == TRUE)
		{
			// 이동 메시지를 보내줄 순간이 되었다면 이동 메시지를 보내준다.	
			uiMovedCharsCounter	=	0;	
			
			// 이동한 몬스터들을 추출한다.
			for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clMonster.DidSendMoveInfo(pstSector->MonsterCharList.m_Data[j]) == TRUE)
				{	
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->MonsterCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}
			
			// 이동한 플레이어들을 추출한다.
			for(j = 0; j < pstSector->PlayerCharList.m_NowUseDataCount; j++)
			{
				if((pPlayer = pMyWorld->pPlayerOnAccount[pstSector->PlayerCharList.m_Data[j]]) == NULL) continue;
				
				if(pPlayer->DidSendMoveInfo() == TRUE)						
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->PlayerCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}							
			}
			
			// 이동한 배를 추출한다.
			for(j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clSOShip.DidSendMoveInfo(pstSector->ShipCharList.m_Data[j]) == TRUE)
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->ShipCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}									
			
			// 이동 메시지를 보낼 주체를 고른 후 메시지를 보내준다.
			if(uiMovedCharsCounter >= 1)
			{
				pPrimaryChar			=	(BaseChar*)pMyWorld->GetBaseChar(uiMovedChars[0]);
				
				if(pPrimaryChar != NULL)						
				{
					pPrimaryChar->SendFieldMsg(ON_RESPONSE_POSITION, SO_SFM_MEANDNEARCHAR, LPVOID(uiMovedChars), LPVOID(uiMovedCharsCounter));																					
				}
			}
		}
	}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	주변 섹터에 플레이어가 있는 섹터가 있는지 알아온다.
//	수정 일자	:	2002-06-03 오후 6:16:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::IsPlayerInNearSector(SI32 siArrayIndex)
{
	SI32		i;
	strSector	*pstSector;	

	for(i = 0; i < 9; ++i )
	{
		pstSector	=	m_pstSector[siArrayIndex].pNearSector[i];

		if(pstSector != NULL) {
			if(pstSector->PlayerCharList.m_NowUseDataCount != 0) 
				return	TRUE;

			if(pstSector->ShipCharList.m_NowUseDataCount != 0) 
				return	TRUE;
		}
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	X, Y를 기준으로 주위 섹터의 플레이어 및 몬스터를 얻어온다.
//	수정 일자	:	2002-11-24 오전 11:38:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOMap::GetNearSectorPlayerList( SI32 siX, SI32 siY, UI16 *pCharUniIDs, SI32 siGetCharInfo )
{
	SI32		i, j;
	SI32		siCharNum;
	strSector	*pstSector;	
	SI32		siArrayIndex;

	siArrayIndex	=	GetSectorIndex( siX, siY );
	siCharNum		=	0;

	if( IsValidSectorIndex( siArrayIndex ) == FALSE )				return 0;	

	for( i = 0; i < 9; i++ )
	{
		if( ( pstSector	= m_pstSector[ siArrayIndex ].pNearSector[ i ] ) == NULL )	continue;

		if( siGetCharInfo & ON_MAP_CHARID_PLAYER )
		{
			if( pstSector->PlayerCharList.m_NowUseDataCount != 0 )
			{
				memcpy( &pCharUniIDs[ siCharNum ], pstSector->PlayerCharList.m_Data, sizeof( UI16 ) * pstSector->PlayerCharList.m_NowUseDataCount );
				
				/*
				{
					int		siDataCounter;

					for( siDataCounter = 0; siDataCounter < pstSector->PlayerCharList.m_NowUseDataCount; siDataCounter++)
					{
						if( pstSector->PlayerCharList.m_Data[ siDataCounter ] == 0 )
						{
							pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "111 - [ %u, %d, %d - %d ]", uiMapID, siX, siY, siGetCharInfo );							
						}
					}
				}
				*/


				siCharNum	+=	pstSector->PlayerCharList.m_NowUseDataCount;
			}				
		}

		if( siGetCharInfo & ON_MAP_CHARID_PLAYERINSHIP )
		{
			if( pstSector->ShipCharList.m_NowUseDataCount != 0 ) 
			{
				for( j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++ )
				{
					siCharNum	+=	pMyWorld->clSOShip.GetCustomers( pstSector->ShipCharList.m_Data[ j ],  &pCharUniIDs[ siCharNum ] );					
				}				
			}			
		}

		if( siGetCharInfo & ON_MAP_CHARID_MONSTER )
		{
			if( pstSector->MonsterCharList.m_NowUseDataCount != 0 )
			{
				memcpy( &pCharUniIDs[ siCharNum ], pstSector->MonsterCharList.m_Data, sizeof( UI16 ) * pstSector->MonsterCharList.m_NowUseDataCount );
				
				/*
				{
					int		siDataCounter;

					for( siDataCounter = 0; siDataCounter < pstSector->MonsterCharList.m_NowUseDataCount; siDataCounter++)
					{
						if( pstSector->MonsterCharList.m_Data[ siDataCounter ] == 0 )
						{
							pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "333 - [ %u, %d, %d - %d ]", uiMapID, siX, siY, siGetCharInfo );							
						}
					}
				}
				*/

				siCharNum	+=	pstSector->MonsterCharList.m_NowUseDataCount;
			}
		}			

		if( siGetCharInfo & ON_MAP_CHARID_SHIP )
		{
			if( pstSector->ShipCharList.m_NowUseDataCount != 0 )
			{
				memcpy( &pCharUniIDs[ siCharNum ], pstSector->ShipCharList.m_Data, sizeof( UI16 ) * pstSector->ShipCharList.m_NowUseDataCount );				

				/*
				{
					int		siDataCounter;

					for( siDataCounter = 0; siDataCounter < pstSector->ShipCharList.m_NowUseDataCount; siDataCounter++)
					{
						if( pstSector->ShipCharList.m_Data[ siDataCounter ] == 0 )
						{
							pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "444 - [ %u, %d, %d - %d ]", uiMapID, siX, siY, siGetCharInfo );							
						}
					}
				}
				*/

				siCharNum	+=	pstSector->ShipCharList.m_NowUseDataCount;
			}
		}					
	}			

	/*
	{
		{
			int		siDataCounter;

			for( siDataCounter = 0; siDataCounter < siCharNum; siDataCounter++)
			{
				if( pCharUniIDs[ siDataCounter ] == 0 )
				{
					pMyWorld->clServer->WriteInfo("..\\ZeroPlayerID.txt", "101 - [ %u, %d, %d - %d ]", uiMapID, siX, siY, siGetCharInfo );							
				}
			}
		}
	}
	*/
	
	
	return	siCharNum;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	X, Y를 기준으로 주위 섹터의 아이템 정보를 얻어온다.
//	수정 일자	:	2002-06-04 오전 10:52:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOMap::GetNearSectorItemList(SI32 siX, SI32 siY, OnlineReceiveFieldItemInfo *pFieldItemInfo)
{
	SI32		i, j;
	SI32		siItemNum;
	strSector	*pstSector;	
	SI32		siArrayIndex;
	strTI		*pstTile;

	siArrayIndex	=	GetSectorIndex(siX, siY);

	siItemNum	=	0;

	if( IsValidSectorIndex( siArrayIndex ) == FALSE ) return 0;

	for(i = 0; i < 9; i++)
	{
		
		pstSector	=	m_pstSector[siArrayIndex].pNearSector[i];		
		
		if(pstSector != NULL)
		{
			if(pstSector->ItemList.m_NowUseDataCount != 0)
			{	
				for(j = 0; j < pstSector->ItemList.m_NowUseDataCount; j++)
				{
					pstTile	=	pstSector->ItemList.m_Data[j];

					pFieldItemInfo[siItemNum].uiItemID		=	pstTile->uiItemID;
					pFieldItemInfo[siItemNum].uiQuantity	=	pstTile->uiItemQuantity;
					pFieldItemInfo[siItemNum].siX			=	pstTile->uiX;
					pFieldItemInfo[siItemNum].siY			=	pstTile->uiY;				
					siItemNum++;
				}
			}		
		}
	}			

	return	siItemNum;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 섹터 포인터 리스트를 넘겨 받아서 해당 섹터에 포한된 아이템들의 리스트를 얻는다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

SI32   SOMap::GetSectorItemList( strSector **pSector, OnlineReceiveFieldItemInfo *pFieldItemInfo )
{
	SI32	i, j;
	SI32	siItemNum;
	strTI	*pstTile;

	siItemNum = 0;

	if( pSector == NULL ) return 0;

	for( i = 0; i < 9; ++i ) {

		if( pSector[ i ] == NULL ) continue;
 
		for( j = 0; j < pSector[ i ]->ItemList.m_NowUseDataCount; ++j ) {

			pstTile = pSector[ i ]->ItemList.m_Data[ j ];

			pFieldItemInfo[siItemNum].uiItemID		=	pstTile->uiItemID;
			pFieldItemInfo[siItemNum].uiQuantity	=	pstTile->uiItemQuantity;
			pFieldItemInfo[siItemNum].siX			=	pstTile->uiX;
			pFieldItemInfo[siItemNum].siY			=	pstTile->uiY;								
			siItemNum++;

		}

	}

	return siItemNum;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 섹터 포인터 리스트를 넘겨 받아서 해당 섹터에 포한된 플레이어 및 몬스터들의 리스트를 얻는다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

SI32   SOMap::GetSectorPlayerList( strSector **pSector, UI16 *pCharUniIDs, SI32 siGetCharInfo )
{
	SI32	i, j;
	SI32	siCharNum;

	siCharNum = 0;

	if( pSector == NULL ) return 0;

	for( i = 0; i < 9; ++i ) {

		if( pSector[ i ] == NULL ) continue;

		if( siGetCharInfo & ON_MAP_CHARID_PLAYER ) {
				
			if( pSector[ i ]->PlayerCharList.m_NowUseDataCount != 0 ) {

				memcpy( &pCharUniIDs[ siCharNum ], pSector[ i ]->PlayerCharList.m_Data, 
					    sizeof(UI16) * pSector[ i ]->PlayerCharList.m_NowUseDataCount );
					
				siCharNum += pSector[ i ]->PlayerCharList.m_NowUseDataCount;
			}			
		}

		if( siGetCharInfo & ON_MAP_CHARID_PLAYERINSHIP ) {
			
			if( pSector[ i ]->ShipCharList.m_NowUseDataCount != 0 ) 
			{
				for(j = 0; j < pSector[ i ]->ShipCharList.m_NowUseDataCount; j++)
				{
					siCharNum	+=	pMyWorld->clSOShip.GetCustomers( pSector[ i ]->ShipCharList.m_Data[j],  &pCharUniIDs[ siCharNum ]);
				}
			}
		}

		if( siGetCharInfo & ON_MAP_CHARID_MONSTER ) {

			if( pSector[ i ]->MonsterCharList.m_NowUseDataCount != 0 ) {
					
				memcpy( &pCharUniIDs[ siCharNum ], pSector[ i ]->MonsterCharList.m_Data, 
					    sizeof(UI16) * pSector[ i ]->MonsterCharList.m_NowUseDataCount );
					
				siCharNum += pSector[ i ]->MonsterCharList.m_NowUseDataCount;
			}
		}

		if( siGetCharInfo & ON_MAP_CHARID_SHIP ) {

			if( pSector[ i ]->ShipCharList.m_NowUseDataCount != 0 ) {
					
				memcpy( &pCharUniIDs[ siCharNum ], pSector[ i ]->ShipCharList.m_Data, 
					    sizeof(UI16) * pSector[ i ]->ShipCharList.m_NowUseDataCount );
					
				siCharNum += pSector[ i ]->ShipCharList.m_NowUseDataCount;
			}
		}
	}

	return siCharNum;
}


void SOMap::PrintSectorList( strSector **pSector )
{
	int i;
	for( i = 0; i < 9; i++ ) {
		if( pSector[ i ] != NULL ) {
			printf( "[%d,%d]", pSector[ i ]->uiX, pSector[ i ]->uiY );
		}
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	무도장인지 알아온다.
//	수정 일자	:	2002-07-15 오후 4:26:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::IsWarFieldMap()
{
	return	bIsFieldMap;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	타일의 정보를 얻어온다.
//	수정 일자	:	2002-07-18 오후 3:29:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::GetTileInfo(SI32 siX, SI32 siY, CHAR *pszBuffer)
{
	if(IsInMapArea(siX, siY) == FALSE)		return	FALSE;	// 맵 영역 밖에 있다.		

	sprintf(pszBuffer, "Tile Info [%d, [%d] :: Account : [%u], Item ID : [%u], Item Quantity : [%u]", 
				siX, siY, 
				pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID,
				pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID,	
				pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	맵의 ID를 얻어온다.
//	수정 일자	:	2002-07-19 오후 2:45:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	SOMap::GetMapID()
{
	return	uiMapID;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화한다.
//	수정 일자	:	2002-07-26 오후 5:12:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOMap::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld				=	pWorld;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	siSector2가 siSector1의 주위 섹터인지 알아온다.
//	수정 일자	:	2003-05-24 오후 6:25:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMap::IsNearSector( SI32 siSector1, SI32 siSector2 )
{
	SI32		i;
	strSector	*pSector1	=	&m_pstSector[ siSector1 ];
	strSector	*pSector2	=	&m_pstSector[ siSector2 ];

	for( i = 0; i < 9; i++ )
	{
		if( pSector1->pNearSector[ i ] == pSector2 )			return	TRUE;		
	}	

	return	FALSE;
}
