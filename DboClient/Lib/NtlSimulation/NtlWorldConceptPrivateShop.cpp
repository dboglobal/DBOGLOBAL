#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptPrivateShop.h"
#include "NtlSobManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldConceptPrivateShop : 판매자
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldConceptPrivateShop::CNtlWorldConceptPrivateShop(void)
{
}

CNtlWorldConceptPrivateShop::~CNtlWorldConceptPrivateShop(void)
{
}

RwBool CNtlWorldConceptPrivateShop::IsEnableAction(EAvatarAction eAction) 
{
	switch(eAction)
	{
	case E_ACTION_MOVE:
	case E_ACTION_PICK_WORLD:
	case E_ACTION_PICK_OBJECT:
	case E_ACTION_TARGETTING:
	case E_ACTION_ATTACK:
	case E_ACTION_CHANGE_HEADING:
	case E_ACTION_CHARGING:
	case E_ACTION_LOOTING:
	case E_ACTION_CAN_COMMUNICATION:
		return FALSE;
	}

	return TRUE;
}

RwUInt32 CNtlWorldConceptPrivateShop::GetPrivateShopState()
{
	return m_uiPrivateShopState;
}

void CNtlWorldConceptPrivateShop::SetPrivateShopState(RwUInt32 uiPrivateShopState)
{
	m_uiPrivateShopState = uiPrivateShopState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldConceptPrivateShopVisitor : 구매자
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldConceptPrivateShopVisitor::CNtlWorldConceptPrivateShopVisitor(void)
{
}

CNtlWorldConceptPrivateShopVisitor::~CNtlWorldConceptPrivateShopVisitor(void)
{
}

RwBool CNtlWorldConceptPrivateShopVisitor::IsEnableAction(EAvatarAction eAction) 
{
	switch(eAction)
	{
	case E_ACTION_MOVE:
	case E_ACTION_PICK_WORLD:
	case E_ACTION_PICK_OBJECT:
	case E_ACTION_TARGETTING:
	case E_ACTION_ATTACK:
	case E_ACTION_CHANGE_HEADING:
	case E_ACTION_CAMERA_CONTROL:
	case E_ACTION_CHARGING:
	case E_ACTION_LOOTING:
	case E_ACTION_CAN_COMMUNICATION:
		return FALSE;
	}

	return TRUE;
}

void CNtlWorldConceptPrivateShopVisitor::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);

	switch (iState)
	{
		case WORLD_STATE_ENTER:
		{
			GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_PRIVATE_SHOP, SLCLASS_AVATAR, FALSE);	
			GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_PRIVATE_SHOP, SLCLASS_VEHICLE, FALSE);
		}
		break;
		case WORLD_STATE_EXIT:
		{
			GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_PRIVATE_SHOP, SLCLASS_AVATAR, TRUE);
			GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_PRIVATE_SHOP, SLCLASS_VEHICLE, FALSE);
			CNtlWorldConceptController::ChangeState(WORLD_PLAY_NONE);
		}
		break;
	}
}