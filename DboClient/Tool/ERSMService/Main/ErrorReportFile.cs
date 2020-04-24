using System;
using System.Collections.Generic;
using System.Text;

namespace ERSMService.Main
{
    /// <summary>
    /// 에러 리포트 파일 구조체
    /// </summary>
    class ErrorReportFile
    {
        public string folderName;           // 파일이 위치한 폴더명                                            
        public string fileName;             // 파일명 (폴더명 제외)                                    
        public string userID;               // 유저 ID
        public string fileDate;             // 파일 생성 날짜                                    
    }
}
