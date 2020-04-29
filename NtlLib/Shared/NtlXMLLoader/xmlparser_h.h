

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for MSXML Header Files\xmlparser.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __xmlparser_h_h__
#define __xmlparser_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IXMLNodeSource_FWD_DEFINED__
#define __IXMLNodeSource_FWD_DEFINED__
typedef interface IXMLNodeSource IXMLNodeSource;

#endif 	/* __IXMLNodeSource_FWD_DEFINED__ */


#ifndef __IXMLParser_FWD_DEFINED__
#define __IXMLParser_FWD_DEFINED__
typedef interface IXMLParser IXMLParser;

#endif 	/* __IXMLParser_FWD_DEFINED__ */


#ifndef __IXMLParser2_FWD_DEFINED__
#define __IXMLParser2_FWD_DEFINED__
typedef interface IXMLParser2 IXMLParser2;

#endif 	/* __IXMLParser2_FWD_DEFINED__ */


#ifndef __IXMLParser3_FWD_DEFINED__
#define __IXMLParser3_FWD_DEFINED__
typedef interface IXMLParser3 IXMLParser3;

#endif 	/* __IXMLParser3_FWD_DEFINED__ */


#ifndef __IXMLNodeFactory_FWD_DEFINED__
#define __IXMLNodeFactory_FWD_DEFINED__
typedef interface IXMLNodeFactory IXMLNodeFactory;

#endif 	/* __IXMLNodeFactory_FWD_DEFINED__ */


#ifndef __XMLParser_FWD_DEFINED__
#define __XMLParser_FWD_DEFINED__

#ifdef __cplusplus
typedef class XMLParser XMLParser;
#else
typedef struct XMLParser XMLParser;
#endif /* __cplusplus */

#endif 	/* __XMLParser_FWD_DEFINED__ */


#ifndef __XMLParser26_FWD_DEFINED__
#define __XMLParser26_FWD_DEFINED__

#ifdef __cplusplus
typedef class XMLParser26 XMLParser26;
#else
typedef struct XMLParser26 XMLParser26;
#endif /* __cplusplus */

#endif 	/* __XMLParser26_FWD_DEFINED__ */


#ifndef __XMLParser30_FWD_DEFINED__
#define __XMLParser30_FWD_DEFINED__

#ifdef __cplusplus
typedef class XMLParser30 XMLParser30;
#else
typedef struct XMLParser30 XMLParser30;
#endif /* __cplusplus */

#endif 	/* __XMLParser30_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_xmlparser_0000_0000 */
/* [local] */ 

//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1998.
//
//--------------------------------------------------------------------------




