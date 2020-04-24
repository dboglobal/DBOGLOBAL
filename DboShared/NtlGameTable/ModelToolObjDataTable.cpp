#include "StdAfx.h"
#include "ModelToolObjDataTable.h"
#include "NtlXMLDoc.h"

//#define	XML_PROP_LIST_ELEMENT								"/OBJECT_PROPERTY/OBJECT_ELEMENT/ELEMENT"
//#define XML_PROP_LIST_ELEMENT_NAME_ATTRIBUTE				"NAME"

//#define XML_PROP_NAME_ATTRIBUTE								"/OBJECT_SCRIPT/HEADER/NAME"
//#define XML_PROP_ANIM_DATA_ELEMENT							"/OBJECT_SCRIPT/ANIM_TABLE/ANIM_DATA"

//#define XML_PROP_ANIM_DATA_ELEMENT_KEYID_ATTRIBUTE			"KEYID"
//#define XML_PROP_ANIM_DATA_ELEMENT_PLAYTIME_ATTRIBUTE		"PLAYTIME"

bool sMODELTOOL_OBJ_TBLDAT::Add(int nAnimKeyID, float fDurationTime)
{
	int nRemapAnimKeyID = GetRemapAnimKeyID(nAnimKeyID);

	_ASSERT(nRemapAnimKeyID >= 0 && nRemapAnimKeyID < NTL_MAX_SIZE_OBJ_ANIM);
	if( !(nRemapAnimKeyID >= 0 && nRemapAnimKeyID < NTL_MAX_SIZE_OBJ_ANIM) )
		return false;

	m_aObjTbldat[nRemapAnimKeyID].fDurationTime	= fDurationTime;
	m_aObjTbldat[nRemapAnimKeyID].bDataLoaded	= true;

	return true;
}

sOBJ_DATA_INFO *sMODELTOOL_OBJ_TBLDAT::GetAnim(int nAnimKeyID)
{
	int nRemapAnimKeyID = GetRemapAnimKeyID(nAnimKeyID);

	_ASSERT(nRemapAnimKeyID >= 0 && nRemapAnimKeyID < NTL_MAX_SIZE_OBJ_ANIM);
	if( !(nRemapAnimKeyID >= 0 && nRemapAnimKeyID < NTL_MAX_SIZE_OBJ_ANIM) )
		return NULL;

	return &m_aObjTbldat[nRemapAnimKeyID];
}

float sMODELTOOL_OBJ_TBLDAT::GetAnimDurationTime(int nAnimKeyID)
{
	sOBJ_DATA_INFO * pTblidata = GetAnim(nAnimKeyID);
	if ( NULL == pTblidata )
	{
		return 0.0f;
	}
	return pTblidata->fDurationTime;
}


int sMODELTOOL_OBJ_TBLDAT::GetRemapAnimKeyID( int nAnimKeyID)
{
	return (nAnimKeyID - OBJECT_ANIMATION_START);
}

CModelToolObjDataTable::CModelToolObjDataTable()
{
}

CModelToolObjDataTable::~CModelToolObjDataTable()
{
	Destroy();
}

bool CModelToolObjDataTable::Create(const char *lpszPath)
{
	std::string strPath( lpszPath );
	std::string strFileName;

	if(LoadPropertyList(lpszPath, "ObjectPropertyList.XML") == false)
		return false;

	return true;
}

void CModelToolObjDataTable::Destroy()
{
	TABLEIT stTableIT  = m_mapTableList.begin();
	TABLEIT endTableIT = m_mapTableList.end();
	for(;stTableIT != endTableIT;)
	{
		sMODELTOOL_OBJ_TBLDAT *pModelToolObjData = stTableIT->second;
		if(pModelToolObjData != NULL)
		{
			delete pModelToolObjData;
			pModelToolObjData = NULL;
		}
		++stTableIT;
	}
	m_mapTableList.clear();
}

