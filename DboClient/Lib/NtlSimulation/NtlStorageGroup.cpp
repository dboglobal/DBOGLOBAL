// Precompile
#include "precomp_ntlsimulation.h"
#include "NtlStorageGroup.h"

// Storage
#include "NtlStorageMTContainer.h"
#include "NtlStorageTextSerializer.h"
#include "NtlStorageBinarySerializer.h"

//
#include "NtlTokenizer.h"

#include "NtlDebug.h"

CNtlStorageGroup::CNtlStorageGroup()
{

}

CNtlStorageGroup::~CNtlStorageGroup()
{

}

bool CNtlStorageGroup::Create()
{
	return true;
}

void CNtlStorageGroup::Destroy()
{
	for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
	{
		if( pair.second )
		{
			pair.second->Destroy();

			NTL_DELETE( pair.second );
		}
	}

	m_mapStorageUnit.clear();
}

/**
* \brief NtlStorageGroup의 개념적인 Base load 함수
* 특정한 로딩을 해야 하는 Group이 생길 경우는 이 Class를 상속받는
* 자식 클래스에서 오버라이딩해서 사용을 한다.
* \param pS		(CNtlStorageSerializer*) Load에 사용될 Serializer buffer
* \return 성공여부
*/
bool CNtlStorageGroup::Load(CNtlStorageSerializer* pS)
{
	if( eNTL_STORAGE_SERIALIZER_TEXT == pS->GetStorageSerializerType() )
	{
		CNtlTokenizer lexer( pS->GetData() );
		if( !lexer.IsSuccess() )
			return false;

		// Text Parser

		// [카테고리]
		// [KEY] = [VALUE];
		
		eNTL_STORAGE_TYPE eCurrentType = eNTL_STORAGE_INVALID;
		
		enum eTextState
		{
			eTEXT_KEY = 0,
			eTEXT_EQUAL,
			eTEXT_VALUE,
			eTEXT_SEMICOLON
		};

		eTextState eState = eTEXT_KEY;
		std::string strKey, strValue;
		for(;;)
		{
			std::string& token = lexer.PeekNextToken();

			// 현재 토큰이 카테고리인지 확인하고 카테고리이면 다음 값은 키 -> = -> 값 -> ; 으로 데이터를 확인한다.
			for( int i=0; i < eNTL_STORAGE_NUMS; ++i )
			{
				if( token.compare( g_acNtlStorageTypeString[i] ) == 0 )
				{
					eState = eTEXT_KEY;
					eCurrentType = (eNTL_STORAGE_TYPE)i;
					token = lexer.PeekNextToken();
					break;
				}
			}

			if( token.empty() )
				break;

			switch( eState )
			{
			case eTEXT_KEY:
				{
					if( token == "=" ||
						token == ";" )
					{
						return false;
					}
					else
					{
						strKey = token;
						eState = eTEXT_EQUAL;
					}
				}
				break;
			case eTEXT_EQUAL:
				{
					if( token != "=" )
					{
						return false;
					}
					else
					{
						eState = eTEXT_VALUE;
					}
				}
				break;
			case eTEXT_VALUE:
				{
					if( token == "=" ||
						token == ";" )
					{
						return false;
					}
					else
					{
						strValue = token;
						eState = eTEXT_SEMICOLON;
					}
				}
				break;
			case eTEXT_SEMICOLON:
				{
					if( token != ";" )
					{
						return false;
					}
					else
					{
						// eTEXT_SEMICOLON 이 확인되면 저장하고 있는 Key와 Value를 현재의 Current Type에 맞는
						// Unit에 넣어준다. 만약 eCurrentType의 현재의 Group에 포함되지 않은 Unit이라면
						// 오류를 발생하고 return 한다. 그렇게 되면 현재까지 저장한 Key Value를 제외하고 이외의
						// 항목들은 다 Default로 남아 있게 된다.
						UNITMAP::iterator it = m_mapStorageUnit.find( eCurrentType );
						if( it == m_mapStorageUnit.end() )
							return false;

						// 현재 들어온 KEY값이 MappingTable에 존재하지 않는 경우, 현재의 라인을날려버린다. 그렇게 되면 잘못된
						// KEY값의 항목만 저장이 되지 않고 나머지는 정상적으로 처리한다.
						unsigned int uiKey = 0;
						if( !GetNtlStorageMTContainer()->GetKeyFromString( strKey, uiKey ) )
						{
							eState = eTEXT_KEY;
							continue;
						}
						else
						{
							(*it).second->SetData( uiKey, strValue );
						}

						eState = eTEXT_KEY;
					}
				}
				break;
			default:
				break;
			}
		}

		return true;
	}
	else if( eNTL_STORAGE_SERIALIZER_BINARY == pS->GetStorageSerializerType() )
	{
		// Binary는 각각의 알맞은 키를 가져와야함.
		/*CNtlStorageBinarySerializer* pBianrySerializer = (CNtlStorageBinarySerializer*)pS;*/

		unsigned int uiKey;
		eNTL_STORAGE_TYPE eCurrentType = eNTL_STORAGE_INVALID;

		int nCount = 0;
		RwBool bLoaded = FALSE;
		while(!bLoaded)
		{
			// 무한루프 방지
			if( ++nCount > 1000000 )
				break;

			if( pS->IsEmpty() )
				break;

			(*pS) >> uiKey;
			switch( uiKey )
			{
				// Case By Case
			case dSTORAGE_CATEGORY:
				{
					// 카테고리에 알맞은 유닛을 만든다.
					std::string strCategory;
					(*pS) >> strCategory;

					// 어떠한 카테고리인지 확인
					eNTL_STORAGE_TYPE eType = eNTL_STORAGE_INVALID;;
					for( int i=0; i < (int)eNTL_STORAGE_NUMS; ++i )
					{
						if( strCategory.compare( g_acNtlStorageTypeString[i] ) == 0 )
						{
							eType = (eNTL_STORAGE_TYPE)i;

							break;
						}
					}

					if( eType == eNTL_STORAGE_INVALID )
					{
						// 정의되어 있는 카테고리를 찾지 못했으면 데이타가 잘못된 것이다.
						return false;
					}

					// If it does not, overwrite it if you are making it.
					UNITMAP::iterator it = m_mapStorageUnit.find( eType );
					if( it == m_mapStorageUnit.end() )
					{
						CNtlStorageUnit* pUnit = NTL_NEW CNtlStorageUnit();
						if( !pUnit->Create( eType ) )
						{
							return false;
						}
					}

					eCurrentType = eType;					
				}
				break;
			case dSTORAGE_EOF:
				{
					// 정상적으로 로딩이 완료
					bLoaded = TRUE;
				}
				break;
			default:
				{
					// 카테고리가 아닌 것은 모두 다 uiKey에 따른 데이타이다.
					// 현재 커렌트 타입을 가져와서
					UNITMAP::iterator it = m_mapStorageUnit.find( eCurrentType );
					if( it == m_mapStorageUnit.end() )
					{
						return false;
					}

					std::string strData;
					(*pS) >> strData;
					(*it).second->SetData( uiKey, strData );
				}
				break;

			}
		}

		// ByNarySerializer에 다시 Data를 넣어줄 필요가 없다.
		/*for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		{
			if( pair.second )
			{
				(*pBianrySerializer) << (unsigned int)dSTORAGE_CATEGORY;
				(*pBianrySerializer) << (std::string) g_acNtlStorageTypeString[pair.second->GetType()];

				for each( std::pair< unsigned int, std::string > pairValue in pair.second->m_mapStorageData )
				{
					(*pBianrySerializer) << (std::string)GetNtlStorageMTContainer()->GetString( pairValue.first );
					(*pBianrySerializer) << (std::string)pairValue.second;
				}			
			}
		}
		*/

		return true;
	}

	return false;
}

