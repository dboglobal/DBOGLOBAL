using System;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using System.Windows.Forms;
using System.IO;


namespace Babidi
{
    /// <summary>
    /// WinAPI인 GetPrivateProfileString과 WritePrivateprofileString 함수를 사용하기 위한 매니저 클래스
    /// </summary>
    public class CIniManager
    {
        // ---- ini 파일 의 읽고 쓰기를 위한 API 함수 선언 ----
        [DllImport("kernel32.dll")]
        private static extern int GetPrivateProfileString(    // ini Read 함수
                    String section,
                    String key,
                    String def,
                    StringBuilder retVal,
                    int size,
                    String filePath);

        [DllImport("kernel32.dll")]
        private static extern long WritePrivateProfileString(  // ini Write 함수
                    String section,
                    String key,
                    String val,
                    String filePath);

        /// <summary>
        /// INI 파일에 쓰기
        /// </summary>
        /// <param name="Section">레이블</param>
        /// <param name="Key">키값</param>
        /// <param name="Value">값</param>
        /// <param name="avsPath">INI 파일</param>
        public static void G_IniWriteValue(String Section, String Key, String Value, string avsPath)
        {
            try
            {
                String strPath = Application.StartupPath + "\\" + avsPath;

                WritePrivateProfileString(Section, Key, Value, strPath);
            }
            catch (System.Exception e)
            {
                // TODO : LogMessage
                MessageBox.Show(e.Message);
                return;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="Section"></param>
        /// <param name="Key"></param>
        /// <param name="avsPath"></param>
        /// <returns></returns>
        public static String G_IniReadValue(String Section, String Key, string avsPath)
        {
            try
            {
                String strPath = Application.StartupPath + "\\" + avsPath;

                StringBuilder temp = new StringBuilder(2000);
                int i = GetPrivateProfileString(Section, Key, "", temp, 2000, strPath);

                return temp.ToString();
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.Message);
                // Todo : LogMeessage
                return null;
            }
        }
    }
}