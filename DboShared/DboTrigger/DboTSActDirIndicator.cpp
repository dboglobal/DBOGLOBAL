#include "precomp_trigger.h"
#include "DboTSActDirIndicator.h"

#include "NtlDirection.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActDirIndicator, CNtlTSAction )


CDboTSActDirIndicator::CDboTSActDirIndicator( void )
{
	m_bSelfDirIndicator = false;
	m_bDirIndicatorStart = true;
	m_byDirType = INVALID_DIRECTION_INDICATE_TYPE;
	memset( &m_uiData, -1, sizeof( m_uiData ) );
}

void CDboTSActDirIndicator::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
    #define B2b(val) (val ? true : false)           ///< BOOL형을 bool형으로 변경한다.

	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "self" ) )
	{
		m_bSelfDirIndicator = B2b(clProperty.GetValueAsInt( "self" ));
	}
	else
	{
		m_bSelfDirIndicator = false;
	}

	if ( clProperty.IsExist( "stype" ) )
	{
		m_bDirIndicatorStart = B2b(clProperty.GetValueAsInt( "stype" ));
	}
	else
	{
		m_bDirIndicatorStart = true;
	}

	if ( clProperty.IsExist( "dtype" ) )
	{
		m_byDirType = (unsigned char)clProperty.GetValueAsInt( "dtype" );
	}
	else
	{
		m_byDirType = INVALID_DIRECTION_INDICATE_TYPE;
	}

	switch ( m_byDirType )
	{
	case DIRECTION_INDICATE_TYPE_TOBJECT:
		{
			if ( clProperty.IsExist( "tblidx" ) )
			{
				m_uiData.sTObj.uiObjTblIdx = clProperty.GetValueAsInt( "tblidx" );
			}
			else
			{
				m_uiData.sTObj.uiObjTblIdx = 0xffffffff;
			}
		}
		break;
	case DIRECTION_INDICATE_TYPE_NPC:
		{
			if ( clProperty.IsExist( "tblidx" ) )
			{
				m_uiData.sNPC.uiNPCTblIdx = clProperty.GetValueAsInt( "tblidx" );
			}
			else
			{
				m_uiData.sNPC.uiNPCTblIdx = 0xffffffff;
			}
		}
		break;
	case DIRECTION_INDICATE_TYPE_POINT:
		{
			if ( clProperty.IsExist( "x" ) )
			{
				m_uiData.sPos.fX = (float)atof( clProperty.GetValue( "x" ).c_str() );
			}
			else
			{
				m_uiData.sPos.fX = 0.f;
			}

			if ( clProperty.IsExist( "y" ) )
			{
				m_uiData.sPos.fY = (float)atof( clProperty.GetValue( "y" ).c_str() );
			}
			else
			{
				m_uiData.sPos.fY = 0.f;
			}

			if ( clProperty.IsExist( "z" ) )
			{
				m_uiData.sPos.fZ = (float)atof( clProperty.GetValue( "z" ).c_str() );
			}
			else
			{
				m_uiData.sPos.fZ = 0.f;
			}
		}
		break;
	}
}

void CDboTSActDirIndicator::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_bSelfDirIndicator );
	clProperty.m_defProperty["self"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_bDirIndicatorStart );
	clProperty.m_defProperty["stype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_byDirType );
	clProperty.m_defProperty["dtype"] = g_NtlTSString;

	switch ( m_byDirType )
	{
	case DIRECTION_INDICATE_TYPE_TOBJECT:
		{
			sprintf_s( g_NtlTSString, "%d", m_uiData.sTObj.uiObjTblIdx );
			clProperty.m_defProperty["tblidx"] = g_NtlTSString;
		}
		break;
	case DIRECTION_INDICATE_TYPE_NPC:
		{
			sprintf_s( g_NtlTSString, "%d", m_uiData.sNPC.uiNPCTblIdx );
			clProperty.m_defProperty["tblidx"] = g_NtlTSString;
		}
		break;
	case DIRECTION_INDICATE_TYPE_POINT:
		{
			sprintf_s( g_NtlTSString, "%f", m_uiData.sPos.fX );
			clProperty.m_defProperty["x"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uiData.sPos.fY );
			clProperty.m_defProperty["y"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uiData.sPos.fZ );
			clProperty.m_defProperty["z"] = g_NtlTSString;
		}
		break;
	}
}
