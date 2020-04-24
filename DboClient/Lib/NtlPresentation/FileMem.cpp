#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlCoreUtil.h"

#include "FileMem.h"

CFileMem::CFileMem(void)
{
	m_pFileMem	= NULL;
	m_MemSize	= 0;
	m_pFile		= NULL;
}

CFileMem::~CFileMem(void)
{
	Free();
}

VOID CFileMem::Free()
{
	if(m_pFileMem)
	{
		RwFree(m_pFileMem);
		m_pFileMem = NULL;
	}
}

RwBool CFileMem::Load(RwChar* _pFilePath, RwChar* _pFileMode)
{
	if(fopen_s(&m_pFile, _pFilePath, _pFileMode))
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pFilePath << ")");
		return FALSE;
	}
	else
	{
		m_MemSize = _filelength(_fileno(m_pFile));
		if(m_MemSize == -1)
		{
			DBO_TRACE(FALSE, "CFileMem::Load, file length is -1.");
			fclose(m_pFile);
			return FALSE;
		}
		else
		{
			m_pFileMem = static_cast<BYTE*>(RwRealloc(m_pFileMem, m_MemSize, rwMEMHINTDUR_GLOBAL));
			fread(m_pFileMem, m_MemSize, 1, m_pFile);
		}

		fclose(m_pFile);
	}

	return TRUE;
}

RwBool CFileMem::Save()
{
	if(	!m_pFile ||
		!m_MemSize || 
		!m_pFileMem ||
		m_MemSize == -1 ||
		_filelength(_fileno(m_pFile)) != m_MemSize)
	{
		DBO_TRACE(FALSE, "CFileMem::Save, exception handled.");
		return FALSE;
	}

	fwrite(m_pFileMem, m_MemSize, 1, m_pFile);

	return TRUE;
}

RwBool CFileMem::Update(RwInt32 _FileSize)
{
	if(m_MemSize != _FileSize)
	{
		m_pFileMem = static_cast<BYTE*>(RwRealloc(m_pFileMem, _FileSize, rwMEMHINTDUR_GLOBAL));
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BYTE* CFileMem::GetDat()
{
	return m_pFileMem;
}

RwBool CFileMem::SetDat(RwInt32 _MemPos, RwInt32 _MemSize, BYTE* pDat)
{
	CopyMemory(&m_pFileMem[_MemPos], pDat, _MemSize);	

	return TRUE;
}

