#ifndef _NTL_NAVI_PE_IMPORTER_H_
#define _NTL_NAVI_PE_IMPORTER_H_


#include "NtlNaviLoadingQueue.h"


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_ODProp
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPE_ODProp : public CNtlNaviLoadingEntity
{
public:
	CNtlNaviCS						m_csPropInfo;

	// Input
	unsigned int					m_uiFieldID;
	CNtlNaviDataMng*				m_pNaviDataMng;
	std::string						m_strPath;

	// Output
	CNtlNaviPropOutDoorInfo*		m_pPropInfo;

public:
	CNtlNaviPE_ODProp( unsigned int uiFieldID, CNtlNaviDataMng* pNaviDataMng, std::string& strPath ) { m_uiFieldID = uiFieldID; m_pNaviDataMng = pNaviDataMng; m_strPath = strPath; m_pPropInfo = NULL; }
	virtual ~CNtlNaviPE_ODProp( void ) { return; }

public:
	virtual void					RunMultiThread( void );
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_IDProp
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPE_IDProp : public CNtlNaviLoadingEntity
{
public:
	CNtlNaviCS						m_csPropInfo;

	// Input
	unsigned int					m_uiBlockID;
	CNtlNaviDataMng*				m_pNaviDataMng;
	std::string						m_strPath;

	// Output
	CNtlNaviPropInDoorInfo*			m_pPropInfo;

public:
	CNtlNaviPE_IDProp( unsigned int uiBlockID, CNtlNaviDataMng* pNaviDataMng, std::string& strPath ) { m_uiBlockID = uiBlockID; m_pNaviDataMng = pNaviDataMng; m_strPath = strPath; m_pPropInfo = NULL; }
	virtual ~CNtlNaviPE_IDProp( void ) { return; }

public:
	virtual void					RunMultiThread( void );
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_ODGroup
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPE_ODGroup : public CNtlNaviLoadingEntity
{
public:
	struct sAGENT_DATA
	{
		float						fRadius;
		iShape*						pShape;
	};

public:
	CNtlNaviCS						m_csODGroup;

	// Input
	unsigned int					m_uiGroupID;
	std::string						m_strImportPath;
	std::vector< sAGENT_DATA >		m_defAgentList;

	// Output
	iMesh*							m_pMesh;

public:
	CNtlNaviPE_ODGroup( unsigned int uiGroupID, const std::string& strImportPath  ) { m_uiGroupID = uiGroupID; m_strImportPath = strImportPath; m_pMesh = NULL; }
	virtual ~CNtlNaviPE_ODGroup( void ) { return; }

	void							AttachAgent( float fRadius, iShape* pShape );

public:
	virtual void					RunMultiThread( void );
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_IDGroup
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPE_IDGroup : public CNtlNaviLoadingEntity
{
public:
	struct sAGENT_DATA
	{
		float						fRadius;
		iShape*						pShape;
	};

public:
	CNtlNaviCS						m_csIDGroup;

	// Input
	unsigned int					m_uiGroupID;
	std::string						m_strImportPath;
	std::vector< sAGENT_DATA >		m_defAgentList;

	// Output
	iMesh*							m_pMesh;

public:
	CNtlNaviPE_IDGroup( unsigned int uiGroupID, const std::string& strImportPath  ) { m_uiGroupID = uiGroupID; m_strImportPath = strImportPath; m_pMesh = NULL; }
	virtual ~CNtlNaviPE_IDGroup( void ) { return; }

	void							AttachAgent( float fRadius, iShape* pShape );

public:
	virtual void					RunMultiThread( void );
};


#endif