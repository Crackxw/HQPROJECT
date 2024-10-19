#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  담당 : 김태곤
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
// 온라인공용 파서 사용
#include "../SCCommon/OnlineCommonParser.h"

extern _clGame* pGame;
extern	_Config			Config;
extern	XSPR			TileSpr[];
extern	char			SavePath[];
extern	XSPR			PannelSpr;
extern clock_t			CurrentClock;


void InitTileFile();
BOOL LoadTileFile();
// 바다 타일 정보를 불러온다.
BOOL LoadSeaType();
BOOL LoadNewBlkType();
// 성에 사용될 타일 정보들을 불러온다. 
BOOL LoadCastleType();

//------------------------------------
// 그림저장을 위한 변수 
//------------------------------------
XSPR GameSpr[MAX_GAME_SPR_NUMBER];      // 일반적으로 사용될 스프라이트 


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

// 지도용 타일을 불러온다.
void LoadMapTile(SHORT maptype)
{
	// 읽어와야 할 파일을 분석한다. 
	
	LoadTileFile();
	InitTileFile();
	// 바다 타일 정보 불러오기
	LoadSeaType();	
	LoadNewBlkType();
	LoadCastleType();
	

	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;


	// 실제로 데이터 파일을 읽어온다. 
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

	   // savepath를 고려하여 화일이름을 보정한다. 
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


// 지도용 타일을 해제한다.
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

// 일반 그림들을 불러온다. 
void LoadFnt()
{
	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	char buffer2[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;

	while(!exit)
	{
		// 처음에 로딩하지 않는다는 플래그를 가진 spr 파일은 로딩하지 않는다.
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
			// savepath를 고려하여 화일이름을 보정한다. 
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

	// savepath를 고려하여 화일이름을 보정한다. 
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

		// 한 줄을 읽어온다. 
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

	//TileID가 NULL인경우 

	for( i=0; i<MAX_MAP_TILE_SPR_NUMBER; i++ )
	{
		if(clTileID[i].GetString() && (TileID != NULL))
		{
			if( strcmp(clTileID[i].GetString(), TileID) == 0 )
				return i;
		}
	}
	//clGrp.Error("GetValueOfTileID", "[%s] - 찾기 실패", TileID);
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
	
	// savepath를 고려하여 화일이름을 보정한다. 
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
		
		// 한 줄을 읽어온다. 
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
					clGrp.Error("LoadSeaType", "최대 개수 초과(MAX COUNT OVER!!)");
				
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

   // savepath를 고려하여 화일이름을 보정한다. 
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

		// 한 줄을 읽어온다. 
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
					clGrp.Error("LoadNewBlkType", "최대 개수 초과(MAX COUNT OVER!!)");

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

   // savepath를 고려하여 화일이름을 보정한다. 
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

		// 한 줄을 읽어온다. 
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
					clGrp.Error("LoadCastleType", "최대 개수 초과(MAX COUNT OVER!!)");

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
	// 온라인 공용 파서를 이용하도록 수정
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "ImgFile.txt";

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Image List File Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
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

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
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
	
	// savepath를 고려하여 화일이름을 보정한다. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "ImgFile.txt" );
	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// 첫번째 한줄은 무시한다. 
	fgets(buffer, 1024, fp);

	while( fgets(buffer, 1024, fp) != NULL  )
	{ 
		if( strlen(buffer) < 5 )			continue;

		ZeroMemory(temp, sizeof(temp));
		memcpy(temp, buffer, sizeof(temp)-1);

		// 주석으로 시작하는 줄은 건너뛴다.
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
	clGrp.Error("GetValueOfGame", "[%s]에 해당하는 ID 없음(Cannot Find ID [%s]!)", ID, ID);
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


// 게임의 종류에 따라 적절하게 판넬을 불러온다. 
BOOL LoadPannel(SI32 sicountry)
{

	char buffer[128];


	// 온라인 게임 전투에서의 판넬. 
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


// num 에 해당하는 이미지 파일을 로딩한다.
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
		clGrp.Error("LoadImgFile", "[%d]에 해당하는 값 없음(Cannot Find Value [%d]!)", num, num);
		return FALSE;
	}

	// 미리 로딩해 둔 것은 siPreLoad가 1이다.
	if( ImgFileList[fileNum].siPreLoad == 1 )
		return TRUE;


	// savepath를 고려하여 화일이름을 보정한다. 
	GetFileNamePath(ImgFileList[fileNum].clImgFileName, SavePath, buffer);

	if( clGrp.LoadXspr(buffer, GameSpr[fileNum]) == FALSE )
	{
		clGrp.Error("LoadImgFile - Load Error", buffer);
	}

	// 이미지가 로딩된 시간을 저장한다.
	// 후에 가장 오래된 이미지 부터 삭제하기 위함. 
	ImgFileList[fileNum].LoadedClock = CurrentClock;	
	return TRUE;
}


// 게임중에 로딩된 이미지 파일의 메모리를 모두 해제한다.
void FreeImgFile()
{
	SI32 i;
	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( ImgFileList[i].siPreLoad == 0 )
		{
			if( GameSpr[i].Image )
			{
				// 이미지가 로딩된 시간을 초기화한다. 
				ImgFileList[i].LoadedClock = 0;
				clGrp.FreeXspr(GameSpr[i]);
				GameSpr[i].Image = NULL;
			}
		}
	}
}

// 로딩된 시간을 근거로 오래된 이미지를 제거한다.
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
			if( GameSpr[i].Image && ImgFileList[i].LoadedClock != 0) // 이미지가 있고. 이미지가 로딩된 시간이 기록되어 있고. 
			{
				gab = abs(CurrentClock - ImgFileList[i].LoadedClock);

				if(gab > 300000	&& oldclock < gab)					// 로딩된지 최소한 10분 이상 경과하였고.
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
		// 이미지가 로딩된 시간을 초기화한다. 
		ImgFileList[oldimageindex].LoadedClock = 0;

		clGrp.FreeXspr(GameSpr[oldimageindex]);
		GameSpr[oldimageindex].Image = NULL;
	}
}



#else
//--------------------------------------------------------------------
//  담당 : 김태곤
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
// 바다 타일 정보를 불러온다.
BOOL LoadSeaType();
BOOL LoadNewBlkType();
// 성에 사용될 타일 정보들을 불러온다. 
BOOL LoadCastleType();

//------------------------------------
// 그림저장을 위한 변수 
//------------------------------------
XSPR GameSpr[MAX_GAME_SPR_NUMBER];      // 일반적으로 사용될 스프라이트 


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
	FileTileGrss1		= GetValueOfTileID("FILE_TILE_GRASS");//노멀-초지
	FileTileDiff1		= GetValueOfTileID("FILE_TILE_MUD");//노멀-진흙
	FileTileHill0		= GetValueOfTileID("FILE_TILE_NORMAL");
	FileTileHill1		= GetValueOfTileID("FILE_TILE_ROAD");//노멀-길
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

// 지도용 타일을 불러온다.
void LoadMapTile(SHORT maptype)
{
	// 읽어와야 할 파일을 분석한다. 
	
	LoadTileFile();
	InitTileFile();
	// 바다 타일 정보 불러오기
	LoadSeaType();	
	LoadNewBlkType();
	LoadCastleType();
	

	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;


	// 실제로 데이터 파일을 읽어온다. 
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

	   // savepath를 고려하여 화일이름을 보정한다. 
	   GetFileNamePath(buffer, SavePath, buffer);

	   //대상 일단 임시로 로드..
	   //if( maptype == MAP_TYPE_NORMAL && index == 0)
		//if( maptype == MAP_TYPE_NORMAL && index < 2)
	   if(1)
	   {
		   TileSpr[index].Image = NULL;
		   if( clGrp.LoadXsprWithAllocMem( buffer, TileSpr[index],MAX_ONLINE_TILEDATA_IMAGESIZE ) == FALSE)
		   //if(clGrp.LoadXspr("C:\\공유방\\대상 살빼기용\\tile\\TA_SO_grass.Spr", TileSpr[index])==FALSE)
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


// 지도용 타일을 해제한다.
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

// 일반 그림들을 불러온다. 
void LoadFnt()
{
	int index=0;
	int exit=FALSE;
	char buffer[FILE_NAME_SIZE];
	char buffer2[FILE_NAME_SIZE];
	int errorcheckswitch=FALSE;

	while(!exit)
	{
		// 처음에 로딩하지 않는다는 플래그를 가진 spr 파일은 로딩하지 않는다.
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
			// savepath를 고려하여 화일이름을 보정한다. 
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

	// savepath를 고려하여 화일이름을 보정한다. 
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

		// 한 줄을 읽어온다. 
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

	//TileID가 NULL인경우 

	for( i=0; i<MAX_MAP_TILE_SPR_NUMBER; i++ )
	{
		if(clTileID[i].GetString() && (TileID != NULL))
		{
			if( strcmp(clTileID[i].GetString(), TileID) == 0 )
				return i;
		}
	}
	//clGrp.Error("GetValueOfTileID", "[%s] - 찾기 실패", TileID);
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
	
	// savepath를 고려하여 화일이름을 보정한다. 
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
		
		// 한 줄을 읽어온다. 
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
					clGrp.Error("LoadSeaType", "최대 개수 초과");
				
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

   // savepath를 고려하여 화일이름을 보정한다. 
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

		// 한 줄을 읽어온다. 
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
					clGrp.Error("LoadNewBlkType", "최대 개수 초과");

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

   // savepath를 고려하여 화일이름을 보정한다. 
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

		// 한 줄을 읽어온다. 
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
					clGrp.Error("LoadCastleType", "최대 개수 초과");

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
	// 온라인 공용 파서를 이용하도록 수정
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "ImgFile.txt";

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Image List File Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
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

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
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
	
	// savepath를 고려하여 화일이름을 보정한다. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "ImgFile.txt" );
	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// 첫번째 한줄은 무시한다. 
	fgets(buffer, 1024, fp);

	while( fgets(buffer, 1024, fp) != NULL  )
	{ 
		if( strlen(buffer) < 5 )			continue;

		ZeroMemory(temp, sizeof(temp));
		memcpy(temp, buffer, sizeof(temp)-1);

		// 주석으로 시작하는 줄은 건너뛴다.
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
	clGrp.Error("GetValueOfGame", "[%s]에 해당하는 ID 없음", ID);
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

//Tree Type이 들어있는 파일을 읽어온다.
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



// 게임의 종류에 따라 적절하게 판넬을 불러온다. 
BOOL LoadPannel(SI32 sicountry)
{

	char buffer[128];


	// 온라인 게임 전투에서의 판넬. 
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


// num 에 해당하는 이미지 파일을 로딩한다.
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
		clGrp.Error("LoadImgFile", "[%d]에 해당하는 값 없음", num);
		return FALSE;
	}

	// 미리 로딩해 둔 것은 siPreLoad가 1이다.
	if( ImgFileList[fileNum].siPreLoad == 1 )
		return TRUE;


	// savepath를 고려하여 화일이름을 보정한다. 
	GetFileNamePath(ImgFileList[fileNum].clImgFileName, SavePath, buffer);

	if( clGrp.LoadXspr(buffer, GameSpr[fileNum]) == FALSE )
	{
		clGrp.Error("LoadImgFile - Load Error", buffer);
	}

	// 이미지가 로딩된 시간을 저장한다.
	// 후에 가장 오래된 이미지 부터 삭제하기 위함. 
	ImgFileList[fileNum].LoadedClock = CurrentClock;	
	return TRUE;
}


// 게임중에 로딩된 이미지 파일의 메모리를 모두 해제한다.
void FreeImgFile()
{
	SI32 i;
	for( i=0; i<MAX_GAME_SPR_NUMBER; i++ )
	{
		if( ImgFileList[i].siPreLoad == 0 )
		{
			if( GameSpr[i].Image )
			{
				// 이미지가 로딩된 시간을 초기화한다. 
				ImgFileList[i].LoadedClock = 0;
				clGrp.FreeXspr(GameSpr[i]);
				GameSpr[i].Image = NULL;
			}
		}
	}
}

// 로딩된 시간을 근거로 오래된 이미지를 제거한다.
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
			if( GameSpr[i].Image && ImgFileList[i].LoadedClock != 0) // 이미지가 있고. 이미지가 로딩된 시간이 기록되어 있고. 
			{
				gab = abs(CurrentClock - ImgFileList[i].LoadedClock);

				if(gab > 300000	&& oldclock < gab)					// 로딩된지 최소한 10분 이상 경과하였고.
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
		// 이미지가 로딩된 시간을 초기화한다. 
		ImgFileList[oldimageindex].LoadedClock = 0;

		clGrp.FreeXspr(GameSpr[oldimageindex]);
		GameSpr[oldimageindex].Image = NULL;
	}
}

#endif