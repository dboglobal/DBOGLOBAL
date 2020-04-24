#include "precomp_ntlpresentation.h"
#include "NtlDNController.h"
#include "NtlPLVisualManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlProfiler.h"


CDNNode::CDNNode( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucStartValue, RwUInt8 ucFinishValue, RwReal fBlendFactor, CNtlDNController* pController )
{
	m_fBeginTime = fBeginTime;
	m_fDurationTime = fDurationTime;
	m_ucStartValue = ucStartValue;
	m_ucFinishValue = ucFinishValue;
	m_fCurrentTime = 0;
	m_fBlendFactor = fBlendFactor;
	m_bCurrentStart = FALSE;
	m_pController = pController;
}

CDNNode::CDNNode( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucFinishValue, RwReal fBlendFactor, CNtlDNController* pController )
{
	m_fBeginTime = fBeginTime;
	m_fDurationTime = fDurationTime;
	
	m_ucFinishValue = ucFinishValue;
	m_fCurrentTime = 0;
	m_fBlendFactor = fBlendFactor;
	m_bCurrentStart = TRUE;
	m_pController = pController;
}

CDNNode::~CDNNode(VOID)
{

}

RwBool CDNNode::IsFinished(VOID)
{
	if( m_fCurrentTime > m_fBeginTime + m_fDurationTime )
		return TRUE;
	else
		return FALSE;
}

RwInt32 CDNNode::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	if( m_fCurrentTime < m_fBeginTime )
		return NOT_START;
	
	if( m_bCurrentStart )
	{
		m_bCurrentStart = FALSE;
		m_ucStartValue = m_pController->GetLastValue();
	}
	
	RwReal fFactor = ( m_fCurrentTime - m_fBeginTime ) / m_fDurationTime;

	if( fFactor > 1.0f ) 
		fFactor = 1.0f;

	return (RwInt32)( ( ( 1 - fFactor ) * m_ucStartValue ) + ( fFactor * m_ucFinishValue ) );
}

CNtlDNController::CNtlDNController(VOID)
{
	m_ucLastValue = 0;
	m_bSetDNEffect = FALSE;
}

CNtlDNController::~CNtlDNController(VOID)
{
	DeleteInstance();
}

RwBool CNtlDNController::CreateInstance(VOID)
{
	return TRUE;
}

VOID CNtlDNController::DeleteInstance(VOID)
{
	ClearNodes();
}

CNtlDNController* CNtlDNController::GetInstance(VOID)
{
	static CNtlDNController Instance;
	return &Instance;
}

VOID CNtlDNController::AddDNNode( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucStartValue, RwUInt8 ucFinishValue, RwReal fBlendFactor /* = 1.0f  */)
{
	m_listDNNode.push_back( NTL_NEW CDNNode( fBeginTime, fDurationTime, ucStartValue, ucFinishValue, fBlendFactor, this ) );
}

VOID CNtlDNController::AddDNNodeCurrentStart( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucFinishValue, RwReal fBlendFactor /* = 1.0f  */)
{
	m_listDNNode.push_back( NTL_NEW CDNNode( fBeginTime, fDurationTime, ucFinishValue, fBlendFactor, this ) );
}

VOID CNtlDNController::AddDNNodeCurrentEnd( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucStartValue, RwReal fBlendFactor /* = 1.0f  */)
{
	m_listDNNode.push_back( NTL_NEW CDNNode( fBeginTime, fDurationTime, ucStartValue, m_ucLastValue, fBlendFactor, this ) );
}

VOID CNtlDNController::Update( RwReal fElapsedTime )
{
	NTL_SPROFILE("CNtlDNController::Update")

	if( m_listDNNode.size() <= 0)
	{
		// 살아 있는 Node가 없더라고, 현재 DNEffect가 적용된 상태(현재 DNValue가 0이 아닌 상태)에서는 
		// DN Effect가 적용되었다고 판단한다. (2007.7.12 by agebreak)
		if(m_ucLastValue == 0)
		{
			m_bSetDNEffect = FALSE;
		}

		NTL_RPROFILE_VOID()
	}

	RwReal fCumulatedFactor= 0.0f;
	RwReal fCumulatedValue = 0.0f;
	m_bSetDNEffect = FALSE;

	for( DNLIST_IT it = m_listDNNode.begin() ; it != m_listDNNode.end() ; )
	{
		CDNNode* pNode = (*it);
		RwInt32 nResult = pNode->Update( fElapsedTime );

		if( nResult != CDNNode::NOT_START )
		{
			fCumulatedFactor+= pNode->GetBlendFactor();
			fCumulatedValue += nResult * pNode->GetBlendFactor();
			m_bSetDNEffect = TRUE;

			if( pNode->IsFinished() )
			{
				NTL_DELETE( *it );
				it = m_listDNNode.erase( it );
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
			continue;
		}
	}

	if( m_bSetDNEffect )
	{
		m_ucLastValue = (RwUInt8)( fCumulatedValue / fCumulatedFactor );
		CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
		pWorldEntity->SetDNEffect( m_ucLastValue );
	}	

	NTL_EPROFILE()
}

VOID CNtlDNController::ClearNodes(VOID)
{
	DNLIST_IT it;

	for( it = m_listDNNode.begin() ; it != m_listDNNode.end() ; ++it )
	{
		NTL_DELETE( *it );
	}

	m_listDNNode.clear();
}
