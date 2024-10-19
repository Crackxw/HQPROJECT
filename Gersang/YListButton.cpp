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
extern	_Text 				Text;								// 텍스트 
extern	char 				SavePath[];
extern	_Config 			Config;								// 환경 변수 

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
//	설명				:	링크 버튼을 생성한다.
//	ButtonNum			:	버튼의 개수.
//	x, y				:	좌표.
//	Width, Height		:	각 버튼의 크기.
//	strLinkButtonInfo	:	각 링크 버튼의 대한 정보. (각 버튼들의 위치와 이름을 필요로 한다.)
//	flag				:	옵션.
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
//	설명			: 버튼의 갯수없이 버튼을 생성한다.(타일의 종류에 대한 버튼에서 사용)
//	x, y			: 좌표.
//	Width, Height		: 각 버튼의 크기.
//	strLinkButtonInfo	: 각 링크 버튼의 대한 정보. (각 버튼들의 위치와 이름을 필요로 한다.)
//	flag			: 옵션.
//------------------------------------------------------------------------------------------------
BOOL	_LinkedPressButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, strLinkButtonInfoEx *pLinkButtonInfo, INT flag)
{
	int		i;
	
//	if(m_lpButton)	clGrp.Error("FKJE8567", "YYLISTBUTTON0000012");
		
	// 개수를 알아온다.
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
/// 함수 :  VOID _LinkedPressButton::Free()
/// 설명 : 
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
	//마우스 위치가 버튼 영역 안에 있고 버튼이 눌리어진 상태이라면
	//현재의 버튼을 선택한 것이다.
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
			//전에 버튼이 눌리어진 상태에서	버튼에서 떼어졌다면.		
			m_CurrentIndex = GetButtonInList();	
			m_bViewListLocal = FALSE;				
			m_bViewListGlobal = FALSE;
		}
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _LinkedPressButton::GetButtonInList()
/// 목적 : 
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
/// 목적 : 유효한 인덱스인지 검사한다.
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
/// 목적 : 국가를 설저하는 함수이다.
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _LinkedPressButton::GetButton()
{
	return m_CurrentIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _LinkedPressButton::SetButton(SHORT Index)
/// 목적 : 국가를 설저하는 함수이다.
////////////////////////////////////////////////////////////////////////////////////////////////
void	_LinkedPressButton::SetButton(SHORT Index)
{
	m_CurrentIndex  = Index;
}

//------------------------------------------------------------------------------------------------
//	설명		:	버튼의 활동 여부를 변경한다.
//	bAction		:	활동 여부.
//	uiButtonID	:	변경할 버튼의 ID.
//------------------------------------------------------------------------------------------------
VOID	_LinkedPressButton::SetButtonAction(BOOL bAction, UI16 uiButtonID)
{
	m_lpButton[uiButtonID].SetAction(bAction);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void	_LinkedPressButton::SetAction(BOOL bAction)
/// 목적 : 각 버튼이 활동할 것인지의 여부를 나타낸다.
////////////////////////////////////////////////////////////////////////////////////////////////
void	_LinkedPressButton::SetAction(BOOL bAction)
{
	int		i;

	// 모든 버튼을 비활성화 시킨다.
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
/// 목적 : 각 버튼이 활동할 것인지의 여부를 나타낸다.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_LinkedPressButton::SetProcess(BOOL bProcess)
{
	INT		i;

	for(i=0; i < m_ButtonNum; i++)	m_lpButton[i].SetProcess(bProcess);		
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL	_LinkedPressButton::IsViewList(SHORT KindOf)
/// 목적 : 리스트 박스가 보여지고 있는지를 리턴한다.
/// 인자 : Kindof -   BUTTON_VIEW_LIST_GLOBAL 버튼 중의 하나라도 리스트를 보고 있는지.
///					  BUTTON_VIEW_LIST_LOCAL  자기 버튼이 리스트를 보고 있는지.
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
/// 목적 : 동맹을 설정하기 위한 버튼이다.
////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------
//	설명			:	국가를 설정하는 버튼을 만든다
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
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectNationButton::Free()
{	
	_LinkedPressButton::Free();			
}

//------------------------------------------------------------------------------------------------
//	설명				:	버튼들을 처리한다.
//	bPush				:	현재 버튼의 눌리 상태.
//------------------------------------------------------------------------------------------------
BOOL	_SelectNationButton::Process(BOOL bPush)
{	
	if(m_lpButton[m_CurrentIndex].IsAction() == TRUE)
	{
		// 마우스 위치가 버튼 영역 안에 있고 버튼이 눌리어진 상태이라면
		// 현재의 버튼을 선택한 것이다.
		// 또한 m_bViewListLocal 값과 m_bViewListGlobal를 검사하여
		// 이전 부터 눌르고 있었던 건지 아닌지를 알아내어
		// 누르고 있지 않았을때에만 버튼을 선택한 걸로 한다.
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
			//전에 버튼이 눌리어진 상태에서	버튼에서 떼어졌다면.		
			m_CurrentIndex		= GetButtonInList();	
			m_bViewListLocal	= FALSE;				
			m_bViewListGlobal	= FALSE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------
//	설명	:	특정 국가의 버튼의 상태를 바꾼다.
//	bAction	:	특정 국가의 버튼의 활동 여부.
//	Nation	:	특정 국가.
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
/// 목적 : 국가를 설저하는 함수이다.
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
/// 목적 : 국가를 리턴하는 함수이다.
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _SelectNationButton::GetNation()
{
	return m_NationArray[GetButton()];
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _SelectNationButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
/// 목적 : 현재의 국가명을 출력한다.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectNationButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
{	
	INT		i;
	SHORT	x, y;	

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();	
	 
	//현재의 버튼이 선택되어 있지 않다면 전에 선택한 것만 찍는다.
	//현재의 버튼을 선택한 상태이면 버튼을 고를 수 있게 리스트를 보여준다.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 0, 1, 2, 
													  BUTTON_PUT_NOMOVE);			
	}
	else
	{	
		// 선택한 국가를 화면에 찍어준다.
		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 1);

		// 새로운 국가를 선택하도록 국가를 화면에 보여준다.
		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], lpXspr, 
									0, 1, 2,
									BUTTON_PUT_NOMOVE);		
	}	
}

//------------------------------------------------------------------------------------------------
//	설명			:	현재의 국가명을 출력한다.
//	hdc				:	출력할 DC.
//	PlayerNumber	:	출력할 플레이어의 번호.
//------------------------------------------------------------------------------------------------
VOID _SelectNationButton::Put(HDC hdc, SHORT PlayerNumber)
{	
	INT		i;
	SHORT	x, y;	

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();	
	 
	//현재의 버튼이 선택되어 있지 않다면 전에 선택한 것만 찍는다.
	//현재의 버튼을 선택한 상태이면 버튼을 고를 수 있게 리스트를 보여준다.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);		
	}
	else
	{	
		// 선택한 국가를 화면에 찍어준다.
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);				

		// 새로운 국가를 선택하도록 국가를 화면에 보여준다.
		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(hdc, m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], BUTTON_PUT_NOMOVE);				
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID _SelectAllyButton::Create(SHORT YAxis, SHORT x, SHORT y, SHORT Width, SHORT Height)
/// 목적 : 동맹을 설정하기 위한 버튼이다.
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
/// 목적 : 동맹을 설정하기 위한 버튼이다.
////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectAllyButton::Free()
{	
	_LinkedPressButton::Free();			
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL  _SelectAllyButton::Process(BOOL bPush)
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _SelectAllyButton::Process(BOOL bPush)
{
	//마우스 위치가 버튼 영역 안에 있고 버튼이 눌리어진 상태이라면
	//현재의 버튼을 선택한 것이다.
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
			//전에 버튼이 눌리어진 상태에서	버튼에서 떼어졌다면.		
			m_CurrentIndex		= GetButtonInList();	
			m_bViewListLocal	= FALSE;				
			m_bViewListGlobal	= FALSE;
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// void _SelectAllyButton::SetAlly(SHORT Ally)
/// 목적 : 국가를 설저하는 함수이다.
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
/// 목적 : 국가를 리턴하는 함수이다.
////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _SelectAllyButton::GetAlly()
{
	SHORT	Button = GetButton();

	if(Button >= 8)	
		clGrp.Error("FKJE8567", "YYLISTBUTTON0000011 [%d]", Button);	

	return m_AllyArray[Button];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _SelectAllyButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
/// 목적 : 동맹 상태의 수를 화면에 출력한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _SelectAllyButton::Put(SHORT PlayerNumber, XSPR* lpXspr)
{	
	INT		i;
	SHORT	ButtonHeight;
	SHORT	x, y;

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();
	 
	//현재의 버튼이 선택되어 있지 않다면 전에 선택한 것만 찍는다.
	//현재의 번튼을 선택한 상태이면 버튼을 고를 수 있게 리스트를 보여준다.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 0, 1, 2,
													  BUTTON_PUT_NOMOVE);		
	}
	else
	{		
		//버튼의 높이를 구한다.
		ButtonHeight = m_lpButton[m_CurrentIndex].GetHeight();

		m_lpButton[m_CurrentIndex].Put(x, y, lpXspr, 1);		

		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], lpXspr, 
								0, 1, 2,
								BUTTON_PUT_NOMOVE);		
	}
}

