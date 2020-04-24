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

protected:
    void   Init();                                          ///< 초기화 작업을 수행한다.
    void   Destroy();                                       ///< 마무리 작업을 수행한다.
    void   Run();                                           ///< 스레드를 생성하여, 작업을 수행한다.
};
