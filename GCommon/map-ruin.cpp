//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>

SI16 RuinSizeArray[][3] ={
		CROP_TYPE_RUIN0,       CROP_TYPE_RUIN0_XSIZE,		CROP_TYPE_RUIN0_YSIZE,
		CROP_TYPE_RUIN1,       CROP_TYPE_RUIN1_XSIZE,		CROP_TYPE_RUIN1_YSIZE,
		CROP_TYPE_RUIN2,       CROP_TYPE_RUIN2_XSIZE,		CROP_TYPE_RUIN2_YSIZE,
		CROP_TYPE_TUTTLE1,     CROP_TYPE_TUTTLE1_XSIZE,		CROP_TYPE_TUTTLE1_YSIZE,
		CROP_TYPE_SIGN1,       CROP_TYPE_SIGN1_XSIZE,	    CROP_TYPE_SIGN1_YSIZE,
		CROP_TYPE_FLAGK1,      CROP_TYPE_FLAGK1_XSIZE,      CROP_TYPE_FLAGK1_YSIZE,
		CROP_TYPE_FLAGC1,      CROP_TYPE_FLAGC1_XSIZE,      CROP_TYPE_FLAGC1_YSIZE,
		CROP_TYPE_FLAGJ1,      CROP_TYPE_FLAGJ1_XSIZE,      CROP_TYPE_FLAGJ1_YSIZE,
		CROP_TYPE_BROKENTREE1, CROP_TYPE_BROKENTREE1_XSIZE, CROP_TYPE_BROKENTREE1_YSIZE,
		CROP_TYPE_ODDTREE1,	   CROP_TYPE_ODDTREE1_XSIZE,    CROP_TYPE_ODDTREE1_YSIZE,
		

		CROP_TYPE_DOLL1,			CROP_TYPE_DOLL1_XSIZE,		CROP_TYPE_DOLL1_YSIZE,
		CROP_TYPE_WELL1,			CROP_TYPE_WELL1_XSIZE,	   	CROP_TYPE_WELL1_YSIZE,
		CROP_TYPE_STONE1,			CROP_TYPE_STONE1_XSIZE,	    CROP_TYPE_STONE1_YSIZE,
		CROP_TYPE_STONE2,			CROP_TYPE_STONE2_XSIZE,	    CROP_TYPE_STONE2_YSIZE,
		CROP_TYPE_AD11,				CROP_TYPE_AD11_XSIZE,        CROP_TYPE_AD11_YSIZE,
		CROP_TYPE_AD12,				CROP_TYPE_AD12_XSIZE,        CROP_TYPE_AD12_YSIZE,
		CROP_TYPE_AD13,				CROP_TYPE_AD13_XSIZE,        CROP_TYPE_AD13_YSIZE,
		CROP_TYPE_CASTLETOWER,		CROP_TYPE_CASTLETOWER_XSIZE,		CROP_TYPE_CASTLETOWER_YSIZE,
		CROP_TYPE_DESTROYEDCASTLE,  CROP_TYPE_DESTROYEDCASTLE_XSIZE,	CROP_TYPE_DESTROYEDCASTLE_YSIZE,
		CROP_TYPE_CASTLEGATE,		CROP_TYPE_CASTLEGATE_XSIZE,			CROP_TYPE_CASTLEGATE_YSIZE,
		CROP_TYPE_CASTLETILE,		CROP_TYPE_CASTLETILE_XSIZE,			CROP_TYPE_CASTLETILE_YSIZE,
		CROP_TYPE_CASTLELARGEGATE,	CROP_TYPE_CASTLELARGEGATE_XSIZE,	CROP_TYPE_CASTLELARGEGATE_YSIZE,
		CROP_TYPE_TEMPLE,			CROP_TYPE_TEMPLE_XSIZE,				CROP_TYPE_TEMPLE_YSIZE,
		CROP_TYPE_TOMB1,			CROP_TYPE_TOMB1_XSIZE,				CROP_TYPE_TOMB1_YSIZE,
		CROP_TYPE_VOLCANO,			5,									5,

	  -1,					 -1,                            -1 };

// 지도에 각종 지형지물을 넣는다. 
void _Map::PutRuin(SHORT x, SHORT y, SHORT type, SHORT mount)
{
	SHORT i, j;
	int  k = 0;
	SHORT tempx, tempy;
	SHORT xsize = - 1, ysize = -1;
	
	if(type==0)
	{
		type=GetCropTypeMap(x, y);
	}
	
	//
	while(1)
	{
		if(RuinSizeArray[k][0] == -1)	break;	
		if(type == RuinSizeArray[k][0])	
		{
			xsize = RuinSizeArray[k][1];
			ysize = RuinSizeArray[k][2];	
		}
		
		k++;
	}

	if( xsize == -1 )
	{
		//clGrp.Error("PutRuin", "%d", type);
		return ;
	}
	
	if( mount == 0 )
	{
		for(i=0;i<ysize;i++)
			for(j=0;j<xsize;j++)
			{
				
				tempx = x-xsize+1+j;
				tempy = y-ysize+1+i;
				
				// 충돌을 체크하게 만든다.
				if(IsInMapArea(tempx, tempy)==TRUE)
				{
					if( pCollMap )
					{
						if( pCollMap[tempx+YOff[tempy]] & BLOCK_RUIN )
						{
							pCollMap[tempx+YOff[tempy]] -= BLOCK_RUIN;
						}
					}						
					SetCropTypeMap(0, tempx,tempy);
					SetCropVaryMap(0, tempx,tempy);
					SetCropMountMap(0, tempx,tempy);
					SetCropStepMap(0, tempx,tempy);
				}
			}
	}
	else
	{						  
		SetCropTypeMap((unsigned char)type, x,y);
		
		// 충돌을 체크하게 만든다.
		for(i=0;i<ysize;i++)
			for(j=0;j<xsize;j++)
			{
				tempx=x-xsize+1+j;
				tempy=y-ysize+1+i;
				
				if(IsInMapArea(tempx, tempy)==TRUE)
				{
					if( pCollMap )
						pCollMap[tempx+YOff[tempy]] |= BLOCK_RUIN;
					
					SetCropVaryMap(0, tempx,tempy);
					SetCropMountMap((unsigned char)mount, tempx,tempy);
					SetCropStepMap(1, tempx,tempy);
				}
			}
			
	}
	
}
