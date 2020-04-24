//***********************************************************************************
//	File		:	VenusConfig.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <string>

#include <d3dx9.h>
#include "rwcore.h"

enum EDIT_LAYER
{
	EDIT_LAYER_EFFECT_EDIT= 0x00,
	EDIT_LAYER_TEXTURE,
	EDIT_LAYER_MESH,
	EDIT_LAYER_PATH,
	EDIT_LAYER_EVENT,

	EDIT_LAYER_MAX_COUNT
};

const DWORD	DATA_VERSION = 0x00000000;

/**
 * \ingroup Venus
 * \brief 공통으로 사용되는 Config 설정과 Save/Load를 담당하는 클래스
 * \date 2006-08-01
 * \author agebreak
 */
class CVenusConfig
{
public:
enum EDIT_MODE
{
	EDIT_MODE_MOVE = 0x00,
	EDIT_MODE_ROTATE,
	EDIT_MODE_SCALE
};
	std::string			m_strMainPath;

	RwInt32				m_nCurrentLayer;

	RwInt32				m_nEditMode;
	RwReal				m_fEditMoveSpeed;
	RwReal				m_fEditRotateSpeed;
	RwReal				m_fEditScaleSpeed;

	RwBool				m_bShowUpdateNews;
	std::string			m_strWinwordPath;

	// Time
	RwReal				m_fTimeSpeed;
	RwBool				m_bTimePause;
	RwBool				m_bTimeRepeat;

	// dummy
	RwBool				m_bShowGrid;
	RwBool				m_bShowTerrain;
	RwBool				m_bShowSky;


	// Light
	RwBool				m_bUseAmbient;
	RwRGBAReal			m_AmbientColor;

	RwBool				m_bUseLight;
	RwInt32				m_nLightType;
	RwRGBAReal			m_LightColor;
	RwV3d				m_vLightDirection;

	RwReal				m_fLightRadius;
	RwReal				m_fLightConeAngle;

	// Auto Save Max Time
	RwReal				m_fAutoSaveMaxTime;

	// Resource Path 
	std::string			m_strClumpTexturePath;
	std::string			m_strClumpPath;
	std::string			m_strAnimationPath;
	std::string			m_strUvAnimationPath;
	std::string			m_strEffectTexturePath;
    std::string         m_strSoundPath;
	std::string			m_strWorldPath;

	// Ball Speed
	RwReal				m_fEventBallSpeed;
	RwReal				m_fEventBallLifeTime;

	// Save
	CString				m_strAppName;
	CString				m_strFileName;
	CString				m_strPathName;

	// Render State
	D3DFILLMODE			m_D3DFillMode;

    RwRGBA              m_BackGroundColor;            ///< BackGround Color Load시 임시로 사용하는 변수

    RwBool              m_bRenderBoundingSphere;

public:
	CVenusConfig(void);
	~CVenusConfig(void);
	static	CVenusConfig&	GetInstance();

	void	SetMainDirectory();

	void	Save();                     ///< 작업내용및 Config를 Save한다.
	void	Load();                     ///< 작업내용및 Config를 Load한다.

	void	GetString(RwChar* strDest, RwChar* strSrc);
	void	ShowUpdateNews();
	RwBool	FindWinword(std::string& strWinwordPath, RwChar* path);
	RwBool	SubFindWinword(std::string& strWinwordPath);
    
protected:
    void	SaveGrid(FILE* pFile, CString strHead);     ///< Grid 설정을 저장한다.
    void	SaveTime(FILE* pFile, CString strHead);     ///< Time 설정을 저장한다.
    void	SaveDummy(FILE* pFile, CString strHead);    ///< Dummy 설정을 저장한다.
    void	SaveLight(FILE* pFile, CString strHead);    ///< Light 설정을 저장한다.
    void	SaveEvent(FILE* pFile, CString strHead);    ///< Event 설정을 저장한다.
    void	SavePath(FILE* pFile, CString strHead);     ///< Path 설정을 저장한다.

    void	LoadGrid(FILE* pFile);                      ///< Grid 설정을 로드한다.
    void	LoadTime(FILE* pFile);                      ///< Time 설정을 로드한다.
    void	LoadDummy(FILE* pFile);                     ///< Dummy 설정을 로드한다.
    void	LoadLight(FILE* pFile);                     ///< Light 설정을 로드한다.
    void	LoadEvent(FILE* pFile);                     ///< Event 설정을 로드한다.
    void	LoadPath(FILE* pFile);                      ///< Path 설정을 로드한다.

};
