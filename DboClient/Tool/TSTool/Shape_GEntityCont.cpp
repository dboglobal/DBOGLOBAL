#include "StdAfx.h"
#include "Shape_GEntityCont.h"
#include "TSDoc.h"
#include "TSView.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "Attr_Page.h"
#include "OptionListBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	CShape_GEntityCont
//
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL( CShape_GEntityCont, CShape, 1 )


template<> void AFXAPI SerializeElements<CShape_GEntityCont::sATTR>( CArchive &ar, CShape_GEntityCont::sATTR* pData, INT_PTR nCount )
{
	if ( ar.IsStoring() )
	{
		for ( int i = 0; i < nCount; ++i )
		{
			ar << pData[i].strName;
			ar << pData[i].strValue;
		}
	}
	else
	{
		for ( int i = 0; i < nCount; ++i )
		{
			ar >> pData[i].strName;
			ar >> pData[i].strValue;
		}
	}
}


template<>
void AFXAPI SerializeElements<CShape_GEntityCont::sENTITY>( CArchive &ar, CShape_GEntityCont::sENTITY* pData, INT_PTR nCount )
{
	if ( ar.IsStoring() )
	{
		for ( int i = 0; i < nCount; ++i )
		{
			ar << pData[i].strType;
			ar << pData[i].strData;
		}
	}
	else
	{
		for ( int i = 0; i < nCount; ++i )
		{
			ar >> pData[i].strType;
			ar >> pData[i].strData;
		}
	}
}

CShape_GEntityCont::CShape_GEntityCont( void )
{
	SetupTheme();
}

CShape_GEntityCont::CShape_GEntityCont( const CPoint &point, CString strType )
{
	m_ptOrigin = point;

	m_strContType = strType;

	CString strContId;
	strContId.Format( _T("%d"), NTL_TS_TC_ID_INVALID );
	m_strContId = strContId;

	m_bShowAttr = true;

	m_bShowEntity = true;

	SetupTheme();

	m_nWidth = GetIdealSize().cx;
	m_nHeight = GetIdealSize().cy;

	UpdateHeightInfo();
}

CShape_GEntityCont::~CShape_GEntityCont( void )
{
}

CRect CShape_GEntityCont::GetBoundBox( void ) const
{
	CRect rt( m_ptOrigin, CSize( m_nWidth, m_nHeight ) );
	return rt;
}

CPoint CShape_GEntityCont::GetHandlePoint( HSHAPE handle ) const
{
	ASSERT( handle >= 0 && handle < 4 );

	switch ( handle )
	{
	case 0: return CPoint( m_ptOrigin );
	case 1: return CPoint( m_ptOrigin + CSize( 0, m_nHeight ) );
	case 2: return CPoint( m_ptOrigin + CSize( m_nWidth, m_nHeight ) );
	case 3: return CPoint( m_ptOrigin + CSize( m_nWidth, 0 ) );
	}

	return CPoint();
}

bool CShape_GEntityCont::Intersects( const CRect &rect ) const
{
	CRgn rgn;

	rgn.CreateRectRgnIndirect( CRect( m_ptOrigin, CSize( m_nWidth, m_nHeight ) ) );
	return rgn.RectInRegion( rect ) ? true : false;
}

void CShape_GEntityCont::Activate( const CPoint &point )
{
	if ( GetContTypeRect().PtInRect( point ) )
	{
		ShowContAttrEditBox();
	}
	else if ( GetContIdRect().PtInRect( point ) )
	{
		if ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 )
		{
			EditContId();
		}
		else
		{
			ShowContAttrEditBox();
		}
	}
	else if ( GetAttrAt( point ) != -1 && m_bShowAttr )
	{
		if ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 )
		{
			EditAttr( GetAttrAt( point ) );
		}
		else
		{
			ShowContAttrEditBox();
		}
	}
	else if ( GetEntityAt( point ) != -1 && m_bShowEntity )
	{
		if ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 )
		{
			EditEntity( GetEntityAt( point ) );
		}
		else if ( GetAsyncKeyState( VK_LSHIFT ) & 0x8000 )
		{
			int nIdx = GetEntityAt( point );

			CString strMsg;
			strMsg.Format( _T("[%s]를 정말 지우시겠습니까?"), m_EntityList[nIdx].strType.GetString() );

			if ( IDOK == AfxMessageBox( strMsg, MB_OKCANCEL ) )
			{
				RemoveEntity( nIdx );
			}
		}
		else
		{
			ShowEntityAttrEditBox( GetEntityAt( point ) );
		}
	}
	else if( GetNewEntityRect().PtInRect( point ) && m_bShowEntity )
	{
		ShowEntityAttrAddBox();
	}
}

