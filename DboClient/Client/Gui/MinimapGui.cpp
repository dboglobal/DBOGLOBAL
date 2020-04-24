#include "precomp_dboclient.h"
#include "MinimapGui.h"

#include <algorithm>

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// share
#include "NPCTable.h"
#include "WorldTable.h"
#include "TextAllTable.h"
#include "ObjectTable.h"
#include "MobTable.h"
#include "NtlTeleport.h"
#include "LandMarkTable.h"
#include "SpawnTable.h"
#include "TableContainer.h"
#include "NpcServerTable.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlWorldCommon.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLGuiManager.h"
#include "NtlPLVisualManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobGroup.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSLApi.h"
#include "DboTSCQAgency.h"
#include "DboTSCMain.h"
#include "NtlOtherParam.h"
#include "NtlWorldConceptTutorial.h"
#include "DboTSEntityFastFinder.h"
#include "NtlWorldConceptScramble.h"
#include "NtlFSMDef.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"


CSurfaceCamera* CMapItem::m_pRealMapCamera = NULL;
CSurfaceCamera* CMapItem::m_pRealMapCamera2 = NULL;

#define MINIMAP_BAR_Y		1

#define MINIMAP_BAR_HEIGHT	47
#define MINIMAP_BACK_HEIGHT	136

#define MINIMAP_CENTER_X	96
#define MINIMAP_CENTER_Y	109

#define MINIMAP_UPDATETIME		0.1f

#define dLENGTH_VISIBLE_PARTY_ARROW			100.f	///< 일정 거리 이상 떨어진 파티원의 화살표는 보여주지 않는다

#define	dONE_SIZE_RESOURCE_COUNT			3		///< 한 축의 리소스 갯수

#define	dFIELD_RESOURCE_SIZE				128		///< 필드맵 리소스 한 면의 사이즈
#define	dRENDER_TARGET_MAP_SIZE				384		///< 백버퍼에 저장된 전체 지도의 한 면의 사이즈
#define	dRESULT_DEFUALT_SIZE				256		///< 최종 텍스처의 기본 사이즈

#define	dSURFACE_SIZE						150		///< 미니맵 서페이스 사이즈

// avooo's comment : Adjusting the icons with the actual map ratio is too small or too large. Adjust it arbitrarily.
#define dICON_RATE_0						1.5f
#define dICON_RATE_1						1.f
#define dICON_RATE_2						0.9f

#define dMAP_SCALE_0						2.0f
#define dMAP_SCALE_1						1.3f
#define dMAP_SCALE_2						1.0f

#define dNEXTQUEST_ICON_DISTANCE_FROM_CENTER	59.f
#define dNEXTQUEST_ICON_RADIUS					9.f

#define dSEAL_REMAIN_TIME					(3.f)	///< 인장 플래쉬가 보여지는 시간
#define dSEAL_REST_TIME						(5.f)	///< 인장 플래쉬가 보여지고 나서 일정시간은 다시 보여주지 않는다
#define dSEAL_ENEMY_FLASH_FILE				"Minimap_Signal_Effect_Red.swf"
#define dSEAL_MY_TEAM_FLASH_FILE			"Minimap_Signal_Effect_Blue.swf"


CMapItem::CMapItem()
:m_iCurField(INVALID_INDEX)
,m_iPostField(INVALID_INDEX)
,m_PostWorldID(INVALID_WORLDID)
,m_fScale(dMAP_SCALE_1)
{
	m_rtAreaRect.left = 0.0f;
	m_rtAreaRect.right = 0.0f;
	m_rtAreaRect.top = 0.0f;
	m_rtAreaRect.bottom = 0.0f;	
}

CMapItem::~CMapItem()
{

}

VOID CMapItem::Destroy()
{
	NTL_FUNCTION("CMapItem::Destroy");

	m_SurfaceMap2.UnsetTexture();
	Logic_DeleteTexture( m_SurfaceMap2.GetFilterTexture() );

	m_SurfaceMap.UnsetTexture();	

	for(RwUInt8 i = 0 ; i < NUM_MAPPOSITON ; ++i )
	{
		Logic_DeleteTexture(m_ResourceSurface[i].GetTexture());	
		m_ResourceSurface[i].UnsetTexture();
	}

	NTL_RETURNVOID();
}

VOID CMapItem::CreateCamera()
{
	NTL_FUNCTION("CMapItem::CreateCamera");

	m_pRealMapCamera = NTL_NEW CSurfaceCamera;
	m_pRealMapCamera->Create(dRENDER_TARGET_MAP_SIZE, dRENDER_TARGET_MAP_SIZE);

	m_pRealMapCamera2 = NTL_NEW CSurfaceCamera;
	m_pRealMapCamera2->Create(dRESULT_DEFUALT_SIZE, dRESULT_DEFUALT_SIZE);	

	NTL_RETURNVOID();
}

VOID CMapItem::DestroyCamera()
{
	NTL_FUNCTION("CMapItem::DestroyCamera");

	m_pRealMapCamera->Destroy();
	NTL_DELETE(m_pRealMapCamera);

	m_pRealMapCamera2->Destroy();
	NTL_DELETE(m_pRealMapCamera2);		

	NTL_RETURNVOID();
}

VOID CMapItem::Create()
{
	NTL_FUNCTION("CMapItem::Create");

	// Real minimap surface
	m_SurfaceMap.SetRectWH(0, 0, dRESULT_DEFUALT_SIZE, dRESULT_DEFUALT_SIZE);	

	m_SurfaceMap2.SetFilterTexture( Logic_CreateTexture("minimapMask.png") );
	m_SurfaceMap2.SetRectWH(860, 30, dSURFACE_SIZE, dSURFACE_SIZE);	

	NTL_RETURNVOID();
}

VOID CMapItem::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_SurfaceMap2.SetPosition(iXPos, iYPos);
}

VOID CMapItem::SetScale(RwReal fScale)
{
	m_fScale = fScale;
}

RwReal CMapItem::GetScale()
{
	return m_fScale;
}

RwReal CMapItem::GetRate()
{
	return (RwReal)dSURFACE_SIZE / (dRESULT_DEFUALT_SIZE*m_fScale);
}

RwBool CMapItem::ReadyMakeTexture(RwReal fElapsed)
{
	// 아직 서버로 부터 EnterWorld 패킷을 받지 않았다
	RwUInt32 uiWorldID = Logic_GetActiveWorldId();
	if( uiWorldID == 0 )
		return false;

	RwV3d vFieldPos;
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return false;

	CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	RwInt32 iFieldIndex = pWorldEntity->GetWorldFieldIdx( pAvatar->GetPosition(), vFieldPos );	

	if( iFieldIndex == -1 )
		return false;

	if( m_iCurField != iFieldIndex || uiWorldID != m_PostWorldID )
	{
		m_iPostField	= m_iCurField;
		m_iCurField		= iFieldIndex;

		UpdateAreaRect(vFieldPos);
		UpdatePotalMapUpdateResource();
	}


	// Create Texture
	if( !m_pRealMapCamera->CameraBeginUpdate() )
		return false;

	// UV 조정 : 오른손 좌표계를 쓰기에 좌상단 방향이 + 이다.
	RwV3d vAvatarPos = pAvatar->GetPosition();
	RwInt32 iAreaSize = dGET_WORLD_PARAM()->WorldFieldSize * dONE_SIZE_RESOURCE_COUNT;
	RwReal fHalfSize = (RwReal)dRESULT_DEFUALT_SIZE / 2.f;
	RwReal fU_R = (m_rtAreaRect.left - vAvatarPos.x + fHalfSize*m_fScale) / iAreaSize;
	RwReal fU_L = (m_rtAreaRect.left - vAvatarPos.x - fHalfSize*m_fScale) / iAreaSize;
	RwReal fV_B = (m_rtAreaRect.top - vAvatarPos.z + fHalfSize*m_fScale) / iAreaSize;
	RwReal fV_T = (m_rtAreaRect.top - vAvatarPos.z - fHalfSize*m_fScale) / iAreaSize;

	m_SurfaceMap.GetSurface()->m_SnapShot.UVs[0] = fU_L;
	m_SurfaceMap.GetSurface()->m_SnapShot.UVs[1] = fV_T;
	m_SurfaceMap.GetSurface()->m_SnapShot.UVs[2] = fU_R;
	m_SurfaceMap.GetSurface()->m_SnapShot.UVs[3] = fV_B;

	for(RwUInt8 i = 0 ; i < NUM_MAPPOSITON ; ++i)
	{
		if( m_ResourceSurface[i].GetTexture() )
			m_ResourceSurface[i].Render();
	}

	return true;
}

VOID CMapItem::EndMakeTexture()
{
	m_pRealMapCamera->CameraEndUpdate();

	m_pRealMapCamera->CreateTexture();

	m_SurfaceMap.SetTexture(m_pRealMapCamera->GetTexture());

	// Create Texture
	if( m_pRealMapCamera2->CameraBeginUpdate() )
	{
		m_SurfaceMap.Render();

		m_pRealMapCamera2->CameraEndUpdate();

		m_pRealMapCamera2->CreateTexture();
	}	

	m_SurfaceMap2.SetTexture(m_pRealMapCamera2->GetTexture());
}

VOID CMapItem::Render()
{
	m_SurfaceMap2.Render();
}

VOID CMapItem::UpdateAreaRect(RwV3d vFieldPos)
{
	m_rtAreaRect.left = vFieldPos.x + dGET_WORLD_PARAM()->WorldFieldSize * 2;
	m_rtAreaRect.right = vFieldPos.x - dGET_WORLD_PARAM()->WorldFieldSize;
	m_rtAreaRect.top = vFieldPos.z + dGET_WORLD_PARAM()->WorldFieldSize * 2;
	m_rtAreaRect.bottom = vFieldPos.z - dGET_WORLD_PARAM()->WorldFieldSize;
}

VOID CMapItem::UpdatePotalMapUpdateResource()
{
	WORLDID curWorldID = Logic_GetActiveWorldId();

	if( m_PostWorldID != curWorldID )
	{
		m_PostWorldID = curWorldID;

		for( RwUInt8 i = 0 ; i < NUM_MAPPOSITON ; ++i )
		{
			// 월드가 바뀌면 리소스를 전부 다시 읽어들인다
			Logic_DeleteTexture(m_ResourceSurface[i].GetTexture());
			m_ResourceSurface[i].UnsetTexture();
			LoadingSurface(m_ResourceSurface[i], CalcIndex(m_iCurField, i), i);
		}
	}
	else
	{
		RwInt32 iFieldIndex;
		std::map<RwInt32, gui::CTexture*> mapResourceSurface;
		std::map<RwInt32, gui::CTexture*>::iterator it_resource;

		// 기존의 리소스를 보관
		for( RwUInt8 i = 0 ; i < NUM_MAPPOSITON ; ++i )
		{
			iFieldIndex = CalcIndex(m_iPostField, i);

			if( iFieldIndex != INVALID_INDEX && m_ResourceSurface[i].GetTexture() != NULL )
			{
				mapResourceSurface[iFieldIndex] = m_ResourceSurface[i].GetTexture();
			}

			m_ResourceSurface[i].UnsetTexture();
		}

		for( RwUInt8 i = 0 ; i < NUM_MAPPOSITON ; ++i )
		{
			iFieldIndex = CalcIndex(m_iCurField, i);
			it_resource = mapResourceSurface.find(iFieldIndex);

			if( it_resource == mapResourceSurface.end() )
			{
				// 새로운 리소스를 로딩한다
				LoadingSurface(m_ResourceSurface[i], iFieldIndex, i);
			}
			else
			{
				// 기존 리소스를 활용한다
				m_ResourceSurface[i].SetTexture(it_resource->second);
				mapResourceSurface.erase(it_resource);
			}
		}

		// 재활용되지 않는 리소스는 삭제한다
		it_resource = mapResourceSurface.begin();
		for( ; it_resource != mapResourceSurface.end() ; ++it_resource )
		{
			Logic_DeleteTexture(it_resource->second);
		}
	}	
}

VOID CMapItem::LoadingSurface(CSurfaceGui& surface, RwInt32 iFieldIndex, RwInt32 iMapPosition)
{
	if( iFieldIndex == INVALID_INDEX )
		return;

	RwChar acPathName[256] = "";
	RwChar acFileName[16] = "";
	CWorldTable* pWorldTable = API_GetTableContainer()->GetWorldTable();
	sWORLD_TBLDAT* pWORLD_TBLDAT = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData(Logic_GetActiveWorldTableId()) );

	// 미니맵의 리소스는 4자리의 숫자로 된 파일이름으로 되어 있다
	// 확장자는 DDS 이며 현재 프로젝트에서 DDS는 확장자를 붙이지 않고 쓴다
	sprintf_s(acFileName, "%04d", iFieldIndex);

	::WideCharToMultiByte(GetACP(), 0, pWORLD_TBLDAT->wszResourceFolder, -1, acPathName, 256, NULL, NULL);


	surface.SetTexture( Logic_CreateTexture((RwChar*)acFileName, TEXTYPE_MAP, acPathName ) );
	surface.SetRectWH((iMapPosition%dONE_SIZE_RESOURCE_COUNT) * dFIELD_RESOURCE_SIZE,
		(iMapPosition/dONE_SIZE_RESOURCE_COUNT) * dFIELD_RESOURCE_SIZE,
		dFIELD_RESOURCE_SIZE,	dFIELD_RESOURCE_SIZE);
}

