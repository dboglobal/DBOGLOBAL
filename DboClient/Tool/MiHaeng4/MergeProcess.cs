using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace MiHaeng4
{
    class MergeProcess
    {
        public bool MergeXML(string[] fileNames)
        {
            StreamWriter swMergeXML = new StreamWriter(MH4Global.mergeXMLFileName);

            MH4Global.pgbXML.Minimum = 0;
            MH4Global.pgbXML.Maximum = fileNames.Length;
            MH4Global.pgbXML.Value = 0;

            // HTML 헤더
            swMergeXML.WriteLine("<dataroot>");
            swMergeXML.WriteLine("<Table_Data_KOR>");
            swMergeXML.WriteLine("<F1>Quest_Text_Index</F1>");
            swMergeXML.WriteLine("<F2>Quest_Text</F2>");
            swMergeXML.WriteLine("</Table_Data_KOR>");

            for(int i = 0; i < fileNames.Length; ++i)
            {
                string fileName = fileNames[i];
                string nakedName = Path.GetFileName(fileName);
                MH4Global.lbXML.Text = "Merge : " + nakedName;
                MH4Global.lbXML.Update();

                StreamReader sr = new StreamReader(fileName);
                int cnt = 0;
                while(!sr.EndOfStream) 
                {
                    if(cnt < 5)
                    {
                        sr.ReadLine();
                        ++cnt;
                    }
                    else
                    {
                        string line = sr.ReadLine();
                        if(!sr.EndOfStream) // 마지막 줄인 경우에는 XML의 풋이기 때문에 쓰지 않는다.
                        {
                            swMergeXML.WriteLine(line);
                        }                        
                    }                 
                }                
                sr.Close();

                MH4Global.pgbXML.Value += 1;
            }

            swMergeXML.WriteLine("</dataroot>");

            swMergeXML.Flush();
            swMergeXML.Close();

            MH4Global.lbXML.Text = "Merge Complete => " + MH4Global.mergeXMLFileName;
            MH4Global.lbXML.Update();

            return true;
        }

        public bool MergeRDF(string[] fileNames)
        {
            return true;
        }
    }
}