typedef /* [public] */ 
enum __MIDL___MIDL_itf_xmlparser_0000_0000_0001
    {
        XML_ELEMENT	= 1,
        XML_ATTRIBUTE	= ( XML_ELEMENT + 1 ) ,
        XML_PI	= ( XML_ATTRIBUTE + 1 ) ,
        XML_XMLDECL	= ( XML_PI + 1 ) ,
        XML_DOCTYPE	= ( XML_XMLDECL + 1 ) ,
        XML_DTDATTRIBUTE	= ( XML_DOCTYPE + 1 ) ,
        XML_ENTITYDECL	= ( XML_DTDATTRIBUTE + 1 ) ,
        XML_ELEMENTDECL	= ( XML_ENTITYDECL + 1 ) ,
        XML_ATTLISTDECL	= ( XML_ELEMENTDECL + 1 ) ,
        XML_NOTATION	= ( XML_ATTLISTDECL + 1 ) ,
        XML_GROUP	= ( XML_NOTATION + 1 ) ,
        XML_INCLUDESECT	= ( XML_GROUP + 1 ) ,
        XML_PCDATA	= ( XML_INCLUDESECT + 1 ) ,
        XML_CDATA	= ( XML_PCDATA + 1 ) ,
        XML_IGNORESECT	= ( XML_CDATA + 1 ) ,
        XML_COMMENT	= ( XML_IGNORESECT + 1 ) ,
        XML_ENTITYREF	= ( XML_COMMENT + 1 ) ,
        XML_WHITESPACE	= ( XML_ENTITYREF + 1 ) ,
        XML_NAME	= ( XML_WHITESPACE + 1 ) ,
        XML_NMTOKEN	= ( XML_NAME + 1 ) ,
        XML_STRING	= ( XML_NMTOKEN + 1 ) ,
        XML_PEREF	= ( XML_STRING + 1 ) ,
        XML_MODEL	= ( XML_PEREF + 1 ) ,
        XML_ATTDEF	= ( XML_MODEL + 1 ) ,
        XML_ATTTYPE	= ( XML_ATTDEF + 1 ) ,
        XML_ATTPRESENCE	= ( XML_ATTTYPE + 1 ) ,
        XML_DTDSUBSET	= ( XML_ATTPRESENCE + 1 ) ,
        XML_LASTNODETYPE	= ( XML_DTDSUBSET + 1 ) 
    } 	XML_NODE_TYPE;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_xmlparser_0000_0000_0002
    {
        XML_VERSION	= XML_LASTNODETYPE,
        XML_ENCODING	= ( XML_VERSION + 1 ) ,
        XML_STANDALONE	= ( XML_ENCODING + 1 ) ,
        XML_NS	= ( XML_STANDALONE + 1 ) ,
        XML_XMLSPACE	= ( XML_NS + 1 ) ,
        XML_XMLLANG	= ( XML_XMLSPACE + 1 ) ,
        XML_SYSTEM	= ( XML_XMLLANG + 1 ) ,
        XML_PUBLIC	= ( XML_SYSTEM + 1 ) ,
        XML_NDATA	= ( XML_PUBLIC + 1 ) ,
        XML_AT_CDATA	= ( XML_NDATA + 1 ) ,
        XML_AT_ID	= ( XML_AT_CDATA + 1 ) ,
        XML_AT_IDREF	= ( XML_AT_ID + 1 ) ,
        XML_AT_IDREFS	= ( XML_AT_IDREF + 1 ) ,
        XML_AT_ENTITY	= ( XML_AT_IDREFS + 1 ) ,
        XML_AT_ENTITIES	= ( XML_AT_ENTITY + 1 ) ,
        XML_AT_NMTOKEN	= ( XML_AT_ENTITIES + 1 ) ,
        XML_AT_NMTOKENS	= ( XML_AT_NMTOKEN + 1 ) ,
        XML_AT_NOTATION	= ( XML_AT_NMTOKENS + 1 ) ,
        XML_AT_REQUIRED	= ( XML_AT_NOTATION + 1 ) ,
        XML_AT_IMPLIED	= ( XML_AT_REQUIRED + 1 ) ,
        XML_AT_FIXED	= ( XML_AT_IMPLIED + 1 ) ,
        XML_PENTITYDECL	= ( XML_AT_FIXED + 1 ) ,
        XML_EMPTY	= ( XML_PENTITYDECL + 1 ) ,
        XML_ANY	= ( XML_EMPTY + 1 ) ,
        XML_MIXED	= ( XML_ANY + 1 ) ,
        XML_SEQUENCE	= ( XML_MIXED + 1 ) ,
        XML_CHOICE	= ( XML_SEQUENCE + 1 ) ,
        XML_STAR	= ( XML_CHOICE + 1 ) ,
        XML_PLUS	= ( XML_STAR + 1 ) ,
        XML_QUESTIONMARK	= ( XML_PLUS + 1 ) ,
        XML_LASTSUBNODETYPE	= ( XML_QUESTIONMARK + 1 ) 
    } 	XML_NODE_SUBTYPE;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_xmlparser_0000_0000_0003
    {
        XML_E_PARSEERRORBASE	= 0xc00ce500L,
        XML_E_ENDOFINPUT	= XML_E_PARSEERRORBASE,
        XML_E_MISSINGEQUALS	= ( XML_E_ENDOFINPUT + 1 ) ,
        XML_E_MISSINGQUOTE	= ( XML_E_MISSINGEQUALS + 1 ) ,
        XML_E_COMMENTSYNTAX	= ( XML_E_MISSINGQUOTE + 1 ) ,
        XML_E_BADSTARTNAMECHAR	= ( XML_E_COMMENTSYNTAX + 1 ) ,
        XML_E_BADNAMECHAR	= ( XML_E_BADSTARTNAMECHAR + 1 ) ,
        XML_E_BADCHARINSTRING	= ( XML_E_BADNAMECHAR + 1 ) ,
        XML_E_XMLDECLSYNTAX	= ( XML_E_BADCHARINSTRING + 1 ) ,
        XML_E_BADCHARDATA	= ( XML_E_XMLDECLSYNTAX + 1 ) ,
        XML_E_MISSINGWHITESPACE	= ( XML_E_BADCHARDATA + 1 ) ,
        XML_E_EXPECTINGTAGEND	= ( XML_E_MISSINGWHITESPACE + 1 ) ,
        XML_E_BADCHARINDTD	= ( XML_E_EXPECTINGTAGEND + 1 ) ,
        XML_E_BADCHARINDECL	= ( XML_E_BADCHARINDTD + 1 ) ,
        XML_E_MISSINGSEMICOLON	= ( XML_E_BADCHARINDECL + 1 ) ,
        XML_E_BADCHARINENTREF	= ( XML_E_MISSINGSEMICOLON + 1 ) ,
        XML_E_UNBALANCEDPAREN	= ( XML_E_BADCHARINENTREF + 1 ) ,
        XML_E_EXPECTINGOPENBRACKET	= ( XML_E_UNBALANCEDPAREN + 1 ) ,
        XML_E_BADENDCONDSECT	= ( XML_E_EXPECTINGOPENBRACKET + 1 ) ,
        XML_E_INTERNALERROR	= ( XML_E_BADENDCONDSECT + 1 ) ,
        XML_E_UNEXPECTED_WHITESPACE	= ( XML_E_INTERNALERROR + 1 ) ,
        XML_E_INCOMPLETE_ENCODING	= ( XML_E_UNEXPECTED_WHITESPACE + 1 ) ,
        XML_E_BADCHARINMIXEDMODEL	= ( XML_E_INCOMPLETE_ENCODING + 1 ) ,
        XML_E_MISSING_STAR	= ( XML_E_BADCHARINMIXEDMODEL + 1 ) ,
        XML_E_BADCHARINMODEL	= ( XML_E_MISSING_STAR + 1 ) ,
        XML_E_MISSING_PAREN	= ( XML_E_BADCHARINMODEL + 1 ) ,
        XML_E_BADCHARINENUMERATION	= ( XML_E_MISSING_PAREN + 1 ) ,
        XML_E_PIDECLSYNTAX	= ( XML_E_BADCHARINENUMERATION + 1 ) ,
        XML_E_EXPECTINGCLOSEQUOTE	= ( XML_E_PIDECLSYNTAX + 1 ) ,
        XML_E_MULTIPLE_COLONS	= ( XML_E_EXPECTINGCLOSEQUOTE + 1 ) ,
        XML_E_INVALID_DECIMAL	= ( XML_E_MULTIPLE_COLONS + 1 ) ,
        XML_E_INVALID_HEXIDECIMAL	= ( XML_E_INVALID_DECIMAL + 1 ) ,
        XML_E_INVALID_UNICODE	= ( XML_E_INVALID_HEXIDECIMAL + 1 ) ,
        XML_E_WHITESPACEORQUESTIONMARK	= ( XML_E_INVALID_UNICODE + 1 ) ,
        XML_E_ENDOFPE	= ( XML_E_WHITESPACEORQUESTIONMARK + 1 ) ,
        XML_E_TOKEN_ERROR	= ( XML_E_PARSEERRORBASE + 0x50 ) ,
        XML_E_SUSPENDED	= XML_E_TOKEN_ERROR,
        XML_E_STOPPED	= ( XML_E_SUSPENDED + 1 ) ,
        XML_E_UNEXPECTEDENDTAG	= ( XML_E_STOPPED + 1 ) ,
        XML_E_UNCLOSEDTAG	= ( XML_E_UNEXPECTEDENDTAG + 1 ) ,
        XML_E_DUPLICATEATTRIBUTE	= ( XML_E_UNCLOSEDTAG + 1 ) ,
        XML_E_MULTIPLEROOTS	= ( XML_E_DUPLICATEATTRIBUTE + 1 ) ,
        XML_E_INVALIDATROOTLEVEL	= ( XML_E_MULTIPLEROOTS + 1 ) ,
        XML_E_BADXMLDECL	= ( XML_E_INVALIDATROOTLEVEL + 1 ) ,
        XML_E_MISSINGROOT	= ( XML_E_BADXMLDECL + 1 ) ,
        XML_E_UNEXPECTEDEOF	= ( XML_E_MISSINGROOT + 1 ) ,
        XML_E_BADPEREFINSUBSET	= ( XML_E_UNEXPECTEDEOF + 1 ) ,
        XML_E_PE_NESTING	= ( XML_E_BADPEREFINSUBSET + 1 ) ,
        XML_E_INVALID_CDATACLOSINGTAG	= ( XML_E_PE_NESTING + 1 ) ,
        XML_E_UNCLOSEDPI	= ( XML_E_INVALID_CDATACLOSINGTAG + 1 ) ,
        XML_E_UNCLOSEDSTARTTAG	= ( XML_E_UNCLOSEDPI + 1 ) ,
        XML_E_UNCLOSEDENDTAG	= ( XML_E_UNCLOSEDSTARTTAG + 1 ) ,
        XML_E_UNCLOSEDSTRING	= ( XML_E_UNCLOSEDENDTAG + 1 ) ,
        XML_E_UNCLOSEDCOMMENT	= ( XML_E_UNCLOSEDSTRING + 1 ) ,
        XML_E_UNCLOSEDDECL	= ( XML_E_UNCLOSEDCOMMENT + 1 ) ,
        XML_E_UNCLOSEDMARKUPDECL	= ( XML_E_UNCLOSEDDECL + 1 ) ,
        XML_E_UNCLOSEDCDATA	= ( XML_E_UNCLOSEDMARKUPDECL + 1 ) ,
        XML_E_BADDECLNAME	= ( XML_E_UNCLOSEDCDATA + 1 ) ,
        XML_E_BADEXTERNALID	= ( XML_E_BADDECLNAME + 1 ) ,
        XML_E_BADELEMENTINDTD	= ( XML_E_BADEXTERNALID + 1 ) ,
        XML_E_RESERVEDNAMESPACE	= ( XML_E_BADELEMENTINDTD + 1 ) ,
        XML_E_EXPECTING_VERSION	= ( XML_E_RESERVEDNAMESPACE + 1 ) ,
        XML_E_EXPECTING_ENCODING	= ( XML_E_EXPECTING_VERSION + 1 ) ,
        XML_E_EXPECTING_NAME	= ( XML_E_EXPECTING_ENCODING + 1 ) ,
        XML_E_UNEXPECTED_ATTRIBUTE	= ( XML_E_EXPECTING_NAME + 1 ) ,
        XML_E_ENDTAGMISMATCH	= ( XML_E_UNEXPECTED_ATTRIBUTE + 1 ) ,
        XML_E_INVALIDENCODING	= ( XML_E_ENDTAGMISMATCH + 1 ) ,
        XML_E_INVALIDSWITCH	= ( XML_E_INVALIDENCODING + 1 ) ,
        XML_E_EXPECTING_NDATA	= ( XML_E_INVALIDSWITCH + 1 ) ,
        XML_E_INVALID_MODEL	= ( XML_E_EXPECTING_NDATA + 1 ) ,
        XML_E_INVALID_TYPE	= ( XML_E_INVALID_MODEL + 1 ) ,
        XML_E_INVALIDXMLSPACE	= ( XML_E_INVALID_TYPE + 1 ) ,
        XML_E_MULTI_ATTR_VALUE	= ( XML_E_INVALIDXMLSPACE + 1 ) ,
        XML_E_INVALID_PRESENCE	= ( XML_E_MULTI_ATTR_VALUE + 1 ) ,
        XML_E_BADXMLCASE	= ( XML_E_INVALID_PRESENCE + 1 ) ,
        XML_E_CONDSECTINSUBSET	= ( XML_E_BADXMLCASE + 1 ) ,
        XML_E_CDATAINVALID	= ( XML_E_CONDSECTINSUBSET + 1 ) ,
        XML_E_INVALID_STANDALONE	= ( XML_E_CDATAINVALID + 1 ) ,
        XML_E_UNEXPECTED_STANDALONE	= ( XML_E_INVALID_STANDALONE + 1 ) ,
        XML_E_DOCTYPE_IN_DTD	= ( XML_E_UNEXPECTED_STANDALONE + 1 ) ,
        XML_E_MISSING_ENTITY	= ( XML_E_DOCTYPE_IN_DTD + 1 ) ,
        XML_E_ENTITYREF_INNAME	= ( XML_E_MISSING_ENTITY + 1 ) ,
        XML_E_DOCTYPE_OUTSIDE_PROLOG	= ( XML_E_ENTITYREF_INNAME + 1 ) ,
        XML_E_INVALID_VERSION	= ( XML_E_DOCTYPE_OUTSIDE_PROLOG + 1 ) ,
        XML_E_DTDELEMENT_OUTSIDE_DTD	= ( XML_E_INVALID_VERSION + 1 ) ,
        XML_E_DUPLICATEDOCTYPE	= ( XML_E_DTDELEMENT_OUTSIDE_DTD + 1 ) ,
        XML_E_RESOURCE	= ( XML_E_DUPLICATEDOCTYPE + 1 ) ,
        XML_E_MISSINGNAME	= ( XML_E_RESOURCE + 1 ) ,
        XML_E_LASTERROR	= ( XML_E_MISSINGNAME + 1 ) 
    } 	XML_ERROR_CODE;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_xmlparser_0000_0000_0004
    {
        XMLPARSER_IDLE	= 0,
        XMLPARSER_WAITING	= ( XMLPARSER_IDLE + 1 ) ,
        XMLPARSER_BUSY	= ( XMLPARSER_WAITING + 1 ) ,
        XMLPARSER_ERROR	= ( XMLPARSER_BUSY + 1 ) ,
        XMLPARSER_STOPPED	= ( XMLPARSER_ERROR + 1 ) ,
        XMLPARSER_SUSPENDED	= ( XMLPARSER_STOPPED + 1 ) 
    } 	XML_PARSER_STATE;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_xmlparser_0000_0000_0005
    {
        XMLFLAG_FLOATINGAMP	= 0x1,
        XMLFLAG_SHORTENDTAGS	= 0x2,
        XMLFLAG_CASEINSENSITIVE	= 0x4,
        XMLFLAG_NONAMESPACES	= 0x8,
        XMLFLAG_NOWHITESPACE	= 0x10,
        XMLFLAG_IE4QUIRKS	= 0x20,
        XMLFLAG_NODTDNODES	= 0x40,
        XMLFLAG_IE4COMPATIBILITY	= 0xff,
        XMLFLAG_IGNORESXMLDECL	= 0x100,
        XMLFLAG_SAX	= 0x200,
        XMLFLAG_NORMALIZELB	= 0x400,
        XMLFLAG_IGNOREENCODING	= 0x800,
        XMLFLAG_USEWINHTTP	= 0x1000,
        XMLFLAG_RUNBUFFERONLY	= 0x2000,
        XMLFLAG_LAZYRESYNC	= 0x4000,
        XMLFLAG_PROHIBIT_DTD	= 0x8000,
        XMLFLAG_IGNOREILLEGALCHARS	= 0x10000,
        XMLFLAG_IE5COMPATIBILITY	= 0xf0100
    } 	XML_PARSER_FLAGS;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_xmlparser_0000_0000_0006
    {
        XMLNF_STARTDOCUMENT	= 0,
        XMLNF_STARTDTD	= ( XMLNF_STARTDOCUMENT + 1 ) ,
        XMLNF_ENDDTD	= ( XMLNF_STARTDTD + 1 ) ,
        XMLNF_STARTDTDSUBSET	= ( XMLNF_ENDDTD + 1 ) ,
        XMLNF_ENDDTDSUBSET	= ( XMLNF_STARTDTDSUBSET + 1 ) ,
        XMLNF_ENDPROLOG	= ( XMLNF_ENDDTDSUBSET + 1 ) ,
        XMLNF_STARTENTITY	= ( XMLNF_ENDPROLOG + 1 ) ,
        XMLNF_ENDENTITY	= ( XMLNF_STARTENTITY + 1 ) ,
        XMLNF_ENDDOCUMENT	= ( XMLNF_ENDENTITY + 1 ) ,
        XMLNF_DATAAVAILABLE	= ( XMLNF_ENDDOCUMENT + 1 ) ,
        XMLNF_LASTEVENT	= XMLNF_DATAAVAILABLE
    } 	XML_NODEFACTORY_EVENT;

