#include "precomp_dboclient.h"
#include "WorldMap.h"

// core
#include "NtlMath.h"
#include "NtlDebug.h"

// share
#include "LandMarkTable.h"
#include "WorldTable.h"
#include "WorldMapTable.h"
#include "TextAllTable.h"
#include "ObjectTable.h"
#include "NtlTeleport.h"
#include "NPCTable.h"
#include "TableContainer.h"

// sound
#include "GUISoundDefine.h"
#include "WorldSoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPlVisualManager.h"

// simulation
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLAPI.h"
#include "NtlSLGlobal.h"
#include "NtlOtherParam.h"
#include "NtlSLLogic.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSobGroup.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlWorldConceptScramble.h"

// dbo
#include "DboEvent.h"
#include "DboLogic.h"
#include "DialogDefine.h"
#include "DialogManager.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DboTableInfo.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "QuestSearchGui.h"
#include "DboGlobal.h"
#include "DboPacketGenerator.h"


#define dMAINWORLD_INDEX		1

#define WORLDMAP_UPDATETIME		0.1f
#define dUPDATE_TIME_TEST_CHANGE_ZONE		(3.f)

#define dMAPADJUST_INMAP		10
#define dMAP_WIDTH				806
#define dMAP_HEIGHT				605

#define dMAP_DEFAULT_REDUCED_SCALE	0.125f

#define dMINIMAM_ALPHA			70.f

#define dCHECK_BUTTON_VERT_GAP	(10)

// Landmark
#define dLANDMARK_SIZE_1		0
#define dLANDMARK_SIZE_2		32
#define dLANDMARK_SIZE_3		64
#define dLANDMARK_SIZE_4		128

// Warfog
#define dWARFOG_SCHEDULE_WAIT_OPEN_DIALOG	3.f		///< 워포그가 밝혀지기전 월드맵이 뜨기까지의 시간
#define dWARFOG_SCHEDULE_DELAY				.5f		///< 월드맵이 뜨고 워포그가 사라지기 전의 지연시간
#define dWARFOG_SCHEDULE_DISSAPEAR			3.f		///< 워포그가 밝아지는 시간

#define dWARFOG_SCHEDULE_TIME_BY_DISSAPEAR	(dWARFOG_SCHEDULE_WAIT_OPEN_DIALOG + dWARFOG_SCHEDULE_DELAY)
#define dWARFOR_SCHEDULE_TOTAL_TIME			(dWARFOG_SCHEDULE_WAIT_OPEN_DIALOG + dWARFOG_SCHEDULE_DELAY + dWARFOG_SCHEDULE_DISSAPEAR)

#define dWARFOG_NAMEKSIGN_EFFECT			"GME_NamekSign"


CWorldMapGui::CWorldMapGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pDialogName(NULL)
,m_pRegionName(NULL)
,m_pExitButton(NULL)
,m_pDummy(NULL)
,m_byFocusArea(MWFT_NONE)
,m_uiRenderingWorldID(INVALID_WORLDID)
,m_uiRenderingZoneID(INVALID_ZONEID)
,m_FocusZoneID(INVALID_ZONEID)
,m_FocusMapID(INVALID_TBLIDX)
,m_byMapMode(WORLDMAP_TYPE_ZONE)
,m_fElapsedTime(0.0f)
,m_fElapsedTestChangeZone(0.f)
,m_fMapScale(0.0f)
,m_byDboRateType(MAP_DBO_1)
,m_uiFocusLandMarkIndex(INVALID_INDEX)
,m_uiPressedZoneIndex(INVALID_INDEX)
, m_uiPressedMapIndex(INVALID_TBLIDX)
,m_uiActiveWorldID(INVALID_WORLDID)
,m_uiActiveZoneID(INVALID_ZONEID)
,m_bRightMouse(FALSE)
,m_bChangedMap_by_User(FALSE)
,m_iMapStartX(57)
,m_iMapStartY(143)
,m_pQuestSearch(NULL)
{
	m_tScrambleVisible.bScramble					= FALSE;
	m_tScrambleVisible.bShowOurTeam					= TRUE;
	m_tScrambleVisible.bShowOurTeamMiniMap			= TRUE;
	m_tScrambleVisible.bShowOtherTeam				= TRUE;
	m_tScrambleVisible.bShowOtherTeamMiniMap		= TRUE;

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	CObjectTable* pObjectTable = API_GetTableContainer()->GetObjectTable(pAvatarInfo->sCharPf.bindWorldId);	
	if( pObjectTable )
	{
		sOBJECT_TBLDAT* pOBJECT_TBLDAT = reinterpret_cast<sOBJECT_TBLDAT*>(pObjectTable->FindData(pAvatarInfo->sCharPf.bindObjectTblidx));
		if( pOBJECT_TBLDAT )
		{
			m_BindInfo.byBindType		= pAvatarInfo->sCharPf.byBindType;
			m_BindInfo.WorldID			= pAvatarInfo->sCharPf.bindWorldId;
			m_BindInfo.v3Pos.x			= pOBJECT_TBLDAT->vLoc.x;
			m_BindInfo.v3Pos.y			= pOBJECT_TBLDAT->vLoc.y;
			m_BindInfo.v3Pos.z			= pOBJECT_TBLDAT->vLoc.z;
		}
	}


	// Warfog
	for( RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i )
	{
		m_WarFog[i].bShow			= FALSE;
		m_WarFog[i].uiWarfogIndex	= INVALID_SERIAL_ID;
	}

	m_WarFogDisappearEvent.bActiveEffect = FALSE;
	m_WarFogDisappearEvent.uiWarfogIndex = INVALID_SERIAL_ID;
	m_WarFogDisappearEvent.fRemainTime	= 0.f;
	m_WarFogDisappearEvent.fElapsed		= 0.f;
}

CWorldMapGui::~CWorldMapGui()
{	

}

RwBool CWorldMapGui::Create()
{
	NTL_FUNCTION( "CWorldMapGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\WorldMap.srf", "gui\\WorldMap.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_WORLDMAP);

	CRectangle rect;

	// 
	rect.SetRectWH(m_iMapStartX + DBOGUI_DIALOG_TITLE_X, m_iMapStartY + DBOGUI_DIALOG_TITLE_Y, 145, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_WORLDMAP_TITLE"));
	m_pDialogName->Enable(false);

	// 
	m_pRegionName = (gui::CStaticBox*)GetComponent("stbRegionName");

	m_pstbRecommendedLevel = (gui::CStaticBox*)GetComponent("stbRecommendedLevel");


	rect.SetRectWH(0, 0, 200, 14);
	for(int i = 0 ; i < MAX_LANDMARK_NAME ; ++i )
	{
		m_pLandMarkName[i] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER, TRUE );
		m_pLandMarkName[i]->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_pLandMarkName[i]->SetEffectMode(TE_SHADOW);
		m_pLandMarkName[i]->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
		m_pLandMarkName[i]->Enable(false);
		m_pLandMarkName[i]->Show(false);
	}

	m_pcbbSearchWorld = (gui::CComboBox*)GetComponent("cbbSearchWorld");
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxTop"));
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxMid"));
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxButtom"));
	
	m_pcbbSearchZone = (gui::CComboBox*)GetComponent("cbbSearchZone");
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxTop"));
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxMid"));
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxButtom"));

	m_pcbbSearchCity = (gui::CComboBox*)GetComponent("cbbSearchCity");
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxTop"));
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxMid"));
	//m_srfcbbListboxTop.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfcbbListboxButtom"));

	m_pcbbSearchNpc = (gui::CComboBox*)GetComponent("cbbSearchNpc");
	m_pcbbSearchNpc->AddItem(GetDisplayStringManager()->GetString("DST_WORLDMAP_SEARCH_NPC_TITLE"), 0);
	// TO DO: while loop.. I think to add npc search entry
	{

	}




	// 
	m_pAlphaScrollbar = (gui::CScrollBar*)GetComponent( "scbAlphaScroll" );

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CWorldMapGui::OnClicked_CloseButton);

	std::wstring wstrText;

	// "우리 유파 보기 버튼"
	wstrText = GetDisplayStringManager()->GetString("DST_WORLDMAP_SHOW_OUR_GUILD");
	m_pVisibleOurGuildMemberButton = (gui::CButton*)GetComponent( "btnVisibleOurGuildMember" );
	m_pVisibleOurGuildMemberButton->SetToolTip(wstrText);
	m_pVisibleOurGuildMemberButton->SetToggleMode(true);
	m_pVisibleOurGuildMemberButton->SetDown(true);
	m_slotVisibleOurGuildMemberButton = m_pVisibleOurGuildMemberButton->SigToggled().Connect(this, &CWorldMapGui::OnToggle_VisibleOurGuildMemberButton);

	// "우리 유파 미니맵에 보기 버튼"
	wstrText = GetDisplayStringManager()->GetString("DST_WORLDMAP_SHOW_OUR_GUILD_IN_MINIMAP");
	m_pVisibleOurGuildMemberMiniMapButton = (gui::CButton*)GetComponent( "btnVisibleOurGuildMemberMiniMap" );
	m_pVisibleOurGuildMemberMiniMapButton->SetToolTip(wstrText);
	m_pVisibleOurGuildMemberMiniMapButton->SetToggleMode(true);
	m_pVisibleOurGuildMemberMiniMapButton->SetDown(true);
	m_slotVisibleOurGuildMemberMiniMapButton = m_pVisibleOurGuildMemberMiniMapButton->SigToggled().Connect(this, &CWorldMapGui::OnToggle_VisibleOurGuildMemberMiniMapButton);

	// "우리 유파"
	m_pOurGuild = (gui::CStaticBox*)GetComponent( "stbOurGuild" );;
	m_pOurGuild->SetText( GetDisplayStringManager()->GetString("DST_WORLDMAP_SHOW_OUR_GUILD_MEMBER") );


	// "상대 유파 보기 버튼"
	wstrText = GetDisplayStringManager()->GetString("DST_WORLDMAP_SHOW_OTHER_GUILD");
	m_pVisibleOtherGuildMemberButton = (gui::CButton*)GetComponent( "btnVisibleOtherGuildMember" );
	m_pVisibleOtherGuildMemberButton->SetToolTip(wstrText);
	m_pVisibleOtherGuildMemberButton->SetToggleMode(true);
	m_pVisibleOtherGuildMemberButton->SetDown(true);
	m_slotVisibleOtherGuildMemberButton = m_pVisibleOtherGuildMemberButton->SigToggled().Connect(this, &CWorldMapGui::OnToggle_VisibleOtherGuildMemberButton);

	// "상대 유파 미니맵에 보기 버튼"
	wstrText = GetDisplayStringManager()->GetString("DST_WORLDMAP_SHOW_OTHER_GUILD_IN_MINIMAP");
	m_pVisibleOtherGuildMemberMiniMapButton = (gui::CButton*)GetComponent( "btnVisibleOtherGuildMemberMiniMap" );
	m_pVisibleOtherGuildMemberMiniMapButton->SetToolTip(wstrText);
	m_pVisibleOtherGuildMemberMiniMapButton->SetToggleMode(true);
	m_pVisibleOtherGuildMemberMiniMapButton->SetDown(true);
	m_slotVisibleOtherGuildMemberMiniMapButton = m_pVisibleOtherGuildMemberMiniMapButton->SigToggled().Connect(this, &CWorldMapGui::OnToggle_VisibleOtherGuildMemberMiniMapButton);

	// "상대 유파"
	m_pOtherGuild = (gui::CStaticBox*)GetComponent( "stbOtherGuild" );
	m_pOtherGuild->SetText( GetDisplayStringManager()->GetString("DST_WORLDMAP_SHOW_OTHER_GUILD_MEMBER") );	



	// "투명도"
	m_pTransparency = (gui::CStaticBox*)GetComponent( "stbTransparency" );
	m_pTransparency->SetText( GetDisplayStringManager()->GetString("DST_WORLDMAP_TRANSPARENCY") );	

	// 월드맵의 틀
	m_MapFrameUp.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameUL" ) );	
	m_MapFrameUp.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameUC" ) );	
	m_MapFrameUp.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameUR" ) );		

	m_MapFrameLC.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameLC" ) );
	m_MapFrameRC.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameRC" ) );

	m_MapFrameDown.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameBL" ) );	
	m_MapFrameDown.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameBC" ) );	
	m_MapFrameDown.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfFrameBR" ) );	

	// DBO 지도간 비율
	/*m_surDboRate[MAP_DBO_1].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfMapRate1" ) );
	m_surDboRate[MAP_DBO_1_25].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfMapRate125" ) );
	m_surDboRate[MAP_DBO_1_5].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfMapRate15" ) );
	m_surDboRate[MAP_DBO_2].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfMapRate2" ) );
	m_surDboRate[MAP_DBO_4].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfMapRate2"));*/

	// (피아구분)팀을 나누어서 싸우는 경우의 사람들
	m_surCamp[CAMP_PEOPLE_MY_PARTY]		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfCampParty" ) );
	m_surCamp[CAMP_PEOPLE_MY_TEAM]		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfCampMyTeam" ) );
	m_surCamp[CAMP_PEOPLE_EMENY_TEAM]	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfCampEnemy" ) );

	// 도장전 인장
	m_surScrambleSeal[DBO_TEAM_MY_TEAM]		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfSeal_MyTeam" ) );
	m_surScrambleSeal[DBO_TEAM_ENEMY]		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfSeal_Enemy" ) );
	m_surScrambleSeal[DBO_TEAM_NEUTRAILITY]	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfSeal_NoTeam" ) );

	// map surface
	m_surBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBack" ) );	

	//// map surface
	//m_srfMap.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "MapSurface" ) );
	m_srfMap.SetRectWH(0, 0, 1024, 1024);

	//// Bus route surface
	//m_srfBusRoute.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "MapSurface" ) );	
	m_srfBusRoute.SetRectWH(0, 0, 1024, 1024);

	// Avatar Mark
	m_surMarkAvatar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkAvatar" ) );

	// Bus Mark Human
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_North" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_NorthEast" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_EAST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_East" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_SouthEast" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_South" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_South_West" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_WEST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_West" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_Normal_NorthWest" ) );

	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_North" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_NorthEast" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_EAST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_East" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_SouthEast" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_South" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_South_West" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_WEST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_West" ) );
	m_surMarkBus[BUS_SHAPE_HUMAN][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusHuman_GetOn_NorthWest" ) );

	// Bus mark namek
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_North" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_NorthEast" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_EAST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_East" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_SouthEast" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_South" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_South_West" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_WEST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_West" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_Normal_NorthWest" ) );

	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_North" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_NorthEast" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_EAST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_East" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_SouthEast" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_South" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_South_West" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_WEST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_West" ) );
	m_surMarkBus[BUS_SHAPE_NAMEK][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusNamek_GetOn_NorthWest" ) );

	// bus mark majin
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_North" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_NorthEast" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_EAST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_East" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_SouthEast" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_South" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_SOUTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_South_West" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_WEST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_West" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_NORMAL][BUS_DIRECTION_NORTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_Normal_NorthWest" ) );

	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_North" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_NorthEast" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_EAST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_East" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH_EAST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_SouthEast" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_South" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_SOUTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_South_West" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_WEST]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_West" ) );
	m_surMarkBus[BUS_SHAPE_MAJIN][BUS_MARK_GET_ON][BUS_DIRECTION_NORTH_WEST]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "srfBusMajin_GetOn_NorthWest" ) );


	// to do loop 7
	{
		// set surface srfBattleDragonBallIconWorld
	}

	// party member mark
	m_surMarkPartryMember.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkPartyMember" ) );


	// to do mark srfMarkFieldBoss


	// land mark
	m_surLandMark[LMS_TYPE_1].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "LandMarkSurface_1" ) );
	m_surLandMark[LMS_TYPE_2].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "LandMarkSurface_2" ) );
	m_surLandMark[LMS_TYPE_3].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMap.srf", "LandMarkSurface_3" ) );
	m_surLandMark[LMS_TYPE_4].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "LandMarkSurface_4"));

	// Next proceeding quest mark
	m_surNextQuestMark[eQMI_TARGET_TYPE_NPC].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuest" ) );
	m_surNextQuestMark[eQMI_TARGET_TYPE_OBJECT].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srsNextQuestCheck" ) );
	m_surNextQuestMark[eQMI_TARGET_TYPE_POSITION].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuest_Position" ) );

	// Binding mark
	m_surBindMark.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBindPopoStone" ) );


	m_srfQTSlotIcon[0].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon1"));
	m_srfQTSlotIcon[1].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon2"));
	m_srfQTSlotIcon[2].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon3"));
	m_srfQTSlotIcon[3].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon4"));
	m_srfQTSlotIcon[4].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon5"));
	m_srfQTSlotIcon[5].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon6"));
	m_srfQTSlotIcon[6].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon7"));
	m_srfQTSlotIcon[7].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon8"));
	m_srfQTSlotIcon[8].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon9"));
	m_srfQTSlotIcon[9].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIcon10"));

	m_srfQTSlotIconWorld.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfQTSlotIconWorld"));


	m_srfPortal_Man.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfPortal_Man"));
	m_srfPortalUnRegMark.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMap.srf", "srfPortalUnRegMark"));

	m_pflaScanQuest = (gui::CFlash*)GetComponent("flaScanQuest");
	m_pflaScanNpc = (gui::CFlash*)GetComponent("flaScanNpc");
	m_pflaScanRader = (gui::CFlash*)GetComponent("flaScanRader");
	m_pflaScanRaderRange = (gui::CFlash*)GetComponent("flaScanRaderRange");

	//
	m_pstbQuestInfo = (gui::CStaticBox*)GetComponent("stbQuestInfo");
	m_pstbQuestInfo->SetText(GetDisplayStringManager()->GetString("DST_QUESTINFOMATION"));
	m_pstbQuestInfo->Show(false);
	//
	m_pbtnQuestInfoOnOff = (gui::CButton*)GetComponent("btnQuestInfoOnOff");
	m_pbtnQuestInfoOnOff->SetToggleMode(true);
	m_pbtnQuestInfoOnOff->SetToolTip(GetDisplayStringManager()->GetString("DST_WORLDMAP_QUEST_SEARCH_TOOLTIP"));
	m_slotQuestInfoToggle = m_pbtnQuestInfoOnOff->SigToggled().Connect(this, &CWorldMapGui::OnToggle_QuestInfoButton);
	m_pbtnQuestInfoOnOff->Show(false);

	//
	m_pstbPopoStone = (gui::CStaticBox*)GetComponent("stbPopoStone");
	m_pstbPopoStone->SetText(GetDisplayStringManager()->GetString("DST_POPOSTONE_TITLE"));
	//
	m_pbtnPopoStone = (gui::CButton*)GetComponent("btnPopoStone");
	m_pbtnPopoStone->SetToggleMode(true);
	m_pbtnPopoStone->SetToolTip(GetDisplayStringManager()->GetString("DST_WORLDMAP_CHECK_BOX_TOOLTIP"));
	m_slotPopoStoneToggle = m_pbtnPopoStone->SigToggled().Connect(this, &CWorldMapGui::OnToggle_PopoStoneButton);

	//
	m_pstbPortal = (gui::CStaticBox*)GetComponent("stbPortal");
	m_pstbPortal->SetText(GetDisplayStringManager()->GetString("DST_WORLDMAP_PORTAL"));
	//
	m_pbtnPortal = (gui::CButton*)GetComponent("btnPortal");
	m_pbtnPortal->SetToggleMode(true);
	m_pbtnPortal->SetToolTip(GetDisplayStringManager()->GetString("DST_WORLDMAP_CHECK_BOX_TOOLTIP"));
	m_slotPortalToggle = m_pbtnPortal->SigToggled().Connect(this, &CWorldMapGui::OnToggle_PortalButton);

	// 
	m_pBusRoute = (gui::CStaticBox*)GetComponent("stbBusRoute");
	m_pBusRoute->SetText(GetDisplayStringManager()->GetString("DST_WORLDMAP_BUS_ROUTE_PAINT"));
	// 
	m_pBusRouteButton = (gui::CButton*)GetComponent("btnBusRoute");
	m_pBusRouteButton->SetToggleMode(true);
	m_pBusRouteButton->SetToolTip(GetDisplayStringManager()->GetString("DST_WORLDMAP_CHECK_BOX_TOOLTIP"));
	m_slotBusRouteToggle = m_pBusRouteButton->SigToggled().Connect(this, &CWorldMapGui::OnToggle_BusRouteButton);


	// for post render
	m_pDummy = NTL_NEW gui::CComponent(m_pThis, GetNtlGuiManager()->GetSurfaceManager() );


	// default value
	// Show bus routes from the beginning
	m_pBusRouteButton->SetDown(true);

	ShowScrambleCampComponent(false);


	// sig
	m_slotAlphaScrollChanged		= m_pAlphaScrollbar->SigValueChanged().Connect( this, &CWorldMapGui::OnScrollChanged );
	m_slotAlphaScrollSliderMoved	= m_pAlphaScrollbar->SigSliderMoved().Connect( this, &CWorldMapGui::OnScrollChanged );
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CWorldMapGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CWorldMapGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CWorldMapGui::OnMove );
	m_slotResize		= m_pThis->SigResize().Connect( this, &CWorldMapGui::OnResize );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CWorldMapGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CWorldMapGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CWorldMapGui::OnPaint );
	m_slotPostPaint		= m_pDummy->SigPaint().Connect( this, &CWorldMapGui::OnPostPaint );

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg(g_EventEndterWorld);
	LinkMsg(g_EventBindNotify);
	LinkMsg(g_EventMap);
	LinkMsg(g_EventChangeWorldConceptState);
	LinkMsg(g_EventDojoNotify);	

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CWorldMapGui::Destroy()
{
	NTL_FUNCTION("CWorldMapGui::Destroy");

	ClearSeal();
	CheckInfoWindow();
	UnloadQuestSearch();;

	m_srfMap.UnsetTexture();
	m_srfBusRoute.UnsetTexture();

	UnLoadWorldFocus();
	UnloadLandMark();
	UnloadWarFogTexture();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventEndterWorld);
	UnLinkMsg(g_EventBindNotify);
	UnLinkMsg(g_EventMap);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventDojoNotify);

	NTL_DELETE(m_pDialogName);
	for (int i = 0; i < MAX_LANDMARK_NAME; ++i)
	{
		NTL_DELETE(m_pLandMarkName[i]);
	}
	NTL_DELETE(m_pDummy);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CWorldMapGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_WORLDMAP);
}

