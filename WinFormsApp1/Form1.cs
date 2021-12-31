using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        [DllImport("test1.dll", EntryPoint = "Print")]
        static extern void Hello(byte[] str);

        [DllImport("test1.dll", EntryPoint = "Sum")]
        static extern int Sum(int a, int b);

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            //int sum = Sum(33, 22);
            //System.Console.WriteLine(sum);

            //Hello(Encoding.UTF8.GetBytes("Foobar测试"));
            //Console.WriteLine(Encoding.UTF8.GetString(hello));


            textBox1.Text = textBox1.Text + DateTime.Now.ToString() + "\r\n";

        }
    }
}
