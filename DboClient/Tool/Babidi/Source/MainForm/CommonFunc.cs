using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Windows.Forms;
using System.IO;
using System.Drawing;

namespace Babidi
{
    public partial class MainForm : Form
    {
        protected String[]                      m_aDataState;
        protected Babidi.Find     m_findForm = null;

        public enum DATA_SYNC
        {
            DATA_CLIENT,
            DATA_SERVER,

            DATA_NUMS, 
            DATA_INVALID = 0xFF
        }

        public enum DATA_TYPE
        {
            DATA_TYPE_DEF,
            DATA_TYPE_ALARM,
            DATA_TYPE_KOR,
            DATA_TYPE_JPN,
            DATA_TYPE_USER,

            DATA_TYPE_NUMS,
            DATA_TYPE_INVALID = 0xFF
        }

        public enum DATA_STATE
        {
            DATA_STATE_NORMAL,                  // 정상
            DATA_STATE_HTML,                    // HTML 메시지
            DATA_STATE_MSGBOX,                  // MSGBOX

            DATA_STATE_SYNTAX_ERROR,      // Syntax Error(HTML and Static)
            DATA_STATE_MISS_TYPE,         // Alarm Type Miss
            DATA_STATE_LENGTHOVER,        // Length Over
            DATA_STATE_LINEOVER,          // Line Over

            DATA_STATE_UNKNOWN,           // Unknown State ( Error )
            DATA_STATE_NUMS
        }

        public DataTable m_tblClient;
        public DataTable m_tblServer;

        protected String m_strClientDef;
        protected String m_strClientAlarm;
        protected String m_strClientKor;
        protected String m_strClientJpn;
        protected String m_strClientUser;

        protected String m_strServerDef;
        protected String m_strServerAlarm;
        protected String m_strServerKor;
        protected String m_strServerJpn;
        protected String m_strServerUser;

        protected const String CLIENT_TABLE = "tblClient";
        protected const String SERVER_TABLE = "tblServer";
        

        /// <summary>
        /// Data Manager 기능에 필요한 데이타 및 컨트롤을 초기화한다.
        /// </summary>
        public void InitDataManager()
        {
            m_aDataState = new String[(int)DATA_STATE.DATA_STATE_NUMS];
            m_aDataState[(int)DATA_STATE.DATA_STATE_NORMAL] = "0";
            m_aDataState[(int)DATA_STATE.DATA_STATE_HTML] = "1";
            m_aDataState[(int)DATA_STATE.DATA_STATE_MSGBOX] = "2";
            m_aDataState[(int)DATA_STATE.DATA_STATE_SYNTAX_ERROR] = "3";
            m_aDataState[(int)DATA_STATE.DATA_STATE_MISS_TYPE] = "4";
            m_aDataState[(int)DATA_STATE.DATA_STATE_LENGTHOVER] = "5";
            m_aDataState[(int)DATA_STATE.DATA_STATE_LINEOVER] = "6";
            m_aDataState[(int)DATA_STATE.DATA_STATE_UNKNOWN] = "7";
        }

        /// <summary>
        /// Table에서 Default로 지정해줘야 하는 것들을 채워준다.
        /// </summary>
        /// <param name="table">테이블</param>
        public void CreateDefaultDataType(ref DataTable table)
        {
            foreach (DataRow row in table.Rows)
            {
                if (row[COL_HTML].ToString().Length <= 0)
                {
                    row[COL_HTML] = "0";
                }

                if (row[COL_LINELENGTH].ToString().Length <= 0)
                {
                    row[COL_LINELENGTH] = "1,128";
                }
            }
        }

        /// <summary>
        /// Data들의 상태를 검사한다.
        /// </summary>
        /// <param name="table">table</param>
        public void CheckDataState(ref DataTable table)
        {
            String strAlarm;
            foreach (DataRow row in table.Rows)
            {
                row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_NORMAL]);

                strAlarm = row[COL_ALARMTYPE].ToString();
                if (strAlarm.Length > 0)
                {
                    String[] strType = strAlarm.Split(',');

                    if (strType.GetLength(0) > 2)
                    {
                        // Type Error
                        row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_MISS_TYPE]);
                    }

                    // Msg Box 인지 Bit Check
                    //strType[0].

