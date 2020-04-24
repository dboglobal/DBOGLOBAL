// Attr_ACT_SToCEvt.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_SToCEvt.h"
#include "NtlObject.h"


// CAttr_ACT_SToCEvt 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_SToCEvt, CAttr_Page, 1)

CAttr_ACT_SToCEvt::CAttr_ACT_SToCEvt()
	: CAttr_Page(CAttr_ACT_SToCEvt::IDD)
	, m_nCurSelIndex_SToCData( -1 )
	, m_nCurSelIndex_SToCCondData( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwIndex1(0xffffffff)
	, m_nCount1(0)
	, m_dwIndex2(0xffffffff)
	, m_nCount2(0)
	, m_dwIndex3(0xffffffff)
	, m_nCount3(0)
	, m_dwNeedItemIndex1( 0xffffffff )
	, m_fDropPercent1( 1.f )
	, m_dwQTblIdx( 0xffffffff )
	, m_nSubCondLvlDiffLess( 0 )
{
	m_dwWTblIdx1			= 0xffffffff;
	m_dwObjTblIdx1			= 0xffffffff;
	m_dwIndQTextTblIdx1		= 0xffffffff;
	m_dwDiaQTextTblIdx1		= 0xffffffff;
	m_dwItemTblIdx1			= 0xffffffff;

	m_dwWTblIdx2			= 0xffffffff;
	m_dwObjTblIdx2			= 0xffffffff;
	m_dwIndQTextTblIdx2		= 0xffffffff;
	m_dwDiaQTextTblIdx2		= 0xffffffff;
	m_dwItemTblIdx2			= 0xffffffff;

	m_dwWTblIdx3			= 0xffffffff;
	m_dwObjTblIdx3			= 0xffffffff;
	m_dwIndQTextTblIdx3		= 0xffffffff;
	m_dwDiaQTextTblIdx3		= 0xffffffff;
	m_dwItemTblIdx3			= 0xffffffff;

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC);
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_NEEDITEM1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_NEEDITEM1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DROPPERCENT1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DROPPERCENT1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QTBLIDX_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QTBLIDX_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_CHECK );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE1_COMBO );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE2_COMBO );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE3_COMBO );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX3_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC);
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_CHECK );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC);
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_CHECK );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC);
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC);
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_OBJECT_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_NEEDITEM1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_NEEDITEM1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DROPPERCENT1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DROPPERCENT1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_CHECK );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QTBLIDX_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QTBLIDX_EDITOR );

	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE1_COMBO );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX1_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX1_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE2_COMBO );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX2_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX2_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE3_COMBO );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_INDTEXT3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_DIATEXT3_EDITOR );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX3_STATIC );
	m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_VISIT].push_back( IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX3_EDITOR );

	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_EDITOR );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX2_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_MAX].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX2_EDITOR );

	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_EDITOR );

	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_EDITOR );

	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_EDITOR );

	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_EDITOR );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX2_STATIC );
	m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM].push_back( IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX2_EDITOR );
}

CAttr_ACT_SToCEvt::~CAttr_ACT_SToCEvt()
{
}

