#pragma once

//////////////////////////////////////////////////////////////////////////
// CNtlWorldPropBaseTemplateEx
//////////////////////////////////////////////////////////////////////////

class CNtlWorldPropBaseTemplateEx
{
public:
	CNtlWorldPropBaseTemplateEx();
	virtual ~CNtlWorldPropBaseTemplateEx();

	virtual void	UpdatePropertyPosInWorld(RwV3d& vPos);

protected:
	virtual void	UpdatePropertyPosInLoadedWorld(RwV3d& vPos) = 0;
	virtual void	UpdatePropertyPosInUnloadedWorld(RwV3d& vPos) = 0;

	virtual RwInt32 GetIndexPosInWorld(RwV3d& vPos) = 0;
	virtual RwBool	IsLoaded(const RwUInt32& iIndex) = 0;

	void			SetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA);
	void			GetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA);
};

//////////////////////////////////////////////////////////////////////////
// CNtlWorldPropBaseTemplateExSector : Sector 단위로 실행
//////////////////////////////////////////////////////////////////////////

class CNtlWorldPropBaseTemplateExSector : public CNtlWorldPropBaseTemplateEx
{
public:
	CNtlWorldPropBaseTemplateExSector();
	virtual ~CNtlWorldPropBaseTemplateExSector();

	virtual RwInt32 GetIndexPosInWorld(RwV3d& vPos);
	virtual RwBool	IsLoaded(const RwUInt32& iIndex);
};

//////////////////////////////////////////////////////////////////////////
// CNtlWorldPropBaseTemplateExField : Field 단위로 실행
//////////////////////////////////////////////////////////////////////////

class CNtlWorldPropBaseTemplateExField : public CNtlWorldPropBaseTemplateEx
{
public:
	CNtlWorldPropBaseTemplateExField();
	virtual ~CNtlWorldPropBaseTemplateExField();

	virtual RwInt32 GetIndexPosInWorld(RwV3d& vPos);
	virtual RwBool	IsLoaded(const RwUInt32& iIndex);
};

