#ifndef __NTL_PLSKYDRAGON_H__
#define __NTL_PLSKYDRAGON_H__


#include "NtlPLSky.h"


enum eCUR_FADE_STATUS
{
	eCFS_NOTHING = 0,
	eCFS_IN,
	eCFS_OUT,
};

#define dSKY_DRAGON_FADE_VARIATION_TIME (2.0f)


#define dSKY_DRAGON_LAYER0_ROTATION_SPEED (+0.003f)
#define dSKY_DRAGON_LAYER1_ROTATION_SPEED (+0.005f)
#define dSKY_DRAGON_LAYER2_ROTATION_SPEED (-0.002f)


class CNtlPLSkyDragon : public CNtlPLSky
{
public:
	CNtlPLSkyDragon(void);
	virtual	~CNtlPLSkyDragon(void);

	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual RwBool	Update(RwReal fElapsed);
	virtual void	HandleEvents(RWS::CMsg &pMsg);
	virtual	void	UpdateFieldVariation(RwReal fElapsed);

protected:
	eCUR_FADE_STATUS m_CFSIdx;

protected:
	VOID	FadeLerp(eCUR_FADE_STATUS _CFS, RpAtomic* _pAtom, RwReal _STime, RwReal _ETime);
	VOID	VisibilitySwitch(RwBool _Switch);
	VOID	VisibilitySwitchOfMainSky(RwBool _Switch);

public:

	// Dragon appearance direction
	VOID FadeIn();

	// Dragon disappearance direction
	VOID FadeOut();

	eCUR_FADE_STATUS GetFadeStatus() { return m_CFSIdx; }
};

#endif //__NTL_PLSKYDRAGON_H__