CString CAttr_ACT_SToCEvt::GetPageData( void )
{
	UpdateData( TRUE );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	if ( m_ctrStartBtn.GetCheck() == BST_CHECKED )		strData += PakingPageData( _T("etype"), eSTOC_EVT_TYPE_START );
	else if ( m_ctrEndBtn.GetCheck() == BST_CHECKED )	strData += PakingPageData( _T("etype"), eSTOC_EVT_TYPE_END );

	strData += PakingPageData( _T("eitype"), (int)m_ctrSvrEvtType.GetItemData( m_ctrSvrEvtType.GetCurSel() ) );
	strData += PakingPageData( _T("apptype"), (int)m_ctrApplyTargetType.GetItemData( m_ctrApplyTargetType.GetCurSel() ) );

	strData += PakingPageData( _T("idx0"), m_dwIndex1 );
	strData += PakingPageData( _T("cnt0"), m_nCount1 );
	strData += PakingPageData( _T("idx1"), m_dwIndex2 );
	strData += PakingPageData( _T("cnt1"), m_nCount2 );
	strData += PakingPageData( _T("idx2"), m_dwIndex3 );
	strData += PakingPageData( _T("cnt2"), m_nCount3 );
	strData += PakingPageData( _T("nidx0"), m_dwNeedItemIndex1 );
	strData += PakingPageData( _T("rate0"), m_fDropPercent1 );
	strData += PakingPageData( _T("qtidx"), m_dwQTblIdx );

	strData += PakingPageData( _T("otype0"), (int)m_ctrObjType1.GetItemData( m_ctrObjType1.GetCurSel() ) );
	strData += PakingPageData( _T("widx0"), m_dwWTblIdx1 );
	strData += PakingPageData( _T("tidx0"), m_dwObjTblIdx1 );
	strData += PakingPageData( _T("it0"), m_dwIndQTextTblIdx1 );
	strData += PakingPageData( _T("dt0"), m_dwDiaQTextTblIdx1 );
	strData += PakingPageData( _T("iidx0"), m_dwItemTblIdx1 );

	strData += PakingPageData( _T("otype1"), (int)m_ctrObjType2.GetItemData( m_ctrObjType2.GetCurSel() ) );
	strData += PakingPageData( _T("widx1"), m_dwWTblIdx2 );
	strData += PakingPageData( _T("tidx1"), m_dwObjTblIdx2 );
	strData += PakingPageData( _T("it1"), m_dwIndQTextTblIdx2 );
	strData += PakingPageData( _T("dt1"), m_dwDiaQTextTblIdx2 );
	strData += PakingPageData( _T("iidx1"), m_dwItemTblIdx2 );

	strData += PakingPageData( _T("otype2"), (int)m_ctrObjType3.GetItemData( m_ctrObjType3.GetCurSel() ) );
	strData += PakingPageData( _T("widx2"), m_dwWTblIdx3 );
	strData += PakingPageData( _T("tidx2"), m_dwObjTblIdx3 );
	strData += PakingPageData( _T("it2"), m_dwIndQTextTblIdx3 );
	strData += PakingPageData( _T("dt2"), m_dwDiaQTextTblIdx3 );
	strData += PakingPageData( _T("iidx2"), m_dwItemTblIdx3 );

	strData += PakingPageData( _T("ectype"), (int)m_ctrSvrEvtCondType.GetItemData( m_ctrSvrEvtCondType.GetCurSel() ) );
	strData += PakingPageData( _T("cd0"), m_strCondIndex1 );
	strData += PakingPageData( _T("cd1"), m_strCondIndex2 );

	unsigned int uiSubCondType = 0;
	if ( m_ctrSubCondLvlDiffLess.GetCheck() == BST_CHECKED )
	{
		uiSubCondType |= eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS;

		strData += PakingPageData( _T("scd0"), m_nSubCondLvlDiffLess );
	}

	strData += PakingPageData( _T("esctype"), uiSubCondType );

	return strData;
}

