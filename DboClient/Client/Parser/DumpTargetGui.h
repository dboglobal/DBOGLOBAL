/*****************************************************************************
 *
 * File			: DumpTargetGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 26	
 * Abstract		: DBO dump target graphic user interface
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DUMPTARGET_GUI_H__
#define __DUMPTARGET_GUI_H__

#include "NtlConsole.h"
#include "NtlDumpTarget.h"

class CDumpGui;

class CDumpTargetGui : public CNtlDumpTarget
{
private:

	CDumpGui *m_pDumpGui;
	
public:

	CDumpTargetGui();
	~CDumpTargetGui();

	virtual void Dump(void);
	virtual void Active(RwBool bActive);
	virtual void Color(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
};

#endif
