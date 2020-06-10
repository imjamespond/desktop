using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    class Clipboard
    {
        [DllImport("User32.dll")]
        public static extern int
          SetClipboardViewer(int hWndNewViewer);

        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern bool
               ChangeClipboardChain(IntPtr hWndRemove,
                                    IntPtr hWndNewNext);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern int SendMessage(IntPtr hwnd, int wMsg,
                                             IntPtr wParam,
                                             IntPtr lParam);

        public IntPtr nextClipboardViewer;
        private Form1 form1;

        public Clipboard(Form1 form1)
        {
            this.form1 = form1;
        }

        public void init(int handle)
        {
            nextClipboardViewer = (IntPtr)SetClipboardViewer((int)handle);
        }

        public void WndProc(ref System.Windows.Forms.Message m)
        {
            // defined in winuser.h
            const int WM_DRAWCLIPBOARD = 0x308;
            const int WM_CHANGECBCHAIN = 0x030D;

            switch (m.Msg)
            {
                case WM_DRAWCLIPBOARD:
                    Clipboard.SendMessage(this.nextClipboardViewer, m.Msg, m.WParam, m.LParam);
                    IDataObject iData = new DataObject();
                    iData = System.Windows.Forms.Clipboard.GetDataObject();
                    if (iData.GetDataPresent(DataFormats.Rtf))
                        Console.WriteLine((string)iData.GetData(DataFormats.Rtf));
                    //richTextBox1.Rtf = (string)iData.GetData(DataFormats.Rtf);
                    else if (iData.GetDataPresent(DataFormats.Text))
                        form1.OnClipboradChange((string)iData.GetData(DataFormats.Text));
                    else
                        Console.WriteLine("[Clipboard data is not RTF or ASCII Text]");
                    break;

                case WM_CHANGECBCHAIN:
                    if (m.WParam == this.nextClipboardViewer)
                        this.nextClipboardViewer = m.LParam;
                    else
                        Clipboard.SendMessage(this.nextClipboardViewer, m.Msg, m.WParam, m.LParam);
                    break;

                default:
                    form1.BaseWndProc(ref m);
                    break;
            }
        }
    }
}