typedef struct _XML_NODE_INFO
    {
    DWORD dwSize;
    DWORD dwType;
    DWORD dwSubType;
    BOOL fTerminal;
    const WCHAR *pwcText;
    ULONG ulLen;
    ULONG ulNsPrefixLen;
    PVOID pNode;
    PVOID pReserved;
    } 	XML_NODE_INFO;



extern RPC_IF_HANDLE __MIDL_itf_xmlparser_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_xmlparser_0000_0000_v0_0_s_ifspec;


#ifndef __XMLPSR_LIBRARY_DEFINED__
#define __XMLPSR_LIBRARY_DEFINED__

/* library XMLPSR */
/* [version][lcid][helpstring][uuid] */ 


EXTERN_C const IID LIBID_XMLPSR;

#ifndef __IXMLNodeSource_INTERFACE_DEFINED__
#define __IXMLNodeSource_INTERFACE_DEFINED__

/* interface IXMLNodeSource */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IXMLNodeSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d242361d-51a0-11d2-9caf-0060b0ec3d39")
    IXMLNodeSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFactory( 
            /* [in] */ IXMLNodeFactory *pNodeFactory) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFactory( 
            /* [out] */ IXMLNodeFactory **ppNodeFactory) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Abort( 
            /* [in] */ BSTR bstrErrorInfo) = 0;
        
        virtual ULONG STDMETHODCALLTYPE GetLineNumber( void) = 0;
        
        virtual ULONG STDMETHODCALLTYPE GetLinePosition( void) = 0;
        
        virtual ULONG STDMETHODCALLTYPE GetAbsolutePosition( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLineBuffer( 
            /* [out] */ const WCHAR **ppwcBuf,
            /* [out] */ ULONG *pulLen,
            /* [out] */ ULONG *pulStartPos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLastError( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetErrorInfo( 
            /* [out] */ BSTR *pbstrErrorInfo) = 0;
        
        virtual ULONG STDMETHODCALLTYPE GetFlags( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetURL( 
            /* [out] */ const WCHAR **ppwcBuf) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXMLNodeSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXMLNodeSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXMLNodeSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXMLNodeSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFactory )( 
            IXMLNodeSource * This,
            /* [in] */ IXMLNodeFactory *pNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *GetFactory )( 
            IXMLNodeSource * This,
            /* [out] */ IXMLNodeFactory **ppNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IXMLNodeSource * This,
            /* [in] */ BSTR bstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetLineNumber )( 
            IXMLNodeSource * This);
        
        ULONG ( STDMETHODCALLTYPE *GetLinePosition )( 
            IXMLNodeSource * This);
        
        ULONG ( STDMETHODCALLTYPE *GetAbsolutePosition )( 
            IXMLNodeSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLineBuffer )( 
            IXMLNodeSource * This,
            /* [out] */ const WCHAR **ppwcBuf,
            /* [out] */ ULONG *pulLen,
            /* [out] */ ULONG *pulStartPos);
        
        HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IXMLNodeSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetErrorInfo )( 
            IXMLNodeSource * This,
            /* [out] */ BSTR *pbstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetFlags )( 
            IXMLNodeSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetURL )( 
            IXMLNodeSource * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        END_INTERFACE
    } IXMLNodeSourceVtbl;

    interface IXMLNodeSource
    {
        CONST_VTBL struct IXMLNodeSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLNodeSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXMLNodeSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXMLNodeSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXMLNodeSource_SetFactory(This,pNodeFactory)	\
    ( (This)->lpVtbl -> SetFactory(This,pNodeFactory) ) 

#define IXMLNodeSource_GetFactory(This,ppNodeFactory)	\
    ( (This)->lpVtbl -> GetFactory(This,ppNodeFactory) ) 

#define IXMLNodeSource_Abort(This,bstrErrorInfo)	\
    ( (This)->lpVtbl -> Abort(This,bstrErrorInfo) ) 

#define IXMLNodeSource_GetLineNumber(This)	\
    ( (This)->lpVtbl -> GetLineNumber(This) ) 

#define IXMLNodeSource_GetLinePosition(This)	\
    ( (This)->lpVtbl -> GetLinePosition(This) ) 

#define IXMLNodeSource_GetAbsolutePosition(This)	\
    ( (This)->lpVtbl -> GetAbsolutePosition(This) ) 

#define IXMLNodeSource_GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos)	\
    ( (This)->lpVtbl -> GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos) ) 

#define IXMLNodeSource_GetLastError(This)	\
    ( (This)->lpVtbl -> GetLastError(This) ) 

#define IXMLNodeSource_GetErrorInfo(This,pbstrErrorInfo)	\
    ( (This)->lpVtbl -> GetErrorInfo(This,pbstrErrorInfo) ) 

#define IXMLNodeSource_GetFlags(This)	\
    ( (This)->lpVtbl -> GetFlags(This) ) 

#define IXMLNodeSource_GetURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetURL(This,ppwcBuf) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXMLNodeSource_INTERFACE_DEFINED__ */


#ifndef __IXMLParser_INTERFACE_DEFINED__
#define __IXMLParser_INTERFACE_DEFINED__

/* interface IXMLParser */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IXMLParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d242361e-51a0-11d2-9caf-0060b0ec3d39")
    IXMLParser : public IXMLNodeSource
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetURL( 
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fAsync) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BOOL fFullyAvailable,
            /* [in] */ IMoniker *pimkName,
            /* [in] */ LPBC pibc,
            /* [in] */ DWORD grfMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetInput( 
            /* [in] */ IUnknown *pStm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PushData( 
            /* [in] */ const unsigned char *pData,
            /* [in] */ ULONG ulChars,
            /* [in] */ BOOL fLastBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadDTD( 
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadEntity( 
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fpe) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ParseEntity( 
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen,
            /* [in] */ BOOL fpe) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExpandEntity( 
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRoot( 
            /* [in] */ PVOID pRoot) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRoot( 
            /* [in] */ PVOID *ppRoot) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ long lChars) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParserState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Suspend( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFlags( 
            /* [in] */ ULONG iFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSecureBaseURL( 
            /* [in] */ const WCHAR *pszBaseUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSecureBaseURL( 
            /* [out] */ const WCHAR **ppwcBuf) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXMLParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXMLParser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXMLParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFactory )( 
            IXMLParser * This,
            /* [in] */ IXMLNodeFactory *pNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *GetFactory )( 
            IXMLParser * This,
            /* [out] */ IXMLNodeFactory **ppNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IXMLParser * This,
            /* [in] */ BSTR bstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetLineNumber )( 
            IXMLParser * This);
        
        ULONG ( STDMETHODCALLTYPE *GetLinePosition )( 
            IXMLParser * This);
        
        ULONG ( STDMETHODCALLTYPE *GetAbsolutePosition )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLineBuffer )( 
            IXMLParser * This,
            /* [out] */ const WCHAR **ppwcBuf,
            /* [out] */ ULONG *pulLen,
            /* [out] */ ULONG *pulStartPos);
        
        HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetErrorInfo )( 
            IXMLParser * This,
            /* [out] */ BSTR *pbstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetFlags )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetURL )( 
            IXMLParser * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        HRESULT ( STDMETHODCALLTYPE *SetURL )( 
            IXMLParser * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fAsync);
        
        HRESULT ( STDMETHODCALLTYPE *Load )( 
            IXMLParser * This,
            /* [in] */ BOOL fFullyAvailable,
            /* [in] */ IMoniker *pimkName,
            /* [in] */ LPBC pibc,
            /* [in] */ DWORD grfMode);
        
        HRESULT ( STDMETHODCALLTYPE *SetInput )( 
            IXMLParser * This,
            /* [in] */ IUnknown *pStm);
        
        HRESULT ( STDMETHODCALLTYPE *PushData )( 
            IXMLParser * This,
            /* [in] */ const unsigned char *pData,
            /* [in] */ ULONG ulChars,
            /* [in] */ BOOL fLastBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *LoadDTD )( 
            IXMLParser * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl);
        
        HRESULT ( STDMETHODCALLTYPE *LoadEntity )( 
            IXMLParser * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fpe);
        
        HRESULT ( STDMETHODCALLTYPE *ParseEntity )( 
            IXMLParser * This,
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen,
            /* [in] */ BOOL fpe);
        
        HRESULT ( STDMETHODCALLTYPE *ExpandEntity )( 
            IXMLParser * This,
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen);
        
        HRESULT ( STDMETHODCALLTYPE *SetRoot )( 
            IXMLParser * This,
            /* [in] */ PVOID pRoot);
        
        HRESULT ( STDMETHODCALLTYPE *GetRoot )( 
            IXMLParser * This,
            /* [in] */ PVOID *ppRoot);
        
        HRESULT ( STDMETHODCALLTYPE *Run )( 
            IXMLParser * This,
            /* [in] */ long lChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetParserState )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *Suspend )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IXMLParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFlags )( 
            IXMLParser * This,
            /* [in] */ ULONG iFlags);
        
        HRESULT ( STDMETHODCALLTYPE *SetSecureBaseURL )( 
            IXMLParser * This,
            /* [in] */ const WCHAR *pszBaseUrl);
        
        HRESULT ( STDMETHODCALLTYPE *GetSecureBaseURL )( 
            IXMLParser * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        END_INTERFACE
    } IXMLParserVtbl;

    interface IXMLParser
    {
        CONST_VTBL struct IXMLParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLParser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXMLParser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXMLParser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXMLParser_SetFactory(This,pNodeFactory)	\
    ( (This)->lpVtbl -> SetFactory(This,pNodeFactory) ) 

#define IXMLParser_GetFactory(This,ppNodeFactory)	\
    ( (This)->lpVtbl -> GetFactory(This,ppNodeFactory) ) 

#define IXMLParser_Abort(This,bstrErrorInfo)	\
    ( (This)->lpVtbl -> Abort(This,bstrErrorInfo) ) 

#define IXMLParser_GetLineNumber(This)	\
    ( (This)->lpVtbl -> GetLineNumber(This) ) 

#define IXMLParser_GetLinePosition(This)	\
    ( (This)->lpVtbl -> GetLinePosition(This) ) 

#define IXMLParser_GetAbsolutePosition(This)	\
    ( (This)->lpVtbl -> GetAbsolutePosition(This) ) 

#define IXMLParser_GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos)	\
    ( (This)->lpVtbl -> GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos) ) 

#define IXMLParser_GetLastError(This)	\
    ( (This)->lpVtbl -> GetLastError(This) ) 

#define IXMLParser_GetErrorInfo(This,pbstrErrorInfo)	\
    ( (This)->lpVtbl -> GetErrorInfo(This,pbstrErrorInfo) ) 

#define IXMLParser_GetFlags(This)	\
    ( (This)->lpVtbl -> GetFlags(This) ) 

#define IXMLParser_GetURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetURL(This,ppwcBuf) ) 


#define IXMLParser_SetURL(This,pszBaseUrl,pszRelativeUrl,fAsync)	\
    ( (This)->lpVtbl -> SetURL(This,pszBaseUrl,pszRelativeUrl,fAsync) ) 

#define IXMLParser_Load(This,fFullyAvailable,pimkName,pibc,grfMode)	\
    ( (This)->lpVtbl -> Load(This,fFullyAvailable,pimkName,pibc,grfMode) ) 

#define IXMLParser_SetInput(This,pStm)	\
    ( (This)->lpVtbl -> SetInput(This,pStm) ) 

#define IXMLParser_PushData(This,pData,ulChars,fLastBuffer)	\
    ( (This)->lpVtbl -> PushData(This,pData,ulChars,fLastBuffer) ) 

#define IXMLParser_LoadDTD(This,pszBaseUrl,pszRelativeUrl)	\
    ( (This)->lpVtbl -> LoadDTD(This,pszBaseUrl,pszRelativeUrl) ) 

#define IXMLParser_LoadEntity(This,pszBaseUrl,pszRelativeUrl,fpe)	\
    ( (This)->lpVtbl -> LoadEntity(This,pszBaseUrl,pszRelativeUrl,fpe) ) 

#define IXMLParser_ParseEntity(This,pwcText,ulLen,fpe)	\
    ( (This)->lpVtbl -> ParseEntity(This,pwcText,ulLen,fpe) ) 

#define IXMLParser_ExpandEntity(This,pwcText,ulLen)	\
    ( (This)->lpVtbl -> ExpandEntity(This,pwcText,ulLen) ) 

#define IXMLParser_SetRoot(This,pRoot)	\
    ( (This)->lpVtbl -> SetRoot(This,pRoot) ) 

#define IXMLParser_GetRoot(This,ppRoot)	\
    ( (This)->lpVtbl -> GetRoot(This,ppRoot) ) 

#define IXMLParser_Run(This,lChars)	\
    ( (This)->lpVtbl -> Run(This,lChars) ) 

#define IXMLParser_GetParserState(This)	\
    ( (This)->lpVtbl -> GetParserState(This) ) 

#define IXMLParser_Suspend(This)	\
    ( (This)->lpVtbl -> Suspend(This) ) 

#define IXMLParser_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IXMLParser_SetFlags(This,iFlags)	\
    ( (This)->lpVtbl -> SetFlags(This,iFlags) ) 

#define IXMLParser_SetSecureBaseURL(This,pszBaseUrl)	\
    ( (This)->lpVtbl -> SetSecureBaseURL(This,pszBaseUrl) ) 

#define IXMLParser_GetSecureBaseURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetSecureBaseURL(This,ppwcBuf) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXMLParser_INTERFACE_DEFINED__ */


#ifndef __IXMLParser2_INTERFACE_DEFINED__
#define __IXMLParser2_INTERFACE_DEFINED__

/* interface IXMLParser2 */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IXMLParser2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("50fb5b1a-a75c-4ff4-86b2-14c62189e2cc")
    IXMLParser2 : public IXMLParser
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetMaxXmlSize( 
            /* [in] */ ULONG MaxSizeCch) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxXmlSize( 
            /* [out] */ ULONG *MaxSizeCch) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXMLParser2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXMLParser2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXMLParser2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFactory )( 
            IXMLParser2 * This,
            /* [in] */ IXMLNodeFactory *pNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *GetFactory )( 
            IXMLParser2 * This,
            /* [out] */ IXMLNodeFactory **ppNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IXMLParser2 * This,
            /* [in] */ BSTR bstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetLineNumber )( 
            IXMLParser2 * This);
        
        ULONG ( STDMETHODCALLTYPE *GetLinePosition )( 
            IXMLParser2 * This);
        
        ULONG ( STDMETHODCALLTYPE *GetAbsolutePosition )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLineBuffer )( 
            IXMLParser2 * This,
            /* [out] */ const WCHAR **ppwcBuf,
            /* [out] */ ULONG *pulLen,
            /* [out] */ ULONG *pulStartPos);
        
        HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetErrorInfo )( 
            IXMLParser2 * This,
            /* [out] */ BSTR *pbstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetFlags )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetURL )( 
            IXMLParser2 * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        HRESULT ( STDMETHODCALLTYPE *SetURL )( 
            IXMLParser2 * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fAsync);
        
        HRESULT ( STDMETHODCALLTYPE *Load )( 
            IXMLParser2 * This,
            /* [in] */ BOOL fFullyAvailable,
            /* [in] */ IMoniker *pimkName,
            /* [in] */ LPBC pibc,
            /* [in] */ DWORD grfMode);
        
        HRESULT ( STDMETHODCALLTYPE *SetInput )( 
            IXMLParser2 * This,
            /* [in] */ IUnknown *pStm);
        
        HRESULT ( STDMETHODCALLTYPE *PushData )( 
            IXMLParser2 * This,
            /* [in] */ const unsigned char *pData,
            /* [in] */ ULONG ulChars,
            /* [in] */ BOOL fLastBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *LoadDTD )( 
            IXMLParser2 * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl);
        
        HRESULT ( STDMETHODCALLTYPE *LoadEntity )( 
            IXMLParser2 * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fpe);
        
        HRESULT ( STDMETHODCALLTYPE *ParseEntity )( 
            IXMLParser2 * This,
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen,
            /* [in] */ BOOL fpe);
        
        HRESULT ( STDMETHODCALLTYPE *ExpandEntity )( 
            IXMLParser2 * This,
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen);
        
        HRESULT ( STDMETHODCALLTYPE *SetRoot )( 
            IXMLParser2 * This,
            /* [in] */ PVOID pRoot);
        
        HRESULT ( STDMETHODCALLTYPE *GetRoot )( 
            IXMLParser2 * This,
            /* [in] */ PVOID *ppRoot);
        
        HRESULT ( STDMETHODCALLTYPE *Run )( 
            IXMLParser2 * This,
            /* [in] */ long lChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetParserState )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *Suspend )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IXMLParser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFlags )( 
            IXMLParser2 * This,
            /* [in] */ ULONG iFlags);
        
        HRESULT ( STDMETHODCALLTYPE *SetSecureBaseURL )( 
            IXMLParser2 * This,
            /* [in] */ const WCHAR *pszBaseUrl);
        
        HRESULT ( STDMETHODCALLTYPE *GetSecureBaseURL )( 
            IXMLParser2 * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        HRESULT ( STDMETHODCALLTYPE *SetMaxXmlSize )( 
            IXMLParser2 * This,
            /* [in] */ ULONG MaxSizeCch);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxXmlSize )( 
            IXMLParser2 * This,
            /* [out] */ ULONG *MaxSizeCch);
        
        END_INTERFACE
    } IXMLParser2Vtbl;

    interface IXMLParser2
    {
        CONST_VTBL struct IXMLParser2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLParser2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXMLParser2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXMLParser2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXMLParser2_SetFactory(This,pNodeFactory)	\
    ( (This)->lpVtbl -> SetFactory(This,pNodeFactory) ) 

#define IXMLParser2_GetFactory(This,ppNodeFactory)	\
    ( (This)->lpVtbl -> GetFactory(This,ppNodeFactory) ) 

#define IXMLParser2_Abort(This,bstrErrorInfo)	\
    ( (This)->lpVtbl -> Abort(This,bstrErrorInfo) ) 

#define IXMLParser2_GetLineNumber(This)	\
    ( (This)->lpVtbl -> GetLineNumber(This) ) 

#define IXMLParser2_GetLinePosition(This)	\
    ( (This)->lpVtbl -> GetLinePosition(This) ) 

#define IXMLParser2_GetAbsolutePosition(This)	\
    ( (This)->lpVtbl -> GetAbsolutePosition(This) ) 

#define IXMLParser2_GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos)	\
    ( (This)->lpVtbl -> GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos) ) 

