

#ifndef __JO_DEFINES_H__
#define __JO_DEFINES_H__

///////////////////////////////////////////////////////////
// Ado ฐüทร
#define	ADO_CONNECT_SUCCEED				0L
#define	ADO_CONNECT_STRING_NULL			-1L
#define ADO_CONNECT_FAILED				-2L

///////////////////////////////////////////////////////////

#define ADO_BEGIN()		try {
#define ADO_END()			}
#define ADO_ON_ERROR(e)	} catch (_com_error &e) {

///////////////////////////////////////////////////////////


#endif