VOID CWorldMapGui::OnToggle_QuestInfoButton(gui::CComponent * pComponent, bool bToggled)
{
	return VOID();
}

VOID CWorldMapGui::OnToggle_PopoStoneButton(gui::CComponent * pComponent, bool bToggled)
{
	return VOID();
}

VOID CWorldMapGui::OnToggle_PortalButton(gui::CComponent * pComponent, bool bToggled)
{
	return VOID();
}

VOID CWorldMapGui::OnToggle_BusRouteButton(gui::CComponent* pComponent, bool bToggled)
{

}

VOID CWorldMapGui::OnToggle_VisibleOurGuildMemberButton(gui::CComponent* pComponent, bool bToggled)
{
	m_tScrambleVisible.bShowOurTeam = bToggled;
}

VOID CWorldMapGui::OnToggle_VisibleOurGuildMemberMiniMapButton(gui::CComponent* pComponent, bool bToggled)
{
	m_tScrambleVisible.bShowOurTeamMiniMap = bToggled;

	if( m_tScrambleVisible.bShowOurTeamMiniMap )
	{
		CDboEventGenerator::MapEvent(MMT_MINIMAP_SHOW_OUR_TEAM);
	}
	else
	{
		CDboEventGenerator::MapEvent(MMT_MINIMAP_HIDE_OUR_TEAM);
	}
}

VOID CWorldMapGui::OnToggle_VisibleOtherGuildMemberButton(gui::CComponent* pComponent, bool bToggled)
{
	m_tScrambleVisible.bShowOtherTeam = bToggled;
}

VOID CWorldMapGui::OnToggle_VisibleOtherGuildMemberMiniMapButton(gui::CComponent* pComponent, bool bToggled)
{
	m_tScrambleVisible.bShowOtherTeamMiniMap = bToggled;

	if( m_tScrambleVisible.bShowOtherTeamMiniMap )
	{
		CDboEventGenerator::MapEvent(MMT_MINIMAP_SHOW_OTHER_TEAM);
	}
	else
	{
		CDboEventGenerator::MapEvent(MMT_MINIMAP_HIDE_OTHER_TEAM);
	}
}

VOID CWorldMapGui::Update(RwReal fElapsed)
{
	if( m_pQuestSearch )
		m_pQuestSearch->Update(fElapsed);

	UpdateWarfogEffect(fElapsed);

	if( IsShow() == FALSE )
		return;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pAvatar == NULL)
		return;

	// by daneos: remove the if from UpdateChangeZoneMap, to fix avatar mark, quest marks, etc.
	UpdateChangeZoneMap(fElapsed);

	// 연산량을 줄이기 위해 일정시간마다 Update
	if( m_fElapsedTime < WORLDMAP_UPDATETIME )
	{
		m_fElapsedTime += fElapsed;
		return;
	}

	m_fElapsedTime = 0.0f;	

	UpdateAvatarMark(pAvatar);
	UpdatePartyMemberMark(pAvatar);
	UpdateCampPeople(pAvatar);
	UpdateBusPosition();
	UpdateNextQuestMark();	
}

RwBool CWorldMapGui::UpdateChangeZoneMap(RwReal fElapsed)
{
	if( m_bChangedMap_by_User )
		return FALSE;

	// Show other zone maps when switching to another zone in zone map mode
	if( m_byMapMode != WORLDMAP_TYPE_ZONE )
		return FALSE;

	m_fElapsedTestChangeZone += fElapsed;

	if( m_fElapsedTestChangeZone < dUPDATE_TIME_TEST_CHANGE_ZONE )
		return FALSE;

	m_fElapsedTestChangeZone = 0.f;


	RwBool bFoundMapNameCode = TRUE;
	RwUInt8 byTempMapMode = WORLDMAP_TYPE_ZONE;
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	if( pWorldEntity == NULL )
	{
		DBO_FAIL("Invalid World entity pointer");
		return FALSE;
	}

	TBLIDX idxAreaInfoIndex = pWorldEntity->GetMapNameCode(pAvatar->GetPosition());

	if( idxAreaInfoIndex == 0xffffffff )
	{
		// avooo's commnet : 아마도 맵네임인덱스가 지정되지 않은 곳일 것이다.
		// 월드 모드로 열어주자
		idxAreaInfoIndex	= 200100000;
		byTempMapMode		= WORLDMAP_TYPE_WORLD;

		bFoundMapNameCode	= FALSE;
	}

	RwUInt32 iTemp			= idxAreaInfoIndex/1000;
	RwUInt32 iTemp2			= iTemp/100*100;
	RwUInt32 iRenderingZoneID = iTemp - iTemp2;

	if( iRenderingZoneID == m_uiRenderingZoneID )
		return FALSE;	

	if( bFoundMapNameCode )
	{
		sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);
		if( !pWORLD_MAP_TBLDAT )
			return FALSE;

		if( INVALID_WORLDMAP_TYPE == pWORLD_MAP_TBLDAT->byMapType )
			return FALSE;

		m_byMapMode = pWORLD_MAP_TBLDAT->byMapType;
	}

	m_byMapMode				= byTempMapMode;
	m_uiRenderingZoneID		= iRenderingZoneID;
	m_uiFocusLandMarkIndex	= INVALID_INDEX;

	AnalysisAreaInfo();
	return TRUE;
}

VOID CWorldMapGui::UpdateWarfogEffect(RwReal fElapsed)
{
	if( m_WarFogDisappearEvent.uiWarfogIndex != INVALID_SERIAL_ID )
	{
		if( m_WarFogDisappearEvent.fElapsed < dWARFOG_SCHEDULE_WAIT_OPEN_DIALOG ||
			m_WarFogDisappearEvent.bActiveEffect )
		{
			// 월드맵이 열릴 때 지도, 워포그, 랜드마크 등의 텍스처를 디스크에서 읽기에 느리다.
			// SwitchDialog에서 텍스처를 모두 로딩한 후 m_WarFogDisappearEvent.bActiveEffect이 true가 된다
			// 따라서, 텍스처 로딩 중 공연히 워포그 이벤트 업데이트 시간이 흐르지 않는다
			m_WarFogDisappearEvent.fElapsed += fElapsed;
		}

		if( m_WarFogDisappearEvent.fElapsed >= m_WarFogDisappearEvent.fRemainTime )
		{
			m_WarFogDisappearEvent.fElapsed = m_WarFogDisappearEvent.fRemainTime;
		}

		if( m_WarFogDisappearEvent.fElapsed >= dWARFOG_SCHEDULE_WAIT_OPEN_DIALOG )
		{
			// 일정 시간이 지나면 자동으로 열린다
			if( GetDialogManager()->IsOpenDialog(DIALOG_WORLDMAP) == FALSE )
				GetDialogManager()->OpenDialog(DIALOG_WORLDMAP);
		}

		if( m_WarFogDisappearEvent.fElapsed >= dWARFOG_SCHEDULE_TIME_BY_DISSAPEAR )
		{
			// 지연 시간이 지나고 워포그가 사라진다
			for(RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i)
			{
				if( m_WarFog[i].bShow &&
					m_WarFog[i].uiWarfogIndex == m_WarFogDisappearEvent.uiWarfogIndex )
				{
					RwReal fDissaperUpdateTime = m_WarFogDisappearEvent.fElapsed - dWARFOG_SCHEDULE_TIME_BY_DISSAPEAR;
					RwUInt8 byAlpha = (RwUInt8)(255.f * ((dWARFOG_SCHEDULE_DISSAPEAR - fDissaperUpdateTime)
						/ dWARFOG_SCHEDULE_DISSAPEAR));

					m_WarFog[i].srfWarFog.SetAlpha(byAlpha);

					break;
				}
			}
		}

		// 워포그 사라지기 이벤트 종료
		if( m_WarFogDisappearEvent.fElapsed == m_WarFogDisappearEvent.fRemainTime )
		{
			for(RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i)
			{
				if( m_WarFog[i].uiWarfogIndex == m_WarFogDisappearEvent.uiWarfogIndex )
				{
					m_WarFog[i].bShow = FALSE;
					m_WarFog[i].srfWarFog.SetAlpha(255);
				}
			}

			m_WarFogDisappearEvent.bActiveEffect	= FALSE;
			m_WarFogDisappearEvent.uiWarfogIndex	= INVALID_SERIAL_ID;
			m_WarFogDisappearEvent.fRemainTime		= 0.f;
			m_WarFogDisappearEvent.fElapsed			= 0.f;			
		}
	}
}

VOID CWorldMapGui::UpdateAvatarMark(CNtlSobAvatar* pAvatar)
{
	if( !pAvatar )
		return;

	// Position
	RwInt32 iAvatarMarkX, iAvatarMarkY;
	GetAvatarMarkPosition(iAvatarMarkX, iAvatarMarkY, pAvatar);
	m_surMarkAvatar.SetCenterPosition(iAvatarMarkX, iAvatarMarkY);

	// Direction
	RwV3d& v3AvatarDir = pAvatar->GetDirection();
	m_surMarkAvatar.SetZRot( GetAngle(v3AvatarDir) );
}

VOID CWorldMapGui::UpdatePartyMemberMark(CNtlSobAvatar* pAvatar)
{
	if( m_tScrambleVisible.bScramble )
		return;

	if( !pAvatar )
		return;

	CNtlParty* pParty = pAvatar->GetParty();
	RwV3d v3MemberPos;
	RwUInt8 byMember = 0;
	COMMUNITY_ITER it_party = pParty->Begin();

	for( RwUInt8 i = 0 ; i < (NTL_MAX_MEMBER_IN_PARTY - 1) ; ++i )
		m_aPartyMember[i].bShow = FALSE;

	for( ; it_party != pParty->End() ; ++it_party )
	{
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it_party );

		if( pMember->hSerial == pAvatar->GetSerialID() )
			continue;

		if( INVALID_SERIAL_ID != pMember->hBusSerial )
			continue;

		RwUInt32 uiActiveWorldId = Logic_GetActiveWorldId(); 
		if( pMember->uiWorldID != uiActiveWorldId )
			continue;

		if( m_uiRenderingWorldID != uiActiveWorldId )
			continue;

		v3MemberPos = pMember->vPos;
		m_aPartyMember[byMember].bShow = TRUE;
		m_aPartyMember[byMember].v2Pos.x = (m_v2MapPos.x - v3MemberPos.x) * m_fMapScale;
		m_aPartyMember[byMember].v2Pos.y = (m_v2MapPos.y - v3MemberPos.z) * m_fMapScale;
		m_aPartyMember[byMember].pwcName = pMember->wszMemberName;

		++byMember;
	}
}

