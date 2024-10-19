#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include <etc.h>
#include <config.h>

#include <myfile.h>

#include <map.h>
#include <cltString.h>
#include <OnlineWorld.h>
#include "bindJxFile.h"

// robypark 2004/9/30 19:9
// �¶��ΰ��� �ļ� ���
#include "../SCCommon/OnlineCommonParser.h"

extern _clGame* pGame;
extern	_Config			Config;
extern	XSPR			TileSpr[];
extern	char			SavePath[];
extern	XSPR			PannelSpr;
extern clock_t			CurrentClock;


void InitTileFile();
BOOL LoadTileFile();
// �ٴ� Ÿ�� ������ �ҷ��´�.
BOOL LoadSeaType();
BOOL LoadNewBlkType();
// ���� ���� Ÿ�� �������� �ҷ��´�. 
BOOL LoadCastleType();

//------------------------------------
// �׸������� ���� ���� 
//------------------------------------
XSPR GameSpr[MAX_GAME_SPR_NUMBER];      // �Ϲ������� ���� ��������Ʈ 


int SetWordToFileDefine(char* source_string, char* target_string);


cltString clTileID[MAX_MAP_TILE_SPR_NUMBER];
cltString clTileName[MAX_MAP_TILE_SPR_NUMBER]; 

int FileTileGrss1;
int FileTileDiff1;
int FileTileHill0;
int FileTileHill1;
int FileTileNewBlkGate;
int FileTileSea0;
int FileTileCastle2;
int	FileTileBlackTile;
int	FileTileShallow;

extern unsigned char SeaType[15][MAX_SEA_VARY*2];
extern unsigned char NewBlkType[15][MAX_NEWBLK_VARY*2];
extern unsigned char CastleType[15][MAX_CASTLE_VARY*2];

typedef struct 
{
	char	    clImgFileId[64];
	SI32		siImgFileValue;
	char 	    clImgFileName[64];
	SI32		siPreLoad;
	DWORD		LoadedClock;
} _ImgFileList;

_ImgFileList ImgFileList[MAX_GAME_SPR_NUMBER];


void InitTileFile()
{
	FileTileGrss1		= GetValueOfTileID("FILE_TILE_GRSS1");
	FileTileDiff1		= GetValueOfTileID("FILE_TILE_DIFF1");
	FileTileHill0		= GetValueOfTileID("FILE_TILE_HILL0");
	FileTileHill1		= GetValueOfTileID("FILE_TILE_HILL1");
	FileTileNewBlkGate  = GetValueOfTileID("FILE_TILE_NEWBLKGATE");
	FileTileSea0		= GetValueOfTileID("FILE_TILE_SEA0");
	FileTileCastle2		= GetValueOfTileID("FILE_TILE_CASTLE2");
	FileTileBlackTile	= GetValueOfTileID("FILE_TILE_BLACKTILE");
	FileTileShallow		= GetValueOfTileID("FILE_TILE_SHALLOW");
}

