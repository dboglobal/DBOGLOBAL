#include "precomp_ntlpresentation.h"
#include "NtlPLShoreLineProp.h"
#include "NtlXMLDoc.h"
#include "NtlPLResourceManager.h"
#include "ntlworldcommon.h"


CNtlPLShoreLineCurProp::CNtlPLShoreLineCurProp(void)
{
	m_CurShoreLineType	= eST_NOTHING;
	m_pUVSingleSLC		= NULL;
	m_pUVDualSLC		= NULL;
	m_pSequenceSLC		= NULL;
}

CNtlPLShoreLineCurProp::~CNtlPLShoreLineCurProp(void)
{

}

CUVSingleShoreLineContainer::CUVSingleShoreLineContainer(void)
{
	m_pTex				= NULL;
	m_AccumulatedDist	= 0.0f;
	m_FlowDirControlFlg	= TRUE;
	D3DXMatrixIdentity(&m_mTex);
}

CUVSingleShoreLineContainer::~CUVSingleShoreLineContainer(void)
{
	if(m_pTex)
	{
		RwTextureDestroy(m_pTex);
		m_pTex = NULL;
	}	
}

VOID CUVSingleShoreLineContainer::Update(RwReal _ElapsedTime)
{
	RwReal DistInTex = m_MPS * _ElapsedTime;

	if(m_FlowDirControlFlg)
	{
		m_AccumulatedDist -= DistInTex; 

		if(m_AccumulatedDist < -m_UVRange)
		{
			m_FlowDirControlFlg = FALSE;
			m_AccumulatedDist	= -m_UVRange;
		}
	}
	else
	{
		m_AccumulatedDist += DistInTex;

		if(m_AccumulatedDist > m_UVRange)
		{
			m_FlowDirControlFlg = TRUE;
			m_AccumulatedDist	= m_UVRange;
		}
	}
	
	m_mTex._32 = m_AccumulatedDist;
}

CSequenceShoreLineContainer::CSequenceShoreLineContainer(void)
{
	m_ppTexPack			= NULL;
	m_FrameCnt			= 0;
	m_FrameIdx			= 0;
	m_FPS				= 0;
	m_AccumulatedTime	= 0.0f;
}

CSequenceShoreLineContainer::~CSequenceShoreLineContainer(void)
{
	for(RwInt32 i = 0; i < m_FrameCnt; ++i)
	{
		RwTextureDestroy(m_ppTexPack[i]);
	}

	NTL_ARRAY_DELETE(m_ppTexPack);
}


VOID CSequenceShoreLineContainer::Update(RwReal _ElapsedTime)
{
	_ElapsedTime -= (RwReal)(static_cast<RwInt32>(_ElapsedTime));

	RwReal SecPerFrame = 1.0f / m_FPS;

	m_AccumulatedTime += _ElapsedTime;

	RwInt32 CntFrame = 0;
	while(m_AccumulatedTime >= SecPerFrame)
	{
		m_AccumulatedTime -= SecPerFrame;
		++CntFrame;
	}

	m_FrameIdx += CntFrame;
	while(m_FrameIdx >= m_FrameCnt)
	{
		m_FrameIdx -= m_FrameCnt;
	}
}

CUVDualShoreLineContainer::CUVDualShoreLineContainer()
{
	m_pTex[0]				= NULL;
	m_pTex[1]				= NULL;
	m_AccumulatedDist[0]	= 0.0f;
	m_AccumulatedDist[1]	= 0.0f;
	m_FlowDirControlFlg[0]	= TRUE;
	m_FlowDirControlFlg[1]	= TRUE;
	D3DXMatrixIdentity(&m_mTex[0]);
	D3DXMatrixIdentity(&m_mTex[1]);
}

CUVDualShoreLineContainer::~CUVDualShoreLineContainer()
{
	if(m_pTex[0])
	{
		RwTextureDestroy(m_pTex[0]);
		m_pTex[0] = NULL;
	}

	if(m_pTex[1])
	{
		RwTextureDestroy(m_pTex[1]);
		m_pTex[1] = NULL;
	}
}

