#pragma once


#include "Shape_GEntityCont.h"


class CShape_Switch : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_Switch )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_Switch( void )					{ return; }

public:
	CShape_Switch( const CPoint &point );
	virtual ~CShape_Switch( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};