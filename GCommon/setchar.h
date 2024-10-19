#ifndef _SETCHAR_H
#define _SETCHAR_H  1

#include "../SCCommon/ParametaBox.h"

// �� ĳ���� ��ȣ�� �����ش�. 
// ���ϰ�: 0           -> �� ĳ���͹�ȣ�� ����. 
//         0 �̿��� �� -> �� ĳ���� ��ȣ 
SI32 FindEmptyID(SI32 pnumber);

// �Ѱ��� ĳ���͸� �����Ѵ�. 
void DeleteChar(SHORT id);
// ��� ĳ���͸� �����Ѵ�.
void DeleteChar();

// �Ϲ� ĳ���͸� �����Ѵ�. 
BOOL SetChar(SHORT kind, SHORT id, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip = NULL, MyWeapons *pmw = NULL,SI32 onlineid = 0);

// robypark 2004/6/9 12:36
// �Ϲ� ĳ���͸� �����Ѵ�.
// �����ϴ� ĳ���͸� ���� �Լ� 
// �����ϴ� ĳ���ʹ� ĳ���� ����(Kind)�� �޶��� ��, ��ü���� ĳ���� ����Ʈ ��� ������ �־�� �ȵȴ�.
// SHORT CharOrderNumber: ������ �迭���� ��� ��ġ�� �ִ°�?
BOOL SetChar(SHORT kind, _NewID idChar, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID = -1);

#endif