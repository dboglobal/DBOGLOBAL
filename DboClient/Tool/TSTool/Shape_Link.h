#pragma once


#include "Shape.h"


class CTSDoc;
class CTSView;


//////////////////////////////////////////////////////////////////////////
//
//	CShape_Link
//
//////////////////////////////////////////////////////////////////////////


class CShape_Link : public CShape
{
	DECLARE_SERIAL( CShape_Link )

// Declarations
public:
	enum { NEW_NODE = 1024 };

	enum eBRANCH_TYPE
	{
		eBRANCH_TYPE_DEFAULT,
		eBRANCH_TYPE_YES,
		eBRANCH_TYPE_NO,
		eBRANCH_TYPE_ERROR,
		eBRANCH_TYPE_LOGIN
	};

	class CAnchor
	{
	// Member variables
	protected:
		CPoint							m_point;

	public:
		CShape*							pShape;
		double							dRx;
		double							dRy;

	// Methods
	public:
		const CPoint&					GetPoint( void ) const { return m_point; }
		void							MovePoint( const CPoint &point );
		void							UpdatePoint( void );
	};

// Member variables
protected:
	// Temp
	CPoint								m_ptTempEnd;

	// 생성 여부
	bool								m_bCreated;

	// Anchor
	CAnchor								m_SrcAnchor;
	CAnchor								m_DestAnchor;
	CArray<CPoint, const CPoint&>		m_NodeList;
	CArray<int, int>					m_SegTypeList;

	// Branch type
	eBRANCH_TYPE						m_eBranchType;

	// BranchName
	bool								m_bShowBranchName;
	CString								m_strBranchName;
	CSize								m_BranchNameOffset;
	CFont*								m_pBranchNameFont;

	// Shape
	int									m_nPenStyle;
	COLORREF							m_LineColor;

// Constructions and Destructions
protected:
	CShape_Link( void )					{ m_eBranchType = eBRANCH_TYPE_DEFAULT; SetupTheme(); }

public:
	CShape_Link( const CPoint &point, CShape* pShape );
	virtual ~CShape_Link( void )		{ return; }

// Methods
public:
	const CAnchor&						GetSrcAnchor( void ) const { return m_SrcAnchor; }
	const CAnchor&						GetDestAnchor( void ) const { return m_DestAnchor; }

	virtual int							GetHandlesCount( void ) const;
	virtual CPoint						GetHandlePoint( HSHAPE handle ) const;

	bool								IsCreated( void ) const { return m_bCreated; }

	virtual HSHAPE						GetHandleAt( const CPoint &point ) const;

	virtual bool						Intersects( const CRect& rect ) const;

	virtual void						Activate( const CPoint& point );
	virtual void						Update( void );

	virtual void						BeginDrag( const CPoint& point, HSHAPE handle = -1 );
	virtual void						Drag( const CPoint& point );
	virtual void						EndDrag( void );

	virtual void						Render( CDC* pDC ) const;
	virtual void						RenderTracker( CDC* pDC ) const;

	virtual void						Serialize( CArchive& ar );

	virtual void						DragTemp( const CPoint& point );
	void								Finish( const CPoint& point, CShape* pShape );

	eBRANCH_TYPE						GetBranchType( void ) const;
	void								SetBranchType( eBRANCH_TYPE eType );

	const CString&						GetBranchName( void ) const;
	void								EditBranchName( void );

// Implementations
protected:
	virtual void						SetupTheme( void );

	// Segment 조작
	virtual double						GetSegLength( int nIndex ) const;
	virtual double						GetSegAngle( int nIndex ) const;
	virtual bool						IntersectsSeg( const CRect &rect, int nIndex ) const;
	virtual int							BreakSeg( int nIndex, const CPoint &point );
	virtual void						RemoveSeg( int nIndex );
	bool								OptimizeSeg( void );

	// 좌표 변환
	virtual void						SrcTransform( CPoint p[], int nCount ) const;
	virtual void						DestTransform( CPoint p[], int nCount ) const;

	virtual CPoint						GetCenterPoint( void ) const;

	virtual CRect						GetBranchNameRect( void ) const;
};


inline CShape_Link::eBRANCH_TYPE CShape_Link::GetBranchType( void ) const
{
	return m_eBranchType;
}

inline void CShape_Link::SetBranchType( CShape_Link::eBRANCH_TYPE eType )
{
	m_eBranchType = eType;
	SetupTheme();
}

inline const CString& CShape_Link::GetBranchName( void ) const
{
	return m_strBranchName;
}
