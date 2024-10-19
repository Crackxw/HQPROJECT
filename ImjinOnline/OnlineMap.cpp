/************************************************************************************************
	���ϸ� : OnlineMap.h
	����� : ������

	Game���� ���� �÷��� �ϴ� Map�� ������ ������.
************************************************************************************************/
#include <GSL.h>
#include <clGame.h>
#include "..\Gersang\Directx.h"
#include <SoundManager.h>
#include <Graphics16.h>
#include "OnlineWorld.h"
#include "OnlineMap-bf.h"
#include "OnlineObjectParser.h"
#include "OnlineItem.h"
#include "OnlineMyData.h"
#include "OnlineClient.h"
#include "OnlineResource.h"
#include "OnlineMiniMap.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "OnlineVillageManager.h"
#include "OnlineETC.h"
#include "OnlineMapSound.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

#define ABS(x)	(((x) < 0) ? -(x) : (x))

#define ONLINE_NEWTILEDRAW


extern	SoundManager			*g_pSoundManager;
extern  _clGame* pGame;

#define	ON_MAP_DRAWOBJECTBIT			(0x8000)

#define	ON_MAP_MAKEDRAWOBJECT(x)		(ON_MAP_DRAWOBJECTBIT | (x))
#define	ON_MAP_GETOBJECTID(x)			((x) & (~ON_MAP_DRAWOBJECTBIT))
#define	ON_MAP_ISDRAWOBJECT(x)			((x & ON_MAP_DRAWOBJECTBIT) ? TRUE : FALSE)

static	VOID	MakeMapFile(SI32 siDirection);

#define	ON_MAP_CORNER_WESTNORTH			1
#define	ON_MAP_CORNER_EASTNORTH			2
#define	ON_MAP_CORNER_WESTSOUTH			4
#define	ON_MAP_CORNER_EASTSOUTH			8

