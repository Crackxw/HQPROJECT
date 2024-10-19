/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoCommand.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef	__ADO_COMMAND_H_INCLUDED__
#define __ADO_COMMAND_H_INCLUDED__

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
   no_namespace rename("EOF", "EndOfFile")

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#define COMMAND_PTR(ptr)	(*(_CommandPtr*)ptr)

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

class CAdoCommand : public _CommandPtr
{
public:
	CAdoCommand	();
	~CAdoCommand	();

public:

	_RecordsetPtr	Execute				(
		VARIANT*	pRecordsAffected	= NULL,
		VARIANT*	pParameters			= NULL,
		long		lOptions			= adCmdStoredProc);
	_variant_t		GetParameterValue	(short nIndex);
	void			Init				(
		struct _Connection*		pConnection,
		LPCTSTR					szCommandText,
		enum CommandTypeEnum	ct	= adCmdStoredProc);
	void			RefreshParameters	(void);
	void			SetActiveConnection	(struct _Connection* pConnection);
	void			SetCommandText		(LPCTSTR szCommandText);
	void			SetCommandType		(enum CommandTypeEnum ct);
	void			SetParameterValue	(short nIndex, _variant_t vtValue);
};

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#endif	//	__ADO_COMMAND_H_INCLUDED__