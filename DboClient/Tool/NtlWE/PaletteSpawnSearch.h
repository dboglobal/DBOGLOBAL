#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Spawn.h"

class CPaletteSpawnSearch : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSpawnSearch)

	enum ESPAWNLIST
	{
		ESPAWNLIST_FIELD_NUMBER = 0,
		ESPAWNLIST_INDEX,
		ESPAWNLIST_NAME,
		ESPAWNLIST_LEVEL,
		ESPAWNLIST_POSITION,
		ESPAWNLIST_NEST_COUNT,
		ESPAWNLIST_COOL_TIME,
		ESPAWNLIST_MOVE_TYPE,
		ESPAWNLIST_WANDER_RANGE,
		ESPAWNLIST_MOVE_RANGE,
		ESPAWNLIST_DELAY_TIME,
		ESPAWNLIST_PATH_INDEX,
		ESPAWNLIST_SPAWN_GROUP_INDEX,
		ESPAWNLIST_PLAY_SCRIPT_INDEX,
		ESPAWNLIST_PLAY_SCRIPT_SCENE_INDEX,
		ESPAWNLIST_AI_SCRIPT_INDEX,
		ESPAWNLIST_AI_SCRIPT_SCENE_INDEX,

		ESPAWNLIST_NUM,
	};

public:
	CPaletteSpawnSearch(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSpawnSearch();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_SEARCH };

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual BOOL	OnInitDialog();
	afx_msg void	OnLvnItemchangedPathlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnLvnItemchangedSpawnList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnCbnSelchangeListOption();
	RwBool			RefreshSpawnList();
	VOID			RefreshSpawnListItem(RwInt32 iItem, CSpawn* pSpawn);
	VOID			RefreshSpawnCount();
	VOID			ReFreshModification();
	VOID			InsertNode(RwInt32 _FieldNum, RwInt32 _Index, string& Name, RwInt32 _SpawnLvl, RwV3d& _Pos, void* pData = NULL);

public:
	RwBool		m_AscendingSortFlags[ESPAWNLIST_NUM];
	CComboBox	m_SpawnListOptionCombo;
	CListCtrl	m_SpawnList;
	CString		m_StrMinLvl;
	CString		m_StrMaxLvl;
	CString		m_StrSpawnIndex;
	CString		m_StrSpawnCountInfo;

public:
	afx_msg void OnBnClickedCreate();
public:
	afx_msg void OnBnClickedDelete();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnLvnColumnclickSpawnList(NMHDR *pNMHDR, LRESULT *pResult);
	RwBool	OnSortData(RwInt32 _Index);
public:
	afx_msg void OnBnClickedPortal();
public:
	CString m_strFieldNumber;
public:
	CString m_strIndex;
public:
	CString m_strNestCount;
public:
	CString m_strCoolTime;
public:
	CString m_strWanderRange;
public:
	CString m_strMoveRange;
public:
	CString m_strDelayTime;
public:
	CString m_strPathIndex;
public:
	CString m_strSpawnGroupIdx;
public:
	CString m_strPlayScriptIdx;
public:
	CString m_strPlayScriptSceneIdx;
public:
	CComboBox m_cbMoveType;
public:
	afx_msg void OnBnClickedIndexOk();
public:
	afx_msg void OnBnClickedNestCountOk();
public:
	afx_msg void OnBnClickedCoolTimeOk();
public:
	afx_msg void OnBnClickedMoveTypeOk();
public:
	afx_msg void OnBnClickedWanderRangeOk();
public:
	afx_msg void OnBnClickedMoveRangeOk();
public:
	afx_msg void OnBnClickedDelayTimeOk();
public:
	afx_msg void OnBnClickedPathIndexOk();
public:
	afx_msg void OnBnClickedSpawnGroupIdxOk();
public:
	afx_msg void OnBnClickedPlayscriptIdxOk();
public:
	afx_msg void OnBnClickedPlayscriptSceneIdxOk();
public:
	afx_msg void OnBnClickedAllOk();
	afx_msg void OnBnClickedAiscriptIdxOk();
	afx_msg void OnBnClickedAiscriptSceneIdxOk();
	CString m_strAIScriptIdx;
	CString m_strAIScriptSceneIdx;
	afx_msg void OnBnClickedExportSpawnList();
	afx_msg void OnBnClickedExportRuleOut();
};
