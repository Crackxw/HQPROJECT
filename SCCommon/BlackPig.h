#ifndef _BLACKPIG_H
#define _BLACKPIG_H

#define IN
#define OUT
#define INOUT

class CBlackPig
{
public:
	CBlackPig();
	~CBlackPig();

public:
	void			SetKey( IN unsigned char cKey1, IN unsigned char cKey2 ) 
	{ 
		m_cKey1 = cKey1; m_cKey2 = cKey2;  m_bSerial	= FALSE;
	};

	void			GetKey( unsigned char &cKey1, unsigned char &cKey2 ) {
		cKey1 = m_cKey1; cKey2 = m_cKey2;
	}

	void			SetAutoKey( BOOL b );
	BOOL			GetAutoKey()	{ return m_bSerial; };

	void			Encode( IN unsigned char *pSrc, OUT unsigned char *pDest, DWORD *pdwRK );
//	BOOL			Decode( IN unsigned char *pSrc, OUT unsigned char *pDest );
	BOOL			Decode( IN unsigned char *pSrc, OUT unsigned char *pDest, DWORD *pdwRK );

private:
	
	BOOL					m_bSerial;

	unsigned char			m_cKey1;
	unsigned char			m_cKey2;

	unsigned short			local_usSize;
	unsigned char			local_cProcessKey1;
	unsigned char			local_cProcessKey2;
	unsigned char			*local_pCur;
	unsigned char			*local_pEnd;
};

#endif