void CShape_GEntityCont::Drag( const CPoint &point )
{
	CPoint p;

	switch ( m_hDragHandle )
	{
	case 0:
		p = GetHandlePoint( 2 );
		m_nWidth = p.x - point.x;
		m_nHeight = p.y - point.y;
		AdjustSize();
		m_ptOrigin = p - CSize(m_nWidth, m_nHeight);
		break;

	case 1:
		p = GetHandlePoint( 3 );
		m_nWidth = p.x - point.x;
		m_nHeight = point.y - p.y;
		AdjustSize();
		m_ptOrigin = p - CSize(m_nWidth, 0);
		break;

	case 2:
		p = GetHandlePoint( 0 );
		m_nWidth = point.x - p.x;
		m_nHeight = point.y - p.y;
		AdjustSize();
		break;

	case 3:
		p = GetHandlePoint( 1 );
		m_nWidth = point.x - p.x;
		m_nHeight = p.y - point.y;
		AdjustSize();
		m_ptOrigin = p - CSize(0, m_nHeight);
		break;
	}

	CShape::Drag( point );
}

void CShape_GEntityCont::Render( CDC* pDC ) const
{
	CShape::Render( pDC );

	CPen pen( PS_SOLID, 0, m_LineColor ), *pOldPen;
	CBrush brush(m_FillColor), *pOldBrush;
	CFont *pOldFont;

	pOldPen = pDC->SelectObject( &pen );
	pOldBrush = pDC->SelectObject( &brush );
	pOldFont = pDC->SelectObject( GetContIdFont() );

	pDC->SetBkMode( TRANSPARENT );

	// Shape
	pDC->Rectangle( CRect(m_ptOrigin, CSize(m_nWidth, m_nHeight)) );

	if ( m_bShowAttr )
	{
		pDC->MoveTo( m_ptOrigin + CSize(0, m_aHeightList[2]) );
		pDC->LineTo( m_ptOrigin + CSize(m_nWidth, m_aHeightList[2]) );
	}

	if ( m_bShowEntity )
	{
		int nIndex = 4 + GetAttrCount();
		pDC->MoveTo( m_ptOrigin + CSize(0, m_aHeightList[nIndex]) );
		pDC->LineTo( m_ptOrigin + CSize(m_nWidth, m_aHeightList[nIndex]) );
	}

	// Container id
	pDC->SetTextColor( m_ContIdColor );
	pDC->DrawText( m_strContId, GetContIdRect(), DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX );

	// Container type
	CString strContType;
	strContType.Format( _T("[%s]"), m_strContType );

	pDC->SelectObject( m_pContTypeFont );
	pDC->SetTextColor( m_ContTypeColor );
	pDC->DrawText( strContType, GetContTypeRect(), DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX );

	// Seperator names
	pDC->SelectObject( m_pSeperatorNameFont );
	pDC->SetTextColor( m_SeperatorNameColor );

	if ( m_bShowAttr ) pDC->DrawText( _T("Attr"), GetAttrSeperatorNameRect(), DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
	if ( m_bShowEntity ) pDC->DrawText( _T("ENTITY"), GetEntitySeperatorNameRect(), DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

	// Attr
	if ( m_bShowAttr )
	{
		pDC->SetTextColor( m_AttrColor );

		CString strAttInfo;

		for ( int i = 0; i < GetAttrCount(); ++i )
		{
			strAttInfo = m_AttrList[i].strName;
			strAttInfo += _T(" : ");
			strAttInfo += m_AttrList[i].strValue;

			pDC->SelectObject( GetAttrFont(i) );
			pDC->DrawText( strAttInfo, GetAttrRect(i), DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX );
		}
	}

	// Entity
	if ( m_bShowEntity )
	{
		pDC->SetTextColor( m_EntityColor );

		CString strEntityInfo;

		for ( int i = 0; i < GetEntityCount(); ++i )
		{
			strEntityInfo = m_EntityList[i].strType;
			strEntityInfo += _T(" : ");
			strEntityInfo += m_EntityList[i].strData;

			pDC->SelectObject( GetEntityFont(i) );
			pDC->DrawText( strEntityInfo, GetEntityRect(i), DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX );
		}
	}

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );
	pDC->SelectObject( pOldFont );
}

void CShape_GEntityCont::RenderTracker(CDC *pDC) const
{
	CPen pen( PS_DOT, 0, m_LineColor ), *pOldPen;
	CBrush* pOldBrush;

	pOldPen = pDC->SelectObject( &pen );
	pOldBrush = (CBrush*)pDC->SelectStockObject( NULL_BRUSH );

	pDC->SetBkMode( TRANSPARENT );
	pDC->Rectangle( CRect( m_ptOrigin, CSize(m_nWidth, m_nHeight)) );

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );

	CShape::RenderTracker( pDC );
}

