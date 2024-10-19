//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �Է±� ���� ǥ�ñ�
//	File Name		: OnlineInputStatusControl.h
//	Birth Date		: 2004. 11. 01.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Է� ���¿� ���� ǥ�ø� �����ϴ� Ŭ�����Դϴ�.(�ѱ�/����/�߹� ��)
//		��� ������ ��/�� ���� �Է� ��� ǥ��, ����â, ����â ���� ǥ���մϴ�.
//		��� �� ��´��� ���� ��ܿ� ǥ�õǾ�� �ϴ� ���Դϴ�.
//		GS_IMEBase�� �ݵ�� ���ԵǾ�� �մϴ�.
//
//===================================================================================================

#ifndef	__ONLINE_INPUTSTATUSCONTROL_H__
#define	__ONLINE_INPUTSTATUSCONTROL_H__

class OnlineInputStatusControl
{
private:
	static	cltOnlineWorld*		m_pOnlineWorld;					// OnlineWorld ����ü
	
	static	XSPR				m_sprImeIconBase;				// ��������Ʈ - IME ������ �عٴ�
	static	XSPR				m_sprImeIcon;					// ��������Ʈ - IME �����ܵ�
	static	XSPR				m_sprCandidate;					// ��������Ʈ - ����â ��¿� �عٴ�
	static	XSPR				m_sprReadingString;				// ��������Ʈ - ����â ��¿� �عٴ�
	static	int					m_iImeIconFontNo;				// ���� ����� IME �������� ��ȣ

public:
	OnlineInputStatusControl	( void );
	~OnlineInputStatusControl	( void );

	static	void	Init				( cltOnlineWorld *pOnlineWorld );
	static	void	Free				( void );
	static	void	Draw				( LPDIRECTDRAWSURFACE7 lpSurface );

private:
	static	DWORD	GetCurrentLocaleIconIndex	( void );
	static	DWORD	GetCurrentImeIconIndex		( void );
};

#endif//__ONLINE_INPUTSTATUSCONTROL_H__