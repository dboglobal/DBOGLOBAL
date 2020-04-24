#pragma once
#include "afxcmn.h"


// CConvertingDlg dialog

class CConvertingDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertingDlg)

public:
	enum EConverState
	{
		LOAD_CHARACTER,
		LOAD_ITEM,
		LOAD_OBJECT,
		SAVE_DATA,		
		SAVE_COMPLETE,
		SAVE_FAIL,
        UPDATE_CHAR,
        UPDATE_OBJ,
        UPDATE_ITEM,
	};

    enum EConvertType
    {
        CONVERT_BIN,
        CONVERT_ENC_BIN,        
        CONVERT_UPDATE_PROP,
    };

public:
	CConvertingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConvertingDlg();

// Dialog Data
	enum { IDD = IDD_CONVERTDLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

protected:
	void SetState(EConverState eState);
	static DWORD WINAPI ConvertBinary(LPVOID pParam);    
    static DWORD WINAPI PropDataResave(LPVOID pParam);                  ///< 프로퍼티만 업데이트하여 스크립트에 저장한다.

    RwBool  UpdateAnimData(const char* szName, CNtlTypeAnimTable* pAnimTable);              ///< 애니메이션 정보를 Update한다. (duration time)
    void    CheckAnimEventTime(const char* szName, STypeAnimData* pAnimData);               ///< 애니메이션 플레이 시간보다 더 늦은 이벤트가 있는지 체크한다. 

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    
    RwBool LoadPropertyAll();

public:
    CEdit			 m_msgEdit;
    CProgressCtrl	 m_progress;
    CButton			 m_okButton;
    static EConvertType m_eType;
public:
    CRichEditCtrl     m_reLog;
};
