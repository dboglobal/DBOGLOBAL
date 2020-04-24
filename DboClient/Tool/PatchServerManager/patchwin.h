/*-------------------------------------------------------------------------*\
|                                                                           |
|  FILE: PATCHWIN.H                                                         |
|                                                                           |
|                                                                           |
|  RTPatch Patch Apply DLL API Header File				    |
|  ver. 10.00                                                               |
|                                                                           |
|                                                                           |
|  (C) Copyright Pocket Soft, Inc. 1997-2006.  All Rights Reserved.         |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef PATCHWIN_INCLUDED
#define PATCHWIN_INCLUDED
# include <windows.h>

/*-----------------------------------------------------------------------*\
|  C++ name mangling prevention -- beginning of block                     |
\*-----------------------------------------------------------------------*/
# ifdef __cplusplus
  extern "C" {
# endif /*__cplusplus */


# ifdef _WIN32

// STRUCTURES

struct FileAttrib {
unsigned Flags;
unsigned Attributes;
FILETIME ChangedTime;
FILETIME CreateTime;
FILETIME AccessTime;
unsigned long Size;
};

struct PatchAsyncParm {
long Position;
long Count;
void * Buffer;
};

struct PatchUserFileParm {
wchar_t * OldName;
wchar_t * NewName;
unsigned SelfReg;
unsigned Shared;
unsigned Backup;
};

typedef struct _DIRWALKDATE {
unsigned short Day   : 5;
unsigned short Month : 4;
unsigned short Year  : 7;
} DIRWALKDATE;

typedef struct _DIRWALKTIME {
unsigned short TwoSecs : 5;
unsigned short Minutes : 6;
unsigned short Hours   : 5;
} DIRWALKTIME;

typedef struct _MaintenanceInfo {
DWORD dwSize;
int LastReturnCode; 
BOOL UnPatched;
DWORD ApplyCount;
DWORD PatchNameSize;
DWORD CommentSize;
DWORD LastFileNameSize;
DIRWALKTIME LastTime;
DIRWALKDATE LastDate;
UCHAR * PatchName;
UCHAR * LastFileName;
UCHAR * Comment;
} MaintenanceInfo;

// CONSTANTS

# define PATCH_SUCCESS  0
# define PATCH_BUSY     -1

// Attrib Flags
# define ATTRIB_ATTRIBUTE     0x1
# define ATTRIB_CHANGED_DATE  0x2
# define ATTRIB_CREATE_DATE   0x4
# define ATTRIB_ACCESS_DATE   0x8
# define ATTRIB_SIZE          0x10

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetOpen( int Handle, HANDLE (CALLBACK *FileOpenPtr)( wchar_t *, int ) );

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetDirWalk( int Handle, 
	int (CALLBACK *DirWalkPtr)( wchar_t *, int, WIN32_FIND_DATAW * ) );

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetCreate( int Handle, 
	HANDLE (CALLBACK *FileCreatePtr)(wchar_t * ) );

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetDelete( int Handle,
	int (CALLBACK *FileDeletePtr)(wchar_t * ) );

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetRename( int Handle,
	int (CALLBACK *FileRenamePtr)(wchar_t *, wchar_t * ) );

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetAttribSet( int Handle,
	int (CALLBACK *FileAttribSetPtr)(wchar_t *, struct FileAttrib *) );

int /* ErrCode */
    __declspec( dllimport ) CALLBACK 
    RTPatchSetAttribGet( int Handle,
	int (CALLBACK *FileAttribGetPtr)(wchar_t *, struct FileAttrib *) );

UINT 
    __declspec( dllimport ) 
    CALLBACK RTPatchApply32( LPSTR CmdLine, 
			    LPVOID (CALLBACK * CallBackFn)(UINT, LPVOID),
			    BOOL WaitFlag);

UINT
    __declspec( dllimport )
    __cdecl RTPatchApply32NoCall( LPSTR CmdLine );

DWORD
    __declspec( dllimport ) 
    WINAPI RTPatchEnumPatches (
           char * szPatchEntry,
           MaintenanceInfo * MaintEntry, 
           int Index);

DWORD
    __declspec( dllimport ) 
     WINAPI RTPatchGetCost( 
           LPWSTR  PatchFile,          // Path to the Patch File
           LPDWORD BiggestNewFileSize, // Receives the size of the largest NEW file
           LPDWORD NewSystemSize,      // Receives the size of the NEW System of files
           LPDWORD OldSystemSize,      // Receives the size of the OLD System of files
           LPDWORD NumberOfFiles,      // Receives the number of files in the patch
           BOOL EZPatch);              // Caller sets to TRUE if PatchFile is an EZPatch (.exe) patch

// RTPatchGetCost Error Codes
// 0: Success
// 1: could not open patch file
// 2: need to upgrade patchw32.dll
// 9: corrupt patch file


# else
struct PatchAsyncParm {
long Position;
long Count;
void * Buffer;
};

struct PatchUserFileParm {
wchar_t * OldName;
wchar_t * NewName;
unsigned long SelfReg;
unsigned long Shared;
unsigned long Backup;
};

UINT CALLBACK RTPatchApply( LPSTR CmdLine, 
			    LPVOID (CALLBACK * CallBackFn)(UINT, LPVOID),
			    BOOL WaitFlag);

UINT _far _cdecl RTPatchApplyNoCall( LPSTR CmdLine );
# endif
/*-----------------------------------------------------------------------*\
    C++ name mangling prevention -- end of block
\*-----------------------------------------------------------------------*/
# ifdef __cplusplus
  };
# endif

# endif /* PATCHWIN_INCLUDED */
