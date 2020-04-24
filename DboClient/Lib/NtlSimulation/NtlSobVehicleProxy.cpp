#include "precomp_ntlsimulation.h"
#include "NtlSobVehicleProxy.h"

// shared
#include "TableContainer.h"
#include "VehicleTable.h"

// presentation
#include "NtlDefaultItemData.h"
#include "NtlPLCharacter.h"
#include "NtlPLSceneManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlSob.h"


DEFINITION_MEMORY_POOL( CNtlSobVehicleProxy )


CNtlSobVehicleProxy::CNtlSobVehicleProxy()
{
	m_pPLVehicle = NULL;
}

CNtlSobVehicleProxy::~CNtlSobVehicleProxy()
{
	DeleteVehicleEntity( m_pPLVehicle );
}

RwBool CNtlSobVehicleProxy::Create(RwUInt32 uiCompType)
{
	CNtlSobProxy::Create( uiCompType );

	return TRUE;
}

void CNtlSobVehicleProxy::Destroy(void)
{
	CNtlSobProxy::Destroy();
}

void CNtlSobVehicleProxy::Update(RwReal fElapsed)
{
	CNtlSobProxy::Update( fElapsed );
}

void CNtlSobVehicleProxy::HandleEvents(RWS::CMsg &pMsg)
{
	if ( pMsg.Id == g_EventSobCreate )
	{
		SNtlEventSobVehicleCreate* pSobCreate = reinterpret_cast<SNtlEventSobVehicleCreate*>(pMsg.pData);

		m_pPLVehicle = CreateVehicleEntity( pSobCreate->vLoc, pSobCreate->vDir, pSobCreate->uiVehicleTblIdx, pSobCreate->bForChild );
	}
}

void CNtlSobVehicleProxy::SetPosition( const RwV3d* pPos )
{
	if ( m_pPLVehicle )
	{
		m_pPLVehicle->SetPosition( pPos );
	}
}

void CNtlSobVehicleProxy::SetDirection( const RwV3d* pDir )
{
	if ( m_pPLVehicle )
	{
		RwReal fXAngle, fYAngle;
		CNtlMath::LineToAngleXY( pDir, fXAngle, fYAngle );
		m_pPLVehicle->SetAngleX( fXAngle );
		m_pPLVehicle->SetAngleY( fYAngle );
	}
}

RwV3d CNtlSobVehicleProxy::GetDirection( void )
{
	if ( m_pPLVehicle )
	{
		return m_pPLVehicle->GetDirection();
	}
	else
	{
		return CNtlSobProxy::GetDirection();
	}
}

