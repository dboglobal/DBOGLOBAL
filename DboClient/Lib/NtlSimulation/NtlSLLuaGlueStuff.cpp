#include "precomp_ntlsimulation.h"
#include "NtlSLLuaGlueStuff.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlCoreUtil.h"

// presentation
#include "NtlPLEntity.h"

// simulation 
#include "NtlSobManager.h"
#include "NtlSobActor.h"
#include "NtlFSMStateBase.h"
#include "NtlBehaviorBase.h"
#include "NtlSobProxy.h"


CNtlLuaGlueStack* CNtlLuaGlueStack::m_pInstance = NULL;

CNtlLuaGlueStack g_LuaGlueStack;

CNtlLuaGlueStack::CNtlLuaGlueStack()
{
	NTL_ASSERT(m_pInstance == NULL, "CNtlLuaGlueStack class created multiple instance");
	m_pInstance = this;

	Reset();
}

CNtlLuaGlueStack::~CNtlLuaGlueStack()
{
	m_pInstance = NULL;
}

CNtlLuaGlueStack* CNtlLuaGlueStack::GetInstance(void)
{
	return m_pInstance;
}


void CNtlLuaGlueStack::ResetTransform(void)
{
	m_TM.byTransMask = 0;
	CNtlMath::MathRwV3dAssign(&m_TM.vPos, 0.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&m_TM.vDir, 1.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&m_TM.vAddAxisValue, 0.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&m_TM.vScale, 1.0f, 1.0f, 1.0f);
}

void CNtlLuaGlueStack::Reset(void)
{
	m_pActor = NULL;
	m_pPLEntity = NULL;
	m_eEditType = LUA_STACK_EDIT_ACTOR;
	m_sActiveObj.eType = LUA_STACK_ACTIVEOBJ_NONE;
	m_sActiveObj.uObj.pObj = NULL;
	m_sHit.Reset();

	ResetTransform();
}

void CNtlLuaGlueStack::BeginTransform(void)
{
	ResetTransform();
}

void CNtlLuaGlueStack::EndTransform(void)
{
	// 방향 최종 결정.
	RwV3d vRight, vUp;
	RwV3dCrossProduct(&vRight, &m_TM.vDir, &CNtlPLGlobal::m_vYAxisV3);  
	RwV3dCrossProduct(&vUp, &m_TM.vDir, &vRight);

	RwV3d vAddDir;
	CNtlMath::MathRwV3dAssign(&vAddDir, 0.0f, 0.0f, 0.0f); 
	CNtlMath::MathRwV3dAddMul(&vAddDir, &vRight, m_TM.vAddAxisValue.x);
	CNtlMath::MathRwV3dAddMul(&vAddDir, &vUp, m_TM.vAddAxisValue.y);
	CNtlMath::MathRwV3dAddMul(&vAddDir, &m_TM.vDir, m_TM.vAddAxisValue.z);
	RwV3dAdd(&m_TM.vDir, &m_TM.vDir, &vAddDir);
	RwV3dNormalize(&m_TM.vDir, &m_TM.vDir);

	if(m_eEditType == LUA_STACK_EDIT_ACTOR)
	{
		if(m_pActor == NULL)
		{
			NtlLogFilePrint("edit actor not setting!!! used CNtlLuaGlueStack::EndTransform function");
            return;
		}
		
		if(m_pActor->GetFlags() & SLFLAG_LUA_TRANSFORM_ENABLE)
		{
			if(m_TM.byTransMask & LUA_STACK_ACTIVETRANS_POS)
				m_pActor->SetPosition(&m_TM.vPos);
			if(m_TM.byTransMask & LUA_STACK_ACTIVETRANS_DIR)
				m_pActor->SetDirection(&m_TM.vDir);
		}
	}
	else
	{
		if(m_pPLEntity == NULL)
		{
			NtlLogFilePrint("edit presentation object not setting!!! used CNtlLuaGlueStack::EndTransform function");
		}
		else
		{
			RwMatrix mat;
			RwMatrixSetIdentity(&mat);

			RwV3d vPos = m_TM.vPos;
			if( !(m_TM.byTransMask & LUA_STACK_ACTIVETRANS_POS) )
			{
				if(m_pActor)
					vPos = m_pActor->GetPosition();
			}

			RwV3d vDir = m_TM.vDir;
			if( !(m_TM.byTransMask & LUA_STACK_ACTIVETRANS_DIR) )
			{
				if(m_pActor)
					vDir = m_pActor->GetDirection();
			}
			
			RwV3dCrossProduct(&vRight, &vDir, &CNtlPLGlobal::m_vYAxisV3);  
			RwV3dCrossProduct(&vUp, &vDir, &vRight);

			RwV3dAssignMacro(&mat.up, &vUp);
			RwV3dAssignMacro(&mat.right, &vRight);
			RwV3dAssignMacro(&mat.at, &vDir);

			if( (m_TM.byTransMask & LUA_STACK_ACTIVETRANS_SCALE) )
				RwMatrixScale(&mat, &m_TM.vScale, rwCOMBINEPOSTCONCAT);

			RwMatrixTranslate(&mat, &vPos, rwCOMBINEPOSTCONCAT);
			m_pPLEntity->SetMatrix(mat); 
		}
	}
}

