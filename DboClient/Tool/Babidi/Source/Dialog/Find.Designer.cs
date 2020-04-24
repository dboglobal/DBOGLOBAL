namespace Babidi
{
    partial class Find
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.m_textSearch = new System.Windows.Forms.TextBox();
            this.btnNextSearch = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "찾을 내용:";
            // 
            // m_textSearch
            // 
            this.m_textSearch.Location = new System.Drawing.Point(11, 26);
            this.m_textSearch.Name = "m_textSearch";
            this.m_textSearch.Size = new System.Drawing.Size(290, 21);
            this.m_textSearch.TabIndex = 1;
            this.m_textSearch.KeyDown += new System.Windows.Forms.KeyEventHandler(this.m_textSearch_KeyDown);
            // 
            // btnNextSearch
            // 
            this.btnNextSearch.Location = new System.Drawing.Point(181, 76);
            this.btnNextSearch.Name = "btnNextSearch";
            this.btnNextSearch.Size = new System.Drawing.Size(120, 21);
            this.btnNextSearch.TabIndex = 2;
            this.btnNextSearch.Text = "다음 찾기";
            this.btnNextSearch.UseVisualStyleBackColor = true;
            this.btnNextSearch.Click += new System.EventHandler(this.btnNextSearch_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 50);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(225, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "현재 셀이 위치한 Column에서 찾습니다.";
            // 
            // Find
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(314, 107);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnNextSearch);
            this.Controls.Add(this.m_textSearch);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Find";
            this.Text = "Find";
            this.TopMost = true;
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.m_textSearch_KeyDown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox m_textSearch;
        private System.Windows.Forms.Button btnNextSearch;
        private System.Windows.Forms.Label label2;
    }
}