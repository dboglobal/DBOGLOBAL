using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.IO;

namespace MiHaeng4
{
    enum ConvertState
    {
        CONVERT_XML,        
        CONVERT_TAG_XLS,
    };

    public partial class Form1 : Form
    {
        private ConvertState convertState;
        private bool bReadyWork;

        public Form1()
        {
            // 멀티 스레드에서도 다른 스레드의 컨트롤에 접근 가능
            Control.CheckForIllegalCrossThreadCalls = false;

            bReadyWork = false;

            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Text = MH4Global.MHVer;
            MH4Global.pgbXLS = pgbCurrent;
            MH4Global.lbXLS = lbCurrent;
            MH4Global.pgbXML = pgbXML;
            MH4Global.lbXML = lbInfoXML;
            MH4Global.mainForm = this;
            
            RegistryKey kr = Registry.CurrentUser.OpenSubKey("MiHaeng4");
            if(kr != null)
            {
                // XLS 리스트를 가져온다.
                if(kr.GetValue("XLS_COUNT") != null)
                {
                    int xlsCnt = (int)kr.GetValue("XLS_COUNT");
                    for (int i = 0; i < xlsCnt; ++i)
                    {
                        string xlsFileName = (string)kr.GetValue("XLS_" + i.ToString());
                        listXLS.Items.Add(xlsFileName);
                    }                
                }

                // XML 리스트를 가져온다
                if(kr.GetValue("XML_COUNT") != null)
                {
                    int xmlCnt = (int)kr.GetValue("XML_COUNT");
                    for(int i = 0; i < xmlCnt; ++i)
                    {
                        string xmlFileName = (string)kr.GetValue("XML_" + i.ToString());
                        listXML.Items.Add(xmlFileName);
                    }
                }

                // Text All Data File 경로를 레지스트리에서 가져온다
                if(kr.GetValue("TextAllData") != null)
                {
                    textDataFile.Text = (string)kr.GetValue("TextAllData");
                }
                
                kr.Close();
            }            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 현재 리스트의 내용을 레지스트리에 저장한다.            
            RegistryKey kr = Registry.CurrentUser.CreateSubKey("MiHaeng4");

            // XLS 리스트
            kr.SetValue("XLS_COUNT", listXLS.Items.Count);
            for(int i = 0; i < listXLS.Items.Count; ++i)
            {
                kr.SetValue("XLS_" + i.ToString(), listXLS.Items[i].Text);
            }

            // XML 리스트
            kr.SetValue("XML_COUNT", listXML.Items.Count);
            for (int i = 0; i < listXML.Items.Count; ++i)
            {
                kr.SetValue("XML_" + i.ToString(), listXML.Items[i].Text);
            }

            kr.Close();

            TextAllDataTable.Close();
        }

        private void btAddXLS_Click(object sender, EventArgs e)
        {
            if (openXLS.ShowDialog() == DialogResult.OK)
            {
                foreach(string strFileName in openXLS.FileNames)
                {
                    if(!listXLS.Items.ContainsKey(strFileName))                    
                    {
                        listXLS.Items.Add(strFileName);
                    }                    
                }
            }            
        }

        private void btSelectAll_Click(object sender, EventArgs e)
        {
            if(listXLS.CheckedItems.Count > 0)
            {
                foreach(ListViewItem item in listXLS.Items)
                {
                    item.Checked = false;
                }                
            }
            else
            {
                foreach (ListViewItem item in listXLS.Items)
                {
                    item.Checked = true;
                }                
            }
        }

        private void btClear_Click(object sender, EventArgs e)
        {
            listXLS.Items.Clear();            
        }

        private void btDelete_Click(object sender, EventArgs e)
        {
            foreach(ListViewItem item in listXLS.Items)
            {
                foreach (ListViewItem selItem in listXLS.CheckedItems)
                {
                    if(item.Equals(selItem))
                    {
                        listXLS.Items.Remove(selItem);
                    }
                }
            }            
        }

        private void btConvertNoneTagXML_Click(object sender, EventArgs e)
        {
            if(listXLS.CheckedItems.Count == 0)
            {
                MessageBox.Show("Check 1 more files!");
                return;
            }

            for (int i = 0; i < listXLS.CheckedItems.Count; ++i)
            {
                ConvertXML convertXML = new ConvertXML(pgbCurrent, lbCurrent);
                convertXML.LoadXLS(listXLS.CheckedItems[i].Text);                
                convertXML.ConvertNoneTagXML();                
            }
        }

        private void btConvertTagXML_Click(object sender, EventArgs e)
        {
            if (listXLS.CheckedItems.Count == 0)
            {
                MessageBox.Show("Check 1 more files!");
                return;
            }

            if(textDataFile.Text == "")
            {
                MessageBox.Show("Browse Text All Data Table File!!");
                return;
            }

            LocalSource selectLocal = new LocalSource();
            selectLocal.SetLabelText("Select Source Localize of XLS Files");
            selectLocal.cmbSourceLocal.Items.Clear();
            selectLocal.cmbSourceLocal.Items.Add("Korean");
            selectLocal.cmbSourceLocal.Items.Add("Japanese");
            selectLocal.cmbSourceLocal.SelectedIndex = 0;
            selectLocal.ShowDialog();

            if (MH4Global.localSource != LocalizeSource.LOCAL_KOREAN && MH4Global.localSource != LocalizeSource.LOCAL_JAPANESE)
                return;
            
            if(TextAllDataTable.OpenTextAllData(textDataFile.Text, MH4Global.localSource))
            {
                ConvertTagXML();
            }
            else
            {
                convertState = ConvertState.CONVERT_XML;
                bReadyWork = true;
            }
        }

