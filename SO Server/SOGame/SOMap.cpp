#include "SOMain.h"
#include "SOWorld.h"
#include "SOMap.h"
#include "OnlineObjectParser.h"
#include "OnlineFieldParser.h"
#include "GSCDefine.h"

SOWorld	*	SOMap::pMyWorld;					// World

//----------------------------------------------------------------------------------------------------------------
//	����	:	������.
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
//	����	:	�Ҹ���.
//----------------------------------------------------------------------------------------------------------------
SOMap::~SOMap()
{
	delete [] m_pSectorNode;
	Free(); 
}  

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-07-26 ���� 5:13:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	
	// ���� �ε��Ѵ�.
	if((fp = fopen(pMapIndexHeader->szMapFileName, "rb")) != NULL)
	{	
		if(fread(&Header, sizeof(LoadSaveMapHeader), 1, fp) != 1)
			goto END;		

		puiMultiplyMapYOff	=	new UI32[Header.siYsize];		
		if(puiMultiplyMapYOff == NULL)			goto END;		
		for(i = 0; i < Header.siYsize; i++)
			puiMultiplyMapYOff[i]	=	i * Header.siXsize;		

		// ���� ������ �޸𸮸� �Ҵ� �޴´�.
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
		

		// �ʿ� �Ӽ��� �������ش�.
		if(SetMap(pReadMap) == FALSE)
			goto END;		

		uiMapID			=	pMapIndexHeader->siMapIndex;
		siMapType		=	pMapIndexHeader->siMapType;
		uiVillageCode	=	pMapIndexHeader->uiVillageCode;
		bIsFieldMap		=	(pMapIndexHeader->fWarFlag == 2);
		siWeather		=	pMapIndexHeader->siWeather;

		// ���� ���� ���� ���´�.
		if(m_siXsize % ON_TILENUM_PER_SECTOR == 0)			siHorizonSectorNum	=	m_siXsize / ON_TILENUM_PER_SECTOR;	
		else												siHorizonSectorNum	=	m_siXsize / ON_TILENUM_PER_SECTOR + 1;	

		// ���� ���� ���� ���´�.
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

				// �� ���͸� �ʱ�ȭ �Ѵ�.				
				InitSector( siSectorIndex );		
				
				m_pstSector[ siSectorIndex ].uiY = i;
				m_pstSector[ siSectorIndex ].uiX = j;

				// ���� ������ �����͸� �����Ѵ�.
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
		// �Ҵ� �޾Ҵ� ��� ���� �����Ѵ�.
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
//	����	:	����.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���͸� �����Ѵ�.
//	���� ����	:	2002-06-03 ���� 3:30:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo)
{
	SI32		siNearCharNum;

	if(pBaseChar->uiAccount == 0)																				return	FALSE;	// ��ȿ���� ���� ID�̴�.
	if(IsInMapArea(pBaseChar->siX, pBaseChar->siY) == FALSE)													return	FALSE;	// �� ���� �ۿ� �ִ�.
	if(pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiTileAtb & ON_TILE_ATB_COLL)				return	FALSE;	// �浹 �����̴�.		
	if((pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiTileKind & uiCharMoveableTileInfo) == 0)	return	FALSE;	// �ش� Ÿ�Ͽ� ĳ���Ͱ� ���� �ִ� Ÿ������ �Ǵ��Ѵ�.
	if(pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID != 0)							return	FALSE;	// �̹� �ٸ� �÷��̾ �ɰ��� �ִ�.		
	
	// ���Ϳ� �߰� ��Ų��.
	if( AddCharToSector(pBaseChar->siX, pBaseChar->siY, pBaseChar) == FALSE )									return	FALSE;	// ���Ϳ� �÷��̾ �߰� ��Ų��.	

	SetCharID(pBaseChar->siX, pBaseChar->siY, pBaseChar);	

	// ������ �÷��̾���� ���Ѵ�.
	siNearCharNum =	GetNearSectorPlayerList(pBaseChar->siX, pBaseChar->siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
	pBaseChar->SendFieldMsg( ON_ADDCHAR, SO_SFM_CUSTOM, LPVOID( siNearCharNum ), LPVOID( uiCharUniIDs ) );	
	
	return	TRUE;	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �̵� ���� �� ���� �߰��Ǵ� ���� �� �����Ǵ� ���͸� ��´�. 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���͸� �̵� ��Ų��.
//	���� ����	:	2002-06-03 ���� 11:38:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::MoveChar( BaseChar *pBaseChar, SI32 siX, SI32 siY, BOOL bWarUnit,UI32 uiCharMoveableTileInfo)
{
	SI32						siCurSectorIndex, siDestSectorIndex;
	SI32						siAddCharsNum, siDelCharsNum, siAddItemsNum;
	SI32						siNewInfoOnMove[3];			// 0���� �����ؾ��� ĳ�����̰� 1���� �����ؾ��� ĳ�����̴�.	


	SI32						i;
	SOPlayer					*pPlayer;	
	cltSOShip					*pSOShip;
	UI16						uiCustomerNum;
	static UI16					uiCustoms[SO_SHIP_MAX_CUSTOMER];	


	static strSector			*pAddSector[ 9 ];				// ���� �߰��Ǵ� ������ �����͸� ������ ��
	static strSector			*pDelSector[ 9 ];				// ������ ������ �����͸� ������ ��

	if(pBaseChar->uiAccount == 0)															return	FALSE;	// ��ȿ���� ���� ID�̴�.
	if(IsInMapArea(siX, siY) == FALSE)														return	FALSE;	// �� ���� �ۿ� �ִ�.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)					return	FALSE;	// �浹 �����̴�.		
	if((pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind & uiCharMoveableTileInfo) == 0)	return	FALSE;	// �ش� Ÿ�Ͽ� ĳ���Ͱ� ���� �ִ� Ÿ������ �Ǵ��Ѵ�.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)								return	FALSE;	// �̹� �ٸ� �÷��̾ �ɰ��� �ִ�.	
				
	// �̵��Ϸ��� ���Ͱ� ���� �ִ� ���Ϳ� ������ �˻��Ѵ�.
	siCurSectorIndex	=	GetSectorIndex(pBaseChar->siX, pBaseChar->siY);		// ���� ĳ���Ͱ� ��ġ�ϰ� �ִ� ���͸� ���Ѵ�.
	siDestSectorIndex	=	GetSectorIndex(siX, siY);							// �̵��� ��ġ�� ���͸� ���Ѵ�.

	// �̵��� ���Ͱ� ���� ������ ���� �������� �˻��Ѵ�.
	if( IsNearSector( siCurSectorIndex, siDestSectorIndex ) == FALSE )
	{
//		pMyWorld->clServer->WriteInfo( "..\\MoveCharError.txt", "[ ID : %s ], [ %u ] [ %d, %d ] --> [ %d, %d ]",
		writeInfoToFile( "MoveCharError.txt", "[ ID : %s ], [ %u ] [ %d, %d ] --> [ %d, %d ]",
										pBaseChar->szID, pBaseChar->uiMapID, pBaseChar->siX, pBaseChar->siY,
										siX, siY );
										
		

		return	FALSE;
	}


	// �ٸ� ���ͷ� �̵��� ����� ó���� ���ش�..
	if(siCurSectorIndex != siDestSectorIndex)
	{
		// �߰��Ǵ� ���Ϳ� �����Ǵ� ���͸� ���Ѵ�		
		GetNewSectorList( siCurSectorIndex, siDestSectorIndex, pAddSector, pDelSector );		

		// ���ο� ���Ϳ� ĳ���͸� �߰� ��Ų��.
		if( AddCharToSector(siDestSectorIndex, pBaseChar) == TRUE )
		{
			// ������ ���Ϳ��� ĳ���͸� �����ϰ� --------------------------------------------------------*
			DelCharToSector(siCurSectorIndex, pBaseChar);		

			// ���Ͱ� ����Ǿ���.
			siAddItemsNum = GetSectorItemList( pAddSector, FieldItemInfo );
			siDelCharsNum = GetSectorPlayerList( pDelSector, &uiAddDelCharsID[ 0 ], ON_MAP_CHARID_FIELD );							
			siAddCharsNum = GetSectorPlayerList( pAddSector, &uiAddDelCharsID[ siDelCharsNum ], ON_MAP_CHARID_FIELD );				

			siNewInfoOnMove[0]	=	siDelCharsNum;			// �����ؾ� �ϴ� ĳ������ ��.		
			siNewInfoOnMove[1]	=	siAddCharsNum;			// �߰��ؾ� �ϴ� ĳ������ ��.		
			siNewInfoOnMove[2]	=	siAddItemsNum;			// �߰��ؾ� �ϴ� �������� ��.		

			if(ISPLAYER(pBaseChar->uiAccount) == TRUE)
			{
				pPlayer	=	(SOPlayer*)pBaseChar;
					
				// ������ ���ο� ������ �����ش�.
				pPlayer->SendFieldMsg(ON_NEWINFOONMOVE, SO_SFM_ONLYME, LPVOID(siNewInfoOnMove), uiAddDelCharsID, FieldItemInfo);
			}		
			else if(ISSHIP(pBaseChar->uiAccount) == TRUE)
			{
				pSOShip			=	(cltSOShip*)pBaseChar;

				// �迡ź �÷��̾�鿡�� ���ο� ������ �����ش�.
				uiCustomerNum	=	pSOShip->GetCustomers(uiCustoms);

				for(i = 0; i < uiCustomerNum; i++)
				{
					if((pPlayer = pMyWorld->pPlayerOnAccount[uiCustoms[i]]) != NULL)
						pPlayer->SendFieldMsg(ON_NEWINFOONMOVE, SO_SFM_ONLYME, LPVOID(siNewInfoOnMove), uiAddDelCharsID, FieldItemInfo);
				}		
			}
			

			// �� ������ �÷��̾�鿡�� ���� ���� �� �߰��϶�� �˷���� �Ѵ�.
			siDelCharsNum = GetSectorPlayerList( pDelSector, &uiAddDelCharsID[ 0 ], ON_MAP_CHARID_ALLPLAYER );							
			siAddCharsNum = GetSectorPlayerList( pAddSector, &uiAddDelCharsID[ siDelCharsNum ], ON_MAP_CHARID_ALLPLAYER );						

			// �� ĳ���� ���� ������ �����ش�. ����� ���Ϳ��������ؼ�..			
			pBaseChar->SendFieldMsg(ON_DELCHAR, SO_SFM_CUSTOM, LPVOID( siDelCharsNum ), LPVOID( uiAddDelCharsID ) );

			// �� ĳ���� �߰� ������ �����ش�. �߰��Ǵ� ���Ϳ��������ؼ�..			
			pBaseChar->SendFieldMsg(ON_ADDCHAR, SO_SFM_CUSTOM, LPVOID( siAddCharsNum ), LPVOID( &uiAddDelCharsID[ siDelCharsNum ]) );		
		}	
		else
		{
			// ���͸� �̵� ��Ű�µ� �����ߴ�.
			return	FALSE;

		}
	}

	DelCharID( pBaseChar->siX, pBaseChar->siY );									// �ʿ��� ĳ������ ID�� �������ش�.
	SetCharID( siX, siY, pBaseChar );												// �ʿ� ĳ������ID�� �������ش�.
		
	// ĳ������ ���ο� ��ġ�� �������ش�.
	pBaseChar->siX	=	siX;
	pBaseChar->siY	=	siY;	

	if(bWarUnit == TRUE)
		pBaseChar->SendFieldMsg( ON_RESPONSE_POSITION, SO_SFM_MEANDNEARCHAR, LPVOID( &pBaseChar->uiAccount ), LPVOID( 1 ) );
	else // ĳ���Ͱ� �̵��ߴٰ� �����Ѵ�.
		pBaseChar->SetMoved();

	return	TRUE;
} 

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ���͸� �����Ѵ�.
//	���� ����	:	2002-06-03 ���� 3:32:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::DelChar(BaseChar *pBaseChar)
{

	// �ʿ��� ������� �ϴ� ĳ������ ID�� �� ĳ������ ID ���� �Ѵ�.
	if( pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID == pBaseChar->uiAccount )		
	{
		DelCharID(pBaseChar->siX, pBaseChar->siY);
	
		// ���Ϳ��� �����Ѵ�.
		DelCharToSector(pBaseChar->siX, pBaseChar->siY, pBaseChar);		
		
		pBaseChar->SendFieldMsg( ON_DELCHAR, SO_SFM_ONLYNEARCHAR ); 
		
		return	TRUE;
	}
	else																													
	{
		// �ʿ��� ������� �ϴ� ĳ������ ID�� �� ĳ������ ID�� �ƴϴ�.
//		pMyWorld->clServer->WriteInfo("..\\log_for_debug.txt", "2 [Map ID : %d]  [uiAccount willing to be deleted : %d]  [ID from the x, y on the map ID : %d]",
		writeInfoToFile("log_for_debug.txt", "2 [Map ID : %d]  [uiAccount willing to be deleted : %d]  [ID from the x, y on the map ID : %d]",
										pBaseChar->uiMapID, pBaseChar->uiAccount, pstMap[puiMultiplyMapYOff[pBaseChar->siY] + pBaseChar->siX].uiCharUniID );
		return	FALSE;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ܼ��� ĳ������ ID�� �ʿ� ������ ���ش�.
//	���� ����	:	2002-06-03 ���� 11:47:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::SetCharID(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	pBaseChar->uiAccount;			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ܼ��� ĳ������ ID�� �ʿ��� ������ �ش�.
//	���� ����	:	2002-06-03 ���� 11:57:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::DelCharID(SI32 siX, SI32 siY)
{
	pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID	=	0;			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ܼ��� �ʿ� �������� ������ ���ش�.
//	���� ����	:	2002-06-03 ���� 12:17:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// DB�� ������ ������Ʈ�Ѵ�.
	if( bUpdateDBSwitch == TRUE )
	{
		if( bSendToDB == TRUE )
		{
			// DB�� �������� �����Ѵ�.
			FieldItemInfo.uiMapID		=	uiMapID;
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;
			FieldItemInfo.uiID			=	uiItemID;
			FieldItemInfo.uiQuantity	=	uiItemQuantity;
		
			pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_APPENDFIELDITEM, &FieldItemInfo, sizeof( FieldItemInfo ) );
		}

	}

	// ������ �� Ÿ���� ���� ������ �̹� �߰��Ǿ� ������ 
	// �� �̻� �߰����� �ʴ´�.
	if( bExistItem == FALSE )
	{
		// �ش� ���Ϳ��� ���� ��Ų��.			
		AddItemToSector( siX, siY, &pstMap[ puiMultiplyMapYOff[ siY ] + siX ] );
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ܼ��� �ʿ� �������� ���Ÿ� ���ش�.
//	���� ����	:	2002-06-03 ���� 12:17:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::DelItemID( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	DBIKFieldItemInfo	FieldItemInfo;

	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	0;
	pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	0;	

	// DB�� ������ ������Ʈ�Ѵ�.
	if(bUpdateDBSwitch == TRUE)
	{
		if( bSendToDB == TRUE )
		{
			// DB������ �������� �����Ѵ�.
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʿ� ID�� �����Ѵ�.
//	���� ����	:	2002-05-21 ���� 3:17:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::SetID(SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI32 uiCharMoveableTileInfo)
{	
	if(IsInMapArea(siX, siY) == FALSE)
	{
		return FALSE;	// �� ���� �ۿ� �ִ�.
	}

	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)
	{
		return FALSE;	// �浹 �����̴�.
	}

	if(pBaseChar->uiAccount != 0)
	{
		// �ʿ� ĳ���͸� �����Ѵ�.

		// �켱 �ش� Ÿ�Ͽ� ĳ���Ͱ� ���� �ִ� Ÿ������ �Ǵ��Ѵ�.
		if((pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind & uiCharMoveableTileInfo) == 0)
		{
			return FALSE;
		}

		if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)
		{
			// �̹� �ٸ� �÷��̾ �ɰ��� �ִ�.
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
		// �ʿ� �������� �����Ѵ�.

		if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0)
		{
			// �̹� �ٸ� �������� �ɰ����ִ�.
			return FALSE;
		}
		else
		{
			pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID			=	uiItemID;
			pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity	=	uiItemQuantity;

			// DB�� ������ ������Ʈ�Ѵ�.
			if(bUpdateDBSwitch == TRUE)
			{
				DBIKFieldItemInfo	FieldItemInfo;

				// DB�� �������� �����Ѵ�.
				FieldItemInfo.siX			=	siX;
				FieldItemInfo.siY			=	siY;
				FieldItemInfo.uiID			=	uiItemID;
				FieldItemInfo.uiQuantity	=	uiItemQuantity;
				pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDFIELDITEM, LPVOID(&FieldItemInfo), NULL);
			}

			// �ش� ���Ϳ��� ���� ��Ų��.			
			AddItemToSector(siX, siY, &pstMap[puiMultiplyMapYOff[siY] + siX]);
			return TRUE;
		}	
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ��� ID�� �����Ѵ�.
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

		// DB�� ������ ������Ʈ�Ѵ�.
		if(bUpdateDBSwitch == TRUE)
		{
			DBIKFieldItemInfo	FieldItemInfo;

			// DB������ �������� �����Ѵ�.
			FieldItemInfo.siX			=	siX;
			FieldItemInfo.siY			=	siY;			
			pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_DELETEFIELDITEM, LPVOID(&FieldItemInfo), NULL);
		}

		// �������� �ִ� Ÿ���� ���� �����ش�.
		DelItemToSector(siX, siY, &pstMap[puiMultiplyMapYOff[siY] + siX]);		
	}	
}
*/
//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� �Ӽ��� �������ش�.
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

	// ������Ʈ�� ��� ĳ���Ϳ� �浹�ϴ� ������Ʈ�� �ְ� �׳� ����ϴ� ������Ʈ�� �ִ�.
	// �� ���θ� �̸� ����Ѵ�.
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

			// Ÿ�� ������ ���´�.
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
			// �浹 ������Ʈ�� �ɰ��� �ִ��� �˾ƿ´�.
			switch(pObjectInfoBfs->BaseBf.uiCode)
			{			
			case ON_MAP_OI_CODE_RELATIVEPOS:
				// �� ��ġ�� ������Ʈ�� �浹 �Ӽ��� ������ ������ �̰͵� �浹 �Ӽ���
				// ������ �ִ�.
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
//	����	:	���� ������ �ִ� ĳ���͵��� ID�� ���´�.
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
//	����	:	�ʿ��� ĳ������ ID�� ���´�.
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
//	����	:	���� ������ �ִ� �������� ID�� ���´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�� Ÿ�Ͽ� �����Ǿ� �ִ� �������� ������ ���´�.
//	����	:	�� Ÿ�Ͽ� �������� ������ TRUE. �������� ������ FALSE.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::GetItemID(SI32 siX, SI32 siY, UI16 *pItemIDs, UI16 *pItemQuantity)
{
	// ��ȿ�� �������� üũ�Ѵ�.
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
//	����	:	��ǥ�� �������ش�.
//----------------------------------------------------------------------------------------------------------------
VOID	SOMap::CorrectionPos(SI32 *psiSX, SI32 *psiSY, SI32 *psiEX, SI32 *psiEY)
{
	*psiSX	=	max(0, *psiSX);
	*psiSY	=	max(0, *psiSY);
	*psiEX	=	min(m_siXsize - 1, *psiEX);
	*psiEY	=	min(m_siYsize - 1, *psiEY);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� �����Ǿ� �ִ� ĳ������ id�� item�� id�� ���´�.
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
						// ���� �����͸� ������ �ִ��� Ȯ���Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʿ��� Ư�� ��ǥ�� ã�´�.
//	���� ����	:	2002-08-05 ���� 11:04:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
POINT	SOMap::SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 )
{

	POINT	TempPoint;
	SI32	TempDistance;
	SI32	TempCount;
	SI32	TempX;
	SI32	TempY;
	SI32	siDirectionCounter;
	strTI	*pTileInfo;
	SI32	siSameItemSearchArea	=	3;			// ���� �������� �ֺ��� �ִ��� �˻��� �� �� ������ ũ��(������)

	// ĳ���͸� ���� ��ġ�� ã����.
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
					case 0:			// ��
						TempX = siCX + TempCount;
						TempY = siCY - TempDistance;
						break;
					case 1:			// �Ʒ�
						TempX = siCX - TempCount;
						TempY = siCY + TempDistance;
						break;
					case 2:			// ��					
						TempX = siCX - TempDistance;
						TempY = siCY - TempCount;
						break;
					case 3:			// ��
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

		// �ʵ忡 ������ �������� ON_MAX_ITEMNUM_ON_TILE�� �ʰ��� �� ����.
		if( siParam2 <= ON_MAX_ITEMNUM_ON_TILE )
		{		

			// ���ִ� �ڸ��� ���� ������ �������� �ִ��� �˻��ؼ� ������ �� ��ġ�� �����Ѵ�.
			if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
			{
				pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

				// �ϴ� �浹 Ÿ���� �ƴϰ� Ÿ�� ������ ���̾�� �Ѵ�.
				if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == siParam1) )
				{
					// ---- 20050216 - ������ ����
					// �� Ÿ�Ͽ� ���� �� �ִ� �ִ� �������� ���� �ʰ����� �ʴ��� �˻��Ѵ�.
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

			// �ֺ��� ���ʷ� ���鼭 ���� ������ �������� �ִ��� �˻��ؼ� ������ �� ��ġ�� �����Ѵ�. (siSameItemSearchArea��ŭ)
			for(TempDistance = 1; TempDistance <= siSameItemSearchArea; TempDistance++)
			{
				for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
				{
					for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
					{
						switch(siDirectionCounter)
						{
						case 0:			// ��
							TempX = siCX + TempCount;		TempY = siCY - TempDistance;		break;
						case 1:			// �Ʒ�
							TempX = siCX - TempCount;		TempY = siCY + TempDistance;		break;
						case 2:			// ��					
							TempX = siCX - TempDistance;	TempY = siCY - TempCount;			break;
						case 3:			// ��
							TempX = siCX + TempDistance;	TempY = siCY + TempCount;			break;
						}

						if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
						{
							pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];

							// �浹 Ÿ���� �ƴϰ� Ÿ�� ������ ���̰� �� Ÿ�Ͽ� ���� ������ �������� ������ �����Ѵ�.
							if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == siParam1) )
							{		
								// �� Ÿ�Ͽ� ���� �� �ִ� �ִ� �������� ���� �ʰ����� �ʴ��� �˻��Ѵ�.
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

			// ���� ������ �������� �߰ߵ��� �ʾ����Ƿ� �ִ� 200������ �� ���� ã�´�.

			// ���� ���ִ� �ڸ��� �������� ���� �� ������ üũ�Ѵ�.
			if((siCX >= 0) && (siCX < m_siXsize) && (siCY >= 0) && (siCY < m_siYsize))
			{
				pTileInfo	=	&pstMap[puiMultiplyMapYOff[siCY] + siCX];

				// �ϴ� �浹 Ÿ���� �ƴϰ� Ÿ�� ������ ���̰� �������� ���� �� ���̸� �����Ѵ�.
				if( (pTileInfo->uiTileAtb & ON_TILE_ATB_COLL) == 0 && (pTileInfo->uiTileKind & ON_TILE_KIND_GROUND) && (pTileInfo->uiItemID == 0) )
				{
					TempPoint.x = siCX;
					TempPoint.y = siCY;
					return TempPoint;
				}
			}

			// �ֺ� �ִ� 200������ �������� ���� �� ���� ã�ƺ���.
			for(TempDistance = 1; TempDistance < 200; TempDistance++)
			{
				for(TempCount = (-TempDistance + 1); TempCount < TempDistance + 1; TempCount++)
				{
					for(siDirectionCounter = 0; siDirectionCounter < 4; siDirectionCounter++)
					{
						switch(siDirectionCounter)
						{
						case 0:			// ��
							TempX = siCX + TempCount;		TempY = siCY - TempDistance;		break;
						case 1:			// �Ʒ�
							TempX = siCX - TempCount;		TempY = siCY + TempDistance;		break;
						case 2:			// ��					
							TempX = siCX - TempDistance;	TempY = siCY - TempCount;			break;
						case 3:			// ��
							TempX = siCX + TempDistance;	TempY = siCY + TempCount;			break;
						}

						if((TempX >= 0) && (TempX < m_siXsize) && (TempY >= 0) && (TempY < m_siYsize))
						{
							pTileInfo	=	&pstMap[puiMultiplyMapYOff[TempY] + TempX];

							// �浹 Ÿ���� �ƴϰ� Ÿ�� ������ ���̰� �������� ���� �� ���̸� �����Ѵ�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	DB�κ��� �ʵ忡 ������ �ִ� �������� ������ ���ͼ� �ʿ� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::SetItemFromDB()
{
	// �������� �����ϱ� ���� �Լ� SetID�� ȣ�� �� �� �������� DB���� �����Ѵ�.
	// ������ DB���� ���°� �ٽ� ������ �ʿ䰡 ���� ������ UpdateDBSwitch�� FALSE ��Ų��.
	bUpdateDBSwitch	=	FALSE;

	// �������� �ʵ忡 �����Ѵ�.
	// �ӵ� �� ȿ������ ����Ͽ� GetDBInfo ������ SetID�� ȣ���Ͽ� �������� �����Ѵ�.
	pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_ALLFIELDITEM, LPVOID(this), NULL);

	bUpdateDBSwitch	=	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SIZE	SOMap::GetMapSize()
{
	SIZE	szMapSize;

	szMapSize.cx	=	m_siXsize;
	szMapSize.cy	=	m_siYsize;

	return szMapSize;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ҼӵǾ� �ִ� ������ �ڵ带 ���´�.
//	���� ����	:	2002-04-22 ���� 11:07:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	SOMap::GetVillageCode()
{
	return uiVillageCode;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�浹 Ÿ������ �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::IsColl(SI32 siX, SI32 siY)
{
	return	OnlineBaseMap::IsColl(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�浹 Ÿ������ �˾ƿ´�.(Ư�� ĳ���� ���忡��)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb)
{
	// �ٸ� ĳ���Ͱ� �� �ִٸ� �浹 Ÿ���̴�.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID != 0)	return	TRUE;

	return	OnlineBaseMap::IsColl(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb, 
								  pstMap[puiMultiplyMapYOff[siY] + siX].uiTileKind,
								siCharMoveableTileAtb);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ΰ�?
//	���� ����	:	2002-04-22 ���� 11:08:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::IsDungeon()
{
	if(siMapType == MAPINDEXPARSERMAPTYPE_DUNGEON)		return	TRUE;
	if(uiVillageCode != 0)								return	TRUE;
	
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �� �� �ִ� �����ΰ�?
//	���� ����	:	2003-04-09 ���� 13:25 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::CanBeRainy()
{
	if( (siWeather & MAPINDEXPARSERWEATHER_RAIN) )		return	TRUE;
	
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� �� �ִ� �����ΰ�?
//	���� ����	:	2003-04-09 ���� 13:27 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::CanBeSnowy()
{
	if( (siWeather & MAPINDEXPARSERWEATHER_SNOW) )		return	TRUE;
	
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���͸� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-05-29 ���� 2:04:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::InitSector(SI32 siArrayIndex)
{
	SI32	siGroundTileNum, siSeaTileNum;	


	siGroundTileNum	= siSeaTileNum = 400;

	// Sector�� �ʱ�ȭ �Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���͸� �����Ѵ�.
//	���� ����	:	2002-05-27 ���� 1:02:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::FreeSector(SI32 siArrayIndex)
{
	m_pstSector[siArrayIndex].PlayerCharList.Release();
	m_pstSector[siArrayIndex].MonsterCharList.Release();	
	m_pstSector[siArrayIndex].ShipCharList.Release();	
	m_pstSector[siArrayIndex].ItemList.Release();
	
	ZeroMemory(m_pstSector[siArrayIndex].pNearSector, sizeof(m_pstSector[siArrayIndex].pNearSector));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ Index�� ���Ѵ�.
//	���� ����	:	2002-05-27 ���� 1:15:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOMap::GetSectorIndex(SI32 siX, SI32 siY)
{
/*
	int x = int( ( siX - siY + m_siXsize - 1) / ON_TILENUM_PER_SECTOR  );
	int y = int( ( siX + siY ) / ON_TILENUM_PER_SECTOR );
	
	return ( y * siHorizonSectorNum ) + x;
*/
	 return	( ( siY / ON_TILENUM_PER_SECTOR ) * siHorizonSectorNum + ( siX / ON_TILENUM_PER_SECTOR ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϳ� �÷��̾ �߰� ��Ų��.
//	���� ����	:	2002-05-30 ���� 10:05:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::AddCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	SI32	siArrayIndex;	

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	// ���Ϳ� �߰��Ѵ�.
	return	AddCharToSector(siArrayIndex, pBaseChar);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϳ� �÷��̾ �߰� ��Ų��.
//	���� ����	:	2002-06-03 ���� 11:53:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::AddCharToSector(SI32	siArrayIndex, BaseChar *pBaseChar)
{
	SI32	siRet = -1;

	strSector *pSector = &m_pstSector[siArrayIndex];

	// ������ �÷��̾ �ٸ� ���Ϳ� �ִٰ� �� ���ͷ� �̵��� �Դٸ� �������ش�.	
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϳ��� �÷��̾ �����Ѵ�.
//	���� ����	:	2002-05-30 ���� 10:09:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::DelCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar)
{
	SI32	siArrayIndex;

	siArrayIndex	=	GetSectorIndex(siX, siY);

	// ���Ϳ��� �����Ѵ�.
	DelCharToSector(siArrayIndex, pBaseChar);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϳ��� �÷��̾ �����Ѵ�.
//	���� ����	:	2002-05-30 ���� 10:09:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::DelCharToSector(SI32	siArrayIndex, BaseChar *pBaseChar)
{	
	SI32	siDeleteTypeRet = -2;
	strSector *pSector = &m_pstSector[siArrayIndex];
	
	// ���Ϳ��� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϳ��� �������� �ִ� Ÿ���� �߰��Ѵ�.
//	���� ����	:	2002-06-03 ���� 9:42:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::AddItemToSector(SI32 siX, SI32 siY, strTI *pstTile)
{
	SI32	siArrayIndex;
		
	if(IsInMapArea(siX, siY) == FALSE)	return;

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	m_pstSector[siArrayIndex].ItemList.Add(pstTile);

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ϳ��� �������� �ִ� Ÿ���� �����Ѵ�.
//	���� ����	:	2002-06-03 ���� 9:47:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::DelItemToSector(SI32 siX, SI32 siY, strTI *pstTile)
{
	SI32	siArrayIndex;
		
	if(IsInMapArea(siX, siY) == FALSE)	return;

	siArrayIndex	=	GetSectorIndex(siX, siY);
	
	m_pstSector[siArrayIndex].ItemList.DeleteType(pstTile);

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ʿ� �����Ѵ�.
//	���� ����	:	2002-06-03 ���� 1:08:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::SetItem(SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	if(IsInMapArea(siX, siY) == FALSE)												return	FALSE;	// �� ���� �ۿ� �ִ�.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)			return	FALSE;	// �浹 �����̴�.
	if(uiItemID == 0 || uiItemQuantity == 0)										return	FALSE;	// ��ȿ���� ���� ���̴�.

	// �����Ϸ��� ���� �̹� �ٸ� �������� �ִ� ��� ���� �����̸� ������ ���ؼ� �����ϰ� �ٸ� �����̸� FALSE�� �����Ѵ�.
	if( pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID != 0 )
	{
		if( pstMap[puiMultiplyMapYOff[siY]+siX].uiItemID == uiItemID )
		{
//			return TRUE;
			// ---- ������ - 20050304 �߰� : ������ ������ �ٽ� üũ�Ѵ�.
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

	SetItemID(siX, siY, uiItemID, uiItemQuantity, bSendToDB );										// �������� �����Ѵ�.

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ʿ��� �����Ѵ�.
//	���� ����	:	2002-06-03 ���� 1:08:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::DelItem( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	if(IsInMapArea(siX, siY) == FALSE)												return	FALSE;	// �� ���� �ۿ� �ִ�.
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiTileAtb & ON_TILE_ATB_COLL)			return	FALSE;	// �浹 �����̴�.	
	if(pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID == 0)							return	FALSE;	// ���� �ɰ��� �ִ� �������� ����.
	
	DelItemID( pPlayer, siX, siY, bSendToDB );																// �������� �����Ѵ�.

	return	TRUE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Polling
//	���� ����	:	2002-06-03 ���� 5:02:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// �� ���Ϳ� 8 ���� ������ �÷��̾ �ִٸ� ���� ó���� ���ش�.		
		if(IsPlayerInNearSector(i) == FALSE)	continue;
		
		pstSector	=	&m_pstSector[i];	

		for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
		{			
			// ���Ͱ� Ȱ���� �ϵ��� �Ѵ�.
			pMyWorld->clMonster.Action(pstSector->MonsterCharList.m_Data[j]);						
		}				

		// �޽����� �����־�� �ϴ� �������̶��
		// �� ���Ϳ��� �̵��� �־��� ĳ���͵��� �޽����� �����ش�.
		if(bSendMoveMsg == TRUE)
		{
			// �̵� �޽����� ������ ������ �Ǿ��ٸ� �̵� �޽����� �����ش�.	
			uiMovedCharsCounter	=	0;	
			
			// �̵��� ���͵��� �����Ѵ�.
			for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clMonster.DidSendMoveInfo(pstSector->MonsterCharList.m_Data[j]) == TRUE)
				{	
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->MonsterCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}
			
			// �̵��� �÷��̾���� �����Ѵ�.
			for(j = 0; j < pstSector->PlayerCharList.m_NowUseDataCount; j++)
			{
				if((pPlayer = pMyWorld->pPlayerOnAccount[pstSector->PlayerCharList.m_Data[j]]) == NULL) continue;
				
				if(pPlayer->DidSendMoveInfo() == TRUE)						
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->PlayerCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}							
			}
			
			// �̵��� �踦 �����Ѵ�.
			for(j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clSOShip.DidSendMoveInfo(pstSector->ShipCharList.m_Data[j]) == TRUE)
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->ShipCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}									
			 
			// �̵� �޽����� ���� ��ü�� �� �� �޽����� �����ش�.
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
			// ���Ͱ� Ȱ���� �ϵ��� �Ѵ�.
			pMyWorld->clMonster.Action(pstSector->MonsterCharList.m_Data[j]);						
		}				

		// �޽����� �����־�� �ϴ� �������̶��
		// �� ���Ϳ��� �̵��� �־��� ĳ���͵��� �޽����� �����ش�.
		if(bSendMoveMsg == TRUE)
		{
			// �̵� �޽����� ������ ������ �Ǿ��ٸ� �̵� �޽����� �����ش�.	
			uiMovedCharsCounter	=	0;	
			
			// �̵��� ���͵��� �����Ѵ�.
			for(j = 0; j < pstSector->MonsterCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clMonster.DidSendMoveInfo(pstSector->MonsterCharList.m_Data[j]) == TRUE)
				{	
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->MonsterCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}
			
			// �̵��� �÷��̾���� �����Ѵ�.
			for(j = 0; j < pstSector->PlayerCharList.m_NowUseDataCount; j++)
			{
				if((pPlayer = pMyWorld->pPlayerOnAccount[pstSector->PlayerCharList.m_Data[j]]) == NULL) continue;
				
				if(pPlayer->DidSendMoveInfo() == TRUE)						
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->PlayerCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}							
			}
			
			// �̵��� �踦 �����Ѵ�.
			for(j = 0; j < pstSector->ShipCharList.m_NowUseDataCount; j++)
			{					
				if(pMyWorld->clSOShip.DidSendMoveInfo(pstSector->ShipCharList.m_Data[j]) == TRUE)
				{
					uiMovedChars[uiMovedCharsCounter]		=	pstSector->ShipCharList.m_Data[j];					
					uiMovedCharsCounter++;
				}
			}									
			
			// �̵� �޽����� ���� ��ü�� �� �� �޽����� �����ش�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ֺ� ���Ϳ� �÷��̾ �ִ� ���Ͱ� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-06-03 ���� 6:16:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	X, Y�� �������� ���� ������ �÷��̾� �� ���͸� ���´�.
//	���� ����	:	2002-11-24 ���� 11:38:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	X, Y�� �������� ���� ������ ������ ������ ���´�.
//	���� ����	:	2002-06-04 ���� 10:52:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ������ ����Ʈ�� �Ѱ� �޾Ƽ� �ش� ���Ϳ� ���ѵ� �����۵��� ����Ʈ�� ��´�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ������ ����Ʈ�� �Ѱ� �޾Ƽ� �ش� ���Ϳ� ���ѵ� �÷��̾� �� ���͵��� ����Ʈ�� ��´�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���������� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 4:26:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::IsWarFieldMap()
{
	return	bIsFieldMap;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Ÿ���� ������ ���´�.
//	���� ����	:	2002-07-18 ���� 3:29:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOMap::GetTileInfo(SI32 siX, SI32 siY, CHAR *pszBuffer)
{
	if(IsInMapArea(siX, siY) == FALSE)		return	FALSE;	// �� ���� �ۿ� �ִ�.		

	sprintf(pszBuffer, "Tile Info [%d, [%d] :: Account : [%u], Item ID : [%u], Item Quantity : [%u]", 
				siX, siY, 
				pstMap[puiMultiplyMapYOff[siY] + siX].uiCharUniID,
				pstMap[puiMultiplyMapYOff[siY] + siX].uiItemID,	
				pstMap[puiMultiplyMapYOff[siY] + siX].uiItemQuantity );

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ID�� ���´�.
//	���� ����	:	2002-07-19 ���� 2:45:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	SOMap::GetMapID()
{
	return	uiMapID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ�Ѵ�.
//	���� ����	:	2002-07-26 ���� 5:12:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOMap::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld				=	pWorld;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	siSector2�� siSector1�� ���� �������� �˾ƿ´�.
//	���� ����	:	2003-05-24 ���� 6:25:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
