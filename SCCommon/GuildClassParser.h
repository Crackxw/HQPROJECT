#ifndef SO_GUILD_CLASS_PARSER_HEADER
#define SO_GUILD_CLASS_PARSER_HEADER

#define	SO_MAX_GUILD_SC_LENGTH			3				// ���� �ڵ�.(����)�� �ִ� ����

#define	SO_GUILD_SC_DAI		"DAI"		// ���
#define	SO_GUILD_SC_DOB		"DOB"		// ����
#define	SO_GUILD_SC_DHS		"DHS"		// �����
#define	SO_GUILD_SC_BON		"BON"		// �������
#define	SO_GUILD_SC_HNS		"HNS"		// ���
#define	SO_GUILD_SC_HND		"HND"		// �ൿ����
#define	SO_GUILD_SC_JNJ		"JNJ"		// ����
#define	SO_GUILD_SC_SHN		"SHN"		// ��ȯ
#define	SO_GUILD_SC_JWJ		"JWJ"		// ������

struct	strGuildClassInfo
{
	SI32	siRanking;
	SI32	siClassCode;								// ���� �ڵ�.(����)
	UI32	uiClassName;								// ���� �̸�.
	CHAR	szClassCode[SO_MAX_GUILD_SC_LENGTH + 1];	// ���� �ڵ�.(����)
};

class	cltGuildClassParser
{
public:
	SI32				m_siClassNum;					// ���� ����.
	strGuildClassInfo	*m_pstGuildClassInfo;			// ����.
public:
	cltGuildClassParser();				// ������.
	~cltGuildClassParser();				// �Ҹ���.

	BOOL	Init(CHAR *pszFileName);	// �ʱ�ȭ.
	VOID	Free();						// ����.
};

#endif
