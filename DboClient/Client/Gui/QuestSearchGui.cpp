#include "precomp_dboclient.h"
#include "QuestSearchGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "TextAllTable.h"
#include "NPCTable.h"
#include "TableContainer.h"

// gui
#include "GuiUtil.h"

// presetation
#include "NtlPLGuiManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLVisualManager.h"

// simulation
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "DboTSCQAgency.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

// client
#include "DialogPriority.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DialogDefine.h"

#define dQUEST_MARK			"scouter_radar_scanpoint.swf"

#define dADJUST_AREA_LEFT			8
#define dADJUST_AREA_RIGHT			8
#define dADJUST_AREA_TOP			27
#define dADJUST_AREA_BOTTOM			14

#define dSEARCH_RADIUS				1500.f
#define dMAP_DEFAULT_REDUCED_SCALE	0.125f

CQuestSearchGui::CQuestSearchGui(const RwChar* pName)
{
}

CQuestSearchGui::~CQuestSearchGui()
{

}

RwBool CQuestSearchGui::Create(RwReal fMapScale)
{
	NTL_FUNCTION( "CQuestSearchGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\QuestSearch.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_WORLDMAP_QUEST_SEARCH);

	CRectangle rect;
	RwReal fReducedScale = fMapScale/dMAP_DEFAULT_REDUCED_SCALE;

	// avooo's commnet : 이 부분을 테이블로 빼자. 월드별로 검색 범위를 다르게...
	if( 5 == Logic_GetActiveWorldTableId() )
		fReducedScale /= 3.f;

	// 측정시의 배경
	m_pFlashSearch = (gui::CFlash*)GetComponent("flaQuestSearch");
	m_slotMovieEnd = m_pFlashSearch->SigMovieEnd().Connect( this, &CQuestSearchGui::OnMovieEnd );
	rect = m_pFlashSearch->GetFrameResolution();

	m_pFlashSearch->SetSize((RwInt32)((RwReal)rect.GetWidth() * fReducedScale), (RwInt32)((RwReal)rect.GetHeight() * fReducedScale));

	if( !SetQuestInfo() )
		NTL_RETURN(FALSE);

	// sig	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CQuestSearchGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CQuestSearchGui::OnMouseLeave );

	Show(true);

	NTL_RETURN(TRUE);
}

VOID CQuestSearchGui::Destroy()
{
	NTL_FUNCTION("CQuestSearchGui::Destroy");

	m_pFlashSearch->Unload();

	ITER_QUEST_INFO it = m_mapQuestInfo.begin();
	for( ; it != m_mapQuestInfo.end() ; ++it )
	{
		sQuestInfo* pQuestInfo = it->second;
		
		if(pQuestInfo->pFlash)
		{
			pQuestInfo->pFlash->Unload();
			NTL_DELETE(pQuestInfo->pFlash);
		}

		NTL_DELETE(pQuestInfo);
	}

	if( GetInfoWndManager()->GetRequestGui() == DIALOG_UNKNOWN &&
		GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_QUEST_SEARCH )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CQuestSearchGui::Update(RwReal fElapsed)
{
	m_pFlashSearch->Update(fElapsed);

	ITER_QUEST_INFO it = m_mapQuestInfo.begin();
	for( ; it != m_mapQuestInfo.end() ; ++it )
	{
		sQuestInfo* pQuestInfo = it->second;
		pQuestInfo->pFlash->Update(fElapsed);
	}
}

VOID CQuestSearchGui::SetArea(const CRectangle& rtWorldMap, RwReal fMapScale, const RwV2d& v2MapPos, RwInt32 iAvatarXPos_in_Map, RwInt32 iAvatarYPos_in_Map)
{
	m_iAvatarXPos	= iAvatarXPos_in_Map - rtWorldMap.left;
	m_iAvatarYPos	= iAvatarYPos_in_Map - rtWorldMap.top;

	ResetArea(rtWorldMap, fMapScale, v2MapPos);
}

VOID CQuestSearchGui::ResetArea(const CRectangle& rtWorldMap, RwReal fMapScale, const RwV2d& v2MapPos)
{
	CRectangle rtScreen;

	// 플래쉬 클리핑을 위한 영역 설정
	rtScreen.left		= rtWorldMap.left	+ dADJUST_AREA_LEFT;
	rtScreen.top		= rtWorldMap.top	+ dADJUST_AREA_TOP;
	rtScreen.right		= rtWorldMap.right	- dADJUST_AREA_RIGHT;
	rtScreen.bottom		= rtWorldMap.bottom	- dADJUST_AREA_BOTTOM;

	SetPosition(rtScreen);

	m_pFlashSearch->SetPosition(m_iAvatarXPos - m_pFlashSearch->GetWidth()/2 - dADJUST_AREA_LEFT,
								m_iAvatarYPos - m_pFlashSearch->GetHeight()/2 - dADJUST_AREA_TOP);

	// 퀘스트 마크 위치 보정
	CRectangle rtQuest;
	ITER_QUEST_INFO it = m_mapQuestInfo.begin();
	for( ; it != m_mapQuestInfo.end() ; ++it )
	{
		sQuestInfo* pQuestInfo = it->second;

		if( m_mapQuestInfo.begin() == it )
			rtQuest = pQuestInfo->pFlash->GetFrameResolution();

		if(pQuestInfo->pFlash)
		{
			CRectangle rtMark;
			RwInt32 iMarkX = (RwInt32)((v2MapPos.x - pQuestInfo->fXPos) * fMapScale) - dADJUST_AREA_LEFT;
			RwInt32 iMarkY = (RwInt32)((v2MapPos.y - pQuestInfo->fZPos) * fMapScale) - dADJUST_AREA_TOP;

			rtMark.SetCenterRect(iMarkX, iMarkY, rtQuest.GetWidth(), rtQuest.GetHeight());
			pQuestInfo->pFlash->SetPosition(rtMark);
		}
	}
}

RwBool CQuestSearchGui::SetQuestInfo()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CDboTSCQAgency* pTSCQAgency = pAvatar->GetQuestAgency();
	if ( !pTSCQAgency )
		return FALSE;

	RwV3d v3AvatarPos = pAvatar->GetPosition();
	RwV2d v2AvatarPos;

	v2AvatarPos.x = v3AvatarPos.x;
	v2AvatarPos.y = v3AvatarPos.z;


	// 아바타와 일정 반경에 있는 퀘스트를 찾는다.

	SGET_QUEST_INFO* pSGETQuestInfo = NULL;

	// avooo's commnet : 이 부분을 테이블로 빼자. 월드별로 검색 범위를 다르게...
	if( 5 == Logic_GetActiveWorldTableId() )
		pSGETQuestInfo = pTSCQAgency->GetQuestInfoList_ForQuestSerarch(Logic_GetActiveWorldTableId(), v3AvatarPos.x, v3AvatarPos.z, dSEARCH_RADIUS/3.f);
	else
		pSGETQuestInfo = pTSCQAgency->GetQuestInfoList_ForQuestSerarch(Logic_GetActiveWorldTableId(), v3AvatarPos.x, v3AvatarPos.z, dSEARCH_RADIUS);

	if( !pSGETQuestInfo )
	{
		DBO_FAIL("Not exist quest info, world table ID : " << Logic_GetActiveWorldTableId() << "avatar pos x : " << v3AvatarPos.x << " pos y : " << v3AvatarPos.z);
		return FALSE;
	}
	
	CQuestTextDataTable*	pQuestTable		= API_GetTableContainer()->GetQuestTextDataTable();
	CNPCTable*				pNPCTable		= API_GetTableContainer()->GetNpcTable();
	CTextTable*				pNPCTextTable	= API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();

	// 아바타와 같은 존에 존재하는 퀘스트를 찾기 위해
	CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	if( !pWorldEntity )
	{
		DBO_FAIL("Invalid World entity pointer");
		return FALSE;
	}
	
	TBLIDX idxAvatarWorldZone = pWorldEntity->GetMapNameCode(v3AvatarPos);
	if( idxAvatarWorldZone == 0xffffffff )
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			WCHAR awcBuffer[256] = L"";
			swprintf_s(awcBuffer, 256, L"Not exist map code of avatar x : %f,	y : %f", v3AvatarPos.x, v3AvatarPos.z);
			GetAlarmManager()->AlarmMessage(awcBuffer);
		}		
		return FALSE;
	}

	idxAvatarWorldZone /= 1000;
	

	for( RwInt32 i = 0 ; i < (RwInt32)pSGETQuestInfo->QuestInfoList.size() ; ++i )
	{
		sNPC_TBLDAT* pNPC_TBLDAT = reinterpret_cast<sNPC_TBLDAT*>( pNPCTable->FindData(pSGETQuestInfo->QuestInfoList[i].uiNPCTblIdx) );
		if( !pNPC_TBLDAT )
		{
			DBO_FAIL("Not exist NPC table of index " << pSGETQuestInfo->QuestInfoList[i].uiNPCTblIdx);
			return FALSE;
		}

		ITER_QUEST_INFO it = m_mapQuestInfo.find(pNPC_TBLDAT->Name);
		if( it == m_mapQuestInfo.end() )
		{
			// quest search에 아직 저장되지 않은 퀘스트를 가지고 있는 NPC이다

			RwV2d v2QuestPos;

			v2QuestPos.x = pSGETQuestInfo->QuestInfoList[i].fXPos;
			v2QuestPos.y = pSGETQuestInfo->QuestInfoList[i].fZPos;


			// 아바타와 같은 존에 존재하는 퀘스트를 찾기 위해
			if( FALSE == IsSameZoneWithAvatar(idxAvatarWorldZone, v2QuestPos.x, v2QuestPos.y) )
				continue;


			sQuestInfo* pQuestInfo = NTL_NEW sQuestInfo();
            m_mapQuestInfo[ pNPC_TBLDAT->Name ] = pQuestInfo;

			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTable->FindData(pSGETQuestInfo->QuestInfoList[i].dwQuestTitle) );
			if( !pQUEST_TEXT_DATA_TBLDAT )
			{
				WCHAR awcBuffer[256];
				swprintf_s(awcBuffer, 256, L"CQuestSearchGui::SetQuestInfo, Not exist Quest text data table of index : %u", pSGETQuestInfo->QuestInfoList[i].dwQuestTitle);
				pQuestInfo->listQuestTitle.push_back(awcBuffer);
			}
			else
			{
				pQuestInfo->listQuestTitle.push_back( gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size()) );
			}			

			pQuestInfo->wstrNPCName	= pNPCTextTable->GetText(pNPC_TBLDAT->Name);			
			pQuestInfo->fXPos		= pSGETQuestInfo->QuestInfoList[i].fXPos;
			pQuestInfo->fZPos		= pSGETQuestInfo->QuestInfoList[i].fZPos;
			pQuestInfo->pFlash		= NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
			pQuestInfo->pFlash->Enable(false);
			pQuestInfo->pFlash->Load(dQUEST_MARK);
			pQuestInfo->pFlash->Show(false);			
		}
		else
		{
			// quest search에 저장된 퀘스트를 가지고 있는 NPC이다			

			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTable->FindData(pSGETQuestInfo->QuestInfoList[i].dwQuestTitle) );
			if( !pQUEST_TEXT_DATA_TBLDAT )
			{
				DBO_FAIL("Not exist Quest text data table of index " << pSGETQuestInfo->QuestInfoList[i].dwQuestTitle);
				continue;
			}

			sQuestInfo* pQuestInfo = it->second;

			pQuestInfo->listQuestTitle.push_back( gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size()) );
		}
	}


	if( m_mapQuestInfo.size() == 0 )
		GetAlarmManager()->AlarmMessage("DST_SCOUTER_QUEST_SEARCH_NOT_QUEST");

	// 퀘스트 이벤트
	CNtlSLEventGenerator::TSSearchQuest();

	return TRUE;
}

