/*****************************************************************************
 *
 * File			: NtlPLEntityBlend.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 11. 05	
 * Abstract		: Presentation object entity blend class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLENTITY_BLEND_H__
#define __NTL_PLENTITY_BLEND_H__

class CNtlPLEntity;

// Alpha Blend의 가중치 정보를 관리하는 클래스
class CNtlPLEntityAlphaWeightBlend
{
private:

	enum 
	{
		PLEAW_BLEND,                    // 블렌드 중
		PLEAW_END                       // 블렌드가 끝난 상태
	};

	RwUInt8	m_byBlendType;
	RwReal	m_fWeightValue;
	RwReal	m_fCurrTime;

	RwReal	m_fStartWeightValue;
	RwReal	m_fEndWeightValue;
	RwReal	m_fBlendTime;
	RwReal	m_fLifeTime;
	RwBool	m_bLoop;

	RwBool	m_bCalcWeightValue;
	RwBool	m_bFinish;

public:

	CNtlPLEntityAlphaWeightBlend();
	~CNtlPLEntityAlphaWeightBlend();

	virtual void Update(RwReal fElapsed);

	RwBool	IsCalcWeightValue(void);
	RwBool	IsFinish(void);

	void	SetWeight(RwReal fWeightValue, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
	void	SetWeight(RwReal fStartWeightValue, RwReal fEndWeightValue, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);

	RwReal	GetWeight(void) const { return m_fWeightValue; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLAtomicAlphaWeightBlend : public CNtlPLEntityAlphaWeightBlend
{
private:

	std::string m_strName;

public:

	CNtlPLAtomicAlphaWeightBlend();
	~CNtlPLAtomicAlphaWeightBlend();

	void	Update(RwReal fElapsed);

	void	SetWeight(const RwChar *pAtomicName, RwReal fWeightValue, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
	void	SetWeight(const RwChar *pAtomicName, RwReal fStartWeightValue, RwReal fEndWeightValue, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);

	std::string& GetAtomicName(void); 
};

inline std::string& CNtlPLAtomicAlphaWeightBlend::GetAtomicName(void)
{
	return m_strName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 알파 블렌드 가중치 객체들을 관리하는 컨트롤러
class CNtlPLEntityBlendController
{

public:
    CNtlPLEntityBlendController(CNtlPLEntity *pPLEntity);
    ~CNtlPLEntityBlendController();

    void Update(RwReal fElapsed);

    CNtlPLEntityAlphaWeightBlend*  AddAlpha(RwReal fWeightAlpha, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
    CNtlPLEntityAlphaWeightBlend*  AddAlpha(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);    

	CNtlPLAtomicAlphaWeightBlend*  AddAtomicAlpha(const RwChar *pAtomicName, RwReal fWeightAlpha, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
    CNtlPLAtomicAlphaWeightBlend*  AddAtomicAlpha(const RwChar *pAtomicName, RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);    
    
    void AddAlphaBlend(CNtlPLEntityAlphaWeightBlend* pAlphaBlend);              ///< 임의의 알파 블렌딩 객체를 리스트에 추가한다.
    void RemoveAlpha(CNtlPLEntityAlphaWeightBlend* pAlphaBlend);
	void RemoveAtomicAlpha(CNtlPLAtomicAlphaWeightBlend *pAtomicAlphaBlend);

    inline RwBool IsFinish(void);

protected:

    void	UpdateEntityAlpha(RwReal fElasped);
	void	UpdateAtomicAlpha(RwReal fElapsed);

	CNtlPLAtomicAlphaWeightBlend* FindAtomicAlphaBlend(const RwChar *pAtomicName);	

protected:

	typedef std::list<CNtlPLEntityAlphaWeightBlend*> ListAlphaWeightBlend;
	typedef std::list<CNtlPLAtomicAlphaWeightBlend*> ListAtomicAlphaWeightBlend;

	CNtlPLEntity				*m_pPLEntity;

	ListAlphaWeightBlend		m_listAlphaBlend;
	ListAtomicAlphaWeightBlend	m_listAtomicAlphaBlend;
};

inline RwBool CNtlPLEntityBlendController::IsFinish(void)
{
	if(m_listAlphaBlend.empty() && m_listAtomicAlphaBlend.empty())
		return TRUE;

	return FALSE;
}

#endif
