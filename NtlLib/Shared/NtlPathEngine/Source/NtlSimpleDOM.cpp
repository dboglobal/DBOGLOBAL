#include "Precomp_NtlPathEngine.h"
#include "NtlSimpleDOM.h"

#pragma warning(disable : 4786)

#include <stdlib.h>

#include "NtlPathEngineLog.h"

using std::string;

void
CNtlSimpleDOM::clear()
{
	_name = "";
	_attributes.clear();
	_children.clear();
}

bool
CNtlSimpleDOM::hasAttribute(const std::string& attribute) const
{
	return _attributes.find(attribute) != _attributes.end();
}

std::string
CNtlSimpleDOM::getAttribute(const std::string& attribute) const
{
	std::map<std::string, std::string>::const_iterator i = _attributes.find(attribute);
	if(i == _attributes.end())
	{
		return "";
	}
	return i->second;
}

long
CNtlSimpleDOM::attributeAsLong(const std::string& attribute) const
{
	string value = getAttribute(attribute);
	//assertD(!value.empty());
	char* ptr;
	long result = strtol(value.c_str(), &ptr, 10);
	/*assertD(*ptr == 0);*/
	return result;
}
long
CNtlSimpleDOM::attributeAsLongWithDefault(const std::string& attribute, long defaultValue) const
{
	if(!hasAttribute(attribute))
	{
		return defaultValue;
	}
	string value = getAttribute(attribute);
	/*assertD(!value.empty());*/
	char* ptr;
	long result = strtol(value.c_str(), &ptr, 10);
	/*assertD(*ptr == 0);*/
	return result;
}
float
CNtlSimpleDOM::attributeAsFloat(const std::string& attribute) const
{
	string value = getAttribute(attribute);
	/*assertD(!value.empty());*/
	char* ptr;
	float result = static_cast<float>(strtod(value.c_str(), &ptr));
	/*assertD(*ptr == 0);*/
	return result;
}
float
CNtlSimpleDOM::attributeAsFloatWithDefault(const std::string& attribute, float defaultValue) const
{
	if(!hasAttribute(attribute))
	{
		return defaultValue;
	}
	string value = getAttribute(attribute);
	/*assertD(!value.empty());*/
	char* ptr;
	float result = static_cast<float>(strtod(value.c_str(), &ptr));
	/*assertD(*ptr == 0);*/
	return result;
}
bool
CNtlSimpleDOM::attributeAsBool(const std::string& attribute) const
{
	string value = getAttribute(attribute);
	if(value == "true")
	{
		return true;
	}
	/*assertD(value == "false");*/
	return false;
}
bool
CNtlSimpleDOM::attributeAsBoolWithDefault(const std::string& attribute, bool defaultValue) const
{
	if(!hasAttribute(attribute))
	{
		return defaultValue;
	}
	return attributeAsBool(attribute);
}

long
CNtlSimpleDOM::firstChildWithName(const std::string& name) const
{
	size_t i;
	for(i = 0; i < _children.size(); i++)
	{
		if(_children[i]._name == name)
		{
			return (long)i;
		}
	}
	return -1;
}

CNtlSimpleDOM&
CNtlSimpleDOM::refFirstChildWithName(const std::string& name)
{
	long i = firstChildWithName(name);
	/*assertD(i >= 0);*/
	return _children[i];
}

const CNtlSimpleDOM&
CNtlSimpleDOM::refFirstChildWithName(const std::string& name) const
{
	long i = firstChildWithName(name);
	/*assertD(i >= 0);*/
	return _children[i];
}

//////////////////////////////////////////////////////////////////////////
// CNtlSimpleDOM static function
//////////////////////////////////////////////////////////////////////////

void CNtlSimpleDOM::ReadToken( std::istream& is, std::string& token )
{
	is >> token;
	while(token == "[#")
	{
		// token is a comment start
		// skip tokens until comment end
		do
		{
			is >> token;
		}
		while(token != "#]");

		// then read the first token after the comment end
		// (which may in turn be another commend start)
		is >> token;
	}
}

void CNtlSimpleDOM::LoadElement( const std::string& element, std::istream& is, CNtlSimpleDOM& result )
{
	/*assertR(element.size() > 1)*/
		result._name = element.c_str() + 1;
	while(1)
	{
		string token;
		ReadToken(is, token);
		/*assertR(token.size() >= 1)*/
			if(token[0] == '<')
			{
				if(token.size() > 1)
				{
					if(result._name != token.c_str() + 1)
					{
						const char* attributes[3];
						attributes[0] = "closing_tag";
						attributes[1] = token.c_str() + 1;
						attributes[2] = 0;
						CNtlPathEngineLog::GetInstance()->handle( "Fatal", "Parsing white space delimited script - closing tag does not agree.", attributes );
					}
				}
				return;
			}
			if(token[0] == '>')
			{
				result._children.push_back(CNtlSimpleDOM());
				LoadElement(token, is, result._children.back());
			}
			else
			{
			/*	assertR(token[0] == '.');
				assertR(token.size() > 1)*/
					string attribute = token.c_str() + 1;
				string value;
				ReadToken(is, value);
				/*assertR(!value.empty());*/
				if(value[value.size() - 1] == '<')
				{
					const char* attributes[3];
					attributes[0] = "value";
					attributes[1] = value.c_str();
					attributes[2] = 0;
					CNtlPathEngineLog::GetInstance()->handle("Warning", "Value ends in '<' character, is this intended?", attributes);
				}
				result._attributes[attribute] = value;
			}
	}
}

void CNtlSimpleDOM::LoadWhiteSpaceDelimited( std::istream& is, CNtlSimpleDOM& result )
{
	result.clear();
	string token;
	ReadToken(is, token);
	/*assertR(token.size() > 1);
	assertR(token[0] == '>');*/
	LoadElement(token, is, result);
}