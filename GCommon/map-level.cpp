#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>

extern unsigned char NormalHillType[][MAX_TILE_VARY];
extern SHORT Dir8Q[];

//  x, y������ level �� �����Ѵ�.
void _Map::SetLevel(SHORT x, SHORT y, SHORT level)
{
	if( pLevelInfo == NULL )			return;
	if( IsInMapArea(x, y) == FALSE )	return;

	pLevelInfo[x+YOff[y]] &= 0x0f;
	pLevelInfo[x+YOff[y]] |= (level<<4);
}

// x, y  ������ level�� ���´�. 
SHORT _Map::GetLevel(SHORT x, SHORT y)
{
	// ��ȿ�� ���� �������� Ȯ���Ѵ�. 
	if( IsInMapArea(x, y) == FALSE )return 0;

	// �����̸� �� ���� �״�� ������, 
	if( pTopLevel )		
	{
		if(GetLevelMode(pTopLevel[x+YOff[y]], x, y) == 15)	return pTopLevel[x+YOff[y]];
		// ������̸� ���̿��� ��ĭ ����. 
		else if(GetLevelMode(pTopLevel[x+YOff[y]], x, y))	return pTopLevel[x+YOff[y]]-1;
	}

	return 0;
}

// �������� ����� ���� ���� 
// 2��� ��ǥ 
SHORT _Map::GetLastLevel(SHORT x, SHORT y)
{
	SHORT level;

	level=GetLevel(x, y);

	if(level >= 0)
	{
       // ��ֹ��� ����� ����� ���� 
	   level += GetNewBlkLevel(x, y);
    }
	else return 0;


	return level;

}







// sx, sy�ֺ��� �� �迭�� Ȯ���Ѵ�. 
int _Map::GetLevelNear1(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// ���� 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if(GetLevelMode(level, x, y)&1)number|=4;
		if(GetLevelMode(level, x, y)&2)number|=8;
	}

	// ���� 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetLevelMode(level, x, y)&2)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=4;
	}

	// ���� 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=2;

	 }

	 // ���� 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=2;
		if(GetLevelMode(level, x, y)&4)number|=8;

	 }

	 // ������ 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&2)number|=4;
	 }

	 // �ϼ��� 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetLevelMode(level, x, y)&8)number|=1;

	 }

	 // �ϵ��� 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=2;
	 }

	 // ������ 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=8;
	 }


	return number;

}

// sx, sy:2��� ��ǥ 
int _Map::GetLevelNear2(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// ���� 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if((GetLevelMode(level, x, y)&1)==0)number|=4;
		if((GetLevelMode(level, x, y)&2)==0)number|=8;
	}


	// ���� 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if((GetLevelMode(level, x, y)&2)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=4;
	}


	// ���� 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=2;

	 }

	 // ���� 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=2;
		if((GetLevelMode(level, x, y)&4)==0)number|=8;

	 }

	 // ������ 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&2)==0)number|=4;
	 }

	 // �ϼ��� 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if((GetLevelMode(level, x, y)&8)==0)number|=1;

	 }

	 // �ϵ��� 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=2;
	 }

	 // ������ 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=8;
	 }

	 number=15-number;

	return number;

}



