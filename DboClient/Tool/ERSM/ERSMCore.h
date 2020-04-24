#pragma once

NCDeclareSGT(ERSMCore);

/**
 * \ingroup ERSManager
 * \brief ERSManager의 메인 처리를 하는 Core 클래스
 * \date 2007-01-26
 * \author agebreak
 */
class ERSMCore
{
public:
    ERSMCore(void);
    virtual ~ERSMCore(void);

    bool   Init();                                          ///< 초기화 작업을 수행한다.
    void   Destroy();                                       ///< 마무리 작업을 수행한다.
    bool   Run();                                           ///< 스레드를 생성하여, 작업을 수행한다.

protected:
	static DWORD WINAPI WorkThread(LPVOID pParam);			///< 실제 작업을 수행하는 스레드
	std::string GetModulePath();							///< 현재 서비스 프로그램 파일의 경로를 가져온다.

public:
	static bool	m_bEndFlag;									///< 스레드를 끝내기 위한 플래그
	static CTime m_prevRunTime;

};