RwInt32 CMapItem::CalcIndex(RwInt32 iCenterIndex, RwInt32 iPosition)
{
	RwInt32 iResult;

	// 월드의 한 축의 사이즈
	RwInt32 iWorldEdgeSize = dGET_WORLD_PARAM()->WorldFieldNum;	

	switch(iPosition)
	{
	case MP_LT:
		{
			RwInt32 iCompare = iCenterIndex + iWorldEdgeSize;
			iResult = iCompare + 1;

			// 세로축이 다르면 가장 좌측이라 이보다 좌측의 필드는 없음을 나타낸다
			if( (iCompare/iWorldEdgeSize) != (iResult/iWorldEdgeSize) )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	case MP_CT:
		{
			RwInt32 iCompare = iCenterIndex + iWorldEdgeSize;
			iResult = iCompare;

			break;
		}
	case MP_RT:
		{
			RwInt32 iCompare = iCenterIndex + iWorldEdgeSize;
			iResult = iCompare - 1;

			// 세로축이 다르면 가장 우측이라 이보다 우측의 필드는 없음을 나타낸다
			if( (iCompare/iWorldEdgeSize) != (iResult/iWorldEdgeSize) )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	case MP_LC:
		{
			RwInt32 iCompare = iCenterIndex;
			iResult = iCompare + 1;

			// 세로축이 다르면 가장 좌측이라 이보다 좌측의 필드는 없음을 나타낸다
			if( (iCompare/iWorldEdgeSize) != (iResult/iWorldEdgeSize) )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	case MP_CC:
		{
			iResult = iCenterIndex;
			break;
		}
	case MP_RC:
		{
			RwInt32 iCompare = iCenterIndex;
			iResult = iCompare - 1;

			// 세로축이 다르면 가장 우측이라 이보다 우측의 필드는 없음을 나타낸다
			if( (iCompare/iWorldEdgeSize) != (iResult/iWorldEdgeSize) )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	case MP_LB:
		{
			RwInt32 iCompare = iCenterIndex - iWorldEdgeSize;
			iResult = iCompare + 1;

			if( iCompare < 0 )
			{
				iResult = INVALID_INDEX;
				break;
			}

			// 세로축이 다르면 가장 좌측이라 이보다 좌측의 필드는 없음을 나타낸다
			if( (iCompare/iWorldEdgeSize) != (iResult/iWorldEdgeSize) )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	case MP_CB:
		{
			RwInt32 iCompare = iCenterIndex - iWorldEdgeSize;
			iResult = iCompare;

			if( iCompare < 0 )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	case MP_RB:
		{
			RwInt32 iCompare = iCenterIndex - iWorldEdgeSize;
			iResult = iCompare - 1;

			if( iCompare < 0 )
			{
				iResult = INVALID_INDEX;
				break;
			}

			// 세로축이 다르면 가장 우측이라 이보다 우측의 필드는 없음을 나타낸다
			if( (iCompare/iWorldEdgeSize) != (iResult/iWorldEdgeSize) )
			{
				iResult = INVALID_INDEX;
				break;
			}

			break;
		}
	default:
		{
			DBO_FAIL("wrong index");
			iResult = INVALID_INDEX;
			break;
		}
	}

	if( !IsWorldIndex(iResult) )
		iResult = INVALID_INDEX;

	return iResult;
}

CMapItem::RealRect CMapItem::GetAreaRect()
{	
	return m_rtAreaRect;
}

RwBool CMapItem::IsWorldIndex(RwInt32 iIndex)
{	
	if( 0 <= iIndex && iIndex < dGET_WORLD_PARAM()->WorldHeightfieldNum )
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
// CMinimapGui
////////////////////////////////////////////////////////////////////////////////

CMinimapGui::CMinimapGui(VOID)
{
}

CMinimapGui::CMinimapGui( const RwChar* pName )
:CNtlPLGui( pName )
,m_pMapShowBtn(NULL)
,m_pMapHideBtn(NULL)
,m_pWorldMapBtn(NULL)
,m_pExpansionBtn(NULL)
,m_pReductionBtn(NULL)
,m_fElapsedTime(0.0f)
,m_byMapStyle(MS_NORMAL)
,m_byCurScale(1)
,m_idxAreaInfoIndex(0xffffffff)
{
	m_tSCRAMBLE_VISIBLE.bScramble		= false;
	m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam		= false;
	m_tSCRAMBLE_VISIBLE.bShowScrambleEnemyTeam	= false;

	m_fMiniMapScaleLevels[0] = dMAP_SCALE_0;
	m_fMiniMapScaleLevels[1] = dMAP_SCALE_1;
	m_fMiniMapScaleLevels[2] = dMAP_SCALE_2;

	m_pMapNameTextTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
}

CMinimapGui::~CMinimapGui(VOID)
{
}

////////////////////////////////////////////////////////////////////////////////
// Operation

RwBool CMinimapGui::Create(VOID)
{
	NTL_FUNCTION("CMinimapGui::Create");

	if(!CNtlPLGui::Create("", "gui\\Minimap.srf", "gui\\Minimap.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	char acBuffer[64] = "";

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_pMapShowBtn	= (gui::CButton*)GetComponent( "btnMapShow" );
	m_pMapHideBtn	= (gui::CButton*)GetComponent( "btnMapHide" );
	m_pWorldMapBtn	= (gui::CButton*)GetComponent( "btnWorldMap" );	
	m_pExpansionBtn	= (gui::CButton*)GetComponent( "btnZoomIn" );
	m_pReductionBtn	= (gui::CButton*)GetComponent( "btnZoomOut" );
	m_pRegionTitle	= (gui::CStaticBox*)GetComponent( "stbTitle" );

	m_pRegionTitle->CreateFontStd(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);

	m_pMapShowBtn		->SetToolTip(GetDisplayStringManager()->GetString("DST_MINIMAP_SHOW_MINIMAP"));
	m_pMapHideBtn		->SetToolTip(GetDisplayStringManager()->GetString("DST_MINIMAP_MINIMIZE"));
	m_pWorldMapBtn		->SetToolTip(GetDisplayStringManager()->GetString("DST_MINIMAP_SHOW_WORLDMAP"));
	m_pExpansionBtn		->SetToolTip(GetDisplayStringManager()->GetString("DST_MINIMAP_ZOOM_IN"));
	m_pReductionBtn		->SetToolTip(GetDisplayStringManager()->GetString("DST_MINIMAP_ZOOM_OUT"));

	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_surBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfMiniMapBack" ) ); 
	m_surBack.SetPositionfromParent(9, 31);
	m_surFrame.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfMiniMapTitleBar" ) ); 
	m_surFrame.SetPosition( rtScreen.left, rtScreen.top + MINIMAP_BAR_Y );

	m_surMarkAvatar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfMarkAvatarMinimap" ) );

	// Create mini-map items
	m_MapItem.Create();

	// Mob mark
	m_surMarkMob[MMT_STRONG]					.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkMobStrong" ) );
	m_surMarkMob[MMT_NORMAL]					.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkMobNormal" ) );
	m_surMarkMob[MMT_WEAK]						.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkMobWeak" ) );
	m_surMarkMob[MMT_HAVE_DRAGONBALL]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkMobHaveDragonBall" ) );
	m_surMarkMob[MMT_HAVE_QUEST].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfMarkMobQuest"));

	// Quest Mark
	m_surMarkQuest[QMT_NEW]						.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfNewQuest" ) );
	m_surMarkQuest[QMT_PROGRESS]				.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfProgressQuest" ) );
	m_surMarkQuest[QMT_REWARD]					.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRewardQuest" ) );

	// Next Quest Mark
	m_surMarkNextQuest[NQMT_INSIGHT]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuest" ) );
	m_surMarkNextQuest[NQMT_INSIGHT_POSITION]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuest_Position" ) );
	m_surMarkNextQuest[NQMT_CHECK].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Minimap.srf", "srsNextQuestCheck"));
	m_surMarkNextQuest[NQMT_OUTSIGHT]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuestArrow" ) );

	// Checkpoint mark
	m_surMarkCheckPoint[MMP_INSIGHT]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuest" ) );
	m_surMarkCheckPoint[MMP_OUTSIGHT]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfNextQuestArrow" ) );

	// Party member mark
	m_surMarkPartyMember[MMP_INSIGHT]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkPartyMember" ) );
	m_surMarkPartyMember[MMP_OUTSIGHT]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMarkPartyMemberArrow" ) );

	//Hidden Objects
	m_surHiddenObject.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfHiddenObject" ) );

	// 포포스톤
	m_surPopoStone.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfPopoStone" ) );
	m_surPopoStoneActive.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfBindPopoStone"));

	// 공유 타겟
	for( RwUInt8 i = 0 ; i < NTL_MAX_SHARETARGET_COUNT ; ++i )
	{
		sprintf_s(acBuffer, 64, "srfCommonTargetMark_%d", i+1);
		m_surMarkCommonTarget[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", acBuffer ) );
	}

	// 도장 인장
	m_surMarkDojoSeal[DBO_TEAM_MY_TEAM]		.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfSealMyTeamTeam" ) );
	m_surMarkDojoSeal[DBO_TEAM_ENEMY]		.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfSealEnemyTeam" ) );
	m_surMarkDojoSeal[DBO_TEAM_NEUTRAILITY]	.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfSealNoTeam" ) );

	// (피아구분)팀을 나누어서 싸우는 경우의 사람들
	m_surCamp[CAMP_PEOPLE_MY_PARTY]				.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfScrambleCampPartyTeam" ) );
	m_surCamp[CAMP_PEOPLE_MY_TEAM]				.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfScrambleCampMyTeam" ) );
	m_surCamp[CAMP_PEOPLE_EMENY_TEAM]			.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Minimap.srf", "srfScrambleCampEnemyTeam" ) );


	// default value
	m_pRegionTitle->SetText(L"");

	m_slotMapShowButtonClicked = m_pMapShowBtn->SigClicked().Connect( this, &CMinimapGui::OnClicked_MapShowButton );
	m_slotMapHideButtonClicked = m_pMapHideBtn->SigClicked().Connect( this, &CMinimapGui::OnClicked_MapHideButton );
	m_slotWorldMapClicked = m_pWorldMapBtn->SigClicked().Connect( this, &CMinimapGui::OnClicked_WorldMap );
	m_slotExpansionClicked = m_pExpansionBtn->SigClicked().Connect( this, &CMinimapGui::OnClicked_Expansion );
	m_slotReductionClicked = m_pReductionBtn->SigClicked().Connect( this, &CMinimapGui::OnClicked_Reduction );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CMinimapGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CMinimapGui::OnMove );
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CMinimapGui::OnMouseMove );
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect( this, &CMinimapGui::OnMouseLeave );
	m_slotRegionTitleMouseMove = m_pRegionTitle->SigMouseMove().Connect( this, &CMinimapGui::OnRegionTitleMouseMove );
	m_slotRegionTitleMouseLeave = m_pRegionTitle->SigMouseLeave().Connect( this, &CMinimapGui::OnRegionTitleMouseLeave );
	m_slotCaptureWheelMove		= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CMinimapGui::OnCaptureWheelMove );

	AdjustIconSize();

	GetNtlGuiManager()->AddUpdateFunc( this );

	// Link updatebefore
	GetNtlGuiManager()->AddUpdateBeforeFunc( this );

	LinkMsg(g_EventMap);
	LinkMsg(g_EventDirectionNfy);
	LinkMsg(g_EventChangeWorldConceptState);
	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventDojoStateNotify);

	NTL_RETURN(TRUE);
}

void CMinimapGui::Destroy(VOID)
{
	NTL_FUNCTION("CMinimapGui::Destroy");

	//for each( sLANKMARK* pLandMark in m_listLandMark )
	//	NTL_DELETE(pLandMark);

	ClearSeal();

	CheckInfoWindow();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	// Unlink updatebefore
	GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );

	m_MapItem.Destroy();

	UnLinkMsg(g_EventMap);
	UnLinkMsg(g_EventDirectionNfy);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventDojoStateNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CMinimapGui::ClearSeal()
{
	std::map<RwUInt32, sSCRAMBLE_SEAL*>::iterator it_Seal = m_mapScrambleSeal.begin();
	for( ; it_Seal != m_mapScrambleSeal.end() ; ++it_Seal )
	{
		sSCRAMBLE_SEAL* pSCRAMBLE_SEAL = it_Seal->second;
		NTL_DELETE(pSCRAMBLE_SEAL);
	}
	m_mapScrambleSeal.clear();


	std::map<RwUInt32, sSCRAMBLE_SEAL_NOTIFY*>::iterator it_SealNotify = m_mapScrambleSealNotify.begin();
	for( ; it_SealNotify != m_mapScrambleSealNotify.end() ; ++it_SealNotify )
	{
		sSCRAMBLE_SEAL_NOTIFY* pSCRAMBLE_SEAL_NOTIFY = it_SealNotify->second;

		if( pSCRAMBLE_SEAL_NOTIFY->pFlash )
			pSCRAMBLE_SEAL_NOTIFY->pFlash->Unload();

		NTL_DELETE(pSCRAMBLE_SEAL_NOTIFY);
	}
	m_mapScrambleSealNotify.clear();
}

VOID CMinimapGui::Update( RwReal fElapsed )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pAvatar == NULL)
		return;

	if( !IsShow() || MS_NORMAL != m_byMapStyle )
		return;


	std::map<RwUInt32, sSCRAMBLE_SEAL_NOTIFY*>::iterator it_SealNotify = m_mapScrambleSealNotify.begin();
	for( ; it_SealNotify != m_mapScrambleSealNotify.end(); it_SealNotify++ )
	{
		sSCRAMBLE_SEAL_NOTIFY* pSCRAMBLE_SEAL_NOTIFY = it_SealNotify->second;

		if(pSCRAMBLE_SEAL_NOTIFY && pSCRAMBLE_SEAL_NOTIFY->pFlash )
			pSCRAMBLE_SEAL_NOTIFY->pFlash->Update(fElapsed);
	}


	m_fElapsedTime += fElapsed;

	// 연산량을 줄이기 위해 일정시간마다 Update
	if( m_fElapsedTime < MINIMAP_UPDATETIME )
		return;

	// m_fElapsedTime 초기화는 UpdateBeforeCamera()함수에서

	CNtlGuild*		pGuild				= pAvatar->GetGuild();
	const WCHAR*	pwcAvatarGuildName	= pGuild->GetGuildName();
	CNtlSobAvatarAttr* pAvatarAttr		= reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
	CNtlBeCharData*	pBeData				= reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());
	SCtrlStuff*		pCtrlStuff			= pBeData->GetCtrlStuff();

	m_hBus_with_Avatar					=	pCtrlStuff->sRide.hTargetSerialId;

	m_vecTriggerObject.clear();
	m_vecMob.clear();
	m_vecNPC.clear();
	m_vecQuest.clear();
	m_vectNextQuest.clear();
	m_vecPartyMember.clear();
	m_listCampPeople.clear();

	for( RwUInt8 i = 0 ; i < NTL_MAX_SHARETARGET_COUNT ; ++i )
	{
		m_aCommonTarget[i].bShow		= false;
		m_aCommonTarget[i].bInfoWindow	= false;
	}


	// 대상의 위치를 임시 저장
	RwV3d v3TempPos;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	RwV3d v3Avatar = pAvatar->GetPosition();
	RwV2d v2Avatar;
	v2Avatar.x = v3Avatar.x;
	v2Avatar.y = v3Avatar.z;

	// 아바타
	if( m_hBus_with_Avatar == INVALID_SERIAL_ID )
	{
		RwV3d vDirAvatar = pAvatar->GetDirection();
		RwV2d v2DirAvatar;
		v2DirAvatar.x = vDirAvatar.x;
		v2DirAvatar.y = vDirAvatar.z;

		m_surMarkAvatar.SetZRot( GetAngle(v2DirAvatar) );
	}	

	CNtlSobGroup* pSobGroup = NULL;
	RwReal fRealRate = m_MapItem.GetRate();

	RwReal fLimitedDistance;

	if( m_MapItem.GetScale() < 1.0f )
		fLimitedDistance = (dRESULT_DEFUALT_SIZE/2 * m_MapItem.GetScale()) - 5; // : Adjust
	else
		fLimitedDistance = (dRESULT_DEFUALT_SIZE/2 * m_MapItem.GetScale()) - 14; // : Adjust


	static SERIAL_HANDLE ahBus_with_Party[NTL_MAX_MEMBER_IN_PARTY];
	CNtlParty* pParty = pAvatar->GetParty();

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		ahBus_with_Party[i] = INVALID_SERIAL_ID;


	if( m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam || 
		m_tSCRAMBLE_VISIBLE.bShowScrambleEnemyTeam )
	{
		// 도장전 팀
		CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
		if( pSobGroup )
		{
			CNtlWorldConceptScramble* pWorldConcept = reinterpret_cast<CNtlWorldConceptScramble*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DOJO_SCRAMBLE) );
			if( pWorldConcept )
			{
				CNtlSobGroup::MapObject::iterator it;
				for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
				{
					CNtlSobPlayer*		pSobPlayer		= reinterpret_cast<CNtlSobPlayer*>( it->second );
					CNtlSobPlayerAttr*	pSobPlayerAttr	= reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );
					const WCHAR*		pwcGuildName	= pSobPlayer->GetGuildName();
					RwV3d				v3Pos			= pSobPlayer->GetPosition();

					sCAMP_PEOPLE tCAMP_PEOPLE;

					tCAMP_PEOPLE.v2Pos.x		= v3Pos.x;
					tCAMP_PEOPLE.v2Pos.y		= v3Pos.z;
					tCAMP_PEOPLE.pwcName		= pSobPlayerAttr->GetName();

					TransWorldToMapAxis( v2Avatar, tCAMP_PEOPLE.v2Pos, fRealRate, tCAMP_PEOPLE.v2Icon );					

					if( pWorldConcept->IsEnemy(pwcAvatarGuildName, pwcGuildName) )
					{
						if( !m_tSCRAMBLE_VISIBLE.bShowScrambleEnemyTeam )
							continue;

						tCAMP_PEOPLE.ePeopleType = CAMP_PEOPLE_EMENY_TEAM;
					}
					else
					{
						if( !m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam )
							continue;

						if( Logic_IsMyPartyMember( pSobPlayer->GetSerialID() ) )
							tCAMP_PEOPLE.ePeopleType = CAMP_PEOPLE_MY_PARTY;
						else
							tCAMP_PEOPLE.ePeopleType = CAMP_PEOPLE_MY_TEAM;
					}

					m_listCampPeople.push_back(tCAMP_PEOPLE);
				}
			}
		}
	}

	if( !m_tSCRAMBLE_VISIBLE.bScramble )
	{
		// 파티 맴버 위치 업데이트
		RwUInt8 byPartyMemberIndex = 0;
		COMMUNITY_ITER it_party = pParty->Begin();
		for( ; it_party != pParty->End() ; ++it_party )
		{
			sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it_party );

			ahBus_with_Party[byPartyMemberIndex] = pMember->hBusSerial;
			++byPartyMemberIndex;


			if( pMember->hSerial == pAvatar->GetSerialID() )
				continue;

			if( pMember->uiWorldID != Logic_GetActiveWorldId() )
				continue;

			sPARTYMEMBER partyMember;

			partyMember.v2Pos.x		= pMember->vPos.x;
			partyMember.v2Pos.y		= pMember->vPos.z;
			partyMember.pwcName		= pMember->wszMemberName;

			if( pMember->hBusSerial == INVALID_SERIAL_ID )
			{
				partyMember.bShow		= TRUE;
				partyMember.bInfoWindow	= TRUE;
			}
			else
			{
				partyMember.bShow		= FALSE;
				partyMember.bInfoWindow	= TRUE;
			}

			if( IsV2MoreLength(&partyMember.v2Pos, &v2Avatar, fLimitedDistance) )
			{
				// 일정 거리 떨어진 파티원의 화살표 아이콘은 아예 보여주지 않는다
				if( Logic_InFollowRange(pAvatar, pMember->vPos, dLENGTH_VISIBLE_PARTY_ARROW ) )
					continue;

				partyMember.eType = MMP_OUTSIGHT;

				RwV2d v2Dir;

				RwV2dSub(&v2Dir, &partyMember.v2Pos, &v2Avatar);
				partyMember.fAngle = GetAngle(v2Dir, &v2Dir);

				// GUI는 x, y가 +방향이 반대방향
				v2Dir *= -1.f;

				partyMember.v2OutCenter = v2Dir * dNEXTQUEST_ICON_DISTANCE_FROM_CENTER;

				partyMember.v2OutCenter.x += MINIMAP_CENTER_X;
				partyMember.v2OutCenter.y += MINIMAP_CENTER_Y;

			}
			else
			{
				partyMember.eType	= MMP_INSIGHT;				
				TransWorldToMapAxis( v2Avatar, partyMember.v2Pos, fRealRate, partyMember.v2Icon );


				RwInt32 iCommonTargetIndex = Logic_isPartyShareTarget(pMember->hSerial);
				if( iCommonTargetIndex >= 0 )
				{
					partyMember.bShow	= TRUE;
					SetCommonTargetInfo(iCommonTargetIndex, partyMember.v2Pos, partyMember.v2Icon);
				}
			}

			m_vecPartyMember.push_back(partyMember);
		}
	}

	// 트리거 오브젝트 위치 업데이트
	pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_TRIGGER_OBJECT );
	if( pSobGroup )
	{
		CNtlOtherParam* pOtherParam = pAvatar->GetOtherParam();
		sBindInfo* pBindInfo = pOtherParam->GetBindInfo();
		CNtlSobGroup::MapObject::iterator it;
		for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
		{
			CNtlSobTriggerObject* pTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>( it->second );
			if( !pTriggerObject )
			{
				DBO_FAIL("Not exist trigger object of handle : " << it->second);
				continue;
			}

			CNtlSobTriggerObjectAttr* pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( pTriggerObject->GetSobAttr() );
			sOBJECT_TBLDAT* pOBJECT_TBLDAT = pTriggerObjectAttr->GetTriggerObjectTbl();

			if( BIT_FLAG_TEST(pOBJECT_TBLDAT->dwFunction,eDBO_TRIGGER_OBJECT_FUNC_BIND) )
			{
				if( pBindInfo->byBindType == REVIVAL_TYPE_BIND_POINT &&
					pBindInfo->BindedWorldID == Logic_GetActiveWorldId() )
				{
					v3TempPos = pTriggerObject->GetPosition();

					sTRIGGER_OBJECT triggerObject;

					triggerObject.v2Pos.x	= v3TempPos.x;
					triggerObject.v2Pos.y	= v3TempPos.z;
					TransWorldToMapAxis( v2Avatar, triggerObject.v2Pos, fRealRate, triggerObject.v2Icon );
					triggerObject.eType		= TOMT_POPOSTONE;
					triggerObject.bShow = (pBindInfo->uiBindedObejcTblIdx == pOBJECT_TBLDAT->tblidx) ? true : false;
					triggerObject.bInfoWindow	= true;
					triggerObject.wstrText	= GetDisplayStringManager()->GetString("DST_MINIMAP_POPOSTONE");					

					RwInt32 iCommonTargetIndex = Logic_isPartyShareTarget( pTriggerObject->GetSerialID() );
					if( iCommonTargetIndex >= 0 )
					{
						triggerObject.bShow			= false;
						SetCommonTargetInfo(iCommonTargetIndex, triggerObject.v2Pos, triggerObject.v2Icon);
					}

					m_vecTriggerObject.push_back( triggerObject );
				}
			}
		}
	}

	// 몬스터 위치 업데이트
	pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_MONSTER );
	if( pSobGroup )
	{
		CNtlSobGroup::MapObject::iterator it;
		for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
		{
			CNtlSobMonster* pMonster = reinterpret_cast<CNtlSobMonster*>( it->second );
			CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pMonster->GetSobAttr() );

			if( NTL_FSMSID_DIE == Logic_GetActorStateId(pMonster) )
				continue;


			v3TempPos = pMonster->GetPosition();			

			sMONSTER monster;

			monster.eMobMarkType	= MMT_NORMAL;
			monster.bShow			= true;
			monster.bInfoWindow		= true;

			if(pMonsterAttr->IsHaveDragonBall())
			{
				if(Logic_ExistScouterEffect(ACTIVE_SCOUTER_CHECK_DRAGONBALL))
					monster.eMobMarkType = MMT_HAVE_DRAGONBALL;
			}

			// to do: check if required for quest
			//monster.eMobMarkType = MMT_HAVE_QUEST;

			else
			{
				RwUInt8 byCGType = Logic_GetCounterpartGradeType(pAvatarAttr, pMonsterAttr);

				if( byCGType == CGT_STRONG_1 )
				{
					monster.eMobMarkType = MMT_STRONG;
				}
				else if( byCGType == CGT_WEAK_1 )
				{
					monster.eMobMarkType = MMT_WEAK;
				}
				else
				{
					monster.eMobMarkType = MMT_NORMAL;
				}
			}

			monster.v2Pos.x			= v3TempPos.x;
			monster.v2Pos.y			= v3TempPos.z;
			monster.pMOB_TBLDAT		= pMonsterAttr->GetMobTbl();

			TransWorldToMapAxis( v2Avatar, monster.v2Pos, fRealRate, monster.v2Icon );

			RwInt32 iCommonTargetIndex = Logic_isPartyShareTarget( pMonster->GetSerialID() );
			if( iCommonTargetIndex >= 0 )
			{
				monster.bShow		= false;
				SetCommonTargetInfo(iCommonTargetIndex, monster.v2Pos, monster.v2Icon);
			}

			m_vecMob.push_back( monster );
		}
	}

	// NPC, 퀘스트 위치 업데이트
	pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_NPC );

	CNpcServerTable* pNpcServerTbl = API_GetTableContainer()->GetNpcServerTable();

	if( pSobGroup )
	{
		CNtlSobGroup::MapObject::iterator it;

		for( it = pSobGroup->GetObjects().begin(); it != pSobGroup->GetObjects().end() ; ++it )
		{
			CNtlSobNpc* pNpc = reinterpret_cast<CNtlSobNpc*>( (*it).second );
			CNtlSobNpcAttr* pNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNpc->GetSobAttr() );

			v3TempPos = pNpc->GetPosition();

			if( pNpc->GetQuestMark() )
			{
				sMINIMAP_QUEST sQuest;

				sQuest.v2Pos.x		= v3TempPos.x;
				sQuest.v2Pos.y		= v3TempPos.z;
				sQuest.ucQuest		= pNpc->GetQuestMark();
				sQuest.pNPC_TBLDAT	= pNpcAttr->GetNpcTbl();
				sQuest.bShow		= true;
				sQuest.bInfoWindow	= true;
				TransWorldToMapAxis( v2Avatar, sQuest.v2Pos, fRealRate, sQuest.v2Icon );

				RwInt32 iCommonTargetIndex = Logic_isPartyShareTarget( pNpc->GetSerialID() );
				if( iCommonTargetIndex >= 0 )
				{
					sQuest.bShow		= false;
					SetCommonTargetInfo(iCommonTargetIndex, sQuest.v2Pos, sQuest.v2Icon);
				}

				m_vecQuest.push_back( sQuest );
			}
			else
			{
				sNPC_TBLDAT* pNPC_TBLDAT = pNpcAttr->GetNpcTbl();

				if (pNPC_TBLDAT->byJob == NPC_JOB_TALKER || pNPC_TBLDAT->byJob == NPC_JOB_GATE_KEEPER || pNPC_TBLDAT->byJob == NPC_JOB_COMIC_NPC ||
					pNPC_TBLDAT->bValidity_Able == false)
					continue;

				sNPC_SERVER_TBLDAT* pNpcServerTbldat = reinterpret_cast<sNPC_SERVER_TBLDAT*>(pNpcServerTbl->FindData(pNPC_TBLDAT->tblidx));
				if (pNpcServerTbldat->bValidity_Able == false)
					continue;

				sMINIMAP_NPC sNPC;					

				sNPC.v2Pos.x		= v3TempPos.x;
				sNPC.v2Pos.y		= v3TempPos.z;				
				sNPC.ucJob			= pNPC_TBLDAT->byJob;
				sNPC.bGetOnBus		= false;
				sNPC.bShow			= true;
				sNPC.bInfoWindow	= true;
				sNPC.pNPC_TBLDAT	= pNpcAttr->GetNpcTbl();
				sNPC.hHandle		= pNpc->GetSerialID();
				TransWorldToMapAxis( v2Avatar, sNPC.v2Pos, fRealRate, sNPC.v2Icon );

				RwInt32 iCommonTargetIndex = Logic_isPartyShareTarget( pNpc->GetSerialID() );
				if( iCommonTargetIndex >= 0 )
				{
					sNPC.bShow		= false;
					SetCommonTargetInfo(iCommonTargetIndex, sNPC.v2Pos, sNPC.v2Icon);
				}
				else if( Logic_IsBus( reinterpret_cast<CNtlSobActor*>(pNpc) ) )
				{
					// 아바타가 타고 있는 버스
					if( pNpc->GetSerialID() == m_hBus_with_Avatar )
						sNPC.bGetOnBus = true;
					else
					{
						// 파티원이 타고 있는 버스인지 검사
						for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
						{
							if( ahBus_with_Party[i] == pNpc->GetSerialID() )
							{
								sNPC.bGetOnBus = true;
								break;
							}
						}
					}
				}

				m_vecNPC.push_back( sNPC );
			}
		}
	}

	// Sob 객체는 없지만 테이블상에 미니맵 영역에 위치하는 NPC를 찾는다
	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) == false)
	{
		CDboTSCMain* pTCSMain = GetTSCMain();
		CDboTSEntityFastFinder* pEntityFastFinder = pTCSMain->GetEntityFastFinder();
		if( pEntityFastFinder )
		{
			CDboTSWorldMng* pWorldMng = pEntityFastFinder->FindWorldMng( Logic_GetActiveWorldTableId() );
			if( pWorldMng )
			{
				CSpawnTable* pNPCSpawnTbl = API_GetTableContainer()->GetNpcSpawnTable( Logic_GetActiveWorldTableId() );
				
				if( pNPCSpawnTbl )
				{
					CNPCTable* pNPCTable = API_GetTableContainer()->GetNpcTable();
					std::list< sTSEntity* > FindEntityList;

					pWorldMng->FindEntity( v3Avatar.x, v3Avatar.z, fLimitedDistance, FindEntityList );

					// avooo's commnet : Will be modified by finding data in this partial preprocessed file
					for each( sTSEntity* pTSEntity in FindEntityList )
					{
						RwBool bExist = false;

						for( RwUInt32 i = 0 ; i < m_vecNPC.size() ; ++i )
						{
							sMINIMAP_NPC* pstNPC = &m_vecNPC[i];

							if( pTSEntity->uiTSEntityID == pstNPC->pNPC_TBLDAT->tblidx )
							{
								bExist = true;
								break;
							}
						}

						for( RwUInt32 i = 0 ; i < m_vecQuest.size() ; ++i )
						{
							sMINIMAP_QUEST* pstQuest = &m_vecQuest[i];

							if( pTSEntity->uiTSEntityID == pstQuest->pNPC_TBLDAT->tblidx )
							{
								bExist = true;
								break;
							}
						}

						if( !bExist )
						{
							sNPC_TBLDAT* pNPC_TBLDAT = reinterpret_cast<sNPC_TBLDAT*>( pNPCTable->FindData(pTSEntity->uiTSEntityID) );
							if( pNPC_TBLDAT )
							{
								if( BIT_FLAG_TEST(pNPC_TBLDAT->dwFunc_Bit_Flag, NPC_FUNC_FLAG_MOVING_NPC) )
									continue;

								if( pNPC_TBLDAT->byJob == NPC_JOB_TALKER || pNPC_TBLDAT->byJob == NPC_JOB_GATE_KEEPER || pNPC_TBLDAT->byJob == NPC_JOB_COMIC_NPC ||
									pNPC_TBLDAT->bValidity_Able == false)
									continue;

								sNPC_SERVER_TBLDAT* pNpcServerTbldat = reinterpret_cast<sNPC_SERVER_TBLDAT*>(pNpcServerTbl->FindData(pNPC_TBLDAT->tblidx));
								if (pNpcServerTbldat->bValidity_Able == false)
									continue;


								sMINIMAP_NPC sNPC;
								CTable::TABLEIT it_Spawn = pNPCSpawnTbl->Begin();

								for( ; it_Spawn != pNPCSpawnTbl->End() ; ++it_Spawn )
								{
									sSPAWN_TBLDAT* pSPAWN_TBLDAT = reinterpret_cast<sSPAWN_TBLDAT*>( it_Spawn->second );
									if( pSPAWN_TBLDAT && pSPAWN_TBLDAT->mob_Tblidx == pNPC_TBLDAT->tblidx )
									{
										sNPC.v2Pos.x		= pSPAWN_TBLDAT->vSpawn_Loc.x;
										sNPC.v2Pos.y		= pSPAWN_TBLDAT->vSpawn_Loc.z;				
										sNPC.ucJob			= pNPC_TBLDAT->byJob;
										sNPC.bGetOnBus		= false;
										sNPC.pNPC_TBLDAT	= pNPC_TBLDAT;
										TransWorldToMapAxis( v2Avatar, sNPC.v2Pos, fRealRate, sNPC.v2Icon );

										m_vecNPC.push_back( sNPC );
										break;
									}
								}
							}
							else
							{
								DBO_FAIL("When searching NPC position, Can not find NPC table of index : " << pTSEntity->uiTSEntityID);
							}
						}
					}
				}
			}
		}
	}


	// Update the next quest location
	CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
	for( MAP_NEXTQUEST_ITER it = pOtherParam->GetNextQuestBegin() ; it != pOtherParam->GetNextQuestEnd(); ++it )
	{
		sNEXT_QUEST nextQuest;
		sNextQuest& SLNextQuest = it->second;		

		// It does not show if it exists in another world.
		if( Logic_GetActiveWorldId() != SLNextQuest.WorldID )
		{
			continue;
		}
		else
		{
			nextQuest.bShow			= SLNextQuest.bShow;
			nextQuest.bInfoWindow	= SLNextQuest.bShow;
		}

		SERIAL_HANDLE hHandle = INVALID_SERIAL_ID;

		if( SLNextQuest.eTargetType == eQMI_TARGET_TYPE_NPC )
		{
			hHandle = GetNtlSobManager()->GetNpcTriggerSerialFromId(SLNextQuest.Tblidx);
			CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>( GetNtlSobManager()->GetSobObject(hHandle) );

			if( pNPC )
			{
				// If there is an NPC nearby, it gets its location from the Sob object
				v3TempPos			= pNPC->GetPosition();
			}
			else
			{
				// if there is no Sob object, it fetches position data of spawn table stored in ts side
				const RwV3d* pPos = GetTSCMain()->FindNPCPosition(SLNextQuest.WorldID, SLNextQuest.Tblidx);

				v3TempPos.x = pPos->x;
				v3TempPos.y = pPos->y;
				v3TempPos.z = pPos->z;
			}

			nextQuest.pTable = SLNextQuest.pNPC_TBLDAT;
		}
		else
		{
			v3TempPos.x = SLNextQuest.v3RealPosition.x;
			v3TempPos.z = SLNextQuest.v3RealPosition.z;
		}

		nextQuest.v2Pos.x = v3TempPos.x;
		nextQuest.v2Pos.y = v3TempPos.z;

		if( IsV2MoreLength(&nextQuest.v2Pos, &v2Avatar, fLimitedDistance) )
		{
			nextQuest.eType = NQMT_OUTSIGHT;

			RwV2d v2Dir;

			RwV2dSub(&v2Dir, &nextQuest.v2Pos, &v2Avatar);
			nextQuest.fAngle = GetAngle(v2Dir, &v2Dir);

			// GUI는 x, y가 +방향이 반대방향
			v2Dir *= -1.f;

			nextQuest.v2OutCenter = v2Dir * dNEXTQUEST_ICON_DISTANCE_FROM_CENTER;

			nextQuest.v2OutCenter.x += MINIMAP_CENTER_X;
			nextQuest.v2OutCenter.y += MINIMAP_CENTER_Y;
		}
		else
		{
			if( SLNextQuest.eTargetType == eQMI_TARGET_TYPE_POSITION )
			{
				nextQuest.eType			= NQMT_INSIGHT_POSITION;
				nextQuest.bInfoWindow	= false;
			}
			else
			{
				nextQuest.eType = NQMT_INSIGHT;
			}

			TransWorldToMapAxis( v2Avatar, nextQuest.v2Pos, fRealRate, nextQuest.v2Icon );

			if( hHandle != INVALID_SERIAL_ID )
			{
				RwInt32 iCommonTargetIndex = Logic_isPartyShareTarget( hHandle );
				if( iCommonTargetIndex >= 0 )
				{
					nextQuest.bShow		= false;
					SetCommonTargetInfo(iCommonTargetIndex, nextQuest.v2Pos, nextQuest.v2Icon);
				}
			}
		}

		nextQuest.WorldID		= SLNextQuest.WorldID;
		nextQuest.eTargetType	= SLNextQuest.eTargetType;
		nextQuest.pwcText		= SLNextQuest.wstrName.c_str();

		m_vectNextQuest.push_back(nextQuest);
	}

	// 체크 포인트
	std::vector<sCHECK_POINT>::iterator it_CheckPoint = m_vecCheckPoint.begin();
	for( ; it_CheckPoint != m_vecCheckPoint.end() ; ++it_CheckPoint )
	{
		sCHECK_POINT& checkPoint = *it_CheckPoint;

		checkPoint.bShow		= true;

		if( IsV2MoreLength(&checkPoint.v2Pos, &v2Avatar, fLimitedDistance) )
		{
			checkPoint.eType = MMP_OUTSIGHT;

			RwV2d v2Dir;

			RwV2dSub(&v2Dir, &checkPoint.v2Pos, &v2Avatar);
			checkPoint.fAngle = GetAngle(v2Dir, &v2Dir);

			// GUI는 x, y가 +방향이 반대방향
			v2Dir *= -1.f;

			checkPoint.v2OutCenter = v2Dir * dNEXTQUEST_ICON_DISTANCE_FROM_CENTER;

			checkPoint.v2OutCenter.x += MINIMAP_CENTER_X;
			checkPoint.v2OutCenter.y += MINIMAP_CENTER_Y;
		}
		else
		{
			checkPoint.eType = MMP_INSIGHT;
			TransWorldToMapAxis( v2Avatar, checkPoint.v2Pos, fRealRate, checkPoint.v2Icon );
		}
	}

	// 랜드마크
	/*for each( sLANKMARK* pLandMark in m_listLandMark )
	{
		if( IsV2MoreLength(&pLandMark->v2Pos, &v2Avatar, fLimitedDistance) )
		{
			pLandMark->bShow = false;
			continue;
		}

		pLandMark->bShow = true;
		TransWorldToMapAxis( v2Avatar, pLandMark->v2Pos, fRealRate, pLandMark->v2Icon );
	}*/

	// 공유 타겟 : 혹시 위의 로직에 포함되지 않지만 공유 타겟을 가리키는 정보가 남아있는지 검사
	//			   예) 플레이어
	sSHARETARGET_INFO* pSHARETARGET_INFO = pAvatar->GetParty()->GetShareTargetInfos();
	if( pSHARETARGET_INFO )
	{
		for( RwUInt8 i = 0 ; i < NTL_MAX_SHARETARGET_COUNT ; ++i )
		{
			if( m_aCommonTarget[i].bShow )
				continue;

			if( pSHARETARGET_INFO[i].hTarget == INVALID_SERIAL_ID )
				continue;

			if( pSHARETARGET_INFO[i].hTarget == pAvatar->GetSerialID() )
				continue;

			CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pSHARETARGET_INFO[i].hTarget);
			if( !pSob )
				continue;

			RwV3d v3Pos = pSob->GetPosition();
			m_aCommonTarget[i].v2Pos.x = v3Pos.x;
			m_aCommonTarget[i].v2Pos.y = v3Pos.z;

			if( IsV2MoreLength(&m_aCommonTarget[i].v2Pos, &v2Avatar, fLimitedDistance) )
				continue;

			m_aCommonTarget[i].bShow		= true;
			m_aCommonTarget[i].bInfoWindow	= true;
			TransWorldToMapAxis( v2Avatar, m_aCommonTarget[i].v2Pos, fRealRate, m_aCommonTarget[i].v2Icon );

			CNtlSobAttr* pSobAttr = pSob->GetSobAttr();

			if( pSob->GetClassID() == SLCLASS_PLAYER )
			{
				CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobAttr );
				m_aCommonTarget[i].wstrName = (WCHAR*)pSobPlayerAttr->GetName();
			}
		}
	}

	// 도장 인장
	std::map<RwUInt32, sSCRAMBLE_SEAL*>::iterator it_Seal = m_mapScrambleSeal.begin();
	for( ; it_Seal != m_mapScrambleSeal.end() ; ++it_Seal )
	{
		sSCRAMBLE_SEAL*	pSCRAMBLE_SEAL	= it_Seal->second;

		if( IsV2MoreLength(&pSCRAMBLE_SEAL->v2Pos, &v2Avatar, fLimitedDistance) )
		{
			pSCRAMBLE_SEAL->bShow		= false;
			pSCRAMBLE_SEAL->bInfoWindow	= false;
			continue;
		}

		pSCRAMBLE_SEAL->bShow		= true;
		pSCRAMBLE_SEAL->bInfoWindow	= true;
		TransWorldToMapAxis( v2Avatar, pSCRAMBLE_SEAL->v2Pos, fRealRate, pSCRAMBLE_SEAL->v2Icon );
	}

	it_SealNotify = m_mapScrambleSealNotify.begin();
	for( ; it_SealNotify != m_mapScrambleSealNotify.end() ; ++it_SealNotify )
	{
		sSCRAMBLE_SEAL_NOTIFY* pSCRAMBLE_SEAL_NOTIFY = it_SealNotify->second;

		if( IsV2MoreLength(&pSCRAMBLE_SEAL_NOTIFY->v2Pos, &v2Avatar, fLimitedDistance) )
			continue;

		if( pSCRAMBLE_SEAL_NOTIFY->fRemainTime > 0.f )
		{
			pSCRAMBLE_SEAL_NOTIFY->fRemainTime -= fElapsed;
			if( pSCRAMBLE_SEAL_NOTIFY->fRemainTime < 0.f )
				pSCRAMBLE_SEAL_NOTIFY->fRemainTime = 0.f;
		}
		else
		{
			if( pSCRAMBLE_SEAL_NOTIFY->fRestTime > 0.f )
			{
				pSCRAMBLE_SEAL_NOTIFY->fRestTime -= fElapsed;
				if( pSCRAMBLE_SEAL_NOTIFY->fRestTime < 0.f )
					pSCRAMBLE_SEAL_NOTIFY->fRestTime = 0.f;

				continue;
			}
		}

		if( !pSCRAMBLE_SEAL_NOTIFY->pFlash )
			continue;

		RwV2d v2Icon;
		TransWorldToMapAxis( v2Avatar, pSCRAMBLE_SEAL_NOTIFY->v2Pos, fRealRate, v2Icon );

		CRectangle rtFlash = pSCRAMBLE_SEAL_NOTIFY->pFlash->GetFrameResolution();
		rtFlash.SetCenterRect((RwInt32)v2Icon.x, (RwInt32)v2Icon.y,
			rtFlash.GetWidth(), rtFlash.GetHeight());

		pSCRAMBLE_SEAL_NOTIFY->pFlash->SetPosition(rtFlash);

		if( false == pSCRAMBLE_SEAL_NOTIFY->pFlash->IsPlayMovie() )
			pSCRAMBLE_SEAL_NOTIFY->pFlash->PlayMovie(TRUE);
	}
}

