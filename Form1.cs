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

        public Form1()
        {
            InitializeComponent();

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
    }
}
