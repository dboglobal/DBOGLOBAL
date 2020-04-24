#pragma once


#include "Shape.h"


class CTSDoc;
class CTSView;


//////////////////////////////////////////////////////////////////////////
//
//	CShape_GEntityCont
//
//////////////////////////////////////////////////////////////////////////


class CShape_GEntityCont : public CShape
{
	DECLARE_SERIAL( CShape_GEntityCont )

// Declarations
public:
	struct sATTR
	{
		CString							strName;
		CString							strValue;
	};

	struct sENTITY
	{
		CString							strType;
		CString							strData;
	};

	typedef std::map<std::string, CRuntimeClass*> mapdef_EntityList;

// Member variables
protected:
	int									m_nWidth;
	int									m_nHeight;

	// Container type
	int									m_nContTypeHeight;

	CString								m_strContType;

protected:
	CFont*								m_pContTypeFont;
	COLORREF							m_ContTypeColor;

	// Container id
	int									m_nContIdHeight;
	CString								m_strContId;
	CFont*								m_pContIdFont;
	COLORREF							m_ContIdColor;

	// Attr
	bool								m_bShowAttr;
	int									m_nAttrHeight;
	int									m_nNewAttrHeight;
	CArray< sATTR, const sATTR& >		m_AttrList;
	CFont*								m_pAttrFont;
	COLORREF							m_AttrColor;

	// Entity
	bool								m_bShowEntity;
	int									m_nEntityHeight;
	int									m_nNewEntityHeight;
	CArray< sENTITY, const sENTITY& >	m_EntityList;
	CFont*								m_pEntityFont;
	COLORREF							m_EntityColor;

	// Attr와 Entity의 분류자
	int									m_nSeperatorNameHeight;
	CFont*								m_pSeperatorNameFont;
	COLORREF							m_SeperatorNameColor;

	// 윤곽선
	COLORREF							m_FillColor;
	COLORREF							m_LineColor;

	// 높이값 계산 공간
	int									m_aHeightList[128];

// Constructions and Destructions
protected:
	CShape_GEntityCont( void );

public:
	CShape_GEntityCont( const CPoint &point, CString strType );
	virtual ~CShape_GEntityCont( void );

// Methods
public:
	// Shapes
	virtual CRect						GetBoundBox( void ) const;

	virtual int							GetHandlesCount( void ) const { return 4; }
	virtual CPoint						GetHandlePoint( HSHAPE handle ) const;

	const CString&						GetContType( void ) const;

	virtual bool						Intersects( const CRect &rect ) const;

	virtual void						Activate( const CPoint &point );

	virtual void						Drag( const CPoint &point );

	virtual void						Render( CDC* pDC ) const;
	virtual void						RenderTracker( CDC* pDC ) const;

	virtual void						Serialize( CArchive &ar );

	// Container id
	NTL_TS_TC_ID						GetContID( void ) const;
	virtual void						EditContId( void );

	// Container Attr
	virtual const CArray<sATTR, const sATTR&>& GetAttrList( void ) const { return m_AttrList; }
	virtual bool						HasAttr( const CString& strName );
	virtual CString						GetAttr( const CString& strName );
	virtual CString						GetAttrString( void );
	virtual void						SetAttr( int nIndex, const sATTR& sATTR );
	virtual void						SetAttr( const CString& strName, const CString& strValue );
	virtual void						SetAttrString( const CString& strString );
	virtual void						EditAttr( int nIndex );
	virtual void						AddAttr( CString strName, const CString& strValue, bool bAutoEdit = false, int nIndex = -1 );
	virtual void						RemoveAttr( int nIndex );
	virtual void						RemoveAttr( CString strName );
	virtual void						GetProperty( CNtlTSScrProperty& clProperty ) { return; }

	// Container entity
	virtual CArray<sENTITY, const sENTITY&>* GetEntityListPtr( void ) { return &m_EntityList; }
	virtual const CArray<sENTITY, const sENTITY&>& GetEntityList( void ) const { return m_EntityList; }
	virtual void						SetEntity( int nIndex, const sENTITY& sEntity );
	virtual void						EditEntity( int nIndex );
	virtual void						AddEntity( CString strType, const CString& strData, bool bAutoEdit = false, int nIndex = -1 );
	virtual void						RemoveEntity( int nIndex );
	void								GetEntityProperty( int nIndex, CNtlTSScrProperty& clProperty );

// Implementations
protected:
	// Shapes
	virtual void						SetupTheme( void );

	virtual CSize						GetMinSize( void ) const;
	virtual CSize						GetMaxSize( void ) const;
	virtual CSize						GetIdealSize( void ) const;
	virtual void						AdjustSize( void );

	virtual void						UpdateHeightInfo( void );

	// Container type
	virtual CRect						GetContTypeRect( void ) const;

	// Container id
	virtual CFont*						GetContIdFont( void ) const;
	virtual CRect						GetContIdRect( void ) const;

	// Container Attr
	virtual CFont*						GetAttrFont( int nIndex ) const;
	virtual int							GetAttrCount( void ) const { return (int)m_AttrList.GetSize(); }
	virtual int							GetAttrAt( const CPoint& point ) const;
	virtual CRect						GetAttrSeperatorNameRect( void ) const;
	virtual CRect						GetAttrRect( int nIndex ) const;
	virtual CRect						GetNewAttrRect( void ) const;

	// Container entity
	virtual CFont*						GetEntityFont( int nIndex ) const;
	virtual int							GetEntityCount( void ) const { return (int)m_EntityList.GetSize(); }
	virtual int							GetEntityAt( const CPoint& point ) const;
	virtual CRect						GetEntitySeperatorNameRect( void ) const;
	virtual CRect						GetEntityRect( int nIndex ) const;
	virtual CRect						GetNewEntityRect( void ) const;

	// Edit
	virtual void						ShowContAttrEditBox( void ) { return; }
	virtual void						ShowEntityAttrEditBox( int nIdx );
	virtual void						ShowEntityAttrAddBox( void ) { return; }
};


inline const CString& CShape_GEntityCont::GetContType( void ) const
{
	return m_strContType;
}

inline NTL_TS_TC_ID CShape_GEntityCont::GetContID( void ) const
{
	return m_strContId.IsEmpty() ? NTL_TS_T_ID_INVALID : atoi( m_strContId.GetString() );
}


template<> void AFXAPI SerializeElements<CShape_GEntityCont::sATTR>( CArchive &ar, CShape_GEntityCont::sATTR* pData, INT_PTR nCount );
template<> void AFXAPI SerializeElements<CShape_GEntityCont::sENTITY>( CArchive &ar, CShape_GEntityCont::sENTITY* pData, INT_PTR nCount );
