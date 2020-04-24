#include "precomp_ntlsimulation.h"
#include "NtlSobDynamicObjectProxy.h"

// shared
#include "DynamicObjectTable.h"
#include "NtlObject.h"

// core
#include "NtlMath.h"

//// presentation
#include "NtlPLEvent.h"
#include "NtlPLEntity.h"
#include "NtlPLSceneManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLHelpers.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSob.h"
#include "NtlSobDynamicObject.h"
#include "NtlSobDynamicObjectAttr.h"
#include "ItemMixMachineTable.h"
#include "NtlSLLogicDef.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "TableContainer.h"
#include "NtlSLApi.h"
#include "DynamicObjectTable.h"
#include "NtlShareTargetMark.h"

DEFINITION_MEMORY_POOL( CNtlSobDynamicObjectProxy )

CNtlSobDynamicObjectProxy::CNtlSobDynamicObjectProxy( void )
{
	m_pPLDynamicObject		= NULL;
	m_pPLDynamicObjectName	= NULL;
	m_pTargetMark			= NULL;
    m_pShareTargetMark      = NULL;

	m_fObjectWidth			= 1.0f;
	m_fObjectHeight			= 1.0f;
	m_fObjectDepth			= 1.0f;

	m_byMainState			= eDYNAMIC_OBJECT_STATE_SPAWN;
}

CNtlSobDynamicObjectProxy::~CNtlSobDynamicObjectProxy( void )
{
	DestroyPLObjectName();
	DestroyPLTargetMark();
	DestroyDynamicObject();

    NTL_DELETE(m_pShareTargetMark);
}

RwBool CNtlSobDynamicObjectProxy::Create( RwUInt32 uiCompType )
{
	CNtlSobProxy::Create( uiCompType );

	return TRUE;
}

void CNtlSobDynamicObjectProxy::Destroy( void )
{
	CNtlSobProxy::Destroy();
}

void CNtlSobDynamicObjectProxy::Update( RwReal fElapsed )
{
	if( m_pPLDynamicObjectName )
	{
		const RwBBox *pBox = m_pPLDynamicObject->GetBoundingBox();
		RwV3d vPos = m_pPLDynamicObject->GetPosition();
		vPos.y += (pBox->sup.y - pBox->inf.y) + NTL_NAMEBOX_OFFSET;

		m_pPLDynamicObjectName->SetPosition( &vPos );
	}
}

