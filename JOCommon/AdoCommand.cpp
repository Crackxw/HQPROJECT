
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoCommand.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "AdoCommand.h"

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CAdoCommand::CAdoCommand() : _CommandPtr("ADODB.Command")
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CAdoCommand::~CAdoCommand()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

_RecordsetPtr CAdoCommand::Execute(VARIANT*	pRecordsAffected	/*= NULL*/,
									 VARIANT*	pParameters			/*= NULL*/,
									 long		lOptions			/*= adCmdStoredProc*/)
{
	return COMMAND_PTR(this)->Execute(pRecordsAffected, pParameters, lOptions);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

_variant_t CAdoCommand::GetParameterValue(short nIndex)
{
	return COMMAND_PTR(this)->Parameters->GetItem(nIndex)->GetValue();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CAdoCommand::Init(struct _Connection*	pConnection,
						 LPCTSTR				szCommandText,
						 enum CommandTypeEnum	ct	/*= adCmdStoredProc*/)
{
	SetActiveConnection(pConnection);
	SetCommandText(szCommandText);
	SetCommandType(ct);
	RefreshParameters();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CAdoCommand::RefreshParameters()
{
	COMMAND_PTR(this)->Parameters->Refresh();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CAdoCommand::SetActiveConnection	(struct _Connection* pConnection)
{
	COMMAND_PTR(this)->PutRefActiveConnection(pConnection);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CAdoCommand::SetCommandText(LPCTSTR szCommandText)
{
	COMMAND_PTR(this)->CommandText = szCommandText;
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CAdoCommand::SetCommandType(enum CommandTypeEnum ct)
{
	COMMAND_PTR(this)->CommandType = ct;
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CAdoCommand::SetParameterValue(short nIndex, _variant_t vtValue)
{
	
	COMMAND_PTR(this)->Parameters->GetItem(nIndex)->put_Value(vtValue);
}
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


