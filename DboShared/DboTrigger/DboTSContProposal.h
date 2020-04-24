#ifndef _DBO_TSACTCONTPROPOSAL_H_
#define _DBO_TSACTCONTPROPOSAL_H_


#include "DboTSContGAct.h"


/**
	Proposal container
*/


class CDboTSContProposal : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	bool								m_bDspSummary;

	unsigned int						m_uiTitle;
	unsigned int						m_uiArea;
	unsigned int						m_uiGoal;
	unsigned int						m_uiGrade;
	eGRADE_TYPE							m_eGradeType;
	unsigned int						m_uiSort;
	unsigned int						m_uiContents;
	NTL_TS_TC_ID						m_tcReward;

	NTL_TS_TC_ID						m_tcOk;
	NTL_TS_TC_ID						m_tcCancel;

// Constructions and Destructions
public:
	CDboTSContProposal( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_PROPOSAL; }

	bool								GetDisplaySummary( void ) const;
	void								SetDisplaySummary( bool bShow );

	unsigned int						GetTitle( void ) const;
	void								SetTitle( unsigned int uiTitle );

	unsigned int						GetArea( void ) const;
	void								SetArea( unsigned int uiArea );

	unsigned int						GetGoal( void ) const;
	void								SetGoal( unsigned int uiGoal );

	unsigned int						GetGrade( void ) const;
	void								SetGrade( unsigned int uiGrade );

	eGRADE_TYPE							GetGradeType( void ) const;
	void								SetGradeType( eGRADE_TYPE eGradeType );

	unsigned int						GetSort( void ) const;
	void								SetSort( unsigned int uiSort );

	unsigned int						GetContents( void ) const;
	void								SetContents( unsigned int uiContents );

	NTL_TS_TC_ID						GetReward( void ) const;
	void								SetReward( NTL_TS_TC_ID tcId );

	NTL_TS_TC_ID						GetOkLink( void ) const;
	void								SetOkLink( NTL_TS_TC_ID tcId );

	NTL_TS_TC_ID						GetCancelLink( void ) const;
	void								SetCancelLink( NTL_TS_TC_ID tcId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSContProposal::GetDisplaySummary( void ) const
{
	return m_bDspSummary;
}

inline unsigned int CDboTSContProposal::GetTitle( void ) const
{
	return m_uiTitle;
}

inline unsigned int CDboTSContProposal::GetArea( void ) const
{
	return m_uiArea;
}

inline unsigned int CDboTSContProposal::GetGoal( void ) const
{
	return m_uiGoal;
}

inline unsigned int CDboTSContProposal::GetGrade( void ) const
{
	return m_uiGrade;
}

inline eGRADE_TYPE CDboTSContProposal::GetGradeType( void ) const
{
	return m_eGradeType;
}

inline unsigned int CDboTSContProposal::GetSort( void ) const
{
	return m_uiSort;
}

inline unsigned int CDboTSContProposal::GetContents( void ) const
{
	return m_uiContents;
}

inline NTL_TS_TC_ID CDboTSContProposal::GetReward( void ) const
{
	return m_tcReward;
}

inline NTL_TS_TC_ID CDboTSContProposal::GetOkLink( void ) const
{
	return m_tcOk;
}

inline NTL_TS_TC_ID CDboTSContProposal::GetCancelLink( void ) const
{
	return m_tcCancel;
}


#endif