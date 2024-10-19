#include <GSL.h>
#include <main.h>
#include "..\Gersang\rain.h"
#include <colortable.h>
#include <etc.h>
#include <CharOrg.h>
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 
extern	unsigned char				DarkTable[][256];					// ��ο� �ܰ� 
extern	CCharFileLoadManager	g_FileLoadManager;					// ������ ĳ����
extern	SI32						UserPlayerNumber;					// ����� ���� 

// ������. 
cltElectronShock::cltElectronShock():
m_bElectronDrawSwitch(FALSE),
m_siElectronDrawStep(0),
m_siElectronDrawRepeat(0)
{
}

void cltElectronShock::SetElectronDraw(BOOL mode)
{
	if(m_bElectronDrawSwitch==TRUE)
	{
		if(mode==TRUE)return ;
	}

	m_bElectronDrawSwitch	=	mode;
	m_siElectronDrawStep	=	0;

}

// ���� ������ ����� �׷��ش�. 
void cltElectronShock::Draw(POINT drawstartpoint, RECT realimagerect)
{

	if(m_bElectronDrawSwitch==FALSE)return ;

	SI32 file=FILE_ELECTRON;
	SI32 font=0+m_siElectronDrawStep;

	SI32 xsize=GetFileXsize(file);
	SI32 ysize=GetFileYsize(file);

	SI32 startx=(drawstartpoint.x + realimagerect.left + (realimagerect.right  - realimagerect.left)/2)-xsize/2;
	SI32 starty=(drawstartpoint.y + realimagerect.top  + (realimagerect.bottom - realimagerect.top )/2)-ysize/2;

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);
	}
}


void cltElectronShock::Action()
{
	// ���� ���� �׸��� �����Ѵ�. 
	if(m_bElectronDrawSwitch==TRUE)
	{
		m_siElectronDrawStep++;
		if(m_siElectronDrawStep==8)
		{
			m_siElectronDrawStep=0;
			m_siElectronDrawRepeat++;
			if(m_siElectronDrawRepeat>3)
			{
				m_bElectronDrawSwitch=FALSE;
			}
		}
	}

}
