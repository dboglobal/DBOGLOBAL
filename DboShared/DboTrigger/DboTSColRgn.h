#ifndef _DBO_TSCOLRGN_H_
#define _DBO_TSCOLRGN_H_


#include "DboTSCoreDefine.h"


/**
	Collision region event
*/


class CDboTSColRgn : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	struct sRadiusData
	{
		float x, z;
		float r;
	};

	struct sRectangleData
	{
		float x[4], z[4];
	};

	union uColRgnData
	{
		sRadiusData						sRadius;
		sRectangleData					sRectangle;
	};

// Member variables
protected:
	unsigned int						m_uiWorldTblIdx;
	unsigned int						m_uiColCheckType;

	eEVENT_COL_RGN_TYPE					m_eColRgnType;
	uColRgnData							m_uColRngData;

	bool								m_bAABBMaked;
	float								m_fAABBBeginX, m_fAABBBeginZ;
	float								m_fAABBEndX, m_fAABBEndZ;

// Constructions
public:
	CDboTSColRgn( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_COL_RGN; }

	unsigned int						GetWorldIdx( void ) const;

	void								SetWorldIdx( unsigned int uiWorldIdx );

	unsigned int						GetColCheckType( void ) const;

	void								SetColCheckType( unsigned int uiColCheckType );

	void								SetRadiusData( float fX, float fZ, float fRadius );

	void								SetRectangleData( float fX0, float fZ0,
														  float fX1, float fZ1,
														  float fX2, float fZ2,
														  float fX3, float fZ3 );

	bool								ColCheck( float fBeginX, float fBeginZ, float fEndX, float fEndZ );

	bool								InCheck( float fPosX, float fPosZ );

	bool								IsInAABB( float fPosX, float fPosZ, float fPlusOffset = 0.f );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

	bool								IsInvalid( void );

	eCOL_RESULT_TYPE					ColCheck_Radius( float fBeginX, float fBeginZ, float fEndX, float fEndZ );
	eCOL_RESULT_TYPE					ColCheck_Retangle( float fBeginX, float fBeginZ, float fEndX, float fEndZ );
	bool								IsInRadius( float fX, float fZ, sRadiusData& sRadData );
	bool								IsInRectangle( float fX, float fZ, sRectangleData& sRectData );
	bool								IsIntersectLine( float fAx, float fAz, float fBx, float fBz, float fCx, float fCz, float fDx, float fDz );
};


inline unsigned int CDboTSColRgn::GetWorldIdx( void ) const
{
	return m_uiWorldTblIdx;
}

inline unsigned int CDboTSColRgn::GetColCheckType( void ) const
{
	return m_uiColCheckType;
}


#endif