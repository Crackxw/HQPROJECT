#ifndef _HQARIELLABELSCRIPTSYSTEM_H_
#define _HQARIELLABELSCRIPTSYSTEM_H_

class HQArielLabelScriptSystem
{
protected:
	FILE *			fpScript;
	long			fsize;

public:
	virtual	BOOL	Open		(char * filename);
	virtual void	Close		(void);

	virtual	BOOL	PreProcess	(void)				{return TRUE;};
	virtual	BOOL	Read		(void)				{return TRUE;};
};

#endif