// ������ Ÿ���� �ҷ��´�.
void LoadMapTile(SHORT maptype)
{
	// �о�;� �� ������ �м��Ѵ�. 
	
	LoadTileFile();
	InitTileFile();
	// �ٴ� Ÿ�� ���� �ҷ�����
	LoadSeaType();	
	LoadNewBlkType();
	LoadCastleType();
	

	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;


	// ������ ������ ������ �о�´�. 
   while(!exit)
   {
	   switch(maptype)
	   {
	   case MAP_TYPE_NORMAL:
		   wsprintf(buffer, "tile\\normal\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_SNOW:
		   wsprintf(buffer, "tile\\snow\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_BROWN:
		   wsprintf(buffer, "tile\\brown\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_GREEN:
		   wsprintf(buffer, "tile\\green\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;
		   
	   case MAP_TYPE_DARK:
		   wsprintf(buffer, "tile\\dark\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_VOLCANO:
		   wsprintf(buffer, "tile\\volcano\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_BRIGHT:
		   wsprintf(buffer, "tile\\bright\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   default:
		   wsprintf(buffer, "tile\\normal\\");
		   strcat(buffer, clTileName[index].GetString());
		   clGrp.Error("32947", "ds082:%d", maptype);
		   break;
	   }

	   // savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
	   GetFileNamePath(buffer, SavePath, buffer);

	   if(clGrp.LoadXspr(buffer, TileSpr[index])==FALSE)
	   {
		   if(errorcheckswitch==FALSE)
		   {
			   errorcheckswitch=TRUE;
		      wsprintf(buffer, "LoadMapTile, name:%s index:%d", buffer, index);
			  
			  clGrp.FlipToGDISurface();
		      MessageBox(pGame->Hwnd, buffer, "Error", MB_OK);
		   }
	   }
	   index++;

		if( index >= MAX_MAP_TILE_SPR_NUMBER )
			break;
   }
}


// ������ Ÿ���� �����Ѵ�.
void FreeMapTile()
{

	SHORT i;

	for(i=0;i<MAX_MAP_TILE_SPR_NUMBER;i++)
	{
		if(TileSpr[i].Image)
		{
			clGrp.FreeXspr(TileSpr[i]);
			TileSpr[i].Image=NULL;
		}
	}

}

// �Ϲ� �׸����� �ҷ��´�. 
void LoadFnt()
{
	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	char buffer2[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;

	while(!exit)
	{
		// ó���� �ε����� �ʴ´ٴ� �÷��׸� ���� spr ������ �ε����� �ʴ´�.
//		ImgFileList[index].siPreLoad = 0;

		if( ImgFileList[index].siPreLoad == 1)
		{
/*
			if(ImgFileList[index].siImgFileValue == FILE_ON_PORTRAIT)
			{
				if(g_fTestVersion == FALSE)
				{
					switch(pGame->LanguageCode)
					{
					case ELANGAGECODE_TAIWAN:	case ELANGAGECODE_CHINA:	case ELANGAGECODE_HONGKONG:		case ELANGAGECODE_JAPAN:
						strcpy(ImgFileList[index].clImgFileName, "fnt\\Taiwan_Portrait_online.Spr");
						break;
					}
				}
			}
*/		
			// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
			GetFileNamePath(ImgFileList[index].clImgFileName, SavePath, buffer);
			
			if(clGrp.LoadXspr(buffer, GameSpr[ImgFileList[index].siImgFileValue])==FALSE)
			{
				if(errorcheckswitch==FALSE)
				{
					errorcheckswitch=TRUE;
					wsprintf(buffer2, "LoadFnt, name:%s index:%d", buffer, index);
					
					clGrp.Error("Error", buffer2);
				}
			}
		}


		index++;
		
		if(index >= MAX_GAME_SPR_NUMBER)
		{
			exit=TRUE;
		}
	}


}


BOOL FreeFnt()
{
	SHORT i;

	for(i = 0;i < MAX_GAME_SPR_NUMBER;i++)
	{
		if(GameSpr[i].Image)
		{
			clGrp.FreeXspr(GameSpr[i]);
			GameSpr[i].Image=NULL;
		}
	}


	if( PannelSpr.Image )
	{
		clGrp.FreeXspr(PannelSpr);
		PannelSpr.Image = NULL;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL LoadTileFile()
{
	FILE	*fp = NULL;
	char	buffer[128], trimBuffer[128];
	int		index = 0;
	char	c;
	char	id[64], name[64];

	int		i = 0, j = 0;
	BOOL	bRet = TRUE;

	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "TileFile.txt" );

	if( fp == NULL )
	{
		clGrp.Error("LoadTileFile", "%s - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}

	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));

		// �� ���� �о�´�. 
		if( fgets(buffer, 128, fp) == NULL )break;

		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				trimBuffer[index] = ' ';
				index++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				break;
			}
		}

		if( trimBuffer[0] != '\0' )
		{
			sscanf(trimBuffer, "%s %s", id, name);
			clTileID[j] = id;
			clTileName[j] = name;
	
			j++;
		}
		if( j >= MAX_MAP_TILE_SPR_NUMBER )	break;
	}

END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}


SI32 GetValueOfTileID(CHAR *TileID)
{
	SI32 i;

	//TileID�� NULL�ΰ�� 

	for( i=0; i<MAX_MAP_TILE_SPR_NUMBER; i++ )
	{
		if(clTileID[i].GetString() && (TileID != NULL))
		{
			if( strcmp(clTileID[i].GetString(), TileID) == 0 )
				return i;
		}
	}
	//clGrp.Error("GetValueOfTileID", "[%s] - ã�� ����", TileID);
	return 0;
}


BOOL LoadSeaType()
{
	FILE	*fp = NULL;
	char	buffer[256], trimBuffer[256];
	int		index = 0;
	char	c;
	
	int		i = 0;
	int		word = 0;
	int		p = 0;
	BOOL	bRet = TRUE;
	
	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
	//GetFileNamePath(, SavePath, buffer);
	
	fp = BindJXFile::GetInstance()->GetFile( "SeaType.txt" );

	if( fp == NULL )
	{
		clGrp.Error("LoadSeaType", "%s - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}
	
	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));
		
		// �� ���� �о�´�. 
		if( fgets(buffer, 256, fp) == NULL )break;
		
		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				if( word >= MAX_SEA_VARY*2 )
				{
					word = 0;
					p++;
				}
				if( p >= 15 )
					clGrp.Error("LoadSeaType", "�ִ� ���� �ʰ�(MAX COUNT OVER!!)");
				
				trimBuffer[index] = '\0';
				
				
				if( word % 2 == 0 )
					SeaType[p][word] = GetValueOfTileID(trimBuffer);
				else
					SeaType[p][word] = atoi(trimBuffer);
				index = 0;
				word++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				index = 0;
				break;
			}
		}
	}
	
END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}


BOOL LoadNewBlkType()
{
	FILE	*fp = NULL;
	char	buffer[256], trimBuffer[256];
	int		index = 0;
	char	c;

	int		i = 0;
	int		word = 0;
	int		p = 0;
	BOOL	bRet = TRUE;

   // savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//   GetFileNamePath(, SavePath, buffer);

   fp = BindJXFile::GetInstance()->GetFile( "NewBlkType.txt" );
	if( fp == NULL )
	{
		clGrp.Error("LoadNewBlkType", "[%s] - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}

	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));

		// �� ���� �о�´�. 
		if( fgets(buffer, 256, fp) == NULL )break;

		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				if( word >= MAX_NEWBLK_VARY*2 )
				{
					word = 0;
					p++;
				}
				if( p >= 15 )
					clGrp.Error("LoadNewBlkType", "�ִ� ���� �ʰ�(MAX COUNT OVER!!)");

				trimBuffer[index] = '\0';


				if( word % 2 == 0 )
					NewBlkType[p][word] = GetValueOfTileID(trimBuffer);
				else
					NewBlkType[p][word] = atoi(trimBuffer);

				index = 0;
				word++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				index = 0;
				break;
			}
		}
	}

END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}


