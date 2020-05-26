#include "pch.h"
#include "Erwin.h"

// 将...erwin\Data Modeler r9目录加入库目录
#import "EAL.dll"

using namespace SCAPI;

Erwin::Erwin()
{
	//HRESULT hr;
	//CLSID clsid;
	//hr = CLSIDFromProgID(OLESTR("erwin9.SCAPI"), &clsid);

	ISCApplicationPtr scAppPtr;
	HRESULT hr;
	hr = scAppPtr.CreateInstance(__uuidof(SCAPI::Application));

	ISCPropertyBagPtr scBag;

	// Retrieve all of application environment properties in one call
	scBag = scAppPtr->GetApplicationEnvironment()->GetPropertyBag();
	// Get an array with categories by using empty string as a category name
	scBag = scAppPtr->GetApplicationEnvironment()->GetPropertyBag("", "Categories");

	// Get Api Version value Application Api category
	scBag = scAppPtr->GetApplicationEnvironment()->GetPropertyBag("Application.Api", "Api Version");
}