#include "precomp_trigger.h"
#include "NtlTSEntity.h"
#include "NtlTSRecv.h"


/**
	Entity
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSEntity, CNtlTSScrObject )


CNtlTSEntity::CNtlTSEntity( void )
: m_pParent( 0 )
{
}

CNtlTSEntity* CNtlTSEntity::GetParent( void )
{
	return m_pParent;
}

CNtlTSEntity* CNtlTSEntity::SetParent( CNtlTSEntity* pParent )
{
	CNtlTSEntity* pOld = m_pParent;
	m_pParent = pParent;
	return pOld;
}

CNtlTSEntity* CNtlTSEntity::GetRoot( void )
{
	if ( m_pParent )
	{
		return m_pParent->GetRoot();
	}

	return this;
}

NTL_TSRESULT CNtlTSEntity::SearchSelf( CNtlTSRecv* pTSRecv, void* pParam )
{
	if ( pTSRecv ) return pTSRecv->Search( this, pParam );
	return NTL_TSRESULT_TYPE_ERROR;
}

NTL_TSRESULT CNtlTSEntity::RunSelf( CNtlTSRecv* pTSRecv, void* pParam )
{
	if ( pTSRecv ) return pTSRecv->Run( this, pParam );
	return NTL_TSRESULT_TYPE_ERROR;
}

void CNtlTSEntity::MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList )
{
	defObjList.push_back( this );
}