#define IXMLParser2_GetLastError(This)	\
    ( (This)->lpVtbl -> GetLastError(This) ) 

#define IXMLParser2_GetErrorInfo(This,pbstrErrorInfo)	\
    ( (This)->lpVtbl -> GetErrorInfo(This,pbstrErrorInfo) ) 

#define IXMLParser2_GetFlags(This)	\
    ( (This)->lpVtbl -> GetFlags(This) ) 

#define IXMLParser2_GetURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetURL(This,ppwcBuf) ) 


#define IXMLParser2_SetURL(This,pszBaseUrl,pszRelativeUrl,fAsync)	\
    ( (This)->lpVtbl -> SetURL(This,pszBaseUrl,pszRelativeUrl,fAsync) ) 

#define IXMLParser2_Load(This,fFullyAvailable,pimkName,pibc,grfMode)	\
    ( (This)->lpVtbl -> Load(This,fFullyAvailable,pimkName,pibc,grfMode) ) 

#define IXMLParser2_SetInput(This,pStm)	\
    ( (This)->lpVtbl -> SetInput(This,pStm) ) 

#define IXMLParser2_PushData(This,pData,ulChars,fLastBuffer)	\
    ( (This)->lpVtbl -> PushData(This,pData,ulChars,fLastBuffer) ) 