bool CModelToolObjDataTable::Add(const std::string &strModelName, sMODELTOOL_OBJ_TBLDAT *pModelToolObjData)
{
	if(m_mapTableList.find( strModelName ) != m_mapTableList.end())
	{
		_ASSERT(0);
		return false;
	}
	
	m_mapTableList[strModelName] = pModelToolObjData;
	return true;
}

sMODELTOOL_OBJ_TBLDAT *CModelToolObjDataTable::FindData(const std::string &strModelName)
{
	if(m_mapTableList.find( strModelName ) == m_mapTableList.end())
		return NULL;

	return m_mapTableList[strModelName];
}

bool CModelToolObjDataTable::LoadPropertyList(const char *lpszPath, const char * lpszFileName)
{
	CNtlXMLDoc doc;
	std::string strPath( lpszPath );
	std::string strFileName;
	
	if(doc.Create() == false)
	{
		_ASSERT(0);
		return false;
	}
	
	strFileName = strPath;
	strFileName += "/";
	strFileName += lpszFileName;

	if(doc.Load( const_cast<char *>(strFileName.c_str()) ) == false)
	{
		_ASSERT(0);
		return false;
	}
	
	IXMLDOMNodeList *pList = doc.SelectNodeList("/OBJECT_PROPERTY/OBJECT_ELEMENT/ELEMENT");
	IXMLDOMNode* pNode = NULL;
	char szBuffer[1024];

	long  lNum;
	pList->get_length(&lNum);
	for(long i = 0; i < lNum ; ++i)
	{
		pList->get_item(i,&pNode);

		if(!doc.GetTextWithAttributeName(pNode, "NAME", szBuffer, sizeof(szBuffer)))
		{
			_ASSERT(0);
			return false;
		}
		strFileName = strPath + "/" + szBuffer;
		
		if( LoadProperty( strFileName.c_str() ) == false )
		{
			_ASSERT(0);
			return false;
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	return true;
}

bool CModelToolObjDataTable::LoadProperty(const char * lpszFullPathFileName)
{
	CNtlXMLDoc doc;
	if(doc.Create() == false)
	{
		_ASSERT(0);
		return false;
	}
	
	if(doc.Load( const_cast<char *>(lpszFullPathFileName) ) == false)
	{
		_ASSERT(0);
		return false;
	}
	
	char szBuffer[1024];
	if( !doc.GetDataWithXPath(const_cast<char *>("/OBJECT_SCRIPT/HEADER/NAME"), szBuffer, sizeof(szBuffer)) )
	{
		_ASSERT(0);
		return false;
	}

	sMODELTOOL_OBJ_TBLDAT	*pModelToolObjData = new sMODELTOOL_OBJ_TBLDAT;
	
	Add(szBuffer, pModelToolObjData);

	IXMLDOMNodeList *pAnimDataList = doc.SelectNodeList("/OBJECT_SCRIPT/ANIM_TABLE/ANIM_DATA");
	IXMLDOMNode* pAnimDataNode = NULL;

	long	lAnimDataNodeNum = 0;
	int		nAnimKeyID = 0;
	float	fDurationTime = 0.f;

	pAnimDataList->get_length(&lAnimDataNodeNum);
	for(long m = 0; m < lAnimDataNodeNum ; ++m)
	{
		pAnimDataList->get_item(m,&pAnimDataNode);

		if(!doc.GetTextWithAttributeName(pAnimDataNode, "KEYID", szBuffer, sizeof(szBuffer)))
		{
			_ASSERT(0);
			return false;
		}
		nAnimKeyID = atoi(szBuffer);

		if(!doc.GetTextWithAttributeName(pAnimDataNode, "PLAYTIME", szBuffer, sizeof(szBuffer)))
		{
			_ASSERT(0);
			return false;
		}

		fDurationTime = (float)atof(szBuffer);
		
		pModelToolObjData->Add(nAnimKeyID, fDurationTime);

		nAnimKeyID = 0;
		fDurationTime = 0;
		
		if(pAnimDataNode)
		{
			pAnimDataNode->Release(); 
			pAnimDataNode = NULL;
		}
	}
	
	if(pAnimDataList)
	{
		pAnimDataList->Release(); 
		pAnimDataList = NULL;
	}

	char *pTokenStart = NULL;
	char *pTokenEnd = NULL;
	char szToken[64];

	if(doc.GetTextWithAttributeName(doc.SelectSingleNode("/OBJECT_SCRIPT/BODY/BBOX"), "MIN", szBuffer, sizeof(szBuffer)))
	{
		pTokenStart = szBuffer;
		pTokenEnd = strstr( szBuffer, ";");
		strncpy_s(szToken, sizeof(szToken), pTokenStart, pTokenEnd - pTokenStart);
		pModelToolObjData->m_vMin.x = (float) atof(szToken);

		pTokenStart = pTokenEnd + 1;
		pTokenEnd = strstr( pTokenStart, ";");
		strncpy_s(szToken, sizeof(szToken), pTokenStart, pTokenEnd - pTokenStart);
		pModelToolObjData->m_vMin.y = (float) atof(szToken);

		pTokenStart = pTokenEnd + 1;
		strncpy_s(szToken, sizeof(szToken), pTokenStart, strlen(szBuffer));
		pModelToolObjData->m_vMin.z = (float) atof(szToken);
		

		if(!doc.GetTextWithAttributeName(doc.SelectSingleNode("/OBJECT_SCRIPT/BODY/BBOX"), "MAX", szBuffer, sizeof(szBuffer)))
		{
			_ASSERT(0);
			return false;
		}
		
		pTokenStart = szBuffer;
		pTokenEnd = strstr( szBuffer, ";");
		strncpy_s(szToken, sizeof(szToken), pTokenStart, pTokenEnd - pTokenStart);
		pModelToolObjData->m_vMax.x = (float) atof(szToken);

		pTokenStart = pTokenEnd + 1;
		pTokenEnd = strstr( pTokenStart, ";");
		strncpy_s(szToken, sizeof(szToken), pTokenStart, pTokenEnd - pTokenStart);
		pModelToolObjData->m_vMax.y = (float) atof(szToken);

		pTokenStart = pTokenEnd + 1;
		strncpy_s(szToken, sizeof(szToken), pTokenStart, strlen(szBuffer));
		pModelToolObjData->m_vMax.z = (float) atof(szToken);

		pModelToolObjData->m_fScale = 1.0f;
	}
	else
	{
		if( !doc.GetDataWithXPath("/OBJECT_SCRIPT/BODY/BBOX", szBuffer, sizeof(szBuffer)) )
		{
			_ASSERT(0);
			return false;
		}
		
		CNtlVector vTemp;
		pTokenStart = szBuffer;
		pTokenEnd = strstr( szBuffer, ";");
		strncpy_s(szToken, sizeof(szToken), pTokenStart, pTokenEnd - pTokenStart);
		vTemp.x = (float) atof(szToken);

		pTokenStart = pTokenEnd + 1;
		pTokenEnd = strstr( pTokenStart, ";");
		strncpy_s(szToken, sizeof(szToken), pTokenStart, pTokenEnd - pTokenStart);
		vTemp.y = (float) atof(szToken);

		pTokenStart = pTokenEnd + 1;
		strncpy_s(szToken, sizeof(szToken), pTokenStart, strlen(szBuffer));
		vTemp.z = (float) atof(szToken);

		pModelToolObjData->m_vMin.x = -(vTemp.x / 2.0f);
		pModelToolObjData->m_vMin.y = 0.0f;
		pModelToolObjData->m_vMin.z = -(vTemp.z / 2.0f);
		
		pModelToolObjData->m_vMax.x = vTemp.x / 2.0f;
		pModelToolObjData->m_vMax.y = vTemp.y;
		pModelToolObjData->m_vMax.z = vTemp.z / 2.0f;

		pModelToolObjData->m_fScale = 1.0f;
	}

	return true;
}

