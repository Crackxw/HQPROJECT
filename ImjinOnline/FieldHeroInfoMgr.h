/***************************************************************
*
* ���ϼ���: �¶��� �ʵ忡�� ���ΰ� ĳ���͵鿡 ���� ���� ������
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
		vector<CFieldHeroInfo>	m_vecFieldHeroInfo;			// ���ΰ� ĳ���� ���� ����Ʈ

	public:
		// �޸� ����
		void			Free(void);

		// HQ�� FieldHeroInfo.txt ���� �б�
		BOOL			LoadFieldHeroInfo(void);

		// ĳ���� Online Kind�� �´� CFieldHeroInfo���
		CFieldHeroInfo	*GetFieldHeroInfo(SI32 siOnlineCharKind);

		// ���� ĳ���� ���� ���� ���
		SI32			GetHeroCount(void);
};

#endif	// _FIELDHEROINFOMGR_H_