void CNtlSobDynamicObjectProxy::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobCreate )
	{
		SNtlEventSobDynamicObjectCreate* pSobCreate = reinterpret_cast< SNtlEventSobDynamicObjectCreate* > (msg.pData);

		CDynamicObjectTable* pDynamicObjectTbl = API_GetTableContainer()->GetDynamicObjectTable();
		NTL_ASSERT(pDynamicObjectTbl, "CNtlSobDynamicObjectProxy::HandleEvents => Dynamic ObjectTable is null" );

		// Index 가져오기
		TBLIDX idxDynamicObject = INVALID_TBLIDX;
		switch( pSobCreate->byType )
		{
		case eDBO_DYNAMIC_OBJECT_TYPE_HOIPOI_MIX_MACHINE:
			{
				CItemMixMachineTable* pMachineTable = API_GetTableContainer()->GetItemMixMachineTable();
				NTL_ASSERT( pMachineTable, "ItemMixMachine Table is null");

				sITEM_MIX_MACHINE_TBLDAT* pTblData = (sITEM_MIX_MACHINE_TBLDAT*)pMachineTable->FindData( pSobCreate->uiTableIndexForType );
				NTL_ASSERT( pTblData, "ItemMixMachine Table index is invalid." << pSobCreate->uiTableIndexForType );

				idxDynamicObject = pTblData->dynamicObjectTblidx;
			}
			break;
		default:
			{
				NTL_ASSERTFAIL( "Invalid dynamic object type - " << pSobCreate->byType );
			}
			break;
		}

		if( idxDynamicObject == INVALID_TBLIDX )
		{
			NTL_ASSERTFAIL( "Dynamic Object Create Failed - Table data is invalid" );
			return;
		}

		m_pPLDynamicObject = CreateDynamicObject( pSobCreate->vLoc, idxDynamicObject);
		m_pPLDynamicObject->SetSerialID( m_pSobObj->GetSerialID() );

		const RwBBox* pBox = m_pPLDynamicObject->GetBoundingBox();
		if( pBox )
		{
			m_fObjectWidth	= pBox->sup.x - pBox->inf.x;
			m_fObjectHeight	= pBox->sup.y - pBox->inf.y;
			m_fObjectDepth	= pBox->sup.z - pBox->inf.z;
		}

		UpdateState( pSobCreate->byState );

		//// 이름 생성
		CreatePLObjectName();
	}
	else if( msg.Id == g_EventSobTargetSelect )
	{
		SNtlEventSobTargetSelect* pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>(msg.pData);
		if( m_pSobObj->GetSerialID() == pSobTargetSelect->hSerialId )
		{
			CreatePLTargetMark();
		}
	}
	else if( msg.Id == g_EventSobTargetSelectRelease )
	{
		DestroyPLTargetMark();
	}
	else if( msg.Id == g_EventSobGotFocus )
	{
		m_pPLDynamicObject->SetAddColor( 40, 40, 40 );
	}
	else if( msg.Id == g_EventSobLostFocus )
	{
		m_pPLDynamicObject->SetAddColor( 0, 0, 0 );
	}
	else if( msg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*)msg.pData;

		if( pData->uiSerialId == m_pSobObj->GetSerialID() )
		{
			// Spawn 이후 애니메이션 상태가 업데이트 되지 않을 때
			sDYNAMIC_OBJECT_TBLDAT* pDynamicTbl = ((CNtlSobDynamicObjectAttr*)m_pSobObj->GetSobAttr())->GetDynamicObjectTbl();

			if ( pData->uiBaseAnimKey == pDynamicTbl->spawnAnimation )
				m_pPLDynamicObject->SetTriggerAnimation( pDynamicTbl->idleAnimation, 0.0f, TRUE );
		}
	}
	else if( msg.Id == g_EventDynamicObjectUpdateState )
	{
		SNtlEventDynamicObjectUpdateState* pState = reinterpret_cast<SNtlEventDynamicObjectUpdateState*>(msg.pData);

		UpdateState( pState->pState->byState );
	}
    else if(msg.Id == g_EventShareTargetSelect)
    {
        SobShareTargetSelectHandler(msg);
    }
    else if(msg.Id == g_EventShareTargetRelease)
    {
        SobShareTargetReleaseHandler(msg);
    }
}

void CNtlSobDynamicObjectProxy::SetPosition( const RwV3d* pPos )
{
	if( m_pPLDynamicObject )
	{
		m_pPLDynamicObject->SetPosition( pPos );
	}
}

