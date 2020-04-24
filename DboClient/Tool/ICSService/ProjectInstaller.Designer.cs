namespace ICSService
{
    partial class ProjectInstaller
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.Service1 = new System.ServiceProcess.ServiceProcessInstaller();
            this.ICSServer = new System.ServiceProcess.ServiceInstaller();
            // 
            // Service1
            // 
            this.Service1.Account = System.ServiceProcess.ServiceAccount.LocalService;
            this.Service1.Password = null;
            this.Service1.Username = null;
            // 
            // ICSServer
            // 
            this.ICSServer.ServiceName = "ICSServer";
            this.ICSServer.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.Service1,
            this.ICSServer});

        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller Service1;
        private System.ServiceProcess.ServiceInstaller ICSServer;
    }
}