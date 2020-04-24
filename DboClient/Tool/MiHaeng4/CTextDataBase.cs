using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using System.Reflection; // For Missing.Value and BindingFlags
using System.Runtime.InteropServices; // For COMException
using Excel = Microsoft.Office.Interop.Excel;
using System.Windows.Forms;

namespace MiHaeng4
{
    class CTextDataBase
    {
        public List<CTextData> listText;
        private ProgressBar pgb;

        public CTextDataBase(ProgressBar pgb)
        {
            listText = new List<CTextData>();
            this.pgb = pgb;
        }

        public void AddData(CTextData data)
        {
            listText.Add(data);
        }

        public void Clear()
        {
            listText.Clear();
        }

        public bool SaveXML(string strFileName)
        {
            try
            {
                XmlTextWriter writer = new XmlTextWriter(strFileName, Encoding.UTF8);
                writer.Formatting = Formatting.Indented;
                writer.WriteStartElement("dataroot");
                writer.WriteStartElement("Table_Data_KOR");
                writer.WriteElementString("F1", "Quest_Text_Index");
                writer.WriteElementString("F2", "Quest_Text");
                writer.WriteEndElement();
                foreach (CTextData data in listText)
                {
                    writer.WriteStartElement("Table_Data_KOR");
                    writer.WriteElementString("F1", data.Id);
                    writer.WriteElementString("F2", data.Text);
                    writer.WriteEndElement();
                }
                writer.WriteEndElement();
                writer.Close();
            }
            catch
            {
                MessageBox.Show("파일을 열수 없습니다. 읽기전용으로 설정되어 있는지 확인해 보세요","뵨태돌이 현준!!");
                return false;
            }            

            return true;
        }

        public bool ConvertTag()
        {
            pgb.Minimum = 0;
            pgb.Maximum = listText.Count;
            pgb.Value = 0;

            char[] splitChar = { '[', ']' };
            foreach(CTextData data in listText)
            {
                string[] words = data.Text.Split(splitChar);                

                for(int i = 1; i < words.Length; i = i + 2)
                {
                    if (words[i] == "user" || words[i] == "class" || words[i] == "race")
                        continue;

                    // Text All Data에서 ID를 검색한다.                          
                    string strConvertText = TextAllDataTable.ConvertTag(words[i]);
                    if(strConvertText != null)
                    {
                        data.Text = data.Text.Replace("[" + words[i] + "]", "[" + strConvertText + "]");
                    }
                }

                pgb.Value = Math.Min(pgb.Maximum, pgb.Value + 1);
            }
            return true;
        }
    }
}
