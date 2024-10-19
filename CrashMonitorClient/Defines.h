

#ifndef __CRASH_CLIENT_DEFINES_H__
#define __CRASH_CLIENT_DEFINES_H__

const	int		MAX_CRASHINFO_NUMBER		= 15;
const	int		MAX_SELECT_WHERE_NUMBER		= 4;
const	int		MAX_CRASH_TYPE_NUMBER		= 21;
const	int		MAX_STATISTICS_TYPE_NUMBER	= 3;

const	char	EXCEPTION_TYPE[MAX_CRASH_TYPE_NUMBER][128] = 
{
	"TOTAL",				 "ACCESS_VIOLATION",		"ARRAY_BOUNDS_EXCEEDED",
	"BREAKPOINT",		 	 "DATATYPE_MISALIGNMENT",   "FLT_DENORMAL_OPERAND",	
	"FLT_DIVIDE_BY_ZERO", 	 "FLT_INEXACT_RESULT",		"FLT_INVALID_OPERATION",	
	"FLT_OVERFLOW",		 	 "FLT_STACK_CHECK",			"FLT_UNDERFLOW",			
	"ILLEGAL_INSTRUCTION", 	 "IN_PAGE_ERROR",	  		"INT_DIVIDE_BY_ZERO",		
	"INT_OVERFLOW",		 	 "INVALID_DISPOSITION",     "NONCONTINUABLE_EXCEPTION",
	"PRIV_INSTRUCTION",	  	  "SINGLE_STEP",			"STACK_OVERFLOW"
};
const	char	STATISTICS_TITLE_NAME[MAX_STATISTICS_TYPE_NUMBER][128] = 
{
	"Exception type",		"Number",					"Percentage"
};
const	char	CRASH_FILED_INFO[MAX_CRASHINFO_NUMBER][64] = 
{
	"ID",					"Crash Type",				"Crash Contents",
	"OS Type",				"Process Type",				"IP",
	"Process Number",		"Process Level",			"Page Size",
	"Start Time",			"End Time",					"Crash Address",
	"Line Address",			"Nation Code",				"Supposition"
};

const	char	SEARCHWHERE[6][3] = 
{
	"=",	"<>",	"<",	">",	"<=",	">="
};

const	char	SEARCHFIELDNAME[5][32] = 
{
	"CrashType",			"CrashAddress",				"LineAddress",
	"EndTime",				"NationCode"
};

const	char	SEARCHOPERATOR[2][4] = 
{
	"AND",	"OR"
};

#endif // __CRASH_CLIENT_DEFINES_H__