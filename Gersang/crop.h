#ifndef _CROP_H
#define _CROP_H

// kind�� ��� ��Ȯ�� ��ġ�� �ִ� ���ΰ�?
BOOL IsCropHarvest(SHORT id, SHORT kind);
// ����� ���� ��� ã�´�. 
BOOL FindNearCrop(SHORT x, SHORT y, SHORT &dx, SHORT &dy, SHORT croptype);

#endif