                    int nAlarmType = Convert.ToInt32(strType[0].ToString());
                    int nIsMb = nAlarmType & 0x00000001;
                    int nIsLobbyMb = nAlarmType & 0x00000002;
                    if (nIsMb == 0x00000001 || nIsLobbyMb == 0x00000002)
                    {
                        // MessageBox!
                        row[COL_MB] = "1";
                        row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_MSGBOX]);
                    }
                    else
                    {
                        row[COL_MB] = "0";
                    }
                }


                // Line, Length
                String[] strLineLength = row[COL_LINELENGTH].ToString().Split(',');
                int nLine = Convert.ToInt32(strLineLength[0].ToString());
                int nLength = Convert.ToInt32(strLineLength[1].ToString());

                bool bIsHtml = false;
                if (row[COL_HTML].ToString().Contains("1"))
                    bIsHtml = true;

                if (bIsHtml)
                {
                    String[] strSplitToken = { "[br]" };
                    // 한국어 검색
                    String[] strKorLineCount = row[COL_KOR].ToString().Split(strSplitToken, StringSplitOptions.None);
                    String[] strJpnLineCount = row[COL_JPN].ToString().Split(strSplitToken, StringSplitOptions.None);

                    if (strKorLineCount.GetLength(0) > nLine)
                        row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LINEOVER]);

                    if (strJpnLineCount.GetLength(0) > nLine)
                        row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LINEOVER]);

                    foreach (String strKor in strKorLineCount)
                    {
                        if (strKor.Length > nLength)
                            row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LENGTHOVER]);
                    }

                    foreach (String strJpn in strJpnLineCount)
                    {
                        if (strJpn.Length > nLength)
                            row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LENGTHOVER]);
                    }

                    // 일본어 검색
                }
                else
                {
                    String[] strSplitToken = { "\\n" };
                    // 한국어 검색
                    String[] strKorLineCount = row[COL_KOR].ToString().Split(strSplitToken, StringSplitOptions.None);
                    String[] strJpnLineCount = row[COL_JPN].ToString().Split(strSplitToken, StringSplitOptions.None);

                    if (strKorLineCount.GetLength(0) > nLine)
                        row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LINEOVER]);

                    if (strJpnLineCount.GetLength(0) > nLine)
                        row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LINEOVER]);

                    foreach (String strKor in strKorLineCount)
                    {
                        if (strKor.Length > nLength)
                            row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LENGTHOVER]);
                    }

                    foreach (String strJpn in strJpnLineCount)
                    {
                        if (strJpn.Length > nLength)
                            row[COL_STATE] = Convert.ToString(m_aDataState[(int)DATA_STATE.DATA_STATE_LENGTHOVER]);
                    }

                }
            }
        }

        /// <summary>
        /// TABLE에 데이타를 만든다.
        /// </summary>
        /// <param name="gridControl">DATA의 컨트롤</param>
        /// <param name="table">Data Table</param>
        /// <param name="strDef">정의 파일</param>
        /// <param name="strAlarm">메시지 타입 파일</param>
        /// <param name="strKor">KOR 파일</param>
        /// <param name="strJpn">JPN 파일</param>
        /// <param name="strUser">유저 노트</param>
        /// <param name="eSync">클라이언트냐 서버냐</param>
        public void CreateData(ref DataGridView gridControl,
           ref DataTable table,
           String strDef,
           String strAlarm,
           String strKor,
           String strJpn,
           String strUser,
           DATA_SYNC eSync)
        {
            if (eSync == DATA_SYNC.DATA_CLIENT)
                table = new DataTable(CLIENT_TABLE);
            else if (eSync == DATA_SYNC.DATA_SERVER)
                table = new DataTable(SERVER_TABLE);

            DataView dataView = new DataView(table);
            dataView.ApplyDefaultSort = false;

            // Data View를 Data Grid Control 에 연결한다.
            gridControl.DataSource = dataView;

            // ID Column 추가
            DataColumn col = new DataColumn(COL_ID, typeof(System.Int32));
            col.AutoIncrement = true;
            col.ReadOnly = true;
            col.Caption = "이 ID는 UNIQUE한 숫자로서 STRING의 선언 번호 (순서) 를 나타냅니다.";
            table.Columns.Add(col);

            // DEFINE Column 추가
            col = new DataColumn(COL_DEFINE, typeof(System.String));
            col.ReadOnly = true;
            col.Unique = true;
            table.Columns.Add(col);

            col = new DataColumn(COL_COMMENT, typeof(System.String));
            col.ReadOnly = true;
            table.Columns.Add(col);

            col = new DataColumn(COL_KOR, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_JPN, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_ALARMTYPE, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_NOTE, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_MB, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_HTML, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_LINELENGTH, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_STATE, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_SORT_STRING, typeof(System.String));
            table.Columns.Add(col);

            col = new DataColumn(COL_SORT_INT, typeof(System.Int32));
            table.Columns.Add(col);

            // Table에 Primary Key를 잡아준다.
            table.PrimaryKey = new DataColumn[] { table.Columns[COL_DEFINE] };

            if (eSync == DATA_SYNC.DATA_CLIENT)
            {
                m_strClientDef = strDef;
                m_strClientAlarm = strAlarm;
                m_strClientKor = strKor;
                m_strClientJpn = strJpn;
                m_strClientUser = strUser;
            }
            else
            {
                m_strServerDef = strDef;
                m_strServerAlarm = strAlarm;
                m_strServerKor = strKor;
                m_strServerJpn = strJpn;
                m_strServerUser = strUser;
            }

            LoadData(eSync, DATA_TYPE.DATA_TYPE_DEF, strDef);
            LoadData(eSync, DATA_TYPE.DATA_TYPE_ALARM, strAlarm);
            LoadData(eSync, DATA_TYPE.DATA_TYPE_KOR, strKor);
            LoadData(eSync, DATA_TYPE.DATA_TYPE_JPN, strJpn);
            LoadData(eSync, DATA_TYPE.DATA_TYPE_USER, strUser);
            
            CreateDefaultDataType(ref table);

            // Data를 체크한다.
            CheckDataState(ref table);

            // 자동 정렬이 되지 않도록 한다
            foreach (DataGridViewColumn column in gridControl.Columns)
            {
                column.SortMode = DataGridViewColumnSortMode.NotSortable;
            }

            gridControl.Columns[COL_SORT_STRING].Visible = false;
            gridControl.Columns[COL_SORT_INT].Visible = false;
        }

        public void CreateHighLight(ref DataGridView gridView)
        {
            foreach (DataGridViewRow rows in gridView.Rows)
            {
                DataRowView dataRowView = (DataRowView)rows.DataBoundItem;

                String strState = dataRowView.Row[COL_STATE].ToString();
                int nState = Convert.ToInt32(strState);

                // Todo
                switch ((DATA_STATE)nState)
                {
                    case DATA_STATE.DATA_STATE_NORMAL:                  // 정상
                        break;

                    case DATA_STATE.DATA_STATE_HTML:
                        CellColorChange(rows, Color.Black,  Color.FromArgb(0xC5, 0xFF, 0xEE));
                        break;

                    case DATA_STATE.DATA_STATE_MSGBOX:                  // MSGBOX
                        CellColorChange(rows, Color.Black, Color.FromArgb(0x29, 0xB0, 0xFF));
                        break;

                    case DATA_STATE.DATA_STATE_SYNTAX_ERROR:      // Syntax Error(HTML and Static)
                        CellColorChange(rows, Color.Black, Color.FromArgb(0xFF, 0x87, 0x88));
                        break;

                    case DATA_STATE.DATA_STATE_MISS_TYPE:         // Alarm Type Miss
                        CellColorChange(rows, Color.Black, Color.FromArgb(0xFF, 0xCC, 0x5B));
                        break;

                    case DATA_STATE.DATA_STATE_LENGTHOVER:        // Length Over
                        CellColorChange(rows, Color.Black, Color.FromArgb(0xFF, 0xCC, 0x5B));
                        break;

                    case DATA_STATE.DATA_STATE_LINEOVER:          // Line Over
                        CellColorChange(rows, Color.Black, Color.FromArgb(0xFF, 0xCC, 0x5B)); ;
                        break;

                    case DATA_STATE.DATA_STATE_UNKNOWN:           // Unknown State ( Error )
                        CellColorChange(rows, Color.Black, Color.FromArgb(0xFF, 0xB2, 0xDE));
                        break;
                }
            }
        }

