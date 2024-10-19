#ifndef SO_GUILD_CLASS_PARSER_HEADER
#define SO_GUILD_CLASS_PARSER_HEADER

#define	SO_MAX_GUILD_SC_LENGTH			3				// 직급 코드.(문자)의 최대 길이

#define	SO_GUILD_SC_DAI		"DAI"		// 대방
#define	SO_GUILD_SC_DOB		"DOB"		// 도방
#define	SO_GUILD_SC_DHS		"DHS"		// 대행수
#define	SO_GUILD_SC_BON		"BON"		// 본전행수
#define	SO_GUILD_SC_HNS		"HNS"		// 행수
#define	SO_GUILD_SC_HND		"HND"		// 행동대장
#define	SO_GUILD_SC_JNJ		"JNJ"		// 전주
#define	SO_GUILD_SC_SHN		"SHN"		// 사환
#define	SO_GUILD_SC_JWJ		"JWJ"		// 지원자

struct	strGuildClassInfo
{
	SI32	siRanking;
	SI32	siClassCode;								// 직급 코드.(숫자)
	UI32	uiClassName;								// 직급 이름.
	CHAR	szClassCode[SO_MAX_GUILD_SC_LENGTH + 1];	// 직급 코드.(문자)
};

class	cltGuildClassParser
{
public:
	SI32				m_siClassNum;					// 직급 개수.
	strGuildClassInfo	*m_pstGuildClassInfo;			// 직급.
public:
	cltGuildClassParser();				// 생성자.
	~cltGuildClassParser();				// 소멸자.

	BOOL	Init(CHAR *pszFileName);	// 초기화.
	VOID	Free();						// 해제.
};

#endif