#define	ON_MAP_TILECACHEWIDTH			(ON_SCREEN_XSIZE + 256)
#define	ON_MAP_TILECACHEHEIGHT			(ON_SCREEN_YSIZE + 128)


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineMap::OnlineMap() : OnlineBaseMap()
{
	SI32	i;	

	pMyOnlineWorld = NULL;
	ZeroMemory(szFileName, sizeof(szFileName));		
	m_MapIndex     = -1;
	ZeroMemory(&m_MapInfo, sizeof(m_MapInfo));
	m_siX		   = 0;
	m_siY		   = 0;

	m_TileDrawCacheSurface = NULL;

	m_pstMap       = NULL;
	siVMercenary   = NULL;

	ZeroMemory(&m_rcScreenRect, sizeof(m_rcScreenRect));	

	for(i = 0; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++)
	{
		uiMultiplyCharMapYOff[i]	=	i * MAX_ONLINE_CHAR_MAP_XSIZE;
	}

	m_fHeroSelect = FALSE;
	fpMap = NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineMap::~OnlineMap()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Create
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::Create(cltOnlineWorld *pOnlineWorld)
{
	m_BlackTile = pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BLACKTILE);


	// ǥ�������� �б�	
	if(FieldParser.Init("Online\\Object\\Field Object.txt") == FALSE)
		clGrp.Error("", "FieldParser.Init(""Online\\Object\\Field Object.txt"")");	



	// Portal Parser
	if(PortalParser.ReadText("Online\\Map\\MapPortal.txt") == FALSE)
		clGrp.Error("", "PortalParser.ReadText(""Online\\Map\\MapPortal.txt"")");	



	pOnlineWorld->pMemoryProtected->AddMemory(PortalParser.pPortalHeader, sizeof(PortalHeader) * PortalParser.siPortalNum);


	// Map Index Parser
	if(MapIndexParser.ReadText("Online\\Map\\MapIndex.txt", pOnlineWorld->pOnlineVillageManager->GetVillageParser()) == FALSE)
		clGrp.Error("", "MapIndexParser.ReadText(""Online\\Map\\MapIndex.txt"")");	


	
	pOnlineWorld->pMemoryProtected->AddMemory(MapIndexParser.pMapIndexHeader, sizeof(MapIndexHeader) * MapIndexParser.siMapIndexNum);

	m_MapBoothInfo.Init();

	m_fHeroSelect = FALSE;

#ifdef  ONLINE_NEWTILEDRAW
	clGrp.CreateOffScreen(m_TileDrawCacheSurface, ON_SCREEN_XSIZE + 256, ON_SCREEN_YSIZE + 128, TRUE, FALSE);
	clGrp.ClearSurface(m_TileDrawCacheSurface);
#else
	m_TileDrawCacheSurface = NULL;
#endif

	return Load(0, pOnlineWorld);					// �⺻������ 0�� ���� ������ �ִ�.
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::Init(SI16 MapIndex, SI16 siXsize, SI16 siYsize, LoadSaveMapTI *pTI, cltOnlineWorld *pOnlineWorld)
{
	SI32			x, y;
	SI16			i;
	MilepostHeader	*pHeader;
	SI16			Max;
	SI32			TempIndex = 0;

	
	m_siX		   = 0;
	m_siY		   = 0;
	m_siRX		   = 0;
	m_siRY		   = 0;
	m_siXsize	   = siXsize;
	m_siYsize	   = siYsize;
	m_MapIndex     = MapIndex;
	m_IsBoothMap   = m_MapStoreParser.IsMapIndex( m_MapIndex );


	ZeroMemory(&m_MapInfo, sizeof(m_MapInfo));
	CopyMemory(&m_MapInfo, MapIndexParser.GetMapIndexData(MapIndex), sizeof(m_MapInfo));
	pMyOnlineWorld = pOnlineWorld;
	m_MapBoothInfo.Init();

	Max = FieldParser.GetTotalNum();
	for(i = 0; i < Max; i++)
	{		
		pHeader	= FieldParser.GetMilepost(i);

		TempIndex = pMyOnlineWorld->pOnlineResource->GetObjectFileIndex(pHeader->szImageFileName);
		pHeader->siFile = (SI16)TempIndex;
	}
	
	if(m_pstMap) delete[] m_pstMap;
	m_pstMap	 = new strTI[MAX_ONLINE_CHAR_MAP_XSIZE * MAX_ONLINE_CHAR_MAP_YSIZE];	
	if(siVMercenary) delete[] siVMercenary;
	siVMercenary = new SI16[MAX_ONLINE_CHAR_MAP_XSIZE * MAX_ONLINE_CHAR_MAP_YSIZE * 4];
	
	if(m_pstMap == NULL)
		return FALSE;

	ZeroMemory(m_pstMap, sizeof(strTI) * MAX_ONLINE_CHAR_MAP_XSIZE * MAX_ONLINE_CHAR_MAP_YSIZE);

	// ��� ���̾ ���ش�.
	DelID(m_siX, m_siY, MAX_ONLINE_CHAR_MAP_XSIZE, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_ALL);
	for(y = 0; y < MAX_ONLINE_CHAR_MAP_YSIZE; y++)
	{
		for(x = 0; x < MAX_ONLINE_CHAR_MAP_XSIZE; x++)
		{
			SetTile(x + m_siX, y + m_siY, -1, -1, 0, 0, 0);
		}
	}		

	m_rcScreenRect.left		=	0;
	m_rcScreenRect.top		=	0;
	m_rcScreenRect.right	=	ON_SCREEN_XSIZE - 1;
	m_rcScreenRect.bottom	=	ON_SCREEN_YSIZE - 34;

	CreateDrawTable();	
	m_fHeroSelect = FALSE;

	ZeroMemory(&m_AniData, sizeof(m_AniData));

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����		:	�� ������ ���� �׸���(ĳ����, ������Ʈ, Ÿ��).
//				:	���� ���콺�� ����Ű�� �ִ� �� ���� ���⼭ ���� �� �ִ�.
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32				siPutX, siPutY;
	SI32				siCharTileX, siCharTileY;
	SI32				siCharTileX1, siCharTileY1;
	SI32				siTileX, siTileY;
	SI32				siX, siY, siBaseCharTileX, siBaseCharTileY;
	SI32				i;		
	strTI*				pstTI;
	strTI*				pstTI1;
	CItemHeader			*pItemHeader;
	SI16				siObjectID;
	SI32				*psiDrawOrder;	
	XSPR				*pItemSpr;
	RECT				rcItemRealRect;
	SI32				siDistance;	
	SI32				siShortDistance;
	UI16				uiMousePointCharID = 0;
	SI32				siMousePointObjectID = 0;
	SI32				siMousePointObjectX = 0;
	SI32				siMousePointObjectY = 0;
	SI32				siMousePointCharX, siMousePointcharY;
	POINT				ptMouse;
	MilepostHeader*		lpMilepostHeader;
	SI32				Kind;
	BOOL				ObjAtt;
	SI16				Width, Height;
	SI16				TempWaterP;
	float				TempTileZoneP;
	SI16				TempTileWater1;
	SI16				TempTileWater2;
	SI16				WaterP   = 100;
	static SI16			SelectID = 0;
	SI16				TempX,  TempY;	




	SI16*				lpVMercenaryIndex;

	pMyOnlineWorld->pMousePointInfo->Init();
	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);
	

	TransformMapXYToCharMapXY(m_siX, m_siY, siBaseCharTileX, siBaseCharTileY);

#ifdef  ONLINE_NEWTILEDRAW
	// Ÿ���� �켱 �׸���.		
	DrawTile(pSurface);				
//	return;
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
#else
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// Ÿ���� �켱 �׸���.		
		DrawTile(pSurface);				
#endif
		m_fHeroSelect = FALSE;

		// �ٴڿ� ����Ŵ� Object ���
		psiDrawOrder	=	siDrawOrder;
		for( i = 0; i < siTileNo; i++, psiDrawOrder += 2)
		{
			siPutX			=	psiDrawOrder[0];
			siPutY			=	psiDrawOrder[1];			

			siCharTileX		=	siBaseCharTileX + siPutX;
			siCharTileY		=	siBaseCharTileY + siPutY;

			siTileX			=	m_siX + siPutX;
			siTileY			=	m_siY + siPutY;						

			if((siCharTileX >= 0) && (siCharTileX < MAX_ONLINE_CHAR_MAP_XSIZE) && (siCharTileY >= 0) && (siCharTileY < MAX_ONLINE_CHAR_MAP_YSIZE))
			{
				pstTI = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];								

				// ������Ʈ�� �׸���.(�̵��Ұ����� Object�� �׸���.)
				if(ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID))
				{	
					if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(ON_MAP_GETOBJECTID(pstTI->uiObjectID)) == TRUE)
					{
						Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(ON_MAP_GETOBJECTID(pstTI->uiObjectID))) & ATB_OBJECT_ISOBJECT;
						ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(ON_MAP_GETOBJECTID(pstTI->uiObjectID), ATB_OBJECT_VILLAGE);
						
						if(Kind || ObjAtt)
						{
							siObjectID		=	ON_MAP_GETOBJECTID(pstTI->uiObjectID);																							

							if(siObjectID != 0)
							{											
								pMyOnlineWorld->pIOnlineObject->DrawTileType(siObjectID, SelectID);												

								if(pMyOnlineWorld->pIOnlineObject->CheckImage(siObjectID, (SI16)ptMouse.x, (SI16)ptMouse.y, pSurface) == TRUE)
								{
									siMousePointObjectX  = siTileX;
									siMousePointObjectY  = siTileY;
									siMousePointObjectID = siObjectID;
								}
							}					
						}
					}
				}					

				// ������Ʈ�� �׸���.(�̵������� Object�� �׸���.)
				if(((pstTI->bUnknownObjectID == TRUE) || ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID)))
				{	
					if(pstTI->bUnknownObjectID == TRUE)
					{
						// �˼� ���� Object�� ���
						if(TransformMapXYToCharMapXY(pstTI->siSourceObjectX, pstTI->siSourceObjectY, siCharTileX1, siCharTileY1) == TRUE)
						{
							pstTI1     = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1];								

							if(ON_MAP_ISDRAWOBJECT(pstTI1->uiObjectID))
							{
								siObjectID = ON_MAP_GETOBJECTID(pstTI1->uiObjectID);
								if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == TRUE)
								{
									// �̵����� Object���� �˻�
									Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
									ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
									if((Kind == 0) && (ObjAtt == 0))
									{
										// ���� Tile��ġ�� ���� �Ʒ������� �˻�
										pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &Width, &Height);
										if(((Width / 2) == (siCharTileX - siCharTileX1)) && ((Height / 2) == (siCharTileY - siCharTileY1)))
										{
											if(siObjectID != 0)
											{											
												pMyOnlineWorld->pIOnlineObject->DrawTileType(siObjectID, SelectID);

												if(pMyOnlineWorld->pIOnlineObject->CheckImage(siObjectID, (SI16)ptMouse.x, (SI16)ptMouse.y, pSurface) == TRUE)
												{
													siMousePointObjectX  = siTileX;
													siMousePointObjectY  = siTileY;
													siMousePointObjectID = siObjectID;
												}
											}					
										}
									}
								}
							}							
						}						
					}
					else
					{
						// �ڽ��� Object�� ���
						siObjectID = ON_MAP_GETOBJECTID(pstTI->uiObjectID);																							
						if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == TRUE)
						{
							Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
							ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
							if((Kind == 0) && (ObjAtt == 0))
							{
								// ���� Tile��ġ�� ���� �Ʒ������� �˻�
								pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &Width, &Height);
								if((Width == 1) && (Height == 1))
								{
									if(siObjectID != 0)
									{											
										pMyOnlineWorld->pIOnlineObject->DrawTileType(siObjectID, SelectID);												

										if(pMyOnlineWorld->pIOnlineObject->CheckImage(siObjectID, (SI16)ptMouse.x, (SI16)ptMouse.y, pSurface) == TRUE)
										{
											siMousePointObjectX  = siTileX;
											siMousePointObjectY  = siTileY;
											siMousePointObjectID = siObjectID;
										}
									}					
								}
							}
						}
					}
				}
			}
		}

		// �ٴ� ���� ����Ŵ� Object ���
		psiDrawOrder	=	siDrawOrder;
		for( i = 0; i < siTileNo; i++, psiDrawOrder += 2)
		{
			siPutX			=	psiDrawOrder[0];
			siPutY			=	psiDrawOrder[1];			

			siCharTileX		=	siBaseCharTileX + siPutX;
			siCharTileY		=	siBaseCharTileY + siPutY;

			siTileX			=	m_siX + siPutX;
			siTileY			=	m_siY + siPutY;						

			if((siCharTileX >= 0) && (siCharTileX < MAX_ONLINE_CHAR_MAP_XSIZE) && (siCharTileY >= 0) && (siCharTileY < MAX_ONLINE_CHAR_MAP_YSIZE))
			{
				pstTI             = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];								
				lpVMercenaryIndex = &siVMercenary[((siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX) * 4];

				// �������� �׸���.
				if(pstTI->uiItemID != 0)
				{
					pItemHeader		=	pMyOnlineWorld->pItemsInfo->GetItem(pstTI->uiItemID);					

					if(pMyOnlineWorld->pOnlineInventory->IsActive())
					{
						if( pMyOnlineWorld->pOnlineInventory->GetItemPickUpID() )
						{
							// actdoll (2004/07/23 18:11) : �ʵ忡 ������ �������� ��ġ�� ������ ������ �ٲ����.
							POINT	ptDest;
							pMyOnlineWorld->pOnlineInventory->GetItemPickUpMapPos( ptDest );
							if( ( ptDest.x == siTileX ) && ( ptDest.y == siTileY ) )	pItemHeader = NULL;
						}
					}
					if(pItemHeader)
					{
						pItemSpr		=	pMyOnlineWorld->pOnlineResource->GetxsprItem( pItemHeader->m_siSm_SpriteFile );												

						// Ÿ�� ��ǥ�� ȭ�� ��ǥ�� ��ȯ�Ѵ�.
						if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siTileX, siTileY, siX, siY) == TRUE)
						{							
							// �������� Real Rect�� �����Ѵ�.
							rcItemRealRect.left		=	siX - (pItemSpr->Header.Xsize / 2) - m_siRX;
							rcItemRealRect.top		=	siY - (pItemSpr->Header.Ysize / 2) - m_siRY;
							rcItemRealRect.right	=	rcItemRealRect.left + pItemSpr->Header.Xsize;
							rcItemRealRect.bottom	=	rcItemRealRect.top + pItemSpr->Header.Ysize;
						
							// �� ���콺�� �������� ����Ű�� �ִ°�?
							if(PtInRect(&rcItemRealRect, ptMouse) == TRUE)
							{
								// ���콺�� �������� ����Ű���� �����Ѵ�.
								pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_ITEM, siTileX, siTileY, pstTI->uiItemID);								

							}				

							clGrp.PutSpriteT(rcItemRealRect.left, rcItemRealRect.top, 
											pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[ pItemSpr->Header.Start[pItemHeader->m_siSm_SpriteFont] ]);
						}											
					}
				}

				// ����ٴϴ� �뺴�� ����Ѵ�.
				if(lpVMercenaryIndex[0] != 0)
				{
					SI32		siVMercenaryCount;

					for(siVMercenaryCount = 0; siVMercenaryCount < 4; siVMercenaryCount++)
					{
						if(lpVMercenaryIndex[siVMercenaryCount] != 0)
						{		
							SI32		siUniqueID;

							siUniqueID = pMyOnlineWorld->pIOnlineChar->GetUniqueID(lpVMercenaryIndex[siVMercenaryCount]);

							// ���� Tile�� ��ġ�� ���� Tile�� �̵��ϴ� ������, �̵��ϴ� ���̸� ������ �󸶳� �Ǵ��� �˻�
							TempTileZoneP = 1.0f;
							pMyOnlineWorld->pIOnlineChar->GetMovePersentByVMercenary(siUniqueID, &TempX);
							if((TempX < 50) && (TempX > 0))
							{
								TempTileZoneP = (float)TempX / 50.0f;
							}

							if(TempTileZoneP < 1.0f)
							{
								// ���� ���� Tile�� �Ѿ���� �ϴ� ���
								// ���� Tile�� ���� Tile�� ���̸� ���
								TempTileWater1 = GetWaterFlag(pstTI);
								switch(TempTileWater1)
								{
									case 0:		TempTileWater1 = 100;	break;
									case 1:		TempTileWater1 = 87;	break;
									case 2:		TempTileWater1 = 76;	break;
									default:	TempTileWater1 = 100;	break;
								}

								// ������ Tile�� ������ ����
								pMyOnlineWorld->pIOnlineChar->GetNPosByVMercenary(siUniqueID, &TempX, &TempY);
								pstTI1 = NULL;
								if(TransformMapXYToCharMapXY(TempX, TempY, siCharTileX1, siCharTileY1) == TRUE)
								{
									pstTI1 = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1];								
								}

								TempTileWater2 = 0;
								if(pstTI1) TempTileWater2 = GetWaterFlag(pstTI1);
								switch(TempTileWater2)
								{
									case 0:		TempTileWater2 = 100;	break;
									case 1:		TempTileWater2 = 87;	break;
									case 2:		TempTileWater2 = 76;	break;
									default:	TempTileWater2 = 100;	break;
								}

								TempWaterP = TempTileWater1 + (SI16)((float)(TempTileWater2 - TempTileWater1) * TempTileZoneP);
							}
							else
							{
								// ���� ���� Tile�� �ͼ� �߾����� �̵��Ϸ��� ���
								// ���� Tile�� ���� Tile�� ���̸� ���
								TempTileWater1 = GetWaterFlag(pstTI);
								TempWaterP     = 100;
								switch(TempTileWater1)
								{
									case 0:	TempWaterP = 100;	break;
									case 1:	TempWaterP = 85;	break;
									case 2:	TempWaterP = 76;	break;
								}
							}
							pMyOnlineWorld->pIOnlineChar->DrawByVMercenary(lpVMercenaryIndex[siVMercenaryCount], TempWaterP);
						}
					}
				}

				// ĳ���͸� �׸���.
				if(pstTI->siCharID != 0)
				{					
					SI32		siUniqueID;

					siUniqueID = pMyOnlineWorld->pIOnlineChar->GetUniqueID(pstTI->siCharID);

					// �� ĳ������ ��쿡�� ���� ���콺�� ������ �ʴ´�.
					if(siUniqueID != pMyOnlineWorld->pMyData->GetMyCharUniqueID())
					{
						if(pMyOnlineWorld->pIOnlineChar->IsPointToChar(pstTI->siCharID, &ptMouse, &siDistance) == TRUE)
						{			
							if(uiMousePointCharID == 0)	
							{
								siShortDistance			=	siDistance;
								uiMousePointCharID		=	pstTI->siCharID;
								siMousePointCharX		=	siTileX;
								siMousePointcharY		=	siTileY;
							}
							else 
							{
								if(siShortDistance < siDistance)
								{
									siShortDistance			=	siDistance;
									uiMousePointCharID		=	pstTI->siCharID;
									siMousePointCharX		=	siTileX;
									siMousePointcharY		=	siTileY;
								}						
							}

						}						
					}						
					else
					{
						if(pMyOnlineWorld->pIOnlineChar->IsPointToChar(pstTI->siCharID, &ptMouse, &siDistance) == TRUE)
						{			
							if(uiMousePointCharID == 0)	
							{
								m_fHeroSelect = TRUE;
							}
							else 
							{
								if(siShortDistance < siDistance)
								{
									m_fHeroSelect = TRUE;
								}						
							}
						}
					}

					// ���� Tile�� ��ġ�� ���� Tile�� �̵��ϴ� ������, �̵��ϴ� ���̸� ������ �󸶳� �Ǵ��� �˻�
					TempTileZoneP = 1.0f;
					pMyOnlineWorld->pIOnlineChar->GetMovePersent(siUniqueID, &TempX);
					if((TempX < 50) && (TempX > 0))
					{
						TempTileZoneP = (float)TempX / 50.0f;
					}

					if(TempTileZoneP < 1.0f)
					{
						// ���� ���� Tile�� �Ѿ���� �ϴ� ���
						// ���� Tile�� ���� Tile�� ���̸� ���
						TempTileWater1 = GetWaterFlag(pstTI);
						switch(TempTileWater1)
						{
							case 0:		TempTileWater1 = 100;	break;
							case 1:		TempTileWater1 = 87;	break;
							case 2:		TempTileWater1 = 76;	break;
							default:	TempTileWater1 = 100;	break;
						}

						// ������ Tile�� ������ ����
						pMyOnlineWorld->pIOnlineChar->GetNPos(siUniqueID, &TempX, &TempY);
						pstTI1 = NULL;
						if(TransformMapXYToCharMapXY(TempX, TempY, siCharTileX1, siCharTileY1) == TRUE)
						{
							pstTI1 = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1];								
						}

						TempTileWater2 = 0;
						if(pstTI1) TempTileWater2 = GetWaterFlag(pstTI1);
						switch(TempTileWater2)
						{
							case 0:		TempTileWater2 = 100;	break;
							case 1:		TempTileWater2 = 87;	break;
							case 2:		TempTileWater2 = 76;	break;
							default:	TempTileWater2 = 100;	break;
						}

						TempWaterP = TempTileWater1 + (SI16)((float)(TempTileWater2 - TempTileWater1) * TempTileZoneP);
					}
					else
					{
						// ���� ���� Tile�� �ͼ� �߾����� �̵��Ϸ��� ���
						// ���� Tile�� ���� Tile�� ���̸� ���
						TempTileWater1 = GetWaterFlag(pstTI);
						TempWaterP     = 100;
						switch(TempTileWater1)
						{
							case 0:	TempWaterP = 100;	break;
							case 1:	TempWaterP = 85;	break;
							case 2:	TempWaterP = 76;	break;
						}
					}
					pMyOnlineWorld->pIOnlineChar->Draw(pstTI->siCharID, TempWaterP);
					if(pMyOnlineWorld->pMyData->GetMyCharUniqueID() == siUniqueID) WaterP = TempWaterP;
				}

				// ������Ʈ�� �׸���.(�̵��Ұ����� Object�� �׸���.)
				if(ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID))
				{	
					if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(ON_MAP_GETOBJECTID(pstTI->uiObjectID)) == FALSE)
					{
						Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(ON_MAP_GETOBJECTID(pstTI->uiObjectID))) & ATB_OBJECT_ISOBJECT;
						ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(ON_MAP_GETOBJECTID(pstTI->uiObjectID), ATB_OBJECT_VILLAGE);
						
						if(Kind || ObjAtt)
						{
							siObjectID		=	ON_MAP_GETOBJECTID(pstTI->uiObjectID);																							

							if(siObjectID != 0)
							{											
								pMyOnlineWorld->pIOnlineObject->Draw(siObjectID, SelectID);												

								if(pMyOnlineWorld->pIOnlineObject->CheckImage(siObjectID, (SI16)ptMouse.x, (SI16)ptMouse.y, pSurface) == TRUE)
								{
									siMousePointObjectX  = siTileX;
									siMousePointObjectY  = siTileY;
									siMousePointObjectID = siObjectID;
								}
							}					
						}
					}
				}

				// ������Ʈ�� �׸���.(�̵������� Object�� �׸���.)
				if((pstTI->bUnknownObjectID == TRUE) || ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID))
				{	
					if(pstTI->bUnknownObjectID == TRUE)
					{
						// �˼� ���� Object�� ���
						if(TransformMapXYToCharMapXY(pstTI->siSourceObjectX, pstTI->siSourceObjectY, siCharTileX1, siCharTileY1) == TRUE)
						{
							pstTI1     = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1];								

							if(ON_MAP_ISDRAWOBJECT(pstTI1->uiObjectID))
							{
								siObjectID = ON_MAP_GETOBJECTID(pstTI1->uiObjectID);
								if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == FALSE)
								{
									// �̵����� Object���� �˻�
									Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
									ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
									if((Kind == 0) && (ObjAtt == 0))
									{
										// ���� Tile��ġ�� ���� �Ʒ������� �˻�
										pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &Width, &Height);
										if(((Width / 2) == (siCharTileX - siCharTileX1)) && ((Height / 2) == (siCharTileY - siCharTileY1)))
										{
											if(siObjectID != 0)
											{											
												pMyOnlineWorld->pIOnlineObject->Draw(siObjectID, SelectID);

												if(pMyOnlineWorld->pIOnlineObject->CheckImage(siObjectID, (SI16)ptMouse.x, (SI16)ptMouse.y, pSurface) == TRUE)
												{
													siMousePointObjectX  = siTileX;
													siMousePointObjectY  = siTileY;
													siMousePointObjectID = siObjectID;
												}
											}					
										}
									}
								}
							}							
						}						
					}
					else
					{
						// �ڽ��� Object�� ���
						siObjectID = ON_MAP_GETOBJECTID(pstTI->uiObjectID);																							
						if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == FALSE)
						{
							Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
							ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
							if((Kind == 0) && (ObjAtt == 0))
							{
								// ���� Tile��ġ�� ���� �Ʒ������� �˻�
								pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &Width, &Height);
								if((Width == 1) && (Height == 1))
								{
									if(siObjectID != 0)
									{											
										pMyOnlineWorld->pIOnlineObject->Draw(siObjectID, SelectID);												

										if(pMyOnlineWorld->pIOnlineObject->CheckImage(siObjectID, (SI16)ptMouse.x, (SI16)ptMouse.y, pSurface) == TRUE)
										{
											siMousePointObjectX  = siTileX;
											siMousePointObjectY  = siTileY;
											siMousePointObjectID = siObjectID;
										}
									}					
								}
							}
						}
					}
				}

				if(pstTI->siFieldGuideTable != 0)
				{
					lpMilepostHeader = FieldParser.GetMilepost(pstTI->siFieldGuideTable - 1);
					pItemSpr = pMyOnlineWorld->pOnlineResource->GetxsprObject(lpMilepostHeader->siFile);

					if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siTileX, siTileY, siX, siY) == TRUE)
					{
						clGrp.PutSpriteT(
							siX - (pItemSpr->Header.Xsize / 2) - m_siRX
							, siY - pItemSpr->Header.Ysize - m_siRY
							, pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[ pItemSpr->Header.Start[0]]);
					}
				}

				// ���� Item ���
				if( !m_IsBoothMap )	continue;

				// ���� ���
				BoothMapDraw(siTileX, siTileY, ptMouse, pSurface);
			}
		}

		// �ִϸ��̼� ��ο�
		AnimationDraw(pSurface);
		clGrp.UnlockSurface(pSurface);
	}	

	// Interface�� Mouse Point�� �ɸ����� �˻�
	BOOL		fFlag = FALSE;
	TransformMouseCoordToMapXY(ptMouse.x, ptMouse.y, siX, siY);
	if(pMyOnlineWorld->pOnlineFieldArea)
	{
		if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE)
		{
			pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_INTERFACE, siX, siY);
			fFlag = TRUE;
		}
	}
	if(pMyOnlineWorld->pMousePointInfo->GetPointKind() != ON_MOUSE_POINT_INTERFACE)
	{
		if(pMyOnlineWorld->pOnlineInventory)
		{
			if(pMyOnlineWorld->pOnlineInventory->IsActive())
			{
				if(pMyOnlineWorld->pOnlineInventory->GetDragItemInfo() != -1)
				{
					pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_INTERFACE, siX, siY);
					fFlag = TRUE;
				}
			}
		}
	}
	if(fFlag == FALSE)
	{
		if(pMyOnlineWorld->pMousePointInfo->GetPointKind() == ON_MOUSE_POINT_INTERFACE)
		{
			pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_FIELD, siX, siY);
		}
	}

	// ĳ���͸� ����Ű�� ���� ó���Ѵ�.
	SelectID = 0;
	if(pMyOnlineWorld->pMousePointInfo->GetPointKind() != ON_MOUSE_POINT_INTERFACE)
	{
		// Character �˻�
		if(uiMousePointCharID)
		{
			// ���� ����Ű�� ĳ���Ͱ� �ڱ� �ڽ��� �ƴϿ��� �Ѵ�.
			if((pMyOnlineWorld->pIOnlineChar->GetUniqueID(uiMousePointCharID) != pMyOnlineWorld->pMyData->GetMyCharUniqueID())
				&& (pMyOnlineWorld->pIOnlineChar->GetShow(pMyOnlineWorld->pIOnlineChar->GetUniqueID(uiMousePointCharID)) == TRUE))	// �׷�����(��������) ĳ�������� �˻�
			{
				pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_CHAR, siMousePointCharX, siMousePointcharY, 
									pMyOnlineWorld->pIOnlineChar->GetUniqueID(uiMousePointCharID));
			}		
		}	

		if(pMyOnlineWorld->pMousePointInfo->GetPointKind() == ON_MOUSE_POINT_FIELD)
		{
			// ������Ʈ �˻�
			if(siMousePointObjectID)
			{
				// �����Ѱ� NPC���� �˻�
				NPCIndexHeader*		lpNPCIndexHeader;
				
				lpNPCIndexHeader = m_NPCParser.GetNPC(siMousePointObjectX, siMousePointObjectY, m_MapIndex);
				if(lpNPCIndexHeader)
				{
					pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_NPC, siMousePointObjectX, siMousePointObjectY, lpNPCIndexHeader->siCode);
					SelectID = siMousePointObjectID;
				}
				else
				{
					pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_OBJECT, siMousePointObjectX, siMousePointObjectY, siMousePointObjectID);
					SelectID = siMousePointObjectID;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����		:	Ÿ�ϸ� �׸���.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::DrawTile(LPDIRECTDRAWSURFACE7 pSurface)
{
	DWORD	TempCheckTime;

	TempCheckTime = GetNowCpuTime();

#ifdef  ONLINE_NEWTILEDRAW
/*	SI16 siCountX, siCountY;
	SI32 TempXX, TempYY, siCharTileX, siCharTileY;
	strTI*	pstTI;

	// Cache�� �ö���ִ� Ÿ�ϵ��� �������� �ʱ����� GetxsprTile()�� ȣ���Ѵ�.
	for(siCountY = -ON_MAP_VIEW_YSIZE_HALF; siCountY <= ON_MAP_VIEW_YSIZE_HALF; siCountY++)
	{
		TempYY = m_siY + siCountY;
		for(siCountX = -ON_MAP_VIEW_XSIZE_HALF; siCountX <= ON_MAP_VIEW_XSIZE_HALF; siCountX++)
		{
			TempXX = m_siX + siCountX;
			TransformMapXYToCharMapXY(TempXX, TempYY, siCharTileX, siCharTileY);
			pstTI = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];
			if(pstTI->siFile != -1)
				pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->siFile);
		}
	}
*/
	RECT	TempRect;
	TempRect.left   = (ON_MAP_TILECACHEWIDTH  / 2) - ((m_rcScreenRect.right - m_rcScreenRect.left + 1) / 2) + m_siRX;
	TempRect.top    = (ON_MAP_TILECACHEHEIGHT / 2) - ((m_rcScreenRect.bottom - m_rcScreenRect.top + 1) / 2) + m_siRY;						
	TempRect.right  = TempRect.left + 800;
	TempRect.bottom = TempRect.top  + 600;
	pSurface->BltFast(0, 0, m_TileDrawCacheSurface, &TempRect, DDBLTFAST_WAIT);
#else
	SI16	siCountX, siCountY;
	SI32	siCenterX, siCenterY;
	SI32	siStartX, siStartY;
	SI32	siCharTileX, siCharTileY;
	strTI*	pstTI;
	XSPR	*pXsprTile;	
	XSPR	*pXsprRoad;
	SI16	Temp_YetTileIndex;								// ����ȭ�� ���� ���(�ٷ� ���� Tile ������ �����ϰ�, ������ ���� Tile�� Index�� ������ �� Tile Data�� �̿��ϰ� ��)
	XSPR	*Temp_YetpXsprTile;	
	SI16	Temp_YetRoadIndex;
	XSPR	*Temp_YetpXsprRoad;
	
	Temp_YetTileIndex = -1;
	Temp_YetpXsprTile = m_BlackTile;	
	Temp_YetRoadIndex = 0;
	Temp_YetpXsprRoad = NULL;
	for(siCountY = -ON_MAP_VIEW_YSIZE_HALF; siCountY <= ON_MAP_VIEW_YSIZE_HALF; siCountY++)
	{
		for(siCountX = -ON_MAP_VIEW_XSIZE_HALF; siCountX <= ON_MAP_VIEW_XSIZE_HALF; siCountX++)
		{
			if(((m_siX + siCountX) < 0) || ((m_siY + siCountY) < 0)	|| ((m_siX + siCountX) >= m_siXsize) || ((m_siY + siCountY) >= m_siYsize))
			{
				// Tile �ʱ�ȭ
				TransformMapXYToCharMapXYNonClipping((m_siX + siCountX), (m_siY + siCountY), siCharTileX, siCharTileY);

				// Ÿ���� �߽����� ���Ѵ�.
				siCenterX = (siCountX * ONLINE_TILE_XSIZE_HALF) - (siCountY * ONLINE_TILE_XSIZE_HALF) + (m_rcScreenRect.right - m_rcScreenRect.left + 1) / 2 - m_siRX;
				siCenterY = (siCountX * ONLINE_TILE_YSIZE_HALF) + (siCountY * ONLINE_TILE_YSIZE_HALF) + (m_rcScreenRect.bottom - m_rcScreenRect.top + 1) / 2 - m_siRY;						
			
				// Ÿ���� ũ�⿡ ���߾ �������� ��ġ�� ����Ѵ�.
				siStartX  = siCenterX - ONLINE_TILE_XSIZE_HALF;
				siStartY  = siCenterY - ONLINE_TILE_YSIZE_HALF;				
				pXsprTile = m_BlackTile;

				// ���.
				clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, pXsprTile->Image);
			}
			else
			{
				TransformMapXYToCharMapXY((m_siX + siCountX), (m_siY + siCountY), siCharTileX, siCharTileY);

				// Ÿ���� �߽����� ���Ѵ�.
				siCenterX = (siCountX * ONLINE_TILE_XSIZE_HALF) - (siCountY * ONLINE_TILE_XSIZE_HALF) + (m_rcScreenRect.right - m_rcScreenRect.left + 1) / 2 - m_siRX;
				siCenterY = (siCountX * ONLINE_TILE_YSIZE_HALF) + (siCountY * ONLINE_TILE_YSIZE_HALF) + (m_rcScreenRect.bottom - m_rcScreenRect.top + 1) / 2 - m_siRY;

				// Ÿ���� ũ�⿡ ���߾ �������� ��ġ�� ����Ѵ�.
				siStartX = siCenterX - ONLINE_TILE_XSIZE_HALF;
				siStartY = siCenterY - ONLINE_TILE_YSIZE_HALF;				

				pstTI = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];
	
				pXsprTile = Temp_YetpXsprTile;
				if(pstTI->siFile != Temp_YetTileIndex)
				{
					pXsprTile = m_BlackTile;
					if(pstTI->siFile != -1)
					{
						pXsprTile = pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->siFile);

						Temp_YetTileIndex = pstTI->siFile;
						Temp_YetpXsprTile = pXsprTile;
					}
				}

				pXsprRoad = Temp_YetpXsprRoad;
				if(pstTI->uiRoadFile != Temp_YetRoadIndex)
				{
					pXsprRoad = NULL;
					if(pstTI->uiRoadFile != 0)
					{					
						pXsprRoad = pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->uiRoadFile);

						Temp_YetRoadIndex = pstTI->uiRoadFile;
						Temp_YetpXsprRoad = pXsprRoad;
					}
				}

				// ���.
				clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, &pXsprTile->Image[pXsprTile->Header.Start[pstTI->siFont]]);
				if(GetAsyncKeyState(VK_SHIFT))
				{
					if(siVMercenary[(((siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX) * 4)])
					{
						clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, m_BlackTile->Image);
					}
				}

				if(pXsprRoad != NULL)
				{						
					clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, &pXsprRoad->Image[pXsprRoad->Header.Start[pstTI->uiRoadFont]]);					
				}
			}
		}
	}	
