#include "precomp_ntlsimulation.h"
#include "NtlSobWorldItemProxy.h"

// shared
#include "ItemTable.h"
#include "TableContainer.h"
#include "DragonBallTable.h"

// core
#include "NtlMath.h"

//// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLItem.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSob.h"
#include "NtlSobWorldItem.h"
#include "NtlSobWorldItemAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"


#define WORLDITEM_MODELNAME_NORMAL			"I_DRP_01"
#define WORLDITEM_MODELNAME_SUPERIOR		"I_DRP_02"
#define WORLDITEM_MODELNAME_EXCELLENT		"I_DRP_07"
#define WORLDITEM_MODELNAME_RARE			"I_DRP_03"
#define WORLDITEM_MODELNAME_LEGENDARY		"I_DRP_04"
#define WORLDITEM_MODELNAME_ETC				"I_DRP_05"
#define WORLDITEM_MODELNAME_UNIDENTIFIED	"I_DRP_06"

#define WORLDITEM_MODELNAME_MONEY_SMALL		"I_DRP_11"
#define WORLDITEM_MODELNAME_MONEY_NORMAL	"I_DRP_12"
#define WORLDITEM_MODELNAME_MONEY_BIG		"I_DRP_13"

#define WORLDITEM_MODELNAME_DRANGONBALL_BASIC		"I_DRP_21"
#define WORLDITEM_MODELNAME_DRANGONBALL_NORMAL		"I_DRP_22"

DEFINITION_MEMORY_POOL( CNtlSobWorldItemProxy )

CNtlSobWorldItemProxy::CNtlSobWorldItemProxy()
{
	m_pWorldItem = NULL;
}

CNtlSobWorldItemProxy::~CNtlSobWorldItemProxy()
{

}

RwBool CNtlSobWorldItemProxy::Create(RwUInt32 uiCompType)
{
	CNtlSobProxy::Create(uiCompType);

	return TRUE;
}

/*Human_Male*/
VOID CNtlSobWorldItemProxy::Destroy(VOID)
{
	DeletePLWorldItem();

	CNtlSobProxy::Destroy();
}


VOID CNtlSobWorldItemProxy::HandleEvents( RWS::CMsg &pMsg )
{
	if( pMsg.Id == g_EventSobCreate )
	{
		CreatePLWorldItem( pMsg );
	}
	else if(pMsg.Id == g_EventSobGotFocus)
	{
		m_pWorldItem->SetAddColor(40, 40, 40);
	}
	else if(pMsg.Id == g_EventSobLostFocus)
	{
		m_pWorldItem->SetAddColor(0, 0, 0);
	}
}