BOOL LoadCastleType()
{
	FILE	*fp = NULL;
	char	buffer[256], trimBuffer[256];
	int		index = 0;
	char	c;

	int		i = 0;
	int		word = 0;
	int		p = 0;
	BOOL	bRet = TRUE;

   // savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//   GetFileNamePath(, SavePath, buffer);

   fp = BindJXFile::GetInstance()->GetFile( "CastleType.txt" );
	if( fp == NULL )
	{
		clGrp.Error("LoadCastleType", "%s - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}

	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));

		// �� ���� �о�´�. 
		if( fgets(buffer, 256, fp) == NULL )break;

		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				if( word >= MAX_CASTLE_VARY*2 )
				{
					word = 0;
					p++;
				}
				if( p >= 15 )
					clGrp.Error("LoadCastleType", "�ִ� ���� �ʰ�(MAX COUNT OVER!!)");

				trimBuffer[index] = '\0';


				if( word % 2 == 0 )
					CastleType[p][word] = GetValueOfTileID(trimBuffer);
				else
					CastleType[p][word] = atoi(trimBuffer);
				index = 0;
				word++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				index = 0;
				break;
			}
		}
	}

END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}

BOOL LoadImgFileList()
{
	// robypark 2004/9/30 19:9
	// �¶��� ���� �ļ��� �̿��ϵ��� ����
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "ImgFile.txt";

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Image List File Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		SI32	siIndex, siPreLoad;

		char	pszResourceID[128], pszFilePath[512], pszTemp[256];

		ocp.GetValue( pszResourceID,		127,	iLineNo, iColNo );		// Resource ID
		ocp.GetValue( siIndex,						iLineNo, iColNo );		// Index
		ocp.GetValue( pszFilePath,			511,	iLineNo, iColNo );		// Resource File Path
		ocp.GetValue( siPreLoad,					iLineNo, iColNo );		// PreLoad
		ocp.GetValue( pszTemp,				255,	iLineNo, iColNo );		// Description
		
		ImgFileList[siIndex].siImgFileValue	= siIndex;
		ImgFileList[siIndex].siPreLoad		= siPreLoad;
		strcpy(ImgFileList[siIndex].clImgFileId, pszResourceID);
		strcpy(ImgFileList[siIndex].clImgFileName, pszFilePath);

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
/*
	FILE	*fp;

	CHAR	buffer[1024], temp[5];
	CHAR	delimiter[] = "`\t\n\r";
	CHAR	*pToken = NULL;
	CHAR	imgFileId[256], imgFileName[256];
	SI32 value, preloadswitch;
	
	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "ImgFile.txt" );
	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// ù��° ������ �����Ѵ�. 
	fgets(buffer, 1024, fp);

	while( fgets(buffer, 1024, fp) != NULL  )
	{ 
		if( strlen(buffer) < 5 )			continue;

		ZeroMemory(temp, sizeof(temp));
		memcpy(temp, buffer, sizeof(temp)-1);

		// �ּ����� �����ϴ� ���� �ǳʶڴ�.
		if( strstr(temp, "//") != NULL )	continue;	

		sscanf(buffer, "%s %d %s %d", imgFileId, &value, imgFileName, &preloadswitch);
		

		ImgFileList[value].siImgFileValue	= value;
		ImgFileList[value].siPreLoad		= preloadswitch;
		strcpy(ImgFileList[value].clImgFileId,imgFileId);
		strcpy(ImgFileList[value].clImgFileName,imgFileName);
	}
	fclose(fp);



	return TRUE;
*/
}



SI32 GetValueOfGameSprNumID(CHAR *ID)
{
	SI32 i;

	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{


		if(ImgFileList[i].clImgFileId[0] == 0)continue;

		if( strcmp(ImgFileList[i].clImgFileId, ID) == 0 )
		{
			return ImgFileList[i].siImgFileValue;
		}
	}
	clGrp.Error("GetValueOfGame", "[%s]�� �ش��ϴ� ID ����(Cannot Find ID [%s]!)", ID, ID);
	return -1;
}



void _FileVary::Init()
{
#ifdef _IMJIN2PLUS
	OnlineLogoFile	= GetValueOfGameSprNumID("FILE_IMJINONLINE");
	LogoFile		= GetValueOfGameSprNumID("FILE_IMJIN2PLUS");
	TreeFile		= GetValueOfGameSprNumID("FILE_TREE_PLUS");
#else
	LogoFile		= GetValueOfGameSprNumID("FILE_IMJIN2");
	TreeFile		= GetValueOfGameSprNumID("FILE_TREE0");
#endif
}


