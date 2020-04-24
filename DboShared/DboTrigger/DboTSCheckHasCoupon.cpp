#include "precomp_trigger.h"
#include "DboTSCheckHasCoupon.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckHasCoupon, CNtlTSCond )


CDboTSCheckHasCoupon::CDboTSCheckHasCoupon( void )
{
}

void CDboTSCheckHasCoupon::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );
}

void CDboTSCheckHasCoupon::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );
}
