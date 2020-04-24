#pragma once
#include "afxwin.h"

#include "CustomItems.h"
#include "NtlScriptEncrypter.h"

enum EItemID
{
    ID_FILE_LOAD,
    ID_OP_IP,
    ID_OP_PORT,
    ID_LOCAL_DSP,
    ID_LOCAL_SYNC_DSP,
    ID_BUGTRAP_IP,
    ID_BUGTRAP_PORT,
};


/**
 * \ingroup MrPoPo
 * \brief   Config 파일 설정및 암호화 뷰 클래스
 *
 * \date 2008-07-31
 * \author agebreak
 */
class CMPPConfigSetting : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMPPConfigSetting)

protected:
	CMPPConfigSetting();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPConfigSetting();

public:
	enum { IDD = IDD_MPPCONFIGSETTING };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


public:
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedBtnSavexml();
    afx_msg void OnBnClickedBtnSaveenc();

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
        afx_msg void OnBnClickedBtnLuaCompile();

    void    UpdateConfigData();                         ///< 컨피그 데이터를 업데이트합니다.
    void    LoadLuaFiles();

protected:
    CButton m_btnSaveXML;
    CButton m_btnSaveEnc;
    CXTBrowseEdit   m_edLuaDir;
    CXTBrowseEdit   m_edOutDir;

    CXTPPropertyGrid        m_property;
    CXTPPropertyGridItem*   m_pCatConfig;
    CCustomItemFileBox*     m_pFileLoad;
    CXTPPropertyGridItem*   m_pOpIp;
    CXTPPropertyGridItem*   m_pOpPort;
    CCustomItemFileBox*     m_pLocalDsp;
    CCustomItemFileBox*     m_pLocalSyncDsp;
    CXTPPropertyGridItem*   m_pBugTrapIp;
    CXTPPropertyGridItem*   m_pBugTrapPort;

    SConfigData             m_configData;               ///< 컨피그 정보 데이터
public:
    afx_msg void OnEnChangeEdLua();
};


