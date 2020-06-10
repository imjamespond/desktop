using System;
using System.Json;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        [DllImport("user32.dll")]
        static extern bool SetForegroundWindow(IntPtr hWnd);

        private DllProgram dllProgram;
        private Clipboard clipboard;

        public Form1()
        {
            InitializeComponent();

            this.clipboard = new Clipboard(this);
            this.clipboard.init((int)this.Handle);
        }

        public Form1(DllProgram dllProgram)
        { 
            InitializeComponent();

            this.dllProgram = dllProgram;
            this.clipboard = new Clipboard(this);
            this.clipboard.init((int)this.Handle);
        } 

        public void OnClipboradChange(string text)
        {
            //MessageBox.Show(str);
            richTextBox1.Text = text;
            SetForegroundWindow(this.Handle);
        }

        public void BaseWndProc(ref System.Windows.Forms.Message m)
        {
            base.WndProc(ref m);
        }

        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            clipboard.WndProc(ref m);
        }

        private void button1_Click(object sender, EventArgs e)
        { 
            try
            {
                this.dllProgram.AddEntity(richTextBox1.Text);
            }
            catch (Exception ex)
            {

                Console.WriteLine(ex.Message);
            }
        }
         
    }
}
