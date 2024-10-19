#ifndef __CELEMENT__
#define __CELEMENT__

// ������ ���Ұ� �Ǵ� Ŭ������ �θ� Ŭ���� 
class CElement
{
private:
	bool	m_bUsed;						//	���� �� ������ ��� ���� 
	int		m_key;							//  ������ ������ Ű�� 
	DWORD	m_uniqID;						//  ������ ������ ���̵� �� 

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