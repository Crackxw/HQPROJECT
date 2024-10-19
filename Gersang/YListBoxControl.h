#ifndef	IMJIN2_LISTBOXCONTROL_HEADER
#define IMJIN2_LISTBOXCONTROL_HEADER

#include	"ycontrol.h"
//#include	"setmulti.h"

#define		MAX_LIST_BOX_NUM								10			//ListBox�� �ִ� ������ �� �ִ� �ν��Ͻ� ��

#define		MAX_LIST_BOX_SELL								100
#define		LISTBOX_SERVICEPROVIDER_UPDOWN_PRESS_DELAY		200

#define		CONTROL_SCROLL_MIN_HEIGHT						20

//���� ���ι��̴��� ����Ʈ �ڽ�.(SetMulti)
#define		LISTBOX_SERVICEPROVIDER_X				68						
#define		LISTBOX_SERVICEPROVIDER_Y				266					
#define		LISTBOX_SERVICEPROVIDER_WIDTH			376
#define		LISTBOX_SERVICEPROVIDER_HEIGHT			128

#define		LISTBOX_SERVICEPROVIDER_SELLWIDTH		300
#define		LISTBOX_SERVICEPROVIDER_SELLHEIGHT		20

//���� ������ ����Ʈ �ڽ�.
#define		LISTBOX_ENUMSESSION_X					98
#define		LISTBOX_ENUMSESSION_Y					114
#define		LISTBOX_ENUMSESSION_WIDTH				451
#define		LISTBOX_ENUMSESSION_HEIGHT				170

#define		LISTBOX_ENUMSESSION_SELLWIDTH			LISTBOX_SERVICEPROVIDER_SELLWIDTH
#define		LISTBOX_ENUMSESSION_SELLHEIGHT			LISTBOX_SERVICEPROVIDER_SELLHEIGHT			

//���� ������ ����Ʈ �ڽ�.
#define		MAX_LISTBOX_SELECTMAPFILE_SELL			300

#define		LISTBOX_SELECTMAPFILE_SELLWIDTH			LISTBOX_SELECTMAPFILE_PUTTEXT_WIDTH
#define		LISTBOX_SELECTMAPFILE_SELLHEIGHT		20

//����Ʈ �ڽ����� ��ũ�ѿ� ���� ��Ʈ...
//��ũ�� �� �ٿ� ��ư�� ���� ȭ�� �̸��� ��Ʈ �ѹ�.
#define		CONTROL_SCROLL_UP_DOWN_SPR_FILENAME				"yfnt\\scroll-up-down.spr"
#define		CONTROL_FONTNUM_UP_UNPRESSED					0
#define		CONTROL_FONTNUM_UP_PRESSED						1
#define		CONTROL_FONTNUM_DOWN_UNPRESSED					2
#define		CONTROL_FONTNUM_DOWN_PRESSED					3

//��ũ�� �ٿ� ���� ȭ�� �̸��� ��Ʈ �ѹ�.
#define		CONTROL_SCROLL_BAR_SPR_FILENAME					"yfnt\\scrollbar.spr"
#define		CONTROL_FONTNUM_SCROLLBAR						0

class _YListBox : public _YControl
{	
protected:
	static	LPDIRECTDRAWSURFACE7	m_lpScrollBarSurface;
protected:
	static	XSPR					m_UpDownSpr;
	static	XSPR					m_BarSpr;
	static	XSPR					m_BarEdgeSpr;
	static	SHORT					m_nReferenceCount;	
protected:
	RECT							m_rcUpButton;
	RECT							m_rcDownButton;
	RECT							m_rcBarInfo;								

protected:
	BOOL					m_bAction;
	BOOL					m_bProcess;

	SHORT					m_SelectedSell;
	SHORT					m_VisibleSellNum;
	SHORT					m_StartSellNumber;
	SHORT					m_SellWidth;
	SHORT					m_SellHeight;	
	SHORT					m_SellNum;		
	SHORT					m_PageUnit;
	char					m_lplpSellName[MAX_LIST_BOX_SELL][128];
	LPVOID					m_lplpData[MAX_LIST_BOX_SELL];
	BOOL					m_bUpPressed;
	BOOL					m_bDownPressed;

	SI16					m_siUpDefaultFont;
	SI16					m_siUpPressedFont;
	SI16					m_siDownDefaultFont;
	SI16					m_siDownPressedFont;
	SI16					m_siScrollBarStartFont;
protected:	
	SHORT					m_PutTextX;
	SHORT					m_PutTextY;
	SHORT					m_PutTextWidth;
	SHORT					m_PutTextHeight;

