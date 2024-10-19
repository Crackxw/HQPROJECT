#ifndef _ONLINE_FONT_H
#define _ONLINE_FONT_H

// �ۼ��� : ������
// ����� : ������

#pragma warning ( disable : 4786 )

#include <iostream>
#include <vector>

using namespace std;
 
#include "Online.h"




#define	ON_FONT_CHAR		0
#define	ON_FONT_CHAT		1
#define	ON_FONT_ITEM		2
#define	ON_FONT_LOGIN		3
#define	ON_FONT_BUTTON		4
#define	ON_FONT_PANNEL		5
#define	ON_FONT_STATUS		6
#define	ON_FONT_CHATLIST	7 
#define ON_FONT_VILLAGE		8
#define ON_FONT_MILE		9
#define ON_FONT_NUMBERC		10
#define ON_FONT_BUTTON2		11
#define ON_FONT_CHAR2		12
#define ON_FONT_LOGIN2		13
#define	ON_FONT_CHAT2		14
#define	ON_FONT_MINIMAP1	15
#define	ON_FONT_MINIMAP2	16


class	OnlineFonts
{
public:
	vector<HFONT>	_Font;
	vector<HFONT>	_FontUnderline;

	SI16	siCnt;
	char	m_szFontName[20][20];

public:
	OnlineFonts();
	~OnlineFonts();
	
	BOOL	Create();					// ��Ʈ�� �����Ѵ�.
	VOID	Free();													// ��Ʈ�� �����Ѵ�.
	HFONT	GetFont( SI16 siIndex );								// ��Ʈ�� ���Ϲ޴´�.
	HFONT	GetFontUnderline( SI16 siIndex );						// ��Ʈ�� ���Ϲ޴´�.
	char*	GetFontName(SI16 siIndex);

private:
	BOOL	LoadFont();												// ��Ʈ�� �ؽ�Ʈ ���Ͽ��� �о� �´�.
};


#endif 
