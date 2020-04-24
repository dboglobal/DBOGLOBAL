using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection; // For Missing.Value and BindingFlags
using System.Runtime.InteropServices; // For COMException
using Excel = Microsoft.Office.Interop.Excel;
using System.Windows.Forms;
using System.Threading;

namespace MiHaeng4
{
    class TextAllData
    {
        public string id;              // ID 
        public string type;            // 타입
        public string colRow;          // 행렬
    };

    class TextAllDataTable
    {
        private static Excel.Application appTextAllData;
        private static Excel.Range usedRange;
        private static Excel.Range usedRangeJap;
        private static Dictionary<string, TextAllData> textDataMap;        
        private static Dictionary<string, TextAllData> currMap;
        private static Dictionary<string, string> dicConvertTag;
        private static Dictionary<string, string> dicConvertJapan;
        private static string textAllDataFileName;
        private static Object thisLock = new Object();

        public static void Close()
        {
            if(appTextAllData != null)
            {
                appTextAllData.ActiveWorkbook.Close(false, Missing.Value, Missing.Value);
            }
        }

        public static void InsertMap(string col, string row, string type)
        {
            Excel.Range rngText = usedRange.get_Range(col + row, Missing.Value);            
            Excel.Range rngID = (Excel.Range)usedRange.Cells[rngText.Row, rngText.Column - 1];

            lock(thisLock)
            {
                if (rngText.Value2 == null || rngID.Value2 == null)
                    return;

                string keyText = rngText.Value2.ToString();
                if (currMap.ContainsKey(keyText))
                    return;

                TextAllData data = new TextAllData();
                data.id = rngID.Value2.ToString();
                data.type = type;
                data.colRow = col + row;

                currMap.Add(keyText, data);
            }           
        }

        // 이미 로딩되었으면 True, 스레드로 넘기면 false를 반환
        public static bool OpenTextAllData(string strFileName, LocalizeSource localSource)
        {
            if(localSource == LocalizeSource.LOCAL_KOREAN ||
               localSource == LocalizeSource.LOCAL_JAPANESE)
            {
                if (textDataMap != null && textDataMap.Count > 0)
                    return true;

                textDataMap = new Dictionary<string, TextAllData>();
                dicConvertTag = new Dictionary<string, string>();
                dicConvertJapan = new Dictionary<string, string>();

                currMap = textDataMap;
            }
            // 일본어 태그 변경은 필요없다.
            //else if(localSource == LocalizeSource.LOCAL_JAPANESE)
            //{
            //    if (textDataMapJap != null && textDataMapJap.Count > 0)
            //        return true;

            //    textDataMapJap = new Dictionary<string, TextAllData>();
            //    dicConvertJapan = new Dictionary<string, string>();

            //    currMap = textDataMapJap;
            //}            

            if (appTextAllData == null)
            {
                textAllDataFileName = strFileName;

                appTextAllData = new Excel.Application();
                appTextAllData.Workbooks.Open(strFileName, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value,
                                          Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
            }

            Excel.Workbook workBook = appTextAllData.Workbooks[1];
            workBook.Activate();

            Excel.Worksheet sheet = (Excel.Worksheet)workBook.Sheets[LocalizeSource.LOCAL_KOREAN]; 
            usedRange = sheet.UsedRange;
            int rowCnt = usedRange.Rows.Count;

            sheet = (Excel.Worksheet)workBook.Sheets[LocalizeSource.LOCAL_JAPANESE];
            usedRangeJap = sheet.UsedRange;

            MH4Global.pgbXLS.Minimum = 2;
            MH4Global.pgbXLS.Maximum = rowCnt;
            MH4Global.pgbXLS.Value = 2;                        

            MH4Global.lbXLS.Text = "Load Text All Table...";
            MH4Global.lbXLS.Update();

            Thread[] trdList = new Thread[MH4Global.THREAD_MAX];
            for (int i = 0; i < MH4Global.THREAD_MAX; ++i)
            {
                ThreadTask task = new ThreadTask(i + 2, rowCnt + 1, MH4Global.THREAD_MAX);
                trdList[i] = new Thread(task.InsertTextAllData);
                trdList[i].IsBackground = true;
                trdList[i].Start();
            }

            return false;
        }

        public static string ConvertTag(string text)
        {
            // 캐시에 있는지 찾아본다.
            string strConvertText;            
            if(dicConvertTag.TryGetValue(text, out strConvertText))
            {
                return strConvertText;
            }

            TextAllData data = null;
            StringBuilder sbConvertText;
            bool bExist = false;

            if(MH4Global.localSource == LocalizeSource.LOCAL_KOREAN)
            {
                if (textDataMap.TryGetValue(text, out data))
                {
                    bExist = true;                    
                }
            }            

            if(bExist)
            {
                sbConvertText = new StringBuilder(data.type);
                sbConvertText.Append(" = \"");
                sbConvertText.Append(data.id);
                sbConvertText.Append("\"");

                dicConvertTag.Add(text, sbConvertText.ToString());   // 캐시에 넣어둔다

                return sbConvertText.ToString();
            }

            return strConvertText;
        }

        // 일본어 버전 텍스트로 변환해서 반환한다.
        public static string ConvertJapanText(string text)
        {
            string strConvertText;
            if(dicConvertJapan.TryGetValue(text, out strConvertText))
            {
                return strConvertText;
            }
            else
            {
                TextAllData data;
                if(textDataMap.TryGetValue(text, out data))
                {
                    Excel.Range rng = (Excel.Range)usedRangeJap.get_Range(data.colRow, Missing.Value);
                    strConvertText = rng.Value2.ToString();

                    dicConvertJapan.Add(text, strConvertText);
                    return strConvertText;
                }
            }

            return null;
        }
    }
}
