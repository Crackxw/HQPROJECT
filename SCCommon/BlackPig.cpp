#include <GSL.h>
#include "blackpig.h"
#include "CSeed.h"
#include "CEncrypt.h"

CBlackPig::CBlackPig()
{
	m_bSerial	= FALSE;
}

CBlackPig::~CBlackPig()
{
	
}
void CBlackPig::SetAutoKey( BOOL b )			// 일련번호를 적용시킬때 한번만 호출해 준다
{
	m_bSerial = b;
}


void CBlackPig::Encode( IN unsigned char *pSrc, OUT unsigned char *pDest, DWORD *pdwRK )
{
	if( m_bSerial )
	{
		m_cKey1++; m_cKey2++;
	}

	local_usSize = *(unsigned short *)pSrc;
	
	if(local_usSize >= 19)
	{
		if(pdwRK != NULL)//패킷을 복호화 한다.
			CSeed::GetInstance()->SeedEncrypt((BYTE*)&pDest[3], pdwRK);
	}

	CEncrypt::GetInstance()->EncodeMessage((char*)&pDest[3], (char*)&pSrc[3], local_usSize - 3);

	/*
	if( local_usSize <= 7 ) {

		memcpy( pDest, pSrc, local_usSize );
		return;
	}

	pDest[ 2 ] = pSrc[ local_usSize - 1 ];

	local_cProcessKey1 = m_cKey1;
	local_cProcessKey2 = m_cKey2;

	local_pEnd = (unsigned char*)( pSrc + local_usSize );

	*(unsigned short *)pDest = local_usSize;

	*(DWORD *)( pDest + 3 ) = *(DWORD *)( pSrc + 3 );	

	pDest += 6;

	for( pSrc += 7;	pSrc < local_pEnd; ++pSrc ) {

		++pDest;

		local_cProcessKey1 = local_cProcessKey1 ^ *pSrc;

		*pDest = local_cProcessKey1 + local_cProcessKey2;

		local_cProcessKey2 = local_cProcessKey2 + *pSrc;
	}
	*/

}

//2004-12-07 Seed를 이용한 복호화 - 최병주
//BOOL CBlackPig::Decode( IN unsigned char *pSrc, OUT unsigned char *pDest)
BOOL CBlackPig::Decode( IN unsigned char *pSrc, OUT unsigned char *pDest, DWORD *pdwRK )
{
	if( m_bSerial )
	{
		m_cKey1++; m_cKey2++;
	}

	local_usSize = *(unsigned short *)pSrc;
    
	int nSize = local_usSize - 3;

	memcpy(pDest, pSrc, 3);
	CEncrypt::GetInstance()->DecodeMessage((char*)&pDest[3], (char*)&pSrc[3], nSize);

	if(local_usSize >= 19)
	{
		if(pdwRK != NULL)//패킷을 복호화 한다.
			CSeed::GetInstance()->SeedDecrypt((BYTE*)&pDest[3], pdwRK);
	}

/*
	if(local_usSize < 19)
	{   //패킷사이즈가 19보다 작으면 다른 복호화를 한다.
//		memcpy(pDest, pSrc, 3);
//		CEncrypt::GetInstance()->DecodeMessage((char*)&pDest[3], (char*)&pSrc[3], nSize);

		memcpy(pDest, pSrc, local_usSize);
		return TRUE;
	}

	if(pdwRK != NULL)
	{   //패킷을 복호화 한다.
		CSeed::GetInstance()->SeedDecrypt((BYTE*)&pSrc[3], pdwRK);
		memcpy(pDest, pSrc, local_usSize);
	}
*/
/*

	if( local_usSize <= 7 ) {

		memcpy( pDest, pSrc, local_usSize );
		return TRUE;
	}

	local_cProcessKey1 = m_cKey1;
	local_cProcessKey2 = m_cKey2;

	local_pEnd = (unsigned char *)( pSrc + local_usSize );

	local_pCur = pSrc;

	*(unsigned short *)pDest = local_usSize;

	*(DWORD *)( pDest + 3 ) = *(DWORD *)( pSrc + 3 );
	
	pDest += 6;

	for( pSrc += 7; pSrc < local_pEnd; ++pSrc ) {

		++pDest;

		*pDest = *pSrc - local_cProcessKey2;

		*pDest = *pDest ^ local_cProcessKey1;

		local_cProcessKey1 = local_cProcessKey1 ^ *pDest;

		local_cProcessKey2 = local_cProcessKey2 + *pDest;
	}
	

	if( local_pCur[ 2 ] == *pDest ) return TRUE;
	else return FALSE;
*/
	return TRUE;

}