#endif

	pMyOnlineWorld->siMapTileDrawTimeAddCount++;
	pMyOnlineWorld->siMapTileDrawTimeAddTime = pMyOnlineWorld->siMapTileDrawTimeAddTime + (__int64)(GetNowCpuTime() - TempCheckTime);

	if(pMyOnlineWorld->siMapTileDrawTimeAddCount > 15)
	{
		pMyOnlineWorld->dwMapTileDrawTime = pMyOnlineWorld->siMapTileDrawTimeAddTime / pMyOnlineWorld->siMapTileDrawTimeAddCount;

		pMyOnlineWorld->siMapTileDrawTimeAddTime  = 0;
		pMyOnlineWorld->siMapTileDrawTimeAddCount = 0;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Tile Cache�� Ÿ�ϸ� �׸���.
//----------------------------------------------------------------------------------------------------------------
VOID OnlineMap::DrawTileCache()
{
#ifdef  ONLINE_NEWTILEDRAW
	DWORD	TempCheckTime;
	SI16	siCountX, siCountY;
	SI32	siCenterX, siCenterY;
	SI32	siStartX, siStartY;
	SI32	siCharTileX, siCharTileY;
	strTI*	pstTI;
	XSPR	*pXsprTile;	
	XSPR	*pXsprRoad;
	SI16	Temp_YetTileIndex;								// ����ȭ�� ���� ���(�ٷ� ���� Tile ������ �����ϰ�, ������ ���� Tile�� Index�� ������ �� Tile Data�� �̿��ϰ� ��)
	XSPR	*Temp_YetpXsprTile;	
	SI16	Temp_YetRoadIndex;
	XSPR	*Temp_YetpXsprRoad;
	SI32	TempsiCountXMulONLINE_TILE_XSIZE_HALF;
	SI32	TempsiCountXMulONLINE_TILE_YSIZE_HALF;
	SI32	TempsiCountYMulONLINE_TILE_XSIZE_HALF;
	SI32	TempsiCountYMulONLINE_TILE_YSIZE_HALF;
	SI32	TempON_MAP_TILECACHEWIDTHHALF;
	SI32	TempON_MAP_TILECACHEHEIGHTHALF;
	SI32	TempXX;
	SI32	TempYY;
	
	TempCheckTime = GetNowCpuTime();

	TempON_MAP_TILECACHEWIDTHHALF  = ON_MAP_TILECACHEWIDTH  / 2;
	TempON_MAP_TILECACHEHEIGHTHALF = ON_MAP_TILECACHEHEIGHT / 2;

	if(clGrp.LockSurface(m_TileDrawCacheSurface) == TRUE)
	{
		clGrp.SetClipArea(0, 0, ON_MAP_TILECACHEWIDTH - 1, ON_MAP_TILECACHEHEIGHT - 1);

		Temp_YetTileIndex = -1;
		Temp_YetpXsprTile = m_BlackTile;	
		Temp_YetRoadIndex = 0;
		Temp_YetpXsprRoad = NULL;
		for(siCountY = -ON_MAP_VIEW_YSIZE_HALF; siCountY <= ON_MAP_VIEW_YSIZE_HALF; siCountY++)
		{
			TempYY                                = m_siY + siCountY;
			TempsiCountYMulONLINE_TILE_XSIZE_HALF = siCountY * ONLINE_TILE_XSIZE_HALF;
			TempsiCountYMulONLINE_TILE_YSIZE_HALF = siCountY * ONLINE_TILE_YSIZE_HALF;

			for(siCountX = -ON_MAP_VIEW_XSIZE_HALF; siCountX <= ON_MAP_VIEW_XSIZE_HALF; siCountX++)
			{
				TempXX                                = m_siX + siCountX;
				TempsiCountXMulONLINE_TILE_XSIZE_HALF = siCountX * ONLINE_TILE_XSIZE_HALF;
				TempsiCountXMulONLINE_TILE_YSIZE_HALF = siCountX * ONLINE_TILE_YSIZE_HALF;

				// Ÿ���� �߽����� ���Ѵ�.
				siCenterX = TempsiCountXMulONLINE_TILE_XSIZE_HALF - TempsiCountYMulONLINE_TILE_XSIZE_HALF + TempON_MAP_TILECACHEWIDTHHALF;
				siCenterY = TempsiCountXMulONLINE_TILE_YSIZE_HALF + TempsiCountYMulONLINE_TILE_YSIZE_HALF + TempON_MAP_TILECACHEHEIGHTHALF;
			
				// Ÿ���� ũ�⿡ ���߾ �������� ��ġ�� ����Ѵ�.
				siStartX  = siCenterX - ONLINE_TILE_XSIZE_HALF;
				siStartY  = siCenterY - ONLINE_TILE_YSIZE_HALF;				

				if((TempXX < 0) || (TempYY < 0)	|| (TempXX >= m_siXsize) || (TempYY >= m_siYsize))
				{
					pXsprTile = m_BlackTile;

					// ���.
					clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, pXsprTile->Image);
				}
				else
				{
					TransformMapXYToCharMapXY(TempXX, TempYY, siCharTileX, siCharTileY);

					pstTI = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];
		
					pXsprTile = Temp_YetpXsprTile;
					if(pstTI->siFile != Temp_YetTileIndex)
					{
						pXsprTile = m_BlackTile;
						if(pstTI->siFile != -1)
						{
							pXsprTile = pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->siFile);

							Temp_YetTileIndex = pstTI->siFile;
							Temp_YetpXsprTile = pXsprTile;
						}
					}

					pXsprRoad = Temp_YetpXsprRoad;
					if(pstTI->uiRoadFile != Temp_YetRoadIndex)
					{
						pXsprRoad = NULL;
						if(pstTI->uiRoadFile != 0)
						{					
							pXsprRoad = pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->uiRoadFile);

							Temp_YetRoadIndex = pstTI->uiRoadFile;
							Temp_YetpXsprRoad = pXsprRoad;
						}
					}

					if(pXsprTile != NULL)
					{
						clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, &pXsprTile->Image[pXsprTile->Header.Start[pstTI->siFont]]);
					}

					if(pXsprRoad != NULL)
					{						
						clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, &pXsprRoad->Image[pXsprRoad->Header.Start[pstTI->uiRoadFont]]);					
					}
//--------
// SoundMap������ üũ�Ϸ���... �ʿ�.
#ifdef _DEBUG
				{
				SM_POINT pos;
				SM_ATTRIB smAttrib;
				int r = 0, g = 0, b = 255;

				pos.x = TempXX;
				pos.y = TempYY;

				smAttrib = pMyOnlineWorld->m_pOnlineMapSound->GetAttrib(pos);

				switch(smAttrib & 0xF0)
				{
					case SMA_KOREA :
						r = 0, g = 0, b = 63;
						break;
					case SMA_JAPAN :
						r = 0, g = 63, b = 63;
						break;
					case SMA_TAIWAN	:
						r = 0, g = 63, b = 0;
						break;
					case SMA_CHINA :
						r = 63, g = 63, b = 0;
						break;
				}

				int x0, x1, x2, x3, y0, y1, y2, y3;

				x0 = siCenterX; y0 = siCenterY - ONLINE_TILE_YSIZE_HALF;
				x1 = siCenterX + ONLINE_TILE_XSIZE_HALF - 1; y1 = siCenterY;
				x2 = siCenterX; y2 = siCenterY + ONLINE_TILE_YSIZE_HALF; - 1;
				x3 = siCenterX - ONLINE_TILE_XSIZE_HALF; y3 = siCenterY;

				clGrp.LineC(x0, y0, x3, y3, r, g, b);
				clGrp.LineC(x1, y1, x0, y0, r, g, b);
				clGrp.LineC(x2, y2, x1, y1, r, g, b);
				clGrp.LineC(x3, y3, x2, y2, r, g, b);
//				clGrp.BoxC(siStartX, siStartY, siStartX + ONLINE_TILE_XSIZE - 1, siStartY + ONLINE_TILE_YSIZE - 1, 120);
				}
#endif
//--------
				}
			}
		}	

		clGrp.SetClipFullScreen();
		clGrp.UnlockSurface(m_TileDrawCacheSurface);
	}

	pMyOnlineWorld->siMapTileDrawTimeAddTime = pMyOnlineWorld->siMapTileDrawTimeAddTime + (__int64)(GetNowCpuTime() - TempCheckTime);
