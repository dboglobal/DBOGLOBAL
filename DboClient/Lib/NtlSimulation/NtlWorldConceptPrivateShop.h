#pragma once

#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief 개인상점 이용 할때의 제어 클래스 (현재는 아무것도 제어하지 않는다)
 * \date 2007-07-11
 * \author Cz
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldConceptPrivateShop : 판매자
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNtlWorldConceptPrivateShop : public CNtlWorldConceptController
{
	RwUInt32 m_uiPrivateShopState; // ePRIVATESHOP_STATE
public:
	CNtlWorldConceptPrivateShop(void);
	~CNtlWorldConceptPrivateShop(void);

	virtual RwBool		IsEnableAction(EAvatarAction eAction);
	
	virtual RwUInt32	GetPrivateShopState();
	virtual void		SetPrivateShopState(RwUInt32 uiPrivateShopState);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldConceptPrivateShopVisitor : 구매자
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNtlWorldConceptPrivateShopVisitor : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptPrivateShopVisitor(void);
	~CNtlWorldConceptPrivateShopVisitor(void);

	virtual void	ChangeState(RwInt32 iState);				
	virtual RwBool	IsEnableAction(EAvatarAction eAction);
};
