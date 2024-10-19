
#pragma once

struct NPCIndexHeader;
class cltOnlineWorld;
class OnlineNPCTalk
{
public:
	OnlineNPCTalk(cltOnlineWorld *pWorld);
	~OnlineNPCTalk();

	void				Action();
	void				Draw(LPDIRECTDRAWSURFACE7 pSurface);

private:
	cltOnlineWorld		*m_pMyWorld;
	NPCIndexHeader		*m_pNpcHeader;


	DWORD				m_dwDelayTime;
	int					m_nNowNPCIndex;
	int					m_nTextDraw;
	SI32				m_siDrawX, m_siDrawY;
	BOOL				m_bDraw;
};