void CAttr_ACT_SToCEvt::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("etype") == strKey )
	{
		eSTOC_EVT_TYPE eType = (eSTOC_EVT_TYPE)atoi( strValue.GetBuffer() );

		if ( eSTOC_EVT_TYPE_START == eType )	m_ctrStartBtn.SetCheck( BST_CHECKED );
		else if ( eSTOC_EVT_TYPE_END == eType )	m_ctrEndBtn.SetCheck( BST_CHECKED );
	}
	else if ( _T("eitype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSvrEvtType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSvrEvtType.GetItemData( i ) == nValue )
			{
				m_ctrSvrEvtType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("apptype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrApplyTargetType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrApplyTargetType.GetItemData( i ) == nValue )
			{
				m_ctrApplyTargetType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx0") == strKey )
	{
		m_dwIndex1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("cnt0") == strKey )
	{
		m_nCount1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("idx1") == strKey )
	{
		m_dwIndex2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("cnt1") == strKey )
	{
		m_nCount2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("idx2") == strKey )
	{
		m_dwIndex3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("cnt2") == strKey )
	{
		m_nCount3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("nidx0") == strKey )
	{
		m_dwNeedItemIndex1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rate0") == strKey )
	{
		m_fDropPercent1 = atof( strValue.GetBuffer() );
	}
	else if ( _T("qtidx") == strKey )
	{
		m_dwQTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("otype0") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrObjType1.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrObjType1.GetItemData( i ) == nValue )
			{
				m_ctrObjType1.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("widx0") == strKey )
	{
		m_dwWTblIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("tidx0") == strKey )
	{
		m_dwObjTblIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("it0") == strKey )
	{
		m_dwIndQTextTblIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dt0") == strKey )
	{
		m_dwDiaQTextTblIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("iidx0") == strKey )
	{
		m_dwItemTblIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("otype1") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrObjType2.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrObjType2.GetItemData( i ) == nValue )
			{
				m_ctrObjType2.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("widx1") == strKey )
	{
		m_dwWTblIdx2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("tidx1") == strKey )
	{
		m_dwObjTblIdx2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("it1") == strKey )
	{
		m_dwIndQTextTblIdx2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dt1") == strKey )
	{
		m_dwDiaQTextTblIdx2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("iidx1") == strKey )
	{
		m_dwItemTblIdx2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("otype2") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrObjType3.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrObjType3.GetItemData( i ) == nValue )
			{
				m_ctrObjType3.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("widx2") == strKey )
	{
		m_dwWTblIdx3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("tidx2") == strKey )
	{
		m_dwObjTblIdx3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("it2") == strKey )
	{
		m_dwIndQTextTblIdx3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dt2") == strKey )
	{
		m_dwDiaQTextTblIdx3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("iidx2") == strKey )
	{
		m_dwItemTblIdx3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ectype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSvrEvtCondType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSvrEvtCondType.GetItemData( i ) == nValue )
			{
				m_ctrSvrEvtCondType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("cd0") == strKey )
	{
		m_strCondIndex1 = strValue.GetBuffer();
	}
	else if ( _T("cd1") == strKey )
	{
		m_strCondIndex2 = strValue.GetBuffer();
	}
	else if ( _T("esctype") == strKey )
	{
		unsigned int uiSubCondType = atoi( strValue.GetBuffer() );

		if ( (uiSubCondType & eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS) == eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS )
		{
			m_ctrSubCondLvlDiffLess.SetCheck( BST_CHECKED );
		}
		else if ( (uiSubCondType & eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS) != eSTOC_EVT_SUB_COND_DATA_FLAG_LEVEL_DIFF_LESS )
		{
			m_ctrSubCondLvlDiffLess.SetCheck( BST_UNCHECKED );
		}
	}
	else if ( _T("scd0") == strKey )
	{
		m_nSubCondLvlDiffLess = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_SToCEvt::InitData_SToCEvtData( void )
{
	m_nCurSelIndex_SToCData = -1;

	UpdateData( FALSE );
}

void CAttr_ACT_SToCEvt::InitData_SToCEvtCondData( void )
{
	m_nCurSelIndex_SToCCondData = -1;

	UpdateData( FALSE );
}

void CAttr_ACT_SToCEvt::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_START_CHECK, m_ctrStartBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_END_CHECK, m_ctrEndBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_SVREVTTYPE_COMBO, m_ctrSvrEvtType);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_APPLY_TARGET_COMBO, m_ctrApplyTargetType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_INDEX1_EDITOR, m_dwIndex1);
	DDV_MinMaxUInt(pDX, m_dwIndex1, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_COUNTER1_EDITOR, m_nCount1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_INDEX2_EDITOR, m_dwIndex2);
	DDV_MinMaxUInt(pDX, m_dwIndex2, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_COUNTER2_EDITOR, m_nCount2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_INDEX3_EDITOR, m_dwIndex3);
	DDV_MinMaxUInt(pDX, m_dwIndex3, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_COUNTER3_EDITOR, m_nCount3);
	DDV_MinMaxUInt(pDX, m_dwIndex3, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_NEEDITEM1_EDITOR, m_dwNeedItemIndex1);
	DDV_MinMaxUInt(pDX, m_dwIndex3, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_DROPPERCENT1_EDITOR, m_fDropPercent1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_QTBLIDX_EDITOR, m_dwQTblIdx);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE1_COMBO, m_ctrObjType1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX1_EDITOR, m_dwWTblIdx1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX1_EDITOR, m_dwObjTblIdx1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_INDTEXT1_EDITOR, m_dwIndQTextTblIdx1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_DIATEXT1_EDITOR, m_dwDiaQTextTblIdx1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX1_EDITOR, m_dwItemTblIdx1);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE2_COMBO, m_ctrObjType2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX2_EDITOR, m_dwWTblIdx2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX2_EDITOR, m_dwObjTblIdx2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_INDTEXT2_EDITOR, m_dwIndQTextTblIdx2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_DIATEXT2_EDITOR, m_dwDiaQTextTblIdx2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX2_EDITOR, m_dwItemTblIdx2);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_OBJTYPE3_COMBO, m_ctrObjType3);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_WTBLIDX3_EDITOR, m_dwWTblIdx3);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_OBJTIDX3_EDITOR, m_dwObjTblIdx3);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_INDTEXT3_EDITOR, m_dwIndQTextTblIdx3);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_DIATEXT3_EDITOR, m_dwDiaQTextTblIdx3);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_QITEMTIDX3_EDITOR, m_dwItemTblIdx3);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_SVREVTCONDTYPE_COMBO, m_ctrSvrEvtCondType);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_STATIC, m_ctrCondIndex1Static);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX1_EDITOR, m_strCondIndex1);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX2_STATIC, m_ctrCondIndex2Static);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_COND_INDEX2_EDITOR, m_strCondIndex2);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_CHECK, m_ctrSubCondLvlDiffLess);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_STOCEVT_SVREVTSUBCONDTYPE_LVL_DIFF_LESS_EDITOR, m_nSubCondLvlDiffLess);
}

