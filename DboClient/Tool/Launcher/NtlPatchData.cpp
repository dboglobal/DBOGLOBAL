#include "StdAfx.h"
#include "DataType.h"
#include "NtlPatchData.h"
#include "NtlXMLDoc.h"
#include "NtlPatchXmlSave.h"
#include "NtlLauncherConfig.h"

CNtlPatchHistory *CNtlPatchHistory::s_pInstance = NULL;
CNtlPatchHistory::~CNtlPatchHistory()
{
	Destroy();
}

void CNtlPatchHistory::Destroy()
{
	for(unsigned int i = 0; i < m_vecNewVerClientRtDiff.size(); i++)
	{
		if(m_vecNewVerClientRtDiff[i])
		{
			delete m_vecNewVerClientRtDiff[i];
			m_vecNewVerClientRtDiff[i] = NULL;
		}
	}
	m_vecNewVerClientRtDiff.clear();
}

BOOL CNtlPatchHistory::LoadCurVersion(const char *pszFullPathFileName)
{
	CNtlXMLDoc doc;
	doc.Create();
	
	if(doc.Load( const_cast<char *>(pszFullPathFileName) ) == false)
	{
		//File이 없음
		return FALSE;
	}

	TCHAR strBuffer[1024];
	
	IXMLDOMNode* pNode = doc.SelectSingleNode(PATCH_CUR_VER_CLIENT_XML_NODE);
	if(!doc.GetTextWithAttributeName(pNode, PATCH_CUR_VER_ATTRIBUTE, strBuffer, 1024))
	{
		return FALSE;
	}
	m_CurVerClient.Set(strBuffer);

    if(doc.GetTextWithAttributeName(pNode, PATCH_CUR_VER_DATE, strBuffer, 1024))
    {        
        m_strPatchDate = strBuffer;        
    }    
    else
    {
        m_strPatchDate = "0710";    // 걍 디폴트값
    }
    
	
	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}

	pNode = doc.SelectSingleNode(PATCH_CUR_VER_LAUNCHER_XML_NODE);
	if(!doc.GetTextWithAttributeName(pNode, PATCH_CUR_VER_ATTRIBUTE, strBuffer, 1024))
	{
		//Ver Attribute가 없음
		return FALSE;
	}
	
	m_CurVerLauncher.Set(strBuffer);

	if(pNode)
	{
		pNode->Release();
		pNode = NULL;
	}
	
	m_CurVerFullPathFileName = pszFullPathFileName;

	return TRUE;
}

BOOL CNtlPatchHistory::LoadPatchList(const char *pszFullPathFileName)
{
	CNtlXMLDoc doc;
	doc.Create();
	
	if(doc.Load( const_cast<TCHAR *>(pszFullPathFileName) ) == false)
	{
		ASSERT(0);
		return FALSE;
	}

    char szVersion[1024] = {0,};
    char szFileName[1024] = {0,};
    char szDate[32] = {0,};

	// character element list 얻어 온다.
	IXMLDOMNodeList *pList = doc.SelectNodeList("/PatchList/ClientDiffRtPatch/Data");
	IXMLDOMNode* pNode = NULL;

	long  lNodeCount;
	pList->get_length(&lNodeCount);
	for(long i = 0; i < lNodeCount; ++i)
	{
		pList->get_item(i,&pNode);
		if(!doc.GetTextWithAttributeName(pNode, "Ver", szVersion, 1024))
		{
			ASSERT(0);
			return FALSE;
		}
		
		if(!doc.GetTextWithAttributeName(pNode, "PatchFile", szFileName, 1024))
		{
			ASSERT(0);
			return FALSE;
		}

        if(!doc.GetTextWithAttributeName(pNode, "Date", szDate, sizeof(szDate)))
        {
            ASSERT(0);
            return FALSE;
        }

		if(pNode)
		{
			pNode->Release();
			pNode = NULL;
		}

		CNtlPatchData *pPatchData = new CNtlPatchData;
		pPatchData->SetVersion(szVersion);
		pPatchData->SetServerFullPathFileName(CNtlLauncherConfig::GetInstance()->GetPatchServerFilePath(), szFileName);
		pPatchData->SetClientFullPathFileName(PATCH_CLIENT_FILE_TEMP_PATH, szFileName);
        pPatchData->SetDate(szDate);

		m_vecNewVerClientRtDiff.push_back(pPatchData);
	}

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	pNode = doc.SelectSingleNode("PatchList/ClientFullRtPatch/Data");
	if(!doc.GetTextWithAttributeName(pNode, "Ver", szVersion, 1024))
	{
		ASSERT(0);
		return FALSE;
	}
	
	if(!doc.GetTextWithAttributeName(pNode, "PatchFile", szFileName, 1024))
	{
		ASSERT(0);
		return FALSE;
	}
	
	if(pNode)
	{
		pNode->Release();
		pNode = NULL;
	}
	m_NewVerClientRtFull.SetVersion(szVersion);
	m_NewVerClientRtFull.SetServerFullPathFileName(CNtlLauncherConfig::GetInstance()->GetPatchServerFilePath(), szFileName);
	m_NewVerClientRtFull.SetClientFullPathFileName(PATCH_CLIENT_FILE_TEMP_PATH, szFileName);

	pNode = doc.SelectSingleNode("PatchList/LauncherFullRtPatch/Data");
	if(!doc.GetTextWithAttributeName(pNode, "Ver", szVersion, 1024))
	{
		ASSERT(0);
		return FALSE;
	}
	
	if(!doc.GetTextWithAttributeName(pNode, "PatchFile", szFileName, 1024))
	{
		ASSERT(0);
		return FALSE;
	}

	if(pNode)
	{
		pNode->Release();
		pNode = NULL;
	}

	m_NewVerLauncherRtFull.SetVersion(szVersion);
	m_NewVerLauncherRtFull.SetServerFullPathFileName(CNtlLauncherConfig::GetInstance()->GetPatchServerFilePath(), szFileName);
	m_NewVerLauncherRtFull.SetClientFullPathFileName(PATCH_CLIENT_FILE_TEMP_PATH, szFileName);

	return TRUE;
}

