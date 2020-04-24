#pragma once
#include "afxwin.h"
#include "NtlWorldFileMemAccessor.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CPathMeshFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CPathMeshFindDlg)

public:
	CPathMeshFindDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathMeshFindDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATH_MESH_FIND };

	enum ESEARCHLIST
	{
		ESEARCHLIST_NAME,
		ESEARCHLIST_POS_X,
		ESEARCHLIST_POS_Y,
		ESEARCHLIST_POS_Z,

		ESEARCHLIST_NUM,
	};

	typedef void (CPathMeshFindDlg::*_CBOBJECT)(char*, RwV3d*, RwV3d*, RwV3d*, void*);

protected:
	virtual void				DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL				PreTranslateMessage(MSG* pMsg);
	virtual BOOL				OnInitDialog();

protected:
	void						AddSearchList(char* pcName, RwV3d* pvPos);
	RwBool						OnSortData(RwInt32 _Index);
	RwBool						IsInString(const char* pSrc, const char* pCmp);

	void						GetPathEngineMeshName(char* pName, std::vector<std::string>& vecNaviMesh, std::vector<std::string>& vecObsMesh);

	void						GetWorldObject(RwInt32 iFieldIdx, _CBOBJECT cbObject, void* pData);
	void						SearchRot(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData);
	void						SearchArea(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData);
	void						ObsException(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData);

	void						SetClmupTransform(RpClump* pRpClump, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos);
	void						CalcClmupBoundingSphere(RpClump* pRpClump, RwSphere* pRwSphere);
	void						BuildClmupConvexHull(RpClump* pRpClump, std::vector<RwV3d>& vecResult);

	void						GenObsException(std::map<RwInt32, RwReal*>& mapPE, std::vector<RwV3d>& vecConvexHull);

	void						SetPathEngineTileProperty(RwV3d& vPos, RwReal fHeight, std::map<RwInt32, RwReal*>& mapPE);
	void						GenPathEngineTilePropertyFromImage(RwInt32 iSectorIdx, RwImage* pRwImage, std::map<RwInt32, RwReal*>& mapPE);

	RwBool						IsInConvexHull(RwV3d& vPos, std::vector<RwV3d>& vecConvexHull);
	
public:
	afx_msg void				OnBnClickedBtnPortal();
	afx_msg void				OnLvnColumnClickList(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	//RwTexture*					m_pRwTexture;
	RwImage*					m_pRwImage;
	RwBool						m_AscendingSortFlags[ESEARCHLIST_NUM];
	std::vector<std::string>	m_vecExceptionList;

protected:
	CListCtrl					m_listSearch;

	CButton						m_BtnNaviMesh;
	CButton						m_BtnObsMesh;

public:
	
	afx_msg void OnBnClickedBtnSearchRot();
	afx_msg void OnBnClickedBtnSearchArea();
	afx_msg void OnBnClickedBtnObsException();
	afx_msg void OnBnClickedBtnImport();
};


//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

class CPathMeshFindDlg : public CDialog, public CNtlWorldFileMemAccessor
{
	DECLARE_DYNAMIC(CPathMeshFindDlg)

public:
	CPathMeshFindDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathMeshFindDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATH_MESH_FIND };

	enum ESEARCHLIST
	{
		ESEARCHLIST_NAME,
		ESEARCHLIST_POS_X,
		ESEARCHLIST_POS_Y,
		ESEARCHLIST_POS_Z,

		ESEARCHLIST_NUM,
	};

	typedef void (CPathMeshFindDlg::*_CBOBJECT)(char*, RwV3d*, RwV3d*, RwV3d*, void*);

protected:
	virtual void				DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL				PreTranslateMessage(MSG* pMsg);
	virtual BOOL				OnInitDialog();

protected:
	void						AddSearchList(char* pcName, RwV3d* pvPos);
	RwBool						OnSortData(RwInt32 _Index);
	RwBool						IsInString(const char* pSrc, const char* pCmp);

	void						GetPathEngineMeshName(char* pName, std::vector<std::string>& vecNaviMesh, std::vector<std::string>& vecObsMesh);

	void						GetWorldObject(RwInt32 iFieldIdx, _CBOBJECT cbObject, void* pData);
	void						SearchRot(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData);
	void						SearchArea(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData);
	void						ObsException(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData);

	void						SetClmupTransform(RpClump* pRpClump, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos);
	void						CalcClmupBoundingSphere(RpClump* pRpClump, RwSphere* pRwSphere);
	void						BuildClmupConvexHull(RpClump* pRpClump, std::vector<RwV3d>& vecResult);

	void						GenObsException(std::map<RwInt32, RwReal*>& mapPE, std::vector<RwV3d>& vecConvexHull);

	void						SetPathEngineTileProperty(RwV3d& vPos, RwReal fHeight, std::map<RwInt32, RwReal*>& mapPE);
	void						GenPathEngineTilePropertyFromImage(RwInt32 iSectorIdx, RwImage* pRwImage, std::map<RwInt32, RwReal*>& mapPE);

	RwBool						IsInConvexHull(RwV3d& vPos, std::vector<RwV3d>& vecConvexHull);

public:
	afx_msg void				OnBnClickedBtnPortal();
	afx_msg void				OnLvnColumnClickList(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	//RwTexture*					m_pRwTexture;
	RwImage*					m_pRwImage;
	RwBool						m_AscendingSortFlags[ESEARCHLIST_NUM];
	std::vector<std::string>	m_vecExceptionList;

protected:
	CListCtrl					m_listSearch;

	CButton						m_BtnNaviMesh;
	CButton						m_BtnObsMesh;

public:

	afx_msg void OnBnClickedBtnSearchRot();
	afx_msg void OnBnClickedBtnSearchArea();
	afx_msg void OnBnClickedBtnObsException();
	afx_msg void OnBnClickedBtnImport();
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif