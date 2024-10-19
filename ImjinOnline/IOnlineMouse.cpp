#include <GSL.h>
#include "IOnlineMouse.h"
#include <Mouse.h>
#include "bindJXFile.h"
#include "../SCCommon/OnlineCommonParser.h"

extern _InputDevice				IpD;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
IOnlineMouse::~IOnlineMouse()
{	
	Free();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::Init()
{
	m_siCurrentMouse = 0;

	// Ŀ�� �̹��� �ε�
	m_CursorImageMgr.LoadCursorImage();

	// Ŀ�� ���� �ε�
	LoadCursorInfo();

	// ���콺�� ���̵��� �������ش�.
	ShowMouse(TRUE);

	// ���콺�� �������ش�.
	SetMouse(ON_MOUSE_BASIC);	
}

BOOL	IOnlineMouse::LoadCursorInfo()
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\Cursor\\CursorInfo.dat";

	// ������ ����
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
		POINT ptHotSpot;
		char szEnumIndexCursorInfo[48];
		char szEnumIndexCursorImg[48];

		SMOUSE_ANI_STATUS	sMouse_ani_status[ON_MOUSE_MAX_ANI_STATUS];

		// �ڷ� �ޱ� ����
		ocp.GetValue( szEnumIndexCursorInfo,		47,		iLineNo, iColNo );
		ocp.GetValue( szEnumIndexCursorImg,			47,		iLineNo, iColNo );
		ocp.GetValue( (SI32 &)ptHotSpot.x,					iLineNo, iColNo );
		ocp.GetValue( (SI32 &)ptHotSpot.y,					iLineNo, iColNo );

		for (SI32 i = 0; i < ON_MOUSE_MAX_ANI_STATUS; i++)
		{
			ocp.GetValue( (SI16	&)(sMouse_ani_status[i].m_ssStartFont),			iLineNo, iColNo );	// Ŀ�� ���� ��Ʈ ��ȣ
			ocp.GetValue( (SI16	&)(sMouse_ani_status[i].m_ssFontNumber),		iLineNo, iColNo );	// Ŀ�� �ִϸ��̼� ��Ʈ ��
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_scAnimationOption),	iLineNo, iColNo );	// �ִϸ��̼� �ɼ�(����, �ѹ���, ...)
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_scAnimationMethod),	iLineNo, iColNo );	// �ִϸ��̼� ���(�Ϲ�, �Ųٷ�, �Ϲ�->�Ųٷ�, ...)
			ocp.GetValue( (char	&)(sMouse_ani_status[i].m_ssFrameDelay),		iLineNo, iColNo );	// �ִϸ��̼� ������
		}

		// Ŀ�� ����
		m_MouseCursor[CMouseTypeEnum::GetData(szEnumIndexCursorInfo)].Init(m_CursorImageMgr.GetCursorImage(CCurosrImageEnum::GetData(szEnumIndexCursorImg)), sMouse_ani_status, ptHotSpot);

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::Free()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� �����Ѵ�.
//				robypark 2005/2/15 15:46	SI16 ssSubMouseType ���� �߰�. ���� ���콺�� ��� ���� ������ ���� ��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::SetMouse(SI16 siMouse, bool bEnabled/* = true*/)
{
	if(m_siCurrentMouse == siMouse)
		return;

	m_siCurrentMouse	=	siMouse;

	m_MouseCursor[m_siCurrentMouse].SetMouse(bEnabled);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�α��� ���콺�� ��ȯ/�����Ѵ�.(�α���, ���ӻ��� �ȷ�Ʈ�� ���� �ٸ�)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::SetLoginMouse(BOOL bLogin)
{
	m_MouseCursor[ON_MOUSE_BASIC].SetMouse(true);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	IOnlineMouse::GetMouse()
{
	return m_siCurrentMouse;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ġ�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::SetPos(SI32 siX, SI32 siY)
{	
	m_ptMouse.x	=	siX;
	m_ptMouse.y	=	siY;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ġ�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::GetPos(LPPOINT pPoint)
{
	*pPoint		=	m_ptMouse;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� ���̵��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::ShowMouse(BOOL bshow)
{
	m_bShow	=	bshow;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� ���̴� �������� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineMouse::IsShowMouse()
{
	return m_bShow;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� ȭ�鿡 �׸���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineMouse::Draw()
{
	if(m_bShow == TRUE)
	{
		m_MouseCursor[m_siCurrentMouse].Draw(&m_ptMouse);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺�� �׼�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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