        private void btTagXLS_Click(object sender, EventArgs e)
        {
            if (listXLS.CheckedItems.Count == 0)
            {
                MessageBox.Show("Check 1 more files!");
                return;
            }

            if (textDataFile.Text == "")
            {
                MessageBox.Show("Browse Text All Data Table File!!");
                return;
            }

            LocalSource selectLocal = new LocalSource();
            selectLocal.SetLabelText("Select Convert XLS Work to Do");
            selectLocal.cmbSourceLocal.Items.Clear();
            selectLocal.cmbSourceLocal.Items.Add("Korean Tag to Index");
            selectLocal.cmbSourceLocal.Items.Add("Korean Tag to Japanese");
            selectLocal.cmbSourceLocal.SelectedIndex = 0;
            selectLocal.ShowDialog();

            if (TextAllDataTable.OpenTextAllData(textDataFile.Text, MH4Global.localSource))
            {
                ConvertTagXLS();
            }
            else
            {
                convertState = ConvertState.CONVERT_TAG_XLS;
                bReadyWork = true;
            }
        }

        // 텍스트 데이터가 다 읽히면 호출되는 메소드
        public void OnLoadedTextAllData()
        {
            if (bReadyWork == false)
                return;

            if (convertState == ConvertState.CONVERT_XML)
            {
                ConvertTagXML();
            }
            else if (convertState == ConvertState.CONVERT_TAG_XLS)
            {
                ConvertTagXLS();
            }                        
        }

        private void ConvertTagXML()
        {
            for (int i = 0; i < listXLS.CheckedItems.Count; ++i)
            {
                ConvertXML convertXML = new ConvertXML(pgbCurrent, lbCurrent);
                convertXML.LoadXLS(listXLS.CheckedItems[i].Text);
                convertXML.ConvertTagXML();
            }

            bReadyWork = false;
        }
        
        private void ConvertTagXLS()
        {
            for (int i = 0; i < listXLS.CheckedItems.Count; ++i)
            {
                ConvertXLS convertXLS = new ConvertXLS();
                convertXLS.ConvertTagXLS(listXLS.CheckedItems[i].Text, MH4Global.localSource);
            }

            bReadyWork = false;
        }

        private void btDataFile_Click(object sender, EventArgs e)
        {
            if(openTextTable.ShowDialog() == DialogResult.OK)
            {
                textDataFile.Text = openTextTable.FileName;

                RegistryKey kr = Registry.CurrentUser.CreateSubKey("MiHaeng4");
                kr.SetValue("TextAllData", openTextTable.FileName);
                kr.Close();
            }
        }       


        //////////////////////////////////////////////////////////////////////////
        
        private void btAddXML_Click(object sender, EventArgs e)
        {
            if(openXML.ShowDialog() == DialogResult.OK)
            {
                foreach(string fileName in openXML.FileNames)
                {
                    if(!listXML.Items.ContainsKey(fileName))
                    {
                        listXML.Items.Add(fileName);
                    }
                }
            }
        }

        private void btSelectXML_Click(object sender, EventArgs e)
        {
            if(listXML.CheckedItems.Count > 0)
            {
                foreach(ListViewItem item in listXML.Items)
                {
                    item.Checked = false;
                }
            }
            else
            {
                foreach(ListViewItem item in listXML.Items)
                {
                    item.Checked = true;
                }
            }
        }

        private void btClearXML_Click(object sender, EventArgs e)
        {
            listXML.Clear();
        }

        private void btRemoveXML_Click(object sender, EventArgs e)
        {
            foreach(ListViewItem item in listXML.Items)
            {
                foreach(ListViewItem checkItem in listXML.CheckedItems)
                {
                    if(item.Equals(checkItem))
                    {
                        listXML.Items.Remove(checkItem);
                    }
                }
            }
        }

        private void btMergeXML_Click(object sender, EventArgs e)
        {
            if(listXML.CheckedItems.Count == 0)
            {
                MessageBox.Show("Check 1 more files!");
                return;
            }

            string[] fileNames = new string[listXML.CheckedItems.Count];
            for(int i = 0; i < listXML.CheckedItems.Count; ++i)
            {
                fileNames[i] = listXML.CheckedItems[i].Text;
            }

            MergeProcess mergePrcoess = new MergeProcess();
            mergePrcoess.MergeXML(fileNames);            
        }

        private void btConvertRDF_Click(object sender, EventArgs e)
        {
            if (listXML.CheckedItems.Count == 0)
            {
                MessageBox.Show("Check 1 more files!");
                return;
            }

            foreach(ListViewItem item in listXML.CheckedItems)
            {
                string rdfFileName = item.Text.Replace(".XML", ".rdf");
                rdfFileName = rdfFileName.Replace(".xml", ".rdf");
                string[] fileNames = new string[] { item.Text };
                ConvertRDF convertRDF = new ConvertRDF();
                convertRDF.ConvertToRDF(fileNames, rdfFileName);
            }

            lbInfoXML.Text = "Convert RDF Complete!";
        }

        private void btMergeRDF_Click(object sender, EventArgs e)
        {
            if (listXML.CheckedItems.Count == 0)
            {
                MessageBox.Show("Check 1 more files!");
                return;
            }

            string[] fileNames = new string[listXML.CheckedItems.Count];
            for (int i = 0; i < listXML.CheckedItems.Count; ++i)
            {
                fileNames[i] = listXML.CheckedItems[i].Text;                
            }

            ConvertRDF convertRDF = new ConvertRDF();
            convertRDF.ConvertToRDF(fileNames, MH4Global.mergeRDFFileName);

            lbInfoXML.Text = "Merge to RDF Complete!";            
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            
        }

    }
}