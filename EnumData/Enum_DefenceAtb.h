//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: 임진록 온라인 [거상] - DefenceAtb 수열값
//	File Name		: Enum_DefenceAtb.h
//	Birth Date		: 2004. 03. 29.	
//	Creator			: 조 현 준
//	Editer			: 조 현 준
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		거상에서 사용되는 수열값 데이터이며, 이 내용은 프로그램 컴파일에 첨부됩니다.
//		수열을 추가할 때에는 특별한 경우를 제외하고 끝부분과 MAX값 사이에 추가하시기 바랍니다.				
//						
//===================================================================================================						

#ifndef	__ENUM_DEFENCEATB_H__
#define	__ENUM_DEFENCEATB_H__

//-----------------------------------------------------------------------------
// Name: ENUM_DEFENCEATB
// Code: actdoll (2004-01-01)
// Desc: 
//		수열을 정의하는 글자수가 영문기준 47자를 넘어가면 안된다!!!!
//		이곳에 수열을 추가할 경우 소스의 생성자 부분에서
//		SET_PROTOCOL을 이용해 해당 수열명을 반드시 등록해야 한다. 잊지말자!!!
//-----------------------------------------------------------------------------
typedef enum	ENUM_DEFENCEATB		
{						
	DEFENCEATB_INVALID				= -1	,	//		에러값이다.(이 값은 죽어도 절대불변!!!)
	DEFENCEATB_FIRST				= 0		,	//		안쓰는 값. 기준으로 쓰기 위한 값이다.
	DEFENCEATB_NONE					= 0		,	//		세팅값. 이 값을 설정에서 쓰면 작동을 안하게끔 한다.

	DEFENCEATB_HUMAN						,	//1		
	DEFENCEATB_ARMOUR    					,	//2		
	DEFENCEATB_HORSE    					,	//3		
	DEFENCEATB_AIRPLANE   					,	//4		
	DEFENCEATB_OILARMOUR   					,	//5		
	DEFENCEATB_FLOAT	 					,	//6		
	DEFENCEATB_ANIMAL    					,	//7		
	DEFENCEATB_GHOST	   					,	//8		
	DEFENCEATB_SOFT		   					,	//9		

	// <주의> 변경시 char-attack, calcattack의 RealApower도 수정해야 함, 끊김없이 순차적이어야 함.

	DEFENCEATB_MAX								//		총 수열 갯수(실제 갯수는 아니다. 이것 밖으로 캐릭터를 추가하면 절대 안된다!!!!)
};						

//-----------------------------------------------------------------------------
// Name: CLASS_DEFENCEATB
// Code: actdoll (2004-01-01)
// Desc: 수열에 대한 스트링 글자를 관리하는 클래스다.
//-----------------------------------------------------------------------------
class CLASS_DEFENCEATB
{
private:
	struct DATA
	{
		char	m_pszString[48];
	};
	static DATA	m_Data[DEFENCEATB_MAX];

private:
	static	void	SetData( int index, char *pszStr );

public:
	static	ENUM_DEFENCEATB	GetData		( char *pszStr );
	static	const char*		GetData		( ENUM_DEFENCEATB enIndex );
	static	int				GetMaxCount	( void )	{ return DEFENCEATB_MAX; }

	CLASS_DEFENCEATB();
	~CLASS_DEFENCEATB();
};

#endif//__ENUM_DEFENCEATB_H__