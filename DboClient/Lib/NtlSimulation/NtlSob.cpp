#include "precomp_ntlsimulation.h"
#include "NtlSob.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEntity.h"

// simulation
#include "NtlSobAttr.h"
#include "NtlSobProxy.h"
#include "NtlSobAttrFactory.h"
#include "NtlSobElapsedController.h"
#include "NtlSLLuaGlue.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"

// CNtlSLFactory* CNtlSLFactory::m_pFactory[MAX_SLCLASS] = {NULL, };

CNtlSob::CNtlSob() 
{ 
	m_uiClassID = INVALID_SLCLASS_ID;
	m_uiFlags = 0;
	m_strName[0] = 0;

	m_hSerialID = INVALID_SERIAL_ID; 
	m_hOwnerID = INVALID_SERIAL_ID;
	m_hTargetID = INVALID_SERIAL_ID;
	m_hVehicleID = INVALID_SERIAL_ID;
	m_hLastAttackerID = INVALID_SERIAL_ID;
	m_uiVisualLuaID = 0;

	m_pSobParent = NULL;
	m_pSobAttr = NULL;
	m_pSobProxy = NULL;

	m_fWeightElapsedTime = 1.0f;

	m_bEventHook = TRUE;

	CNtlMath::MathRwV3dAssign(&m_vPos, 0.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&m_vDirection, 0.0f, 0.0f, 0.0f);
}

CNtlSob::~CNtlSob()
{
	if(m_pSobParent)
	{
		m_pSobParent->RemoveSob(this);
		m_pSobParent = NULL;
	}
}
/*

CNtlSob* CNtlSob::CreateSobFactory(RwUInt32 uiClassId)
{
	NTL_FUNCTION("CNtlSob::CreateSobFactory");

	CNtlSLFactory *pFactory = CNtlSLFactory::m_pFactory[uiClassId];

	if(pFactory)
	{
		NTL_RETURN(pFactory->CreateFactory()); 
	}

	NTL_RETURN(NULL);
}

void CNtlSob::DeleteSobFactory(CNtlSob *pSobObj)
{
	NTL_FUNCTION("CNtlSob::DeleteSobFactory");

	if(pSobObj)
	{
		delete pSobObj;
	}

	NTL_RETURNVOID();
}
*/

RwBool CNtlSob::Create(void) 
{
	m_pSobAttr = CNtlSobAttrFactory::CreateSobAttrFactory(m_uiClassID);
	if(m_pSobAttr)
	{
		m_pSobAttr->SetClassID(m_uiClassID);
	}

	return TRUE; 
}

void CNtlSob::PostCreate(void)
{
	if(GetFlags() & SLFLAG_LUA_INTERLOCKING)
		LuaExec_SobCreate(this);
}

void CNtlSob::Destroy(void)
{
	if(GetFlags() & SLFLAG_LUA_INTERLOCKING)
		LuaExec_SobDestroy(this);

	if(INVALID_SERIAL_ID != m_hVehicleID)
	{
		if ( GetNtlSobManager()->GetSobObject( m_hVehicleID ) )
		{
			CNtlSLEventGenerator::SobDelete( m_hVehicleID );
		}
		m_hVehicleID = INVALID_SERIAL_ID;
	}

	if(m_pSobAttr)
	{
		CNtlSobAttrFactory::DeleteSobAttrFactory(m_pSobAttr);
		m_pSobAttr = NULL;
	}

	if(m_pSobProxy)
	{
		m_pSobProxy->Destroy();

		NTL_DELETE( m_pSobProxy );
	}
}

void CNtlSob::ServerChangeOut(void)
{
	m_hOwnerID = INVALID_SERIAL_ID;
	m_hTargetID = INVALID_SERIAL_ID;
}

void CNtlSob::ServerChangeIn(void)
{
}

void CNtlSob::Update(RwReal fElapsed)
{
	if(m_pSobProxy)
		m_pSobProxy->Update(fElapsed);	
}

