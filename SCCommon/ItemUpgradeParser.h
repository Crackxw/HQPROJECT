#ifndef _ITEMUPGRADEPARSER_H_
#define _ITEMUPGRADEPARSER_H_

#include "ItemUpgradeList.h"
#include <stdlib.h>
#include <time.h>

class CItemUpgradeParser  
{
public:
	CItemUpgradeParser();												//������
	virtual ~CItemUpgradeParser();										//�Ҹ���		

	void Initialize();													//�ʱ�ȭ		
	void Destroy();														//���� ó��	

	bool ReadScriptFile();												//���� ����

	OnItemUpgradeResultInfo* GetUpgradeResult(int CurrentItemCode, int magicstonekind);		//���� �����ۿ��� ���� ���׷��̵� �� �������� ������ �����


	int GetNextItemCode( int CurrentItemCode );							//���� ������ �ڵ带 ��ȯ
	OnItemUpgradeResultInfo GetNextItemCodeAndResult( int CurrentItemCode );	//���� ������ �ڵ� �� ����, ���и� ��ȯ. �� �Լ��� Ȯ���� ����ϴ� ����( ���� ���� �� ���� ���� ) �� ��츸 �����ϴ� ����


private:

	CItemUpgradeList *m_pUpgradeItemList;								//�� �����ۿ� ���ؼ� �迭�� ������ ���� (�ϳ��� �迭�� ���ؼ��� 10���� ���׷��̵� �����ۿ� ���� ������ ������ ����)

	OnItemUpgradeResultInfo m_stItemUpgradeResult;						//�����ͷ� �Ѱ��� ����ü ����
};

#endif
