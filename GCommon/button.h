/* 
	�պ��� ������(��.�� + õ.�� = ��ư)
	ǥ�� ���
	1. ������ : �����Ͽ��� �ִ� �κ�
	2. õ  �� : õ���� ��ȭ���� �ִ� �κ�
*/

#ifndef _BUTTON_HEADER
#define	_BUTTON_HEADER

#include "..\Gersang\buttonssprmanagement.h"


#define		BUTTON_PUT_DEFAULT			((DWORD)0)
#define		BUTTON_PUT_LEFT				((WORD)1)
#define		BUTTON_PUT_CENTER			((WORD)2)
#define		BUTTON_PLAYSOUND_ONMOUSE	((WORD)4)
#define		BUTTON_PUT_NOMOVE			((DWORD)1)
#define		BUTTON_PUT_NOBORDER			((DWORD)2)
#define		BUTTON_PUT_NODARK			((DWORD)4)
#define		BUTTON_PUT_FILLCOLOR		((DWORD)8)

#define		BUTTON_VIEW_LIST_GLOBAL		0		// �����Ͽ���
#define		BUTTON_VIEW_LIST_LOCAL		1		// �����Ͽ���

#define		BOX_PUT_LEFT				((WORD)1)
#define		BOX_PUT_CENTER				((WORD)2)
#define		BOX_PUT_ORIGINAL			((WORD)3)
#define		BOX_PUT_DEFAULT				((DWORD)0)
#define		BOX_PUT_NOBORDER			((DWORD)1)

#define		GLOBAL_BUTTON_NUM						8	
#define		BUTTON_GLOBAL_DECIDE					0
#define		BUTTON_GLOBAL_BACK						1
#define		BUTTON_GLOBAL_EXIT						2
#define		BUTTON_GLOBAL_SEARCH					3
#define		BUTTON_GLOBAL_CREATE					4
#define		BUTTON_GLOBAL_JOIN						5
#define		BUTTON_GLOBAL_REPLAY					6
#define		BUTTON_GLOBAL_GOPLAY					7

//��ư�� ���� ũ�� ����
#define		BUTTON_WIDTH						80
#define		BUTTON_HEIGHT						24

//�����ϱ� ��ư�� ���� ����
#define		BUTTON_DECIDE_X						495
#define		BUTTON_DECIDE_Y						419

//���ư��� ��ư�� ���� ����
#define		BUTTON_BACK_X						65
#define		BUTTON_BACK_Y						BUTTON_DECIDE_Y

//�����ϱ� ��ư�� ���� ����
#define		BUTTON_EXIT_X						BUTTON_BACK_X
#define		BUTTON_EXIT_Y						BUTTON_DECIDE_Y

//�˻��ϱ� ��ư�� ���� ����
#define		BUTTON_SEARCH_X						208
#define		BUTTON_SEARCH_Y						BUTTON_DECIDE_Y

//�游��� ��ư�� ���� ����
#define		BUTTON_CREATE_X						BUTTON_DECIDE_X
#define		BUTTON_CREATE_Y						BUTTON_DECIDE_Y

//�����ϱ� ��ư�� ���� ����
#define		BUTTON_JOIN_X						352
#define		BUTTON_JOIN_Y						BUTTON_DECIDE_Y

//�ٽú��� ��ư�� ���� ����
#define		BUTTON_REPLAY_X						280
#define		BUTTON_REPLAY_Y						BUTTON_DECIDE_Y

//�����ϱ� ��ư�� ���� ����
#define		BUTTON_GOPLAY_X						BUTTON_DECIDE_X						
#define		BUTTON_GOPLAY_Y						BUTTON_DECIDE_Y


//���� ��ư���� ���� ȭ�� �̸��� ��Ʈ �ѹ�.
#define		BUTTON_NUMBUTTONS_SPR_FILENAME		"yfnt\\lobbynumbutton.spr"
#define		BUTTON_FONTNUM_ONE					0
#define		BUTTON_FONTNUM_TWO					1
#define		BUTTON_FONTNUM_THREE				2
#define		BUTTON_FONTNUM_FOUR					3
#define		BUTTON_FONTNUM_FIVE					4
#define		BUTTON_FONTNUM_SIX					5
#define		BUTTON_FONTNUM_SEVEN				6
#define		BUTTON_FONTNUM_EIGHT				7

