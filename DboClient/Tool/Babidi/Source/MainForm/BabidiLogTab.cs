using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Text;
using System.IO;

namespace Babidi
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Babidi.eLogType 을 사용하는 문자열
        /// </summary>
        String[] m_strType;

        /// <summary>
        /// Babidi Log를 초기화하고 관련된 멤버나 자원을 초기화한다.
        /// </summary>
        protected void InitBabidiLog()
        {
            // TYPE을 String으로 정의
            m_strType = new String[(int)eLogType.eLOGTYPE_NUMS];

            m_strType[(int)eLogType.eLOGTYPE_FILE]  = "FILE ERROR";
            m_strType[(int)eLogType.eLOGTYPE_ID]    = "ID MISS";
            m_strType[(int)eLogType.eLOGTYPE_LINE_OVER] = "LINE OVER";
            m_strType[(int)eLogType.eLOGTYPE_LENGTH_OVER] = "LENGTH OVER";
            m_strType[(int)eLogType.eLOGTYPE_CONFIG] = "CONFIG";
            
            // 바비디 로그의 리스트 뷰의 스타일을 정의한다.
            listLog.View = View.Details;
            listLog.Dock = DockStyle.Fill;
            listLog.LabelEdit = false;
            listLog.GridLines = true;
            
            int nWidth = tabLocalize.Width;

            listLog.Columns.Add("ID", (int)((float)nWidth * 0.03f), HorizontalAlignment.Center);
            listLog.Columns.Add("TIME", (int)((float)nWidth * 0.15f), HorizontalAlignment.Center);
            listLog.Columns.Add("TYPE", (int)((float)nWidth * 0.07f), HorizontalAlignment.Center);
            listLog.Columns.Add("PLACE", (int)((float)nWidth * 0.15f), HorizontalAlignment.Center);
            listLog.Columns.Add("LOG", (int)((float)nWidth * 0.58f), HorizontalAlignment.Center);

            listLog.Resize += new EventHandler(OnResizeListLog);
        }

        /// <summary>
        /// 리스트뷰의 컨트롤이 리사이즈 될 때 발생하는 이벤트 함수
        /// </summary>
        /// <param name="sender">이벤트를 보내는 객체</param>
        /// <param name="e">이벤트의 구조체</param>
        void OnResizeListLog(object sender, EventArgs e)
        {
            //int nWidth = tabLocalize.Width;

            //listLog.Columns[0].Width = (int)((float)nWidth * 0.03f);
            //listLog.Columns[1].Width = (int)((float)nWidth * 0.15f);
            //listLog.Columns[2].Width = (int)((float)nWidth * 0.07f);
            //listLog.Columns[3].Width = (int)((float)nWidth * 0.15f);
            //listLog.Columns[4].Width = (int)((float)nWidth * 0.58f);
        }

        /// <summary>
        /// Log를 남긴 Index
        /// </summary>
        protected int m_nLogIndex = 0;

        /// <summary>
        /// Babidi_Log에 Log를 기록한다.
        /// </summary>
        /// <param name="nType">타입의 문자열</param>
        /// <param name="strPlace">장소의 문자열</param>
        /// <param name="strLog">로그의 문자열</param>
        public void AddLog(int nType, string strPlace, string strLog)
        {
            // ID 로 리스트 아이템 생성
            String strID = String.Format("{0}", m_nLogIndex);
            ListViewItem lvi_log = new ListViewItem(strID);

            // 현재시간
            DateTime nowTime = DateTime.Now;
            lvi_log.SubItems.Add(nowTime.ToString());

            // TYPE
            string strFileLog = String.Format("{0,3} - {1} : ", m_nLogIndex, DateTime.Now.ToShortTimeString() );
            if (nType > m_strType.Length - 1)
            {
                lvi_log.SubItems.Add("UNKNOWN");
                strFileLog += "UNKNOWN : ";
            }
            else
            {
                lvi_log.SubItems.Add(m_strType[nType]);
                strFileLog += m_strType[nType] + " : ";
            }

            // PLACE
            lvi_log.SubItems.Add(strPlace);
            strFileLog += "[" + strPlace + "] ";

            // Log
            lvi_log.SubItems.Add(strLog);
            strFileLog += strLog;

            // 리스트뷰에 아이템 추가
            listLog.Items.Add(lvi_log);

            // File에 쓴다.
            string strFileName =  String.Format("BabidiLog{0}.txt", DateTime.Today.ToShortDateString() );
            AddFileLog(strFileName, strFileLog);

            if (nType.CompareTo((int)eLogType.eLOGTYPE_FILE) == 0)
                MessageBox.Show(strPlace + strLog, m_strType[nType].ToString());

            m_nLogIndex++;

            FocusTab();
        }

        public void AddFileLog(string strFile, string strLog)
        {
            try
            {
                using (FileStream fs = new FileStream(strFile, FileMode.Append, FileAccess.Write))
                {
                    using (StreamWriter sw = new StreamWriter(fs, Encoding.Unicode))
                    {
                        sw.WriteLine("{0}", strLog);
                        sw.Close();
                    }
                }
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }

        /// <summary>
        /// Baibidi_Log Tab이 비활성화 되어 있다면 활성화 시켜준다.
        /// </summary>
        public void FocusTab()
        {
            //tabLocalize.TabPages["tabLog"].BackColor = Color.Red;
            tabLocalize.SelectTab("tabLog");
        }
    }
}
