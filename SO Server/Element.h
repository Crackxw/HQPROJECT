#ifndef __CELEMENT__
#define __CELEMENT__

// 집합의 원소가 되는 클래스의 부모 클래스 
class CElement
{
private:
	bool	m_bUsed;						//	현재 이 원소의 사용 여부 
	int		m_key;							//  원소의 설정된 키값 
	DWORD	m_uniqID;						//  원소의 유일한 아이디 값 

public:
	CElement();
	virtual ~CElement();

	void	SetUsed(bool which);
	bool	GetUsed();

	void	SetKey(int key);
	int		GetKey();

private:	
	void	SetUniqID(DWORD uniqID);
	DWORD	GetUniqID();
};

#endif