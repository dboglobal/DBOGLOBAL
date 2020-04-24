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
    class ConvertXML
    {
        private CTextDataBase dataBase;
        private string fileName;
        private ProgressBar pgbCurrent;
        private Label lbCurrent;        

        public ConvertXML(ProgressBar pgbCurrent, Label lbCurrent)
        {
            dataBase = new CTextDataBase(pgbCurrent);
            this.pgbCurrent = pgbCurrent;
            this.lbCurrent = lbCurrent;
        }

        public bool ConvertNoneTagXML()
        {
            fileName = fileName.Replace("XLS", "XML");
            fileName = fileName.Replace("xls", "XML");
            string nakedFileName = System.IO.Path.GetFileName(fileName);
            lbCurrent.Text = "Saving... " + nakedFileName;
            dataBase.SaveXML(fileName);
            lbCurrent.Text = "Convert Complete : " + nakedFileName;

            return true;
        }

        public bool ConvertTagXML()
        {
            lbCurrent.Text = "Converting Tag...";
            lbCurrent.Update();
            dataBase.ConvertTag();
            ConvertNoneTagXML();

            return true;
        }

        public bool LoadXLS(string strFileName)
        {
            fileName = strFileName;
            string nakedFileName = System.IO.Path.GetFileName(strFileName);
            Excel.Application app = new Excel.Application();            
            app.Workbooks.Open(strFileName, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value,
                               Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
            
            Excel.Workbook workBook = app.Workbooks[1];
            workBook.Activate();
            Excel.Worksheet sheet = (Excel.Worksheet)workBook.Sheets[1];
            
            pgbCurrent.Minimum = 2;
            pgbCurrent.Maximum = sheet.UsedRange.Rows.Count;
            pgbCurrent.Value = 2;
            int rowCont = sheet.UsedRange.Rows.Count;            

            MH4Global.lbXLS.Text = "Loading and Converting... : " + nakedFileName;
            MH4Global.lbXLS.Update();

            // ÀÏ¹Ý Äù½ºÆ® ¹®¼­ÀÎÁö, ½ºÆó¼È Äù½ºÆ® ¹®¼­ÀÎÁö ÆÄ¾ÇÇÑ´Ù.
            bool bIsSpecialXLS = MH4Global.IsSpecialQuestXLS(strFileName);

            Thread[] trdList = new Thread[MH4Global.THREAD_MAX];
            for (int i = 0; i < MH4Global.THREAD_MAX; ++i)
            {
                ThreadTask task = new ThreadTask(dataBase, i + 2, rowCont + 1, MH4Global.THREAD_MAX, sheet.UsedRange);
                if(bIsSpecialXLS)
                {
                    trdList[i] = new Thread(task.LoadXLSSpecial);
                }
                else
                {
                    trdList[i] = new Thread(task.LoadXLS);
                }
                
                trdList[i].IsBackground = true;
                trdList[i].Start();
            }

            for (int i = 0; i < MH4Global.THREAD_MAX; ++i)
            {
                trdList[i].Join();
            }

            app.ActiveWorkbook.Close(false, Missing.Value, Missing.Value);

            return true;
        }             
    }
}
