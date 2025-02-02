#include <GSL.h>
#include "IOnlineMouse.h"
#include <Mouse.h>
#include "bindJXFile.h"
#include "../SCCommon/OnlineCommonParser.h"

extern _InputDevice				IpD;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
IOnlineMouse::~IOnlineMouse()
{	
	Free();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::Init()
{
	m_siCurrentMouse = 0;

	// 醴憮 檜嘐雖 煎萄
	m_CursorImageMgr.LoadCursorImage();

	// 醴憮 薑爾 煎萄
	LoadCursorInfo();

	// 葆辦蝶蒂 爾檜紫煙 撲薑п遽棻.
	ShowMouse(TRUE);

	// 葆辦蝶蒂 撲薑п遽棻.
	SetMouse(ON_MOUSE_BASIC);	
}

BOOL	IOnlineMouse::LoadCursorInfo()
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\Cursor\\CursorInfo.dat";

	// だ橾擊 翮堅
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : 轎溘腆 縑楝だ橾貲 瞳營
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// だ諒擊 衛濛и棻.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 濠猿 匐儀
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 等檜攪 塭檣 蹺轎

		SI32	iLineNo, iColNo;

		// 濠猿蒂 牖離瞳戲煎 嫡擊 遽綠蒂 и棻.
		POINT ptHotSpot;
		char szEnumIndexCursorInfo[48];
		char szEnumIndexCursorImg[48];

		SMOUSE_ANI_STATUS	sMouse_ani_status[ON_MOUSE_MAX_ANI_STATUS];

		// 濠猿 嫡晦 偃衛
		ocp.GetValue( szEnumIndexCursorInfo,		47,		iLineNo, iColNo );
		ocp.GetValue( szEnumIndexCursorImg,			47,		iLineNo, iColNo );
		ocp.GetValue( (SI32 &)ptHotSpot.x,					iLineNo, iColNo );
		ocp.GetValue( (SI32 &)ptHotSpot.y,					iLineNo, iColNo );

		for (SI32 i = 0; i < ON_MOUSE_MAX_ANI_STATUS; i++)
		{
			ocp.GetValue( (SI16	&)(sMouse_ani_status[i].m_ssStartFont),			iLineNo, iColNo );	// 醴憮 衛濛 アお 廓��
			ocp.GetValue( (SI16	&)(sMouse_ani_status[i].m_ssFontNumber),		iLineNo, iColNo );	// 醴憮 擁棲詭檜暮 アお 熱
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_scAnimationOption),	iLineNo, iColNo );	// 擁棲詭檜暮 褫暮(瑞Щ, и廓虜, ...)
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_scAnimationMethod),	iLineNo, iColNo );	// 擁棲詭檜暮 寞徹(橾奩, 剪紱煎, 橾奩->剪紱煎, ...)
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_ssFrameDelay),		iLineNo, iColNo );	// 擁棲詭檜暮 裁溯檜
		}

		// 醴憮 撲薑
		m_MouseCursor[CMouseTypeEnum::GetData(szEnumIndexCursorInfo)].Init(m_CursorImageMgr.GetCursorImage(CCurosrImageEnum::GetData(szEnumIndexCursorImg)), sMouse_ani_status, ptHotSpot);

		// �挫弇蝏� - 虜擒 雖旎 籀葬и 匙檜 葆雖虞 鏽歲檜歷棻賊 薑鼻檜棻.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}

		// �挫� 餌髦 - 檜匙檜 雖旎 葆雖虞 塭檣簞蒂 籀葬ж堅 氈棻賊 粽溯檜觼
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::Free()
{
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶蒂 撲薑и棻.
//				robypark 2005/2/15 15:46	SI16 ssSubMouseType 檣濠 蹺陛. 瞪癱 葆辦蝶橾 唳辦 撮睡 撲薑擊 嬪и 高
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::SetMouse(SI16 siMouse, bool bEnabled/* = true*/)
{
	if(m_siCurrentMouse == siMouse)
		return;

	m_siCurrentMouse	=	siMouse;

	m_MouseCursor[m_siCurrentMouse].SetMouse(bEnabled);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	煎斜檣 葆辦蝶煎 瞪��/п薯и棻.(煎斜檣, 啪歜鼻曖 つ溯お陛 憮煎 棻葷)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::SetLoginMouse(BOOL bLogin)
{
	m_MouseCursor[ON_MOUSE_BASIC].SetMouse(true);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	IOnlineMouse::GetMouse()
{
	return m_siCurrentMouse;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬪纂蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::SetPos(SI32 siX, SI32 siY)
{	
	m_ptMouse.x	=	siX;
	m_ptMouse.y	=	siY;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬪纂蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::GetPos(LPPOINT pPoint)
{
	*pPoint		=	m_ptMouse;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶蒂 爾檜紫煙 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::ShowMouse(BOOL bshow)
{
	m_bShow	=	bshow;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶陛 爾檜朝 鼻鷓檣雖 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineMouse::IsShowMouse()
{
	return m_bShow;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶蒂 �飛橦� 斜萼棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineMouse::Draw()
{
	if(m_bShow == TRUE)
	{
		m_MouseCursor[m_siCurrentMouse].Draw(&m_ptMouse);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶曖 擋暮.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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