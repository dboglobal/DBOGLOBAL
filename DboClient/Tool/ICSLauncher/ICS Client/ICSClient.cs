using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;

namespace ICSLauncher
{
    class ICSClient
    {
        private TcpClient client = null;
        private NetworkStream stream = null;
        private StreamReader reader = null;
        private StreamWriter writer = null;

        public ICSClient()
        {
        }

        ~ICSClient()
        {
        }

        public int Run(string ip, int port)
        {
            int errCode = 0;
            errCode = Connect(ip, port);
            if (errCode == 0)
            {
                errCode = SendLauncherExcute();
                Close();
            }
            
            return errCode;
        }

        private void Close()
        {
            if(reader != null)
            {
                reader.Close();
                writer.Close();
                stream.Close();
                client.Close();
            }            
        }

        private int Connect(string ip, int port)
        {
            try
            {
                client = new TcpClient(ip, port);                
                stream = client.GetStream();
                reader = new StreamReader(stream);
                writer = new StreamWriter(stream);
                writer.AutoFlush = true;
            }
            catch(SocketException ex)
            {
                Console.WriteLine("[ErrCode {0}] Server Connect Fail (Socket Error Code " + ex.Message, ICSErrorCode.ERR_NOT_CONNECT_SERVER);

                return ICSErrorCode.ERR_NOT_CONNECT_SERVER;
            }

            Console.WriteLine("Server Connect Success ! - " + ip);
            
            return 0;
        }

        /// <summary>
        /// 런처 실행을 서버에 요청한다.
        /// </summary>
        /// <returns>에러코드. 0이면 성공</returns>
        private int SendLauncherExcute()
        {
            writer.WriteLine(ICSPacket.C_REQ_LAUNCER_EXECUTE + "#" + Program.keyId);            

            try
            {
                string message = reader.ReadLine();

                return Convert.ToInt32(message);
            }
            catch
            {
                // 서버 연결이 끊겼음
                return ICSErrorCode.ERR_DISCONNECT_SERVER;
            }            
        }       
    }
}