#define IXMLParser2_LoadDTD(This,pszBaseUrl,pszRelativeUrl)	\
    ( (This)->lpVtbl -> LoadDTD(This,pszBaseUrl,pszRelativeUrl) ) 

#define IXMLParser2_LoadEntity(This,pszBaseUrl,pszRelativeUrl,fpe)	\
    ( (This)->lpVtbl -> LoadEntity(This,pszBaseUrl,pszRelativeUrl,fpe) ) 

#define IXMLParser2_ParseEntity(This,pwcText,ulLen,fpe)	\
    ( (This)->lpVtbl -> ParseEntity(This,pwcText,ulLen,fpe) ) 

#define IXMLParser2_ExpandEntity(This,pwcText,ulLen)	\
    ( (This)->lpVtbl -> ExpandEntity(This,pwcText,ulLen) ) 

#define IXMLParser2_SetRoot(This,pRoot)	\
    ( (This)->lpVtbl -> SetRoot(This,pRoot) ) 

#define IXMLParser2_GetRoot(This,ppRoot)	\
    ( (This)->lpVtbl -> GetRoot(This,ppRoot) ) 

#define IXMLParser2_Run(This,lChars)	\
    ( (This)->lpVtbl -> Run(This,lChars) ) 

#define IXMLParser2_GetParserState(This)	\
    ( (This)->lpVtbl -> GetParserState(This) ) 

