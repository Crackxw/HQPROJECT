/***************************************************************
*
* 파일설명: 온라인 필드에서 주인공 캐릭터들에 관한 정보 관리자
*
***************************************************************/

#ifndef	_FIELDHEROINFOMGR_H_
	#define	_FIELDHEROINFOMGR_H_

#pragma warning ( disable : 4786 )

#include	"FieldHeroInfo.h"

#include	<vector>

using namespace std;

class CFieldHeroInfoMgr
{
	public:
		CFieldHeroInfoMgr();
		virtual ~CFieldHeroInfoMgr();

	private:
		vector<CFieldHeroInfo>	m_vecFieldHeroInfo;			// 주인공 캐릭터 정보 리스트

	public:
		// 메모리 해제
		void			Free(void);

		// HQ의 FieldHeroInfo.txt 파일 읽기
		BOOL			LoadFieldHeroInfo(void);

		// 캐릭터 Online Kind에 맞는 CFieldHeroInfo얻기
		CFieldHeroInfo	*GetFieldHeroInfo(SI32 siOnlineCharKind);

		// 주인 캐릭터 정보 개수 얻기
		SI32			GetHeroCount(void);
};

#endif	// _FIELDHEROINFOMGR_H_