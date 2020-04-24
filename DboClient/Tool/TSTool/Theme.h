#pragma once


//////////////////////////////////////////////////////////////////////////
//
//	CTheme
//
//////////////////////////////////////////////////////////////////////////


class CTheme : public CObject  
{
// Member variables
protected:
	mutable CFont*						m_pEditFont;

	int									m_nHandleSize;

	// Common
	COLORREF							m_LineColor;

	// Grid
	int									m_nGridX;
	int									m_nGridY;

	COLORREF							m_GridColor;
	COLORREF							m_GridBackgroundColor;

	// Note
	CFont*								m_pNoteFont;

	int									m_nCornerSize;

	COLORREF							m_NoteBackgroundColor;
	COLORREF							m_NoteTextColor;
	COLORREF							m_NoteShadowColor;

	// Generic entity container
	CFont*								m_pContIdFont;
	CFont*								m_pContTypeFont;
	CFont*								m_pAttrFont;
	CFont*								m_pEntityFont;
	CFont*								m_pSeperatorNameFont;

	int									m_nContIdHeight;
	int									m_nAttrHeight;
	int									m_nEntityHeight;
	int									m_nContTypeHeight;
	int									m_nSeperatorNameHeight;

	COLORREF							m_GECBackgroundColor;
	COLORREF							m_ContTypeColor;
	COLORREF							m_ContIdColor;
	COLORREF							m_AttrColor;
	COLORREF							m_EntityColor;
	COLORREF							m_SeperatorNameColor;

	// Link
	COLORREF							m_LineColor_Def;
	COLORREF							m_LineColor_Yes;
	COLORREF							m_LineColor_No;
	COLORREF							m_LineColor_Error;
	COLORREF							m_LineColor_LogIn;
	CFont*								m_pBranchNameFont;

// Constructions and Destructions
public:
	CTheme( void );
	virtual ~CTheme( void );

// Methods
public:
	CFont*								EditFont( CFont* pRefFont, double scale ) const;

	int									GetHandleSize( void ) const { return m_nHandleSize; }

	// Common
	COLORREF							GetLineColor( void ) const { return m_LineColor; }

	// Grid
	int									GetGridX( void ) const { return m_nGridX; }
	int									GetGridY( void ) const { return m_nGridY; }

	COLORREF							GetGridBackgroundColor( void ) const { return m_GridBackgroundColor; }
	COLORREF							GetGridColor( void ) const { return m_GridColor; }

	// Note
	CFont*								GetNoteFont( void ) const { return m_pNoteFont; }

	int									GetCornerSize( void ) const { return m_nCornerSize; }

	COLORREF							GetNoteBackgroundColor( void ) const { return m_NoteBackgroundColor; }
	COLORREF							GetNoteTextColor( void ) const { return m_NoteTextColor; }
	COLORREF							GetNoteShadowColor( void ) const { return m_NoteShadowColor; }

	// Generic entity container
	CFont*								GetContTypeFont( void ) const { return m_pContTypeFont; }
	CFont*								GetContIdFont( void ) const { return m_pContIdFont; }
	CFont*								GetAttrFont( void ) const { return m_pAttrFont; }
	CFont*								GetEntityFont( void ) const { return m_pEntityFont; }
	CFont*								GetSeperatorNameFont( void ) const { return m_pSeperatorNameFont; }

	int									GetContTypeHeight( void ) const { return m_nContTypeHeight; }
	int									GetContIdHeight( void ) const { return m_nContIdHeight; }
	int									GetAttrHeight( void ) const { return m_nAttrHeight; }
	int									GetEntityHeight( void ) const { return m_nEntityHeight; }
	int									GetSeperatorNameHeight( void ) const { return m_nSeperatorNameHeight; }

	COLORREF							GetGECBackgroundColor( void ) const { return m_GECBackgroundColor; }
	COLORREF							GetContTypeColor( void ) const { return m_ContTypeColor; }
	COLORREF							GetContIdColor( void ) const { return m_ContIdColor; }
	COLORREF							GetAttrColor( void ) const { return m_AttrColor; }
	COLORREF							GetEntityColor( void ) const { return m_EntityColor; }
	COLORREF							GetSeperatorNameColor( void ) const { return m_SeperatorNameColor; }

	// Link
	COLORREF							GetLineColor_Def( void ) const { return m_LineColor_Def; }
	COLORREF							GetLineColor_Yes( void ) const { return m_LineColor_Yes; }
	COLORREF							GetLineColor_No( void ) const { return m_LineColor_No; }
	COLORREF							GetLineColor_Error( void ) const { return m_LineColor_Error; }
	COLORREF							GetLineColor_LogIn( void ) const { return m_LineColor_LogIn; }

	CFont*								GetBranchNameFont( void ) const { return m_pBranchNameFont; }
};