#define IXMLParser2_Suspend(This)	\
    ( (This)->lpVtbl -> Suspend(This) ) 

#define IXMLParser2_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IXMLParser2_SetFlags(This,iFlags)	\
    ( (This)->lpVtbl -> SetFlags(This,iFlags) ) 

#define IXMLParser2_SetSecureBaseURL(This,pszBaseUrl)	\
    ( (This)->lpVtbl -> SetSecureBaseURL(This,pszBaseUrl) ) 

#define IXMLParser2_GetSecureBaseURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetSecureBaseURL(This,ppwcBuf) ) 


#define IXMLParser2_SetMaxXmlSize(This,MaxSizeCch)	\
    ( (This)->lpVtbl -> SetMaxXmlSize(This,MaxSizeCch) ) 

#define IXMLParser2_GetMaxXmlSize(This,MaxSizeCch)	\
    ( (This)->lpVtbl -> GetMaxXmlSize(This,MaxSizeCch) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXMLParser2_INTERFACE_DEFINED__ */


#ifndef __IXMLParser3_INTERFACE_DEFINED__
#define __IXMLParser3_INTERFACE_DEFINED__

/* interface IXMLParser3 */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IXMLParser3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C9407B83-04E7-496a-AF26-A5677AEFCC86")
    IXMLParser3 : public IXMLParser2
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ const WCHAR *pwcName,
            /* [in] */ VARIANT value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ const WCHAR *pwcName,
            /* [retval][out] */ VARIANT *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXMLParser3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXMLParser3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXMLParser3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFactory )( 
            IXMLParser3 * This,
            /* [in] */ IXMLNodeFactory *pNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *GetFactory )( 
            IXMLParser3 * This,
            /* [out] */ IXMLNodeFactory **ppNodeFactory);
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IXMLParser3 * This,
            /* [in] */ BSTR bstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetLineNumber )( 
            IXMLParser3 * This);
        
        ULONG ( STDMETHODCALLTYPE *GetLinePosition )( 
            IXMLParser3 * This);
        
        ULONG ( STDMETHODCALLTYPE *GetAbsolutePosition )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLineBuffer )( 
            IXMLParser3 * This,
            /* [out] */ const WCHAR **ppwcBuf,
            /* [out] */ ULONG *pulLen,
            /* [out] */ ULONG *pulStartPos);
        
        HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetErrorInfo )( 
            IXMLParser3 * This,
            /* [out] */ BSTR *pbstrErrorInfo);
        
        ULONG ( STDMETHODCALLTYPE *GetFlags )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetURL )( 
            IXMLParser3 * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        HRESULT ( STDMETHODCALLTYPE *SetURL )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fAsync);
        
        HRESULT ( STDMETHODCALLTYPE *Load )( 
            IXMLParser3 * This,
            /* [in] */ BOOL fFullyAvailable,
            /* [in] */ IMoniker *pimkName,
            /* [in] */ LPBC pibc,
            /* [in] */ DWORD grfMode);
        
        HRESULT ( STDMETHODCALLTYPE *SetInput )( 
            IXMLParser3 * This,
            /* [in] */ IUnknown *pStm);
        
        HRESULT ( STDMETHODCALLTYPE *PushData )( 
            IXMLParser3 * This,
            /* [in] */ const unsigned char *pData,
            /* [in] */ ULONG ulChars,
            /* [in] */ BOOL fLastBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *LoadDTD )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl);
        
        HRESULT ( STDMETHODCALLTYPE *LoadEntity )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pszBaseUrl,
            /* [in] */ const WCHAR *pszRelativeUrl,
            /* [in] */ BOOL fpe);
        
        HRESULT ( STDMETHODCALLTYPE *ParseEntity )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen,
            /* [in] */ BOOL fpe);
        
        HRESULT ( STDMETHODCALLTYPE *ExpandEntity )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pwcText,
            /* [in] */ ULONG ulLen);
        
        HRESULT ( STDMETHODCALLTYPE *SetRoot )( 
            IXMLParser3 * This,
            /* [in] */ PVOID pRoot);
        
        HRESULT ( STDMETHODCALLTYPE *GetRoot )( 
            IXMLParser3 * This,
            /* [in] */ PVOID *ppRoot);
        
        HRESULT ( STDMETHODCALLTYPE *Run )( 
            IXMLParser3 * This,
            /* [in] */ long lChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetParserState )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *Suspend )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IXMLParser3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetFlags )( 
            IXMLParser3 * This,
            /* [in] */ ULONG iFlags);
        
        HRESULT ( STDMETHODCALLTYPE *SetSecureBaseURL )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pszBaseUrl);
        
        HRESULT ( STDMETHODCALLTYPE *GetSecureBaseURL )( 
            IXMLParser3 * This,
            /* [out] */ const WCHAR **ppwcBuf);
        
        HRESULT ( STDMETHODCALLTYPE *SetMaxXmlSize )( 
            IXMLParser3 * This,
            /* [in] */ ULONG MaxSizeCch);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxXmlSize )( 
            IXMLParser3 * This,
            /* [out] */ ULONG *MaxSizeCch);
        
        HRESULT ( STDMETHODCALLTYPE *SetProperty )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pwcName,
            /* [in] */ VARIANT value);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            IXMLParser3 * This,
            /* [in] */ const WCHAR *pwcName,
            /* [retval][out] */ VARIANT *value);
        
        END_INTERFACE
    } IXMLParser3Vtbl;

    interface IXMLParser3
    {
        CONST_VTBL struct IXMLParser3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLParser3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXMLParser3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXMLParser3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXMLParser3_SetFactory(This,pNodeFactory)	\
    ( (This)->lpVtbl -> SetFactory(This,pNodeFactory) ) 

#define IXMLParser3_GetFactory(This,ppNodeFactory)	\
    ( (This)->lpVtbl -> GetFactory(This,ppNodeFactory) ) 

#define IXMLParser3_Abort(This,bstrErrorInfo)	\
    ( (This)->lpVtbl -> Abort(This,bstrErrorInfo) ) 

#define IXMLParser3_GetLineNumber(This)	\
    ( (This)->lpVtbl -> GetLineNumber(This) ) 

#define IXMLParser3_GetLinePosition(This)	\
    ( (This)->lpVtbl -> GetLinePosition(This) ) 

#define IXMLParser3_GetAbsolutePosition(This)	\
    ( (This)->lpVtbl -> GetAbsolutePosition(This) ) 

#define IXMLParser3_GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos)	\
    ( (This)->lpVtbl -> GetLineBuffer(This,ppwcBuf,pulLen,pulStartPos) ) 

#define IXMLParser3_GetLastError(This)	\
    ( (This)->lpVtbl -> GetLastError(This) ) 

#define IXMLParser3_GetErrorInfo(This,pbstrErrorInfo)	\
    ( (This)->lpVtbl -> GetErrorInfo(This,pbstrErrorInfo) ) 

#define IXMLParser3_GetFlags(This)	\
    ( (This)->lpVtbl -> GetFlags(This) ) 

#define IXMLParser3_GetURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetURL(This,ppwcBuf) ) 


#define IXMLParser3_SetURL(This,pszBaseUrl,pszRelativeUrl,fAsync)	\
    ( (This)->lpVtbl -> SetURL(This,pszBaseUrl,pszRelativeUrl,fAsync) ) 

#define IXMLParser3_Load(This,fFullyAvailable,pimkName,pibc,grfMode)	\
    ( (This)->lpVtbl -> Load(This,fFullyAvailable,pimkName,pibc,grfMode) ) 

#define IXMLParser3_SetInput(This,pStm)	\
    ( (This)->lpVtbl -> SetInput(This,pStm) ) 

#define IXMLParser3_PushData(This,pData,ulChars,fLastBuffer)	\
    ( (This)->lpVtbl -> PushData(This,pData,ulChars,fLastBuffer) ) 

#define IXMLParser3_LoadDTD(This,pszBaseUrl,pszRelativeUrl)	\
    ( (This)->lpVtbl -> LoadDTD(This,pszBaseUrl,pszRelativeUrl) ) 

#define IXMLParser3_LoadEntity(This,pszBaseUrl,pszRelativeUrl,fpe)	\
    ( (This)->lpVtbl -> LoadEntity(This,pszBaseUrl,pszRelativeUrl,fpe) ) 

#define IXMLParser3_ParseEntity(This,pwcText,ulLen,fpe)	\
    ( (This)->lpVtbl -> ParseEntity(This,pwcText,ulLen,fpe) ) 

#define IXMLParser3_ExpandEntity(This,pwcText,ulLen)	\
    ( (This)->lpVtbl -> ExpandEntity(This,pwcText,ulLen) ) 

#define IXMLParser3_SetRoot(This,pRoot)	\
    ( (This)->lpVtbl -> SetRoot(This,pRoot) ) 

#define IXMLParser3_GetRoot(This,ppRoot)	\
    ( (This)->lpVtbl -> GetRoot(This,ppRoot) ) 

#define IXMLParser3_Run(This,lChars)	\
    ( (This)->lpVtbl -> Run(This,lChars) ) 

#define IXMLParser3_GetParserState(This)	\
    ( (This)->lpVtbl -> GetParserState(This) ) 

#define IXMLParser3_Suspend(This)	\
    ( (This)->lpVtbl -> Suspend(This) ) 

#define IXMLParser3_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IXMLParser3_SetFlags(This,iFlags)	\
    ( (This)->lpVtbl -> SetFlags(This,iFlags) ) 

#define IXMLParser3_SetSecureBaseURL(This,pszBaseUrl)	\
    ( (This)->lpVtbl -> SetSecureBaseURL(This,pszBaseUrl) ) 

#define IXMLParser3_GetSecureBaseURL(This,ppwcBuf)	\
    ( (This)->lpVtbl -> GetSecureBaseURL(This,ppwcBuf) ) 


#define IXMLParser3_SetMaxXmlSize(This,MaxSizeCch)	\
    ( (This)->lpVtbl -> SetMaxXmlSize(This,MaxSizeCch) ) 

#define IXMLParser3_GetMaxXmlSize(This,MaxSizeCch)	\
    ( (This)->lpVtbl -> GetMaxXmlSize(This,MaxSizeCch) ) 


#define IXMLParser3_SetProperty(This,pwcName,value)	\
    ( (This)->lpVtbl -> SetProperty(This,pwcName,value) ) 

#define IXMLParser3_GetProperty(This,pwcName,value)	\
    ( (This)->lpVtbl -> GetProperty(This,pwcName,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXMLParser3_INTERFACE_DEFINED__ */


#ifndef __IXMLNodeFactory_INTERFACE_DEFINED__
#define __IXMLNodeFactory_INTERFACE_DEFINED__

/* interface IXMLNodeFactory */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IXMLNodeFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d242361f-51a0-11d2-9caf-0060b0ec3d39")
    IXMLNodeFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE NotifyEvent( 
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ XML_NODEFACTORY_EVENT iEvt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginChildren( 
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ XML_NODE_INFO *pNodeInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndChildren( 
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ BOOL fEmpty,
            /* [in] */ XML_NODE_INFO *pNodeInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Error( 
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ HRESULT hrErrorCode,
            /* [in] */ USHORT cNumRecs,
            /* [in] */ XML_NODE_INFO **apNodeInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateNode( 
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ PVOID pNodeParent,
            /* [in] */ USHORT cNumRecs,
            /* [in] */ XML_NODE_INFO **apNodeInfo) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXMLNodeFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXMLNodeFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXMLNodeFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXMLNodeFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyEvent )( 
            IXMLNodeFactory * This,
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ XML_NODEFACTORY_EVENT iEvt);
        
        HRESULT ( STDMETHODCALLTYPE *BeginChildren )( 
            IXMLNodeFactory * This,
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ XML_NODE_INFO *pNodeInfo);
        
        HRESULT ( STDMETHODCALLTYPE *EndChildren )( 
            IXMLNodeFactory * This,
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ BOOL fEmpty,
            /* [in] */ XML_NODE_INFO *pNodeInfo);
        
        HRESULT ( STDMETHODCALLTYPE *Error )( 
            IXMLNodeFactory * This,
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ HRESULT hrErrorCode,
            /* [in] */ USHORT cNumRecs,
            /* [in] */ XML_NODE_INFO **apNodeInfo);
        
        HRESULT ( STDMETHODCALLTYPE *CreateNode )( 
            IXMLNodeFactory * This,
            /* [in] */ IXMLNodeSource *pSource,
            /* [in] */ PVOID pNodeParent,
            /* [in] */ USHORT cNumRecs,
            /* [in] */ XML_NODE_INFO **apNodeInfo);
        
        END_INTERFACE
    } IXMLNodeFactoryVtbl;

    interface IXMLNodeFactory
    {
        CONST_VTBL struct IXMLNodeFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLNodeFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXMLNodeFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXMLNodeFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXMLNodeFactory_NotifyEvent(This,pSource,iEvt)	\
    ( (This)->lpVtbl -> NotifyEvent(This,pSource,iEvt) ) 

#define IXMLNodeFactory_BeginChildren(This,pSource,pNodeInfo)	\
    ( (This)->lpVtbl -> BeginChildren(This,pSource,pNodeInfo) ) 

#define IXMLNodeFactory_EndChildren(This,pSource,fEmpty,pNodeInfo)	\
    ( (This)->lpVtbl -> EndChildren(This,pSource,fEmpty,pNodeInfo) ) 

#define IXMLNodeFactory_Error(This,pSource,hrErrorCode,cNumRecs,apNodeInfo)	\
    ( (This)->lpVtbl -> Error(This,pSource,hrErrorCode,cNumRecs,apNodeInfo) ) 

#define IXMLNodeFactory_CreateNode(This,pSource,pNodeParent,cNumRecs,apNodeInfo)	\
    ( (This)->lpVtbl -> CreateNode(This,pSource,pNodeParent,cNumRecs,apNodeInfo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXMLNodeFactory_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_XMLParser;

#ifdef __cplusplus

class DECLSPEC_UUID("f5078f19-c551-11d3-89b9-0000f81fe221")
XMLParser;
#endif

EXTERN_C const CLSID CLSID_XMLParser26;

#ifdef __cplusplus

class DECLSPEC_UUID("f5078f20-c551-11d3-89b9-0000f81fe221")
XMLParser26;
#endif

EXTERN_C const CLSID CLSID_XMLParser30;

#ifdef __cplusplus

class DECLSPEC_UUID("f5078f31-c551-11d3-89b9-0000f81fe221")
XMLParser30;
#endif
#endif /* __XMLPSR_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


