#include "StdAfx.h"
#include "DataType.h"
#include "NtlLauncherConfig.h"
#include "NtlXMLDoc.h"

CNtlLauncherConfig *CNtlLauncherConfig::s_pInstance = NULL;

CNtlLauncherConfig::~CNtlLauncherConfig(void)
{
}

const char *CNtlLauncherConfig::GetCaptionName()
{
	return m_strCaptionName;
}

const char *CNtlLauncherConfig::GetPatchServerName()
{
	return m_strPatchServerName;
}

const char *CNtlLauncherConfig::GetPatchServerFilePath()
{
	return m_strPatchServerFilePath;
}

const char *CNtlLauncherConfig::GetHtmlInfoFullPathFileName()
{
	return m_strHtmlInfoFullPathFileName;
}


BOOL CNtlLauncherConfig::Create(const char *pszFileName)
{
	CNtlXMLDoc doc;
	doc.Create();
	
	if(doc.Load( const_cast<char *>(pszFileName) ) == false)	
	{
		return FALSE;
	}

	TCHAR strBuffer[1024];
	
	IXMLDOMNode* pNode = doc.SelectSingleNode(LAUNCHER_NAME_XML_NODE);
	if(!doc.GetTextWithAttributeName(pNode, LAUNCHER_CAPTION_NAME_ATTRIBUTE, strBuffer, 1024))
	{
		return FALSE;
	}
	
	m_strCaptionName = strBuffer;

	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}



	pNode = doc.SelectSingleNode(LAUNCHER_PATCH_SERVER_XML_NODE);
	if(!doc.GetTextWithAttributeName(pNode, LAUNCHER_SERVER_NAME_ATTRIBUTE, strBuffer, 1024))
	{
		return FALSE;
	}
	
	m_strPatchServerName = strBuffer;

	if(!doc.GetTextWithAttributeName(pNode, LAUNCHER_SERVER_FILE_PATH_ATTRIBUTE, strBuffer, 1024))
	{
		return FALSE;
	}

	m_strPatchServerFilePath = strBuffer;

	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}


	pNode = doc.SelectSingleNode(LAUNCHER_HTML_INFO_XML_NODE);
	if(!doc.GetTextWithAttributeName(pNode, LAUNCHER_HTML_FULLPATH_FILE_NAME_ATTRIBUTE, strBuffer, 1024))
	{
		return FALSE;
	}
	m_strHtmlInfoFullPathFileName = strBuffer;
	
	if(pNode)
	{
		pNode->Release();
		pNode = NULL;
	}
	
	return TRUE;
}