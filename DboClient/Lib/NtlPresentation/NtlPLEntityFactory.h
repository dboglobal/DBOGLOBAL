/*****************************************************************************
 *
 * File			: NtlPLEntityFactory.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer entity factory class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLENTITY_FACTORY_H__
#define __NTL_PLENTITY_FACTORY_H__

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"
#include "NtlPLEntity.h"

/**
 * \ingroup NtlPresentation
 * entity 생성 및 소멸을 담당하는 factory class.
 * presentation의 모든 entity의 생성 및 소멸은 factory 방식을 사용한다.
 *
 */

class CNtlPLEntityFactory
{
public:

	/**
	*  entity를 생성하는 함수.
	*  \param pEntityName 생성할 entity name.
	*  \retrun 생성에 성공하면 entity pointer, 실패하면 NULL를 리턴한다.
	*  \see DeleteEntity
	*/
	static CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const SPLEntityCreateParam* _pParam);

	/**
	*  entity를 소멸하는 함수.
	*  \param pEntity 소멸할 entity pointer.
	*  \see CreateEntity
	*/
	static void DeleteEntity(CNtlPLEntity *pEntity);
};


#endif