VOID CMinimapGui::UpdateRegionTitle()
{
	std::wstring wstrText;
	CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	TBLIDX idxAreaInfoIndex = pWorldEntity->GetMapNameCode( GetNtlSLGlobal()->GetSobAvatar()->GetPosition() );

	if( m_idxAreaInfoIndex != idxAreaInfoIndex )
	{		
		m_idxAreaInfoIndex = idxAreaInfoIndex;

		if( m_pMapNameTextTable->GetAreaName( m_idxAreaInfoIndex, &wstrText ) == false )
		{
			if( m_pMapNameTextTable->GetSectorName( m_idxAreaInfoIndex, &wstrText ) == false )
			{
				if( m_pMapNameTextTable->GetZoneName( m_idxAreaInfoIndex, &wstrText ) == false )
				{
					if( Logic_IsUIDevInfoVisible() == false )
						wstrText = GetDisplayStringManager()->GetString("DST_MINIMAP_UNKNOWN");
				}
			}
		}

		m_pRegionTitle->SetText( wstrText.c_str() );
	}
}

VOID CMinimapGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	if( MS_HIDE == m_byMapStyle )
		return;

	UpdateRegionTitle();

	// 연산량을 줄이기 위해 일정시간마다 Update
	if( m_fElapsedTime < MINIMAP_UPDATETIME )
		return;


	m_fElapsedTime = 0.0f;

	if( m_MapItem.ReadyMakeTexture(fElapsed) )
	{
		if( MS_NORMAL == m_byMapStyle )
		{
			RwUInt32 i = 0;
			CRectangle rtScreen = m_pThis->GetScreenRect();
			CMapItem::RealRect rtArea = m_MapItem.GetAreaRect();
			RwReal fRate = ((RwReal)dGET_WORLD_PARAM()->WorldFieldSize * (RwReal)dONE_SIZE_RESOURCE_COUNT)
				/ (RwReal)dRENDER_TARGET_MAP_SIZE;

			// 랜드마크
			/*for each( sLANKMARK* pLandMark in m_listLandMark )
			{
				if( !pLandMark->bShow )
					continue;

				m_surMarkLandMark[pLandMark->eLandMarkType].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pLandMark->v2Pos.x)/fRate),
					(RwInt32)((RwReal)(rtArea.top - pLandMark->v2Pos.y)/fRate));
				m_surMarkLandMark[pLandMark->eLandMarkType].Render();
			}*/

			// 트리거 오브젝트
			for( i = 0 ; i < m_vecTriggerObject.size() ; ++i )
			{
				sTRIGGER_OBJECT& triggerObject = m_vecTriggerObject[i];

				if( triggerObject.bShow )
				{
					m_surPopoStoneActive.SetCenterPosition((RwInt32)((RwReal)(rtArea.left - triggerObject.v2Pos.x)/fRate),
						(RwInt32)((RwReal)(rtArea.top - triggerObject.v2Pos.y)/fRate));
					m_surPopoStoneActive.Render();
				}
				else
				{
					m_surPopoStone.SetCenterPosition((RwInt32)((RwReal)(rtArea.left - triggerObject.v2Pos.x) / fRate),
						(RwInt32)((RwReal)(rtArea.top - triggerObject.v2Pos.y) / fRate));
					m_surPopoStone.Render();
				}
			}

			// 히든 오브젝트
			for( i = 0 ; i < m_vecHiddenObject.size() ; ++i )
			{
				sHIDDEN_OBJECT& hiddenObject = m_vecHiddenObject[i];

				if( hiddenObject.bShow )
				{
					m_surHiddenObject.SetCenterPosition((RwInt32)((RwReal)(rtArea.left - hiddenObject.v2Pos.x)/fRate),
						(RwInt32)((RwReal)(rtArea.top - hiddenObject.v2Pos.y)/fRate));
					m_surHiddenObject.Render();
				}
			}

			// 도장 인장
			std::map<RwUInt32, sSCRAMBLE_SEAL*>::iterator it_Seal = m_mapScrambleSeal.begin();
			for( ; it_Seal != m_mapScrambleSeal.end() ; ++it_Seal )
			{
				sSCRAMBLE_SEAL* pSCRAMBLE_SEAL = it_Seal->second;

				if( pSCRAMBLE_SEAL->bShow )
				{
					m_surMarkDojoSeal[pSCRAMBLE_SEAL->eTeamSeal].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pSCRAMBLE_SEAL->v2Pos.x)/fRate),
						(RwInt32)((RwReal)(rtArea.top - pSCRAMBLE_SEAL->v2Pos.y)/fRate));
					m_surMarkDojoSeal[pSCRAMBLE_SEAL->eTeamSeal].Render();
				}
			}

			// 몬스터
			for( i = 0 ; i < m_vecMob.size() ; ++i )
			{
				sMONSTER& monster = m_vecMob[i];

				if( !monster.bShow )
					continue;

				if( monster.eMobMarkType >= NUM_MMT )
				{
					DBO_FAIL("Invalid mob mark type");
					continue;
				}

				m_surMarkMob[monster.eMobMarkType].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - monster.v2Pos.x)/fRate),
					(RwInt32)((RwReal)(rtArea.top - monster.v2Pos.y)/fRate));
				m_surMarkMob[monster.eMobMarkType].Render();
			}

			// NPC
			RwReal fNPCMarkRate = dICON_RATE_0;

			switch(m_byCurScale)
			{
				case 0:		fNPCMarkRate = dICON_RATE_0;		break;
				case 1:		fNPCMarkRate = dICON_RATE_1;		break;
				case 2:		fNPCMarkRate = dICON_RATE_2;		break;
			}

			for( i = 0 ; i < m_vecNPC.size() ; ++i )
			{
				sMINIMAP_NPC* pstNPC = &m_vecNPC[i];
				gui::CSurface* pSurface;

				if( !pstNPC->bShow )
					continue;

				if( pstNPC->bGetOnBus )
					m_surMarkNPC.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRideOnBus" ) );
				else
					m_surMarkNPC.SetSurface( Logic_GetNPCClassIconSurface(pstNPC->ucJob, false) );

				pSurface = m_surMarkNPC.GetSurface();				

				m_surMarkNPC.SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fNPCMarkRate));
				m_surMarkNPC.SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fNPCMarkRate));

				m_surMarkNPC.SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pstNPC->v2Pos.x)/fRate),
					(RwInt32)((RwReal)(rtArea.top - pstNPC->v2Pos.y)/fRate));
				m_surMarkNPC.Render();
			}

			// Quest
			CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
			for( i = 0 ; i < m_vecQuest.size() ; ++i )
			{
				sMINIMAP_QUEST* pstQuest = &m_vecQuest[i];

				if( !pstQuest->bShow )
					continue;

				if( pOtherParam->IsExistNextQuest_of_NPC(pstQuest->pNPC_TBLDAT->tblidx) == false)
					continue;

				RwInt32 nType;

				if( pstQuest->ucQuest == EQuestMarkReward )
					nType = 2;	
				else if( pstQuest->ucQuest == EQuestMarkProgress )
					nType = 1;
				else 
					nType = 0;

				m_surMarkQuest[nType].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pstQuest->v2Pos.x)/fRate),
					(RwInt32)((RwReal)(rtArea.top - pstQuest->v2Pos.y)/fRate));
				m_surMarkQuest[nType].Render();
			}

			// 다음 퀘스트
			for( i = 0 ; i < m_vectNextQuest.size() ; ++i )
			{
				sNEXT_QUEST* pNextQuest = &m_vectNextQuest[i];

				if( pNextQuest->bShow == false )
					continue;

				if( pNextQuest->WorldID != Logic_GetActiveWorldId() )
					continue;

				if( NQMT_OUTSIGHT != pNextQuest->eType )
				{
					m_surMarkNextQuest[pNextQuest->eType].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pNextQuest->v2Pos.x)/fRate),
						(RwInt32)((RwReal)(rtArea.top - pNextQuest->v2Pos.y)/fRate));
					m_surMarkNextQuest[pNextQuest->eType].Render();
				}
				// avooo's comment : Outwardly pointing icons are drawn in the OnPaint () function.
			}

			// 체크 포인트
			for( i = 0 ; i < m_vecCheckPoint.size() ; ++i )
			{
				sCHECK_POINT* pCheckPoint = &m_vecCheckPoint[i];

				if( pCheckPoint->bShow == false )
					continue;

				if( pCheckPoint->eType == MMP_INSIGHT )
				{
					m_surMarkCheckPoint[MMP_INSIGHT].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pCheckPoint->v2Pos.x)/fRate),
						(RwInt32)((RwReal)(rtArea.top - pCheckPoint->v2Pos.y)/fRate));
					m_surMarkCheckPoint[MMP_INSIGHT].Render();
				}				
				// avooo's comment : Outwardly pointing icons are drawn in the OnPaint () function.
			}

			// 파티맴버
			if( !m_tSCRAMBLE_VISIBLE.bScramble )
			{
				for( i = 0 ; i < m_vecPartyMember.size() ; ++i )
				{
					sPARTYMEMBER* pPartyMember = &m_vecPartyMember[i];

					if( pPartyMember->bShow == false )
						continue;

					if( pPartyMember->eType == MMP_INSIGHT )
					{
						m_surMarkPartyMember[MMP_INSIGHT].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - pPartyMember->v2Pos.x)/fRate),
							(RwInt32)((RwReal)(rtArea.top - pPartyMember->v2Pos.y)/fRate));
						m_surMarkPartyMember[MMP_INSIGHT].Render();
					}				
					// avooo's comment : Outwardly pointing icons are drawn in the OnPaint () function.
				}
			}

			// (Pia Division) People who fight in separate teams
			LIST_CAMP_PEOPLE::iterator it_CampPeople = m_listCampPeople.begin();
			for( ; it_CampPeople != m_listCampPeople.end() ; ++it_CampPeople )
			{
				sCAMP_PEOPLE& rCAMP_PEOPLE = *it_CampPeople;

				m_surCamp[rCAMP_PEOPLE.ePeopleType].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - rCAMP_PEOPLE.v2Pos.x)/fRate),
					(RwInt32)((RwReal)(rtArea.top - rCAMP_PEOPLE.v2Pos.y)/fRate));
				m_surCamp[rCAMP_PEOPLE.ePeopleType].Render();
			}

			// Shared target
			for( i = 0 ; i < NTL_MAX_SHARETARGET_COUNT ; ++i )
			{
				if( !m_aCommonTarget[i].bShow )
					continue;

				m_surMarkCommonTarget[i].SetCenterPosition((RwInt32)((RwReal)(rtArea.left - m_aCommonTarget[i].v2Pos.x)/fRate),
					(RwInt32)((RwReal)(rtArea.top - m_aCommonTarget[i].v2Pos.y)/fRate));
				m_surMarkCommonTarget[i].Render();
			}
		}

		m_MapItem.EndMakeTexture();		
	}	
}

