#pragma once


#include "Shape_GEntityCont.h"


class CShape_GCond : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_GCond )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_GCond( void )				{ return; }

public:
	CShape_GCond( const CPoint &point );
	virtual ~CShape_GCond( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};