	BOOL					IsValidIndex(SHORT Index);	
	BOOL					IsTopSell();
	BOOL					IsBottomSell();
	RECT					GetRectPageUpButton();
	RECT					GetRectPageDownButton();
	BOOL					IsPressedUpButton();
	BOOL					IsPressedDownButton();
	BOOL					IsPressedPageUpButton();
	BOOL					IsPressedPageDownButton();	
	VOID					GetStartSellOnMovingListBar();		
	
public:
	_YListBox();
	~_YListBox();

	virtual		void		Create(SHORT x, SHORT y, SHORT width, SHORT height, 
								   SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight,
									SI16 siUpDefaultFont, SI16 siUpPressedFont, SI16 siDownDefaultFont, SI16 siDownPressedFont, SI16 siScrollBarStartFont);	
	virtual		SHORT		GetWidth();
	virtual		SHORT		GetHeight();
	virtual		SHORT		GetSellWidth();
	virtual		SHORT		GetSellHeight();
	virtual		SHORT		GetCurrentPosition();
	virtual		SHORT		GetVisibleSellNum();
	virtual		SHORT		GetStartSellNumber();
	virtual		VOID		RefreshData();
	virtual		VOID		Draw();	
	virtual		VOID		DrawScrollBar(); // ������ 2������ ���		
	virtual		VOID		Action(BOOL bPush);
	virtual		SHORT		AddSell(LPVOID lpData, CHAR *SellName);
	virtual		VOID		DelSell(SHORT SellIndex);
	virtual		LPVOID		GetData(SHORT SellIndex);
	virtual		SHORT		GetSellNum();
	virtual		VOID		SetPosition(SHORT NewPosition);
	virtual		SHORT		NextSell();
	virtual		SHORT		PrevSell();			
	virtual		RECT		GetBarInfo();	
	virtual		SHORT		NextPage();
	virtual		SHORT		PrevPage();	
	virtual		VOID		SetAction(BOOL bAction);					// ����Ʈ �ڽ��� ó������ �ʴ´�.(�ð������ε� ��Ÿ����)
	virtual		VOID		SetProcess(BOOL bProcess);					// ����Ʈ �ڽ��� ó������ �ʴ´�.
	virtual		VOID		DelAllSell();								// ����Ʈ �ڽ����� ��� �������� ���ش�.
};


class _YServiceProviderListBox : public _YListBox
{
private:		

public:
	_YServiceProviderListBox();
	~_YServiceProviderListBox();

	BOOL						Create();	
	VOID						Action(BOOL bPush, BOOL& IsDoubleClicked, UI32& SelectedServiceProvider);
	VOID						AllDestroyServiceProvider();	
	BOOL						AddSell(CHAR *MenuName);
	VOID						Draw();					
	VOID						SetPosition(SHORT NewPosition);
	SHORT						NextSell();
	SHORT						PrevSell();	
	VOID						SetAction(BOOL	Action);
	VOID						SetProcess(BOOL bProcess);	
};

class _YEnumSessionListBox : public _YListBox
{
private:

public:
	_YEnumSessionListBox();
	~_YEnumSessionListBox();

	BOOL						Create();				
	BOOL						AddSell(CHAR *MenuName);
	VOID						Draw();						
	VOID						SetPosition(SHORT NewPosition);
	SHORT						NextSell();
	SHORT						PrevSell();	
	VOID						SetAction(BOOL	bAction);	

	VOID						Action(BOOL bPush, BOOL& IsDoubleClicked, UI32& SelectedSession);	
	VOID						AllDestroySession();	

};

class _YSelectMapFileListBox : public _YListBox
{
private:	
	_SelectMapFileButton		*m_lpBSelectMapFiles;
	HANDLE						m_FileSearchHandle;
	WIN32_FIND_DATA				m_ffd;
protected:
	VOID	StartSearchMapFile();
	BOOL	IsSearchMapFile(CHAR *GetFileName, CHAR *GetFullFileName);
	VOID	EndSearchMapFile();
public:
	_YSelectMapFileListBox();
	~_YSelectMapFileListBox ();

	BOOL						Create();				
	VOID						Draw();					
	VOID						SetPosition(SHORT NewPosition);
	SHORT						NextSell();
	SHORT						PrevSell();	
	VOID						SetAction(BOOL bAction, SHORT PlayerNum = 0);
	VOID						SetProcess(BOOL bProcess);
	BOOL						AddSell(CHAR* MenuName, _MapFileInfo* lpMapFileInfo);	
	_MapFileInfo*				GetMapFileInfo(SHORT nIndex);				
	CHAR*						GetFileName(SHORT Index);
	CHAR*						GetFileName();
	VOID						Action(BOOL bPush);	
	VOID						AllDestroyFile();			
	VOID						CutFileExpansion(CHAR* szFileName);	
	BOOL						IsUseCurrentSelectedMap();				//���� ������ ������ ����� �� �ִ� �����ΰ�..
	BOOL						IsHaveMap(CHAR* szFileName);
};

#endif
