#pragma once


#include "NtlPLEntity.h"
#include "ntlworldcommon.h"
#include "NtlSoundDefines.h"


#ifdef dNTL_WORLD_TOOL_MODE

class CNtlPLObject;

#endif


class CNtlPLSound : public CNtlPLEntity
{
public:
	CNtlPLSound(void);
	virtual ~CNtlPLSound(void);


#ifdef dNTL_WORLD_TOOL_MODE

protected:
	CNtlPLObject* m_pNtlPLObj;

#endif

protected:
	RwChar			m_SoundFileName[128];
	RwReal			m_Volume;
	RwReal			m_RadiusMin;
	RwReal			m_RadiusMax;
	SOUND_HANDLE    m_hSound;
	RwV3d			m_Pos;

public:
	virtual VOID	SetVision(RwBool _Flag);
	virtual RwChar*	GetName() { return m_SoundFileName; }
	virtual RwReal&	GetVolume() { return m_Volume; }
	virtual RwReal&	GetRadius(RwBool IsMin) { if(IsMin) { return m_RadiusMin; } else { return m_RadiusMax; } }
	virtual VOID	ControlVolume(RwReal Value2Append);
	virtual VOID	ControlRadiusMin(RwReal Value2Append);
	virtual VOID	ControlRadiusMax(RwReal Value2Append);
	virtual RwInt32	PlaySound(RwChar* pFileName);
    virtual RwInt32 PlayEffectSound(RwChar* pFileName, RwBool bLoop, RwInt32 nVolume, RwReal fDistance, RwReal fDecayDist, RwReal fPitch);


public:
	void*	operator new(size_t size);
	void	operator delete(void *pObj);

	virtual RwV3d	GetPosition(void);
	virtual void	SetPosition(const RwV3d *pPos);
	virtual RwBool	SetProperty(const CNtlPLProperty *pProperty);

	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();

	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlPLSoundBGM : public CNtlPLSound
{
protected:
	RwReal			m_Radius;

public:
	CNtlPLSoundBGM(void);
	virtual ~CNtlPLSoundBGM(void);

	void*	operator new(size_t size);
	void	operator delete(void *pObj);

	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();

	virtual void	SetPosition(const RwV3d *pPos);

	virtual RwInt32	PlaySound(RwChar* pFileName);
	
	// Sound BGM은 Radius가 Min과 Max가 동일하다.
	virtual RwReal&	GetRadius(RwBool IsMin = TRUE)			{ return m_Radius; }

	// Sound BGM은 Volume은 조절이 불가능하다.
	virtual RwReal&	GetVolume()								{ return m_Volume; }

	virtual VOID	ControlVolume(RwReal Value2Append);
	virtual VOID	ControlRadiusMin(RwReal Value2Append);
	virtual VOID	ControlRadiusMax(RwReal Value2Append);

	virtual VOID	SetVision(RwBool _Flag);
};