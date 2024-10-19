#ifndef __ON_NUMBER_CONTROL_H
#define __ON_NUMBER_CONTROL_H


#define NUMBERCONTROL_INFOTYPE_NORMAL				0					// Info Type : ����
#define NUMBERCONTROL_INFOTYPE_MARKET				1					// Info Type : ����
#define NUMBERCONTROL_INFOTYPE_BANK					2					// Info Type : ����
#define NUMBERCONTROL_INFOTYPE_GOVERNMENTINVEST		3					// Info Type : ��û, ����

#define NUMBERCONTROL_UNIT_MONEY                    0                   // ���Ǵ��� ( �� : ��)
#define NUMBERCONTROL_UNIT_THING                    1                   // ���� (�� : ��)
#define	NUMBERCONTROL_UNIT_ARMY						2                   // ������ �δ�� (�� : �δ�) robypark 2004/11/18 16:46


/********************************************************************************************
	���ϸ�	:	OnlineNumberControl.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.07.23
	������	:	2001.12.10
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
	VOID				Draw( LPDIRECTDRAWSURFACE7 lpSurface );									// ���
	VOID				Init( __int64 *pCount, cltOnlineWorld *pOnlineWorld, __int64 MaxNumberCount = -1);
	BOOL				IsShow();																// ���̴°�?
	BOOL				IsSuccess();															// ���� �ߴ°�?
	VOID				SetShow( SI16 x, SI16 y );												// ���̰� �Ѵ�.
	VOID				SetShowCenter(void);														// ���̰� �Ѵ�.
	VOID				Initialize(__int64 MaxNumberCount = -1);									// �ʱ�ȭ
	VOID				InitializeMarket(SI32 ItemMoney, SI32 ItemName, SI32 OKButtonName, __int64 MaxNumberCount = -1);															// �ʱ�ȭ(������)
	VOID				InitializeBank(__int64 MaxNumberCount = -1);																											 	// �ʱ�ȭ(�������)
	VOID				InitializeGovernmentInvestment(__int64 MaxNumberCount = -1, __int64 MinNumberCount = -1, __int64 NowInvestmentMoney = -1, __int64 MyInvestmentMoney = -1);	// �ʱ�ȭ(��û, ���ڽ�)
	VOID				Free();																	// ����

	VOID                SetUnit(SI32 siUnit);                                                   // ��������

	// robypark 2004/12/31 15:25
	// ������ ��Ʈ���� �ݴ´�. (��ҹ�ư Ŭ���� ������ ó��)
	VOID				CloseNumberControl(VOID);

private:
	VOID				KeyMessage();															// Ű �޽����� �޴´�.

private:
	cltOnlineWorld		*pMyOnlineWorld;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	OnlineEditControl	*pEditBox;
//	HWND				m_YetEditHWND;
	HOEC				m_hOec;																	// �ѹ� ��Ʈ��
	HOEC				m_hPrevOec;																// �� �ڵ�� �Ѿ���� ������ ��Ʈ��
	BOOL				m_bPrevOecEnable;														// ���� �ڵ��� Ȱ��ȭ ��ų ���̳�
	SI16				m_bPrevOecShortKey;

	SI32                m_siUnit;                                                               // ����

	XSPR				*pBorderSpr;															// ���� �׸�
	XSPR				*pButton1;																// ��ư �׸�
	XSPR				*pButton2;																// ��ư �׸�
	XSPR				*pButton3;																// ��ư �׸�
	XSPR				*pButton4;																// ��ư �׸�

	CHAR				m_pszBuffer[512];														// ���ڸ� ���� �ϰ� �ִ� ����	
	
	BOOL				bAction;																// Ȱ��ȭ �Ǿ� �ֳ�?
	BOOL				bMouseDown;																// ���콺 ��ư ����
	BOOL				bSuccess;																// ����	
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