BOOL CAttr_ACT_SToCEvt::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrStartBtn.SetCheck( BST_UNCHECKED );
	m_ctrEndBtn.SetCheck( BST_UNCHECKED );

	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Mob kill") ), eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT );
	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Mob item") ), eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT );
	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Delivery item") ), eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM );
	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Object item") ), eSTOC_EVT_DATA_TYPE_OBJECT_ITEM );
	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Public item") ), eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT );
	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Custom evt cnt") ), eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT );
	m_ctrSvrEvtType.SetItemData( m_ctrSvrEvtType.AddString( _T("Visit") ), eSTOC_EVT_DATA_TYPE_VISIT );
	int nIdx = m_ctrSvrEvtType.AddString( _T("Invalid") );
	m_ctrSvrEvtType.SetItemData( nIdx, eSTOC_EVT_DATA_TYPE_INVALID );
	m_ctrSvrEvtType.SetCurSel( nIdx );

	m_ctrApplyTargetType.SetItemData( m_ctrApplyTargetType.AddString( _T("World") ), 1 );
	nIdx = m_ctrApplyTargetType.AddString( _T("Party") );
	m_ctrApplyTargetType.SetItemData( nIdx, 0 );
	m_ctrApplyTargetType.SetCurSel( nIdx );

	m_ctrObjType1.SetItemData( m_ctrObjType1.AddString( _T("NPC") ), OBJTYPE_NPC );
	m_ctrObjType1.SetItemData( m_ctrObjType1.AddString( _T("Trigger object") ), OBJTYPE_TOBJECT );
	nIdx = m_ctrObjType1.AddString( _T("Invalid") );
	m_ctrObjType1.SetItemData( nIdx, INVALID_OBJTYPE );
	m_ctrObjType1.SetCurSel( nIdx );

	m_ctrObjType2.SetItemData( m_ctrObjType2.AddString( _T("NPC") ), OBJTYPE_NPC );
	m_ctrObjType2.SetItemData( m_ctrObjType2.AddString( _T("Trigger object") ), OBJTYPE_TOBJECT );
	nIdx = m_ctrObjType2.AddString( _T("Invalid") );
	m_ctrObjType2.SetItemData( nIdx, INVALID_OBJTYPE );
	m_ctrObjType2.SetCurSel( nIdx );

	m_ctrObjType3.SetItemData( m_ctrObjType3.AddString( _T("NPC") ), OBJTYPE_NPC );
	m_ctrObjType3.SetItemData( m_ctrObjType3.AddString( _T("Trigger object") ), OBJTYPE_TOBJECT );
	nIdx = m_ctrObjType3.AddString( _T("Invalid") );
	m_ctrObjType3.SetItemData( nIdx, INVALID_OBJTYPE );
	m_ctrObjType3.SetCurSel( nIdx );

	m_ctrSvrEvtCondType.SetItemData( m_ctrSvrEvtCondType.AddString( _T("Auto equip item") ), eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM );
	m_ctrSvrEvtCondType.SetItemData( m_ctrSvrEvtCondType.AddString( _T("Equip item") ), eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM );
	m_ctrSvrEvtCondType.SetItemData( m_ctrSvrEvtCondType.AddString( _T("Have item") ), eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM );
	m_ctrSvrEvtCondType.SetItemData( m_ctrSvrEvtCondType.AddString( _T("Quest item") ), eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM );
	nIdx = m_ctrSvrEvtCondType.AddString( _T("Invalid") );
	m_ctrSvrEvtCondType.SetItemData( nIdx, eSTOC_EVT_COND_DATA_TYPE_INVALID );
	m_ctrSvrEvtCondType.SetCurSel( nIdx );

	m_ctrSubCondLvlDiffLess.SetCheck( BST_UNCHECKED );

	vecdef_ControlList::iterator it = m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].begin();
	for ( ; it != m_defMapList_SToCData[eSTOC_EVT_DATA_TYPE_MAX].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	it = m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_MAX].begin();
	for ( ; it != m_defMapList_SToCCondData[eSTOC_EVT_COND_DATA_TYPE_MAX].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrStocevtSvrevttypeCombo();

	OnCbnSelchangeTsActAttrStocevtSvrevtcondtypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_SToCEvt, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_STOCEVT_START_CHECK, &CAttr_ACT_SToCEvt::OnBnClickedTsActAttrStocevtStartCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_STOCEVT_END_CHECK, &CAttr_ACT_SToCEvt::OnBnClickedTsActAttrStocevtEndCheck)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_STOCEVT_SVREVTTYPE_COMBO, &CAttr_ACT_SToCEvt::OnCbnSelchangeTsActAttrStocevtSvrevttypeCombo)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_STOCEVT_SVREVTCONDTYPE_COMBO, &CAttr_ACT_SToCEvt::OnCbnSelchangeTsActAttrStocevtSvrevtcondtypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_SToCEvt 메시지 처리기입니다.