VOID CWorldMapGui::UpdateCampPeople(CNtlSobAvatar* pAvatar)
{
	m_listCamp.clear();

	if( !pAvatar )
		return;

	CNtlWorldConceptScramble* pWorldConcept = reinterpret_cast<CNtlWorldConceptScramble*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DOJO_SCRAMBLE) );
	if( !pWorldConcept )
		return;

	UpdateScrambleSeal();


	CNtlGuild*		pGuild				= pAvatar->GetGuild();
	const WCHAR*	pwcAvatarGuildName	= pGuild->GetGuildName();

	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
	if( !pSobGroup )
		return;

	CNtlSobGroup::MapObject::iterator it;
	for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
	{
		CNtlSobPlayer*		pSobPlayer		= reinterpret_cast<CNtlSobPlayer*>( it->second );
		CNtlSobPlayerAttr*	pSobPlayerAttr	= reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );
		const WCHAR*		pwcGuildName	= pSobPlayer->GetGuildName();
		RwV3d				v3Pos			= pSobPlayer->GetPosition();

		sCAMP_PEOPLE tCAMP_PEOPLE;

		GetMapPos_from_RealPos(v3Pos.x, v3Pos.z, tCAMP_PEOPLE.v2Pos.x, tCAMP_PEOPLE.v2Pos.y);

		if( FALSE == IsinMap(tCAMP_PEOPLE.v2Pos.x, tCAMP_PEOPLE.v2Pos.y) )
			continue;

		tCAMP_PEOPLE.pwcName		= pSobPlayerAttr->GetName();

		if( pWorldConcept->IsEnemy(pwcAvatarGuildName, pwcGuildName) )
		{
			if( !m_tScrambleVisible.bShowOtherTeam )
				continue;

			tCAMP_PEOPLE.ePeopleType = CAMP_PEOPLE_EMENY_TEAM;
		}
		else
		{
			if( !m_tScrambleVisible.bShowOurTeam )
				continue;

			if( Logic_IsMyPartyMember( pSobPlayer->GetSerialID() ) )
				tCAMP_PEOPLE.ePeopleType = CAMP_PEOPLE_MY_PARTY;
			else
				tCAMP_PEOPLE.ePeopleType = CAMP_PEOPLE_MY_TEAM;
		}

		m_listCamp.push_back(tCAMP_PEOPLE);
	}	
}

VOID CWorldMapGui::UpdateBusPosition()
{
	m_mapBusPos.clear();

	if(m_byMapMode != WORLDMAP_TYPE_ZONE && m_byMapMode != WORLDMAP_TYPE_ZONE2)
		return;

	if( m_uiActiveWorldID != m_uiRenderingWorldID )
		return;

	if( m_uiActiveZoneID != m_uiRenderingZoneID )
		return;

	CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();

	// 아바타 주위의 Sob 객체중 버스 NPC 업데이트
	CNtlSobGroup::MapObject::iterator it_sobGroup;
	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_NPC );
	if( pSobGroup )
	{
		for( it_sobGroup = pSobGroup->GetObjects().begin() ; it_sobGroup != pSobGroup->GetObjects().end() ; ++it_sobGroup )
		{
			CNtlSobNpc* pSobNPC = reinterpret_cast<CNtlSobNpc*>( it_sobGroup->second );
			if( !pSobNPC )
			{
				DBO_FAIL("Not exist NPC sob object");
				continue;
			}

			CNtlSobNpcAttr* pSobNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSobNPC->GetSobAttr() );
			sNPC_TBLDAT* pNPC_TBLDAT = pSobNPCAttr->GetNpcTbl();

			if( NPC_JOB_BUS != pNPC_TBLDAT->byJob )
				continue;


			sBusPosition tBusPos;
			RwV3d& v3BusPos = pSobNPC->GetPosition();

			const sBusRoute* pBusRoute = pOtherParam->GetBusRoute(pSobNPC->GetSerialID());
			if( !pBusRoute )
				continue;

			tBusPos.hBus			= pSobNPC->GetSerialID();
			tBusPos.eBusShapeType	= pBusRoute->eBusShapeType;
			tBusPos.eBusDirection	= GetBusDirectionType( GetAngle( pSobNPC->GetDirection() ) );
			GetMapPos_from_RealPos(v3BusPos.x, v3BusPos.z, tBusPos.position.x, tBusPos.position.y);

			m_mapBusPos[pSobNPC->GetSerialID()] = tBusPos;
		}
	}


	// 아바타 주위에 없는 Sob 객체중 버스 NPC 업데이트	
	for( MAP_BUS_ROUTE_ITER it = pOtherParam->GetBusRouteBegin() ; it != pOtherParam->GetBusRouteEnd(); ++it )
	{
		sBusPosition busPosition;
		sBusRoute& rBusRoute = it->second;

		// 아바타 주위(클라이언트 내부 정보)에 있는 정보를 가지고 이미 위치를 업데이트 했다
		BUS_POS_ITER it_BusPos = m_mapBusPos.find(rBusRoute.hBus);
		if( it_BusPos != m_mapBusPos.end() )
			continue;

		sBusPosition tBusPos;

		tBusPos.hBus			= rBusRoute.hBus;
		tBusPos.eBusShapeType	= rBusRoute.eBusShapeType;
		tBusPos.eBusDirection	= GetBusDirectionType( GetAngle( rBusRoute.v3Dir ) );
		
		GetMapPos_from_RealPos(rBusRoute.v3Pos.x, rBusRoute.v3Pos.z, tBusPos.position.x, tBusPos.position.y);		

		m_mapBusPos[rBusRoute.hBus] = tBusPos;
	}
}

VOID CWorldMapGui::UpdateNextQuestMark()
{
	m_listNextQuest.clear();

	CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
	for( MAP_NEXTQUEST_ITER it = pOtherParam->GetNextQuestBegin() ; it != pOtherParam->GetNextQuestEnd(); ++it )
	{
		sNEXT_QUEST nextQuest;
		sNextQuest& SLNextQuest = it->second;

		nextQuest.WorldID		= SLNextQuest.WorldID;
		nextQuest.eTargetType	= SLNextQuest.eTargetType;
		nextQuest.pwcText		= SLNextQuest.wstrName.c_str();

		switch(m_byMapMode)
		{
		case WORLDMAP_TYPE_CITY:
		case WORLDMAP_TYPE_ZONE:
		case WORLDMAP_TYPE_ZONE2:
			{
				GetMapPos_from_RealPos(SLNextQuest.v3RealPosition.x, SLNextQuest.v3RealPosition.z,
					nextQuest.position.x, nextQuest.position.y);

				if( nextQuest.WorldID == m_uiRenderingWorldID )
				{
					if( IsinMap(nextQuest.position.x, nextQuest.position.y) )
						nextQuest.bShow = SLNextQuest.bShow;
					else
						nextQuest.bShow = FALSE;
				}
				else
					nextQuest.bShow = FALSE;

				break;
			}
		case WORLDMAP_TYPE_WORLD:
			{
				GetMapPos_from_RealPos(SLNextQuest.v3RealPosition.x, SLNextQuest.v3RealPosition.z,
					nextQuest.position.x, nextQuest.position.y);

				if( nextQuest.WorldID == m_uiRenderingWorldID )
					nextQuest.bShow = SLNextQuest.bShow;
				else
					nextQuest.bShow = FALSE;

				break;
			}
		case WORLDMAP_TYPE_INSTANCE_MAP:
			{
				break;
			}
		default:
			{
				DBO_FAIL("Invalid map type : " << m_byMapMode);
				continue;
			}
		}			

		m_listNextQuest.push_back(nextQuest);
	}
}

VOID CWorldMapGui::UpdateBindInfo()
{
	if( m_BindInfo.WorldID != m_uiRenderingWorldID )
	{
		m_BindInfo.bShow = FALSE;
		return;
	}
	else
		m_BindInfo.bShow = TRUE;

	CPos position;
	GetMapPos_from_RealPos(m_BindInfo.v3Pos.x, m_BindInfo.v3Pos.z, position.x, position.y);
	m_surBindMark.SetCenterPosition(position.x, position.y);

	switch(m_byMapMode)
	{
	case WORLDMAP_TYPE_CITY:
	case WORLDMAP_TYPE_ZONE:
	case WORLDMAP_TYPE_ZONE2:
		{
			m_BindInfo.bShow = IsinMap(position.x, position.y);
			break;
		}
	case WORLDMAP_TYPE_WORLD:
	case WORLDMAP_TYPE_INSTANCE_MAP:
		break;
	}
}

VOID CWorldMapGui::UpdateWarFog()
{
	UnloadWarFogTexture();	

	// 존모드에서만 워포그가 나온다
	if( m_byMapMode != WORLDMAP_TYPE_ZONE && m_byMapMode != WORLDMAP_TYPE_ZONE2)
		return;


	sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);
	if( !pWORLD_MAP_TBLDAT )
		return;

	CNtlOtherParam* pOther = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
	char acBuffer[64] = "";

	for(RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i )
	{
		if( pWORLD_MAP_TBLDAT->wWarfog[i] != dINVALID_WARFOG_INDEX )
		{
			// If you are telling or knowing the warfog
			if( pOther->IsExistWarFog_of_Index(pWORLD_MAP_TBLDAT->wWarfog[i]) ||
				pWORLD_MAP_TBLDAT->wWarfog[i] == m_WarFogDisappearEvent.uiWarfogIndex )
			{
				sprintf_s(acBuffer, 64, "fog_%05d", pWORLD_MAP_TBLDAT->wWarfog[i]);

				m_WarFog[i].srfWarFog.SetTexture(Logic_CreateTexture(acBuffer, TEXTTYPE_WARFOG), TRUE);

				if( m_WarFog[i].srfWarFog.GetTexture() )
				{
					m_WarFog[i].bShow			= TRUE;
					m_WarFog[i].uiWarfogIndex	= pWORLD_MAP_TBLDAT->wWarfog[i];
				}
				else
				{
					if( Logic_IsUIDevInfoVisible() )
					{
						// 테이블 상의 워포그의 텍스처 리소스가 존재하지 않는다
						WCHAR awcBuffer[128] = L"";
						wprintf(awcBuffer, 128, L"Not exist warfog file : fog_%05d.dds", pWORLD_MAP_TBLDAT->wWarfog[i]);
						GetAlarmManager()->AlarmMessage(awcBuffer, CAlarmManager::ALARM_TYPE_CHAT_WARN);
					}
				}
			}			
		}
	}
}

VOID CWorldMapGui::UpdateScrambleSeal()
{
	if( !m_tScrambleVisible.bScramble )
		return;

	MAP_SCRAMBLE_SEAL_MARK::iterator it_ScrambleSealMark = m_mapScrambleSealMark.begin();
	for( ; it_ScrambleSealMark != m_mapScrambleSealMark.end() ; ++it_ScrambleSealMark )
	{
		sSCRAMBLE_SEAL_MARK* pSCRAMBLE_SEAL_MARK = it_ScrambleSealMark->second;

		GetMapPos_from_RealPos(pSCRAMBLE_SEAL_MARK->v3Pos.x, pSCRAMBLE_SEAL_MARK->v3Pos.z,
							   pSCRAMBLE_SEAL_MARK->position.x, pSCRAMBLE_SEAL_MARK->position.y);

		if( FALSE == IsinMap(pSCRAMBLE_SEAL_MARK->position.x, pSCRAMBLE_SEAL_MARK->position.y) )
		{
			pSCRAMBLE_SEAL_MARK->bShow = FALSE;
			continue;
		}
		
		pSCRAMBLE_SEAL_MARK->bShow = TRUE;
	}
}

RwBool CWorldMapGui::AnalysisAreaInfo()
{
	m_byFocusArea = MWFT_NONE;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	if( FALSE == LoadingMapSurface(m_byMapMode) )
		return FALSE;

	LoadingLandMark();
	UpdatePartyMemberMark(pAvatar);
	UpdateBusPosition();
	UpdateNextQuestMark();
	UpdateBindInfo();
	UpdateWarFog();
	UpdateAvatarMark(pAvatar);

	return TRUE;
}

RwBool CWorldMapGui::LoadingMapSurface(RwUInt8 byMapMode)
{
	RwChar acPathName[128] = "";
	RwChar acFileName[128] = "";
	CWorldTable* pWorldTable = API_GetTableContainer()->GetWorldTable();
	sWORLD_TBLDAT* pWORLD_TBLDAT = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData( m_uiRenderingWorldID ) );
	
	if(NULL == pWORLD_TBLDAT)/// woosungs_test 
	{
		DBO_WARNING_MESSAGE("CWorldMapGui::LoadingMapSurface(RwUInt8 byMapMode): \
						Not Found pWorldTable->FindData(" << m_uiRenderingWorldID);
		return FALSE;
	}//

	m_srfMap.UnsetTexture();
	m_srfBusRoute.UnsetTexture();


	m_byMapMode = byMapMode;

	::WideCharToMultiByte(GetACP(), 0, pWORLD_TBLDAT->wszResourceFolder, -1, acPathName, 128, NULL, NULL);
	
	switch(m_byMapMode)
	{
		case WORLDMAP_TYPE_WORLD:
		{
			// 월드 이름
			sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);

			if ( NULL == pWORLD_MAP_TBLDAT )
				return FALSE;

			CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
			std::wstring wsWorldName = pMapNameTable->GetZoneName(pWORLD_MAP_TBLDAT->Worldmap_Name);
			m_pRegionName->SetText(wsWorldName.c_str());

			m_pstbRecommendedLevel->Clear();

			// 맵의 좌상단(리터칭된 좌상단)
			m_v2MapPos.x = pWORLD_MAP_TBLDAT->vStandardLoc.x;
			m_v2MapPos.y = pWORLD_MAP_TBLDAT->vStandardLoc.z;

			// 텍스처 이름
			sprintf_s(acFileName, 128, "world");

			// 맵 스케일
			m_fMapScale = (RwReal)pWORLD_MAP_TBLDAT->fWorldmapScale;
		}
		break;
		case WORLDMAP_TYPE_ZONE:
		case WORLDMAP_TYPE_ZONE2:
		{
			// 존 이름
			sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);

			if ( NULL == pWORLD_MAP_TBLDAT )
				return FALSE;

			CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
			std::wstring wsZoneName = pMapNameTable->GetZoneName(pWORLD_MAP_TBLDAT->Worldmap_Name);
			m_pRegionName->SetText(wsZoneName.c_str());

			m_pstbRecommendedLevel->Format(GetDisplayStringManager()->GetString("DST_WORLDMAP_RECOMMENDED_LEVEL"), pWORLD_MAP_TBLDAT->byRecomm_Min_Level, pWORLD_MAP_TBLDAT->byRecomm_Max_Level);

			// 맵의 시작점(좌상단)
			m_v2MapPos.x = pWORLD_MAP_TBLDAT->vStandardLoc.x;
			m_v2MapPos.y = pWORLD_MAP_TBLDAT->vStandardLoc.z;

			// 텍스처 이름
			sprintf_s(acFileName, 128, "%02d", m_uiRenderingZoneID);

			// 맵 스케일
			m_fMapScale = (RwReal)pWORLD_MAP_TBLDAT->fWorldmapScale;
		}
		break;
		case WORLDMAP_TYPE_CITY:
		{
			// 섹션 이름
			sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);

			if ( NULL == pWORLD_MAP_TBLDAT )
				return FALSE;

			CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
			std::wstring wsAreaName = pMapNameTable->GetSectorName(pWORLD_MAP_TBLDAT->Worldmap_Name);
			m_pRegionName->SetText(wsAreaName.c_str());

			m_pstbRecommendedLevel->Clear();

			// 맵의 시작점(좌상단)
			m_v2MapPos.x = pWORLD_MAP_TBLDAT->vStandardLoc.x;
			m_v2MapPos.y = pWORLD_MAP_TBLDAT->vStandardLoc.z;

			// 텍스처 이름
			sprintf_s(acFileName, 128, "%02d", m_uiRenderingZoneID);

			// 맵 스케일
			m_fMapScale = (RwReal)pWORLD_MAP_TBLDAT->fWorldmapScale;
		}
		break;
		case WORLDMAP_TYPE_INSTANCE_MAP:
		{
			// 존 이름
			sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);

			if ( NULL == pWORLD_MAP_TBLDAT )
				return FALSE;

			m_pstbRecommendedLevel->Clear();

			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pAvatar )
			{
				CNtlDojo*		pDojo			= pAvatar->GetDojo();
				sSCRAMBLE_TEAM*	pSCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
				if( pSCRAMBLE_TEAM )
				{
					std::wstring wstrDojoName = pSCRAMBLE_TEAM->awcGuildName;
					wstrDojoName += L" ";
					wstrDojoName += GetDisplayStringManager()->GetString("DST_DOJO");
					m_pRegionName->SetText( wstrDojoName.c_str() );					
				}
			}

			// 맵의 시작점(좌상단)
			m_v2MapPos.x = pWORLD_MAP_TBLDAT->vStandardLoc.x;
			m_v2MapPos.y = pWORLD_MAP_TBLDAT->vStandardLoc.z;

			// 텍스처 이름
			sprintf_s(acFileName, 128, "%02d", m_uiRenderingZoneID);

			// 맵 스케일
			m_fMapScale = (RwReal)pWORLD_MAP_TBLDAT->fWorldmapScale;
		}
		break;

		default:
		{
			DBO_FAIL("Invalid map mode: " << (int)m_byMapMode);
			return FALSE;
		}
	}
	//DBO_WARNING_MESSAGE("acPathName: " << acPathName << " acFileName: " << acFileName);
	RwReal fReducedScale = m_fMapScale/dMAP_DEFAULT_REDUCED_SCALE;

	if( 1.f  <= fReducedScale || WORLDMAP_TYPE_WORLD == m_byMapMode )
	{
		m_byDboRateType = MAP_DBO_1;
	}
	else if( fReducedScale == 1.25f )
	{
		m_byDboRateType = MAP_DBO_1_25;
	}
	else if( fReducedScale == 1.5f )
	{
		m_byDboRateType = MAP_DBO_1_5;
	}
	else if( fReducedScale == 2.f )
	{
		m_byDboRateType = MAP_DBO_2;
	}
	else if (fReducedScale == 4.f)
	{
		m_byDboRateType = MAP_DBO_4;
	}
	else
	{
		m_byDboRateType = MAP_DBO_1;

		if( WORLDMAP_TYPE_ZONE == m_byMapMode )
		{
			DBO_FAIL("Invalid world map ratio. scale: " << fReducedScale);
		}		
	}

	gui::CTexture* pMapTexture = Logic_CreateTexture(acFileName, TEXTYPE_MAP, acPathName);
	if( !pMapTexture )
		return FALSE;

	m_srfMap.SetTexture(pMapTexture, TRUE);

	// If you have a bus route, read it.
	std::string strBusRouteName = acFileName;
	strBusRouteName += "_bus";
	gui::CTexture* pBusRouteTexture = Logic_CreateTexture((char*)strBusRouteName.c_str(), TEXTYPE_MAP, acPathName);
	if( pBusRouteTexture )
	{
		m_srfBusRoute.SetTexture(pBusRouteTexture, TRUE);
		ShowBusRouteComponent(true);
	}
	else
	{
		ShowBusRouteComponent(false);
	}


	LoadingWorldFocus();
	CheckInfoWindow();
	return TRUE;
}

