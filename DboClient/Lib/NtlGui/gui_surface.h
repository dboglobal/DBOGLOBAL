#ifndef __GUI_SURFACE_H__
#define __GUI_SURFACE_H__

#include "gui_define.h"
#include "rectangle.h"
#include "position.h"

struct PlaneSnapShot
{
	CRectangle		rtRect;		// Plane ÁÂÇ¥
	unsigned char	uRed;
	unsigned char	uGreen;
	unsigned char	uBlue;
	unsigned char	uAlpha;
	float			UVs[4];	
	unsigned int	uBlend;		// Blending Type = 0 ; glass, 1 ; light
};

struct TriSnapShot
{
	CPos			ptPos[3];
	unsigned char	uRed;
	unsigned char	uGreen;
	unsigned char	uBlue;
	unsigned char	uAlpha;
	float			UVs[6];	
	unsigned int	uBlend;		// Blending Type = 0 ; glass, 1 ; light
};

START_GUI

class CTexture;

#define SURFACE_MAX_SIZE      512

class CSurface// : public 
{
public:

	CSurface ()
	{
		m_Original.rtRect.left = 0;
		m_Original.rtRect.top = 0;
		m_Original.rtRect.right	= 0;
		m_Original.rtRect.bottom = 0;
		m_Original.uRed = 255;
		m_Original.uGreen = 255;
		m_Original.uBlue = 255;
		m_Original.uAlpha = 255;
		m_Original.UVs[0] = 0.0f;
		m_Original.UVs[1] = 0.0f;
		m_Original.UVs[2] = 1.0f;
		m_Original.UVs[3] = 1.0f;
		m_Original.uBlend = 0;

		m_SnapShot.rtRect.left = 0;
		m_SnapShot.rtRect.top = 0;
		m_SnapShot.rtRect.right = 0;
		m_SnapShot.rtRect.bottom = 0;
		m_SnapShot.uRed = 255;
		m_SnapShot.uGreen = 255;
		m_SnapShot.uBlue = 255;
		m_SnapShot.uAlpha = 255;
		m_SnapShot.UVs[0] = 0.0f;
		m_SnapShot.UVs[1] = 0.0f;
		m_SnapShot.UVs[2] = 1.0f;
		m_SnapShot.UVs[3] = 1.0f;
		m_SnapShot.uBlend = 0;

		m_pTexture = NULL;

		m_fAngle = 0;
	}

	PlaneSnapShot m_Original;
	PlaneSnapShot m_SnapShot;
	CTexture	  *m_pTexture;
	float		  m_fAngle;
};

#define INIT_PLANESNAPSHOT( snap ) \
	snap.rtRect.left = 0; snap.rtRect.top = 0; snap.rtRect.right = 0; snap.rtRect.bottom = 0; \
	snap.uRed = 255; snap.uGreen = 255; snap.uBlue = 255; snap.uAlpha = 255; \
	snap.UVs[0] = 0.0f;	snap.UVs[1] = 0.0f; snap.UVs[2] = 1.0f;	snap.UVs[3] = 1.0f; \
	snap.uBlend = 0;

END_GUI


#endif