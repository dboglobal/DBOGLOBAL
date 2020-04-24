using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.ComponentModel;

namespace ICSLauncher
{
    class ICSServer
    {
        private TcpListener server = null;
        private Socket      client = null;
        private NetworkStream stream = null;
        private StreamReader  reader = null;
        private StreamWriter writer = null;

        private string launcherPath;
        
        public ICSServer()
        {

        }   
     
         ~ICSServer()
        {
            if(server != null)
            {
                server.Stop();
            }
        }

        public void Close()
        {
            reader.Close();
            writer.Close();
            stream.Close();
            client.Close();

            Console.WriteLine("Client Disconnect");
        }

        public void Run(int port, string launcherPath)
        {
            this.launcherPath = launcherPath;            
            Accept(port);
        }

        private void Accept(int port)
        {
            try
            {
                IPAddress MyIP = Dns.GetHostAddresses(Dns.GetHostName())[0];

                server = new TcpListener(MyIP, port);
                server.Start();

                Console.WriteLine("--- ICS Server Start ---");
            }
            catch(Exception ex)
            {
                Console.WriteLine("Server Create Fail : " + ex.StackTrace);                
            }
            
            while(true)
            {
                client = server.AcceptSocket();
                client.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.KeepAlive, 1000 * 60 * 120);

                if (client.Connected)
                {
                    Console.WriteLine("Client Connect - " + IPAddress.Parse(((IPEndPoint)client.RemoteEndPoint).Address.ToString()));

                    stream = new NetworkStream(client);
                    reader = new StreamReader(stream);
                    writer = new StreamWriter(stream);
                    writer.AutoFlush = true;
                }

                Thread thread = new Thread(new ThreadStart(Receive));
                thread.Start();
            }                        
        }

        // 클라이언트로 부터 받은 패킷을 처리한다.
        private void Receive()
        {
            try
            {
                while (true)
                {
                    if(client.Connected == false)       // 클라이언트와 소켓이 끊겼을때
                    {
                        Close();
                        break;
                    }

                    string message = reader.ReadLine().Trim();
                    if (message == null)
                        continue;

                    char[] ch = { '#' };
                    string[] token = message.Split(ch);
                    switch (token[0])
                    {
                        case ICSPacket.C_REQ_LAUNCER_EXECUTE:
                            {
                                // 런처를 실행한다.
                                ExecuteLauncher(launcherPath);                                                                
                            }
                            break;
                    }
                }
            }
            catch   // 클라이언트와 통신 실패 or 끊기
            {
                Close();
            }            
        }

        private int ExecuteLauncher(string launcherPath)
        {
            try
            {
                string path = Path.GetDirectoryName(launcherPath);
                if(path != "")
                {
                    System.Environment.CurrentDirectory = path;
                }
                
                // 인자를 줘서 런처를 실행한다.                                
                System.Diagnostics.Process process = new System.Diagnostics.Process();
                process.StartInfo.FileName = launcherPath;
                process.StartInfo.Arguments = "CHECKSUM";
                if(process.Start())
                {
                    Console.WriteLine("DBO Launcher Execute Success - " + launcherPath);
                }
                else                
                {
                    Console.WriteLine("[ErrCode {0}] Can't Start Launcher - " + launcherPath, ICSErrorCode.ERR_NOT_FIND_LAUNCHER);
                    writer.WriteLine(ICSErrorCode.ERR_NOT_FIND_LAUNCHER.ToString());                    
                    
                    return ICSErrorCode.ERR_NOT_FIND_LAUNCHER;
                }

                // 런처가 종료될때까지 대기한다.
                process.WaitForExit();
                if(process.ExitCode == 0)
                {
                    Console.WriteLine("Launcher Integrity Build Process Success");
                }
                else
                {
                    Console.WriteLine("[ErrCode {0}] Launcher Integrity Build Process Fail", process.ExitCode);
                }

                writer.WriteLine(process.ExitCode.ToString());                
            }
            catch (Win32Exception ex)
            {
                Console.WriteLine("[ErrCode {0}] Can't Start Launcher :" + launcherPath + "\n" + ex.StackTrace, ICSErrorCode.ERR_NOT_FIND_LAUNCHER);
                writer.WriteLine(ICSErrorCode.ERR_NOT_FIND_LAUNCHER.ToString());
                return ICSErrorCode.ERR_NOT_FIND_LAUNCHER;
            }

            return 0;
        }
    }
}