VOID CUVDualShoreLineContainer::Update(RwReal _ElapsedTime)
{
	// first uv pass
	RwReal DistInTex0 = m_MPS0 * _ElapsedTime;

	if(m_FlowDirControlFlg[0])
	{
		m_AccumulatedDist[0] -= DistInTex0; 

		if(m_AccumulatedDist[0] < -m_UVRange[0])
		{
			m_FlowDirControlFlg[0]	= FALSE;
			m_AccumulatedDist[0]	= -m_UVRange[0];
		}
	}
	else
	{
		m_AccumulatedDist[0] += DistInTex0;

		if(m_AccumulatedDist[0] > m_UVRange[0])
		{
			m_FlowDirControlFlg[0]	= TRUE;
			m_AccumulatedDist[0]	= m_UVRange[0];
		}
	}

	m_mTex[0]._32 = m_AccumulatedDist[0];

	// seconde uv pass
	RwReal DistInTex1 = m_MPS1 * _ElapsedTime;

	if(m_FlowDirControlFlg[1])
	{
		m_AccumulatedDist[1] -= DistInTex1; 

		if(m_AccumulatedDist[1] < -m_UVRange[1])
		{
			m_FlowDirControlFlg[1]	= FALSE;
			m_AccumulatedDist[1]	= -m_UVRange[1];
		}
	}
	else
	{
		m_AccumulatedDist[1] += DistInTex1;

		if(m_AccumulatedDist[1] > m_UVRange[1])
		{
			m_FlowDirControlFlg[1]	= TRUE;
			m_AccumulatedDist[1]	= m_UVRange[1];
		}
	}

	m_mTex[1]._32 = m_AccumulatedDist[1];
}


CNtlPLShoreLineProp::CNtlPLShoreLineProp(void)
{
}

CNtlPLShoreLineProp::~CNtlPLShoreLineProp(void)
{
	for(RwUInt32 i = 0; i < m_vecUVSingleSLC.size(); ++i)
	{
		CUVSingleShoreLineContainer* pDel = m_vecUVSingleSLC[i];
		NTL_DELETE(pDel);
	}

	for(RwUInt32 i = 0; i < m_vecUVDualSLC.size(); ++i)
	{
		CUVDualShoreLineContainer* pDel = m_vecUVDualSLC[i];
		NTL_DELETE(pDel);
	}

	for(RwUInt32 i = 0; i < m_vecSequenceSLC.size(); ++i)
	{
		CSequenceShoreLineContainer* pDel = m_vecSequenceSLC[i];
		NTL_DELETE(pDel);
	}

	m_vecUVSingleSLC.clear();
	m_vecUVDualSLC.clear();
	m_vecSequenceSLC.clear();
}

RwBool CNtlPLShoreLineProp::LoadUVSingleShoreLineContainer(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	CUVSingleShoreLineContainer*	pUVSingleSLC = NTL_NEW CUVSingleShoreLineContainer;
	RwChar							TempBuf[dSHORELINE_PROP_MAX_STRING_SIZE];

	if(!pDoc->GetTextWithAttributeName(pNode, "name", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVSingleSLC);
		return FALSE;
	}
	else
	{
		strcpy_s(pUVSingleSLC->m_KeyName, dSHORELINE_PROP_MAX_STRING_SIZE, TempBuf);
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "size", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVSingleSLC);
		return FALSE;
	}
	else
	{
		pUVSingleSLC->m_Size = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "mps", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVSingleSLC);
		return FALSE;
	}
	else
	{
		pUVSingleSLC->m_MPS = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "uvrange", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVSingleSLC);
		return FALSE;
	}
	else
	{
		pUVSingleSLC->m_UVRange = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "texname0", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVSingleSLC);
		return FALSE;
	}
	else
	{
		pUVSingleSLC->m_pTex = CNtlPLResourceManager::GetInstance()->LoadTexture(TempBuf, ".\\texture\\ntlwe\\ShoreLine\\");
		DBO_ASSERT(pUVSingleSLC->m_pTex, "Texture load failed.");

		RwTextureSetAddressing(pUVSingleSLC->m_pTex, rwTEXTUREADDRESSCLAMP);
	}

	m_vecUVSingleSLC.push_back(pUVSingleSLC);

	return TRUE;
}

