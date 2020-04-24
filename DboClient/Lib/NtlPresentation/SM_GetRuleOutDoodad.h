#pragma once

#include "isectormethod.h"

class CNtlPLObjectProperty;

class CSM_GetRuleOutDoodad_RetContainer
{
public:
	enum eRULEOUT_TYPE
	{
		eRULEOUT_TYPE_NONE	= 0x0,
		eRULEOUT_TYPE_UPPER = 0x1,
		eRULEOUT_TYPE_LOWER = 0x2,
	};

	struct SSM_GetRuleOutDoodad
	{
		RwUInt32		eRuleOutType;
		RwChar			acName[128];
		RwV3d			vTranslation;
	};

	typedef std::vector<SSM_GetRuleOutDoodad*>	tRET_VEC;
	typedef tRET_VEC::iterator					tRET_VEC_ITER;

protected:
	tRET_VEC m_vecRet;

public:
	CSM_GetRuleOutDoodad_RetContainer();
	virtual ~CSM_GetRuleOutDoodad_RetContainer();

	void			Clear();
	void			Copy(CSM_GetRuleOutDoodad_RetContainer* pRuleOutDoodad_RetContainer);
	void			Push_Back(SSM_GetRuleOutDoodad* pRuleOutDoodad);
	void			Push_Back(const RwUInt32 eRuleOutType, const RwChar* pName, const RwV3d* pvTranslation);
	tRET_VEC_ITER	Begin() { return m_vecRet.begin(); }
	tRET_VEC_ITER	End()	{ return m_vecRet.end(); }		
};

class CSM_GetRuleOutDoodad : public ISectorMethod
{
protected:
	CSM_GetRuleOutDoodad_RetContainer m_RetContainer;

public:
	CSM_GetRuleOutDoodad(void);
	virtual ~CSM_GetRuleOutDoodad(void);

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos);
	virtual RwBool DestroySelfRetMap();
	virtual	RwBool Analyze(DWORD _SectorIdx);
	virtual	VOID*  GetAnalysis();
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI);
	virtual RwBool Report();

private:
	RwBool		GetSRTBBoxFromObjectProperty(RwBBox* pOutBox, CNtlPLObjectProperty* pProperty, const RwV3d* pScaling, const RwV3d* pRotation, const RwV3d* pTranslation);
	RwBool		CheckRuleOut(RwInt32* peRuleOutType, RwV3d vPos, RwReal fUpper, RwReal fLower, const RwV3d* pvHeightfield);
};
