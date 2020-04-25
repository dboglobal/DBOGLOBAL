#include "gui_precomp.h"
#include "gui_resourcefilepaser.h"
#include "gui_componenttokenizer.h"
#include "gui_componenttype.h"

START_GUI

CResourceFileParser::CResourceFileParser(const std::string &filename)
{
	CComponentTokenizer lexer(filename);
	if(!lexer.IsSuccess())
	{
		m_strError = "["+filename+"]"+" exist not file";
		return;
	}
	
	std::string strError;
	typedef std::pair<std::string,ComponentInfo> stlNAMED_COMPONENT_PAIR;
	std::stack<stlNAMED_COMPONENT_PAIR> stlCompStack;
	std::map<std::string, int> stlCompEnumerator;

	enum EParseState
	{
		PARSE_GLOBAL,						// 전체 frame
		PARSE_RESOURCE_TYPE,    // component type
		PARSE_RESOURCE_NAME,    // component name
		PARSE_RESOURCE,         // component 
		PARSE_VARIABLE_NAME,    // variable name
		PARSE_VARIABLE_VALUE    // variable value
	};

	EParseState State = PARSE_GLOBAL;
	EParseState PrevState = PARSE_GLOBAL;
	std::string strComponentType;
	std::string strComponentName;
	std::string strVariableName;
	std::list<std::string> stlVariableValue;
	std::string strToplevelStyle = "";

	while (true)
	{
		// determine current state by peeking the next token in the input stream
		const std::string &token = lexer.PeekNextToken();
		if (token == "") 
			break;

		CComponentType *pCompType = NULL;
		switch (State)
		{
		// We're in the global scope of the .gui file. 
		// Look for 'style', and component declarations
		case PARSE_GLOBAL:
			if (token == "style")
			{
				if (lexer.PeekNextToken() != "=")
				{
					m_strError = "Missing '=' following global style-declaration";
					return; 
				}
				PrevState = PARSE_GLOBAL;
				State = PARSE_VARIABLE_VALUE;
				strVariableName = token;
			}
			else 
			{
				// an existing component type is being declared.
				State = PARSE_RESOURCE_TYPE;
				strComponentType = token;
				strComponentName = "";

				ComponentInfo Info;
				Info.type = token;
				stlCompStack.push(stlNAMED_COMPONENT_PAIR("", Info));
			}
			break;
		case PARSE_RESOURCE_TYPE:
			if (token == "{")
			{
				// no-named component, create default name (button01 etc)
				int nNextID = stlCompEnumerator[strComponentType]++;
				char buf[20];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				sprintf_s(buf, 20, "%d", nNextID);
#else
				sprintf(buf, "%d", nNextID);
#endif
				if (strlen(buf) == 1) strComponentName = strComponentType+"0"+buf;
				else strComponentName = strComponentType+buf;
				stlCompStack.top().first = strComponentName;

				State = PARSE_RESOURCE;
			}
			else
			{
				// register component name
				if(m_stlCompMap.find(token) != m_stlCompMap.end())
				{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
					char buf[1024];
					sprintf_s(buf, 1024, "Error ==>  gui resource file에 동일한 이름 존재. [ %s ]",token.c_str());
#else
					sprintf(buf, "Error ==>  gui resource file에 동일한 이름 존재. [ %s ]",token.c_str());
#endif
					m_strError = buf;
					return;
				}
				strComponentName = token;
				stlCompStack.top().first = strComponentName;
				State = PARSE_RESOURCE_NAME;
			}
			break;
		case PARSE_RESOURCE_NAME:
			if (token != "{")
			{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				char buf[1024];
				sprintf_s(buf, 1024, "Missing '{' following declaration of component '%s'", strComponentName.c_str());
#else
				char buf[1024];
				sprintf(buf, "Missing '{' following declaration of component '%s'", strComponentName.c_str());
#endif
				m_strError = buf;
				return;
			}
			State = PARSE_RESOURCE;
			break;
		case PARSE_RESOURCE:
			if (token == "}")
			{
				// declaration of current component is finished

				if (stlCompStack.empty())
				{
					m_strError = "Found '}' outside component";
					return;
				}
				stlNAMED_COMPONENT_PAIR comp = stlCompStack.top();
				stlCompStack.pop();

				// assign name to component info.
				comp.second.name = comp.first;

				// insert created component into component map
				std::pair<std::map<std::string, ComponentInfo>::iterator, bool> it = 
					m_stlCompMap.insert(
						std::make_pair(comp.first, comp.second));
				
				if (!it.second)
				{
					m_strError = "All component names must be unique";
					return; 
				}

				// This creation map is used to make sure that components get
				// pointers to their parent upon creation.
				// This map controls the ordering of component-creations using
				// component-nesting level as sort criteria.
				m_stlCompSortMap.insert(std::make_pair((const int)stlCompStack.size(),	&(*it.first).second));

				if (!stlCompStack.empty())
				{
					// the component being finalized is the child of another component
					// add finalized component to parent's child list.
					// frame component에 button등의 component를 child로 등록한다...
					stlNAMED_COMPONENT_PAIR &parent_comp = stlCompStack.top();
					parent_comp.second.m_stlChildren.push_back(&(*it.first).second);
				}
				else
				{
					// parsing of a global component is finished - return to global parse state
					State = PARSE_GLOBAL;
				}
			}
			else if (token.length() == 1 && lexer.IsOperator(token[0]))
			{
				m_strError = "All component names must be unique";
				return;
			}
			else
			{
				// we've entered the declaration of a component option
				PrevState = PARSE_RESOURCE;
				State = PARSE_VARIABLE_NAME;
				strVariableName = token;
				stlVariableValue.clear();
			}
			break;
		case PARSE_VARIABLE_NAME:
			if (token == "=")
			{
				// a component option with a value is being declared
				State = PARSE_VARIABLE_VALUE;
			}
			else if (token == ";")
			{
				stlCompStack.top().second.Options.m_stlOptions.insert(std::make_pair(strVariableName, ""));
				strVariableName = "";
				stlVariableValue.clear();
				State = PrevState;
			}
			else
			{
				m_strError = "Expected ';' following variable declaration";
				return; 
			}
			break;
		case PARSE_VARIABLE_VALUE:
			if (token != ";")
			{
				if(token == "=")
				{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
					char buf[1024];
					sprintf_s(buf, 1024, "Error ==>  ; missing [ resource name = %s , resource variable = %s ]",strComponentName.c_str(),strVariableName.c_str());
#else
					char buf[1024];
					sprintf(buf, "Error ==>  ; missing [ resource name = %s , resource variable = %s ]",strComponentName.c_str(),strVariableName.c_str());
#endif
					m_strError = buf;
					return;
				}
				else
					stlVariableValue.push_back(token);
			}
			else
			{
				if (stlCompStack.empty())
				{
					// the value declared is not a component option, but a global option value
					m_stlGlobals.push_back(stlVARIABLE_PAIR(strVariableName, EvaluateExpression(stlVariableValue)));
				}
				else
				{
					// add option to current component's option list
					stlCompStack.top().second.Options.m_stlOptions.insert(std::make_pair(strVariableName, EvaluateExpression(stlVariableValue)));
				}
				strVariableName = "";
				stlVariableValue.clear();
				State = PrevState;
			}
			break;
		}

		lexer.PopToPeek();
	}
}

