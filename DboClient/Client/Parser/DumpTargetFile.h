/*****************************************************************************
 *
 * File			: DumpTargetFile.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 26	
 * Abstract		: DBO dump target file
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DUMPTARGET_FILE_H__
#define __DUMPTARGET_FILE_H__

#include "NtlConsole.h"
#include "NtlDumpTarget.h"

class CDumpTargetFile : public CNtlDumpTarget
{
private:

	char	m_pcFileName[250];		/// Dump File Name		woosungs_test 20090804

public:

	CDumpTargetFile();
	~CDumpTargetFile();

	virtual void Dump(void);
	virtual void Active(RwBool bActive);
	/// access
	char*	GetDumpFileName()		{	return m_pcFileName;	}
	void	SetDumpFileName(char* pcFileName);
};

#endif
