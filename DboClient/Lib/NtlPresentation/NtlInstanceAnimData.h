/*****************************************************************************
 *
 * File			: NtlInstanceAnimData.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 11	
 * Abstract		: NTL NtlInstanceAnimData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_INSTANCE_ANIMDATA_H__
#define __NTL_INSTANCE_ANIMDATA_H__

#include "NtlAnimEventData.h"
#include "NtlPLResource.h"
#include "NtlTypeAnimData.h"

/*!
 * \Animation
 * SInstanceAnimData: 변하는 Data(Read, Write 가능)
 * 실제 Resource들(RtAnimAnimation)을 관리
 */
struct SInstanceAnimData
{
	CNtlPLResource	*pResource;			///< Resource
	STypeAnimData	*pTypeAnimData;		///< TypeAnimData Reference

	SInstanceAnimData() : pTypeAnimData(NULL), pResource(NULL) {}
};

typedef	std::map<RwUInt32, SInstanceAnimData *>		      INSTANCE_ANIM_MAP;
typedef	std::map<RwUInt32, SInstanceAnimData *>::iterator INSTANCE_ANIM_MAP_ITER;

class CNtlInstanceAnimTable
{
public:
	CNtlInstanceAnimTable() : m_pTypeAnimTable(NULL) {};
	~CNtlInstanceAnimTable() {};

protected:
	INSTANCE_ANIM_MAP	m_mapInstanceAnim;		///< Instance Animation Table
	CNtlTypeAnimTable	*m_pTypeAnimTable;		///< TypeAnimTable Reference

public:
	SInstanceAnimData *Get(RwUInt32 uiKey);
	SInstanceAnimData *Add(RwUInt32 uiKey);

	void	Create(CNtlTypeAnimTable *pTypeAnimTable);
	void	Destroy();
};


#endif