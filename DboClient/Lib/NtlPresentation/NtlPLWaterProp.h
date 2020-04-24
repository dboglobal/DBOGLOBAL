#ifndef __NTLPLWATERPROPERTY_H__
#define __NTLPLWATERPROPERTY_H__


#include "NtlPLProperty.h"
#include "NtlCoreUtil.h"
#include <string>
#include <vector>


#define dWATER_PROP_MAX_STRING_SIZE (32)


class CNtlPLWaterProp : public CNtlPLProperty
{
public:
	CNtlPLWaterProp(void);
	virtual ~CNtlPLWaterProp(void);

private:
	struct sWATER_PROP 
	{
		std::string	_Name;
		RwReal		_FPS;
		RwInt32		_NumFrame;
		RwBool		_Specular;
	};

private:
	RwReal						m_Ver;
	RwChar						m_TexPath[NTL_MAX_DIR_PATH];

public:
	std::vector<sWATER_PROP>	m_vecWaterProp;

public:
	void SetVer(RwReal Ver) { m_Ver = Ver; }
	void SetTexPath(RwChar* TexPath) { strcpy_s(m_TexPath, TexPath); }
	RwReal GetVer() { return m_Ver; }
	RwChar* GetTexPath() { return m_TexPath; }

public:
	virtual RwBool	Load( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );
	virtual RwBool	Save( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );
};


#endif