VOID CWorldMapGui::LoadingWorldFocus()
{
	UnLoadWorldFocus();

	if( m_uiRenderingWorldID == INVALID_WORLDID )
		return;

	if( m_byMapMode != WORLDMAP_TYPE_WORLD )
		return;


	CRectangle rtScreen = m_pThis->GetScreenRect();


	GetNtlGuiManager()->GetReourceManager()->AddPage("gui\\WorldMapFocus.rsr");
	GetNtlGuiManager()->GetSurfaceManager()->AddPage("gui\\WorldMapFocus.srf");


	// The reference coordinates are the upper left corner of the world map frame.

	m_aMainWorldFocus[MWFT_YAHOI_WEST].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfYahoiWest" ) );
	m_aMainWorldFocus[MWFT_YAHOI_WEST].surface.SetPositionfromParent(159, 105);	
	m_aMainWorldFocus[MWFT_YAHOI_WEST].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_YAHOI_WEST].surface.GetRect(m_aMainWorldFocus[MWFT_YAHOI_WEST].rtClick);
	m_aMainWorldFocus[MWFT_YAHOI_WEST].mapID = 1;
	m_aMainWorldFocus[MWFT_YAHOI_WEST].zoneID = 1;

	m_aMainWorldFocus[MWFT_YAHOI_EAST].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfYahoiEast" ) );
	m_aMainWorldFocus[MWFT_YAHOI_EAST].surface.SetPositionfromParent(263, 69);
	m_aMainWorldFocus[MWFT_YAHOI_EAST].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_YAHOI_EAST].surface.GetRect(m_aMainWorldFocus[MWFT_YAHOI_EAST].rtClick);
	m_aMainWorldFocus[MWFT_YAHOI_EAST].mapID = 1;
	m_aMainWorldFocus[MWFT_YAHOI_EAST].zoneID = 2;

	m_aMainWorldFocus[MWFT_KARIN].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfKarin" ) );
	m_aMainWorldFocus[MWFT_KARIN].surface.SetPositionfromParent(145, 173);
	m_aMainWorldFocus[MWFT_KARIN].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_KARIN].surface.GetRect(m_aMainWorldFocus[MWFT_KARIN].rtClick);
	m_aMainWorldFocus[MWFT_KARIN].mapID = 1;
	m_aMainWorldFocus[MWFT_KARIN].zoneID = 7;

	m_aMainWorldFocus[MWFT_PORUNGAROCKS].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfPorungarocks" ) );
	m_aMainWorldFocus[MWFT_PORUNGAROCKS].surface.SetPositionfromParent(219, 291);
	m_aMainWorldFocus[MWFT_PORUNGAROCKS].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_PORUNGAROCKS].surface.GetRect(m_aMainWorldFocus[MWFT_PORUNGAROCKS].rtClick);
	m_aMainWorldFocus[MWFT_PORUNGAROCKS].mapID = 1;
	m_aMainWorldFocus[MWFT_PORUNGAROCKS].zoneID = 5;

	m_aMainWorldFocus[MWFT_FRANGFRANG].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfFrangFrang" ) );
	m_aMainWorldFocus[MWFT_FRANGFRANG].surface.SetPositionfromParent(161, 292);	
	m_aMainWorldFocus[MWFT_FRANGFRANG].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_FRANGFRANG].surface.GetRect(m_aMainWorldFocus[MWFT_FRANGFRANG].rtClick);
	m_aMainWorldFocus[MWFT_FRANGFRANG].mapID = 1;
	m_aMainWorldFocus[MWFT_FRANGFRANG].zoneID = 3;

	m_aMainWorldFocus[MWFT_MUSH_NORTH].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfMushNorth" ) );
	m_aMainWorldFocus[MWFT_MUSH_NORTH].surface.SetPositionfromParent(395, 176);	
	m_aMainWorldFocus[MWFT_MUSH_NORTH].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_MUSH_NORTH].surface.GetRect(m_aMainWorldFocus[MWFT_MUSH_NORTH].rtClick);
	m_aMainWorldFocus[MWFT_MUSH_NORTH].mapID = 1;
	m_aMainWorldFocus[MWFT_MUSH_NORTH].zoneID = 9;

	m_aMainWorldFocus[MWFT_WESTLAND].surface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "WorldMapFocus.srf", "srfWestland" ) );
	m_aMainWorldFocus[MWFT_WESTLAND].surface.SetPositionfromParent(255, 153);	
	m_aMainWorldFocus[MWFT_WESTLAND].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_WESTLAND].surface.GetRect(m_aMainWorldFocus[MWFT_WESTLAND].rtClick);
	m_aMainWorldFocus[MWFT_WESTLAND].mapID = 1;
	m_aMainWorldFocus[MWFT_WESTLAND].zoneID = 8;

	m_aMainWorldFocus[MWFT_MUSH_SOUTH].surface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMapFocus.srf", "srfMushSouth"));
	m_aMainWorldFocus[MWFT_MUSH_SOUTH].surface.SetPositionfromParent(348, 300);
	m_aMainWorldFocus[MWFT_MUSH_SOUTH].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_MUSH_SOUTH].surface.GetRect(m_aMainWorldFocus[MWFT_MUSH_SOUTH].rtClick);
	m_aMainWorldFocus[MWFT_MUSH_SOUTH].mapID = 1;
	m_aMainWorldFocus[MWFT_MUSH_SOUTH].zoneID = 10;

	m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].surface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMapFocus.srf", "srfPorungarocksSouth"));
	m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].surface.SetPositionfromParent(230, 318);
	m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].surface.GetRect(m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].rtClick);
	m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].mapID = 1;
	m_aMainWorldFocus[MWFT_PORUNGAROCKS_SOUTH].zoneID = 6;

	/*m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].surface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMapFocus.srf", "srfFrangFrangSouth"));
	m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].surface.SetPositionfromParent(176, 396);
	m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].surface.GetRect(m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].rtClick);
	m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].mapID = 1;
	m_aMainWorldFocus[MWFT_FRANGFRANG_SOUTH].zoneID = 4;*/

	m_aMainWorldFocus[MWFT_PAPAYA].surface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WorldMapFocus.srf", "srfPaPaYa"));
	m_aMainWorldFocus[MWFT_PAPAYA].surface.SetPositionfromParent(437, 458);
	m_aMainWorldFocus[MWFT_PAPAYA].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
	m_aMainWorldFocus[MWFT_PAPAYA].surface.GetRect(m_aMainWorldFocus[MWFT_PAPAYA].rtClick);
	m_aMainWorldFocus[MWFT_PAPAYA].mapID = 15;
	m_aMainWorldFocus[MWFT_PAPAYA].zoneID = 12;


	RwUInt8 byAlpha = GetAlpha();

	for( RwUInt8 i = 0 ; i < NUM_MWFT; ++i )
		m_aMainWorldFocus[i].surface.SetAlpha(byAlpha);
}

VOID CWorldMapGui::LoadingLandMark()
{	
	RwChar acFileName[64] = "";
	RwChar acFocusFileName[64] = "";
	RwReal fXPos, fYPos;

	sLAND_MARK_TBLDAT* pLAND_MARK_TBLDAT;
	CLandMarkTable* pLandMarkTable = API_GetTableContainer()->GetLandMarkTable();
	CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlOtherParam* pOtherParam = pAvatar->GetOtherParam();


	// Landmark initialization
	UnloadLandMark();

	for( RwUInt8 i = 0 ; i < MAX_LANDMARK_NAME ; ++i )
		m_pLandMarkName[i]->Clear();


	RwUInt8 byIndex = 0;
	//RwUInt32 uiAreaIndex = 200000000 + m_uiRenderingWorldID*100000 + m_uiRenderingZoneID*1000;
	RwUInt32 uiAreaIndex = 1000 * (m_uiRenderingZoneID + 100 * (m_uiRenderingWorldID + 2000));
	for(CTable::TABLEIT it = pLandMarkTable->Begin(); it != pLandMarkTable->End() ; ++it )
	{
		RwUInt32 uiIndex = it->first;
		uiIndex = 1000 * (uiIndex / 1000);

		if( m_byMapMode != WORLDMAP_TYPE_WORLD )
		{
			if( uiIndex != uiAreaIndex )
				continue;
		}	

		pLAND_MARK_TBLDAT = reinterpret_cast<sLAND_MARK_TBLDAT*>(it->second);
		NTL_ASSERT(LMS_TYPE_1 <= pLAND_MARK_TBLDAT->byIconSize && pLAND_MARK_TBLDAT->byIconSize < NUM_LMT, "CWorldMapGui::LoadingLandMark, Invalid Landmark icon size of index : " << (int)pLAND_MARK_TBLDAT->byIconSize);

		// Even the landmark of the same zone is displayed according to the map mode.
		RwUInt8 byBitFlag = MAKE_BIT_FLAG(pLAND_MARK_TBLDAT->byLandmarkBitflag);

		if( m_byMapMode == WORLDMAP_TYPE_WORLD )
		{
			if( false == BIT_FLAG_TEST(byBitFlag, ( LB_WORLD | LB_WORLDSEC | LB_WORLDZONE | LB_WORLDZONESEC ) ) )
				continue;
		}
		else if( m_byMapMode == WORLDMAP_TYPE_ZONE)
		{
			if( false == BIT_FLAG_TEST(byBitFlag, ( LB_ZONE | LB_ZONESEC | LB_WORLDZONE | LB_WORLDZONESEC ) ) )
				continue;
		}
		else if( m_byMapMode == WORLDMAP_TYPE_CITY)
		{
			if( false == BIT_FLAG_TEST(byBitFlag, ( LB_SECTION | LB_ZONESEC | LB_WORLDSEC | LB_WORLDZONESEC ) ) )
				continue;
		}

		LandMarkInfo landMark;

		fXPos = (m_v2MapPos.x - pLAND_MARK_TBLDAT->LandmarkLoc.x) * m_fMapScale;
		fYPos = (m_v2MapPos.y - pLAND_MARK_TBLDAT->LandmarkLoc.z) * m_fMapScale;

		::WideCharToMultiByte(GetACP(), 0, pLAND_MARK_TBLDAT->wszIconName, -1, acFileName, 64, NULL, NULL);

		// Focusing texture name (append suffix _1 to original file name)
		// Yes) 1000000.png   ->    1000000_1.png
		char* pSeeker = strchr(acFileName, '.');
		strncpy_s(acFocusFileName, 64, acFileName, pSeeker - acFileName);
		sprintf_s(acFocusFileName, 64, "%s%s", acFocusFileName, "_1.png");

		// Save landmark information
		landMark.uiTableIndex = it->first;
		landMark.iPosX = (RwInt32)fXPos + m_iMapStartX;
		landMark.iPosY = (RwInt32)fYPos + m_iMapStartY;
		landMark.pLAND_MARK_TBLDAT = pLAND_MARK_TBLDAT;


		// Show landmark icon
		byBitFlag = MAKE_BIT_FLAG(pLAND_MARK_TBLDAT->byLandmarkDisplayBitFlag);
		if( BIT_FLAG_TEST(byBitFlag, (LDBF_ICON | LDBF_NAMEICON ) ) )
		{
			landMark.bShowLandMark = TRUE;
			landMark.pTexture = Logic_CreateTexture(acFileName, TEXTYPE_LANDMARK);
			landMark.pFocusingTexture = Logic_CreateTexture(acFocusFileName, TEXTYPE_LANDMARK);
		}		
		else
			landMark.bShowLandMark = FALSE;

		// To increase the focus priority that the tooltip is
		if( landMark.pLAND_MARK_TBLDAT->Note == INVALID_INDEX )
			m_listLandMark.push_front(landMark);
		else
			m_listLandMark.push_back(landMark);


		// Show landmark name
		std::wstring wstr;
		byBitFlag = MAKE_BIT_FLAG(pLAND_MARK_TBLDAT->byLandmarkDisplayBitFlag);
		if( BIT_FLAG_TEST(byBitFlag, (LDBF_NAME | LDBF_NAMEICON ) ) )
		{
			if( pLAND_MARK_TBLDAT->wLinkWarfogIdx != dINVALID_WARFOG_INDEX )
			{
				if (IsWarfogActive(pLAND_MARK_TBLDAT->wLinkWarfogIdx))
				{
					continue;
				}
			}

			if( FALSE == pMapNameTable->GetAreaName(pLAND_MARK_TBLDAT->LandmarkName, &wstr) )
			{
				if( FALSE == pMapNameTable->GetSectorName(pLAND_MARK_TBLDAT->LandmarkName, &wstr) )
				{
					if( FALSE == pMapNameTable->GetZoneName(pLAND_MARK_TBLDAT->LandmarkName, &wstr) )
					{
						if( Logic_IsUIDevInfoVisible() )
						{
							// Text table does not exist
							WCHAR awcBuffer[256] = L"";
							swprintf_s(awcBuffer, 256, L"Not Exist Textall_table : %u", pLAND_MARK_TBLDAT->LandmarkName);
							wstr = awcBuffer;
						}
						else
						{
							wstr.clear();
						}
					}
				}					
			}

			m_pLandMarkName[byIndex]->SetText(wstr.c_str());
			m_pLandMarkName[byIndex]->Show(true);

			// 랜드마크 이름 위치
			if( landMark.pLAND_MARK_TBLDAT->byIconSize == 0 )
			{
				SetLandMarkNamePosition(m_pLandMarkName[byIndex], landMark.iPosX , landMark.iPosY, dLANDMARK_SIZE_1);
			}
			else if( landMark.pLAND_MARK_TBLDAT->byIconSize == 1 )
			{
				SetLandMarkNamePosition(m_pLandMarkName[byIndex], landMark.iPosX, landMark.iPosY + dLANDMARK_SIZE_2/2, dLANDMARK_SIZE_2);
			}
			else if( landMark.pLAND_MARK_TBLDAT->byIconSize == 2 )
			{
				SetLandMarkNamePosition(m_pLandMarkName[byIndex], landMark.iPosX, landMark.iPosY + dLANDMARK_SIZE_3/2, dLANDMARK_SIZE_3);
			}
			else if (landMark.pLAND_MARK_TBLDAT->byIconSize == 3)
			{
				SetLandMarkNamePosition(m_pLandMarkName[byIndex], landMark.iPosX, landMark.iPosY + dLANDMARK_SIZE_4 / 2, dLANDMARK_SIZE_4);
			}

			++byIndex;
		}
	}
}

