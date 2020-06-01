#pragma once
class WndHelper
{
public:
	static void Test(bool debug = false);

};

namespace km
{
    void SetHook(HWND);

    template<class T>
    T* GetSingleton()
    {
        volatile static T* pSingleton = 0;

        if (pSingleton == NULL)
        {
            ::EnterCriticalSection(&cs);

            if (pSingleton == NULL)
            {
                try
                {
                    pSingleton = new T();
                }
                catch (...)
                {
                    // Something went wrong.
                }
            }

            ::LeaveCriticalSection(&cs);
        }

        return const_cast<T*>(pSingleton);
    }
}
