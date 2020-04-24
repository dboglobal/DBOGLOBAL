#include "precomp_ntlsimulation.h"
#include "NtlSobMerchantItemAttr.h"

// Table
#include "MerchantTable.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlGameTableContainer.h"


CNtlSobMerchantAttr::CNtlSobMerchantAttr()
{		
}

CNtlSobMerchantAttr::~CNtlSobMerchantAttr()
{

}

void CNtlSobMerchantAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobMerchantItemCreate* pSobMerchantItemCreate = reinterpret_cast<SNtlEventSobMerchantItemCreate*>(pMsg.pData);
		CMerchantTable *pMerchantTbl = GetGameTableContainer()->GetMerchantTable();

		m_pMerchantTbl = reinterpret_cast<sMERCHANT_TBLDAT*>( pMerchantTbl->FindData(pSobMerchantItemCreate->uiClassId) );
	}
}

sMERCHANT_TBLDAT* CNtlSobMerchantAttr::GetMerchantTbl(void) const
{
	return m_pMerchantTbl;
}

const WCHAR* CNtlSobMerchantAttr::GetTabName(void) const
{
	if(m_pMerchantTbl)
	{
		if( m_pMerchantTbl->wstrName.c_str() )
		{
			return m_pMerchantTbl->wstrName.c_str();
		}
	}

	return NULL;
}