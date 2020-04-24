//***********************************************************************************
//
//	File		:	NtlIniFile.h
//
//	Begin		:	2006-01-05
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Myoung Jin, Choi		( yoshiki@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#ifndef __NTLINIFILE_H__
#define __NTLINIFILE_H__

#include "NtlString.h"

class CNtlIniFile
{
public:

	CNtlIniFile(void);

	virtual ~CNtlIniFile(void);


	int							Create(const char * lpszFullName);

	int							Create(const char * lpszPathName, const char * lpszFileName);


public:


	CNtlString					Read(const char *group, const char *key);

	bool						Read(const char *group, const char *key, CNtlString &val);


	bool						Read(const char *group, const char *key, bool &flag);	

	bool						Read(const char *group, const char *key, char &num);

	bool						Read(const char *group, const char *key, short &num);

	bool						Read(const char *group, const char *key, int &num);

	bool						Read(const char *group, const char *key, float &num);

	bool						Read(const char *group, const char *key, unsigned char &num);

	bool						Read(const char *group, const char *key, unsigned short &num);

	bool						Read(const char *group, const char *key, unsigned int &num);

	bool						Read(const char *pszGroup, const char *pszKey, DWORD &dwNumber);


public:

	const char *				GetConfigFileName() { return m_strConfigFileName.c_str(); }

	const char *				GetLastReadGroup() { return m_strLastReadGroup.c_str(); }

	const char *				GetLastReadKey() { return m_strLastReadKey.c_str(); }

private:

	CNtlString					m_strConfigFileName;

	CNtlString					m_strLastReadGroup;

	CNtlString					m_strLastReadKey;

};

#endif // __NTLINIFILE_H__
