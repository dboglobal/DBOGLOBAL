#pragma once


#include "Shape_GEntityCont.h"


class CShape_GAct : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_GAct )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_GAct( void )					{ return; }

public:
	CShape_GAct( const CPoint &point );
	virtual ~CShape_GAct( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );
};