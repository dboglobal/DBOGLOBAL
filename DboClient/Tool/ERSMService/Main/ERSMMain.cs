using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace ERSMService.Main
{
    class ERSMMain
    {
        ERSMConfig ersmConfig = new ERSMConfig();
        DateTime dtPrevRun;

        public void Start()
        {            
            Console.WriteLine("--- ERSM Service Start ----");

            // Config 파일을 Load 한다.
            if(ersmConfig.LoadConfig(ERSMConfig.configFileName))
            {
                Console.WriteLine("ERSM Config File Read Success...");
            }
            else
            {
                Console.WriteLine("ERSM Config File Read Fail!!");
                return;
            }

            Thread mainThread = new Thread(this.MainLoop);
            mainThread.IsBackground = true;
            mainThread.Start();            
        }
        
        /// <summary>
        /// 무한 루프를 돌면서 시간이 되면 스레드를 작동시킨다.
        /// </summary>
        private void MainLoop()
        {
            // 처음 시작할때 한번은 무조건 돌린다.            
            dtPrevRun = DateTime.Now;

            DateTime dtCurrent = DateTime.Now;
            ERSMWorker ersmWorker = new ERSMWorker();
            ersmWorker.StartWork();

            while(true)
            {
                dtCurrent = DateTime.Now;
                TimeSpan tsGap = dtCurrent - dtPrevRun;
                if(tsGap.Minutes >= ERSMConfig.ERSMRunTime)
                {
                    Thread ersmWorkerThread = new Thread(ersmWorker.StartWork);
                    ersmWorkerThread.IsBackground = true;
                    ersmWorkerThread.Start();
                }

                // 1분 단위로 돈다
                Thread.Sleep(1000 * 60);
            }
        }
    }
}
