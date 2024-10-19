#include <GSL.h>
#include <main.h>
#include <clGame.h>

#include <player.h>
#include <etc.h>
#include <config.h>
#include "music.h"

#include "YListButton.h"
#include "ytitlefile.h"
#include "text.h"


extern	_clGame					*pGame;
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	char 				SavePath[];
extern	_Config 			Config;								// ȯ�� ���� 

BOOL	_LinkedPressButton::m_bViewListGlobal = FALSE;

SHORT	_SelectNationButton::m_NationArray[6]={COUNTRY_KOREA, COUNTRY_JAPAN, COUNTRY_CHINA, COUNTRY_RANDOM, COUNTRY_NO, COUNTRY_NEUTRAL}; 
SHORT	_SelectAllyButton::m_AllyArray[MAX_MULTI_PLAYER] = {0, 1, 2, 3, 4, 5, 6, 7};


//////////////////////////////////_LinkedPressButton////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// _LinkedPressButton::_LinkedPressButton()
////////////////////////////////////////////////////////////////////////////////////////////////////////
_LinkedPressButton::_LinkedPressButton()
{
	m_ButtonNum = 0;
	m_lpButton = NULL;
	m_CurrentIndex = 0;
	m_bViewListLocal = FALSE;	
	m_ListButtonPosData = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// _LinkedPressButton::~_LinkedPressButton()
////////////////////////////////////////////////////////////////////////////////////////////////////////
_LinkedPressButton::~_LinkedPressButton()
{
	Free();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL _LinkedPressButton::Create(SHORT ButtonNum, SHORT x, SHORT y, SHORT Width, SHORT Height, char lplpButtonNames[][128], int flag)
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _LinkedPressButton::Create(SHORT ButtonNum, SHORT x, SHORT y, SHORT Width, SHORT Height, SHORT* lpListbuttonPosData, int flag)
{
	int		i;
	
//	if(m_lpButton)	clGrp.Error("FKJE8567", "YYLISTBUTTON0000012");
		
	

	m_lpButton				= new _Button[ButtonNum];
	m_ListButtonPosData		= new SHORT[ButtonNum*2];

	if(m_lpButton == NULL)
	{
		clGrp.Error("FKJE8567", "YYLISTBUTTON0000013");		
		return FALSE;
	}

	for(i=0; i < ButtonNum; i++)
	{
		m_lpButton[i].Create(x, y, Width, Height, "", (WORD)flag);				
		m_ListButtonPosData[i*2] = lpListbuttonPosData[i * 2];
		m_ListButtonPosData[i*2 + 1] = lpListbuttonPosData[i * 2 + 1];
	}	

	m_ButtonNum = ButtonNum;

	return TRUE;
}

//------------------------------------------------------------------------------------------------
//	����				:	��ũ ��ư�� �����Ѵ�.
//	ButtonNum			:	��ư�� ����.
//	x, y				:	��ǥ.
//	Width, Height		:	�� ��ư�� ũ��.
//	strLinkButtonInfo	:	�� ��ũ ��ư�� ���� ����. (�� ��ư���� ��ġ�� �̸��� �ʿ�� �Ѵ�.)
//	flag				:	�ɼ�.
//------------------------------------------------------------------------------------------------
BOOL	_LinkedPressButton::Create(SHORT ButtonNum, SHORT x, SHORT y, SHORT Width, SHORT Height, strLinkButtonInfo *pLinkButtonInfo, INT flag)
{
	int		i;
	
//	if(m_lpButton)	clGrp.Error("FKJE8567", "YYLISTBUTTON0000012");	

	m_lpButton				= new _Button[ButtonNum];
	m_ListButtonPosData		= new SHORT[ButtonNum*2];

	if(!m_lpButton && !m_ListButtonPosData)
	{
		clGrp.Error("FKJE8567", "YYLISTBUTTON0000013");		
		return FALSE;
	}

	for(i=0; i < ButtonNum; i++)
	{
		m_lpButton[i].Create(x, y, Width, Height, pLinkButtonInfo[i].szButtonName, (WORD)flag);				

		m_ListButtonPosData[i*2]		=	pLinkButtonInfo[i].x;
		m_ListButtonPosData[i*2 + 1]	=	pLinkButtonInfo[i].y;
	}	
	
	m_ButtonNum = ButtonNum;	

	return TRUE;
}
//------------------------------------------------------------------------------------------------
//	����			: ��ư�� �������� ��ư�� �����Ѵ�.(Ÿ���� ������ ���� ��ư���� ���)
//	x, y			: ��ǥ.
//	Width, Height		: �� ��ư�� ũ��.
//	strLinkButtonInfo	: �� ��ũ ��ư�� ���� ����. (�� ��ư���� ��ġ�� �̸��� �ʿ�� �Ѵ�.)
//	flag			: �ɼ�.
//------------------------------------------------------------------------------------------------
BOOL	_LinkedPressButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, strLinkButtonInfoEx *pLinkButtonInfo, INT flag)
{
	int		i;
	
//	if(m_lpButton)	clGrp.Error("FKJE8567", "YYLISTBUTTON0000012");
		
	// ������ �˾ƿ´�.
	SI16 siCount	=	0;

	while(strcmp(pLinkButtonInfo[siCount].szButtonName, "") != 0)
	{
		siCount++;		
	}

//	if(m_lpButton)	clGrp.Error("FKJE8567", "YYLISTBUTTON0000012");	

	m_lpButton				= new _Button[siCount];
	m_ListButtonPosData		= new SHORT[siCount*2];

	if(!m_lpButton && !m_ListButtonPosData)
	{
		clGrp.Error("FKJE8567", "YYLISTBUTTON0000013");		
		return FALSE;
	}

	for(i=0; i < siCount; i++)
	{
		m_lpButton[i].Create(x, y, Width, Height, pLinkButtonInfo[i].szButtonName, (WORD)flag);				

		m_ListButtonPosData[i*2]		=	pLinkButtonInfo[i].x;
		m_ListButtonPosData[i*2 + 1]	=	pLinkButtonInfo[i].y;
	}	
	
	m_ButtonNum = siCount;	

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� :  VOID _LinkedPressButton::Free()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _LinkedPressButton::Free()
{
	if(m_lpButton)	
	{
		delete [] m_lpButton;
		m_lpButton = NULL;
	}

	if(m_ListButtonPosData)
	{
		delete [] m_ListButtonPosData;
		m_ListButtonPosData = NULL;
	}

	m_ButtonNum = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL _LinkedPressButton::Process(BOOL bPush)
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _LinkedPressButton::Process(BOOL bPush)
{
	//���콺 ��ġ�� ��ư ���� �ȿ� �ְ� ��ư�� �������� �����̶��
	//������ ��ư�� ������ ���̴�.
	if(m_lpButton[m_CurrentIndex].IsAction() == TRUE)
	{
		if((m_lpButton[m_CurrentIndex].IsInMouse()== TRUE) && 
			(bPush == TRUE) && (m_bViewListLocal == FALSE) && (m_bViewListGlobal  == FALSE))
		{
			m_bViewListLocal = TRUE;				
			m_bViewListGlobal = TRUE;
			
			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			return TRUE;
		}	
		else if((bPush == FALSE) && (m_bViewListLocal == TRUE) && (m_bViewListGlobal == TRUE))
		{
			//���� ��ư�� �������� ���¿���	��ư���� �������ٸ�.		
			m_CurrentIndex = GetButtonInList();	
			m_bViewListLocal = FALSE;				
			m_bViewListGlobal = FALSE;
		}
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _LinkedPressButton::GetButtonInList()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_LinkedPressButton::GetButtonInList()
{
	SHORT	i;	
		
	if(m_ListButtonPosData == NULL) clGrp.Error("FKJE8567", "YYLISTBUTTON0000001");			
	for(i=0; i < m_ButtonNum; i++)
	{
		if(m_lpButton[i].IsAction() == TRUE)
		{
			if(m_lpButton[i].IsInMouse(m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 +1]) == TRUE)		
				return i;		
		}
	}

	return m_CurrentIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL _LinkedPressButton::IsValidIndex(SHORT Index)
/// ���� : ��ȿ�� �ε������� �˻��Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _LinkedPressButton::IsValidIndex(SHORT Index)
{
	if(Index >= m_ButtonNum || Index < 0)
		return FALSE;
	else
		return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// SHORT _LinkedPressButton::NextButton()
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _LinkedPressButton::NextButton()
{	
	m_CurrentIndex++;
	m_CurrentIndex	%=	m_ButtonNum;

	return m_CurrentIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// SHORT _LinkedPressButton::GetButton()
/// ���� : ������ �����ϴ� �Լ��̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _LinkedPressButton::GetButton()
{
	return m_CurrentIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _LinkedPressButton::SetButton(SHORT Index)
/// ���� : ������ �����ϴ� �Լ��̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
void	_LinkedPressButton::SetButton(SHORT Index)
{
	m_CurrentIndex  = Index;
}

//------------------------------------------------------------------------------------------------
//	����		:	��ư�� Ȱ�� ���θ� �����Ѵ�.
//	bAction		:	Ȱ�� ����.
//	uiButtonID	:	������ ��ư�� ID.
//------------------------------------------------------------------------------------------------
VOID	_LinkedPressButton::SetButtonAction(BOOL bAction, UI16 uiButtonID)
{
	m_lpButton[uiButtonID].SetAction(bAction);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void	_LinkedPressButton::SetAction(BOOL bAction)
/// ���� : �� ��ư�� Ȱ���� �������� ���θ� ��Ÿ����.
////////////////////////////////////////////////////////////////////////////////////////////////
void	_LinkedPressButton::SetAction(BOOL bAction)
{
	int		i;

	// ��� ��ư�� ��Ȱ��ȭ ��Ų��.
	for(i=0; i < m_ButtonNum; i++)		
		SetButtonAction(bAction, i);		

	if(bAction==FALSE) 
	{
		if(m_bViewListGlobal == TRUE && m_bViewListLocal == TRUE) 
		{
			m_bViewListGlobal	= FALSE;	
			m_bViewListLocal	= FALSE;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	_LinkedPressButton::SetProcess(BOOL bProcess)
/// ���� : �� ��ư�� Ȱ���� �������� ���θ� ��Ÿ����.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_LinkedPressButton::SetProcess(BOOL bProcess)
{
	INT		i;

	for(i=0; i < m_ButtonNum; i++)	m_lpButton[i].SetProcess(bProcess);		
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL	_LinkedPressButton::IsViewList(SHORT KindOf)
/// ���� : ����Ʈ �ڽ��� �������� �ִ����� �����Ѵ�.
/// ���� : Kindof -   BUTTON_VIEW_LIST_GLOBAL ��ư ���� �ϳ��� ����Ʈ�� ���� �ִ���.
///					  BUTTON_VIEW_LIST_LOCAL  �ڱ� ��ư�� ����Ʈ�� ���� �ִ���.
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_LinkedPressButton::IsViewList(SHORT KindOf)
{
	if(KindOf == BUTTON_VIEW_LIST_GLOBAL)
	{
		return m_bViewListGlobal;
	}
	else if(KindOf == BUTTON_VIEW_LIST_LOCAL)
	{		
		return m_bViewListLocal;
	}
	
	clGrp.Error("FKJE8567", "YYLISTBUTTON0000010");
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID _SelectAllyButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height)
/// ���� : ������ �����ϱ� ���� ��ư�̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------
//	����			:	������ �����ϴ� ��ư�� �����
//	YAxis			:
//	x, y			:
//	Width, Height	:
//------------------------------------------------------------------------------------------------
VOID _SelectNationButton::Create(SHORT YAxis, SHORT x, SHORT y, SHORT Width, SHORT Height)
{		
	strLinkButtonInfo	stLinkButtonInfo[6];	

	stLinkButtonInfo[0].x	=	91;
	stLinkButtonInfo[0].y	=	YAxis;
	strcpy(stLinkButtonInfo[0].szButtonName, Text.Korea.Get());

	stLinkButtonInfo[1].x	=	138;
	stLinkButtonInfo[1].y	=	YAxis;
	strcpy(stLinkButtonInfo[1].szButtonName, Text.Japan.Get());

	stLinkButtonInfo[2].x	=	185;
	stLinkButtonInfo[2].y	=	YAxis;
	strcpy(stLinkButtonInfo[2].szButtonName, Text.China.Get());

	stLinkButtonInfo[3].x	=	232;
	stLinkButtonInfo[3].y	=	YAxis;
	strcpy(stLinkButtonInfo[3].szButtonName, Text.Random.Get());

	stLinkButtonInfo[4].x	=	279;
	stLinkButtonInfo[4].y	=	YAxis;
	strcpy(stLinkButtonInfo[4].szButtonName, Text.NoCountry.Get());

	stLinkButtonInfo[5].x	=	326;
	stLinkButtonInfo[5].y	=	YAxis;
	strcpy(stLinkButtonInfo[5].szButtonName, Text.Spectator.Get());

	stLinkButtonInfo[0].x	=	21;
	stLinkButtonInfo[0].y	=	YAxis;
	strcpy(stLinkButtonInfo[0].szButtonName, Text.Korea.Get());
	
	stLinkButtonInfo[1].x	=	65;
	stLinkButtonInfo[1].y	=	YAxis;
	strcpy(stLinkButtonInfo[1].szButtonName, Text.Japan.Get());
	
	stLinkButtonInfo[2].x	=	109;
	stLinkButtonInfo[2].y	=	YAxis;
	strcpy(stLinkButtonInfo[2].szButtonName, Text.China.Get());
	
	stLinkButtonInfo[3].x	=	153;
	stLinkButtonInfo[3].y	=	YAxis;
	strcpy(stLinkButtonInfo[3].szButtonName, Text.Random.Get());
	
	stLinkButtonInfo[4].x	=	197;
	stLinkButtonInfo[4].y	=	YAxis;
	strcpy(stLinkButtonInfo[4].szButtonName, Text.NoCountry.Get());
	
	stLinkButtonInfo[5].x	=	241;
	stLinkButtonInfo[5].y	=	YAxis;
	strcpy(stLinkButtonInfo[5].szButtonName, Text.Spectator.Get());


	_LinkedPressButton::Create(6, x, y, Width, Height, stLinkButtonInfo, BUTTON_PUT_LEFT);			
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID _SelectAllyButton::Free()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectNationButton::Free()
{	
	_LinkedPressButton::Free();			
}

//------------------------------------------------------------------------------------------------
//	����				:	��ư���� ó���Ѵ�.
//	bPush				:	���� ��ư�� ���� ����.
//------------------------------------------------------------------------------------------------
BOOL	_SelectNationButton::Process(BOOL bPush)
{	
	if(m_lpButton[m_CurrentIndex].IsAction() == TRUE)
	{
		// ���콺 ��ġ�� ��ư ���� �ȿ� �ְ� ��ư�� �������� �����̶��
		// ������ ��ư�� ������ ���̴�.
		// ���� m_bViewListLocal ���� m_bViewListGlobal�� �˻��Ͽ�
		// ���� ���� ������ �־��� ���� �ƴ����� �˾Ƴ���
		// ������ ���� �ʾ��������� ��ư�� ������ �ɷ� �Ѵ�.
		if((m_lpButton[m_CurrentIndex].IsInMouse() == TRUE) && 
			(bPush == TRUE) && (m_bViewListLocal == FALSE) && (m_bViewListGlobal  == FALSE))
		{
			m_bViewListLocal = TRUE;				
			m_bViewListGlobal = TRUE;

			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			return TRUE;
		}	
		else if((bPush == FALSE) && (m_bViewListLocal == TRUE) && (m_bViewListGlobal == TRUE))
		{
			//���� ��ư�� �������� ���¿���	��ư���� �������ٸ�.		
			m_CurrentIndex		= GetButtonInList();	
			m_bViewListLocal	= FALSE;				
			m_bViewListGlobal	= FALSE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------
//	����	:	Ư�� ������ ��ư�� ���¸� �ٲ۴�.
//	bAction	:	Ư�� ������ ��ư�� Ȱ�� ����.
//	Nation	:	Ư�� ����.
//------------------------------------------------------------------------------------------------
VOID	_SelectNationButton::SetButtonAction(BOOL bAction, SHORT Nation)
{
	SHORT	i;

	for(i = 0; i < m_ButtonNum; i++)
	{
		if(m_NationArray[i] == Nation)
			_LinkedPressButton::SetButtonAction(bAction, i);
	}
}	

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _SelectNationButton::SetNation(SHORT Nation)
/// ���� : ������ �����ϴ� �Լ��̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
void _SelectNationButton::SetNation(SHORT Nation)
{
	for(int i=0; i < m_ButtonNum; i++)
	{
		if(m_NationArray[i] == Nation)
		{
			SetButton(i);	
			break;
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// SHORT _SelectNationButton::GetNation()
/// ���� : ������ �����ϴ� �Լ��̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _SelectNationButton::GetNation()
{
	return m_NationArray[GetButton()];
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _SelectNationButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
/// ���� : ������ �������� ����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectNationButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
{	
	INT		i;
	SHORT	x, y;	

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();	
	 
	//������ ��ư�� ���õǾ� ���� �ʴٸ� ���� ������ �͸� ��´�.
	//������ ��ư�� ������ �����̸� ��ư�� �� �� �ְ� ����Ʈ�� �����ش�.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 0, 1, 2, 
													  BUTTON_PUT_NOMOVE);			
	}
	else
	{	
		// ������ ������ ȭ�鿡 ����ش�.
		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 1);

		// ���ο� ������ �����ϵ��� ������ ȭ�鿡 �����ش�.
		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], lpXspr, 
									0, 1, 2,
									BUTTON_PUT_NOMOVE);		
	}	
}

//------------------------------------------------------------------------------------------------
//	����			:	������ �������� ����Ѵ�.
//	hdc				:	����� DC.
//	PlayerNumber	:	����� �÷��̾��� ��ȣ.
//------------------------------------------------------------------------------------------------
VOID _SelectNationButton::Put(HDC hdc, SHORT PlayerNumber)
{	
	INT		i;
	SHORT	x, y;	

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();	
	 
	//������ ��ư�� ���õǾ� ���� �ʴٸ� ���� ������ �͸� ��´�.
	//������ ��ư�� ������ �����̸� ��ư�� �� �� �ְ� ����Ʈ�� �����ش�.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);		
	}
	else
	{	
		// ������ ������ ȭ�鿡 ����ش�.
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);				

		// ���ο� ������ �����ϵ��� ������ ȭ�鿡 �����ش�.
		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(hdc, m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], BUTTON_PUT_NOMOVE);				
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID _SelectAllyButton::Create(SHORT YAxis, SHORT x, SHORT y, SHORT Width, SHORT Height)
/// ���� : ������ �����ϱ� ���� ��ư�̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectAllyButton::Create(SHORT YAxis, SHORT x, SHORT y, SHORT Width, SHORT Height)
{
	strLinkButtonInfo	stLinkButtonInfo[MAX_MULTI_PLAYER];		

	stLinkButtonInfo[0].x	=	89;
	stLinkButtonInfo[0].y	=	YAxis;	
	strcpy(stLinkButtonInfo[0].szButtonName, Text.Number_1.Get());

	stLinkButtonInfo[1].x	=	136;
	stLinkButtonInfo[1].y	=	YAxis;
	strcpy(stLinkButtonInfo[1].szButtonName, Text.Number_2.Get());

	stLinkButtonInfo[2].x	=	183;
	stLinkButtonInfo[2].y	=	YAxis;
	strcpy(stLinkButtonInfo[2].szButtonName, Text.Number_3.Get());

	stLinkButtonInfo[3].x	=	230;
	stLinkButtonInfo[3].y	=	YAxis;
	strcpy(stLinkButtonInfo[3].szButtonName, Text.Number_4.Get());

	stLinkButtonInfo[4].x	=	277;
	stLinkButtonInfo[4].y	=	YAxis;
	strcpy(stLinkButtonInfo[4].szButtonName, Text.Number_5.Get());

	stLinkButtonInfo[5].x	=	324;
	stLinkButtonInfo[5].y	=	YAxis;
	strcpy(stLinkButtonInfo[5].szButtonName, Text.Number_6.Get());

	stLinkButtonInfo[6].x	=	371;
	stLinkButtonInfo[6].y	=	YAxis;
	strcpy(stLinkButtonInfo[6].szButtonName, Text.Number_7.Get());

	stLinkButtonInfo[7].x	=	418;
	stLinkButtonInfo[7].y	=	YAxis;
	strcpy(stLinkButtonInfo[7].szButtonName, Text.Number_8.Get());

	stLinkButtonInfo[0].x	=	18;
	stLinkButtonInfo[0].y	=	YAxis;	
	strcpy(stLinkButtonInfo[0].szButtonName, Text.Number_1.Get());

	stLinkButtonInfo[1].x	=	62;
	stLinkButtonInfo[1].y	=	YAxis;
	strcpy(stLinkButtonInfo[1].szButtonName, Text.Number_2.Get());

	stLinkButtonInfo[2].x	=	106;
	stLinkButtonInfo[2].y	=	YAxis;
	strcpy(stLinkButtonInfo[2].szButtonName, Text.Number_3.Get());

	stLinkButtonInfo[3].x	=	150;
	stLinkButtonInfo[3].y	=	YAxis;
	strcpy(stLinkButtonInfo[3].szButtonName, Text.Number_4.Get());

	stLinkButtonInfo[4].x	=	194;
	stLinkButtonInfo[4].y	=	YAxis;
	strcpy(stLinkButtonInfo[4].szButtonName, Text.Number_5.Get());

	stLinkButtonInfo[5].x	=	238;
	stLinkButtonInfo[5].y	=	YAxis;
	strcpy(stLinkButtonInfo[5].szButtonName, Text.Number_6.Get());

	stLinkButtonInfo[6].x	=	282;
	stLinkButtonInfo[6].y	=	YAxis;
	strcpy(stLinkButtonInfo[6].szButtonName, Text.Number_7.Get());

	stLinkButtonInfo[7].x	=	326;
	stLinkButtonInfo[7].y	=	YAxis;
	strcpy(stLinkButtonInfo[7].szButtonName, Text.Number_8.Get());

	_LinkedPressButton::Create(MAX_MULTI_PLAYER, x, y, Width, Height, stLinkButtonInfo, BUTTON_PUT_LEFT);			
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID _SelectAllyButton::Free()
/// ���� : ������ �����ϱ� ���� ��ư�̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectAllyButton::Free()
{	
	_LinkedPressButton::Free();			
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL  _SelectAllyButton::Process(BOOL bPush)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _SelectAllyButton::Process(BOOL bPush)
{
	//���콺 ��ġ�� ��ư ���� �ȿ� �ְ� ��ư�� �������� �����̶��
	//������ ��ư�� ������ ���̴�.
	if(m_lpButton[m_CurrentIndex].IsAction() == TRUE)
	{
		if((m_lpButton[m_CurrentIndex].IsInMouse() == TRUE) && 
			(bPush == TRUE) && (m_bViewListLocal == FALSE) && (m_bViewListGlobal  == FALSE))
		{
			m_bViewListLocal = TRUE;				
			m_bViewListGlobal = TRUE;

			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			return TRUE;
		}	
		else if((bPush == FALSE) && (m_bViewListLocal == TRUE) && (m_bViewListGlobal == TRUE))
		{
			//���� ��ư�� �������� ���¿���	��ư���� �������ٸ�.		
			m_CurrentIndex		= GetButtonInList();	
			m_bViewListLocal	= FALSE;				
			m_bViewListGlobal	= FALSE;
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _SelectAllyButton::SetAlly(SHORT Ally)
/// ���� : ������ �����ϴ� �Լ��̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
void _SelectAllyButton::SetAlly(SHORT Ally)
{	
	for(int i=0; i < m_ButtonNum; i++)
	{
		if(m_AllyArray[i] == Ally)
		{
			SetButton(i);	
			break;
		}
	}		
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// SHORT _SelectAllyButton::GetAlly()
/// ���� : ������ �����ϴ� �Լ��̴�.
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _SelectAllyButton::GetAlly()
{
	SHORT	Button = GetButton();

	if(Button >= 8)	
		clGrp.Error("FKJE8567", "YYLISTBUTTON0000011 [%d]", Button);	

	return m_AllyArray[Button];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _SelectAllyButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
/// ���� : ���� ������ ���� ȭ�鿡 ����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectAllyButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
{	
	INT		i;
	SHORT	ButtonHeight;
	SHORT	x, y;

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();
	 
	//������ ��ư�� ���õǾ� ���� �ʴٸ� ���� ������ �͸� ��´�.
	//������ ��ư�� ������ �����̸� ��ư�� �� �� �ְ� ����Ʈ�� �����ش�.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 0, 1, 2,
													  BUTTON_PUT_NOMOVE);		
	}
	else
	{		
		//��ư�� ���̸� ���Ѵ�.
		ButtonHeight = m_lpButton[m_CurrentIndex].GetHeight();

		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 1);		

		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], lpXspr, 
								0, 1, 2,
								BUTTON_PUT_NOMOVE);		
	}
}

//------------------------------------------------------------------------------------------------
//	����			:	������ ������ ����Ѵ�.
//	hdc				:	����� DC.
//	PlayerNumber	:	����� �÷��̾��� ��ȣ.
//------------------------------------------------------------------------------------------------
VOID	_SelectAllyButton::Put(HDC hdc, SHORT PlayerNumber)
{	
	INT		i;
	SHORT	x, y;	

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();	
	 
	//������ ��ư�� ���õǾ� ���� �ʴٸ� ���� ������ �͸� ��´�.
	//������ ��ư�� ������ �����̸� ��ư�� �� �� �ְ� ����Ʈ�� �����ش�.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);		
	}
	else
	{	
		// ������ ������ ȭ�鿡 ����ش�.
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);				

		// ���ο� ������ �����ϵ��� ������ ȭ�鿡 �����ش�.
		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(hdc, m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], BUTTON_PUT_NOMOVE);				
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::Create()
/// ���� : �� ��ư�� ��� �ʱ�ȭ �Ѵ�.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Create()
{
	BYTE		InitAlly[MAX_MULTI_PLAYER] = {0, 1, 2, 3, 4, 5, 6, 7};

	SHORT		PlayerYAxis[MAX_MULTI_PLAYER] =
	{		
		38,
		65,
		92,
		119,
		146,
		173,
		200,
		227
	};

	SHORT		ButtonPosData[MAX_MULTI_PLAYER][2]=
	{
		{253, 11}, 
		{253, 38}, 
		{253, 65}, 
		{253, 92}, 
		{253, 119}, 
		{253, 146}, 
		{253, 173}, 
		{253, 200}		
	};

	SHORT		i;		
	CHAR		szFileNameBuffer[1024];

	
	if(m_Xspr.Image) clGrp.Error("FKJE8567", "YYYLISTBUTTON0000002");
	GetFileNamePath(BUTTON_NUMBUTTONS_SPR_FILENAME, SavePath, szFileNameBuffer);
	if(clGrp.LoadXspr(szFileNameBuffer, m_Xspr)==FALSE) clGrp.Error("FKJE8567", "YYLISTBUTTON0000003 [%s]", szFileNameBuffer);		

	for(i=0; i < MAX_MULTI_PLAYER; i++)	
	{			
		BAllyButton[i].Create(PlayerYAxis[i], ButtonPosData[i][0], ButtonPosData[i][1], m_Xspr.Header.Xsize, m_Xspr.Header.Ysize);		
	}
	
	for(i=0; i < MAX_MULTI_PLAYER; i++)	
		BAllyButton[i].SetAlly(InitAlly[i]);	

	m_ViewListIndex	= -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::Free()
/// ���� : �� ��ư�� ��� �ʱ�ȭ �Ѵ�.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Free()
{	
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)	
		BAllyButton[i].Free();	

	if(m_Xspr.Image) clGrp.FreeXspr(m_Xspr);		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _AllyButtonManagement::ViewListProcess()
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _AllyButtonManagement::ViewListProcess()
{
	SHORT	i;
	BOOL	bRet;

	bRet = FALSE;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
	{
		if(BAllyButton[i].IsViewList(BUTTON_VIEW_LIST_LOCAL) == FALSE)
			BAllyButton[i].SetProcess(FALSE);
		else	
		{
			m_ViewListIndex	=	i;
			bRet = TRUE;					
		}
	}

	if(bRet == FALSE)
	{
		m_ViewListIndex	= -1;
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::SetAction(BOOL bAction)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::SetAction(BOOL bAction)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		SetAction(i, bAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::SetAction(SHORT Index, SHORT bAction)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::SetAction(SHORT Index, SHORT bAction)
{
	BAllyButton[Index].SetAction(bAction);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::Set(SHORT NewAlly)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Set(SHORT* lpAllys)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		BAllyButton[i].SetAlly(lpAllys[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::Get(SHORT* lpAllys)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Get(SHORT* lpAllys)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		lpAllys[i] = BAllyButton[i].GetAlly();	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::Put()
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Put(LPDIRECTDRAWSURFACE7 pSurface)
{	
	SHORT	i;
	HFONT	OldFont;
	HDC		hdc;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		for(i = 0; i < MAX_MULTI_PLAYER; i++)
			BAllyButton[i].Put(i, &m_Xspr);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		OldFont		=	(HFONT) SelectObject(hdc, pGame->pBattle->BriefingFont);			

		for(i = 0; i < MAX_MULTI_PLAYER; i++)
			BAllyButton[i].Put(hdc, i);

		SelectObject(hdc, OldFont);
		pSurface->ReleaseDC(hdc);
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::PutViewList()
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::PutViewList(LPDIRECTDRAWSURFACE7 pSurface)
{
	HFONT	OldFont;
	HDC		hdc;

	if(m_ViewListIndex != -1)
	{
		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			BAllyButton[m_ViewListIndex].Put(m_ViewListIndex, &m_Xspr);
			clGrp.UnlockSurface(pSurface);
		}

		if(pSurface->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);		
			OldFont		=	(HFONT) SelectObject(hdc, pGame->pBattle->BriefingFont);					
			BAllyButton[m_ViewListIndex].Put(hdc, m_ViewListIndex);		

			SelectObject(hdc, OldFont);
			pSurface->ReleaseDC(hdc);
		}	
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _AllyButtonManagement::Process(BOOL bPush, SHORT* GetAllys)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Process(BOOL bPush, SHORT* GetAllys)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		BAllyButton[i].Process(bPush);

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		GetAllys[i] = BAllyButton[i].GetAlly();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::Create()
/// ���� : �� ��ư�� ��� �ʱ�ȭ �Ѵ�.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Create()
{

	BYTE		InitNation[MAX_MULTI_PLAYER];

	SHORT		PlayerYAxis[MAX_MULTI_PLAYER] =
	{		
		38,
		65,
		92,
		119,
		146,
		173,
		200,
		227
	};

	SHORT		ButtonPosData[MAX_MULTI_PLAYER][2]=
	{
		{208, 11}, 
		{208, 38}, 
		{208, 65}, 
		{208, 92}, 
		{208, 119}, 
		{208, 146}, 
		{208, 173}, 
		{208, 200}
	};

	SHORT		i;		
	CHAR		szFileNameBuffer[1024];			
	

	if(m_XsprButton.Image) clGrp.Error("FKJE8567", "YYLISTBUTTON0000006");
	GetFileNamePath(BUTTON_NATIONBUTTONS_SPR_FILENAME, SavePath, szFileNameBuffer);
	if(clGrp.LoadXspr(szFileNameBuffer, m_XsprButton)==FALSE) clGrp.Error("FKJE8567", "YYLISTBUTTON0000007 [%s]", szFileNameBuffer);	

	for(i=0; i < MAX_MULTI_PLAYER; i++)	
	{			
		BNationButton[i].Create(PlayerYAxis[i], ButtonPosData[i][0], ButtonPosData[i][1], m_XsprButton.Header.Xsize, m_XsprButton.Header.Ysize);		
	}
	
	for(i=0; i < MAX_MULTI_PLAYER; i++)	
		BNationButton[i].SetNation(InitNation[i]);	

	m_ViewListIndex	= -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::Free()
/// ���� : �� ��ư�� ��� �ʱ�ȭ �Ѵ�.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Free()
{	
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)	
		BNationButton[i].Free();	

	if(m_XsprButton.Image) clGrp.FreeXspr(m_XsprButton);		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _NationButtonManagement::ViewListProcess()
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _NationButtonManagement::ViewListProcess()
{
	SHORT	i;
	BOOL	bRet;

	bRet = FALSE;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
	{
		if(BNationButton[i].IsViewList(BUTTON_VIEW_LIST_LOCAL) == FALSE)
			BNationButton[i].SetProcess(FALSE);
		else	
		{
			m_ViewListIndex	=	i;
			bRet = TRUE;					
		}
	}

	if(bRet == FALSE)
	{
		m_ViewListIndex	= -1;
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::SetAction(BOOL bAction)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::SetAction(BOOL bAction)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		SetAction(i, bAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::SetAction(SHORT Index, SHORT bAction)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::SetAction(SHORT Index, SHORT bAction)
{
	BNationButton[Index].SetAction(bAction);
}

//------------------------------------------------------------------------------------------------
//	����	:	Ư�� ��ư�� Ȱ�� ���θ� �����Ѵ�.
//	Index	:	Ư�� �� ��ư�� Ȱ�� ���θ� �����ϹǷ� ��ư�� �ε���.
//------------------------------------------------------------------------------------------------
VOID	_NationButtonManagement::SetButtonAction(SHORT Index, BOOL bAction, SHORT Nation)
{
	BNationButton[Index].SetButtonAction(bAction, Nation);
}

//------------------------------------------------------------------------------------------------
//	����	:	��ư���� Ȱ�� ���θ� �����Ѵ�.
//				Ư�� �� ������ ��ư���� ���¸� �ٲ� �� ����Ѵ�.
//	bAction	:	�� �÷��̾���� Ư�� �� ������ ���¸� ��� �ؾ� �ϴ��� ��Ÿ���� ����.
//	Nation	:	� ������ ��ư�� ���¸� �ٲ� ���ΰ�.
//------------------------------------------------------------------------------------------------
VOID	_NationButtonManagement::SetButtonAction(BOOL bAction[MAX_MULTI_PLAYER], SHORT Nation)
{
	UI32	i;

	for(i = 0; i < MAX_MULTI_PLAYER; i++)
		SetButtonAction(i, bAction[i], Nation);		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::Set(SHORT NewNation)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Set(SHORT* lpNations)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		BNationButton[i].SetNation(lpNations[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::Get(SHORT* lpNations)
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Get(SHORT* lpNations)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		lpNations[i] = BNationButton[i].GetNation();	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::Put()
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Put(LPDIRECTDRAWSURFACE7 pSurface)
{
	SHORT	i;
	HFONT	OldFont;
	HDC		hdc;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		for(i = 0; i < MAX_MULTI_PLAYER; i++)
			BNationButton[i].Put(i, &m_XsprButton);		

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		OldFont		=	(HFONT) SelectObject(hdc, pGame->pBattle->BriefingFont);			

		for(i = 0; i < MAX_MULTI_PLAYER; i++)
			BNationButton[i].Put(hdc, i);		

		SelectObject(hdc, OldFont);
		pSurface->ReleaseDC(hdc);
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _NationButtonManagement::PutViewList()
/// ���� :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::PutViewList(LPDIRECTDRAWSURFACE7 pSurface)
{	
	HFONT	OldFont;
	HDC		hdc;

	if(m_ViewListIndex != -1)
	{
		if(clGrp.LockSurface(pSurface) == TRUE)
		{
			BNationButton[m_ViewListIndex].Put(m_ViewListIndex, &m_XsprButton);
			clGrp.UnlockSurface(pSurface);
		}

		if(pSurface->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);		

	
			OldFont		=	(HFONT) SelectObject(hdc, pGame->pBattle->BriefingFont);			
			BNationButton[m_ViewListIndex].Put(hdc, m_ViewListIndex);		

			SelectObject(hdc, OldFont);
			pSurface->ReleaseDC(hdc);
		}	
	}
}


//------------------------------------------------------------------------------------------------
//	����				:	���� ������ ��ư���� ��� ó�����ش�.
//	bPush				:	���� ��ư�� ���� ����.
//	GetNations			:	�������� ���� ������ �� �迭.
//------------------------------------------------------------------------------------------------
VOID	_NationButtonManagement::Process(BOOL bPush, SHORT* GetNations)
{
	SHORT	i;	
	
	for(i=0; i < MAX_MULTI_PLAYER; i++)					
		BNationButton[i].Process(bPush);	
	
	for(i=0; i < MAX_MULTI_PLAYER; i++)
		GetNations[i]	=	BNationButton[i].GetNation();		
}

