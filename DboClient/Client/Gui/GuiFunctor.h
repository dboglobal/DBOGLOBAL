////////////////////////////////////////////////////////////////////////////////
// File: GuiFunctor.h
// Desc: Gui Utility
//
// 2006.09.11 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_FUNCTOR_H__
#define __GUI_FUNCTOR_H__

class CPos;

#define BASIC_STAT_COLOR	RGB( 255, 255, 255 )
#define GOOD_STAT_COLOR		RGB( 0, 255, 0 )
#define BAD_STAT_COLOR		RGB( 255, 0, 0 )

class CGuiFunctor
{
public:
	//! Enumuration
	enum DECISION { BIG_IS_GOOD, SMALL_IS_GOOD };

	//! Destructor
	~CGuiFunctor(VOID) {}

	//! Access
	static CGuiFunctor* GetInstance(VOID) { static CGuiFunctor instance; return &instance; }

	//! Functions
	RwBool Calc3DPosTo2D(const RwV3d& v3DPos, RwReal fHeight, CPos* vOffset, CPos* pResPos );

	RwReal GetDistanceFromAvatar(const RwV3d& vDestPos );
	RwReal GetDistanceFromCamera(const RwV3d& vDestPos );

	RwUInt32 ColorDecision( RwUInt32 uiBase, RwUInt32 uiLast, DECISION eDecision = BIG_IS_GOOD,
							 RwUInt32 uiBasicColor = BASIC_STAT_COLOR, RwUInt32 uiGoodColor = GOOD_STAT_COLOR, RwUInt32 uiBadColor = BAD_STAT_COLOR );
	RwUInt32 ColorDecisionF( RwReal fBase, RwReal fLast, DECISION eDecision = BIG_IS_GOOD,
							 RwUInt32 uiBasicColor = BASIC_STAT_COLOR, RwUInt32 uiGoodColor = GOOD_STAT_COLOR, RwUInt32 uiBadColor = BAD_STAT_COLOR );

private:
	CGuiFunctor(VOID) {}
};

static CGuiFunctor* GetGuiFuntor(VOID)
{
	return CGuiFunctor::GetInstance();
}

#endif//__GUI_FUNCTOR_H__