#pragma once

class CNtlDTProp;
struct SSplineProp;

// CPropertyPane

class CPropertyPane : public CWnd
{
	DECLARE_DYNAMIC(CPropertyPane)

private:

	void			SetPropToGdi(void);
	void			SetGdiToProp(void);

	void			OnItemValueChangedGridNotify(WPARAM wParam, LPARAM lParam);
	void			OnEditChangedGridNotify(WPARAM wParam, LPARAM lParam);

public:

	CPropertyPane();
	virtual ~CPropertyPane();

	CXTPPropertyGrid			m_PropGrid;
	CXTPPropertyGridItem		*m_pSplineControlItem;
	CXTPPropertyGridItem		*m_pSplineCurveItem;
	CXTPPropertyGridItem		*m_pSplineUtilItem;
	
	// spline property gdi
	CXTPPropertyGridItemEnum	*m_pSplineShape;
	CXTPPropertyGridItemDouble	*m_pSplineTime;
	CXTPPropertyGridItemNumber	*m_pSplineCtrlCnt;
	CXTPPropertyGridItemNumber	*m_pSplineCurveDiv;
	CXTPPropertyGridItemDouble	*m_pSplineCurveCircleRadius;
	CXTPPropertyGridItemNumber	*m_pSplineCurveCircleCnt;
	CXTPPropertyGridItemDouble	*m_pSplineCurveCircleHeight;

	// spline porperty util
	CXTPPropertyGridItemBool	*m_pSplineUtilUseWorldPos;
	CXTPPropertyGridItemBool	*m_pSplineUtilUseUniformLerp;
	CXTPPropertyGridItemDouble	*m_pSplineUtilUniformSpeed;

	CNtlDTProp		*m_pDTProp;
	SSplineProp		*m_pSplineProp;

	void			ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData);
	void			DeActiveSplineProp(void);
	void			DeleteSplineProp(CNtlDTProp *pDTProp);
	void			RefreshSplineProp(void);
	void			RebuildProp();

	void*			GetActivePropData();	

	void			SetStandardPosition(RwV3d vPos);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);
};


