#include "precomp_trigger.h"
#include "DboTSActOPCam.h"


/**
	Operation camera action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActOPCam, CNtlTSAction )


CDboTSActOPCam::CDboTSActOPCam( void )
{
}

void CDboTSActOPCam::SetFuncName( const std::string& strFName )
{
	m_strFuncName = strFName;
}

void CDboTSActOPCam::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "fname" ) )
	{
		SetFuncName( clProperty.GetValue( "fname" ) );
	}
}

void CDboTSActOPCam::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	clProperty.m_defProperty["fname"] = GetFuncName();
}
