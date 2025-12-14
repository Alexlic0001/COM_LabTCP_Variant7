// server.cpp
#include <windows.h>
#include <initguid.h>
#include "ifn_07.h"
#include "cfn_07.h"
#include "registry.h"  // Добавьте этот include

// Определение глобальных переменных
long g_lObjs = 0;
long g_lLocks = 0;
HMODULE g_hModule = NULL;  // Handle DLL модуля


// Имена компонента
const wchar_t g_szFriendlyName[] = L"IFN07 COM Component (Вариант 7)";
const wchar_t g_szVerIndProgID[] = L"IFN07.Component";
const wchar_t g_szProgID[] = L"IFN07.Component.1";

// Точка входа DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;  // Сохраняем handle модуля
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// Основная точка входа COM: получение фабрики классов
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
    if (!ppv) return E_INVALIDARG;
    *ppv = NULL;

    if (!IsEqualCLSID(rclsid, CLSID_IFN07Component)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    CIFN07ClassFactory* pCF = new CIFN07ClassFactory();
    if (!pCF) return E_OUTOFMEMORY;

    HRESULT hr = pCF->QueryInterface(riid, ppv);
    if (FAILED(hr)) {
        delete pCF;
    }

    return hr;
}

STDAPI DllCanUnloadNow() {
    return (g_lObjs == 0 && g_lLocks == 0) ? S_OK : S_FALSE;
}

// Функции авторегистрации
STDAPI DllRegisterServer() {
    return RegisterServer(g_hModule,
        CLSID_IFN07Component,
        g_szFriendlyName,
        g_szVerIndProgID,
        g_szProgID);
}

STDAPI DllUnregisterServer() {
    return UnregisterServer(CLSID_IFN07Component,
        g_szVerIndProgID,
        g_szProgID);
}