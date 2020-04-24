using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Text;
using ERSMService.Main;

namespace ERSMService
{
    public partial class ERSM : ServiceBase
    {
        public ERSM()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            ERSMMain ersmMain = new ERSMMain();
            ersmMain.Start();
        }

        protected override void OnStop()
        {
            // TODO: 서비스를 중지하는 데 필요한 작업을 수행하는 코드를 여기에 추가합니다.
        }
    }
}
