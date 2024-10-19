//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> ĳ���� ���� â
//	File Name		: OnlineCharStatus.h
//	Birth Date		: 2001. 12. 28.
//	Creator			: �� �� ��, �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ����� �ֿܼ��� ĳ���� ����â�� ����ִ� �κ��Դϴ�.
//
//===================================================================================================

#ifndef __ONLINE_CHARSTATUS_H__
#define __ONLINE_CHARSTATUS_H__

//-----------------------------------------------------------------------------
// Name: OnlineCharStatus
// Code: All (2004-02-16)
// Desc: ���� ��� ���� �� �ܺ� Ŭ���� ����
//-----------------------------------------------------------------------------
#include <Graphics16.h>
#include <Button.h>

#define ON_CHAR_STATUS_NUM		4

class   _ParametaBox;
class	IOnlineFollower;
class	OnlineEditControl;

//-----------------------------------------------------------------------------
// Name: OnlineCharStatus
// Code: All (2004-02-16)
// Desc: Ŭ���� ����
//-----------------------------------------------------------------------------
class	OnlineCharStatus
{
private:
	cltOnlineWorld				*m_pMyOnlineWorld;									// ���� ���� Ŭ����. �ܺο��� �޾ƿ�
	IOnlineFollower				*m_pMyFollower;										// �뺴 ���� Ŭ����. �ܺο��� �޾ƿ�

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����(�� �ϴ��� �����ʹ� ���߿� ������ ��)
	OnlineEditControl			*m_pEditBox_Rename;									// ����Ʈ �ڽ� UI - ���� â���� �뺴 �̸� �ٲ� ��
	HOEC						m_hOecRename;										// ����Ʈ �ڽ� UI �ڵ�- ���� â���� �뺴 �̸� �ٲ� ��

	_Button						m_Btn_ReName;										// ��ư UI - �̸� �ٲٱ�
	_Button						m_Btn_GrowUp[ ON_CHAR_STATUS_NUM ];					// ��ư UI - �ɷ�ġ UP
	_Button						m_Btn_Close;										// ��ư UI - â�ݱ�

	XSPR						m_Spr_Frm, m_Spr_Btn_Rename, m_Spr_Btn_GrowUp, m_Spr_Btn_Common;

	SI16						m_siX,		m_siY;									// ����â�� ������ x, y
	SI16						m_siWidth,	m_siHeight;								// ����â�� ���� �ʺ�
	SI32						m_siAccountNo;										// �� Ŭ���̾�Ʈ ����ڰ� �������� �ο����� ���� ��ȣ
	SI32						m_siStatus;											// ���� ����â�� �۵� ���¸� �����Ѵ�.(?)
//	SI16						siFont;												// �ʻ� ��Ʈ. ����� ����â���� �ʻ� ����� ���ϹǷ� �Ⱦ���.
	SI32						m_siSelectedDBSlotNo;								// ���� ������ �뺴�� DB ���� ��ȣ(0�ϰ�� �� ���ɸ��� ������ ���´�)
	UI08						m_uiGrownUpDBSlotNo;								// ���� ������ ������ ��û�� �뺴�� DB ���� ��ȣ. �������� ���� �� ���� �����Ű�� ���� �� ���� ����.
	UI08						m_uiGrownUpAbility;									// ���� ������ ������ ��û�� �뺴�� �ɷ� ����. �������� ���� �� ���� �����Ű�� ���� �� ���� ����.
	SI16						m_siNoFieldHandle;									// �ʵ� ���� �Ұ��� ������ ���� �ڵ�
	CHAR						m_pszCharName[ ON_ID_LENGTH + 1 ];					// �뺴 �̸��� ������ �Ѵ�.( �ӽ� ���� )

	BOOL						m_bMousePushStatus;									// ���� ���콺 ���� ����.	
	SI16						m_siRenameBtnStatus;								// �̸� �ٲٱ� ��ư ����.
	UI08						m_uiRenameDBSlotNo;									// �̸��� �ٲٷ��� �뺴�� DB ���� ��ȣ
	RECT						m_rtBook;

public:
	OnlineCharStatus		( void );
	~OnlineCharStatus		( void );

	VOID	Init			( SI32 siAccountNo, cltOnlineWorld *pOnlineWorld, IOnlineFollower *pFollower );
	VOID	Free			( void );
	VOID	Action			( void );
	VOID	Draw			( LPDIRECTDRAWSURFACE7 pSurface );

	VOID	SetActive		( BOOL bActive );
	BOOL	IsActive		( void );
	VOID	SetSelect		( UI08 uiDBSlotNo );
//	VOID	SetX(SI16 six);
	OnlineEditControl	*GetRenameEditBox() { return m_pEditBox_Rename; };		// actdoll : ��͵� ���߿� ����
	HOEC	GetHOECRename( void )	{ return m_hOecRename; }

//----------------------------------------------------------------------------
//	�������� ��Ŷ ���� �� �����ϴ� �Լ�
//----------------------------------------------------------------------------
public:
	VOID	RecvMsg_GrownUp(BOOL bFlag);
	VOID	RecvMsg_Rename(BOOL bFlag);
};	

#endif//__ONLINE_CHARSTATUS_H__
