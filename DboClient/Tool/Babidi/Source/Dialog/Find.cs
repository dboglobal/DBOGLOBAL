using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Babidi
{
    // 서치 함수
    public delegate bool SearchDelegate( string strSearch, Boolean bFirst);

    public partial class Find : Form
    {
        public SearchDelegate m_delSearch = null;
        
        public Find()
        {
            InitializeComponent();
        }

        public Find(SearchDelegate delSearch)
        {
            m_delSearch = delSearch;

            InitializeComponent();

        }

        private void btnNextSearch_Click(object sender, EventArgs e)
        {
            if (m_delSearch == null)
            {
                MessageBox.Show("검색 Delegate를 찾지 못했습니다: Babidi 오류입니다. 담당자에게 문의하세요.");
                return;
            }

            if (!m_delSearch(this.m_textSearch.Text, false))
            {
                MessageBox.Show("현재 선택된 Cell의 Column에서 해당 내용을 찾지 못했습니다.");

                //if (DialogResult.OK == MessageBox.Show("현재 선택된 Cell의 Column에서 해당 내용을 찾지 못했습니다. 처음부터 다시 찾으시겠습니까?"))
                //{
                //    if (!m_delSearch(this.m_textSearch.Text, true))
                //    {
                //        MessageBox.Show("현재 선택된 Cell의 Column에서 해당 내용을 찾지 못했습니다.");
                //    }
                //}
            }

            this.Focus();
        }

        private void m_textSearch_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                btnNextSearch_Click(null, null);
            }
            else if (e.KeyCode == Keys.Escape)
            {
                Close();
            }
        }
    }
}