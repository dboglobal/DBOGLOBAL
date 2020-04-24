#include "precomp_ntlsimulation.h"
#include "NtlSobVehicleAttr.h"
#include "NtlDebug.h"

// shared
#include "TableContainer.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlSob.h"


DEFINITION_MEMORY_POOL(CNtlSobVehicleAttr)


CNtlSobVehicleAttr::CNtlSobVehicleAttr() 
{
	m_pVehicleTable = NULL;
}


void CNtlSobVehicleAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if ( pMsg.Id == g_EventSobCreate )
	{
		SNtlEventSobVehicleCreate* pSobVehicleCreate = (SNtlEventSobVehicleCreate*)pMsg.pData;

		m_pVehicleTable = (sVEHICLE_TBLDAT*)API_GetTableContainer()->GetVehicleTable()->FindData( pSobVehicleCreate->uiVehicleTblIdx );
	}
}
