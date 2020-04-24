using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

namespace ICSLauncher
{
    class Program
    {
        public string mode;
        public string serverIP;
        public int serverPort;        

        static public string keyId;

        static int Main(string[] args)
        {
            Program program = new Program();
            int errCode = 0;

            if (program.LoadConfig("ICSCOnfig.xml") == false)
            {
                return ICSErrorCode.ERR_CONFIG_LOAD_FAIL;
            }

            program.mode = program.mode.ToUpper();
            if(program.mode == "CLIENT")
            {
                ICSClient client = new ICSClient();
                errCode = client.Run(program.serverIP, program.serverPort);
                if(errCode == 0)
                {
                    Console.WriteLine("Server Integrity Check System Build Success!");
                }
                else
                {
                    Console.WriteLine("[ErrCode {0}] Server Integrity Check System Build Fail.", errCode);
                }
            }            

            System.Environment.ExitCode = errCode;

            return errCode;
        }

        /// <summary>
        /// XML 파일로부터 컨피그 설정을 읽어와서 세팅한다.
        /// </summary>        
        private bool LoadConfig(string configFile)
        {
            try
            {
                XmlTextReader reader = new XmlTextReader("ICSConfig.xml");
                reader.WhitespaceHandling = WhitespaceHandling.None;

                while (reader.Read())
                {
                    reader.MoveToContent();

                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:
                            if (reader.Name == "MODE")
                            {
                                reader.Read();
                                this.mode = reader.Value;
                            }
                            else if (reader.Name == "SERVER")
                            {
                                this.serverIP = reader.GetAttribute("IP");
                                this.serverPort = Convert.ToInt32(reader.GetAttribute("PORT"));
                            }                            
                            else if(reader.Name == "KEY")
                            {
                                Program.keyId = reader.GetAttribute("id");
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
            Console.WriteLine("Launcher Key ID = " + Program.keyId);

            return true;
        }        
    }
}