// ����� �ɴ´�. 
void _Map::PutLevel(SHORT x, SHORT y, SHORT level, SHORT mode)
{
	SHORT i, j;
	SHORT number;

	// ��ȿ�� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(x, y) == FALSE )return ;


	if( pTopLevel )	
	{
		// ������ �ְ� ���̰� ���� ������ ���̺��� ���ų� ������, 
		if( pTopLevel[x+YOff[y]] <= level )
		{
		   // �ְ� ���̸� ���� ������ ���̷� �����. 
		   pTopLevel[x+YOff[y]] = (unsigned char)level;

		   // ������ �̹����� ���´�. 
		   SetProperFileNFont(x, y, Dir8Q[15]);

		   // ������ �����. 
		   SetLevelSlope(x, y, HILL_PLANE);
		}
	}

	// �������� ��濡 ��� ��ġ�� ������ �����Ѵ�. 
	SetLevelMode(level, x, y, 15);

	// �α� ������ ������ ��ġ�� ����̸� �ٸ� Ÿ�ϵ��� ���ش�. 
	if(mode==TRUE)
	{
		// ���� ���ش�. 
		if(GetSeaFile(x/2, y/2))
		{
			PutSea(x / 2 * 2,      (y)/2*2, FALSE, FALSE);
			PutSea(x / 2 * 2 + 1,  (y)/2*2, FALSE, FALSE);
			PutSea(x / 2 * 2,      (y)/2*2+1, FALSE, FALSE);
			PutSea(x / 2 * 2 + 1,  (y)/2*2+1, FALSE, FALSE);
		}
	}


	// �Ʒ����� ����ϸ�, 
	if(level > 1 && GetLevelMode(level-1, x, y) != 15)
	{
		PutLevel(x, y, level - 1, mode);
	}



	// ������ ���� �α��� ��� ���¸� Ȯ���Ѵ�. 
	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// ������ ���� �ڽ��� �����Ѵ�. 
			if(i==0 && j==0)continue;

			// ���� ������ ����͵� ���ܵȴ�. 
			if(IsInMapArea(x + j, y + i) == FALSE)continue;


			// �α��� �ٸ� Ÿ�ϵ��� ���ش�. 
	        if(mode==TRUE)
			{
		         // ���� ���ش�. 
		         if(GetSeaFile ((x + j) / 2,(y + i) / 2) )
				 {
					 PutSea((x+j)/2*2,  (y+i)/2*2, FALSE, FALSE);
					 PutSea((x+j)/2*2+1,(y+i)/2*2, FALSE, FALSE);
					 PutSea((x+j)/2*2,  (y+i)/2*2+1, FALSE, FALSE);
					 PutSea((x+j)/2*2+1,(y+i)/2*2+1, FALSE, FALSE);
				 }
			}

			// �̹� �ش� ���̿��� �游�� ���¶�� �� �̻� �� ���� ����. 
		    if(GetLevelMode(level, x+j, y+i)==15)continue;

			// �ֺ��� ��� ���¸� ���´�.
		    number=GetLevelNear1(x + j, y + i, level);

			// �ֺ��� �̾����� ����� ���� ���� �������̶��, 
			if(Dir8Q[number] == -2)
			{
				// �ֺ��� ����� �ɾ��ش�. 
				PutLevel(x+j, y+i, level, mode);
			}
			else
			{

				// �ֺ��� ����� ������ �� �ִ� ���¸�, ����� �����Ѵ�.
				if(number==15)
				{
					PutLevel(x+j, y+i, level, mode);
				}
				else
				{
				
					// �ְ� �����̰ų� �������� �ƹ��͵� ���ٸ�, 
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

				    // �Ʒ����� ����ϸ�, 
	                if( level>1 && GetLevelMode(level-1, x+j, y+i) != 15 )
					{
						PutLevel(x+j, y+i, level-1, mode);
					}
				}
			}

		}

}


// ���̸� �����. 
void _Map::DeleteLevel(SHORT x, SHORT y, SHORT level)
{

	SHORT i, j;
	SHORT number;

	// ��ȿ�� �������� Ȯ���Ѵ�. 
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
			// �ڽ��� �����Ѵ�. 
			if(i==0 && j==0)continue;

			if(level==0)continue;

			// ������ ����͵� ���ܵȴ�. 
			if(x+j>=0 && x+j<MH.MapXsize && y+i>=0 && y+i<MH.MapYsize){}
			else continue;

			
		    
		    if(GetLevelMode(level, x+j, y+i) == 0)continue;

		       number=GetLevelNear2(x+j, y+i, level);

			// �ֺ��� ���� ����. 
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

					// �Ʒ����� ����ϸ�, 
	                if(level<(MAX_LEVEL_STEP-1))
					{
		               DeleteLevel(x+j, y+i, level+1);
					}
				}

			}

		}


}



/////////////////////////////////////////////////////////////////////////////
// ������� �Լ� 
/////////////////////////////////////////////////////////////////////////////

// ���̸� ���Ѵ�.������� -1�� �����ϰ� ������ ������ ���̸� �����Ѵ�. 
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
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>

extern unsigned char NormalHillType[][MAX_TILE_VARY];
extern SHORT Dir8Q[];

//  x, y������ level �� �����Ѵ�.
void _Map::SetLevel(SHORT x, SHORT y, SHORT level)
{
	if( pLevelInfo == NULL )			return;
	if( IsInMapArea(x, y) == FALSE )	return;

	pLevelInfo[x+YOff[y]] &= 0x0f;
	pLevelInfo[x+YOff[y]] |= (level<<4);
}

