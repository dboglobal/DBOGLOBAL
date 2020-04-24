#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "TSView.h"
#include "SCtrl_Note.h"
#include "Shape_Note.h"


/** 
	CSCtrl_Note
*/


CSCtrl_Note::CSCtrl_Note( CTSView* pView )
: CSCtrl( pView )
{
	m_pShapeNote = NULL;
}

void CSCtrl_Note::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnLButtonDown( point, nFlags );

	ASSERT( NULL == m_pShapeNote );

	m_pShapeNote = New( m_pView->Align2Grid(point) );
	GetDoc()->AddContainerShape( m_pShapeNote );

	m_pView->SelectShape( m_pShapeNote );
}

void CSCtrl_Note::OnLButtonUp( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnLButtonUp( point, nFlags );

	if ( m_pShapeNote )
	{
		m_pShapeNote = NULL;
	}
}

void CSCtrl_Note::OnMouseMove( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnMouseMove( point, nFlags );
}

void CSCtrl_Note::OnDoubleClick( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnDoubleClick( point, nFlags );
}

void CSCtrl_Note::OnContextMenu( const CPoint &point )
{
	CSCtrl::OnContextMenu( point );
}

void CSCtrl_Note::OnDeactivate( void )
{
	CSCtrl::OnDeactivate();

	if ( m_pShapeNote )
	{
		m_pShapeNote = NULL;
	}
}

CShape_Note* CSCtrl_Note::New( const CPoint& point )
{
	return new CShape_Note( point );
}
