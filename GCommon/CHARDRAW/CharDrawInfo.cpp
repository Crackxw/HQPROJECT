#include <GSL.h>
#include <main.h>
#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
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
extern	_MainData					GD;									// ������ �ֵ����� 


