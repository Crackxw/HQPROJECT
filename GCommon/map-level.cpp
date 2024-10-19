#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>

extern unsigned char NormalHillType[][MAX_TILE_VARY];
extern SHORT Dir8Q[];

//  x, y지역에 level 을 설정한다.
void _Map::SetLevel(SHORT x, SHORT y, SHORT level)
{
	if( pLevelInfo == NULL )			return;
	if( IsInMapArea(x, y) == FALSE )	return;

	pLevelInfo[x+YOff[y]] &= 0x0f;
	pLevelInfo[x+YOff[y]] |= (level<<4);
}

// x, y  지역의 level을 얻어온다. 
SHORT _Map::GetLevel(SHORT x, SHORT y)
{
	// 유효한 지도 영역인지 확인한다. 
	if( IsInMapArea(x, y) == FALSE )return 0;

	// 평지이면 그 높이 그대로 얻어오고, 
	if( pTopLevel )		
	{
		if(GetLevelMode(pTopLevel[x+YOff[y]], x, y) == 15)	return pTopLevel[x+YOff[y]];
		// 경사지이면 높이에서 한칸 뺀다. 
		else if(GetLevelMode(pTopLevel[x+YOff[y]], x, y))	return pTopLevel[x+YOff[y]]-1;
	}

	return 0;
}

// 성곽등을 고려한 최종 높이 
// 2배수 좌표 
SHORT _Map::GetLastLevel(SHORT x, SHORT y)
{
	SHORT level;

	level=GetLevel(x, y);

	if(level >= 0)
	{
       // 장애물용 언덕을 고려한 높이 
	   level += GetNewBlkLevel(x, y);
    }
	else return 0;


	return level;

}







// sx, sy주변의 층 배열을 확인한다. 
int _Map::GetLevelNear1(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if(GetLevelMode(level, x, y)&1)number|=4;
		if(GetLevelMode(level, x, y)&2)number|=8;
	}

	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetLevelMode(level, x, y)&2)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=4;
	}

	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=2;
		if(GetLevelMode(level, x, y)&4)number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&2)number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetLevelMode(level, x, y)&8)number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=8;
	 }


	return number;

}

// sx, sy:2배수 좌표 
int _Map::GetLevelNear2(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if((GetLevelMode(level, x, y)&1)==0)number|=4;
		if((GetLevelMode(level, x, y)&2)==0)number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if((GetLevelMode(level, x, y)&2)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=2;
		if((GetLevelMode(level, x, y)&4)==0)number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&2)==0)number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if((GetLevelMode(level, x, y)&8)==0)number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=8;
	 }

	 number=15-number;

	return number;

}



// 언덕을 심는다. 
void _Map::PutLevel(SHORT x, SHORT y, SHORT level, SHORT mode)
{
	SHORT i, j;
	SHORT number;

	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE )return ;


	if( pTopLevel )	
	{
		// 기존의 최고 높이가 새로 설정된 높이보다 낮거나 같으면, 
		if( pTopLevel[x+YOff[y]] <= level )
		{
		   // 최고 높이를 새로 설정된 높이로 만든다. 
		   pTopLevel[x+YOff[y]] = (unsigned char)level;

		   // 적절한 이미지를 얻어온다. 
		   SetProperFileNFont(x, y, Dir8Q[15]);

		   // 평지로 만든다. 
		   SetLevelSlope(x, y, HILL_PLANE);
		}
	}

	// 동서남북 사방에 언덕 위치가 있음을 설정한다. 
	SetLevelMode(level, x, y, 15);

	// 인근 지형에 영향을 미치는 모드이면 다른 타일들을 없앤다. 
	if(mode==TRUE)
	{
		// 물을 없앤다. 
		if(GetSeaFile(x/2, y/2))
		{
			PutSea(x / 2 * 2,      (y)/2*2, FALSE, FALSE);
			PutSea(x / 2 * 2 + 1,  (y)/2*2, FALSE, FALSE);
			PutSea(x / 2 * 2,      (y)/2*2+1, FALSE, FALSE);
			PutSea(x / 2 * 2 + 1,  (y)/2*2+1, FALSE, FALSE);
		}
	}


	// 아랫층이 빈약하면, 
	if(level > 1 && GetLevelMode(level-1, x, y) != 15)
	{
		PutLevel(x, y, level - 1, mode);
	}



	// 설정한 지역 인근의 언덕 상태를 확인한다. 
	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 설정한 지역 자신은 제외한다. 
			if(i==0 && j==0)continue;

			// 지도 범위를 벗어난것도 제외된다. 
			if(IsInMapArea(x + j, y + i) == FALSE)continue;


			// 인근의 다른 타일들을 없앤다. 
	        if(mode==TRUE)
			{
		         // 물을 없앤다. 
		         if(GetSeaFile ((x + j) / 2,(y + i) / 2) )
				 {
					 PutSea((x+j)/2*2,  (y+i)/2*2, FALSE, FALSE);
					 PutSea((x+j)/2*2+1,(y+i)/2*2, FALSE, FALSE);
					 PutSea((x+j)/2*2,  (y+i)/2*2+1, FALSE, FALSE);
					 PutSea((x+j)/2*2+1,(y+i)/2*2+1, FALSE, FALSE);
				 }
			}

			// 이미 해당 높이에서 충만된 상태라면 더 이상 할 것은 없다. 
		    if(GetLevelMode(level, x+j, y+i)==15)continue;

			// 주변의 언덕 상태를 얻어온다.
		    number=GetLevelNear1(x + j, y + i, level);

			// 주변에 이어지는 언덕이 전혀 없이 독립적이라면, 
			if(Dir8Q[number] == -2)
			{
				// 주변에 언덕을 심어준다. 
				PutLevel(x+j, y+i, level, mode);
			}
			else
			{

				// 주변에 언덕이 완전히 차 있는 상태면, 언덕을 설정한다.
				if(number==15)
				{
					PutLevel(x+j, y+i, level, mode);
				}
				else
				{
				
					// 최고 높이이거나 상위층에 아무것도 없다면, 
	                if(level == (MAX_LEVEL_STEP-1) || GetLevelMode(level + 1, x + j, y + i) == 0)
					{
	   
						if( pTopLevel )	
						{
							if( pTopLevel[(x+j) + YOff[y+i]] <= level )
							{
								pTopLevel[(x+j) + YOff[y+i]] = (unsigned char)level;

								SetProperFileNFont(x + j, y + i, Dir8Q[number]);

								SetLevelSlope( x + j, y + i, HILL_SLOPE );
							}
						}
					}

	 			    SetLevelMode((SI32)level, x+j, y+i, (UI08)number);

				    // 아랫층이 빈약하면, 
	                if( level>1 && GetLevelMode(level-1, x+j, y+i) != 15 )
					{
						PutLevel(x+j, y+i, level-1, mode);
					}
				}
			}

		}

}