void CShape_GEntityCont::Serialize( CArchive &ar )
{
	CShape::Serialize( ar );

	if ( ar.IsStoring() )
	{
		ar << m_nWidth;
		ar << m_nHeight;

		ar << (int)m_bShowAttr;
		ar << (int)m_bShowEntity;

		ar << m_strContType;
		ar << m_strContId;
	}
	else
	{
		int nTrue;

		ar >> m_nWidth;
		ar >> m_nHeight;

		ar >> nTrue; m_bShowAttr = nTrue ? true : false;
		ar >> nTrue; m_bShowEntity = nTrue ? true : false;

		ar >> m_strContType;
		ar >> m_strContId;
	}

	m_AttrList.Serialize( ar );
	m_EntityList.Serialize( ar );

	UpdateHeightInfo();
}

void CShape_GEntityCont::EditContId( void )
{
	GetActiveView()->BeginEditText( m_strContId, GetContIdRect(), GetContIdFont(), ES_CENTER );
}

bool CShape_GEntityCont::HasAttr( const CString& strName )
{
	int nSize = (int)m_AttrList.GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		if ( strName == m_AttrList[i].strName )
		{
			return true;
		}
	}

	return false;
}

CString CShape_GEntityCont::GetAttr( const CString& strName )
{
	int nSize = (int)m_AttrList.GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		if ( strName == m_AttrList[i].strName )
		{
			return m_AttrList[i].strValue;
		}
	}

	return CString();
}

CString CShape_GEntityCont::GetAttrString( void )
{
	CString strResult;
	CString strData;

	int nSize = (int)m_AttrList.GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		strData.Format( _T("%s # %s # "), m_AttrList[i].strName, m_AttrList[i].strValue );

		strResult += strData;
	}

	return strResult;
}