// ������ ������ ���� �����ϰ� �ǳ��� �ҷ��´�. 
BOOL LoadPannel(SI32 sicountry)
{

	char buffer[128];


	// �¶��� ���� ���������� �ǳ�. 
	strcpy(buffer, "fnt\\pannel_online.spr");

	GetFileNamePath(buffer, SavePath, buffer);

	if(PannelSpr.Image)
	{
		clGrp.FreeXspr(PannelSpr);
		PannelSpr.Image = NULL;
	}

	clGrp.LoadXspr(buffer, PannelSpr);

	return TRUE;

}


// num �� �ش��ϴ� �̹��� ������ �ε��Ѵ�.
BOOL LoadImgFile(SI32 num)
{
	SI32 fileNum;
	CHAR buffer[FILE_NAME_SIZE];
	SI32 i;

	fileNum = -1;
	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( num == ImgFileList[i].siImgFileValue )
		{
			fileNum = ImgFileList[i].siImgFileValue;
			break;
		}
	}

	if( fileNum == -1 )
	{
		clGrp.Error("LoadImgFile", "[%d]�� �ش��ϴ� �� ����(Cannot Find Value [%d]!)", num, num);
		return FALSE;
	}

	// �̸� �ε��� �� ���� siPreLoad�� 1�̴�.
	if( ImgFileList[fileNum].siPreLoad == 1 )
		return TRUE;


	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
	GetFileNamePath(ImgFileList[fileNum].clImgFileName, SavePath, buffer);

	if( clGrp.LoadXspr(buffer, GameSpr[fileNum]) == FALSE )
	{
		clGrp.Error("LoadImgFile - Load Error", buffer);
	}

	// �̹����� �ε��� �ð��� �����Ѵ�.
	// �Ŀ� ���� ������ �̹��� ���� �����ϱ� ����. 
	ImgFileList[fileNum].LoadedClock = CurrentClock;	
	return TRUE;
}


// �����߿� �ε��� �̹��� ������ �޸𸮸� ��� �����Ѵ�.
void FreeImgFile()
{
	SI32 i;
	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( ImgFileList[i].siPreLoad == 0 )
		{
			if( GameSpr[i].Image )
			{
				// �̹����� �ε��� �ð��� �ʱ�ȭ�Ѵ�. 
				ImgFileList[i].LoadedClock = 0;
				clGrp.FreeXspr(GameSpr[i]);
				GameSpr[i].Image = NULL;
			}
		}
	}
}

// �ε��� �ð��� �ٰŷ� ������ �̹����� �����Ѵ�.
void FreeOldImageByTime()
{
	SI32 i;
	SI32 oldimageindex = -1;
	SI32 oldclock = 0;
	SI32 gab;

	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( ImgFileList[i].siPreLoad == 0 )
		{
			if( GameSpr[i].Image && ImgFileList[i].LoadedClock != 0) // �̹����� �ְ�. �̹����� �ε��� �ð��� ��ϵǾ� �ְ�. 
			{
				gab = abs(CurrentClock - ImgFileList[i].LoadedClock);

				if(gab > 300000	&& oldclock < gab)					// �ε����� �ּ��� 10�� �̻� ����Ͽ���.
				{
					oldclock = gab;
					oldimageindex = i;
					break;					
				}
			}
		}
	}

	if(oldimageindex  >= 0)
	{
		// �̹����� �ε��� �ð��� �ʱ�ȭ�Ѵ�. 
		ImgFileList[oldimageindex].LoadedClock = 0;

		clGrp.FreeXspr(GameSpr[oldimageindex]);
		GameSpr[oldimageindex].Image = NULL;
	}
}



#else
//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include <etc.h>
#include <config.h>

#include <myfile.h>

#include <map.h>
#include <cltString.h>
#include <OnlineWorld.h>
#include "bindJxFile.h"

extern _clGame* pGame;
extern	_Config			Config;
extern	XSPR			TileSpr[];
extern	char			SavePath[];
extern	XSPR			PannelSpr;
extern clock_t			CurrentClock;


void InitTileFile();
BOOL LoadTileFile();
// �ٴ� Ÿ�� ������ �ҷ��´�.
BOOL LoadSeaType();
BOOL LoadNewBlkType();
// ���� ���� Ÿ�� �������� �ҷ��´�. 
BOOL LoadCastleType();

//------------------------------------
// �׸������� ���� ���� 
//------------------------------------
XSPR GameSpr[MAX_GAME_SPR_NUMBER];      // �Ϲ������� ���� ��������Ʈ 


int SetWordToFileDefine(char* source_string, char* target_string);


cltString clTileID[MAX_MAP_TILE_SPR_NUMBER];
cltString clTileName[MAX_MAP_TILE_SPR_NUMBER]; 

int FileTileGrss1;
int FileTileDiff1;
int FileTileHill0;
int FileTileHill1;
int FileTileNewBlkGate;
int FileTileSea0;
int FileTileCastle2;
int	FileTileBlackTile;
int	FileTileShallow;

extern unsigned char SeaType[15][MAX_SEA_VARY*2];
extern unsigned char NewBlkType[15][MAX_NEWBLK_VARY*2];
extern unsigned char CastleType[15][MAX_CASTLE_VARY*2];

typedef struct 
{
	char	    clImgFileId[64];
	SI32		siImgFileValue;
	char 	    clImgFileName[64];
	SI32		siPreLoad;
	DWORD		LoadedClock;
} _ImgFileList;

_ImgFileList ImgFileList[MAX_GAME_SPR_NUMBER];


