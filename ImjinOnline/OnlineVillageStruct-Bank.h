#ifndef	_ONLINE_VILLAGEBASESTRUCTUREBANK_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREBANK_HEADER

#include <DDraw.h>

#include "Online.h"
#include "OnlineVillageBaseStructure.h"
#include "OnlineEconomy.h"


#define ON_BANK_NUMPAD_MAX				12


/////////////////////////////////////////////////////
// �ٿ� �Ա�

#define ON_BANK_SAVE_INIT      					20
#define ON_BANK_SAVE_INPUT                      21
#define ON_BANK_SAVE_WAIT                       22
#define ON_BANK_SAVE_EXIT                       23
#define ON_BANK_SAVE_ERROR                      24



/////////////////////////////////////////////////////
// �ٿ� ����

#define ON_BANK_WITHDRAW_INIT					30
#define ON_BANK_WITHDRAW_INPUT					31
#define ON_BANK_WITHDRAW_WAIT					32
#define ON_BANK_WITHDRAW_EXIT                   33
#define ON_BANK_WITHDRAW_ERROR                  34


//============================================================================================
//============================================================================================

 

class	OnlineVillageStructureBank : public OnlineVillageBaseStructure
{
private:
	cltOnlineWorld			*pMyOnlineWorld;

	_Button					BDeposit, BDrawing;						// ���°���, �Ա�, ���, ����
	_Button					BBack;


	XSPR					SubSpr, ExitSpr;
	XSPR					MainButtonSpr01, *pBorder;
	
	SI32					siActionStatus;							// ���� �ϰ� �ִ� ���� �˸��� ��
	SI16					siSpriteNum;
	BOOL					bMouseLDown;							// ���콺 ���¸� üũ�� �Ѵ�.
	SI16					m_siAccontNum;
	

	SI16					siX, siY;

	SI32					siTalk;
	DWORD					dwTime;

	SI32                    m_siSaveStatus;
	SI32                    m_siWithdrawStatus;

public:

	OnlineVillageStructureBank();
	virtual	~OnlineVillageStructureBank();
		
	virtual	VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);
	virtual	VOID			Free();
	virtual	BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);
	virtual BOOL			Action();

	VOID					SetSaveStatus(SI32 status)		{ m_siSaveStatus = status; };
	VOID					SetWithdrawStatus(SI32 status)	{ m_siWithdrawStatus = status; };

private:	
	VOID					Draw(LPDIRECTDRAWSURFACE7 pSurface);							// ���� ��ο�
	VOID					CheckButton();													// ���� ȭ�鿡 �ִ� ��ư���� üũ �Ѵ�.

    VOID                    SaveAction();
	VOID                    WithdrawAction();
	VOID                    SetAllButtonAction(BOOL);
};

#endif
