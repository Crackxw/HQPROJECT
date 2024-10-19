#ifndef _ITEMUPGRADE_H_
#define _ITEMUPGRADE_H_

#include "OnlineCommonStruct.h"

class CItemUpgrade  
{
public:
	CItemUpgrade();																//생성자
	virtual ~CItemUpgrade();													//소멸자

	void Initialize();															//초기화
	void Destroy();																//종료 처리

	void SetNext( CItemUpgrade  * );											//Upgrade 아이템 클래스 노드 설정
	CItemUpgrade  *GetNext( );													//Upgrade 아이템 클래스 얻어옮

	void SetPre( CItemUpgrade * );												//UnUpgrade 아이템 클래스 노드 설정
	CItemUpgrade  *GetPre( );													//UnUpgrade 아이템 클래스 얻어옮

	int GetItemCode();															//아이템 코드 반환
	void SetItemCode( int ItemCode );											//아이템 코드 설정

	void SetItemInfo( int, short, int, int );									//아이템 정보 설정

	short GetResult( int Percentage );											//확률 계산 해서 리턴

private:
	int m_ItemCode;																//아이템 코드 번호
	short m_Level;																//마석을 몇번 사용했는지에 대한 정보를 가지고 있음
	int m_Success;																//성공
	int m_Broken;																//부서짐
	int m_Fail;																	//실패

	CItemUpgrade  *m_pPre;														//UnUpgrade 아이템 클래스 노드
	CItemUpgrade  *m_pNext;														//Upgrade 아이템 클래스 노드
};

#endif