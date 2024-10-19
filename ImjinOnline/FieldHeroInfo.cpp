/***************************************************************
*
* 파일설명: 온라인 필드에서 주인공 캐릭터에 관한 정보 클래스
*
***************************************************************/

//#include	<windows.h>
#include	"GSCDefine.h"
#include	"FieldHeroInfo.h"

CFieldHeroInfo::CFieldHeroInfo()
{
	Initialize();
}

CFieldHeroInfo::~CFieldHeroInfo()
{
	Initialize();
}

// 초기화
void	CFieldHeroInfo::Initialize(void)
{
	m_siOnlineHeroKind		= 0;				// 주인공 캐릭터 온라인 Kind
	m_bFemale				= false;			// 남성이라면 false, 여성이라면 true
	m_siNation				= ON_COUNTRY_KOREA;	// 국가 구분
	m_siSndHungriest		= 0;				// 아주 배가 고플 때 사운드
	m_siSndHungrier			= 0;				// 배가 고픈 정도가 중간일 때 사운드
	m_siSndHungry			= 0;				// 배가 조금 고플 때 사운드
	m_siSndBoring			= 0;				// 캐릭터가 한 곳에 오래 머물 때 사운드
	m_siSndTryBattle		= 0;				// 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드
	m_siSndMove				= 0;				// 캐릭터가 이동할 때 사운드
	m_siMinmapIconFontNo	= 0;				// 미니맵에 표시되는 주인공 아이콘 이미지 폰트 번호
	m_siRefugeeKind			= 0;				// 공성전 시간에 피난민 표시될 클라이언트 Kind
}

// 데이터 설정
// SI32 siOnlineHeroKind: 주인공 캐릭터 온라인 Kind
// bool bFemale: 남성이라면 false, 여성이라면 true
// SI32 siSndHungriest: 아주 배가 고플 때 사운드
// SI32 siSndHungrier: 배가 고픈 정도가 중간일 때 사운드
// SI32 siSndHungry: 배가 조금 고플 때 사운드
// SI32 siSndBoring: 캐릭터가 한 곳에 오래 머물 때 사운드
// SI32 siSndTryBattle: 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드
// SI32 siSndMove: 캐릭터가 이동할 때 사운드
// SI16 siMinmapIconFontNo: 미니맵에 표시되는 주인공 아이콘 이미지 폰트 번호
// SI32 siRefugeeKind: 공성전 시간에 피난민 표시될 클라이언트 Kind
void	CFieldHeroInfo::SetFieldHeroData(SI32 siOnlineHeroKind, bool bFemale, SI32 siNation, SI32 siSndHungriest, SI32 siSndHungrier, SI32 siSndHungry, SI32 siSndBoring, SI32 siSndTryBattle, SI32 siSndMove, SI16 siMinmapIconFontNo, SI32 siRefugeeKind)
{
	m_siOnlineHeroKind		= siOnlineHeroKind;
	m_bFemale				= bFemale;
	m_siNation				= siNation;
	m_siSndHungriest		= siSndHungriest;
	m_siSndHungrier			= siSndHungrier;
	m_siSndHungry			= siSndHungry;
	m_siSndBoring			= siSndBoring;
	m_siSndTryBattle		= siSndTryBattle;
	m_siSndMove				= siSndMove;
	m_siMinmapIconFontNo	= siMinmapIconFontNo;
	m_siRefugeeKind			= siRefugeeKind;
}

// 주인공 캐릭터 온라인 Kind 얻기
SI32	CFieldHeroInfo::GetOnlineHeroKind(void)
{
	return m_siOnlineHeroKind;
}

// 이 주인공 캐릭터가 여성인가?
bool	CFieldHeroInfo::IsHeroFemale(void)
{
	if (m_bFemale)
		return true;

	return false;
}

// 이 주인공 캐릭터가 남성인가?
bool	CFieldHeroInfo::IsHeroMale(void)
{
	if (m_bFemale)
		return false;

	return true;
}

// 캐릭터 국가 얻기
SI32	CFieldHeroInfo::GetCharacterNation(void)
{
	return m_siNation;
}

// 아주 배가 고플 때 사운드 얻기
SI32	CFieldHeroInfo::GetSndHungriest(void)
{
	return m_siSndHungriest;
}

// 배가 고픈 정도가 중간일 때 사운드 얻기
SI32	CFieldHeroInfo::GetSndHungrier(void)
{
	return m_siSndHungrier;
}

// 배가 조금 고플 때 사운드 얻기
SI32	CFieldHeroInfo::GetSndHungry(void)
{
	return m_siSndHungry;
}

// 캐릭터가 한 곳에 오래 머물 때 사운드 얻기
SI32	CFieldHeroInfo::GetSndBoring(void)
{
	return m_siSndBoring;
}

// 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드 얻기
SI32	CFieldHeroInfo::GetSndTryBattle(void)
{
	return m_siSndTryBattle;
}

// 캐릭터가 이동할 때 사운드 얻기
SI32	CFieldHeroInfo::GetSndMove(void)
{
	return m_siSndMove;
}

// 미니맵에 표시되는 주인공 아이콘 이미지 폰트 번호 얻기
SI16	CFieldHeroInfo::GetMinmapIconFontNo(void)
{
	return m_siMinmapIconFontNo;
}

// 공성전 시간에 피난민으로 표시될 Kind 얻기
SI32	CFieldHeroInfo::GetRefugeeKind(void)
{
	return m_siRefugeeKind;
}