void CAttr_ACT_SToCEvt::OnBnClickedTsActAttrStocevtStartCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( m_ctrStartBtn.GetCheck() == BST_CHECKED )
	{
		m_ctrEndBtn.SetCheck( BST_UNCHECKED );
	}
	else
	{
		m_ctrEndBtn.SetCheck( BST_CHECKED );
	}
}

void CAttr_ACT_SToCEvt::OnBnClickedTsActAttrStocevtEndCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( m_ctrEndBtn.GetCheck() == BST_CHECKED )
	{
		m_ctrStartBtn.SetCheck( BST_UNCHECKED );
	}
	else
	{
		m_ctrStartBtn.SetCheck( BST_CHECKED );
	}
}

void CAttr_ACT_SToCEvt::OnCbnSelchangeTsActAttrStocevtSvrevttypeCombo()
{
	if ( -1 != m_nCurSelIndex_SToCData )
	{
		DWORD dwData = (DWORD)m_ctrSvrEvtType.GetItemData( m_nCurSelIndex_SToCData );

		if ( m_defMapList_SToCData.end() != m_defMapList_SToCData.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList_SToCData[dwData].begin();
			for ( ; it != m_defMapList_SToCData[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_HIDE );
			}
		}

		InitData_SToCEvtCondData();
	}

	m_nCurSelIndex_SToCData = m_ctrSvrEvtType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex_SToCData )
	{
		DWORD dwData = (DWORD)m_ctrSvrEvtType.GetItemData( m_nCurSelIndex_SToCData );

		if ( m_defMapList_SToCData.end() != m_defMapList_SToCData.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList_SToCData[dwData].begin();
			for ( ; it != m_defMapList_SToCData[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_SHOW );
			}
		}
		else
		{
			m_nCurSelIndex_SToCData = -1;
		}
	}
	else
	{
		m_nCurSelIndex_SToCData = -1;
	}
}

void CAttr_ACT_SToCEvt::OnCbnSelchangeTsActAttrStocevtSvrevtcondtypeCombo()
{
	if ( -1 != m_nCurSelIndex_SToCCondData )
	{
		DWORD dwData = (DWORD)m_ctrSvrEvtCondType.GetItemData( m_nCurSelIndex_SToCCondData );

		if ( m_defMapList_SToCCondData.end() != m_defMapList_SToCCondData.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList_SToCCondData[dwData].begin();
			for ( ; it != m_defMapList_SToCCondData[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_HIDE );
			}
		}

		InitData_SToCEvtData();
	}

	m_nCurSelIndex_SToCCondData = m_ctrSvrEvtCondType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex_SToCCondData )
	{
		DWORD dwData = (DWORD)m_ctrSvrEvtCondType.GetItemData( m_nCurSelIndex_SToCCondData );

		if ( m_defMapList_SToCCondData.end() != m_defMapList_SToCCondData.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList_SToCCondData[dwData].begin();
			for ( ; it != m_defMapList_SToCCondData[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_SHOW );
			}

			switch ( dwData )
			{
			case eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM:
				{
					m_ctrCondIndex1Static.SetWindowText( "Item Index" );
				}
				break;
			case eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM:
				{
					m_ctrCondIndex1Static.SetWindowText( "Item Index" );
				}
				break;
			case eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM:
				{
					m_ctrCondIndex1Static.SetWindowText( "Item Index" );
				}
				break;
			case eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM:
				{
					m_ctrCondIndex1Static.SetWindowText( "QItem Index" );
					m_ctrCondIndex2Static.SetWindowText( "QItem Counter" );
				}
				break;
			}
		}
		else
		{
			m_nCurSelIndex_SToCCondData = -1;
		}
	}
	else
	{
		m_nCurSelIndex_SToCCondData = -1;
	}
}
