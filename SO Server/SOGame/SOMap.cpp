#include "SOMain.h"
#include "SOWorld.h"
#include "SOMap.h"
#include "OnlineObjectParser.h"
#include "OnlineFieldParser.h"
#include "GSCDefine.h"

SOWorld	*	SOMap::pMyWorld;					// World

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	儅撩濠.
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
//	撲貲	:	模資濠.
//----------------------------------------------------------------------------------------------------------------
SOMap::~SOMap()
{
	delete [] m_pSectorNode;
	Free(); 
}  

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-07-26 螃�� 5:13:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	
	// 裘擊 煎萄и棻.
	if((fp = fopen(pMapIndexHeader->szMapFileName, "rb")) != NULL)
	{	
		if(fread(&Header, sizeof(LoadSaveMapHeader), 1, fp) != 1)
			goto END;		

		puiMultiplyMapYOff	=	new UI32[Header.siYsize];		
		if(puiMultiplyMapYOff == NULL)			goto END;		
		for(i = 0; i < Header.siYsize; i++)
			puiMultiplyMapYOff[i]	=	i * Header.siXsize;		

		// 裘擊 盪濰й 詭賅葬蒂 й渡 嫡朝棻.
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
		

		// 裘縑 樓撩擊 撲薑п遽棻.
		if(SetMap(pReadMap) == FALSE)
			goto END;		

		uiMapID			=	pMapIndexHeader->siMapIndex;
		siMapType		=	pMapIndexHeader->siMapType;
		uiVillageCode	=	pMapIndexHeader->uiVillageCode;
		bIsFieldMap		=	(pMapIndexHeader->fWarFlag == 2);
		siWeather		=	pMapIndexHeader->siWeather;

		// 陛煎 播攪 熱蒂 橢橫螞棻.
		if(m_siXsize % ON_TILENUM_PER_SECTOR == 0)			siHorizonSectorNum	=	m_siXsize / ON_TILENUM_PER_SECTOR;	
		else												siHorizonSectorNum	=	m_siXsize / ON_TILENUM_PER_SECTOR + 1;	

		// 撮煎 播攪 熱蒂 橢橫螞棻.
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

				// 陝 播攪蒂 蟾晦�� и棻.				
				InitSector( siSectorIndex );		
				
				m_pstSector[ siSectorIndex ].uiY = i;
				m_pstSector[ siSectorIndex ].uiX = j;

				// 輿嬪 播攪曖 ん檣攪蒂 雖薑и棻.
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
		// й渡 嫡懊湍 賅萇 匙擊 п薯и棻.
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
//	撲貲	:	п薯.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 3:30:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo)
{
	SI32		siNearCharNum;

	if(pBaseChar->uiAccount == 0)																				return	FALSE;	// 嶸�褲狫� 彊擎 ID檜棻.
	if(IsInMapArea(pBaseChar->siX, pBaseChar->siY) == FALSE)													return	FALSE;	// 裘 艙羲 夤縑 氈棻.
	if(pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiTileAtb & ON_TILE_ATB_COLL)				return	FALSE;	// 醱給 雖羲檜棻.		
	if((pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiTileKind & uiCharMoveableTileInfo) == 0)	return	FALSE;	// п渡 顫橾縑 議葛攪陛 撲熱 氈朝 顫橾檣雖 っ欽и棻.
	if(pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID != 0)							return	FALSE;	// 檜嘐 棻艇 Ы溯檜橫陛 褕啖螳 氈棻.		
	
	// 播攪縑 蹺陛 衛鑑棻.
	if( AddCharToSector(pBaseChar->siX, pBaseChar->siY, pBaseChar) == FALSE )									return	FALSE;	// 播攪縑 Ы溯檜橫蒂 蹺陛 衛鑑棻.	

	SetCharID(pBaseChar->siX, pBaseChar->siY, pBaseChar);	

	// 輿嬪縑 Ы溯檜橫菟擊 掘и棻.
	siNearCharNum =	GetNearSectorPlayerList(pBaseChar->siX, pBaseChar->siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
	pBaseChar->SendFieldMsg( ON_ADDCHAR, SO_SFM_CUSTOM, LPVOID( siNearCharNum ), LPVOID( uiCharUniIDs ) );	
	
	return	TRUE;	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 檜翕 ц擊 陽 億煎 蹺陛腎朝 播攪 諦 餉薯腎朝 播攪蒂 橢朝棻. 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪蒂 檜翕 衛鑑棻.
//	熱薑 橾濠	:	2002-06-03 螃瞪 11:38:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::MoveChar( BaseChar *pBaseChar, SI32 siX, SI32 siY, BOOL bWarUnit,UI32 uiCharMoveableTileInfo)
{
	SI32						siCurSectorIndex, siDestSectorIndex;
	SI32						siAddCharsNum, siDelCharsNum, siAddItemsNum;
	SI32						siNewInfoOnMove[3];			// 0廓擎 餉薯п撿й 議葛攪檜堅 1廓擎 餉薯п撿й 議葛攪檜棻.	


	SI32						i;
	SOPlayer					*pPlayer;	
	cltSOShip					*pSOShip;
	UI16						uiCustomerNum;
	static UI16					uiCustoms[SO_SHIP_MAX_CUSTOMER];	


	static strSector			*pAddSector[ 9 ];				// 億煎 蹺陛腎朝 播攪曖 ん檣攪蒂 盪濰й 夠
	static strSector			*pDelSector[ 9 ];				// 餉薯й 播攪曖 ん檣攪蒂 盪濰й 夠

	if(pBaseChar->uiAccount == 0)															return	FALSE;	// 嶸�褲狫� 彊擎 ID檜棻.
	if(IsInMapArea(siX, siY) == FALSE)														return	FALSE;	// 裘 艙羲 夤縑 氈棻.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)					return	FALSE;	// 醱給 雖羲檜棻.		
	if((pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind & uiCharMoveableTileInfo) == 0)	return	FALSE;	// п渡 顫橾縑 議葛攪陛 撲熱 氈朝 顫橾檣雖 っ欽и棻.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)								return	FALSE;	// 檜嘐 棻艇 Ы溯檜橫陛 褕啖螳 氈棻.	
				
	// 檜翕ж溥朝 播攪陛 ⑷營 氈朝 播攪諦 偽擎雖 匐餌и棻.
	siCurSectorIndex	=	GetSectorIndex(pBaseChar->siX, pBaseChar->siY);		// ⑷營 議葛攪陛 嬪纂ж堅 氈朝 播攪蒂 掘и棻.
	siDestSectorIndex	=	GetSectorIndex(siX, siY);							// 檜翕й 嬪纂曖 播攪蒂 掘и棻.

	// 檜翕й 播攪陛 ⑷營 播攪曖 輿嬪 播攪檣雖 匐餌и棻.
	if( IsNearSector( siCurSectorIndex, siDestSectorIndex ) == FALSE )
	{
//		pMyWorld->clServer->WriteInfo( "..\\MoveCharError.txt", "[ ID : %s ], [ %u ] [ %d, %d ] --> [ %d, %d ]",
		writeInfoToFile( "MoveCharError.txt", "[ ID : %s ], [ %u ] [ %d, %d ] --> [ %d, %d ]",
										pBaseChar->szID, pBaseChar->uiMapID, pBaseChar->siX, pBaseChar->siY,
										siX, siY );
										
		

		return	FALSE;
	}


	// 棻艇 播攪煎 檜翕й 唳辦曖 籀葬蒂 п遽棻..
	if(siCurSectorIndex != siDestSectorIndex)
	{
		// 蹺陛腎朝 播攪諦 餉薯腎朝 播攪蒂 掘и棻		
		GetNewSectorList( siCurSectorIndex, siDestSectorIndex, pAddSector, pDelSector );		

		// 億煎遴 播攪縑 議葛攪蒂 蹺陛 衛鑑棻.
		if( AddCharToSector(siDestSectorIndex, pBaseChar) == TRUE )
		{
			// 晦襄曖 播攪縑憮 議葛攪蒂 薯剪ж堅 --------------------------------------------------------*
			DelCharToSector(siCurSectorIndex, pBaseChar);		

			// 播攪陛 滲唳腎歷棻.
			siAddItemsNum = GetSectorItemList( pAddSector, FieldItemInfo );
			siDelCharsNum = GetSectorPlayerList( pDelSector, &uiAddDelCharsID[ 0 ], ON_MAP_CHARID_FIELD );							
			siAddCharsNum = GetSectorPlayerList( pAddSector, &uiAddDelCharsID[ siDelCharsNum ], ON_MAP_CHARID_FIELD );				

			siNewInfoOnMove[0]	=	siDelCharsNum;			// 餉薯п撿 ж朝 議葛攪曖 熱.		
			siNewInfoOnMove[1]	=	siAddCharsNum;			// 蹺陛п撿 ж朝 議葛攪曖 熱.		
			siNewInfoOnMove[2]	=	siAddItemsNum;			// 蹺陛п撿 ж朝 嬴檜蠱曖 熱.		

			if(ISPLAYER(pBaseChar->uiAccount) == TRUE)
			{
				pPlayer	=	(SOPlayer*)pBaseChar;
					
				// 釭縑啪 億煎遴 薑爾蒂 爾頂遽棻.
				pPlayer->SendFieldMsg(ON_NEWINFOONMOVE, SO_SFM_ONLYME, LPVOID(siNewInfoOnMove), uiAddDelCharsID, FieldItemInfo);
			}		
			else if(ISSHIP(pBaseChar->uiAccount) == TRUE)
			{
				pSOShip			=	(cltSOShip*)pBaseChar;

				// 寡縑驕 Ы溯檜橫菟縑啪 億煎遴 薑爾蒂 爾頂遽棻.
				uiCustomerNum	=	pSOShip->GetCustomers(uiCustoms);

				for(i = 0; i < uiCustomerNum; i++)
				{
					if((pPlayer = pMyWorld->pPlayerOnAccount[uiCustoms[i]]) != NULL)
						pPlayer->SendFieldMsg(ON_NEWINFOONMOVE, SO_SFM_ONLYME, LPVOID(siNewInfoOnMove), uiAddDelCharsID, FieldItemInfo);
				}		
			}
			

			// 頂 輿嬪曖 Ы溯檜橫菟縑啪 釭蒂 餉薯 塽 蹺陛ж塭堅 憲溥醜撿 и棻.
			siDelCharsNum = GetSectorPlayerList( pDelSector, &uiAddDelCharsID[ 0 ], ON_MAP_CHARID_ALLPLAYER );							
			siAddCharsNum = GetSectorPlayerList( pAddSector, &uiAddDelCharsID[ siDelCharsNum ], ON_MAP_CHARID_ALLPLAYER );						

			// 頂 議葛攪 餉薯 薑爾蒂 爾頂遽棻. 慇橫釭朝 播攪艙羲縑渠п憮..			
			pBaseChar->SendFieldMsg(ON_DELCHAR, SO_SFM_CUSTOM, LPVOID( siDelCharsNum ), LPVOID( uiAddDelCharsID ) );

			// 頂 議葛攪 蹺陛 薑爾蒂 爾頂遽棻. 蹺陛腎朝 播攪艙羲縑渠п憮..			
			pBaseChar->SendFieldMsg(ON_ADDCHAR, SO_SFM_CUSTOM, LPVOID( siAddCharsNum ), LPVOID( &uiAddDelCharsID[ siDelCharsNum ]) );		
		}	
		else
		{
			// 播攪蒂 檜翕 衛酈朝等 褒ぬц棻.
			return	FALSE;

		}
	}

	DelCharID( pBaseChar->siX, pBaseChar->siY );									// 裘縑憮 議葛攪曖 ID蒂 餉薯п遽棻.
	SetCharID( siX, siY, pBaseChar );												// 裘縑 議葛攪曖ID蒂 撲薑п遽棻.
		
	// 議葛攪曖 億煎遴 嬪纂蒂 雖薑п遽棻.
	pBaseChar->siX	=	siX;
	pBaseChar->siY	=	siY;	

	if(bWarUnit == TRUE)
		pBaseChar->SendFieldMsg( ON_RESPONSE_POSITION, SO_SFM_MEANDNEARCHAR, LPVOID( &pBaseChar->uiAccount ), LPVOID( 1 ) );
	else // 議葛攪陛 檜翕ц棻堅 撲薑и棻.
		pBaseChar->SetMoved();

	return	TRUE;
} 

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 3:32:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::DelChar(BaseChar *pBaseChar)
{

	// 裘縑憮 雖辦溥堅 ж朝 議葛攪曖 ID陛 頂 議葛攪曖 ID 罹撿 и棻.
	if( pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID == pBaseChar->uiAccount )		
	{
		DelCharID(pBaseChar->siX, pBaseChar->siY);
	
		// 播攪縑憮 餉薯и棻.
		DelCharToSector(pBaseChar->siX, pBaseChar->siY, pBaseChar);		
		
		pBaseChar->SendFieldMsg( ON_DELCHAR, SO_SFM_ONLYNEARCHAR ); 
		
		return	TRUE;
	}
	else																													
	{
		// 裘縑憮 雖辦溥堅 ж朝 議葛攪曖 ID陛 頂 議葛攪曖 ID陛 嬴棲棻.
//		pMyWorld->clServer->WriteInfo("..\\log_for_debug.txt", "2 [Map ID : %d]  [uiAccount willing to be deleted : %d]  [ID from the x, y on the map ID : %d]",
		writeInfoToFile("log_for_debug.txt", "2 [Map ID : %d]  [uiAccount willing to be deleted : %d]  [ID from the x, y on the map ID : %d]",
										pBaseChar->uiMapID, pBaseChar->uiAccount, pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID );
		return	FALSE;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	欽牖�� 議葛攪曖 ID蒂 裘縑 撲薑虜 п遽棻.
//	熱薑 橾濠	:	2002-06-03 螃瞪 11:47:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::SetCharID(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	pBaseChar->uiAccount;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	欽牖�� 議葛攪曖 ID蒂 裘縑憮 餉薯п 遽棻.
//	熱薑 橾濠	:	2002-06-03 螃瞪 11:57:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::DelCharID(SI32 siX, SI32 siY)
{
	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	0;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	欽牖�� 裘縑 嬴檜蠱擊 撲薑虜 п遽棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 12:17:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// DB縑 薑爾蒂 機等檜おи棻.
	if( bUpdateDBSwitch == TRUE )
	{
		if( bSendToDB == TRUE )
		{
			// DB縑 嬴檜蠱擊 盪濰и棻.
			FieldItemInfo.uiMapID		=	uiMapID;
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;
			FieldItemInfo.uiID			=	uiItemID;
			FieldItemInfo.uiQuantity	=	uiItemQuantity;
		
			pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_APPENDFIELDITEM, &FieldItemInfo, sizeof( FieldItemInfo ) );
		}

	}

	// 晦襄曖 檜 顫橾檜 播攪 薑爾縑 檜嘐 蹺陛腎橫 氈戲賊 
	// 渦 檜鼻 蹺陛ж雖 彊朝棻.
	if( bExistItem == FALSE )
	{
		// п渡 播攪縑紫 んл 衛鑑棻.			
		AddItemToSector( siX, siY, &pstMap[ puiMultiplyMapYOff[ siY ] + siX ] );
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	欽牖�� 裘縑 嬴檜蠱擊 薯剪虜 п遽棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 12:17:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::DelItemID( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	DBIKFieldItemInfo	FieldItemInfo;

	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	0;
	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	0;	

	// DB縑 薑爾蒂 機等檜おи棻.
	if(bUpdateDBSwitch == TRUE)
	{
		if( bSendToDB == TRUE )
		{
			// DB縑憮紫 嬴檜蠱擊 餉薯и棻.
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	裘縑 ID蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-05-21 螃�� 3:17:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::SetID(SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI32 uiCharMoveableTileInfo)
{	
	if(IsInMapArea(siX, siY) == FALSE)
	{
		return FALSE;	// 裘 艙羲 夤縑 氈棻.
	}

	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)
	{
		return FALSE;	// 醱給 雖羲檜棻.
	}

	if(pBaseChar->uiAccount != 0)
	{
		// 裘縑 議葛攪蒂 撲薑и棻.

		// 辦摹 п渡 顫橾縑 議葛攪陛 撲熱 氈朝 顫橾檣雖 っ欽и棻.
		if((pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind & uiCharMoveableTileInfo) == 0)
		{
			return FALSE;
		}

		if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)
		{
			// 檜嘐 棻艇 Ы溯檜橫陛 褕啖螳 氈棻.
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
		// 裘縑 嬴檜蠱擊 撲薑и棻.

		if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0)
		{
			// 檜嘐 棻艇 嬴檜蠱檜 褕啖螳氈棻.
			return FALSE;
		}
		else
		{
			pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	uiItemID;
			pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	uiItemQuantity;

			// DB縑 薑爾蒂 機等檜おи棻.
			if(bUpdateDBSwitch == TRUE)
			{
				DBIKFieldItemInfo	FieldItemInfo;

				// DB縑 嬴檜蠱擊 盪濰и棻.
				FieldItemInfo.siX			=	siX;
				FieldItemInfo.siY			=	siY;
				FieldItemInfo.uiID			=	uiItemID;
				FieldItemInfo.uiQuantity	=	uiItemQuantity;
				pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDFIELDITEM, LPVOID(&FieldItemInfo), NULL);
			}

			// п渡 播攪縑紫 んл 衛鑑棻.			
			AddItemToSector(siX, siY, &pstMap[puiMultiplyMapYOff[siY] + siX]);
			return TRUE;
		}	
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	裘縑憮 ID蒂 餉薯и棻.
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

		// DB縑 薑爾蒂 機等檜おи棻.
		if(bUpdateDBSwitch == TRUE)
		{
			DBIKFieldItemInfo	FieldItemInfo;

			// DB縑憮紫 嬴檜蠱擊 餉薯и棻.
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;			
			pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_DELETEFIELDITEM, LPVOID(&FieldItemInfo), NULL);
		}

		// 嬴檜蠱檜 氈朝 顫橾擊 餉薯 衛麵遽棻.
		DelItemToSector(siX, siY, &pstMap[puiMultiplyMapYOff[siY] + siX]);		
	}	
}
*/
//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	裘縑 樓撩擊 撲薑п遽棻.
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

	// 螃粽薛お檣 唳辦 議葛攪諦 醱給ж朝 螃粽薛お陛 氈堅 斜傖 鱔婁ж朝 螃粽薛お陛 氈棻.
	// 斜 罹睡蒂 嘐葬 啗骯и棻.
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

			// 顫橾 謙盟紫 橢橫螞棻.
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
			// 醱給 螃粽薛お陛 褕啖螳 氈朝雖 憲嬴螞棻.
			switch(pObjectInfoBfs->BaseBf.uiCode)
			{			
			case ON_MAP_OI_CODE_RELATIVEPOS:
				// 錳 嬪纂曖 螃粽薛お陛 醱給 樓撩擊 陛雖堅 氈戲賊 檜匙紫 醱給 樓撩擊
				// 陛雖堅 氈棻.
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
//	撲貲	:	橾薑 艙羲縑 氈朝 議葛攪菟曖 ID蒂 橢橫螞棻.
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
//	撲貲	:	裘縑憮 議葛攪曖 ID蒂 橢橫螞棻.
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
//	撲貲	:	橾薑 艙羲縑 氈朝 嬴檜蠱曖 ID蒂 橢橫螞棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и 顫橾縑 撲薑腎橫 氈朝 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
//	葬欐	:	斜 顫橾縑 嬴檜蠱檜 氈戲賊 TRUE. 嬴檜蠱檜 橈戲賊 FALSE.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::GetItemID(SI32 siX, SI32 siY, UI16 *pItemIDs, UI16 *pItemQuantity)
{
	// 嶸�褲� 雖羲檣雖 羹觼и棻.
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
//	撲貲	:	謝ル蒂 掖薑п遽棻.
//----------------------------------------------------------------------------------------------------------------
VOID	SOMap::CorrectionPos(SI32 *psiSX, SI32 *psiSY, SI32 *psiEX, SI32 *psiEY)
{
	*psiSX	=	max(0, *psiSX);
	*psiSY	=	max(0, *psiSY);
	*psiEX	=	min(m_siXsize - 1, *psiEX);
	*psiEY	=	min(m_siYsize - 1, *psiEY);
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	裘縑 撲薑腎橫 氈朝 議葛攪曖 id釭 item曖 id蒂 橢橫螞棻.
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
						// 褒薯 ん檣攪蒂 陛雖堅 氈朝雖 �挫恉挬�.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	裘縑憮 か薑 謝ル蒂 瓊朝棻.
//	熱薑 橾濠	:	2002-08-05 螃瞪 11:04:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
POINT	SOMap::SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 )
{

	POINT	TempPoint;
	SI32	TempDistance;
	SI32	TempCount;
	SI32	TempX;
	SI32	TempY;
	SI32	siDirectionCounter;
	strTI	*pTileInfo;
	SI32	siSameItemSearchArea	=	3;			// 偽擎 嬴檜蠱檜 輿滲縑 氈朝雖 匐儀й 陽 斜 艙羲曖 觼晦(奩雖葷)

	// 議葛攪蒂 褕擊 嬪纂蒂 瓊擊陽.
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
					case 0:			// 嬪
						TempX = siCX + TempCount;
						TempY = siCY - TempDistance;
						break;
					case 1:			// 嬴楚
						TempX = siCX - TempCount;
						TempY = siCY + TempDistance;
						break;
					case 2:			// 謝					
						TempX = siCX - TempDistance;
						TempY = siCY - TempCount;
						break;
					case 3:			// 辦
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

		// в萄縑 幗葬朝 嬴檜蠱擎 ON_MAX_ITEMNUM_ON_TILE擊 蟾婁й 熱 橈棻.
		if( siParam2 <= ON_MAX_ITEMNUM_ON_TILE )
		{		

			// 憮氈朝 濠葬縑 偽擎 謙盟曖 嬴檜蠱檜 氈朝雖 匐餌п憮 氈戲賊 斜 嬪纂蒂 葬欐и棻.
			if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
			{
				pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

				// 橾欽 醱給 顫橾檜 嬴棲堅 顫橾 謙盟朝 階檜橫撿 и棻.
				if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == siParam1) )
				{
					// ---- 20050216 - 彌艙渠 熱薑
					// и 顫橾縑 場橾 熱 氈朝 譆渠 嬴檜蠱曖 熱蒂 蟾婁ж雖 彊朝雖 匐餌и棻.
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

			// 輿滲擊 離滔煎 給賊憮 偽擎 謙盟曖 嬴檜蠱檜 氈朝雖 匐餌п憮 氈戲賊 斜 嬪纂蒂 葬欐и棻. (siSameItemSearchArea虜躑)
			for(TempDistance = 1; TempDistance <= siSameItemSearchArea; TempDistance++)
			{
				for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
				{
					for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
					{
						switch(siDirectionCounter)
						{
						case 0:			// 嬪
							TempX = siCX + TempCount;		TempY = siCY - TempDistance;		break;
						case 1:			// 嬴楚
							TempX = siCX - TempCount;		TempY = siCY + TempDistance;		break;
						case 2:			// 謝					
							TempX = siCX - TempDistance;	TempY = siCY - TempCount;			break;
						case 3:			// 辦
							TempX = siCX + TempDistance;	TempY = siCY + TempCount;			break;
						}

						if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
						{
							pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];

							// 醱給 顫橾檜 嬴棲堅 顫橾 謙盟朝 階檜堅 斜 顫橾縑 偽擎 謙盟曖 嬴檜蠱檜 氈戲賊 葬欐и棻.
							if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == siParam1) )
							{		
								// и 顫橾縑 場橾 熱 氈朝 譆渠 嬴檜蠱曖 熱蒂 蟾婁ж雖 彊朝雖 匐餌и棻.
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

			// 偽擎 謙盟曖 嬴檜蠱檜 嫦唯腎雖 彊懊戲嘎煎 譆渠 200偃梱雖 綴 階擊 瓊朝棻.

			// 試盪 憮氈朝 濠葬陛 嬴檜蠱檜 橈朝 綴 階檣雖 羹觼и棻.
			if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
			{
				pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

				// 橾欽 醱給 顫橾檜 嬴棲堅 顫橾 謙盟朝 階檜堅 嬴檜蠱檜 橈朝 綴 階檜賊 葬欐и棻.
				if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == 0) )
				{
					TempPoint.x = siCX;
					TempPoint.y = siCY;
					return TempPoint;
				}
			}

			// 輿滲 譆渠 200偃梱雖 嬴檜蠱檜 橈朝 綴 階擊 瓊嬴獄棻.
			for(TempDistance = 1; TempDistance < 200; TempDistance++)
			{
				for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
				{
					for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
					{
						switch(siDirectionCounter)
						{
						case 0:			// 嬪
							TempX = siCX + TempCount;		TempY = siCY - TempDistance;		break;
						case 1:			// 嬴楚
							TempX = siCX - TempCount;		TempY = siCY + TempDistance;		break;
						case 2:			// 謝					
							TempX = siCX - TempDistance;	TempY = siCY - TempCount;			break;
						case 3:			// 辦
							TempX = siCX + TempDistance;	TempY = siCY + TempCount;			break;
						}

						if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
						{
							pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];

							// 醱給 顫橾檜 嬴棲堅 顫橾 謙盟朝 階檜堅 嬴檜蠱檜 橈朝 綴 階檜賊 葬欐и棻.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB煎睡攪 в萄縑 雲橫螳 氈朝 嬴檜蠱曖 薑爾蒂 橢橫諦憮 裘縑 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::SetItemFromDB()
{
	// 嬴檜蠱擊 撲薑ж晦 嬪и л熱 SetID蒂 ��轎 衛 斜 嬴檜蠱擊 DB縑紫 盪濰и棻.
	// ж雖虜 DB縑憮 橢橫螞勘 棻衛 撲薑й в蹂陛 橈晦 陽僥縑 UpdateDBSwitch蒂 FALSE 衛鑑棻.
	bUpdateDBSwitch	=	FALSE;

	// 嬴檜蠱擊 в萄縑 撲薑и棻.
	// 樓紫 塽 �螃票瑰� 堅溥ж罹 GetDBInfo 頂縑憮 SetID蒂 ��轎ж罹 嬴檜蠱擊 撲薑и棻.
	pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_ALLFIELDITEM, LPVOID(this), NULL);

	bUpdateDBSwitch	=	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘曖 餌檜鍔蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SIZE	SOMap::GetMapSize()
{
	SIZE	szMapSize;

	szMapSize.cx	=	m_siXsize;
	szMapSize.cy	=	m_siYsize;

	return szMapSize;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模樓腎橫 氈朝 葆擊曖 囀萄蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 11:07:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	SOMap::GetVillageCode()
{
	return uiVillageCode;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	醱給 顫橾檣雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::IsColl(SI32 siX, SI32 siY)
{
	return	OnlineBaseMap::IsColl(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	醱給 顫橾檣雖 憲嬴螞棻.(か薑 議葛攪 殮濰縑憮)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb)
{
	// 棻艇 議葛攪陛 憮 氈棻賊 醱給 顫橾檜棻.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)	return	TRUE;

	return	OnlineBaseMap::IsColl(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb, 
								  pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind,
								siCharMoveableTileAtb);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	湍瞪檣陛?
//	熱薑 橾濠	:	2002-04-22 螃瞪 11:08:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::IsDungeon()
{
	if(siMapType == MAPINDEXPARSERMAPTYPE_DUNGEON)		return	TRUE;
	if(uiVillageCode != 0)								return	TRUE;
	
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	綠陛 螢 熱 氈朝 雖羲檣陛?
//	熱薑 橾濠	:	2003-04-09 螃�� 13:25 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::CanBeRainy()
{
	if( (siWeather & MAPINDEXPARSERWEATHER_RAIN) )		return	TRUE;
	
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	換檜 螢 熱 氈朝 雖羲檣陛?
//	熱薑 橾濠	:	2003-04-09 螃�� 13:27 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::CanBeSnowy()
{
	if( (siWeather & MAPINDEXPARSERWEATHER_SNOW) )		return	TRUE;
	
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-05-29 螃�� 2:04:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::InitSector(SI32 siArrayIndex)
{
	SI32	siGroundTileNum, siSeaTileNum;	


	siGroundTileNum	= siSeaTileNum = 400;

	// Sector蒂 蟾晦�� и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-05-27 螃�� 1:02:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::FreeSector(SI32 siArrayIndex)
{
	m_pstSector[siArrayIndex].PlayerCharList.Release();
	m_pstSector[siArrayIndex].MonsterCharList.Release();	
	m_pstSector[siArrayIndex].ShipCharList.Release();	
	m_pstSector[siArrayIndex].ItemList.Release();
	
	ZeroMemory(m_pstSector[siArrayIndex].pNearSector, sizeof(m_pstSector[siArrayIndex].pNearSector));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪曖 Index蒂 掘и棻.
//	熱薑 橾濠	:	2002-05-27 螃�� 1:15:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOMap::GetSectorIndex(SI32 siX, SI32 siY)
{
/*
	int x = int( ( siX - siY + m_siXsize - 1) / ON_TILENUM_PER_SECTOR  );
	int y = int( ( siX + siY ) / ON_TILENUM_PER_SECTOR );
	
	return ( y * siHorizonSectorNum ) + x;
*/
	 return	( ( siY / ON_TILENUM_PER_SECTOR ) * siHorizonSectorNum + ( siX / ON_TILENUM_PER_SECTOR ) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪縑 Ы溯檜橫蒂 蹺陛 衛鑑棻.
//	熱薑 橾濠	:	2002-05-30 螃瞪 10:05:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::AddCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	SI32	siArrayIndex;	

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	// 播攪縑 蹺陛и棻.
	return	AddCharToSector(siArrayIndex, pBaseChar);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪縑 Ы溯檜橫蒂 蹺陛 衛鑑棻.
//	熱薑 橾濠	:	2002-06-03 螃瞪 11:53:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::AddCharToSector(SI32	siArrayIndex, BaseChar *pBaseChar)
{
	SI32	siRet = -1;

	strSector *pSector = &m_pstSector[siArrayIndex];

	// ⑷營曖 Ы溯檜橫陛 棻艇 播攪縑 氈棻陛 檜 播攪煎 檜翕п 諮棻賊 撲薑п遽棻.	
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪縑憮 Ы溯檜橫蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-05-30 螃瞪 10:09:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::DelCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	SI32	siArrayIndex;

	siArrayIndex	=	GetSectorIndex(siX, siY);

	// 播攪縑憮 餉薯и棻.
	DelCharToSector(siArrayIndex, pBaseChar);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪縑憮 Ы溯檜橫蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-05-30 螃瞪 10:09:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::DelCharToSector(SI32	siArrayIndex, BaseChar *pBaseChar)
{	
	SI32	siDeleteTypeRet = -2;
	strSector *pSector = &m_pstSector[siArrayIndex];
	
	// 播攪縑憮 餉薯и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪縑憮 嬴檜蠱檜 氈朝 顫橾擊 蹺陛и棻.
//	熱薑 橾濠	:	2002-06-03 螃瞪 9:42:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::AddItemToSector(SI32 siX, SI32 siY, strTI *pstTile)
{
	SI32	siArrayIndex;
		
	if(IsInMapArea(siX, siY) == FALSE)	return;

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	m_pstSector[siArrayIndex].ItemList.Add(pstTile);

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	播攪縑憮 嬴檜蠱檜 氈朝 顫橾擊 餉薯и棻.
//	熱薑 橾濠	:	2002-06-03 螃瞪 9:47:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::DelItemToSector(SI32 siX, SI32 siY, strTI *pstTile)
{
	SI32	siArrayIndex;
		
	if(IsInMapArea(siX, siY) == FALSE)	return;

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	m_pstSector[siArrayIndex].ItemList.DeleteType(pstTile);

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 裘縑 撲薑и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 1:08:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::SetItem(SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	if(IsInMapArea(siX, siY) == FALSE)												return	FALSE;	// 裘 艙羲 夤縑 氈棻.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)			return	FALSE;	// 醱給 雖羲檜棻.
	if(uiItemID == 0 || uiItemQuantity == 0)										return	FALSE;	// 嶸�褲狫� 彊擎 高檜棻.

	// 撢たж溥朝 夠縑 檜嘐 棻艇 嬴檜蠱檜 氈朝 唳辦 偽擎 謙盟檜賊 熱榆擊 渦п憮 撢たж堅 棻艇 謙盟檜賊 FALSE蒂 葬欐и棻.
	if( pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0 )
	{
		if( pstMap[puiMultiplyMapYOff[siY]+siX].uiItemID == uiItemID )
		{
//			return TRUE;
			// ---- 彌艙渠 - 20050304 蹺陛 : 嬴檜蠱 偎熱蒂 棻衛 羹觼и棻.
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

	SetItemID(siX, siY, uiItemID, uiItemQuantity, bSendToDB );										// 嬴檜蠱擊 撲薑и棻.

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 裘縑憮 薯剪и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 1:08:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::DelItem( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	if(IsInMapArea(siX, siY) == FALSE)												return	FALSE;	// 裘 艙羲 夤縑 氈棻.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)			return	FALSE;	// 醱給 雖羲檜棻.	
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID == 0)							return	FALSE;	// ⑷營 褕啖螳 氈朝 嬴檜蠱檜 橈棻.
	
	DelItemID( pPlayer, siX, siY, bSendToDB );																// 嬴檜蠱擊 撲薑и棻.

	return	TRUE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Polling
//	熱薑 橾濠	:	2002-06-03 螃�� 5:02:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// ⑷ 播攪縑 8 寞щ 輿嬪縑 Ы溯檜橫陛 氈棻賊 跨蝶攪 籀葬蒂 п遽棻.		
		if(IsPlayerInNearSector(i) == FALSE)	continue;
		
		pstSector	=	&m_pstSector[i];	

		for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
		{			
			// 跨蝶攪陛 �做螃� ж紫煙 и棻.
			pMyWorld->clMonster.Action(pstSector->MonsterCharList.m_Data[j]);						
		}				

		// 詭衛雖蒂 爾頂輿橫撿 ж朝 Щ溯歜檜塭賊
		// 檜 播攪縑憮 檜翕檜 氈歷湍 議葛攪菟曖 詭衛雖蒂 爾頂遽棻.
		if(bSendMoveMsg == TRUE)
		{
			// 檜翕 詭衛雖蒂 爾頂還 牖除檜 腎歷棻賊 檜翕 詭衛雖蒂 爾頂遽棻.	
			uiMovedCharsCounter	=	0;	
			
			// 檜翕и 跨蝶攪菟擊 蹺轎и棻.
			for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clMonster.DidSendMoveInfo(pstSector->MonsterCharList.m_Data[j]) == TRUE)
				{	
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->MonsterCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}
			
			// 檜翕и Ы溯檜橫菟擊 蹺轎и棻.
			for(j = 0; j < pstSector->PlayerCharList.m_NowUseDataCount; j++)
			{
				if((pPlayer = pMyWorld->pPlayerOnAccount[pstSector->PlayerCharList.m_Data[j]]) == NULL) continue;
				
				if(pPlayer->DidSendMoveInfo() == TRUE)						
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->PlayerCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}							
			}
			
			// 檜翕и 寡蒂 蹺轎и棻.
			for(j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clSOShip.DidSendMoveInfo(pstSector->ShipCharList.m_Data[j]) == TRUE)
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->ShipCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}									
			 
			// 檜翕 詭衛雖蒂 爾鳥 輿羹蒂 堅艇 �� 詭衛雖蒂 爾頂遽棻.
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
			// 跨蝶攪陛 �做螃� ж紫煙 и棻.
			pMyWorld->clMonster.Action(pstSector->MonsterCharList.m_Data[j]);						
		}				

		// 詭衛雖蒂 爾頂輿橫撿 ж朝 Щ溯歜檜塭賊
		// 檜 播攪縑憮 檜翕檜 氈歷湍 議葛攪菟曖 詭衛雖蒂 爾頂遽棻.
		if(bSendMoveMsg == TRUE)
		{
			// 檜翕 詭衛雖蒂 爾頂還 牖除檜 腎歷棻賊 檜翕 詭衛雖蒂 爾頂遽棻.	
			uiMovedCharsCounter	=	0;	
			
			// 檜翕и 跨蝶攪菟擊 蹺轎и棻.
			for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clMonster.DidSendMoveInfo(pstSector->MonsterCharList.m_Data[j]) == TRUE)
				{	
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->MonsterCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}
			
			// 檜翕и Ы溯檜橫菟擊 蹺轎и棻.
			for(j = 0; j < pstSector->PlayerCharList.m_NowUseDataCount; j++)
			{
				if((pPlayer = pMyWorld->pPlayerOnAccount[pstSector->PlayerCharList.m_Data[j]]) == NULL) continue;
				
				if(pPlayer->DidSendMoveInfo() == TRUE)						
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->PlayerCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}							
			}
			
			// 檜翕и 寡蒂 蹺轎и棻.
			for(j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clSOShip.DidSendMoveInfo(pstSector->ShipCharList.m_Data[j]) == TRUE)
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->ShipCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}									
			
			// 檜翕 詭衛雖蒂 爾鳥 輿羹蒂 堅艇 �� 詭衛雖蒂 爾頂遽棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	輿滲 播攪縑 Ы溯檜橫陛 氈朝 播攪陛 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 6:16:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	X, Y蒂 晦遽戲煎 輿嬪 播攪曖 Ы溯檜橫 塽 跨蝶攪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-11-24 螃瞪 11:38:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	X, Y蒂 晦遽戲煎 輿嬪 播攪曖 嬴檜蠱 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-04 螃瞪 10:52:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 播攪 ん檣攪 葬蝶お蒂 剩啖 嫡嬴憮 п渡 播攪縑 んи脹 嬴檜蠱菟曖 葬蝶お蒂 橢朝棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 播攪 ん檣攪 葬蝶お蒂 剩啖 嫡嬴憮 п渡 播攪縑 んи脹 Ы溯檜橫 塽 跨蝶攪菟曖 葬蝶お蒂 橢朝棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼠紫濰檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 4:26:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::IsWarFieldMap()
{
	return	bIsFieldMap;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	顫橾曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-18 螃�� 3:29:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOMap::GetTileInfo(SI32 siX, SI32 siY, CHAR *pszBuffer)
{
	if(IsInMapArea(siX, siY) == FALSE)		return	FALSE;	// 裘 艙羲 夤縑 氈棻.		

	sprintf(pszBuffer, "Tile Info [%d, [%d] :: Account : [%u], Item ID : [%u], Item Quantity : [%u]", 
				siX, siY, 
				pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID,
				pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID,	
				pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity );

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	裘曖 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-19 螃�� 2:45:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	SOMap::GetMapID()
{
	return	uiMapID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�倆挬�.
//	熱薑 橾濠	:	2002-07-26 螃�� 5:12:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOMap::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld				=	pWorld;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	siSector2陛 siSector1曖 輿嬪 播攪檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2003-05-24 螃�� 6:25:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
