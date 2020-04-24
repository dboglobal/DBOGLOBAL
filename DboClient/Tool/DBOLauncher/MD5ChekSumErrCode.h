#pragma once

/// 체크섬 클래스 에러 코드
#define CHECKSUM_SUCCESS                            0
#define CHECKSUM_NOT_CREATE_CHEKSUMFILE             301             ///< 체크섬 파일을 생성할 수 없음
#define CHECKSUM_FAILED_TO_OPEN_FILE                302             ///< 타겟 파일을 열 수 없음
#define CHECKSUM_FAILED_TO_READ_FILE                303             ///< 타겟 파일을 읽을 수 없음
#define CHECKSUM_FAILED_TO_OPEN_MD5                 304             ///< MD5 체크섬 리스트 파일을 열수 없음
#define CHECKSUM_FAILED_WRONG_CHECKSUM              305             ///< 체크섬 결과가 다름 
#define CHECKSUM_SYSTEM_ERROR						306             ///< 
#define CHECKSUM_USER_BREAK							307             ///< 
