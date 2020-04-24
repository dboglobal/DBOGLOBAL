//***********************************************************************************
//
//	File		:	Table.h
//
//	Begin		:	2006-03-09
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Desc		:	Chung Doo sup  ( john@ntl-inc.com )
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"

#include "NtlSerializer.h"

#include <comutil.h>
#include <map>

class CNtlSerializer;
class CNtlXMLDoc;

#pragma pack(push, 4)
struct sTBLDAT
{
public:

	sTBLDAT()
		:tblidx(INVALID_TBLIDX) {}

	virtual ~sTBLDAT() {}

	TBLIDX				tblidx;


public:

	bool LoadFromBinary(CNtlSerializer& serializer)
	{
		if (serializer.GetDataSize() < GetDataSize())
		{
			return false;
		}

		serializer.Out(GetDataAddress(), GetDataSize());
		return true;
	}

	bool LoadFromChunk(CNtlSerializer& serializer)
	{
		if (serializer.GetDataSize() < GetDataSize())
		{
			return false;
		}

		serializer.Out(GetDataAddress(), GetDataSize());
		return true;
	}

	void SaveToBinary(CNtlSerializer& serializer)
	{
		serializer.In(GetDataAddress(), GetDataSize());
	}

	void* GetDataAddress()
	{
		return (char*)this + sizeof(void*);
	}

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

typedef void (*Dbo_TableErrorCallback)(char* pszMessage, void* pvArg);


enum eTABLE_LOCAL_LOAD_TYPE
{
  TABLE_LOAD_TYPE_NORMAL = 0,
  TABLE_LOAD_TYPE_REWRITE,
  TABLE_LOAD_TYPE_OVERRIDING,
};

class CTable
{
public:
	enum eLOADING_METHOD
	{
		LOADING_METHOD_XML = 0,
		LOADING_METHOD_BINARY,
		LOADING_METHOD_SECURED_BINARY,

		LOADING_METHOD_INVALID = 0xFFui8
	};

	enum eCONSTANT
	{
		TABLE_XML_FILE_NAME_MAX_LENGTH = 128
	};

public:

	typedef std::map<TBLIDX, sTBLDAT *> TABLE;
	typedef TABLE::iterator TABLEIT;
	typedef TABLE::value_type TABLEVAL;
	
public:

	CTable(void);
	virtual ~CTable(void);

	bool		Create(DWORD dwCodePage);

private:

	void		Destroy();

protected:

	void Init();

public:

	TABLEIT	Begin() { return m_mapTableList.begin(); }
	TABLEIT	End() { return m_mapTableList.end(); }
	virtual sTBLDAT *			FindData(TBLIDX tblidx) { (void) tblidx; return NULL; }
	size_t GetNumberOfTables() { return m_mapTableList.size(); }

protected:

	virtual WCHAR** GetSheetListInWChar() = 0;
	virtual void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage) = 0;
	virtual bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName) = 0;
	virtual bool AddTable(void* pvTable, bool bReload, bool bUpdate) = 0;
	virtual bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData) = 0;

public:

	void						Reset();

	bool						LoadFromXml(char* pszFileName, bool bReload, bool bUpdate);

	bool						LoadFromXml(WCHAR* pwszFileName, bool bReload, bool bUpdate);

	bool						LoadFromChunk(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate) = 0;

	virtual bool				SaveToBinary(CNtlSerializer& serializer) = 0;


public:

	DWORD						HexToDec(WCHAR* pwszHexString);


protected:

	virtual bool				InitializeFromXmlDoc(CNtlXMLDoc* pXmlDoc, WCHAR* pwszFileName, bool bReload, bool bUpdate);


protected:

	bool						CheckInvalidValue(BSTR bstr) { return bstr[0] == L'@' ? true : false; }

	char						READ_CHAR(BSTR bstr, const WCHAR* pwszFieldName, BYTE chInvalidValue = 0);

	BYTE						READ_BYTE(BSTR bstr, const WCHAR* pwszFieldName, BYTE byInvalidValue = INVALID_BYTE);

	WORD						READ_WORD(BSTR bstr, const WCHAR* pwszFieldName, WORD wInvalidValue = INVALID_WORD);

	DWORD						READ_DWORD(BSTR bstr, DWORD dwInvalidValue = INVALID_DWORD){ return CheckInvalidValue( bstr ) ? dwInvalidValue : (DWORD)(_wtoi64(bstr));}

	TBLIDX						READ_TBLIDX(BSTR bstr, DWORD dwInvalidValue = INVALID_TBLIDX){ return CheckInvalidValue( bstr ) ? dwInvalidValue : (DWORD)(_wtoi64(bstr));}

	FLOAT						READ_FLOAT(BSTR bstr, const WCHAR* pwszFieldName, float fInvalidValue = INVALID_FLOAT);

	DOUBLE						READ_DOUBLE(BSTR bstr, const WCHAR* pwszFieldName, DOUBLE dInvalidValue = INVALID_DOUBLE);

	BSTR 						READ_STR(BSTR bstr, const char * lpszInvalidValue = "") { return CheckInvalidValue( bstr ) ? (BSTR)lpszInvalidValue : bstr; }

	bool						READ_STR(std::string & rDest, BSTR bstr, const char * lpszInvalidValue = "");

	bool						READ_STR(std::wstring & rDest, BSTR bstr, const WCHAR * lpwszInvalidValue = L"");

	bool						READ_STRING(BSTR bstr, char* pszBuffer, DWORD dwBufferLength, const char* pszInvalidValue = "");

	bool						READ_STRINGW(BSTR bstr, WCHAR* pwszBuffer, DWORD dwBufferLength, const WCHAR* pwszInvalidValue = L"");

	bool 						READ_BOOL(BSTR bstr, const WCHAR* pwszFieldName, bool bInvalidlValue = false);

	DWORD 						READ_BITFLAG(BSTR bstr, DWORD dwInvalidValue = 0) { return CheckInvalidValue( bstr ) ? dwInvalidValue : HexToDec( bstr ); }

	//void						CheckNegativeInvalid(char* pszFormatString, BSTR bstr); // @가 있으면 안되는 필드로 Assert를 뿌려준다.

	void						CheckNegativeInvalid(const wchar_t* pwszFormatString, BSTR bstr); // @가 있으면 안되는 필드로 Assert를 뿌려준다.

public:

	static void					SetErrorCallbackFunction(Dbo_TableErrorCallback pfnErrorCallback, void* pvErrorCallbackArg = NULL);

	static void					CallErrorCallbackFunction(char* pszFormatString, ...);

	static void					CallErrorCallbackFunction(WCHAR* pwszFormatString, ...);

	WCHAR*						GetWszXmlFileName() { return m_wszXmlFileName; }
protected:

	TABLE						m_mapTableList;

protected:
	DWORD						m_dwCodePage;

	// This is only for displaying a message, so it can't be used for the other purposes.
	// 메시지를 출력하기 위해 선언된 멤버 변수이므로, 정해진 용도 이외에는 사용하면 안 된다.
	// by YOSHIKI(2007-08-07)
	WCHAR								m_wszXmlFileName[CTable::TABLE_XML_FILE_NAME_MAX_LENGTH + 1];

	static Dbo_TableErrorCallback		m_pfnErrorCallback;

	static void*						m_pvErrorCallbackArg;
};
