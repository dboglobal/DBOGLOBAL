#pragma once

#include "NtlWorldPropBaseTemplateEx.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CNtlWorldPropMaterialController : public CNtlWorldPropBaseTemplateExField
{
public:
	CNtlWorldPropMaterialController();
	virtual ~CNtlWorldPropMaterialController();

	virtual void	UpdatePropertyPosInLoadedWorld(RwV3d& vPos);
	virtual void	UpdatePropertyPosInUnloadedWorld(RwV3d& vPos);

	virtual void	DeletePropertyPosInWorld(RwV3d& vPos);

private:
	typedef std::map<std::string, BYTE> MAP_MATERIAL;
	typedef MAP_MATERIAL::iterator		MAP_MATERIAL_IT;

	// material property map
	MAP_MATERIAL	m_mapMaterial;

	// temporary
	RwChar			m_cBTextureName[rwTEXTUREBASENAMELENGTH];
	RwChar			m_acDTextureName[dNTL_SPLAT_LAYER_NUM][rwTEXTUREBASENAMELENGTH];

	BYTE*			m_apbyAlphaBuffer[dNTL_SPLAT_LAYER_NUM];
	BYTE*			m_pbyBuffer;
	sNTL_FIELD_PROP	m_NtlFieldProp;
	
public:
	RwBool	LoadScript(const RwChar* pFileName);
	RwBool	SaveImage(const char* pFileName);

private: // m_mapMaterial Method
	void	AddMaterial(char *pName, BYTE byMaterial);
	BYTE	GetMaterial(char *pName);

private:
	void	CopyQuadBuffer(BYTE* pQuadBuffer, RwInt32 iQuad, BYTE* pBuffer);
	void	CopyAlphaFromTexture(BYTE* pBuffer, RwTexture* pTexture);
	void	UpdatePropertyFromTemporary(RwV3d& vPos);
	void	ClearTemporary();
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

class CNtlWorldPropMaterialController : public CNtlWorldPropBaseTemplateExField, public CNtlWorldFileMemAccessor
{
public:
	CNtlWorldPropMaterialController();
	virtual ~CNtlWorldPropMaterialController();

	virtual void	UpdatePropertyPosInLoadedWorld(RwV3d& vPos);
	virtual void	UpdatePropertyPosInUnloadedWorld(RwV3d& vPos);

	virtual void	DeletePropertyPosInWorld(RwV3d& vPos);

private:
	typedef std::map<std::string, BYTE> MAP_MATERIAL;
	typedef MAP_MATERIAL::iterator		MAP_MATERIAL_IT;

	// material property map
	MAP_MATERIAL	m_mapMaterial;

	// temporary
	RwChar			m_cBTextureName[rwTEXTUREBASENAMELENGTH];
	RwChar			m_acDTextureName[dNTL_SPLAT_LAYER_NUM][rwTEXTUREBASENAMELENGTH];

	BYTE*			m_apbyAlphaBuffer[dNTL_SPLAT_LAYER_NUM];
	BYTE*			m_pbyBuffer;
	sNTL_FIELD_PROP	m_NtlFieldProp;

public:
	RwBool	LoadScript(const RwChar* pFileName);
	RwBool	SaveImage(const char* pFileName);

private: // m_mapMaterial Method
	void	AddMaterial(char *pName, BYTE byMaterial);
	BYTE	GetMaterial(char *pName);

private:
	void	CopyQuadBuffer(BYTE* pQuadBuffer, RwInt32 iQuad, BYTE* pBuffer);
	void	CopyAlphaFromTexture(BYTE* pBuffer, RwTexture* pTexture);
	void	UpdatePropertyFromTemporary(RwV3d& vPos);
	void	ClearTemporary();
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif