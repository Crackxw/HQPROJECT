#ifndef _ITEMUPGRADEPARSER_H_
#define _ITEMUPGRADEPARSER_H_

#include "ItemUpgradeList.h"
#include <stdlib.h>
#include <time.h>

class CItemUpgradeParser  
{
public:
	CItemUpgradeParser();												//생성자
	virtual ~CItemUpgradeParser();										//소멸자		

	void Initialize();													//초기화		
	void Destroy();														//종료 처리	

	bool ReadScriptFile();												//파일 읽음

	OnItemUpgradeResultInfo* GetUpgradeResult(int CurrentItemCode, int magicstonekind);		//현재 아이템에서 다음 업그레이드 될 아이템의 정보를 얻어줌


	int GetNextItemCode( int CurrentItemCode );							//다음 아이템 코드를 반환
	OnItemUpgradeResultInfo GetNextItemCodeAndResult( int CurrentItemCode );	//다음 아이템 코드 및 성공, 실패를 반환. 이 함수는 확률로 계산하는 마석( 붉은 마석 및 검은 마석 ) 일 경우만 생각하는 것임


private:

	CItemUpgradeList *m_pUpgradeItemList;								//각 아이템에 대해서 배열로 가지로 있음 (하나의 배열에 대해서는 10개의 업그레이드 아이템에 대한 정보를 가지고 있음)

	OnItemUpgradeResultInfo m_stItemUpgradeResult;						//포인터로 넘겨줄 구조체 변수
};

#endif
