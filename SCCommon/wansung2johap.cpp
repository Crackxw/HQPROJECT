/************************************************************************************************
	ÆÄÀÏ¸í : Wansung2Johap.cpp
	´ã´çÀÚ : ÀÌÀ±¼®

	¿Ï¼ºÇü ¹®ÀÚ¿­À» Á¶ÇÕÇü ¹®ÀÚ¿­·Î ¹Ù²Û´Ù.
************************************************************************************************/
#include <GSL.h>
#include "wansung2johap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wansung2johap::wansung2johap()
{

}

wansung2johap::~wansung2johap()
{

}

int wansung2johap::Convert_Start(unsigned char *hg)
{
	int loc;
    char *p,*d;

    if (*hg > 201 && *(hg+1) > 160) {
        loc = (*hg-202)*93 + (*(hg+1)-161);
        *hg = 224+(int)(loc/186);
        if ((loc%186) < 77) *(hg+1) = 49+(loc%186);
        else *(hg+1) = 145 + (126 - (loc%186));
        *(hg+2)=0;
        return(1);
    }
    d = strstr((char*)h_buffer, (char *)hg);
    if ((d != NULL) && ((int)d % 2 != 1)) {
        sprintf((char*)hg, "Ô%c\0", 128 + ((int)d/2));
        return(0);
    }
    if( *hg < 161 || *(hg+1) < 161 ) return(1);
        
    loc = ((int)(*hg) - 176 )*94;
    loc+= ((int)(*(hg+1)) - 161);
    if ( loc >= 2350 ) {    *hg=' ';    *(hg+1)=' '; *(hg+2)=0; }
    else if ( loc >= 0 ) { 
        p = (char*)i_h_buffer+loc*2; 
        *hg=*p; 
        *(hg+1)=*(p+1); 
    }
    if( *hg < 0x80 ) return(1);
    return(0);
}

unsigned char * wansung2johap::Input_Wansung(unsigned char *hangul_string)
{
	int	result =0,i=0;
	unsigned char c=0;
	unsigned char *data;

	result = strlen((char *)hangul_string);

	data = hangul_string;
	for (i=0;i<result;i++)
	{
		if(Convert_Start(hangul_string)==0) 
			hangul_string+=2; 
		else hangul_string++;
	}

	return (unsigned char*)data;
}