// x, y  ������ level�� ���´�. 
SHORT _Map::GetLevel(SHORT x, SHORT y)
{
	// ��ȿ�� ���� �������� Ȯ���Ѵ�. 
	if( IsInMapArea(x, y) == FALSE )return 0;

	// �����̸� �� ���� �״�� ������, 
	if( pTopLevel )		
	{
		if(GetLevelMode(pTopLevel[x+YOff[y]], x, y) == 15)	return pTopLevel[x+YOff[y]];
		// ������̸� ���̿��� ��ĭ ����. 
		else if(GetLevelMode(pTopLevel[x+YOff[y]], x, y))	return pTopLevel[x+YOff[y]]-1;
	}

	return 0;
}

// �������� ����� ���� ���� 
// 2��� ��ǥ 
SHORT _Map::GetLastLevel(SHORT x, SHORT y)
{
	SHORT level;

	level=GetLevel(x, y);

	if(level >= 0)
	{
       // ��ֹ��� ����� ����� ���� 
	   level += GetNewBlkLevel(x, y);
    }
	else return 0;


	return level;

}







// sx, sy�ֺ��� �� �迭�� Ȯ���Ѵ�. 
int _Map::GetLevelNear1(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// ���� 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if(GetLevelMode(level, x, y)&1)number|=4;
		if(GetLevelMode(level, x, y)&2)number|=8;
	}

	// ���� 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetLevelMode(level, x, y)&2)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=4;
	}

	// ���� 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=1;
		if(GetLevelMode(level, x, y)&8)number|=2;

	 }

	 // ���� 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=2;
		if(GetLevelMode(level, x, y)&4)number|=8;

	 }

	 // ������ 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&2)number|=4;
	 }

	 // �ϼ��� 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetLevelMode(level, x, y)&8)number|=1;

	 }

	 // �ϵ��� 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if(GetLevelMode(level, x, y)&4)number|=2;
	 }

	 // ������ 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if(GetLevelMode(level, x, y)&1)number|=8;
	 }


	return number;

}

// sx, sy:2��� ��ǥ 
int _Map::GetLevelNear2(SHORT sx, SHORT sy, SHORT level)
{

	int number=0;
	SHORT x, y;

	// ���� 
	x=sx; y=sy+1;
	if(y<MH.MapYsize)
	{
		if((GetLevelMode(level, x, y)&1)==0)number|=4;
		if((GetLevelMode(level, x, y)&2)==0)number|=8;
	}


	// ���� 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if((GetLevelMode(level, x, y)&2)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=4;
	}


	// ���� 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=1;
		if((GetLevelMode(level, x, y)&8)==0)number|=2;

	 }

	 // ���� 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=2;
		if((GetLevelMode(level, x, y)&4)==0)number|=8;

	 }

	 // ������ 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&2)==0)number|=4;
	 }

	 // �ϼ��� 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if((GetLevelMode(level, x, y)&8)==0)number|=1;

	 }

	 // �ϵ��� 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize && y>=0)
	 {
		if((GetLevelMode(level, x, y)&4)==0)number|=2;
	 }

	 // ������ 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize && y<MH.MapYsize)
	 {
		if((GetLevelMode(level, x, y)&1)==0)number|=8;
	 }

	 number=15-number;

	return number;

}



