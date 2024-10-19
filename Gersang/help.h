#ifndef _HELP_H
#define _HELP_H

#define SURFACE_WIDTH_HELP  300
#define SURFACE_HEIGHT_HELP 100

#define MAX_TEXT_LENGTH		128

class _Help 
{
private:	
	CHAR	m_szExtendText[MAX_TEXT_LENGTH];	// 화면 하단에 표시되는 텍스트로 전투 중에 마우스 
												// 커서 아래에 있는 객체 이름을 표시할 때 사용한다.

public:
	SHORT	m_ssHelpSwitch;					// 도움말 텍스트창을 사용하는지를 나타내는 플래그
	SHORT	m_ssHelpDrawX, m_ssHelpDrawY;	// 도움말 텍스트창이 그려지는 위치
	BOOL	m_bUsedInfo[3];					// 도움말 텍스트창에 표시되는 추가 텍스트 사용하는지를 나타내는 플래그
											// 3줄의 추가 텍스트 사용가능

	CHAR	m_szText[MAX_TEXT_LENGTH];			// 문자. 도움말 텍스트창의 제목(Title) 텍스트
	CHAR	m_szInfo[3][MAX_TEXT_LENGTH];		// 도움말 텍스트창에 표시되는 추가 텍스트 내용

	// 자원 표시할 때 사용. 임진록 게임에서 캐릭터 생산비용 표시할 때 사용
	// 거상 온라인 전투에서는 사용하지 않음.
	SHORT	m_ssMoney;						// 생산비용(돈)
	SHORT	m_ssTree;						// 생산비용(목재)
	SHORT	m_ssMaintenance;				// 생산비용(차지하는 밥그릇 수)
	SHORT	m_ssMagicPower;					// 기술 사용시 필요한 마법력. 거상 온라인 전투에서도 사용함.
	CHAR	m_scHotKey;						// 단축키. 거상 온라인 전투에서 사용함.

	VOID	Init();
	VOID	Draw();
	VOID	PutExtendHelp(CHAR* lpExtendText);
	VOID	SetHelp(SHORT drawx, SHORT drawy, CHAR* text, CHAR* extendtext,
				SHORT money, SHORT tree, SHORT maintenance,  SHORT magicpower, CHAR hotKey, 				
				CHAR* lpInfo1=NULL, CHAR* lpInfo2=NULL, CHAR* lpInfo3=NULL);
};


#endif