RwBool CNtlPLShoreLineProp::LoadUVDualShoreLineContainer(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	CUVDualShoreLineContainer*	pUVDualSLC = NTL_NEW CUVDualShoreLineContainer;
	RwChar						TempBuf[dSHORELINE_PROP_MAX_STRING_SIZE];

	if(!pDoc->GetTextWithAttributeName(pNode, "name", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		strcpy_s(pUVDualSLC->m_KeyName, dSHORELINE_PROP_MAX_STRING_SIZE, TempBuf);
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "size", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_Size = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "mps0", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_MPS0 = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "mps1", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_MPS1 = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "uvrange0", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_UVRange[0] = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "uvrange1", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_UVRange[1] = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "texname0", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_pTex[0] = CNtlPLResourceManager::GetInstance()->LoadTexture(TempBuf, ".\\texture\\ntlwe\\ShoreLine\\");
		DBO_ASSERT(pUVDualSLC->m_pTex[0], "Texture load failed.");

		RwTextureSetAddressing(pUVDualSLC->m_pTex[0], rwTEXTUREADDRESSCLAMP);
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "texname1", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pUVDualSLC);
		return FALSE;
	}
	else
	{
		pUVDualSLC->m_pTex[1] = CNtlPLResourceManager::GetInstance()->LoadTexture(TempBuf, ".\\texture\\ntlwe\\ShoreLine\\");
		DBO_ASSERT(pUVDualSLC->m_pTex[1], "Texture load failed.");

		RwTextureSetAddressing(pUVDualSLC->m_pTex[1], rwTEXTUREADDRESSCLAMP);
	}


	m_vecUVDualSLC.push_back(pUVDualSLC);

	return TRUE;
}

RwBool CNtlPLShoreLineProp::LoadSequenceShoreLineContainer(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	CSequenceShoreLineContainer*	pSequenceSLC = NTL_NEW CSequenceShoreLineContainer;
	RwChar							TempBuf[dSHORELINE_PROP_MAX_STRING_SIZE];

	if(!pDoc->GetTextWithAttributeName(pNode, "name", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pSequenceSLC);
		return FALSE;
	}
	else
	{
		strcpy_s(pSequenceSLC->m_KeyName, dSHORELINE_PROP_MAX_STRING_SIZE, TempBuf);
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "size", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pSequenceSLC);
		return FALSE;
	}
	else
	{
		pSequenceSLC->m_Size = static_cast<RwReal>(atof(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "fps", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pSequenceSLC);
		return FALSE;
	}
	else
	{
		pSequenceSLC->m_FPS = static_cast<RwInt32>(atoi(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "frame", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pSequenceSLC);
		return FALSE;
	}
	else
	{
		pSequenceSLC->m_FrameCnt = static_cast<RwInt32>(atoi(TempBuf));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "texname0", TempBuf, dSHORELINE_PROP_MAX_STRING_SIZE))
	{
		NTL_DELETE(pSequenceSLC);
		return FALSE;
	}
	else
	{
		strcpy_s(pSequenceSLC->m_TexName0, dSHORELINE_PROP_MAX_STRING_SIZE, TempBuf);
	}

	pSequenceSLC->m_ppTexPack = NTL_NEW RwTexture* [pSequenceSLC->m_FrameCnt];
	
	string  strFolderName	= pSequenceSLC->m_TexName0 + string("\\");
	string	strTexPath		= ".\\texture\\ntlwe\\ShoreLine\\";
	RwChar	FileName[256]	= {0,};

	strTexPath = strTexPath + strFolderName;

	for(int j = 0; j < pSequenceSLC->m_FrameCnt; ++j)
	{	
		::sprintf_s(FileName, 256, "%s%d", pSequenceSLC->m_TexName0, j);
		
		pSequenceSLC->m_ppTexPack[j] = CNtlPLResourceManager::GetInstance()->LoadTexture(FileName, strTexPath.c_str());
		DBO_ASSERT(pSequenceSLC->m_ppTexPack[j], "Texture load failed.");

		RwTextureSetAddressing(pSequenceSLC->m_ppTexPack[j], rwTEXTUREADDRESSCLAMP);
	}

	m_vecSequenceSLC.push_back(pSequenceSLC);

	return TRUE;
}