// 높이를 지운다. 
void _Map::DeleteLevel(SHORT x, SHORT y, SHORT level)
{

	SHORT i, j;
	SHORT number;

	// 유효한 영역인지 확인한다. 
	if(x>=0 && x<MH.MapXsize && y>=0 && y<MH.MapYsize){}
	else return ;

	if(level>0)
		SetLevelMode(level, x, y, 0);

	if( pTopLevel )	
	{
		if( pTopLevel[x+YOff[y]] >= level )
		{
			if( level > 0 )
				pTopLevel[x+YOff[y]] = level-1;

			SetLevelSlope(x, y, HILL_PLANE);
			SetProperFileNFont(x, y, Dir8Q[15]);
		}
	   
    }

	if(level<(MAX_LEVEL_STEP-1))
	{
		DeleteLevel(x, y, level+1);
	}



	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 자신은 제외한다. 
			if(i==0 && j==0)continue;

			if(level==0)continue;

			// 범위를 벗어난것도 제외된다. 
			if(x+j>=0 && x+j<MH.MapXsize && y+i>=0 && y+i<MH.MapYsize){}
			else continue;

			
		    
		    if(GetLevelMode(level, x+j, y+i) == 0)continue;

		       number=GetLevelNear2(x+j, y+i, level);

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				DeleteLevel(x+j, y+i, level);
			}
			else
			{
				if(number==15)
				{
					DeleteLevel(x+j, y+i, level);
				}
				else
				{
	 			
					SetLevelMode(level, x+j, y+i, (UI08)number);

					if( pTopLevel )	
					{
						if( pTopLevel[(x+j) + YOff[y+i]] >= level )
						{
							if(level>0)
								pTopLevel[(x+j) + YOff[y+i]] = (unsigned char)level;
					  							  
							SetProperFileNFont(x+j, y+i, Dir8Q[number]);
							SetLevelSlope(x+j, y+i, HILL_SLOPE);
						}
					}

					// 아랫층이 빈약하면, 
	                if(level<(MAX_LEVEL_STEP-1))
					{
		               DeleteLevel(x+j, y+i, level+1);
					}
				}

			}

		}


}



/////////////////////////////////////////////////////////////////////////////
// 양정모용 함수 
/////////////////////////////////////////////////////////////////////////////

// 높이를 구한다.경사지는 -1을 리턴하고 나머지 지역은 높이를 리턴한다. 
SHORT _Map::GetHillLayer(SHORT x, SHORT y)
{
	SHORT level=GetLevel(x, y);

	if(GetLevelInfo(x, y)==HILL_SLOPE)
	{
		return -1;
	}
	return level;
}


#else
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>