VOID CWorldMapGui::SetLandMarkNamePosition(gui::CStaticBox* pStatic, RwInt32 iX, RwInt32 iY, RwInt32 iLandMarkSize)
{
	RwInt32 iLandMarkNameHalfWidth = pStatic->GetWidth()/2;
	RwInt32 iLandMarkNameHalfHeight = pStatic->GetHeight()/2;


	// 랜드마크 이름이 지도의 하단경계를 벗어나면 랜드마크의 오른쪽 혹은 왼쪽에 이름을 위치시킨다
	if( m_iMapStartY + 600 - dMAPADJUST_INMAP < iY + iLandMarkNameHalfHeight )
	{
		if( iX >= 512 )
			iX = iX - iLandMarkNameHalfWidth - iLandMarkSize/2;
		else
			iX = iX + iLandMarkNameHalfWidth + iLandMarkSize/2;

		iY -= iLandMarkSize/2;
	}


	RwInt32 iCompare, iCompare2;

	// 랜드마크 이름이 지도의 왼쪽경계를 벗어나면
	iCompare = m_iMapStartX + dMAPADJUST_INMAP;
	iCompare2 = iX - iLandMarkNameHalfWidth;
	if( iCompare > iCompare2 )
		iX += iCompare - iCompare2;

	// If the landmark name is outside the right border of the map
	iCompare = m_iMapStartX + 1024 - dMAPADJUST_INMAP;
	iCompare2 = iX + iLandMarkNameHalfWidth;
	if( iCompare < iCompare2 )
		iX -= iCompare2 - iCompare;

	pStatic->SetPosition(iX - iLandMarkNameHalfWidth, iY);
}

sWORLD_MAP_TBLDAT* CWorldMapGui::GetWorldMapTable(RwUInt32 uiWorldID, RwUInt32 uiZoneID)
{
	CWorldMapTable*		pWorldMapTable		= API_GetTableContainer()->GetWorldMapTable();
	CTable::TABLEIT		it					= pWorldMapTable->Begin();

	for( ; it != pWorldMapTable->End() ; ++it )
	{
		sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = reinterpret_cast<sWORLD_MAP_TBLDAT*>(it->second);

		if( uiWorldID	== pWORLD_MAP_TBLDAT->World_Tblidx &&
			uiZoneID	== pWORLD_MAP_TBLDAT->Zone_Tblidx )
			return pWORLD_MAP_TBLDAT;
	}

	return NULL;
}

VOID CWorldMapGui::GetAvatarMarkPosition(RwInt32& iOutputX, RwInt32& iOutputY, CNtlSobAvatar* pAvatar)
{
	if( !pAvatar )
		return;

	RwV3d v3AvatarPos = pAvatar->GetPosition();
	iOutputX = (RwInt32)((m_v2MapPos.x - v3AvatarPos.x) * m_fMapScale) + m_iMapStartX;
	iOutputY = (RwInt32)((m_v2MapPos.y - v3AvatarPos.z) * m_fMapScale) + m_iMapStartY;
}

VOID CWorldMapGui::GetMapPos_from_RealPos(RwReal fX, RwReal fZ, RwInt32& iX, RwInt32& iY)
{
	fX = (m_v2MapPos.x - fX) * m_fMapScale;
	fZ = (m_v2MapPos.y - fZ) * m_fMapScale;

	iX = (RwInt32)fX + m_iMapStartX;
	iY = (RwInt32)fZ + m_iMapStartY;
}

VOID CWorldMapGui::LocateComponent()
{
	CRectangle rect;
	CRectangle rtScreen = m_pThis->GetScreenRect();	
	RwInt32 iAdjustX, iAdjustY;
	RwInt32 iOldMapStartX = m_iMapStartX;
	RwInt32 iOldMapStartY = m_iMapStartY;

	m_iMapStartX = (rtScreen.GetWidth() - dMAP_WIDTH)/2;
	m_iMapStartY = (rtScreen.GetHeight() - dMAP_HEIGHT)/2;

	iAdjustX = m_iMapStartX - iOldMapStartX;
	iAdjustY = m_iMapStartY - iOldMapStartY;

	m_surBack.SetRectWH(0, 0, rtScreen.GetWidth(), rtScreen.GetHeight());

	// 월드맵 프레임
	m_MapFrameUp	.SetPosition(m_iMapStartX, m_iMapStartY);
	m_MapFrameLC	.SetPosition(m_iMapStartX, m_iMapStartY + m_MapFrameUp.GetHeight());
	m_MapFrameRC	.SetPosition(m_iMapStartX + dMAP_WIDTH - m_MapFrameRC.GetWidth() - 1, m_iMapStartY + m_MapFrameUp.GetHeight());
	m_MapFrameDown	.SetPosition(m_iMapStartX, m_iMapStartY + m_MapFrameUp.GetHeight() + m_MapFrameRC.GetHeight());


	m_srfMap.SetPosition(m_iMapStartX + 2, m_iMapStartY);
	m_srfBusRoute.SetPosition(m_iMapStartX + 2, m_iMapStartY);

	for(RwUInt8 i = 0 ; i < NUM_MWFT; ++i)
	{
		m_aMainWorldFocus[i].surface.SetPositionbyParent(rtScreen.left + m_iMapStartX, rtScreen.top + m_iMapStartY);
		m_aMainWorldFocus[i].surface.GetRect(m_aMainWorldFocus[i].rtClick);
	}	

	m_pExitButton		->SetPosition(m_iMapStartX + 785, m_iMapStartY + 5);


	// 유파쟁탈전 체크 버튼
	RwInt32 iCheckButtonWidth = m_pVisibleOtherGuildMemberButton->GetWidth();	
	RwInt32 iCheckPosX = m_iMapStartX + 763;
	RwInt32 iCheckPosY = m_iMapStartY + 551;

	m_pVisibleOtherGuildMemberMiniMapButton	->SetPosition(iCheckPosX, iCheckPosY);

	iCheckPosX -= (iCheckButtonWidth + dCHECK_BUTTON_VERT_GAP);
	m_pVisibleOtherGuildMemberButton		->SetPosition(iCheckPosX, iCheckPosY);

	iCheckPosX -= dCHECK_BUTTON_VERT_GAP + m_pOtherGuild->GetWidth();
	m_pOtherGuild							->SetPosition(iCheckPosX, iCheckPosY - 3);

	
	iCheckPosX = m_iMapStartX + 763;
	iCheckPosY -= dCHECK_BUTTON_VERT_GAP + m_pVisibleOtherGuildMemberButton->GetHeight();
	m_pVisibleOurGuildMemberMiniMapButton->SetPosition(iCheckPosX, iCheckPosY);

	iCheckPosX -= (iCheckButtonWidth + dCHECK_BUTTON_VERT_GAP);
	m_pVisibleOurGuildMemberButton->SetPosition(iCheckPosX, iCheckPosY);

	iCheckPosX -= dCHECK_BUTTON_VERT_GAP + m_pOurGuild->GetWidth();
	m_pOurGuild->SetPosition(iCheckPosX, iCheckPosY - 3);

	
	// title
	m_pDialogName->SetPosition(m_iMapStartX + DBOGUI_DIALOG_TITLE_X, m_iMapStartY + DBOGUI_DIALOG_TITLE_Y);

	// map name
	m_pRegionName->SetPosition(m_iMapStartX + 30, m_iMapStartY + 30);

	// recommended level
	m_pstbRecommendedLevel->SetPosition(m_iMapStartX + 50, m_iMapStartY + m_pRegionName->GetHeight() + 30);

	m_pcbbSearchNpc->SetPosition(m_iMapStartX + 650, m_iMapStartY + 50);

	m_pcbbSearchWorld->SetPosition(m_iMapStartX + 650, m_iMapStartY + 420);
	m_pcbbSearchZone->SetPosition(m_iMapStartX + 650, m_iMapStartY + 450);
	m_pcbbSearchCity->SetPosition(m_iMapStartX + 650, m_iMapStartY + 480);

	m_pTransparency->SetPosition(m_iMapStartX + 650, m_iMapStartY + 579 - m_pAlphaScrollbar->GetHeight() - 7);
	m_pAlphaScrollbar->SetPosition(m_iMapStartX + 650, m_iMapStartY + 579);


	// quest info
	m_pstbQuestInfo->SetPosition(m_iMapStartX + 40, m_iMapStartY + 574);
	m_pbtnQuestInfoOnOff->SetPosition(m_pstbQuestInfo->GetPosition().right + 10, m_iMapStartY + 574 - 2);

	// popo stone
	m_pstbPopoStone->SetPosition(m_pbtnQuestInfoOnOff->GetPosition().right + 10, m_iMapStartY + 574);
	m_pbtnPopoStone->SetPosition(m_pstbPopoStone->GetPosition().right + 10, m_iMapStartY + 574 - 2);

	// portals
	m_pstbPortal->SetPosition(m_pbtnPopoStone->GetPosition().right + 10, m_iMapStartY + 574);
	m_pbtnPortal->SetPosition(m_pstbPortal->GetPosition().right + 10, m_iMapStartY + 574 - 2);

	// bus route
	m_pBusRoute->SetPosition(m_pbtnPortal->GetPosition().right + 10, m_iMapStartY + 574);
	m_pBusRouteButton->SetPosition(m_pBusRoute->GetPosition().right + 10, m_iMapStartY + 574 - 2);

	for( RwUInt8 i = 0 ; i < MAX_LANDMARK_NAME ; ++i )
	{
		rect = m_pLandMarkName[i]->GetPosition();
		m_pLandMarkName[i]->SetPosition(rect.left + iAdjustX, rect.top + iAdjustY);
	}

	LANDMARK_ITER it = m_listLandMark.begin();
	for( ; it != m_listLandMark.end() ; ++it )
	{		
		it->iPosX += iAdjustX;
		it->iPosY += iAdjustY;
	}

	// WarFog : Always have textures in the center of the screen
	for(RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i)
	{
		gui::CSurface* pSurface = m_WarFog[i].srfWarFog.GetSurface();

		// avooo's comment : Texture size is 1024, 768
		pSurface->m_SnapShot.rtRect.left		= (rtScreen.GetWidth() - 1024)/2;
		pSurface->m_SnapShot.rtRect.top			= (rtScreen.GetHeight() - 768)/2;
		pSurface->m_SnapShot.rtRect.right		= pSurface->m_SnapShot.rtRect.left + 1024;
		pSurface->m_SnapShot.rtRect.bottom		= pSurface->m_SnapShot.rtRect.top + 1024;
	}

	UpdateBindInfo();

	if(m_pQuestSearch)
	{
		CRectangle rtMapArea;

		rtMapArea.SetRectWH(m_iMapStartX, m_iMapStartY, dMAP_WIDTH, dMAP_HEIGHT);
		m_pQuestSearch->ResetArea(rtMapArea, m_fMapScale, m_v2MapPos);
	}
}

VOID CWorldMapGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_WORLDMAP )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

RwReal CWorldMapGui::GetAngle(RwV3d v3Dir)
{
	static RwV2d v2North = {0.f, 1.f};
	float fSQRT = sqrt(v3Dir.x*v3Dir.x + v3Dir.z*v3Dir.z);
	RwV2d v2Dir;

	v2Dir.x = v3Dir.x / fSQRT;
	v2Dir.y = v3Dir.z / fSQRT;

	RwReal fTheta = acos( RwV2dDotProduct( &v2North, &v2Dir ) );

	if( v3Dir.x >= 0.0f )
		return NTL_RAD2DEG( fTheta );

	return NTL_RAD2DEG( -fTheta );
}

VOID CWorldMapGui::OnScrollChanged(RwInt32 iOffset)
{
	SetAlpha((RwUInt8)iOffset);
}

VOID CWorldMapGui::OnMouseDown( const CKey& key )
{
	if( m_byMapMode == WORLDMAP_TYPE_INSTANCE_MAP )
		return;

	if( m_WarFogDisappearEvent.bActiveEffect )
		return;

	if( !IsinMap((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		return;

	if( m_pQuestSearch )
		return;

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		//// Landmark click
		//CRectangle rect;
		//LANDMARK_ITER it_landmark = m_listLandMark.begin();
		//for( ; it_landmark != m_listLandMark.end() ; ++it_landmark )
		//{
		//	LandMarkInfo& landMark = *it_landmark;
		//	rect.SetRectWH(landMark.iPosX - dLANDMARK_SIZE_1 / 2, landMark.iPosY - dLANDMARK_SIZE_1 / 2, dLANDMARK_SIZE_1, dLANDMARK_SIZE_1);

		//	if( rect.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		//	{
		//		m_uiPressedZoneIndex = INVALID_INDEX;
		//		m_uiPressedMapIndex = INVALID_TBLIDX;
		//		m_bRightMouse = FALSE;

		//		return;
		//	}
		//}

		// 존 클릭
		m_uiPressedZoneIndex = m_FocusZoneID;
		m_uiPressedMapIndex = m_FocusMapID;
	//	DBO_WARNING_MESSAGE("m_uiPressedZoneIndex: " << m_uiPressedZoneIndex << ", m_uiPressedMapIndex: " << m_uiPressedMapIndex);
		m_bRightMouse = FALSE;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		m_uiPressedZoneIndex = INVALID_ZONEID;
		m_uiPressedMapIndex = dMAINWORLD_INDEX;
		m_bRightMouse = TRUE;
	}
}

VOID CWorldMapGui::OnMouseUp( const CKey& key )
{
	if( m_byMapMode == WORLDMAP_TYPE_INSTANCE_MAP )
		return;

	if( m_WarFogDisappearEvent.bActiveEffect ||	!IsinMap((RwInt32)key.m_fX, (RwInt32)key.m_fY) ||
		m_pQuestSearch )
	{
		m_uiPressedZoneIndex = INVALID_ZONEID;
		m_uiPressedMapIndex = INVALID_TBLIDX;
		m_bRightMouse = FALSE;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_byMapMode != WORLDMAP_TYPE_CITY && m_uiPressedZoneIndex != INVALID_ZONEID )
		{
			TBLIDX index = m_FocusZoneID;
			if( index == m_uiPressedZoneIndex )
			{
				// From world map to zone map, we use coordinates
				if( m_byMapMode == WORLDMAP_TYPE_WORLD)
				{
					m_byMapMode				= WORLDMAP_TYPE_ZONE;
					m_uiRenderingZoneID		= m_uiPressedZoneIndex;
					m_uiRenderingWorldID	= m_uiPressedMapIndex;
					m_bChangedMap_by_User	= TRUE;
					AnalysisAreaInfo();
				}
				/*else if( m_byMapMode == WORLDMAP_TYPE_ZONE )
				{
					m_byMapMode = WORLDMAP_TYPE_ZONE2;
					m_uiRenderingZoneID = m_uiPressedZoneIndex;
					m_uiRenderingWorldID = m_uiPressedMapIndex;

					m_bChangedMap_by_User = TRUE;
					AnalysisAreaInfo();
				}*/
			}
		}
		else
		{
			// Go to the town map with the index linked to the landmark
		}		
	}
	else if( m_bRightMouse && key.m_nID == UD_RIGHT_BUTTON )
	{
		//if (m_byMapMode == WORLDMAP_TYPE_ZONE2)
		//{
		//	// to do
		//	m_uiRenderingZoneID = 1;
		//	m_uiRenderingWorldID = 1;

		//	m_byMapMode = WORLDMAP_TYPE_ZONE;
		//	m_bChangedMap_by_User = TRUE;
		//	AnalysisAreaInfo();
		//}
		/*else*/ if(m_byMapMode != WORLDMAP_TYPE_WORLD)
		{
			m_byMapMode				= WORLDMAP_TYPE_WORLD;
			m_uiRenderingWorldID	= dMAINWORLD_INDEX;
			m_uiRenderingZoneID		= 0;
			m_bChangedMap_by_User	= TRUE;
			AnalysisAreaInfo();
		}
	}

	m_uiPressedZoneIndex = INVALID_ZONEID;
	m_uiPressedMapIndex = INVALID_TBLIDX;
	m_bRightMouse = FALSE;
}

VOID CWorldMapGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	LocateComponent();
	CheckInfoWindow();
}

