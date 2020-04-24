#pragma once


#include "Shape_GEntityCont.h"


class CShape_UsrSel : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_UsrSel )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_UsrSel( void )				{ return; }

public:
	CShape_UsrSel( const CPoint& point );
	virtual ~CShape_UsrSel( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};