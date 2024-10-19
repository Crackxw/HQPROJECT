#ifndef _ITEMUPGRADELIST_H_
#define _ITEMUPGRADELIST_H_

#include "ItemUpgrade.h"

class CItemUpgradeList  
{
public:
	CItemUpgradeList();																//������
	virtual ~CItemUpgradeList();													//�Ҹ���

	void Initialize();																//�ʱ�ȭ
	void Destroy();																	//���� ó��

	void AddItemList( CItemUpgrade * );												//����Ʈ�� CItemUpgrade �߰�
	void AddUpgradeItem(int, short, int = 0, int = 0);								//CItemUpgrade �����ؼ� ���� ���� �Ŀ� ����Ʈ�� �߰�

	CItemUpgrade *GetFirst();														//m_pFirst ��ȯ
	CItemUpgrade *GetLast();														//m_pLast ��ȯ

	CItemUpgrade *pFindNextItemCode( int CurrentItemCode );							//������ �ڵ带 ������ �ִ� CItemUpgrade ��ü�� �ּҸ� ��ȯ
	int iFindNextItemCode( int CurrentItemCode );									//������ �ڵ带 ������ �ִ� CItemUpgrade ��ü�� ������ �ڵ带 ��ȯ

	int GetCode();																	//�������� �ε��� �ڵ尪 ����
	void SetCode( int );															//�������� �ε��� �ڵ尪 ����

	char* GetItemName();															//������ �̸� ����
	void SetItemName( char* );														//������ �̸� ����

	short GetResult( int CurrentLevel, int Percentage );								//� ���� �޾Ƽ� �����̳�, ���г�, broken �̳ĸ� ���� ����


	


private:
	CItemUpgrade *m_pFirst;															//ó�� ���
	CItemUpgrade *m_pLast;															//������ ���

	int m_Code;																		//������ �ڵ�
	char m_strItemName[ 64 ];														//������ �̸�

};


#endif
