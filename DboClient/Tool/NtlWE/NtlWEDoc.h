// NtlWEDoc.h : CNtlWEDoc 클래스의 인터페이스
//

#include "FogPropDlg.h"
#include "SkyPropDlg.h"
#include "LightPropDlg.h"
#include "SLPropDlg.h"
//#include "BGMPropDlg.h"
#include "FieldSoundPropDlg.h"
#include "FieldColorPropDlg.h"
#include "WeatherDlg.h"
#include "FieldHeatHazeDlg.h"
#include "GamePropDlg.h"
#include "ntlworldcommon.h"

#pragma once

class CNtlPLEntity;

enum EFIELDPROP_MODE
{
	EFIELDPROP_MODE_NONE,
	EFIELDPROP_MODE_OUTDOOR,
	EFIELDPROP_MODE_INDOOR,
	EFIELDPROP_MODE_OBJECT,
};

class CNtlWEDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CNtlWEDoc();
	DECLARE_DYNCREATE(CNtlWEDoc)

// 특성
public:

// 작업
public:
	RwUInt32				m_uiFieldPropMode;

	sNTL_FIELD_PROP*		m_pNtlFieldProp;
	sNTL_BLOCK_PROP*		m_pNtlBlockProp;
	sNTL_FIELD_PROP*		m_pNtlFieldPropClipboard;
	sNtlWorldParam			m_NtlWorldParam;			// FileNewDlg
	sNTL_WORLD_SL			m_NtlWorldSL;
	sNTL_DOODADS_PROP_PARAM m_NtlDoodadsPropParam;
	CString					m_CurNestMemberCnt;
	CString					m_CurPathIndex;

	// grouping indices
	CString	m_CurSGroupIdx;
	CString	m_CurSPlayScriptIdx;
	CString	m_CurSPlayScriptSceneIdx;
	CString	m_CurSAIScriptIdx;
	CString	m_CurSAIScriptSceneIdx;

	RwInt32				m_FogColor[3];
	CFogPropDlg			m_FogPropDlg;
	CSkyPropDlg			m_SkyPropDlg;
	CLightPropDlg		m_LightPropDlg;
	CSLPropDlg			m_SLPropDlg;
	//CBGMPropDlg			m_BGMPropDlg;
	CFieldSoundPropDlg	m_FieldSoundProp;
	CFieldColorPropDlg	m_FieldColorPropDlg;
	CWeatherDlg			m_WeatherDlg;

	// 인도어
	CGamePropDlg		m_GamePropDlg;

	std::wstring		m_MapNameStr;

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
	void CreateWorldData();

// 구현
public:
	virtual ~CNtlWEDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSave();
	afx_msg void OnPortal();
	afx_msg void OnUpdatePortal(CCmdUI *pCmdUI);
	afx_msg void OnMerge();
	afx_msg void OnUpdateMerge(CCmdUI *pCmdUI);
	afx_msg void OnFileVerification();
	afx_msg void OnAttrFog();
	afx_msg void OnUpdateAttrFog(CCmdUI *pCmdUI);
	afx_msg void OnAttrSky();
	afx_msg void OnUpdateAttrSky(CCmdUI *pCmdUI);
	afx_msg void OnAttrLight();
	afx_msg void OnUpdateAttrLight(CCmdUI *pCmdUI);
	afx_msg void OnMenuSlopelighting();
	afx_msg void OnUpdateMenuSlopelighting(CCmdUI *pCmdUI);

	afx_msg void OnPropCreate();
	afx_msg void OnPropDelete();
	afx_msg void OnFieldBgm();
	void	SetSLData(RwV3d& Pos);
	VOID	SetSerialID(CNtlPLEntity* pNtlPLEntity);
	VOID	SetObjNameIdx(CNtlPLEntity* pNtlPLEntity);

	afx_msg void OnHeightfieldupdate();
	afx_msg void OnUpdateHeightfieldupdate(CCmdUI *pCmdUI);
	afx_msg void OnVisibilityDistance();
	afx_msg void OnCreateaminimap();
	afx_msg void OnFieldFieldname();
	afx_msg void OnInformation();
	afx_msg void OnFieldMonopower();
	afx_msg void OnUpdateFieldMonopower(CCmdUI *pCmdUI);
	afx_msg void OnCreateTransparentTiles();
	afx_msg void OnDeleteTransparentTiles();
//	afx_msg void OnCreatePvs();
//	afx_msg void OnDestroyPvs();
	afx_msg void OnMaterialPropUpdate();
	afx_msg void OnFieldcolor();
	afx_msg void OnFieldsearch();
	afx_msg void OnWeather();
	afx_msg void OnFieldheathaze();
	afx_msg void OnBlockPortal();
	afx_msg void OnSectorPortal();
	afx_msg void OnFieldPortal();
	afx_msg void OnBlockGameProperty();
	afx_msg void OnBlockSkyfloating();
	afx_msg void OnPropertyClipboardCopy();
	afx_msg void OnPropertyClipboardPaste();

	// FieldPropMode
	RwUInt32	GetFieldPropMode()							{ return m_uiFieldPropMode; }
	VOID		SetFieldPropMode(RwUInt32 uiFieldPropMode)	{ m_uiFieldPropMode = uiFieldPropMode; }
	RwBool		GetFieldProp(sNTL_FIELD_PROP& sNtlFieldProp);
	VOID		ApplyFieldProp(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID);
	afx_msg void OnTempProofsinvalidsectorlink();
};