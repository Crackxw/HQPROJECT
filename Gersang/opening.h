#ifndef _OPENING_H
#define _OPENING_H

#ifdef _IMJIN2
#ifdef _IMJIN2PLUS
#define		OPENING_FILENAME		"movie\\openplus.mpg"
#else
#define		OPENING_FILENAME		"movie\\open.mpeg"
#endif
#endif

#ifdef _MYTH
#ifdef _ALLCOUNTRY
#define		OPENING_FILENAME		"movie\\open.avi"
#endif

#ifdef _KOREAONLY
#define		OPENING_FILENAME		"movie\\openk.avi"
#endif

#ifdef _JAPANONLY
#define		OPENING_FILENAME		"movie\\openj.avi"
#endif

#ifdef _CHINAONLY
#define		OPENING_FILENAME		"movie\\openc.avi"
#endif
#endif

//BOOL	InitOpening();
//SHORT	Opening();
//VOID	FreeOpening();

#endif