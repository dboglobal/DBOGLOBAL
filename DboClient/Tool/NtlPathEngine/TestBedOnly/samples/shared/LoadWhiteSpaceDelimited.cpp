
#pragma warning(disable : 4786)

#include "LoadWhiteSpaceDelimited.h"
#include "Error.h"
#include "SimpleDOM.h"
#include <istream>

using std::string;

static void
ReadToken(std::istream& is, std::string& token)
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

static void
LoadElement(const std::string& element, std::istream& is, cSimpleDOM& result)
{
    assertR(element.size() > 1)
    result._name = element.c_str() + 1;
    while(1)
    {
        string token;
        ReadToken(is, token);
        assertR(token.size() >= 1)
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
                    Error("Fatal", "Parsing white space delimited script - closing tag does not agree.", attributes);
                }
            }
            return;
        }
        if(token[0] == '>')
        {
            result._children.push_back(cSimpleDOM());
            LoadElement(token, is, result._children.back());
        }
        else
        {
            assertR(token[0] == '.');
            assertR(token.size() > 1)
            string attribute = token.c_str() + 1;
            string value;
            ReadToken(is, value);
            assertR(!value.empty());
            if(value[value.size() - 1] == '<')
            {
                const char* attributes[3];
                attributes[0] = "value";
                attributes[1] = value.c_str();
                attributes[2] = 0;
                Error("Warning", "Value ends in '<' character, is this intended?", attributes);
            }
            result._attributes[attribute] = value;
        }
    }
}

void 
LoadWhiteSpaceDelimited(std::istream& is, cSimpleDOM& result)
{
    result.clear();
    string token;
    ReadToken(is, token);
    assertR(token.size() > 1);
    assertR(token[0] == '>');
    LoadElement(token, is, result);
}
