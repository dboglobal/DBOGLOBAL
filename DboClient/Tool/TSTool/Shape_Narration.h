#pragma once


#include "Shape_GEntityCont.h"


class CShape_Narration : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_Narration )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_Narration( void )			{ return; }

public:
	CShape_Narration( const CPoint &point );
	virtual ~CShape_Narration( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};