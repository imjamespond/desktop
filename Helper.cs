using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Forms;
using Rectg = System.Drawing.Rectangle;
using Point = System.Drawing.Point;
using System.Collections;

namespace WindowsFormsApplication1
{
    class Helper
    {

        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern int GetWindowThreadProcessId(IntPtr hwnd, out int ID);


        private delegate bool WNDENUMPROC(IntPtr hWnd, int lParam);
        //用来遍历所有窗口 
        [DllImport("user32.dll")]
        private static extern bool EnumWindows(WNDENUMPROC lpEnumFunc, int lParam);
        //获取窗口Text 
        [DllImport("user32.dll")]
        private static extern int GetWindowTextW(IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)]StringBuilder lpString, int nMaxCount);
        //获取窗口类名 
        [DllImport("user32.dll")]
        private static extern int GetClassNameW(IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)]StringBuilder lpString, int nMaxCount);


        #region 获取另一系统文本框值
        [DllImport("User32.dll", EntryPoint = "FindWindow")]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);//通过title获得窗口
        [DllImport("User32.dll", EntryPoint = "FindWindowEx")]
        public static extern IntPtr FindWindowEx(IntPtr hwndParent, IntPtr hwndChildAfter, string lpClassName, string lpWindowName);//通过父窗口获得子窗口
        [DllImport("User32.dll", EntryPoint = "FindEx")]
        public static extern IntPtr FindEx(IntPtr hwnd, IntPtr hwndChild, string lpClassName, string lpWindowName);
        [DllImport("user32.dll")]
        public static extern bool GetWindowRect(IntPtr hwnd, ref Rectg rectangle);


        [DllImport("user32.dll", EntryPoint = "SendMessageA")]
        private static extern int SendMessageA(IntPtr hwnd, int wMsg, int wParam, StringBuilder lParam);
        [DllImport("user32.dll")]
        private static extern int SendMessage(IntPtr hWnd, int wMsg, int wParam, string s);
        #endregion
        public const int WM_GETTEXT = 0x000D;
        public const int WM_SETTEXT = 0x000c;

        delegate void SetLoctionCallback();
        delegate void SetListCallback(ArrayList list);

        private string searchText;
        private Point lastLoc;

        public Helper()
        {
            System.Timers.Timer t = new System.Timers.Timer(1000);//实例化Timer类，设置间隔?毫秒；
            t.Elapsed += new System.Timers.ElapsedEventHandler(
                delegate(object source, System.Timers.ElapsedEventArgs e)
                {
                    //跟随输入框位置
                    Form1 f1 = Control.FromHandle(Form1.HWnd) as Form1;
                    if (f1 != null && !f1.IsDisposed)
                    {
                        SetLoctionCallback d = new SetLoctionCallback(this.SetLoction);
                        f1.Invoke(d, new object[] { });
                    }

                    //取得输入框值
                    string text = this.GetText();
                    if (text != null && text.Length > 0 && text != this.searchText)
                    {
                        ArrayList list = Http.Get(text);
                        SetListCallback d = new SetListCallback(f1.SetListView);
                        f1.Invoke(d, new object[] { list });

                        this.searchText = text;
                    }


                });//到达时间的时候执行事件；
            t.AutoReset = true;//设置是执行一次（false）还是一直执行(true)；
            t.Enabled = true;//是否执行System.Timers.Timer.Elapsed事件；
        }

        private IntPtr FindWnd(string wndTitle)
        {
            IntPtr maindHwnd = IntPtr.Zero;

            EnumWindows(delegate(IntPtr hWnd, int lParam)
            {
                StringBuilder sb = new StringBuilder(256);
                GetWindowTextW(hWnd, sb, sb.Capacity);
                string title = sb.ToString();


                if (title != null && title.StartsWith("AllFusion ERwin Data Modeler"))
                {
                    //int processId;
                    ////获取进程ID   
                    //GetWindowThreadProcessId(hWnd, out processId);
                    //Console.WriteLine("{0} {1}", title, processId); 
                    maindHwnd = FindWindow(null, wndTitle); //获得句柄
                }

                return true;
            }, 0);

            return maindHwnd;
        }

        private IntPtr FindNewAttrWnd()
        {
            IntPtr hWnd = this.FindWnd("New Attribute");
            IntPtr inputHWnd = IntPtr.Zero;
            if (hWnd != IntPtr.Zero)
            {
                for (int i = 0; i < 3; i++)
                {
                    //Console.WriteLine(inputHWnd);
                    inputHWnd = FindWindowEx(hWnd, inputHWnd, null, null);
                }

            }
            return inputHWnd;
        }

        public void SetLoction()
        {
            IntPtr inputHWnd = this.FindNewAttrWnd();
            if (inputHWnd != IntPtr.Zero)
            {
                Rectg rect = new Rectg();
                GetWindowRect(inputHWnd, ref rect);

                Form1 f1 = Control.FromHandle(Form1.HWnd) as Form1;
                if (f1 != null)
                {
                    Point loc = new Point(rect.Left, rect.Height);
                    if (lastLoc != null && loc.Equals(lastLoc))
                    {
                        return;
                    }
                    f1.Location = loc;
                    f1.TopMost = true;
                    this.lastLoc = loc;
                }
            }
        }

        public void SetText(string text)
        {
            IntPtr inputHWnd = this.FindNewAttrWnd();
            if (inputHWnd != IntPtr.Zero)
            {
                SendMessage(inputHWnd, WM_SETTEXT, 0, text);

                this.searchText = text;
            }
        }
        public string GetText()
        {
            IntPtr inputHWnd = this.FindNewAttrWnd();
            if (inputHWnd != IntPtr.Zero)
            {
                const int buffer_size = 1024;
                StringBuilder buffer = new StringBuilder(buffer_size);
                SendMessageA(inputHWnd, WM_GETTEXT, buffer_size, buffer);
                //Console.WriteLine(string.Format("取到的值是：{0}", buffer.ToString()));//取值一直是空字符串
                return buffer.ToString();
            }
            return null;
        } 
    }
}
