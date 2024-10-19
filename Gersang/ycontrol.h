#ifndef	IMJIN2_GAME_CONTROL_HEADER
#define IMJIN2_GAME_CONTROL_HEADER

#include <button.h>

#define		CONTROL_TEXTINPUT_MAX_CHAR						128
#define		CONTROL_OUTPUTMESSAGE_DEFAULT_DELAY				500

class	_YControl
{
protected:
	CHAR	m_ControlName[255];

	SHORT	m_x;
	SHORT	m_y;

	SHORT	m_Controlx;
	SHORT	m_Controly;

	virtual		BOOL		IsInMouse();
public:
	_YControl();
	virtual	~_YControl();

	virtual		void		Init();
	virtual		void		RefreshData() {};
	virtual		void		Draw();
	virtual		void		Action(BOOL bPush) {};	
};

class _TextInput : public _YControl
{
private:
	BOOL		m_IsAction;	
	CHAR		m_szText[CONTROL_TEXTINPUT_MAX_CHAR + 1];	//널 문자를위해 +1을 해준다.
	SHORT		m_nStrlen;
public:
	_TextInput();
	
	virtual	VOID		Create(SHORT x, SHORT y, SHORT width, SHORT height);	
	virtual	VOID		Action(BOOL bPush);	
	virtual	VOID		Draw(HDC hdc);
	virtual	SHORT		GetText(CHAR*	lpszTextBuffer);
};


class _YOutPutMessage : public _YControl
{
private:
	SHORT		m_DelayTime;
public:
	_YOutPutMessage();
	~_YOutPutMessage();

	VOID		Create(SHORT x, SHORT y, SHORT width, SHORT height);
	VOID		Draw();
	VOID		SetText(CHAR* lpszMsg);
	void		Action();
};


#endif
