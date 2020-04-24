#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptDirect.h"

// core
#include "NtlDebug.h"

// PL
#include "NtlPLGlobal.h"
#include "NtlPLOptionManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "InputActionMap.h"
#include "NtlStorageManager.h"

// World Concept Direct에서 적용할 지형 및 사물 시야거리
#define dDIRECT_TERRAIN_FAR		(512.f)
#define dDIRECT_OBJECT_FAR		(512.f)

CNtlWorldConceptDirect::CNtlWorldConceptDirect(void)
{
	
}

CNtlWorldConceptDirect::~CNtlWorldConceptDirect(void)
{
}

RwBool CNtlWorldConceptDirect::IsEnableAction(EAvatarAction eAction) 
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

	return FALSE;
}

void CNtlWorldConceptDirect::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);

	switch(iState)
	{
	case WORLD_STATE_ENTER:
		{
			GetInputActionMap()->Reset();
			CNtlSLEventGenerator::ActionMapHalt();

            // Entity Culling을 끈다 (by agebreak)
            CNtlPLGlobal::m_bEntityNotCulling = TRUE;

			// 지형 시야 거리, 사물 시야 거리 옵션을 최대로 설정한다.
			// 지형 시야거리 ( 1lv : 200, 2lv : 300, 3lv : 400, 4lv : 512 )
			if( GetNtlPLOptionManager()->GetTerrainFar() != dDIRECT_TERRAIN_FAR )
				GetNtlPLOptionManager()->SetTerrainFar( dDIRECT_TERRAIN_FAR );
			
			// 사물 시야거리 : 200, 300, 400, 512 )
			if( GetNtlPLOptionManager()->GetObjectFar() != dDIRECT_OBJECT_FAR )
				GetNtlPLOptionManager()->SetObjectFar( dDIRECT_OBJECT_FAR );

			break;
		}
	case WORLD_STATE_EXIT:
		{
			CNtlWorldConceptController::ChangeState(WORLD_PLAY_NONE);

			// 옵션 복원
			// 지형 시야거리 ( 1lv : 200, 2lv : 300, 3lv : 400, 4lv : 512 )
			RwInt32 nTerrainRangeLevel = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_TERRAIN_RANGE );
			RwReal fTerranRange = 512.f;
			switch( nTerrainRangeLevel )
			{
			case 0: fTerranRange = 200.f; break;
			case 1: fTerranRange = 300.f; break;
			case 2: fTerranRange = 400.f; break;
			case 3: fTerranRange = 512.f; break;
			default: fTerranRange = 512.f; break;
			}
			if( GetNtlPLOptionManager()->GetTerrainFar() != fTerranRange )
				GetNtlPLOptionManager()->SetTerrainFar( fTerranRange );

			// 사물 시야거리 : 200, 300, 400, 512 )
			RwInt32 nObjectRangeLevel = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_OBJECT_RANGE );
			RwReal fObjectRange = 512.f;
			switch( nObjectRangeLevel )
			{
			case 0: fObjectRange = 200.f; break;
			case 1: fObjectRange = 300.f; break;
			case 2: fObjectRange = 400.f; break;
			case 3: fObjectRange = 512.f; break;
			default: fObjectRange = 512.f; break;
			}

			if( GetNtlPLOptionManager()->GetObjectFar() != fObjectRange )
				GetNtlPLOptionManager()->SetObjectFar( fObjectRange );

            CNtlPLGlobal::m_bEntityNotCulling = FALSE;

			break;
		}
	}
}