extern unsigned char NormalHillType[][MAX_TILE_VARY];
extern SHORT Dir8Q[];

//  x, y지역에 level 을 설정한다.
void _Map::SetLevel(SHORT x, SHORT y, SHORT level)
{
	if( pLevelInfo == NULL )			return;
	if( IsInMapArea(x, y) == FALSE )	return;

	pLevelInfo[x+YOff[y]] &= 0x0f;
	pLevelInfo[x+YOff[y]] |= (level<<4);
}

// x, y  지역의 level을 얻어온다. 
SHORT _Map::GetLevel(SHORT x, SHORT y)
{
	// 유효한 지도 영역인지 확인한다. 
	if( IsInMapArea(x, y) == FALSE )return 0;

	// 평지이면 그 높이 그대로 얻어오고, 
	if( pTopLevel )		
	{
		if(GetLevelMode(pTopLevel[x+YOff[y]], x, y) == 15)	return pTopLevel[x+YOff[y]];
		// 경사지이면 높이에서 한칸 뺀다. 
		else if(GetLevelMode(pTopLevel[x+YOff[y]], x, y))	return pTopLevel[x+YOff[y]]-1;
	}

	return 0;
}

// 성곽등을 고려한 최종 높이 
// 2배수 좌표 
SHORT _Map::GetLastLevel(SHORT x, SHORT y)
{
	SHORT level;

	level=GetLevel(x, y);

	if(level >= 0)
	{
       // 장애물용 언덕을 고려한 높이 
	   level += GetNewBlkLevel(x, y);
    }
	else return 0;


	return level;

}







// sx, sy주변의 층 배열을 확인한다. 
int _Map::GetLevelNear1(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if(GetLevelMode(level, x, y)&1)number|=4;
		if(GetLevelMode(level, x, y)&2)number|=8;
	}

	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetLevelMode(level, x, y)&2)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=4;
	}

	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=2;
		if(GetLevelMode(level, x, y)&4)number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&2)number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetLevelMode(level, x, y)&8)number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=8;
	 }


	return number;

}

// sx, sy:2배수 좌표 
int _Map::GetLevelNear2(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if((GetLevelMode(level, x, y)&1)==0)number|=4;
		if((GetLevelMode(level, x, y)&2)==0)number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if((GetLevelMode(level, x, y)&2)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=2;
		if((GetLevelMode(level, x, y)&4)==0)number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&2)==0)number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if((GetLevelMode(level, x, y)&8)==0)number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=8;
	 }

	 number=15-number;

	return number;

}



// 언덕을 심는다. 
void _Map::PutLevel(SHORT x, SHORT y, SHORT level, SHORT mode)
{
	SHORT i, j;
	SHORT number;

	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE )return ;


	if( pTopLevel )	
	{
		// 기존의 최고 높이가 새로 설정된 높이보다 낮거나 같으면, 
		if( pTopLevel[x+YOff[y]] <= level )
		{
		   // 최고 높이를 새로 설정된 높이로 만든다. 
		   pTopLevel[x+YOff[y]] = (unsigned char)level;

		   // 적절한 이미지를 얻어온다. 
		   SetProperFileNFont(x, y, Dir8Q[15]);

		   // 평지로 만든다. 
		   SetLevelSlope(x, y, HILL_PLANE);
		}
	}

	// 동서남북 사방에 언덕 위치가 있음을 설정한다. 
	SetLevelMode(level, x, y, 15);

	// 인근 지형에 영향을 미치는 모드이면 다른 타일들을 없앤다. 
	if(mode==TRUE)
	{
		// 물을 없앤다. 
		if(GetSeaFile(x/2, y/2))
		{
			//PutSea(x / 2 * 2,      (y)/2*2, FALSE, FALSE);
			//PutSea(x / 2 * 2 + 1,  (y)/2*2, FALSE, FALSE);
			//PutSea(x / 2 * 2,      (y)/2*2+1, FALSE, FALSE);
			//PutSea(x / 2 * 2 + 1,  (y)/2*2+1, FALSE, FALSE);
			PutSea_Daesang(x ,      y , FALSE);
			

		}
	}


	// 아랫층이 빈약하면, 
	if(level > 1 && GetLevelMode(level-1, x, y) != 15)
	{
		PutLevel(x, y, level - 1, mode);
	}



	// 설정한 지역 인근의 언덕 상태를 확인한다. 
	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 설정한 지역 자신은 제외한다. 
			if(i==0 && j==0)continue;

			// 지도 범위를 벗어난것도 제외된다. 
			if(IsInMapArea(x + j, y + i) == FALSE)continue;


			// 인근의 다른 타일들을 없앤다. 
	        if(mode==TRUE)
			{
		         // 물을 없앤다. 
		         if(GetSeaFile ((x + j) / 2,(y + i) / 2) )
				 {
					 //PutSea((x+j)/2*2,  (y+i)/2*2, FALSE, FALSE);
					 //PutSea((x+j)/2*2+1,(y+i)/2*2, FALSE, FALSE);
					 //PutSea((x+j)/2*2,  (y+i)/2*2+1, FALSE, FALSE);
					 //PutSea((x+j)/2*2+1,(y+i)/2*2+1, FALSE, FALSE);
					 PutSea_Daesang(x+j,y+i, FALSE);

				 }
			}

			// 이미 해당 높이에서 충만된 상태라면 더 이상 할 것은 없다. 
		    if(GetLevelMode(level, x+j, y+i)==15)continue;

			// 주변의 언덕 상태를 얻어온다.
		    number=GetLevelNear1(x + j, y + i, level);

			// 주변에 이어지는 언덕이 전혀 없이 독립적이라면, 
			if(Dir8Q[number] == -2)
			{
				// 주변에 언덕을 심어준다. 
				PutLevel(x+j, y+i, level, mode);
			}
			else
			{

				// 주변에 언덕이 완전히 차 있는 상태면, 언덕을 설정한다.
				if(number==15)
				{
					PutLevel(x+j, y+i, level, mode);
				}
				else
				{
				
					// 최고 높이이거나 상위층에 아무것도 없다면, 
	                if(level == (MAX_LEVEL_STEP-1) || GetLevelMode(level + 1, x + j, y + i) == 0)
					{
	   
						if( pTopLevel )	
						{
							if( pTopLevel[(x+j) + YOff[y+i]] <= level )
							{
								pTopLevel[(x+j) + YOff[y+i]] = (unsigned char)level;

								SetProperFileNFont(x + j, y + i, Dir8Q[number]);

								SetLevelSlope( x + j, y + i, HILL_SLOPE );
							}
						}
					}

	 			    SetLevelMode((SI32)level, x+j, y+i, (UI08)number);

				    // 아랫층이 빈약하면, 
	                if( level>1 && GetLevelMode(level-1, x+j, y+i) != 15 )
					{
						PutLevel(x+j, y+i, level-1, mode);
					}
				}
			}

		}

}