void InitTileFile()
{
	FileTileGrss1		= GetValueOfTileID("FILE_TILE_GRASS");//���-����
	FileTileDiff1		= GetValueOfTileID("FILE_TILE_MUD");//���-����
	FileTileHill0		= GetValueOfTileID("FILE_TILE_NORMAL");
	FileTileHill1		= GetValueOfTileID("FILE_TILE_ROAD");//���-��
	FileTileNewBlkGate  = GetValueOfTileID("FILE_TILE_NEWBLKGATE");
	FileTileSea0		= GetValueOfTileID("FILE_TILE_SEA0");
	FileTileCastle2		= GetValueOfTileID("FILE_TILE_CASTLE2");
	FileTileBlackTile	= GetValueOfTileID("FILE_TILE_BLACKTILE");
	FileTileShallow		= -1;
	/*
	FileTileGrss1		= GetValueOfTileID("FILE_TILE_GRSS1");
	FileTileDiff1		= GetValueOfTileID("FILE_TILE_DIFF1");
	FileTileHill0		= GetValueOfTileID("FILE_TILE_HILL0");
	FileTileHill1		= GetValueOfTileID("FILE_TILE_HILL1");
	FileTileNewBlkGate  = GetValueOfTileID("FILE_TILE_NEWBLKGATE");
	FileTileSea0		= GetValueOfTileID("FILE_TILE_SEA0");
	FileTileCastle2		= GetValueOfTileID("FILE_TILE_CASTLE2");
	FileTileBlackTile	= GetValueOfTileID("FILE_TILE_BLACKTILE");
	FileTileShallow		= GetValueOfTileID("FILE_TILE_SHALLOW");
	*/
}