void CNtlLuaGlueStack::SetPosition(RwReal fPosX, RwReal fPosY, RwReal fPosZ)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_POS;
	CNtlMath::MathRwV3dAssign(&m_TM.vPos, fPosX, fPosY, fPosZ);
}

void CNtlLuaGlueStack::AddPosition(RwReal fPosX, RwReal fPosY, RwReal fPosZ)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_POS;

	m_TM.vPos.x += fPosX;
	m_TM.vPos.y += fPosY;
	m_TM.vPos.z += fPosZ;
}

void CNtlLuaGlueStack::SetScale(RwReal fScaleX, RwReal fScaleY, RwReal fScaleZ)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_SCALE;
	CNtlMath::MathRwV3dAssign(&m_TM.vScale, fScaleX, fScaleY, fScaleZ);
}

void CNtlLuaGlueStack::SetDirection(RwReal fDirX, RwReal fDirY, RwReal fDirZ)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_DIR;
	CNtlMath::MathRwV3dAssign(&m_TM.vDir, fDirX, fDirY, fDirZ);
}

void CNtlLuaGlueStack::AddDirectionAxisX(RwReal fMinValue, RwReal fMaxValue)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_DIR;
	m_TM.vAddAxisValue.x = NtlRandomNumber(fMinValue, fMaxValue); 
}

void CNtlLuaGlueStack::AddDirectionAxisY(RwReal fMinValue, RwReal fMaxValue)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_DIR;
	m_TM.vAddAxisValue.y = NtlRandomNumber(fMinValue, fMaxValue);
}

void CNtlLuaGlueStack::AddDirectionAxisZ(RwReal fMinValue, RwReal fMaxValue)
{
	m_TM.byTransMask |= LUA_STACK_ACTIVETRANS_DIR;
	m_TM.vAddAxisValue.z = NtlRandomNumber(fMinValue, fMaxValue);
}

void CNtlLuaGlueStack::SetActiveObj(ELuaGlueActiveObjType eType, const void *pObj)
{
	m_sActiveObj.eType = eType;
	m_sActiveObj.uObj.pObj = const_cast<void*>(pObj);
}

RwBool CNtlLuaGlueStack::AddActivePLEntity(const CNtlPLEntity *pPLEntity)
{
	if(m_sActiveObj.uObj.pObj == NULL)
		return FALSE;

	if(m_sActiveObj.eType == LUA_STACK_ACTIVEOBJ_SOB)
	{
		CNtlSobProxy *pSobProxy = m_sActiveObj.uObj.pSobObj->GetSobProxy();
		if(pSobProxy)
			pSobProxy->AddPLChildEntity(const_cast<CNtlPLEntity*>(pPLEntity));
			
		return TRUE;
	}
	else if(m_sActiveObj.eType == LUA_STACK_ACTIVEOBJ_STATE)
	{
		m_sActiveObj.uObj.pState->AddPLEntity(const_cast<CNtlPLEntity*>(pPLEntity));
		return TRUE;
	}
	else if(m_sActiveObj.eType == LUA_STACK_ACTIVEOBJ_BEHAVIOR)
	{
		m_sActiveObj.uObj.pBehavior->AddPLEntity(const_cast<CNtlPLEntity*>(pPLEntity));
		return TRUE;
	}

	return FALSE;
}


void CNtlLuaGlueStack::SetHit(const SHitStuff *pHit)
{
	memcpy(&m_sHit, pHit, sizeof(SHitStuff));
}
