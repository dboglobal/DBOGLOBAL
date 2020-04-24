#pragma once


// CActorPane

class CActorPane : public CWnd
{
	DECLARE_DYNAMIC(CActorPane)

public:

	CActorPane();
	virtual ~CActorPane();

	UINT	m_uiSerialId;
	FLOAT	m_fPosX;
	FLOAT	m_fPosY;
	FLOAT	m_fPosZ;

	CXTPPropertyGrid			m_PropGrid;
	CXTPPropertyGridItem		*m_pActorHandleItem;
	CXTPPropertyGridItem		*m_pActorPositionItem;

	CXTPPropertyGridItemNumber	*m_pHandle;
	CXTPPropertyGridItemDouble	*m_pPosX;
	CXTPPropertyGridItemDouble	*m_pPosY;
	CXTPPropertyGridItemDouble	*m_pPosZ;

	void SetActiveActor(RwUInt32 uiSerialId);
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


