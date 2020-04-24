#pragma once


#include "Shape_GEntityCont.h"


class CShape_Start : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_Start )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_Start( void )				{ return; }

public:
	CShape_Start( const CPoint& point );
	virtual ~CShape_Start( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};