//------------------------------------------------------------------------------------------------
//	설명			:	현재의 동맹을 출력한다.
//	hdc				:	출력할 DC.
//	PlayerNumber	:	출력할 플레이어의 번호.
//------------------------------------------------------------------------------------------------
VOID	_SelectAllyButton::Put(HDC hdc, SHORT PlayerNumber)
{	
	INT		i;
	SHORT	x, y;	

	x = m_lpButton[m_CurrentIndex].GetX();
	y = m_lpButton[m_CurrentIndex].GetY();	
	 
	//현재의 버튼이 선택되어 있지 않다면 전에 선택한 것만 찍는다.
	//현재의 버튼을 선택한 상태이면 버튼을 고를 수 있게 리스트를 보여준다.
	if(m_bViewListLocal == FALSE)
	{
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);		
	}
	else
	{	
		// 선택한 국가를 화면에 찍어준다.
		m_lpButton[m_CurrentIndex].Put(hdc, x, y, BUTTON_PUT_NOMOVE);				

		// 새로운 국가를 선택하도록 국가를 화면에 보여준다.
		for(i=0; i < m_ButtonNum; i++)		
			m_lpButton[i].Put(hdc, m_ListButtonPosData[i * 2], m_ListButtonPosData[i * 2 + 1], BUTTON_PUT_NOMOVE);				
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _AllyButtonManagement::Create()
/// 설명 : 각 버튼을 모두 초기화 한다.
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
/// 함수 : VOID _AllyButtonManagement::Free()
/// 설명 : 각 버튼을 모두 초기화 한다.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Free()
{	
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)	
		BAllyButton[i].Free();	

	if(m_Xspr.Image) clGrp.FreeXspr(m_Xspr);		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _AllyButtonManagement::ViewListProcess()
/// 설명 :
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
/// 함수 : VOID _AllyButtonManagement::SetAction(BOOL bAction)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::SetAction(BOOL bAction)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		SetAction(i, bAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _AllyButtonManagement::SetAction(SHORT Index, SHORT bAction)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::SetAction(SHORT Index, SHORT bAction)
{
	BAllyButton[Index].SetAction(bAction);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _AllyButtonManagement::Set(SHORT NewAlly)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Set(SHORT* lpAllys)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		BAllyButton[i].SetAlly(lpAllys[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _AllyButtonManagement::Get(SHORT* lpAllys)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _AllyButtonManagement::Get(SHORT* lpAllys)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		lpAllys[i] = BAllyButton[i].GetAlly();	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _AllyButtonManagement::Put()
/// 설명 :
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
/// 함수 : VOID _AllyButtonManagement::PutViewList()
/// 설명 :
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
/// 함수 : VOID _AllyButtonManagement::Process(BOOL bPush, SHORT* GetAllys)
/// 설명 :
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
/// 함수 : VOID _NationButtonManagement::Create()
/// 설명 : 각 버튼을 모두 초기화 한다.
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
/// 함수 : VOID _NationButtonManagement::Free()
/// 설명 : 각 버튼을 모두 초기화 한다.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Free()
{	
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)	
		BNationButton[i].Free();	

	if(m_XsprButton.Image) clGrp.FreeXspr(m_XsprButton);		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _NationButtonManagement::ViewListProcess()
/// 설명 :
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
/// 함수 : VOID _NationButtonManagement::SetAction(BOOL bAction)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::SetAction(BOOL bAction)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		SetAction(i, bAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _NationButtonManagement::SetAction(SHORT Index, SHORT bAction)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::SetAction(SHORT Index, SHORT bAction)
{
	BNationButton[Index].SetAction(bAction);
}

//------------------------------------------------------------------------------------------------
//	설명	:	특정 버튼의 활동 여부를 변경한다.
//	Index	:	특정 한 버튼의 활동 여부를 변경하므로 버튼의 인덱스.
//------------------------------------------------------------------------------------------------
VOID	_NationButtonManagement::SetButtonAction(SHORT Index, BOOL bAction, SHORT Nation)
{
	BNationButton[Index].SetButtonAction(bAction, Nation);
}

//------------------------------------------------------------------------------------------------
//	설명	:	버튼들의 활동 여부를 변경한다.
//				특정 한 국가의 버튼만의 상태를 바꿀 때 사용한다.
//	bAction	:	각 플레이어들이 특정 한 국가의 상태를 어떻게 해야 하는지 나타내는 변수.
//	Nation	:	어떤 국가의 버튼의 상태를 바꿀 것인가.
//------------------------------------------------------------------------------------------------
VOID	_NationButtonManagement::SetButtonAction(BOOL bAction[MAX_MULTI_PLAYER], SHORT Nation)
{
	UI32	i;

	for(i = 0; i < MAX_MULTI_PLAYER; i++)
		SetButtonAction(i, bAction[i], Nation);		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _NationButtonManagement::Set(SHORT NewNation)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Set(SHORT* lpNations)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		BNationButton[i].SetNation(lpNations[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _NationButtonManagement::Get(SHORT* lpNations)
/// 설명 :
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _NationButtonManagement::Get(SHORT* lpNations)
{
	SHORT	i;

	for(i=0; i < MAX_MULTI_PLAYER; i++)
		lpNations[i] = BNationButton[i].GetNation();	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _NationButtonManagement::Put()
/// 설명 :
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
/// 함수 : VOID _NationButtonManagement::PutViewList()
/// 설명 :
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
//	설명				:	국가 선택의 버튼들을 모두 처리해준다.
//	bPush				:	현재 버튼의 눌리 상태.
//	GetNations			:	국가들의 대한 정보를 얻어갈 배열.
//------------------------------------------------------------------------------------------------
VOID	_NationButtonManagement::Process(BOOL bPush, SHORT* GetNations)
{
	SHORT	i;	
	
	for(i=0; i < MAX_MULTI_PLAYER; i++)					
		BNationButton[i].Process(bPush);	
	
	for(i=0; i < MAX_MULTI_PLAYER; i++)
		GetNations[i]	=	BNationButton[i].GetNation();		
}