// 높이를 지운다. 
void _Map::DeleteLevel(SHORT x, SHORT y, SHORT level)
{

	SHORT i, j;
	SHORT number;

	// 유효한 영역인지 확인한다. 
	if(x>=0 && x<MH.MapXsize && y>=0 && y<MH.MapYsize){}
	else return ;

	if(level>0)
		SetLevelMode(level, x, y, 0);

	if( pTopLevel )	
	{
		if( pTopLevel[x+YOff[y]] >= level )
		{
			if( level > 0 )
				pTopLevel[x+YOff[y]] = level-1;

			SetLevelSlope(x, y, HILL_PLANE);
			SetProperFileNFont(x, y, Dir8Q[15]);
		}
	   
    }

	if(level<(MAX_LEVEL_STEP-1))
	{
		DeleteLevel(x, y, level+1);
	}



	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 자신은 제외한다. 
			if(i==0 && j==0)continue;

			if(level==0)continue;

			// 범위를 벗어난것도 제외된다. 
			if(x+j>=0 && x+j<MH.MapXsize && y+i>=0 && y+i<MH.MapYsize){}
			else continue;

			
		    
		    if(GetLevelMode(level, x+j, y+i) == 0)continue;

		       number=GetLevelNear2(x+j, y+i, level);

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				DeleteLevel(x+j, y+i, level);
			}
			else
			{
				if(number==15)
				{
					DeleteLevel(x+j, y+i, level);
				}
				else
				{
	 			
					SetLevelMode(level, x+j, y+i, (UI08)number);

					if( pTopLevel )	
					{
						if( pTopLevel[(x+j) + YOff[y+i]] >= level )
						{
							if(level>0)
								pTopLevel[(x+j) + YOff[y+i]] = (unsigned char)level;
					  							  
							SetProperFileNFont(x+j, y+i, Dir8Q[number]);
							SetLevelSlope(x+j, y+i, HILL_SLOPE);
						}
					}

					// 아랫층이 빈약하면, 
	                if(level<(MAX_LEVEL_STEP-1))
					{
		               DeleteLevel(x+j, y+i, level+1);
					}
				}

			}

		}


}



/////////////////////////////////////////////////////////////////////////////
// 양정모용 함수 
/////////////////////////////////////////////////////////////////////////////

// 높이를 구한다.경사지는 -1을 리턴하고 나머지 지역은 높이를 리턴한다. 
SHORT _Map::GetHillLayer(SHORT x, SHORT y)
{
	SHORT level=GetLevel(x, y);

	if(GetLevelInfo(x, y)==HILL_SLOPE)
	{
		return -1;
	}
	return level;
}
#endif