void CNtlSobVehicleProxy::SetAngleY( RwReal fAngle )
{
	if ( m_pPLVehicle )
	{
		m_pPLVehicle->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

void CNtlSobVehicleProxy::SetBaseAnimation( RwUInt32 uiAnimKey, RwBool bLoop /*= TRUE*/, RwReal fStartTime /*= 0.0f*/ )
{
	if ( m_pPLVehicle )
	{
		m_pPLVehicle->SetBaseAnimation( uiAnimKey, fStartTime, bLoop );
	}
}

RwBool CNtlSobVehicleProxy::IsExistBaseAnimation( RwUInt32 uiAnimKey )
{
	if ( NULL == m_pPLVehicle )
	{
		return FALSE;
	}

	return m_pPLVehicle->IsExistAnim(uiAnimKey); 
}

RwUInt32 CNtlSobVehicleProxy::GetBaseAnimationKey( void )
{
	if ( NULL == m_pPLVehicle )
	{
		return FALSE;
	}

	return m_pPLVehicle->GetCurBaseAnimKey(); 
}

RwBool CNtlSobVehicleProxy::IsBaseAnimationEnd( void )
{
	if ( NULL == m_pPLVehicle )
	{
		return FALSE;
	}

	return m_pPLVehicle->GetBaseCurrentAnimEnd(); 
}

void CNtlSobVehicleProxy::SetAnimSpeed( RwReal fAniSpeed )
{
	if ( NULL == m_pPLVehicle )
	{
		return;
	}

	m_pPLVehicle->SetAnimSpeed( fAniSpeed ); 
}

RwReal CNtlSobVehicleProxy::GetAnimSpeed( void )
{
	if ( NULL == m_pPLVehicle )
	{
		return 1.f;
	}

	return m_pPLVehicle->GetAnimSpeed(); 
}

RwReal CNtlSobVehicleProxy::GetPLEntityHeight( void )
{
	return m_pPLVehicle->GetHeight();
}

RwReal CNtlSobVehicleProxy::GetPLEntityWidth( void )
{
	return m_pPLVehicle->GetWidth();
}

RwReal CNtlSobVehicleProxy::GetPLEntityDepth( void )
{
	return m_pPLVehicle->GetDepth();
}

RwReal CNtlSobVehicleProxy::GetPLEntityBaseScale( void )
{
	return m_pPLVehicle->GetBaseScale();
}

RwReal CNtlSobVehicleProxy::GetScale( void )
{
	return m_pPLVehicle->GetScale();
}

void CNtlSobVehicleProxy::EnableVisible( RwBool bEnable )
{
	if ( m_pPLVehicle )
	{
		m_pPLVehicle->SetVisible( bEnable );
	}
}

CNtlPLCharacter* CNtlSobVehicleProxy::CreateVehicleEntity(const RwV3d& vLoc, const RwV3d& vDir, RwUInt32 uiVehicleTblIdx, RwBool bForChild)
{
	CNtlPLCharacter* pPLVehicle = NULL;

	sVEHICLE_TBLDAT* pVehicleTbl = (sVEHICLE_TBLDAT*)API_GetTableContainer()->GetVehicleTable()->FindData( uiVehicleTblIdx );
	if ( NULL == pVehicleTbl )
	{
		return pPLVehicle;
	}

	RwBool bPcFlag = FALSE;
	RwUInt8 byRace = RACE_HUMAN;
	RwUInt8 byClass = PC_CLASS_HUMAN_FIGHTER;
	RwUInt8 byGender = GENDER_MALE;
	RwUInt8 byHair = DEF_ITEM_START;
	RwUInt8 byFace = DEF_ITEM_START;
	RwUInt8 byHairColor = DEF_ITEM_START;
	RwUInt8 bySkinColor = DEF_ITEM_START;

	SPLCharacterCreateParam sParam;

	sParam.pPos			= &vLoc;
	sParam.uiSerialId	= m_pSobObj->GetSerialID();

	sParam.bPcFlag		= bPcFlag;
	sParam.uiRace		= byRace;
	sParam.uiClass		= byClass;
	sParam.uiGender		= byGender;
	sParam.uiHeadType	= byHair;
	sParam.uiFaceType	= byFace;
	sParam.uiHeadColorType = byHairColor;
	sParam.uiSkinColorType = bySkinColor;
	sParam.bNotShadingFlag = FALSE;

	char szVehicleModelName[1024];

	if (pVehicleTbl->bySRPType)
	{
		if (bForChild)
		{
			sprintf_s(szVehicleModelName, 1024, "%s_CH_SRP%d", pVehicleTbl->szModelName, pVehicleTbl->bySRPType);
		}
		else
		{
			sprintf_s(szVehicleModelName, 1024, "%s_SRP%d", pVehicleTbl->szModelName, pVehicleTbl->bySRPType);
		}
	}
	else
	{
		sprintf_s(szVehicleModelName, 1024, "%s", pVehicleTbl->szModelName);
	}

	// lleo52 추후 수정 필요
	int nStrLen = strlen( szVehicleModelName );
	for ( int i = 0; i < nStrLen; ++i )
	{
		if ( ::isalpha( szVehicleModelName[i] ) && ::islower( szVehicleModelName[i] ) )
		{
			szVehicleModelName[i] = (char)::toupper( szVehicleModelName[i] );
		}
	}

	pPLVehicle = (CNtlPLCharacter*)(GetSceneManager()->CreateEntity( PLENTITY_CHARACTER, szVehicleModelName, &sParam ));

	NTL_ASSERT( pPLVehicle, "Creating the PLEntity of a vehicle is failed  : " << pVehicleTbl->szModelName );

	pPLVehicle->SetSerialID( m_pSobObj->GetSerialID() );

	pPLVehicle->SetDirection( &vDir );

	pPLVehicle->SetWeightElapsedTime( 0.f );

	pPLVehicle->SetFlags( NTL_PLEFLAG_WEIGHT_ELAPSED_TIME );

	return pPLVehicle;
}

void CNtlSobVehicleProxy::DeleteVehicleEntity(CNtlPLCharacter*& pPLVehicle)
{
	if ( pPLVehicle )
	{
		GetSceneManager()->DeleteEntity( pPLVehicle );
		pPLVehicle = NULL;
	}
}
