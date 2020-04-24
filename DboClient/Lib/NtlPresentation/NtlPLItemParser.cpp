#include "precomp_ntlpresentation.h"
#include "NtlPLItemParser.h"
#include "NtlDebug.h"
#include "NtlTokenizer.h"
#include "NtlPLItem.h"
#include "NtlPLPropertyContainer.h"


CNtlPLItemParser::CNtlPLItemParser()
{
    CreateKeyword();
}

CNtlPLItemParser &CNtlPLItemParser::GetInstance()
{
	static CNtlPLItemParser	s_PLItemParser;
	return s_PLItemParser;
}

void CNtlPLItemParser::CreateKeyword() 
{
#define ITEM_TABLE_ADD(id) m_ItemMatchTable.Add(#id, id)

 
   ITEM_TABLE_ADD(ITEM_IDLE);
   ITEM_TABLE_ADD(ITEM_ATTACK);
}