#pragma once


enum eIMAGE_TYPE
{
	eIT_NMAP_PROP = 0,
	eIT_SMAP_PROP,
	eIT_BGM_PROP,
};


class CNtlWorldPropBaseTemplate
{
public:
	CNtlWorldPropBaseTemplate(void);
	virtual ~CNtlWorldPropBaseTemplate(void);

protected:
	VOID	SetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA);
	VOID	SetPropImage(RwImage* _pImg, RwInt32 _IdxData, eIMAGE_TYPE _eIT);
	VOID	SetPropImageDat(RwV3d& _PosCur, RwRGBA& _RGBA, eIMAGE_TYPE _eIT, RwInt32 _Idx, FILE* pFile);
	VOID	GetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA);
	VOID	GetPropImageDat(RwV3d& _PosCur, RwRGBA& _RGBA, eIMAGE_TYPE _eIT, RwInt32 _Idx, FILE* pFile);

	virtual	VOID AppendPropDat(RwV3d& PosTile, DWORD _PropDat) = 0;
	virtual	VOID DeletePropDat(RwV3d& PosTile, DWORD _PropDat) = 0;
	virtual VOID LoadPropImage(RwChar* pTexName, RwInt32 IdxProp) = 0;
	virtual VOID SavePropImage(RwInt32 Width, RwInt32 Height, RwInt32 Depth, RwInt32 IdxProp, RwChar* pIdxName) = 0;

private:
	VOID	GetPropImage(eIMAGE_TYPE _eIT);
	VOID*	GetProp(RwInt32 IdxField, eIMAGE_TYPE _eIT);

public:
	static DWORD GetHexCode(RwInt32 _Idx);
};
