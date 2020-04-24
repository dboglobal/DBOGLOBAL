#pragma once


#include "TSMng.h"
#include "Theme.h"
#include "TSProject.h"
#include "TSTrigger.h"


//////////////////////////////////////////////////////////////////////////
//
//	CTSDoc
//
//////////////////////////////////////////////////////////////////////////


class CTSDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE( CTSDoc )
	DECLARE_MESSAGE_MAP()

// Member variables
protected:
	static CTSDoc*						s_pInstance;

	CTSMng								m_clTSMng;

	CTheme								m_clTheme;

	const int							c_nExtentX;
	const int							c_nExtentY;

	CTSProject							m_TSProject;
	eSEL_GROUP_TYPE						m_eSelGroupType;
	CTSGroup*							m_pTSSelGroup;

// Constructions and Destructions
protected:
	CTSDoc( void );

public:
	virtual ~CTSDoc( void );

// Methods
public:
	virtual BOOL						OnNewDocument( void );
	virtual BOOL						OnOpenDocument( LPCTSTR lpszPathName );

	virtual void						Serialize( CArchive& ar );

	virtual void						DeleteContents( void );

	static CTSDoc*						GetInstance( void ) { return s_pInstance; }

	CTheme&								GetTheme( void ) { return m_clTheme; }

	int									GetExtentX( void ) const { return c_nExtentX; }
	int									GetExtentY( void ) const { return c_nExtentY; }

	CTSProject*							GetProjectObject( void );

	bool								IsProjectLoaded( void ) const;

	DWORD								GetProjectMode( void ) const;

	eSEL_GROUP_TYPE						GetSelGroupType( void ) const;
	CTSGroup*							GetSelGroup( void ) const { return m_pTSSelGroup; }

	void								SelMainGroup( void );
	void								SelExceptGroup( NTL_TS_TG_ID tgID );
	void								UnSelGroup( void );

	CShape*								GetShapeAt( const CPoint &point ) const;
	void								GetShapesInRect( const CRect &rect, ShapeList& ShapeList ) const;

	virtual void						MakeShapesBounds( CRect &rect ) const;

	void								AddShape( CShape* pShape );
	void								AddLinkShape( CShape* pShape );
	void								AddContainerShape( CShape* pShape );
	void								AddNoteShape( CShape* pShape );
	void								RemoveShape( CShape* pShape );

	void								Render( CDC* pDC );

	void								OnShapeChange( CShape* pShape = NULL );

	void								OnCreateProject( void );
	void								OnOpenProject( void );
	void								OnSaveProject( void );
	void								OnCloseProject( void );
	void								OnExportTrigger( void );
	void								OnExportTriggerAll( void );

	void								OnEncryptionTrigger( void );

	void								OnExportEventor( void );

	void								OnListReload( void );

// Implementations
protected:
	void								UpdateConstraints( CShape* pShape );
	void								Unselect( CShape* pShape = NULL );

#ifdef _DEBUG
	virtual void						AssertValid( void ) const;
	virtual void						Dump( CDumpContext& dc ) const;
#endif
};


inline CTSProject*	CTSDoc::GetProjectObject( void )
{
	return &m_TSProject;
}
