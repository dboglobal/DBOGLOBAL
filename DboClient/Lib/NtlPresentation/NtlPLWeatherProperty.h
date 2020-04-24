#pragma  once

#include "NtlPLProperty.h"
#include "NtlPLWeatherDefine.h"

class CNtlPLWeatherProperty : public CNtlPLProperty
{
public:
	CNtlPLWeatherProperty();
	virtual ~CNtlPLWeatherProperty();

	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);

	//RwBool			LoadEmitter(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode, SPLWeatherEmitter* pEmitter);
	//RwBool			LoadParticle(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode, SPLWeatherEmitter* pEmitter);

	void			SetSoundPath(const RwChar * pName) { m_strSoundPath = pName; }
	const RwChar *	GetSoundPath() const { return m_strSoundPath.c_str(); }

	void			SetTexturePath(const RwChar * pName) { m_strTexturePath = pName; }
	const RwChar *	GetTexturePath() const { return m_strTexturePath.c_str(); }

public:
	typedef std::map<RwInt32, SPLWeatherEmitter>	EMITTER_MAP;
	typedef EMITTER_MAP::iterator					EMITTER_MAP_IT;
	EMITTER_MAP										m_mapEmitter;

protected:
	std::string									m_strSoundPath;
	std::string									m_strTexturePath;
};