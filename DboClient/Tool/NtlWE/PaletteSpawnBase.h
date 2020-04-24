#pragma once


#include "SpawnMacro.h"
#include "Spawn.h"


class CPaletteSpawnBase : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSpawnBase)

public:
	CPaletteSpawnBase(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSpawnBase();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_CurSpawnPos;
	CString m_CurSpawnDir;
	CString m_CurSpawnMacro;
	CString m_CurSpawnInfo;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

public:
	VOID Refresh(RwV3d& _SpawnPos, RwV3d& _SpawnDir, CSpawnMacro& _SpawnMacro, CSpawn* pSpawn);
};