// ������ Ÿ���� �ҷ��´�.
void LoadMapTile(SHORT maptype)
{
	// �о�;� �� ������ �м��Ѵ�. 
	
	LoadTileFile();
	InitTileFile();
	// �ٴ� Ÿ�� ���� �ҷ�����
	LoadSeaType();	
	LoadNewBlkType();
	LoadCastleType();
	

	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;


	// ������ ������ ������ �о�´�. 
   while(!exit)
   {
	   switch(maptype)
	   {
	   case MAP_TYPE_NORMAL:
		   wsprintf(buffer, "tile\\normal\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_SNOW:
		   wsprintf(buffer, "tile\\snow\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_BROWN:
		   wsprintf(buffer, "tile\\brown\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_GREEN:
		   wsprintf(buffer, "tile\\green\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;
		   
	   case MAP_TYPE_DARK:
		   wsprintf(buffer, "tile\\dark\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_VOLCANO:
		   wsprintf(buffer, "tile\\volcano\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   case MAP_TYPE_BRIGHT:
		   wsprintf(buffer, "tile\\bright\\");
		   strcat(buffer, clTileName[index].GetString());
		   break;

	   default:
		   wsprintf(buffer, "tile\\normal\\");
		   strcat(buffer, clTileName[index].GetString());
		   clGrp.Error("32947", "ds082:%d", maptype);
		   break;
	   }

	   // savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
	   GetFileNamePath(buffer, SavePath, buffer);

	   //��� �ϴ� �ӽ÷� �ε�..
	   //if( maptype == MAP_TYPE_NORMAL && index == 0)
		//if( maptype == MAP_TYPE_NORMAL && index < 2)
	   if(1)
	   {
		   TileSpr[index].Image = NULL;
		   if( clGrp.LoadXsprWithAllocMem( buffer, TileSpr[index],MAX_ONLINE_TILEDATA_IMAGESIZE ) == FALSE)
		   //if(clGrp.LoadXspr("C:\\������\\��� �컩���\\tile\\TA_SO_grass.Spr", TileSpr[index])==FALSE)
		   {
			   errorcheckswitch=TRUE;
			   wsprintf(buffer, "LoadMapTile, name:%s index:%d", buffer, index);
			   
			   clGrp.FlipToGDISurface();
			   MessageBox(pGame->Hwnd, buffer, "Error", MB_OK);
		   }
	   }
	   else
	   {		   
		   if(clGrp.LoadXspr(buffer, TileSpr[index])==FALSE)
		   {
			   if(errorcheckswitch==FALSE)
			   {
				   errorcheckswitch=TRUE;
				   wsprintf(buffer, "LoadMapTile, name:%s index:%d", buffer, index);
				   
				   clGrp.FlipToGDISurface();
				   MessageBox(pGame->Hwnd, buffer, "Error", MB_OK);
			   }
		   }
	   }

	   index++;

		if( index >= MAX_MAP_TILE_SPR_NUMBER )
			break;
   }
}


// ������ Ÿ���� �����Ѵ�.
void FreeMapTile()
{

	SHORT i;

	for(i=0;i<MAX_MAP_TILE_SPR_NUMBER;i++)
	{
		if(TileSpr[i].Image)
		{
			clGrp.FreeXspr(TileSpr[i]);
			TileSpr[i].Image=NULL;
		}
	}

}

// �Ϲ� �׸����� �ҷ��´�. 
void LoadFnt()
{
	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	char buffer2[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;

	while(!exit)
	{
		// ó���� �ε����� �ʴ´ٴ� �÷��׸� ���� spr ������ �ε����� �ʴ´�.
//		ImgFileList[index].siPreLoad = 0;

		if( ImgFileList[index].siPreLoad == 1)
		{
/*
			if(ImgFileList[index].siImgFileValue == FILE_ON_PORTRAIT)
			{
				if(g_fTestVersion == FALSE)
				{
					switch(pGame->LanguageCode)
					{
					case ELANGAGECODE_TAIWAN:	case ELANGAGECODE_CHINA:	case ELANGAGECODE_HONGKONG:		case ELANGAGECODE_JAPAN:
						strcpy(ImgFileList[index].clImgFileName, "fnt\\Taiwan_Portrait_online.Spr");
						break;
					}
				}
			}
*/		
			// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
			GetFileNamePath(ImgFileList[index].clImgFileName, SavePath, buffer);
			
			if(clGrp.LoadXspr(buffer, GameSpr[ImgFileList[index].siImgFileValue])==FALSE)
			{
				if(errorcheckswitch==FALSE)
				{
					errorcheckswitch=TRUE;
					wsprintf(buffer2, "LoadFnt, name:%s index:%d", buffer, index);
					
					clGrp.Error("Error", buffer2);
				}
			}
		}


		index++;
		
		if(index >= MAX_GAME_SPR_NUMBER)
		{
			exit=TRUE;
		}
	}


}


BOOL FreeFnt()
{
	SHORT i;

	for(i = 0;i < MAX_GAME_SPR_NUMBER;i++)
	{
		if(GameSpr[i].Image)
		{
			clGrp.FreeXspr(GameSpr[i]);
			GameSpr[i].Image=NULL;
		}
	}


	if( PannelSpr.Image )
	{
		clGrp.FreeXspr(PannelSpr);
		PannelSpr.Image = NULL;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL LoadTileFile()
{
	FILE	*fp = NULL;
	char	buffer[128], trimBuffer[128];
	int		index = 0;
	char	c;
	char	id[64], name[64];

	int		i = 0, j = 0;
	BOOL	bRet = TRUE;

	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "TileFile.txt" );

	if( fp == NULL )
	{
		clGrp.Error("LoadTileFile", "%s - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}

	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));

		// �� ���� �о�´�. 
		if( fgets(buffer, 128, fp) == NULL )break;

		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				trimBuffer[index] = ' ';
				index++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				break;
			}
		}

		if( trimBuffer[0] != '\0' )
		{
			sscanf(trimBuffer, "%s %s", id, name);
			clTileID[j] = id;
			clTileName[j] = name;
	
			j++;
		}
		if( j >= MAX_MAP_TILE_SPR_NUMBER )	break;
	}

END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}


SI32 GetValueOfTileID(CHAR *TileID)
{
	SI32 i;

	//TileID�� NULL�ΰ�� 

	for( i=0; i<MAX_MAP_TILE_SPR_NUMBER; i++ )
	{
		if(clTileID[i].GetString() && (TileID != NULL))
		{
			if( strcmp(clTileID[i].GetString(), TileID) == 0 )
				return i;
		}
	}
	//clGrp.Error("GetValueOfTileID", "[%s] - ã�� ����", TileID);
	return 0;
}


BOOL LoadSeaType()
{
	FILE	*fp = NULL;
	char	buffer[256], trimBuffer[256];
	int		index = 0;
	char	c;
	
	int		i = 0;
	int		word = 0;
	int		p = 0;
	BOOL	bRet = TRUE;
	
	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
	//GetFileNamePath(, SavePath, buffer);
	
	fp = BindJXFile::GetInstance()->GetFile( "SeaType.txt" );

	if( fp == NULL )
	{
		clGrp.Error("LoadSeaType", "%s - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}
	
	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));
		
		// �� ���� �о�´�. 
		if( fgets(buffer, 256, fp) == NULL )break;
		
		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				if( word >= MAX_SEA_VARY*2 )
				{
					word = 0;
					p++;
				}
				if( p >= 15 )
					clGrp.Error("LoadSeaType", "�ִ� ���� �ʰ�");
				
				trimBuffer[index] = '\0';
				
				
				if( word % 2 == 0 )
					SeaType[p][word] = GetValueOfTileID(trimBuffer);
				else
					SeaType[p][word] = atoi(trimBuffer);
				index = 0;
				word++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				index = 0;
				break;
			}
		}
	}
	
END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}


BOOL LoadNewBlkType()
{
	FILE	*fp = NULL;
	char	buffer[256], trimBuffer[256];
	int		index = 0;
	char	c;

	int		i = 0;
	int		word = 0;
	int		p = 0;
	BOOL	bRet = TRUE;

   // savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//   GetFileNamePath(, SavePath, buffer);

   fp = BindJXFile::GetInstance()->GetFile( "NewBlkType.txt" );
	if( fp == NULL )
	{
		clGrp.Error("LoadNewBlkType", "[%s] - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}

	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));

		// �� ���� �о�´�. 
		if( fgets(buffer, 256, fp) == NULL )break;

		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				if( word >= MAX_NEWBLK_VARY*2 )
				{
					word = 0;
					p++;
				}
				if( p >= 15 )
					clGrp.Error("LoadNewBlkType", "�ִ� ���� �ʰ�");

				trimBuffer[index] = '\0';


				if( word % 2 == 0 )
					NewBlkType[p][word] = GetValueOfTileID(trimBuffer);
				else
					NewBlkType[p][word] = atoi(trimBuffer);

				index = 0;
				word++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				index = 0;
				break;
			}
		}
	}