VOID CNtlSobWorldItemProxy::CreatePLWorldItem( RWS::CMsg& msg )
{
	NTL_FUNCTION("CNtlSobWorldItemProxy::CreatePLWorldItem");

	RwChar*	szModelName = NULL;
	CNtlSobWorldItemAttr* pAttr = reinterpret_cast<CNtlSobWorldItemAttr*>( m_pSobObj->GetSobAttr() );	

	if( pAttr->IsItem() )
	{
		if( pAttr->IsIdentified() ) 
		{
			if( pAttr->IsDragonBall() )
			{
                // 드래곤볼 타입을 판단한다.
                sITEM_TBLDAT* pTblDat = pAttr->GetItemTbl();
                if(!pTblDat)
                    return;

                CDragonBallTable* pDBTable = API_GetTableContainer()->GetDragonBallTable();
                if(!pDBTable)
                    return;

                eDRAGON_BALL_TYPE byType = pDBTable->GetDropItemType(pTblDat->tblidx);
        //        NTL_ASSERT(byType != DRAGON_BALL_TYPE_NONE, __FUNCTION__ << "DragonBall Drop Type Invalid" );

				if (byType == DRAGON_BALL_TYPE_NONE) // by daneos
				{
					szModelName = WORLDITEM_MODELNAME_DRANGONBALL_BASIC;
				}
				else if (byType == DRAGON_BALL_TYPE_BASIC)
                {
				    szModelName = WORLDITEM_MODELNAME_DRANGONBALL_BASIC;
                }
                else if(byType == DRAGON_BALL_TYPE_NORMAL)
                {
                    szModelName = WORLDITEM_MODELNAME_DRANGONBALL_NORMAL;
                }
				else
				{
					szModelName = WORLDITEM_MODELNAME_DRANGONBALL_NORMAL;

					// to do: set model-name depending on the amount of db stars
				}
			}
			else
			{
				switch( pAttr->GetRank() )
				{
					case ITEM_RANK_NOTHING: szModelName = WORLDITEM_MODELNAME_ETC; break;
					case ITEM_RANK_NORMAL: szModelName = WORLDITEM_MODELNAME_NORMAL; break;
					case ITEM_RANK_SUPERIOR: szModelName = WORLDITEM_MODELNAME_SUPERIOR; break;
					case ITEM_RANK_EXCELLENT: szModelName = WORLDITEM_MODELNAME_EXCELLENT; break;
					case ITEM_RANK_RARE: szModelName = WORLDITEM_MODELNAME_RARE; break;
					case ITEM_RANK_LEGENDARY: szModelName = WORLDITEM_MODELNAME_LEGENDARY; break;
				}			
			}			
		}
		else
		{
			szModelName = WORLDITEM_MODELNAME_UNIDENTIFIED;
		}
	}
	else if( pAttr->IsMoney() )
	{
		if( pAttr->GetMoney() < 101 )
		{
			szModelName = WORLDITEM_MODELNAME_MONEY_SMALL;
		}
		else if( pAttr->GetMoney() < 301 )
		{
			szModelName = WORLDITEM_MODELNAME_MONEY_NORMAL;
		}
		else
		{
			szModelName = WORLDITEM_MODELNAME_MONEY_BIG;
		}
	}	
	else
		NTL_ASSERTFAIL( "World Item type is worng" );

	m_pWorldItem = static_cast<CNtlPLItem*>( GetSceneManager()->CreateEntity( PLENTITY_ITEM, szModelName ) );
	NTL_ASSERT( m_pWorldItem, "world item proxy is NULL  : " << szModelName );
    if(!m_pWorldItem)
        NTL_RETURNVOID();

	m_pWorldItem->SetSerialID( m_pSobObj->GetSerialID() );

	NTL_RETURNVOID();
}

VOID CNtlSobWorldItemProxy::DeletePLWorldItem(VOID)
{
	// pl character destroy
	if( m_pWorldItem )
	{
		GetSceneManager()->DeleteEntity( m_pWorldItem );
		m_pWorldItem = NULL;
	}
}

void CNtlSobWorldItemProxy::AddWorld(void)
{
	if(m_pWorldItem)
		m_pWorldItem->AddWorld();
}

void CNtlSobWorldItemProxy::RemoveWorld(void)
{
	if(m_pWorldItem)
		m_pWorldItem->RemoveWorld();
}

void CNtlSobWorldItemProxy::SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop /*= TRUE*/, RwReal fStartTime /*= 0.0f*/)
{
	if(m_pWorldItem)
		m_pWorldItem->SetAnimation(uiAnimKey, fStartTime, bLoop);
}

VOID CNtlSobWorldItemProxy::SetPosition(const RwV3d *pPos)
{
	if(m_pWorldItem)
	{
		m_pWorldItem->SetPosition(pPos);
	}
}

VOID CNtlSobWorldItemProxy::SetDirection(const RwV3d *pDir)
{
	if(m_pWorldItem)
	{
		RwReal fAngle = CNtlMath::LineToAngleY(pDir);
		m_pWorldItem->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

VOID CNtlSobWorldItemProxy::SetAngleY(RwReal fAngle)
{
	if(m_pWorldItem)
	{
		m_pWorldItem->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

VOID CNtlSobWorldItemProxy::EnableVisible(RwBool bEnable)
{
	CNtlSobProxy::EnableVisible(bEnable);

	if( m_pWorldItem )
	{
		m_pWorldItem->SetVisible( bEnable );
	}
}