namespace MiHaeng4
{
    partial class LocalSource
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
            this.cmbSourceLocal = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // cmbSourceLocal
            // 
            this.cmbSourceLocal.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbSourceLocal.FormattingEnabled = true;
            this.cmbSourceLocal.Location = new System.Drawing.Point(15, 70);
            this.cmbSourceLocal.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.cmbSourceLocal.Name = "cmbSourceLocal";
            this.cmbSourceLocal.Size = new System.Drawing.Size(343, 28);
            this.cmbSourceLocal.TabIndex = 0;
            this.cmbSourceLocal.SelectedIndexChanged += new System.EventHandler(this.cmbSourceLocal_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 45);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(261, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "Select Source Localize of XLS Files";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(145, 113);
            this.button1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(96, 38);
            this.button1.TabIndex = 2;
            this.button1.Text = "OK";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // LocalSource
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(375, 170);
            this.ControlBox = false;
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbSourceLocal);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LocalSource";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Select Source Localize";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.ComboBox cmbSourceLocal;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
    }
}