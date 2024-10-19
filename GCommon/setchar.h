#ifndef _SETCHAR_H
#define _SETCHAR_H  1

#include "../SCCommon/ParametaBox.h"

// 빈 캐릭터 번호를 돌려준다. 
// 리턴값: 0           -> 빈 캐릭터번호가 없다. 
//         0 이외의 값 -> 빈 캐릭터 번호 
SI32 FindEmptyID(SI32 pnumber);

// 한개의 캐릭터를 삭제한다. 
void DeleteChar(SHORT id);
// 모든 캐릭터를 삭제한다.
void DeleteChar();

// 일반 캐릭터를 설정한다. 
BOOL SetChar(SHORT kind, SHORT id, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip = NULL, MyWeapons *pmw = NULL,SI32 onlineid = 0);

// robypark 2004/6/9 12:36
// 일반 캐릭터를 설정한다.
// 변신하는 캐릭터를 위한 함수 
// 변신하는 캐릭터는 캐릭터 종류(Kind)만 달라질 뿐, 전체적인 캐릭터 리스트 등에는 영향을 주어서는 안된다.
// SHORT CharOrderNumber: 유닛이 배열에서 어느 위치에 있는가?
BOOL SetChar(SHORT kind, _NewID idChar, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID = -1);

#endif