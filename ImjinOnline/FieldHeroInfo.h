/***************************************************************
*
* ���ϼ���: �¶��� �ʵ忡�� ���ΰ� ĳ���Ϳ� ���� ���� Ŭ����
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
		SI32						m_siOnlineHeroKind;		// ���ΰ� ĳ���� �¶��� Kind
		bool						m_bFemale;				// �����̶�� false, �����̶�� true
		SI32						m_siNation;				// ĳ���� ����
		SI32						m_siSndHungriest;		// ���� �谡 ���� �� ����
		SI32						m_siSndHungrier;		// �谡 ���� ������ �߰��� �� ����
		SI32						m_siSndHungry;			// �谡 ���� ���� �� ����
		SI32						m_siSndBoring;			// ĳ���Ͱ� �� ���� ���� �ӹ� �� ����
		SI32						m_siSndTryBattle;		// ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ����
		SI32						m_siSndMove;			// ĳ���Ͱ� �̵��� �� ����
		SI16						m_siMinmapIconFontNo;	// �̴ϸʿ� ǥ�õǴ� ���ΰ� ������ �̹��� ��Ʈ ��ȣ
		SI32						m_siRefugeeKind;		// ������ �ð��� �ǳ��� ǥ�õ� Ŭ���̾�Ʈ Kind

	public:
		// �ʱ�ȭ
		void	Initialize(void);

		// ������ ����
		// SI32 siOnlineHeroKind: ���ΰ� ĳ���� �¶��� Kind
		// bool bFemale: �����̶�� false, �����̶�� true
		// SI32 siNation: ĳ���� ����
		// SI32 siSndHungriest: ���� �谡 ���� �� ����
		// SI32 siSndHungrier: �谡 ���� ������ �߰��� �� ����
		// SI32 siSndHungry: �谡 ���� ���� �� ����
		// SI32 siSndBoring: ĳ���Ͱ� �� ���� ���� �ӹ� �� ����
		// SI32 siSndTryBattle: ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ����
		// SI32 siSndMove: ĳ���Ͱ� �̵��� �� ����
		// SI16 siMinmapIconFontNo: �̴ϸʿ� ǥ�õǴ� ���ΰ� ������ �̹��� ��Ʈ ��ȣ
		// SI32 siRefugeeKind: ������ �ð��� �ǳ��� ǥ�õ� Ŭ���̾�Ʈ Kind
		void	SetFieldHeroData(SI32 siOnlineHeroKind, bool bFemale, SI32 siNation, SI32 siSndHungriest, SI32 siSndHungrier, SI32 siSndHungry, SI32 siSndBoring, SI32 siSndTryBattle, SI32 siSndMove, SI16 siMinmapIconFontNo, SI32 siRefugeeKind);

		// ���ΰ� ĳ���� �¶��� Kind ���
		SI32	GetOnlineHeroKind(void);

		// �� ���ΰ� ĳ���Ͱ� �����ΰ�?
		bool	IsHeroFemale(void);

		// �� ���ΰ� ĳ���Ͱ� �����ΰ�?
		bool	IsHeroMale(void);

		// ĳ���� ���� ���
		SI32	GetCharacterNation(void);

		// ���� �谡 ���� �� ���� ���
		SI32	GetSndHungriest(void);

		// �谡 ���� ������ �߰��� �� ���� ���
		SI32	GetSndHungrier(void);

		// �谡 ���� ���� �� ���� ���
		SI32	GetSndHungry(void);

		// ĳ���Ͱ� �� ���� ���� �ӹ� �� ���� ���
		SI32	GetSndBoring(void);

		// ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ���� ���
		SI32	GetSndTryBattle(void);

		// ĳ���Ͱ� �̵��� �� ���� ���
		SI32	GetSndMove(void);

		// �̴ϸʿ� ǥ�õǴ� ���ΰ� ������ �̹��� ��Ʈ ��ȣ ���
		SI16	GetMinmapIconFontNo(void);

		// ������ �ð��� �ǳ������� ǥ�õ� Kind ���
		SI32	GetRefugeeKind(void);
};

#endif	// _FIELDHEROINFO_H_
