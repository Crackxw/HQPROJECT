/***************************************************************
*
* ���ϼ���: �¶��� �ʵ忡�� ���ΰ� ĳ���Ϳ� ���� ���� Ŭ����
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

// �ʱ�ȭ
void	CFieldHeroInfo::Initialize(void)
{
	m_siOnlineHeroKind		= 0;				// ���ΰ� ĳ���� �¶��� Kind
	m_bFemale				= false;			// �����̶�� false, �����̶�� true
	m_siNation				= ON_COUNTRY_KOREA;	// ���� ����
	m_siSndHungriest		= 0;				// ���� �谡 ���� �� ����
	m_siSndHungrier			= 0;				// �谡 ���� ������ �߰��� �� ����
	m_siSndHungry			= 0;				// �谡 ���� ���� �� ����
	m_siSndBoring			= 0;				// ĳ���Ͱ� �� ���� ���� �ӹ� �� ����
	m_siSndTryBattle		= 0;				// ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ����
	m_siSndMove				= 0;				// ĳ���Ͱ� �̵��� �� ����
	m_siMinmapIconFontNo	= 0;				// �̴ϸʿ� ǥ�õǴ� ���ΰ� ������ �̹��� ��Ʈ ��ȣ
	m_siRefugeeKind			= 0;				// ������ �ð��� �ǳ��� ǥ�õ� Ŭ���̾�Ʈ Kind
}

// ������ ����
// SI32 siOnlineHeroKind: ���ΰ� ĳ���� �¶��� Kind
// bool bFemale: �����̶�� false, �����̶�� true
// SI32 siSndHungriest: ���� �谡 ���� �� ����
// SI32 siSndHungrier: �谡 ���� ������ �߰��� �� ����
// SI32 siSndHungry: �谡 ���� ���� �� ����
// SI32 siSndBoring: ĳ���Ͱ� �� ���� ���� �ӹ� �� ����
// SI32 siSndTryBattle: ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ����
// SI32 siSndMove: ĳ���Ͱ� �̵��� �� ����
// SI16 siMinmapIconFontNo: �̴ϸʿ� ǥ�õǴ� ���ΰ� ������ �̹��� ��Ʈ ��ȣ
// SI32 siRefugeeKind: ������ �ð��� �ǳ��� ǥ�õ� Ŭ���̾�Ʈ Kind
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

// ���ΰ� ĳ���� �¶��� Kind ���
SI32	CFieldHeroInfo::GetOnlineHeroKind(void)
{
	return m_siOnlineHeroKind;
}

// �� ���ΰ� ĳ���Ͱ� �����ΰ�?
bool	CFieldHeroInfo::IsHeroFemale(void)
{
	if (m_bFemale)
		return true;

	return false;
}

// �� ���ΰ� ĳ���Ͱ� �����ΰ�?
bool	CFieldHeroInfo::IsHeroMale(void)
{
	if (m_bFemale)
		return false;

	return true;
}

// ĳ���� ���� ���
SI32	CFieldHeroInfo::GetCharacterNation(void)
{
	return m_siNation;
}

// ���� �谡 ���� �� ���� ���
SI32	CFieldHeroInfo::GetSndHungriest(void)
{
	return m_siSndHungriest;
}

// �谡 ���� ������ �߰��� �� ���� ���
SI32	CFieldHeroInfo::GetSndHungrier(void)
{
	return m_siSndHungrier;
}

// �谡 ���� ���� �� ���� ���
SI32	CFieldHeroInfo::GetSndHungry(void)
{
	return m_siSndHungry;
}

// ĳ���Ͱ� �� ���� ���� �ӹ� �� ���� ���
SI32	CFieldHeroInfo::GetSndBoring(void)
{
	return m_siSndBoring;
}

// ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ���� ���
SI32	CFieldHeroInfo::GetSndTryBattle(void)
{
	return m_siSndTryBattle;
}

// ĳ���Ͱ� �̵��� �� ���� ���
SI32	CFieldHeroInfo::GetSndMove(void)
{
	return m_siSndMove;
}

// �̴ϸʿ� ǥ�õǴ� ���ΰ� ������ �̹��� ��Ʈ ��ȣ ���
SI16	CFieldHeroInfo::GetMinmapIconFontNo(void)
{
	return m_siMinmapIconFontNo;
}

// ������ �ð��� �ǳ������� ǥ�õ� Kind ���
SI32	CFieldHeroInfo::GetRefugeeKind(void)
{
	return m_siRefugeeKind;
}