void CNtlSobDynamicObjectProxy::SetDirection( const RwV3d* pDir )
{
	if( m_pPLDynamicObject )
	{
		RwReal fAngle = CNtlMath::LineToAngleY( pDir );
		m_pPLDynamicObject->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

RwV3d CNtlSobDynamicObjectProxy::GetDirection( void )
{
	if( m_pPLDynamicObject )
	{
		return m_pPLDynamicObject->GetDirection();
	}

	return CNtlSobProxy::GetDirection();
}

void CNtlSobDynamicObjectProxy::SetAngleY( RwReal fAngle )
{
	if( m_pPLDynamicObject )
	{
		m_pPLDynamicObject->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

RwReal CNtlSobDynamicObjectProxy::GetPLEntityWidth( void )
{
	return m_fObjectWidth;
}

RwReal CNtlSobDynamicObjectProxy::GetPLEntityHeight( void )
{
	return m_fObjectHeight;
}

RwReal CNtlSobDynamicObjectProxy::GetPLEntityDepth( void )
{
	return m_fObjectDepth;
}

void CNtlSobDynamicObjectProxy::EnableNameVisible( RwBool bVisible )
{
	if( m_pPLDynamicObjectName )
	{
		m_pPLDynamicObjectName->SetVisible( bVisible );
	}
}

void CNtlSobDynamicObjectProxy::SetNameColor( const WCHAR* pwcName, COLORREF color )
{
	if( m_pPLDynamicObjectName )
	{
		m_pPLDynamicObjectName->SetNameColor( pwcName, color );
	}
}

CNtlPLObject* CNtlSobDynamicObjectProxy::CreateDynamicObject( const RwV3d& vLoc, RwUInt32 uiDynamicObjectTblIdx )
{
	CNtlPLObject* pObject = NULL;

	sDYNAMIC_OBJECT_TBLDAT* pTblDat = (sDYNAMIC_OBJECT_TBLDAT*)API_GetTableContainer()->GetDynamicObjectTable()->FindData( uiDynamicObjectTblIdx );
	if( NULL == pTblDat )
	{
		return NULL;
	}

	SPLObjectCreateParam sParam;
	sParam.bLoadMap = false;
	sParam.pPos = &vLoc;

	// 스케쥴 로딩을 끈다.
	GetNtlResourceManager()->SetLoadScheduling(FALSE);

	// 프로퍼티가 없어서 임시로 하드코딩된 모델 데이타를 체크인한다.
	pObject = (CNtlPLObject*)(GetSceneManager()->CreateEntity( PLENTITY_OBJECT, pTblDat->szModelName, &sParam ));

	GetNtlResourceManager()->SetLoadScheduling(TRUE);

	NTL_ASSERT( pObject, "Creating the PLEntity of a dynamic object is failed  : " << pTblDat->szModelName );

	pObject->SetSerialID( m_pSobObj->GetSerialID() );

	return pObject;
}

void CNtlSobDynamicObjectProxy::DestroyDynamicObject()
{
	if( m_pPLDynamicObject )
	{
		GetSceneManager()->DeleteEntity( m_pPLDynamicObject );
		m_pPLDynamicObject = NULL;
	}
}

void CNtlSobDynamicObjectProxy::CreatePLObjectName( void )
{
	DestroyPLObjectName();

	if( !m_pSobObj )
		return;

	CNtlSobDynamicObjectAttr* pSobAttr = reinterpret_cast<CNtlSobDynamicObjectAttr*>(m_pSobObj->GetSobAttr());
	
	const RwBBox* pBox = m_pPLDynamicObject->GetBoundingBox();
	if( pBox == NULL )
		return;

	if( !pSobAttr->IsNameCreate() )
		return;

	const WCHAR* pName = pSobAttr->GetName();

	m_pPLDynamicObjectName = Helper_CreatePlayerName( NULL, NTL_OBJ_NAME_COLOR, pName );

	RwV3d vPos = m_pPLDynamicObject->GetPosition();
	vPos.y += (pBox->sup.y - pBox->inf.y) + NTL_NAMEBOX_OFFSET;

	m_pPLDynamicObjectName->SetPosition( &vPos );
}

void CNtlSobDynamicObjectProxy::DestroyPLObjectName( void )
{
	if( m_pPLDynamicObjectName )
	{
		GetSceneManager()->DeleteEntity( m_pPLDynamicObjectName );
		m_pPLDynamicObjectName = NULL;
	}
}

void CNtlSobDynamicObjectProxy::CreatePLTargetMark( void )
{
	DestroyPLTargetMark();

	// SobObj 가 존재하면 타겟마크를 생성해주고 아니라면 생성하지 않는다.

	CNtlSobDynamicObjectAttr* pAttrObj = dynamic_cast< CNtlSobDynamicObjectAttr* > ( m_pSobObj->GetSobAttr() );
	if ( NULL == pAttrObj ) return;

    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        // 공유 타겟 설정이 되어 있으면 일반 마크는 뜨지 않는다.
        CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_TARGET);
        return;
    }

	if ( NULL == m_pTargetMark )
	{
		const RwBBox *pBox = m_pPLDynamicObject->GetBoundingBox();
		if(pBox == NULL)
			return;

        RwV3d vPos = m_pPLDynamicObject->GetPosition();
        vPos.y += (pBox->sup.y - pBox->inf.y)+NTL_NAMEBOX_OFFSET;

		m_pTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_TARGET);                
		m_pTargetMark->SetPosition(&vPos);
	}
}

void CNtlSobDynamicObjectProxy::DestroyPLTargetMark( void )
{
    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        m_pShareTargetMark->CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_NORMAL);
    }	
    else if(m_pTargetMark)
    {
        GetSceneManager()->DeleteEntity(m_pTargetMark);
        m_pTargetMark = NULL;
    }
}

