#ifndef __ONLINEGAMEOPTION_H
#define __ONLINEGAMEOPTION_H

class OnlineGameOption
{
public:
	BOOL		m_Option_PKFlag;
	
public:
	OnlineGameOption();
	virtual	~OnlineGameOption();

	BOOL					ReadText(char *pFileName);										// General �ε�
	VOID					Free();															// �����
};


#endif
