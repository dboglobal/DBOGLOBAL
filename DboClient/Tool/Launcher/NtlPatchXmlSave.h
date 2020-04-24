#pragma once

#include "NtlXMLDoc.h"

class CNtlPatchXmlSave : public CNtlXMLDoc
{
public:
	CNtlPatchXmlSave();
	~CNtlPatchXmlSave();

protected:
    IXMLDOMElement*     m_pElemRoot;             ///< Root Element

public:
	BOOL SaveCurVersion(const char *pszClientVersion, const char* pszDate, const char *pszLauncherVersion, const char *pszFullPathFileName);
};