void CNtlSobDynamicObjectProxy::UpdateState( RwUInt8 byState )
{
	m_byMainState = byState;
	
	// State에 알맞은 Animation을 적용한다.
	sDYNAMIC_OBJECT_TBLDAT* pDynamicTbl = ((CNtlSobDynamicObjectAttr*)m_pSobObj->GetSobAttr())->GetDynamicObjectTbl();

	RwUInt32 uiAniKey = pDynamicTbl->idleAnimation;
	switch( byState )
	{
	case eDYNAMIC_OBJECT_STATE_SPAWN:
		uiAniKey = pDynamicTbl->spawnAnimation;
		m_pPLDynamicObject->SetTriggerAnimation( uiAniKey, 0.0f, FALSE );
		break;
	case eDYNAMIC_OBJECT_STATE_IDLE:
		uiAniKey = pDynamicTbl->idleAnimation;
		m_pPLDynamicObject->SetTriggerAnimation( uiAniKey, 0.0f, TRUE );
		break;
	case eDYNAMIC_OBJECT_STATE_DESPAWN:
		uiAniKey = pDynamicTbl->despawnAnimation;
		m_pPLDynamicObject->SetTriggerAnimation( uiAniKey, 0.0f, FALSE );
		break;
	default:
		break;
	}
}

void CNtlSobDynamicObjectProxy::CreateShareTargetMark( RwUInt8 bySlot, RwUInt32 type )
{
    if(!m_pShareTargetMark)
    {
        m_pShareTargetMark = NTL_NEW CNtlShareTargetMark(m_pSobObj);
    }

    m_pShareTargetMark->CreateShareTargetMark(bySlot, type);    
}

void CNtlSobDynamicObjectProxy::DeleteShareTargetMark()
{
    if(!m_pShareTargetMark)
        return;

    m_pShareTargetMark->DeleteShareTargetMark();
}

void CNtlSobDynamicObjectProxy::SobShareTargetSelectHandler( RWS::CMsg& pMsg )
{
    SNtlEventShareTargetSelect* pData = (SNtlEventShareTargetSelect*)pMsg.pData;
    if(pData->hSerialId == m_pSobObj->GetSerialID())
    {
        // 기존 타겟 마크를 삭제하기 전에 현재 상태를 저장해둔다        
        RwBool bTargetMode = m_pTargetMark ? TRUE : FALSE;

        DestroyPLTargetMark(); 

        if(bTargetMode)
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_TARGET);
        }
        else
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_NORMAL);
        }        
    }
}

void CNtlSobDynamicObjectProxy::SobShareTargetReleaseHandler( RWS::CMsg& pMsg )
{
    SNtlEventShareTargetRelease* pData = (SNtlEventShareTargetRelease*)pMsg.pData;
    if(pData->hSerialId == m_pSobObj->GetSerialID())
    {
        DeleteShareTargetMark();
    }
}