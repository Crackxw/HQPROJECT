#include <GSL.h>
#include <main.h>

#include <config.h>
#include "YControl.h"

#include "YListControl.h"

extern	_Config 			Config;								// ȯ�� ���� 


////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _YListControl::_YListControl()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
_YListControl::_YListControl() : _YControl()
{		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _YListControl::~_YListControl()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
_YListControl::~_YListControl()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _YListControl::Create(SHORT x, SHORT y, SHORT SellWidth, SHORT SellHeight)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::Create(SHORT x, SHORT y, SHORT SellWidth, SHORT SellHeight)
{
	SHORT	i;

	m_x			=	x;
	m_y			=	y;	
	m_SellWidth	=	SellWidth;
	m_SellHeight=	SellHeight;

	//�ƹ��͵� ������ �Ǿ� ���� �ʴ�.
	m_CurrentTotalSellNum	=	0;
	m_nSelectedSell			=	-1;	

	for(i=0; i < Y_LISTCONTROL_MAX_SELLNUM; i++)	
	{
		m_ButtonPoint[i]	=	-1;
		m_IsUseSell[i]		=	FALSE;
	}


}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _YListControl::Free()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::Free()
{
	m_CurrentTotalSellNum	=	0;
	m_nSelectedSell			=	-1;		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListControl::GetEmptySell()
/// ���� : 
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
/// �Լ� : VOID _YListControl::Action(BOOL bPush, BOOL *lpIsDoubleClicked)
/// ���� : 
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
/// �Լ� : VOID _YListControl::Draw(LPDIRECTDRAWSURFACE lpSurface)
/// ���� : 
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
/// �Լ� : SHORT _YListControl::AddSell(SHORT x, SHORT y, CHAR* Name, DWORD Flag)
/// ���� : 
/// ���ϰ� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::AddSell(SHORT x, SHORT y, CHAR* Name, DWORD Flag)
{
	SHORT	nEmptySellNum;
	BOOL	IsSelect;

	IsSelect		=	(Flag & Y_LISTCONTROL_SELECT) ? TRUE : FALSE; 		

	//��� ���� ��� �ִ��� �޴´�.
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
/// �Լ� : SHORT _YListControl::RenameSell(CHAR* szName)
/// ���� : 
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
/// �Լ� : SHORT  _YListControl::DelSell()
/// ���� : 
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
/// �Լ� : SHORT _YListControl::GetSell(CHAR* szName)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::GetSell(CHAR* szName)
{
	if(szName)	
		strcpy(szName, m_BSell[m_ButtonPoint[m_nSelectedSell]].GetMenuName());		
	
	return strlen(szName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListControl::GetSelectedSell()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListControl::GetSelectedSell()
{
	return m_nSelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _YListControl::SetSelectedSell(SHORT NewSell)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::SetSelectedSell(SHORT NewSell)
{
	_YListControl::SetSelect(NewSell);	
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _YListControl::SetSelect(SHORT Index)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListControl::SetSelect(SHORT Index)
{
	if(m_nSelectedSell != -1)	m_BSell[m_ButtonPoint[m_nSelectedSell]].SetSelected(FALSE);

	m_BSell[m_ButtonPoint[Index]].SetSelected(TRUE);
	m_nSelectedSell	=	Index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT	_YListControl::GetCurrentTotalSellNum()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListControl::GetCurrentTotalSellNum()
{
	return m_CurrentTotalSellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListControl::IsSelectedSomething()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListControl::IsSelectedSomething()
{
	return (m_nSelectedSell != -1) ? TRUE : FALSE;
}


