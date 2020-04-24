// FileInfo.cpp: implementation of the CFileInfo class.
// (c) Codeguru & friends
// Coded by Antonio Tejada Lacaci. 1999
// atejada@espanet.com
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "FileInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
// BUFFER MUST be 4 multiple
#define FILEBUFSIZE 4096

/*
---- The original CRC32 code stated -----

Modified from a version posted on the Pascal echo by Floor
A.C. Naaijkens (note introduction of a Constant and a
Function and making the Array a Constant outside of
crc32() which should improve speed a lot; further
references made to a C version in a File of Snippets from the
(112 min left), (H)elp, More?   C Echo marked as "Copyright (C) 1986 Gary S. Brown" (mostly to
compare the large Arrays, which proved to be identical), and to
"File verification using CRC" by Mark R. Nelson in Dr. Dobbs'
Journal, May 1992.  [...]
(this was the part I found in Nelson).  The result is a CRC
value identical to that calculated by PKZip and ARJ

---- eof quote
*/

const DWORD CRC32[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d };

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CFileInfo::CFileInfo() {
		m_strFilePath = _T("");
		m_dwChecksum = 0;
		m_dwCRC = 0xFFFFFFFF;
		m_dwAttributes = 0xFFFFFFFF;
		m_uhChecksumBytes = 0;
		m_uhCRCBytes = 0;
		m_uhFileSize = 0;
		m_lParam = 0;
		m_timCreation.GetCurrentTime();
		m_timLastAccess.GetCurrentTime();
		m_timLastWrite.GetCurrentTime();
	}

	CFileInfo::CFileInfo(const CFileInfo& finf) {
		m_strFilePath = finf.m_strFilePath;
		m_dwChecksum = finf.m_dwChecksum;
		m_dwCRC = finf.m_dwCRC;
		m_dwAttributes = finf.m_dwAttributes;
		m_uhChecksumBytes = finf.m_uhChecksumBytes;
		m_uhCRCBytes = finf.m_uhCRCBytes;
		m_uhFileSize = finf.m_uhFileSize;
		m_lParam = finf.m_lParam;
		m_timCreation = finf.m_timCreation;
		m_timLastAccess = finf.m_timLastAccess;
		m_timLastWrite = finf.m_timLastWrite;
	}

	CFileInfo::~CFileInfo() {
	}

	void CFileInfo::Create(const WIN32_FIND_DATA* pwfd, const CString strFilePath, LPARAM lParam) {
		m_strFilePath = strFilePath + pwfd->cFileName;
		m_dwAttributes = pwfd->dwFileAttributes;
		m_uhFileSize = (((ULONGLONG) pwfd->nFileSizeHigh) << 32) + pwfd->nFileSizeLow;
		m_uhChecksumBytes = 0;
		m_uhCRCBytes = 0;
		m_timCreation = pwfd->ftCreationTime;
		m_timLastAccess = pwfd->ftLastAccessTime;
		m_timLastWrite = pwfd->ftLastWriteTime;
		m_lParam = lParam;
	}

	void CFileInfo::Create(const CString strFilePath, LPARAM lParam) {
		WIN32_FIND_DATA wfd;
		HANDLE hFind;
		int nBarPos;

		hFind = FindFirstFile(strFilePath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE) {
			nBarPos = strFilePath.ReverseFind(TCHAR('\\'));
			Create(&wfd, strFilePath.Left(nBarPos+1), lParam);
			FindClose(hFind);
		}
	}

	DWORD CFileInfo::GetChecksum(const ULONGLONG uhUpto, const BOOL bRecalc, 
		const volatile BOOL* pbAbort, volatile ULONG* pulCount) {
			HANDLE hf;
			DWORD dwBuffer[FILEBUFSIZE];
			DWORD dwRead;
			ULONGLONG uhCount;

			uhCount = (uhUpto+3) & 0xFFFFFFFFFFFFFFFC; // Make 4 multiple and get ceil
			if ((0 == uhUpto) ||(uhUpto>m_uhFileSize)) uhCount = m_uhFileSize;

			if (bRecalc) {
				m_uhChecksumBytes = 0;
				m_dwChecksum=0;
			}

			if (m_uhChecksumBytes < uhCount) {
				// Not all bytes summed
				hf = CreateFile(m_strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
					FILE_FLAG_SEQUENTIAL_SCAN, NULL);
				SetFilePointer(hf, m_uhChecksumBytes, (PLONG) &dwRead, FILE_BEGIN);
				while ((m_uhChecksumBytes<uhCount) &&  // Bytes pending
					(ReadFile(hf, dwBuffer, MIN(sizeof(dwBuffer), (DWORD) uhCount), &dwRead, 
					NULL)) &&                        // ReadFile was Ok
					(dwRead>0) &&                       // More than 0 bytes were read
					((pbAbort == NULL) || (!*pbAbort))) // Not aborted by another thread
				{
					// Calc checksum for buffer
					for (UINT i=0;i<(dwRead >> 2);i++) {
						m_dwChecksum += dwBuffer[i];
						if (pulCount) InterlockedExchange((long*)pulCount,(*pulCount)+4);
					}
					if (dwRead & 0x3)
						// Calc Checksum of last dword padding with zeroes
						m_dwChecksum += dwBuffer[(dwRead >> 2)] & 
						(0xFFFFFFFF >> ((4-(dwRead & 0x3)) << 3));
					if (pulCount) InterlockedExchange((long*)pulCount,(*pulCount) + (dwRead & 0x3));
					m_uhChecksumBytes += dwRead;
				}
				CloseHandle(hf);
			}

			return m_dwChecksum;
	}

	DWORD CFileInfo::GetCRC(const ULONGLONG uhUpto, const BOOL bRecalc, 
		const volatile BOOL* pbAbort, volatile ULONG* pulCount) {
			HANDLE hf;
			DWORD dwRead;
			ULONGLONG uhCount;
			DWORD dwBuffer[FILEBUFSIZE];

			uhCount = (uhUpto+3) & 0xFFFFFFFFFFFFFFFC; // Make 4 multiple and get ceil
			if ((0 == uhUpto) ||(uhUpto>m_uhFileSize)) uhCount = m_uhFileSize;

			if (bRecalc) {
				m_dwCRC = 0xFFFFFFFF;
				m_uhCRCBytes = 0;
			}
			if (m_uhCRCBytes < uhCount) {
				// Not all bytes computed
				hf = CreateFile(m_strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
					FILE_FLAG_SEQUENTIAL_SCAN, NULL);
				SetFilePointer(hf, m_uhCRCBytes, (PLONG) &dwRead, FILE_BEGIN);
				// Buffered read
				while ((m_uhCRCBytes<uhCount) &&        // Bytes pending
					(ReadFile(hf, dwBuffer, MIN(sizeof(dwBuffer), (DWORD) uhCount), &dwRead, 
					NULL)) &&                         // Readfile was Ok
					(dwRead>0) &&                        // There were some bytes read
					((pbAbort == NULL) || (!*pbAbort)))  // Wasn't aborted by another thread
				{
					// Calc CRC for buffer
					for (UINT i=0;i<(dwRead >> 2);i++) {
						m_dwCRC = CRC32[(BYTE) (m_dwCRC ^ dwBuffer[i])] ^ ((m_dwCRC >> 8) & 0x00FFFFFF);
						if (pulCount) InterlockedExchange((long*)pulCount,(*pulCount)+4);
					}
					if (dwRead & 0x3) 
						// Calc CRC of last dword padding with zeroes
						m_dwCRC = CRC32[(BYTE) (m_dwCRC ^ (dwBuffer[dwRead >> 2] & 
						(0xFFFFFFFF >> ((4-(dwRead & 0x3)) << 3))))] ^ ((m_dwCRC >> 8) & 0x00FFFFFF);
					if (pulCount) InterlockedExchange((long*)pulCount,(*pulCount)+ (dwRead & 0x3));
					m_uhCRCBytes += dwRead;
				}
				CloseHandle(hf);
				// Adjust final CRC if EOF
				if (m_uhCRCBytes >= m_uhFileSize) m_dwCRC = m_dwCRC ^ 0xFFFFFFFF;
			}
			return m_dwCRC;
	}

	CString CFileInfo::GetFileDrive(void) const {
		TCHAR szDrive[_MAX_DRIVE];
		_tsplitpath_s(m_strFilePath, szDrive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0);
		return CString(szDrive);
	}

	CString CFileInfo::GetFileDir(void) const { 
		TCHAR szDir[_MAX_DIR];
		_tsplitpath_s(m_strFilePath, NULL, 0, szDir, _MAX_DIR, NULL, 0, NULL, 0);
		return CString(szDir);
	}

	CString CFileInfo::GetFileTitle(void) const {
		TCHAR szName[_MAX_FNAME];
		_tsplitpath_s(m_strFilePath, NULL, 0, NULL, 0, szName, _MAX_FNAME, NULL, 0);
		return CString(szName);
	}

	CString CFileInfo::GetFileExt(void) const {
		TCHAR szExt[_MAX_EXT];

		_tsplitpath_s(m_strFilePath, NULL, 0, NULL, 0, NULL, 0, szExt, _MAX_EXT);
		return CString(szExt);
	}

	CFileInfoArray::CFileInfoArray() {
		SetSize(0,50);
	}

	int CFileInfoArray::AddFileInfo(CFileInfo& finf, LPARAM lAddParam) {
		int i=(int)GetSize();

		// Find the position in the array where we have to insert finf
		switch (lAddParam) {
	  case AP_SORTASCENDING | AP_SORTBYSIZE:
		  for (i=0;((i<GetSize()) && ((*this)[i].GetLength64()<finf.GetLength64()));i++);
		  break;
	  case AP_SORTDESCENDING | AP_SORTBYSIZE:
		  for (i=0;((i<GetSize()) && ((*this)[i].GetLength64()>finf.GetLength64()));i++);
		  break;
	  case AP_SORTASCENDING | AP_SORTBYNAME:
		  for (i=0;((i<GetSize()) && ((*this)[i].GetFilePath()<finf.GetFilePath()));i++);
		  break;
	  case AP_SORTDESCENDING | AP_SORTBYNAME:
		  for (i=0;((i<GetSize()) && ((*this)[i].GetFilePath()>finf.GetFilePath()));i++);
		  break;
		}
		// Insert it
		InsertAt(i,finf);
		return i;
	}

	int CFileInfoArray::AddDir(const CString strDirName, const CString strMask, const BOOL bRecurse,
		LPARAM lAddParam, const BOOL bIncludeDirs, const volatile BOOL* pbAbort, 
		volatile ULONG* pulCount) { 

			WIN32_FIND_DATA wfd;
			HANDLE hFind;
			CString strText;
			CFileInfo finf;
			int nCount=0;

			strText = strDirName + _T("*");
			// Iterate through dirs
			hFind = FindFirstFile(strText, &wfd);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					if ((strcmp(wfd.cFileName,_T("."))) && (strcmp(wfd.cFileName,_T(".."))) && 
						(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
							strText = strDirName + wfd.cFileName;
							if (bIncludeDirs) {
								// Add directory itself
								finf.Create(&wfd, strDirName);
								AddFileInfo(finf, lAddParam);
								nCount++;
								if (pulCount) InterlockedIncrement((long*)pulCount);
							}
							if (bRecurse) {
								// Recurse Dirs
								nCount += AddDir(strText+_T("\\"), strMask, bRecurse, lAddParam, 
									bIncludeDirs, pbAbort, pulCount);
							}
					}
				} while (((pbAbort == NULL) || (!*pbAbort)) && (FindNextFile(hFind, &wfd)));
				FindClose(hFind);
			}


			// Iterate through files
			if ((pbAbort == NULL) || (!*pbAbort)) {
				strText = strDirName + strMask;
				hFind = FindFirstFile(strText, &wfd);
				if (hFind != INVALID_HANDLE_VALUE) {
					do {
						if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
							strText = strDirName + wfd.cFileName;
							finf.Create(&wfd, strDirName);
							AddFileInfo(finf, lAddParam);
							nCount++;
							if (pulCount) InterlockedIncrement((long*)pulCount);
						}
					} while (((pbAbort== NULL) || (!*pbAbort)) && (FindNextFile(hFind, &wfd)));
					FindClose(hFind);
				}
			}
			return nCount;
	}

	int CFileInfoArray::AddFile(const CString strFilePath, LPARAM lAddParam) {
		CFileInfo finf;

		finf.Create(strFilePath);
		return AddFileInfo(finf, lAddParam);
	}