VOID CMinimapGui::TransWorldToMapAxis( RwV2d& v2Avatar, RwV2d& v2d, RwReal fRate, RwV2d& V2PosResult )
{
	V2PosResult.x = MINIMAP_CENTER_X - (v2d.x - v2Avatar.x)*fRate;
	V2PosResult.y = MINIMAP_CENTER_Y - (v2d.y - v2Avatar.y)*fRate;
}

VOID CMinimapGui::NextMapStyle()
{
	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if( pWorldConcept )
	{
		if( pWorldConcept->GetConceptType() == WORLD_PLAY_RANK_BATTLE )
			return;

		if( pWorldConcept->GetConceptType() == WORLD_PLAY_TUTORIAL )
		{
			CNtlWorldConceptTutorial* pTutorialConcept = reinterpret_cast<CNtlWorldConceptTutorial*>( pWorldConcept );
			if( pTutorialConcept->IsDisableMinimap() )
				return;
		}
	}


	RwUInt8 byMapMode = GetMapStyle();

	++byMapMode;

	if( byMapMode == MS_NONE )
		byMapMode = MS_NORMAL;

	if( byMapMode == MS_MINIMAM )
	{
		Logic_PlayGUISound(GSD_SYSTEM_BUTTON_CLICK);
	}

	ChangeMapStyle(byMapMode);
}

