#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptTrade.h"

CNtlWorldConceptTrade::CNtlWorldConceptTrade(void)
{
}

CNtlWorldConceptTrade::~CNtlWorldConceptTrade(void)
{
}

void CNtlWorldConceptTrade::ChangeState( RwInt32 eState )
{
	CNtlWorldConceptController::ChangeState( eState );

	if( eState == WORLD_STATE_EXIT )
		CNtlWorldConceptController::ChangeState( WORLD_STATE_NONE );
}