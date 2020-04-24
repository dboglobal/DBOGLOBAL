#ifndef __GUI_COMPONENTTYPE_H__
#define __GUI_COMPONENTTYPE_H__

#include "gui_define.h"

START_GUI

class CComponent;
class CSurfaceManager;

//: Component Type
class CComponentType
{
public:
//! Construction:
	//: component type 持失切.
	CComponentType(){;}

	//: component type 社瑚切.
	virtual ~CComponentType() {;}

	//: create component
	virtual CComponent *CreateComponent(
		CComponent *pParent,
		CSurfaceManager *pSurfaceManager) = 0;

	//: Has option
	virtual bool HasOption(const std::string &optionname) const
	{
		return m_stlOption.find(optionname) != m_stlOption.end();
	}

	struct SOptionType
	{
		enum EDatatype
		{
			STRING,
			BOOL,
			NUMBER
		} type;
	};
	//: Options

	typedef std::map<std::string, SOptionType> stlOPTION_MAP;
	stlOPTION_MAP m_stlOption;

};

END_GUI

#endif

