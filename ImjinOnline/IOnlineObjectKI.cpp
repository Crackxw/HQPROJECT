/************************************************************************************************
	파일명 : IOnlineObjectKI.cpp
	담당자 : 이윤석

	Object 전체의 정보를 관리함
************************************************************************************************/
#include <GSL.h>
#include "Online.h"
#include "OnlineObjectParser.h"
#include <DebugTool.h>
#include "OnlineResource.h"
#include "IOnlineObjectKI.h"
#include "OnlineText.h"


//----------------------------------------------------------------------------------------------------------------	
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
IOnlineObjectKI::IOnlineObjectKI()
{
	pOnlineObjectKI	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
IOnlineObjectKI::~IOnlineObjectKI()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.
//----------------------------------------------------------------------------------------------------------------

BOOL	 IOnlineObjectKI::Init(OnlineResource *pOnlineResource, OnlineText *pOnlineText)
{
	SI16					i;
	OnlineObjectParser		ObjectParser;													// 오브젝트 파서 생성
	ObjectHeader			*pHeader;														// 오브젝트 헤더

	ObjectParser.Init( "Online\\Object\\Object.txt" );

	siObjectNum				=	ObjectParser.GetObjectNum();								// 오브젝트 수를 리턴을 받는다.

	if( siObjectNum == 0 )		return FALSE;


	pOnlineObjectKI			=	new OnlineObjectKI[ siObjectNum + 1 ];
	pHeader					=	ObjectParser.GetObjectHeader();

	if( pHeader == NULL )		return FALSE;


	for( i=1 ; i<=siObjectNum ; i++ )														// 리소스에서 파일이름을 넣고 해당 인덱스를 받는다.
	{
		pHeader[i].siFile	=	pOnlineResource->GetObjectFileIndex( pHeader[i].szObjectFileName );	
		if( pHeader[i].siFile == -1 )	clGrp.Error( "IOnlineObjectKI", "File Name [%s]", pHeader[i].szObjectFileName );
	}


	for( i=1 ; i<=siObjectNum ; i++ )														// 오브젝트 KI에 설정을 한다.
	{
		pOnlineObjectKI[i].Init( pHeader[i].szObjectName, pHeader[i].siKind, 
								 pHeader[i].siFile, pHeader[i].siFont, 
								 pHeader[i].siXsize, pHeader[i].siYsize, 
								 pHeader[i].siCenterX, pHeader[i].siCenterY, 
								 pHeader[i].siAttribute, 0, pHeader[i].siFrame,
								 (pHeader[i].siMark == ON_TEXT_UNUSED)? NULL : pOnlineText->GetObjectMarkText(pHeader[i].siMark));
	}

	ObjectParser.Free();
	return TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineObjectKI::Free()
{
	if(pOnlineObjectKI)
	{
		delete [] pOnlineObjectKI;
		pOnlineObjectKI = NULL;
		siObjectNum		= 0;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	이름을 구한다.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineObjectKI::GetName(SI32 siKind, CHAR pszName[32])
{
	SI32	siIndex		=	GetIndex(siKind);

	pOnlineObjectKI[siIndex].GetName(pszName);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	이름을 구한다.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineObjectKI::GetNameFromIndex(SI32 siIndex, CHAR pszName[32])
{
	pOnlineObjectKI[siIndex].GetName(pszName);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	설명을 구한다.
//----------------------------------------------------------------------------------------------------------------
CHAR	*IOnlineObjectKI::GetMark(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return(pOnlineObjectKI[siIndex].GetMark());
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	설명을 구한다.
//----------------------------------------------------------------------------------------------------------------
CHAR	*IOnlineObjectKI::GetMarkFromIndex(SI32 siIndex)
{
	return(pOnlineObjectKI[siIndex].GetMark());
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	화일을 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetFile(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetFile();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	화일을 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetFileFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetFile();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	폰트를 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetFont(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetFont();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	폰트를 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetFontFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetFont();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Xsize를 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetXsize(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetXsize();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Xsize를 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetXsizeFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetXsize();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Ysize를 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetYsize(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetYsize();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Ysize를 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetYsizeFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetYsize();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	스프라이트 X 중심점을 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetSprCenterX(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetSprCenterX();	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	스프라이트 X 중심점을 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetSprCenterXFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetSprCenterX();	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	스프라이트 Y 중심점을 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetSprCenterY(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetSprCenterY();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	스프라이트 Y 중심점을 구한다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetSprCenterYFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetSprCenterY();
}
/*
//----------------------------------------------------------------------------------------------------------------
//	설명	:	속성을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	IOnlineObjectKI::GetAtb(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetAtb();
}
*/
/*
//----------------------------------------------------------------------------------------------------------------
//	설명	:	속성을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	IOnlineObjectKI::GetAtbFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetAtb();
}
*/
//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을 종류를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetVillageKind(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetVillageKind();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을 종류를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetVillageKindFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetVillageKind();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을 종류를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetTotalFrame(SI32 siKind)
{
	SI32	siIndex		=	GetIndex(siKind);

	return pOnlineObjectKI[siIndex].GetTotalFrame();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을 종류를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	IOnlineObjectKI::GetTotalFrameFromIndex(SI32 siIndex)
{
	return pOnlineObjectKI[siIndex].GetTotalFrame();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	인덱스를 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineObjectKI::GetIndex(SI32 siKind)
{
	SI32	i;

	for(i = 1; i <= siObjectNum; i++)
	{
		if(pOnlineObjectKI[i].GetKind() == siKind)
			return i;
	}

	return 0;
}