RwBool CQuestSearchGui::IsSameZoneWithAvatar(RwUInt32 uiAvatarMapCode, RwReal fXPos, RwReal fZPos)
{
	RwV3d v3Pos;
	CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	if( !pWorldEntity )
	{
		DBO_FAIL("Invalid World entity pointer");
		return FALSE;
	}
	

	v3Pos.x = fXPos;
	v3Pos.y = 0.f;
	v3Pos.z = fZPos;

	TBLIDX idxAreaInfoIndex = pWorldEntity->GetMapNameCode(v3Pos);

	if( idxAreaInfoIndex == 0xffffffff )
		return FALSE;

	idxAreaInfoIndex /= 1000;

	if( idxAreaInfoIndex == uiAvatarMapCode )
		return TRUE;

	return FALSE;
}

VOID CQuestSearchGui::OnMovieEnd(gui::CComponent* pComponent)
{
	ITER_QUEST_INFO it = m_mapQuestInfo.begin();
	for( ; it != m_mapQuestInfo.end() ; ++it )
	{
		sQuestInfo* pQuestInfo = it->second;
		pQuestInfo->pFlash->PlayMovie(TRUE);
		pQuestInfo->pFlash->Show(true);
	}
}

VOID CQuestSearchGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	QUEST_SEARCH_LIST listMarkInfo;	

	CRectangle rtRect, rtInfo;
	ITER_QUEST_INFO it = m_mapQuestInfo.begin();
	for( ; it != m_mapQuestInfo.end() ; ++it )
	{
		sQuestInfo* pQuestInfo = it->second;

		if( FALSE == pQuestInfo->pFlash->IsVisible() )
			continue;

		rtRect = pQuestInfo->pFlash->GetPosition();
		if( rtRect.PtInRect(nX, nY) )
		{
			sQUEST_SEARCH_INFO info;			

			rtInfo = rtRect;

			info.pwstrNPCName		= &(pQuestInfo->wstrNPCName);
			info.pListQuestTitle	= &(pQuestInfo->listQuestTitle);

			listMarkInfo.push_back(info);
		}
	}

	if( listMarkInfo.size() > 0 )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_QUEST_SEARCH,
			rtScreen.left + rtInfo.left, rtScreen.top + rtInfo.top, (VOID*)&listMarkInfo, DIALOG_UNKNOWN );
	}
	else
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CQuestSearchGui::OnMouseLeave(gui::CComponent* pComponent)
{	
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}