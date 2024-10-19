#include <GSL.h>
#include <graphics16.h>
#include <main.h>
#include "YSelectControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _BaseSelectButton::_BaseSelectButton()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
_BaseSelectButton::_BaseSelectButton()
{
	m_lpButtons		=	NULL;
	m_nButtonNum	=	0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _BaseSelectButton::~_BaseSelectButton()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
_BaseSelectButton::~_BaseSelectButton()
{
	Free();	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _BaseSelectButton::Free()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _BaseSelectButton::Free()
{
	if(m_lpButtons)
	{
		delete [] m_lpButtons;
		m_lpButtons = NULL;
	}

	m_nButtonNum = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _BaseSelectButton::Create(SHORT ButtonNum, LPRECT lpPoint)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _BaseSelectButton::Create(SHORT ButtonNum, strBaseSelectButtonInfo *pstBaseSelectButtonInfo)
{
	SHORT	i;

	if(m_lpButtons) clGrp.Error("FKJE8567", "YYSELECTCONTROL0000001");

	m_nButtonNum = ButtonNum;	

	m_lpButtons	 = new _SelectButton[m_nButtonNum];	

	for(i=0; i < m_nButtonNum; i++)
	{
		m_lpButtons[i].Create(	pstBaseSelectButtonInfo[i].rcButton.left,	
								pstBaseSelectButtonInfo[i].rcButton.top, 
								pstBaseSelectButtonInfo[i].rcButton.right - pstBaseSelectButtonInfo[i].rcButton.left, 
								pstBaseSelectButtonInfo[i].rcButton.bottom - pstBaseSelectButtonInfo[i].rcButton.top,
								pstBaseSelectButtonInfo[i].rcButton.left,	
								pstBaseSelectButtonInfo[i].rcButton.top, 
								pstBaseSelectButtonInfo[i].szName, BUTTON_PUT_LEFT, TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _BaseSelectButton::Process(BOOL bPush)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _BaseSelectButton::Process(BOOL bPush)
{
	SHORT	i;
	BOOL	bIsDoubleClicked;

	for(i=0; i < m_nButtonNum; i++)
	{
		if(m_lpButtons[i].Process(bPush, &bIsDoubleClicked) == TRUE)			
			return i;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _BaseSelectButton::Draw(SHORT SelectXSPR* lpXspr, SHORT FontNum[][3], WORD PutTextFlag/* = BUTTON_PUT_NOBORDER | BUTTON_PUT_NOMOVE*/)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _BaseSelectButton::Draw(XSPR* lpXspr, SHORT FontNum[][3], WORD PutTextFlag/* = BUTTON_PUT_NOBORDER | BUTTON_PUT_NOMOVE*/)
{
	SHORT	i;

	for(i=0; i < m_nButtonNum; i++)	
		m_lpButtons[i].Put(lpXspr, FontNum[i][0], FontNum[i][1], FontNum[i][2], PutTextFlag);
}

//------------------------------------------------------------------------------------------------
//	����	:
//------------------------------------------------------------------------------------------------
VOID	_BaseSelectButton::Draw(HDC hdc, SI32 siSelectButton)
{
	SHORT	i;

	for(i=0; i < m_nButtonNum; i++)	
	{
		if(siSelectButton == i)
			m_lpButtons[i].SetSelected(TRUE);

		m_lpButtons[i].Put(hdc);	

		if(siSelectButton == i)
			m_lpButtons[i].SetSelected(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _BaseSelectButton::SetAction(BOOL bPush)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _BaseSelectButton::SetAction(BOOL bPush)
{
	SHORT	i;

	for(i=0; i < m_nButtonNum; i++)
		m_lpButtons[i].SetAction(bPush);
}