CNtlPatchData *CNtlPatchHistory::GetNewVerClientRtDiff()
{
	CNtlPatchVersion *pCurVersion = GetCurVerClient();
	for(unsigned int i = 0; i < m_vecNewVerClientRtDiff.size(); i++)
	{
		CNtlPatchVersion *pNewVersion = m_vecNewVerClientRtDiff[i]->GetVersion();

		if( pCurVersion->GetMajor() < pNewVersion->GetMajor())
			return m_vecNewVerClientRtDiff[i];
		else if(pCurVersion->GetMajor() == pNewVersion->GetMajor() )
		{
			if(pCurVersion->GetMinor() < pNewVersion->GetMinor())
				return m_vecNewVerClientRtDiff[i];
			else if(pCurVersion->GetMinor() == pNewVersion->GetMinor())
			{
				if(pCurVersion->GetBuild() < pNewVersion->GetBuild() )
					return m_vecNewVerClientRtDiff[i];
			}
		}
	}
	return NULL;
}

CNtlPatchData *CNtlPatchHistory::GetNewVerClientRtFull()
{
	if( m_NewVerClientRtFull.GetVersion()->GetMajor() == 0 &&
		m_NewVerClientRtFull.GetVersion()->GetMinor() == 0 &&
		m_NewVerClientRtFull.GetVersion()->GetBuild() == 0 )
		return NULL;

	return &m_NewVerClientRtFull;
}


CNtlPatchData *CNtlPatchHistory::GetNewVerLauncherRtFull()
{
	CNtlPatchVersion *pCurVersion = GetCurVerLauncher();
	CNtlPatchVersion *pNewVersion = m_NewVerLauncherRtFull.GetVersion();
	if( pCurVersion->GetMajor() < pNewVersion->GetMajor() ||
		pCurVersion->GetMinor() < pNewVersion->GetMinor() ||
		pCurVersion->GetBuild() < pNewVersion->GetBuild() )
	{
		return &m_NewVerLauncherRtFull;
	}

	if( m_NewVerLauncherRtFull.GetVersion()->GetMajor() == 0 &&
		m_NewVerLauncherRtFull.GetVersion()->GetMinor() == 0 &&
		m_NewVerLauncherRtFull.GetVersion()->GetBuild() == 0 )
		return NULL;

	return NULL;
}

void CNtlPatchHistory::SetCurVerClient(CNtlPatchVersion *pVersion)
{
	m_CurVerClient.Set(pVersion->Get());
	SaveCurVersion(m_CurVerFullPathFileName);
}

void CNtlPatchHistory::SetCurVerLauncher(CNtlPatchVersion *pVersion)
{
	m_CurVerLauncher.Set(pVersion->Get());
	SaveCurVersion(m_CurVerFullPathFileName);
}

BOOL CNtlPatchHistory::SaveCurVersion(const char *pszFullPathFileName)
{
	CNtlPatchXmlSave XmlSave;
	XmlSave.SaveCurVersion( m_CurVerClient.Get(), 
                            m_strPatchDate,
		                    m_CurVerLauncher.Get(),
							pszFullPathFileName);
	
	return TRUE;
}

