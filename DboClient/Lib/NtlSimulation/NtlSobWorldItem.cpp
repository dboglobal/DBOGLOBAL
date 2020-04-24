#include "precomp_ntlsimulation.h"
#include "NtlSobWorldItem.h"


// shared
#include "GraphicDataTable.h"

// core
#include "NtlMath.h"

// presentation
#include "NtlPLGlobal.h"
#include "NtlPLSceneManager.h"


// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAttr.h"
#include "NtlSobProxy.h"
#include "NtlSobWorldItemProxy.h"
#include "NtlSLLogic.h"
#include "NtlBehaviorData.h"
#include "NtlSobManager.h"
#include "NtlSLVisualDeclear.h"

#include "GUISoundDefine.h"

DEFINITION_MEMORY_POOL(CNtlSobWorldItem)

CNtlSobWorldItem::CNtlSobWorldItem()
: m_fTime ( 0.0f ), m_fDestTime( 0.0f ), m_pSeq( NULL ), m_eState( WI_NONE ), StateUpdate( NULL ), m_bVisualEff( FALSE ), m_bLootSend(FALSE)
{
	SetFlags(SLFLAG_TARGET_NEAR_APPROACH);
}

CNtlSobWorldItem::~CNtlSobWorldItem()
{
	
}

RwBool CNtlSobWorldItem::Create(void)
{
	NTL_FUNCTION("CNtlSobWorldItem::Create");

	if(!CNtlSobActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobWorldItemProxy;
	m_pSobProxy->Create( NTL_SLPROXY_COMP_EQUIP_ITEM );
	m_pSobProxy->SetSobObj(this);

	// class name 설정.
	SetClassName(SLCLASS_NAME_WORLD_ITEM);

	NTL_RETURN(TRUE);
}

void CNtlSobWorldItem::Destroy(void)
{
	NTL_FUNCTION("CNtlSobWorldItem::Destroy");

	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}


void CNtlSobWorldItem::Update(RwReal fElapsed)
{
	CNtlSobActor::Update(fElapsed);

	m_fTime += fElapsed;
	(this->*StateUpdate)();
}

CNtlFSMLayer* CNtlSobWorldItem::CreateFSM(void)
{
	return NULL;
}

CNtlSobInfluence* CNtlSobWorldItem::CreateSobInfluence(void)
{
	return NULL;
}

void CNtlSobWorldItem::CreateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobWorldItem::CreateEventHandler");

	NTL_RETURNVOID();
}

void CNtlSobWorldItem::DestroyEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobWorldItem::DestroyEventHandler");
	
	NTL_RETURNVOID();
}

void CNtlSobWorldItem::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobWorldItem::HandleEvents");

	if( pMsg.Id == g_EventSobCreate )
	{
		SNtlEventSobWorldItemCreate* pWorldItemCreate = reinterpret_cast<SNtlEventSobWorldItemCreate*>( pMsg.pData );

		// World item attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// World Item Proxy Events
		GetSobProxy()->HandleEvents(pMsg);

		// World item create event handler.
		CreateEventHandler(pMsg);

		// 초기 좌표 setting
		RwV3dAssignMacro( &m_vDestPos, &pWorldItemCreate->vLoc );

		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(this, &m_vDestPos, sHStuff);
		m_vDestPos.y = sHStuff.fFinialHeight;
		
		SetPosition( &m_vDestPos );

		// DropEffect
		if (pWorldItemCreate->eObjType == OBJTYPE_DROPITEM
			|| pWorldItemCreate->eObjType == OBJTYPE_DROPMONEY)
		{
			bool bNew = false;
			if (pWorldItemCreate->eObjType == OBJTYPE_DROPITEM)
				bNew = reinterpret_cast<sITEM_STATE*>(pWorldItemCreate->pState)->bIsNew;
			else if (pWorldItemCreate->eObjType == OBJTYPE_DROPMONEY)
				bNew = reinterpret_cast<sMONEY_STATE*>( pWorldItemCreate->pState )->bIsNew;

			if (bNew)
				SwitchState(WI_CREATE);
			else
				SwitchState(WI_NOCREATE);
		}
	}
	
	CNtlSobActor::HandleEvents(pMsg); 
		
	NTL_RETURNVOID();
}

