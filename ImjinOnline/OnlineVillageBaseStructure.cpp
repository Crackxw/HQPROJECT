// ����� : ������

#include <GSL.h>

#include <Main.h> 
#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineVillageManager.h"
#include "OnlineVillageStructManager.h"
#include "OnlineVillageBaseStructure.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"


extern _InputDevice				IpD;


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageBaseStructure::OnlineVillageBaseStructure()
{
	siKind			=	ON_VIL_ST_KIND_UNKNOWN;
	pMyOnlineWorld	=	NULL;
	BackImage.Image	=	NULL;
	pVillageHeader	=	NULL;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageBaseStructure::~OnlineVillageBaseStructure()										
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillageBaseStructure::Init( UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager )
{
	uiVillageCode		=	VillageCode;																		// ������ ���̵�
	pMyOnlineWorld		=	pOnlineWorld;	
	pButtonSpr			=	pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_SCROLLV );	// ������ ��ư�� �̹���
	pVillageManager		=	pVillageStructManager;

	// ������ ������ �޴´�.
	pVillageHeader		=	pMyOnlineWorld->pOnlineVillageManager->GetVillageByCode(uiVillageCode);
	if( pVillageHeader == NULL )	clGrp.Error("OnlineVillageBaseStructure::Init", pMyOnlineWorld->pOnlineText->Get( ON_TEXT_NO_VILLAGE_INFO ) );

	// ��ư�� ������ �Ѵ�.
	BExit.Create( 620, 550, pButtonSpr->Header.Xsize, pButtonSpr->Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillageBaseStructure::Free()
{
	siKind			=	ON_VIL_ST_KIND_UNKNOWN;
	pMyOnlineWorld	=	NULL;

	clGrp.FreeXspr( BackImage );
	BackImage.Image	=	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�׼�.	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageBaseStructure::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( siKind == ON_VIL_ST_KIND_UNKNOWN )	return TRUE;
	else									return FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����(�Ʒü�, ����, ����...)�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	OnlineVillageBaseStructure::GetKind()
{
	return pVillageManager->GetKind();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȿ�� �ǹ����� �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageBaseStructure::IsVaildStructure()
{
	if(pMyOnlineWorld != NULL)
		return TRUE;
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Start Mouse Pos ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillageBaseStructure::SetStartMousePos(POINT ptStartMousePos)
{
	StartMousePos = ptStartMousePos;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���콺 ���¸� üũ�ؼ� �ǹ����� ������ ���� �˾� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillageBaseStructure::CheckExit()
{
	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);

	if( ( BExit.Process( bMouseDown ) == TRUE ) || ( LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) ) )
	{
		pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
		return TRUE;
	}

	if( IpD.LeftPressSwitch )	bMouseDown	=	TRUE;
	else						bMouseDown	=	FALSE;

	return FALSE;
}