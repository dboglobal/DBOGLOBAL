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
        public void dataGridServer_Sorted(object sender, EventArgs e)
        {
            CreateHighLight(ref dataGridServer);
        }

        public void SaveServerData()
        {
            SaveData(DATA_TYPE.DATA_TYPE_ALARM, ref m_tblServer, m_strServerAlarm);
            SaveData(DATA_TYPE.DATA_TYPE_KOR, ref m_tblServer, m_strServerKor);
            SaveData(DATA_TYPE.DATA_TYPE_JPN, ref m_tblServer, m_strServerJpn);
            SaveData(DATA_TYPE.DATA_TYPE_USER, ref m_tblServer, m_strServerUser);
        }
    }
}