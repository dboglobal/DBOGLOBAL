namespace Babidi
{
    /// <summary>
    /// Babidi_Log에 출력될 타입
    /// </summary>
    enum eLogType
    {
        eLOGTYPE_FILE = 0,          // FILE 처리에서 에러
        eLOGTYPE_ID,                // ID   관련 에러
        eLOGTYPE_LINE_OVER,         // LINE OVER시 에러
        eLOGTYPE_LENGTH_OVER,       // LENGTH OVER시 에러
        eLOGTYPE_CONFIG,            // CONFIG 에서의 오류
        eLOGTYPE_EDIT,              // 편집 상에서의 오류

        eLOGTYPE_NUMS,
        eLOGTYPE_INVALID = 0xFF
    }
}