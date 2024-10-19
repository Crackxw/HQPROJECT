#ifndef _ITEMUPGRADE_H_
#define _ITEMUPGRADE_H_

#include "OnlineCommonStruct.h"

class CItemUpgrade  
{
public:
	CItemUpgrade();																//������
	virtual ~CItemUpgrade();													//�Ҹ���

	void Initialize();															//�ʱ�ȭ
	void Destroy();																//���� ó��

	void SetNext( CItemUpgrade  * );											//Upgrade ������ Ŭ���� ��� ����
	CItemUpgrade  *GetNext( );													//Upgrade ������ Ŭ���� ����

	void SetPre( CItemUpgrade * );												//UnUpgrade ������ Ŭ���� ��� ����
	CItemUpgrade  *GetPre( );													//UnUpgrade ������ Ŭ���� ����

	int GetItemCode();															//������ �ڵ� ��ȯ
	void SetItemCode( int ItemCode );											//������ �ڵ� ����

	void SetItemInfo( int, short, int, int );									//������ ���� ����

	short GetResult( int Percentage );											//Ȯ�� ��� �ؼ� ����

private:
	int m_ItemCode;																//������ �ڵ� ��ȣ
	short m_Level;																//������ ��� ����ߴ����� ���� ������ ������ ����
	int m_Success;																//����
	int m_Broken;																//�μ���
	int m_Fail;																	//����

	CItemUpgrade  *m_pPre;														//UnUpgrade ������ Ŭ���� ���
	CItemUpgrade  *m_pNext;														//Upgrade ������ Ŭ���� ���
};

#endif