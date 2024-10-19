/************************************************************************************************
	���ϸ� : OnlineCharUI.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_ONLINECHARUI_H
#define _ONLINE_ONLINECHARUI_H

class OnlineCharUI
{
private:
	SI32			siPrevSendOrderFrame;					// ������ ����� ���´� ������	
	OnOrder			Order;
			
	cltOnlineWorld	*pOnlineWorld;
	BOOL			bAction;								// �� Ŭ������ ���� �۵��ϴ°�?
public:
	OnlineCharUI();
	~OnlineCharUI();

	VOID	Init(cltOnlineWorld	*pOnlineWorld);				// �ʱ�ȭ �Լ�.
	VOID	Init();											// �ʱ�ȭ �Լ�.	
	VOID	Poll();											// ���� �Լ�.
	VOID	Free();											// �����Լ�.
	VOID	SetAction(BOOL baction);						// �� Ŭ������ �۵� ���θ� �����Ѵ�.,	
	BOOL	IsAction() { return bAction; }

	VOID	SetMove(SI32 siX, SI32 siY);						// �̵� 
	VOID	SetAttack(UI16 uiUniqueID);							// ����.
	VOID	SetUseObject(SI32 siX, SI32 siY);					// ������Ʈ ���(���� ����),

	// robypark 2005/1/27 18:49
	VOID	SetUseObject_VillageDefence(SI32 siX, SI32 siY);	// ���� �����������̽��� ���ڴٰ� ó��
	
	VOID	SetPickUpItem(SI32 siX, SI32 siY, UI32 uiItemID);	// �������� �ݴ´�.
	VOID	SetUseNPC(SI32 siX, SI32 siY, SI16 siNPCCode);		// NPC 

	VOID	SetShortDistanceAttackVillage(SI32 siX, SI32 siY);	// ���� ����(�ٰŸ�)
	VOID	SetLongDistanceAttackVillage(SI32 siX, SI32 siY);	// ���� ����(���Ÿ�)
	VOID	SetFieldAttack(UI16	uiUniqueID);					// �ʵ忡�� ��������.

	// robypark 2004/11/9 17:12
	VOID	SetRequestSupplyGoodsFromSupplyWagon(UI16 uiSupplyWagonUniqueID);	// ������ ���� ������ ������ ������ ����ǰ ��û

	// robypark 2004/11/9 18:19
	VOID	SetRequestSupplyGoodsFromVillage(SI16 uiVillagePosX, SI16 uiVillagePosY);	// ������ ���� ������ ������ ����ǰ ��û

	// robypark 2005/1/25 18:53
	// ���� ������ �ٸ� ������ ���� ġ�� ���
	VOID	SetOrderHeal(UI16 uiDestUniqueID);
};


#endif
