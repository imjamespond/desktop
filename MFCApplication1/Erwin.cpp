#include "pch.h"
#include "Erwin.h"

// 将...erwin\Data Modeler r9目录加入库目录
#import "EAL.dll"

using namespace SCAPI;

ISCPersistenceUnitPtr OpenModel(ISCApplicationPtr& scAppPtr, CString& csFullPath);
//ISCSessionPtr OpenSession(ISCApplicationPtr& scAppPtr);
//void IteratePersistenceUnits(ISCApplicationPtr& scAppPtr);
ISCPersistenceUnitPtr CreateNewModel(ISCApplicationPtr&);

Erwin::Erwin()
{
	//HRESULT hr;
	//CLSID clsid;
	// https://docs.microsoft.com/en-us/windows/win32/com/-progid--key
	// HKEY_LOCAL_MACHINE\SOFTWARE\Classes\{ProgID}
	//hr = CLSIDFromProgID(OLESTR("erwin9.SCAPI"), &clsid);

	CoInitialize(NULL);// 初始化COM空间？
	ISCApplicationPtr scAppPtr;
	HRESULT hr;
	hr = scAppPtr.CreateInstance(__uuidof(SCAPI::Application));
	// 此处必须选x64,不然无法初始COM, 报错REGDB_E_CLASSNOTREG Class not registered
	CoUninitialize();

	if (!scAppPtr)
	{
		TRACE("scAppPtr is null");
		return;
	}


	OpenModel(scAppPtr, CString(R"(D:\Program Files\erwin\Data Modeler r9\BackupFiles\Samples\Standard\eMovies.erwin)") );

}

ISCPersistenceUnitPtr OpenModel(ISCApplicationPtr& scAppPtr, CString& csFullPath)
{
	ISCPersistenceUnitCollectionPtr scPUnitColPtr;
	scPUnitColPtr = scAppPtr->GetPersistenceUnits();
	ISCPersistenceUnitPtr scPUnitPtr = scPUnitColPtr -> Add(COleVariant(csFullPath), "RDO=yes");
	

	ISCSessionCollectionPtr scSessionColPtr = scAppPtr->GetSessions();
	ISCSessionPtr scSessionPtr = scSessionColPtr->Add();  // add a new session
	CComVariant varResult = scSessionPtr->Open(scPUnitPtr, (long)SCD_SL_M0); // open unit
	if (varResult.vt == VT_BOOL && varResult.boolVal == FALSE)
		return NULL;

	for (long i(0); i < scPUnitColPtr->GetCount(); ++i)
	{
		ISCPersistenceUnitPtr itemPtr = scPUnitColPtr->GetItem(i);
		TRACE(itemPtr->GetName().GetBSTR());

		/*ISCModelObjectCollectionPtr objsPtr = scSessionPtr->GetModelObjects()->Collect( itemPtr->);
		for (long j(0); j < objsPtr->GetCount(); ++j)
		{
			ISCModelObjectPtr objPtr = objsPtr->GetItem(j);
			TRACE(objPtr->GetName().GetBSTR());
		}*/
	}

	//Make sure the Session Ptr is Open
	if (!scSessionPtr->IsOpen())
	{
		TRACE("Session Not Opened");
		return NULL;
	}
	ISCModelObjectCollectionPtr scModelObjColPtr = scSessionPtr->GetModelObjects();
	IUnknownPtr _NewEnum = NULL;
	IEnumVARIANT* ObjCollection;

	_NewEnum = scModelObjColPtr->Get_NewEnum();
	if (_NewEnum != NULL)
	{
		HRESULT hr = _NewEnum->QueryInterface(IID_IEnumVARIANT, (LPVOID*)&ObjCollection);
		if (!FAILED(hr))
		{ 
			COleVariant xObject;
			while (S_OK == ObjCollection->Next(1, &xObject, NULL))
			{
				ISCModelObjectPtr pxItem = (V_DISPATCH(&xObject));
				// ISCModelObject in xObject was AddRefed already. All we need is to
				//attach it to a smart pointer
				xObject.Clear();
				// Process the Item
				CString csName = pxItem->GetName().GetBSTR();
				CString csID = pxItem->GetObjectId();
				CString csType = pxItem->GetClassName();
				TRACE("csName: %S, csID: %S, csType: %S\n",csName, csID, csType);
				// …
			}
			if (ObjCollection)
				ObjCollection->Release();
		}
	}

	return scPUnitPtr;
}