END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}


BOOL LoadCastleType()
{
	FILE	*fp = NULL;
	char	buffer[256], trimBuffer[256];
	int		index = 0;
	char	c;

	int		i = 0;
	int		word = 0;
	int		p = 0;
	BOOL	bRet = TRUE;

   // savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//   GetFileNamePath(, SavePath, buffer);

   fp = BindJXFile::GetInstance()->GetFile( "CastleType.txt" );
	if( fp == NULL )
	{
		clGrp.Error("LoadCastleType", "%s - File Open Error!", buffer);
		bRet = FALSE;
		goto END;
	}

	while( 1 )
	{		
		ZeroMemory(buffer, sizeof(buffer));
		ZeroMemory(trimBuffer, sizeof(trimBuffer));

		// �� ���� �о�´�. 
		if( fgets(buffer, 256, fp) == NULL )break;

		i = index = 0;
		while( 1 )
		{
			c = buffer[i++];
			if( c == ',' )
			{
				if( word >= MAX_CASTLE_VARY*2 )
				{
					word = 0;
					p++;
				}
				if( p >= 15 )
					clGrp.Error("LoadCastleType", "�ִ� ���� �ʰ�");

				trimBuffer[index] = '\0';


				if( word % 2 == 0 )
					CastleType[p][word] = GetValueOfTileID(trimBuffer);
				else
					CastleType[p][word] = atoi(trimBuffer);
				index = 0;
				word++;
			}
			else if( isalpha(c) || isdigit(c) || c == '_' || c == '-' || c == '.' )
			{
				trimBuffer[index] = c;
				index++;
			}
			else if( c == '\n' )
			{
				trimBuffer[index] = '\0';
				index = 0;
				break;
			}
		}
	}

END:
	if( fp )
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}

BOOL LoadImgFileList()
{
	// robypark 2004/9/30 19:9
	// �¶��� ���� �ļ��� �̿��ϵ��� ����
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "ImgFile.txt";

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Image List File Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		SI32	siIndex, siPreLoad;

		char	pszResourceID[128], pszFilePath[512], pszTemp[256];

		ocp.GetValue( pszResourceID,		127,	iLineNo, iColNo );		// Resource ID
		ocp.GetValue( siIndex,						iLineNo, iColNo );		// Index
		ocp.GetValue( pszFilePath,			511,	iLineNo, iColNo );		// Resource File Path
		ocp.GetValue( siPreLoad,					iLineNo, iColNo );		// PreLoad
		ocp.GetValue( pszTemp,				255,	iLineNo, iColNo );		// Description
		
		ImgFileList[siIndex].siImgFileValue	= siIndex;
		ImgFileList[siIndex].siPreLoad		= siPreLoad;
		strcpy(ImgFileList[siIndex].clImgFileId, pszResourceID);
		strcpy(ImgFileList[siIndex].clImgFileName, pszFilePath);

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
/*
	FILE	*fp;

	CHAR	buffer[1024], temp[5];
	CHAR	delimiter[] = "`\t\n\r";
	CHAR	*pToken = NULL;
	CHAR	imgFileId[256], imgFileName[256];
	SI32 value, preloadswitch;
	
	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "ImgFile.txt" );
	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// ù��° ������ �����Ѵ�. 
	fgets(buffer, 1024, fp);

	while( fgets(buffer, 1024, fp) != NULL  )
	{ 
		if( strlen(buffer) < 5 )			continue;

		ZeroMemory(temp, sizeof(temp));
		memcpy(temp, buffer, sizeof(temp)-1);

		// �ּ����� �����ϴ� ���� �ǳʶڴ�.
		if( strstr(temp, "//") != NULL )	continue;	

		sscanf(buffer, "%s %d %s %d", imgFileId, &value, imgFileName, &preloadswitch);
		

		ImgFileList[value].siImgFileValue	= value;
		ImgFileList[value].siPreLoad		= preloadswitch;
		strcpy(ImgFileList[value].clImgFileId,imgFileId);
		strcpy(ImgFileList[value].clImgFileName,imgFileName);
	}
	fclose(fp);

	return TRUE;
*/
}



SI32 GetValueOfGameSprNumID(CHAR *ID)
{
	SI32 i;

	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{


		if(ImgFileList[i].clImgFileId[0] == 0)continue;

		if( strcmp(ImgFileList[i].clImgFileId, ID) == 0 )
		{
			return ImgFileList[i].siImgFileValue;
		}
	}
	clGrp.Error("GetValueOfGame", "[%s]�� �ش��ϴ� ID ����", ID);
	return -1;
}



void _FileVary::Init()
{
	ReadTreeFileText_Daesang();
	#ifdef _IMJIN2PLUS
	OnlineLogoFile	= GetValueOfGameSprNumID("FILE_IMJINONLINE");
	LogoFile		= GetValueOfGameSprNumID("FILE_IMJIN2PLUS");
	//TreeFile		= GetValueOfGameSprNumID("FILE_TREE_PLUS");
#else
	LogoFile		= GetValueOfGameSprNumID("FILE_IMJIN2");
	//TreeFile		= GetValueOfGameSprNumID("FILE_TREE0");
#endif

	for( int i =0 ; i < m_siTreeFileNo; i++)
	{
		m_TreeFiles[i] = GetValueOfGameSprNumID( m_TreeImgFile[i].FileName);
	}	
}

