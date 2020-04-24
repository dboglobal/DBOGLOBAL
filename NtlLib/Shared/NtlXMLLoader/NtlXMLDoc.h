#pragma once

#include <oleauto.h>
#include "msxml2.h"

// WARNING :
// Only 1 CNtlXMLDoc object is allowed to be created in 1 thread.
// This library is not responsible for any problem which occurs when you create 2 or more CNtlXMLDoc objects in the same thread.
// - YOSHIKI
class CNtlXMLDoc
{
public:
	enum EMaxConstants
	{
		MAX_UNICODE_FILE_NAME_LENGTH = 128,
		MAX_UNICODE_XPATH_LENGTH = 128,
		MAX_ATTRIBUTE_NAME_IN_WCHAR = 1024
	};

public:
	CNtlXMLDoc(void);
	virtual ~CNtlXMLDoc(void);

	bool Create();
	bool Destroy();

protected:
	void Init();    

public:
	bool Load(WCHAR* pwszFileName, LONG* lLineNumber, BSTR* bstrErrorReasonString);
	bool Load(WCHAR* pwszFileName);
	bool Load(char* pszFileName);

	bool LoadXML(char* szXMLBuffer);					///< 버퍼로부터 XML 내용을 읽어들인다. (by agebreak)
	bool LoadXML(WCHAR* wszXMLBuffer);

	IXMLDOMNode* SelectSingleNode(WCHAR* pwszXPath);
	IXMLDOMNode* SelectSingleNode(char* pszXPath);

	IXMLDOMNodeList* SelectNodeList(WCHAR* pwszXPath);
	IXMLDOMNodeList* SelectNodeList(char* pszXPath);

	bool GetTextWithAttributeName(IXMLDOMNode* pNode, WCHAR* pwszAttributeName, WCHAR* pwszResultText, int nBufferSizeInWChars);
	bool GetTextWithAttributeName(IXMLDOMNode* pNode, char* pszAttributeName, char* pszResultText, int nBufferSizeInBytes);

	bool GetDataWithXPath(WCHAR* pwszXPath, WCHAR* pwszResultData, int nBufferSizeInWChars);
	bool GetDataWithXPath(char* pszXPath, char* pszResultData, int nBufferSizeInBytes);

	IXMLDOMDocument* GetDocument(void);
    bool SetIndent(WCHAR* szIndentFileName);           ///< XML파일을 저장할때 열을 맞춘다.
    
protected:
	static DWORD m_dwNumberOfCreatedInstances;

	IXMLDOMDocument* m_pXMLDocument;

	bool m_bIsFileLoaded;
};