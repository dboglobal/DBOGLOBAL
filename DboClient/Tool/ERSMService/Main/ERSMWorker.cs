using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

using ICSharpCode.SharpZipLib.Zip;
using ICSharpCode.SharpZipLib.Zip.Compression.Streams;

namespace ERSMService.Main
{
    class ERSMWorker
    {
        private List<ErrorReportFile> listErrorReport;

        public void StartWork()
        {
            listErrorReport = new List<ErrorReportFile>();

            Console.WriteLine(DateTime.Now.ToShortTimeString() + " #Work Thread is Start...");

            // 1. 파일의 정보들을 가져온다.
            if (GetListZipFiles(ERSMConfig.ERSFolder))
            {
                Console.WriteLine(DateTime.Now.ToShortTimeString() + "Get Error Report List(Zip Files) Successs...");
            }
            else
            {
                Console.WriteLine(DateTime.Now.ToShortTimeString() + "Get Error Report List(Zip FIles) Fail!!");
                return;
            }

            // 2. 파일 이름을 변경하여 하위 폴더에 저장한다.
            if (RenameZipFiles())
            {
                Console.WriteLine(DateTime.Now.ToShortTimeString() + "Error Report File Process Success..");
            }
            else
            {
                Console.WriteLine(DateTime.Now.ToShortTimeString() + "Error Report File Process Fail!");
                return;
            }
        }

        private bool GetListZipFiles(string folderName)
        {
            // 1. 하위 폴더를 검색한다. (에러 리포트 파일은 루트 폴더밑의 1단계 하위 폴더에 존재한다)
            string[] subDirList =  Directory.GetDirectories(folderName);            
            
            // 2. 하위 폴더들의 ZipFile들을 검색한다.
            foreach (string subDir in subDirList)
            {
                string[] zipFiles = Directory.GetFiles(subDir, "*.zip", SearchOption.TopDirectoryOnly);
                foreach (string zipFile in zipFiles)
                {
                    // 파일 정보를 리스트에 담는다.                    
                    DateTime dtCreate = File.GetCreationTime(zipFile);
                    ErrorReportFile reportFile = new ErrorReportFile();
                    reportFile.folderName = subDir;             // 폴더 이름
                    reportFile.fileName = Path.GetFileName(zipFile);              // 파일 이름
                    reportFile.fileDate = string.Format("{0:D}{1:00}{2:00}", dtCreate.Year, dtCreate.Month, dtCreate.Day); // 파일 생성 날짜
                    reportFile.userID = GetUserIDFromZipFile(zipFile);
                    listErrorReport.Add(reportFile);
                }
            }

            return true;
        }

        /// <summary>
        /// zip 파일내에서 유저 ID를 가져온다.
        /// </summary>
        /// <param name="zipFileName"></param>
        /// <returns></returns>
        private string GetUserIDFromZipFile(string zipFileName)
        {
            ZipInputStream zis = new ZipInputStream(File.OpenRead(zipFileName));
            if (zis == null)
            {
                Console.WriteLine("UnZip is Fail!! " + zipFileName);
                return null;
            }

            ZipEntry theEntry;
            while((theEntry = zis.GetNextEntry()) != null)
            {
                string fileName = Path.GetFileName(theEntry.Name);
                if(fileName == ERSMConfig.idFileName) 
                {
                    XmlTextReader reader = new XmlTextReader(zis);
                    reader.WhitespaceHandling = WhitespaceHandling.None;   
                    while(reader.Read())
                    {
                        reader.MoveToContent();
                        
                        // 유저 ID가 들어있는 노드를 찾은후에 그 값을 반환한다.
                        if(reader.NodeType == XmlNodeType.Element && reader.Name == ERSMConfig.nodeUserIDElement)
                        {
                            reader.Read();
                            zis.Close();
                            return reader.Value;
                        }
                    }
                }
            }

            zis.Close();
            return null;
        }

        private bool RenameZipFiles()
        {
            foreach(ErrorReportFile report in listErrorReport)
            {
                // 날짜별 폴더를 생성한다.
                string newFolderName = report.folderName + @"\" + report.fileDate;
                Directory.CreateDirectory(newFolderName);

                // 생성된 폴더에 파일 이름을 변경하여 넣는다.
                string oldFileName = report.folderName + @"\" + report.fileName;
                string newFileName = newFolderName + @"\" + report.userID + "_" + report.fileName;
                try
                {
                    File.Move(oldFileName, newFileName);
                }
                catch(Exception ex)
                {
                    continue;
                }

                Console.WriteLine("Rename File : " + oldFileName + " -> " + newFileName);
            }
            return true;
        }
    }    
}
