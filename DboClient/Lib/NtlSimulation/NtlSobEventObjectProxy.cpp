#include "precomp_ntlsimulation.h"
#include "NtlSobEventObjectProxy.h"

// shared

// core
#include "NtlMath.h"

//// presentation
#include "NtlPLEvent.h"
#include "NtlPLSceneManager.h"
#include "NtlPLObject.h"
#include "NtlPlApi.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSob.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "TextAllTable.h"

DEFINITION_MEMORY_POOL( CNtlSobEventObjectProxy )

CNtlSobEventObjectProxy::CNtlSobEventObjectProxy()
{
	m_pPLObject		= NULL;
	m_fObjectWidth	= 1.0f;
	m_fObjectHeight	= 1.0f;
	m_fObjectDepth	= 1.0f;
}

CNtlSobEventObjectProxy::~CNtlSobEventObjectProxy()
{
	
}

RwBool CNtlSobEventObjectProxy::Create(RwUInt32 uiCompType)
{
	CNtlSobProxy::Create(uiCompType);

	return TRUE;
}

void CNtlSobEventObjectProxy::Destroy(VOID)
{
    m_pPLObject = NULL;
	CNtlSobProxy::Destroy();
}


void CNtlSobEventObjectProxy::Update(RwReal fElapsed)
{
}

void CNtlSobEventObjectProxy::HandleEvents( RWS::CMsg &pMsg )
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SobCreateEventHandler(pMsg);
	}	
	else if(pMsg.Id == g_EventSobGotFocus)
	{
		m_pPLObject->SetAddColor(40, 40, 40);        
        
        if(m_pPLObject->GetMilepostID())
        {
            // 클라이언트 레이어의 UI쪽으로 메시지를 쏴서 툴팁 표시        
            std::wstring& wstrString = API_GetTableContainer()->GetTextAllTable()->GetMilePostTable()->GetText(m_pPLObject->GetMilepostID());
            CNtlSLEventGenerator::ShowToolTip(TRUE, 0, 0, wstrString.c_str());
        }
	}
	else if(pMsg.Id == g_EventSobLostFocus)
	{
		m_pPLObject->SetAddColor(0, 0, 0);

        if(m_pPLObject->GetMilepostID())
        {
            // 클라이언트 레이어의 UI쪽으로 메시지를 쏴서 툴팁 제거
            CNtlSLEventGenerator::ShowToolTip(FALSE, 0, 0, L"");
        }        
	}		
}

void CNtlSobEventObjectProxy::SobCreateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobEventObjectCreate *pSobCreate = reinterpret_cast<SNtlEventSobEventObjectCreate*>(pMsg.pData);
	m_pPLObject = reinterpret_cast<CNtlPLObject*>( pSobCreate->pPlEntity );

	m_pPLObject->SetSerialID(m_pSobObj->GetSerialID());

	// width, height, depth
	const RwBBox *pBox = m_pPLObject->GetBoundingBox();
	if(pBox)
	{
		m_fObjectWidth	= pBox->sup.x - pBox->inf.x;
		m_fObjectHeight = pBox->sup.y - pBox->inf.y;
		m_fObjectDepth	= pBox->sup.z - pBox->inf.z;
	}
}

void CNtlSobEventObjectProxy::SetPosition(const RwV3d *pPos)
{
	if(m_pPLObject)
	{
		m_pPLObject->SetPosition(pPos);
	}
}

void CNtlSobEventObjectProxy::SetDirection(const RwV3d *pDir)
{
	if(m_pPLObject)
	{
		RwReal fAngle = CNtlMath::LineToAngleY(pDir);
		m_pPLObject->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

void CNtlSobEventObjectProxy::SetAngleY(RwReal fAngle)
{
	if(m_pPLObject)
	{
		m_pPLObject->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

RwReal CNtlSobEventObjectProxy::GetPLEntityWidth(void)
{
	return m_fObjectWidth;
}


RwReal CNtlSobEventObjectProxy::GetPLEntityHeight(void)
{
	return m_fObjectHeight;
}


RwReal CNtlSobEventObjectProxy::GetPLEntityDepth(void)
{
	return m_fObjectDepth;
}