using Microsoft.Win32;
using System;
using System.Runtime.InteropServices;
using System.Windows;

namespace ClassLibrary1
{

    [Guid("D3CE54A2-9C8D-4EA0-AB31-2A97970F469A")]
    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
    [ComVisible(true)]
    public interface ITest
    {
        [DispId(1)]
        void Run();

        [DispId(4)]
        void TestSignedInteger(sbyte b, short s, int i, long l);
    }

    [Guid("A7A5C4C9-F4DA-4CD3-8D01-F7F42512ED04")]
    [ClassInterface(ClassInterfaceType.None)]
    [ComVisible(true)]
    [ProgId("ABC.ManagedLib.Test")]
    public class Class1 : ITest
    {
        [ComRegisterFunction]
        public static void RegisterFunction(Type t)
        {
            Registry.ClassesRoot.CreateSubKey("CLSID\\{" + t.GUID.ToString().ToUpper() + "}\\Programmable");
            RegistryKey rk = Registry.ClassesRoot.OpenSubKey( ("CLSID\\{"+ t.GUID.ToString().ToUpper() + "}"), true);
            rk.SetValue(string.Empty, "erwin9 Test Addin");
            rk.Close();

        }
        [ComUnregisterFunction]
        public static void UnRegisterFunction(Type t)
        {
            Registry.ClassesRoot.DeleteSubKey("CLSID\\{" + t.GUID.ToString().ToUpper() + "}\\Programmable", false);

        }

        public void Run()
        {
            MessageBox.Show("123"); 
        }

        public void TestSignedInteger(sbyte b, short s, int i, long l)
        {
            throw new NotImplementedException();
        }
    }
}
