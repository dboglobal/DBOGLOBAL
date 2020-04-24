using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MiHaeng4
{
    public partial class LocalSource : Form
    {
        public LocalSource()
        {
            InitializeComponent();

            cmbSourceLocal.Items.Add("Korean");
            cmbSourceLocal.Items.Add("Japanese");            
            
            cmbSourceLocal.SelectedIndex = 0;
        }        

        private void button1_Click(object sender, EventArgs e)
        {
            MH4Global.localSource = (LocalizeSource)(cmbSourceLocal.SelectedIndex + 1);

            this.Close();
        }

        public void SetLabelText(string strText)
        {
            this.label1.Text = strText;
        }

        private void cmbSourceLocal_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}