#region SAVE FUNCTIONS
        public Boolean SaveData(DATA_TYPE eType, ref DataTable table, String filename)
        {
            try
            {
                using (FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write))
                {

                    using (StreamWriter sw = new StreamWriter(fs, Encoding.Unicode))
                    {
                        switch (eType)
                        {
                            case DATA_TYPE.DATA_TYPE_DEF:
                                break;
                            case DATA_TYPE.DATA_TYPE_ALARM:
                                SaveAlarmData(sw, ref table);
                                break;
                            case DATA_TYPE.DATA_TYPE_KOR:
                                SaveKorData(sw, ref table);
                                break;
                            case DATA_TYPE.DATA_TYPE_JPN:
                                SaveJpnData(sw, ref table);
                                break;
                            case DATA_TYPE.DATA_TYPE_USER:
                                SaveUserData(sw, ref table);
                                break;
                        }

                        sw.Close();
                    }
                }
            }
            catch (System.Exception e)
            {
                AddLog((int)Babidi.eLogType.eLOGTYPE_FILE, e.Source, e.Message);

                return false;
            }

            return true;
        }

        public Boolean SaveAlarmData(StreamWriter sw, ref DataTable table)
        {
            String strDef;
            String strType;
            foreach (DataRow row in table.Rows)
            {

                strDef = row[COL_DEFINE].ToString();
                strType = row[COL_ALARMTYPE].ToString();

                if (strType.Length <= 0)
                    continue;

                sw.WriteLine("{0}={1};", strDef, strType);
            }

            sw.WriteLine();

            return true;
        }

        public Boolean SaveKorData(StreamWriter sw, ref DataTable table)
        {
            String strDef;
            String strData;
            foreach (DataRow row in table.Rows)
            {
                strDef = row[COL_DEFINE].ToString();
                strData = row[COL_KOR].ToString();

                if (strData.Length <= 0)
                    continue;

                sw.WriteLine("{0}=\"{1}\"", strDef, strData);
            }

            sw.WriteLine();

            return true;
        }

        public Boolean SaveJpnData(StreamWriter sw, ref DataTable table)
        {
            String strDef;
            String strData;
            foreach (DataRow row in table.Rows)
            {
                strDef = row[COL_DEFINE].ToString();
                strData = row[COL_JPN].ToString();

                if (strData.Length <= 0)
                    continue;

                sw.WriteLine("{0}=\"{1}\"", strDef, strData);
            }

            sw.WriteLine();

            return true;
        }

        public Boolean SaveUserData(StreamWriter sw, ref DataTable table)
        {
            String strDef;
            String strData;
            foreach (DataRow row in table.Rows)
            {
                strDef = row[COL_DEFINE].ToString();
                strData = String.Format("{0},{1}",
                    row[COL_HTML].ToString(),
                    row[COL_LINELENGTH].ToString());

                if (strData.Length <= 0)
                    continue;

                sw.WriteLine("{0}={1}", strDef, strData);
                sw.WriteLine(row[COL_NOTE].ToString());
            }

            sw.WriteLine();

            return true;
        }
