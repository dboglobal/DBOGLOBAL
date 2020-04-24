#pragma once


#include "Shape_GEntityCont.h"


class CShape_End : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_End )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_End( void )					{ return; }

public:
	CShape_End( const CPoint &point );
	virtual ~CShape_End( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};