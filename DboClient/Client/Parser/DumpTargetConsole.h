/*****************************************************************************
 *
 * File			: DumpTargetConsole.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 26	
 * Abstract		: DBO dump target console
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DUMPTARGET_CONSOLE_H__
#define __DUMPTARGET_CONSOLE_H__

#include "NtlConsole.h"
#include "NtlDumpTarget.h"

class CDumpTargetConsole : public CNtlDumpTarget
{
private:

	CNtlConsole m_ConWnd;

public:

	CDumpTargetConsole();
	~CDumpTargetConsole();

	virtual void Dump(void);
	virtual void Active(RwBool bActive);
	virtual void Color(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
};

#endif