//Tree Type�� ����ִ� ������ �о�´�.
BOOL _FileVary::ReadTreeFileText_Daesang()
{	
	if( m_TreeImgFile )
	{
		delete [] m_TreeImgFile;
		m_TreeImgFile = NULL;
	}
	
	m_TreeFiles = NULL;
	SetNowFile(0);
	
	FILE *fp = BindJXFile::GetInstance()->GetFile( "TreeFile.txt" );
	if( fp == NULL) return FALSE;
	
	int siTreeNum = 0;
	char tmp[1024];
	char seps[]   = " ,\t\n";
	
	
	while( fgets( tmp, 1024, fp) != NULL)
	{
		if( strlen(tmp) > 0)
			siTreeNum++;	
	}
	
	m_siTreeFileNo = siTreeNum;	

	if( siTreeNum == 0 ) return FALSE;


	m_TreeFiles = new SI32 [ siTreeNum ];
	for( int i = 0; i < siTreeNum ; i++ )
		m_TreeFiles[i] = -1;

	
	m_TreeImgFile = new _TreeFile[siTreeNum];
	
	fseek( fp, 0, SEEK_SET);
	
	i = 0;
	while( fgets( tmp, 1024, fp) != NULL)
	{
		if( strlen(tmp) <= 0) continue;
		sscanf( tmp, "%s%d", m_TreeImgFile[i].FileName, &m_TreeImgFile[i].BlockSize);
		i++;
	}
	
	fclose(fp);
	return TRUE;
}



// ������ ������ ���� �����ϰ� �ǳ��� �ҷ��´�. 
BOOL LoadPannel(SI32 sicountry)
{

	char buffer[128];


	// �¶��� ���� ���������� �ǳ�. 
	strcpy(buffer, "fnt\\pannel_online.spr");

	GetFileNamePath(buffer, SavePath, buffer);

	if(PannelSpr.Image)
	{
		clGrp.FreeXspr(PannelSpr);
		PannelSpr.Image = NULL;
	}

	clGrp.LoadXspr(buffer, PannelSpr);

	return TRUE;

}


// num �� �ش��ϴ� �̹��� ������ �ε��Ѵ�.
BOOL LoadImgFile(SI32 num)
{
	SI32 fileNum;
	CHAR buffer[FILE_NAME_SIZE];
	SI32 i;

	fileNum = -1;
	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( num == ImgFileList[i].siImgFileValue )
		{
			fileNum = ImgFileList[i].siImgFileValue;
			break;
		}
	}

	if( fileNum == -1 )
	{
		clGrp.Error("LoadImgFile", "[%d]�� �ش��ϴ� �� ����", num);
		return FALSE;
	}

	// �̸� �ε��� �� ���� siPreLoad�� 1�̴�.
	if( ImgFileList[fileNum].siPreLoad == 1 )
		return TRUE;


	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
	GetFileNamePath(ImgFileList[fileNum].clImgFileName, SavePath, buffer);

	if( clGrp.LoadXspr(buffer, GameSpr[fileNum]) == FALSE )
	{
		clGrp.Error("LoadImgFile - Load Error", buffer);
	}

	// �̹����� �ε��� �ð��� �����Ѵ�.
	// �Ŀ� ���� ������ �̹��� ���� �����ϱ� ����. 
	ImgFileList[fileNum].LoadedClock = CurrentClock;	
	return TRUE;
}


// �����߿� �ε��� �̹��� ������ �޸𸮸� ��� �����Ѵ�.
void FreeImgFile()
{
	SI32 i;
	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( ImgFileList[i].siPreLoad == 0 )
		{
			if( GameSpr[i].Image )
			{
				// �̹����� �ε��� �ð��� �ʱ�ȭ�Ѵ�. 
				ImgFileList[i].LoadedClock = 0;
				clGrp.FreeXspr(GameSpr[i]);
				GameSpr[i].Image = NULL;
			}
		}
	}
}

// �ε��� �ð��� �ٰŷ� ������ �̹����� �����Ѵ�.
void FreeOldImageByTime()
{
	SI32 i;
	SI32 oldimageindex = -1;
	SI32 oldclock = 0;
	SI32 gab;

	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( ImgFileList[i].siPreLoad == 0 )
		{
			if( GameSpr[i].Image && ImgFileList[i].LoadedClock != 0) // �̹����� �ְ�. �̹����� �ε��� �ð��� ��ϵǾ� �ְ�. 
			{
				gab = abs(CurrentClock - ImgFileList[i].LoadedClock);

				if(gab > 300000	&& oldclock < gab)					// �ε����� �ּ��� 10�� �̻� ����Ͽ���.
				{
					oldclock = gab;
					oldimageindex = i;
					break;					
				}
			}
		}
	}

	if(oldimageindex  >= 0)
	{
		// �̹����� �ε��� �ð��� �ʱ�ȭ�Ѵ�. 
		ImgFileList[oldimageindex].LoadedClock = 0;

		clGrp.FreeXspr(GameSpr[oldimageindex]);
		GameSpr[oldimageindex].Image = NULL;
	}
}

#endif