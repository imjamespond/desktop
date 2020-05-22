using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public static IntPtr HWnd;

        private Helper hp = new Helper();
        private Clipboard clip = new Clipboard();

        public Form1()
        {
            InitializeComponent();

            this.clip.init((int)this.Handle);

            //this.listBox1.Items.AddRange(new object[] {});
            //for (int i = 0; i < 20; i++)
            //{
            //    this.listBox1.Items.Add(string.Format("haha{0}", i));
            //}

            this.listView1.FullRowSelect = true;
            //this.listView1.SelectedIndexChanged += new EventHandler(this.ListViewSingleSelected);
            this.listView1.MouseClick += new MouseEventHandler(this.HandleMouseDown);

            Form1.HWnd = this.Handle;
        }

        //private void ListViewSingleSelected(object sender, EventArgs e)
        //{
        //    ListView.SelectedListViewItemCollection items = this.listView1.SelectedItems;
        //    foreach (ListViewItem item in items)
        //    {
        //        hp.SetText(item.Text);
        //    }
        //}

        void HandleMouseDown(object sender, MouseEventArgs e)
        {
            ListViewHitTestInfo info = listView1.HitTest(e.X, e.Y);
            if (info.SubItem != null)
            {
                hp.SetText(info.SubItem.Text);
            }
        }

        public void SetListView(ArrayList list)
        {
            this.listView1.Items.Clear();
            ArrayList items = new ArrayList();
            foreach (string[] arr in list)
            {
                items.Add(new ListViewItem(arr, -1));
            }
            this.listView1.Items.AddRange((ListViewItem[])items.ToArray(typeof(ListViewItem)));
        }

        public void SetLocation()
        {
        }


        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            // defined in winuser.h
            const int WM_DRAWCLIPBOARD = 0x308;
            const int WM_CHANGECBCHAIN = 0x030D;

            switch (m.Msg)
            {
                case WM_DRAWCLIPBOARD: 
                    Clipboard.SendMessage(clip.nextClipboardViewer, m.Msg, m.WParam,
                                m.LParam);
                    IDataObject iData = new DataObject();
                    iData = System.Windows.Forms.Clipboard.GetDataObject();
                    if (iData.GetDataPresent(DataFormats.Rtf))
                        Console.WriteLine((string)iData.GetData(DataFormats.Rtf));
                    //richTextBox1.Rtf = (string)iData.GetData(DataFormats.Rtf);
                    else if (iData.GetDataPresent(DataFormats.Text))
                        Console.WriteLine((string)iData.GetData(DataFormats.Text));
                    else
                        Console.WriteLine("[Clipboard data is not RTF or ASCII Text]");

                    break;

                case WM_CHANGECBCHAIN:
                    if (m.WParam == clip.nextClipboardViewer)
                        clip.nextClipboardViewer = m.LParam;
                    else
                        Clipboard.SendMessage(clip.nextClipboardViewer, m.Msg, m.WParam,
                                    m.LParam);
                    break;

                default:
                    base.WndProc(ref m);
                    break;
            }
        }
    }
}
