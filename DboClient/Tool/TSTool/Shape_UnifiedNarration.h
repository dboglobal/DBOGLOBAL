#pragma once


#include "Shape_GEntityCont.h"


class CShape_UnifiedNarration : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_UnifiedNarration )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_UnifiedNarration( void )			{ return; }

public:
	CShape_UnifiedNarration( const CPoint &point );
	virtual ~CShape_UnifiedNarration( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};