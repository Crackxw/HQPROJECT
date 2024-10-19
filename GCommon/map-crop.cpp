#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include <CharOrg.h>

#include <map.h>

extern	_MainData		GD;
extern	_Char			CR[];
extern	_Map			Map;

void _Map::PutCrop(SHORT x, SHORT y, SHORT type, SHORT vary, SHORT mount)
{

	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(type==0)
	{
	   SetCropTypeMap(0, x, y);
	   SetCropVaryMap((unsigned char)vary, x, y);
	   SetCropMountMap(0, x, y);
	   SetCropStepMap(0, x, y);
	}
	else
	{
	   SetCropTypeMap((unsigned char)type, x, y);
	   SetCropVaryMap((unsigned char)vary, x, y);
	   SetCropMountMap((unsigned char)mount, x, y);
	   SetCropStepMap(1, x, y);
	}

}

void _Map::PutCrop(SHORT x, SHORT y, SHORT type)
{
	SHORT start, end;
	SHORT mount=0;

	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// 찍을 지점을 선택한다. 
	switch(type)
	{
	    case CROP_TYPE_RICE:
			start=CROP_RICE0;
			end=CROP_RICE3;
			mount=MAX_RICE_MOUNT;
 		    break;
 		
	    case CROP_TYPE_POTATO:   // 임진록추가
			start=CROP_POTATO0;
			end=CROP_POTATO2;
			mount=MAX_POTATO_MOUNT;
 		    break;
	    case CROP_TYPE_TREE:
			start=CROP_TREE0;
			end=CROP_TREE7;
			mount=MAX_TREE_MOUNT;
 		    break;

	    case CROP_TYPE_BAMBOO:
			start=CROP_BAMBOO0;
			end=CROP_BAMBOO4;
			mount=MAX_BAMBOO_MOUNT;
 		    break;

		default:
			start=CROP_RICE0;
			end=CROP_RICE3;
			mount=MAX_RICE_MOUNT;
 		    break;
	}
	
	SHORT vary=start+GD.Random()%(end-start+1);

	PutCrop(x, y, type, vary, mount);
}

void _Map::PutTree(SHORT x, SHORT y, SHORT type)
{
	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// 나무를 삭제한다. 
	if(type==0)
	{
		PutCrop(x, y, type);
		DeleteCollMap(x,   y,   BLOCK_TREE);
	}
	// 나무를 심는다. 
	else
	{
      PutCrop(x, y, type);
	  SetCollMap(x,  y,   BLOCK_TREE);
	}
}


// 곡물이 있는 위치인가?
BOOL _Map::IsCrop(SHORT x, SHORT y)
{
	if(IsInMapArea(x, y) == FALSE)return FALSE;

	if(GetCropTypeMap(x, y))return TRUE;
	else return FALSE;
}


// id캐릭터가 type의 곡물을 수확할 수 있는가?
BOOL IsCropHarvest(SHORT id, SHORT type)
{
	return FALSE;
}


// 주변에 나무 심기가 적당한가?
BOOL _Map::IsGoodForTree(SHORT x, SHORT y)
{

	SHORT i, j;

	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			if(IsInMapArea(x+j, y+i)==FALSE)continue;

			if(IsColl(x+j, y+i, BLOCK_NEWBLK)==TRUE)
			{
				return FALSE;
			}

		}
	
	return TRUE;

}




#else

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include <CharOrg.h>

#include <map.h>
#include "myfile.h"

extern	_MainData		GD;
extern	_Char			CR[];
extern	_Map			Map;
extern _FileVary		FV;
extern XSPR GameSpr[];

void _Map::PutCrop(SHORT x, SHORT y, SHORT type, SHORT vary, SHORT mount, SHORT TreeType)
{


	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(type==0)
	{
	   SetCropTypeMap(0, x, y);
	   SetCropVaryMap((unsigned char)vary, x, y);
	   SetCropMountMap(0, x, y);
	   SetCropStepMap(0, x, y);
	   SetCropTreeMap(0, x, y);
	}
	else
	{
	   SetCropTypeMap((unsigned char)type, x, y);
	   SetCropVaryMap((unsigned char)vary, x, y);
	   SetCropMountMap((unsigned char)mount, x, y);
	   SetCropStepMap(1, x, y);
   	   SetCropTreeMap((unsigned char) TreeType, x, y);

	}

}