VOID CWorldMapGui::OnResize( RwInt32 iOldW, RwInt32 iOldH )
{
	LocateComponent();

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	UpdateAvatarMark(pAvatar);
	UpdatePartyMemberMark(pAvatar);
	UpdateBusPosition();
	UpdateNextQuestMark();
}

VOID CWorldMapGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwUInt32 iIndex = 0;
	CRectangle rect;
	CRectangle rtScreen = m_pThis->GetScreenRect();
	MINIMAPINFO_LIST listMarkInfo;
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlParty* pParty = pAvatar->GetParty();

	if( m_tScrambleVisible.bShowOurTeam || m_tScrambleVisible.bShowOtherTeam )
	{
		// (피아구분)팀을 나누어서 싸우는 경우의 사람들
		LIST_CAMP::iterator it_Camp = m_listCamp.begin();
		for( ; it_Camp != m_listCamp.end() ; ++it_Camp )
		{
			sCAMP_PEOPLE tCAMP_PEOPLE = *it_Camp;

			if( tCAMP_PEOPLE.ePeopleType >= NUM_CAMP_PEOPLE )
				continue;

			RwInt32 iWidth = m_surCamp[tCAMP_PEOPLE.ePeopleType].GetWidth();
			RwInt32 iHeight = m_surCamp[tCAMP_PEOPLE.ePeopleType].GetHeight();
			RwInt32 iPosX = (RwInt32)tCAMP_PEOPLE.v2Pos.x - iWidth/2 + m_iMapStartX;
			RwInt32 iPosY = (RwInt32)tCAMP_PEOPLE.v2Pos.y - iHeight/2 + m_iMapStartY;

			if( iPosX <= nX && nX <= (iPosX + iWidth) &&
				iPosY <= nY && nY <= (iPosY + iHeight) )
			{
				sMINIMAPINFO info;

				if( CAMP_PEOPLE_MY_PARTY == tCAMP_PEOPLE.ePeopleType )
					info.iType = MMIT_PARTY;
				else
					info.iType = MMIT_NPC;

				info.wcsString = tCAMP_PEOPLE.pwcName;

				listMarkInfo.push_back(info);
			}	
		}
	}

	if( !m_tScrambleVisible.bScramble )
	{
		// 파티원
		for(RwInt8 i = 0 ; i < (NTL_MAX_MEMBER_IN_PARTY - 1) ; ++i )
		{
			if( !m_aPartyMember[i].bShow )
				continue;

			RwInt32 iWidth = m_surMarkPartryMember.GetWidth();
			RwInt32 iHeight = m_surMarkPartryMember.GetHeight();
			RwInt32 iPosX = (RwInt32)m_aPartyMember[i].v2Pos.x - iWidth/2 + m_iMapStartX;
			RwInt32 iPosY = (RwInt32)m_aPartyMember[i].v2Pos.y - iHeight/2 + m_iMapStartY;

			if( iPosX <= nX && nX <= (iPosX + iWidth) &&
				iPosY <= nY && nY <= (iPosY + iHeight) )
			{
				sMINIMAPINFO info;

				info.iType = MMIT_PARTY;
				info.wcsString = m_aPartyMember[i].pwcName;

				listMarkInfo.push_back(info);
			}	
		}	
	}	

	// 랜드마크
	RwBool bFocusLandMark = FALSE;

	LANDMARK_ITER it_landmark = m_listLandMark.begin();
	for( ; it_landmark != m_listLandMark.end() ; ++it_landmark )
	{
		LandMarkInfo& landMark = *it_landmark;

		if( landMark.pLAND_MARK_TBLDAT->byIconSize == 0 )
			rect.SetRectWH(landMark.iPosX - dLANDMARK_SIZE_1/2, landMark.iPosY - dLANDMARK_SIZE_1/2, dLANDMARK_SIZE_1, dLANDMARK_SIZE_1);
		else if( landMark.pLAND_MARK_TBLDAT->byIconSize == 1 )
			rect.SetRectWH(landMark.iPosX - dLANDMARK_SIZE_2/2, landMark.iPosY - dLANDMARK_SIZE_2/2, dLANDMARK_SIZE_2, dLANDMARK_SIZE_2);
		else if( landMark.pLAND_MARK_TBLDAT->byIconSize == 2 )
			rect.SetRectWH(landMark.iPosX - dLANDMARK_SIZE_3/2, landMark.iPosY - dLANDMARK_SIZE_3/2, dLANDMARK_SIZE_3, dLANDMARK_SIZE_3);
		else if (landMark.pLAND_MARK_TBLDAT->byIconSize == 3)
			rect.SetRectWH(landMark.iPosX - dLANDMARK_SIZE_4 / 2, landMark.iPosY - dLANDMARK_SIZE_4 / 2, dLANDMARK_SIZE_4, dLANDMARK_SIZE_4);

		if( rect.PtInRect(nX, nY) )
		{
			if (landMark.pLAND_MARK_TBLDAT->wLinkWarfogIdx != dINVALID_WARFOG_INDEX)
			{
				if (IsWarfogActive(landMark.pLAND_MARK_TBLDAT->wLinkWarfogIdx))
				{
					bFocusLandMark = FALSE;
					m_uiFocusLandMarkIndex = INVALID_INDEX;
					++iIndex;
					continue;
				}
			}

			// 랜드마크 포커싱
			bFocusLandMark = TRUE;
			m_uiFocusLandMarkIndex = iIndex;
			

			// 툴팁
			if( landMark.pLAND_MARK_TBLDAT->Note != INVALID_INDEX )
			{
				sMINIMAPINFO info;
				CMapNameTextTable* pMapNameTextTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();

				info.iType = MMIT_LANDMARK;
				pMapNameTextTable->GetZoneName(landMark.pLAND_MARK_TBLDAT->Note, &info.wcsString);

				listMarkInfo.push_back(info);
			}
			else
			{
				sMINIMAPINFO info;

				info.iType = MMIT_LANDMARK;
				info.wcsString = GetDisplayStringManager()->GetString("DST_WORLDMAP_CITY_INFO");

				listMarkInfo.push_back(info);
			}

		}

		++iIndex;
	}

	if( !bFocusLandMark )
		m_uiFocusLandMarkIndex = INVALID_INDEX;

	// 다음 진행 퀘스트
	NEXTQUEST_ITER it_nextQuest = m_listNextQuest.begin();
	for( ; it_nextQuest != m_listNextQuest.end() ; ++it_nextQuest )
	{
		sNEXT_QUEST& nextQuest = *it_nextQuest;

		if( nextQuest.bShow )
		{
			RwInt32 iWidth = m_surNextQuestMark[nextQuest.eTargetType].GetWidth();
			RwInt32 iHeight = m_surNextQuestMark[nextQuest.eTargetType].GetHeight();
			RwInt32 iPosX = nextQuest.position.x - iWidth/2;
			RwInt32 iPosY = nextQuest.position.y - iHeight/2;

			if( iPosX <= nX && nX <= (iPosX + iWidth) &&
				iPosY <= nY && nY <= (iPosY + iHeight) )
			{
				sMINIMAPINFO info;

				info.iType = MMIT_QUEST;

				// ex) [metatag =5]재배맨 ==> 재배맨
				const WCHAR* pwcText = wcschr(nextQuest.pwcText, L']');
				if( pwcText )
				{
					info.wcsString = pwcText + 1;
				}
				else
				{
					info.wcsString = nextQuest.pwcText;
				}

				listMarkInfo.push_back(info);
			}			
		}
	}

	// Binding point
	if( m_BindInfo.bShow )
	{
		CRectangle rectBind;

		m_surBindMark.GetRect(rectBind);
		rectBind.SetRect(493, 255, 505, 267);

		if( rectBind.PtInRect(nX, nY) )
		{
			sMINIMAPINFO info;

			info.iType = MMIT_BIND_POS;
			info.wcsString = GetDisplayStringManager()->GetString("DST_WORLDMAP_BIND_POPOICON_TOOLTIP");

			listMarkInfo.push_back(info);
		}
	}

	if( CanRenderBus() )
	{
		CRectangle rect;
		BUS_POS_ITER it_BusPos = m_mapBusPos.begin();
		for( ; it_BusPos != m_mapBusPos.end() ; ++it_BusPos )
		{
			sBusPosition& rBusPos = it_BusPos->second;

			COMMUNITY_ITER it_party = pParty->Begin();
			for( ; it_party != pParty->End() ; ++it_party )
			{
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it_party );
				if( rBusPos.hBus == pMember->hBusSerial )
				{
					m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_GET_ON][rBusPos.eBusDirection].GetRect(rect);
					rect.SetRectWH(rect.left - rtScreen.left,
									rect.top - rtScreen.top,
									m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_GET_ON][rBusPos.eBusDirection].GetWidth(),
									m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_GET_ON][rBusPos.eBusDirection].GetHeight());

					if( rect.PtInRect(nX, nY) )
					{
						sMINIMAPINFO info;

						info.iType = MMIT_PARTY;
						info.wcsString = pMember->wszMemberName;

						listMarkInfo.push_back(info);
					}
				}
			}
		}
	}

	// 도장전 인장
	if( m_tScrambleVisible.bScramble )
	{
		MAP_SCRAMBLE_SEAL_MARK::iterator it_ScrambleSealMark = m_mapScrambleSealMark.begin();
		for( ; it_ScrambleSealMark != m_mapScrambleSealMark.end() ; ++it_ScrambleSealMark )
		{
			sSCRAMBLE_SEAL_MARK* pSCRAMBLE_SEAL_MARK = it_ScrambleSealMark->second;

			if( !pSCRAMBLE_SEAL_MARK->bShow )
				continue;

			RwInt32 iWidth = m_surScrambleSeal[pSCRAMBLE_SEAL_MARK->eTeamState].GetWidth();
			RwInt32 iHeight = m_surScrambleSeal[pSCRAMBLE_SEAL_MARK->eTeamState].GetHeight();
			RwInt32 iPosX = pSCRAMBLE_SEAL_MARK->position.x - iWidth/2;
			RwInt32 iPosY = pSCRAMBLE_SEAL_MARK->position.y - iHeight/2;

			if( iPosX <= nX && nX <= (iPosX + iWidth) &&
				iPosY <= nY && nY <= (iPosY + iHeight) )
			{
				sMINIMAPINFO info;

				info.iType = MMIT_NPC;
				info.wcsString = pSCRAMBLE_SEAL_MARK->wstrName;

				listMarkInfo.push_back(info);
			}	
		}
	}	

	if( listMarkInfo.size() > 0 )
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_MINIMAP,
			rtScreen.left + nX, rtScreen.top + nY, (VOID*)&listMarkInfo, DIALOG_WORLDMAP );
	}
	else
		GetInfoWndManager()->ShowInfoWindow( FALSE );

	if( m_uiRenderingWorldID != INVALID_TBLIDX && m_byMapMode == WORLDMAP_TYPE_WORLD)
	{
		for(RwUInt8 i = 0 ; i < NUM_MWFT ; ++i)
		{
			if( m_aMainWorldFocus[i].rtClick.PtInRect(nX, nY) )
			{
				m_byFocusArea = i;

				if( m_FocusZoneID != m_aMainWorldFocus[m_byFocusArea].zoneID )
				{
					sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_aMainWorldFocus[m_byFocusArea].mapID, m_aMainWorldFocus[m_byFocusArea].zoneID);
					if( pWORLD_MAP_TBLDAT )
					{
						CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
						std::wstring wsWorldName = pMapNameTable->GetZoneName(pWORLD_MAP_TBLDAT->Worldmap_Name);
						m_pRegionName->SetText(wsWorldName.c_str());
						m_pstbRecommendedLevel->Format(GetDisplayStringManager()->GetString("DST_WORLDMAP_RECOMMENDED_LEVEL"), pWORLD_MAP_TBLDAT->byRecomm_Min_Level, pWORLD_MAP_TBLDAT->byRecomm_Max_Level);
					}
				}

				m_FocusZoneID = m_aMainWorldFocus[m_byFocusArea].zoneID;
				m_FocusMapID = m_aMainWorldFocus[m_byFocusArea].mapID;
				
				return;
			}
		}

		if (m_FocusZoneID != INVALID_ZONEID)
		{
			sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);
			CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
			std::wstring wsWorldName = pMapNameTable->GetZoneName(pWORLD_MAP_TBLDAT->Worldmap_Name);

			m_pRegionName->SetText(wsWorldName.c_str());
			m_pstbRecommendedLevel->Clear();
		}

		m_byFocusArea = MWFT_NONE;
		m_FocusZoneID = INVALID_ZONEID;
		m_FocusMapID = INVALID_TBLIDX;
	}
	else
	{
		if (bFocusLandMark == FALSE)
		{
			m_byFocusArea = MWFT_NONE;
			m_FocusZoneID = INVALID_ZONEID;
			m_FocusMapID = INVALID_TBLIDX;
		}
	}
}