//���� ��ư���� ���� ȭ�� �̸��� ��Ʈ �ѹ�.
#define		BUTTON_NATIONBUTTONS_SPR_FILENAME	"yfnt\\lobbynationbutton.spr"
#define		BUTTON_FONTNUM_KOREA				0
#define		BUTTON_FONTNUM_JAPAN				1
#define		BUTTON_FONTNUM_CHINA				2
#define		BUTTON_FONTNUM_TAIWAN				3

class XSPR;

// �⺻��ư
class _Button
{
protected:			
	BOOL		m_bAction;											//�� ��ư�� Ȱ���� �ϴ� �� ��Ÿ����.	(�ð������� ��ο�����.)
	BOOL		m_bProcess;											//�� ��ư�� ó���� �־�� �ϴ��� ��Ÿ����.	m_bAction���� ���̴� ��»��� �����̴�.	(�ð������� ������ �ʴ´�.)
	CHAR		m_MenuName[128];

	SHORT		m_x;
	SHORT		m_y;
	SHORT		m_Width;
	SHORT		m_Height;
	BOOL		m_bSoundPush;	

	BOOL		MbPrevFrameInButton;								//���� �����ӿ� ���콺�� ���� ��ư ���� �־�����...

	WORD		MuiFlag;

	// robypark 2004/11/15 14:3
	// ��ư ���� �Ӽ����� Visible�� �߰�
	BOOL		m_bVisibleWindow;

protected:
	virtual		VOID		PutText(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, SHORT Width, SHORT Height, LPDIRECTDRAWSURFACE7 lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);

