using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Babidi
{
    // Delegate
    public delegate bool AlarmDelegate(string strSearch);

    public partial class CFormAlarm : Form
    {
        public AlarmDelegate m_deleAlarm = null;

        public CFormAlarm( AlarmDelegate deleAlarm )
        {
            m_deleAlarm = deleAlarm;

            InitializeComponent();

            // Alarm Type
            checkMsgBox.CheckedChanged += new EventHandler(MsgBoxCheckedChange);
            checkLobbyMsgBox.CheckedChanged += new EventHandler(MsgBoxCheckedChange);
            checkChatWarn.CheckedChanged += new EventHandler(MsgBoxCheckedChange);
            checkTimeFixed.CheckedChanged += new EventHandler(MsgBoxCheckedChange);
            checkAction.CheckedChanged += new EventHandler(MsgBoxCheckedChange);
            checkChatNotice.CheckedChanged += new EventHandler(MsgBoxCheckedChange);

            // WorkID
            textWorkID.TextChanged += new EventHandler(textWorkID_TextChanged);
                       
            // Msg Type
            checkNone.CheckedChanged += new EventHandler(MsgTypeCheckedChange) ;
            checkOK.CheckedChanged += new EventHandler(MsgTypeCheckedChange);
            checkCancel.CheckedChanged += new EventHandler(MsgTypeCheckedChange);
            checkInput.CheckedChanged += new EventHandler(MsgTypeCheckedChange);
            checkCustom.CheckedChanged += new EventHandler(MsgTypeCheckedChange);

            // Popup
            checkPopup.CheckedChanged += new EventHandler(checkPopup_CheckedChanged);

            // S-Time
            textTime.TextChanged += new EventHandler(textTime_TextChanged);

            // Accept Duplicate
            checkDuplicate.CheckedChanged += new EventHandler(checkDuplicate_CheckedChanged);

            // String1, 2, 3, 4, CustomWork 1, 2, 3, 4
            textCustomString1.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomString2.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomString3.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomString4.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomWorkID1.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomWorkID2.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomWorkID3.TextChanged += new EventHandler(textCustomString1_TextChanged);
            textCustomWorkID4.TextChanged += new EventHandler(textCustomString1_TextChanged);

            btnOK.Click += new EventHandler(btnOK_Click);
        }

        void checkDuplicate_CheckedChanged(object sender, EventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            MakeResult();
        }

        protected void MakeResult()
        {
            textMsgResult.Clear();

            // Type 자체가 정해져 있지 않다면 리턴시킨다.
            if( textAlarmType.TextLength <= 0 )
                return;

            // Alarm Type 여부 판단
            int nAlarmType = new int();
            if (checkMsgBox.Checked)
                nAlarmType |= 0x00000001;

            if (checkLobbyMsgBox.Checked)
                nAlarmType |= 0x00000002;

            if (checkChatWarn.Checked)
                nAlarmType |= 0x00000004;

            if (checkTimeFixed.Checked)
                nAlarmType |= 0x00000008;

            if (checkAction.Checked)
                nAlarmType |= 0x00000010;

            if (checkChatNotice.Checked)
                nAlarmType |= 0x00000020;

            // MsgBox Type 여부 판단
            int nMsgBoxType = new int();
            if (checkNone.Checked)
                nMsgBoxType |= 0;

            if (checkOK.Checked)
                nMsgBoxType |= 0x00000001;

            if (checkCancel.Checked)
                nMsgBoxType |= 0x00000002;

            if (checkInput.Checked)
                nMsgBoxType |= 0x00000004;

            if (checkCustom.Checked)
                nMsgBoxType |= 0x00000008;
            
            // Append All
            textMsgResult.AppendText(textAlarmType.Text);

            // Work ID
            if (((nAlarmType & 0x00000001) == 0x00000001) ||
               ((nAlarmType & 0x00000002) == 0x00000002))
            {
                if (textWorkID.TextLength > 0)
                {
                    textMsgResult.AppendText(",");
                    textMsgResult.AppendText(textWorkID.Text);
                }

                if (textMsgType.TextLength > 0)
                {
                    textMsgResult.AppendText(",");
                    textMsgResult.AppendText(textMsgType.Text);
                }
            }
           
              // alarm Type이 MSGBOX 또는 Lobby MB 속성이 포함되어 있다면
            if ( ( (nAlarmType & 0x00000001) == 0x00000001 ) ||
                 ( ( nAlarmType & 0x00000002) == 0x00000002 ) )
            {
                textMsgResult.AppendText(",");
                if (!checkPopup.Checked)
                {
                    textMsgResult.AppendText("0");
                }
                else
                {
                    textMsgResult.AppendText("1");
                }
            }

            if (((nAlarmType & 0x00000001) == 0x00000001) ||
                ((nAlarmType & 0x00000002) == 0x00000002))
            {
                if (textTime.TextLength > 0)
                {
                    textMsgResult.AppendText(",");
                    textMsgResult.AppendText(textTime.Text);
                }
                else
                {
                    textMsgResult.AppendText(",");
                    textMsgResult.AppendText("0");
                }
            }

            if (((nAlarmType & 0x00000001) == 0x00000001) ||
              ((nAlarmType & 0x00000002) == 0x00000002))
            {
                textMsgResult.AppendText(",");
                if (!checkDuplicate.Checked)
                {
                    textMsgResult.AppendText("0");
                }
                else
                {
                    textMsgResult.AppendText("1");
                }
            }

            // MsgBox이고 Custom MsgBoxType이 들어가 있을 때에만 작동
            if (((nAlarmType & 0x00000001) == 0x00000001) ||
               ((nAlarmType & 0x00000002) == 0x00000002))
            {
                if( ( nMsgBoxType & 0x00000008 ) == 0x00000008 )
                {
                    // 순서대로 쌍으로 넣는다.
                    if( textCustomString1.TextLength > 0 &&
                        textCustomWorkID1.TextLength > 0 )
                    {
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomString1.Text);
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomWorkID1.Text);
                    }

                     // 순서대로 쌍으로 넣는다.
                    if( textCustomString2.TextLength > 0 &&
                        textCustomWorkID2.TextLength > 0 )
                    {
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomString2.Text);
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomWorkID2.Text);
                    }

                     // 순서대로 쌍으로 넣는다.
                    if( textCustomString3.TextLength > 0 &&
                        textCustomWorkID3.TextLength > 0 )
                    {
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomString3.Text);
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomWorkID3.Text);
                    }

                     // 순서대로 쌍으로 넣는다.
                    if( textCustomString4.TextLength > 0 &&
                        textCustomWorkID4.TextLength > 0 )
                    {
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomString4.Text);
                        textMsgResult.AppendText(",");
                        textMsgResult.AppendText(textCustomWorkID4.Text);
                    }
                }
            }       
        }

        void textCustomString1_TextChanged(object sender, EventArgs e)
        {
            MakeResult();
            //throw new Exception("The method or operation is not implemented.");
        }

        void textTime_TextChanged(object sender, EventArgs e)
        {
            MakeResult();
            //throw new Exception("The method or operation is not implemented.");
        }

        void checkPopup_CheckedChanged(object sender, EventArgs e)
        {
            MakeResult();
            //throw new Exception("The method or operation is not implemented.");
        }

        void textWorkID_TextChanged(object sender, EventArgs e)
        {
            MakeResult();
            //throw new Exception("The method or operation is not implemented.");
        }

        void btnOK_Click(object sender, EventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");

            // ClipBoard에 TextBox의 내용을 복사하고 닫는다.
            Clipboard.SetData(DataFormats.Text, textMsgResult.Text);

            // Delegate 호출
            if (m_deleAlarm != null )
                m_deleAlarm(textMsgResult.Text);
            
            Close();
        }

        void MsgBoxCheckedChange(object sender, EventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            MakeAlarmType();

            MakeResult();
        }

        void MsgTypeCheckedChange(object sender, EventArgs e)
        {
            MakeMsgType();

            MakeResult();
        }

        protected void MakeAlarmType()
        {
            int nAlarmType = new int();

            if (checkMsgBox.Checked)
                nAlarmType |= 0x00000001;

            if (checkLobbyMsgBox.Checked)
                nAlarmType |= 0x00000002;

            if (checkChatWarn.Checked)
                nAlarmType |= 0x00000004;

            if (checkTimeFixed.Checked)
                nAlarmType |= 0x00000008;

            if (checkAction.Checked)
                nAlarmType |= 0x00000010;

            if (checkChatNotice.Checked)
                nAlarmType |= 0x00000020;

            textAlarmType.Clear();
            if( nAlarmType != 0 )
                textAlarmType.AppendText(Convert.ToString(nAlarmType));

            // alarm Type이 MSGBOX 또는 Lobby MB 속성이 포함되어 있다면
            if ( ( (nAlarmType & 0x00000001) == 0x00000001 ) ||
                 ( ( nAlarmType & 0x00000002) == 0x00000002 ) )
            {
                // 무언가 입력되어 있지 않다면 Default값 세팅
                if( textWorkID.TextLength <= 0 )
                {
                    textWorkID.AppendText("MBW_NONE");
                }

                if(checkNone.Checked == false)
                {
                    checkNone.Checked = true;
                }

                checkPopup.Checked = false;
                checkDuplicate.Checked = true;
            }
            // 아니라면 MessageBox를 지워준다.
            else
            {
                textWorkID.Clear();

                checkNone.Checked = false;
                checkOK.Checked = false;
                checkCancel.Checked = false;
                checkInput.Checked = false;
                checkCustom.Checked = false;
                checkDuplicate.Checked = false;
                
                textMsgType.Clear();
            }
        }

        protected void MakeMsgType()
        {
            int nMsgBoxType = new int();
            if (checkNone.Checked)
                nMsgBoxType |= 0;

            if (checkOK.Checked)
                nMsgBoxType |= 0x00000001;

            if (checkCancel.Checked)
                nMsgBoxType |= 0x00000002;

            if (checkInput.Checked)
                nMsgBoxType |= 0x00000004;

            if (checkCustom.Checked)
                nMsgBoxType |= 0x00000008;

            textMsgType.Clear();
            textMsgType.AppendText(Convert.ToString(nMsgBoxType));
        }
    }
}