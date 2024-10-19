/************************************************************************************************
	파일명 : Josa.h
	담당자 : 이윤석
************************************************************************************************/
#if !defined(AFX_JOSA_H__BBD774B5_3E21_4821_A6F6_66783FC1A2F8__INCLUDED_)
#define AFX_JOSA_H__BBD774B5_3E21_4821_A6F6_66783FC1A2F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class JOSA  
{
public:
	void JosaProc (char *Source , char *Filter );
	void JosaProc (char *Source );
	JOSA();
	virtual ~JOSA();

};

#endif // !defined(AFX_JOSA_H__BBD774B5_3E21_4821_A6F6_66783FC1A2F8__INCLUDED_)
