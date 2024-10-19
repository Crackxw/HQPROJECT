#ifndef _HELP_H
#define _HELP_H

#define SURFACE_WIDTH_HELP  300
#define SURFACE_HEIGHT_HELP 100

#define MAX_TEXT_LENGTH		128

class _Help 
{
private:	
	CHAR	m_szExtendText[MAX_TEXT_LENGTH];	// ȭ�� �ϴܿ� ǥ�õǴ� �ؽ�Ʈ�� ���� �߿� ���콺 
												// Ŀ�� �Ʒ��� �ִ� ��ü �̸��� ǥ���� �� ����Ѵ�.

public:
	SHORT	m_ssHelpSwitch;					// ���� �ؽ�Ʈâ�� ����ϴ����� ��Ÿ���� �÷���
	SHORT	m_ssHelpDrawX, m_ssHelpDrawY;	// ���� �ؽ�Ʈâ�� �׷����� ��ġ
	BOOL	m_bUsedInfo[3];					// ���� �ؽ�Ʈâ�� ǥ�õǴ� �߰� �ؽ�Ʈ ����ϴ����� ��Ÿ���� �÷���
											// 3���� �߰� �ؽ�Ʈ ��밡��

	CHAR	m_szText[MAX_TEXT_LENGTH];			// ����. ���� �ؽ�Ʈâ�� ����(Title) �ؽ�Ʈ
	CHAR	m_szInfo[3][MAX_TEXT_LENGTH];		// ���� �ؽ�Ʈâ�� ǥ�õǴ� �߰� �ؽ�Ʈ ����

	// �ڿ� ǥ���� �� ���. ������ ���ӿ��� ĳ���� ������ ǥ���� �� ���
	// �Ż� �¶��� ���������� ������� ����.
	SHORT	m_ssMoney;						// ������(��)
	SHORT	m_ssTree;						// ������(����)
	SHORT	m_ssMaintenance;				// ������(�����ϴ� ��׸� ��)
	SHORT	m_ssMagicPower;					// ��� ���� �ʿ��� ������. �Ż� �¶��� ���������� �����.
	CHAR	m_scHotKey;						// ����Ű. �Ż� �¶��� �������� �����.

	VOID	Init();
	VOID	Draw();
	VOID	PutExtendHelp(CHAR* lpExtendText);
	VOID	SetHelp(SHORT drawx, SHORT drawy, CHAR* text, CHAR* extendtext,
				SHORT money, SHORT tree, SHORT maintenance,  SHORT magicpower, CHAR hotKey, 				
				CHAR* lpInfo1=NULL, CHAR* lpInfo2=NULL, CHAR* lpInfo3=NULL);
};


#endif