////////////////////////////////////////////////////////////////////////////////
// Event function

VOID CMinimapGui::ChangeMapStyle(RwUInt8 byStyle)
{
	m_byMapStyle = byStyle;

	switch(m_byMapStyle)
	{
	case MS_NORMAL:
		{
			CRectangle rect = m_pThis->GetScreenRect();
			rect.SetRectWH( rect.left, rect.top, rect.GetWidth(), MINIMAP_BAR_HEIGHT + MINIMAP_BACK_HEIGHT );

			m_pMapShowBtn->Show( false );
			m_pMapHideBtn->Show( true );
			m_pExpansionBtn->Show( true );
			m_pReductionBtn->Show( true );

			SetPosition( rect );

			RwBool bPlaySound = true;

			GetDialogManager()->OpenDialog(DIALOG_MINIMAP, INVALID_SERIAL_ID, bPlaySound);

			break;
		}		
	case MS_MINIMAM:
		{
			CRectangle rect = m_pThis->GetScreenRect();
			rect.SetRectWH( rect.left, rect.top, rect.GetWidth(), MINIMAP_BAR_HEIGHT );

			m_pMapShowBtn->Show( true );
			m_pMapHideBtn->Show( false );
			m_pExpansionBtn->Show( false );
			m_pReductionBtn->Show( false );

			SetPosition( rect );

			CheckInfoWindow();

			break;
		}
	case MS_HIDE:
		{
			GetDialogManager()->CloseDialog(DIALOG_MINIMAP);

			break;
		}
	default:
		{
			DBO_FAIL("Wrong map style : " << m_byMapStyle);
			break;
		}
	}
}

