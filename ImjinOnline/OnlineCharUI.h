/************************************************************************************************
	파일명 : OnlineCharUI.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_ONLINECHARUI_H
#define _ONLINE_ONLINECHARUI_H

class OnlineCharUI
{
private:
	SI32			siPrevSendOrderFrame;					// 이전에 명령을 보냈던 프레임	
	OnOrder			Order;
			
	cltOnlineWorld	*pOnlineWorld;
	BOOL			bAction;								// 이 클래스가 지금 작동하는가?
public:
	OnlineCharUI();
	~OnlineCharUI();

	VOID	Init(cltOnlineWorld	*pOnlineWorld);				// 초기화 함수.
	VOID	Init();											// 초기화 함수.	
	VOID	Poll();											// 폴링 함수.
	VOID	Free();											// 해제함수.
	VOID	SetAction(BOOL baction);						// 이 클래스에 작동 여부를 설정한다.,	
	BOOL	IsAction() { return bAction; }

	VOID	SetMove(SI32 siX, SI32 siY);						// 이동 
	VOID	SetAttack(UI16 uiUniqueID);							// 공격.
	VOID	SetUseObject(SI32 siX, SI32 siY);					// 오브젝트 사용(마을 포함),

	// robypark 2005/1/27 18:49
	VOID	SetUseObject_VillageDefence(SI32 siX, SI32 siY);	// 마을 수성인터페이스로 들어가겠다고 처리
	
	VOID	SetPickUpItem(SI32 siX, SI32 siY, UI32 uiItemID);	// 아이템을 줍는다.
	VOID	SetUseNPC(SI32 siX, SI32 siY, SI16 siNPCCode);		// NPC 

	VOID	SetShortDistanceAttackVillage(SI32 siX, SI32 siY);	// 마을 공격(근거리)
	VOID	SetLongDistanceAttackVillage(SI32 siX, SI32 siY);	// 마을 공격(원거리)
	VOID	SetFieldAttack(UI16	uiUniqueID);					// 필드에서 때려보기.

	// robypark 2004/11/9 17:12
	VOID	SetRequestSupplyGoodsFromSupplyWagon(UI16 uiSupplyWagonUniqueID);	// 공성전 보급 수레에 공성전 유닛이 보급품 요청

	// robypark 2004/11/9 18:19
	VOID	SetRequestSupplyGoodsFromVillage(SI16 uiVillagePosX, SI16 uiVillagePosY);	// 공성전 보급 수레가 마을에 보급품 요청

	// robypark 2005/1/25 18:53
	// 보급 수레가 다른 공성전 유닛 치료 명령
	VOID	SetOrderHeal(UI16 uiDestUniqueID);
};


#endif
