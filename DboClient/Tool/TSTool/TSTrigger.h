#pragma once


#include "Shape.h"


/** 
	CTSGroup
*/


class CTSGroup : public CObject
{
	DECLARE_SERIAL( CTSGroup )

// Member variables
public:
	NTL_TS_TG_ID						tgID;

	ShapeList							LinkLayer;
	ShapeList							ContainerLayer;
	ShapeList							NoteLayer;

// Construction and Destructions
public:
	CTSGroup( void );
	virtual ~CTSGroup( void );

// Methods
public:
	void								RemoveAll( void );

	NTL_TS_TG_ID						GetGroupID( void );

	ShapeList*							GetContainerLayer( void );

	virtual void						Serialize( CArchive& ar );
};


/** 
	CTSTrigger
*/


class CTSTrigger : public CObject
{
	DECLARE_SERIAL( CTSTrigger )

// Member variables
protected:
	DWORD								m_dwVersion;

	// 트리거의 속성
	CString								m_strAttr;

	// 트리거의 메인 그룹
	CTSGroup*							m_pMainGroup;

	// 트리거의 예외 그룹
	CArray<CTSGroup*, CTSGroup*>		m_ExceptGroup;

// Constructions and Destructions
public:
	CTSTrigger( void );
	virtual ~CTSTrigger( void );

// Methods
public:
	const CString&						GetAttr( void ) const;
	CString								GetAttr( CString strName );
	void								SetAttr( CString& strData );
	void								AddAttr( CString strKey, CString strValue );
	void								AddAttr( CString strKey, int nValue );
	void								ClearAttr( void );

	CTSGroup*							GetMainGroup( void );
	CTSGroup*							AddMainGroup( void );
	void								DelMainGroup( void );

	int									GetExceptionGroupCnt( void ) const;
	CTSGroup*							GetExceptionGroup( int nIndex );
	CTSGroup*							GetExceptionGroup( NTL_TS_TG_ID tgID );
	CTSGroup*							AddExceptionGroup( NTL_TS_TG_ID tgID );
	void								DelExceptionGroup( NTL_TS_TG_ID tgID );
	void								DelExceptionGroupAll( void );

	void								ClearAll( void );

	void								Load( CArchive& ar );
	void								Save( CArchive& ar );

// Implementations
protected:
	CString								PakingPageData( CString strKey, CString strValue );
	CString								PakingPageData( CString strKey, int nValue );
};


inline const CString& CTSTrigger::GetAttr( void ) const
{
	return m_strAttr;
}

inline class CTSGroup* CTSTrigger::GetMainGroup( void )
{
	return m_pMainGroup;
}

inline class CTSGroup* CTSTrigger::GetExceptionGroup( int nIndex )
{
	return m_ExceptGroup[nIndex];
}