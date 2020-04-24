#include "precomp_ntlsimulation.h"
#include "DboTSDefine.h"
#include "NtlSobManager.h"
#include "NtlSob.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobMonsterAttr.h"
#include "MobTable.h"


RwBool ConvertSkillUseTargetHandleToInfo( SERIAL_HANDLE hTarget, SSkillUseTargetInfo& sTargetInfo )
{
	CNtlSob* pObj = GetNtlSobManager()->GetSobObject( hTarget );
	if ( pObj )
	{
		sTargetInfo.uiClassID = pObj->GetClassID();

		switch ( sTargetInfo.uiClassID )
		{
		case SLCLASS_NPC:
			{
				sTargetInfo.tblIdx = ((CNtlSobNpcAttr*)pObj->GetSobAttr())->GetNpcTbl()->tblidx;
			}
			return TRUE;

		case SLCLASS_MONSTER:
			{
				sTargetInfo.tblIdx = ((CNtlSobMonsterAttr*)pObj->GetSobAttr())->GetMobTbl()->tblidx;
			}
			return TRUE;

		default:
			{
				sTargetInfo.uiClassID = INVALID_SLCLASS_ID;
				sTargetInfo.tblIdx = 0xffffffff;
			}
			return FALSE;
		}
	}
	else
	{
		sTargetInfo.uiClassID = INVALID_SLCLASS_ID;
		sTargetInfo.tblIdx = 0xffffffff;
		return FALSE;
	}

	return FALSE;
}

RwBool ConvertItemUseTargetHandleToInfo( SERIAL_HANDLE hTarget, SItemUseTargetInfo& sTargetInfo )
{
	CNtlSob* pObj = GetNtlSobManager()->GetSobObject( hTarget );
	if ( pObj )
	{
		sTargetInfo.uiClassID = pObj->GetClassID();

		switch ( sTargetInfo.uiClassID )
		{
		case SLCLASS_NPC:
			{
				sTargetInfo.tblIdx = ((CNtlSobNpcAttr*)pObj->GetSobAttr())->GetNpcTbl()->tblidx;
			}
			return TRUE;

		case SLCLASS_MONSTER:
			{
				sTargetInfo.tblIdx = ((CNtlSobMonsterAttr*)pObj->GetSobAttr())->GetMobTbl()->tblidx;
			}
			return TRUE;

		default:
			{
				sTargetInfo.uiClassID = INVALID_SLCLASS_ID;
				sTargetInfo.tblIdx = 0xffffffff;
			}
			return FALSE;
		}
	}
	else
	{
		sTargetInfo.uiClassID = INVALID_SLCLASS_ID;
		sTargetInfo.tblIdx = 0xffffffff;
		return FALSE;
	}

	return FALSE;
}


#ifdef __OUT_QUEST_TRACE__
CDummyConsole g_clConsole;
#endif