	// ������ 2 PLUS������ ���
	virtual		VOID		PutTextPlus(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutTextPlus(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);

public:
	_Button();
	_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR *lpMenuName, BOOL bAction = TRUE);
	_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR *lpMenuName, WORD flag, BOOL bAction = TRUE);

	virtual		BOOL		IsAction() { return m_bAction;}
	virtual		BOOL		IsProcess()	{ return m_bProcess;}
	virtual		VOID		Create(INT x, INT y, INT m_Width, INT m_Height, CHAR* MenuName, BOOL bAction = TRUE);	
	virtual		VOID		Create(INT x, INT y, INT m_Width, INT m_Height, CHAR* MenuName, WORD flag, BOOL bAction = TRUE);	

	// ��ư�� ��ġ�� ���ҽÿ�
	virtual		VOID		SetX(SHORT NewX);
	virtual		VOID		SetY(SHORT NewY);

	virtual		VOID		SetMenuName(CHAR* lpNewMenuName);
	virtual		VOID		SetAction(BOOL bAction);
	virtual		VOID		SetProcess(BOOL bProcess);

	virtual		VOID		SetSelect(BOOL bSelect) {MbPrevFrameInButton = bSelect;}
	virtual		BOOL		IsSelect(VOID) {return MbPrevFrameInButton;}

	// ��ư�� ��ġ�� ���ϴ� �Ͷ����� �׻� 2���� �̷���� ����(�� : ��ư ������, �� : ��ư �����϶�)
	virtual		BOOL		IsInMouse(); 
	virtual		BOOL		IsInMouse(INT x, INT y);

	virtual		BOOL		IsPressing(BOOL bPush);
	virtual		BOOL		IsPressing(INT x, INT y, BOOL bPush);

	virtual		BOOL		Process(BOOL bPush);		
	virtual		BOOL		Process(INT x, INT y, BOOL bPush);

	virtual		VOID		Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT);

	virtual		VOID		Put(LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag = BUTTON_PUT_DEFAULT); // ������ ��
	virtual		VOID		Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag = BUTTON_PUT_DEFAULT);	

	virtual		VOID		Put(XSPR* lpXspr, SHORT FontNumber);
	virtual		VOID		Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT FontNumber);
	
	virtual		VOID		Put1(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put1(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);
	virtual		VOID		Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);

	virtual		VOID		Put(XSPR* lpXspr, SI16 siDefaultFontNum,	SI16 siSelectedFontNum,		SI16 siOnMouseFontNum, 
											  BOOL bDefaultFontTrans,	BOOL bSelectedFontTrans,	BOOL bOnMouseTrans,
												DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put(SI16 siX, SI16 siY, XSPR* lpXspr, SI16 siDefaultFontNum,	SI16 siSelectedFontNum,		SI16 siOnMouseFontNum, 
																  BOOL bDefaultFontTrans,	BOOL bSelectedFontTrans,	BOOL bOnMouseTrans,
								DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	
	// ��ư�� ����� �����ϰ� ������ : �����Ͽ���(�� ���ý� �������� Ȯ��, ��� ��ư�� ���)
	virtual		VOID		Put2(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, SHORT NoActionFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT); 

	virtual		VOID		Put1(HDC hdc, DWORD PutFlag = BUTTON_PUT_DEFAULT);
	virtual		VOID		Put(HDC hdc, DWORD PutFlag = BUTTON_PUT_DEFAULT, COLORREF OffTextColor = RGB(161, 161, 161), COLORREF OnTextColor = RGB(219, 213, 200));
	virtual		VOID		Put1(HDC hdc, SHORT x, SHORT y, DWORD PutFlag = BUTTON_PUT_DEFAULT);
	virtual		VOID		Put(HDC hdc, SHORT x, SHORT y, COLORREF	OffTextColor, COLORREF	OnTextColor, DWORD PutFlag = BUTTON_PUT_DEFAULT);

	virtual		VOID		Put(HDC hdc, SI16 siX, SI16 siY, SI16 siWitdh, SI16 siHeight, DWORD PutFlag = BUTTON_PUT_DEFAULT); // ������

	virtual		VOID		PutSpriteButton(SI16 siX, SI16 siY, XSPR *pSpr, SI16 siFont, BOOL bDark, BOOL bTrans);						// ��������Ʈ�� �̿��Ͽ� ��ư�� ��´�.
	
	// ��ư�� ũ�⸦ ��� ���� �Լ�
	virtual		SHORT		GetX() {return m_x;}
	virtual		SHORT		GetY() {return m_y;}
	virtual		SHORT		GetWidth() {return m_Width;}
	virtual		SHORT		GetHeight() {return m_Height;}	
	virtual		CHAR*		GetMenuName() {return m_MenuName;}
	virtual		RECT		GetSize();
	
	// robypark 2004/11/15 14:5
	// �� ��ư ������ ���̴����� ���´�.
	BOOL		IsVisibleWindow(void);

	// robypark 2004/11/15 14:5
	// �� ��ư ������ ���̴����� �����Ѵ�.
	void		ShowWindow(BOOL bVisible);
}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���ù�ư
class	_SelectButton : public _Button
{
private:
	SHORT			m_TextPutX;
	SHORT			m_TextPutY;

	//���콺 ���� Ŭ���� ����� ������..
	static	BOOL	m_PrevMousePressedTime;		// ������
	static	BOOL	m_IsMouseLeftButtonPressed; // ������

protected:
	BOOL			m_bSelected;

	virtual	BOOL	IsDoubleClick();			// ������
public:	
	_SelectButton();
	_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, BOOL bAction = TRUE);	
	_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, WORD flag, BOOL bAction = TRUE);	
	virtual ~_SelectButton();

	virtual	VOID	Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, BOOL bAction = TRUE);	// õ��
	virtual	VOID	Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, BOOL bAction = TRUE);	// õ��
	virtual	VOID	Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, WORD flag,  BOOL bAction = TRUE); // õ��
	
	virtual	VOID	Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);

	/* �����ϰ� õ�� �ٸ� �κ�(7 Line) */
	virtual	VOID	Put(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);	// ������
	virtual	VOID	Put(HDC hdc, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);  // ������

	virtual	VOID	Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT); // õ��
	virtual	VOID	Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);	// õ��
	virtual	VOID	Put(INT x, INT y, INT TextPutX, INT TextPutY, LPDIRECTDRAWSURFACE7 lpSurface,XSPR *pXspr = NULL, SHORT FontNum = 0, DWORD PutFlag = BUTTON_PUT_DEFAULT, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);	// õ��
	virtual	VOID	Put(INT x, INT y, INT TextPutX, INT TextPutY, HDC hdc, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);  // õ��

	// ������ 2PLUS������ �����...
	virtual	VOID	PutPlus(HDC hdc, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);  // ������2 Plus������ ���

	/* �����ϰ� õ�� �ٸ� �κ� - õ������ ���� */
	virtual	BOOL	Process(BOOL bPush, BOOL *pbIsDoubleClick = NULL);		// õ��
	virtual	BOOL	Process(INT x, INT y, BOOL bPush, BOOL *pbIsDoubleClick = NULL);	// õ��

	virtual	VOID	SetSelected(BOOL bSelected);	
	virtual	BOOL	IsSelected();
	virtual	VOID	SetTextPutX(SHORT TextPutX);
	virtual	VOID	SetTextPutY(SHORT TextPutY);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������
