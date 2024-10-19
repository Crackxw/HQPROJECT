#ifndef __ON_NUMBER_CONTROL_H
#define __ON_NUMBER_CONTROL_H


#define NUMBERCONTROL_INFOTYPE_NORMAL				0					// Info Type : 보통
#define NUMBERCONTROL_INFOTYPE_MARKET				1					// Info Type : 상점
#define NUMBERCONTROL_INFOTYPE_BANK					2					// Info Type : 은행
#define NUMBERCONTROL_INFOTYPE_GOVERNMENTINVEST		3					// Info Type : 관청, 투자

#define NUMBERCONTROL_UNIT_MONEY                    0                   // 돈의단위 ( 예 : 원)
#define NUMBERCONTROL_UNIT_THING                    1                   // 물건 (예 : 개)
#define	NUMBERCONTROL_UNIT_ARMY						2                   // 공성전 부대수 (예 : 부대) robypark 2004/11/18 16:46


/********************************************************************************************
	파일명	:	OnlineNumberControl.h
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2001.07.23
	수정일	:	2001.12.10
********************************************************************************************/

class OnlineEditControl;
class _Button;


class OnlineNumberControl
{
public:
	OnlineNumberControl();
	~OnlineNumberControl();

	__int64				GetNumber();
	VOID				SetNumber(__int64 sinumber);
	VOID				Draw( LPDIRECTDRAWSURFACE7 lpSurface );									// 출력
	VOID				Init( __int64 *pCount, cltOnlineWorld *pOnlineWorld, __int64 MaxNumberCount = -1);
	BOOL				IsShow();																// 보이는가?
	BOOL				IsSuccess();															// 성공 했는가?
	VOID				SetShow( SI16 x, SI16 y );												// 보이게 한다.
	VOID				SetShowCenter(void);														// 보이게 한다.
	VOID				Initialize(__int64 MaxNumberCount = -1);									// 초기화
	VOID				InitializeMarket(SI32 ItemMoney, SI32 ItemName, SI32 OKButtonName, __int64 MaxNumberCount = -1);															// 초기화(상점시)
	VOID				InitializeBank(__int64 MaxNumberCount = -1);																											 	// 초기화(은행사용시)
	VOID				InitializeGovernmentInvestment(__int64 MaxNumberCount = -1, __int64 MinNumberCount = -1, __int64 NowInvestmentMoney = -1, __int64 MyInvestmentMoney = -1);	// 초기화(관청, 투자시)
	VOID				Free();																	// 날림

	VOID                SetUnit(SI32 siUnit);                                                   // 단위설정

	// robypark 2004/12/31 15:25
	// 숫자판 컨트롤을 닫는다. (취소버튼 클릭과 동일한 처리)
	VOID				CloseNumberControl(VOID);

private:
	VOID				KeyMessage();															// 키 메시지를 받는다.

private:
	cltOnlineWorld		*pMyOnlineWorld;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	OnlineEditControl	*pEditBox;
//	HWND				m_YetEditHWND;
	HOEC				m_hOec;																	// 넘버 컨트롤
	HOEC				m_hPrevOec;																// 이 핸들로 넘어오기 이전의 컨트롤
	BOOL				m_bPrevOecEnable;														// 이전 핸들을 활성화 시킬 것이냐
	SI16				m_bPrevOecShortKey;

	SI32                m_siUnit;                                                               // 단위

	XSPR				*pBorderSpr;															// 보더 그림
	XSPR				*pButton1;																// 버튼 그림
	XSPR				*pButton2;																// 버튼 그림
	XSPR				*pButton3;																// 버튼 그림
	XSPR				*pButton4;																// 버튼 그림

	CHAR				m_pszBuffer[512];														// 숫자를 저장 하고 있는 버퍼	
	
	BOOL				bAction;																// 활성화 되어 있나?
	BOOL				bMouseDown;																// 마우스 버튼 상태
	BOOL				bSuccess;																// 성공	
	__int64				*pNumber;
	SI16				siX, siY;
	__int64				MaxNumber;
	__int64				MinNumber;

	_Button				BAll, BOk, BCancel, BNumPad[11];

	// Market Item Data
	SI32				NumberControlInfoType;
	SI32				ItemMoney;
	SI32				ItemName;
	SI32				OKButtonName;

	__int64				NowInstmentMoney;
	__int64				MyInstmentMoney;
};


#endif
