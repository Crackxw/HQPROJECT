/************************************************************************************************
	파일명 : OnlineMap.h
	담당자 : 이윤석

	Game에서 현재 플레이 하는 Map의 정보를 가진다.
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
//	설명	:	생성자.
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
//	설명	:	소멸자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineMap::~OnlineMap()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Create
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::Create(cltOnlineWorld *pOnlineWorld)
{
	m_BlackTile = pOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BLACKTILE);


	// 표지판정보 읽기	
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

	return Load(0, pOnlineWorld);					// 기본적으로 0번 맵을 가지고 있다.
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.
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

	// 모든 레이어를 없앤다.
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
//	설명		:	전 지도를 전부 그린다(캐릭터, 오브젝트, 타일).
//				:	현재 마우스가 가리키고 있는 것 역시 여기서 얻어올 수 있다.
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
	// 타일을 우선 그린다.		
	DrawTile(pSurface);				
//	return;
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
#else
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 타일을 우선 그린다.		
		DrawTile(pSurface);				
#endif
		m_fHeroSelect = FALSE;

		// 바닥에 출려돼는 Object 출력
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

				// 오브젝트를 그린다.(이동불가능한 Object를 그린다.)
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

				// 오브젝트를 그린다.(이동가능한 Object를 그린다.)
				if(((pstTI->bUnknownObjectID == TRUE) || ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID)))
				{	
					if(pstTI->bUnknownObjectID == TRUE)
					{
						// 알수 없는 Object일 경우
						if(TransformMapXYToCharMapXY(pstTI->siSourceObjectX, pstTI->siSourceObjectY, siCharTileX1, siCharTileY1) == TRUE)
						{
							pstTI1     = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1];								

							if(ON_MAP_ISDRAWOBJECT(pstTI1->uiObjectID))
							{
								siObjectID = ON_MAP_GETOBJECTID(pstTI1->uiObjectID);
								if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == TRUE)
								{
									// 이동가능 Object인지 검사
									Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
									ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
									if((Kind == 0) && (ObjAtt == 0))
									{
										// 현재 Tile위치가 제일 아랫쪽인지 검사
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
						// 자신이 Object인 경우
						siObjectID = ON_MAP_GETOBJECTID(pstTI->uiObjectID);																							
						if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == TRUE)
						{
							Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
							ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
							if((Kind == 0) && (ObjAtt == 0))
							{
								// 현재 Tile위치가 제일 아랫쪽인지 검사
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

		// 바닥 위에 출려돼는 Object 출력
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

				// 아이템을 그린다.
				if(pstTI->uiItemID != 0)
				{
					pItemHeader		=	pMyOnlineWorld->pItemsInfo->GetItem(pstTI->uiItemID);					

					if(pMyOnlineWorld->pOnlineInventory->IsActive())
					{
						if( pMyOnlineWorld->pOnlineInventory->GetItemPickUpID() )
						{
							// actdoll (2004/07/23 18:11) : 필드에 떨궈진 아이템의 위치를 얻어오는 구문이 바뀌었다.
							POINT	ptDest;
							pMyOnlineWorld->pOnlineInventory->GetItemPickUpMapPos( ptDest );
							if( ( ptDest.x == siTileX ) && ( ptDest.y == siTileY ) )	pItemHeader = NULL;
						}
					}
					if(pItemHeader)
					{
						pItemSpr		=	pMyOnlineWorld->pOnlineResource->GetxsprItem( pItemHeader->m_siSm_SpriteFile );												

						// 타일 좌표를 화면 좌표로 변환한다.
						if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siTileX, siTileY, siX, siY) == TRUE)
						{							
							// 아이템의 Real Rect를 설정한다.
							rcItemRealRect.left		=	siX - (pItemSpr->Header.Xsize / 2) - m_siRX;
							rcItemRealRect.top		=	siY - (pItemSpr->Header.Ysize / 2) - m_siRY;
							rcItemRealRect.right	=	rcItemRealRect.left + pItemSpr->Header.Xsize;
							rcItemRealRect.bottom	=	rcItemRealRect.top + pItemSpr->Header.Ysize;
						
							// 현 마우스가 아이템을 가르키고 있는가?
							if(PtInRect(&rcItemRealRect, ptMouse) == TRUE)
							{
								// 마우스가 아이템을 가르키도록 설정한다.
								pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_ITEM, siTileX, siTileY, pstTI->uiItemID);								

							}				

							clGrp.PutSpriteT(rcItemRealRect.left, rcItemRealRect.top, 
											pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[ pItemSpr->Header.Start[pItemHeader->m_siSm_SpriteFont] ]);
						}											
					}
				}

				// 따라다니는 용병을 출력한다.
				if(lpVMercenaryIndex[0] != 0)
				{
					SI32		siVMercenaryCount;

					for(siVMercenaryCount = 0; siVMercenaryCount < 4; siVMercenaryCount++)
					{
						if(lpVMercenaryIndex[siVMercenaryCount] != 0)
						{		
							SI32		siUniqueID;

							siUniqueID = pMyOnlineWorld->pIOnlineChar->GetUniqueID(lpVMercenaryIndex[siVMercenaryCount]);

							// 현재 Tile의 위치가 목적 Tile로 이동하는 중인지, 이동하는 중이면 비율이 얼마나 되는지 검사
							TempTileZoneP = 1.0f;
							pMyOnlineWorld->pIOnlineChar->GetMovePersentByVMercenary(siUniqueID, &TempX);
							if((TempX < 50) && (TempX > 0))
							{
								TempTileZoneP = (float)TempX / 50.0f;
							}

							if(TempTileZoneP < 1.0f)
							{
								// 현재 다음 Tile로 넘어갈려고 하는 경우
								// 앞의 Tile과 현재 Tile의 높이를 계산
								TempTileWater1 = GetWaterFlag(pstTI);
								switch(TempTileWater1)
								{
									case 0:		TempTileWater1 = 100;	break;
									case 1:		TempTileWater1 = 87;	break;
									case 2:		TempTileWater1 = 76;	break;
									default:	TempTileWater1 = 100;	break;
								}

								// 목적지 Tile의 정보를 얻어옴
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
								// 현재 목적 Tile로 와서 중앙으로 이동하려는 경우
								// 앞의 Tile과 현재 Tile의 높이를 계산
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

				// 캐릭터를 그린다.
				if(pstTI->siCharID != 0)
				{					
					SI32		siUniqueID;

					siUniqueID = pMyOnlineWorld->pIOnlineChar->GetUniqueID(pstTI->siCharID);

					// 내 캐릭터일 경우에는 전투 마우스로 변하지 않는다.
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

					// 현재 Tile의 위치가 목적 Tile로 이동하는 중인지, 이동하는 중이면 비율이 얼마나 되는지 검사
					TempTileZoneP = 1.0f;
					pMyOnlineWorld->pIOnlineChar->GetMovePersent(siUniqueID, &TempX);
					if((TempX < 50) && (TempX > 0))
					{
						TempTileZoneP = (float)TempX / 50.0f;
					}

					if(TempTileZoneP < 1.0f)
					{
						// 현재 다음 Tile로 넘어갈려고 하는 경우
						// 앞의 Tile과 현재 Tile의 높이를 계산
						TempTileWater1 = GetWaterFlag(pstTI);
						switch(TempTileWater1)
						{
							case 0:		TempTileWater1 = 100;	break;
							case 1:		TempTileWater1 = 87;	break;
							case 2:		TempTileWater1 = 76;	break;
							default:	TempTileWater1 = 100;	break;
						}

						// 목적지 Tile의 정보를 얻어옴
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
						// 현재 목적 Tile로 와서 중앙으로 이동하려는 경우
						// 앞의 Tile과 현재 Tile의 높이를 계산
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

				// 오브젝트를 그린다.(이동불가능한 Object를 그린다.)
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

				// 오브젝트를 그린다.(이동가능한 Object를 그린다.)
				if((pstTI->bUnknownObjectID == TRUE) || ON_MAP_ISDRAWOBJECT(pstTI->uiObjectID))
				{	
					if(pstTI->bUnknownObjectID == TRUE)
					{
						// 알수 없는 Object일 경우
						if(TransformMapXYToCharMapXY(pstTI->siSourceObjectX, pstTI->siSourceObjectY, siCharTileX1, siCharTileY1) == TRUE)
						{
							pstTI1     = &m_pstMap[uiMultiplyCharMapYOff[siCharTileY1] + siCharTileX1];								

							if(ON_MAP_ISDRAWOBJECT(pstTI1->uiObjectID))
							{
								siObjectID = ON_MAP_GETOBJECTID(pstTI1->uiObjectID);
								if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == FALSE)
								{
									// 이동가능 Object인지 검사
									Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
									ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
									if((Kind == 0) && (ObjAtt == 0))
									{
										// 현재 Tile위치가 제일 아랫쪽인지 검사
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
						// 자신이 Object인 경우
						siObjectID = ON_MAP_GETOBJECTID(pstTI->uiObjectID);																							
						if(pMyOnlineWorld->pIOnlineObject->CheckTileDraw(siObjectID) == FALSE)
						{
							Kind   = pMyOnlineWorld->pIOnlineObjectKI->GetAtbFromIndex(pMyOnlineWorld->pIOnlineObject->GetKindIndex(siObjectID)) & ATB_OBJECT_ISOBJECT;
							ObjAtt = pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE);
							if((Kind == 0) && (ObjAtt == 0))
							{
								// 현재 Tile위치가 제일 아랫쪽인지 검사
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

				// 자판 Item 출력
				if( !m_IsBoothMap )	continue;

				// 좌판 출력
				BoothMapDraw(siTileX, siTileY, ptMouse, pSurface);
			}
		}

		// 애니메이션 드로우
		AnimationDraw(pSurface);
		clGrp.UnlockSurface(pSurface);
	}	

	// Interface에 Mouse Point가 걸리는지 검사
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

	// 캐릭터를 가리키는 것을 처리한다.
	SelectID = 0;
	if(pMyOnlineWorld->pMousePointInfo->GetPointKind() != ON_MOUSE_POINT_INTERFACE)
	{
		// Character 검사
		if(uiMousePointCharID)
		{
			// 현재 가리키는 캐릭터가 자기 자신이 아니여야 한다.
			if((pMyOnlineWorld->pIOnlineChar->GetUniqueID(uiMousePointCharID) != pMyOnlineWorld->pMyData->GetMyCharUniqueID())
				&& (pMyOnlineWorld->pIOnlineChar->GetShow(pMyOnlineWorld->pIOnlineChar->GetUniqueID(uiMousePointCharID)) == TRUE))	// 그려지는(보여지는) 캐릭터인지 검사
			{
				pMyOnlineWorld->pMousePointInfo->SetPointTo(ON_MOUSE_POINT_CHAR, siMousePointCharX, siMousePointcharY, 
									pMyOnlineWorld->pIOnlineChar->GetUniqueID(uiMousePointCharID));
			}		
		}	

		if(pMyOnlineWorld->pMousePointInfo->GetPointKind() == ON_MOUSE_POINT_FIELD)
		{
			// 오브젝트 검사
			if(siMousePointObjectID)
			{
				// 선택한게 NPC인지 검사
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
//	설명		:	타일만 그린다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::DrawTile(LPDIRECTDRAWSURFACE7 pSurface)
{
	DWORD	TempCheckTime;

	TempCheckTime = GetNowCpuTime();

#ifdef  ONLINE_NEWTILEDRAW
/*	SI16 siCountX, siCountY;
	SI32 TempXX, TempYY, siCharTileX, siCharTileY;
	strTI*	pstTI;

	// Cache에 올라와있는 타일들을 해제하지 않기위해 GetxsprTile()을 호출한다.
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
	SI16	Temp_YetTileIndex;								// 최적화를 위해 사용(바로 전의 Tile 찍은걸 저장하고, 다음에 찍을 Tile과 Index가 같으면 이 Tile Data를 이용하게 함)
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
				// Tile 초기화
				TransformMapXYToCharMapXYNonClipping((m_siX + siCountX), (m_siY + siCountY), siCharTileX, siCharTileY);

				// 타일의 중심점을 구한다.
				siCenterX = (siCountX * ONLINE_TILE_XSIZE_HALF) - (siCountY * ONLINE_TILE_XSIZE_HALF) + (m_rcScreenRect.right - m_rcScreenRect.left + 1) / 2 - m_siRX;
				siCenterY = (siCountX * ONLINE_TILE_YSIZE_HALF) + (siCountY * ONLINE_TILE_YSIZE_HALF) + (m_rcScreenRect.bottom - m_rcScreenRect.top + 1) / 2 - m_siRY;						
			
				// 타일의 크기에 맞추어서 시작점의 위치를 계산한다.
				siStartX  = siCenterX - ONLINE_TILE_XSIZE_HALF;
				siStartY  = siCenterY - ONLINE_TILE_YSIZE_HALF;				
				pXsprTile = m_BlackTile;

				// 출력.
				clGrp.PutSpriteT(siStartX, siStartY, ONLINE_TILE_XSIZE, ONLINE_TILE_YSIZE, pXsprTile->Image);
			}
			else
			{
				TransformMapXYToCharMapXY((m_siX + siCountX), (m_siY + siCountY), siCharTileX, siCharTileY);

				// 타일의 중심점을 구한다.
				siCenterX = (siCountX * ONLINE_TILE_XSIZE_HALF) - (siCountY * ONLINE_TILE_XSIZE_HALF) + (m_rcScreenRect.right - m_rcScreenRect.left + 1) / 2 - m_siRX;
				siCenterY = (siCountX * ONLINE_TILE_YSIZE_HALF) + (siCountY * ONLINE_TILE_YSIZE_HALF) + (m_rcScreenRect.bottom - m_rcScreenRect.top + 1) / 2 - m_siRY;

				// 타일의 크기에 맞추어서 시작점의 위치를 계산한다.
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

				// 출력.
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
//	설명	:	Tile Cache에 타일만 그린다.
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
	SI16	Temp_YetTileIndex;								// 최적화를 위해 사용(바로 전의 Tile 찍은걸 저장하고, 다음에 찍을 Tile과 Index가 같으면 이 Tile Data를 이용하게 함)
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

				// 타일의 중심점을 구한다.
				siCenterX = TempsiCountXMulONLINE_TILE_XSIZE_HALF - TempsiCountYMulONLINE_TILE_XSIZE_HALF + TempON_MAP_TILECACHEWIDTHHALF;
				siCenterY = TempsiCountXMulONLINE_TILE_YSIZE_HALF + TempsiCountYMulONLINE_TILE_YSIZE_HALF + TempON_MAP_TILECACHEHEIGHTHALF;
			
				// 타일의 크기에 맞추어서 시작점의 위치를 계산한다.
				siStartX  = siCenterX - ONLINE_TILE_XSIZE_HALF;
				siStartY  = siCenterY - ONLINE_TILE_YSIZE_HALF;				

				if((TempXX < 0) || (TempYY < 0)	|| (TempXX >= m_siXsize) || (TempYY >= m_siYsize))
				{
					pXsprTile = m_BlackTile;

					// 출력.
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
// SoundMap영역을 체크하려면... 필요.
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
//	설명	:	해제.
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

	// 기본 맵을 해제한다.
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
//	설명	:	주인공 캐릭터를 가운데에 오도록 맵을 이동한다.
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

	// 현재 속성을 검사하여 이상한 속성이 검출되면 처리
	lpstrTI = m_pstMap;
	for(TempCount = ((MAX_ONLINE_CHAR_MAP_XSIZE * MAX_ONLINE_CHAR_MAP_YSIZE) - 1); TempCount >= 0; TempCount--)
	{
		if(lpstrTI->siBackupAtb & ON_TILE_ATB_COLL)
		{
			lpstrTI->siAtb |= ON_TILE_ATB_COLL;
		}

		lpstrTI++;
	}
	
	// 로드되어 있지 않은 타일을 로드한다.
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
			// PreProcessorBeforeLoadUnloadedTile은 새로운 위치가 변하기 전에 해주어야 할 작업들을 해주는 것이다.
			// 오브젝트 해제는 여기서 해주어야 한다.		
			PreProcessorBeforeLoadUnloadedTile(siDirection);
		}
		else
		{
			for(SI32 TempCount = 0; TempCount < MAX_ONLINE_OBJECT; TempCount++)
			{
				pMyOnlineWorld->pIOnlineObject->Free(TempCount);
			}
		}

		// 캐릭터가 움직이면 좌판을 다시 읽어 들일 필요가 있따.
		reLoadBooth =	TRUE;
		m_siX		=	siX;
		m_siY		=	siY;

		// LoadUnloadedTile은 반드시 m_siX와 m_siY가 설정된 후에 
		// 호출해 주어야 한다.
		LoadUnloadedTile(siDirection);
	}	

	m_siRX	=	siRX;
	m_siRY	=	siRY;				

	if( !m_IsBoothMap )		return;	
	if( !reLoadBooth )		return;	

	
	// 현재 맵에 있는 자판 정보 검사
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

					// Server로 이 자판에 대한 Data를 요청
					pMyOnlineWorld->pOnlineClient->SendDisplayBoothItemMsg(m_MapStoreParser.GetMapStore(TempCount)->siMapStoreCode);
				}
			}
		}
	}

	// 거리가 너무 멀어진 자판들을 검사하여 삭제
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로이 맵의 포커스를 가진 캐릭터를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineMap::SetCurrentFocusCharID(UI16 uiNewFocusCharID)
{
	m_uiCurrentFocusCharID	=	uiNewFocusCharID;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로이 맵의 포커스를 가진 캐릭터를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	OnlineMap::GetCurrentFocusCharID()
{
	return m_uiCurrentFocusCharID;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	화면의 마우스 좌표를 Map의 X, Y 좌표로 얻어온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformMouseCoordToMapXY(SI32	siMouseX,SI32 siMouseY, SI32 &siMapX, SI32 &siMapY)
{
	SI32 siRealX, siRealY;

	// Screen Dot 좌표를 Real Coordinate 좌표로 변환을 한다.
	TransformScreenDotXYToRealDotXY(siMouseX, siMouseY, siRealX, siRealY);

	// Real Coordinate 좌표를 MapX, Y로 계산한다.
	TransformRealDotXYToMapXY(siRealX, siRealY, siMapX, siMapY);


	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Screen Dot 좌표를 실제 절대적인 위치로 전환을 해서 Dot값으로 얻어온다.
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
//	설명		:	절대 좌표를 Map의 좌표로 바꾸어준다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformRealDotXYToMapXY(SI32	siRealDotX, SI32 siRealDotY, SI32 &siMapX, SI32 &siMapY)
{
	// 마우스의 좌표와 가장 가까운 중심의 좌표를 구한다. 
	siMapX  =	( siRealDotX/2 + siRealDotY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;
	siMapY	=	(-siRealDotX/2 + siRealDotY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;

	// Map의 경계를 벗어나는지를 검사한다.
	if(siMapX <  0)         siMapX = 0;
	if(siMapY <  0)         siMapY = 0;
	if(siMapX >= m_siXsize) siMapX = m_siXsize - 1;
	if(siMapY >= m_siYsize) siMapY = m_siYsize - 1;
	return TRUE;
}

//--------------------------------------------------------------------------
//	설명	:	실제 절대적인 위치로 Screen Dot 좌표로 전환을 한다.
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
// Desc: Map[y][x]의 중심위치를 절대적인 위치로 전환을 해서 Dot값으로 얻어온다.
//
//                 |------(0,0)------|
//                 |      /   \      |
//--------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToRealDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siRealDotCX, SI32 &siRealDotCY)
{
	if(IsInMapArea(siMapX, siMapY) == FALSE)	return FALSE;

	// 타일의 중심점을 구한다.
	siRealDotCX = siMapX * ONLINE_TILE_XSIZE_HALF 
			    - siMapY * ONLINE_TILE_XSIZE_HALF;
	
	siRealDotCY = siMapX * ONLINE_TILE_YSIZE_HALF 
		  	    + siMapY * ONLINE_TILE_YSIZE_HALF ;

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: TransformMapXYToScreenDotCenterXY()
// Desc: Map[y][x]의 중심위치를 절대적인 위치로 전환을 해서 화면을 기준으로 Dot값으로 얻어온다.
//--------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToScreenDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siScreenDotCX, SI32 &siScreenDotCY)
{
	if(IsInScreenMapArea(siMapX, siMapY) == FALSE)	return FALSE;

	// 타일의 중심점을 구한다.
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
//	설명	:	타일 좌표를 캐릭터만의 맵좌표로 변환한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToCharMapXY(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY)
{
	if(IsInCharMapArea(siMapX, siMapY) == FALSE) return FALSE;
	
	siCharMapX	=	siMapX - (m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE);
	siCharMapY	=	siMapY - (m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE);

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	타일 좌표를 캐릭터만의 맵좌표로 변환한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::TransformMapXYToCharMapXYNonClipping(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY)
{
	siCharMapX	=	siMapX - (m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE);
	siCharMapY	=	siMapY - (m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE);

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	맵의 좌표를 실제 맵상의 도트 좌표로 변환한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	실제 맵상의 도트 좌표를 맵의 좌표로 변환한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineMap::TransformRealXYToMapXY(SI32 siRealX, SI32 siRealY, SI32 &siMapX, SI32 &siMapY)
{
	float		TempFloat;

	// 마우스의 좌표와 가장 가까운 중심의 좌표를 구한다. 
	TempFloat =  (float)m_siYsize / (float)(m_siXsize + m_siYsize);
	siRealX   -= (SI32)((float)((m_siXsize * ONLINE_TILE_XSIZE_HALF) + (m_siYsize * ONLINE_TILE_XSIZE_HALF)) * TempFloat);
	siMapX    =	 ( siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;
	siMapY	  =  (-siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;

	// Map의 경계를 벗어나는지를 검사한다.
	if(siMapX <  0)         siMapX = 0;
	if(siMapY <  0)         siMapY = 0;
	if(siMapX >= m_siXsize) siMapX = m_siXsize - 1;
	if(siMapY >= m_siYsize) siMapY = m_siYsize - 1;
	return TRUE;

	
	
	
	/*
	// 마우스의 좌표와 가장 가까운 중심의 좌표를 구한다. 
	siRealX -=  (((m_siXsize * ONLINE_TILE_XSIZE) / 2) + (m_siXsize * ONLINE_TILE_XSIZE_HALF)) / 2;
	siMapX  =	( siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;
	siMapY	=	(-siRealX/2 + siRealY + ONLINE_TILE_XSIZE_HALF / 2) / ONLINE_TILE_XSIZE_HALF;

	// Map의 경계를 벗어나는지를 검사한다.
	if(siMapX <  0)         siMapX = 0;
	if(siMapY <  0)         siMapY = 0;
	if(siMapX >= m_siXsize) siMapX = m_siXsize - 1;
	if(siMapY >= m_siYsize) siMapY = m_siYsize - 1;
	return TRUE;
	*/
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	맵의 실제 크기를 계산한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineMap::GetMapRealSize(SI32 &siRealX, SI32 &siRealY)
{
	siRealX = (m_siXsize * ONLINE_TILE_XSIZE_HALF) + (m_siYsize * ONLINE_TILE_XSIZE_HALF);
	siRealY = (m_siXsize * ONLINE_TILE_YSIZE_HALF) + (m_siYsize * ONLINE_TILE_YSIZE_HALF);

	return TRUE;
}
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	해당 Map X,Y가 캐릭터 Map의 크기를 벗어나지 않는지 검사한다.
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
// Desc: 해당 Map X,Y가 화면에 보여주고 있는 Map의 크기를 벗어나지 않는지 검사한다.
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
//	설명	:	해당 스크린상의 X, Y가 맵을 출력하는 사각형 영역을 벗어나지 않는지 검사한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsInScreenRect(SI32 siX, SI32 siY)
{
	if(siX >= m_rcScreenRect.left && siY >= m_rcScreenRect.top && siX <= m_rcScreenRect.right && siY <= m_rcScreenRect.bottom)
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
// 설명	:	충돌 타일인지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsColl(SI32 siX, SI32 siY)
{		
	SI32	siCharTileX, siCharTileY;

	// dauduk (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
	//	맵의 충돌 타입에 대한 계산 부분에서 해당 블럭 값에 대한 충돌체크가 지정한 맵의 사이즈보다 커질 경우
	//	억세스 바이올레이션이 발생한다. 가장 빈도수가 높은 에러다.
	//	때문에 맵의 규격을 체크하여 사이즈를 검사하고, 그 값을 초과하는 범위에서는 무조건 충돌상태라는것으로 리턴하게끔 수정했다.
	//	다음 버그 리포팅에 이 부분에서 버그가 안나오거나 조치에 따른 특별한 문제가 없을 경우 이 주석을 제거해도 좋다.
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
// 설명	:	충돌 타일인지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb)
{		
	SI32	siCharTileX, siCharTileY;

	// dauduk (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
	//	맵의 충돌 타입에 대한 계산 부분에서 해당 블럭 값에 대한 충돌체크가 지정한 맵의 사이즈보다 커질 경우
	//	억세스 바이올레이션이 발생한다. 가장 빈도수가 높은 에러다.
	//	때문에 맵의 규격을 체크하여 사이즈를 검사하고, 그 값을 초과하는 범위에서는 무조건 충돌상태라는것으로 리턴하게끔 한다.
	//	다음 버그 리포팅에 이 부분에서 버그가 안나오거나 조치에 따른 특별한 문제가 없을 경우 이 주석을 제거해도 좋다.


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
// 설명	:	충돌 Type을 얻어온다.(0 : 이동불가, 1 : 캐릭터가 위에 있음, 2 : 이동 가능)
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
//	설명	:	이동 시킨다.
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
//	설명	:	맵을 저장한다.
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

	// 헤더를 준비한다.
	PrepareHeader(&pLoadSaveMap->Header);	

	// 타일 사이즈를 할당 받는다.
	pLoadSaveMap->pTI	=	new	LoadSaveMapTI[pLoadSaveMap->Header.siXsize * pLoadSaveMap->Header.siYsize];	
	
	if(pLoadSaveMap->pTI == NULL)
	{
		bRet = FALSE;
		goto END;
	}
	
	// 타일의 대한 정보를 복사한다.
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
		// 헤더를 화일에 쓴다.
		if(fwrite(&pLoadSaveMap->Header, sizeof(LoadSaveMapHeader), 1, fp) != 1)
		{
			bRet = FALSE;
			goto END;
		}

		// 데이터를 화일에 쓴다.		
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
//	설명	:	맵을 로드한다.
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
		// 화일 에서 맵이 시작하는 위치를 얻어온다.
		fseek(fpMap, sizeof(LoadSaveMapHeader), SEEK_SET);
		fgetpos(fpMap, &fposMap);			
		
		// 다시 0으로 돌려준다.
		fseek(fpMap, 0, SEEK_SET);
		
		// 헤더를 읽어온다.
		if(fread(&LoadSaveMap.Header, sizeof(LoadSaveMapHeader), 1, fpMap) != 1)
		{
			bRet = FALSE;
			goto END;
		}			
		
		// 맵을 다시 초기화 한다.
		// 여기에서 전체 맵에 대한 메모리를 할당 받지는 않는다.		
		if(Init(MapIndex, LoadSaveMap.Header.siXsize, LoadSaveMap.Header.siYsize, NULL, pOnlineWorld) == FALSE)
		{
			bRet= FALSE;
			goto END;
		}						
		
		// Mini Map 처리
		if(pMyOnlineWorld->pOnlineMiniMap)
		{
			// actdoll (2004/07/09 11:09) : 만약 본맵과 관련된 데이터가 왔을 경우 중국용 미니맵을 따로 갱신한다.
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
		
		// 구름 처리
		if(pMyOnlineWorld->pOnlineFieldEffect)
		{
			if(lpMapIndexHeader->siWeather & MAPINDEXPARSERWEATHER_CLOUD) pMyOnlineWorld->pOnlineFieldEffect->SetCloud(TRUE);
			else                                                          pMyOnlineWorld->pOnlineFieldEffect->SetCloud(FALSE);
			pMyOnlineWorld->pOnlineFieldEffect->ChangeMap(MapIndex);
		}

//		PlayFitMusic();
		
		// 성공했다.
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
//	설명	:	헤더에 데이터를 넣는다.
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
//	설명	:	타일을 설정한다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetTile(SI16 siTileX, SI16 siTileY, SI16 siFileIndex, SI16 siFontIndex, UI32 siAtb, UI16 uiRoadFileIndex, UI16 uiRoadFontIndex)
{
	XSPR	*pTileXspr, *pRoadXspr;
	SI32	siCharTileX, siCharTileY;
	strTI	*pstTI;

	// Char Map Area 에서의 위치를 구한다.
	if(m_pstMap)
	{
		if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
		{	

			pstTI	=	&m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX];
			
			pstTI->siFile		=	siFileIndex;						
			pstTI->siFont		=	GETFONTINDEX(siFontIndex);						// 최 상위 1 비트는 충돌 폰트인지의 여부를 가지고 있다.
			pstTI->uiTileAtb    =   siAtb;
			pstTI->uiRoadFile	=	uiRoadFileIndex;
			pstTI->uiRoadFont	=	uiRoadFontIndex;
			pstTI->siFieldGuideTable = 0;

			// ObjectID는 나중에 밖으로 빼야 한다.
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
				// 화일의 인덱스가 있다.
				// 즉 길이다.
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
//	설명	:	새로운 맵을 로딩하고 그 지역으로 이동
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMap::ChangeMap(SI16 MapIndex, SI16 siX, SI16 siY)
{
	// Object 초기화
	pMyOnlineWorld->pIOnlineObject->FreeAll();

	// Map Change
	Load(MapIndex, pMyOnlineWorld);

	// Path Find 부분
	pMyOnlineWorld->pclFindPath->SetFindPathSize(m_siXsize, m_siYsize);

	// Mini Map 처리
	if(pMyOnlineWorld->pOnlineMiniMap)
	{
		pMyOnlineWorld->pOnlineMiniMap->Warp(siX, siY);
	}

	// Map Position Change
	SetXY(siX, siY);

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 맵의 X Y 위치를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetXY(SI16 siX, SI16 siY)
{
	// PreProcessorBeforeLoadUnloadedTile은 새로운 위치가 변하기 전에 해주어야 할 작업들을 해주는 것이다.
	// 오브젝트 해제는 여기서 해주어야 한다.		
	PreProcessorBeforeLoadUnloadedTile(0);

	m_siX	=	siX;
	m_siY	=	siY;

	// 전체를 모두 업데이트한다.
	LoadUnloadedTile(0);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 맵의 X 위치를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetX(SI16 siX)
{
	m_siX	=	siX;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 맵의 X 위치를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::SetY(SI16 siY)
{
	m_siY	=	siY;
}
/*
//----------------------------------------------------------------------------------------------------------------
//	설명	:	현재 맵의 X 변위를 얻어온다.	
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineMap::GetRX()
{
	return m_siRX;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	현재 맵의 Y 변위를 얻어온다.	
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineMap::GetRY()
{
	return m_siRY;
}
*/
//----------------------------------------------------------------------------------------------------------------
//	설명	:	맵에 캐릭터의 ID 및 오브젝트의 ID를 설정한다.
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

				// siCharID와 siObjectID는 배열의 인덱스 이지만		
				// siItemID는 실제 아이템의 ID이다.
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

	// 캐릭터나 오브젝트일 경우에만 충돌을 설정해준다.
	if(siCharID != 0)
	{
		SetTileAtb(siTileX, siTileY, siXTileNum, siYTileNum, ON_TILE_ATB_COLL, TRUE);
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	특정 위치에 1x1 오브젝트를 심는다. 속도를 위해서 사용하는 함수이다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineMap::SetObjectID(SI16 siTileX, SI16 siTileY, UI16 uiObjectID)
{
	SI32	siCharTileX, siCharTileY;

	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].uiObjectID =  uiObjectID;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	특정 위치에 오브젝트를 심는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				// ID로 이동가능한 지역인지 검사
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
//	설명	:	특정 위치에서 캐릭터 및 오브젝트을 뺀다.(오브젝트가 있을 경우에는 오브젝트 배열에서도 삭제한다.)
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
					// 이 타일에 있는 오브젝트 ID가 화면을 찍는데 쓰이는 ID라면 
					// 오브젝트 배열에서 해당 오브젝트를 삭제한다.
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
//	설명	:	특정 위치에서 아이템 data를 수정한다.
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
//	설명	:	특정 타일에 특정 속성을 정의하거나 뺀다.
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
//	설명	:	맵에 설정되어 있는 오브젝트의 ID를 얻어온다.
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
//	설명	:	맵에 설정되어 있는 캐릭터의 ID를 얻어온다.
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
//	설명	:	맵에 설정되어 있는 아이템의 ID를 얻어온다.
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
//	설명	:	맵에 설정되어 있는 아이템의 갯수를 얻어온다.
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
//	설명	:	맵의 사이즈를 얻어온다(인터패이스 없애면서 추가되는 함수)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::GetSize(SI32 *psiXsize, SI32 *psiYsize)
{
	*psiXsize	=	m_siXsize;
	*psiYsize	=	m_siYsize;	
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	맵의 현재 위치를 얻어온다(인터패이스 없애면서 추가되는 함수)
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineMap::GetCurrentPos(SI32 *psiX, SI32 *psiY)
{
	*psiX	=	m_siX;				
	*psiY	=	m_siY;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	사각형을 찍는 범위를 얻어온다(인터패이스 없애면서 추가되는 함수)
//----------------------------------------------------------------------------------------------------------------
RECT	OnlineMap::GetScreenRect()
{
	return m_rcScreenRect;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	m_siX와 m_siY를 수정하고 LoadUnloadedTile을 호출하려고 할때 m_siX와 m_siY를 수정하기 전에 호출해 주어야한다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineMap::PreProcessorBeforeLoadUnloadedTile(SI32 siDirection)
{	
	SI32	siTileX;
	SI32	siTileY;

	if(siDirection == 0)
	{
		// 지도에 셋팅 되어 있는 모든 오브젝트를 삭제한다.		
		DelID(m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE, m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE, MAX_ONLINE_CHAR_MAP_XSIZE, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_OBJECT);
	}
	else
	{
		// 조금만 움직였으니 기존에 있던 부분은 그냥 복사한다.
		// 기존의 데이타를 움직인다.
		if(ON_EAST & siDirection)
		{			
			// 비어 있는 칸을 채워 넣는다.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			DelID(siTileX, siTileY, 1, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_OBJECT);			
		}
		else if(ON_WEST & siDirection)
		{
			// 비어 있는 칸을 채워 넣는다.
			siTileX	=	m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;							

			DelID(siTileX, siTileY, 1, MAX_ONLINE_CHAR_MAP_YSIZE, ON_MAP_DELID_OBJECT);		
		}

		if(ON_NORTH & siDirection)
		{			
			// 비어 있는 칸을 채워 넣는다.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE;		

			DelID(siTileX, siTileY, MAX_ONLINE_CHAR_MAP_XSIZE, 1, ON_MAP_DELID_OBJECT);
		}
		else if(ON_SOUTH & siDirection)
		{
			// 비어 있는 칸을 채워 넣는다.
			siTileX	=	m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE;
			siTileY	=	m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE;			

			DelID(siTileX, siTileY, MAX_ONLINE_CHAR_MAP_XSIZE, 1, ON_MAP_DELID_OBJECT);
		}		
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	로드되어있지 않은 타일을 로드한다.
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
		// 지도에 셋팅 되어 있는 모든 ID를 삭제한다.
		// 오브젝트를 삭제하지 않는다.
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
						// 아직 로드되지 않은 타일이다.

						// 화일 포인터를 해당 좌표의 포인터로 이동시킨다.
						fsetpos(fpMap, &fposMap);
						fseek(fpMap, (siTileY * (/*sizeof(LoadSaveMapTI)*/10 * m_siXsize)) + (/*sizeof(LoadSaveMapTI)*/10 * siTileX), SEEK_CUR);

						// 세팅한다.
						fread(&TileInfo.stLoadSaveTile, sizeof(LoadSaveTile), 1, fpMap);						
						fread(&TileInfo.stLoadSaveRoad, sizeof(LoadSaveRoad), 1, fpMap);						
						fread(&TileInfo.uiObjectInfo, sizeof(UI16), 1, fpMap);						

						// 타일을 설정한다.						
						SetTile(siTileX, siTileY, TileInfo.stLoadSaveTile.uiFile, TileInfo.stLoadSaveTile.uiFont, TileInfo.stLoadSaveTile.uiAtb, TileInfo.stLoadSaveRoad.uiRoadFile, TileInfo.stLoadSaveRoad.uiRoadFont);						

						AnalysisObjectInfo(siTileX, siTileY, TileInfo.uiObjectInfo);

					}				
				}	
			}
		}		
	}
	else
	{
		// 조금만 움직였으니 기존에 있던 부분은 그냥 복사한다.
		// 기존의 데이타를 움직인다.
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

		// 새로운 데이타 부분을 얻어온다.
		if(ON_EAST & siDirection)
		{			
			// 비어 있는 칸을 채워 넣는다.
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

			// 모서리 부분을 설정한다.
			siCornerTile	|=	(ON_MAP_CORNER_EASTNORTH | ON_MAP_CORNER_EASTSOUTH);

			//MakeMapFile(ON_EAST);
		}
		else if(ON_WEST & siDirection)
		{
			// 비어 있는 칸을 채워 넣는다.
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

			// 모서리 부분을 설정한다.
			siCornerTile	|=	(ON_MAP_CORNER_WESTNORTH | ON_MAP_CORNER_WESTSOUTH);
		}

		if(ON_NORTH & siDirection)
		{						
			// 비어 있는 칸을 채워 넣는다.
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
			// 비어 있는 칸을 채워 넣는다.
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

	// 표지판 정리
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
// Desc: 그리기순서테이블을 만든다
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
	// 그리기순서 테이블 제작
	//--------------------------------------------------------------------------
	// 위쪽 반
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
	
	// 아래쪽 반
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
	// 여기부턴 Fog 원형 Table 계산 
	//--------------------------------------------------------------------------
	/*
	for(siCountC = 0; siCountC  <= siHalfXSize
	siCountA, siCountB, 
	siCircleOrder[
	*/
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	오브젝트의 정보를 분석하여 처리해준다.(마을의 상대 좌표의 경우에도 이쪽으로 들어온다.)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		case ON_MAP_OI_CODE_KIND:				// Kind 를 아는 Object일때
			
			// 새로운 오브젝트이다.
			// 오브젝트를 추가한다.			
			siObjectID	=	pMyOnlineWorld->pIOnlineObject->Init(pObjectInfoBfs->BaseBf.siParameter, siTileX, siTileY, pMyOnlineWorld);
			
			if(siObjectID != 0)
			{
				SetObjectID2(siTileX, siTileY, ON_MAP_MAKEDRAWOBJECT(siObjectID));
			}			
			break;
		case ON_MAP_OI_CODE_RELATIVEPOS:		// Kind를 모르는 Object일때
			// 상대 좌표를 가지고 있는 오브젝트이다.
			// 버퍼에 넣는다.											

			
			if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
			{
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].bUnknownObjectID	=	TRUE;
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSourceObjectX	=	siTileX + pObjectInfoBfs->RelativePosBf.siX;
				m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSourceObjectY	=	siTileY + pObjectInfoBfs->RelativePosBf.siY;
			}
			break;
		case ON_MAP_OI_CODE_VILLAGE:			// 마을일때
			// 마을이다.
			// 해당 위치에 마을을 얻어온다.
			if((pVillageHeader = pMyOnlineWorld->pOnlineVillageManager->GetVillageByPos(siTileX, siTileY, m_MapIndex)) != NULL)
			{
				siObjectID	=	pMyOnlineWorld->pIOnlineObject->Init(pVillageHeader, pMyOnlineWorld);		

				// 타일에 오브젝트를 설정한다.(속도를 고려하여 이 위치에 논다.)
				if(siObjectID != 0)	
				{
					SetObjectID2(siTileX, siTileY, ON_MAP_MAKEDRAWOBJECT(siObjectID));			
				}
			}
			else if((pPortalHeader = PortalParser.GetPortal(siTileX, siTileY, m_MapIndex)) != NULL)
			{
				// 마을이 아니면 Portal이다.
				siObjectID	=	pMyOnlineWorld->pIOnlineObject->Init(pPortalHeader, pMyOnlineWorld);

				// 타일에 오브젝트를 설정한다.(속도를 고려하여 이 위치에 논다.)
				if(siObjectID != 0)	
				{
					SetObjectID2(siTileX, siTileY, ON_MAP_MAKEDRAWOBJECT(siObjectID));			
				}
			}
			break;
		}
	}	

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길인 경우 추가 이동 속도를 구한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				case 2:			// 빠르게
					*psiAddMoveSpeed = 2;
					return	TRUE;	

				case 3:			// 느리게
					*psiAddMoveSpeed = -2;
					return	TRUE;	
			}
		}
	}	

	return FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Tile의 물 속성을 구해온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI08 	OnlineMap::GetWaterFlag(SI16 siTileX, SI16 siTileY)
{
	SI32	siCharTileX, siCharTileY;
	
	if(TransformMapXYToCharMapXY(siTileX, siTileY, siCharTileX, siCharTileY) == TRUE)
	{
		return (m_pstMap[uiMultiplyCharMapYOff[siCharTileY] + siCharTileX].siSpeed & 0xff0000) >> 16;
	}	

	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Tile의 물 속성을 구해온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI08 	OnlineMap::GetWaterFlag(strTI* lpTileData)
{
	return (lpTileData->siSpeed & 0xff0000) >> 16;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마우스가 가리키는 캐릭터의 ID를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 최종적으로 오브젝트를 가리키는지 알아온다.
		TransformMouseCoordToMapXY(pptMouse->x, pptMouse->y, siX, siY);
		*psiObjectID	=	GetObjectID(siX, siY);
		
		if(*psiObjectID != 0)
			return TRUE;
		else
			return FALSE;
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	이미 다른 캐릭터가 자리를 예약해 두었는지 본다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
//	설명	:	맵에서 특정 좌표를 찾는다.
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
	

	// 캐릭터를 심을 위치를 찾을때.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Map Store 정보를 얻는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get War Field Map
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32 OnlineMap::GetWarFieldMap(void)
{
	return m_MapInfo.siWarFieldMap;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 FieldMap이 PK가 가능한지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineMap::CheckPKFieldMap(void)
{
	return m_MapInfo.fWarFlag == 2 ? TRUE : FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 FieldMap이 PK가 가능한지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

						// 좌표 계산
						TempIndex = TempCount;
						if(TempIndex >= (((lpMapStoreHeader->siMapStoreMapYPosition - lpMapStoreHeader->siMapStoreMapYStartPosition) * (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1)) + (lpMapStoreHeader->siMapStoreMapXPosition - lpMapStoreHeader->siMapStoreMapXStartPosition)))
						{
							TempIndex++;
						}

						TempItemXPosition = TempIndex % (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1);
						TempItemYPosition = TempIndex / (lpMapStoreHeader->siMapStoreMapXEndPosition - lpMapStoreHeader->siMapStoreMapXStartPosition + 1);

						TempItemXPosition += lpMapStoreHeader->siMapStoreMapXStartPosition;
						TempItemYPosition += lpMapStoreHeader->siMapStoreMapYStartPosition;

						// 타일 좌표를 화면 좌표로 변환한다.
						if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(TempItemXPosition, TempItemYPosition, siX, siY) == TRUE)
						{							
							// 아이템의 Real Rect를 설정한다.
							rcItemRealRect.left		=	siX - (pItemSpr->Header.Xsize / 2) - m_siRX;
							rcItemRealRect.top		=	siY - (pItemSpr->Header.Ysize / 2) - m_siRY - 16;
							rcItemRealRect.right	=	rcItemRealRect.left + pItemSpr->Header.Xsize;
							rcItemRealRect.bottom	=	rcItemRealRect.top + pItemSpr->Header.Ysize;
						
							// 현 마우스가 아이템을 가르키고 있는가?
							if(PtInRect(&rcItemRealRect, ptMouse) == TRUE)
							{
								// 마우스가 아이템을 가르키도록 설정한다.
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