VOID CNtlPLShoreLineProp::SetCurProp(eSHORELINE_TYPE _eShoreLineType, string& KeyName)
{
	m_ShoreLineCurProp.m_CurShoreLineType = _eShoreLineType;

	switch(m_ShoreLineCurProp.m_CurShoreLineType)
	{
	case eST_UV_SINGLE:
		{	
			for(RwUInt32 i = 0; i < m_vecUVSingleSLC.size(); ++i)
			{
				if(!strcmp(m_vecUVSingleSLC[i]->m_KeyName, KeyName.c_str()))
				{
					m_ShoreLineCurProp.m_pUVSingleSLC = m_vecUVSingleSLC[i];
					return;
				}
			}
		}
		break;

	case eST_UV_DUAL:
		{
			for(RwUInt32 i = 0; i < m_vecUVDualSLC.size(); ++i)
			{
				if(!strcmp(m_vecUVDualSLC[i]->m_KeyName, KeyName.c_str()))
				{
					m_ShoreLineCurProp.m_pUVDualSLC = m_vecUVDualSLC[i];
					return;
				}
			}
		}
		break;

	case eST_SEQUENCE:
		{
			for(RwUInt32 i = 0; i < m_vecSequenceSLC.size(); ++i)
			{
				if(!strcmp(m_vecSequenceSLC[i]->m_KeyName, KeyName.c_str()))
				{
					m_ShoreLineCurProp.m_pSequenceSLC = m_vecSequenceSLC[i];
					return;
				}
			}
		}
		break;
	}
}


VOID CNtlPLShoreLineProp::Update(RwReal _ElapsedTime)
{
	for(RwUInt32 i = 0; i < m_vecUVSingleSLC.size(); ++i)
	{
		m_vecUVSingleSLC[i]->Update(_ElapsedTime);
	}

	for(RwUInt32 i = 0; i < m_vecUVDualSLC.size(); ++i)
	{
		m_vecUVDualSLC[i]->Update(_ElapsedTime);
	}

	for(RwUInt32 i = 0; i < m_vecSequenceSLC.size(); ++i)
	{
		m_vecSequenceSLC[i]->Update(_ElapsedTime);
	}
}

CShoreLineContainer* CNtlPLShoreLineProp::GetContainer(RwChar* pScriptName)
{
	for(RwUInt32 i = 0; i < m_vecUVSingleSLC.size(); ++i)
	{
		if(!strcmp(m_vecUVSingleSLC[i]->m_KeyName, pScriptName))
		{
			return static_cast<CShoreLineContainer*>(m_vecUVSingleSLC[i]);
		}
	}

	for(RwUInt32 i = 0; i < m_vecUVDualSLC.size(); ++i)
	{
		if(!strcmp(m_vecUVDualSLC[i]->m_KeyName, pScriptName))
		{
			return static_cast<CShoreLineContainer*>(m_vecUVDualSLC[i]);
		}
	}

	for(RwUInt32 i = 0; i < m_vecSequenceSLC.size(); ++i)
	{
		if(!strcmp(m_vecSequenceSLC[i]->m_KeyName, pScriptName))
		{
			return static_cast<CShoreLineContainer*>(m_vecSequenceSLC[i]);
		}
	}

	return NULL;
}