RwUInt8 CMinimapGui::GetMapStyle()
{
	return m_byMapStyle;
}

RwInt32 CMinimapGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_RANK_BATTLE);
		if( pWorldConcept )
		{
			if( pWorldConcept->GetState() == WORLD_STATE_ENTER ||
				pWorldConcept->GetState() == WORLD_STATE_IDLE )
				return -1;
		}

		pWorldConcept = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_T_BUDOKAI);
		if( pWorldConcept )
		{
			if( pWorldConcept->GetState() == WORLD_STATE_ENTER ||
				pWorldConcept->GetState() == WORLD_STATE_IDLE )
				return -1;
		}

		UpdateRegionTitle();
		Show( true );
	}
	else
	{
		Show( false );

		CheckInfoWindow();
	}

	return 1;
}

VOID CMinimapGui::ZooninMiniMap()
{	
	if( m_byCurScale == (dMINIMAPSCALE_COUNT - 1) || m_byCurScale >= dMINIMAPSCALE_COUNT)
		return;

	++m_byCurScale;
	m_MapItem.SetScale(m_fMiniMapScaleLevels[m_byCurScale]);

	AdjustIconSize();
}

VOID CMinimapGui::ZoomoutMiniMap()
{
	if( m_byCurScale == 0 )
		return;

	--m_byCurScale;
	m_MapItem.SetScale(m_fMiniMapScaleLevels[m_byCurScale]);

	AdjustIconSize();
}

VOID CMinimapGui::AdjustIconSize()
{
	gui::CSurface* pSurface;
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwReal fRate = 1.0f;

	switch(m_byCurScale)
	{
		case 0:
		{			
			fRate = dICON_RATE_0;
		}
		break;
		case 1:
		{
			fRate = dICON_RATE_1;
		}
		break;
		case 2:
		{
			fRate = dICON_RATE_2;
		}
		break;
	}

	pSurface = m_surMarkAvatar.GetSurface();
	m_surMarkAvatar.SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) / fRate));
	m_surMarkAvatar.SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) / fRate));
	m_surMarkAvatar.SetCenterPosition(rtScreen.left + MINIMAP_CENTER_X, rtScreen.top + MINIMAP_CENTER_Y);

	pSurface = m_surMarkMob[0].GetSurface();
	for( RwUInt8 i = 0 ; i < NUM_MMT ; ++i )
	{
		float fNewRate = (i == MMT_HAVE_DRAGONBALL) ? fRate * 2.f : fRate;

		m_surMarkMob[i].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fNewRate));
		m_surMarkMob[i].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fNewRate));
	}	

	pSurface = m_surMarkQuest[0].GetSurface();
	for( RwUInt8 i = 0 ; i < NUM_QMT ; ++i )
	{
		m_surMarkQuest[i].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
		m_surMarkQuest[i].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));
	}

	pSurface = m_surMarkNextQuest[NQMT_INSIGHT].GetSurface();
	m_surMarkNextQuest[NQMT_INSIGHT].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surMarkNextQuest[NQMT_INSIGHT].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surMarkNextQuest[NQMT_INSIGHT_POSITION].GetSurface();
	m_surMarkNextQuest[NQMT_INSIGHT_POSITION].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surMarkNextQuest[NQMT_INSIGHT_POSITION].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surMarkCheckPoint[MMP_INSIGHT].GetSurface();
	m_surMarkCheckPoint[MMP_INSIGHT].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surMarkCheckPoint[MMP_INSIGHT].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surHiddenObject.GetSurface();
	m_surHiddenObject.SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surHiddenObject.SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surPopoStone.GetSurface();
	m_surPopoStone.SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surPopoStone.SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surPopoStoneActive.GetSurface();
	m_surPopoStoneActive.SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surPopoStoneActive.SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surMarkPartyMember[MMP_INSIGHT].GetSurface();
	m_surMarkPartyMember[MMP_INSIGHT].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
	m_surMarkPartyMember[MMP_INSIGHT].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));

	pSurface = m_surMarkCommonTarget[0].GetSurface();
	for( RwUInt8 i = 0 ; i < NTL_MAX_SHARETARGET_COUNT ; ++i )
	{
		m_surMarkCommonTarget[i].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
		m_surMarkCommonTarget[i].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));
	}	

	pSurface = m_surMarkDojoSeal[0].GetSurface();
	for( RwUInt8 i = 0 ; i < NUM_DBO_TEAM ; ++i )
	{
		m_surMarkDojoSeal[i].SetWidth((RwInt32)((RwReal)(pSurface->m_Original.rtRect.right - pSurface->m_Original.rtRect.left) * fRate));
		m_surMarkDojoSeal[i].SetHeight((RwInt32)((RwReal)(pSurface->m_Original.rtRect.bottom - pSurface->m_Original.rtRect.top) * fRate));
	}
}

VOID CMinimapGui::SetCommonTargetInfo(RwInt32 iCommonTargetIndex, RwV2d& v2Pos, RwV2d& v2Icon)
{
	m_aCommonTarget[iCommonTargetIndex].bShow	= true;
	m_aCommonTarget[iCommonTargetIndex].v2Pos	= v2Pos;
	m_aCommonTarget[iCommonTargetIndex].v2Icon	= v2Icon;
}

VOID CMinimapGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_MINIMAP &&
		GetInfoWndManager()->GetRequestGui() == DIALOG_MINIMAP )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

RwReal CMinimapGui::GetAngle(RwV2d v2Dir, RwV2d* pv3OutSqrtDir /* = NULL */)
{
	static RwV2d v2North = {0.f, 1.f};
	RwV2d v2Temp = v2Dir;
	float fSQRT = sqrt(v2Temp.x*v2Temp.x + v2Temp.y*v2Temp.y);

	v2Temp.x /= fSQRT;
	v2Temp.y /= fSQRT;

	if( pv3OutSqrtDir )
		*pv3OutSqrtDir = v2Temp;

	RwReal fTheta = acos( RwV2dDotProduct( &v2North, &v2Temp ) );

	if( v2Temp.x >= 0.0f )
		return NTL_RAD2DEG( fTheta );

	return NTL_RAD2DEG( -fTheta );
}

////////////////////////////////////////////////////////////////////////////////
// Callback function

VOID CMinimapGui::OnClicked_MapShowButton( gui::CComponent* pComponent )
{
	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TUTORIAL) )
		return;

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MINIMAP ) )
		return;

	ChangeMapStyle(MS_NORMAL);
}

VOID CMinimapGui::OnClicked_MapHideButton( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MINIMAP ) )
		return;

	ChangeMapStyle(MS_MINIMAM);
}

VOID CMinimapGui::OnClicked_WorldMap( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MINIMAP ) )
		return;

	GetDialogManager()->OpenDialog(DIALOG_WORLDMAP);
}

VOID CMinimapGui::OnClicked_Expansion( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MINIMAP ) )
		return;

	ZooninMiniMap();
}

VOID CMinimapGui::OnClicked_Reduction( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MINIMAP ) )
		return;

	ZoomoutMiniMap();
}

VOID CMinimapGui::OnPaint(VOID)
{
	if( MS_NORMAL == m_byMapStyle )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwUInt32 i = 0;
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;

		m_MapItem.Render();

		// 다음 퀘스트
		for( i = 0 ; i < m_vectNextQuest.size() ; ++i )
		{
			sNEXT_QUEST* pNextQuest = &m_vectNextQuest[i];

			if( pNextQuest->bShow == false )
				continue;

			if( pNextQuest->WorldID != Logic_GetActiveWorldId() )
				continue;

			if( pNextQuest->eType == NQMT_OUTSIGHT )
			{				
				m_surMarkNextQuest[NQMT_OUTSIGHT].SetCenterPosition(rtScreen.left + MINIMAP_CENTER_X,
					rtScreen.top + MINIMAP_CENTER_Y);
				m_surMarkNextQuest[NQMT_OUTSIGHT].SetZRot(pNextQuest->fAngle);
				m_surMarkNextQuest[NQMT_OUTSIGHT].Render();
			}

			// avooo's commnet :The inner icon is drawn in UpdateBeforeCamera ()
		}

		// Checkpoint
		for( i = 0 ; i < m_vecCheckPoint.size() ; ++i )
		{
			sCHECK_POINT* pCheckPoint = &m_vecCheckPoint[i];

			if( pCheckPoint->bShow == false )
				continue;

			if( pCheckPoint->eType == MMP_OUTSIGHT )
			{				
				m_surMarkCheckPoint[MMP_OUTSIGHT].SetCenterPosition(rtScreen.left + MINIMAP_CENTER_X,
					rtScreen.top + MINIMAP_CENTER_Y);
				m_surMarkCheckPoint[MMP_OUTSIGHT].SetZRot(pCheckPoint->fAngle);
				m_surMarkCheckPoint[MMP_OUTSIGHT].Render();
			}

			// avooo's commnet : The inner icon is drawn in UpdateBeforeCamera ()
		}

		// 파티원
		if( !m_tSCRAMBLE_VISIBLE.bScramble )
		{
			for( i = 0 ; i < m_vecPartyMember.size() ; ++i )
			{
				sPARTYMEMBER* partyMember = &m_vecPartyMember[i];

				if( partyMember->bShow == false )
					continue;

				if( partyMember->eType == MMP_OUTSIGHT )
				{				
					m_surMarkPartyMember[MMP_OUTSIGHT].SetCenterPosition(rtScreen.left + MINIMAP_CENTER_X,
						rtScreen.top + MINIMAP_CENTER_Y);
					m_surMarkPartyMember[MMP_OUTSIGHT].SetZRot(partyMember->fAngle);
					m_surMarkPartyMember[MMP_OUTSIGHT].Render();
				}

				// avooo's commnet : The inner icon is drawn in UpdateBeforeCamera ()
			}
		}


		if( m_hBus_with_Avatar == INVALID_SERIAL_ID )
			m_surMarkAvatar.Render();

		m_surBack.Render();
	}

	m_surFrame.Render();
}

VOID CMinimapGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_surBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_surFrame.SetPosition( rtScreen.left, rtScreen.top + MINIMAP_BAR_Y );

	m_surMarkAvatar.SetCenterPosition( rtScreen.left + MINIMAP_CENTER_X, rtScreen.top + MINIMAP_CENTER_Y );

	m_MapItem.SetPosition(rtScreen.left + 20, rtScreen.top + 35);

	CheckInfoWindow();
}