void CShape_GEntityCont::SetAttr( int nIndex, const sATTR& sAttr )
{
	ASSERT( nIndex >= 0 && nIndex < GetAttrCount() );

	m_AttrList[nIndex] = sAttr;

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_GEntityCont::SetAttr( const CString& strName, const CString& strValue )
{
	int i;
	int nSize = (int)m_AttrList.GetSize();
	for ( i = 0; i < nSize; ++i )
	{
		if ( strName == m_AttrList[i].strName )
		{
			m_AttrList[i].strValue = strValue;
			break;
		}
	}

	if ( i == nSize )
	{
		AddAttr( strName, strValue, false );
		return;
	}

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_GEntityCont::SetAttrString( const CString& strString )
{
	m_AttrList.RemoveAll();

	enum { eTOKEN_TYPE_KEY, eTOKEN_TYPE_VALUE };

	CString strKey, strValue;

	CString strToken;
	int nStart = 0;
	bool bLoop = true;

	int nTokType = eTOKEN_TYPE_KEY;

	while ( bLoop )
	{
		strToken = strString.Tokenize( _T("#"), nStart );
		if ( nStart >= strString.GetLength() ) bLoop = false;

		switch ( nTokType )
		{
		case eTOKEN_TYPE_KEY:
			{
				strKey = strToken.Trim();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_VALUE;
			}
			break;

		case eTOKEN_TYPE_VALUE:
			{
				strValue = strToken.Trim();

				{
					int nIndex = (int)m_AttrList.GetSize();

					CShape_GEntityCont::sATTR newAttr;
					newAttr.strName = strKey;
					newAttr.strValue = strValue;

					m_AttrList.InsertAt( nIndex, newAttr );
				}

				strKey.Empty();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_KEY;
			}
			break;
		}
	}

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_GEntityCont::EditAttr( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < GetAttrCount() );

	GetActiveView()->BeginEditText( m_AttrList[nIndex].strName, GetAttrRect( nIndex ), GetAttrFont( nIndex ) );
}

void CShape_GEntityCont::AddAttr( CString strName, const CString& strValue, bool bAutoEdit /*= true*/, int nIndex /*= -1*/ )
{
	if ( -1 == nIndex ) nIndex = (int)m_AttrList.GetSize();

	sATTR newAttr;
	newAttr.strName = strName;
	newAttr.strValue = strValue;

	m_AttrList.InsertAt( nIndex, newAttr );

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );

	if ( bAutoEdit ) EditAttr( nIndex );
}

void CShape_GEntityCont::RemoveAttr( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < GetAttrCount() );

	m_AttrList.RemoveAt( nIndex );

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_GEntityCont::SetEntity( int nIndex, const sENTITY& sEntity )
{
	ASSERT( nIndex >= 0 && nIndex < GetEntityCount() );

	m_EntityList[nIndex] = sEntity;

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_GEntityCont::EditEntity( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < GetEntityCount() );

	GetActiveView()->BeginEditText( m_EntityList[nIndex].strData, GetEntityRect( nIndex ), GetEntityFont( nIndex ) );
}

void CShape_GEntityCont::AddEntity( CString strType, const CString& strData, bool bAutoEdit /*= true*/, int nIndex /*= -1*/ )
{
	if ( -1 == nIndex ) nIndex = GetEntityCount();

	sENTITY newEntity;
	newEntity.strType = strType;
	newEntity.strData = strData;

	m_EntityList.InsertAt( nIndex, newEntity );

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );

	if ( bAutoEdit ) EditEntity( nIndex );
}

void CShape_GEntityCont::RemoveEntity( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < GetEntityCount() );

	m_EntityList.RemoveAt( nIndex );

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_GEntityCont::RemoveAttr( CString strName )
{
	int nSize = (int)m_AttrList.GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		if ( strName == m_AttrList[i].strName )
		{
			RemoveAttr( i );
			return;
		}
	}
}

void CShape_GEntityCont::GetEntityProperty( int nIndex, CNtlTSScrProperty& clProperty )
{
	enum { eTOKEN_TYPE_KEY, eTOKEN_TYPE_VALUE };

	CString strData = m_EntityList[nIndex].strData;
	if ( strData.IsEmpty() ) return;

	CString strKey, strValue;

	CString strToken;
	int nStart = 0;
	bool bLoop = true;

	int nTokType = eTOKEN_TYPE_KEY;

	while ( bLoop )
	{
		strToken = strData.Tokenize( _T("#"), nStart );
		if ( nStart >= strData.GetLength() ) bLoop = false;

		switch ( nTokType )
		{
		case eTOKEN_TYPE_KEY:
			{
				strKey = strToken.Trim();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_VALUE;
			}
			break;

		case eTOKEN_TYPE_VALUE:
			{
				strValue = strToken.Trim();

				clProperty.m_defProperty[strKey.GetBuffer()] = strValue;

				strKey.Empty();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_KEY;
			}
			break;
		}
	}
}

