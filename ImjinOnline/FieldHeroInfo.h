/***************************************************************
*
* 파일설명: 온라인 필드에서 주인공 캐릭터에 관한 정보 클래스
*
***************************************************************/

#ifndef	_FIELDHEROINFO_H_
	#define	_FIELDHEROINFO_H_

#include	"Directives.h"

class CFieldHeroInfo
{
	public:
		CFieldHeroInfo();
		virtual ~CFieldHeroInfo();

	private:
		SI32						m_siOnlineHeroKind;		// 주인공 캐릭터 온라인 Kind
		bool						m_bFemale;				// 남성이라면 false, 여성이라면 true
		SI32						m_siNation;				// 캐릭터 국가
		SI32						m_siSndHungriest;		// 아주 배가 고플 때 사운드
		SI32						m_siSndHungrier;		// 배가 고픈 정도가 중간일 때 사운드
		SI32						m_siSndHungry;			// 배가 조금 고플 때 사운드
		SI32						m_siSndBoring;			// 캐릭터가 한 곳에 오래 머물 때 사운드
		SI32						m_siSndTryBattle;		// 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드
		SI32						m_siSndMove;			// 캐릭터가 이동할 때 사운드
		SI16						m_siMinmapIconFontNo;	// 미니맵에 표시되는 주인공 아이콘 이미지 폰트 번호
		SI32						m_siRefugeeKind;		// 공성전 시간에 피난민 표시될 클라이언트 Kind

	public:
		// 초기화
		void	Initialize(void);

		// 데이터 설정
		// SI32 siOnlineHeroKind: 주인공 캐릭터 온라인 Kind
		// bool bFemale: 남성이라면 false, 여성이라면 true
		// SI32 siNation: 캐릭터 국가
		// SI32 siSndHungriest: 아주 배가 고플 때 사운드
		// SI32 siSndHungrier: 배가 고픈 정도가 중간일 때 사운드
		// SI32 siSndHungry: 배가 조금 고플 때 사운드
		// SI32 siSndBoring: 캐릭터가 한 곳에 오래 머물 때 사운드
		// SI32 siSndTryBattle: 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드
		// SI32 siSndMove: 캐릭터가 이동할 때 사운드
		// SI16 siMinmapIconFontNo: 미니맵에 표시되는 주인공 아이콘 이미지 폰트 번호
		// SI32 siRefugeeKind: 공성전 시간에 피난민 표시될 클라이언트 Kind
		void	SetFieldHeroData(SI32 siOnlineHeroKind, bool bFemale, SI32 siNation, SI32 siSndHungriest, SI32 siSndHungrier, SI32 siSndHungry, SI32 siSndBoring, SI32 siSndTryBattle, SI32 siSndMove, SI16 siMinmapIconFontNo, SI32 siRefugeeKind);

		// 주인공 캐릭터 온라인 Kind 얻기
		SI32	GetOnlineHeroKind(void);

		// 이 주인공 캐릭터가 여성인가?
		bool	IsHeroFemale(void);

		// 이 주인공 캐릭터가 남성인가?
		bool	IsHeroMale(void);

		// 캐릭터 국가 얻기
		SI32	GetCharacterNation(void);

		// 아주 배가 고플 때 사운드 얻기
		SI32	GetSndHungriest(void);

		// 배가 고픈 정도가 중간일 때 사운드 얻기
		SI32	GetSndHungrier(void);

		// 배가 조금 고플 때 사운드 얻기
		SI32	GetSndHungry(void);

		// 캐릭터가 한 곳에 오래 머물 때 사운드 얻기
		SI32	GetSndBoring(void);

		// 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드 얻기
		SI32	GetSndTryBattle(void);

		// 캐릭터가 이동할 때 사운드 얻기
		SI32	GetSndMove(void);

		// 미니맵에 표시되는 주인공 아이콘 이미지 폰트 번호 얻기
		SI16	GetMinmapIconFontNo(void);

		// 공성전 시간에 피난민으로 표시될 Kind 얻기
		SI32	GetRefugeeKind(void);
};

#endif	// _FIELDHEROINFO_H_