std::string CResourceFileParser::EvaluateExpression(std::list<std::string> &expression)
{
	bool is_expression = true;
	std::string result;
	std::list<std::string>::iterator it = expression.begin();
	for (;it!=expression.end();it++)
	{
		std::string &s = *it;
		if (s == "+" || s == "-" || s == "*" || s == "/" || s == "(" || s == ")")
		{
			continue;
		}
		const char *str = s.c_str();
		for (;*str;str++)
		{
			if ((*str < '0' || *str > '9') && *str != '.')
			{
				break;
			}
		}
		if (*str)
		{
			is_expression = false;
			break;
		}
	}

	if (!is_expression)
	{
		return ConcatenateStringlist(expression);
	}

	/*
		Please note:
		This expression evaluation function is NOT FINISHED!
		This is a totally basic no-operator-precedence implementation
		that disregards paranthesis and just plain sucks, but 
		I don't have the willpower to complete it - someone else feel free :-)
	*/

	int op = 0;
	float result_val = 0.0f;
	for (it=expression.begin();it!=expression.end();it++)
	{
		std::string &s = *it;
		if (s == "+") op = 0;
		else if (s == "-") op = 1;
		else if (s == "*") op = 2;
		else if (s == "/") op = 3;
		else if (s == "(" || s == ")")
		{
			continue;
		}
		else
		{
			if (op == -1) 
			{
				assert(0 && "Could not evaluate expresstion");
			}
			float val = (float)atof(s.c_str());
			switch (op)
			{
			case 0:
				result_val += val;
				break;
			case 1:
				result_val -= val;
				break;
			case 2:
				result_val *= val;
				break;
			case 3:
				result_val /= val;
				break;
			default:
				result_val = val;
				break;
			}
			op = -1;
		}
	}


	std::string str;
	char buf[20];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(buf, 20, "%.2f",result_val);
#else
	sprintf(buf, "%.2f",result_val);
#endif

	//	int nRVal = (int)(result_val+0.5);
	//
	//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	//	sprintf_s(buf, 20, "%d",nRVal);
	//#else
	//	sprintf(buf, "%d",nRVal);
	//#endif
	str = buf;
	return str;
}

std::string CResourceFileParser::ConcatenateStringlist(std::list<std::string> &expression)
{
	std::string result;
	int size_list = (int)expression.size();
	std::list<std::string>::iterator it;
	for (it=expression.begin();it!=expression.end();it++)
	{
		result.append(*it);
		size_list--;
		if (size_list > 0) result.append(1, ' ');
	}
	return result;
}

VOID CResourceFileParser::ReadError(std::string& strError)
{
	strError = m_strError;
}


END_GUI