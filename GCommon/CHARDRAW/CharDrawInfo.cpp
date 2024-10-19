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

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 
extern	unsigned char				DarkTable[][256];					// 어두운 단계 
extern	CCharFileLoadManager	g_FileLoadManager;					// 조합형 캐릭터
extern	SI32						UserPlayerNumber;					// 사용자 관련 
extern	_MainData					GD;									// 게임의 주데이터 


