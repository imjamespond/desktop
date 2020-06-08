﻿using Microsoft.Win32;
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    [Guid("D3CE54A2-9C8D-4EA0-AB31-2A97970F469A")]
    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
    [ComVisible(true)]
    public interface ITest
    {
        [DispId(1)]
        void Run();

    }

    [Guid("A7A5C4C9-F4DA-4CD3-8D01-F7F42512ED04")]
    [ClassInterface(ClassInterfaceType.None)]
    [ComVisible(true)]
    [ProgId("Erwin.ManagedLib.Test1")]
    public class DllProgram : ITest
    {
        [ComRegisterFunction]
        public static void RegisterFunction(Type t)
        {
            Registry.ClassesRoot.CreateSubKey("CLSID\\{" + t.GUID.ToString().ToUpper() + "}\\Programmable");
            RegistryKey rk = Registry.ClassesRoot.OpenSubKey(("CLSID\\{" + t.GUID.ToString().ToUpper() + "}"), true);
            rk.SetValue(string.Empty, "erwin9 Test Addin");
            rk.Close();

        }
        [ComUnregisterFunction]
        public static void UnRegisterFunction(Type t)
        {
            Registry.ClassesRoot.DeleteSubKey("CLSID\\{" + t.GUID.ToString().ToUpper() + "}\\Programmable", false);

        }

        SCAPI.Application scApp;
        SCAPI.PersistenceUnit scPUnit;
        SCAPI.Session scSession;

        public void Run()
        {

            this.scApp = (SCAPI.Application)Activator.CreateInstance(Type.GetTypeFromProgID("erwin9.SCAPI"));

            PopulateModels();

            Close();

            Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }

        void PopulateModels()
        { 
            try
            { 
                /* 
                SCAPI.PersistenceUnit pu = app.PersistenceUnits.Add(@"D:\Program Files\erwin\Data Modeler r9\BackupFiles\Samples\Standard\eMovies.erwin", "RDO=Yes");

                SCAPI.PropertyBag pb = new SCAPI.PropertyBag();
                pb.Add("Hidden_Model", false);
                pu.PropertyBag = pb;*/

                //' Populate the menu with models
                foreach(SCAPI.PersistenceUnit pUint in this.scApp.PersistenceUnits)
                {
                    this.scPUnit = pUint;
                    MessageBox.Show(pUint.Name);
                }

            } catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        void Close()
        {
            /*SCAPI.PropertyBag propBag = this.scPUnit.PropertyBag;
            String sFileName = propBag.Value["Locator"];
            sFileName = sFileName + ".bak";
            this.scPUnit.Save(sFileName);*/


            /*SCAPI.PersistenceUnits scPersistenceUnitCol = this.scApp.PersistenceUnits;
            SCAPI.PropertyBag propBag = new SCAPI.PropertyBag();
            propBag.Add("Name", "Test Model");
            propBag.Add("ModelType", 0);
            SCAPI.PersistenceUnit newModel = scPersistenceUnitCol.Create(propBag);*/


            SCAPI.Sessions scSessionCol = scApp.Sessions;
            scSession = scSessionCol.Add();
            scSession.Open(scPUnit, SCAPI._SC_SessionLevel.SCD_SL_M0);  // open the persistence unit
            if (scSession.IsOpen())
            {
                SCAPI.ModelObjects scModelObjects = scSession.ModelObjects;

                Object transactionID = scSession.BeginNamedTransaction("Test");
                //SCAPI.ModelObjects scChildObjCol = scModelObjects.Collect(scModelObjects.Root, "Entity");   // child objects collection
                SCAPI.ModelObject oEntity = scModelObjects.Add("Entity");
                SCAPI.ModelObject oAttribute = (SCAPI.ModelObject)scModelObjects.Collect(oEntity).Add("Attribute");
                SCAPI.ModelProperty oProperty = oAttribute.Properties.Add("Name");
                oProperty.Value = "Attr A";
                scSession.CommitTransaction(transactionID);
            }

            //close the current model
            /*if (null != this.scPUnit)
            {
                scSession.Close();

                this.scApp.Sessions.Clear();
                //this.scApp.PersistenceUnits.Remove(this.scPUnit, false);
                this.scApp.PersistenceUnits.Clear();
                this.scPUnit = null;
            }*/
            
        }
    }
}