void CNtlSob::SetPosition(const RwV3d *pPos)
{
	RwV3dAssignMacro(&m_vPos, pPos);

	if(m_pSobProxy)
		m_pSobProxy->SetPosition(pPos);
}

void CNtlSob::SetDirection(const RwV3d *pDir)
{
	if((RwReal)fabs(pDir->x) <= 0.01f && (RwReal)fabs(pDir->y) <= 0.01f && (RwReal)fabs(pDir->z) <= 0.01f)
		return;

	RwV3dNormalize(&m_vDirection, pDir); 
	
	if(m_pSobProxy)
		m_pSobProxy->SetDirection(&m_vDirection);
}

void CNtlSob::SetAngleY(RwReal fAngle)
{
	if(m_pSobProxy)
		m_pSobProxy->SetAngleY(fAngle);
}

void CNtlSob::SetDeltaAngleY(RwReal fAngle)
{
	if(m_pSobProxy)
		m_pSobProxy->SetDeltaAngleY(fAngle);
}

void CNtlSob::SetAngleX(RwReal fAngle)
{
	if (m_pSobProxy)
		m_pSobProxy->SetAngleX(fAngle);
}

void CNtlSob::SetDeltaAngleX(RwReal fAngle)
{
	if (m_pSobProxy)
		m_pSobProxy->SetDeltaAngleX(fAngle);
}

void CNtlSob::SetWeightElapsedTime(RwReal fWeightValue)
{
	m_fWeightElapsedTime = fWeightValue;
}

void CNtlSob::SetEventHook(RwBool bEventHook)
{
	m_bEventHook = bEventHook;
}

void CNtlSob::EnableInput(RwBool bEnable)
{
	if(bEnable)
		m_uiFlags = m_uiFlags & ~SLFLAG_NOT_INPUT;
	else
		m_uiFlags = m_uiFlags | SLFLAG_NOT_INPUT;
}

void CNtlSob::EnableVisible(RwBool bEnable)
{
	if(IsVisible())
	{
		if(!bEnable)
		{
			CNtlSLEventGenerator::SobShowNfy(GetSerialID(), FALSE);
		}
	}
	else
	{
		if(bEnable)
		{
			CNtlSLEventGenerator::SobShowNfy(GetSerialID(), TRUE);
		}
	}
	

    if(bEnable)
        m_uiFlags = m_uiFlags & ~SLFLAG_NOT_VISIBLE;
    else
        m_uiFlags = m_uiFlags | SLFLAG_NOT_VISIBLE;

    if(m_pSobProxy)
        m_pSobProxy->EnableVisible(bEnable);
}

void CNtlSob::EnableWeightElapsedTime(RwBool bEnable)
{
	if(bEnable)
		m_uiFlags = m_uiFlags | SLFLAG_WEIGHT_ELAPSED_TIME;
	else
		m_uiFlags = m_uiFlags & ~SLFLAG_WEIGHT_ELAPSED_TIME;
}


RwBool CNtlSob::IsInput(void) const
{
	if(m_uiFlags & SLFLAG_NOT_INPUT)
		return FALSE;

	return TRUE;
}

RwBool CNtlSob::IsVisible(void) const
{
	if(m_uiFlags & SLFLAG_NOT_VISIBLE)
		return FALSE;

	return TRUE;
}

RwBool CNtlSob::IsWeightElapsedTime(void) const
{
	if(m_uiFlags & SLFLAG_WEIGHT_ELAPSED_TIME)
		return TRUE;

	return FALSE;
}

void CNtlSob::Dump(CNtlDumpTarget& dump)
{
	dump << "simulation class id : " << m_uiClassID << "\n";
	dump.Dump();
	if(strlen(m_strName) > 0)
	{
		dump << "simulation class name : " << m_strName << "\n";
		dump.Dump();
	}
	dump << "simulation serial id : " << m_hSerialID << "\n";
	dump.Dump(); 
}


