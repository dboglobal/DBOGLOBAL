#pragma once


#include "Shape_GEntityCont.h"


class CShape_Reward : public CShape_GEntityCont
{
	DECLARE_SERIAL( CShape_Reward )

// Member variables
protected:

// Constructions and Destructions
protected:
	CShape_Reward( void )				{ return; }

public:
	CShape_Reward( const CPoint& point );
	virtual ~CShape_Reward( void );

// Methods
public:
	virtual void						ShowContAttrEditBox( void );
	virtual void						ShowEntityAttrAddBox( void );

	virtual void						SetAttrString( const CString& strString );

	virtual void						GetProperty( CNtlTSScrProperty& clProperty );

// Implementations
protected:
	CString								GetPropertyInfo( CString& strData, CString strName );
};