class _ServiceProviderButton : public _SelectButton
{	

public:	
	_ServiceProviderButton();
	_ServiceProviderButton(SHORT x, SHORT y, SHORT width, SHORT height, char* lpMenuName);	
	virtual ~_ServiceProviderButton();	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������
class _EnumSessionButton : public _SelectButton
{	
public:
	//LPGUID	m_lpGuid;

	_EnumSessionButton();
	_EnumSessionButton(SHORT x, SHORT y, SHORT width, SHORT height, char* lpMenuName);
	virtual ~_EnumSessionButton();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �̰� �ش��� ����
class _MapFileInfo  
{	
public:
	CHAR		m_szFileName[1024];			// õ��
	SHORT		m_MaxPlayerNum;				// õ�� & ������
	DWORD		m_Unique;					// õ��
	SI16		m_siMapXsize;				// õ��
	SI16		m_siMapYsize;				// õ��
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class _SelectMapFileButton : public _SelectButton
{	
public:	
	_MapFileInfo				m_MapFileInfo;

	_SelectMapFileButton();  // ������
	_SelectMapFileButton(SHORT x, SHORT y, SHORT width, SHORT height, char* lpMenuName); // ������
	virtual ~_SelectMapFileButton();  // ������
	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class _Box
{
private:
	SHORT		m_x;
	SHORT		m_y;
	SHORT		m_Width;
	SHORT		m_Height;
	CHAR		m_Name[128];

protected:
	virtual		VOID		PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, HDC	hdc, CHAR *lpText, WORD TextArrange = BUTTON_PUT_CENTER);
public:
	_Box();
	_Box(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* lpName, WORD PutFlag);
	virtual		~_Box();

	virtual		VOID		Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag);
	virtual		VOID		Put(HDC	hdc, DWORD	PutFlag = BOX_PUT_DEFAULT, WORD PutTextFlag = BOX_PUT_DEFAULT);	    
	virtual		VOID		Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BOX_PUT_DEFAULT, WORD PutTextFlag = BOX_PUT_DEFAULT);	

	virtual		SHORT		GetX();
	virtual		SHORT		GetY();	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class _ImageButton	: public _Button
{
	BYTE*		m_lpSampleBuffer;
	BYTE		m_SelectColor;
	SHORT		m_SampleXsize;	
public:
	_ImageButton();
	virtual	~_ImageButton();	

	virtual		VOID	Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* MenuName, BYTE* lpSampleBuffer, BYTE Color, WORD siFlag);	
	virtual		VOID	Create(SHORT x, SHORT y, SHORT Width, SHORT Height, BYTE* lpSampleBuffer, BYTE Color, SHORT nSampleXsize, WORD siFlag);	
	virtual		BOOL	IsInMouse(); 
	virtual		BOOL	Process(BOOL bPush);
	virtual		VOID	Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag);
	virtual		VOID	Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag);
	virtual		VOID	Put(HDC hdc, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ư���� �ʱ�ȭ �Ѵ�.
VOID	OnceInitButton();
VOID	SetActionAllButton(BOOL bAction);

//��ư���� �ܺκ���.
extern	_Button	g_Button[GLOBAL_BUTTON_NUM];
extern	XSPR	g_Buttons_Spr[TOTAL_BUTTONS_KINDOF_NUM];				//��ư�� �ҷ����� ���� ��������Ʈ ����.
extern	XSPR	GOverButtonSpr;											// õ��

BOOL	LoadAllButtons();												//��� ��ư�� ��������Ʈ�� �ε��Ѵ�.
VOID	FreeAllButtons();												//��� ��ư�� ��������Ʈ�� ���� �Ѵ�.
#endif