VOID CWorldMapGui::OnMouseLeave(gui::CComponent* pComponent)
{	
	if( m_uiRenderingWorldID != INVALID_TBLIDX && m_byMapMode == WORLDMAP_TYPE_WORLD )
	{
		sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);
		if (!pWORLD_MAP_TBLDAT)
			return;

		CMapNameTextTable* pMapNameTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
		std::wstring wsWorldName = pMapNameTable->GetZoneName(pWORLD_MAP_TBLDAT->Worldmap_Name);

		m_pRegionName->SetText(wsWorldName.c_str());
		
		if(pWORLD_MAP_TBLDAT->byRecomm_Min_Level != 255)
			m_pstbRecommendedLevel->Format(GetDisplayStringManager()->GetString("DST_WORLDMAP_RECOMMENDED_LEVEL"), pWORLD_MAP_TBLDAT->byRecomm_Min_Level, pWORLD_MAP_TBLDAT->byRecomm_Max_Level);
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CWorldMapGui::OnPaint()
{
	m_surBack.Render();

	// 지도
	m_srfMap.Render();

	// 월드맵상의 존 포커스
	if( m_byFocusArea != MWFT_NONE  && m_byFocusArea <= NUM_MWFT)
		m_aMainWorldFocus[m_byFocusArea].surface.Render();


	// 버스 노선
	if( CanRenderBusRoute() )
		m_srfBusRoute.Render();


	// 랜드마크
	RwUInt32 uiIndex = 0;
	LANDMARK_ITER it_landmark = m_listLandMark.begin();
	for( ; it_landmark != m_listLandMark.end() ; ++it_landmark, ++uiIndex )
	{
		LandMarkInfo& landMark = *it_landmark;

		if( !landMark.bShowLandMark )
			continue;

		m_surLandMark[landMark.pLAND_MARK_TBLDAT->byIconSize].SetCenterPosition(landMark.iPosX, landMark.iPosY);

		if( uiIndex == m_uiFocusLandMarkIndex )
		{
			m_surLandMark[landMark.pLAND_MARK_TBLDAT->byIconSize].SetTexture(landMark.pFocusingTexture);
		}
		else
		{
			m_surLandMark[landMark.pLAND_MARK_TBLDAT->byIconSize].SetTexture(landMark.pTexture);
		}

		m_surLandMark[landMark.pLAND_MARK_TBLDAT->byIconSize].Render();
	}	


	if( CanRenderBus() )
	{
		CNtlSobAvatar*		pAvatar		= GetNtlSLGlobal()->GetSobAvatar();
		CNtlBeCharData*		pBeData		= reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());
		SCtrlStuff*			pCtrlStuff	= pBeData->GetCtrlStuff();
		SERIAL_HANDLE		hBus_with_Avatar = pCtrlStuff->sRide.hTargetSerialId;
	 	CNtlParty*			pParty		= pAvatar->GetParty();
		RwUInt8				byPartyIndex = 0;

		static SERIAL_HANDLE ahBus_with_Party[NTL_MAX_MEMBER_IN_PARTY];

		for( byPartyIndex = 0 ; byPartyIndex < NTL_MAX_MEMBER_IN_PARTY ; ++byPartyIndex )
			ahBus_with_Party[byPartyIndex] = INVALID_SERIAL_ID;

	
		byPartyIndex = 0;
		COMMUNITY_ITER it_party = pParty->Begin();
		for( ; it_party != pParty->End() ; ++it_party )
		{
			sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it_party );
			ahBus_with_Party[byPartyIndex] = pMember->hBusSerial;
			++byPartyIndex;
		}

		BUS_POS_ITER it_BusPos = m_mapBusPos.begin();
		for( ; it_BusPos != m_mapBusPos.end() ; ++it_BusPos )
		{
			sBusPosition& rBusPos = it_BusPos->second;
			RwBool bGetOnBus = FALSE;

			if( hBus_with_Avatar == rBusPos.hBus )
				bGetOnBus = TRUE;
			else
			{				
				for( byPartyIndex = 0 ; byPartyIndex < NTL_MAX_MEMBER_IN_PARTY ; ++byPartyIndex )
				{
					if( ahBus_with_Party[byPartyIndex] == rBusPos.hBus )
					{
						bGetOnBus = TRUE;
						break;
					}
				}				
			}

			if( bGetOnBus )
			{
				m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_GET_ON][rBusPos.eBusDirection].SetCenterPosition(rBusPos.position.x, rBusPos.position.y);
				m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_GET_ON][rBusPos.eBusDirection].Render(true);
			}
			else
			{
				m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_NORMAL][rBusPos.eBusDirection].SetCenterPosition(rBusPos.position.x, rBusPos.position.y);
				m_surMarkBus[rBusPos.eBusShapeType][BUS_MARK_NORMAL][rBusPos.eBusDirection].Render();
			}
		}
	}

	// WarFog
	for(RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i)
	{
		if( m_WarFog[i].bShow )
			m_WarFog[i].srfWarFog.Render();
	}	

	// 지도틀
	m_MapFrameUp.Render();
	m_MapFrameLC.Render();
	m_MapFrameRC.Render();
	m_MapFrameDown.Render();	
}

VOID CWorldMapGui::OnPostPaint()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	SERIAL_HANDLE hBus_with_Avatar = pCtrlStuff->sRide.hTargetSerialId;

	// 바인드 포인트
	if( m_BindInfo.bShow )
		m_surBindMark.Render();

	// 도장전 인장
	if( m_tScrambleVisible.bScramble )
	{
		MAP_SCRAMBLE_SEAL_MARK::iterator it_ScrambleSealMark = m_mapScrambleSealMark.begin();
		for( ; it_ScrambleSealMark != m_mapScrambleSealMark.end() ; ++it_ScrambleSealMark )
		{
			sSCRAMBLE_SEAL_MARK* pSCRAMBLE_SEAL_MARK = it_ScrambleSealMark->second;

			if( !pSCRAMBLE_SEAL_MARK->bShow )
				continue;

			if( pSCRAMBLE_SEAL_MARK->eTeamState >= NUM_DBO_TEAM )
				continue;

			m_surScrambleSeal[pSCRAMBLE_SEAL_MARK->eTeamState].SetCenterPosition(pSCRAMBLE_SEAL_MARK->position.x, 
				pSCRAMBLE_SEAL_MARK->position.y);
			m_surScrambleSeal[pSCRAMBLE_SEAL_MARK->eTeamState].Render();
		}
	}	

	// 다음 진행 퀘스트
	NEXTQUEST_ITER it_nextQuest = m_listNextQuest.begin();
	for( ; it_nextQuest != m_listNextQuest.end() ; ++it_nextQuest )
	{
		sNEXT_QUEST& nextQuest = *it_nextQuest;

		if( nextQuest.bShow == FALSE )
			continue;

		m_surNextQuestMark[nextQuest.eTargetType].SetCenterPosition(nextQuest.position.x, nextQuest.position.y);
		m_surNextQuestMark[nextQuest.eTargetType].Render();
	}

	// 파티원
	if( !m_tScrambleVisible.bScramble )
	{
		for( RwInt8 i = 0 ; i < (NTL_MAX_MEMBER_IN_PARTY - 1) ; ++i )
		{
			if( !m_aPartyMember[i].bShow )
				continue;

			// 지도 영역 안에 있는지 검사해야 한다
			if( IsinMap((RwInt32)m_aPartyMember[i].v2Pos.x, (RwInt32)m_aPartyMember[i].v2Pos.y) )
			{
				m_surMarkPartryMember.SetCenterPosition((RwInt32)m_aPartyMember[i].v2Pos.x + m_iMapStartX,
					(RwInt32)m_aPartyMember[i].v2Pos.y + m_iMapStartY);

				m_surMarkPartryMember.Render();
			}		
		}	
	}	

	// (피아구분)팀을 나누어서 싸우는 경우의 사람들
	LIST_CAMP::iterator it_Camp = m_listCamp.begin();
	for( ; it_Camp != m_listCamp.end() ; ++it_Camp )
	{
		sCAMP_PEOPLE tCAMP_PEOPLE = *it_Camp;

		if( tCAMP_PEOPLE.ePeopleType >= NUM_CAMP_PEOPLE )
			continue;

		// 지도 영역 안에 있는지 검사해야 한다
		if( IsinMap((RwInt32)tCAMP_PEOPLE.v2Pos.x, (RwInt32)tCAMP_PEOPLE.v2Pos.y) )
		{
			m_surCamp[tCAMP_PEOPLE.ePeopleType].SetCenterPosition((RwInt32)tCAMP_PEOPLE.v2Pos.x, (RwInt32)tCAMP_PEOPLE.v2Pos.y);
			m_surCamp[tCAMP_PEOPLE.ePeopleType].Render();
		}
	}

	// 아바타
	if( INVALID_SERIAL_ID == hBus_with_Avatar ||
		FALSE == CanRenderBusRoute() )
	{
		if( Logic_GetActiveWorldId() == m_uiRenderingWorldID )
		{
			CRectangle rect;
			m_surMarkAvatar.GetRect(rect);

			if( IsinMap((rect.right - rect.left)/2 + rect.left, (rect.bottom - rect.top)/2 + rect.top ) )
				m_surMarkAvatar.Render();
		}
	}

	//if( WORLDMAP_TYPE_ZONE == m_byMapMode )
	//	m_surDboRate[m_byDboRateType].Render();
}

VOID CWorldMapGui::SetAlpha(RwUInt8 byAlpha)
{
	RwUInt8 byClacAlpha = (RwUInt8)(dMINIMAM_ALPHA + (RwUInt8)(((RwReal)byAlpha * (255.f - dMINIMAM_ALPHA)) / 255.f));

	m_pDialogName->SetAlpha(byClacAlpha);
	m_pRegionName->SetAlpha(byClacAlpha);
	m_pstbRecommendedLevel->SetAlpha(byClacAlpha);

	for( RwUInt8 i = 0 ; i < MAX_LANDMARK_NAME ; ++i )
		m_pLandMarkName[i]->SetAlpha(byClacAlpha);

	m_pcbbSearchWorld->SetAlpha(byClacAlpha);
	m_pcbbSearchZone->SetAlpha(byClacAlpha);
	m_pcbbSearchCity->SetAlpha(byClacAlpha);
	m_pcbbSearchNpc->SetAlpha(byClacAlpha);

	m_pflaScanQuest->SetAlpha(byClacAlpha);
	m_pflaScanNpc->SetAlpha(byClacAlpha);
	m_pflaScanRader->SetAlpha(byClacAlpha);
	m_pflaScanRaderRange->SetAlpha(byClacAlpha);

	m_pstbQuestInfo->SetAlpha(byClacAlpha);
	m_pbtnQuestInfoOnOff->SetAlpha(byClacAlpha);

	m_pstbPopoStone->SetAlpha(byClacAlpha);
	m_pbtnPopoStone->SetAlpha(byClacAlpha);

	m_pstbPortal->SetAlpha(byClacAlpha);
	m_pbtnPortal->SetAlpha(byClacAlpha);

	m_pExitButton->SetAlpha(byClacAlpha);

	m_surBack.SetAlpha(byClacAlpha);
	m_srfMap.SetAlpha(byClacAlpha);
	m_srfBusRoute.SetAlpha(byClacAlpha);
	m_MapFrameUp.SetAlpha(byClacAlpha);
	m_MapFrameLC.SetAlpha(byClacAlpha);
	m_MapFrameRC.SetAlpha(byClacAlpha);
	m_MapFrameDown.SetAlpha(byClacAlpha);

	m_surBindMark.SetAlpha(byClacAlpha);
	m_surMarkAvatar.SetAlpha(byClacAlpha);
	m_surMarkPartryMember.SetAlpha(byClacAlpha);	

	for( RwUInt8 i = 0 ; i < NUM_eQMI_TARGET_TYPE ; ++i )
		m_surNextQuestMark[i].SetAlpha(byClacAlpha);

	//for( RwUInt8 i = 0 ; i < NUM_MAP_DBO ; ++i )
	//	m_surDboRate[i].SetAlpha(byClacAlpha);

	for( RwUInt8 i = 0 ; i < NUM_LMT ; ++i )
		m_surLandMark[i].SetAlpha(byClacAlpha);

	for( RwUInt8 i = 0 ; i < NUM_MWFT ; ++i )
		m_aMainWorldFocus[i].surface.SetAlpha(byAlpha);

	if( m_pQuestSearch )
		m_pQuestSearch->GetDialog()->SetAlpha(byClacAlpha);

	for( RwUInt8 i = 0 ; i < NUM_BUS_SHAPE_TYPE ; ++i )
	{
		for( RwUInt8 k = 0 ; k < NUM_BUS_MARK ; ++k )
		{
			for( RwUInt8 j = 0 ; j < NUM_BUS_DIRECTION ; ++j )
				m_surMarkBus[i][k][j].SetAlpha(byClacAlpha);
		}
	}
}

RwUInt8 CWorldMapGui::GetAlpha()
{
	return m_pRegionName->GetAlpha();
}

VOID CWorldMapGui::UnLoadWorldFocus()
{
	for( RwUInt8 i = 0 ; i < NUM_MWFT ; ++i )
	{
		m_aMainWorldFocus[i].surface.UnsetTexture();
		m_aMainWorldFocus[i].rtClick.SetRect(0,0,0,0);
	}

	GetNtlGuiManager()->GetReourceManager()->RemovePage("gui\\WorldMapFocus.rsr");
	GetNtlGuiManager()->GetSurfaceManager()->RemovePage("gui\\WorldMapFocus.srf");
}

VOID CWorldMapGui::UnloadLandMark()
{
	LANDMARK_ITER it_landmark = m_listLandMark.begin();
	for( ; it_landmark != m_listLandMark.end() ; ++it_landmark )
	{
		LandMarkInfo& landMark = *it_landmark;

		if(landMark.pTexture)
			Logic_DeleteTexture( landMark.pTexture );

		if(landMark.pFocusingTexture)
			Logic_DeleteTexture( landMark.pFocusingTexture );
	}
	m_listLandMark.clear();
}

VOID CWorldMapGui::UnloadWarFogTexture()
{
	for(RwUInt8 i = 0 ; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG ; ++i)
	{
		m_WarFog[i].bShow			= FALSE;
		m_WarFog[i].uiWarfogIndex	= INVALID_SERIAL_ID;

		m_WarFog[i].srfWarFog.UnsetTexture();		
	}
}

VOID CWorldMapGui::UnloadQuestSearch()
{
	if( m_pQuestSearch )
	{
		m_pQuestSearch->Destroy();
		NTL_DELETE(m_pQuestSearch);
	}
}

VOID CWorldMapGui::ClearSeal()
{
	MAP_SCRAMBLE_SEAL_MARK::iterator it_ScrambleSealMark = m_mapScrambleSealMark.begin();
	for( ; it_ScrambleSealMark != m_mapScrambleSealMark.end() ; ++it_ScrambleSealMark )
	{
		sSCRAMBLE_SEAL_MARK* pSCRAMBLE_SEAL_MARK = it_ScrambleSealMark->second;
		NTL_DELETE(pSCRAMBLE_SEAL_MARK);
	}
	m_mapScrambleSealMark.clear();
}

RwBool CWorldMapGui::IsinMap(RwInt32 iPosX, RwInt32 iPosY)
{
	// Check if it is in the map area of the screen
	if( (m_iMapStartX+dMAPADJUST_INMAP) <= iPosX && iPosX <= (m_iMapStartX-dMAPADJUST_INMAP+dMAP_WIDTH) &&
		(m_iMapStartY+dMAPADJUST_INMAP) <= iPosY && iPosY <= (m_iMapStartY-dMAPADJUST_INMAP+dMAP_HEIGHT) )
		return TRUE;

	return FALSE;
}

RwBool CWorldMapGui::CanRenderBus()
{
	if( WORLDMAP_TYPE_ZONE != m_byMapMode )
		return FALSE; 

	if( false == m_pBusRouteButton->IsDown() )
		return FALSE;

	return TRUE;
}

RwBool CWorldMapGui::CanRenderBusRoute()
{
	if( WORLDMAP_TYPE_ZONE != m_byMapMode )
		return FALSE; 

	if( false == m_pBusRouteButton->IsDown() )
		return FALSE;

	if( NULL == m_srfBusRoute.GetTexture() )
		return FALSE;

	return TRUE;
}

VOID CWorldMapGui::ShowBusRouteComponent(bool bShow)
{
	m_pBusRoute			->Show(bShow);
	m_pBusRouteButton	->Show(bShow);
}

VOID CWorldMapGui::ShowScrambleCampComponent(bool bShow)
{
	m_pOurGuild								->Show(bShow);
	m_pOtherGuild							->Show(bShow);
	m_pVisibleOurGuildMemberButton			->Show(bShow);
	m_pVisibleOurGuildMemberMiniMapButton	->Show(bShow);
	m_pVisibleOtherGuildMemberButton		->Show(bShow);
	m_pVisibleOtherGuildMemberMiniMapButton	->Show(bShow);
}

CWorldMapGui::eBusDirectionType CWorldMapGui::GetBusDirectionType(RwReal fAngle)
{
	fAngle = (RwReal)((RwInt32)fAngle % 360);

	if( 22.5 >= fAngle && fAngle >= -22.5f )
	{
		return BUS_DIRECTION_NORTH;
	}
	else if( 0.f > fAngle && fAngle >= -67.5f )
	{
		return BUS_DIRECTION_NORTH_EAST;
	}
	else if( 0.f > fAngle && fAngle >= -112.5f )
	{
		return BUS_DIRECTION_EAST;
	}
	else if( 0.f > fAngle && fAngle >= -157.5f )
	{
		return BUS_DIRECTION_SOUTH_EAST;
	}
	else if( fAngle <= -157.5f || fAngle >= 157.5f)
	{
		return BUS_DIRECTION_SOUTH;
	}
	else if( fAngle >= 112.5f )
	{
		return BUS_DIRECTION_SOUTH_WEST;
	}
	else if( fAngle >= 67.5f )
	{
		return BUS_DIRECTION_WEST;
	}

	return BUS_DIRECTION_NORTH_WEST;
}

BOOL CWorldMapGui::IsWarfogActive(WORD wWarfogIndex)
{
	CNtlOtherParam* pOther = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
	return pOther->IsExistWarFog_of_Index(wWarfogIndex);
}

