using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks; 
using Newtonsoft.Json.Linq;
using System.Collections;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
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

        public void init(int handle)
        {
            nextClipboardViewer = (IntPtr)SetClipboardViewer((int) handle);

        }
    }
}
