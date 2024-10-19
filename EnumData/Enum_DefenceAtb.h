//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: ������ �¶��� [�Ż�] - DefenceAtb ������
//	File Name		: Enum_DefenceAtb.h
//	Birth Date		: 2004. 03. 29.	
//	Creator			: �� �� ��
//	Editer			: �� �� ��
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		�Ż󿡼� ���Ǵ� ������ �������̸�, �� ������ ���α׷� �����Ͽ� ÷�ε˴ϴ�.
//		������ �߰��� ������ Ư���� ��츦 �����ϰ� ���κа� MAX�� ���̿� �߰��Ͻñ� �ٶ��ϴ�.				
//						
//===================================================================================================						

#ifndef	__ENUM_DEFENCEATB_H__
#define	__ENUM_DEFENCEATB_H__

//-----------------------------------------------------------------------------
// Name: ENUM_DEFENCEATB
// Code: actdoll (2004-01-01)
// Desc: 
//		������ �����ϴ� ���ڼ��� �������� 47�ڸ� �Ѿ�� �ȵȴ�!!!!
//		�̰��� ������ �߰��� ��� �ҽ��� ������ �κп���
//		SET_PROTOCOL�� �̿��� �ش� �������� �ݵ�� ����ؾ� �Ѵ�. ��������!!!
//-----------------------------------------------------------------------------
typedef enum	ENUM_DEFENCEATB		
{						
	DEFENCEATB_INVALID				= -1	,	//		�������̴�.(�� ���� �׾ ����Һ�!!!)
	DEFENCEATB_FIRST				= 0		,	//		�Ⱦ��� ��. �������� ���� ���� ���̴�.
	DEFENCEATB_NONE					= 0		,	//		���ð�. �� ���� �������� ���� �۵��� ���ϰԲ� �Ѵ�.

	DEFENCEATB_HUMAN						,	//1		
	DEFENCEATB_ARMOUR    					,	//2		
	DEFENCEATB_HORSE    					,	//3		
	DEFENCEATB_AIRPLANE   					,	//4		
	DEFENCEATB_OILARMOUR   					,	//5		
	DEFENCEATB_FLOAT	 					,	//6		
	DEFENCEATB_ANIMAL    					,	//7		
	DEFENCEATB_GHOST	   					,	//8		
	DEFENCEATB_SOFT		   					,	//9		

	// <����> ����� char-attack, calcattack�� RealApower�� �����ؾ� ��, ������� �������̾�� ��.

	DEFENCEATB_MAX								//		�� ���� ����(���� ������ �ƴϴ�. �̰� ������ ĳ���͸� �߰��ϸ� ���� �ȵȴ�!!!!)
};						

//-----------------------------------------------------------------------------
// Name: CLASS_DEFENCEATB
// Code: actdoll (2004-01-01)
// Desc: ������ ���� ��Ʈ�� ���ڸ� �����ϴ� Ŭ������.
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