#endif
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	����.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::Free()
{	
	if(m_AniData.AniSpr.Image)	 {clGrp.FreeXspr(m_AniData.AniSpr);   m_AniData.AniSpr.Image  = NULL;}

	if(m_TileDrawCacheSurface)
	{
		m_TileDrawCacheSurface->Release();
		m_TileDrawCacheSurface = NULL;
	}

	FieldParser.Free();
	PortalParser.Free();
	MapIndexParser.Free();
	m_MapStoreParser.Free();
	m_NPCParser.Free();
	m_MapBoothInfo.Free();

	if(fpMap)
	{
		fclose(fpMap);
		fpMap	=	NULL;
	}

	// �⺻ ���� �����Ѵ�.
	if(m_pstMap)
	{
		delete [] m_pstMap;
		m_pstMap	=	NULL;
	}
	if(siVMercenary)
	{
		delete [] siVMercenary;
		siVMercenary = NULL;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���ΰ� ĳ���͸� ����� ������ ���� �̵��Ѵ�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::Action()
{
	SI32	siDirection = 0;
	SI16	siX, siY, siRX, siRY;
	SI16	TempCount;
	strTI*	lpstrTI;
	BOOL	reLoadBooth = FALSE;
	
	pMyOnlineWorld->pIOnlineChar->GetPos(m_uiCurrentFocusCharID, &siX, &siY);
	pMyOnlineWorld->pIOnlineChar->GetRPos(m_uiCurrentFocusCharID, &siRX, &siRY);

	// ���� �Ӽ��� �˻��Ͽ� �̻��� �Ӽ��� ����Ǹ� ó��
	lpstrTI = m_pstMap;
	for(TempCount = ((MAX_ONLINE_CHAR_MAP_XSIZE * MAX_ONLINE_CHAR_MAP_YSIZE) - 1); TempCount >= 0; TempCount--)
	{
		if(lpstrTI->siBackupAtb & ON_TILE_ATB_COLL)
		{
			lpstrTI->siAtb |= ON_TILE_ATB_COLL;
		}

		lpstrTI++;
	}
	
	// �ε�Ǿ� ���� ���� Ÿ���� �ε��Ѵ�.
	if(m_siX != siX || m_siY != siY)
	{
		if(m_siX < siX)
			siDirection	|=	ON_EAST;
		else if(m_siX > siX)
			siDirection	|=	ON_WEST;

		if(m_siY < siY)
			siDirection	|=	ON_SOUTH;
		else if(m_siY > siY)
			siDirection	|=	ON_NORTH;

		if(ABS(m_siX - siX) > 1) siDirection = 0;
		if(ABS(m_siY - siY) > 1) siDirection = 0;

		if(siDirection)
		{
			// PreProcessorBeforeLoadUnloadedTile�� ���ο� ��ġ�� ���ϱ� ���� ���־�� �� �۾����� ���ִ� ���̴�.
			// ������Ʈ ������ ���⼭ ���־�� �Ѵ�.		
			PreProcessorBeforeLoadUnloadedTile(siDirection);
		}
		else
		{
			for(SI32 TempCount = 0; TempCount < MAX_ONLINE_OBJECT; TempCount++)
			{
				pMyOnlineWorld->pIOnlineObject->Free(TempCount);
			}
		}

		// ĳ���Ͱ� �����̸� ������ �ٽ� �о� ���� �ʿ䰡 �ֵ�.
		reLoadBooth =	TRUE;
		m_siX		=	siX;
		m_siY		=	siY;

		// LoadUnloadedTile�� �ݵ�� m_siX�� m_siY�� ������ �Ŀ� 
		// ȣ���� �־�� �Ѵ�.
		LoadUnloadedTile(siDirection);
	}	

	m_siRX	=	siRX;
	m_siRY	=	siRY;				

	if( !m_IsBoothMap )		return;	
	if( !reLoadBooth )		return;	

	
	// ���� �ʿ� �ִ� ���� ���� �˻�
	SMapBoothData*			lpSMapBoothData;


	for(TempCount = 0; TempCount < m_MapStoreParser.GetMapSize(); TempCount++)
	{
		if(ABS(m_siX - m_MapStoreParser.GetMapStore(TempCount)->siMapStoreMapXPosition) < (MAX_ONLINE_CHAR_MAP_XSIZE / 2))
		{
			if(ABS(m_siY - m_MapStoreParser.GetMapStore(TempCount)->siMapStoreMapYPosition) < (MAX_ONLINE_CHAR_MAP_YSIZE / 2))
			{
				lpSMapBoothData = m_MapBoothInfo.GetBoothData(m_MapStoreParser.GetMapStore(TempCount)->siMapStoreCode);
				if(lpSMapBoothData == NULL)
				{
					m_MapBoothInfo.AddBoothData(m_MapStoreParser.GetMapStore(TempCount)->siMapStoreCode
						, m_MapStoreParser.GetMapStore(TempCount)->siMapStoreMapXPosition
						, m_MapStoreParser.GetMapStore(TempCount)->siMapStoreMapYPosition
						, m_MapStoreParser.GetMapStore(TempCount)->siMapStoreMapIndex);

					// Server�� �� ���ǿ� ���� Data�� ��û
					pMyOnlineWorld->pOnlineClient->SendDisplayBoothItemMsg(m_MapStoreParser.GetMapStore(TempCount)->siMapStoreCode);
				}
			}
		}
	}

	// �Ÿ��� �ʹ� �־��� ���ǵ��� �˻��Ͽ� ����
	for(TempCount = 0; TempCount < MAX_BOOTHDATA; TempCount++)
	{
		if(m_MapBoothInfo.m_MapBoothData[TempCount].sifCreate)
		{
			if((m_MapBoothInfo.m_MapBoothData[TempCount].siMapIndex != m_MapIndex)
				|| (ABS(m_siX - m_MapBoothInfo.m_MapBoothData[TempCount].siTileX) >= (MAX_ONLINE_CHAR_MAP_XSIZE / 2))
				|| (ABS(m_siY - m_MapBoothInfo.m_MapBoothData[TempCount].siTileY) >= (MAX_ONLINE_CHAR_MAP_YSIZE / 2)))
			{
				ZeroMemory(&m_MapBoothInfo.m_MapBoothData[TempCount], sizeof(SMapBoothData));
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���� ��Ŀ���� ���� ĳ���͸� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineMap::SetCurrentFocusCharID(UI16 uiNewFocusCharID)
{
	m_uiCurrentFocusCharID	=	uiNewFocusCharID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���� ��Ŀ���� ���� ĳ���͸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	OnlineMap::GetCurrentFocusCharID()
{
	return m_uiCurrentFocusCharID;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	ȭ���� ���콺 ��ǥ�� Map�� X, Y ��ǥ�� ���´�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformMouseCoordToMapXY(SI32	siMouseX,SI32 siMouseY, SI32 &siMapX, SI32 &siMapY)
{
	SI32 siRealX, siRealY;

	// Screen Dot ��ǥ�� Real Coordinate ��ǥ�� ��ȯ�� �Ѵ�.
	TransformScreenDotXYToRealDotXY(siMouseX, siMouseY, siRealX, siRealY);

	// Real Coordinate ��ǥ�� MapX, Y�� ����Ѵ�.
	TransformRealDotXYToMapXY(siRealX, siRealY, siMapX, siMapY);


	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Screen Dot ��ǥ�� ���� �������� ��ġ�� ��ȯ�� �ؼ� Dot������ ���´�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformScreenDotXYToRealDotXY(SI32 siScreenDotX, SI32 siScreenDotY, SI32 &siRealDotX, SI32 &siRealDotY)
{
	siRealDotX = siScreenDotX 
				- ( (m_rcScreenRect.right - m_rcScreenRect.left + 1)/2 ) 
				- ( -m_siX * ONLINE_TILE_XSIZE_HALF + m_siY * ONLINE_TILE_XSIZE_HALF );
	siRealDotY = siScreenDotY  
				- ( (m_rcScreenRect.bottom- m_rcScreenRect.top  + 1)/2 )
				- ( -m_siX * ONLINE_TILE_YSIZE_HALF - m_siY * ONLINE_TILE_YSIZE_HALF );

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	����		:	���� ��ǥ�� Map�� ��ǥ�� �ٲپ��ش�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformRealDotXYToMapXY(SI32	siRealDotX, SI32 siRealDotY, SI32 &siMapX, SI32 &siMapY)
{
	// ���콺�� ��ǥ�� ���� ����� �߽��� ��ǥ�� ���Ѵ�. 
	siMapX  =	( siRealDotX/2 + siRealDotY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;
	siMapY	=	(-siRealDotX/2 + siRealDotY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;

	// Map�� ��踦 ��������� �˻��Ѵ�.
	if(siMapX <  0)         siMapX = 0;
	if(siMapY <  0)         siMapY = 0;
	if(siMapX >= m_siXsize) siMapX = m_siXsize - 1;
	if(siMapY >= m_siYsize) siMapY = m_siYsize - 1;
	return TRUE;
}

//--------------------------------------------------------------------------
//	����	:	���� �������� ��ġ�� Screen Dot ��ǥ�� ��ȯ�� �Ѵ�.
//--------------------------------------------------------------------------
BOOL	OnlineMap::TransformRealDotXYToScreenDotXY(SI32 siRealDotX, SI32 siRealDotY, SI32 &siScreenDotX, SI32 &siScreenDotY)
{
	siScreenDotX = siRealDotX 
				+ (  (m_rcScreenRect.right - m_rcScreenRect.left + 1)/2 ) 
				+ ( -m_siX * ONLINE_TILE_XSIZE_HALF + m_siY * ONLINE_TILE_XSIZE_HALF );
	siScreenDotY = siRealDotY  
				+ (  (m_rcScreenRect.bottom- m_rcScreenRect.top  + 1)/2 )
				+ ( -m_siX * ONLINE_TILE_YSIZE_HALF - m_siY * ONLINE_TILE_YSIZE_HALF );

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: TransformMapXYToRealDotCenterXY()
// Desc: Map[y][x]�� �߽���ġ�� �������� ��ġ�� ��ȯ�� �ؼ� Dot������ ���´�.
//
//                 |------(0,0)------|
//                 |      /   \      |
//--------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToRealDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siRealDotCX, SI32 &siRealDotCY)
{
	if(IsInMapArea(siMapX, siMapY) == FALSE)	return FALSE;

	// Ÿ���� �߽����� ���Ѵ�.
	siRealDotCX = siMapX * ONLINE_TILE_XSIZE_HALF 
			    - siMapY * ONLINE_TILE_XSIZE_HALF;
	
	siRealDotCY = siMapX * ONLINE_TILE_YSIZE_HALF 
		  	    + siMapY * ONLINE_TILE_YSIZE_HALF ;

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: TransformMapXYToScreenDotCenterXY()
// Desc: Map[y][x]�� �߽���ġ�� �������� ��ġ�� ��ȯ�� �ؼ� ȭ���� �������� Dot������ ���´�.
//--------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToScreenDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siScreenDotCX, SI32 &siScreenDotCY)
{
	if(IsInScreenMapArea(siMapX, siMapY) == FALSE)	return FALSE;

	// Ÿ���� �߽����� ���Ѵ�.
	SI32 siCountX = siMapX - m_siX;
	SI32 siCountY = siMapY - m_siY;

	siScreenDotCX = 
				 siCountX * ONLINE_TILE_XSIZE_HALF
				-siCountY * ONLINE_TILE_XSIZE_HALF
				+(m_rcScreenRect.right - m_rcScreenRect.left + 1)/2;
			
	siScreenDotCY = 
				 siCountX * ONLINE_TILE_YSIZE_HALF 
				+siCountY * ONLINE_TILE_YSIZE_HALF 
				+(m_rcScreenRect.bottom - m_rcScreenRect.top + 1)/2;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ÿ�� ��ǥ�� ĳ���͸��� ����ǥ�� ��ȯ�Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToCharMapXY(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY)
{
	if(IsInCharMapArea(siMapX, siMapY) == FALSE) return FALSE;
	
	siCharMapX	=	siMapX - (m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE);
	siCharMapY	=	siMapY - (m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE);

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ÿ�� ��ǥ�� ĳ���͸��� ����ǥ�� ��ȯ�Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToCharMapXYNonClipping(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY)
{
	siCharMapX	=	siMapX - (m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE);
	siCharMapY	=	siMapY - (m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE);

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ��ǥ�� ���� �ʻ��� ��Ʈ ��ǥ�� ��ȯ�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineMap::TransformMapXYToRealXY(SI32 siMapX, SI32 siMapY, SI32 &siRealX, SI32 &siRealY)
{
	if(IsInMapArea(siMapX, siMapY) == FALSE) return FALSE;

	float		TempFloat;

	TempFloat = (float)m_siYsize / (float)(m_siXsize + m_siYsize);
	siRealX	  =	(SI32)((float)((m_siXsize * ONLINE_TILE_XSIZE_HALF) + (m_siYsize * ONLINE_TILE_XSIZE_HALF)) * TempFloat) + (siMapX * ONLINE_TILE_XSIZE_HALF) - (siMapY * ONLINE_TILE_XSIZE_HALF);
	siRealY	  =	(siMapX * ONLINE_TILE_YSIZE_HALF) + (siMapY * ONLINE_TILE_YSIZE_HALF) + ONLINE_TILE_YSIZE_HALF;

	return TRUE;

	
	
	
	/*
	if(IsInMapArea(siMapX, siMapY) == FALSE) return FALSE;

	siRealX		=	((m_siXsize * ONLINE_TILE_XSIZE) / 2) + (siMapX * ONLINE_TILE_XSIZE_HALF) - (siMapY * ONLINE_TILE_XSIZE_HALF);
	siRealY		=	(siMapX * ONLINE_TILE_YSIZE_HALF) + (siMapY * ONLINE_TILE_YSIZE_HALF) + ONLINE_TILE_YSIZE_HALF;

	return TRUE;
	*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �ʻ��� ��Ʈ ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineMap::TransformRealXYToMapXY(SI32 siRealX, SI32 siRealY, SI32 &siMapX, SI32 &siMapY)
{
	float		TempFloat;

	// ���콺�� ��ǥ�� ���� ����� �߽��� ��ǥ�� ���Ѵ�. 
	TempFloat =  (float)m_siYsize / (float)(m_siXsize + m_siYsize);
	siRealX   -= (SI32)((float)((m_siXsize * ONLINE_TILE_XSIZE_HALF) + (m_siYsize * ONLINE_TILE_XSIZE_HALF)) * TempFloat);
	siMapX    =	 ( siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;
	siMapY	  =  (-siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;

	// Map�� ��踦 ��������� �˻��Ѵ�.
	if(siMapX <  0)         siMapX = 0;
	if(siMapY <  0)         siMapY = 0;
	if(siMapX >= m_siXsize) siMapX = m_siXsize - 1;
	if(siMapY >= m_siYsize) siMapY = m_siYsize - 1;
	return TRUE;

	
	
	
	/*
	// ���콺�� ��ǥ�� ���� ����� �߽��� ��ǥ�� ���Ѵ�. 
	siRealX -=  (((m_siXsize * ONLINE_TILE_XSIZE) / 2) + (m_siXsize * ONLINE_TILE_XSIZE_HALF)) / 2;
	siMapX  =	( siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;
	siMapY	=	(-siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;

	// Map�� ��踦 ��������� �˻��Ѵ�.
	if(siMapX <  0)         siMapX = 0;
	if(siMapY <  0)         siMapY = 0;
	if(siMapX >= m_siXsize) siMapX = m_siXsize - 1;
	if(siMapY >= m_siYsize) siMapY = m_siYsize - 1;
	return TRUE;
	*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���� ũ�⸦ ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineMap::GetMapRealSize(SI32 &siRealX, SI32 &siRealY)
{
	siRealX = (m_siXsize * ONLINE_TILE_XSIZE_HALF) + (m_siYsize * ONLINE_TILE_XSIZE_HALF);
	siRealY = (m_siXsize * ONLINE_TILE_YSIZE_HALF) + (m_siYsize * ONLINE_TILE_YSIZE_HALF);

	return TRUE;
}
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ش� Map X,Y�� ĳ���� Map�� ũ�⸦ ����� �ʴ��� �˻��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsInCharMapArea(SI32 siX, SI32 siY)
{
	if((siX < max(0, m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE)) || (siY < max(0, m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE)) ||
		(siX > min(m_siXsize - 1, m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE)) || (siY > min(m_siYsize - 1, m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE)))
		return FALSE;

	return TRUE;
}
*/
//--------------------------------------------------------------------------
// Name: IsInScreenMapArea()
// Desc: �ش� Map X,Y�� ȭ�鿡 �����ְ� �ִ� Map�� ũ�⸦ ����� �ʴ��� �˻��Ѵ�.
//--------------------------------------------------------------------------
BOOL	OnlineMap::IsInScreenMapArea(SI32 siX, SI32 siY)
{
	if((m_siX - ON_MAP_VIEW_XSIZE_HALF) > siX) return FALSE;
	if((m_siY - ON_MAP_VIEW_YSIZE_HALF) > siY) return FALSE;
	if((m_siX + ON_MAP_VIEW_XSIZE_HALF) < siX) return FALSE;
	if((m_siY + ON_MAP_VIEW_YSIZE_HALF) < siY) return FALSE;
		

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ش� ��ũ������ X, Y�� ���� ����ϴ� �簢�� ������ ����� �ʴ��� �˻��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsInScreenRect(SI32 siX, SI32 siY)
{
	if(siX >= m_rcScreenRect.left && siY >= m_rcScreenRect.top && siX <= m_rcScreenRect.right && siY <= m_rcScreenRect.bottom)
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
// ����	:	�浹 Ÿ������ �˾ƿ´�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsColl(SI32 siX, SI32 siY)
{		
	SI32	siCharTileX, siCharTileY;

	// dauduk (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
	//	���� �浹 Ÿ�Կ� ���� ��� �κп��� �ش� �� ���� ���� �浹üũ�� ������ ���� ������� Ŀ�� ���
	//	�＼�� ���̿÷��̼��� �߻��Ѵ�. ���� �󵵼��� ���� ������.
	//	������ ���� �԰��� üũ�Ͽ� ����� �˻��ϰ�, �� ���� �ʰ��ϴ� ���������� ������ �浹���¶�°����� �����ϰԲ� �����ߴ�.
	//	���� ���� �����ÿ� �� �κп��� ���װ� �ȳ����ų� ��ġ�� ���� Ư���� ������ ���� ��� �� �ּ��� �����ص� ����.
	if(m_siXsize <= 0 )					return TRUE;
	if(m_siYsize <= 0 )					return TRUE;
	if(siX < 0 && siX > m_siXsize)		return TRUE;
	if(siY < 0 && siY > m_siYsize)		return TRUE;

	if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
	{	
		return OnlineBaseMap::IsColl(m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siAtb);
	}

	return TRUE;	
}

//----------------------------------------------------------------------------------------------------------------
// ����	:	�浹 Ÿ������ �˾ƿ´�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb)
{		
	SI32	siCharTileX, siCharTileY;

	// dauduk (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
	//	���� �浹 Ÿ�Կ� ���� ��� �κп��� �ش� �� ���� ���� �浹üũ�� ������ ���� ������� Ŀ�� ���
	//	�＼�� ���̿÷��̼��� �߻��Ѵ�. ���� �󵵼��� ���� ������.
	//	������ ���� �԰��� üũ�Ͽ� ����� �˻��ϰ�, �� ���� �ʰ��ϴ� ���������� ������ �浹���¶�°����� �����ϰԲ� �Ѵ�.
	//	���� ���� �����ÿ� �� �κп��� ���װ� �ȳ����ų� ��ġ�� ���� Ư���� ������ ���� ��� �� �ּ��� �����ص� ����.


	if(m_siXsize <= 0 )					return TRUE;
	if(m_siYsize <= 0 )					return TRUE;
	if(siX < 0 && siX > m_siXsize)		return TRUE;
	if(siY < 0 && siY > m_siYsize)		return TRUE;
	
	if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
	{	
		return OnlineBaseMap::IsColl(m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siAtb, 
									 m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siKind,
									 siCharMoveableTileAtb);
	}

	return TRUE;	
}

//----------------------------------------------------------------------------------------------------------------
// ����	:	�浹 Type�� ���´�.(0 : �̵��Ұ�, 1 : ĳ���Ͱ� ���� ����, 2 : �̵� ����)
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineMap::CollCheckType(SI32 siX, SI32 siY)
{
	SI32	siCharTileX, siCharTileY;
	
	if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
	{	
		SI32		Index;

		Index = uiMultiplyCharMapYOff[siCharTileY] + siCharTileX;
		if(m_pstMap[Index].siBackupAtb & ON_TILE_ATB_COLL)
		{
			return 0;
		}

		if(m_pstMap[Index].siCharID & ON_TILE_ATB_COLL)
		{
			return 1;
		}

		return 2;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�̵� ��Ų��.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::Move(SI32 siDirect, SI32 siUnit)
{
	if(siDirect & ON_EAST)
		m_siX	=	min(m_siX + siUnit, m_siXsize - 1);

	if(siDirect & ON_WEST)
		m_siX	=	max(m_siX - siUnit, 0);		

	if(siDirect & ON_NORTH)
		m_siY	=	max(m_siY - siUnit, 0);

	if(siDirect & ON_SOUTH)
		m_siY	=	min(m_siY + siUnit, m_siYsize - 1);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� �����Ѵ�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::Save(CHAR *pszFileName)
{
	/*
	FILE			*fp;
	BOOL			bRet  = TRUE;
	LoadSaveMap		*pLoadSaveMap = NULL;	
	SI32			x, y;
	
	pLoadSaveMap	=	new	LoadSaveMap;
	ZeroMemory(pLoadSaveMap, sizeof(LoadSaveMap));

	// ����� �غ��Ѵ�.
	PrepareHeader(&pLoadSaveMap->Header);	

	// Ÿ�� ����� �Ҵ� �޴´�.
	pLoadSaveMap->pTI	=	new	LoadSaveMapTI[pLoadSaveMap->Header.siXsize * pLoadSaveMap->Header.siYsize];	
	
	if(pLoadSaveMap->pTI == NULL)
	{
		bRet = FALSE;
		goto END;
	}
	
	// Ÿ���� ���� ������ �����Ѵ�.
	for(y = 0; y < pLoadSaveMap->Header.siYsize; y++)
	{
		for(x = 0; x < pLoadSaveMap->Header.siXsize; x++)
		{
			pLoadSaveMap->pTI[y * pLoadSaveMap->Header.siXsize + x].siFile	=	m_pstMap[y * pLoadSaveMap->Header.siXsize + x].siFile;
			pLoadSaveMap->pTI[y * pLoadSaveMap->Header.siXsize + x].siFont	=	m_pstMap[y * pLoadSaveMap->Header.siXsize + x].siFont;
		}
	}

	if(fp = fopen(pszFileName, "wb"))
	{
		// ����� ȭ�Ͽ� ����.
		if(fwrite(&pLoadSaveMap->Header, sizeof(LoadSaveMapHeader), 1, fp) != 1)
		{
			bRet = FALSE;
			goto END;
		}

		// �����͸� ȭ�Ͽ� ����.		
		if(SI32(fwrite(pLoadSaveMap->pTI, sizeof(LoadSaveMapTI), pLoadSaveMap->Header.siXsize * pLoadSaveMap->Header.siYsize, fp)) != 
			pLoadSaveMap->Header.siXsize * pLoadSaveMap->Header.siYsize)
		{
			bRet = FALSE;
			goto END;
		}
	}

END:
	if(pLoadSaveMap)
	{
		if(pLoadSaveMap->pTI)
		{
			delete [] pLoadSaveMap->pTI;
			pLoadSaveMap->pTI = NULL;
		}

		delete pLoadSaveMap;
		pLoadSaveMap = NULL;
	}

	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}


	return bRet;
	*/
	return TRUE;
}

 
//---------------------------------------------------------	-------------------------------------------------------
//	����	:	���� �ε��Ѵ�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::Load(SI16 MapIndex, cltOnlineWorld *pOnlineWorld)
{
	BOOL			bRet  = TRUE;
	LoadSaveMap		LoadSaveMap;		
	MapIndexHeader*	lpMapIndexHeader;
	
	lpMapIndexHeader = MapIndexParser.GetMapData(MapIndex);
	
	if(fpMap)
		fclose(fpMap);
	
	if((fpMap = fopen(lpMapIndexHeader->szMapFileName, "rb")) != NULL)
	{
		// ȭ�� ���� ���� �����ϴ� ��ġ�� ���´�.
		fseek(fpMap, sizeof(LoadSaveMapHeader), SEEK_SET);
		fgetpos(fpMap, &fposMap);			
		
		// �ٽ� 0���� �����ش�.
		fseek(fpMap, 0, SEEK_SET);
		
		// ����� �о�´�.
		if(fread(&LoadSaveMap.Header, sizeof(LoadSaveMapHeader), 1, fpMap) != 1)
		{
			bRet = FALSE;
			goto END;
		}			
		
		// ���� �ٽ� �ʱ�ȭ �Ѵ�.
		// ���⿡�� ��ü �ʿ� ���� �޸𸮸� �Ҵ� ������ �ʴ´�.		
		if(Init(MapIndex, LoadSaveMap.Header.siXsize, LoadSaveMap.Header.siYsize, NULL, pOnlineWorld) == FALSE)
		{
			bRet= FALSE;
			goto END;
		}						
		
		// Mini Map ó��
		if(pMyOnlineWorld->pOnlineMiniMap)
		{
			// actdoll (2004/07/09 11:09) : ���� ���ʰ� ���õ� �����Ͱ� ���� ��� �߱��� �̴ϸ��� ���� �����Ѵ�.
			char	pszTemp[256];
			strcpy( pszTemp, lpMapIndexHeader->szMiniMapFileName );
			if( stricmp( pszTemp, "Online\\Map\\Minimap.Spr" ) == 0 )
			{
				if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_CHINA )		strcpy( pszTemp, "Online\\Map\\Minimap_China.Spr" );
			}
			pMyOnlineWorld->pOnlineMiniMap->LoadMiniMap( pszTemp, MapIndex );
		}
		
		m_MapStoreParser.ReadText("Online\\Map\\MapStore.txt");
		m_MapStoreParser.SetMapIndex(MapIndex);
		
		m_NPCParser.ReadText("Online\\Map\\MapNPC.txt");
		m_NPCParser.SetMapIndex(MapIndex);
		
		m_MapBoothInfo.Clear();
		
		// ���� ó��
		if(pMyOnlineWorld->pOnlineFieldEffect)
		{
			if(lpMapIndexHeader->siWeather & MAPINDEXPARSERWEATHER_CLOUD) pMyOnlineWorld->pOnlineFieldEffect->SetCloud(TRUE);
			else                                                          pMyOnlineWorld->pOnlineFieldEffect->SetCloud(FALSE);
			pMyOnlineWorld->pOnlineFieldEffect->ChangeMap(MapIndex);
		}

//		PlayFitMusic();
		
		// �����ߴ�.
		strcpy(szFileName, lpMapIndexHeader->szMapFileName);
		
END:	
		
		return bRet;
	}
}


VOID OnlineMap::PlayFitMusic(VOID)
{
	if(g_pSoundManager)
	{
		if(m_MapIndex == 0)
		{
			pMyOnlineWorld->SetBackGroundMusic(pMyOnlineWorld->CheckHeroZone(), TRUE);
		}
		else
		{
			MapIndexHeader*	lpMapIndexHeader;
			char		TempBuffer[256];

#ifdef _OGG_PLAY
			lpMapIndexHeader = MapIndexParser.GetMapData(m_MapIndex);
			if(strcmp((char*)lpMapIndexHeader->szBGMFileName, "0") == 0)
			{
				sprintf((char*)TempBuffer, "music\\Dn_Nunonchon_Kyungmae.GBM");
			}
			else
			{
				sprintf((char*)TempBuffer, "music\\%s", lpMapIndexHeader->szBGMFileName);
			}
			GetBgmMgr()->SetNextBgm(TempBuffer);
			GetBgmMgr()->Play(TempBuffer, TRUE);
#else
			if(pMyOnlineWorld->GetSoundID() != 0)
			{
				g_pSoundManager->StopSound(pMyOnlineWorld->GetSoundID());	
				g_pSoundManager->ReleaseSound(pMyOnlineWorld->GetSoundID());
			}

			lpMapIndexHeader = MapIndexParser.GetMapData(m_MapIndex);
			if(strcmp((char*)lpMapIndexHeader->szBGMFileName, "0") == 0)
			{
				sprintf((char*)TempBuffer, "music\\Music1.YAV");
			}
			else
			{
				sprintf((char*)TempBuffer, "music\\%s", lpMapIndexHeader->szBGMFileName);
			}
			pMyOnlineWorld->BackgroundMusic = g_pSoundManager->AddPlayID((char*)TempBuffer, FALSE, TRUE, FALSE);
			g_pSoundManager->PlaySound(pMyOnlineWorld->BackgroundMusic);
#endif
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	����� �����͸� �ִ´�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::PrepareHeader(LoadSaveMapHeader *pHeader)
{
	if(pHeader)
	{
		pHeader->rVersion	=	ONLINE_MAP_VERSION;
		pHeader->siXsize	=	m_siXsize;
		pHeader->siYsize	=	m_siYsize;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Ÿ���� �����Ѵ�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetTile(SI16 siTileX, SI16 siTileY, SI16 siFileIndex, SI16 siFontIndex, UI32 siAtb, UI16 uiRoadFileIndex, UI16 uiRoadFontIndex)
{
	XSPR	*pTileXspr, *pRoadXspr;
	SI32	siCharTileX, siCharTileY;
	strTI	*pstTI;

	// Char Map Area ������ ��ġ�� ���Ѵ�.
	if(m_pstMap)
	{
		if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
		{	

			pstTI	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];
			
			pstTI->siFile		=	siFileIndex;						
			pstTI->siFont		=	GETFONTINDEX(siFontIndex);						// �� ���� 1 ��Ʈ�� �浹 ��Ʈ������ ���θ� ������ �ִ�.
			pstTI->uiTileAtb    =   siAtb;
			pstTI->uiRoadFile	=	uiRoadFileIndex;
			pstTI->uiRoadFont	=	uiRoadFontIndex;
			pstTI->siFieldGuideTable = 0;

			// ObjectID�� ���߿� ������ ���� �Ѵ�.
			pstTI->uiObjectID			=	0;
			pstTI->bUnknownObjectID		=	FALSE;

			if(siFileIndex != -1 && siFontIndex != -1)
			{			
				pTileXspr				=	pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->siFile);

				/*
				pstTI->siAtb   = pTileXspr->Header.Coll[pstTI->siFont] & 0xffff;
				pstTI->siSpeed = pTileXspr->Header.Coll[pstTI->siFont] & 0xffff0000;
				if(pstTI->siAtb != ON_TILE_ATB_COLL)
				{
					pstTI->siAtb   = 0;
					pstTI->siSpeed = pTileXspr->Header.Coll[pstTI->siFont];
				}
				*/
				pstTI->siAtb       = 0;
				pstTI->siBackupAtb = 0;
				pstTI->siSpeed     = pTileXspr->Header.Coll[pstTI->siFont];
			}			

			if(pstTI->uiTileAtb & 0x8)
			{
				pstTI->siAtb       |= ON_TILE_ATB_COLL;
				pstTI->siBackupAtb |= ON_TILE_ATB_COLL;
			}

			if(pstTI->uiTileAtb & 0x1)
			{
				pstTI->siKind	=	ON_TILE_KIND_GROUND;
			}

			if(pstTI->uiTileAtb & 0x2)
			{
				pstTI->siKind	=	ON_TILE_KIND_SEA;
			}

			

			if(uiRoadFileIndex != 0)
			{
				// ȭ���� �ε����� �ִ�.
				// �� ���̴�.
				pRoadXspr         = pMyOnlineWorld->pOnlineResource->GetxsprTile(pstTI->uiRoadFile);
				pstTI->uiRoadKind = pRoadXspr->Header.Coll[pstTI->uiRoadFont];
			}
			else
			{
				pstTI->uiRoadKind		=	0;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���ο� ���� �ε��ϰ� �� �������� �̵�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::ChangeMap(SI16 MapIndex, SI16 siX, SI16 siY)
{
	// Object �ʱ�ȭ
	pMyOnlineWorld->pIOnlineObject->FreeAll();

	// Map Change
	Load(MapIndex, pMyOnlineWorld);

	// Path Find �κ�
	pMyOnlineWorld->pclFindPath->SetFindPathSize(m_siXsize, m_siYsize);

	// Mini Map ó��
	if(pMyOnlineWorld->pOnlineMiniMap)
	{
		pMyOnlineWorld->pOnlineMiniMap->Warp(siX, siY);
	}

	// Map Position Change
	SetXY(siX, siY);

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� ���� X Y ��ġ�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetXY(SI16 siX, SI16 siY)
{
	// PreProcessorBeforeLoadUnloadedTile�� ���ο� ��ġ�� ���ϱ� ���� ���־�� �� �۾����� ���ִ� ���̴�.
	// ������Ʈ ������ ���⼭ ���־�� �Ѵ�.		
	PreProcessorBeforeLoadUnloadedTile(0);

	m_siX	=	siX;
	m_siY	=	siY;

	// ��ü�� ��� ������Ʈ�Ѵ�.
	LoadUnloadedTile(0);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� ���� X ��ġ�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetX(SI16 siX)
{
	m_siX	=	siX;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� ���� X ��ġ�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetY(SI16 siY)
{
	m_siY	=	siY;
}
/*
//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ���� X ������ ���´�.	
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineMap::GetRX()
{
	return m_siRX;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ���� Y ������ ���´�.	
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineMap::GetRY()
{
	return m_siRY;
}
*/
//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� ĳ������ ID �� ������Ʈ�� ID�� �����Ѵ�.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetID(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI16 siCharID, UI16 uiObjectID, UI32 uiItemID, UI16 uiItemQuantity)
{
	SI16	siX, siY;
	SI32	siCharTileX, siCharTileY;	
	strTI	*pstTI;	

	for(siY = siTileY; siY < (siTileY + siYTileNum); siY++)
	{
		for(siX = siTileX; siX < (siTileX + siXTileNum); siX++)
		{
			if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
			{	
				pstTI	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];			

				// siCharID�� siObjectID�� �迭�� �ε��� ������		
				// siItemID�� ���� �������� ID�̴�.
				if(siCharID)
				{
					if(pstTI->siCharID != 0)
						clGrp.Error("", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ID_OVERLAP));

					pstTI->siCharID			=	siCharID;				
				}
				if(uiObjectID)		pstTI->uiObjectID		=	uiObjectID;									
				if(uiItemID)
				{
					pstTI->uiItemID			=	uiItemID;
					pstTI->uiItemQuantity	=	uiItemQuantity;			
				}
			}
		}
	}	

	// ĳ���ͳ� ������Ʈ�� ��쿡�� �浹�� �������ش�.
	if(siCharID != 0)
	{
		SetTileAtb(siTileX, siTileY, siXTileNum, siYTileNum, ON_TILE_ATB_COLL, TRUE);
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Ư�� ��ġ�� 1x1 ������Ʈ�� �ɴ´�. �ӵ��� ���ؼ� ����ϴ� �Լ��̴�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineMap::SetObjectID(SI16 siTileX, SI16 siTileY, UI16 uiObjectID)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiObjectID =  uiObjectID;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Ư�� ��ġ�� ������Ʈ�� �ɴ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineMap::SetObjectID2(SI16 siTileX, SI16 siTileY, UI16 uiObjectID)
{
	SI32		siCharTileX, siCharTileY;
//	SI32		TempXCount, TempYCount;
	SI16		Width, Height;
	SI16		SubWidth, SubHeight;
	SI32		Kind;
	BOOL		ObjAtt;

	ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(ON_MAP_GETOBJECTID(uiObjectID), ATB_OBJECT_VILLAGE);
	pMyOnlineWorld->pIOnlineObject->GetSize(ON_MAP_GETOBJECTID(uiObjectID), &Width, &Height);
	if((Width == 0) || (Height == 0)) return;

	SubWidth  = (Width  / 2) - 1 + (Width  % 2);
	SubHeight = (Height / 2) - 1 + (Height % 2);
	Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(ON_MAP_GETOBJECTID(uiObjectID)));
	ObjAtt = ((Kind & ATB_OBJECT_ISOBJECT) || (ObjAtt));

	/*
	for(TempYCount = 0; TempYCount < Height; TempYCount++)
	{
		for(TempXCount = 0; TempXCount < Width; TempXCount++)
		{
			if(TransformMapXYToCharMapXY((siTileX + TempXCount) - SubWidth, (siTileY + TempYCount) - SubHeight, siCharTileX, siCharTileY) == TRUE)
			{
				// ID�� �̵������� �������� �˻�
				if(ObjAtt)
				{
					m_pstMap[(siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX].siAtb |= ON_TILE_ATB_COLL;
				}
				else
				{
					m_pstMap[(siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX].siAtb &= ON_TILE_RATB_COLL;
				}
			}
		}
	}
	*/

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiObjectID =  uiObjectID;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ư�� ��ġ���� ĳ���� �� ������Ʈ�� ����.(������Ʈ�� ���� ��쿡�� ������Ʈ �迭������ �����Ѵ�.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::DelID(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI32 siDelInfo)
{
	SI32	i, j;
	SI32	siCharTileX, siCharTileY;
	strTI	*pstTI;

	
	for(i = siTileY; i < (siTileY + siYTileNum); i++)
	{
		for(j = siTileX; j < (siTileX + siXTileNum); j++)
		{
			if(TransformMapXYToCharMapXY(j, i, siCharTileX, siCharTileY) == TRUE)
			{
				pstTI	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];

				if(siDelInfo & ON_MAP_DELID_CHAR)									
				{
					pstTI->siCharID			=	0;					
				}

				if(siDelInfo & ON_MAP_DELID_OBJECT)
				{					
					// �� Ÿ�Ͽ� �ִ� ������Ʈ ID�� ȭ���� ��µ� ���̴� ID��� 
					// ������Ʈ �迭���� �ش� ������Ʈ�� �����Ѵ�.
					if(ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID) == TRUE)
					{
						pMyOnlineWorld->pIOnlineObject->Free(ON_MAP_GETOBJECTID(pstTI->uiObjectID));						
					}

					pstTI->uiObjectID			=	0;				
					
					pstTI->bUnknownObjectID		=	FALSE;
					pstTI->siSourceObjectX		=	0;
					pstTI->siSourceObjectY		=	0;
				}

				if(siDelInfo & ON_MAP_DELID_ITEM)
				{
					pstTI->uiItemID			=	0;
				}
			}
		}
	}

	if(siDelInfo & ON_MAP_DELID_CHAR)
	{
		SetTileAtb(siTileX, siTileY, siXTileNum, siYTileNum, ON_TILE_ATB_COLL, FALSE);		
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ư�� ��ġ���� ������ data�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::EditItem(SI16 siTileX, SI16 siTileY, UI16 Qut)
{
	SI32	siCharTileX, siCharTileY;
	strTI	*pstTI;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		pstTI	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];

		pstTI->uiItemQuantity = Qut;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Ư�� Ÿ�Ͽ� Ư�� �Ӽ��� �����ϰų� ����.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetTileAtb(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI32 siAtb, BOOL bSet)
{
	SI16	siX, siY;	
	SI32	siCharTileX, siCharTileY;

	if(bSet) 
	{
		for(siY = siTileY; siY < (siYTileNum + siTileY); siY++)
		{
			for(siX = siTileX; siX < (siXTileNum + siTileX); siX++)
			{
				if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
				{				
					m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siAtb	|=		siAtb;							
				}			
			}
		}
	}
	else
	{
		for(siY = siTileY; siY < (siYTileNum + siTileY); siY++)
		{
			for(siX = siTileX; siX < (siXTileNum + siTileX); siX++)
			{
				if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
				{				
					m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siAtb	&=		(~siAtb);
				}			
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� �����Ǿ� �ִ� ������Ʈ�� ID�� ���´�.
//----------------------------------------------------------------------------------------------------------------
SI32	OnlineMap::GetObjectID(SI16 siTileX, SI16 siTileY)
{
	SI32			siCharTileX, siCharTileY;
	strTI*			pstTI;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)		
	{		
		pstTI	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];

		if(pstTI->bUnknownObjectID == TRUE)
		{
			if(TransformMapXYToCharMapXY(pstTI->siSourceObjectX, pstTI->siSourceObjectY, siCharTileX, siCharTileY) == TRUE)		
			{
				return ON_MAP_GETOBJECTID(m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiObjectID);		
			}
			else
			{
				return 0;
			}
		}			
		else
		{
			return ON_MAP_GETOBJECTID(m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiObjectID);		
		}
	}
	else
	{
		return 0;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� �����Ǿ� �ִ� ĳ������ ID�� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	OnlineMap::GetCharID(SI16 siTileX, SI16 siTileY)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)		
	{
		return m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siCharID;
	}
	else
	{
		return 0;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� �����Ǿ� �ִ� �������� ID�� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	OnlineMap::GetItemID(SI16 siTileX, SI16 siTileY)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)		
	{
		return m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiItemID;
	}
	else
	{
		return 0;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ� �����Ǿ� �ִ� �������� ������ ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	OnlineMap::GetItemQuantity(SI16 siTileX, SI16 siTileY)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)		
	{
		return m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiItemQuantity;
	}
	else
	{
		return 0;
	}
}
//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ����� ���´�(�������̽� ���ָ鼭 �߰��Ǵ� �Լ�)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::GetSize(SI32 *psiXsize, SI32 *psiYsize)
{
	*psiXsize	=	m_siXsize;
	*psiYsize	=	m_siYsize;	
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ���� ��ġ�� ���´�(�������̽� ���ָ鼭 �߰��Ǵ� �Լ�)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::GetCurrentPos(SI32 *psiX, SI32 *psiY)
{
	*psiX	=	m_siX;				
	*psiY	=	m_siY;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�簢���� ��� ������ ���´�(�������̽� ���ָ鼭 �߰��Ǵ� �Լ�)
//----------------------------------------------------------------------------------------------------------------
RECT	OnlineMap::GetScreenRect()
{
	return m_rcScreenRect;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	m_siX�� m_siY�� �����ϰ� LoadUnloadedTile�� ȣ���Ϸ��� �Ҷ� m_siX�� m_siY�� �����ϱ� ���� ȣ���� �־���Ѵ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineMap::PreProcessorBeforeLoadUnloadedTile(SI32 siDirection)
{	
	SI32	siTileX;
	SI32	siTileY;

	if(siDirection == 0)
	{
		// ������ ���� �Ǿ� �ִ� ��� ������Ʈ�� �����Ѵ�.		
		DelID(m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE, m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE, MAX_ONLINE_CHAR_MAP_XSIZE, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_OBJECT);
	}
	else
	{
		// ���ݸ� ���������� ������ �ִ� �κ��� �׳� �����Ѵ�.
		// ������ ����Ÿ�� �����δ�.
		if(ON_EAST & siDirection)
		{			
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			DelID(siTileX, siTileY, 1, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_OBJECT);			
		}
		else if(ON_WEST & siDirection)
		{
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			DelID(siTileX, siTileY, 1, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_OBJECT);		
		}

		if(ON_NORTH & siDirection)
		{			
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE;		

			DelID(siTileX, siTileY, MAX_ONLINE_CHAR_MAP_XSIZE, 1, ON_MAP_DELID_OBJECT);
		}
		else if(ON_SOUTH & siDirection)
		{
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;			

			DelID(siTileX, siTileY, MAX_ONLINE_CHAR_MAP_XSIZE, 1, ON_MAP_DELID_OBJECT);
		}		
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ε�Ǿ����� ���� Ÿ���� �ε��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::LoadUnloadedTile(SI32 siDirection)
{	
	SI32					j, i;
	SI32					siTileX, siTileY;
	SI32					siCharTileX, siCharTileY;		
	SI32					siCharTileX1, siCharTileY1;		
	LoadSaveMap				*pLoadSaveMap = NULL;		
	BOOL					bRet = TRUE;	
	LoadSaveMapTI			TileInfo;			
	SI32					siCornerTile = 0;

	if(siDirection == 0)
	{	
		// ������ ���� �Ǿ� �ִ� ��� ID�� �����Ѵ�.
		// ������Ʈ�� �������� �ʴ´�.
		DelID(m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE, m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE, MAX_ONLINE_CHAR_MAP_XSIZE, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_CHAR | ON_MAP_DELID_ITEM);

		for(siCharTileY = 0, i = -MAX_ONLINE_CHAR_MAP_HALF_YSIZE; i <= MAX_ONLINE_CHAR_MAP_HALF_YSIZE; i++, siCharTileY++)
		{
			siTileY		=	m_siY + i;		

			for(siCharTileX = 0, j = -MAX_ONLINE_CHAR_MAP_HALF_XSIZE; j <= MAX_ONLINE_CHAR_MAP_HALF_XSIZE; j++, siCharTileX++)
			{	
				siTileX		=	m_siX + j;

				if(IsInMapArea(siTileX, siTileY) == TRUE)
				{
					if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX1, siCharTileY1) == TRUE)
					{	
						m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siAtb       = 0;
						m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siBackupAtb = 0;
					}
				}
			}
		}
		for(siCharTileY = 0, i = -MAX_ONLINE_CHAR_MAP_HALF_YSIZE; i <= MAX_ONLINE_CHAR_MAP_HALF_YSIZE; i++, siCharTileY++)
		{
			siTileY		=	m_siY + i;		

			for(siCharTileX = 0, j = -MAX_ONLINE_CHAR_MAP_HALF_XSIZE; j <= MAX_ONLINE_CHAR_MAP_HALF_XSIZE; j++, siCharTileX++)
			{	
				siTileX		=	m_siX + j;

				if(IsInMapArea(siTileX, siTileY) == TRUE)
				{
					//if((m_pstMap[siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE + siCharTileX].siFile == -1) && 
					//	(m_pstMap[siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE + siCharTileX].siFont == -1))
					{
						// ���� �ε���� ���� Ÿ���̴�.

						// ȭ�� �����͸� �ش� ��ǥ�� �����ͷ� �̵���Ų��.
						fsetpos(fpMap, &fposMap);
						fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);

						// �����Ѵ�.
						fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
						fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
						fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

						// Ÿ���� �����Ѵ�.						
						SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						

						AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);

					}				
				}	
			}
		}		
	}
	else
	{
		// ���ݸ� ���������� ������ �ִ� �κ��� �׳� �����Ѵ�.
		// ������ ����Ÿ�� �����δ�.
		if(ON_EAST & siDirection)
		{			
			for(i = 0; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++)										
			{
				for(j = 0; j < (MAX_ONLINE_CHAR_MAP_XSIZE - 1); j++)
				{
					memmove(&m_pstMap[uiMultiplyCharMapYOff[i] + j], &m_pstMap[uiMultiplyCharMapYOff[i] + 1 + j], sizeof(strTI));					
				}
			}

			for(i = 0; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++)
			{
				m_pstMap[uiMultiplyCharMapYOff[i] + MAX_ONLINE_CHAR_MAP_XSIZE - 1].siAtb       = 0;
				m_pstMap[uiMultiplyCharMapYOff[i] + MAX_ONLINE_CHAR_MAP_XSIZE - 1].siBackupAtb = 0;
			}
		}
		else if(ON_WEST & siDirection)
		{
			for(i = 0; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++)										
			{
				for(j = (MAX_ONLINE_CHAR_MAP_XSIZE - 2); j >= 0; j--)
				{
					memmove(&m_pstMap[uiMultiplyCharMapYOff[i] + 1 + j], &m_pstMap[uiMultiplyCharMapYOff[i] + j], sizeof(strTI));			
				}
			}

			for(i = 0; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++)
			{
				m_pstMap[uiMultiplyCharMapYOff[i]].siAtb = 0;
				m_pstMap[uiMultiplyCharMapYOff[i]].siBackupAtb = 0;
			}
		}

		if(ON_NORTH & siDirection)
		{			
			for(i = (MAX_ONLINE_CHAR_MAP_YSIZE - 2); i >= 0; i--)			
				memmove(&m_pstMap[uiMultiplyCharMapYOff[i + 1]], &m_pstMap[uiMultiplyCharMapYOff[i]], sizeof(strTI) * MAX_ONLINE_CHAR_MAP_XSIZE);								

			for(i = 0; i < MAX_ONLINE_CHAR_MAP_XSIZE; i++)
			{
				m_pstMap[uiMultiplyCharMapYOff[0] + i].siAtb       = 0;
				m_pstMap[uiMultiplyCharMapYOff[0] + i].siBackupAtb = 0;
			}
		}
		else if(ON_SOUTH & siDirection)
		{
			for(i = 1; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++)										
				memmove(&m_pstMap[uiMultiplyCharMapYOff[i - 1]], &m_pstMap[uiMultiplyCharMapYOff[i]], sizeof(strTI) * MAX_ONLINE_CHAR_MAP_XSIZE);			

			for(i = 0; i < MAX_ONLINE_CHAR_MAP_XSIZE; i++)
			{
				m_pstMap[uiMultiplyCharMapYOff[MAX_ONLINE_CHAR_MAP_YSIZE - 1] + i].siAtb       = 0;
				m_pstMap[uiMultiplyCharMapYOff[MAX_ONLINE_CHAR_MAP_YSIZE - 1] + i].siBackupAtb = 0;
			}
		}		

		// ���ο� ����Ÿ �κ��� ���´�.
		if(ON_EAST & siDirection)
		{			
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			DelID(siTileX, siTileY, 1, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_CHAR | ON_MAP_DELID_ITEM);

			for(i = 0, siTileY += 1; i < (MAX_ONLINE_CHAR_MAP_YSIZE - 1); i++, siTileY++)
			{	
				if(IsInMapArea(siTileX, siTileY) == TRUE)
				{
					fsetpos(fpMap, &fposMap);
					fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
					fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
					fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
					fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

					SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						
							
					AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);						
				}			
			}							

			// �𼭸� �κ��� �����Ѵ�.
			siCornerTile	|=	(ON_MAP_CORNER_EASTNORTH | ON_MAP_CORNER_EASTSOUTH);

			//MakeMapFile(ON_EAST);
		}
		else if(ON_WEST & siDirection)
		{
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			DelID(siTileX, siTileY, 1, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_CHAR | ON_MAP_DELID_ITEM);

			for(i = 0, siTileY += 1; i < (MAX_ONLINE_CHAR_MAP_YSIZE - 1); i++, siTileY++)
			{	
				if(IsInMapArea(siTileX, siTileY) == TRUE)
				{
					fsetpos(fpMap, &fposMap);
					fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
					fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
					fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
					fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

					SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						

					AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);										
				}			
			}				
			
			//MakeMapFile(ON_WEST);

			// �𼭸� �κ��� �����Ѵ�.
			siCornerTile	|=	(ON_MAP_CORNER_WESTNORTH | ON_MAP_CORNER_WESTSOUTH);
		}

		if(ON_NORTH & siDirection)
		{						
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;		

			DelID(siTileX, siTileY, MAX_ONLINE_CHAR_MAP_XSIZE, 1, ON_MAP_DELID_CHAR | ON_MAP_DELID_ITEM);

			for(i = 0, siTileX += 1; i < (MAX_ONLINE_CHAR_MAP_XSIZE - 1); i++, siTileX++)
			{	
				if(IsInMapArea(siTileX, siTileY) == TRUE)
				{
					fsetpos(fpMap, &fposMap);
					fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
					fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
					fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
					fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

					SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						
					AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);										
				}			
			}							
			
			//MakeMapFile(ON_NORTH);
			siCornerTile	|=	(ON_MAP_CORNER_EASTNORTH | ON_MAP_CORNER_WESTNORTH);
		}
		else if(ON_SOUTH & siDirection)
		{			
			// ��� �ִ� ĭ�� ä�� �ִ´�.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE;			

			DelID(siTileX, siTileY, MAX_ONLINE_CHAR_MAP_XSIZE, 1, ON_MAP_DELID_CHAR | ON_MAP_DELID_ITEM);

			for(i = 0, siTileX += 1; i < (MAX_ONLINE_CHAR_MAP_XSIZE - 1); i++, siTileX++)
			{	
				if(IsInMapArea(siTileX, siTileY) == TRUE)
				{
					fsetpos(fpMap, &fposMap);
					fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
					fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
					fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
					fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

					SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						

					AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);															
				}			
			}						
			
			//MakeMapFile(ON_SOUTH);
			siCornerTile	|=	(ON_MAP_CORNER_EASTSOUTH | ON_MAP_CORNER_WESTSOUTH);
		}			

		if(siCornerTile & ON_MAP_CORNER_WESTNORTH)
		{
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			if(IsInMapArea(siTileX, siTileY) == TRUE)
			{
				fsetpos(fpMap, &fposMap);
				fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
				fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
				fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
				fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

				SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						
				AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);																							
			}			
		}

		if(siCornerTile & ON_MAP_CORNER_EASTNORTH)
		{
			siTileX	=	m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			if(IsInMapArea(siTileX, siTileY) == TRUE)
			{
				fsetpos(fpMap, &fposMap);
				fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
				fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
				fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
				fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

				SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						
				AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);																			
			}			
		}

		if(siCornerTile & ON_MAP_CORNER_WESTSOUTH)
		{
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE;		

			if(IsInMapArea(siTileX, siTileY) == TRUE)
			{
				fsetpos(fpMap, &fposMap);
				fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
				fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
				fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
				fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

				SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						
				AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);																			
			}		
		}

		if(siCornerTile & ON_MAP_CORNER_EASTSOUTH)
		{
			siTileX	=	m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE;		

			if(IsInMapArea(siTileX, siTileY) == TRUE)
			{
				fsetpos(fpMap, &fposMap);
				fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);
				fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
				fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
				fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

				SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						
				AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);																			
			}			
		}
	}

	// ǥ���� ����
	SI16				TempMax;
	SI16				TempCount;
	MilepostHeader*		lpMilepostHeader;
	SI16				TempX;
	SI16				TempY;
	
	TempMax = FieldParser.GetTotalNum();
	for(TempCount = 0; TempCount < TempMax; TempCount++)
	{
		lpMilepostHeader = FieldParser.GetMilepost(TempCount);
		if(m_MapIndex == lpMilepostHeader->siNowMapIndex)
		{
			TempX = (lpMilepostHeader->uiID & 0xffff0000) >> 16;
			TempY = (lpMilepostHeader->uiID & 0x0000ffff);

			/*
			if(TransformMapXYToCharMapXY(TempX, TempY, siCharTileX1, siCharTileY1) == TRUE)
			{
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siAtb       |= ON_TILE_ATB_COLL;
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siBackupAtb |= ON_TILE_ATB_COLL;
			}

			if(TransformMapXYToCharMapXY(TempX + 1, TempY, siCharTileX1, siCharTileY1) == TRUE)
			{
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siAtb       |= ON_TILE_ATB_COLL;
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siBackupAtb |= ON_TILE_ATB_COLL;
			}

			if(TransformMapXYToCharMapXY(TempX, TempY + 1, siCharTileX1, siCharTileY1) == TRUE)
			{
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siAtb       |= ON_TILE_ATB_COLL;
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siBackupAtb |= ON_TILE_ATB_COLL;
			}
			*/

			if(TransformMapXYToCharMapXY(TempX + 1, TempY + 1, siCharTileX1, siCharTileY1) == TRUE)
			{
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siFieldGuideTable = TempCount + 1;
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siAtb       |= ON_TILE_ATB_COLL;
				//m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1].siBackupAtb |= ON_TILE_ATB_COLL;
			}
		}
	}

	// Cache Draw Tile
	DrawTileCache();

	return bRet;
}


VOID	OnlineMap::MakeMapFile(SI32 siDirection)
{
	SI32	i, j;
	CHAR	szTextBuffer[1024];
	CHAR	szTempBuffer[1024];
	SI32	siTileX, siTileY;


	for(i = 0, siTileY = m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE; i < MAX_ONLINE_CHAR_MAP_YSIZE; i++, siTileY++)			
	{
		ZeroMemory(szTextBuffer, sizeof(szTextBuffer));
		for(j = 0, siTileX = m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE; j < MAX_ONLINE_CHAR_MAP_XSIZE; j++, siTileX++)
		{				
			wsprintf(szTempBuffer, "[{%d,%d}%d] ", siTileX, siTileY, m_pstMap[uiMultiplyCharMapYOff[i] + j].uiObjectID);
			strcat(szTextBuffer, szTempBuffer);
		}		
	}			
}

//--------------------------------------------------------------------------
// Name: CreateDrawTable()
// Desc: �׸���������̺��� �����
//--------------------------------------------------------------------------
VOID	OnlineMap::CreateDrawTable(void)
{
	SI32	siCountIDX;
	SI32	siCountIDY;
	
	SI32	siIndex = 0;
	SI32	siCountDraw = 0;
	
	SI32	siHalfXSize = ON_MAP_VIEW_XSIZE_HALF;
	SI32	siHalfYSize = ON_MAP_VIEW_YSIZE_HALF;

	SI32	siCountA;//, siCountB, siCountC;
	
	siTileNo = (siHalfXSize*2+1)*(siHalfYSize*2+1);
	//--------------------------------------------------------------------------
	// �׸������ ���̺� ����
	//--------------------------------------------------------------------------
	// ���� ��
	siCountIDX = -siHalfXSize;
	for(siCountIDY=-siHalfYSize; siCountIDY<=siHalfYSize; siCountIDY++)
	{
		siCountDraw++;
		
		SI32 siCountDrawHalf = siCountDraw/2;
		BOOL siCountDrawHalfMod = siCountDraw%2;
		
		SI32 siCountRevX = siCountIDX;
		SI32 siCountRevY = siCountIDY;
		
		for(siCountA=0; siCountA<siCountDrawHalf; siCountA++)
		{
			siDrawOrder[siIndex++] = siCountRevX;
			siDrawOrder[siIndex++] = siCountRevY;
			siDrawOrder[siIndex++] = siCountRevY;
			siDrawOrder[siIndex++] = siCountRevX;
			
			siCountRevX++;
			siCountRevY--;
		}
		
		if ( siCountDrawHalfMod )
		{
			siDrawOrder[siIndex++] = siCountRevX;
			siDrawOrder[siIndex++] = siCountRevY;
		}
	}
	
	// �Ʒ��� ��
	siCountIDY = siHalfYSize;
	for(siCountIDX=-siHalfXSize+1; siCountIDX<=siHalfXSize; siCountIDX++)
	{
		siCountDraw--;
		
		SI32 siCountDrawHalf = siCountDraw/2;
		BOOL siCountDrawHalfMod = siCountDraw%2;
		
		SI32 siCountRevX = siCountIDX;
		SI32 siCountRevY = siCountIDY;
		
		for(siCountA=0; siCountA<siCountDrawHalf; siCountA++)
		{
			siDrawOrder[siIndex++] = siCountRevX;
			siDrawOrder[siIndex++] = siCountRevY;
			siDrawOrder[siIndex++] = siCountRevY;
			siDrawOrder[siIndex++] = siCountRevX;
			
			siCountRevX++;
			siCountRevY--;
		}
		
		if ( siCountDrawHalfMod )
		{
			siDrawOrder[siIndex++] = siCountRevX;
			siDrawOrder[siIndex++] = siCountRevY;
		}
	}
	//--------------------------------------------------------------------------
	// ������� Fog ���� Table ��� 
	//--------------------------------------------------------------------------
	/*
	for(siCountC = 0; siCountC  <= siHalfXSize
	siCountA, siCountB, 
	siCircleOrder[
	*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������Ʈ�� ������ �м��Ͽ� ó�����ش�.(������ ��� ��ǥ�� ��쿡�� �������� ���´�.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineMap::AnalysisObjectInfo(SI16 siTileX, SI16 siTileY, UI16 uiObjectInfo)
{	
	ObjectInfoBfs*			pObjectInfoBfs;	
	VillageHeader*			pVillageHeader;
	PortalHeader*			pPortalHeader;
	SI16					siObjectID;	
	SI32					siCharTileX, siCharTileY;
//	SI32					siCharTileX1, siCharTileY1;

	if(uiObjectInfo != 0)
	{
		pObjectInfoBfs	=	(ObjectInfoBfs*)&uiObjectInfo;		

		switch(pObjectInfoBfs->BaseBf.uiCode)
		{
		case ON_MAP_OI_CODE_KIND:				// Kind �� �ƴ� Object�϶�
			
			// ���ο� ������Ʈ�̴�.
			// ������Ʈ�� �߰��Ѵ�.			
			siObjectID	=	pMyOnlineWorld->pIOnlineObject->Init(pObjectInfoBfs->BaseBf.siParameter, siTileX, siTileY, pMyOnlineWorld);
			
			if(siObjectID != 0)
			{
				SetObjectID2(siTileX, siTileY, ON_MAP_MAKEDRAWOBJECT(siObjectID));
			}			
			break;
		case ON_MAP_OI_CODE_RELATIVEPOS:		// Kind�� �𸣴� Object�϶�
			// ��� ��ǥ�� ������ �ִ� ������Ʈ�̴�.
			// ���ۿ� �ִ´�.											

			
			if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
			{
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].bUnknownObjectID	=	TRUE;
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSourceObjectX	=	siTileX + pObjectInfoBfs->RelativePosBf.siX;
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSourceObjectY	=	siTileY + pObjectInfoBfs->RelativePosBf.siY;
			}
			break;
		case ON_MAP_OI_CODE_VILLAGE:			// �����϶�
			// �����̴�.
			// �ش� ��ġ�� ������ ���´�.
			if((pVillageHeader = pMyOnlineWorld->pOnlineVillageManager->GetVillageByPos(siTileX, siTileY, m_MapIndex)) != NULL)
			{
				siObjectID	=	pMyOnlineWorld->pIOnlineObject->Init(pVillageHeader, pMyOnlineWorld);		

				// Ÿ�Ͽ� ������Ʈ�� �����Ѵ�.(�ӵ��� ����Ͽ� �� ��ġ�� ���.)
				if(siObjectID != 0)	
				{
					SetObjectID2(siTileX, siTileY, ON_MAP_MAKEDRAWOBJECT(siObjectID));			
				}
			}
			else if((pPortalHeader = PortalParser.GetPortal(siTileX, siTileY, m_MapIndex)) != NULL)
			{
				// ������ �ƴϸ� Portal�̴�.
				siObjectID	=	pMyOnlineWorld->pIOnlineObject->Init(pPortalHeader, pMyOnlineWorld);

				// Ÿ�Ͽ� ������Ʈ�� �����Ѵ�.(�ӵ��� ����Ͽ� �� ��ġ�� ���.)
				if(siObjectID != 0)	
				{
					SetObjectID2(siTileX, siTileY, ON_MAP_MAKEDRAWOBJECT(siObjectID));			
				}
			}
			break;
		}
	}	

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ��� �߰� �̵� �ӵ��� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineMap::GetRoadSpeed(SI16 siTileX, SI16 siTileY, SI32 *psiAddMoveSpeed)
{
	SI32	siCharTileX, siCharTileY;
	UI08	uiRoadKind;
	
	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		uiRoadKind	=	m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiRoadKind;

		if(uiRoadKind != 0)
		{
			switch(uiRoadKind)
			{
				case ON_MAP_ROAD_ATB_ROAD:				
					*psiAddMoveSpeed	=	2;
					return TRUE;
				default:
					return	FALSE;
			}
		}			
		else
		{
			switch(m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSpeed & 0xffff)
			{
				case 2:			// ������
					*psiAddMoveSpeed = 2;
					return	TRUE;	

				case 3:			// ������
					*psiAddMoveSpeed = -2;
					return	TRUE;	
			}
		}
	}	

	return FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Tile�� �� �Ӽ��� ���ؿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI08 	OnlineMap::GetWaterFlag(SI16 siTileX, SI16 siTileY)
{
	SI32	siCharTileX, siCharTileY;
	
	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		return (m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSpeed & 0xff0000) >> 16;
	}	

	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Tile�� �� �Ӽ��� ���ؿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI08 	OnlineMap::GetWaterFlag(strTI* lpTileData)
{
	return (lpTileData->siSpeed & 0xff0000) >> 16;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� ����Ű�� ĳ������ ID�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineMap::GetPointTo(LPPOINT pptMouse, SI32 *psiCharID, SI32 *psiItemPos, SI32 *psiObjectID)
{	
	SI32			siPutX, siPutY;
	SI32			siBaseCharTileX, siBaseCharTileY, siCharTileX, siCharTileY;	
	SI32			siDistance;	
	SI32			*psiDrawOrder;
	SI32			i;
	strTI*			pstTI;
	SI32			siCharID, siItemPos, siObjectID;	
	SI32			siShortDistance;
	SI32			siX, siY;
	RECT			rcItemRealRect;
	XSPR			*pItemSpr;
	CItemHeader		*pItem;

	siCharID	=	siItemPos	=	siObjectID		=	0;
	*psiCharID	=	*psiItemPos	=	*psiObjectID	=	0;

	TransformMapXYToCharMapXY(m_siX, m_siY, siBaseCharTileX, siBaseCharTileY);

	psiDrawOrder	=	siDrawOrder;
	for( i = 0; i < siTileNo; i++, psiDrawOrder += 2)
	{		
		siPutX			=	psiDrawOrder[0];
		siPutY			=	psiDrawOrder[1];

		siCharTileX		=	siBaseCharTileX + siPutX;
		siCharTileY		=	siBaseCharTileY + siPutY;

		if((siCharTileX >= 0) && (siCharTileX < MAX_ONLINE_CHAR_MAP_XSIZE) && (siCharTileY >= 0) && (siCharTileY < MAX_ONLINE_CHAR_MAP_YSIZE))
		{
			pstTI = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];
			
			if((pstTI->siCharID != 0) && (pMyOnlineWorld->pIOnlineChar->GetUniqueID(pstTI->siCharID) != pMyOnlineWorld->pMyData->GetMyCharUniqueID()))
			{
				if(pMyOnlineWorld->pIOnlineChar->IsPointToChar(pstTI->siCharID, pptMouse, &siDistance) == TRUE)
				{
					if(siCharID == 0)	
					{
						siShortDistance	=	siDistance;
						siCharID		=	pstTI->siCharID;
					}
					else 
					{
						if(siShortDistance < siDistance)
						{
							siShortDistance	=	siDistance;
							siCharID		=	pstTI->siCharID;
						}						
					}
				}
			}
			else if((siCharID == 0) && (pstTI->uiItemID != 0))
			{
				if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_siX + siPutX, m_siY + siPutY, siX, siY) == TRUE)	
				{
					pItem					=	pMyOnlineWorld->pItemsInfo->GetItem( pstTI->uiItemID );
					pItemSpr				=	pMyOnlineWorld->pOnlineResource->GetxsprObject( pItem->m_siSm_SpriteFont );

					rcItemRealRect.left		=	siX - (pItemSpr->Header.Xsize / 2) - m_siRX;
					rcItemRealRect.top		=	siY - pItemSpr->Header.Ysize - m_siRY;
					rcItemRealRect.right	=	rcItemRealRect.left + pItemSpr->Header.Xsize;
					rcItemRealRect.bottom	=	rcItemRealRect.top + pItemSpr->Header.Ysize;

					if(PtInRect(&rcItemRealRect, *pptMouse) == TRUE)	siItemPos	=	SI32(((m_siX + siPutX) << 16) | (siY));
				}
			}			
		}
	}		

	if(siCharID)
	{
		*psiCharID		=	siCharID;		
		return TRUE;
	}
	else if(siItemPos)
	{		
		*psiItemPos		=	siItemPos;
		return TRUE;
	}
	else
	{		
		// ���������� ������Ʈ�� ����Ű���� �˾ƿ´�.
		TransformMouseCoordToMapXY(pptMouse->x, pptMouse->y, siX, siY);
		*psiObjectID	=	GetObjectID(siX, siY);
		
		if(*psiObjectID != 0)
			return TRUE;
		else
			return FALSE;
	}	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�̹� �ٸ� ĳ���Ͱ� �ڸ��� ������ �ξ����� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineMap::IsCharReserved(SI32 siX, SI32 siY)
{
	SI32	siCharTileX, siCharTileY;
	
	if(TransformMapXYToCharMapXY(siX, siY, siCharTileX, siCharTileY) == TRUE)
	{	
		if(m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siAtb & ON_TILE_ATB_CHAR_RESERVE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;	
		}
	}

	return FALSE;
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʿ��� Ư�� ��ǥ�� ã�´�.
//----------------------------------------------------------------------------------------------------------------
POINT	OnlineMap::SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1)
{
	POINT	TempPoint;
	SI32	TempDistance;
	SI32	TempCount;
	SI32	TempX;
	SI32	TempY;
	SI32	siDirectionCounter;
	strTI	*pTileInfo;
	SI32	siCharTileX, siCharTileY;
	

	// ĳ���͸� ���� ��ġ�� ã����.
	if(siSearchInfo == ON_MAP_SI_CHAR)
	{
		if(TransformMapXYToCharMapXY(siCX, siCY, siCharTileX, siCharTileY) == TRUE)
		{			

			pTileInfo	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];

			if((pTileInfo->siAtb & ON_TILE_ATB_COLL) == 0 && 
				(pTileInfo->siBackupAtb & ON_TILE_ATB_COLL) == 0 && 
				(pTileInfo->siAtb & ON_TILE_ATB_CHAR_RESERVE) == 0 && 
				pTileInfo->siCharID == 0 &&
				(pTileInfo->uiTileAtb & siParam1 ))
			{
				TempPoint.x = siCX;
				TempPoint.y = siCY;
				return TempPoint;
			}		
		}		
 
		for(TempDistance = 1; TempDistance < 100; TempDistance++)
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
					
					if(TransformMapXYToCharMapXY(TempX, TempY, siCharTileX, siCharTileY) == TRUE)					
					{
						pTileInfo	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];

						if((pTileInfo->siAtb & ON_TILE_ATB_COLL) == 0 && 
							(pTileInfo->siBackupAtb & ON_TILE_ATB_COLL) == 0 && 
							(pTileInfo->siAtb & ON_TILE_ATB_CHAR_RESERVE) == 0 && 
							pTileInfo->siCharID == 0 &&
							(pTileInfo->uiTileAtb & siParam1 ))
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
	

	TempPoint.x = -1;
	TempPoint.y = -1;
	return TempPoint;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Map Store ������ ��´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
MapStoreHeader*	OnlineMap::GetMapStoreParser(SI16 siX, SI16 siY, SI16* lpMPointFlag)
{
	MapStoreHeader*		lpMapStoreHeader;
	
	if( !m_MapStoreParser.IsMapIndex( m_MapIndex ) )	return NULL;

	lpMapStoreHeader = m_MapStoreParser.GetMapStore(siX, siY, m_MapIndex);

	*lpMPointFlag = FALSE;
	if(lpMapStoreHeader)
	{
		if((lpMapStoreHeader->siMapStoreMapXPosition == siX) && (lpMapStoreHeader->siMapStoreMapYPosition == siY))
		{
			*lpMPointFlag = 0;
		}
		else
		{
			*lpMPointFlag = ((siY - lpMapStoreHeader->siMapStoreMapYStartPosition) * (lpMapStoreHeader->siMapStoreMapXEndPosition 
				- lpMapStoreHeader->siMapStoreMapXStartPosition + 1)) + (siX - lpMapStoreHeader->siMapStoreMapXStartPosition) + 1; 

			if(*lpMPointFlag >= (((lpMapStoreHeader->siMapStoreMapYPosition - lpMapStoreHeader->siMapStoreMapYStartPosition)
				* (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1)) + 
				(lpMapStoreHeader->siMapStoreMapXPosition - lpMapStoreHeader->siMapStoreMapXStartPosition) + 1))
			{
				(*lpMPointFlag)--;
			}
		}

		return lpMapStoreHeader;
	}

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get War Field Map
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 OnlineMap::GetWarFieldMap(void)
{
	return m_MapInfo.siWarFieldMap;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� FieldMap�� PK�� �������� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineMap::CheckPKFieldMap(void)
{
	return m_MapInfo.fWarFlag == 2 ? TRUE : FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� FieldMap�� PK�� �������� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL OnlineMap::CheckPKFieldMap(SI16 siMapIndex)
{
	MapIndexHeader*		lpMapIndexHeader;

	lpMapIndexHeader = MapIndexParser.GetMapIndexData(siMapIndex);
	if(lpMapIndexHeader == NULL) return FALSE;

	return lpMapIndexHeader->fWarFlag == 2 ? TRUE : FALSE;
}

void OnlineMap::VMercenaryBufferClear(void)
{
	ZeroMemory(siVMercenary, MAX_ONLINE_CHAR_MAP_XSIZE * MAX_ONLINE_CHAR_MAP_YSIZE * 4 * sizeof(SI16));
}

void OnlineMap::VMercenaryAdd(SI16 x, SI16 y, SI16 siCharID)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(x, y, siCharTileX, siCharTileY) == TRUE)
	{
		SI32		Index;
		SI32		IndexCount;

		Index = (((siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX) * 4);

		for(IndexCount = 0; IndexCount < 4; IndexCount++)
		{
			if(siVMercenary[Index + IndexCount] == 0)
			{
				siVMercenary[Index + IndexCount] = siCharID;
				return;
			}
		}
	}
}

void OnlineMap::VMercenaryDelete(SI16 x, SI16 y, SI16 siCharID)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(x, y, siCharTileX, siCharTileY) == TRUE)
	{
		SI32		Index;
		SI32		IndexCount;
		SI32		IndexCount1;

		Index = (((siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX) * 4);

		for(IndexCount = 0; IndexCount < 4; IndexCount++)
		{
			if(siVMercenary[Index + IndexCount] == siCharID)
			{
				siVMercenary[Index + IndexCount] = 0;

				for(IndexCount1 = (IndexCount + 1); IndexCount1 < 4; IndexCount1++)
				{
					siVMercenary[Index + IndexCount1] = siVMercenary[Index + IndexCount1 - 1];
				}
				return;
			}
		}
	}
}

SI16 OnlineMap::VMercenaryGetCount(SI16 x, SI16 y)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(x, y, siCharTileX, siCharTileY) == TRUE)
	{
		SI32		Index;
		SI32		IndexCount;

		Index = (((siCharTileY * MAX_ONLINE_CHAR_MAP_XSIZE) + siCharTileX) * 4);

		for(IndexCount = 0; IndexCount < 4; IndexCount++)
		{
			if(siVMercenary[Index + IndexCount] == 0)
			{
				return IndexCount;
			}
		}

		return IndexCount;
	}

	return 0;
}

VOID OnlineMap::AnimationDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!m_AniData.bAction)		return;

	if(m_AniData.dwCurFrame < m_AniData.uiMaxFrame)
	{
		if(m_AniData.bFlag)
			clGrp.PutSpriteLightImjinCT(m_AniData.siX, m_AniData.siY, m_AniData.AniSpr.Header.Xsize, m_AniData.AniSpr.Header.Ysize , &m_AniData.AniSpr.Image[m_AniData.AniSpr.Header.Start[m_AniData.dwCurFrame]]);
		else
			clGrp.PutSpriteT(m_AniData.siX, m_AniData.siY, m_AniData.AniSpr.Header.Xsize, m_AniData.AniSpr.Header.Ysize , &m_AniData.AniSpr.Image[m_AniData.AniSpr.Header.Start[m_AniData.dwCurFrame]]);
		m_AniData.dwCurFrame++;
	}
	else
	{
		if(m_AniData.bRepeat)
			m_AniData.dwCurFrame = 0;
		else
		{
			m_AniData.siCurCount++;
			if(m_AniData.siCurCount == m_AniData.siMaxCount)
			{
				m_AniData.bAction = FALSE;
				SetGamePalette(clGrp.Palette);
				if(m_AniData.AniSpr.Image)	 {clGrp.FreeXspr(m_AniData.AniSpr);   m_AniData.AniSpr.Image  = NULL;}
			}
			else
				m_AniData.dwCurFrame = 0;
		}
	}
}

VOID OnlineMap::SetAnimation(SI32 siX, SI32 siY, UI08 MaxFrame, CHAR *szPath, SI08 maxcount, BOOL repeat,bool bThunder)
{
	m_AniData.bAction = TRUE;
	m_AniData.siX = siX;
	m_AniData.siY = siY;
	m_AniData.dwCurFrame = 0;
	m_AniData.uiMaxFrame = MaxFrame;
	m_AniData.bRepeat = repeat;
	m_AniData.siCurCount = 0;
	m_AniData.siMaxCount = maxcount;
	m_AniData.bFlag = bThunder;
	clGrp.LoadXspr(szPath, m_AniData.AniSpr);

	if(bThunder)
		SetGamePalette(pGame->ThunderPalette);
}



VOID OnlineMap::BoothMapDraw(SI16 siTileX, SI16 siTileY, POINT &ptMouse, LPDIRECTDRAWSURFACE7 pSurface)
{
	MapStoreHeader*		lpMapStoreHeader;
	SMapBoothData*		lpSMapBoothData;
	CItemHeader*		pItemHeader;
	XSPR*				pItemSpr;

	SI16				TempItemYPosition;
	SI16				TempCount;
	SI16				TempItemXPosition;
	SI16				TempIndex;
	SI32				siX, siY;	

	RECT				rcItemRealRect;

	lpMapStoreHeader = m_MapStoreParser.GetMapStore(siTileX, siTileY, m_MapIndex);


	if(!lpMapStoreHeader)	return;


	if((lpMapStoreHeader->siMapStoreMapXPosition == siTileX) && (lpMapStoreHeader->siMapStoreMapYPosition == siTileY))
	{
		lpSMapBoothData = m_MapBoothInfo.GetBoothData(lpMapStoreHeader->siMapStoreCode);

		if(lpSMapBoothData)
		{
			if(lpSMapBoothData->sifData)
			{
				for(TempCount = 0; TempCount < ON_MAX_BOOTH_ITEM; TempCount++)
				{
					pItemHeader		=	pMyOnlineWorld->pItemsInfo->GetItem(lpSMapBoothData->uiItemID[TempCount]);

					if(pItemHeader)
					{
						pItemSpr		=	pMyOnlineWorld->pOnlineResource->GetxsprItem( pItemHeader->m_siSm_SpriteFile );

						// ��ǥ ���
						TempIndex = TempCount;
						if(TempIndex >= (((lpMapStoreHeader->siMapStoreMapYPosition - lpMapStoreHeader->siMapStoreMapYStartPosition) * (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1)) + (lpMapStoreHeader->siMapStoreMapXPosition - lpMapStoreHeader->siMapStoreMapXStartPosition)))
						{
							TempIndex++;
						}

						TempItemXPosition = TempIndex % (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1);
						TempItemYPosition = TempIndex / (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1);

						TempItemXPosition += lpMapStoreHeader->siMapStoreMapXStartPosition;
						TempItemYPosition += lpMapStoreHeader->siMapStoreMapYStartPosition;

						// Ÿ�� ��ǥ�� ȭ�� ��ǥ�� ��ȯ�Ѵ�.
						if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(TempItemXPosition, TempItemYPosition, siX, siY) == TRUE)
						{							
							// �������� Real Rect�� �����Ѵ�.
							rcItemRealRect.left		=	siX - (pItemSpr->Header.Xsize / 2) - m_siRX;
							rcItemRealRect.top		=	siY - (pItemSpr->Header.Ysize / 2) - m_siRY - 16;
							rcItemRealRect.right	=	rcItemRealRect.left + pItemSpr->Header.Xsize;
							rcItemRealRect.bottom	=	rcItemRealRect.top + pItemSpr->Header.Ysize;
						
							// �� ���콺�� �������� ����Ű�� �ִ°�?
							if(PtInRect(&rcItemRealRect, ptMouse) == TRUE)
							{
								// ���콺�� �������� ����Ű���� �����Ѵ�.
								pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_ITEM, TempItemXPosition, TempItemYPosition, 0);
							}

							clGrp.PutSpriteT(rcItemRealRect.left, rcItemRealRect.top, 
											pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[ pItemSpr->Header.Start[pItemHeader->m_siSm_SpriteFont] ]);
						}											
					}
				}
			}
		}
	}
}