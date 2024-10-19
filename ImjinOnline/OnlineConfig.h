/********************************************************************************************
	파일명	:	OnlineConfig.h
	작성자	:	정 진 욱
	담담자	:	정 진 욱
	작성일	:	2001.05.23
	수정일	:	2002.04.04
********************************************************************************************/


class OnlineConfig
{
protected:
	char		szID_Name[ON_ACCOUNT_LENGTH+1];										// 아이디
	UI08		uiMiniMapSize;														// 미니맵 사이즈
	UI08		uiServerType;														// 백호, 주작서버 구분(1, 2)
	BOOL		bOpen;

public:
	OnlineConfig();
	virtual ~OnlineConfig();	

	
	BOOL		OpenConfig();														// Config 파일 열기
	void		SetConfig( char *pID );												// COnfig 파일 설정
	void		DeCode( UI08 *pData, UINT nSize );									// 암호 & 해독
	void		SaveConfig();														// COnfig 파일 저장

	void		SetMiniMapSize( UI08 uiSize )		{ if(bOpen)	uiMiniMapSize = uiSize; };	// 미니맵 사이즈를 설정
	void		SetServerType( UI08 uiType )		{ if(bOpen)	uiServerType  = uiType; };	// 서버타입을 설정


	char		*GetID()							{ return szID_Name; }			// 아이디를 리턴함
	UI08		GetMiniMapSIze()					{ return uiMiniMapSize;	};		// 미니맵 사이즈를 리턴.
	UI08		GetServerType()						{ return uiServerType;	};		// 서버타입을 리턴.
};