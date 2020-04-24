
#ifndef __NTL_PLOBJECT_PROPERTY_H__
#define __NTL_PLOBJECT_PROPERTY_H__

#include <string>
#include "NtlPLProperty.h"

class CNtlPLLightProperty : public CNtlPLProperty
{
	public:

		static unsigned int				m_strVer;

	public:

										CNtlPLLightProperty();
		virtual							~CNtlPLLightProperty();

		virtual RwBool					Load( CNtlXMLDoc * pDoc, IXMLDOMNode * pNode );
		virtual RwBool					Save( CNtlXMLDoc * pDoc, IXMLDOMNode * pNode );
};

#endif //__NTL_PLOBJECT_PROPERTY_H__