#endregion

        // 서치 기능
        public Boolean SearchCell(string strText, Boolean bFirst)
        {
            // 현재 셀이 클라이언트에 있는지 서버에 있는지 확인한다.
            DataGridViewCell cells = null;
            Boolean bClient = false;
            if (dataGridClient.DataSource != null)
            {
                cells = dataGridClient.CurrentCell;
                bClient = true;
            }

            if (dataGridServer.DataSource != null)
            {
                cells = dataGridServer.CurrentCell;
                bClient = false;
            }

            if (cells == null)
                return false;

            // 셀의 위치를 확인했으면 현재 cell 위치의 Column에서 검색하기 시작한다.


            DataGridView dataGrid = null;
            if (bClient)
                dataGrid = dataGridClient;
            else
                dataGrid = dataGridServer;
                  
            int nRowStart = bFirst ? 0 : cells.RowIndex;
            int nCol = cells.ColumnIndex;

            for (int i = nRowStart + 1; i < dataGridClient.Rows.Count; ++i)
            {
                DataGridViewRow row = dataGrid.Rows[i];
                DataRowView viewRow = (DataRowView)row.DataBoundItem;

                String strRow = viewRow.Row[nCol].ToString();
                if( strRow.Contains( strText.ToString() ) )
                {
                    dataGrid.CurrentCell = row.Cells[nCol];

                    return true;
                }
            }

            return false;
        }
        
        public Boolean SetAlarmType(string strText)
        {
            // 현재 셀이 클라이언트에 있는지 서버에 있는지 확인한다.
            DataGridViewCell cells = null;
            Boolean bClient = false;
            if (dataGridClient.DataSource != null)
            {
                cells = dataGridClient.CurrentCell;
                bClient = true;
            }

            if (dataGridServer.DataSource != null)
            {
                cells = dataGridServer.CurrentCell;
                bClient = false;
            }

            if (cells == null)
                return false;

            // 현재 셀이 유효하다는 것을 판단했다면 현재의 컬럼이 AlarmType인지 확인한다.
            DataGridView dataGrid = null;
            if (bClient)
                dataGrid = dataGridClient;
            else
                dataGrid = dataGridServer;

            // Column Index가 AlarmType인지 확인한다.
            // 맞다면
            if (cells.OwningColumn.HeaderText == COL_ALARMTYPE)
            {
                cells.Value = strText;

                return true;
            }

            return false;
        }
    }
}