RwInt32 CWorldMapGui::SwitchDialog(bool bOpen)
{
	if( bOpen )	
	{
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
		if( pWorldConcept )
		{
			EWorldPlayConcept ePlayConcept = pWorldConcept->GetConceptType();
			if( ePlayConcept == WORLD_PLAY_TUTORIAL			||
				ePlayConcept == WORLD_PLAY_TIME_MACHINE		||
				ePlayConcept == WORLD_PLAY_RANK_BATTLE		||
				ePlayConcept == WORLD_PLAY_PARTY_DUNGEON	||
				ePlayConcept == WORLD_PLAY_TLQ				||
				ePlayConcept == WORLD_PLAY_CCBD				||
				ePlayConcept == WORLD_PLAY_DWC)
			{
				GetAlarmManager()->AlarmMessage("DST_WORLDMAP_CAN_OPEN_AREA");
				return -1;
			}
		}

		RwBool bFoundMapNameCode = TRUE;
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
		if( pWorldEntity == NULL )
		{
			DBO_FAIL("g_EventEndterWorld, Invalid World entity pointer");
			return -1;
		}

		TBLIDX idxAreaInfoIndex = pWorldEntity->GetMapNameCode(pAvatar->GetPosition());

		if( idxAreaInfoIndex == 0xffffffff )
		{
			// avooo's commnet : Perhaps the map name index is not specified.
			// Let's open in world mode
			idxAreaInfoIndex	= 200100000;
			m_byMapMode			= WORLDMAP_TYPE_WORLD;

			bFoundMapNameCode	= FALSE;
		}

		m_uiRenderingWorldID	= Logic_GetActiveWorldId();
		m_uiFocusLandMarkIndex	= INVALID_INDEX;

		RwUInt32 iTemp			= idxAreaInfoIndex/1000;
		RwUInt32 iTemp2			= iTemp/100*100;
		m_uiRenderingZoneID		= iTemp - iTemp2;

		if( bFoundMapNameCode )
		{
			sWORLD_MAP_TBLDAT* pWORLD_MAP_TBLDAT = GetWorldMapTable(m_uiRenderingWorldID, m_uiRenderingZoneID);
			if( !pWORLD_MAP_TBLDAT )
			{
				m_byMapMode			= INVALID_WORLDMAP_TYPE;
				m_uiRenderingWorldID= INVALID_WORLDID;
				m_uiRenderingZoneID	= INVALID_ZONEID;

				GetAlarmManager()->AlarmMessage("DST_WORLDMAP_CAN_OPEN_AREA");
				return -1;
			}

			if( INVALID_WORLDMAP_TYPE == pWORLD_MAP_TBLDAT->byMapType )
			{
				GetAlarmManager()->AlarmMessage("DST_WORLDMAP_CAN_OPEN_AREA");
				return -1;
			}

			if( WORLDMAP_TYPE_INSTANCE_MAP == pWORLD_MAP_TBLDAT->byMapType )
			{
				if( !pWorldConcept )
				{
					GetAlarmManager()->AlarmMessage("DST_WORLDMAP_CAN_OPEN_AREA");
					return -1;
				}
			}

			m_byMapMode = pWORLD_MAP_TBLDAT->byMapType;
		}
		

		if( FALSE == AnalysisAreaInfo() )
		{
			GetAlarmManager()->AlarmMessage("DST_WORLDMAP_CAN_OPEN_AREA");
			return -1;
		}


		GetIconMoveManager()->IconMoveEnd();

		if( m_WarFogDisappearEvent.uiWarfogIndex != INVALID_SERIAL_ID )
			m_WarFogDisappearEvent.bActiveEffect = TRUE;

		m_bChangedMap_by_User = FALSE;

		m_uiActiveWorldID	= m_uiRenderingWorldID;
		m_uiActiveZoneID	= m_uiRenderingZoneID;

		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
		{
			ShowScrambleCampComponent(true);
		}		
	}
	else
	{
		CheckInfoWindow();
		ShowScrambleCampComponent(false);

		// 워포그가 사라지는 이벤트는 창이 닫히면 바로 종료한다
		m_WarFogDisappearEvent.bActiveEffect = FALSE;
		m_WarFogDisappearEvent.uiWarfogIndex = INVALID_SERIAL_ID;
		m_WarFogDisappearEvent.fRemainTime	= 0.f;
		m_WarFogDisappearEvent.fElapsed		= 0.f;		

		// 월드맵을 그리지 않으면 별도의 텍스처로 분리되어 있는 리소스는 전부 해제한다
		UnLoadWorldFocus();
		UnloadLandMark();
		UnloadWarFogTexture();
		UnloadQuestSearch();

		m_srfMap.UnsetTexture();
		m_srfBusRoute.UnsetTexture();

		m_uiActiveWorldID	= INVALID_WORLDID;
		m_uiActiveZoneID	= INVALID_ZONEID;

		CNtlSLEventGenerator::BusMove(BUS_MOVE_RESET, INVALID_SERIAL_ID, INVALID_TBLIDX, NULL, NULL);
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendBusWorldMapStatus(bOpen);
	Show(bOpen);
	return 1;
}

VOID CWorldMapGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CWorldMapGui::HandleEvents");

	if( msg.Id == g_EventEndterWorld )
	{
		GetDialogManager()->CloseDialog(DIALOG_WORLDMAP);
	}
	else if( msg.Id == g_EventBindNotify )
	{
		CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
		sBindInfo* pBindInfo = pOtherParam->GetBindInfo();
		CObjectTable* pObjectTable = API_GetTableContainer()->GetObjectTable(pBindInfo->BindedWorldID);
		if( pObjectTable == NULL )
		{
			DBO_FAIL("Not exist obejct table of world ID : " << pBindInfo->BindedWorldID);
			NTL_RETURNVOID();
		}

		sOBJECT_TBLDAT* pOBJECT_TBLDAT = reinterpret_cast<sOBJECT_TBLDAT*>(pObjectTable->FindData(pBindInfo->uiBindedObejcTblIdx));
		if( pOBJECT_TBLDAT == NULL )
		{
			DBO_FAIL("Not exist obejct data of world ID, " <<  pBindInfo->BindedWorldID << "  index : " << pBindInfo->uiBindedObejcTblIdx);
			NTL_RETURNVOID();
		}

		// avooo's commnet : 바인드 할 수 있는 지역이라면 무조건 월드 ID와 월드테이블 Index가 동일하다

		m_BindInfo.byBindType		= pBindInfo->byBindType;
		m_BindInfo.WorldID			= pBindInfo->BindedWorldID;
		m_BindInfo.v3Pos.x			= pOBJECT_TBLDAT->vLoc.x;
		m_BindInfo.v3Pos.y			= pOBJECT_TBLDAT->vLoc.y;
		m_BindInfo.v3Pos.z			= pOBJECT_TBLDAT->vLoc.z;		

		if( IsShow() )
			UpdateBindInfo();

		Logic_PlayGUISound(WORLD_SOUND_POPOSTONE);
	}
	else if( msg.Id == g_EventMap )
	{
		SDboEventMap* pEvent = reinterpret_cast<SDboEventMap*>( msg.pData );		

		if( pEvent->iMessage == MMT_WARFOG_UPDATE )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			CNtlOtherParam* pOtherParam = pAvatar->GetOtherParam();
			CNtlSobTriggerObject* pSobTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>(GetNtlSobManager()->GetSobObject(pEvent->uiValue));
			if( pSobTriggerObject == NULL )
			{
				DBO_FAIL("Not exist trigger object of handle : " << pEvent->uiValue);
				NTL_RETURNVOID();
			}

			CNtlSobTriggerObjectAttr* pSobObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>(pSobTriggerObject->GetSobAttr());
			if( pSobObjectAttr == NULL )
			{
				DBO_FAIL("Not exist trigger object attibute of handle : " << pEvent->uiValue);
				NTL_RETURNVOID();
			}

			sOBJECT_TBLDAT* pOBJECT_TBLDAT = reinterpret_cast<sOBJECT_TBLDAT*>(pSobObjectAttr->GetTriggerObjectTbl());
			if( pOBJECT_TBLDAT == NULL)
			{
				DBO_FAIL("Not exist object table ");
				NTL_RETURNVOID();
			}

			if( false == BIT_FLAG_TEST(pOBJECT_TBLDAT->dwFunction, eDBO_TRIGGER_OBJECT_FUNC_NAMEKAN_SIGN) )
			{
				DBO_FAIL("Not namekan sign flag of obejct table index : " << pOBJECT_TBLDAT->tblidx);
				NTL_RETURNVOID();
			}			


			// 워포그 데이터 저장
			pOtherParam->SetWarFolgValue(pOBJECT_TBLDAT->contentsTblidx);

			// 월드상 나메칸 사인에 이펙트를 연출한다
			CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, dWARFOG_NAMEKSIGN_EFFECT);
			if( pPLEntity )
			{
                RwV3d vTriggerObjectPos = pSobTriggerObject->GetPosition();
				pPLEntity->SetPosition(&vTriggerObjectPos);
			}
			else
			{
				DBO_FAIL("Not exist effect of name : " << dWARFOG_NAMEKSIGN_EFFECT);
			}			

			// 월드맵에서 워포그가 사라지는 이펙트
			m_WarFogDisappearEvent.bActiveEffect = FALSE;
			m_WarFogDisappearEvent.uiWarfogIndex = pOBJECT_TBLDAT->contentsTblidx;
			m_WarFogDisappearEvent.fRemainTime	= dWARFOR_SCHEDULE_TOTAL_TIME;
			m_WarFogDisappearEvent.fElapsed		= 0.f;

			// 존 모드에서만 보여준다
			m_byMapMode = WORLDMAP_TYPE_ZONE;
		}
	}
	//else if( msg.Id == g_EventScouter )
	//{
	//	SDboEventScouter* pEvent = reinterpret_cast<SDboEventScouter*>( msg.pData );

	//	if( pEvent->iType == SCOUTER_EVENT_QUEST_SEARCH )
	//	{
	//		// 이미 퀘스트 서치중
	//		if( m_pQuestSearch )
	//			NTL_RETURNVOID();

	//		m_byMapMode = WORLDMAP_TYPE_ZONE;
	//		if( GetDialogManager()->OpenDialog(DIALOG_WORLDMAP) == FALSE )
	//			NTL_RETURNVOID();

	//		m_pQuestSearch = NTL_NEW CQuestSearchGui("QuestSearchGui");
	//		if( !m_pQuestSearch->Create(m_fMapScale) )
	//		{
	//			UnloadQuestSearch();
	//			NTL_RETURNVOID();
	//		}

	//		CRectangle rtMapArea;
	//		RwInt32 iOutputX, iOutputY;

	//		GetAvatarMarkPosition(iOutputX, iOutputY, GetNtlSLGlobal()->GetSobAvatar());

	//		rtMapArea.SetRectWH(m_iMapStartX, m_iMapStartY, dMAP_WIDTH, dMAP_HEIGHT);
	//		m_pQuestSearch->SetArea(rtMapArea, m_fMapScale, m_v2MapPos, iOutputX, iOutputY);

	//		m_pQuestSearch->GetDialog()->SetAlpha( m_srfMap.GetSurface()->m_SnapShot.uAlpha );
	//	}
	//	else if( pEvent->iType == SCOUTER_EVENT_EXIT || pEvent->iType == SCOUTER_EVENT_OUT_OF_ORDER )
	//		UnloadQuestSearch();
	//}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( WORLD_PLAY_DOJO_SCRAMBLE != pEvent->eWorldConcept )
			NTL_RETURNVOID();

		switch(pEvent->uiState)
		{
		case WORLD_STATE_ENTER:
			{
				if( m_tScrambleVisible.bShowOurTeamMiniMap )
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_SHOW_OUR_TEAM);
				}
				else
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_HIDE_OUR_TEAM);
				}

				if( m_tScrambleVisible.bShowOtherTeamMiniMap )
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_SHOW_OTHER_TEAM);
				}
				else
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_HIDE_OTHER_TEAM);
				}

				m_tScrambleVisible.bScramble = TRUE;

				break;
			}
		case WORLD_STATE_EXIT:
		case WORLD_STATE_NONE:
			{
				if( m_tScrambleVisible.bShowOurTeamMiniMap )
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_SHOW_OUR_TEAM);
				}
				else
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_HIDE_OUR_TEAM);
				}

				if( m_tScrambleVisible.bShowOtherTeamMiniMap )
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_SHOW_OTHER_TEAM);
				}
				else
				{
					CDboEventGenerator::MapEvent(MMT_MINIMAP_HIDE_OTHER_TEAM);
				}


				m_tScrambleVisible.bScramble = FALSE;
				ClearSeal();

				break;
			}
		}
	}
	else if( msg.Id == g_EventDojoNotify )
	{		
		if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		if( DOJO_EVENT_SEAL_ATTACK_STATE == pEvent->byDojoEvent )
		{
			TBLIDX			idxObject		= pEvent->uiParam;
			RwUInt8			byState			= *(RwUInt8*)pEvent->pExData;
			sVECTOR2		v2Loc			= *(sVECTOR2*)pEvent->pExData2;

			CObjectTable* pObjectTable = API_GetTableContainer()->GetObjectTable( Logic_GetActiveWorldId() );
			if( pObjectTable == NULL )
			{
				DBO_FAIL("Not exist obejct table of world ID : " << Logic_GetActiveWorldId());
				NTL_RETURNVOID();
			}

			MAP_SCRAMBLE_SEAL_MARK::iterator it_ScrambleSealMark = m_mapScrambleSealMark.find(idxObject);
			if( it_ScrambleSealMark != m_mapScrambleSealMark.end() )
			{
				DBO_FAIL("Already exist seal of object table index : " << idxObject);
				NTL_RETURNVOID();
			}
			
			sSCRAMBLE_SEAL_MARK* pSCRAMBLE_SEAL_MARK = NTL_NEW sSCRAMBLE_SEAL_MARK;

			pSCRAMBLE_SEAL_MARK->eTeamState	= Logic_GetScrambleTeam((eTOBJECT_STATE_TYPE_C)byState);
			pSCRAMBLE_SEAL_MARK->bShow		= FALSE;
			pSCRAMBLE_SEAL_MARK->v3Pos.x	= v2Loc.x;
			pSCRAMBLE_SEAL_MARK->v3Pos.y	= 0.f;
			pSCRAMBLE_SEAL_MARK->v3Pos.z	= v2Loc.z;
			ZeroMemory(&pSCRAMBLE_SEAL_MARK->position, sizeof(pSCRAMBLE_SEAL_MARK->position));

			sOBJECT_TBLDAT* pOBJECT_TBLDAT		= reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTable->FindData(idxObject) );
			if( pOBJECT_TBLDAT )
			{
				CTextTable* pObjectTextTable	= API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
				pSCRAMBLE_SEAL_MARK->wstrName	= pObjectTextTable->GetText(pOBJECT_TBLDAT->dwName);
			}
			else
			{
				WCHAR awcBuffer[128];
				swprintf_s(awcBuffer, 128, L"Not exist sOBJECT_TBLDAT of index : %d", idxObject);
				DBO_FAIL(awcBuffer);
				pSCRAMBLE_SEAL_MARK->wstrName		= awcBuffer;
			}

			m_mapScrambleSealMark[idxObject]		= pSCRAMBLE_SEAL_MARK;
		}
		else if( DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER == pEvent->byDojoEvent )
		{
			// 인장의 상태가 변경되었음을 판단한다
			TBLIDX		dojoTblidx		= pEvent->uiParam;
			TBLIDX		idxObject		= *(TBLIDX*)pEvent->pExData;

			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
				NTL_RETURNVOID();

			CNtlDojo*		pDojo			= pAvatar->GetDojo();
			sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

			if( pSCRAMBLE_INFO->uiScrambleDojoTableIndex != dojoTblidx )
				NTL_RETURNVOID();

			MAP_SCRAMBLE_SEAL::iterator it_ScrambleSealInfo = pSCRAMBLE_INFO->mapScrambleSeal.find(idxObject);
			if( it_ScrambleSealInfo == pSCRAMBLE_INFO->mapScrambleSeal.end() )
				NTL_RETURNVOID();

			MAP_SCRAMBLE_SEAL_MARK::iterator it_ScrambleSealMark = m_mapScrambleSealMark.find(idxObject);
			if( it_ScrambleSealMark == m_mapScrambleSealMark.end() )
				NTL_RETURNVOID();

			sSCRAMBLE_SEAL_INFO*	pSCRAMBLE_SEAL_INFO	= it_ScrambleSealInfo->second;
			sSCRAMBLE_SEAL_MARK*	pSCRAMBLE_SEAL_MARK	= it_ScrambleSealMark->second;

			pSCRAMBLE_SEAL_MARK->eTeamState = Logic_GetScrambleTeam(pSCRAMBLE_SEAL_INFO->eState);
		}
	}

	NTL_RETURNVOID();
}