void CShape_GEntityCont::SetupTheme( void )
{
	CShape::SetupTheme();

	m_nContTypeHeight = GetDoc()->GetTheme().GetContTypeHeight();
	m_pContTypeFont = GetDoc()->GetTheme().GetContTypeFont();

	m_nContIdHeight = GetDoc()->GetTheme().GetContIdHeight();
	m_pContIdFont = GetDoc()->GetTheme().GetContIdFont();

	m_nAttrHeight = GetDoc()->GetTheme().GetAttrHeight();
	m_nNewAttrHeight = m_nAttrHeight / 2;
	m_pAttrFont = GetDoc()->GetTheme().GetAttrFont();

	m_nEntityHeight = GetDoc()->GetTheme().GetEntityHeight();
	m_nNewEntityHeight = m_nEntityHeight / 2;
	m_pEntityFont = GetDoc()->GetTheme().GetEntityFont();

	m_nSeperatorNameHeight = GetDoc()->GetTheme().GetSeperatorNameHeight();
	m_pSeperatorNameFont = GetDoc()->GetTheme().GetSeperatorNameFont();

	m_FillColor = GetDoc()->GetTheme().GetGECBackgroundColor();
	m_LineColor = GetDoc()->GetTheme().GetLineColor();
	m_ContIdColor = GetDoc()->GetTheme().GetContIdColor();
	m_AttrColor = GetDoc()->GetTheme().GetAttrColor();
	m_EntityColor = GetDoc()->GetTheme().GetEntityColor();
	m_SeperatorNameColor = GetDoc()->GetTheme().GetSeperatorNameColor();
	m_ContTypeColor = GetDoc()->GetTheme().GetContTypeColor();
}

CSize CShape_GEntityCont::GetMinSize( void ) const
{
	int nIndex = 6 + GetAttrCount() + GetEntityCount();
	return CSize( 75, m_aHeightList[nIndex] );
}

CSize CShape_GEntityCont::GetMaxSize( void ) const
{
	return GetMinSize() + CSize(1000, 1000);
}

CSize CShape_GEntityCont::GetIdealSize( void ) const
{
	return CSize(150, 150);
}

void CShape_GEntityCont::AdjustSize( void )
{
	CSize MinSize = GetMinSize();
	CSize MaxSize = GetMaxSize();

	ASSERT( MinSize.cx <= MaxSize.cx );
	ASSERT( MinSize.cy <= MaxSize.cy );

	if ( m_nWidth < MinSize.cx) m_nWidth = MinSize.cx;
	if ( m_nWidth > MaxSize.cx) m_nWidth = MaxSize.cx;
	if ( m_nHeight < MinSize.cy) m_nHeight = MinSize.cy;
	if ( m_nHeight > MaxSize.cy) m_nHeight = MaxSize.cy;
}

void CShape_GEntityCont::UpdateHeightInfo( void )
{
	int y = 0;
	int index = 0;
	int i;

	// 0: ContType
	m_aHeightList[index++] = y;
	y += m_nContTypeHeight;

	// 1: name
	m_aHeightList[index++] = y;
	y += m_nContIdHeight;

	// 2: Attr compartiment
	m_aHeightList[index++] = y;
	if ( m_bShowAttr ) y += m_nSeperatorNameHeight;

	// 3 .. : Attr
	for ( i = 0; i < GetAttrCount(); ++i )
	{
		m_aHeightList[index++] = y;
		y += m_bShowAttr ? m_nAttrHeight : 0;
	}

	// 3 + Na: new Attr
	m_aHeightList[index++] = y;
	y += m_bShowAttr ? m_nNewAttrHeight : 0;

	// 4 + Na: Entity compartiment
	m_aHeightList[index++] = y;
	if ( m_bShowEntity ) y += m_nSeperatorNameHeight;

	// 5 + Na .. : Entity
	for ( i = 0; i < GetEntityCount(); ++i )
	{
		m_aHeightList[index++] = y;
		y += m_bShowEntity ? m_nEntityHeight : 0;
	}

	// 5 + Na + Nm: new Entity
	m_aHeightList[index++] = y;
	y += m_bShowEntity ? m_nNewEntityHeight : 0;

	// 6 + Na + Nm: bottom
	m_aHeightList[index++] = y;

	AdjustSize();
}

