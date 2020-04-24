#pragma once

struct SAttachBone
{
	CNtlPLObject*	pNtlPLObject;
	std::string		strName;
	RwReal			fDistanceDelta;
};

class CUpdateEventLightObjectPalette
{
public:
	CUpdateEventLightObjectPalette();
	virtual ~CUpdateEventLightObjectPalette();

	virtual void UpdateLightObjectPalette();
	virtual void RenderLightObjectPalette();

	virtual void DeleteLightObjectPaletteDummies();

private:
	std::vector<CNtlPLEntity*>	m_vecSelected;
	std::vector<SAttachBone>	m_vecAttachBoneList;

	RwInt32						m_iTabAttachBone;
};