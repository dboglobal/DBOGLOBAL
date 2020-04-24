using System;
using System.Collections.Generic;
using System.Text;

namespace ICSLauncher
{
    class ICSErrorCode
    {
        public const int ERR_NOT_CONNECT_SERVER = 101;          // 서버에 연결할 수 없습니다.
        public const int ERR_CONFIG_LOAD_FAIL = 102;            // 컨피그 파일을 읽을 수 없습니다.
        public const int ERR_DISCONNECT_SERVER = 103;           // 서버 연결이 강제로 끊겼습니다.

        public const int ERR_NOT_FIND_LAUNCHER = 201;           // 런처 파일을 찾을 수 없습니다.
    }
}
