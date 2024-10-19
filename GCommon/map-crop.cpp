#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  ��� : ���°�
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

	// ��ȿ�� �������� Ȯ���Ѵ�. 
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

	// ��ȿ�� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// ���� ������ �����Ѵ�. 
	switch(type)
	{
	    case CROP_TYPE_RICE:
			start=CROP_RICE0;
			end=CROP_RICE3;
			mount=MAX_RICE_MOUNT;
 		    break;
 		
	    case CROP_TYPE_POTATO:   // �������߰�
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
	// ��ȿ�� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// ������ �����Ѵ�. 
	if(type==0)
	{
		PutCrop(x, y, type);
		DeleteCollMap(x,   y,   BLOCK_TREE);
	}
	// ������ �ɴ´�. 
	else
	{
      PutCrop(x, y, type);
	  SetCollMap(x,  y,   BLOCK_TREE);
	}
}


// ��� �ִ� ��ġ�ΰ�?
BOOL _Map::IsCrop(SHORT x, SHORT y)
{
	if(IsInMapArea(x, y) == FALSE)return FALSE;

	if(GetCropTypeMap(x, y))return TRUE;
	else return FALSE;
}


// idĳ���Ͱ� type�� ��� ��Ȯ�� �� �ִ°�?
BOOL IsCropHarvest(SHORT id, SHORT type)
{
	return FALSE;
}


// �ֺ��� ���� �ɱⰡ �����Ѱ�?
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
//  ��� : ���°�
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


	// ��ȿ�� �������� Ȯ���Ѵ�. 
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

	// ��ȿ�� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// ���� ������ �����Ѵ�. 
	switch(type)
	{
	    case CROP_TYPE_RICE:
			start=CROP_RICE0;
			end=CROP_RICE3;
			mount=MAX_RICE_MOUNT;
 		    break;
 		
	    case CROP_TYPE_POTATO:   // �������߰�
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
	//TreeType �� �����Ǿ� ������ 
	if( TreeType != -1 )
	{
		BlockSize = FV.m_TreeImgFile[TreeType].BlockSize;
		if( x%BlockSize ) x -= x%BlockSize;
		if( y%BlockSize ) y -= y%BlockSize;
	}
	else
	{
		MessageBox(NULL, "Tree Type�� �����Ͻÿ�(Set Tree Types!)", "", MB_OK);
		return;
	}

	//2x2ũ��� �ٲ�����Ƿ� 
	//if( x%BlockSize ) x--;
	//if( y%BlockSize ) y--;

	// ��ȿ�� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(x, y) == FALSE)return ;

	if(IsColl(x, y, BLOCK_DEEPWATER|BLOCK_CASTLE))return ;

	// ������ �����Ѵ�. 
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
	// ������ �ɴ´�. 
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


// ��� �ִ� ��ġ�ΰ�?
BOOL _Map::IsCrop(SHORT x, SHORT y)
{
	if(IsInMapArea(x, y) == FALSE)return FALSE;

	if(GetCropTypeMap(x, y))return TRUE;
	else return FALSE;
}


// idĳ���Ͱ� type�� ��� ��Ȯ�� �� �ִ°�?
BOOL IsCropHarvest(SHORT id, SHORT type)
{
	return FALSE;
}


// �ֺ��� ���� �ɱⰡ �����Ѱ�?
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