VOID CMinimapGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwUInt32 i = 0;	
	RwInt32 iHalfWidth, iHalfHeight;
	MINIMAPINFO_LIST listMarkInfo;
	RwReal fRate;
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	switch(m_byCurScale)
	{
		case 0:
		{			
			fRate = dICON_RATE_0;
			break;
		}
		case 1:
		{
			fRate = dICON_RATE_1;
			break;
		}
		case 2:
		{
			fRate = dICON_RATE_2;
			break;
		}
		default:
		{
			fRate = dICON_RATE_1;
			break;
		}
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();

	// 아바타
	if( m_surMarkAvatar.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
	{
		CNtlParty*		pParty = pAvatar->GetParty();
		sMINIMAPINFO	info;

		if( pParty->IsHaveGroup() )
			info.iType = MMIT_PARTY;
		else
			info.iType = MMIT_NPC;

		info.wcsString = Logic_GetAvatarName();

		listMarkInfo.push_back(info);
	}

	// 몬스터
	CTextTable* pMonsterTextTable = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

	iHalfWidth = ((RwInt32)((RwReal)m_surMarkMob[MMT_NORMAL].GetWidth() / 2.f / fRate));
	iHalfHeight = ((RwInt32)((RwReal)m_surMarkMob[MMT_NORMAL].GetHeight() / 2.f / fRate));

	for( i = 0 ; i < m_vecMob.size() ; ++i )
	{
		sMONSTER& sMonster = m_vecMob[i];

		if( !sMonster.bInfoWindow )
			continue;

		if( sMonster.v2Icon.x - iHalfWidth  <= nX && nX <= sMonster.v2Icon.x + iHalfWidth &&
			sMonster.v2Icon.y - iHalfHeight <= nY && nY <= sMonster.v2Icon.y + iHalfHeight )
		{			
			sMINIMAPINFO info;

			info.iType		= MMIT_MOB;

			RwBool bCanSeeName = Logic_IsValidScouterEquipped(sMonster.pMOB_TBLDAT->bShow_Name);

			if(bCanSeeName)
				info.wcsString = pMonsterTextTable->GetText(sMonster.pMOB_TBLDAT->Name);
			else
				info.wcsString = GetDisplayStringManager()->GetString( "DST_STATUS_UNIDENTIFIED_NAME" );

			listMarkInfo.push_back(info);
		}			
	}

	// NPC
	CTextTable* pNPCTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();

	iHalfWidth = ((RwInt32)((RwReal)m_surMarkNPC.GetWidth() / 2.f));
	iHalfHeight = ((RwInt32)((RwReal)m_surMarkNPC.GetHeight() / 2.f));

	for( i = 0 ; i < m_vecNPC.size() ; ++i )
	{
		sMINIMAP_NPC& sNPC = m_vecNPC[i];	

		if( !sNPC.bInfoWindow )
			continue;

		if( sNPC.v2Icon.x - iHalfWidth  <= nX && nX <= sNPC.v2Icon.x + iHalfWidth &&
			sNPC.v2Icon.y - iHalfHeight <= nY && nY <= sNPC.v2Icon.y + iHalfHeight )
		{			
			sMINIMAPINFO info;

			info.iType		= MMIT_NPC;
			info.wcsString	= pNPCTextTable->GetText(sNPC.pNPC_TBLDAT->Name);

			const WCHAR* wchJobName = Logic_GetNPCJobName(sNPC.ucJob);
			if (wchJobName)
			{
				info.wcsString += L" : ";
				info.wcsString += wchJobName;
			}

			listMarkInfo.push_back(info);
		}
	}

	// 퀘스트
	iHalfWidth = ((RwInt32)((RwReal)m_surMarkQuest[0].GetWidth() / 2.f / fRate));
	iHalfHeight = ((RwInt32)((RwReal)m_surMarkQuest[0].GetHeight() / 2.f / fRate));

	CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
	for( i = 0 ; i < m_vecQuest.size() ; ++i )
	{
		sMINIMAP_QUEST& sQuest = m_vecQuest[i];

		if( !sQuest.bInfoWindow )
			continue;

		if( pOtherParam->IsExistNextQuest_of_NPC(sQuest.pNPC_TBLDAT->tblidx) )
			continue;

		if( sQuest.v2Icon.x - iHalfWidth <= nX && nX <= sQuest.v2Icon.x + iHalfWidth &&
			sQuest.v2Icon.y - iHalfHeight <= nY && nY <= sQuest.v2Icon.y + iHalfHeight )
		{			
			sMINIMAPINFO info;

			info.iType = MMIT_QUEST;
			info.wcsString = pNPCTextTable->GetText(sQuest.pNPC_TBLDAT->Name);

			const WCHAR* wchJobName = Logic_GetNPCJobName(sQuest.pNPC_TBLDAT->byJob);
			if (wchJobName)
			{
				info.wcsString += L" : ";
				info.wcsString += wchJobName;
			}

			listMarkInfo.push_back(info);
		}			
	}

	// 다음 진행 퀘스트	
	for( i = 0 ; i < m_vectNextQuest.size() ; ++i )
	{
		sNEXT_QUEST& nextQuest = m_vectNextQuest[i];

		if( !nextQuest.bInfoWindow )
			continue;

		RwBool bInIcon = false;

		if( NQMT_INSIGHT			== nextQuest.eType ||
			NQMT_INSIGHT_POSITION	== nextQuest.eType )
		{
			iHalfWidth = ((RwInt32)((RwReal)m_surMarkNextQuest[nextQuest.eType].GetWidth() / 2.f / fRate));
			iHalfHeight = ((RwInt32)((RwReal)m_surMarkNextQuest[nextQuest.eType].GetHeight() / 2.f / fRate));

			if( nextQuest.v2Icon.x - iHalfWidth  <= nX && nX <= nextQuest.v2Icon.x + iHalfWidth &&
				nextQuest.v2Icon.y - iHalfHeight <= nY && nY <= nextQuest.v2Icon.y + iHalfHeight )
				bInIcon = true;
		}
		else
		{
			RwV2d v2Mouse;
			v2Mouse.x = (RwReal)nX;
			v2Mouse.y = (RwReal)nY;

			bInIcon = IsV2MoreLength(&v2Mouse, &nextQuest.v2OutCenter, dNEXTQUEST_ICON_RADIUS);
			bInIcon = !bInIcon;
		}

		if( bInIcon )
		{
			sMINIMAPINFO info;

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

			if( nextQuest.eTargetType == eQMI_TARGET_TYPE_NPC )
			{
				info.iType = MMIT_QUEST;

				sNPC_TBLDAT* pNPC_TBLDAT = reinterpret_cast<sNPC_TBLDAT*>( nextQuest.pTable );

				const WCHAR* wchJobName = Logic_GetNPCJobName(pNPC_TBLDAT->byJob);
				if (wchJobName)
				{
					info.wcsString += L" : ";
					info.wcsString += wchJobName;
				}
			}
			else
			{
				info.iType = MMIT_NEXTQUEST_POSITION;
			}			

			listMarkInfo.push_back(info);
		}
	}

	// Checkpoint
	for( i = 0 ; i < m_vecCheckPoint.size() ; ++i )
	{
		sCHECK_POINT& checkPoint = m_vecCheckPoint[i];

		if( checkPoint.bShow == false )
			continue;

		RwBool bInIcon = false;

		if( checkPoint.eType == MMP_INSIGHT )
		{
			iHalfWidth = ((RwInt32)((RwReal)m_surMarkCheckPoint[MMP_INSIGHT].GetWidth() / 2.f / fRate));
			iHalfHeight = ((RwInt32)((RwReal)m_surMarkCheckPoint[MMP_INSIGHT].GetHeight() / 2.f / fRate));

			if( checkPoint.v2Icon.x - iHalfWidth  <= nX && nX <= checkPoint.v2Icon.x + iHalfWidth &&
				checkPoint.v2Icon.y - iHalfHeight <= nY && nY <= checkPoint.v2Icon.y + iHalfHeight )
				bInIcon = true;
		}
		else
		{
			RwV2d v2Mouse;
			v2Mouse.x = (RwReal)nX;
			v2Mouse.y = (RwReal)nY;

			bInIcon = IsV2MoreLength(&v2Mouse, &checkPoint.v2OutCenter, dNEXTQUEST_ICON_RADIUS);
			bInIcon = !bInIcon;
		}

		if( bInIcon )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			sMINIMAPINFO info;

			info.iType = MMIT_QUEST;
			info.wcsString = checkPoint.wstrText;

			listMarkInfo.push_back(info);
		}
	}

	// Trigger object
	for( i = 0 ; i < m_vecTriggerObject.size() ; ++i )
	{
		sTRIGGER_OBJECT& triggerObject = m_vecTriggerObject[i];

		if( !triggerObject.bInfoWindow )
			continue;

		iHalfWidth = m_surPopoStone.GetWidth() / 2;
		iHalfHeight = m_surPopoStone.GetHeight() / 2;

		if( triggerObject.v2Icon.x - iHalfWidth  <= nX && nX <= triggerObject.v2Icon.x + iHalfWidth &&
			triggerObject.v2Icon.y - iHalfHeight <= nY && nY <= triggerObject.v2Icon.y + iHalfHeight )
		{
			sMINIMAPINFO info;

			info.iType = MMIT_BIND_POS;
			info.wcsString = triggerObject.wstrText;

			listMarkInfo.push_back(info);
		}
	}

	// Hidden Object
	for( i = 0 ; i < m_vecHiddenObject.size() ; ++i )
	{
		sHIDDEN_OBJECT& hiddenObject = m_vecHiddenObject[i];

		if( !hiddenObject.bInfoWindow )
			continue;

		iHalfWidth = m_surHiddenObject.GetWidth() / 2;
		iHalfHeight = m_surHiddenObject.GetHeight() / 2;

		if( hiddenObject.v2Icon.x - iHalfWidth  <= nX && nX <= hiddenObject.v2Icon.x + iHalfWidth &&
			hiddenObject.v2Icon.y - iHalfHeight <= nY && nY <= hiddenObject.v2Icon.y + iHalfHeight )
		{
			sMINIMAPINFO info;

			info.iType = MMIT_BIND_POS;
			info.wcsString = hiddenObject.wstrText;

			listMarkInfo.push_back(info);
		}
	}

	if( m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam || m_tSCRAMBLE_VISIBLE.bShowScrambleEnemyTeam )
	{
		// 도장전 인원
		LIST_CAMP_PEOPLE::iterator it_CampPeople = m_listCampPeople.begin();
		for( ; it_CampPeople != m_listCampPeople.end() ; ++it_CampPeople )
		{
			sCAMP_PEOPLE& rCAMP_PEOPLE = *it_CampPeople;

			if( rCAMP_PEOPLE.ePeopleType >= NUM_CAMP_PEOPLE )
				continue;

			iHalfWidth	= m_surCamp[rCAMP_PEOPLE.ePeopleType].GetWidth() / 2;
			iHalfHeight	= m_surCamp[rCAMP_PEOPLE.ePeopleType].GetHeight() / 2;

			if( rCAMP_PEOPLE.v2Icon.x - iHalfWidth  <= nX && nX <= rCAMP_PEOPLE.v2Icon.x + iHalfWidth &&
				rCAMP_PEOPLE.v2Icon.y - iHalfHeight <= nY && nY <= rCAMP_PEOPLE.v2Icon.y + iHalfHeight )
			{
				sMINIMAPINFO info;

				if( CAMP_PEOPLE_MY_PARTY == rCAMP_PEOPLE.ePeopleType )
				{
					if( !m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam )
						continue;

					info.iType = MMIT_PARTY;
				}
				else if( CAMP_PEOPLE_MY_TEAM == rCAMP_PEOPLE.ePeopleType )
				{
					if( !m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam )
						continue;

					info.iType = MMIT_NPC;
				}
				else if( CAMP_PEOPLE_EMENY_TEAM == rCAMP_PEOPLE.ePeopleType )
				{
					if( !m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam )
						continue;

					info.iType = CAMP_PEOPLE_EMENY_TEAM;
				}

				info.wcsString = rCAMP_PEOPLE.pwcName;

				listMarkInfo.push_back(info);
			}
		}
	}
	
	if( !m_tSCRAMBLE_VISIBLE.bScramble )
	{
		// 파티맴버
		for( i = 0 ; i < m_vecPartyMember.size() ; ++i )
		{
			sPARTYMEMBER& partyMember = m_vecPartyMember[i];
			if( !partyMember.bInfoWindow )
				continue;

			RwBool bInIcon = false;

			if( partyMember.eType == MMP_INSIGHT )
			{				
				iHalfWidth = ((RwInt32)((RwReal)m_surMarkPartyMember[MMP_INSIGHT].GetWidth() / 2.f / fRate));
				iHalfHeight = ((RwInt32)((RwReal)m_surMarkPartyMember[MMP_INSIGHT].GetHeight() / 2.f / fRate));

				if( partyMember.v2Icon.x - iHalfWidth  <= nX && nX <= partyMember.v2Icon.x + iHalfWidth &&
					partyMember.v2Icon.y - iHalfHeight <= nY && nY <= partyMember.v2Icon.y + iHalfHeight )
					bInIcon = true;
			}
			else
			{
				RwV2d v2Mouse;
				v2Mouse.x = (RwReal)nX;
				v2Mouse.y = (RwReal)nY;

				bInIcon = IsV2MoreLength(&v2Mouse, &partyMember.v2OutCenter, dNEXTQUEST_ICON_RADIUS);
				bInIcon = !bInIcon;
			}

			m_surMarkNPC.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRideOnBus" ) );

			if( bInIcon )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				sMINIMAPINFO info;

				info.iType = MMIT_PARTY;
				info.wcsString = partyMember.pwcName;

				listMarkInfo.push_back(info);
			}
		}
	}	

	// 랜드마크
	/*for each( sLANKMARK* pLandMark in m_listLandMark )
	{
		if( !pLandMark->bShow )
			continue;

		iHalfWidth = m_surMarkLandMark[pLandMark->eLandMarkType].GetWidth() / 2;
		iHalfHeight = m_surMarkLandMark[pLandMark->eLandMarkType].GetHeight() / 2;

		if( pLandMark->v2Icon.x - iHalfWidth  <= nX && nX <= pLandMark->v2Icon.x + iHalfWidth &&
			pLandMark->v2Icon.y - iHalfHeight <= nY && nY <= pLandMark->v2Icon.y + iHalfHeight )
		{
			sMINIMAPINFO info;

			info.iType = MMIT_LANDMARK;
			info.wcsString = pLandMark->pwcName;

			listMarkInfo.push_back(info);
		}
	}*/

	// 공유 타겟
	for( RwUInt8 j = 0 ; j < NTL_MAX_SHARETARGET_COUNT ; ++j )
	{
		if( !m_aCommonTarget[j].bInfoWindow )
			continue;

		iHalfWidth = m_surMarkCommonTarget[j].GetWidth() / 2;
		iHalfHeight = m_surMarkCommonTarget[j].GetHeight() / 2;

		if( m_aCommonTarget[j].v2Icon.x - iHalfWidth  <= nX && nX <= m_aCommonTarget[j].v2Icon.x + iHalfWidth &&
			m_aCommonTarget[j].v2Icon.y - iHalfHeight <= nY && nY <= m_aCommonTarget[j].v2Icon.y + iHalfHeight )
		{
			sMINIMAPINFO info;

			info.iType = MMIT_NPC;
			info.wcsString = m_aCommonTarget[j].wstrName;

			listMarkInfo.push_back(info);
		}
	}

	// 도장 인장
	std::map<RwUInt32, sSCRAMBLE_SEAL*>::iterator it_Seal = m_mapScrambleSeal.begin();
	for( ; it_Seal != m_mapScrambleSeal.end() ; ++it_Seal )
	{
		sSCRAMBLE_SEAL* pSCRAMBLE_SEAL = it_Seal->second;

		if( !pSCRAMBLE_SEAL->bInfoWindow )
			continue;

		iHalfWidth	= m_surMarkDojoSeal[pSCRAMBLE_SEAL->eTeamSeal].GetWidth() / 2;
		iHalfHeight	= m_surMarkDojoSeal[pSCRAMBLE_SEAL->eTeamSeal].GetHeight() / 2;

		if( pSCRAMBLE_SEAL->v2Icon.x - iHalfWidth  <= nX && nX <= pSCRAMBLE_SEAL->v2Icon.x + iHalfWidth &&
			pSCRAMBLE_SEAL->v2Icon.y - iHalfHeight <= nY && nY <= pSCRAMBLE_SEAL->v2Icon.y + iHalfHeight )
		{
			sMINIMAPINFO info;

			info.iType = MMIT_NPC;
			info.wcsString = pSCRAMBLE_SEAL->wstrName;

			listMarkInfo.push_back(info);
		}
	}

	if( listMarkInfo.size() > 0 )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_MINIMAP,
			rtScreen.left + nX, rtScreen.top + nY, (VOID*)&listMarkInfo, DIALOG_MINIMAP );
	}
	else
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CMinimapGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CMinimapGui::OnRegionTitleMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_JUST_WTEXT )
		return;


	std::wstring& wstrText = m_pRegionTitle->GetText();
	if( wstrText.empty() )
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect = m_pRegionTitle->GetPosition();


	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
		rtScreen.left + rtRect.left, rtScreen.top + rtRect.top,
		(VOID*)wstrText.c_str(), DIALOG_MINIMAP );
}

VOID CMinimapGui::OnRegionTitleMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CMinimapGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	if( sDelta > 0 )
	{
		ZooninMiniMap();
	}
	else
	{		
		ZoomoutMiniMap();
	}
}

