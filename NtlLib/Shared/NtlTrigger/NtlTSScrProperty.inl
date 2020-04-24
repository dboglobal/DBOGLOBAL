

/** 
	Script property
*/


inline CNtlTSScrProperty::PROP_IT CNtlTSScrProperty::Begin( void )
{
	return m_defProperty.begin();
}

inline CNtlTSScrProperty::PROP_IT	CNtlTSScrProperty::End( void )
{
	return m_defProperty.end();
}

inline CNtlTSScrProperty::PROP_CIT CNtlTSScrProperty::Begin( void ) const
{
	return m_defProperty.begin();
}

inline CNtlTSScrProperty::PROP_CIT CNtlTSScrProperty::End( void ) const
{
	return m_defProperty.end();
}

inline bool CNtlTSScrProperty::IsExist( const std::string& strPropertyName ) const
{
	return ( m_defProperty.find( strPropertyName ) != m_defProperty.end() );
}

inline int CNtlTSScrProperty::GetCount( const std::string& strPropertyName ) const
{
	return (int)m_defProperty.count( strPropertyName );
}

inline const std::string& CNtlTSScrProperty::GetValue( const std::string& strPropertyName, int nOffset ) const
{ 
	static const std::string strBlank;

	PROP_CIT cIt = m_defProperty.find( strPropertyName );

	if ( cIt == m_defProperty.end() )
	{
		printf( "Can not seek this property name. Info[%s]. [%s]", strPropertyName.c_str(), TS_CODE_TRACE() );
		return strBlank;
	}

	for ( ; nOffset > 0; --nOffset )
	{
		++cIt;
		if ( cIt == m_defProperty.end() )
		{
			printf( "Can not seek this entity property. Info[%s]. [%s]", strPropertyName.c_str(), TS_CODE_TRACE() );
			return strBlank;
		}
	}

	return (*cIt).second; 
}

inline int CNtlTSScrProperty::GetValueAsInt( const std::string& strPropertyName, int nOffset ) const
{
	const std::string& val = GetValue( strPropertyName, nOffset );
	return atoi( val.c_str() );
}


inline bool CNtlTSScrProperty::GetValueAsBool( const std::string& strPropertyName, int nOffset ) const
{
	const std::string &val = GetValue( strPropertyName, nOffset );

	if ( val == "yes" || val == "true" || val == "on" || val == "1" )
	{
		return true;
	}
	else if ( val == "no" || val == "false" || val == "off" || val == "0" )
	{
		return false;
	}

	return false;
}

inline void CNtlTSScrProperty::AddProperty( const std::string& strPropertyName, const std::string& strPropertyValue )
{
	m_defProperty.insert( hashdef_PROPERTY::value_type( strPropertyName, strPropertyValue ) );
}

inline void CNtlTSScrProperty::RemoveProperty( const std::string& strPropertyName )
{
	PROP_IT it = m_defProperty.find( strPropertyName );
	while ( it != m_defProperty.end() )
	{
		if ( it->first != strPropertyName ) break;

		PROP_IT it2 = it;
		++it;

		m_defProperty.erase( it2 );
	}
}

inline void CNtlTSScrProperty::Clear( void )
{
	m_defProperty.clear();
}

inline const std::string& CNtlTSScrProperty::operator[]( const std::string& strPropertyName ) const
{
	return GetValue( strPropertyName, 0 );
}