CRect CShape_GEntityCont::GetContTypeRect( void ) const
{
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[0] + 2), CSize(m_nWidth, m_nContTypeHeight) );
	rt.DeflateRect( 4, 2 );
	return rt;
}

CFont* CShape_GEntityCont::GetContIdFont( void ) const
{
	return m_pContIdFont;
}

CRect CShape_GEntityCont::GetContIdRect( void ) const
{
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[1]), CSize(m_nWidth, m_nContIdHeight) );
	rt.DeflateRect( 4, 2 );
	return rt;
}

CFont* CShape_GEntityCont::GetAttrFont( int nIndex ) const
{
	return m_pAttrFont;
}

int CShape_GEntityCont::GetAttrAt( const CPoint& point ) const
{
	for ( int i = 0; i < GetAttrCount(); ++i )
	{
		if ( GetAttrRect(i).PtInRect(point) )
		{
			return i;
		}
	}

	return -1;
}

CRect CShape_GEntityCont::GetAttrSeperatorNameRect( void ) const
{
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[2]), CSize(m_nWidth, m_nSeperatorNameHeight) );
	rt.DeflateRect( 4, 2 );
	return rt;
}

CRect CShape_GEntityCont::GetAttrRect( int nIndex ) const
{
	ASSERT( nIndex >= 0 && nIndex < GetAttrCount() );

	nIndex += 3;
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[nIndex]), CSize(m_nWidth, m_nAttrHeight) );
	rt.DeflateRect( 10, 1, 4, 1 );
	return rt;
}

CRect CShape_GEntityCont::GetNewAttrRect( void ) const
{
	int nIndex = 3 + GetAttrCount();
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[nIndex]), CSize(m_nWidth, m_nNewAttrHeight) );
	return rt;
}

CFont* CShape_GEntityCont::GetEntityFont( int nIndex ) const
{
	ASSERT( nIndex >= 0 && nIndex < GetEntityCount());
	return m_pEntityFont;
}

int CShape_GEntityCont::GetEntityAt( const CPoint& point ) const
{
	for ( int i = 0; i < GetEntityCount(); ++i )
	{
		if ( GetEntityRect(i).PtInRect(point) )
		{
			return i;
		}
	}

	return -1;
}

CRect CShape_GEntityCont::GetEntitySeperatorNameRect( void ) const
{
	int nIndex = 4 + GetAttrCount();
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[nIndex]), CSize(m_nWidth, m_nSeperatorNameHeight) );
	rt.DeflateRect( 4, 2 );
	return rt;
}

CRect CShape_GEntityCont::GetEntityRect( int nIndex ) const
{
	ASSERT( nIndex >= 0 && nIndex < GetEntityCount() );

	nIndex += 5 + GetAttrCount();
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[nIndex]), CSize(m_nWidth, m_nEntityHeight) );
	rt.DeflateRect( 10, 1, 4, 1 );
	return rt;
}

CRect CShape_GEntityCont::GetNewEntityRect( void ) const
{
	int nIndex = 5 + GetAttrCount() + GetEntityCount();
	CRect rt( m_ptOrigin + CSize(0, m_aHeightList[nIndex]), CSize(m_nWidth, m_nHeight - m_aHeightList[nIndex]) );
	return rt;
}


void CShape_GEntityCont::ShowEntityAttrEditBox( int nIdx )
{
	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	CAttr_Page* pPage = GetAttrEntityMng()->GetPage( m_EntityList[nIdx].strType );
	pPage->SetAttr( m_EntityList[nIdx].strData );

	clDiag.AddAttrPage( pPage );

	if ( IDOK == clDiag.DoModal() )
	{
		CAttr_Page* pEditedPage = clDiag.GetEditedAttrPage();

		sENTITY sEntity;
		sEntity.strType = m_EntityList[nIdx].strType;
		sEntity.strData = pEditedPage->GetAttr();

		SetEntity( nIdx, sEntity );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}