/**
* \brief NtlStorageGroup의 개념적인 Base save 함수
* 특별한 저장을 해야 하는 Group이 생길 경우는 이 Class를 상속받는
* 자식 클래스에서 오버라이딩해서 사용을 한다.
* \param pS		(CNtlStorageSerializer*) save에 사용될 Serializer buffer
* \return 성공여부
*/
bool CNtlStorageGroup::Save(CNtlStorageSerializer* pS)
{
	// Storage
	if( eNTL_STORAGE_SERIALIZER_TEXT == pS->GetStorageSerializerType() )
	{
		//CNtlStorageTextSerializer pSerializer = (CNtlStorageTextSerializer)s;

		CNtlStorageTextSerializer* pTextSerializer = (CNtlStorageTextSerializer*)pS;

		for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		{
			(*pTextSerializer) << g_acNtlStorageTypeString[pair.second->GetType()];
			(*pTextSerializer) << "\n";

			if( pair.second )
			{
				for each( std::pair< unsigned int, std::string > pairValue in pair.second->m_mapStorageData )
				{
					(*pTextSerializer) << (std::string)GetNtlStorageMTContainer()->GetString( pairValue.first );
					(*pTextSerializer) << " = ";
					(*pTextSerializer) << (std::string)pairValue.second;	// Data ( std::string )
					(*pTextSerializer) << ";\n";
				}			
			}

			(*pTextSerializer) << "\0";
		}

		return true;
	}
	else if( eNTL_STORAGE_SERIALIZER_BINARY == pS->GetStorageSerializerType() )
	{
		CNtlStorageBinarySerializer* pBianrySerializer = (CNtlStorageBinarySerializer*)pS;

		for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		{
			if( pair.second )
			{
				(*pBianrySerializer) << (unsigned int)dSTORAGE_CATEGORY;
				(*pBianrySerializer) << (std::string) g_acNtlStorageTypeString[pair.second->GetType()];

				for each( std::pair< unsigned int, std::string > pairValue in pair.second->m_mapStorageData )
				{
					(*pBianrySerializer) << (unsigned int)pairValue.first;
					(*pBianrySerializer) << (std::string)pairValue.second;
				}			
			}
		}

		(*pBianrySerializer) << (unsigned int)dSTORAGE_EOF;

		return true;
	}

	return false;
}

