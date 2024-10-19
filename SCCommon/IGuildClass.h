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

	SI32	m_siJWJCode;		// '지원자' 코드
	SI32	m_siSHNCode;		// '사환' 코드
	SI32	m_siHNSCode;		// '행수' 코드
	SI32	m_siDHSCode;		// '대행수' 코드
	SI32	m_siHNDCode;		// '행동대장' 코드
	SI32	m_siDAICode;		// '대방' 코드
public:
	cltIGuildClass();		// 생성자.
	~cltIGuildClass();		// 소멸자.

	BOOL	Init(CHAR *pszFileName);			// 초기화.
	VOID	Free();								// 메모리해제.

	SI32	GetNumericClassCode(CHAR *pszCode);	// 직급 코드.(문자)를 사용하여 직급 코드.(숫자)를 얻어온다.
	CHAR*	GetStringClassCode(SI32 siCode);	// 직급 코드.(숫자)를 사용하여 직급 코드.(문자)를 얻어온다.

	SI32	GetClassName(SI32 siCode);			// 직급 이름을 얻어온다.

	CHAR*	GetHighClass(CHAR *pszCode1, CHAR *pszCode2);	// 두 직급 코드를 비교하여 높은 직위를 얻어온다.
	SI32	GetHighClass(SI32 siCode1, SI32 siCode2);		// 두 직급 코드를 비교하여 높은 직위를 얻어온다.	

	SI32	GetJWJCode();										// '지원자'의 코드를 얻어온다.
	SI32	GetSHNCode();										// '사환'의 코드를 얻어온다.
	SI32	GetHNSCode();										// '행수'의 코드를 얻어온다.
	SI32	GetDHSCode();										// '대행수'의 코드를 얻어온다.
	SI32	GetHNDCode();										// '행동대장'의 코드를 얻어온다.
	SI32	GetDAICode();										// '대방'의 코드를 얻어온다.
	SI32	ClassComp(SI32 siCode1, SI32 siCode2);				// 두 직급을 비교한다.
};	

#endif
