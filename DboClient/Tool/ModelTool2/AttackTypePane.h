#pragma once
#include "afxwin.h"



// CAttackTypePane form view

class CAttackTypePane : public CFormView
{
	DECLARE_DYNCREATE(CAttackTypePane)

protected:
	CAttackTypePane();           // protected constructor used by dynamic creation
	virtual ~CAttackTypePane();

public:
	enum { IDD = IDD_ATTACKTYPEPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CAttackTypePane* GetInstance() {return m_pInstance;};

    void SetAnimEventData(STypeAnimData* pAnimData);

protected:
    void SetEnable(BOOL bEnable);
    void ChangeDamage(int nIndex);                          ///< 데미지르 변경한다.

protected:
    static CAttackTypePane* m_pInstance;
    CEdit m_edDamage[MAX_HIT_EVENT_COUNT];
    CButton m_raPhysical;
    CButton m_raEnergy;

    SEventAnimHit*          m_vEventAnimHit[MAX_HIT_EVENT_COUNT];        ///< Hit Event     

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
    
public:
    afx_msg void OnBnClickedRaPhysical();
    afx_msg void OnBnClickedRaEnergy();
    afx_msg void OnEnChangeEdDamage00();
    afx_msg void OnEnChangeEdDamage01();
    afx_msg void OnEnChangeEdDamage02();
    afx_msg void OnEnChangeEdDamage03();
    afx_msg void OnEnChangeEdDamage04();
    afx_msg void OnEnChangeEdDamage05();
    afx_msg void OnEnChangeEdDamage06();
    afx_msg void OnEnChangeEdDamage07();
    afx_msg void OnEnChangeEdDamage08();
    afx_msg void OnEnChangeEdDamage09();
    afx_msg void OnEnChangeEdDamage10();
    afx_msg void OnEnChangeEdDamage11();
    afx_msg void OnEnChangeEdDamage12();
    afx_msg void OnEnChangeEdDamage13();
    afx_msg void OnEnChangeEdDamage14();
    afx_msg void OnEnChangeEdDamage15();
};


