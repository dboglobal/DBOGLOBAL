#pragma once


#include "SCtrl.h"


class CTSView;
class CShape_Note;


/** 
	CSCtrl_Note
*/


class CSCtrl_Note : public CSCtrl
{
// Member variables
protected:
	CShape_Note*						m_pShapeNote;

// Constructions and Destructions
public:
	CSCtrl_Note( CTSView* pView );
	virtual ~CSCtrl_Note( void )	{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags );
	virtual void						OnLButtonUp( const CPoint &point, UINT nFlags );
	virtual void						OnMouseMove( const CPoint &point, UINT nFlags );
	virtual void						OnDoubleClick( const CPoint &point, UINT nFlags );
	virtual void						OnContextMenu( const CPoint &point );
	virtual void						OnDeactivate( void );

	virtual CShape_Note*				New( const CPoint& point );
};