void _Map::PutCrop(SHORT x, SHORT y, SHORT type, SHORT TreeType)
{
	SHORT start, end;
	SHORT mount=0;

	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// 찍을 지점을 선택한다. 
	switch(type)
	{
	    case CROP_TYPE_RICE:
			start=CROP_RICE0;
			end=CROP_RICE3;
			mount=MAX_RICE_MOUNT;
 		    break;
 		
	    case CROP_TYPE_POTATO:   // 임진록추가
			start=CROP_POTATO0;
			end=CROP_POTATO2;
			mount=MAX_POTATO_MOUNT;
 		    break;
	    case CROP_TYPE_TREE:			
			//start=CROP_TREE0;
			//end=CROP_TREE7;

			start = 0;
			end = GameSpr[FV.GetNowFile()].Header.font_number;

			mount=MAX_TREE_MOUNT;
 		    break;

	    case CROP_TYPE_BAMBOO:
			start=CROP_BAMBOO0;
			end=CROP_BAMBOO4;
			mount=MAX_BAMBOO_MOUNT;
 		    break;

		default:
			start=CROP_RICE0;
			end=CROP_RICE3;
			mount=MAX_RICE_MOUNT;
 		    break;
	}
	
	SHORT vary=start+GD.Random()%(end-start+1);

	PutCrop(x, y, type, vary, mount, TreeType);
}

void _Map::PutTree(SHORT x, SHORT y, SHORT type, SHORT TreeType)
{
		int BlockSize = 2;
	//TreeType 이 설정되어 있으면 
	if( TreeType != -1 )
	{
		BlockSize = FV.m_TreeImgFile[TreeType].BlockSize;
		if( x%BlockSize ) x -= x%BlockSize;
		if( y%BlockSize ) y -= y%BlockSize;
	}
	else
	{
		MessageBox(NULL, "Tree Type을 설정하시오(Set Tree Types!)", "", MB_OK);
		return;
	}

	//2x2크기로 바뀌었으므로 
	//if( x%BlockSize ) x--;
	//if( y%BlockSize ) y--;

	// 유효한 영역인지 확인한다. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// 나무를 삭제한다. 
	if(type==0)
	{
		PutCrop(x, y, type, TreeType);
		DeleteCollMap(x,   y,   BLOCK_TREE);
		for( int i = 0; i < BlockSize; i++)
		{
			for( int j =0; j< BlockSize; j++)
			{
				if(IsInMapArea(x+j, y+i) == TRUE) DeleteCollMap(x+j,   y+i,   BLOCK_TREE);
			}
		}	
	}
	// 나무를 심는다. 
	else
	{
      	PutCrop(x, y, type, TreeType);
		SetCollMap(x,  y,   BLOCK_TREE);
		for( int i = 0; i < BlockSize; i++)
		{
			for( int j =0; j< BlockSize; j++)
			{
				if(IsInMapArea(x+j, y+i) == TRUE) SetCollMap(x+j,   y+i,   BLOCK_TREE);
			}
		}
	}
}


// 곡물이 있는 위치인가?
BOOL _Map::IsCrop(SHORT x, SHORT y)
{
	if(IsInMapArea(x, y) == FALSE)return FALSE;

	if(GetCropTypeMap(x, y))return TRUE;
	else return FALSE;
}


// id캐릭터가 type의 곡물을 수확할 수 있는가?
BOOL IsCropHarvest(SHORT id, SHORT type)
{
	return FALSE;
}


// 주변에 나무 심기가 적당한가?
BOOL _Map::IsGoodForTree(SHORT x, SHORT y)
{

	SHORT i, j;

	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			if(IsInMapArea(x+j, y+i)==FALSE)continue;

			if(IsColl(x+j, y+i, BLOCK_NEWBLK)==TRUE)
			{
				return FALSE;
			}

		}
	
	return TRUE;

}


#endif