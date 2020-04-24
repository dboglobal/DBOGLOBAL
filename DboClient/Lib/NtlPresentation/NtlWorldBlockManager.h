#pragma once

#include <rppvs.h>

#include "ntlworldcommon.h"

class CNtlWorldBlockMananger
{
private:
	RwInt32					m_iWorldBlockSize;
	RwInt32					m_iWorldBlockNum;

	RwInt32					m_iOldBlockIdx;
	RwInt32					m_iNewBlockIdx;

	TBLIDX					m_CurMapNameIdx;

	RwReal					m_fAnotherBlockVariationTime;

	sNTL_FIELD_PROP*		m_pWorldFieldProp;
	sNTL_BLOCK_PROP*		m_pWorldBlockProp;

public:
	CNtlWorldBlockMananger();
	virtual ~CNtlWorldBlockMananger();

	virtual void		Init(RwInt32 iWorldSize, RwInt32 iWorldBlockSize);
	virtual void		Free();

	virtual RwBool		LoadProperty();
	virtual RwBool		SaveProperty();
	virtual RwBool		SaveProperty(RwUInt32 iBlockIdx);

	virtual void		Update(RwReal fElapsed, RwV3d& vPos, eNTL_WORLD_DIR eDir);
	virtual void		UpdateAnotherBlock(RwReal fElapsed, RwV3d& vPos, eNTL_WORLD_DIR eDir);

	virtual RwBool		IsBlockValid(RwV3d& vPos);
	virtual RwBool		IsBlockValid(RwInt32 iIdx);

	virtual RwInt32		GetBlockIdx(RwV3d& vPos);	
	virtual void		SetAnotherBlock(RwReal fVariationTime = 0.001f);

	virtual RwBool		GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp);
	virtual RwBool		SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID);
	virtual RwBool		GetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp);
	virtual RwBool		SetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp, eNTL_BLOCK_PROPID NtlBlockPropID);

	virtual RwBool		GenBlockBBox(RwInt32 iBlockIdx);

// 	RwInt32				GetWorldBlockNum() { return m_iWorldBlockNum; }
// 	RwInt32				GetWorldBlockSize(){ return m_iWorldBlockSize; }
};
