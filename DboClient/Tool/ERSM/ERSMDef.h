#pragma once

#define CONFIG_FILE_NAME    "ERSMConfig.xml"      

#define ERSM_ID_FILE_NAME	"DboErrorInfo.xml"			///< DBO의 유저ID 정보를 가지고 있는 로그 파일이름

#define RCAST				reinterpret_cast
#define CCAST				const_cast

#define ERSM_BUF_SIZE		64							///< 기본 버퍼 사이즈 
#define ERSM_FILE_BUF_SIZE	4096						///< 파일 버퍼 사이즈

#define ERSM_NODE_USER_ID	"EXTRA_ERROR_INFO/USER_ID"	///< 유저 ID가 담겨있는 노드
