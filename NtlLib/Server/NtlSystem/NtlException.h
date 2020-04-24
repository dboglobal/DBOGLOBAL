//***********************************************************************************
//
//	File		:	NtlException.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Exception handling class
//
//***********************************************************************************

#ifndef __NTLEXCEPTION_H__
#define __NTLEXCEPTION_H__

#include "NtlString.h"
#include <stdio.h>

#define THROW_EXCEPTION(s) throw CNtlException(__FILE__, __LINE__, __FUNCTION__, s)

class CNtlException
{
public:
	//
	CNtlException(const char *filename, int line, const char * func, const char * strWhat)
		:m_strFile(filename), m_iLine(line), m_strFunc(func), m_strWhat(strWhat) {}

	//
	virtual ~CNtlException(void) {}


	//
	virtual void Dump(FILE * fp = stderr)
	{
		fprintf(fp, "EXCEPTION : [%s] in file %s [Line:%d] [Func:%s]\n", GetWhat(), GetFile(), GetLine(), GetFunc());
	}

	//
	const char *				GetFile() const { return m_strFile.c_str(); }

	//
	int							GetLine() const { return m_iLine; }

	//
	const char *				GetFunc() const { return m_strFunc.c_str(); }

	//
	const char *				GetWhat() const { return m_strWhat.c_str(); }


protected:

	CNtlString					m_strFile;

	int							m_iLine;

	CNtlString					m_strFunc;

	CNtlString					m_strWhat;

};

#endif // __NTLEXCEPTION_H__
