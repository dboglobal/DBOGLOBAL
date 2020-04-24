#pragma once


#include "Shape_GEntityCont.h"


class CShape_Proposal : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_Proposal )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_Proposal( void )				{ return; }

public:
	CShape_Proposal( const CPoint& point );
	virtual ~CShape_Proposal( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );

	virtual void						Serialize( CArchive &ar );
};