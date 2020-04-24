#pragma once


#include "NtlPLProperty.h"
#include "NtlDebug.h"

using std::string;
using std::vector;
#include <string>
#include <vector>



enum eSHORELINE_TYPE
{
	eST_NOTHING = 0, 
	eST_UV_SINGLE,
	eST_UV_DUAL,
	eST_SEQUENCE,
};


#define dSHORELINE_PROP_MAX_STRING_SIZE (32)


class CShoreLineContainer
{
public:
	CShoreLineContainer(void) {;}
	virtual ~CShoreLineContainer(void) {;}

public:
	RwChar	m_KeyName[dSHORELINE_PROP_MAX_STRING_SIZE];
	RwChar	m_TexName0[dSHORELINE_PROP_MAX_STRING_SIZE];
	RwReal	m_Size;
};

class CUVSingleShoreLineContainer : public CShoreLineContainer
{
public:
	CUVSingleShoreLineContainer(void);
	virtual ~CUVSingleShoreLineContainer(void);

public:
	RwReal			m_MPS;
	RwReal			m_UVRange;
	RwTexture*		m_pTex;
	RwReal			m_AccumulatedDist;
	RwBool			m_FlowDirControlFlg;
	D3DXMATRIXA16	m_mTex;

public:
	VOID Update(RwReal _ElapsedTime);
};

class CUVDualShoreLineContainer : public CShoreLineContainer
{
public:
	CUVDualShoreLineContainer(void);
	virtual ~CUVDualShoreLineContainer(void);

public:
	RwChar			m_TexName1[dSHORELINE_PROP_MAX_STRING_SIZE];
	RwReal			m_MPS0;
	RwReal			m_MPS1;
	RwReal			m_UVRange[2];
	RwTexture*		m_pTex[2];
	RwReal			m_AccumulatedDist[2];
	RwBool			m_FlowDirControlFlg[2];
	D3DXMATRIXA16	m_mTex[2];

public:
	VOID Update(RwReal _ElapsedTime);
};

class CSequenceShoreLineContainer : public CShoreLineContainer
{
public:
	CSequenceShoreLineContainer(void);
	virtual ~CSequenceShoreLineContainer(void);

public:
	RwTexture**		m_ppTexPack;
	RwInt32			m_FrameCnt;
	RwInt32			m_FrameIdx;
	RwInt32			m_FPS;
	RwReal			m_AccumulatedTime;

public:
	VOID Update(RwReal _ElapsedTime);
};

class CNtlPLShoreLineCurProp
{
public:
	CNtlPLShoreLineCurProp(void);
	virtual ~CNtlPLShoreLineCurProp(void);

public:
	eSHORELINE_TYPE					m_CurShoreLineType;
	CUVSingleShoreLineContainer*	m_pUVSingleSLC;
	CUVDualShoreLineContainer*		m_pUVDualSLC;
	CSequenceShoreLineContainer*	m_pSequenceSLC;
};

class CNtlPLShoreLineProp : public CNtlPLProperty
{
public:
	CNtlPLShoreLineProp(void);
	virtual ~CNtlPLShoreLineProp(void);

private:
	CNtlPLShoreLineCurProp	m_ShoreLineCurProp;

public:
	vector<CUVSingleShoreLineContainer*>	m_vecUVSingleSLC;
	vector<CUVDualShoreLineContainer*>		m_vecUVDualSLC;
	vector<CSequenceShoreLineContainer*>	m_vecSequenceSLC;

public:
	VOID			Update(RwReal _ElapsedTime);
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode) { return TRUE; }
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode) { return TRUE; }
	VOID			SetCurProp(eSHORELINE_TYPE _eShoreLineType, string& KeyName);
	RwBool			LoadUVSingleShoreLineContainer(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);
	RwBool			LoadUVDualShoreLineContainer(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);
	RwBool			LoadSequenceShoreLineContainer(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);

public:
	CShoreLineContainer*			GetContainer(RwChar* pScriptName);
	eSHORELINE_TYPE					GetCurType() { return m_ShoreLineCurProp.m_CurShoreLineType; }
	CUVSingleShoreLineContainer*	GetUVSContainer() { return m_ShoreLineCurProp.m_pUVSingleSLC; }
	CUVDualShoreLineContainer*		GetUVDContainer() { return m_ShoreLineCurProp.m_pUVDualSLC; }
	CSequenceShoreLineContainer*	GetSeqContainer() { return m_ShoreLineCurProp.m_pSequenceSLC; }
};
