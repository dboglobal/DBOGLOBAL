using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Babidi
{
    /// <summary>
    /// 메인 윈도우의 클래스
    /// </summary>
    public partial class MainForm : Form
    {
        /// <summary>
        /// Babidi의 환경 설정 파일
        /// </summary>
        protected const String CONFIG_FILE = @"Babidi.ini";
        protected const String STYLE_SECTION = "STYLE";
        
        // CLIENT와 SERVER의 동일 헤더
        protected const String COL_ID = "ID";
        protected const String COL_DEFINE = "DEFINE";    
        protected const String COL_COMMENT = "COMMENT";
        protected const String COL_KOR = "KOR";
        protected const String COL_JPN = "JPN";
        protected const String COL_ALARMTYPE = "ALARM";
        protected const String COL_NOTE = "NOTE";
        protected const String COL_MB = "MSGBOX";
        protected const String COL_HTML = "HTML";
        protected const String COL_LINELENGTH = "LINE/LENGTH";
        protected const String COL_STATE = "STATE";
        
        // String형 Sort용 Column
        protected const String COL_SORT_STRING = "SORT_STRING";

        // Int형 Sort용 Column
        protected const String COL_SORT_INT = "SORT_INT";

        /// <summary>
        /// Form의 생성자
        /// </summary>
        public MainForm()
        {
            InitializeComponent();

            InitConfigTab(CONFIG_FILE);

            InitDataManager();
            
            // Babidi tab Init
            InitBabidiLog();

            this.FormClosing += new FormClosingEventHandler(MainForm_FormClosing);
            dataGridClient.KeyDown += new KeyEventHandler(dataGridClient_KeyDown);
            dataGridServer.KeyDown += new KeyEventHandler(dataGridServer_KeyDown);            
        }

        void dataGridServer_KeyDown(object sender, KeyEventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            try
            {
                //throw new Exception("The method or operation is not implemented.");
                // 붙여넣기 구현
                if (e.Control && e.KeyCode == Keys.V)
                {
                    String strClip = Clipboard.GetText();

                    // Select된 곳 검사
                    if (dataGridServer.DataSource == null)
                        return;

                    DataGridViewCell cells = dataGridServer.CurrentCell;
                    if (cells == null)
                        return;

                    String[] strToken = { "\r\n" };
                    String[] strClips = strClip.Split(strToken, StringSplitOptions.RemoveEmptyEntries);

                    int nRowStart = cells.RowIndex;
                    int nCol = cells.ColumnIndex;

                    for (int i = 0; i < strClips.Length; ++i)
                    {
                        if (dataGridServer.Rows.Count <= nRowStart)
                            return;

                        if (dataGridServer.Rows[nCol].ReadOnly)
                            return;

                        DataGridViewRow row = dataGridServer.Rows[nRowStart];
                        DataRowView viewRow = (DataRowView)row.DataBoundItem;
                        viewRow.Row[nCol] = strClips[i];

                        nRowStart++;
                    }
                }
                else if (e.Control && e.KeyCode == Keys.F)
                {
                    if (dataGridServer.DataSource == null)
                        return;

                    DataGridViewCell cells = dataGridServer.CurrentCell;
                    if (cells == null)
                        return;

                    if (m_findForm == null)
                    {
                        Babidi.SearchDelegate delSearch = new Babidi.SearchDelegate(this.SearchCell);
                        m_findForm = new Babidi.Find(delSearch);

                        m_findForm.FormClosed += new FormClosedEventHandler(m_findForm_FormClosed);

                        m_findForm.Show();
                    }
                    else
                    {
                        m_findForm.Show();
                        m_findForm.Focus();
                    }
                }
            }
            catch (System.Exception ex)
            {
                AddLog((int)eLogType.eLOGTYPE_EDIT, dataGridServer.ToString(), ex.Message);
                return;
            }
        }

        void dataGridClient_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
              if (e.Control && e.KeyCode == Keys.V)
                {
                    String strClip = Clipboard.GetText();

                    // Select된 곳 검사
                    if (dataGridClient.DataSource == null)
                        return;

                    //dataGridClient.ClipboardCopyMode = DataGridViewClipboardCopyMode.
                    DataGridViewCell cells = dataGridClient.CurrentCell;
                    if (cells == null)
                        return;

                    String[] strToken = { "\r\n" };
                    String[] strClips = strClip.Split(strToken, StringSplitOptions.RemoveEmptyEntries);

                    int nRowStart = cells.RowIndex;
                    int nCol = cells.ColumnIndex;

                    for (int i = 0; i < strClips.Length; ++i)
                    {
                        if (dataGridClient.Rows.Count <= nRowStart)
                            return;

                        if (dataGridClient.Rows[nCol].ReadOnly)
                            return;

                        DataGridViewRow row = dataGridClient.Rows[nRowStart];
                        DataRowView viewRow = (DataRowView)row.DataBoundItem;
                        viewRow.Row[nCol] = strClips[i];

                        nRowStart++;
                    }
                }
                else if (e.Control && e.KeyCode == Keys.F)
                {
                    if (dataGridClient.DataSource == null)
                        return;

                    DataGridViewCell cells = dataGridClient.CurrentCell;
                    if (cells == null)
                        return;

                    if (m_findForm == null)
                    {
                        Babidi.SearchDelegate delSearch = new Babidi.SearchDelegate(this.SearchCell);
                        m_findForm = new Babidi.Find(delSearch);

                        m_findForm.FormClosed += new FormClosedEventHandler(m_findForm_FormClosed);

                        m_findForm.Show();
                    }
                    else
                    {
                        m_findForm.Show();
                        m_findForm.Focus();
                    }
                }
            }
            catch (System.Exception ex)
            {
                AddLog((int)eLogType.eLOGTYPE_EDIT, dataGridClient.ToString(), ex.Message);
                return;
            }
            
        }

        void m_findForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            m_findForm = null;
        }

 

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 0x302:
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }

        void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            foreach (DataGridViewColumn col in dataGridClient.Columns)
            {
                CIniManager.G_IniWriteValue(STYLE_SECTION, "CLIENT_COL" + Convert.ToString(col.Index), Convert.ToString(col.Width), CONFIG_FILE);
            }

            foreach (DataGridViewColumn col in dataGridServer.Columns)
            {
                CIniManager.G_IniWriteValue(STYLE_SECTION, "SERVER_COL" + Convert.ToString(col.Index), Convert.ToString(col.Width), CONFIG_FILE);
            }
        }



        protected void LoadStyle(DATA_SYNC eSync)
        {
            if (eSync == DATA_SYNC.DATA_CLIENT)
            {
                foreach (DataGridViewColumn col in dataGridClient.Columns)
                {
                    String strConfig = CIniManager.G_IniReadValue(STYLE_SECTION, "CLIENT_COL" + Convert.ToString(col.Index), CONFIG_FILE);
                    if( strConfig.CompareTo("") == 0 )
                    {
                        col.Width = 100;
                    }
                    else 
                    {
                        col.Width = Convert.ToInt32(strConfig);
                    }
                }
            }
            else if (eSync == DATA_SYNC.DATA_SERVER)
            {
                foreach (DataGridViewColumn col in dataGridServer.Columns)
                {
                    String strConfig = CIniManager.G_IniReadValue(STYLE_SECTION, "SERVER_COL" + Convert.ToString(col.Index), CONFIG_FILE);
                    if (strConfig.CompareTo("") == 0)
                    {
                        col.Width = 100;
                    }
                    else
                    {
                        col.Width = Convert.ToInt32(strConfig);
                    }
                }
            }
        }

        public void CellColorChange(DataGridViewRow rows, Color foreColor, Color backColor)
        {
            foreach (DataGridViewCell cells in rows.Cells)
            {
                cells.Style.BackColor = backColor;
                cells.Style.ForeColor = foreColor;
            }
        }

        public void SetRowColor(int nRowIndex, Color foreColor, Color backColor)
        {
            if (nRowIndex > dataGridClient.Rows.Count - 1)
                return;

            DataGridViewRow rows = dataGridClient.Rows[nRowIndex];

            foreach (DataGridViewCell cells in rows.Cells)
            {
                cells.Style.BackColor = foreColor;
                cells.Style.ForeColor = backColor;
            }
        }

        private void MENU_FILE_SAVE_Click(object sender, EventArgs e)
        {
            if (dataGridClient.DataSource != null)
            {
                SaveClientData();
            }
            else
            {
                MessageBox.Show("현재 작업중인 Client Data가 없습니다.", "Save");
            }

            if (dataGridServer.DataSource != null)
            {
                SaveServerData();
            }
            else
            {
                MessageBox.Show("현재 작업중인 Server Data가 없습니다.", "Save");
            }
        }

        private void EDIT_MSGBOX_Click(object sender, EventArgs e)
        {
            Babidi.CFormAlarm formAlarm = new CFormAlarm(this.SetAlarmType);

           formAlarm.ShowDialog();
        }

        private void SORT_COLUMN_Click(object sender, EventArgs e)
        {
            if (dataGridClient.Visible)
                SortCurSellFromView( ref dataGridClient, ref m_tblClient, true );
            else
                SortCurSellFromView(ref dataGridServer, ref m_tblServer, true);
        }

        private void DECENDING_SORT_COLUMN_Click(object sender, EventArgs e)
        {
            if (dataGridClient.Visible)
                SortCurSellFromView(ref dataGridClient, ref m_tblClient, false);
            else
                SortCurSellFromView(ref dataGridServer, ref m_tblServer, false);
        }

        private void SortCurSellFromView(ref DataGridView view, ref DataTable tbl, bool bAscending)
        {
            if (view.DataSource == null)
                return;

            DataGridViewCell cells = view.CurrentCell;
            if (cells == null)
                return;

            int nCol = cells.ColumnIndex;

            if (nCol != 0)
            {
                foreach (DataRow row in tbl.Rows)
                    row[COL_SORT_STRING] = row[nCol].ToString();

                if (bAscending)
                    view.Sort(view.Columns[COL_SORT_STRING], ListSortDirection.Ascending);
                else
                    view.Sort(view.Columns[COL_SORT_STRING], ListSortDirection.Descending);
            }
            else
            {
                foreach (DataRow row in tbl.Rows)
                    row[COL_SORT_INT] = Convert.ToInt32(row[nCol].ToString());

                if (bAscending)
                    view.Sort(view.Columns[COL_SORT_INT], ListSortDirection.Ascending);
                else
                    view.Sort(view.Columns[COL_SORT_INT], ListSortDirection.Descending);
            }
        }

        private void MENU_FILE_EXIT_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnSaveClientAlarm_Click(object sender, EventArgs e)
        {
          if (dataGridClient.DataSource == null)
          {
              MessageBox.Show( "현재 작업중인 Alarm Data가 없습니다." );
              return;
          }

            SaveData(DATA_TYPE.DATA_TYPE_ALARM, ref m_tblClient, m_strClientAlarm);
        }

        private void btnSaveClientKor_Click(object sender, EventArgs e)
        {
            if (dataGridClient.DataSource == null)
            {
                MessageBox.Show( "현재 작업중인 Client korean data 가 없습니다." );
                return;
            }

            SaveData(DATA_TYPE.DATA_TYPE_KOR, ref m_tblClient, m_strClientKor);
        }

        private void btnSaveClientJap_Click(object sender, EventArgs e)
        {
            if (dataGridClient.DataSource == null)
            {
                MessageBox.Show( "현재 작업중인 Client japanese data 가 없습니다." );
                return;
            }

            SaveData(DATA_TYPE.DATA_TYPE_JPN, ref m_tblClient, m_strClientJpn);
        }

        private void btnSaveClientUser_Click(object sender, EventArgs e)
        {
            if(dataGridClient.DataSource == null)
            {
                MessageBox.Show( "현재 작업중인 Client User data 가 없습니다." );
                return;
            }


            SaveData(DATA_TYPE.DATA_TYPE_USER, ref m_tblClient, m_strClientUser);
        }

        private void btnSaveClientAll_Click(object sender, EventArgs e)
        {
            if (dataGridClient.DataSource == null)
            {
                MessageBox.Show("현재 작업중인 Client data 가 없습니다.");
                return;
            }

            SaveClientData();
        }

        private void btnSaveServerAlarm_Click(object sender, EventArgs e)
        {
            if (dataGridServer.DataSource == null)
            {
                MessageBox.Show("현재 작업중인 Server alarm data 가 없습니다.");
                return;
            }

            SaveData(DATA_TYPE.DATA_TYPE_ALARM, ref m_tblServer, m_strServerAlarm);
        }

        private void btnSaveServerKor_Click(object sender, EventArgs e)
        {
            if (dataGridServer.DataSource == null)
            {
                MessageBox.Show("현재 작업중인 Server korean data 가 없습니다.");
                return;
            }

            SaveData(DATA_TYPE.DATA_TYPE_KOR, ref m_tblServer, m_strServerKor);
            
        }

        private void btnSaveServerJap_Click(object sender, EventArgs e)
        {
            if (dataGridServer.DataSource == null)
            {
                MessageBox.Show("현재 작업중인 Server japanese data 가 없습니다.");
                return;
            }

            SaveData(DATA_TYPE.DATA_TYPE_JPN, ref m_tblServer, m_strServerJpn);
        }

        private void btnSaveServerUser_Click(object sender, EventArgs e)
        {
            if (dataGridServer.DataSource == null)
            {
                MessageBox.Show("현재 작업중인 Server user data 가 없습니다.");
                return;
            }

            SaveData(DATA_TYPE.DATA_TYPE_USER, ref m_tblServer, m_strServerUser);
        }

        private void btnSaveServerAll_Click(object sender, EventArgs e)
        {
            if (dataGridServer.DataSource != null)
            {
                MessageBox.Show("현재 작업중인 Server data 가 없습니다.");
                return;
            }

            SaveServerData();
        }
    }
}