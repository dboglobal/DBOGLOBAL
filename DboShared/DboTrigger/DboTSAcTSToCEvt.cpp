#include "precomp_trigger.h"
#include "DboTSActSToCEvt.h"


/**
	Create quest item action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActSToCEvt, CNtlTSAction )


CDboTSActSToCEvt::CDboTSActSToCEvt( void )
: m_eEvtType( eSTOC_EVT_TYPE_START ),
  m_bApplyTypeWorld( false ),
  m_eEvtDataType( eSTOC_EVT_DATA_TYPE_INVALID ),
  m_eEvtCondDataType( eSTOC_EVT_COND_DATA_TYPE_INVALID ),
  m_uiEvtSubCondDataType( eSTOC_EVT_SUB_COND_DATA_FLAG_INVALID )
{
	memset( &m_uEvtData, 0xffffffff, sizeof( m_uEvtData ) );
	memset( &m_uEvtCondData, 0xffffffff, sizeof( m_uEvtCondData ) );
	memset( &m_sEvtSubCondData, 0, sizeof( m_sEvtSubCondData ) );
}

void CDboTSActSToCEvt::SetEvtType( eSTOC_EVT_TYPE eEvtType )
{
	m_eEvtType = eEvtType;
}

void CDboTSActSToCEvt::SetApplyTypeWorld( bool bApplyTypeWorld )
{
	m_bApplyTypeWorld = bApplyTypeWorld;
}

void CDboTSActSToCEvt::SetEvtDataType( eSTOC_EVT_DATA_TYPE eEvtDataType )
{
	m_eEvtDataType = eEvtDataType;
}

void CDboTSActSToCEvt::SetEvtData( const uSTOC_EVT_DATA& uEvtData )
{
	m_uEvtData = uEvtData;
}

void CDboTSActSToCEvt::SetEvtCondDataType( eSTOC_EVT_COND_DATA_TYPE eEvtCondDataType )
{
	m_eEvtCondDataType = eEvtCondDataType;
}

void CDboTSActSToCEvt::SetEvtCondData( const uSTOC_EVT_COND_DATA& uEvtCondData )
{
	m_uEvtCondData = uEvtCondData;
}

void CDboTSActSToCEvt::SetEvtSubCondDataType( unsigned int uiEvtSubCondDataType )
{
	m_uiEvtSubCondDataType = uiEvtSubCondDataType;
}

void CDboTSActSToCEvt::SetEvtSubCondData( const sSTOC_EVT_SUB_COND_DATA& sEvtSubCondData )
{
	m_sEvtSubCondData = sEvtSubCondData;
}

void CDboTSActSToCEvt::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "etype" ) )
	{
		SetEvtType( (eSTOC_EVT_TYPE)clProperty.GetValueAsInt( "etype" ) );
	}

	if ( clProperty.IsExist( "apptype" ) )
	{
        SetApplyTypeWorld(clProperty.GetValueAsInt( "apptype" ) ? true : false );
	}

	if ( clProperty.IsExist( "eitype" ) )
	{
		SetEvtDataType( (eSTOC_EVT_DATA_TYPE)clProperty.GetValueAsInt( "eitype" ) );

		uSTOC_EVT_DATA uEvtData;
		memset( &uEvtData, 0xffffffff, sizeof( uEvtData ) );

		switch ( GetEvtDataType() )
		{
		case eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT:
			{
				uEvtData.sMobKillCnt[0].uiMobIdx = clProperty.GetValueAsInt( "idx0" );
				uEvtData.sMobKillCnt[0].nMobCnt = clProperty.GetValueAsInt( "cnt0" );
				uEvtData.sMobKillCnt[0].nCurMobCnt = 0;

				uEvtData.sMobKillCnt[1].uiMobIdx = clProperty.GetValueAsInt( "idx1" );
				uEvtData.sMobKillCnt[1].nMobCnt = clProperty.GetValueAsInt( "cnt1" );
				uEvtData.sMobKillCnt[1].nCurMobCnt = 0;

				uEvtData.sMobKillCnt[2].uiMobIdx = clProperty.GetValueAsInt( "idx2" );
				uEvtData.sMobKillCnt[2].nMobCnt = clProperty.GetValueAsInt( "cnt2" );
				uEvtData.sMobKillCnt[2].nCurMobCnt = 0;
			}
			break;
		case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
			{
				uEvtData.sMobKillItemCnt[0].uiMobLIIdx = clProperty.GetValueAsInt( "idx0" );
				uEvtData.sMobKillItemCnt[0].nMobLICnt = clProperty.GetValueAsInt( "cnt0" );
				uEvtData.sMobKillItemCnt[0].nCurMobLICnt = 0;

				uEvtData.sMobKillItemCnt[1].uiMobLIIdx = clProperty.GetValueAsInt( "idx1" );
				uEvtData.sMobKillItemCnt[1].nMobLICnt = clProperty.GetValueAsInt( "cnt1" );
				uEvtData.sMobKillItemCnt[1].nCurMobLICnt = 0;

				uEvtData.sMobKillItemCnt[2].uiMobLIIdx = clProperty.GetValueAsInt( "idx2" );
				uEvtData.sMobKillItemCnt[2].nMobLICnt = clProperty.GetValueAsInt( "cnt2" );
				uEvtData.sMobKillItemCnt[2].nCurMobLICnt = 0;
			}
			break;
		case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
			{
				uEvtData.sDeliveryItemCnt[0].uiItemIdx = clProperty.GetValueAsInt( "idx0" );
				uEvtData.sDeliveryItemCnt[0].nItemCnt = clProperty.GetValueAsInt( "cnt0" );
				uEvtData.sDeliveryItemCnt[0].nCurItemCnt = uEvtData.sDeliveryItemCnt[0].nItemCnt;

				uEvtData.sDeliveryItemCnt[1].uiItemIdx = clProperty.GetValueAsInt( "idx1" );
				uEvtData.sDeliveryItemCnt[1].nItemCnt = clProperty.GetValueAsInt( "cnt1" );
				uEvtData.sDeliveryItemCnt[1].nCurItemCnt = uEvtData.sDeliveryItemCnt[1].nItemCnt;

				uEvtData.sDeliveryItemCnt[2].uiItemIdx = clProperty.GetValueAsInt( "idx2" );
				uEvtData.sDeliveryItemCnt[2].nItemCnt = clProperty.GetValueAsInt( "cnt2" );
				uEvtData.sDeliveryItemCnt[2].nCurItemCnt = uEvtData.sDeliveryItemCnt[2].nItemCnt;
			}
			break;
		case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
			{
				uEvtData.sObjectItemCnt[0].uiItemIdx = clProperty.GetValueAsInt( "idx0" );
				uEvtData.sObjectItemCnt[0].nItemCnt = clProperty.GetValueAsInt( "cnt0" );
				uEvtData.sObjectItemCnt[0].nCurItemCnt = 0;

				uEvtData.sObjectItemCnt[1].uiItemIdx = clProperty.GetValueAsInt( "idx1" );
				uEvtData.sObjectItemCnt[1].nItemCnt = clProperty.GetValueAsInt( "cnt1" );
				uEvtData.sObjectItemCnt[1].nCurItemCnt = 0;

				uEvtData.sObjectItemCnt[2].uiItemIdx = clProperty.GetValueAsInt( "idx2" );
				uEvtData.sObjectItemCnt[2].nItemCnt = clProperty.GetValueAsInt( "cnt2" );
				uEvtData.sObjectItemCnt[2].nCurItemCnt = 0;
			}
			break;
		case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
			{
				uEvtData.sPublicMobItemCnt[0].uiItemIdx = clProperty.GetValueAsInt( "idx0" );
				uEvtData.sPublicMobItemCnt[0].uiRequireItemIdx = clProperty.GetValueAsInt( "nidx0" );
				uEvtData.sPublicMobItemCnt[0].fDropRate = (float)atof( clProperty.GetValue( "rate0" ).c_str() );
				uEvtData.sPublicMobItemCnt[0].nItemCnt = clProperty.GetValueAsInt( "cnt0" );
				uEvtData.sPublicMobItemCnt[0].nCurItemCnt = 0;
			}
			break;
		case eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT:
			{
				uEvtData.sCustomEvtCnt[0].uiQTextTblIdx = clProperty.GetValueAsInt( "qtidx" );
				uEvtData.sCustomEvtCnt[0].uiCustomEvtID = clProperty.GetValueAsInt( "idx0" );
				uEvtData.sCustomEvtCnt[0].nMaxCnt = clProperty.GetValueAsInt( "cnt0" );
				uEvtData.sCustomEvtCnt[0].nCurCnt = 0;
			}
			break;
		case eSTOC_EVT_DATA_TYPE_VISIT:
			{
				if ( clProperty.IsExist( "otype0" ) )
				{
					uEvtData.sVisitEvt[0].bCompleted		= false;
					uEvtData.sVisitEvt[0].byObjType			= (UCHAR)clProperty.GetValueAsInt( "otype0" );
					uEvtData.sVisitEvt[0].uiWorldTblIdx		= clProperty.GetValueAsInt( "widx0" );
					uEvtData.sVisitEvt[0].uiObjTblIdx		= clProperty.GetValueAsInt( "tidx0" );
					uEvtData.sVisitEvt[0].uiIndicatorQText	= clProperty.GetValueAsInt( "it0" );
					uEvtData.sVisitEvt[0].uiDialogText		= clProperty.GetValueAsInt( "dt0" );
					uEvtData.sVisitEvt[0].uiItemTblIdx		= clProperty.GetValueAsInt( "iidx0" );
				}
				else
				{
					uEvtData.sVisitEvt[0].bCompleted		= false;
				}

				if ( clProperty.IsExist( "otype1" ) )
				{
					uEvtData.sVisitEvt[1].bCompleted		= false;
					uEvtData.sVisitEvt[1].byObjType			= (UCHAR)clProperty.GetValueAsInt( "otype1" );
					uEvtData.sVisitEvt[1].uiWorldTblIdx		= clProperty.GetValueAsInt( "widx1" );
					uEvtData.sVisitEvt[1].uiObjTblIdx		= clProperty.GetValueAsInt( "tidx1" );
					uEvtData.sVisitEvt[1].uiIndicatorQText	= clProperty.GetValueAsInt( "it1" );
					uEvtData.sVisitEvt[1].uiDialogText		= clProperty.GetValueAsInt( "dt1" );
					uEvtData.sVisitEvt[1].uiItemTblIdx		= clProperty.GetValueAsInt( "iidx1" );
				}
				else
				{
					uEvtData.sVisitEvt[1].bCompleted		= false;
				}

				if ( clProperty.IsExist( "otype2" ) )
				{
					uEvtData.sVisitEvt[2].bCompleted		= false;
					uEvtData.sVisitEvt[2].byObjType			= (UCHAR)clProperty.GetValueAsInt( "otype2" );
					uEvtData.sVisitEvt[2].uiWorldTblIdx		= clProperty.GetValueAsInt( "widx2" );
					uEvtData.sVisitEvt[2].uiObjTblIdx		= clProperty.GetValueAsInt( "tidx2" );
					uEvtData.sVisitEvt[2].uiIndicatorQText	= clProperty.GetValueAsInt( "it2" );
					uEvtData.sVisitEvt[2].uiDialogText		= clProperty.GetValueAsInt( "dt2" );
					uEvtData.sVisitEvt[2].uiItemTblIdx		= clProperty.GetValueAsInt( "iidx2" );
				}
				else
				{
					uEvtData.sVisitEvt[2].bCompleted		= false;
				}
			}
			break;
		}

		SetEvtData( uEvtData );
	}

	if ( clProperty.IsExist( "ectype" ) )
	{
		SetEvtCondDataType( (eSTOC_EVT_COND_DATA_TYPE)clProperty.GetValueAsInt( "ectype" ) );

		uSTOC_EVT_COND_DATA uEvtCondData;
		memset( &uEvtCondData, 0xffffffff, sizeof( uEvtCondData ) );

		switch ( GetEvtCondDataType() )
		{
		case eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM:
			{
				uEvtCondData.sAutoEquipItem[0].uiItemTblIdx = clProperty.GetValueAsInt( "cd0" );
			}
			break;
		case eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM:
			{
				uEvtCondData.sEquipItem[0].uiItemTblIdx = clProperty.GetValueAsInt( "cd0" );
			}
			break;
		case eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM:
			{
				uEvtCondData.sHaveItem[0].uiItemTblIdx = clProperty.GetValueAsInt( "cd0" );
			}
			break;
		case eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM:
			{
				uEvtCondData.sHaveQItem[0].uiQItemTblIdx = clProperty.GetValueAsInt( "cd0" );
				uEvtCondData.sHaveQItem[0].nQITemCnt = clProperty.GetValueAsInt( "cd1" );
			}
			break;
		}

		SetEvtCondData( uEvtCondData );
	}

	if ( clProperty.IsExist( "esctype" ) )
	{
		unsigned int uiEvtSubCondType = clProperty.GetValueAsInt( "esctype" );

		SetEvtSubCondDataType( uiEvtSubCondType );

		sSTOC_EVT_SUB_COND_DATA sEvtSubCondData;
		memset( &sEvtSubCondData, 0, sizeof( sEvtSubCondData ) );

		if ( uiEvtSubCondType & eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS )
		{
			sEvtSubCondData.sLevelDiffLess.nLevelDiff = clProperty.GetValueAsInt( "scd0" );
		}

		SetEvtSubCondData( sEvtSubCondData );
	}
}

void CDboTSActSToCEvt::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetEvtType() );
	clProperty.m_defProperty["etype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", IsApplyTypeWorld() ? 1 : 0 );
	clProperty.m_defProperty["apptype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetEvtDataType() );
	clProperty.m_defProperty["eitype"] = g_NtlTSString;

	switch ( GetEvtDataType() )
	{
	case eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillCnt[0].uiMobIdx );
			clProperty.m_defProperty["idx0"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillCnt[0].nMobCnt );
			clProperty.m_defProperty["cnt0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillCnt[1].uiMobIdx );
			clProperty.m_defProperty["idx1"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillCnt[1].nMobCnt );
			clProperty.m_defProperty["cnt1"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillCnt[2].uiMobIdx );
			clProperty.m_defProperty["idx2"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillCnt[2].nMobCnt );
			clProperty.m_defProperty["cnt2"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillItemCnt[0].uiMobLIIdx );
			clProperty.m_defProperty["idx0"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillItemCnt[0].nMobLICnt );
			clProperty.m_defProperty["cnt0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillItemCnt[1].uiMobLIIdx );
			clProperty.m_defProperty["idx1"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillItemCnt[1].nMobLICnt );
			clProperty.m_defProperty["cnt1"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillItemCnt[2].uiMobLIIdx );
			clProperty.m_defProperty["idx2"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sMobKillItemCnt[2].nMobLICnt );
			clProperty.m_defProperty["cnt2"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sDeliveryItemCnt[0].uiItemIdx );
			clProperty.m_defProperty["idx0"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sDeliveryItemCnt[0].nItemCnt );
			clProperty.m_defProperty["cnt0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sDeliveryItemCnt[1].uiItemIdx );
			clProperty.m_defProperty["idx1"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sDeliveryItemCnt[1].nItemCnt );
			clProperty.m_defProperty["cnt1"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sDeliveryItemCnt[2].uiItemIdx );
			clProperty.m_defProperty["idx2"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sDeliveryItemCnt[2].nItemCnt );
			clProperty.m_defProperty["cnt2"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sObjectItemCnt[0].uiItemIdx );
			clProperty.m_defProperty["idx0"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sObjectItemCnt[0].nItemCnt );
			clProperty.m_defProperty["cnt0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sObjectItemCnt[1].uiItemIdx );
			clProperty.m_defProperty["idx1"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sObjectItemCnt[1].nItemCnt );
			clProperty.m_defProperty["cnt1"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sObjectItemCnt[2].uiItemIdx );
			clProperty.m_defProperty["idx2"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sObjectItemCnt[2].nItemCnt );
			clProperty.m_defProperty["cnt2"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sPublicMobItemCnt[0].uiItemIdx );
			clProperty.m_defProperty["idx0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sPublicMobItemCnt[0].uiRequireItemIdx );
			clProperty.m_defProperty["nidx0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uEvtData.sPublicMobItemCnt[0].fDropRate );
			clProperty.m_defProperty["rate0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sPublicMobItemCnt[0].nItemCnt );
			clProperty.m_defProperty["cnt0"] = g_NtlTSString;
		}
		break;

	case eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sCustomEvtCnt[0].uiQTextTblIdx );
			clProperty.m_defProperty["qtidx"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sCustomEvtCnt[0].uiCustomEvtID );
			clProperty.m_defProperty["idx0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtData.sCustomEvtCnt[0].nMaxCnt );
			clProperty.m_defProperty["cnt0"] = g_NtlTSString;
		}
		break;

	case eSTOC_EVT_DATA_TYPE_VISIT:
		{
			if ( 0xff != m_uEvtData.sVisitEvt[0].byObjType )
			{
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[0].byObjType );
				clProperty.m_defProperty["otype0"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[0].uiWorldTblIdx );
				clProperty.m_defProperty["widx0"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[0].uiObjTblIdx );
				clProperty.m_defProperty["tidx0"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[0].uiIndicatorQText );
				clProperty.m_defProperty["it0"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[0].uiDialogText );
				clProperty.m_defProperty["dt0"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[0].uiItemTblIdx );
				clProperty.m_defProperty["iidx0"] = g_NtlTSString;
			}

			if ( 0xff != m_uEvtData.sVisitEvt[1].byObjType )
			{
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[1].byObjType );
				clProperty.m_defProperty["otype1"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[1].uiWorldTblIdx );
				clProperty.m_defProperty["widx1"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[1].uiObjTblIdx );
				clProperty.m_defProperty["tidx1"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[1].uiIndicatorQText );
				clProperty.m_defProperty["it1"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[1].uiDialogText );
				clProperty.m_defProperty["dt1"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[1].uiItemTblIdx );
				clProperty.m_defProperty["iidx1"] = g_NtlTSString;
			}

			if ( 0xff != m_uEvtData.sVisitEvt[2].byObjType )
			{
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[2].byObjType );
				clProperty.m_defProperty["otype2"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[2].uiWorldTblIdx );
				clProperty.m_defProperty["widx2"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[2].uiObjTblIdx );
				clProperty.m_defProperty["tidx2"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[2].uiIndicatorQText );
				clProperty.m_defProperty["it2"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[2].uiDialogText );
				clProperty.m_defProperty["dt2"] = g_NtlTSString;
				sprintf_s( g_NtlTSString, "%d", m_uEvtData.sVisitEvt[2].uiItemTblIdx );
				clProperty.m_defProperty["iidx2"] = g_NtlTSString;
			}
		}
		break;
	}

	sprintf_s( g_NtlTSString, "%d", GetEvtCondDataType() );
	clProperty.m_defProperty["ectype"] = g_NtlTSString;

	switch ( GetEvtCondDataType() )
	{
	case eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtCondData.sAutoEquipItem[0].uiItemTblIdx );
			clProperty.m_defProperty["cd0"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtCondData.sEquipItem[0].uiItemTblIdx );
			clProperty.m_defProperty["cd0"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtCondData.sHaveItem[0].uiItemTblIdx );
			clProperty.m_defProperty["cd0"] = g_NtlTSString;
		}
		break;
	case eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM:
		{
			sprintf_s( g_NtlTSString, "%d", m_uEvtCondData.sHaveQItem[0].uiQItemTblIdx );
			clProperty.m_defProperty["cd0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_uEvtCondData.sHaveQItem[0].nQITemCnt );
			clProperty.m_defProperty["cd1"] = g_NtlTSString;
		}
		break;
	}

	unsigned int uiEvtSubCondDataType = GetEvtSubCondDataType();
	sprintf_s( g_NtlTSString, "%d", uiEvtSubCondDataType );
	clProperty.m_defProperty["esctype"] = g_NtlTSString;

	if ( uiEvtSubCondDataType & eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS )
	{
		sprintf_s( g_NtlTSString, "%d", m_sEvtSubCondData.sLevelDiffLess.nLevelDiff );
		clProperty.m_defProperty["scd0"] = g_NtlTSString;
	}
}
