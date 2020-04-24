#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ExTreeCtrl.h"


// CVehicleViewPane 폼 뷰입니다.

class CVehicleViewPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CVehicleViewPane)

protected:
	CVehicleViewPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CVehicleViewPane();

public:
	enum { IDD = IDD_VEHICLEVIEWPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    static CVehicleViewPane* GetInstance() {return m_pInstance;};

    virtual void OnInitialUpdate();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnTvnSelchangedTreeVehicle(NMHDR *pNMHDR, LRESULT *pResult);
    
    void    SetVehicle(CMTCharacter* pVehicle);
    void    SetVehicleAnimation(RwUInt32 uiAnimID);

protected:
    void    UpdateTree();                               ///< 트리의 내용을 업데이트한다.
    void    AttachVehicle(HTREEITEM hItem);             ///< 비클에 캐릭터를 탑승시킨다.

public:
    CImageList*   m_pImageList;                        ///< 트리에 사용할 이미지 리스트
    CExTreeCtrl   m_treeVehicle;    
    
protected:
    static CVehicleViewPane* m_pInstance;

    CMTCharacter*   m_pCharacter;                       ///< 현재 Attach되어 있는 캐릭터
    CMTCharacter*   m_pVehicle;                         ///< 현재 Attach하는 Vehicle
    
};


