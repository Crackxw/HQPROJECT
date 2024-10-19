#ifndef _CHOOSENATION_H
#define _CHOOSENATION_H

#include <button.h>

#define		CHOOSENATION_KOREA_X				0
#define		CHOOSENATION_KOREA_Y				0

#define		CHOOSENATION_JAPAN_X				0
#define		CHOOSENATION_JAPAN_Y				0

#define		CHOOSENATION_CHINA_X				0
#define		CHOOSENATION_CHINA_Y				0

#define		CHOOSENATION_FONTNUM_KOREA			0
#define		CHOOSENATION_FONTNUM_JAPAN			0
#define		CHOOSENATION_FONTNUM_CHINA			0

#define		CHOOSENATION_SPR_KOREA				1
#define		CHOOSENATION_SPR_JAPAN				2
#define		CHOOSENATION_SPR_CHINA				3

#define		CHOOSENATION_SELECTCOLOR_KOREA		68
#define		CHOOSENATION_SELECTCOLOR_JAPAN		70
#define		CHOOSENATION_SELECTCOLOR_CHINA		69

#define		GET_CHOOSENATION_INDEX(X,Y)			(((Y) == BUTTONS_STATUS_NORMAL) ? (-1) : (X))

VOID	InitChooseNation();
SHORT	ChooseNation();
VOID	FreeChooseNation();	

class	_BChooseNationManagement 
{
private:
	enum	{KOREA, JAPAN, CHINA, TOTALNATION=3};
	_ImageButton	m_NationButton[3];							//3개 국가.
	XSPR			m_NationSpr[3];								//3개 국가.
	XSPR			m_ChoseNationSpr[25];						// 선택시 바뀌어지는 그림..
	BYTE*			m_lpSampleBuffer;							//샘플 버퍼.
	BOOL			m_bProcess;
	
	SHORT			m_PreviousPutNation;
public:
	_BChooseNationManagement();
	~_BChooseNationManagement();

	VOID	Create();
	VOID	Free();
	BOOL	Put();
	SHORT	Process(BOOL bPush);	
	VOID	SetProcess(BOOL bProcess);
};

#endif