RwBool CNtlSobWorldItem::IsEnableLoot(VOID)
{
	return ( m_eState == WI_ACTEND ) ? TRUE : FALSE ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implemantation

#define DROP_ITEM_FIN_TIME 1.000f

VOID CNtlSobWorldItem::SwitchState( RwInt32 eState )
{
#define DELAY_BASE_TIME	1.0f

	m_eState = eState;
	m_fTime = 0.0f;

	switch( eState )
	{
		case WI_CREATE:
		{
			m_pSobProxy->EnableVisible(FALSE);
			m_fDestTime = DELAY_BASE_TIME + (NtlRandomNumber(0, 400) * 0.001f);
			StateUpdate = &CNtlSobWorldItem::State_CreateUpdate;
			GetNtlSobManager()->AddUpdate(this);
		}
		break;
		case WI_ACTDROP:
		{
			Logic_PlayGUISound(GSD_SYSTEM_DROP_NORMAL);
			m_fDestAngle = (RwReal)NtlRandomNumber( 360, 900 );
			m_pSobProxy->EnableVisible(TRUE);
			CreateDropEffect();
			StateUpdate = &CNtlSobWorldItem::State_ActDropUpdate;
		}
		break;
		case WI_ACTEND:
		{
			GetNtlSobManager()->RemoveUpdateQueue(this);
			m_pSobProxy->SetBaseAnimation(ITEM_IDLE, TRUE);
		}
		break;
		case WI_NOCREATE:
		{
			m_pSobProxy->EnableVisible(TRUE);
			m_fDestAngle = (RwReal)NtlRandomNumber(360, 900);
			m_fDestTime = 1.50f; // by daneos: for some reason, drop is invisible if we instantly call m_pSobProxy->SetBaseAnimation(ITEM_IDLE, TRUE);
			StateUpdate = &CNtlSobWorldItem::State_NoCreateUpdate;
			GetNtlSobManager()->AddUpdate(this);
		}
		break;
	}
}

VOID CNtlSobWorldItem::State_CreateUpdate(VOID)
{
	if( m_fTime > m_fDestTime )
		SwitchState(WI_ACTDROP );
}

void CNtlSobWorldItem::State_NoCreateUpdate()
{
	if (m_fTime > m_fDestTime)
	{
		SwitchState(WI_ACTEND);
	}

	SetPosition(&m_vDestPos);
	SetAngleY(m_fTime * m_fDestAngle);
}

VOID CNtlSobWorldItem::State_ActDropUpdate(VOID)
{
	RwV3d vLoc = m_vDestPos;

	if( m_fTime > m_fDestTime )
	{
		//Time Sequence 삭제
		CNtlDistTimeSequence *pTempSeq;
		CNtlDistTimeSequence *pSeq = m_pSeq;

		while( pSeq )
		{
			pTempSeq = reinterpret_cast<CNtlDistTimeSequence*>( pSeq->GetNext() );

			NTL_DELETE( pSeq );

			pSeq = pTempSeq;
		}

		SwitchState( WI_ACTEND );
	}
	else
	{
		if (m_pSeq)
		{
			RwReal* pfDelaY = (RwReal*)m_pSeq->GetData(m_fTime);
			vLoc.y += (*pfDelaY);
		}
	}	
	
	// 좌표 setting
	SetPosition(&vLoc);
	SetAngleY( m_fTime * m_fDestAngle );

	if(!m_bVisualEff)
	{
		CreateDropVisualEffect();
		m_bVisualEff = TRUE;
	}
}

VOID CNtlSobWorldItem::CreateDropEffect(VOID)
{
#define DROP_SEQ_CNT 12

	RwReal arrTime[DROP_SEQ_CNT] = { 0.100f, 0.200f, 0.300f, 0.400f,  0.500f, 0.600f, 0.670f, 0.733f, 0.800f, 0.867f, 0.933f, DROP_ITEM_FIN_TIME };
	RwReal arrDist[DROP_SEQ_CNT] = { 1.060f, 1.400f, 1.520f, 1.490f,  1.285f, 0.820f, 0.000f, 0.388f, 0.500f, 0.500f, 0.390f, 0.000f };

	CNtlDistTimeSequence *pSeq = NULL;
	CNtlDistTimeSequence *pTempSeq = NULL;

	for( RwInt32 i = 0 ; i < DROP_SEQ_CNT ; ++i )
	{
		pTempSeq = NTL_NEW CNtlDistTimeSequence;
		pTempSeq->SetTime( arrTime[i] );
		pTempSeq->SetDistance( arrDist[i] );

		if( i == 0 )
		{
			m_pSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add( pTempSeq );
			pSeq = pTempSeq;
		}
	}
}


VOID CNtlSobWorldItem::CreateDropVisualEffect(VOID)
{
	/*if (pITEM_TBLDAT->byUseDisassemble == 3)
	{
		CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, "I_DRP_APR_002");
		if (pPLEntity)
		{
			RwV3d vPos = GetPosition();
			pPLEntity->SetPosition(&vPos);
		}
	}*/

	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_DROPITEM_EFF);
	if(pPLEntity)
	{
		RwV3d vPos = GetPosition();
		pPLEntity->SetPosition(&vPos);
	}
}


void CNtlSobWorldItem::SetLootSend(RwBool bSend)
{
	m_bLootSend = bSend;
}

RwBool CNtlSobWorldItem::IsLootSend(void)
{
	return m_bLootSend;
}
