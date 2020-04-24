#pragma once


struct sTIME_BLOCK_NODE
{
	RwBool	_Vision;
	RwReal	_TimeDelay;	
};


#define dEMUV_UPGRADE03LVL_TBN_NUM (2)
#define dEMUV_UPGRADE04LVL_TBN_NUM (8)


using std::vector;
#include <vector>


class CNtlPLItemEMUVController
{
public:
	CNtlPLItemEMUVController();
	virtual ~CNtlPLItemEMUVController();

protected:
	RwTexture*					m_pEnvTex;
	RwMatrix					m_TexMat;
	RwInt32						m_ID;
	vector<sTIME_BLOCK_NODE>	m_vecTBN;
	RwReal						m_UVSpeed;
	RwReal						m_CurTBNAccumulatedTime;
	RwInt32						m_CurTBNIdx;
	RwBool						m_CurEMUVRefresh;

public:
	virtual VOID Update(RwReal _ElapsedTime) = 0;

	static VOID SetActiveController(CNtlPLItemEMUVController*& _pCurController, RwInt32 _UpgradeIdx);

	RwTexture*	GetEnvTex() { return m_pEnvTex; }
	RwMatrix*	GetTexMat() { return &m_TexMat; }
	RwBool		GetActivation() { return m_vecTBN[m_CurTBNIdx]._Vision; }
};

class CNtlPLItemEMUV_Upgrade03Lvl : public CNtlPLItemEMUVController
{
public:
	CNtlPLItemEMUV_Upgrade03Lvl();
	virtual ~CNtlPLItemEMUV_Upgrade03Lvl() {;}

public:
	virtual VOID Update(RwReal _ElapsedTime);
};

class CNtlPLItemEMUV_Upgrade04Lvl : public CNtlPLItemEMUVController
{
public:
	CNtlPLItemEMUV_Upgrade04Lvl();
	virtual ~CNtlPLItemEMUV_Upgrade04Lvl() {;}

public:
	virtual VOID Update(RwReal _ElapsedTime);
};