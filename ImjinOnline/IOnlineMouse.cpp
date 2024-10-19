#include <GSL.h>
#include "IOnlineMouse.h"
#include <Mouse.h>
#include "bindJXFile.h"
#include "../SCCommon/OnlineCommonParser.h"

extern _InputDevice				IpD;

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	 ¼Ò¸êÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
IOnlineMouse::~IOnlineMouse()
{	
	Free();
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÃÊ±âÈ­
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::Init()
{
	m_siCurrentMouse = 0;

	// Ä¿¼­ ÀÌ¹ÌÁö ·Îµå
	m_CursorImageMgr.LoadCursorImage();

	// Ä¿¼­ Á¤º¸ ·Îµå
	LoadCursorInfo();

	// ¸¶¿ì½º¸¦ º¸ÀÌµµ·Ï ¼³Á¤ÇØÁØ´Ù.
	ShowMouse(TRUE);

	// ¸¶¿ì½º¸¦ ¼³Á¤ÇØÁØ´Ù.
	SetMouse(ON_MOUSE_BASIC);	
}

BOOL	IOnlineMouse::LoadCursorInfo()
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\Cursor\\CursorInfo.dat";

	// ÆÄÀÏÀ» ¿­°í
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : Ãâ·ÂµÉ ¿¡·¯ÆÄÀÏ¸í ÀûÀç
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// ÀÚ·á °Ë»ö
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// µ¥ÀÌÅÍ ¶óÀÎ ÃßÃâ

		SI32	iLineNo, iColNo;

		// ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î ¹ÞÀ» ÁØºñ¸¦ ÇÑ´Ù.
		POINT ptHotSpot;
		char szEnumIndexCursorInfo[48];
		char szEnumIndexCursorImg[48];

		SMOUSE_ANI_STATUS	sMouse_ani_status[ON_MOUSE_MAX_ANI_STATUS];

		// ÀÚ·á ¹Þ±â °³½Ã
		ocp.GetValue( szEnumIndexCursorInfo,		47,		iLineNo, iColNo );
		ocp.GetValue( szEnumIndexCursorImg,			47,		iLineNo, iColNo );
		ocp.GetValue( (SI32 &)ptHotSpot.x,					iLineNo, iColNo );
		ocp.GetValue( (SI32 &)ptHotSpot.y,					iLineNo, iColNo );

		for (SI32 i = 0; i < ON_MOUSE_MAX_ANI_STATUS; i++)
		{
			ocp.GetValue( (SI16	&)(sMouse_ani_status[i].m_ssStartFont),			iLineNo, iColNo );	// Ä¿¼­ ½ÃÀÛ ÆùÆ® ¹øÈ£
			ocp.GetValue( (SI16	&)(sMouse_ani_status[i].m_ssFontNumber),		iLineNo, iColNo );	// Ä¿¼­ ¾Ö´Ï¸ÞÀÌ¼Ç ÆùÆ® ¼ö
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_scAnimationOption),	iLineNo, iColNo );	// ¾Ö´Ï¸ÞÀÌ¼Ç ¿É¼Ç(·çÇÁ, ÇÑ¹ø¸¸, ...)
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_scAnimationMethod),	iLineNo, iColNo );	// ¾Ö´Ï¸ÞÀÌ¼Ç ¹æ¹ý(ÀÏ¹Ý, °Å²Ù·Î, ÀÏ¹Ý->°Å²Ù·Î, ...)
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_ssFrameDelay),		iLineNo, iColNo );	// ¾Ö´Ï¸ÞÀÌ¼Ç µô·¹ÀÌ
		}

		// Ä¿¼­ ¼³Á¤
		m_MouseCursor[CMouseTypeEnum::GetData(szEnumIndexCursorInfo)].Init(m_CursorImageMgr.GetCursorImage(CCurosrImageEnum::GetData(szEnumIndexCursorImg)), sMouse_ani_status, ptHotSpot);

		// È®ÀÎ»ç»ì - ¸¸¾à Áö±Ý Ã³¸®ÇÑ °ÍÀÌ ¸¶Áö¸· ÄÃ·³ÀÌ¾ú´Ù¸é Á¤»óÀÌ´Ù.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}

		// È®ÀÎ »ç»ì - ÀÌ°ÍÀÌ Áö±Ý ¸¶Áö¸· ¶óÀÎÂ°¸¦ Ã³¸®ÇÏ°í ÀÖ´Ù¸é ºê·¹ÀÌÅ©
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇØÁ¦.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::Free()
{
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶¿ì½º¸¦ ¼³Á¤ÇÑ´Ù.
//				robypark 2005/2/15 15:46	SI16 ssSubMouseType ÀÎÀÚ Ãß°¡. ÀüÅõ ¸¶¿ì½ºÀÏ °æ¿ì ¼¼ºÎ ¼³Á¤À» À§ÇÑ °ª
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::SetMouse(SI16 siMouse, bool bEnabled/* = true*/)
{
	if(m_siCurrentMouse == siMouse)
		return;

	m_siCurrentMouse	=	siMouse;

	m_MouseCursor[m_siCurrentMouse].SetMouse(bEnabled);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	·Î±×ÀÎ ¸¶¿ì½º·Î ÀüÈ¯/ÇØÁ¦ÇÑ´Ù.(·Î±×ÀÎ, °ÔÀÓ»óÀÇ ÆÈ·¹Æ®°¡ ¼­·Î ´Ù¸§)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::SetLoginMouse(BOOL bLogin)
{
	m_MouseCursor[ON_MOUSE_BASIC].SetMouse(true);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶¿ì½º¸¦ ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI16	IOnlineMouse::GetMouse()
{
	return m_siCurrentMouse;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::SetPos(SI32 siX, SI32 siY)
{	
	m_ptMouse.x	=	siX;
	m_ptMouse.y	=	siY;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	À§Ä¡¸¦ ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::GetPos(LPPOINT pPoint)
{
	*pPoint		=	m_ptMouse;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶¿ì½º¸¦ º¸ÀÌµµ·Ï ¼³Á¤ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::ShowMouse(BOOL bshow)
{
	m_bShow	=	bshow;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶¿ì½º°¡ º¸ÀÌ´Â »óÅÂÀÎÁö ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	IOnlineMouse::IsShowMouse()
{
	return m_bShow;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶¿ì½º¸¦ È­¸é¿¡ ±×¸°´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::Draw()
{
	if(m_bShow == TRUE)
	{
		m_MouseCursor[m_siCurrentMouse].Draw(&m_ptMouse);
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶¿ì½ºÀÇ ¾×¼Ç.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	IOnlineMouse::Action()
{
	//DWriteStringToFile("siCurrentMouse [%d]", siCurrentMouse);

	if(IpD.LeftPressSwitch)
	{
		if(m_MouseCursor[m_siCurrentMouse].GetAniStatus() != ON_MOUSE_ANI_STATUS_DOWN)
			m_MouseCursor[m_siCurrentMouse].SetAniStatus(ON_MOUSE_ANI_STATUS_DOWN);
	}
	else
	{
		if ((m_MouseCursor[m_siCurrentMouse].GetAniStatus() == ON_MOUSE_ANI_STATUS_DOWN)
			&& (m_MouseCursor[m_siCurrentMouse].IsDoneAnimation()))
		{
			m_MouseCursor[m_siCurrentMouse].SetAniStatus(ON_MOUSE_ANI_STATUS_UP);
		}

		if ((m_MouseCursor[m_siCurrentMouse].GetAniStatus() == ON_MOUSE_ANI_STATUS_UP)
			&& (m_MouseCursor[m_siCurrentMouse].IsDoneAnimation()))
		{
			m_MouseCursor[m_siCurrentMouse].SetAniStatus(ON_MOUSE_ANI_STATUS_NORMAL);
		}
	}

	m_MouseCursor[m_siCurrentMouse].Action();
}