/*****************************************************************************
* File			: NtlSimpleDOM.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 11
* Abstract		: Pathengine cSimpleDOM
*****************************************************************************
* Desc:
*
*****************************************************************************/

#ifndef _NTL_SIMPLE_DOM_
#define _NTL_SIMPLE_DOM_


#include <string>
#include <map>
#include <vector>

class CNtlSimpleDOM
{
public:

	std::string _name;
	std::map<std::string, std::string> _attributes;
	std::vector<CNtlSimpleDOM> _children;

	void clear();
	bool hasAttribute(const std::string& attribute) const;
	std::string getAttribute(const std::string& attribute) const;

	long attributeAsLong(const std::string& attribute) const;
	long attributeAsLongWithDefault(const std::string& attribute, long defaultValue) const;
	float attributeAsFloat(const std::string& attribute) const;
	float attributeAsFloatWithDefault(const std::string& attribute, float defaultValue) const;
	bool attributeAsBool(const std::string& attribute) const;
	bool attributeAsBoolWithDefault(const std::string& attribute, bool defaultValue) const;

	long firstChildWithName(const std::string& name) const;
	CNtlSimpleDOM& refFirstChildWithName(const std::string& name);
	const CNtlSimpleDOM& refFirstChildWithName(const std::string& name) const;

	// Utility
public:
	static void ReadToken(std::istream& is, std::string& token);
	static void LoadElement(const std::string& element, std::istream& is, CNtlSimpleDOM& result);
	static void LoadWhiteSpaceDelimited(std::istream& is, CNtlSimpleDOM& result);
};


#endif