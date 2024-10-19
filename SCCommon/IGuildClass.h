#ifndef SO_IGUILD_CLASS_HEADER
#define SO_IGUILD_CLASS_HEADER

#include "GuildClassParser.h"

#define	SO_GUILD_CLASSCOMP_ERROR	1
#define	SO_GUILD_CLASSCOMP_LOW		1
#define SO_GUILD_CLASSCOMP_EQUAL	2
#define	SO_GUILD_CLASSCOMP_HIGH		3

class	cltIGuildClass
{
private:
	cltGuildClassParser		GuildClassParser;

	SI32	m_siJWJCode;		// '������' �ڵ�
	SI32	m_siSHNCode;		// '��ȯ' �ڵ�
	SI32	m_siHNSCode;		// '���' �ڵ�
	SI32	m_siDHSCode;		// '�����' �ڵ�
	SI32	m_siHNDCode;		// '�ൿ����' �ڵ�
	SI32	m_siDAICode;		// '���' �ڵ�
public:
	cltIGuildClass();		// ������.
	~cltIGuildClass();		// �Ҹ���.

	BOOL	Init(CHAR *pszFileName);			// �ʱ�ȭ.
	VOID	Free();								// �޸�����.

	SI32	GetNumericClassCode(CHAR *pszCode);	// ���� �ڵ�.(����)�� ����Ͽ� ���� �ڵ�.(����)�� ���´�.
	CHAR*	GetStringClassCode(SI32 siCode);	// ���� �ڵ�.(����)�� ����Ͽ� ���� �ڵ�.(����)�� ���´�.

	SI32	GetClassName(SI32 siCode);			// ���� �̸��� ���´�.

	CHAR*	GetHighClass(CHAR *pszCode1, CHAR *pszCode2);	// �� ���� �ڵ带 ���Ͽ� ���� ������ ���´�.
	SI32	GetHighClass(SI32 siCode1, SI32 siCode2);		// �� ���� �ڵ带 ���Ͽ� ���� ������ ���´�.	

	SI32	GetJWJCode();										// '������'�� �ڵ带 ���´�.
	SI32	GetSHNCode();										// '��ȯ'�� �ڵ带 ���´�.
	SI32	GetHNSCode();										// '���'�� �ڵ带 ���´�.
	SI32	GetDHSCode();										// '�����'�� �ڵ带 ���´�.
	SI32	GetHNDCode();										// '�ൿ����'�� �ڵ带 ���´�.
	SI32	GetDAICode();										// '���'�� �ڵ带 ���´�.
	SI32	ClassComp(SI32 siCode1, SI32 siCode2);				// �� ������ ���Ѵ�.
};	

#endif
