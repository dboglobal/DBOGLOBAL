/*****************************************************************************
 *
 * File			: NtlPLCullingScheduling.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2009. 06. 06	
 * Abstract		: Presentation Entity Culling Scheduling
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLCULLINGSCHEDULING_H__
#define __NTL_PLCULLINGSCHEDULING_H__

#include "NtlPLDef.h"
#include "NtlPLEntity.h"

#define MAX_CULLING_UNIT	50000

class CNtlPLCullingOperator
{
private:

	struct SCullOp
	{
		CNtlPLEntity	*pPLEntity[MAX_CULLING_UNIT];
		RwInt32			iStartPointer;
		RwInt32			iEndPointer;
	};

	SCullOp				m_sCullOp;

	RwInt32				m_iMaxCullAtomicNum;
	RwInt32				m_iCurrAtomicNum;

	RwBool				m_bFpsRatio;

private:

	RwReal			GetCullingFpsRatio(void);

public:

	CNtlPLCullingOperator();
	~CNtlPLCullingOperator();

	void			Update(RwReal fElapsed);

	void			AddCulling(CNtlPLEntity *pPLEntity);
	void			RemoveCulling(CNtlPLEntity *pPLEntity);

	void			EnableFpsRatio(RwBool bEnable);

	RwBool			IsFinish(void);

	void			SetMaxCullAtomicNum(RwInt32 iNum);
	void			IncAtomicOperator(void);	
};


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


class CNtlPLCullingScheduling
{
#ifndef dNTL_WORLD_CULLING_NEW
private:

	typedef std::map<CNtlPLEntity*, CNtlPLEntity*> MapCulling;
	
	MapCulling	m_mapObjCulling;
	MapCulling	m_mapCharCulling;
	MapCulling	m_mapEffCulling;

	CNtlPLCullingOperator m_CullObjOp;
	CNtlPLCullingOperator m_CullOp;

	static CNtlPLCullingScheduling *m_pInstance;

private:

	void	FillObjOp(void);
	void	FillOp(void);

public:

	CNtlPLCullingScheduling();
	~CNtlPLCullingScheduling();

	static CNtlPLCullingScheduling* GetInstance(void);


	void	Update(RwReal fElapsed);

	void	AddCulling(CNtlPLEntity *pPLEntity);
	void	RemoveCulling(RwUInt32 uiClassType, CNtlPLEntity *pPLEntity);

	void	SetMaxCullAtomicNum(RwUInt32 uiClassType, RwInt32 iNum);
	void	IncAtomicOperator(RwUInt32 uiClassType);	
#endif
};

#ifndef dNTL_WORLD_CULLING_NEW
static CNtlPLCullingScheduling* GetNtlPLCullingScheduler(void)
{
	return CNtlPLCullingScheduling::GetInstance();
}
#endif

#endif