VOID CMinimapGui::HandleEvents( RWS::CMsg& msg )
{
	NTL_FUNCTION( "CMinimapGui::HandleEvents" );

	if( msg.Id == g_EventMap )
	{
		SDboEventMap* pEvent = reinterpret_cast<SDboEventMap*>(msg.pData);

		switch(pEvent->iMessage)
		{
			case MMT_MINIMAP_ZOON_IN:
			{
				ZooninMiniMap();
				break;
			}
			case MMT_MINIMAP_ZOON_OUT:
			{
				ZoomoutMiniMap();
				break;
			}
			case MMT_APPEAR_HIDDEN_OBJECT:
			{
				sHIDDEN_OBJECT hiddenObject;
				CNtlSobTriggerObject* pSobTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>( GetNtlSobManager()->GetSobObject(pEvent->uiValue) );
				if( !pSobTriggerObject )
				{
					DBO_FAIL("Not exist sob object of handle : " << pEvent->uiValue);
					break;
				}
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				RwV3d v3Avatar = pAvatar->GetPosition();
				RwV2d v2Avatar;

				v2Avatar.x	= v3Avatar.x;
				v2Avatar.y	= v3Avatar.z;

				hiddenObject.hHandle		= pEvent->uiValue;
				hiddenObject.v2Pos.x		= pSobTriggerObject->GetPosition().x;
				hiddenObject.v2Pos.y		= pSobTriggerObject->GetPosition().z;
				TransWorldToMapAxis( v2Avatar, hiddenObject.v2Pos, m_MapItem.GetRate(), hiddenObject.v2Icon );
				hiddenObject.bShow			= true;
				hiddenObject.bInfoWindow	= true;
				hiddenObject.wstrText		= GetDisplayStringManager()->GetString("DST_MINIMAP_HIDDEN");

				m_vecHiddenObject.push_back(hiddenObject);
				break;
			}
			case MMT_DISAPPEAR_HIDDEN_OBJECT:
			{
				std::vector<sHIDDEN_OBJECT>::iterator it = m_vecHiddenObject.begin();
				for( ; it != m_vecHiddenObject.end() ; ++it )
				{
					sHIDDEN_OBJECT& hiddenObject = *it;
					if( hiddenObject.hHandle == pEvent->uiValue )
					{
						m_vecHiddenObject.erase(it);
						break;
					}
				}
				break;
			}
			case MMT_MINIMAP_SHOW_OUR_TEAM:
			{
				m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam = TRUE;
				break;
			}
			case MMT_MINIMAP_HIDE_OUR_TEAM:
			{
				m_tSCRAMBLE_VISIBLE.bShowScrambleMyTeam = FALSE;
				break;
			}
			case MMT_MINIMAP_SHOW_OTHER_TEAM:
			{
				m_tSCRAMBLE_VISIBLE.bShowScrambleEnemyTeam = TRUE;
				break;
			}
			case MMT_MINIMAP_HIDE_OTHER_TEAM:
			{
				m_tSCRAMBLE_VISIBLE.bShowScrambleEnemyTeam = FALSE;
				break;
			}
		}
	}
	else if( msg.Id == g_EventDirectionNfy )
	{
		SNtlEventDirectionNfy* pEvent = reinterpret_cast<SNtlEventDirectionNfy*>( msg.pData );

		if( pEvent->bCreate )
		{
			sCHECK_POINT checkPoint;

			checkPoint.v2Pos.x		= pEvent->vLoc.x;
			checkPoint.v2Pos.y		= pEvent->vLoc.z;

			// 체크 포인트
			checkPoint.wstrText		= GetDisplayStringManager()->GetString("DST_MINIMAP_CHECK_POINT");

			// 기획상 한번에 체크 포인트는 한번에 하나만....
			m_vecCheckPoint.clear();

			m_vecCheckPoint.push_back(checkPoint);
		}
		else
		{
			m_vecCheckPoint.clear();

			/*	차후 기획이 바뀌어 여러개의 체크 포인트가 생길 수 있을 때를 위하여 봉인...
			std::vector<sCHECK_POINT>::iterator it = m_vecCheckPoint.begin();
			for( ; it != m_vecCheckPoint.end() ; ++it )
			{
				sCHECK_POINT& checkPoint = *it;

				if( checkPoint.v2Pos.x == pEvent->vLoc.x &&
					checkPoint.v2Pos.y == pEvent->vLoc.z )
				{
					m_vecCheckPoint.erase(it);
					break;
				}
			}
			*/
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
				ChangeMapStyle(MS_NORMAL);
		}
		else if( pEvent->eWorldConcept == WORLD_PLAY_TIME_MACHINE )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
				m_vecCheckPoint.clear();
		}
		else if( pEvent->eWorldConcept == WORLD_PLAY_T_BUDOKAI ||
			pEvent->eWorldConcept == WORLD_PLAY_RANK_BATTLE )
		{
			if( pEvent->uiState == WORLD_STATE_ENTER )
				GetDialogManager()->CloseDialog( DIALOG_MINIMAP );
		}
		else if( pEvent->eWorldConcept == WORLD_PLAY_DOJO_SCRAMBLE )
		{
			if( pEvent->uiState == WORLD_STATE_ENTER )
			{
				m_tSCRAMBLE_VISIBLE.bScramble = TRUE;
			}
			else if( pEvent->uiState == WORLD_STATE_EXIT ||
				pEvent->uiState == WORLD_STATE_NONE )
			{
				m_tSCRAMBLE_VISIBLE.bScramble = FALSE;
				ClearSeal();
			}
		}
		else if (pEvent->eWorldConcept == WORLD_PLAY_CCBD) // hide minimap in ccbd
		{
			if (pEvent->uiState == WORLD_STATE_ENTER)
				GetDialogManager()->CloseDialog(DIALOG_MINIMAP);
		}
	}
	else if( msg.Id == g_EventDojoNotify )
	{		
		if( false == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		// 인장 정보는 도장 쟁탈전중 생성되면 도장 쟁탈전이 끝날 때까지 삭제하지 않는다
		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		if( DOJO_EVENT_SEAL_ATTACK_BEGIN == pEvent->byDojoEvent )
		{
			sSCRAMBLE_SEAL_NOTIFY* pSCRAMBLE_SEAL_NOTIFY = NULL;
			TBLIDX			idxSealObject	= pEvent->uiParam;
			bool			bOffence		= *(bool*)pEvent->pExData;
			sVECTOR2*		pV2Loc			= (sVECTOR2*)pEvent->pExData2;

			std::map<RwUInt32, sSCRAMBLE_SEAL_NOTIFY*>::iterator it_SealNotify = m_mapScrambleSealNotify.find(idxSealObject);


			if( it_SealNotify == m_mapScrambleSealNotify.end() )
			{
				pSCRAMBLE_SEAL_NOTIFY = NTL_NEW sSCRAMBLE_SEAL_NOTIFY;
				pSCRAMBLE_SEAL_NOTIFY->pFlash = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());				

				if( bOffence )
					pSCRAMBLE_SEAL_NOTIFY->pFlash->Load(dSEAL_ENEMY_FLASH_FILE);
				else
					pSCRAMBLE_SEAL_NOTIFY->pFlash->Load(dSEAL_MY_TEAM_FLASH_FILE);

				pSCRAMBLE_SEAL_NOTIFY->pFlash->Enable(false);
				pSCRAMBLE_SEAL_NOTIFY->pFlash->AutoRewind(true);				

				CRectangle rtQuest = pSCRAMBLE_SEAL_NOTIFY->pFlash->GetFrameResolution();
				pSCRAMBLE_SEAL_NOTIFY->pFlash->SetSize(rtQuest.GetWidth(), rtQuest.GetHeight());

				pSCRAMBLE_SEAL_NOTIFY->bEnable		= true;
				pSCRAMBLE_SEAL_NOTIFY->bOffence		= bOffence;
				pSCRAMBLE_SEAL_NOTIFY->v2Pos.x		= pV2Loc->x;
				pSCRAMBLE_SEAL_NOTIFY->v2Pos.y		= pV2Loc->z;
				pSCRAMBLE_SEAL_NOTIFY->fRemainTime	= dSEAL_REMAIN_TIME;
				pSCRAMBLE_SEAL_NOTIFY->fRestTime	= dSEAL_REST_TIME;

				m_mapScrambleSealNotify[idxSealObject]	= pSCRAMBLE_SEAL_NOTIFY;
			}
			else
			{
				pSCRAMBLE_SEAL_NOTIFY = it_SealNotify->second;

				if( B2b(pSCRAMBLE_SEAL_NOTIFY->bOffence) == bOffence )
				{
					if( pSCRAMBLE_SEAL_NOTIFY->fRemainTime	> 0.f || 
						pSCRAMBLE_SEAL_NOTIFY->fRestTime > 0.f )
						NTL_RETURNVOID();
				}

				if( bOffence )
					pSCRAMBLE_SEAL_NOTIFY->pFlash->Load(dSEAL_ENEMY_FLASH_FILE);
				else
					pSCRAMBLE_SEAL_NOTIFY->pFlash->Load(dSEAL_MY_TEAM_FLASH_FILE);

				pSCRAMBLE_SEAL_NOTIFY->pFlash->PlayMovie(TRUE);

				pSCRAMBLE_SEAL_NOTIFY->bEnable		= true;
				pSCRAMBLE_SEAL_NOTIFY->bOffence		= bOffence;
				pSCRAMBLE_SEAL_NOTIFY->v2Pos.x		= pV2Loc->x;
				pSCRAMBLE_SEAL_NOTIFY->v2Pos.y		= pV2Loc->z;
				pSCRAMBLE_SEAL_NOTIFY->fRemainTime	= dSEAL_REMAIN_TIME;
				pSCRAMBLE_SEAL_NOTIFY->fRestTime	= dSEAL_REST_TIME;
			}
		}
		else if( DOJO_EVENT_SEAL_ATTACK_END == pEvent->byDojoEvent )
		{
			TBLIDX idxSealObject = pEvent->uiParam;
			std::map<RwUInt32, sSCRAMBLE_SEAL_NOTIFY*>::iterator it_SealNotify = m_mapScrambleSealNotify.find(idxSealObject);
			if( it_SealNotify == m_mapScrambleSealNotify.end() )
				NTL_RETURNVOID();

			sSCRAMBLE_SEAL_NOTIFY* pSCRAMBLE_SEAL_NOTIFY = it_SealNotify->second;
			pSCRAMBLE_SEAL_NOTIFY->pFlash->Unload();
			pSCRAMBLE_SEAL_NOTIFY->bEnable = false;
		}
		else if( DOJO_EVENT_SEAL_ATTACK_STATE == pEvent->byDojoEvent )
		{
			TBLIDX			idxObject		= pEvent->uiParam;
			RwUInt8			byState			= *(RwUInt8*)pEvent->pExData;
			sVECTOR2		v2Loc			= *(sVECTOR2*)pEvent->pExData2;

			std::map<RwUInt32, sSCRAMBLE_SEAL*>::iterator it_Seal = m_mapScrambleSeal.find(idxObject);

			if( it_Seal != m_mapScrambleSeal.end() )
			{
				DBO_FAIL("Already exist seal of object table index : " << idxObject);
				NTL_RETURNVOID();
			}

			CObjectTable* pObjectTable = API_GetTableContainer()->GetObjectTable( Logic_GetActiveWorldId() );
			if( pObjectTable == NULL )
			{
				DBO_FAIL("Not exist obejct table of world ID : " << Logic_GetActiveWorldId());
				NTL_RETURNVOID();
			}


			if( MAX_TOBJECT_STATE_TYPE_C <= (eTOBJECT_STATE_TYPE_C)byState )
			{
				DBO_FAIL("Invalid eTOBJECT_STATE_TYPE_C : " << byState);
				byState = TOBJECT_STATE_TYPE_C_DEFENCE_TEAM;
			}


			sSCRAMBLE_SEAL* pSCRAMBLE_SEAL = NTL_NEW sSCRAMBLE_SEAL;

			pSCRAMBLE_SEAL->eTeamSeal		= Logic_GetScrambleTeam((eTOBJECT_STATE_TYPE_C)byState);
			pSCRAMBLE_SEAL->v2Pos.x			= v2Loc.x;
			pSCRAMBLE_SEAL->v2Pos.y			= v2Loc.z;
			pSCRAMBLE_SEAL->v2Icon.x		= 0.f;
			pSCRAMBLE_SEAL->v2Icon.y		= 0.f;
			pSCRAMBLE_SEAL->bShow			= false;
			pSCRAMBLE_SEAL->bInfoWindow		= false;


			sOBJECT_TBLDAT* pOBJECT_TBLDAT		= reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTable->FindData(idxObject) );
			if( pOBJECT_TBLDAT )
			{
				CTextTable* pObjectTextTable	= API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
				pSCRAMBLE_SEAL->wstrName		= pObjectTextTable->GetText(pOBJECT_TBLDAT->dwName);
			}
			else
			{
				WCHAR awcBuffer[128];
				swprintf_s(awcBuffer, 128, L"Not exist sOBJECT_TBLDAT of index : %d", idxObject);
				DBO_FAIL(awcBuffer);
				pSCRAMBLE_SEAL->wstrName		= awcBuffer;
			}

			m_mapScrambleSeal[idxObject] = pSCRAMBLE_SEAL;
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

			std::map<RwUInt32, sSCRAMBLE_SEAL*>::iterator it_Seal = m_mapScrambleSeal.find(idxObject);
			if( it_Seal == m_mapScrambleSeal.end() )
			{
				DBO_FAIL("Not exist seal of object table index : " << idxObject);
				NTL_RETURNVOID();
			}

			sSCRAMBLE_SEAL_INFO*	pSCRAMBLE_SEAL_INFO	= it_ScrambleSealInfo->second;
			sSCRAMBLE_SEAL*			pSCRAMBLE_SEAL		= it_Seal->second;

			pSCRAMBLE_SEAL->eTeamSeal = Logic_GetScrambleTeam(pSCRAMBLE_SEAL_INFO->eState);
		}
	}
	if( msg.Id == g_EventDojoStateNotify )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( msg.pData );

		if( eDBO_DOJO_STATUS_END == pEvent->byDojoState )
		{
			ClearSeal();
		}
	}

	NTL_RETURNVOID();
}