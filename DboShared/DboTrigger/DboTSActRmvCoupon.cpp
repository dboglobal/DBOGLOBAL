#include "precomp_trigger.h"
#include "DboTSActRmvCoupon.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActRmvCoupon, CNtlTSAction )


CDboTSActRmvCoupon::CDboTSActRmvCoupon( void )
{
}

void CDboTSActRmvCoupon::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );
}

void CDboTSActRmvCoupon::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );
}
