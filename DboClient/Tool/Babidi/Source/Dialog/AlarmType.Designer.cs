namespace Babidi
{
    partial class CFormAlarm
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
            this.checkMsgBox = new System.Windows.Forms.CheckBox();
            this.checkLobbyMsgBox = new System.Windows.Forms.CheckBox();
            this.checkChatWarn = new System.Windows.Forms.CheckBox();
            this.checkTimeFixed = new System.Windows.Forms.CheckBox();
            this.checkAction = new System.Windows.Forms.CheckBox();
            this.checkChatNotice = new System.Windows.Forms.CheckBox();
            this.checkNone = new System.Windows.Forms.CheckBox();
            this.checkOK = new System.Windows.Forms.CheckBox();
            this.checkCancel = new System.Windows.Forms.CheckBox();
            this.checkInput = new System.Windows.Forms.CheckBox();
            this.checkCustom = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textMsgResult = new System.Windows.Forms.TextBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textAlarmType = new System.Windows.Forms.TextBox();
            this.textWorkID = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textMsgType = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textTime = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textCustomString1 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textCustomWorkID1 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textCustomWorkID2 = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textCustomString2 = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textCustomWorkID3 = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.textCustomString3 = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.textCustomWorkID4 = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textCustomString4 = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.checkPopup = new System.Windows.Forms.CheckBox();
            this.label15 = new System.Windows.Forms.Label();
            this.checkDuplicate = new System.Windows.Forms.CheckBox();
            this.label16 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // checkMsgBox
            // 
            this.checkMsgBox.AutoSize = true;
            this.checkMsgBox.Location = new System.Drawing.Point(12, 51);
            this.checkMsgBox.Name = "checkMsgBox";
            this.checkMsgBox.Size = new System.Drawing.Size(97, 16);
            this.checkMsgBox.TabIndex = 0;
            this.checkMsgBox.Text = "1 - MSGBOX";
            this.checkMsgBox.UseVisualStyleBackColor = true;
            // 
            // checkLobbyMsgBox
            // 
            this.checkLobbyMsgBox.AutoSize = true;
            this.checkLobbyMsgBox.Location = new System.Drawing.Point(12, 73);
            this.checkLobbyMsgBox.Name = "checkLobbyMsgBox";
            this.checkLobbyMsgBox.Size = new System.Drawing.Size(107, 16);
            this.checkLobbyMsgBox.TabIndex = 1;
            this.checkLobbyMsgBox.Text = "2 - LOBBY MB";
            this.checkLobbyMsgBox.UseVisualStyleBackColor = true;
            // 
            // checkChatWarn
            // 
            this.checkChatWarn.AutoSize = true;
            this.checkChatWarn.Location = new System.Drawing.Point(12, 95);
            this.checkChatWarn.Name = "checkChatWarn";
            this.checkChatWarn.Size = new System.Drawing.Size(118, 16);
            this.checkChatWarn.TabIndex = 2;
            this.checkChatWarn.Text = "4 - CHAT_WARN";
            this.checkChatWarn.UseVisualStyleBackColor = true;
            // 
            // checkTimeFixed
            // 
            this.checkTimeFixed.AutoSize = true;
            this.checkTimeFixed.Location = new System.Drawing.Point(12, 117);
            this.checkTimeFixed.Name = "checkTimeFixed";
            this.checkTimeFixed.Size = new System.Drawing.Size(108, 16);
            this.checkTimeFixed.TabIndex = 3;
            this.checkTimeFixed.Text = "8 - TIMEFIXED";
            this.checkTimeFixed.UseVisualStyleBackColor = true;
            // 
            // checkAction
            // 
            this.checkAction.AutoSize = true;
            this.checkAction.Location = new System.Drawing.Point(12, 139);
            this.checkAction.Name = "checkAction";
            this.checkAction.Size = new System.Drawing.Size(85, 16);
            this.checkAction.TabIndex = 4;
            this.checkAction.Text = "16 - Action";
            this.checkAction.UseVisualStyleBackColor = true;
            // 
            // checkChatNotice
            // 
            this.checkChatNotice.AutoSize = true;
            this.checkChatNotice.Location = new System.Drawing.Point(12, 161);
            this.checkChatNotice.Name = "checkChatNotice";
            this.checkChatNotice.Size = new System.Drawing.Size(133, 16);
            this.checkChatNotice.TabIndex = 5;
            this.checkChatNotice.Text = "32 - CHAT NOTICE";
            this.checkChatNotice.UseVisualStyleBackColor = true;
            // 
            // checkNone
            // 
            this.checkNone.AutoSize = true;
            this.checkNone.Location = new System.Drawing.Point(343, 51);
            this.checkNone.Name = "checkNone";
            this.checkNone.Size = new System.Drawing.Size(79, 16);
            this.checkNone.TabIndex = 6;
            this.checkNone.Text = "0 - NONE";
            this.checkNone.UseVisualStyleBackColor = true;
            // 
            // checkOK
            // 
            this.checkOK.AutoSize = true;
            this.checkOK.Location = new System.Drawing.Point(343, 73);
            this.checkOK.Name = "checkOK";
            this.checkOK.Size = new System.Drawing.Size(61, 16);
            this.checkOK.TabIndex = 7;
            this.checkOK.Text = "1 - OK";
            this.checkOK.UseVisualStyleBackColor = true;
            // 
            // checkCancel
            // 
            this.checkCancel.AutoSize = true;
            this.checkCancel.Location = new System.Drawing.Point(343, 95);
            this.checkCancel.Name = "checkCancel";
            this.checkCancel.Size = new System.Drawing.Size(94, 16);
            this.checkCancel.TabIndex = 8;
            this.checkCancel.Text = "2 - CANCEL";
            this.checkCancel.UseVisualStyleBackColor = true;
            // 
            // checkInput
            // 
            this.checkInput.AutoSize = true;
            this.checkInput.Location = new System.Drawing.Point(343, 117);
            this.checkInput.Name = "checkInput";
            this.checkInput.Size = new System.Drawing.Size(80, 16);
            this.checkInput.TabIndex = 9;
            this.checkInput.Text = "4 - INPUT";
            this.checkInput.UseVisualStyleBackColor = true;
            // 
            // checkCustom
            // 
            this.checkCustom.AutoSize = true;
            this.checkCustom.Location = new System.Drawing.Point(343, 139);
            this.checkCustom.Name = "checkCustom";
            this.checkCustom.Size = new System.Drawing.Size(97, 16);
            this.checkCustom.TabIndex = 10;
            this.checkCustom.Text = "8 - CUSTOM";
            this.checkCustom.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 440);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 11;
            this.label1.Text = "결과값";
            // 
            // textMsgResult
            // 
            this.textMsgResult.Location = new System.Drawing.Point(13, 455);
            this.textMsgResult.Multiline = true;
            this.textMsgResult.Name = "textMsgResult";
            this.textMsgResult.Size = new System.Drawing.Size(516, 100);
            this.textMsgResult.TabIndex = 12;
            this.textMsgResult.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(213, 561);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(112, 26);
            this.btnOK.TabIndex = 13;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 14;
            this.label2.Text = "Alarm Type";
            // 
            // textAlarmType
            // 
            this.textAlarmType.Location = new System.Drawing.Point(12, 24);
            this.textAlarmType.MaxLength = 10;
            this.textAlarmType.Name = "textAlarmType";
            this.textAlarmType.ReadOnly = true;
            this.textAlarmType.Size = new System.Drawing.Size(71, 21);
            this.textAlarmType.TabIndex = 15;
            this.textAlarmType.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textAlarmType.WordWrap = false;
            // 
            // textWorkID
            // 
            this.textWorkID.Location = new System.Drawing.Point(89, 24);
            this.textWorkID.Name = "textWorkID";
            this.textWorkID.Size = new System.Drawing.Size(248, 21);
            this.textWorkID.TabIndex = 17;
            this.textWorkID.WordWrap = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(89, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 12);
            this.label3.TabIndex = 16;
            this.label3.Text = "WorkID";
            // 
            // textMsgType
            // 
            this.textMsgType.Location = new System.Drawing.Point(343, 24);
            this.textMsgType.Name = "textMsgType";
            this.textMsgType.ReadOnly = true;
            this.textMsgType.Size = new System.Drawing.Size(70, 21);
            this.textMsgType.TabIndex = 19;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(343, 9);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 12);
            this.label4.TabIndex = 18;
            this.label4.Text = "MsgType";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(426, 10);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 12);
            this.label5.TabIndex = 20;
            this.label5.Text = "Popup";
            // 
            // textTime
            // 
            this.textTime.Location = new System.Drawing.Point(480, 24);
            this.textTime.Name = "textTime";
            this.textTime.Size = new System.Drawing.Size(48, 21);
            this.textTime.TabIndex = 23;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(480, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(48, 12);
            this.label6.TabIndex = 22;
            this.label6.Text = "S-Time";
            // 
            // textCustomString1
            // 
            this.textCustomString1.Location = new System.Drawing.Point(49, 206);
            this.textCustomString1.Name = "textCustomString1";
            this.textCustomString1.Size = new System.Drawing.Size(480, 21);
            this.textCustomString1.TabIndex = 25;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(10, 215);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(19, 12);
            this.label7.TabIndex = 24;
            this.label7.Text = "S1";
            // 
            // textCustomWorkID1
            // 
            this.textCustomWorkID1.Location = new System.Drawing.Point(49, 233);
            this.textCustomWorkID1.Name = "textCustomWorkID1";
            this.textCustomWorkID1.Size = new System.Drawing.Size(480, 21);
            this.textCustomWorkID1.TabIndex = 27;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(10, 241);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(21, 12);
            this.label8.TabIndex = 26;
            this.label8.Text = "W1";
            // 
            // textCustomWorkID2
            // 
            this.textCustomWorkID2.Location = new System.Drawing.Point(49, 287);
            this.textCustomWorkID2.Name = "textCustomWorkID2";
            this.textCustomWorkID2.Size = new System.Drawing.Size(480, 21);
            this.textCustomWorkID2.TabIndex = 31;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(10, 293);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(21, 12);
            this.label9.TabIndex = 30;
            this.label9.Text = "W2";
            // 
            // textCustomString2
            // 
            this.textCustomString2.Location = new System.Drawing.Point(49, 260);
            this.textCustomString2.Name = "textCustomString2";
            this.textCustomString2.Size = new System.Drawing.Size(480, 21);
            this.textCustomString2.TabIndex = 29;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(10, 267);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(19, 12);
            this.label10.TabIndex = 28;
            this.label10.Text = "S2";
            // 
            // textCustomWorkID3
            // 
            this.textCustomWorkID3.Location = new System.Drawing.Point(49, 341);
            this.textCustomWorkID3.Name = "textCustomWorkID3";
            this.textCustomWorkID3.Size = new System.Drawing.Size(480, 21);
            this.textCustomWorkID3.TabIndex = 35;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(10, 345);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(21, 12);
            this.label11.TabIndex = 34;
            this.label11.Text = "W3";
            // 
            // textCustomString3
            // 
            this.textCustomString3.Location = new System.Drawing.Point(49, 314);
            this.textCustomString3.Name = "textCustomString3";
            this.textCustomString3.Size = new System.Drawing.Size(480, 21);
            this.textCustomString3.TabIndex = 33;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(10, 319);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(19, 12);
            this.label12.TabIndex = 32;
            this.label12.Text = "S3";
            // 
            // textCustomWorkID4
            // 
            this.textCustomWorkID4.Location = new System.Drawing.Point(49, 395);
            this.textCustomWorkID4.Name = "textCustomWorkID4";
            this.textCustomWorkID4.Size = new System.Drawing.Size(480, 21);
            this.textCustomWorkID4.TabIndex = 39;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(10, 397);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(21, 12);
            this.label13.TabIndex = 38;
            this.label13.Text = "W4";
            // 
            // textCustomString4
            // 
            this.textCustomString4.Location = new System.Drawing.Point(49, 368);
            this.textCustomString4.Name = "textCustomString4";
            this.textCustomString4.Size = new System.Drawing.Size(480, 21);
            this.textCustomString4.TabIndex = 37;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(10, 371);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(19, 12);
            this.label14.TabIndex = 36;
            this.label14.Text = "S4";
            // 
            // checkPopup
            // 
            this.checkPopup.AutoSize = true;
            this.checkPopup.Location = new System.Drawing.Point(440, 27);
            this.checkPopup.Name = "checkPopup";
            this.checkPopup.Size = new System.Drawing.Size(15, 14);
            this.checkPopup.TabIndex = 40;
            this.checkPopup.UseVisualStyleBackColor = true;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(15, 186);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(182, 12);
            this.label15.TabIndex = 41;
            this.label15.Text = "CustomStringID & CustomWorkID";
            // 
            // checkDuplicate
            // 
            this.checkDuplicate.AutoSize = true;
            this.checkDuplicate.Location = new System.Drawing.Point(576, 27);
            this.checkDuplicate.Name = "checkDuplicate";
            this.checkDuplicate.Size = new System.Drawing.Size(15, 14);
            this.checkDuplicate.TabIndex = 43;
            this.checkDuplicate.UseVisualStyleBackColor = true;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(534, 9);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(96, 12);
            this.label16.TabIndex = 42;
            this.label16.Text = "AcceptDuplicate";
            // 
            // CFormAlarm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(637, 596);
            this.Controls.Add(this.checkDuplicate);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.checkPopup);
            this.Controls.Add(this.textCustomWorkID4);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.textCustomString4);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.textCustomWorkID3);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.textCustomString3);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.textCustomWorkID2);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.textCustomString2);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textCustomWorkID1);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textCustomString1);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textTime);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textMsgType);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textWorkID);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textAlarmType);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.textMsgResult);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.checkCustom);
            this.Controls.Add(this.checkInput);
            this.Controls.Add(this.checkCancel);
            this.Controls.Add(this.checkOK);
            this.Controls.Add(this.checkNone);
            this.Controls.Add(this.checkChatNotice);
            this.Controls.Add(this.checkAction);
            this.Controls.Add(this.checkTimeFixed);
            this.Controls.Add(this.checkChatWarn);
            this.Controls.Add(this.checkLobbyMsgBox);
            this.Controls.Add(this.checkMsgBox);
            this.Name = "CFormAlarm";
            this.Text = "Alarm Type Dialog";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkMsgBox;
        private System.Windows.Forms.CheckBox checkLobbyMsgBox;
        private System.Windows.Forms.CheckBox checkChatWarn;
        private System.Windows.Forms.CheckBox checkTimeFixed;
        private System.Windows.Forms.CheckBox checkAction;
        private System.Windows.Forms.CheckBox checkChatNotice;
        private System.Windows.Forms.CheckBox checkNone;
        private System.Windows.Forms.CheckBox checkOK;
        private System.Windows.Forms.CheckBox checkCancel;
        private System.Windows.Forms.CheckBox checkInput;
        private System.Windows.Forms.CheckBox checkCustom;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textMsgResult;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textAlarmType;
        private System.Windows.Forms.TextBox textWorkID;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textMsgType;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textTime;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textCustomString1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textCustomWorkID1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textCustomWorkID2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textCustomString2;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textCustomWorkID3;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox textCustomString3;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textCustomWorkID4;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textCustomString4;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.CheckBox checkPopup;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.CheckBox checkDuplicate;
        private System.Windows.Forms.Label label16;
    }
}