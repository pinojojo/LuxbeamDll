using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Runtime.InteropServices;


namespace UseDll
{
    public partial class Form1 : Form
    {


        [DllImport("LuxBeamController.dll",EntryPoint ="ConnectNative")]
        static extern int Connect();

        [DllImport("LuxBeamController.dll",EntryPoint ="LoadImageNative")]
        static extern int LoadImage(byte[] data, int size, int inum);

        [DllImport("LuxBeamController.dll", EntryPoint = "LoadImageFromFileNative")]
        static extern int LoadImageFromFile(string dir, int inum);


        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Connect();

        }

        private void button7_Click(object sender, EventArgs e)
        {
            {
                //string str1 = "00000";
                //string str2 = "11111";
                //StringBuilder sb = new StringBuilder();
                //int x = 1920 * 1080 / 5;
                //for (int i = 0; i < x; i++)
                //{
                //    if (i % 2 == 0)
                //    {
                //        sb.Append(str1);
                //    }
                //    else
                //    {
                //        sb.Append(str2);
                //    }
                //}
                //Console.WriteLine(sb.ToString());

                

            }

            {

                byte[] img = new byte[1920 * 1080];
                for (int i = 0; i < 1920 * 1080; i++)
                {
                    img[i] = (byte)(i % 255);
                }

                LoadImage(img, 1920 * 1080, 0);


            }

            


        

            //string dir = "acbd/fg/kkkbmp";

            //LoadImageFromFile(dir, 0);

        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {

        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
