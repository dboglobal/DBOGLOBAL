using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Text;
using System.IO;
using System.Reflection;
using System.Xml;

namespace ICSService
{
    public partial class Service1 : ServiceBase
    {
        public int serverPort;        

        public Service1()
        {
            InitializeComponent();
        }

        public void Test()
        {
            if (LoadConfig("ICSCOnfig.xml") == false)
            {
                return;
            }

            ICSServer server = new ICSServer();
            server.Run(serverPort);            
        }

        protected override void OnStart(string[] args)
        {
            if (LoadConfig("ICSCOnfig.xml") == false)
            {
                return;
            }

            ICSServer server = new ICSServer();
            server.Run(serverPort);            
        }

        protected override void OnStop()
        {
            // TODO: 서비스를 중지하는 데 필요한 작업을 수행하는 코드를 여기에 추가합니다.
        }

        /// <summary>
        /// XML 파일로부터 컨피그 설정을 읽어와서 세팅한다.
        /// </summary>        
        private bool LoadConfig(string configFile)
        {
            try
            {
                // 서비스에서 설치 폴더명을 읽어오는 코드
                string currentFolder = Path.GetDirectoryName(Assembly.GetAssembly(typeof(Service1)).CodeBase);
                currentFolder = currentFolder.Replace("file:\\", "");

                XmlTextReader reader = new XmlTextReader(currentFolder + @"\" + configFile);
                reader.WhitespaceHandling = WhitespaceHandling.None;

                while (reader.Read())
                {
                    reader.MoveToContent();

                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:                            
                            if (reader.Name == "SERVER")
                            {
                                this.serverPort = Convert.ToInt32(reader.GetAttribute("PORT"));
                            }
                            else if (reader.Name == "LAUNCHER")
                            {
                                ICSLauncherMap.SetLauncher(reader.GetAttribute("id"), reader.GetAttribute("path"));
                            }
                            break;
                    }
                }
            }
            catch
            {
                Console.WriteLine("[Error] Config File Load Fail. - " + configFile);

                return false;
            }

            Console.WriteLine("Config File Read Success...");

            return true;
        }        
    }
}
