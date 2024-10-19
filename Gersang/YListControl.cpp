#include <GSL.h>
#include <main.h>

#include <config.h>
#include "YControl.h"

#include "YListControl.h"

extern	_Config 			Config;								// 환경 변수 


////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YListControl::_YListControl()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
_YListControl::_YListControl() : _YControl()
{		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YListControl::~_YListControl()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
_YListControl::~_YListControl()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YListControl::Create(SHORT x, SHORT y, SHORT SellWidth, SHORT SellHeight)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::Create(SHORT x, SHORT y, SHORT SellWidth, SHORT SellHeight)
{
	SHORT	i;

	m_x			=	x;
	m_y			=	y;	
	m_SellWidth	=	SellWidth;
	m_SellHeight=	SellHeight;

	//아무것도 선택이 되어 있지 않다.
	m_CurrentTotalSellNum	=	0;
	m_nSelectedSell			=	-1;	

	for(i=0; i < Y_LISTCONTROL_MAX_SELLNUM; i++)	
	{
		m_ButtonPoint[i]	=	-1;
		m_IsUseSell[i]		=	FALSE;
	}


}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YListControl::Free()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::Free()
{
	m_CurrentTotalSellNum	=	0;
	m_nSelectedSell			=	-1;		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListControl::GetEmptySell()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::GetEmptySell()
{
	SHORT	i;

	for(i=0; i < Y_LISTCONTROL_MAX_SELLNUM; i++)
	{
		if(m_IsUseSell[i] == FALSE)
			return i;
	}

	return -1;	
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YListControl::Action(BOOL bPush, BOOL *lpIsDoubleClicked)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::Action(BOOL bPush, BOOL *lpIsDoubleClicked)
{
	SHORT	i;
	BOOL	IsDoubleClick;
	

	for(i=0; i < m_CurrentTotalSellNum; i++)
	{			
		if(m_BSell[m_ButtonPoint[i]].Process(bPush, &IsDoubleClick) == TRUE)				
		{
			SetSelect(i);												

			if(IsDoubleClick == TRUE)
				*lpIsDoubleClicked	= TRUE;
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YListControl::Draw(LPDIRECTDRAWSURFACE lpSurface)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	SHORT	i;	

	for(i=0; i < m_CurrentTotalSellNum; i++)		
	{	
		if(GetSelectedSell() == i)
			m_BSell[m_ButtonPoint[i]].Put(lpSurface, BUTTON_PUT_NOMOVE | BUTTON_PUT_FILLCOLOR | BUTTON_PUT_NOBORDER, BUTTON_PUT_LEFT);	
		else
			m_BSell[m_ButtonPoint[i]].Put(lpSurface, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER, BUTTON_PUT_LEFT);			
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListControl::AddSell(SHORT x, SHORT y, CHAR* Name, DWORD Flag)
/// 설명 : 
/// 리턴값 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::AddSell(SHORT x, SHORT y, CHAR* Name, DWORD Flag)
{
	SHORT	nEmptySellNum;
	BOOL	IsSelect;

	IsSelect		=	(Flag & Y_LISTCONTROL_SELECT) ? TRUE : FALSE; 		

	//몇번 셀이 비어 있는지 받는다.
	nEmptySellNum	=	GetEmptySell();

	if(nEmptySellNum != -1)
	{	
		m_IsUseSell[nEmptySellNum]				=	TRUE;	
		m_ButtonPoint[m_CurrentTotalSellNum]	=	nEmptySellNum;		

		m_BSell[m_ButtonPoint[m_CurrentTotalSellNum]].Create(
													m_x - 10, 
													m_y + (m_CurrentTotalSellNum *  m_SellHeight), 
													m_SellWidth, 
													m_SellHeight,
													m_x,  
													m_y + (m_CurrentTotalSellNum *  m_SellHeight), 
													Name, 
													BUTTON_PUT_LEFT);
			
		SetSelect(m_CurrentTotalSellNum);			
		m_CurrentTotalSellNum++;

		return (m_CurrentTotalSellNum - 1);
	}
	else
		return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListControl::RenameSell(CHAR* szName)
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::RenameSell(CHAR* szName)
{
	if(m_nSelectedSell != -1)
	{
		if(m_ButtonPoint[m_nSelectedSell] != -1)		
			m_BSell[m_ButtonPoint[m_nSelectedSell]].SetMenuName(szName);	
		
	}
	
	return m_nSelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT  _YListControl::DelSell()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListControl::DelSell()
{
	SHORT	i;
	SHORT	DeletedSellNum;

	if(m_CurrentTotalSellNum > 0)
	{
		m_IsUseSell[m_ButtonPoint[m_nSelectedSell]]		=	FALSE;		
		DeletedSellNum									=	m_nSelectedSell;

		for(i=m_nSelectedSell; i < (m_CurrentTotalSellNum - 1); i++)
		{
			m_ButtonPoint[i]	=	m_ButtonPoint[i + 1];			

			m_BSell[m_ButtonPoint[i]].SetX(m_x);
			m_BSell[m_ButtonPoint[i]].SetY(m_y + (i * m_SellHeight));

			m_BSell[m_ButtonPoint[i]].SetTextPutX(m_x);
			m_BSell[m_ButtonPoint[i]].SetTextPutY(m_y + (i * m_SellHeight));
		}	

		m_ButtonPoint[m_CurrentTotalSellNum - 1]		=	-1;		
		m_nSelectedSell									=	-1;
		m_CurrentTotalSellNum--;	
	}
	else
	{
		DeletedSellNum	=	-1;
	}

	return DeletedSellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListControl::GetSell(CHAR* szName)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::GetSell(CHAR* szName)
{
	if(szName)	
		strcpy(szName, m_BSell[m_ButtonPoint[m_nSelectedSell]].GetMenuName());		
	
	return strlen(szName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListControl::GetSelectedSell()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::GetSelectedSell()
{
	return m_nSelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YListControl::SetSelectedSell(SHORT NewSell)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::SetSelectedSell(SHORT NewSell)
{
	_YListControl::SetSelect(NewSell);	
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YListControl::SetSelect(SHORT Index)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::SetSelect(SHORT Index)
{
	if(m_nSelectedSell != -1)	m_BSell[m_ButtonPoint[m_nSelectedSell]].SetSelected(FALSE);

	m_BSell[m_ButtonPoint[Index]].SetSelected(TRUE);
	m_nSelectedSell	=	Index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT	_YListControl::GetCurrentTotalSellNum()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListControl::GetCurrentTotalSellNum()
{
	return m_CurrentTotalSellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YListControl::IsSelectedSomething()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListControl::IsSelectedSomething()
{
	return (m_nSelectedSell != -1) ? TRUE : FALSE;
}


