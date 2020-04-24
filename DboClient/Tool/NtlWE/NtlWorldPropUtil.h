#pragma once

class CNtlWorldPropToolData
{
protected:
	std::string m_strPropName[16];
	RwRGBA		m_rgbaPropColor[16];
	RwRGBA		m_rgbaPropColorIntersection;

public:
	CNtlWorldPropToolData();
	virtual ~CNtlWorldPropToolData();

	void			LoadIni(std::string strFilePath);

	const char*		GetPropName(RwInt32 iIdx)	{ return m_strPropName[iIdx].c_str(); }
	const RwRGBA*	GetPropColor(RwInt32 iIdx)	{ return &m_rgbaPropColor[iIdx]; }
	const RwRGBA*	GetPropColorIntersection()	{ return &m_rgbaPropColorIntersection; }		
};