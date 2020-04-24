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
        protected const string CLIENT_SECTION = "CLIENT";
        protected const string CLIENT_DEFINE_DATA = "DEFINE DATA";
        protected const string CLIENT_ALARM_DATA = "ALARM DATA";
        protected const string CLIENT_KOR_DATA = "KOREAN DATA";
        protected const string CLIENT_JPN_DATA = "JAPANESE DATA";
        protected const string CLIENT_USER_DATA = "USER DATA";

        protected const string SERVER_SECTION = "SERVER";
        protected const string SERVER_DEFINE_DATA = "DEFINE DATA";
        protected const string SERVER_ALARM_DATA = "ALARM DATA";
        protected const string SERVER_KOR_DATA = "KOREAN DATA";
        protected const string SERVER_JPN_DATA = "JAPANESE DATA";
        protected const string SERVER_USER_DATA = "USER DATA";

        protected void InitConfigTab(String strConfig)
        {
            String strConfClientDef = CIniManager.G_IniReadValue(CLIENT_SECTION, CLIENT_DEFINE_DATA, strConfig);
            String strConfClientAlarm = CIniManager.G_IniReadValue(CLIENT_SECTION, CLIENT_ALARM_DATA, strConfig);
            String strConfClientKor = CIniManager.G_IniReadValue(CLIENT_SECTION, CLIENT_KOR_DATA, strConfig);
            String strConfClientJpn = CIniManager.G_IniReadValue(CLIENT_SECTION, CLIENT_JPN_DATA, strConfig);
            String strConfClientUser = CIniManager.G_IniReadValue(CLIENT_SECTION, CLIENT_USER_DATA, strConfig);
            String strConfServerDef = CIniManager.G_IniReadValue(SERVER_SECTION, SERVER_DEFINE_DATA, strConfig);
            String strConfServerAlarm = CIniManager.G_IniReadValue(SERVER_SECTION, SERVER_ALARM_DATA, strConfig);
            String strConfServerKor = CIniManager.G_IniReadValue(SERVER_SECTION, SERVER_KOR_DATA, strConfig);
            String strConfServerJpn = CIniManager.G_IniReadValue(SERVER_SECTION, SERVER_JPN_DATA, strConfig);
            String strConfServerUser = CIniManager.G_IniReadValue(SERVER_SECTION, SERVER_USER_DATA, strConfig);
            

            this.textClientDefPath.Clear();
            this.textClientDefPath.AppendText(strConfClientDef);

            this.textClientAlarmPath.Clear();
            this.textClientAlarmPath.AppendText(strConfClientAlarm);

            this.textClientKorPath.Clear();
            this.textClientKorPath.AppendText(strConfClientKor);

            this.textClientJpnPath.Clear();
            this.textClientJpnPath.AppendText(strConfClientJpn);

            this.textClientUserPath.Clear();
            this.textClientUserPath.AppendText(strConfClientUser);

            this.textServerDefPath.Clear();
            this.textServerDefPath.AppendText(strConfServerDef);

            this.textServerAlarmPath.Clear();
            this.textServerAlarmPath.AppendText(strConfServerAlarm);

            this.textServerKorPath.Clear();
            this.textServerKorPath.AppendText(strConfServerKor);

            this.textServerJpnPath.Clear();
            this.textServerJpnPath.AppendText(strConfServerJpn);

            this.textServerUserPath.Clear();
            this.textServerUserPath.AppendText(strConfServerUser);
        }

        private void btnClientDef_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo client string define data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textClientDefPath.Clear();
                this.textClientDefPath.AppendText(dlg.FileName);
            }
        }

        private void btnClientAlarm_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo client string alarm type data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textClientAlarmPath.Clear();
                this.textClientAlarmPath.AppendText(dlg.FileName);
            }
        }

        private void btnClientKor_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo client string korean data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textClientKorPath.Clear();
                this.textClientKorPath.AppendText(dlg.FileName);
            }
        }

        private void btnClientJap_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo client string japanese data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textClientJpnPath.Clear();
                this.textClientJpnPath.AppendText(dlg.FileName);
            }
        }

        private void btnClientUserPath_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo client string user data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textClientUserPath.Clear();
                this.textClientUserPath.AppendText(dlg.FileName);
            }
        }

        private void btnServerDef_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo server string define data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textServerDefPath.Clear();
                this.textServerDefPath.AppendText(dlg.FileName);
            }
        }

        private void btnServerAlarm_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo server string alarm type data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textServerAlarmPath.Clear();
                this.textServerAlarmPath.AppendText(dlg.FileName);
            }
        }

        private void btnServerKor_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo server string korean data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textServerKorPath.Clear();
                this.textServerKorPath.AppendText(dlg.FileName);
            }
        }

        private void btnServerJap_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo server string japanese data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textServerJpnPath.Clear();
                this.textServerJpnPath.AppendText(dlg.FileName);
            }
        }

        private void btnServerUserPath_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Dbo server string user data (*.dat)|*.dat|All files (*.*)|*.*";
            dlg.Title = "Babidi File";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.textServerUserPath.Clear();
                this.textServerUserPath.AppendText(dlg.FileName);
            }
        }

        private void btnSaveConfig_Click(object sender, EventArgs e)
        {
            CIniManager.G_IniWriteValue(CLIENT_SECTION, CLIENT_DEFINE_DATA, this.textClientDefPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(CLIENT_SECTION, CLIENT_ALARM_DATA, this.textClientAlarmPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(CLIENT_SECTION, CLIENT_KOR_DATA, this.textClientKorPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(CLIENT_SECTION, CLIENT_JPN_DATA, this.textClientJpnPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(CLIENT_SECTION, CLIENT_USER_DATA, this.textClientUserPath.Text, CONFIG_FILE);

            CIniManager.G_IniWriteValue(SERVER_SECTION, SERVER_DEFINE_DATA, this.textServerDefPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(SERVER_SECTION, SERVER_ALARM_DATA, this.textServerAlarmPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(SERVER_SECTION, SERVER_KOR_DATA, this.textServerKorPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(SERVER_SECTION, SERVER_JPN_DATA, this.textServerJpnPath.Text, CONFIG_FILE);
            CIniManager.G_IniWriteValue(SERVER_SECTION, SERVER_USER_DATA, this.textServerUserPath.Text, CONFIG_FILE);
        }

        private void btnLoadClient_Click(object sender, EventArgs e)
        {
            if (this.textClientDefPath.TextLength <= 0 ||
                this.textClientAlarmPath.TextLength <= 0 ||
                this.textClientKorPath.TextLength <= 0 ||
                this.textClientJpnPath.TextLength <= 0 ||
                this.textClientUserPath.TextLength <= 0)
            {
                MessageBox.Show("Path not found : Must input all client file", "File not found");
                AddLog((int)eLogType.eLOGTYPE_FILE, "Config", "Path not Found : ");

                return;
            }

            // File의 정보 읽어오기
            FileInfo fDefInfo = new FileInfo(this.textClientDefPath.Text);
            FileInfo fAlarmInfo = new FileInfo(this.textClientAlarmPath.Text);
            FileInfo fKorInfo = new FileInfo(this.textClientKorPath.Text);
            FileInfo fJpnInfo = new FileInfo(this.textClientJpnPath.Text);
            FileInfo fUserInfo = new FileInfo(this.textClientUserPath.Text);

            // File 
            if (!fDefInfo.Exists ||
                !fAlarmInfo.Exists ||
                !fKorInfo.Exists ||
                !fJpnInfo.Exists ||
                !fUserInfo.Exists )
            {
                MessageBox.Show("File not found : *.DAT file", "File not found");
                AddLog((int)eLogType.eLOGTYPE_FILE, "Config", "File not Found");

                return;
            }

            CreateData(ref dataGridClient,
                ref m_tblClient,
                this.textClientDefPath.Text,
                this.textClientAlarmPath.Text,
                this.textClientKorPath.Text,
                this.textClientJpnPath.Text,
                this.textClientUserPath.Text,
                DATA_SYNC.DATA_CLIENT);
            
            tabLocalize.SelectTab("tabClient");

            CreateHighLight(ref dataGridClient);

            LoadStyle(DATA_SYNC.DATA_CLIENT);
            

            dataGridClient.Sorted += new EventHandler(dataGridClient_Sorted);
        }

        private void btnLoadServer_Click(object sender, EventArgs e)
        {
            if (this.textServerDefPath.TextLength <= 0 ||
                this.textServerAlarmPath.TextLength <= 0 ||
                this.textServerKorPath.TextLength <= 0 ||
                this.textServerJpnPath.TextLength <= 0 ||
                this.textServerUserPath.TextLength <= 0)
            {
                MessageBox.Show("Path not found : Must input all server file", "File not found");
                AddLog((int)eLogType.eLOGTYPE_FILE, "Config", "Path not Found : ");

                return;
            }

            // File의 정보 읽어오기
            FileInfo fDefInfo = new FileInfo(this.textServerDefPath.Text);
            FileInfo fAlarmInfo = new FileInfo(this.textServerAlarmPath.Text);
            FileInfo fKorInfo = new FileInfo(this.textServerKorPath.Text);
            FileInfo fJpnInfo = new FileInfo(this.textServerJpnPath.Text);
            FileInfo fUserInfo = new FileInfo(this.textServerUserPath.Text);

            // File 
            if (!fDefInfo.Exists ||
                !fAlarmInfo.Exists ||
                !fKorInfo.Exists ||
                !fJpnInfo.Exists ||
                !fUserInfo.Exists)
            {
                MessageBox.Show("File not found : *.DAT file", "File not found");
                AddLog((int)eLogType.eLOGTYPE_FILE, "Config", "File not Found");

                return;
            }

            CreateData(ref dataGridServer,
                ref m_tblServer,
                this.textServerDefPath.Text,
                this.textServerAlarmPath.Text,
                this.textServerKorPath.Text,
                this.textServerJpnPath.Text,
                this.textServerUserPath.Text,
                DATA_SYNC.DATA_SERVER);

            tabLocalize.SelectTab("tabServer");

            CreateHighLight(ref dataGridServer);

            LoadStyle(DATA_SYNC.DATA_SERVER);

            dataGridServer.Sorted += new EventHandler(dataGridServer_Sorted);
        }
    }
}