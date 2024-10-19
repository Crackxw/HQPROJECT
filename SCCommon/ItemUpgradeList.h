#ifndef _ITEMUPGRADELIST_H_
#define _ITEMUPGRADELIST_H_

#include "ItemUpgrade.h"

class CItemUpgradeList  
{
public:
	CItemUpgradeList();																//생성자
	virtual ~CItemUpgradeList();													//소멸자

	void Initialize();																//초기화
	void Destroy();																	//종료 처리

	void AddItemList( CItemUpgrade * );												//리스트에 CItemUpgrade 추가
	void AddUpgradeItem(int, short, int = 0, int = 0);								//CItemUpgrade 생성해서 정보 넣은 후에 리스트에 추가

	CItemUpgrade *GetFirst();														//m_pFirst 반환
	CItemUpgrade *GetLast();														//m_pLast 반환

	CItemUpgrade *pFindNextItemCode( int CurrentItemCode );							//아이템 코드를 가지고 있는 CItemUpgrade 객체의 주소를 반환
	int iFindNextItemCode( int CurrentItemCode );									//아이템 코드를 가지고 있는 CItemUpgrade 객체의 아이템 코드를 반환

	int GetCode();																	//아이템의 인덱스 코드값 얻어옮
	void SetCode( int );															//아이템의 인덱스 코드값 설정

	char* GetItemName();															//아이템 이름 얻어옮
	void SetItemName( char* );														//아이템 이름 설정

	short GetResult( int CurrentLevel, int Percentage );								//어떤 값을 받아서 성공이냐, 실패냐, broken 이냐를 리턴 해줌


	


private:
	CItemUpgrade *m_pFirst;															//처음 노드
	CItemUpgrade *m_pLast;															//마지막 노드

	int m_Code;																		//아이템 코드
	char m_strItemName[ 64 ];														//아이템 이름

};


#endif
