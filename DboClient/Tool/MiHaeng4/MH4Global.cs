using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace MiHaeng4
{
    public enum LocalizeSource
    {
        LOCAL_KOREAN    = 1,
        LOCAL_JAPANESE  = 2,
        LOCAL_ENGLISH = 3,
    }

    class MH4Global
    {
        public static Form1 mainForm;
        public static ProgressBar pgbXLS;
        public static ProgressBar pgbXML;
        public static Label lbXLS;
        public static Label lbXML;
        public static LocalizeSource localSource = LocalizeSource.LOCAL_ENGLISH;
        public static int[] colList = new int[] { 0, 3, 4, 5, 12, 13, 17, 18, 19, 20, 21,
                                  42, 43, 44, 47, 142, 143, 144, 145, 146, 147,
                                  148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
                                  158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
                                  168, 169, 170, 171, 172, 173, 174, 175, 176};

        public const string MHVer = "Ú­ú¼4 (DBO Quest Text Convert Tool) - 9.7.15 by agebreak";
        public const string mergeXMLFileName = "Table_Quest_Text_Data.xml";
        public const string mergeRDFFileName = "Table_Quest_Text_Data.rdf";
        public const int THREAD_MAX = 5;


        // ½ºÆó¼È Äù½ºÆ® ¹®¼­ÀÎÁö ¹ÝÈ¯ÇÑ´Ù.
        static public bool IsSpecialQuestXLS(string strFileName)
        {
            string strUpperFileName = strFileName.ToUpper();
            return strUpperFileName.Contains("(SPECIAL)");
        }
    }   
}
