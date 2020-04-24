#pragma once


class CTSView;
class CTSDoc;

/** 
	CSCtrl
*/


class CSCtrl : public CObject
{
// Member variables
protected:
	CTSView*							m_pView;

// Constructions and Destructions
public:
	CSCtrl( CTSView* pView );
	virtual ~CSCtrl( void )				{ return; }

// Methods
public:
	CTSDoc*								GetDoc( void );

	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags ) { return; }
	virtual void						OnLButtonUp( const CPoint &point, UINT nFlags ) { return; }
	virtual void						OnMouseMove( const CPoint &point, UINT nFlags ) { return; }
	virtual void						OnDoubleClick( const CPoint &point, UINT nFlags ) { return; }
	virtual void						OnContextMenu( const CPoint &point ) { return; }
	virtual void						OnDeactivate( void ) { return; }
};
