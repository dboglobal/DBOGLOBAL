
/// 여기에서.. Msjexhnd.h 파일에 call back 함수를 만든다.
// 호출된 callback은 dboexceptionhandler에서 받아서.. 
// GetNtlCallStack()->LogFilePrint() 함수를 호출한다.

VOID DboExceptionHandler( PEXCEPTION_POINTERS pExceptionInfo );