bool CNtlStorageGroup::Reset()
{
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->Reset() )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, int nData)
{
	std::ostringstream outstream;
	outstream << nData;
	std::string strData = outstream.str();

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, float fData)
{
	std::ostringstream outstream;
	outstream << fData;
	std::string strData = outstream.str();

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, std::string strData)
{
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, bool bData)
{
	std::string strData;
	if( bData )
		strData = dSTORAGE_TRUE;
	else
		strData = dSTORAGE_FALSE;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData( unsigned int uiKey, const char* acData )
{
	std::string strData( acData );
	
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::GetIntData(unsigned int uiKey, int& nData)
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			nData = atoi(strOut.c_str());
			return true;
		}
	

	return false;
}

bool CNtlStorageGroup::GetStringData(unsigned int uiKey, std::string& strData)
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			strData = strOut.c_str();
			return true;
		}

	return false;
}


bool CNtlStorageGroup::GetBoolData(unsigned int uiKey, bool& bData)
{
	std::string strOut;

	for each(std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit)
	{
		if (pair.second->GetData(uiKey, strOut))
		{
			if (strOut.compare(dSTORAGE_TRUE) == 0)
				bData = true;
			else
				bData = false;

			return true;
		}
	}

	return false;
}

bool CNtlStorageGroup::GetFloatData(unsigned int uiKey, float& fData)
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			fData = (float)atof( strOut.c_str() );
			return true;
		}

	return false;
}


void CNtlStorageGroup::UnitCheck()
{
	for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		pair.second->UnitCheck();
}

void CNtlStorageGroup::SetStorageGroupType( eNTL_STORAGE_GROUP_TYPE eType )
{
	m_eGroupType = eType;
}

eNTL_STORAGE_GROUP_TYPE CNtlStorageGroup::GetStorageGroupType()
{
	return m_eGroupType;
}

int CNtlStorageGroup::GetIntData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			return atoi(strOut.c_str());
		}

	return 0;
}

bool CNtlStorageGroup::GetBoolData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			if( strOut.compare( dSTORAGE_TRUE ) == 0 )
				return true;
			else
				return false;
		}

	return false;
}

float CNtlStorageGroup::GetFloatData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			return (float)atof( strOut.c_str() );
		}

	return 0.0f;
}

std::string CNtlStorageGroup::GetStringData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			return strOut;
		}

	return strOut;
}

bool CNtlStorageGroup::IsHave( eNTL_STORAGE_TYPE eType )
{
	UNITMAP::iterator it = m_mapStorageUnit.find( eType );
	if( it == m_mapStorageUnit.end() )
	{
		return false;
	}

	return true;
}

CNtlStorageUnit* CNtlStorageGroup::GetStorageUnit( eNTL_STORAGE_TYPE eType )
{
	UNITMAP::iterator it = m_mapStorageUnit.find( eType );
	if( it == m_mapStorageUnit.end() )
	{
		return NULL;
	}

	return (*it).second;
}

bool CNtlStorageGroup::SetUnitApplyFunc( NtlStorageUnitApplyFunc func )
{
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		pair.second->SetApplyFunc( func );

	return true;
}