// ����� �ɴ´�. 
void _Map::PutLevel(SHORT x, SHORT y, SHORT level, SHORT mode)
{
	SHORT i, j;
	SHORT number;

	// ��ȿ�� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(x, y) == FALSE )return ;


	if( pTopLevel )	
	{
		// ������ �ְ� ���̰� ���� ������ ���̺��� ���ų� ������, 
		if( pTopLevel[x+YOff[y]] <= level )
		{
		   // �ְ� ���̸� ���� ������ ���̷� �����. 
		   pTopLevel[x+YOff[y]] = (unsigned char)level;

		   // ������ �̹����� ���´�. 
		   SetProperFileNFont(x, y, Dir8Q[15]);

		   // ������ �����. 
		   SetLevelSlope(x, y, HILL_PLANE);
		}
	}

	// �������� ��濡 ��� ��ġ�� ������ �����Ѵ�. 
	SetLevelMode(level, x, y, 15);

	// �α� ������ ������ ��ġ�� ����̸� �ٸ� Ÿ�ϵ��� ���ش�. 
	if(mode==TRUE)
	{
		// ���� ���ش�. 
		if(GetSeaFile(x/2, y/2))
		{
			//PutSea(x / 2 * 2,      (y)/2*2, FALSE, FALSE);
			//PutSea(x / 2 * 2 + 1,  (y)/2*2, FALSE, FALSE);
			//PutSea(x / 2 * 2,      (y)/2*2+1, FALSE, FALSE);
			//PutSea(x / 2 * 2 + 1,  (y)/2*2+1, FALSE, FALSE);
			PutSea_Daesang(x ,      y , FALSE);
			

		}
	}


	// �Ʒ����� ����ϸ�, 
	if(level > 1 && GetLevelMode(level-1, x, y) != 15)
	{
		PutLevel(x, y, level - 1, mode);
	}



	// ������ ���� �α��� ��� ���¸� Ȯ���Ѵ�. 
	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// ������ ���� �ڽ��� �����Ѵ�. 
			if(i==0 && j==0)continue;

			// ���� ������ ����͵� ���ܵȴ�. 
			if(IsInMapArea(x + j, y + i) == FALSE)continue;


			// �α��� �ٸ� Ÿ�ϵ��� ���ش�. 
	        if(mode==TRUE)
			{
		         // ���� ���ش�. 
		         if(GetSeaFile ((x + j) / 2,(y + i) / 2) )
				 {
					 //PutSea((x+j)/2*2,  (y+i)/2*2, FALSE, FALSE);
					 //PutSea((x+j)/2*2+1,(y+i)/2*2, FALSE, FALSE);
					 //PutSea((x+j)/2*2,  (y+i)/2*2+1, FALSE, FALSE);
					 //PutSea((x+j)/2*2+1,(y+i)/2*2+1, FALSE, FALSE);
					 PutSea_Daesang(x+j,y+i, FALSE);

				 }
			}

			// �̹� �ش� ���̿��� �游�� ���¶�� �� �̻� �� ���� ����. 
		    if(GetLevelMode(level, x+j, y+i)==15)continue;

			// �ֺ��� ��� ���¸� ���´�.
		    number=GetLevelNear1(x + j, y + i, level);

			// �ֺ��� �̾����� ����� ���� ���� �������̶��, 
			if(Dir8Q[number] == -2)
			{
				// �ֺ��� ����� �ɾ��ش�. 
				PutLevel(x+j, y+i, level, mode);
			}
			else
			{

				// �ֺ��� ����� ������ �� �ִ� ���¸�, ����� �����Ѵ�.
				if(number==15)
				{
					PutLevel(x+j, y+i, level, mode);
				}
				else
				{
				
					// �ְ� �����̰ų� �������� �ƹ��͵� ���ٸ�, 
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

				    // �Ʒ����� ����ϸ�, 
	                if( level>1 && GetLevelMode(level-1, x+j, y+i) != 15 )
					{
						PutLevel(x+j, y+i, level-1, mode);
					}
				}
			}

		}

}


// ���̸� �����. 
void _Map::DeleteLevel(SHORT x, SHORT y, SHORT level)
{

	SHORT i, j;
	SHORT number;

	// ��ȿ�� �������� Ȯ���Ѵ�. 
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
			// �ڽ��� �����Ѵ�. 
			if(i==0 && j==0)continue;

			if(level==0)continue;

			// ������ ����͵� ���ܵȴ�. 
			if(x+j>=0 && x+j<MH.MapXsize && y+i>=0 && y+i<MH.MapYsize){}
			else continue;

			
		    
		    if(GetLevelMode(level, x+j, y+i) == 0)continue;

		       number=GetLevelNear2(x+j, y+i, level);

			// �ֺ��� ���� ����. 
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

					// �Ʒ����� ����ϸ�, 
	                if(level<(MAX_LEVEL_STEP-1))
					{
		               DeleteLevel(x+j, y+i, level+1);
					}
				}

			}

		}


}



/////////////////////////////////////////////////////////////////////////////
// ������� �Լ� 
/////////////////////////////////////////////////////////////////////////////

// ���̸� ���Ѵ�.������� -1�� �����ϰ� ������ ������ ���̸� �����Ѵ�. 
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