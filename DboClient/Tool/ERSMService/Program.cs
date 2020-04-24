using System.Collections.Generic;
using System.ServiceProcess;
using System.Text;
using ERSMService.Main;

namespace ERSMService
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        static void Main()
        {
            ServiceBase[] ServicesToRun;

            // 동일한 프로세스 내에서 사용자 서비스가 두 개 이상 실행될 수 있습니다.
            // 이 프로세스에 다른 서비스를 추가하려면 다음 줄을 변경하여 두 번째
            // 서비스 개체를 만듭니다. 예를 들면 다음과 같습니다.
            //
            //   ServicesToRun = new ServiceBase[] {new Service1(), new MySecondUserService()};
            //            
            ServicesToRun = new ServiceBase[] { new ERSM() };

            ServiceBase.Run(ServicesToRun);

            //ERSMMain main = new ERSMMain();
            //main.Start();
        }      
        
    }
}