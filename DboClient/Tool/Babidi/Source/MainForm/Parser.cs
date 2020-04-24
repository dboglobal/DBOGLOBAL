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
        
        public Boolean LoadData(DATA_SYNC eSync, DATA_TYPE eType, String filename)
        {
            // StreamReader로 파일을 로드하는 부분은 예외를 처리하도록 한다.
            try
            {
                if (eSync == DATA_SYNC.DATA_CLIENT)
                {
                    // 텍스트 파일을 Default Encoding(ANSI)로 StreamReader로 불러들인다.
                    using (StreamReader sr = new StreamReader(filename, Encoding.Unicode))
                    {
                        String line;

                        while ((line = sr.ReadLine()) != null)
                        {
                            switch (eType)
                            {
                                case DATA_TYPE.DATA_TYPE_DEF:
                                    ParseDef(ref m_tblClient, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_ALARM:
                                    ParseAlarmType(ref m_tblClient, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_KOR:
                                    ParseKorData(ref m_tblClient, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_JPN:
                                    ParseJpnData(ref m_tblClient, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_USER:
                                    {
                                        String strNote = sr.ReadLine();
                                        ParseUserData(ref m_tblClient, line, strNote);
                                        break;
                                    }
                            }
                        }

                        sr.Close();
                    }
                }
                else if (eSync == DATA_SYNC.DATA_SERVER)
                {
                    // 텍스트 파일을 Default Encoding(ANSI)로 StreamReader로 불러들인다.
                    using (StreamReader sr = new StreamReader(filename, Encoding.Unicode))
                    {
                        String line;

                        while ((line = sr.ReadLine()) != null)
                        {
                            switch (eType)
                            {
                                case DATA_TYPE.DATA_TYPE_DEF:
                                    ParseDef(ref m_tblServer, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_ALARM:
                                    ParseAlarmType(ref m_tblServer, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_KOR:
                                    ParseKorData(ref m_tblServer, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_JPN:
                                    ParseJpnData(ref m_tblServer, line);
                                    break;
                                case DATA_TYPE.DATA_TYPE_USER:
                                    {
                                        String strNote = sr.ReadLine();
                                        ParseUserData(ref m_tblServer, line, strNote);
                                        break;
                                    }
                            }
                        }

                        sr.Close();
                    }
                }
                else
                {
                }
            }
            catch (System.Exception e)
            {
                AddLog((int)Babidi.eLogType.eLOGTYPE_FILE, e.Source, e.Message);

                return false;
            }

            return true;
        }

       

        protected Boolean ParseDef(ref DataTable table, String strDef)
        {
            // 문자열의 앞과 뒤에 공백 제거
            strDef = strDef.Trim();

            // 문자열의 길이가 2글자가 안되면 리턴
            if (strDef.Length < 2)
                return false;

            // 각종 예외 토큰들 제외
            if (strDef[0] == '/' && strDef[1] == '/')
                return false;

            if (strDef[0] == '{' || strDef[0] == '}')
                return false;

            if (strDef[0] == ';' || strDef[0] == '#' )
                return false;

            if (strDef.Length > 4)
            {
                if (strDef[0] == 'e' &&
                    strDef[1] == 'n' &&
                    strDef[2] == 'u' &&
                    strDef[3] == 'm')
                    return false;
            }

            // 첫 글자부터 주석이 아니라면 주석이 있는 곳을 찾아서 기억한다.
            System.Int32 nCommentPos = strDef.IndexOf("//");

            // , 을 못찾았다면 정의가 아니다.
            System.Int32 nDefinePos = strDef.IndexOf(',');
            //if (nDefinePos < 0)
            //    return false;

            // Equal로 정의하는 경우도 있다. ( 주석 뒤에 오지 않는다면 )
            System.Int32 nDefinePosEqual = strDef.IndexOf('=');
            if (nDefinePosEqual >= 0 && nCommentPos > nDefinePosEqual )
            {
                nDefinePos = nDefinePosEqual;
            }

            if (nDefinePos < 0)
            {
                // 주석이 있다.
                if (nCommentPos >= 0)
                    nDefinePos = nCommentPos;
                else
                    nDefinePos = strDef.Length;
            }
            
            String strRealDef = strDef.Substring(0, nDefinePos);
            strRealDef = strRealDef.Trim();

            strRealDef = strRealDef.Trim();

            while (strRealDef.Contains("\t"))
            {
                System.Int32 nTabPos = strRealDef.IndexOf('\t');
                strRealDef = strRealDef.Remove(nTabPos, 1);
            }

            // Row 추가
            DataRow row = table.NewRow();
            row[COL_DEFINE] = strRealDef;

            // Comment 가 있으면 Comment도 추가
            if (nCommentPos > -1)
            {
                String strComment = strDef.Substring(nCommentPos, strDef.Length - nCommentPos);
                row[COL_COMMENT] = strComment;
            }

            table.Rows.Add(row);

            return true;
        }

        

        protected Boolean ParseAlarmType(ref DataTable table, String strAlarm)
        {
            // 문자열의 앞 뒤 공백 제거
            strAlarm = strAlarm.Trim();

            // 문자열의 길이가 2글자가 안되면 리턴
            if (strAlarm.Length < 2)
                return false;

            // 첫 글자부터 주석이라면 리턴
            if (strAlarm[0] == '/' && strAlarm[1] == '/')
                return false;

            // 첫 글자부터 주석이 아니라면 주석이 있는 곳을 찾아서 기억한다.
            System.Int32 nCommentPos = strAlarm.IndexOf("//");

            // = 을 못찾았다면 정의가 아니다.
            System.Int32 nDefinePos = strAlarm.IndexOf('=');
            if (nDefinePos < 0)
                return false;

            // 정의를 기억
            String strDef = strAlarm.Substring(0, nDefinePos);

            // ; 가 없으면 라인의 끝이 아니다. 이것은 에러!
            System.Int32 nEndPos = strAlarm.IndexOf(';');
            if (nEndPos <= 0)
                return false;

            // 타입을 기억
            String strType = strAlarm.Substring(nDefinePos + 1, strAlarm.Length - nDefinePos - 2);

            // 정의와 타입 두 개다 앞 뒤 공백을 제거해준다.
            strDef = strDef.Trim();
            strType = strType.Trim();

            // 1. DEFINE에 해당하는 ROW를 찾는다.
            DataRow row = table.Rows.Find(strDef);
            if (row == null)
            {
                AddLog((int)Babidi.eLogType.eLOGTYPE_ID, table.ToString(), strAlarm);
                return false;
            }

            row[COL_ALARMTYPE] = strType;

            return true;
        }

       
        protected Boolean ParseKorData(ref DataTable table, String strKor)
        {
            // 문자열의 앞 뒤 공백 제거
            strKor = strKor.Trim();

            // 문자열의 길이가 2글자가 안되면 리턴
            if (strKor.Length < 2)
                return false;

            // 첫 글자부터 주석이라면 리턴
            if (strKor[0] == '/' && strKor[1] == '/')
                return false;

            // 첫 글자부터 주석이 아니라면 주석이 있는 곳을 찾아서 기억한다.
            System.Int32 nCommentPos = strKor.IndexOf("//");

            // = 을 못찾았다면 정의가 아니다.
            System.Int32 nDefinePos = strKor.IndexOf('=');
            if (nDefinePos < 0)
                return false;

            // 정의를 기억
            String strDef = strKor.Substring(0, nDefinePos);
        
            // 타입을 기억
            String strData = strKor.Substring(nDefinePos + 1, strKor.Length - nDefinePos - 1);

            // 정의와 타입 두 개다 앞 뒤 공백을 제거해준다.
            strDef = strDef.Trim();
            strData = strData.Trim();

            // Data의 앞 뒤 따옴표를 제거해준다. ( 앞뒤 문자열 하나씩 잘라준다. )
            strData = strData.Substring(1, strData.Length - 2);

            DataRow row = table.Rows.Find(strDef);
            if (row == null)
            {
                AddLog((int)Babidi.eLogType.eLOGTYPE_ID, table.ToString(), strKor);
                return false;
            }

            row[COL_KOR] = strData;

            return true;
        }

      

        protected Boolean ParseJpnData(ref DataTable table, String strJpn)
        {
            // 문자열의 앞 뒤 공백 제거
            strJpn = strJpn.Trim();

            // 문자열의 길이가 2글자가 안되면 리턴
            if (strJpn.Length < 2)
                return false;

            // 첫 글자부터 주석이라면 리턴
            if (strJpn[0] == '/' && strJpn[1] == '/')
                return false;

            // 첫 글자부터 주석이 아니라면 주석이 있는 곳을 찾아서 기억한다.
            System.Int32 nCommentPos = strJpn.IndexOf("//");

            // = 을 못찾았다면 정의가 아니다.
            System.Int32 nDefinePos = strJpn.IndexOf('=');
            if (nDefinePos < 0)
                return false;

            // 정의를 기억
            String strDef = strJpn.Substring(0, nDefinePos);

            // 타입을 기억
            String strData = strJpn.Substring(nDefinePos + 1, strJpn.Length - nDefinePos - 1);

            // 정의와 타입 두 개다 앞 뒤 공백을 제거해준다.
            strDef = strDef.Trim();
            strData = strData.Trim();

            // Data의 앞 뒤 따옴표를 제거해준다. ( 앞뒤 문자열 하나씩 잘라준다. )
            strData = strData.Substring(1, strData.Length - 2);

            DataRow row = table.Rows.Find(strDef);
            if (row == null)
            {
                AddLog((int)Babidi.eLogType.eLOGTYPE_ID, table.ToString(), strJpn);
                return false;
            }

            row[COL_JPN] = strData;

            return true;
        }

      

        protected Boolean ParseUserData(ref DataTable table, String strDefRule, String strNote)
        {
            // 문자열의 앞 뒤 공백 제거
            strDefRule = strDefRule.Trim();

            // 문자열의 길이가 아무것도 없으면 리턴
            if (strDefRule.Length <= 0)
                return false;

            // = 을 못찾았다면 정의가 아니다.
            System.Int32 nDefinePos = strDefRule.IndexOf('=');
            if (nDefinePos < 0)
                return false;

            // 정의를 기억
            String strDef = strDefRule.Substring(0, nDefinePos);

            // 타입을 기억
            String strType = strDefRule.Substring(nDefinePos + 1, strDefRule.Length - nDefinePos - 1);

            // 정의와 타입 두 개다 앞 뒤 공백을 제거해준다.
            strDef = strDef.Trim();
            strType = strType.Trim();

            String[] aStrType = strType.Split(',');

            // 3보다 작으면 HTML, LENG의 Error
            if (aStrType.Length < 3)
            {
                // Log
                return false;
            }

            DataRow row = table.Rows.Find(strDef);
            if (row == null)
            {
                AddLog((int)Babidi.eLogType.eLOGTYPE_ID, table.ToString(), strDef);
                return false;
            }

            row[COL_HTML] = aStrType[0];
            row[COL_LINELENGTH] = String.Format("{0},{1}", aStrType[1], aStrType[2]);
            row[COL_NOTE] = strNote;

            return true;
        }
    }
}