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
        public void dataGridClient_Sorted(object sender, EventArgs e)
        {
            CreateHighLight(ref dataGridClient);
        }

        public void SaveClientData()
        {
            SaveData(DATA_TYPE.DATA_TYPE_ALARM, ref m_tblClient, m_strClientAlarm);
            SaveData(DATA_TYPE.DATA_TYPE_KOR, ref m_tblClient, m_strClientKor);
            SaveData(DATA_TYPE.DATA_TYPE_JPN, ref m_tblClient, m_strClientJpn);
            SaveData(DATA_TYPE.DATA_TYPE_USER, ref m_tblClient, m_strClientUser);
        }
    }
}
